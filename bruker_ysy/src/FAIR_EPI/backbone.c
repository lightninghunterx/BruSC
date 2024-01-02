/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2002 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

/*:=INFO=:*******************************************************
 *
 * Description :
 *   it contains functions necessary for responding to a 'load'
 *	of this method. This function will be automatically
 *	called in response to a method file for this method
 *	having been read.
 *
 *::=info=:******************************************************/

#include "method.h"

/* ------------------------------------------------------------
  backbone
  The main part of method code. The consitency of all parameters is checked
  chere, relations between them are resolved and, finally, functions setting
  the base level parameters are called.
  --------------------------------------------------------------*/
void backbone( void )
{

  int dim, ret, nImagesPerRep;
  double minFov[3] = {1e-3, 1e-3, 1e-3},
         minThickness;
  double limExSliceGradient = 100;
  double limExSliceRephaseGradient = 50;



  DB_MSG(("---> backbone"));


  /* Nucleus and related parameters (PVM_BasicFreq1, PVM_GradCalConst)
     are handled by this funtion: */
  STB_UpdateNuclei(Yes);

  /* do not allow a-aliasing */
  PVM_AntiAlias[1] = 1.0;

  /* Encoding
   * note: Grappa reference lines are disabled. Grappa coeeficients will be set
   * in a special adjustment. */
  STB_UpdateEncodingForEpi(
    &NSegments,        /* segment size */
    Yes,               /* ppi in 2nd dim allowed */
    No,                /* ppi ref lines in 2nd dim allowed */
    Yes);              /* partial ft in 2nd dim allowed */

  dim = PTB_GetSpatDim();

  /* handle RF pulses */
  UpdateRFPulses();

  /* repetition type */
  if(PVM_FairRepSpacing==CONST_REP)
  {
    ParxRelsShowInEditor("PVM_RepetitionTime");
    ParxRelsHideInEditor("RecoveryTime");
  }
  else
  {
    ParxRelsHideInEditor("PVM_RepetitionTime");
    ParxRelsShowInEditor("RecoveryTime");
  }

  /* signal type */
  if(PVM_SignalType != SignalType_Echo)
  {
    ParxRelsHideInEditor("RefPulse1,RefPulse1Enum,BwScale");
  }
  else
  {
    ParxRelsShowInEditor("RefPulse1,RefPulse1Enum,BwScale");
  }

  /* excitation pulse */
  ExSlicePulseLength   = ExcPulse1.Length;
  ExSliceRephaseFactor = ExcPulse1.Rpfac;
  ExSliceBandWidth = ExcPulse1.Bandwidth * BwScale/100.0;



  /*** begin Update Geometry: **/
  LocalGeometryMinimaRels(limExSliceGradient, limExSliceRephaseGradient, &minThickness);

  /* do not allow isotropic geometry */
  PVM_IsotropicFovRes = Isot_None;

  // only one package with one slice if 3D
  int maxPackages = dim>2? 1:0;
  int maxPerPackage = dim>2? 1:0;

  STB_UpdateImageGeometry(dim,
                          PVM_Matrix,
                          minFov,
                          0, // total slices (no restr)
                          maxPackages,
                          maxPerPackage,
                          minThickness,
                          1.0); // sliceFovRatio in 3D

  /** Update EPI parameter group */

  PVM_NRepetitions = MAX_OF(1,PVM_NRepetitions);
  PVM_NEchoImages = 1;
  nImagesPerRep = PVM_NEchoImages * GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  ret = STB_EpiUpdate(No,
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

  /* Update geometry again (minFov is known) */

  STB_EpiConstrainGeometry((PVM_Fov[0] / 2)*(PVM_AntiAlias[0] - 1.0)); /* special for EPI */

  STB_UpdateImageGeometry(dim,
                          PVM_Matrix,
                          minFov,
                          0, // total slices (no restr)
                          maxPackages,
                          maxPerPackage,
                          minThickness,
                          1.0); // sliceFovRatio in 3D

  /*** end Update Geometry **/



  /*** end Update Geometry **/

  LocalGradientStrengthRels();
  LocalFrequencyOffsetRels();

  PVM_NRepetitions = MAX_OF(1,PVM_NRepetitions);

  if (ParxRelsParHasValue("PVM_NAverages") == 0)
     PVM_NAverages = 1;

  /* update slice spoiler */
  double riseT = CFG_GradientRiseTime();
  double spoilerThick = dim>2 ? PVM_SpatResol[2]*PVM_EncZf[2] : PVM_SliceThick;
  MRT_UpdateSpoiler("SliceSpoiler",1.5,limExSliceGradient,1.5*riseT,PVM_GradCalConst,spoilerThick);

  /* handling of modules */
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, spoilerThick);
  STB_UpdateSatTransModule(PVM_DeriveGains);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  STB_UpdateTriggerModule();

  /* Update Fair module */
  /* time between end of fair module to center of exc pulse */
  double fixedTime =  0.03                   + /*UpdateDynPars*/
                      0.01                   +
                      PVM_FovSatModuleTime   +
                      PVM_SatTransModuleTime +
                      PVM_FatSupModuleTime   +
                      SliceSpoiler.dur       +  /*d5*/
                      CFG_GradientRiseTime() +  /*d3*/
                      ExcPulse1.Length/2.0;

  double slicePackThick = PVM_SPackArrSliceDistance[0]*(PVM_SPackArrNSlices[0] -1)+PVM_SliceThick;
  double slicePackOffset = PVM_SPackArrSliceOffset[0];

  STB_UpdateFairModule(PVM_DeriveGains, fixedTime, slicePackThick, slicePackOffset, PVM_SliceThick);

  echoTimeRels();

  if(PVM_FairRepSpacing==CONST_REP)
    UpdateRepetitionTime(fixedTime);
  else
    UpdateRecoveryTime(fixedTime);

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

  STB_UpdateDummyScans(PVM_RepetitionTime,PVM_NAverages);

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  /* set baselevel acquisition parameter */
  SetBaseLevelParam();

  /* set baselevel reconstruction parameter */
  SetRecoParam(PVM_EncPpi[1]);


  /* adapt size of trajectory arrays if necessary and set
     PVM_EpiTrajAdj to No if a trajectory relevant parameter has changed */

  STB_EpiCheckTrajectory(PVM_Fov[0],PVM_EffSWh,PVM_SPackArrGradOrient[0][0]);

  DB_MSG(("<--- backbone"));
}




