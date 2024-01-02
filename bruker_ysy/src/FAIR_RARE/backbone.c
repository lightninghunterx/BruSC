/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2007
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id: ";


#define DEBUG      0
#define DB_MODULE  0
#define DB_LINE_NR 0

#include "method.h"


/* ------------------------------------------------------------
 * backbone
 * The main part of method code. The consitency of all parameters
 * is checked here, relations between them are resolved and,
 * finally, functions setting the base level parameters are
 * called.
 */


void backbone( void )
{
  int    ret, dim;
  double minFov[3] = {1e-3, 1e-3, 1e-3},
         minThickness,
         gradientLimit = 57,  /* gradient limit, in % of max, for all channels */
         sliceScale[2],
         grid;

  double tRead[MRT_CPMG_NPOINTS], tPhase[MRT_CPMG_NPOINTS], tSlice[MRT_CPMG_NPOINTS],
         gRead[MRT_CPMG_NPOINTS], gPhase[MRT_CPMG_NPOINTS], gSlice[MRT_CPMG_NPOINTS];


  DB_MSG(("-->backbone\n"));

  STB_UpdateNuclei(No);

  /* time grid for gradienet shapes, in ms */
  grid = MAX_OF(CFG_GradientShapeResolution(),0.002);

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


  dim=PTB_GetSpatDim();

  /* update encoding parameter group                          */

  STB_UpdateEncoding(
    &PVM_RareFactor,
    SEG_INTERLEAVED,
    Yes,
    Yes,
    Yes);

  PVM_EchoPosition = 50.0;


 /* update parameters controlling the data sampling period   */

  MRT_UpdateDigPars(&PVM_EffSWh,
                     PVM_EncMatrix[0],
                    &PVM_AntiAlias[0],
                    &PVM_AcquisitionTime);

  if(dim > 2)
  {
    sliceScale[0] = UT_MinOfDoubleArray(PVM_EncMatrix[2], PVM_EncValues2);
    sliceScale[1] = UT_MaxOfDoubleArray(PVM_EncMatrix[2], PVM_EncValues2);
  }
  else
  {
    sliceScale[0]=sliceScale[1] = 0.0;
  }

  /* when ortogonal slices, allow 100% gradient, otherwise 100/sqrt(3) */
  gradientLimit = PVM_MajSliceOri==Yes? 100.0:57.0;

  LocalGeometryMinimaRels(minFov, &minThickness, gradientLimit);

  /*
   * Constrain minimum fov in 3D phase direction and
   * minimum slice thickness in case of 3D measurements
   */

  if(dim == 3)
  {
    double min;

    min=MAX_OF(minFov[2],minThickness);
    minFov[2] = minThickness = min;

  }

  /* update geometry parameters                               */
  STB_UpdateAtsReferencePosition("PVM_SliceGeoObj");

  // only one package if black-blood module on
  int maxPackages = 1;

  // only one slice per package if 3D
  int maxPerPackage = dim>2? 1:0;

  STB_UpdateImageGeometry(dim,
                          PVM_Matrix,
                          minFov,
                          0, // total slices (no restr)
                          maxPackages,
                          maxPerPackage,
                          minThickness,
                          1.0); // sliceFovRatio in 3D


  updateSpoilers();

  SpoilingRead  = SpoilingReadPx/PVM_SpatResol[0];
  SpoilingSlice = SpoilingSliceSl/PVM_SliceThick ;


  /* calculate gradient strengths */

  ReadGradient = MRT_ReadGrad( PVM_EffSWh, PVM_Fov[0], PVM_GradCalConst );

  solve90_180(&PVM_EchoTime,
              &SpoilingRead, &SpoilingSlice,
              &ExcSliceGradient, &RefSliceGradient,
              &ExcPulse1, &RefPulse1,
              GradStab,
              &ReadDephGradient, &SliceDephGradient,
              PVM_GradCalConst,
              PVM_SliceThick,  PVM_SpatResol[0],
              SliceBandWidthScale);

  ret = MRT_CalculateCpmgPeriod(
    (SlewRatePerCent/100)*PVM_GradCalConst/CFG_GradientRampTime(),
    PVM_GradCalConst*gradientLimit/100,
    PVM_GradCalConst*ReadGradient/100,
    PVM_GradCalConst*RefSliceGradient/100,
    RefPulse1.Length + 2*GradStab, /* add margins if needed */
    PVM_AcquisitionTime + 2*GradStab,   /* add margins if needed */
    grid,
    SpoilingRead,
    SpoilingSlice,
    0.0, /* asym spoiler read */
    0.0, /* asym spoiler slice */
    PVM_SpatResol[1],
    sliceScale,
    tRead,
    tPhase,
    tSlice,
    gRead,
    gPhase,
    gSlice,
    &PVM_EchoTime
    );



  if(ret > 0) /* PVM_EchoTime increased: solve 90-180 again */
  {
    DB_MSG(("re-calculating 90-180"));
    DB_MSG(("TE=%f",PVM_EchoTime));
    solve90_180(&PVM_EchoTime,
    &SpoilingRead, &SpoilingSlice,
    &ExcSliceGradient, &RefSliceGradient,
    &ExcPulse1, &RefPulse1,
    GradStab,
    &ReadDephGradient, &SliceDephGradient,
    PVM_GradCalConst,
    PVM_SliceThick,  PVM_SpatResol[0],
    SliceBandWidthScale);
   DB_MSG(("SliceDephGradient=%f",SliceDephGradient));
   DB_MSG(("TE=%f",PVM_EchoTime));

  }


  /* Rescale gradients, so that GradCalConst --> 1.0
     This is because we use shapes with trim=100 in ppg.*/

  UT_ScaleDoubleArray(MRT_CPMG_NPOINTS, gRead,  1/PVM_GradCalConst, gRead);
  UT_ScaleDoubleArray(MRT_CPMG_NPOINTS, gPhase, 1/PVM_GradCalConst, gPhase);
  UT_ScaleDoubleArray(MRT_CPMG_NPOINTS, gSlice, 1/PVM_GradCalConst, gSlice);


  /* produce gradient shapes */
  make_shapes(grid,
        tRead,tPhase,tSlice, gRead,gPhase,gSlice);

  //DP show Cycl/Px information
  SpoilingReadPx  = SpoilingRead  * PVM_SpatResol[0];
  SpoilingSliceSl = SpoilingSlice * PVM_SliceThick;


  /* calculate frequency lists */
  LocalFrequencyOffsetRels();

  /* repetitions and averages */
  PVM_NRepetitions = MAX_OF(1,PVM_NRepetitions);
  Local_NAveragesRange();

  /* multi-echo parameters */
  PVM_NEchoImages = 1;
  NEchoes = PVM_RareFactor * PVM_NEchoImages;
  effEchoTimeRels();


  /* handling of modules */
  double spoilerThick = dim>2 ? PVM_SpatResol[2]*PVM_EncZf[2] : PVM_SliceThick;
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, spoilerThick);
  STB_UpdateSatTransModule(PVM_DeriveGains);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  STB_UpdateTriggerModule();

  /* Update Fair module */
  double fixedTime =  0.03                   + /*UpdateDynPars*/
                      PVM_FovSatModuleTime   +
                      PVM_SatTransModuleTime +
                      PVM_FatSupModuleTime   +
                      RepetitionSpoiler.dur  +
                      ExcPulse1.Length/2.0   +
                      0.02;

  double slicePackThick = PVM_SPackArrSliceDistance[0]*(PVM_SPackArrNSlices[0] -1)+PVM_SliceThick;
  double slicePackOffset = PVM_SPackArrSliceOffset[0];

  STB_UpdateFairModule(PVM_DeriveGains, fixedTime, slicePackThick, slicePackOffset, PVM_SliceThick);

  /* TR */
  if(PVM_FairRepSpacing==CONST_REP)
    UpdateRepetitionTime(fixedTime);
  else
    UpdateRecoveryTime(fixedTime);

  STB_UpdateDummyScans(PVM_RepetitionTime, PVM_MotionSupOnOff==On ? 1:PVM_NAverages);

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  /* set baselevel acquisition parameters */
  SetBaseLevelParam();

  /* set baselevel reconstruction parameter */
  SetRecoParam();

  DB_MSG(("<--backBone\n"));
}

