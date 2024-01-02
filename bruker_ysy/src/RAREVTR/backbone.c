/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2009-2014
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 * ***************************************************************/


const char * RAREVTR_backbone_id = "$I$ (c) 2007-2014 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	0


#include "method.h"


void backbone(void)
{
  double minFov[3];
  double minSliceThick;
  int dim;

  DB_MSG(("-->backbone"));

  /* update nuclei parameter group                            */

  STB_UpdateNuclei(No);

  dim=PTB_GetSpatDim();

  /* update encoding parameter group                          */

  STB_UpdateEncoding(
    &PVM_RareFactor,
    SEG_INTERLEAVED,
    Yes,
    Yes,
    Yes);


  PVM_EchoPosition = 50;

  /* update parameters controlling the data sampling period   */

  MRT_UpdateDigPars(&PVM_EffSWh,
                     PVM_EncMatrix[0],
                    &PVM_AntiAlias[0],
                    &PVM_AcquisitionTime);

  /* update excitation pulse                                  */

  UpdateRFPulses();


  /* general features of the method */

  PVM_NRepetitions = MAX_OF(1,PVM_NRepetitions);

  /* set limits for read, phase and slice gradients            */

  ControlGradientLimits(PVM_MajSliceOri);


  /* calculate minima for FOV and slice thickness             */

  UpdateGeometryMinima(minFov,
                       &minSliceThick);


  /* update geometry parameters                               */

  // only one slice per package if 3D
  int maxPerPackage = dim>2? 1:0;

  STB_UpdateImageGeometry(dim,
                          PVM_Matrix,
                          minFov,
                          0, // total slices (no restr)
                          0,
                          maxPerPackage,
                          minSliceThick,
                          1.0); // sliceFovRatio in 3D
  
  std::vector<double> spatres=STB_GetSpatResol();
  STB_AtsUpdate(&spatres);

  /* update repetition spoiler */
  double mindur = 5.0*CFG_GradientRiseTime();
  double effSlice = dim>2? PVM_SpatResol[2]*PVM_EncZf[2] : PVM_SliceThick;

  MRT_UpdateSpoiler("RepetitionSpoiler",4.0,60.0,mindur,PVM_GradCalConst,effSlice);

  /* handling of modules */
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, effSlice);
  STB_UpdateSatTransModule(PVM_DeriveGains);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  STB_UpdateFlowSaturationModule(PVM_Nucleus1,PVM_DeriveGains);
  STB_UpdateTriggerModule();

  /* Calculate read and slice gradients */
  UpdateReadSliceGradients();

  /* Sequence elements, TE, TR, total time, TE-dependent modules: */
  UpdateSequenceTiming();

  PVM_RepetitionTime=MultiRepTime[0];
  STB_UpdateDummyScans(PVM_RepetitionTime);

  /* calculate dephasing and phase-encoding gradients         */
  UpdatePhaseGradients();

  /* calculate frequency offsets                              */
  UpdateFrequencyOffsets();

  /* calculate additional echo spoilers (depend on phase gradients) */
  UpdateEchoSpoilers();

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  /* Set ACQP parameters */
  SetBaseLevelParam();

  /* Set RECO parameters */
  SetRecoParam();

  DB_MSG(("<--backbone"));
  return;
}

/*-------------------------------------------------------
 * local utility routines to simplify the backbone
 *------------------------------------------------------*/


