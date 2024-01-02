/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2003-2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/


static const char resid[] = "$Id$ (C) 2003-2007 Bruker BioSpin MRI GmbH";


#define DEBUG           0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"

void backbone(void)
{
  DB_MSG(("-->backbone\n"));

  STB_UpdateNuclei(No);

  /* encoding */
  
  /* do not allow a-aliasing */
  PVM_AntiAlias[1] = 1.0;

  /* note: Grappa reference lines are disabled. Grappa coeeficients will be set
   * in a special adjustment. */
  STB_UpdateEncodingForEpi(
    &NSegments,        /* segment size */
    Yes,               /* ppi in 2nd dim allowed */
    No,                /* ppi ref lines in 2nd dim allowed */
    Yes);              /* partial ft in 2nd dim allowed */ 

  int dim = PTB_GetSpatDim();
  
  /*
   *  update excitation and refocusing pulse
   */  
  UpdateRFPulses(PVM_DeriveGains);

  /*
   * fix timing according to method specific constraints
   */

  Phase3DGradDur = ExcSliceRephTime;

  /*** Update Geometry **/
  
  /*
   *  calculate limits for read phase and slice
   */

  double minFov[3] = {1e-3, 1e-3, 1e-3};

  if(dim == 3)
  {
    ParxRelsShowGroupInEditor("Phase3DEncoding");
    minFov[2] = Phase3DEncodingLimits(PVM_Matrix[2],
                                      GradStabTime,
                                      PVM_GradCalConst);
  }
  else
  {
    ParxRelsHideGroupInEditor("Phase3DEncoding");
  }

  minFov[0] = PVM_EffSWh/PVM_GradCalConst;
  minFov[1] = minFov[0]/16.0;  /* reasonable limit for epi */
  
  double sliceGradRatio;
  double minSliceThick = SliceSelectionLimits(&ExcPul,
                                       100.0,
                                       GradStabTime,
                                       PVM_GradCalConst,
                                       &sliceGradRatio);
 
  /* do not allow isotropic geometry */
  PVM_IsotropicFovRes = Isot_None;

  // only one package with one slice if 3D
  int maxPerPackage = (dim>2 ) ? 1:0;
  
  STB_UpdateImageGeometry(dim, 
                          PVM_Matrix,
                          minFov,
                          0, // total slices (no restr)
                          1, // restriction to single slice package
                          maxPerPackage,
                          minSliceThick,
                          1.0); // sliceFovRatio in 3D

  /** Update EPI parameter group */

  PVM_NEchoImages = 1;
  int nImagesPerRep = PVM_NEchoImages * GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  int ret = STB_EpiUpdate(No,
		      dim, 
		      PVM_EncMatrix, 
		      PVM_AntiAlias,
		      PVM_Fov, 
		      minFov, 
		      &PVM_EffSWh, 
		      PVM_GradCalConst, 
		      nImagesPerRep,
		      NSegments,
		      PVM_EncCentralStep1,
		      PVM_EncPpi[1],
		      PVM_EncNReceivers);
 
  if(ret <0)
    DB_MSG(("--!!!!!!!!!!!!!!! Illegal arguments for STB_UpdateEPI: EPI Module not ready !"));

  /*
   * Update of Diffusion 
   */

  double minte1 =  CalcEchoDelay();
  double prepEchoTime = EchoTime;
  double sliceThick = MAX_OF(PVM_SliceThick,minSliceThick);

  int status = STB_UpdateDiffusionPreparation(&sliceThick,
                                          PVM_DeriveGains,
                                          PVM_Nucleus1,
                                          PtrType3x3 PVM_SPackArrGradOrient[0],
                                          minte1,
                                          PVM_EpiEchoDelay,
                                          &prepEchoTime);

  if(status == 1)
  {
    minSliceThick = sliceThick;
  }

   /* Update geometry again (minFov is known) */
  
  STB_EpiConstrainGeometry((PVM_Fov[0] / 2)*(PVM_AntiAlias[0] - 1.0)); /* special for EPI */

  STB_UpdateImageGeometry(dim, 
                          PVM_Matrix,
                          minFov,
                          0, // total slices (no restr)
                          1, // max 1 slice package
                          maxPerPackage,
                          minSliceThick,
                          1.0); // sliceFovRatio in 3D
                                 
  /*** end Update Geometry **/

  if(dim == 3)
  {
    UpdatePhase3DGradients(PVM_Matrix[2],PVM_Fov[2],PVM_GradCalConst);
  }

  UpdateSliceSelectionGradients(PVM_SliceThick,
                                sliceGradRatio,
                                ExcPul.Bandwidth,
                                PVM_GradCalConst);

  /* handling of modules */
  double spoilerThick = dim>2 ? PVM_SpatResol[2]*PVM_EncZf[2] : PVM_SliceThick;
  STB_UpdateDriftCompModule(spoilerThick, PVM_DeriveGains);
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, spoilerThick);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  
  /*
   *  calculate frequency offsets
   */

  LocalFrequencyOffsetRels();

  /*
   *  update sequence timing
   */

  UpdateEchoTime(prepEchoTime);
  UpdateRepetitionTime();

  PTB_ClearAdjustments();

  PTB_AppendOrderAdjustment(per_scan,per_scan, RG_ADJNAME);

  PTB_AppendAdjustment("EpiTraj",
                       "Adjust Traj.",
                       "Adjust EPI Trajectory",
                       on_demand);

  if(PVM_EncPpi[1] > 1)
  {
    PTB_AppendAdjustment("EpiGrappa",
                         "Adjust GRAPPA Coeff.",
                         "Adjustment of GRAPPA Coefficients",
                         per_scan);
  }

  STB_UpdateDummyScans(PVM_RepetitionTime);
 
  /* Set correct DriftCompUpdateTime after TR-upate */
  STB_UpdateDriftComp(PVM_RepetitionTime);

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");
 
  /* set baselevel acquisition parameter */
  SetBaseLevelParam();

  /* set baselevel reconstruction parameter */
  SetRecoParam(PVM_EncPpi[1]);

  /* adapt size of trajectory arrays if necessary and set 
     PVM_EpiTrajAdj to No if a trajectory relevant parameter has changed */

  STB_EpiCheckTrajectory(PVM_Fov[0],PVM_EffSWh,PVM_SPackArrGradOrient[0][0]);


  DB_MSG(("<--backbone\n"));
  return;
}

