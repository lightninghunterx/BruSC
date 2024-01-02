/*****************************************************************
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

#define DEBUG           0
#define DB_MODULE       0
#define DB_LINE_NR      0

#include "method.h"


void backbone(void)
{
  double minFov[3];
  double minSliceThick;

  DB_MSG(("-->backbone"));

  /* update nuclei parameter group                            */
  STB_UpdateNuclei(No);

  /* update encoding parameter group                          */
  STB_UpdateEncoding( &SegmentSize, SEG_INTERLEAVED, Yes, Yes, Yes, &PVM_EchoPosition, 3);

  /* update parameters controlling the data sampling period   */
  MRT_UpdateDigPars(&PVM_EffSWh, PVM_EncMatrix[0], &PVM_AntiAlias[0], &PVM_AcquisitionTime);

  /* update excitation pulse                                  */
  UpdateRFPulses();

  /* Fix delay 20us will be D[9] in ppg                       */
  FixDelay_1 = 0.002;

  /* general features of the method */
  PVM_NEchoImages = 1;

  /* set limits for read, phase and slice gradients           */
  ControlGradientLimits(PVM_MajSliceOri);

  /* calculate minima for FOV and slice thickness             */
  UpdateGeometryMinima(minFov, &minSliceThick);

  /* update geometry parameters                               */
  int dim=PTB_GetSpatDim();

  /* no restriction to number of slice packages               */
  int maxPackages=0;

  /* only one slice per package if 3D                         */
  int maxPerPackage = dim>2? 1:0;
  if (dim==3)
    ParxRelsMakeNonEditable("PVM_SPackArrNSlices");
  else
    ParxRelsMakeEditable("PVM_SPackArrNSlices");

  STB_UpdateImageGeometry(dim, PVM_Matrix, minFov, 0, /*total slices (no restr)*/
                          maxPackages, maxPerPackage, minSliceThick, 1.0 /* sliceFovRatio in 3D */);

  std::vector<double> spatres=STB_GetSpatResol();
  STB_AtsUpdate(&spatres);

  UpdateSegmentSliceOrder();

  /* update slice spoiler                                     */
  double mindurSlice = 1.5*CFG_GradientRiseTime();
  double spoilerThick = dim>2 ? PVM_SpatResol[2]*PVM_EncZf[2] : PVM_SliceThick;

  MRT_UpdateSpoiler("SliceSpoiler",2.0,ExcSliceGradLim,mindurSlice,PVM_GradCalConst,spoilerThick);

  /* handling of modules                                      */
  STB_UpdateDriftCompModule(spoilerThick, PVM_DeriveGains);
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, spoilerThick);
  STB_UpdateSatTransModule(PVM_DeriveGains);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  STB_UpdateFlowSaturationModule(PVM_Nucleus1,PVM_DeriveGains);
  STB_UpdateTaggingModule(dim,PVM_Fov,PVM_Matrix,PVM_SpatResol[0]*PVM_EncZf[0],PVM_DeriveGains);
  STB_UpdateEvolutionModule();
  UpdateTriggerModule();
  UpdateRFSpoiling();

  /* Calculate read and slice gradients                       */
  UpdateReadSliceGradients();

  /* Sequence elements, TE, TR, total time:                   */
  UpdateSequenceTiming();

  /* DummyScans are not handled via DS in this method!        */
  STB_UpdateDummyScans(SegmentRepTime);

  /* calculate dephasing and phase-encoding gradients         */
  UpdatePhaseGradients();

  /* calculate frequency offsets                              */
  UpdateFrequencyOffsets();

  /* update mapshim parameter class                           */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  /* Set ACQP parameters                                      */
  SetBaseLevelParam();

  /* Set RECO parameters                                      */
  SetRecoParam();

  DB_MSG(("<--backbone"));
  return;
}

/*-------------------------------------------------------
 * local utility routines to simplify the backbone
 *------------------------------------------------------      */