void UpdateSequenceTiming()
/* -------------------------------------------------------
   Adapt sequence elements to the current geometry
   (in this method, only EncGradDur is concerned),
   update TE and TR.
   ReadGrad and ExSliceGrad must be already set.
   ------------------------------------------------------*/
{
  double minEnc2d, minEnc3d, minRephSlice, minDephRead,
    extensionPossible, extensionAllowed, extension,
    minTe1, minTe2;

  /* Part of the exctiation pulse to be refocused: */
  EffPulseDur = ExcPulse1.Length * (ExcPulse1.Rpfac/100);


  /* Part of the echo to be refocused */
  EchoDelay = PVM_AcquisitionTime * PVM_EchoPosition / 100;

  /* Minimum durations of all phase-gradient pulses */

  minEnc2d     = MRT_EncodingTime(PVM_SpatResol[1],
                                  PVM_GradCalConst*Phase2DGradLim/100);

  minEnc3d     = PTB_GetSpatDim()>2?
                 MRT_EncodingTime(PVM_SpatResol[2],
                                     PVM_GradCalConst*Phase3DGradLim/100)
                 : 0;

  minRephSlice = MRT_DephaseTime(EffPulseDur,
                                 CFG_GradientRiseTime(),
                                 ExcSliceGrad,
                                 ExcSliceRephGradLim);

  minDephRead  = MRT_DephaseTime(EchoDelay,
                                 CFG_GradientRiseTime(),
                                 ReadGrad,
                                 ReadDephGradLim);


  /* In this sequence encoding and slice-rephasing pulses are
     simultaneous with duration EncGradDur;
     read-dephasing is independent and given by  ReadDephDur.
     We set ehm first to minima allowed by geometry: */

  EncGradDur = MAX_OF( MAX_OF(minEnc2d ,  minEnc3d), minRephSlice  );

  /* EncGradDur should also contain one ramp, thus: */

  EncGradDur = MAX_OF(EncGradDur, CFG_GradientRiseTime());

  /* same for ReadDeph */

  ReadDephDur = MAX_OF(minDephRead, CFG_GradientRiseTime());


  DB_MSG(("minimum EncGradDur = %f, ReadDephDur = %f", EncGradDur,
                                                       ReadDephDur));


  /* Update TE with the mimimum EncGradDur */

  UpdateEchoTime(&minTe1, &minTe2);


  /* If there is some freedom, make ReadDephDur longer
     (to avoid unnecessarily high amplitude),
     but not too long either.. */

  extensionPossible = (PVM_EchoTime - minTe1)/2;
  extensionAllowed  = 2*EchoDelay - ReadDephDur;
  extension = MIN_OF(extensionPossible, extensionAllowed);
  extension = MAX_OF(extension, 0);
  ReadDephDur += extension;
  /* fill-delay after excitation */
  TeFillDelay1 = (PVM_EchoTime - minTe1)/2 - extension;



  /* If there is some freedom, make EncGradDur  longer
     (to avoid unnecessarily strong phase gradients),
     but not longer than allowed by gradient resolution */

  extensionPossible = (PVM_EchoTime - PVM_MinEchoTime)/2;
  extensionAllowed  = MRT_MaxEncodingTime(PVM_Fov[1], PVM_GradCalConst)-EncGradDur;
  extension = MIN_OF(extensionPossible, extensionAllowed);
  extension = MAX_OF(extension, 0);
  EncGradDur += extension;

  /* fill-delay between pi-pulse and acq */
  TeFillDelay2 = (PVM_EchoTime - minTe2)/2 - extension;


  /* Update modules dependent on TE, here: inversion recovery: */
  SliceSegDur = minLoopRepetitionTime(); /* depends on TE */

  /* Number and effective echo time of multiple echoes */
  UpdateMultiEchoes();

  /* Find min TR */
  UpdateMinRepTime();

  UpdateMultiRepTime();

  /* Calculate total experiment time */
  UpdateTotalTime();
}



void UpdateMinRepTime(void)
{
  int nSlices;

  DB_MSG(("-->UpdateMinRepTime"));


  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  /* min TR in a movie: */
  PVM_MinRepetitionTime = nSlices * minLoopRepetitionTime();

  DB_MSG(("minLoopRepetitionTime() = %f ms", minLoopRepetitionTime()));
  DB_MSG(("PVM_MinRepetitionTime = %f ms", PVM_MinRepetitionTime));


  DB_MSG(("<--UpdateMinRepTime"));
  return;
}