void UpdateRecoveryTime(double fixedTime)
{
  int nSlices,dim,i;
  double TotalTime, tail, packageTime;
  double riseT = CFG_GradientRiseTime();
  double igwt = CFG_InterGradientWaitTime();

  DB_MSG(("-->UpdateRecoveryTime"));

  MinRecoveryTime = igwt  + /* min d0 */
                    2*(PVM_EchoTime/2 - RefPulse1.Length/2 - GradStab - PVM_AcquisitionTime/2) + /* 2*d3 */
                    PVM_AcquisitionTime +
                    GradStab + /* d8 */
                    2.0 +
                    riseT;

  RecoveryTime = MAX_OF(MinRecoveryTime, RecoveryTime);

  TotalTime = 0.0;
  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  /* tail: time from last echo to the end of slice loop */
  tail = (PVM_EchoTime - RefPulse1.Length -2*GradStab)/2 + 2.0 + riseT;

  packageTime =
    nSlices * (
               0.03                   + /*UpdateDynPars*/               
               0.02                   +
               PVM_FatSupModuleTime   +
               PVM_SatTransModuleTime +
               PVM_FovSatModuleTime   +
               RepetitionSpoiler.dur  +
               ExcPulse1.Length / 2.0 +
               PVM_EchoTime*NEchoes   +
               tail
    );


  /* Calculate Total Scan Time */

  TotalTime=0.0;
  double delay = RecoveryTime -
                 MinRecoveryTime +
                 igwt  +
                 riseT +
                 PVM_FairInvPul.Length/2.0;

  PARX_change_dims("MultiRepTime", PVM_FairTIR_NExp);
  /* Access to PVM_FairTIR_Arr */
  for(i=0;i<PVM_FairTIR_NExp;i++)
  {
    MultiRepTime[i] = delay +
                      PVM_FairTIR_Arr[i] -
                      fixedTime +
                      packageTime;
    TotalTime+=MultiRepTime[i];
  }

  dim = PTB_GetSpatDim();
  if( dim >1 )
    TotalTime = TotalTime *PVM_EncMatrix[1]*PVM_NAverages;
  if( dim >2 )
    TotalTime = TotalTime *PVM_EncMatrix[2];

  TotalTime /= PVM_RareFactor;

  TotalTime = TotalTime * PVM_NRepetitions;

  TotalTime = TotalTime *
    (PVM_FairMode==INTERLEAVED || PVM_FairMode==INTERLEAVED2 ? 2:1);

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");
  ParxRelsShowInEditor("MultiRepTime");

  DB_MSG(("<--UpdateRecoveryTime\n"));

}