void UpdateSequenceTiming()
/* -------------------------------------------------------
   Adapt sequence elements to the current geometry
   (in this method, only EncGradDur is concerned),
   update TE and TR.
   ReadGrad and ExSliceGrad must be already set.
   ------------------------------------------------------     */
{
  /* Part of the exctiation pulse to be refocused:            */
  EffPulseDur = ExcPulse1.Length * (ExcPulse1.Rpfac/100);

  /* Part of the echo to be refocused                         */
  EchoDelay = PVM_AcquisitionTime * PVM_EchoPosition / 100;

  /* Update mimimum EncGradDur                                */
  UpdateEncodingDuration();

  FlowCompMinEncDur(&EncGradDur, EffPulseDur, EchoDelay,
                     ReadGrad, ExcSliceGrad,
                     ReadDephGradLim, Phase2DGradLim, ExcSliceRephGradLim);

  /* Update TE with the mimimum EncGradDur                    */
  UpdateEchoTime();

  UpdateFlowComp(EncGradDur, EffPulseDur, EchoDelay, ReadGrad, ExcSliceGrad);

  RewGradDur = EncGradDur/2;  // this is sufficient for equal-lobe flow comp

  /* Update Echo Repetition Time                              */
  UpdateEchoRepTime();

  /* Update time from AcqTrigger ot k-space-zero              */
  UpdateModulesTimeToKzero();

  /* Update time between movie frames                         */
  UpdateFrameRepTime();

  /* Update Segment Duration                                  */
  UpdateSegmentDuration();

  /* Update everything for Inversion                          */
  UpdateInversion();

  /* Update Segment Repetition Time                           */
  UpdateSegmentRepTime();

  /* Set correct DriftCompUpdateTime after TR-upate           */
  STB_UpdateDriftComp( SegmentRepTime );

  /* PCISW-1474: method's specialty: In SegmentSliceMode=Serial dummy scans 
   are done for each slice. This cannot be realized by our common dummy handling.  
   Thus, "empty" acqusitions are interspersed. During these "empty"
   job0-acquisition no acquisition should be done on other jobs as well
   (problems in setup-mode with transactionblocks).
  */
  if ( SegmentSliceOrder==Serial && PVM_DriftCompYesNo==Yes)
  {

    double maxUpdateTime = SegmentRepTime * (PVM_EncGenTotalSteps/PVM_EncGenLoopOuter/SegmentSize);
    int nReloads = floor(  maxUpdateTime / PVM_DriftCompUpdateTime);

    PVM_DriftCompUpdateTime = MIN_OF( maxUpdateTime, maxUpdateTime / nReloads);

    //In DriftComp.mod first reload is done then the next navigator is collected.
    //The last navigator of a slice is troublesome:

    if (PVM_DriftCompNAverages > 1)
    {
     PVM_DriftCompNSkip += 1;
     PVM_DriftCompNAverages -= 1;
    }
  }

  /* Calculate total experiment time                          */
  UpdateTotalTime();
}


void UpdateGeometryMinima(double *minFov, double *minSliceThick)
{
  DB_MSG(("-->UpdateGeometryMinima"));

  int dim = PTB_GetSpatDim();

  minFov[0]      = PVM_EffSWh / (1e-2*ReadGradLim * PVM_GradCalConst);
  *minSliceThick = ExcPulse1.Bandwidth / (1e-2*ExcSliceGradLim * PVM_GradCalConst);

  if(dim >= 2)
  {
    if(FlowCompOnOff == Off)
      minFov[1] = minFov[0]/8;
    else
      minFov[1] = minFov[0]/4;
  }

  if(dim >= 3)
  {
    minFov[2] = *minSliceThick;
  }

  DB_MSG(("<--UpdateGeometryMinima"));
}