void UpdateMultiRepTime(void)
{
  DB_MSG(("-->UpdateMultiRepTime"));

  if(RepTimeMode==Calculate_RepTime)
    T1Exp = MAX_OF(3,T1Exp);

  int dim=PARX_get_dim("MultiRepTime", 1 );
  if(dim != T1Exp )
  {
    PARX_change_dims("MultiRepTime", T1Exp);
  }
  if(RepTimeMode==Calculate_RepTime)
  {
    EstT1Value=MAX_OF(EstT1Value,PVM_MinRepetitionTime);
    MinT1RepTime = MAX_OF(PVM_MinRepetitionTime,MIN_OF(MinT1RepTime,3*EstT1Value));
    double SigMin = 1.0 - exp(-1.0*MinT1RepTime/EstT1Value);
    double step = (0.993262 - SigMin) / ((double)(T1Exp-1));
    double sig=0.0;
    for(int i=0;i<T1Exp;i++)
    {
      sig = SigMin + (double)i* step;
      MultiRepTime[i] = -1.0 * EstT1Value * log(1.0 - sig);
    }
    MultiRepTime[0] = MinT1RepTime;
    MultiRepTime[T1Exp-1] = 5.0*EstT1Value;
    ParxRelsMakeNonEditable("MultiRepTime");
    ParxRelsShowInEditor("EstT1Value,MinT1RepTime");
  }
  else
  {
    ParxRelsHideInEditor("EstT1Value,MinT1RepTime");
    ParxRelsMakeEditable("MultiRepTime");
  }

  /* check legal values */
  for(int i=0;i<T1Exp;i++)
  {
    MultiRepTime[i] = MultiRepTime[i]<PVM_MinRepetitionTime ? PVM_MinRepetitionTime:MultiRepTime[i];
  }


  DB_MSG(("<--UpdateMultiRepTime"));
}

/* calculates PVM_ScanTimeStr and TimeForMovieFrames */

void UpdateTotalTime(void)
{
  int dim = PTB_GetSpatDim();
  double TotalTime=0;
  double TotalRepTime=0.0;

  for(int i=0;i<T1Exp;i++)
    TotalRepTime+=MultiRepTime[i];

  if( dim >1 )
  {
      TotalTime = TotalRepTime * PVM_EncMatrix[1]/PVM_RareFactor
                  * PVM_NAverages;
  }

  if( dim >2 )
    TotalTime = TotalTime * PVM_EncMatrix[2];

  TotalTime = TotalTime * PVM_SatTransRepetitions * PVM_NRepetitions;

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

}

void UpdateGeometryMinima( double *minFov,
                           double *minSliceThick)
{
  int dim;


  DB_MSG(("-->UpdateGeometryMinima"));


  dim=PTB_GetSpatDim();

  minFov[0]     = PVM_EffSWh /
                  (1e-2*ReadGradLim * PVM_GradCalConst);
  *minSliceThick = ExcPulse1.Bandwidth /
                  (1e-2*ExcSliceGradLim * PVM_GradCalConst);

  if(dim >= 2)
  {
    minFov[1] = minFov[0]/8;
  }

  if(dim >= 3)
  {

    minFov[2] = *minSliceThick;
  }

  DB_MSG(("<--UpdateGeometryMinima"));
}


void UpdateReadSliceGradients(void)
{
  DB_MSG(("-->UpdateReadSliceGradients"));

  ReadGrad = MRT_ReadGrad(PVM_EffSWh,
                          PVM_Fov[0],
                          PVM_GradCalConst);

  ExcSliceGrad = MRT_SliceGrad(ExcPulse1.Bandwidth,
                               PVM_SliceThick,
                               PVM_GradCalConst);

  DB_MSG(("<--UpdateReadSliceGradients"));
}

void UpdatePhaseGradients()
{

  DB_MSG(("-->UpdatePhaseGradients"));

  /* Calculation of phase-encoding,
     dephasing and rephasing gradients.

     (ReadGrad, ExcSliceGrad, EchoDelay, EffPulseDur,
      and EncGradDur must be calculated before)       */



  ReadDephGrad = MRT_DephaseGrad(ReadDephDur,
                                 EchoDelay,
                                 CFG_GradientRiseTime(),
                                 ReadGrad);

  SliceRephGrad = MRT_DephaseGrad(EncGradDur,
                                 EffPulseDur,
                                 CFG_GradientRiseTime(),
                                 ExcSliceGrad);

  Phase2DGrad = MRT_PhaseGrad(EncGradDur,
                              PVM_Matrix[1],
                              PVM_Fov[1],
                              PVM_GradCalConst);

  if(PTB_GetSpatDim() == 3)
  {
    Phase3DGrad = MRT_PhaseGrad(EncGradDur,
                                PVM_Matrix[2],
                                PVM_Fov[2],
                                PVM_GradCalConst);
  }
  else
  {
    Phase3DGrad = 0.0;
  }

  DB_MSG(("<--UpdatePhaseGradients"));
  return;
}