void UpdateRepetitionTime(double fixedTime)
{
  int nSlices,dim;
  double TotalTime, tail, packageTime;
  int fairexp;

  DB_MSG(("-->UpdateRepetitionTime"));

  TotalTime = 0.0;
  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  /* tail: time from last echo to the end of slice loop */
  tail = (PVM_EchoTime - RefPulse1.Length -2*GradStab)/2 + 2.0 + CFG_GradientRiseTime();

  packageTime =
    nSlices * (
               0.03                   + /*UpdateDynPars*/
               0.02                   +
               PVM_FatSupModuleTime   +
               PVM_SatTransModuleTime +
               PVM_FovSatModuleTime   +
               RepetitionSpoiler.dur  +
               ExcPulse1.Length / 2.0 +
               PVM_EchoTime*NEchoes   +
               tail
    );

  PVM_MinRepetitionTime = CFG_GradientRiseTime() +
                          PVM_FairInvPul.Length/2.0 +
                          PVM_FairMaxTIR -
                          fixedTime +
                          packageTime +
                          CFG_InterGradientWaitTime();  /* min d0 */



  PVM_RepetitionTime = MAX_OF(PVM_MinRepetitionTime, PVM_RepetitionTime);
  PARX_change_dims("MultiRepTime", 1);
  MultiRepTime[0]=PVM_RepetitionTime;

  /* Calculate Total Scan Time */
  fairexp =  PVM_FairTIR_NExp *
    (PVM_FairMode==INTERLEAVED || PVM_FairMode == INTERLEAVED2 ? 2:1);
  dim = PTB_GetSpatDim();

  if( dim >1 )
    TotalTime = PVM_RepetitionTime *PVM_EncMatrix[1]*PVM_NAverages;
  if( dim >2 )
    TotalTime = TotalTime *PVM_EncMatrix[2];

  TotalTime /= PVM_RareFactor;

  TotalTime = TotalTime * PVM_SatTransRepetitions * PVM_NRepetitions * fairexp;

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");
  ParxRelsHideInEditor("MultiRepTime");

  DB_MSG(("<--UpdateRepetitionTime\n"));

}