void UpdateSegmentSliceOrder(void)
{
  /* Handle SegmentSliceOrder:
     For only one slice fall back to default: Alternate. Adapt editibility   */

  int    nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices);
  if (nSlices > 1)
  {
    ParxRelsMakeEditable("SegmentSliceOrder");
  }
  else
  {
    SegmentSliceOrder=Alternate;
    ParxRelsMakeNonEditable("SegmentSliceOrder");
  }
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

  /* Calculation of phase-encoding, dephasing and rephasing gradients.
     (ReadGrad, ExcSliceGrad, EchoDelay, EffPulseDur, and EncGradDur
     must be calculated before)       */

    double rise = CFG_GradientRiseTime();
    double gcc  = PVM_GradCalConst;
    int    dim  = PTB_GetSpatDim();

    ReadDephGrad =     MRT_DephaseGrad(EncGradDur, EchoDelay,   rise, ReadGrad);

    ExcSliceRephGrad = MRT_DephaseGrad(EncGradDur, EffPulseDur, rise, ExcSliceGrad);

    Phase2DGrad = dim > 1 ?
      MRT_PhaseGrad(EncGradDur, PVM_Matrix[1], PVM_Fov[1], gcc) : 0.0;

    Phase3DGrad = dim == 3 ?
      MRT_PhaseGrad(EncGradDur, PVM_Matrix[2], PVM_Fov[2], gcc) : 0.0;

    Rew2DGrad = dim > 1 ?
      MRT_PhaseGrad(RewGradDur, PVM_Matrix[1], PVM_Fov[1], gcc) : 0.0;

    Rew3DGrad = dim == 3 ?
      MRT_PhaseGrad(RewGradDur, PVM_Matrix[2], PVM_Fov[2], gcc) : 0.0;

  DB_MSG(("<--UpdatePhaseGradients"));
  return;
}

void UpdateFrequencyOffsets( void )
{
  int spatDim = PTB_GetSpatDim();
  int nslices = GTB_NumberOfSlices(PVM_NSPacks,PVM_SPackArrNSlices);

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
    for(int i=0;i<nslices;i++)
      PVM_EffPhase2Offset[i] = -PVM_EffSliceOffset[i];
  }
}


/*--------------------------------------------------------
 * Routine to update RF pulse parameters
 *-------------------------------------------------------*/

void UpdateRFPulses(void)
{

  /* Updates all parameters that belong to ExcPulse1 pulse structure
     (as initialized by STB_InitRFPulse see initMeth.c)
  */

  STB_UpdateRFPulse("ExcPulse1",1,PVM_DeriveGains,Conventional);

  if(PVM_DeriveGains==Yes)
  {
    ParxRelsHideInEditor("ExcPulse1Ampl");
  }
  else
  {
    ParxRelsShowInEditor("ExcPulse1Ampl");
  }

  ParxRelsShowInFile("ExcPulse1Ampl");

  DB_MSG(("<--UpdateRFPulses"));

  return;
}

/*--------------------------------------------------------
 * Routine to control max gradient amplitudes
 *-------------------------------------------------------*/

void ControlGradientLimits(YesNo NotOblique)
{
  DB_MSG(("-->ControlGradientLimits: Obliqueness forbidden: %s",NotOblique==Yes ? "Yes":"No"));


  if(NotOblique==Yes)
  {
    ReadDephGradLim     =
    Phase2DGradLim      =
    Phase3DGradLim      =
    ExcSliceRephGradLim = 100.0;
  }
  else
  {
    /* Gradient limits in % of max. Value 57 (1/sqrt(3))
       is needed for arbitrary oblique slices. */
    ReadDephGradLim     =
    Phase2DGradLim      =
    Phase3DGradLim      =
    ExcSliceRephGradLim = 57.0;
  }

  ReadGradLim        = 100.0;
  ExcSliceGradLim    = 100.0;

  DB_MSG(("-->ControlGradientLimits"));
}

void UpdateEncodingDuration( void )
{
  DB_MSG(("-->UpdateEncodingDuration"));

  double minEnc2d, minEnc3d, minRephSlice, minDephRead;
  double gcc    = PVM_GradCalConst;
  double riseT  = CFG_GradientRiseTime();
  double *resol = PVM_SpatResol;
  int    dim    = PTB_GetSpatDim();

  /* Minimum durations of all phase-gradient pulses */
  minEnc2d = dim>1? MRT_EncodingTime( resol[1], gcc*Phase2DGradLim/100 ) : 0;

  minEnc3d = dim>2? MRT_EncodingTime( resol[2], gcc*Phase3DGradLim/100 ) : 0;

  minRephSlice = MRT_DephaseTime(EffPulseDur,  riseT,
                                 ExcSliceGrad, ExcSliceRephGradLim);

  minDephRead  = MRT_DephaseTime(EchoDelay, riseT,
                                 ReadGrad , ReadDephGradLim);

  /* In this sequence all phase-gradients are simultaneous with duration
     EncGradDur. We set it first to the common (longest) minimum:        */
  EncGradDur = MAX_OF( MAX_OF(minEnc2d ,  minEnc3d),
                       MAX_OF(minRephSlice ,minDephRead ) );

  /* EncGradDur should at least contain one ramp, thus: */
  EncGradDur = RewGradDur = MAX_OF( EncGradDur, riseT );

  DB_MSG(("<--UpdateEncodingDuration"));
}