/****************************************************************/
/*	         L O C A L   F U N C T I O N S			*/
/****************************************************************/

void echoTimeRels( void )
{

  DB_MSG(("---> echoTimeRels()"));

  /* min TE  */
  double riseT = CFG_GradientRiseTime();

  if(PVM_SignalType == SignalType_Fid)
  {
    PVM_MinEchoTime =      /* min gradient echo time */
      ExSlicePulseLength / 2.0     +
      riseT                        + /*d3*/
      riseT                        + /*d3*/
      ExSliceRephaseTime - 2*riseT + /*d1*/
      riseT                        + /*d3*/
      PVM_EpiEchoDelay;
    MinTE_right  = MinTE_left = 0.0; /* not used */
  }
  else
  {
    MinTE_left = /* min half spinecho-time given by left hand side of pi */
      ExSlicePulseLength / 2.0          +
      riseT                             + /*d3*/
      riseT                             + /*d3*/
      ExSliceRephaseTime - 2*riseT      + /*d1*/
      riseT                             + /*d3*/
      SliceSpoiler.dur                  + /*d5*/
      riseT                             + /*d3*/
      RefPulse1.Length/2.0;

    MinTE_right = /* min half spinecho-time given by right hand side of pi */
      RefPulse1.Length/2.0       +
      SliceSpoiler.dur           +  /*d5*/
      riseT                      +  /*d3*/
      PVM_EpiEchoDelay;

    PVM_MinEchoTime = 2 * MAX_OF(MinTE_left, MinTE_right);

  }

  DB_MSG(("left %f, right %f, minTE %f",MinTE_left,MinTE_right, PVM_MinEchoTime));

  EchoTime = EchoTime < PVM_MinEchoTime ?
    PVM_MinEchoTime : EchoTime;


  /** Set Echo Parameters for Scan Editor  **/
  /* echo spacing */
  PVM_EchoTime = 1e3*PVM_Matrix[0]/PVM_EffSWh/PVM_AntiAlias[0];

  PVM_EchoTime1 = EchoTime;

  PVM_EchoPosition = 50.0;

  DB_MSG(("<--- echoTimeRels"));
}