void UpdateEchoSpoilers()
{
  // Calculates spoiling of the FID signals of refocusing pulses.
  DB_MSG(("-->UpdateEchoSpoilers"));

  double gmax = PVM_GradCalConst*1e-3; // kHz/mm
  double maxSpoilSliceLeft;  // max spoiling left to first 180, cycles/mm
  double maxSpoilSliceRight; // max spoiling right to 180, cycles/mm
  double optSpoilSlice, effResolSlice, spoilSliceEff;
  double maxSpoilReadLeft;  // max spoiling left to first 180, cycles/mm
  double maxSpoilReadRight; // max spoiling right to 180, cycles/mm
  double optSpoilRead, effResolRead, spoilReadEff;

  if(EchoSpoilingAuto == Yes)
  {
    EchoSpoilingSlice = 1.0;
    EchoSpoilingRead = 0.25;
    ParxRelsMakeNonEditable("EchoSpoilingSlice,EchoSpoilingRead");
  }
  else
  {
    ParxRelsMakeEditable("EchoSpoilingSlice,EchoSpoilingRead");
  }

  ParxRelsMakeNonEditable("EchoSpoilingSliceEff,EchoSpoilingReadEff");

  // slice spoiler

  effResolSlice = PTB_GetSpatDim()>2? PVM_SpatResol[2] : PVM_SliceThick;

  optSpoilSlice = EchoSpoilingSlice/effResolSlice;

  maxSpoilSliceLeft  = ReadDephDur*gmax*(ExcSliceRephGradLim-0)/100;
  maxSpoilSliceRight = EncGradDur *gmax*(ExcSliceRephGradLim-SliceRephGrad-Phase3DGrad)/100;

  spoilSliceEff = MIN_OF(maxSpoilSliceLeft, maxSpoilSliceRight);
  spoilSliceEff = MIN_OF(spoilSliceEff, optSpoilSlice);
  spoilSliceEff = MAX_OF(spoilSliceEff, 0);

  EchoSpoilingSliceEff = spoilSliceEff*effResolSlice;

  // read spoiler

  effResolRead = PVM_SpatResol[0];

  optSpoilRead = EchoSpoilingRead/effResolRead;

  maxSpoilReadLeft  = ReadDephDur*gmax*(ReadDephGradLim-ReadDephGrad)/100;
  maxSpoilReadRight = EncGradDur *gmax*(ReadDephGradLim-0)/100;

  spoilReadEff = MIN_OF(maxSpoilReadLeft, maxSpoilReadRight);
  spoilReadEff = MIN_OF(spoilReadEff, optSpoilRead);
  spoilReadEff = MAX_OF(spoilReadEff, 0);

  EchoSpoilingReadEff = spoilReadEff*effResolRead;

  // amplitudes used in pulse program
  SliceSpoilGradL = spoilSliceEff * 100/(ReadDephDur*gmax);
  SliceSpoilGradR = spoilSliceEff * 100/(EncGradDur*gmax);
  ReadSpoilGradL = spoilReadEff * 100/(ReadDephDur*gmax);
  ReadSpoilGradR = spoilReadEff * 100/(EncGradDur*gmax);


  DB_MSG(("max slice spoiling: opt %f, left %f, right %f",
          optSpoilSlice, maxSpoilSliceLeft, maxSpoilSliceRight  ));

  DB_MSG(("<--UpdateEchoSpoilers"));
}


void UpdateFrequencyOffsets( void )
{
  int spatDim;
  int i,nslices;

  DB_MSG(("-->UpdateFrequencyOffsets"));
  spatDim = PTB_GetSpatDim();
  nslices = GTB_NumberOfSlices(PVM_NSPacks,PVM_SPackArrNSlices);

  MRT_FrequencyOffsetList(nslices,
                          PVM_EffReadOffset,
                          ReadGrad,
                          PVM_GradCalConst,
                          PVM_ReadOffsetHz );

  MRT_FrequencyOffsetList(nslices,
                          PVM_EffSliceOffset,
                          ExcSliceGrad,
                          PVM_GradCalConst,
                          PVM_SliceOffsetHz );

  if(spatDim == 3)
  {
    for(i=0;i<nslices;i++)
      PVM_EffPhase2Offset[i] = -PVM_EffSliceOffset[i];
  }

  DB_MSG(("<--UpdateFrequencyOffsets"));
}


/*--------------------------------------------------------
 * Routine to update RF pulse parameters
 *-------------------------------------------------------*/

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

/*--------------------------------------------------------
 * Routine to control the visibility of parameters
 *-------------------------------------------------------*/