/* Calculates PVM_MinEchoTime and restrict PVM_EchoTime.
   EffPulseDur, EncGradDur must be set before */
void UpdateEchoTime( void )
{
  DB_MSG(("-->UpdateEchoTime"));

  double riseTime = CFG_GradientRiseTime();
  double rampTime = CFG_GradientRampTime()+CFG_InterGradientWaitTime();
  double gcc      = PVM_GradCalConst;

  double extensionAllowed, extensionPossible, extension;

  PVM_MinEchoTime = EffPulseDur    +   // p0/2: for a symmetric rf-pulse
                    rampTime       +   // d4  : falling ramp of slice grad
                    EncGradDur     +   // d10 : enc. time (ramp+plateau
                    riseTime       +   // d3  : read-on ramp
                    EchoDelay;

  if (FlowCompOnOff == Off)
    PVM_MinEchoTime += rampTime;       // d2 : ramp down encoding

  PVM_EchoTime = MAX_OF(PVM_EchoTime, PVM_MinEchoTime);

  /* If there is some freedom, make EncGradDur longer
     (to avoid unnecessarily strong phase gradients),
     but not longer than allowed by gradient resolution */

  extensionPossible = PVM_EchoTime - PVM_MinEchoTime;

  /* w FlowComp EncGradDur contains 2 lobes w/o FlowComp only one: */
  int encTFac = FlowCompOnOff ? 2 : 1;

  double maxEncGradDur = encTFac * MRT_MaxEncodingTime(PVM_Fov[1], gcc);
  extensionAllowed  = maxEncGradDur - EncGradDur;

  extension = MAX_OF( MIN_OF(extensionPossible, extensionAllowed), 0.0 );

  EncGradDur += extension;

  TeFillDelay = PVM_EchoTime - PVM_MinEchoTime - extension;

  DB_MSG(("<--UpdateEchoTime\n"));
}


void UpdateEchoRepTime( void )
{
  DB_MSG(("-->UpdateEchoRepTime"));

  double minD0  = 0.01  /* ADC_END */ + CFG_InterGradientWaitTime();
  double gcc    = PVM_GradCalConst;
  double resol0 = PVM_SpatResol[0]*PVM_EncZf[0];
  double extensionAllowed, extensionPossible, extension, flowCompFactor, riseT;

  /* Other sequence elements, not involved in TE, e.g. spoilers  */
  double mindurRead =  RewGradDur+CFG_GradientRiseTime();

  MRT_UpdateSpoiler("ReadSpoiler", 2.0, Phase2DGradLim, mindurRead, gcc, resol0);

  MinEchoRepTime = FixDelay_1                     +    /*d9*/
                   SliceSpoiler.dur               +    /*d6*/
                   CFG_GradientRiseTime()         +    /*d3*/
                   ExcPulse1.Length - EffPulseDur +    /*p0*/
                   PVM_EchoTime - EchoDelay       +
                   PVM_AcquisitionTime            +
                   ReadSpoiler.dur                +
                   CFG_GradientRiseTime()         +    /*d3*/
                   minD0;

  //EchoRepTime = MAX_OF(EchoRepTime, MinEchoRepTime);
  EchoRepTime = MinEchoRepTime;

  /*** adapt RewGradDur to desired EchoRepTime ***/
  riseT          = CFG_GradientRiseTime();
  flowCompFactor = FlowCompOnOff == On ? 2 : 1;

  /* 1.) if possible, extend RewGradDur to duration of read spoiler */
  extensionAllowed  = EncGradDur - RewGradDur;
  extensionPossible = ReadSpoiler.dur-riseT - RewGradDur;
  extension         = MAX_OF( MIN_OF(extensionPossible, extensionAllowed),0 );

  if(extension > 0)
  {
    RewGradDur += extension;
    extensionAllowed = EncGradDur/flowCompFactor - RewGradDur;
  }

  /* 2.) if possible, extend RewGradDur to EncGradDur  */
  extensionPossible = (EchoRepTime - MinEchoRepTime);
  extension = MAX_OF(MIN_OF(extensionPossible, extensionAllowed),0);
  if(extension > 0)
  {
    RewGradDur += extension;
    MRT_UpdateSpoiler("ReadSpoiler", 2.0, Phase2DGradLim, RewGradDur+riseT, gcc, resol0);
    MinEchoRepTime += extension;
  }

  DB_MSG(("<--UpdateEchoRepTime\n"));
}