void UpdateRecoveryTime(double fixedTime)
{
  int nSlices,dim,i;
  double TotalTime, packageTime;
  double riseT = CFG_GradientRiseTime();

  DB_MSG(("--> UpdateRecoveryTime"));

  if(PVM_SignalType == SignalType_Fid)
  {
    /*FID: fixed time between end of exc pulse to start of next inv pulse*/
    MinRecoveryTime = riseT                               + /*d3*/
                      riseT                               + /*d3*/
                      (ExSliceRephaseTime - 2.0*riseT)    + /*d1*/
                      riseT                               + /*d3*/
                      (EchoTime - PVM_MinEchoTime)        + /*d2*/
                      PVM_EpiModuleTime                   +
                      riseT;                                /*FairD1*/


  }
  else
  {
    /*SE: fixed time between end of rfc pulse to start of next inv pulse*/
    MinRecoveryTime = SliceSpoiler.dur           + /*d5*/
                      riseT                      + /*d3*/
                      (EchoTime/2 - MinTE_right) + /*d2*/
                      PVM_EpiModuleTime          + /*FairD1*/
                      riseT;                             
  }

  RecoveryTime = MAX_OF(MinRecoveryTime, RecoveryTime);

  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  packageTime =
    nSlices * (0.03                    + /*UpdateDynPars*/
               0.01                    +
               PVM_FovSatModuleTime    +
               PVM_SatTransModuleTime  +
               PVM_FatSupModuleTime    +
               SliceSpoiler.dur        +   /*d5*/
               riseT                   +   /*d4*/
               ExSlicePulseLength/2    +
               EchoTime                +
               PVM_EpiModuleTime - PVM_EpiEchoDelay
              );

  TotalTime=0.0;
  PARX_change_dims("MultiRepTime", PVM_FairTIR_NExp);

  /* Access to FairTIR_Arr */
  for(i=0;i<PVM_FairTIR_NExp;i++)
  {
    MultiRepTime[i]= RecoveryTime - MinRecoveryTime   +  /*d0*/
                     riseT + PVM_FairInvPul.Length/2  +  /*fair module from start to pulse center*/
                     PVM_FairTIR_Arr[i] - fixedTime   +  /*fair module from pulse center to end*/
                     packageTime                      +  /*slice packages*/
                     0.0;                                /*subr FairRec*/
  
    TotalTime+=MultiRepTime[i];
  }

  /** Calculate Total Scan Time and Set for Scan Editor **/

  dim = PTB_GetSpatDim();
  if( dim >1 )
  TotalTime = TotalTime               *
              PVM_EpiNShots           *
              PVM_NAverages           *
              PVM_SatTransRepetitions *
              PVM_NRepetitions;

  if( dim >2 )
  TotalTime = TotalTime *PVM_Matrix[2];

  TotalTime = TotalTime *
    (PVM_FairMode==INTERLEAVED || PVM_FairMode==INTERLEAVED2 ? 2:1);

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);

  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");
  ParxRelsShowInEditor("MultiRepTime");

  DB_MSG(("--> UpdateRecoveryTime"));

}

void UpdateRepetitionTime(double fixedTime)
{
  int nSlices,dim;
  double TotalTime, packageTime;
  int fairexp;
  double riseT = CFG_GradientRiseTime();

  DB_MSG(("--> UpdateRepetitionTime"));

  TotalTime = 0.0;
  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  fairexp =  PVM_FairTIR_NExp *
    (PVM_FairMode==INTERLEAVED || PVM_FairMode==INTERLEAVED2 ? 2:1);

  packageTime =
    nSlices * (0.03                    + /*UpdateDynPars*/
               0.01                    +
               PVM_FovSatModuleTime    +
               PVM_SatTransModuleTime  +
               PVM_FatSupModuleTime    +
               SliceSpoiler.dur        +  /*d5*/
               riseT                   +  /*d3*/
               ExSlicePulseLength/2    +
               EchoTime                +
               PVM_EpiModuleTime - PVM_EpiEchoDelay
              );


  PVM_MinRepetitionTime = packageTime               +
                          riseT                     + /*FairD1*/
                          PVM_FairInvPul.Length/2.0 +
                          PVM_FairMaxTIR - fixedTime;




  PVM_RepetitionTime = MAX_OF(PVM_MinRepetitionTime, PVM_RepetitionTime);
  PARX_change_dims("MultiRepTime", 1);
  MultiRepTime[0]=PVM_RepetitionTime;

  /** Calculate Total Scan Time and Set for Scan Editor **/

  dim = PTB_GetSpatDim();
  if( dim >1 )
  TotalTime = PVM_RepetitionTime      *
              fairexp                 *
              PVM_EpiNShots           *
              PVM_NAverages           *
              PVM_SatTransRepetitions *
              PVM_NRepetitions;

  if( dim >2 )
  TotalTime = TotalTime *PVM_Matrix[2];

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);

  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");
  ParxRelsHideInEditor("MultiRepTime");

  DB_MSG(("<-- UpdateRepetitionTime"));
}

