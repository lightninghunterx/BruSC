/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2008-2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2009 Bruker BioSpin MRI GmbH";

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

/****************************************************************/
/****************************************************************/
/*		I N T E R F A C E   S E C T I O N		*/
/****************************************************************/
/****************************************************************/

/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "method.h"


/****************************************************************/
/*	I M P L E M E N T A T I O N   S E C T I O N		*/
/****************************************************************/


/****************************************************************/
/*		G L O B A L   F U N C T I O N S			*/
/****************************************************************/


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


  DB_MSG(("-->backbone"));

  /* Nuclei and  PVM_GradCalConst  are handled by this funtion: */
  STB_UpdateNuclei(Yes);

   /* encoding */

  /* do not allow a-aliasing */
  PVM_AntiAlias[1] = 1.0;

  /*T1_Epi uses complex valued reconstruction which is
    not possible with double sampling.
    -> deactivate and hide for now.
    If double sampling is required later, magnitude reconstruction
    must be selected in case of double sampling and ISA fit routine
    must be adapted.*/
  PVM_EpiCombine=No;
  ParxRelsHideInEditor("PVM_EpiCombine");
  

  /* note: Grappa reference lines are disabled. Grappa coeeficients will be set
   * in a special adjustment. */
  STB_UpdateEncodingForEpi(
    &NSegments,        /* segment size */
    Yes,               /* ppi in 2nd dim allowed */
    No,                /* ppi ref lines in 2nd dim allowed */
    Yes);              /* partial ft in 2nd dim allowed */

  dim = PTB_GetSpatDim();

  /* handle RF pulse */

  STB_UpdateRFPulse("ExcPul",1,PVM_DeriveGains,Conventional);
  STB_UpdateRFPulse("RefPul",1,PVM_DeriveGains,Conventional);
  STB_UpdateRFPulse("InvPul",1,PVM_DeriveGains,AfpUp);

  if(PVM_DeriveGains==Yes)
  {
    ParxRelsHideInEditor("ExcPulseAmpl");
    ParxRelsHideInEditor("InvPulseAmpl");
  }
  else
  {
    ParxRelsShowInEditor("ExcPulseAmpl");
    ParxRelsShowInEditor("InvPulseAmpl");
  }


  /*** Update Geometry **/

  LocalGeometryMinimaRels(limExSliceGradient, limExSliceRephaseGradient, &minThickness);

  /* method constraints regarding number of slices */

  PVM_NSPacks=1;
  ParxRelsMakeNonEditable("PVM_NSPacks");
  /* more than 1 slice package currently results in errors */

  InpSlicesImport();
  InpSlicesRange();
  SliceGroupSizeRange();
  if(SliceGroupSize>InpSlices) SliceGroupSize=InpSlices;
  InpSlicesRel(); /* InpSlices -> PVM_SPackArrNSlices[i]  */

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

  /* number of slices may have been changed by the updater */
  /* method constraints regarding number of slices */
  InpSlicesImport();
  InpSlicesRange();
  SliceGroupSizeRange();
  if(SliceGroupSize>InpSlices) SliceGroupSize=InpSlices;
  InpSlicesRel();

  NIRPointsRange();
  NIRPointsRel();

  /* Spin-echo mode only allowed if the inversion pulse is  */
  /* followed by just one read out, resp. excitation pulse  */
  /* Adjustments (particularly Ghost and Traj) are allowed to set
     Spin-echo mode (inversion pulse is / must be set to 0 in such cases */
  if(SliceGroupSize==NIRPoints || PVM_EpiAdjustMode >0)
  {
    ParxRelsMakeEditable("PVM_SignalType");
  }
  else
  {
    ParxRelsMakeNonEditable("PVM_SignalType");
    PVM_SignalType = SignalType_Fid;
  }

  /* Ref pulse visibility */
  if (PVM_SignalType == SignalType_Fid)
  {
    ParxRelsHideInEditor("RefPul, RefPulseEnum, RefPulseAmpl");
  }
  else
  {
    ParxRelsShowInEditor("RefPul, RefPulseEnum");
    if (PVM_DeriveGains == Yes)
    {
      ParxRelsHideInEditor("RefPulseAmpl");
    }
    else
    {
      ParxRelsShowInEditor("RefPulseAmpl");
    }
  }

  /** Update EPI parameter group */

  PVM_NRepetitions = MAX_OF(1,PVM_NRepetitions);
  PVM_NEchoImages = 1;
  nImagesPerRep = NIRPoints * GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

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

  LocalGradientStrengthRels();
  LocalFrequencyOffsetRels();

  if (ParxRelsParHasValue("PVM_NAverages") == 0)
     PVM_NAverages = 1;

  /* update slice spoiler */
  double spoilerThick = dim>2 ? PVM_SpatResol[2]*PVM_EncZf[2] : PVM_SliceThick;
  MRT_UpdateSpoiler("SliceSpoiler",5.0,limExSliceGradient,2.0,PVM_GradCalConst,spoilerThick);
  MRT_UpdateSpoiler("InvSliceSpoiler",5.0,limExSliceGradient,2.0,PVM_GradCalConst,spoilerThick);

  /* handling of modules */
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, spoilerThick);
  STB_UpdateSatTransModule(PVM_DeriveGains);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  STB_UpdateTriggerModule();
  STB_UpdateTriggerOutModule();
  STB_UpdateTaggingModule(dim,PVM_Fov,PVM_Matrix,PVM_SpatResol[0]*PVM_EncZf[0],PVM_DeriveGains);

  rfcSpoilerUpdate();

  echoTimeRels();

  IROffsetRange();
  IROffsetRel();

  IRSpacingRange();
  IRSpacingRel();

  repetitionTimeRels();

  PTB_ClearAdjustments();

  PTB_AppendOrderAdjustment(per_scan, per_scan, RG_ADJNAME);

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

  SetIRTime();

  int sliceRep=GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices ); //as Rep_ppg counter
  if( SliceGroupRotation == No) sliceRep/=SliceGroupSize;
  STB_UpdateDummyScans(PVM_RepetitionTime*sliceRep);

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  /* set baselevel acquisition parameter */
  SetBaseLevelParam();

  /* set baselevel reconstruction parameter */
  SetRecoParam(PVM_EncPpi[1]);

  STB_EpiCheckTrajectory(PVM_Fov[0],PVM_EffSWh,PVM_SPackArrGradOrient[0][0]);


  DB_MSG(("<--backbone"));
}