void UpdateModulesTimeToKzero(void)
{
  DB_MSG(("-->UpdateModulesTimeToKzero"));

  ModulesTimeToKzero = AcqTriggerModuleTime    +
                       PVM_TaggingModuleTime   +
                       PVM_InFlowSatModuleTime +
                       PVM_FovSatModuleTime    +
                       PVM_SatTransModuleTime  +
                       PVM_FatSupModuleTime    +

                       FixDelay_1              + /* d9, freqRx */
                       SliceSpoiler.dur        + /* d6         */
                       CFG_GradientRiseTime()  +
                       0.5*ExcPulse1.Length    +
                       EchoRepTime*(PVM_EncCentralStep1 - 1);


  DB_MSG(("   Updated ModulesTimeToKzero to %f", ModulesTimeToKzero));

  DB_MSG(("<--UpdateModulesTimeToKzero"));
}

void UpdateFrameRepTime(void)
{
  DB_MSG(("-->UpdateFrameRepTime"));

  if (PVM_NMovieFrames > 1)
  {
    ParxRelsShowInEditor("FrameRepTime");

    double MinFrameRepTime = EchoRepTime * SegmentSize;
    FrameRepTime = MAX_OF(FrameRepTime, MinFrameRepTime);
  }
  else
  {
    ParxRelsHideInEditor("FrameRepTime");
  }

  DB_MSG(("<--UpdateFrameRepTime"));
}

void UpdateSegmentDuration(void)
{
  DB_MSG(("-->UpdateSegmentDuration"));

  SegmentDuration = EchoRepTime * SegmentSize * PVM_NMovieFrames;

  // Delay FrameRepTime after each frame but the last one
  if (PVM_NMovieFrames > 1)
    SegmentDuration += (FrameRepTime - EchoRepTime * SegmentSize) * (PVM_NMovieFrames - 1);

  DB_MSG(("<--UpdateSegmentDuration"));
}


void UpdateSegmentRepTime(void)
{
  DB_MSG(("-->UpdateSegmentRepTime"));

  double rampTime = CFG_GradientRampTime() + CFG_InterGradientWaitTime();
  double gcc      = PVM_GradCalConst;
  double minSegSpoilDur, minDelayDur=0.001;

  if (SegmentSize > 1 || PVM_NMovieFrames > 1)
  {
    minSegSpoilDur = rampTime;
    MRT_UpdateSpoiler("SegSpoiler", 2.0, 30.0, minSegSpoilDur, gcc, 1.0 );
  }
  else
  {
    minSegSpoilDur = 0.01;
    MRT_UpdateSpoiler("SegSpoiler", 0.0, 30.0, minSegSpoilDur, gcc, PVM_SpatResol[0] );
  }

  double modulesDuration = AcqTriggerModuleTime    +
                           InvTriggerModuleTime    +
                           PVM_SelIrModuleTime     +
                           PVM_TaggingModuleTime   +
                           PVM_InFlowSatModuleTime +
                           PVM_FovSatModuleTime    +
                           PVM_SatTransModuleTime  +
                           PVM_FatSupModuleTime    +
                           SegmentDuration         ;

  MinSegmentRepTime =  0.03            +  /* UpdateDynPars */
                       modulesDuration +
                       SegSpoiler.dur  +  /* d13 at grad_ramp{g10,...} */
                       rampTime        ;  /* d4 at grad_off*/

  if (SegmentSliceOrder == Alternate)
    MinSegmentRepTime *= GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  MinSegmentRepTime += PVM_DriftCompModuleTime;

  // minDelayDur needed to avoid some awkward messages for d1=0 and active driftcomp
  SegmentRepTime  = MAX_OF( MinSegmentRepTime + minDelayDur, SegmentRepTime );

  DB_MSG(("<--UpdateSegmentRepTime"));
 }


