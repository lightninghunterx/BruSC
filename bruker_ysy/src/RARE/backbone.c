/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 * ***************************************************************/


static const char resid[] = "$Id $ (c) 2007Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
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
    Yes,
    NULL,
    3);


  PVM_EchoPosition = 50;

  /* update parameters controlling the data sampling period   */

  MRT_UpdateDigPars( &PVM_EffSWh,
                      PVM_EncMatrix[0],
                     &PVM_AntiAlias[0],
                     &PVM_AcquisitionTime );

  /* update excitation pulse                                  */

  UpdateRFPulses();


  /* general features of the method */

  PVM_NRepetitions = MAX_OF(1,PVM_NRepetitions);


  /* coexistance of modules: */
  if(PVM_SelIrOnOff == On)
  {
    PVM_BlBloodOnOff = Off;
    PVM_NMovieFrames = 1;
  }

  /* set limits for read, phase and slice gradients            */

  ControlGradientLimits(PVM_MajSliceOri);


  /* calculate minima for FOV and slice thickness             */

  if (PTB_GetSpatDim()<3)
  {
    SlabSel=Yes; //enforce SlabSel for 2D
    ParxRelsMakeNonEditable("SlabSel");
  }
  else
  {
    ParxRelsMakeEditable("SlabSel");
  }

  UpdateGeometryMinima(minFov,
                       &minSliceThick);


  /* update geometry parameters                */
  /* for non slab selective excitation offset along slice direction
     must be zero
   */
  if(SlabSel == No)
    STB_ConstrainSlicePosition(false,false,true);

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
                          minSliceThick,
                          1.0); // sliceFovRatio in 3D

  std::vector<double> spatres=STB_GetSpatResol();
  STB_AtsUpdate(&spatres);

  /* update repetition spoiler */
  double mindur = 5.0*CFG_GradientRiseTime();
  double effSlice = dim>2? PVM_SpatResol[2]*PVM_EncZf[2] : PVM_SliceThick;

  MRT_UpdateSpoiler("RepetitionSpoiler",4.0,60.0,mindur,PVM_GradCalConst,effSlice);

  /* handling of modules */
  STB_UpdateDriftCompModule(effSlice, PVM_DeriveGains);
  if(PVM_SelIrOnOff == On)
  {
    PVM_BlBloodOnOff = Off;
  }
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, effSlice);
  STB_UpdateSatTransModule(PVM_DeriveGains);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  STB_UpdateFlowSaturationModule(PVM_Nucleus1, PVM_DeriveGains);
  STB_UpdateTriggerModule();
  STB_UpdateTaggingModule(dim, PVM_Fov, PVM_Matrix, PVM_SpatResol[0]*PVM_EncZf[0],
                          PVM_DeriveGains);
  STB_UpdateEvolutionModule();
  STB_UpdateFwsModule(PVM_FrqWork[0]);

  /* Black Blood Module   */
  {
    GObject slicegeo("PVM_SliceGeo");

    double extents[3],pos[3];
    slicegeo.getAcqFov(extents,0);
    slicegeo.getPosToAcq(pos,0);

  /* Inversion time TI extends up to the centre of the excitation pulse!
   * Some delays of the imaging block and module durations have to be considered.
   * Their sum is handed to the updater. Some modules may not make sense to be
   * combined w inversion or even harm but are taken into acount anyway: */

    double fixedTime = 0.01                        /* freqRx */
                       + PVM_TriggerModuleTime
                       + PVM_InFlowSatModuleTime
                       + PVM_TaggingModuleTime
                       + PVM_FovSatModuleTime
                       + PVM_SatTransModuleTime
                       + PVM_FatSupModuleTime
                       + RepetitionSpoiler.dur     /* d9: ramp up & plateau repetition spoiler */
                       + CFG_GradientRiseTime()    /* d3: ramp up slice sel gradient */
                       + ExcPulse1.Length/2.0;     /* p0/2 */

    STB_UpdateBlBloodModule( &extents[2], &pos[2], 1, fixedTime,
                             PVM_SliceThick, PVM_DeriveGains );
  }

  /* Calculate read and slice gradients */
  UpdateReadSliceGradients();

  /* Sequence elements, TE, TR, total time, TE-dependent modules: */
  UpdateSequenceTiming();

  /* Set correct DriftCompUpdateTime after TR-upate */
  STB_UpdateDriftComp(PVM_RepetitionTime);

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
  int nSlices;

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

  /* Number and effective echo time of multiple echoes */
  UpdateMultiEchoes();

  /* Update module SelIr */
  MinSliceSegDur = minLoopRepetitionTime();
  /* SliceSegDur may be changed by the UpdateSelIrModule! */
  SliceSegDur = MinSliceSegDur;
  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices);

  /* Inversion time TI extends up to the centre of the excitation pulse!         *
   * Some delays of the imaging block and module durations have to be considered.*
   * Their sum is handed to the updater. Some modules may not make sense to be   *
   * combined w inversion or even harm, but are taken into acount anyway:        */

  double fixedTime = 0.01                        /* freqRx */
                     + PVM_InFlowSatModuleTime
                     + PVM_TaggingModuleTime
                     + PVM_FovSatModuleTime
                     + PVM_SatTransModuleTime
                     + PVM_FatSupModuleTime
                     + RepetitionSpoiler.dur     /* d9   */
                     + CFG_GradientRiseTime()    /* d3   */
                     + ExcPulse1.Length/2.0;     /* p0/2 */

  /* consider trigger event within slice-loop only if mode is per_Slice: */
  if (PVM_TriggerMode == per_Slice)
    fixedTime += PVM_TriggerModuleTime;

  STB_UpdateSelIrModule(PVM_SliceThick, PVM_EffSliceOffset, nSlices, &SliceSegDur, 0,
                        fixedTime, PVM_DeriveGains);

  if(PVM_SelIrOnOff==On)
    ParxRelsCopyPar("PVM_InversionTime","PVM_SelIrInvTime");

  if(PVM_BlBloodOnOff==On)
    ParxRelsCopyPar("PVM_InversionTime","PVM_BlBloodInvTime");

  if(PVM_BlBloodOnOff==Off&&PVM_SelIrOnOff==Off)
  {
    PVM_InversionTime = 0.0;
    ParxRelsHideInEditor("PVM_InversionTime");
  }

  /* Find min TR and update TR */
  UpdateRepetitionTime();

  /* delay after the slice loop, used only in IR mode to control TR: */
  SliceSegEndDelay = PVM_SelIrOnOff==On? PVM_RepetitionTime - PVM_MinRepetitionTime : 0;

  /* Calculate total experiment time */
  UpdateTotalTime();
}