/*-------------------------------------------------------
 * local utility routines to simplify the backbone 
 *------------------------------------------------------*/

double CalcEchoDelay(void)
{
  double riseTime, igwt,retVal;
  riseTime = CFG_GradientRiseTime();
  igwt     = CFG_InterGradientWaitTime();

  retVal = 
    ExcPul.Length/2                     +
    riseTime   + igwt                   + /* min TE1/2 filling delay */
    ExcSliceRephTime                    +
    igwt;                                
 
  return retVal;
}

void UpdateEchoTime(double minechotime)
{

  DB_MSG(("-->UpdateEchoTime\n"));

 
  PVM_MinEchoTime = minechotime;
  EchoTime        = MAX_OF(PVM_MinEchoTime,EchoTime);

  /*
   * Set Echo Parameters for Scan Editor  
   */

  /* echo spacing: */
  PVM_EchoTime = 1e3*PVM_Matrix[0]/PVM_EffSWh/PVM_AntiAlias[0];
  ParxRelsShowInEditor("PVM_EchoTime,PVM_NEchoImages");
  ParxRelsShowInFile("PVM_EchoTime,PVM_NEchoImages");
  ParxRelsMakeNonEditable("PVM_NEchoImages");

  PVM_EchoPosition = 50.0;

  DB_MSG(("<--UpdateEchoTime\n"));
  return;
}

void UpdateRepetitionTime(void)
{
  int nslices,dim;
  double TotalTime,slrept;
  double trigger,riseT,igwT;
  double trigger_s,trigger_v;

  DB_MSG(("-->UpdateRepetitionTime\n"));

  nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  
  trigger = STB_UpdateTriggerModule();
  if(PVM_TriggerMode == per_PhaseStep) /* per volume */
  {
    trigger_v=trigger;
    trigger_s=0.0;
  }
  else
  {
    trigger_v=0.0;
    trigger_s=trigger;
  }

  riseT   = CFG_GradientRiseTime();
  igwT    = CFG_InterGradientWaitTime();

  slrept = 
      0.03                                             +  /* UpdateDynPars */
      0.01                                             +
      trigger_s                                        +
      PVM_FovSatModuleTime                             +
      PVM_FatSupModuleTime                             +
      SliceGradStabTime  + riseT                       +  /* d1 */
      ExcPul.Length                                    +
      riseT + igwT                                     +  /* d4 */
      ExcSliceRephTime - riseT                         +  /*d2*/
      riseT + igwT                                     +  /*d3*/
      PVM_DwModDur                                     +
      PVM_EpiModuleTime                                +
      riseT+igwT;                                         /*min d0 */

  PVM_MinRepetitionTime = slrept * nslices + trigger_v + PackDel + PVM_DriftCompModuleTime;

  PVM_RepetitionTime=MAX_OF(PVM_MinRepetitionTime,PVM_RepetitionTime);

  TotalTime = PVM_RepetitionTime 
            * PVM_EpiNShots
            * PVM_NAverages *PVM_DwNDiffExp *  PVM_NRepetitions;

  dim = PTB_GetSpatDim();

  if(dim == 3)
  {
    TotalTime *= PVM_EncMatrix[2];
  }

  /* time for one repetition */
  OneRepTime =PVM_RepetitionTime * PVM_EpiNShots * PVM_NAverages / 1000.0;

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<--UpdateRepetitionTime\n"));
  return;
}

void LocalFrequencyOffsetRels( void )
{
  int nslices;
  double readGrad;

  nslices = GTB_NumberOfSlices(PVM_NSPacks,PVM_SPackArrNSlices);

  readGrad = (PVM_EpiReadEvenGrad+PVM_EpiReadOddGrad)/2.0;

  MRT_FrequencyOffsetList(nslices,
			  PVM_EffReadOffset,
			  readGrad,
			  0.0, /* instead of PVM_GradCalConst, to set offsetHz to zero */
			  PVM_ReadOffsetHz );

  MRT_FrequencyOffsetList(nslices,
			  PVM_EffSliceOffset,
			  ExcSliceGrad,
			  PVM_GradCalConst,
			  PVM_SliceOffsetHz );

  if(PTB_GetSpatDim() == 3)
  {
    for(int i=0;i<nslices;i++)
      PVM_EffPhase2Offset[i] = -PVM_EffSliceOffset[i];
  }

}