void UpdateInversion(void)
{
  DB_MSG(("-->UpdateInversion"));

  double SelIrModuleTimePart1 , SelIrModuleTimePart2, MaxSelIrModuleTime;
  int    nSlices     = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices);
  double riseT       = CFG_GradientRiseTime();
  double dummySegDur = 1.0;


  if (InversionOnOff == Off)
  {
    PVM_SelIrOnOff = Off;
    PVM_InversionTime = 0.0;
    ParxRelsHideInEditor("InversionMode");
    ParxRelsHideGroupInEditor("Selective_IR_Parameters");

    /*  STB_UpdateSelIrModule is required also here. It is used to simply switch off UI entries  */
    /*  and to set its  module time to zero. A zero as fixedTime is OK for this                  */
    STB_UpdateSelIrModule(PVM_SliceThick, PVM_SliceOffset, nSlices, &dummySegDur, 0, 0, PVM_DeriveGains);
  }
  else
  {
     PVM_SelIrOnOff = On;
     ParxRelsShowInEditor("InversionMode");
     ParxRelsShowGroupInEditor("Selective_IR_Parameters");

     if (InversionMode == SliceSelInversion)
     {
         ParxRelsShowInEditor("PVM_SelIrSpoiler");
         ParxRelsShowInEditor("PVM_SelIrSlabThick");
         ParxRelsShowInEditor("PVM_SelIrMinThick");
         ParxRelsShowInEditor("PVM_SelIrNSlabs");
     }
     else if (InversionMode == NonSelInversion)
     {
         ParxRelsHideInEditor("PVM_SelIrSpoiler");
         ParxRelsHideInEditor("PVM_SelIrSlabThick");
         ParxRelsHideInEditor("PVM_SelIrMinThick");
         ParxRelsHideInEditor("PVM_SelIrNSlabs");

         /*Non-selective allowed only for SegmentSliceOrder Serial*/
         if (SegmentSliceOrder == Alternate)
           SegmentSliceOrder = Serial;
     }

     /* Use std inversion module to setup parameters. However, a different scheme is used        */
     /* in SegFLASH.mod. After call of STB_UpdateSelIrModule fix differences:                    */
     STB_UpdateSelIrModule(PVM_SliceThick, PVM_EffSliceOffset, nSlices, &dummySegDur, 0, 
                           ModulesTimeToKzero, PVM_DeriveGains);

     SelIrModuleTimePart1 =   0.01                       /* In SegFLASH.mod:*/
                            + riseT                      /* myIrD4   */
                            + 0.5*PVM_SelIrPul.Length;   /* myIrP0/2 */

     SelIrModuleTimePart2 =   0.5*PVM_SelIrPul.Length    /* myIrP0/2 */
                            + PVM_SelIrSpoiler.dur       /* myIrD2   */
                            + riseT;                     /* myIrD4   */

     /*  PVM_SelIrD0  and one riseT do not exist here. Take account of this:                     */
     PVM_InterInvEndDelay[0] = MAX_OF( 0.0, PVM_SelIrInvTime - ModulesTimeToKzero - SelIrModuleTimePart2 );
     PVM_InterInvDelay       = 0.0;
     PVM_SelIrModuleTime     = SelIrModuleTimePart1 + PVM_SelIrInvTime - ModulesTimeToKzero ;

     MaxSelIrModuleTime = SegmentRepTime/nSlices - SegmentDuration;

     if( PVM_SelIrModuleTime > MaxSelIrModuleTime )
       SegmentRepTime = ( MaxSelIrModuleTime  + SegmentDuration) * nSlices;

     ParxRelsCopyPar("PVM_InversionTime","PVM_SelIrInvTime");
  }

  DB_MSG(("<--UpdateInversion"));
}