void LocalGeometryMinimaRels(double *min_fov, double *min_thickness, double limGradient )
{
  /*
   * This function calculates the minima for the fields-of-view in all
   * three directions as well as the minimum slice thickness. It is always
   * assumed that all slices have the same thickness
   *
   * The results is returned in min_fov[0..2] and min_thickness.
   *
   * Minima for read and slice are based on signel/pulse bandwidth.
   * Minima fo phase encoding are based on maximum allowed duration
   * of a phase encoding pulse (MAX_ENCODING_TIME_ms)
   */

#define MAX_ENCODING_TIME_ms 5.0

  DB_MSG(("-->LocalGeometryMinimaRels\n"));

  switch( PTB_GetSpatDim() )
     {
     case 3:

       min_fov[2] = MRT_PhaseFov( MAX_ENCODING_TIME_ms,
          PVM_Matrix[2],
          limGradient,
          PVM_GradCalConst );
       /* falls through */

     case 2:
       min_fov[1] = MRT_PhaseFov( MAX_ENCODING_TIME_ms,
          PVM_Matrix[1],
          limGradient,
          PVM_GradCalConst );
       /* falls through */

     case 1:
       min_fov[0] = PVM_EffSWh/(0.01 * limGradient * PVM_GradCalConst );
       /* falls through */

     default:

       *min_thickness = ExcPulse1.Bandwidth*SliceBandWidthScale/(limGradient * PVM_GradCalConst );
       break;
     }
  DB_MSG(("<--LocalGeometryMinimaRels\n"));

#undef MAX_ENCODING_TIME
}



void LocalFrequencyOffsetRels( void )
{
  int spatDim;
  int i,nslices;

  DB_MSG(("-->LocalFrequencyOffsetRels\n"));

  spatDim = PTB_GetSpatDim();

  nslices = GTB_NumberOfSlices(PVM_NSPacks,PVM_SPackArrNSlices);

  /*
   * Calculate offset in read direction
   */

  MRT_FrequencyOffsetList(nslices,
        PVM_EffReadOffset,
        ReadGradient,
        PVM_GradCalConst,
        PVM_ReadOffsetHz );

  /*
   * Calculate slice offset
   */

  MRT_FrequencyOffsetList(nslices,
        PVM_EffSliceOffset,
        ExcSliceGradient,
        PVM_GradCalConst,
        PVM_SliceOffsetHz );

  if(spatDim == 3)
  {
    for(i=0;i<nslices;i++)
      PVM_EffPhase2Offset[i] = -PVM_EffSliceOffset[i];
  }

  DB_MSG(("<--LocalFrequencyOffsetRels\n"));

}


