/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2020
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

static double minFlowEncDur(double dim,
                            double pulseDel,  // time from pulse centre to
                                              // onset of falling slice ramp
                            double readGrad,
                            double sliceGrad,
                            const double *resol);

static void findLobes( double encDur, double pulseDel,
                       double   *l1r, double     *l2r,
                       double   *l1p, double     *l2p,
                       double   *l1s, double     *l2s );

void backbone(void)
{
  double minFov[3];
  double minSliceThick;

  DB_MSG(("-->backbone"));

  /* update nuclei parameter group                            */
  STB_UpdateNuclei(No);

  /* update encoding parameter group                          */
  STB_UpdateEncoding(
    NULL,
    SEG_SEQUENTIAL,
    Yes,
    Yes,
    Yes,
    &PVM_EchoPosition,
    3);

  /* update parameters controlling the data sampling period   */
  MRT_UpdateDigPars( &PVM_EffSWh,
                      PVM_EncMatrix[0],
                     &PVM_AntiAlias[0],
                     &PVM_AcquisitionTime );

  /* update excitation pulse                                  */
  UpdateRFPulses();

  /* general features of the method */
  PVM_NEchoImages = 1;

  /* coexistance of modules: */
  if(PVM_SelIrOnOff == On)
  {
    PVM_BlBloodOnOff = Off;
    PVM_NMovieFrames = 1;
  }

  /* Slice Selective Inversion does not work in AngioMode     */
  if (AngioMode==Yes)
  {
    ParxRelsMakeNonEditable("PVM_SelIrOnOff");

    if(PVM_SelIrOnOff == On)
      PVM_SelIrOnOff = Off;
  }
  else
  {
    ParxRelsMakeEditable("PVM_SelIrOnOff");
  }

  /* set limits for read, phase and slice gradients            */
  ControlGradientLimits(PVM_MajSliceOri);

  /* calculate minima for FOV and slice thickness              */
  UpdateGeometryMinima(minFov, &minSliceThick);

  /* update geometry parameters                                */
  int dim=PTB_GetSpatDim();

  /* only one package if black-blood module on                 */
  int maxPackages = PVM_BlBloodOnOff == On? 1:0;

  /* only one slice per package if 3D                          */
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

  /* update slice spoiler                                      */
  double mindurSlice = 1.5*CFG_GradientRiseTime();
  double spoilerThick = dim>2 ? PVM_SpatResol[2]*PVM_EncZf[2] : PVM_SliceThick;
  MRT_UpdateSpoiler("SliceSpoiler",2.0,ExcSliceGradLim,mindurSlice,PVM_GradCalConst,spoilerThick);

  /* handling of modules                                       */
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, spoilerThick);
  STB_UpdateSatTransModule(PVM_DeriveGains);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  STB_UpdateFlowSaturationModule(PVM_Nucleus1,PVM_DeriveGains);
  STB_UpdateTriggerModule();
  STB_UpdateTaggingModule(dim,PVM_Fov,PVM_Matrix,PVM_SpatResol[0]*PVM_EncZf[0],PVM_DeriveGains);
  STB_UpdateEvolutionModule();

  /* Black Blood Module   */
  {
    GObject slicegeo("PVM_SliceGeo");

    double extents[3],pos[3];
    slicegeo.getAcqFov(extents,0);
    slicegeo.getPosToAcq(pos,0);

    /* Inversion time TI extends up to the centre of the excitation pulse!  *
     * Some delays of the imaging block and module durations have to be     * 
     * considered. Their sum is handed to the updater. Some modules may not *
     * make sense to be combined w inversion or even harm, but are taken    *
     * into acount anyway:                                                  */   
    double fixedTime = PVM_TaggingModuleTime
                       + 0.03                        /* UpdateDynPars */
                       + 0.01                        /* freqRx */
                       + PVM_InFlowSatModuleTime
                       + PVM_FovSatModuleTime
                       + PVM_SatTransModuleTime
                       + PVM_FatSupModuleTime
                       + SliceSpoiler.dur            /* d6   */
                       + CFG_GradientRiseTime()      /* d3   */
                       + ExcPulse1.Length/2.0;       /* p0/2 */

    /* for trigger active there's either per_Slice or per_PhaseStep: consider only one */
    fixedTime += PVM_TriggerModuleTime;

    STB_UpdateBlBloodModule( &extents[2], &pos[2], 1, fixedTime,
                             PVM_SliceThick, PVM_DeriveGains);
  }

  UpdateMovie();
  UpdateRFSpoiling();

  /* Calculate read and slice gradients */
  UpdateReadSliceGradients();

  /* Sequence elements, TE, TR, total time: */
  UpdateSequenceTiming();

  /* Dummy Scans                                              */
  STB_UpdateDummyScans(PVM_RepetitionTime);

  /* calculate dephasing and phase-encoding gradients         */
  UpdatePhaseGradients();

  /* calculate frequency offsets                              */
  UpdateFrequencyOffsets();

  /* update mapshim parameter group */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  /* update startup shim parameter group */
  STB_UpdateStartupShims();

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
   ReadGrad and ExcSliceGrad must be already set.
   ------------------------------------------------------*/
{
  DB_MSG(("-->UpdateSequenceTiming"));

  double extension, extensionPossible, extensionAllowed;
  double gcc     = PVM_GradCalConst;
  int    nSlices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);
  int    dim     = PTB_GetSpatDim();

  /* Part of the exctiation pulse to be refocused: */
  EffPulseDur = ExcPulse1.Length * (ExcPulse1.Rpfac/100);

  /* Part of the echo to be refocused */
  EchoDelay = PVM_AcquisitionTime * PVM_EchoPosition / 100;


  /* EncGradDur is the duration of simult. flow compensated
     read, phase and slice gradients. We set it first to minimum: */
  EncGradDur = minFlowEncDur(PTB_GetSpatDim(),
                             EffPulseDur,
                             ReadGrad,
                             ExcSliceGrad,
                             PVM_SpatResol);

  DB_MSG(("   minimum EncGradDur = %f", EncGradDur));

  /* Update TE with this EncGradDur */
  UpdateEchoTime();


  /* If there is some freedom, make EncGradDur longer
     (to avoid unnecessarily strong phase gradients),
     but not longer than allowed by gradient resolution */
  extensionPossible = PVM_EchoTime - PVM_MinEchoTime;

  // 2*MRT_MaxEncodingTime because of 2 shapes
  double MaxEncGradDur = 2*MRT_MaxEncodingTime(PVM_Fov[1], gcc);
  extensionAllowed     = dim >1 ? MaxEncGradDur - EncGradDur: 20;
 
  extension = MAX_OF( MIN_OF(extensionPossible, extensionAllowed), 0.0) ;

  EncGradDur += extension;

  TeFillDelay = PVM_EchoTime - PVM_MinEchoTime - extension;

  RewindGradDur = EncGradDur/2;  // this is sufficient for equal-lobe flow comp

  /* Other sequence elements, not involved in TE, e.g. spoilers  */
  double mindurRead =  RewindGradDur+CFG_GradientRiseTime();
  MRT_UpdateSpoiler( "ReadSpoiler", 2.0, Phase2DGradLim, mindurRead,
                     gcc, PVM_SpatResol[0]*PVM_EncZf[0] );

  /* Update module SelIr */
  MinSliceSegDur = minLoopRepetitionTime();
  /* SliceSegDur may be changed by the UpdateSelIrModule! */
  SliceSegDur = MinSliceSegDur;

  /* Inversion time TI extends up to the centre of the excitation pulse!         *
   * some delays of the imaging block and module durations have to be considered.*
   * Their sum is handed to the updater. Some modules may not make sense to be   *
   * combined w inversion or even harm but are taken into acount anyway:         */
  double fixedTime = PVM_TaggingModuleTime
                     + 0.03                        /* UpdateDynPars */
                     + 0.01                        /* freqRx */
                     + PVM_InFlowSatModuleTime
                     + PVM_FovSatModuleTime
                     + PVM_SatTransModuleTime
                     + PVM_FatSupModuleTime
                     + SliceSpoiler.dur           /* d6   */
                     + CFG_GradientRiseTime()     /* d3   */
                     + ExcPulse1.Length/2.0;      /* p0/2 */

  /* consider trigger event within slice-loop only if mode is per_Slice: *
     (no SelIr in angio-mode!)                                           */
  if (PVM_TriggerMode == per_Slice)
    fixedTime += PVM_TriggerModuleTime;

  STB_UpdateSelIrModule( PVM_SliceThick, PVM_EffSliceOffset, nSlices, &SliceSegDur,
                         0, fixedTime, PVM_DeriveGains );

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

  /* Calculate total experiment time */
  UpdateTotalTime();

  DB_MSG(("<--UpdateSequenceTiming"));
}