void rfcSpoilerUpdate(void)
{
  RfcSpoilerStrength = 2*ExSliceGradient;
  RfcSpoilerStrength = MIN_OF(RfcSpoilerStrength,80);
  RfcSpoilerStrength = MAX_OF(RfcSpoilerStrength,30);

  RfcSpoilerDuration = MAX_OF(RefPul.Length, 2*CFG_GradientRiseTime());
}


/* -----------------------------------------------------------
   Get Nr. of slices
   ----------------------------------------------------------*/

void InpSlicesImport(void)
{
  DB_MSG(("-->InpSlicesImport"));

  InpSlices = PVM_SPackArrNSlices[0];

  DB_MSG(("<--InpSlicesImport"));

}


/* --------------------------------------------------------------
   Further contraints for IRSpacing

   --------------------------------------------------------------*/

void IRSpacingRel(void)
{
  double dummy;
  double trigOutSlice,trigger;

  DB_MSG(("-->IRSpacingRel"));

  trigOutSlice = 0;
  if(PVM_TriggerOutOnOff == On)
  {
    if(PVM_TriggerOutMode == PER_SLICE)
      trigOutSlice = PVM_TriggerOutModuleTime;
  }

  trigger = STB_UpdateTriggerModule();
  if(PVM_TriggerMode == per_PhaseStep) trigger=0.0; /* per volume */

  /* Time from "slice" label to center of excitation pulse */
  dummy = 0.030 + 0.010 + trigger + trigOutSlice
         + PVM_FovSatModuleTime 
         + PVM_SatTransModuleTime
         + PVM_FatSupModuleTime
         + SliceSpoiler.dur          // d5
         + CFG_GradientRiseTime()    // d4
         + ExcPul.Length/2.0;

  /* Time from center of excitation pulse to "lo to slice"  */
  dummy+=  (InpEchoTime + (PVM_EpiModuleTime-PVM_EpiEchoDelay));

  if(SliceGroupRotation==No) MinIRSpacing=SliceGroupSize*dummy;
  else MinIRSpacing=dummy;

  IRSpacing=MAX_OF(MinIRSpacing,IRSpacing);

  DB_MSG(("<--IRSpacingRel"));

}