void ControlGradientLimits(YesNo NotOblique)
{
  DB_MSG(("-->ControlGradientLimits: Obliqueness forbidden: %s",NotOblique==Yes ? "Yes":"No"));

  if(NotOblique==Yes)
  {
    ReadDephGradLim     = 100.0;
    Phase2DGradLim      = 100.0;
    Phase3DGradLim      =  50.0; // remaining 50% for the spoiler
    ExcSliceRephGradLim = 100.0;
  }
  else
  {
    /* Gradient limits in % of max. Value 57 (1/sqrt(3))
       is needed for arbitrary oblique slices. */
    ReadDephGradLim     = 57.0;
    Phase2DGradLim      = 57.0;
    Phase3DGradLim      = 28.5; // remaining 28.5% for the spoiler
    ExcSliceRephGradLim = 57.0;
  }

  ReadGradLim        = 100.0;
  ExcSliceGradLim    = 100.0;

  DB_MSG(("-->ControlGradientLimits"));
}




/* Calculates PVM_MinEchoTime and restrict PVM_EchoTIme.
   EffPulseDur, EncGradDur must be set before.
   *minTe1, *minTe2: TE minima given by 90-180 period and
    180-180 period, respectively */
void UpdateEchoTime(double *minTe1, double *minTe2 )
{
  double riseTime = CFG_GradientRiseTime(),
    rampTime = CFG_GradientRampTime()+CFG_InterGradientWaitTime();

  DB_MSG(("-->UpdateEchoTime\n"));

  // minimum imposed by exc-reph period
  *minTe1 = (
    ExcPulse1.Length * ExcPulse1.Rpfac/100.0 +
    ReadDephDur +
    rampTime + /* min d1 */
    riseTime +
    RefPulse1.Length/2.0 ) * 2.0;

  // minimum imposed by reph-reph period
  *minTe2 =
    RefPulse1.Length +
    2*EncGradDur +
    2*rampTime + // min d6
    2*riseTime +
    PVM_AcquisitionTime;

  PVM_MinEchoTime = MAX_OF(*minTe1,*minTe2);

  PVM_EchoTime = MAX_OF(PVM_EchoTime, PVM_MinEchoTime);


  DB_MSG(("<--echoTimeRels\n"));
}



double minLoopRepetitionTime(void)
/* ---------------------------------------------------------
this function rturns the minimum duration of the innermost
pulse program loop. EffPulseDur must be known.
----------------------------------------------------------*/
{
  double riseTime = CFG_GradientRiseTime();

  double minTR = PVM_TriggerModuleTime
    + PVM_InFlowSatModuleTime
    + PVM_FovSatModuleTime
    + PVM_SatTransModuleTime
    + PVM_FatSupModuleTime;


  if (PVM_FlipBackOnOff == On) {
    minTR += 0.03  // slice label time
      + 0.01  // freqRx
      + RepetitionSpoiler.dur  // duration of spoiler prior to the excitation pulse
      + riseTime  // time to change from spoiler to slice selection grad
      + ExcPulse1.Length / 2 // durations from beginning of first pulse to ...
      + (1 + PVM_NEchoImages * PVM_RareFactor) * PVM_EchoTime // ...
      + ExcPulse1.Length / 2 // ... end of last pulse
      + riseTime; // final time for grad_off
  }

  else {
    minTR += 0.01
      + RepetitionSpoiler.dur
      + riseTime
      + ExcPulse1.Length / 2
      + (0.5 + PVM_NEchoImages * PVM_RareFactor) * PVM_EchoTime // goes to far, we need to subtract ...
      - RefPulse1.Length / 2; // ... half of the refocusing pulse duration
    }

  return minTR;
}

/* ------------------------------------------------------

   ------------------------------------------------------*/
void  UpdateMultiEchoes(void)
{
  int i, MaxNEchoes = 256;

  PVM_NEchoImages = MAX_OF(1, PVM_NEchoImages);
  PVM_NEchoImages = MIN_OF( MAX_OF(1,MaxNEchoes/PVM_RareFactor), PVM_NEchoImages );

  NEchoes = PVM_RareFactor * PVM_NEchoImages;

  PARX_change_dims("EffectiveTE", PVM_NEchoImages);
  EffectiveTE[0] =  PVM_EchoTime*PVM_EncCentralStep1;
  for(i=1; i<PVM_NEchoImages; i++)
    EffectiveTE[i] =  EffectiveTE[0] + i * PVM_RareFactor * PVM_EchoTime;

}