void UpdateRepetitionTime(void)
{
  int nSlices;

  DB_MSG(("-->UpdateRepetitionTime"));

  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  if(AngioMode == No)
  {
    if(PVM_SelIrOnOff == On)
    {
      PVM_MinRepetitionTime = PVM_SelIrModuleTime
                              + PVM_TaggingModuleTime
                              + nSlices * SliceSegDur ;
    }
    else
    {
      /* min TR in a movie: */
      PVM_MinRepetitionTime = nSlices * minLoopRepetitionTime();

      /* if there is no movie, TR also includes some modules: */
      if(PVM_NMovieFrames == 1)
      {
        PVM_MinRepetitionTime +=  PVM_BlBloodModuleTime
                                + PVM_SelIrModuleTime
                                + PVM_TaggingModuleTime ;
      }
    }

    if (PVM_NMovieFrames == 1 && PVM_TriggerMode == per_PhaseStep)
    {
          PVM_MinRepetitionTime += PVM_TriggerModuleTime;

          //Additional trigger_per_phase event if BlBlood is active
          if (PVM_BlBloodOnOff)
            PVM_MinRepetitionTime += PVM_TriggerModuleTime;
    }
  }
  else
    PVM_MinRepetitionTime = minLoopRepetitionTime();

  PVM_RepetitionTime=MAX_OF( PVM_MinRepetitionTime, PVM_RepetitionTime );

  DB_MSG(("<--UpdateRepetitionTime"));
  return;
}