void IROffsetRel(void)
{
  double trigOutSlice,trigger;

  DB_MSG(("-->IROffsetRel"));

  if(PVM_TriggerOutOnOff == On)
  {
    if(PVM_TriggerOutMode == PER_SLICE)
      trigOutSlice = PVM_TriggerOutModuleTime;
    else
      trigOutSlice = 0;
  }
  else trigOutSlice = 0;
  trigger = STB_UpdateTriggerModule();
  if(PVM_TriggerMode == per_PhaseStep) trigger = 0.0; /* per volume */

  MinIROffset0= InvPul.Length/2.0+InvSliceSpoiler.dur+CFG_GradientRiseTime()+2.0;  /* min d10 */

  MinIROffset1= 0.030 + 0.010 + trigger + trigOutSlice
                + PVM_FovSatModuleTime
                + PVM_SatTransModuleTime
                + PVM_FatSupModuleTime
                + SliceSpoiler.dur            // d5
                + CFG_GradientRiseTime()      // d4
                + ExcPul.Length/2.0;

  MinIROffset=MinIROffset0+MinIROffset1;
  /* IR offset: From center of inversion pulse to center of 1. excitation pulse */

  IROffset   = MAX_OF(IROffset,MinIROffset);
  IROffset0  = MinIROffset0+(IROffset-MinIROffset);

  DB_MSG(("<--IROffsetRel"));

}


/****************************************************************/
/*	         L O C A L   F U N C T I O N S			*/
/****************************************************************/


void echoTimeRels( void )
{
  DB_MSG(("--> echoTimeRels()"));

  double riseT = CFG_GradientRiseTime();

  if(PVM_SignalType == SignalType_Fid)
  {
    PVM_MinEchoTime =      /* min gradient echo time */
      ExcPul.Length * ExcPul.Rpfac/100 +  /* p0/2 */
      riseT                            +  /* d4 */
      RephaseTime                      +  /* d1+d4 */
      PVM_EpiEchoDelay;
    MinTE_right  = MinTE_left = 0.0;      /* not used */
  }
  else
  {
    MinTE_left =  /* min half spinecho-time given by left hand side of pi */
      ExcPul.Length * ExcPul.Rpfac/100  +
      riseT                             +   /* D4 */ 
      RephaseTime                       +   /* D1 + D4 */
      RfcSpoilerDuration                +   /* D9 + D4 */
      RefPul.Length/2.0;

    MinTE_right = /* min half spinecho-time given by right hand side of pi */
      RefPul.Length/2.0  +
      RfcSpoilerDuration +          /* D9 + D4 */
      PVM_EpiEchoDelay;

    PVM_MinEchoTime = 2 * MAX_OF(MinTE_left, MinTE_right);
  }

  InpEchoTime = InpEchoTime < PVM_MinEchoTime ?
    PVM_MinEchoTime : InpEchoTime;


  /* Set Echo Parameters for Scan Editor   */
  PVM_EchoTime = 1e3*PVM_Matrix[0]/PVM_EffSWh/PVM_AntiAlias[0];  /* echo spacing */
  PVM_EchoPosition = 50.0;

  DB_MSG(("<-- echoTimeRels"));
}

void repetitionTimeRels( void )
{
  int nSlices,dim;
  double TotalTime/*, trigOutSlice*/, trigOutVol;
  double da_prime,db_prime,corr;

  DB_MSG(("--> repetitionTimeRels"));

  STB_UpdateTriggerModule();
  TotalTime = 0.0;
  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  if(PVM_TriggerOutOnOff == On)
  {
    switch(PVM_TriggerOutMode)
    {
    case PER_SLICE:
        /* trigOutSlice = PVM_TriggerOutModuleTime; */
      trigOutVol = 0.0;
      break;

    case PER_VOLUME:
        /* trigOutSlice = 0.0; */
      trigOutVol = PVM_TriggerOutModuleTime;
      break;

    case AT_START:
    default:
        /* trigOutSlice = 0.0; */
      trigOutVol = 0.0;

    }
  }
  else
      /* trigOutSlice = */ trigOutVol = 0.0;

  dim = PTB_GetSpatDim();

  if(dim>2) /* disable inter-volume delay in 3d */
  {
    PackDel=0;
    ParxRelsMakeNonEditable("PackDel");
  }
  else
  {
    ParxRelsMakeEditable("PackDel");
  }

  if(SliceGroupRotation == No)
    db_prime  = ((IRSpacing-MinIRSpacing)/SliceGroupSize);  /* fill delay for IRspacing */
  else
    db_prime  = (IRSpacing-MinIRSpacing);

  da_prime = (IROffset0-MinIROffset0);

  /* Optimization to guarantee shortest possible minTR;
     alternative: leaving out the delay after the last read-out */

  if(da_prime>db_prime) corr=db_prime;
  else                  corr=da_prime;

  PVM_MinRepetitionTime =
    PVM_TaggingModuleTime          +
    trigOutVol                     +
    0.01                           +   
    InvPul.Length/2.0              +
    ((IROffset-corr)-MinIROffset1) +
    NIRPoints * IRSpacing;

  PVM_RepetitionTime = MAX_OF(PVM_RepetitionTime, PVM_MinRepetitionTime);

  /** Calculate Total Scan Time and Set for Scan Editor **/

  /* nSlices / SliceGroupSize : Number of Inversions   */
  if( dim >1 )
    TotalTime = PVM_RepetitionTime      *
                PVM_EpiNShots           *
                PVM_NAverages           *
                PVM_SatTransRepetitions *
                PVM_NRepetitions;
  if( dim >2 )
    TotalTime *= PVM_Matrix[2];
  TotalTime *= (nSlices / SliceGroupSize);
  if(SliceGroupRotation == Yes)
    TotalTime *= SliceGroupSize;

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);


  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<-- repetitionTimeRels"));
}