void UpdateRepetitionTime(void)
{
  int nSlices;

  DB_MSG(("-->UpdateRepetitionTime"));

  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  if(PVM_SelIrOnOff == On)
  {
    PVM_MinRepetitionTime = PVM_DriftCompModuleTime +
                            PVM_SelIrModuleTime     +
                            nSlices * SliceSegDur;
  }
  else
  {
    PVM_MinRepetitionTime = PVM_DriftCompModuleTime +
                            PVM_BlBloodModuleTime   +
                            PVM_SelIrModuleTime     +
                            nSlices * minLoopRepetitionTime();
  }

  if (PVM_TriggerMode == per_PhaseStep)
  {
    PVM_MinRepetitionTime += PVM_TriggerModuleTime;

    //There is an additional trigger_per_phase event if BlBlood is active
    if (PVM_BlBloodOnOff)
      PVM_MinRepetitionTime += PVM_TriggerModuleTime;
  }

  PVM_RepetitionTime=MAX_OF(PVM_MinRepetitionTime,PVM_RepetitionTime);

  DB_MSG(("<--UpdateRepetitionTime"));
  return;
}



/* calculates PVM_ScanTimeStr and TimeForMovieFrames */

void UpdateTotalTime(void)
{
  int dim = PTB_GetSpatDim();
  double TotalTime=0;

  if( dim >1 )
  {
      TotalTime = PVM_RepetitionTime * PVM_EncGenTotalSteps/PVM_RareFactor
                  * PVM_NAverages;
  }

  /* time for one repetition */
  OneRepTime = TotalTime/1000.0;

  TotalTime = TotalTime * PVM_EvolutionCycles + PVM_EvolutionModuleTime;
  TotalTime = TotalTime * PVM_SatTransRepetitions * PVM_NRepetitions;
  TotalTime = TotalTime * PVM_FwsLoop;

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

  if (SlabSel==No)
  {
    *minSliceThick=1.0e-3; /* slab thickness not constrained by slice selective
                              excitation */
  }
  else
  {
    *minSliceThick = ExcPulse1.Bandwidth /
      (1e-2*ExcSliceGradLim * PVM_GradCalConst);
  }

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
  if (SlabSel==No)
  {
    ExcSliceGrad=0.0;
  }
  else
  {
    ExcSliceGrad = MRT_SliceGrad(ExcPulse1.Bandwidth,
                                 PVM_SliceThick,
                                 PVM_GradCalConst);
  }

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
    RefPulse1.Length/2.0) * 2.0;

  // minimum imposed by reph-reph period
  *minTe2 =
    RefPulse1.Length +
    2*EncGradDur +
    2*rampTime + // min d6
    2*riseTime +
    PVM_AcquisitionTime;

  // take FWS into account
  if (PVM_FwsOnOff) {
    *minTe1 += 2 * (PVM_FwsEchoShift + 0.02);
    *minTe2 += 2 * (PVM_FwsEchoShift + 0.02);
  }

  PVM_MinEchoTime = MAX_OF(*minTe1,*minTe2);

  PVM_EchoTime = MAX_OF(PVM_EchoTime, PVM_MinEchoTime);

  DB_MSG(("<--UpdateEchoTime\n"));
}