void LocalGeometryMinimaRels(double limExSliceGradient, double limExSliceRephaseGradient, double  *min_thickness )
{
  /*
    This function calculates the minimum
    slice thickness.
 (WE DO NOT set min_fov[0 and 1] here, this will be done by the epi module)

  */

  double sliceRampInteg; /* normalised integral falling slice gradient ramp */
  double sliceRephInteg; /* normalised integral slice rephase gradient      */


  /* min slice thickness: */
  /*  Calculate the normalised integral of the descending gradient ramp after
      the RF pulse */
  sliceRampInteg = 0.5 *  CFG_GradientRiseTime();
  /* Calculate the normalised integral of the slice selection rephasing
     gradient */
  sliceRephInteg = ExSliceRephaseTime - CFG_GradientRiseTime();

  /*
	Calculate the ratio of the strength of the slice selection gradient to
	the strength of the slice selection rephase gradient

	n.b. The variable SliceGradRatio is a parameter defined in the file:
	parsDefinition.h but it is NOT included in the definition of MethodClass
	that appears in parsLayout.h. The value of SliceGradRatio determined here
	is used later in "LocalGradientStrengthRels()"
  */
  SliceGradRatio = MRT_SliceGradRatio( ExSlicePulseLength,
					  ExSliceRephaseFactor,
            0.0, /* PVM_ExSliceRampDownWaitTime, */
					  sliceRampInteg,
					  sliceRephInteg );
  /*
    Calculate the minimum slice thickness
  */

  *min_thickness = MRT_MinSliceThickness( ExSliceBandWidth,
					  SliceGradRatio,
					  limExSliceGradient,
					  limExSliceRephaseGradient,
					  PVM_GradCalConst );


} /* end of localGeometryMinima() */


void LocalGradientStrengthRels( void )
{

  /*
    This function calculates all the gradient strengths
  */
  switch( PTB_GetSpatDim() )
    {
    case 3: /* PHASE ENCODING GRADIENT - 3nd DIM */

     /* falls through */
    case 1:
      ReadGradient = (PVM_EpiReadEvenGrad+PVM_EpiReadOddGrad)/2.0; /* used in LocFreqOff */
     /* falls through */
    default: /* SLICE SELECTION GRADIENT */
      ExSliceGradient = MRT_SliceGrad( ExSliceBandWidth,
					   PVM_SliceThick,
					   PVM_GradCalConst );
      ExSliceRephaseGradient = MRT_SliceRephaseGrad( SliceGradRatio,
							 ExSliceGradient );
      break;
    }

}

void LocalFrequencyOffsetRels( void )
{
  int spatDim;
  int i,nslices;

  spatDim = PTB_GetSpatDim();

  nslices = GTB_NumberOfSlices(PVM_NSPacks,PVM_SPackArrNSlices);

  MRT_FrequencyOffsetList(nslices,
			  PVM_EffReadOffset,
			  ReadGradient,
			  0.0, /* instead of PVM_GradCalConst, to set offsetHz to zero */
			  PVM_ReadOffsetHz );

  MRT_FrequencyOffsetList(nslices,
			  PVM_EffSliceOffset,
			  ExSliceGradient,
			  PVM_GradCalConst,
			  PVM_SliceOffsetHz );

  if(spatDim == 3)
  {
    for(i=0;i<nslices;i++)
      PVM_EffPhase2Offset[i] = -PVM_EffSliceOffset[i]/PVM_AntiAlias[2];
  }


}


void UpdateRFPulses()
{
  DB_MSG(("--> UpdateRFPulses"));

  /* Excitation */
  STB_UpdateRFPulse("ExcPulse1",1,PVM_DeriveGains,Conventional);
  STB_UpdateRFPulse("RefPulse1",1,PVM_DeriveGains,Conventional);

  if(PVM_DeriveGains==Yes)
  {
    ParxRelsHideInEditor("ExcPulse1Ampl,RefPulse1Ampl");
  }
  else
  {
    if(PVM_SignalType == SignalType_Fid)
      ParxRelsShowInEditor("ExcPulse1Ampl");
    else
      ParxRelsShowInEditor("ExcPulse1Ampl,RefPulse1Ampl");
  }

  ParxRelsShowInFile("ExcPulse1Ampl,RefPulse1Ampl");

  DB_MSG(("<--UpdateRFPulses"));

  return;
}


/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