void SetIRTime(void)
{
  int i,j,sindex;

  DB_MSG(("-->SetIRTime"));
  double timeperslice;

  PARX_change_dims("IRTime",InpSlices,NIRPoints);

  timeperslice = IRSpacing/SliceGroupSize;

  if(SliceGroupRotation == No) {
    for(i=0;i<InpSlices;i++) {
      for(j=0;j<NIRPoints;j++) {
        sindex=PVM_ObjOrderList[i];
        IRTime[sindex][j]=IROffset+j*IRSpacing+(i % SliceGroupSize)*timeperslice;
      }
    }
  }
  else { /* SliceGroupRotation == Yes */
    for(i=0;i<InpSlices;i++) {
      for(j=0;j<NIRPoints;j++) {
        IRTime[i][j]=IROffset+j*IRSpacing;
      }
    }
  }

  ParxRelsShowInEditor("IRTime");
  ParxRelsMakeNonEditable("IRTime");

  DB_MSG(("<--SetIRTime"));
}

void LocalGeometryMinimaRels(double limExSliceGradient, double limExSliceRephaseGradient, double *min_thickness )
{
  /*
    This function calculates the minima for the minimum  slice thickness.
    It is always assumed that all slices have the same thickness
    (WE DO NOT set min_fov[0 and 1] here, this will be done by the epi module)
 */

  double sliceRampInteg; /* normalised integral falling slice gradient ramp */
  double sliceRephInteg; /* normalised integral slice rephase gradient      */

  /* min slice thickness: */
  /*  Calculate the normalised integral of the descending gradient ramp after
      the RF pulse */
  sliceRampInteg = 0.5 * CFG_GradientRiseTime();
  /* Calculate the normalised integral of the slice selection rephasing
     gradient */
  sliceRephInteg = RephaseTime - CFG_GradientRiseTime();

  /*
	Calculate the ratio of the strength of the slice selection gradient to
	the strength of the slice selection rephase gradient

  */
  SliceGradRatio = MRT_SliceGradRatio( ExcPul.Length,
                                       ExcPul.Rpfac,
                                       0.0,
                                       sliceRampInteg,
                                       sliceRephInteg );
  /*
    Calculate the minimum slice thickness
  */

  *min_thickness = MRT_MinSliceThickness( ExcPul.Bandwidth,
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
      ExSliceGradient = MRT_SliceGrad( ExcPul.Bandwidth,
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

  /* set ReadOffsetHz to zero. In EPI the fov offset is made by reco, not by detection freq. offsets */
  MRT_FrequencyOffsetList(nslices,
			  PVM_EffReadOffset,
			  ReadGradient,
			  0.0, /* instead PVM_GradCalConst; this sets offHz to zero */
			  PVM_ReadOffsetHz );

  MRT_FrequencyOffsetList(nslices,
			  PVM_EffSliceOffset,
			  ExSliceGradient,
			  PVM_GradCalConst,
			  PVM_SliceOffsetHz );

  if(spatDim == 3)
  {
    for(i=0;i<nslices;i++)
      PVM_EffPhase2Offset[i] = -PVM_EffSliceOffset[i];
  }


}

/*
 *  local function to constrain the read offset for slice packages
 */



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