/* calculates PVM_ScanTimeStr and TimeForMovieFrames */

void UpdateTotalTime(void)
{
  DB_MSG(("-->UpdateTotalTime"));

  int nSlices      = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  int dim          = PTB_GetSpatDim();
  double TotalTime = 0;

  if( dim >1 )
  {
    if(PVM_NMovieFrames > 1)
    {
      /* TR is one movie frame, without prep modules: */
      TimeForMovieFrames = PVM_RepetitionTime * PVM_NMovieFrames + PVM_TaggingModuleTime;
      TotalTime = ( PVM_BlBloodModuleTime + TimeForMovieFrames ) *
                    PVM_EncGenTotalSteps * PVM_NAverages;
    }
    else
    {
      if(AngioMode==No)
      {
        /* TR includes prep modules */
        TimeForMovieFrames = PVM_RepetitionTime - PVM_BlBloodModuleTime - PVM_TaggingModuleTime;
        TotalTime = PVM_RepetitionTime * PVM_EncGenTotalSteps * PVM_NAverages;
      }
      else
      {
        TotalTime = PVM_RepetitionTime * PVM_EncGenTotalSteps/PVM_EncGenLoopOuter
                    + PVM_BlBloodModuleTime
                    + PVM_TaggingModuleTime;

        if (PVM_TriggerMode == per_Slice)
          TotalTime += (PVM_BlBloodOnOff == On) ? 2*PVM_TriggerModuleTime : PVM_TriggerModuleTime;

        TotalTime*= nSlices*PVM_NAverages;
        if( dim >2 )
          TotalTime *= PVM_EncGenLoopOuter;         
      }
    }
  }

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

void UpdateGeometryMinima(double *minFov, double *minSliceThick)
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
    minFov[1] = minFov[0]/4;
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

  /* Calculation of phase-encoding,dephasing and rephasing gradients. *
   * (ReadGrad, ExcSliceGrad, EchoDelay, EffPulseDur, EncGradDur and  *
   * RewindGradDur must be calculated before)                         */

  double rise          = CFG_GradientRiseTime();
  double echoPos       = PVM_AcquisitionTime*PVM_EchoPosition/100.;
  double gcc           = PVM_GradCalConst;
  int    dim           = PTB_GetSpatDim();
  double phaseIntegral = 0.0;
  double l1r=0.0, l2r=0.0, l1s=0.0, l2s=0.0, l1p=0.0, l2p=0.0;// lobe durations on r/p/s

  Phase2DGrad = dim > 1 ?
    MRT_PhaseGrad(RewindGradDur, PVM_Matrix[1], PVM_Fov[1], gcc) : 0.0;

  Phase3DGrad = dim == 3 ?
    MRT_PhaseGrad(RewindGradDur, PVM_Matrix[2], PVM_Fov[2], gcc) : 0.0;

  findLobes(EncGradDur, EffPulseDur, &l1r, &l2r, &l1p, &l2p, &l1s, &l2s);

  MRT_FlowCompGradRead (rise, l1r, l2r, echoPos,     ReadGrad,     ReadFcGrad);
  MRT_FlowCompGradSlice(rise, l1s, l2s, EffPulseDur, ExcSliceGrad, ExcSliceFcGrad);

  if(dim>1)
  {
    phaseIntegral = 1e5/(2*gcc*PVM_SpatResol[1]);
    MRT_FlowCompGradPhase(rise, l1p, l2p, phaseIntegral, Phase2DFcGrad);
  }
  else
  {
    memset( Phase2DFcGrad,0,2*sizeof(double));
  }

  if(dim>2)
  {
    phaseIntegral = 1e5/(2*gcc*PVM_SpatResol[2]);
    MRT_FlowCompGradPhase(rise, l1p, l2p, phaseIntegral, Phase3DFcGrad);
  }
  else
  {
    memset( Phase3DFcGrad,0,2*sizeof(double));
  }

  // encoding shapes
  double gradDwell = MAX_OF(CFG_GradientShapeResolution(),0.002);
  int    shpsze    = (int) ( EncGradDur/gradDwell );


  if ( TeFillDelay > EncGradDur )  //long enough to place 2 shapes
  {
    ShapeSize = shpsze;

    RdVar = EncGradDur - gradDwell*ShapeSize;

    TeFillDelay -= EncGradDur ;

    LongTEScan = Yes;
  }
  else
  {
    ShapeSize  = floor ( (EncGradDur + TeFillDelay) / gradDwell );

    RdVar = EncGradDur + TeFillDelay - gradDwell*ShapeSize;

    LongTEScan = No;
  }


  if(shpsze<10)
  {
    char msg[200];
    sprintf(msg, "FcFLASH: min grad dwell too high (%f us)", 1000*gradDwell);
    UT_ReportError(msg);
  }


  PARX_change_dims("RShape",  ShapeSize);
  PARX_change_dims("P2Shape", ShapeSize);
  PARX_change_dims("P3Shape", ShapeSize);
  PARX_change_dims("SShape",  ShapeSize);

  memset(RShape,  0.0, ShapeSize*sizeof(double));
  memset(P2Shape, 0.0, ShapeSize*sizeof(double));
  memset(P3Shape, 0.0, ShapeSize*sizeof(double));
  memset(SShape,  0.0, ShapeSize*sizeof(double));

  double rshape[shpsze] = {0.}, p2shape[shpsze] = {0.};
  double sshape[shpsze] = {0.}, p3shape[shpsze] = {0.};

  makeShape( rshape, shpsze, rise, l1r, l2r, ReadFcGrad[0]/100.,     ReadFcGrad[1]/100.     );
  makeShape(p2shape, shpsze, rise, l1p, l2p, Phase2DFcGrad[0]/100.,  Phase2DFcGrad[1]/100.  );
  makeShape(p3shape, shpsze, rise, l1p, l2p, Phase3DFcGrad[0]/100.,  Phase3DFcGrad[1]/100.  );
  makeShape( sshape, shpsze, rise, l1s, l2s, ExcSliceFcGrad[0]/100., ExcSliceFcGrad[1]/100. );

  std::copy( sshape,  sshape + shpsze, SShape);
  std::copy(p2shape, p2shape + shpsze, P2Shape + ShapeSize - shpsze);
  std::copy(p3shape, p3shape + shpsze, P3Shape + ShapeSize - shpsze);
  std::copy( rshape,  rshape + shpsze,  RShape + ShapeSize - shpsze);


  DB_MSG(("<--UpdatePhaseGradients"));

  return;
}


