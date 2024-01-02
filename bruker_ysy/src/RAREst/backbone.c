/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2007-2014
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

const char * RAREst_backbone_id = "$Id$ (C) 2007-2014 Bruker BioSpin MRI GmbH";


#define DEBUG       0
#define DB_MODULE   1
#define DB_LINE_NR  1

#include "method.h"


#if DEBUG
#define PRINTARRAY(file, array, nelem)  \
{\
int ix; \
FILE *dfp; \
if((dfp=fopen(file,"w"))!=NULL) \
{ \
for(ix=0; ix<(nelem); ix++) fprintf(dfp, " %g\n", (double) array[ix]);\
fclose(dfp); \
} \
}
#else
#define PRINTARRAY(file, array, nelem) ;
#endif



/* ------------------------------------------------------------
 * backbone
 * The main part of method code. The consitency of all parameters
 * is checked here, relations between them are resolved and,
 * finally, functions setting the base level parameters are
 * called.
 */


void backbone( void )
{
  int    i, ret, dim;
  double minFov[3] = {1e-3, 1e-3, 1e-3},
         minThickness,
         gradientLimit = 57,  /* gradient limit, in % of max, for all channels */
         sliceScale[2],
         grid;

  int    nSlices;
  double tRead[MRT_CPMG_NPOINTS], tPhase[MRT_CPMG_NPOINTS], tSlice[MRT_CPMG_NPOINTS],
         gRead[MRT_CPMG_NPOINTS], gPhase[MRT_CPMG_NPOINTS], gSlice[MRT_CPMG_NPOINTS];


  DB_MSG(("-->backbone\n"));


  STB_UpdateNuclei(No);

  /* time grid for gradienet shapes, in ms */
  grid = MAX_OF(CFG_GradientShapeResolution(),0.002);

  /* handle RF pulses */

  UpdateRFPulses();



  /* update encoding parameter group                          */

  STB_UpdateEncoding(
    &PVM_RareFactor,
    SEG_INTERLEAVED,
    Yes,
    Yes,
    Yes);

  dim=PTB_GetSpatDim();

  /* Set PFT to 1.8 by default (InitEncoding is set in initMeth) */
  if(InitEncoding == Yes && dim>1)
  {
    PVM_EncPft[1] = 1.8;
    STB_UpdateEncoding( &PVM_RareFactor, SEG_INTERLEAVED, Yes, Yes, Yes);
    InitEncoding = No;
  }

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

  // only one package if black-blood module on
  int maxPackages = PVM_BlBloodOnOff == On? 1:0;

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
  
  std::vector<double> spatres=STB_GetSpatResol();
  STB_AtsUpdate(&spatres);
  
  updateSpoilers();

  SpoilingRead  = SpoilingReadPx/PVM_SpatResol[0];
  SpoilingSlice = SpoilingSliceSl/PVM_SliceThick ;


  /* calculate gradient strengths */

  ReadGradient = MRT_ReadGrad( PVM_EffSWh, PVM_Fov[0], PVM_GradCalConst );

  solve90_180(
    &PVM_EchoTime,
    &SpoilingRead, &SpoilingSlice,
    &ExcSliceGradient, &RefSliceGradient,
    &ExcPulse1, &RefPulse1,
    GradStab,
    &ReadDephGradient, &SliceDephGradient,
    PVM_GradCalConst,
    PVM_SliceThick,  PVM_SpatResol[0],
    SliceBandWidthScale
  );

  DB_MSG(("SliceDephGradient=%f",SliceDephGradient));
  DB_MSG(("TE=%f",PVM_EchoTime));

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


  for(i=0; i<MRT_CPMG_NPOINTS; i++)
  {
    DB_MSG(("read  t[%d]= %10g, g[%d]= %10g",i,  tRead[i],  i, gRead[i]));
    DB_MSG(("phase t[%d]= %10g, g[%d]= %10g",i,  tPhase[i], i, gPhase[i]));
    DB_MSG(("slice t[%d]= %10g, g[%d]= %10g",i,  tSlice[i], i, gSlice[i]));

  }


  if(ret > 0) /* PVM_EchoTime increased: solve 90-180 again */
  {
    DB_MSG(("re-calculating 90-180"));
    DB_MSG(("TE=%f",PVM_EchoTime));

    solve90_180(
      &PVM_EchoTime,
      &SpoilingRead, &SpoilingSlice,
      &ExcSliceGradient, &RefSliceGradient,
      &ExcPulse1, &RefPulse1,
      GradStab,
      &ReadDephGradient, &SliceDephGradient,
      PVM_GradCalConst,
      PVM_SliceThick,  PVM_SpatResol[0],
      SliceBandWidthScale
    );

    DB_MSG(("SliceDephGradient=%f",SliceDephGradient));
    DB_MSG(("TE=%f",PVM_EchoTime));
  }


  checkIntegrals(tRead, tPhase, tSlice, gRead, gPhase, gSlice);

  /* Rescale gradients, so that GradCalConst --> 1.0
     This is because we use shapes with trim=100 in ppg.*/

  UT_ScaleDoubleArray(MRT_CPMG_NPOINTS, gRead,  1/PVM_GradCalConst, gRead);
  UT_ScaleDoubleArray(MRT_CPMG_NPOINTS, gPhase, 1/PVM_GradCalConst, gPhase);
  UT_ScaleDoubleArray(MRT_CPMG_NPOINTS, gSlice, 1/PVM_GradCalConst, gSlice);


  /* produce gradient shapes */
  make_shapes(grid, tRead, tPhase, tSlice, gRead, gPhase, gSlice);

  //DP show Cycl/Px information
  SpoilingReadPx  = SpoilingRead  * PVM_SpatResol[0];
  SpoilingSliceSl = SpoilingSlice * PVM_SliceThick;


  /* calculate frequency lists */
  LocalFrequencyOffsetRels();

  /* repetitions and averages */
  PVM_NRepetitions = MAX_OF(1,PVM_NRepetitions);
  Local_NAveragesRange();

  /* multi-echo parameters */
  PVM_NEchoImages = MAX_OF(1, PVM_NEchoImages);
  PVM_NEchoImages = MIN_OF(8, PVM_NEchoImages);
  NEchoes = PVM_RareFactor * PVM_NEchoImages;
  effEchoTimeRels();


  /* handling of modules */
  double spoilerThick = dim>2 ? PVM_SpatResol[2]*PVM_EncZf[2] : PVM_SliceThick;
  STB_UpdateDriftCompModule(spoilerThick, PVM_DeriveGains);
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, spoilerThick);
  STB_UpdateSatTransModule(PVM_DeriveGains);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  STB_UpdateTriggerModule();
  STB_UpdateTaggingModule(dim,PVM_Fov,PVM_Matrix,PVM_SpatResol[0]*PVM_EncZf[0],PVM_DeriveGains);
  STB_UpdateEvolutionModule();

  /* Update module SelIr */
  MinSliceSegDur = minLoopRepetitionTime();
  /* SliceSegDur may be changed by the UpdateSelIrModule! */
  SliceSegDur = MinSliceSegDur;

  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices);

  /* Inversion time TI extends up to the centre of the excitation pulse!         *
   * Some delays of the imaging block and module durations have to be considered.*
   * Their sum is handed to the updater. Some modules may not make sense to be   *
   * combined w inversion or even harm but are taken into acount anyway:         */
  double fixedTime =  0.03                      /* UpdateDynPars */
                    + 0.01                      /* freqRx */
                    + PVM_TaggingModuleTime
                    + PVM_FovSatModuleTime
                    + PVM_SatTransModuleTime
                    + PVM_FatSupModuleTime
                    + 0.01                      /* freqTx  */
                    + RepetitionSpoiler.dur     /* d1 + d4 */
                    + ExcPulse1.Length/2.0;     /* p0/2    */

  /* consider trigger event within slice-loop only if mode is per_Slice: */
  if (PVM_TriggerMode == per_Slice)
    fixedTime += PVM_TriggerModuleTime;

  STB_UpdateSelIrModule( PVM_SliceThick, PVM_EffSliceOffset, nSlices,
                        &SliceSegDur, 0, fixedTime, PVM_DeriveGains );

  if(PVM_SelIrOnOff==On)
    ParxRelsCopyPar("PVM_InversionTime","PVM_SelIrInvTime");

  if(PVM_SelIrOnOff==Off)
  {
    PVM_InversionTime = 0.0;
    ParxRelsHideInEditor("PVM_InversionTime");
  }

  /* TR */
  repetitionTimeRels();
  STB_UpdateDummyScans(PVM_RepetitionTime);

  /* Set correct DriftCompUpdateTime after TR-upate */
  STB_UpdateDriftComp(PVM_RepetitionTime);

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  /* set baselevel acquisition parameters */
  SetBaseLevelParam();

  /* set baselevel reconstruction parameter */
  SetRecoParam();

  DB_MSG(("<--backBone\n"));
}