void UpdateTotalTime(void)
{
  DB_MSG(("-->UpdateTotalTime"));

  int    nslices   = GTB_NumberOfSlices(PVM_NSPacks,PVM_SPackArrNSlices);
  int    dim       = PTB_GetSpatDim();
  double TotalTime = 0;

  if ( dim > 1 )
    // unfortunately we do not consider dummies in total scan time
    TotalTime = ( SegmentRepTime * (PVM_EncGenTotalSteps/PVM_EncGenLoopOuter / SegmentSize) ) ;

  //SegSlOrd Serial : Consider DriftComp in Total Scan Time!
  //SegSlOrd Alternate: It is part of TR. Already considered in UpdateSegmentRepTime!
  if (SegmentSliceOrder == Serial)
  {
    TotalTime += PVM_DriftCompModuleTime;
    TotalTime *= nslices;
  }

  TotalTime *= PVM_NAverages;

  if ( dim > 2 )
    TotalTime *= PVM_EncGenLoopOuter;

  /* time for one repetition */
  OneRepTime = TotalTime/1000.0;

  TotalTime = TotalTime * PVM_EvolutionCycles + PVM_EvolutionModuleTime;
  TotalTime = TotalTime * PVM_SatTransRepetitions * PVM_NRepetitions;

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<--UpdateTotalTime"));
}

void UpdateRFSpoiling(void)
{
  DB_MSG(("-->UpdateRFSpoiling"));

  if(RFSpoiling==No)
  {
    PARX_change_dims("RFPhaseList",1);
    RFPhaseList[0] = 0;
  }
  else
  {
    int max = PVM_EncGenTotalSteps/PVM_EncGenLoopOuter + PVM_DummyScans;
    int size = MAX_OF(256,max);
    PARX_change_dims("RFPhaseList",size);
    MRT_RFSpoilPhaseList(117,size,RFPhaseList,Yes);
  }

  DB_MSG(("<--UpdateRFSpoiling"));
}

void UpdateTriggerModule(void)
{
  DB_MSG(("-->UpdateTriggerModule"));

  /*  Slightly different handling of AcqTrigger & InvTrigger:   */
  /*  AcqTrigger always visible, InvTrigger only for Inversion  */

  if (TriggerOnOff == Off)
  {
    AcqTrigger = No;
    InvTrigger = No;

    AcqTriggerModuleTime = 0.0;
    InvTriggerModuleTime = 0.0;

    ParxRelsHideGroupInEditor("Acq_Trigger_Parameters");
    ParxRelsHideGroupInEditor("Inv_Trigger_Parameters");
  }
  else
  {
    if (AcqTrigger == Yes)
    {
      ParxRelsShowGroupInEditor("Acq_Trigger_Parameters");
      AcqTriggerModuleTime = AcqTriggerDelay + TriggerDur;
    }
    else
    {
      ParxRelsHideGroupInEditor("Acq_Trigger_Parameters");
      AcqTriggerModuleTime=0.0;
    }

    if (InvTrigger == Yes)
    {
      ParxRelsShowGroupInEditor("Inv_Trigger_Parameters");
      InvTriggerModuleTime = InvTriggerDelay + TriggerDur;
    }
    else
    {
      ParxRelsHideGroupInEditor("Inv_Trigger_Parameters");
      InvTriggerModuleTime=0.0;
    }
  }

  if (InversionOnOff==On)
  {
    ParxRelsShowInEditor("InvTrigger");

    if (InvTrigger == Yes)
      ParxRelsShowGroupInEditor("Inv_Trigger_Parameters");
  }
  else
  {
    InvTrigger = No;
    InvTriggerModuleTime = 0;
    ParxRelsHideInEditor("InvTrigger");
    ParxRelsHideGroupInEditor("Inv_Trigger_Parameters");
  }

  DB_MSG(("<--UpdateTriggerModule"));
}