void makeShape(double *buf, int n, double ramp, double lobe1, double lobe2,
               double amp1, double amp2)
{
  double t1, t2, t3, t4, t5, tot = lobe1+lobe2;

  t1 = ramp;
  t2 = lobe1-ramp;
  t3 = lobe1;
  t4 = lobe1+ramp;
  t5 = tot-ramp;

  MRT_MakeRamp(buf, n, tot, 0,  t1,   0,     amp1, ramp_lin);
  MRT_MakeRamp(buf, n, tot, t1, t2,   amp1,  amp1, ramp_lin);
  MRT_MakeRamp(buf, n, tot, t2, t3,   amp1,  0,    ramp_lin);
  MRT_MakeRamp(buf, n, tot, t3, t4,   0,     amp2, ramp_lin);
  MRT_MakeRamp(buf, n, tot, t4, t5,   amp2,  amp2, ramp_lin);
  MRT_MakeRamp(buf, n, tot, t5, tot,  amp2,  0,    ramp_lin);
}


void UpdateFrequencyOffsets( void )
{
 DB_MSG(("-->UpdateFrequencyOffsets"));
  int spatDim;
  int i,nslices;

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

void UpdateRFPulses(void)
{

  /* Updates all parameters that belong to ExcPulse1 pulse structure
     (as initialized by STB_InitRFPulse see initMeth.c)
  */
  DB_MSG(("-->UpdateRFPulses"));

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
 * Routine to control the visibility of parameters
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

void UpdateMovie(void)
{
  DB_MSG(("-->UpdateMovie"));

  if(AngioMode==Yes)
    PVM_NMovieFrames=1;

  if(PVM_NMovieFrames == 1)
  {
    ParxRelsHideInEditor("TimeForMovieFrames");
  }
  else
  {
    ParxRelsShowInEditor("TimeForMovieFrames");
  }

  DB_MSG(("<--UpdateMovie"));
}




/* Calculates PVM_MinEchoTime and restrict PVM_EchoTIme.
   EffPulseDur, EncGradDur must be set before */
void UpdateEchoTime( void )
{

  double riseTime = CFG_GradientRiseTime(),
    rampTime = CFG_GradientRampTime()+CFG_InterGradientWaitTime();

  DB_MSG(("-->UpdateEchoTime\n"));

  PVM_MinEchoTime =
    EffPulseDur    +
    rampTime       + //falling ramp of slice grad
    EncGradDur     + //enc. time (two lobes including ramps)
    riseTime       + //read-on ramp
    EchoDelay;


  PVM_EchoTime = MAX_OF(PVM_EchoTime, PVM_MinEchoTime);

  DB_MSG(("<--echoTimeRels\n"));
}



double minLoopRepetitionTime(void)
/* ---------------------------------------------------------
this function returns the minimum duration of the innermost
pulse program loop
----------------------------------------------------------*/
{
  double minTr, minD0,
    riseTime = CFG_GradientRiseTime(),
    igwt = CFG_InterGradientWaitTime();

  minD0 = 0.01  /* ADC_END */ + igwt;

  minTr = 0.030                          + /* UpdateDynPars */
          0.010                          + /* freqRx */
          PVM_InFlowSatModuleTime        +
          PVM_FovSatModuleTime           +
          PVM_SatTransModuleTime         +
          PVM_FatSupModuleTime           +
          SliceSpoiler.dur               + /* d6 */
          riseTime                       + /* d3 */
          ExcPulse1.Length - EffPulseDur +
          PVM_EchoTime - EchoDelay       +
          PVM_AcquisitionTime            + /* AQ_job(0) */
          ReadSpoiler.dur                + /* d11 + d12 */
          riseTime                       + /* d3  */
          minD0;                           /* d0  */

  if ( !AngioMode && PVM_TriggerMode == per_Slice )
    minTr += PVM_TriggerModuleTime;

  /* angio-mode swaps slice & phase-encoding order */
  if ( AngioMode && PVM_TriggerMode == per_PhaseStep )
    minTr += PVM_TriggerModuleTime;

  return minTr;
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
    int max = PVM_EncGenTotalSteps+PVM_DummyScans;
    int size = MAX_OF(256,max);
    PARX_change_dims("RFPhaseList",size);
    MRT_RFSpoilPhaseList(117,size,RFPhaseList,Yes);
  }

  DB_MSG(("<--UpdateRFSpoiling"));
}


static double minFlowEncDur(double dim,
                            double pulseDel,  // time from pulse centre to
                                              // onset of falling slice ramp
                            double readGrad,
                            double sliceGrad,
                            const double *resol )
/* -------------------------------------------------
   min duration of encoding, read-deph and slice rephasing
   (flow compensated) counted from onset of first enc ramp
   to onset of last enc ramps, assuming equal durations of
   both lobes. PVM_AcquisitionTime and PVM_EchoPosition must
   be set before.
   -------------------------------------------------*/
{
  DB_MSG(("-->minFlowEncDur"));

  double rise          = CFG_GradientRiseTime();
  double echoPos       = PVM_AcquisitionTime*PVM_EchoPosition/100.0;
  double gcc           = PVM_GradCalConst;
  double encDur        = 4*CFG_GradientRiseTime(); // initial value
  double increment     = 0.1;
  int i, grad_exceeded = 0;
  double phaseIntegral = 0.0;
  double l1r, l2r, l1s, l2s, l1p, l2p; // lobe durations on r/p/s
  double gval[2], gvalSl[2];

  encDur -= increment;

  do
  {
    encDur += increment;

    findLobes(encDur, pulseDel, &l1r, &l2r, &l1p, &l2p, &l1s, &l2s);

    grad_exceeded = 0;

    MRT_FlowCompGradRead(rise, l1r, l2r, echoPos, readGrad, gval);

    for(i=0;i<2;i++)
      if(fabs(gval[i])>ReadDephGradLim)
        grad_exceeded = 1;

    MRT_FlowCompGradSlice(rise, l1s, l2s, pulseDel, sliceGrad, gvalSl);

    for(i=0;i<2;i++)
      if(fabs(gvalSl[i])>ExcSliceRephGradLim)
        grad_exceeded = 1;

    phaseIntegral = 1e5 / (2*gcc*resol[1]);

    MRT_FlowCompGradPhase(rise, l1p, l2p, phaseIntegral, gval);

    for(i=0;i<2;i++)
      if(fabs(gval[i])>Phase2DGradLim)
        grad_exceeded = 1;


    if(dim>2)
    {
      phaseIntegral = 1e5 / (2*gcc*resol[2]);

      MRT_FlowCompGradPhase(rise, l1p, l2p, phaseIntegral, gval);

      for(i=0;i<2;i++)
        if(fabs(gvalSl[i]+gval[i])>ExcSliceRephGradLim)
          grad_exceeded = 1;
    }

  } while (grad_exceeded && encDur<200);


  DB_MSG(("<-- minFlowEncDur"));

  return encDur;
}

static void findLobes(double encDur, double pulseDel,
                      double *l1r,   double *l2r,
                      double *l1p,   double *l2p,
                      double *l1s,   double *l2s)
{
  // divide encDur to lobes (r,p,s) in the optimal, feasible way

  double rise   = CFG_GradientRiseTime();
  double echoPos = PVM_AcquisitionTime*PVM_EchoPosition/100.0;

  MRT_FlowCompTimingRead (rise, encDur, echoPos, l1r, l2r);
  MRT_FlowCompTimingPhase(rise, encDur, l1p, l2p);
  MRT_FlowCompTimingSlice(rise, encDur, pulseDel, l1s, l2s);

  *l2r = encDur - *l1r;
  *l2p = encDur - *l1p;
  *l2s = encDur - *l1s;
}