void repetitionTimeRels( void )
{
  DB_MSG(("-->repetitionTimeRels"));

  double TotalTime = 0.0;
  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  if (PVM_SelIrOnOff)
  {
    PVM_MinRepetitionTime =  PVM_DriftCompModuleTime
                           + PVM_SelIrModuleTime
                           + nSlices * SliceSegDur;
  }
  else
  {
    PVM_MinRepetitionTime =  PVM_DriftCompModuleTime
                           + PVM_SelIrModuleTime
                           + nSlices * minLoopRepetitionTime();
  }


  if (PVM_TriggerMode == per_PhaseStep)
    PVM_MinRepetitionTime += PVM_TriggerModuleTime;

  PVM_RepetitionTime = MAX_OF( PVM_MinRepetitionTime, PVM_RepetitionTime );

  /* Calculate Total Scan Time and Set for Scan Editor */

  int dim = PTB_GetSpatDim();
  if( dim >1 )
    TotalTime = PVM_RepetitionTime *PVM_EncMatrix[1]*PVM_NAverages;
  if( dim >2 )
    TotalTime = TotalTime *PVM_EncMatrix[2];

  TotalTime /= PVM_RareFactor;

  /* time for one repetition */
  OneRepTime = TotalTime/1000.0;

  TotalTime = TotalTime * PVM_EvolutionCycles + PVM_EvolutionModuleTime;
  TotalTime = TotalTime * PVM_SatTransRepetitions * PVM_NRepetitions;

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<--repetitionTimeRels\n"));

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

  /* print shapes for plotting */
  PRINTARRAY("/tmp/cpmg_r",ReadShape,  nPoints);
  PRINTARRAY("/tmp/cpmg_p",PhaseShape, nPoints);
  PRINTARRAY("/tmp/cpmg_s",SliceShape, nPoints);

}