double minLoopRepetitionTime(void)
/* ---------------------------------------------------------
this function returns the minimum duration of an echo train
including some modules: all delays inside slice-loop in
RARE.ppg. EffPulseDur must be known.
----------------------------------------------------------*/
{
  double minTr, minD0,
    riseTime = CFG_GradientRiseTime(),
    igwt = CFG_InterGradientWaitTime(),
    echoTrain;

  minD0 = igwt;

  // time from centre of exc till end of last ramp down
  if(PVM_FlipBackOnOff == On)
    echoTrain =
      PVM_EchoTime*(NEchoes+1) + ExcPulse1.Length/2 + riseTime;
  else
    echoTrain =
      PVM_EchoTime*(NEchoes+0.5) - RefPulse1.Length/2;

  minTr = 0.03                           +    /* UpdateDynPars */
          0.01                           +    /* freqRx():f1 */
          PVM_InFlowSatModuleTime        +
          PVM_TaggingModuleTime          +
          PVM_FovSatModuleTime           +
          PVM_SatTransModuleTime         +
          PVM_FatSupModuleTime           +
          RepetitionSpoiler.dur          +    /* d9   */
          riseTime                       +    /* d3   */
          ExcPulse1.Length - EffPulseDur +    /* p0/2 */
          echoTrain                      +
          minD0;

  if ( PVM_TriggerMode == per_Slice )
    minTr += PVM_TriggerModuleTime;

  return minTr;
}

/* ------------------------------------------------------

   ------------------------------------------------------*/
void  UpdateMultiEchoes(void)
{
  int i;


  PVM_NEchoImages = MAX_OF(1, PVM_NEchoImages);
  PVM_NEchoImages = MIN_OF(8, PVM_NEchoImages);

  NEchoes = PVM_RareFactor * PVM_NEchoImages;

  PARX_change_dims("EffectiveTE", PVM_NEchoImages);
  EffectiveTE[0] =  PVM_EchoTime*PVM_EncCentralStep1;
  for(i=1; i<PVM_NEchoImages; i++)
    EffectiveTE[i] =  EffectiveTE[0] + i * PVM_RareFactor * PVM_EchoTime;

}