void  make_shapes(double grid,
      const double *tRead, const double *tPhase, const double *tSlice,
      const double *gRead, const double *gPhase, const double *gSlice)
 /* ---------------------------------------------------------------------------
    Draw gradient shapes described by t- and g- tables in the buffers
    ReadShape, PhaseShape, SliceShape, SliceEncShape
 ------------------------------------------------------------------------------*/
{
  double totalTime;
  int i, nPoints;
  double (*rampFun)(double);

  totalTime = tRead[7]-tRead[0];  /* pulse plateau not included */
  ShapeSize = nPoints = (int)(totalTime/grid);

  /* select ramp type */
  rampFun = RampForm==LinearRamp? ramp_lin : ramp_sin;


  /* redimension shapes */
  PARX_change_dims("ReadShape",  nPoints);
  PARX_change_dims("PhaseShape", nPoints);
  PARX_change_dims("SliceShape", nPoints);
  PARX_change_dims("SliceEncShape", nPoints);

  /* zero-filling */
  memset(ReadShape, 0, nPoints*sizeof(double));
  memset(PhaseShape, 0, nPoints*sizeof(double));
  memset(SliceShape, 0, nPoints*sizeof(double));
  memset(SliceEncShape, 0, nPoints*sizeof(double));


  /* draw the readout shape */
  for(i=0; i<7; i++)
    MRT_MakeRamp(ReadShape, nPoints, totalTime, tRead[i]-tRead[0], tRead[i+1]-tRead[0],
     gRead[i], gRead[i+1], rampFun);


  /* draw the phase shape */
  for(i=0; i<7; i++)
    MRT_MakeRamp(PhaseShape, nPoints, totalTime, tPhase[i]-tPhase[0], tPhase[i+1]-tPhase[0],
     gPhase[i], gPhase[i+1], rampFun);

  /* draw the slice shape */
  for(i=0; i<7; i++)
    MRT_MakeRamp(SliceShape, nPoints, totalTime, tSlice[i]-tSlice[0], tSlice[i+1]-tSlice[0],
     gSlice[i], gSlice[i+1], rampFun);

  /* make sure the last point is correct (=slice sel grad) */
  SliceShape[nPoints-1] = gSlice[0];
  PhaseShape[nPoints-1] = 0.0;
  ReadShape[nPoints-1] = 0.0;


}

/* automatic spoiler values */
void updateSpoilers(void)
{
  if(AutoSpoil == Yes)
  {
    SpoilingReadPx = 0.7;
    SpoilingSliceSl = 1.0;
    RepetitionSpoiler.automatic = Yes;
    ParxRelsMakeNonEditable("SpoilingReadPx,SpoilingSliceSl,RepetitionSpoiler");
  }
  else
  {
    ParxRelsMakeEditable("SpoilingReadPx,SpoilingSliceSl,RepetitionSpoiler");
  }

  /* update repetition spoiler */
  double mindur = 3.0*CFG_GradientRiseTime();
  MRT_UpdateSpoiler("RepetitionSpoiler",2.5,70,mindur,PVM_GradCalConst,PVM_SliceThick);
}

void effEchoTimeRels( void )
{
  int i;

  DB_MSG(("-->effEchoTimeRels\n"));

  /*
   * echo spacing (PVM_EchoTime) must be set before
   */


  PARX_change_dims("EffectiveTE", PVM_NEchoImages);
  EffectiveTE[0] =  PVM_EchoTime*PVM_EncCentralStep1;
  for(i=1; i<PVM_NEchoImages; i++)
    EffectiveTE[i] =  EffectiveTE[0] + i * PVM_RareFactor * PVM_EchoTime;

  /* Echo Parameters for Scan Editor  */

  PVM_EchoTime1 = EffectiveTE[0];
  PVM_EchoTime2 = PVM_NEchoImages>1? EffectiveTE[1]:PVM_EchoTime1;

  DB_MSG(("<--echoTimeRels\n"));
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
    ParxRelsShowInEditor("ExcPulse1Ampl,RefPulse1Ampl");
  }

  ParxRelsShowInFile("ExcPulse1Ampl,RefPulse1Ampl");

  DB_MSG(("<--UpdateRFPulses"));

  return;
}