void  checkIntegrals(double *tRead,double * tPhase,double * tSlice,double * gRead,
                     double * gPhase,double * gSlice)
{
  int i;
  double x;

  /* read integral */
  x=0.0;
  for(i=0; i<7; i++)
    x += (tRead[i+1]-tRead[i])*(gRead[i+1]+gRead[i])/2000.0;
  DB_MSG(("read integral = %g cycles/mm (should be %g)", x, 2*SpoilingRead));

  /* phase integral (total) */
  x=0.0;
  for(i=0; i<7; i++)
    x += (tPhase[i+1]-tPhase[i])*(gPhase[i+1]+gPhase[i])/2000.0;
  DB_MSG(("phase integral (total) = %g cycles/mm (should be %g)", x, 0.0));

  /* phase integral (first pulse) */
  x=0.0;
  for(i=0; i<3; i++)
    x += (tPhase[i+1]-tPhase[i])*(gPhase[i+1]+gPhase[i])/2000.0;
  DB_MSG(("phase integral (first pulse)= %g cycles/mm (should be %g)", x, 0.5/PVM_SpatResol[1]));

  /* slice integral */
  x=tSlice[0]*gSlice[0]/1000; /* rectangle under ref. pulse */
  for(i=0; i<7; i++)
    x += (tSlice[i+1]-tSlice[i])*(gSlice[i+1]+gSlice[i])/2000.0;
  DB_MSG(("slice integral = %g cycles/mm (should be %g)", x, 2*SpoilingSlice ));


}

double minLoopRepetitionTime(void)
/* ------------------------------------------------------------*
 * this function returns the minimum duration of an echo train *
 * including some modules: all delays inside slice-loop in     *
 * RAREst.ppg.                                                   *
 *-------------------------------------------------------------*/
{
  double minTr, tail;
  double minD0 = CFG_InterGradientWaitTime();

  /* tail = sequence duration after the last echo */
  tail = (PVM_EchoTime - RefPulse1.Length -2*GradStab)/2 + 2.0 + CFG_GradientRiseTime();

  minTr =  0.03                        /* UpdateDynPars */  
         + 0.01                        /* freqRx  */
         + PVM_TaggingModuleTime
         + PVM_FatSupModuleTime
         + PVM_SatTransModuleTime
         + PVM_FovSatModuleTime
         + 0.01                        /* freqTx  */
         + RepetitionSpoiler.dur       /* d1 + d4 */
         + ExcPulse1.Length / 2.0      /* p0/2    */
         + PVM_EchoTime*NEchoes
         + tail
         + minD0;

  /* consider TriggerSlice()-routine */
  if (PVM_TriggerMode == per_Slice )
    minTr += PVM_TriggerModuleTime;

  return minTr;
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
