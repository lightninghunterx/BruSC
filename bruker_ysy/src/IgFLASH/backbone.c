/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2011
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
                     double pulseDel,  // time from pulse centre to onset of
                                       // falling slice ramp
                     double readGrad,
                     double sliceGrad,
                     const double *resol);

static void findLobes(double encDur, double pulseDel,
                      double *l1r, double *l2r,
                      double *l1p, double *l2p,
                      double *l1s, double *l2s);


void backbone(void)
{
  double minFov[3];
  double minSliceThick;
  int dim;

  DB_MSG(("-->backbone"));

  /* update nuclei parameter group                            */

  STB_UpdateNuclei(No);

  dim=PTB_GetSpatDim();

  if(PVM_NavPosMode == Nav_in_slice)
  {
    FlowCompYesNo = No;
    ParxRelsMakeNonEditable("FlowCompYesNo");
  }
  else
  {
    ParxRelsMakeEditable("FlowCompYesNo");
  }

  /* update encoding parameter group                          */
  PVM_RareFactor = 1;
  if(AngioMode == Yes)
  {
    Nsegments=1;
    ParxRelsMakeNonEditable("Nsegments");
  }
  else
    ParxRelsMakeEditable("Nsegments");


  STB_UpdateEncoding(
    &PVM_RareFactor,
    Seg_mode == sequential ? SEG_SEQUENTIAL: SEG_INTERLEAVED,
    Yes,
    Yes,
    Yes,
    &PVM_EchoPosition);

  PVM_RareFactor = PVM_EncMatrix[1]/Nsegments;

  STB_UpdateEncoding(
    &PVM_RareFactor,
    Seg_mode == sequential ? SEG_SEQUENTIAL: SEG_INTERLEAVED,
    Yes,
    Yes,
    Yes,
    &PVM_EchoPosition);

  Nsegments = PVM_EncMatrix[1]/PVM_RareFactor;

  if(NavSigSetupYesNo==Yes)
    TransActionBlGSP=PVM_EncMatrix[0]*PVM_EncZf[0]*PVM_EncPft[0];
  else
    TransActionBlGSP=1;

  /* update parameters controlling the data sampling period   */

  PVM_AntiAlias[0] = 1; // not allowed: same acq bandwith for navigator and echo

   MRT_UpdateDigPars(&PVM_EffSWh,
                     PVM_EncMatrix[0],
                    &PVM_AntiAlias[0],
                    &PVM_AcquisitionTime);

  /* update excitation pulse                                  */

  UpdateRFPulses();


  /* general features of the method */

  PVM_NEchoImages = 1;

  PVM_NRepetitions = MAX_OF(1,PVM_NRepetitions);


  /* set limits for read, phase and slice gradients            */

  ControlGradientLimits(PVM_MajSliceOri);


  /* calculate minima for FOV and slice thickness             */

  UpdateGeometryMinima(minFov,
                       &minSliceThick);


  /* update geometry parameters                               */
  STB_UpdateAtsReferencePosition("PVM_SliceGeoObj");

  // only one slice allowed if navigator is in-slice
  int maxSlices = PVM_NavPosMode == Nav_in_slice && AngioMode==No ? 1:0;
  int maxPerPackage = dim>2? 1:0;
  STB_UpdateImageGeometry(dim,
                          PVM_Matrix,
                          minFov,
                          maxSlices, // total slices
                          1, // only 1 slice package
                          maxPerPackage,
                          minSliceThick,
                          1.0); // sliceFovRatio in 3D


  /* update slice spoiler */
  double mindurSlice = 1.5*CFG_GradientRiseTime();
  double spoilerThick = dim>2 ? PVM_SpatResol[2]*PVM_EncZf[2] : PVM_SliceThick;
  MRT_UpdateSpoiler("SliceSpoiler",2.0,ExcSliceGradLim,mindurSlice,PVM_GradCalConst,spoilerThick);

  /* handling of modules */
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, spoilerThick);
  STB_UpdateSatTransModule(PVM_DeriveGains);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  STB_UpdateFlowSaturationModule(PVM_Nucleus1,PVM_DeriveGains);
  UpdateRFSpoiling();

  /* update of navigator module */
  STB_UpdateNavModule();

  /* update of parameter visibility */
  UpdateVisibility();

  /* Calculate read and slice gradients */
  UpdateReadSliceGradients();

  /* Sequence elements, TE, TR, total time: */
  UpdateSequenceTiming();
  STB_UpdateDummyScans(PVM_RepetitionTime);
  STB_UpdateDriftComp(PVM_RepetitionTime);

  /* calculate dephasing and phase-encoding gradients         */
  UpdatePhaseGradients();

  /* calculate frequency offsets                              */
  UpdateFrequencyOffsets();

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  /* Set ACQP parameters */
  SetBaseLevelParam();

  /* Set RECO parameters */

  OutputMovieFrames = PVM_NMovieFrames;

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
         extensionPossible, extensionAllowed, extension;

  /* Part of the exctiation pulse to be refocused: */
  EffPulseDur = ExcPulse1.Length * (ExcPulse1.Rpfac/100);

  /* Part of the echo to be refocused */
  EchoDelay = PVM_AcquisitionTime * PVM_EchoPosition / 100;

  if(FlowCompYesNo == No)
  {

    /* Minimum durations of all phase-gradient pulses */

    minEnc2d     = PTB_GetSpatDim()>1?
                   MRT_EncodingTime(PVM_SpatResol[1],
				    PVM_GradCalConst*Phase2DGradLim/100)
                   : 0;
    minEnc3d     = PTB_GetSpatDim()>2?
                   MRT_EncodingTime(PVM_SpatResol[2],
                                     PVM_GradCalConst*Phase3DGradLim/100)
                   : 0;

    minRephSlice = MRT_DephaseTime(EffPulseDur,
				   CFG_GradientRiseTime(),
				   ExcSliceGrad,
				   ExcSliceRephGradLim);

    RephSliceDur = MAX_OF(minRephSlice,CFG_GradientRiseTime());

    minDephRead  = MRT_DephaseTime(EchoDelay,
				   CFG_GradientRiseTime(),
				   ReadGrad,
				   ReadDephGradLim);


    /* In this sequence all phase-gradient pulses are
       simultaneous with duration EncGradDur. We set it first
       to the common (longest) minimum: */

    EncGradDur = MAX_OF( MAX_OF(minEnc2d ,  minEnc3d),
			 MAX_OF(minRephSlice ,minDephRead ) );

    /* EncGradDur should also contain one ramp, thus: */

    EncGradDur = RewGradDur = MAX_OF(EncGradDur, CFG_GradientRiseTime());
  }
  else
  {
    /* EncGradDur is the duration of simult. flow compensated
       read, phase and slice gradients. We set it first to minimum: */

    EncGradDur = minFlowEncDur(PTB_GetSpatDim(),
			       EffPulseDur,
			       ReadGrad,
			       ExcSliceGrad,
			       PVM_SpatResol);

    RewGradDur = EncGradDur/2;  // this is sufficient for equal-lobe flow comp

  }

  DB_MSG(("minimum EncGradDur = %f", EncGradDur));

  /* Update TE with the mimimum EncGradDur */

  UpdateEchoTime();

  /* If there is some freedom, make EncGradDur longer
     (to avoid unnecessarily strong phase gradients),
     but not longer than allowed by gradient resolution */

  extensionPossible = PVM_EchoTime - PVM_MinEchoTime;
  extensionAllowed  = PTB_GetSpatDim()>1? MRT_MaxEncodingTime(PVM_Fov[1], PVM_GradCalConst)-EncGradDur :20;

  DB_MSG(("ext possible = %f, allowed = %f", extensionPossible, extensionAllowed));

  if(FlowCompYesNo == No)
  {
    extension = MIN_OF(extensionPossible, extensionAllowed);
    extension = MAX_OF(extension, 0);
  }
  else
  {
    extension = MAX_OF(extensionPossible,0);
  }

  DB_MSG(("extension = %f",extension));

  if(PVM_NavPosMode == Nav_in_slice)
  {
    EncGradDur += extension/2;
    RephSliceDur += extension/2;
  }
  else
  {
    EncGradDur += extension;
  }

  TeFillDelay = PVM_EchoTime - PVM_MinEchoTime - extension;

  /* Other sequence elements, not involved in TE, e.g. spoilers  */
  double mindurRead =  RewGradDur+CFG_GradientRiseTime();
  MRT_UpdateSpoiler("ReadSpoiler",2.0,Phase2DGradLim,mindurRead,PVM_GradCalConst,PVM_SpatResol[0]*PVM_EncZf[0]);

  /* Find min TR and update TR */
  UpdateRepetitionTime();

  /* Calculate total experiment time */

  UpdateTotalTime();

  /* for PET/MR study */
  UpdateTTLOut();
}


void UpdateRepetitionTime(void)
{
  int nSlices;

  DB_MSG(("-->UpdateRepetitionTime"));


  nSlices = AngioMode ==Yes? 1 : GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  /* min TR in a movie: */
  PVM_MinRepetitionTime =  nSlices * minLoopRepetitionTime();

  PVM_RepetitionTime=MAX_OF(PVM_MinRepetitionTime,PVM_RepetitionTime);

  /*** adapt RewGradDur to desired TR ***/
  double riseT = CFG_GradientRiseTime();
  double flowCompFactor = FlowCompYesNo==Yes ? 2:1;
  double extensionAllowed = EncGradDur/flowCompFactor - RewGradDur;
  double extensionPossible = ReadSpoiler.dur-riseT - RewGradDur;
  double extension = MAX_OF(MIN_OF(extensionPossible, extensionAllowed),0);
  /* 1.) if possible, extend RewGradDur to duration of read spoiler */
  if(extension > 0)
  {
    RewGradDur += extension;
    extensionAllowed = EncGradDur/flowCompFactor - RewGradDur;
  }
  extensionPossible = (PVM_RepetitionTime - PVM_MinRepetitionTime)/nSlices;
  extension = MAX_OF(MIN_OF(extensionPossible, extensionAllowed),0);
  /* 2.) if possible, extend RewGradDur to EncGradDur  */
  if(extension > 0)
  {
    RewGradDur += extension;
    MRT_UpdateSpoiler("ReadSpoiler",2.0,Phase2DGradLim,RewGradDur+riseT,PVM_GradCalConst,PVM_SpatResol[0]*PVM_EncZf[0]);
    PVM_MinRepetitionTime += extension*nSlices;
  }

  DB_MSG(("<--UpdateRepetitionTime"));
  return;
}



/* calculates PVM_ScanTimeStr */

void UpdateTotalTime(void)
{
  int dim = PTB_GetSpatDim();
  double TotalTime=0;

  //total time for overscans
  TotalTime = PVM_RepetitionTime * PVM_EncMatrix[1]*Oversampling * PVM_NAverages;

  if( dim >2 )
    TotalTime = TotalTime * PVM_EncMatrix[2];

  TotalTime = TotalTime * PVM_SatTransRepetitions *PVM_NRepetitions;

  if(AngioMode == Yes)
  {
    int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
    TotalTime = TotalTime * nSlices;
  }

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

}

void UpdateTTLOut(void)
{
  
  double riseTime = CFG_GradientRiseTime();
  
  IgTTLDuration =
    SliceSpoiler.dur               +
    riseTime                       +
    ExcPulse1.Length - EffPulseDur +
    PVM_EchoTime                   +
    PVM_AcquisitionTime *(100.0 - PVM_EchoPosition) / 100.0 +
    ReadSpoiler.dur                +
    riseTime                       +
    0.001;

  if(PET_MR_Study == Yes)
    ParxRelsShowInEditor("IgTTLOut, IgTTLDuration");
  else
  {
    IgTTLOut = No;
    ParxRelsHideInEditor("IgTTLOut, IgTTLDuration");
  }

  PpgTTLOut = IgTTLOut;

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
    if(FlowCompYesNo == No)
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

  if(FlowCompYesNo == No)
  {
    double rise = CFG_GradientRiseTime();

    ReadDephGrad =     MRT_DephaseGrad(EncGradDur, EchoDelay,   rise, ReadGrad);

    if(PVM_NavPosMode != Nav_in_slice)
      ExcSliceRephGrad = MRT_DephaseGrad(EncGradDur, EffPulseDur, rise, ExcSliceGrad);
    else
      ExcSliceRephGrad = MRT_DephaseGrad(RephSliceDur, EffPulseDur, rise, ExcSliceGrad);

    Phase2DGrad = PTB_GetSpatDim() > 1 ?
      MRT_PhaseGrad(EncGradDur, PVM_Matrix[1], PVM_Fov[1], PVM_GradCalConst) : 0.0;

    Phase3DGrad = PTB_GetSpatDim() == 3 ?
      MRT_PhaseGrad(EncGradDur, PVM_Matrix[2], PVM_Fov[2], PVM_GradCalConst) : 0.0;

    Rew2DGrad = PTB_GetSpatDim() > 1 ?
      MRT_PhaseGrad(RewGradDur, PVM_Matrix[1], PVM_Fov[1], PVM_GradCalConst) : 0.0;

    Rew3DGrad = PTB_GetSpatDim() == 3 ?
      MRT_PhaseGrad(RewGradDur, PVM_Matrix[2], PVM_Fov[2], PVM_GradCalConst) : 0.0;

  }
  else
  {
    double ramp = CFG_GradientRampTime();
    double gradDel = CFG_GradientDelayTime();
    double l1r, l2r, l1s, l2s, l1p, l2p; // lobe durations on r/p/s


    int dim = PTB_GetSpatDim();
    double phaseIntegral;

    ExcSliceRephGrad = ReadDephGrad = 0.0;
    Phase2DGrad =  Phase3DGrad = 0.0;

    Rew2DGrad = PTB_GetSpatDim() > 1 ?
      MRT_PhaseGrad(RewGradDur, PVM_Matrix[1], PVM_Fov[1], PVM_GradCalConst) : 0.0;

    Rew3DGrad = PTB_GetSpatDim() == 3 ?
      MRT_PhaseGrad(RewGradDur, PVM_Matrix[2], PVM_Fov[2], PVM_GradCalConst) : 0.0;

    findLobes(EncGradDur, EffPulseDur,
              &l1r, &l2r, &l1p, &l2p, &l1s, &l2s);


    MRT_FlowCompGradRead(ramp,
			 l1r,
			 l2r,
			 PVM_AcquisitionTime*PVM_EchoPosition/100,
			 ReadGrad,
			 ReadFcGrad);

    MRT_FlowCompGradSlice(ramp,
			  l1s,
			  l2s,
			  EffPulseDur+gradDel,
			  ExcSliceGrad,
			  ExcSliceFcGrad);


    if(dim>1)
    {
      phaseIntegral = 1e5/(2*PVM_GradCalConst*PVM_SpatResol[1]);
      MRT_FlowCompGradPhase(ramp, l1p, l2p, phaseIntegral, Phase2DFcGrad);
    }
    else
    {
      memset( Phase2DFcGrad,0,2*sizeof(double));
    }

    if(dim>2)
    {
      phaseIntegral = 1e5/(2*PVM_GradCalConst*PVM_SpatResol[2]);
      MRT_FlowCompGradPhase(ramp, l1p, l2p, phaseIntegral, Phase3DFcGrad);
    }
    else
    {
      memset( Phase3DFcGrad,0,2*sizeof(double));
    }

    // encoding shapes
    double gradDwell = MAX_OF(CFG_GradientShapeResolution(),0.002);
    ShapeSize = floor(EncGradDur/gradDwell);
    if(ShapeSize<10)
    {
      char msg[200];
      sprintf(msg, "IgFLASH: min grad dwell too high (%f us)", 1000*gradDwell);
      UT_ReportError(msg);
    }

    PARX_change_dims("RShape", ShapeSize);
    PARX_change_dims("P2Shape", ShapeSize);
    PARX_change_dims("P3Shape", ShapeSize);
    PARX_change_dims("SShape", ShapeSize);

    memset(RShape, 0, ShapeSize*sizeof(int));
    memset(P2Shape, 0, ShapeSize*sizeof(int));
    memset(P3Shape, 0, ShapeSize*sizeof(int));
    memset(SShape, 0, ShapeSize*sizeof(int));


    makeShape(RShape, ShapeSize, ramp, l1r, l2r, ReadFcGrad[0]/100, ReadFcGrad[1]/100 );
    makeShape(P2Shape, ShapeSize, ramp, l1p, l2p, Phase2DFcGrad[0]/100, Phase2DFcGrad[1]/100 );
    makeShape(P3Shape, ShapeSize, ramp, l1p, l2p, Phase3DFcGrad[0]/100, Phase3DFcGrad[1]/100 );
    makeShape(SShape, ShapeSize, ramp, l1s, l2s, ExcSliceFcGrad[0]/100, ExcSliceFcGrad[1]/100 );

  }

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

  MRT_MakeRamp(buf,n,tot, 0,  t1,   0,     amp1, ramp_lin);
  MRT_MakeRamp(buf,n,tot, t1, t2,   amp1,  amp1, ramp_lin);
  MRT_MakeRamp(buf,n,tot, t2, t3,   amp1,  0,    ramp_lin);
  MRT_MakeRamp(buf,n,tot, t3, t4,   0,     amp2, ramp_lin);
  MRT_MakeRamp(buf,n,tot, t4, t5,   amp2,  amp2, ramp_lin);
  MRT_MakeRamp(buf,n,tot, t5, tot,  amp2,  0,    ramp_lin);



}

void UpdateFrequencyOffsets( void )
{
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

  if(PVM_NavPosMode == Nav_in_slice)
  {
    ExcSliceGradLim = ExcSliceRephGradLim = 50.0;
  }

  DB_MSG(("-->ControlGradientLimits"));
}


/* Calculates PVM_MinEchoTime and restrict PVM_EchoTIme.
   EffPulseDur, EncGradDur must be set before */
void UpdateEchoTime( void )
{

  double riseTime = CFG_GradientRiseTime();
  double rampTime = CFG_GradientRampTime()+CFG_InterGradientWaitTime();
  // encGradDur contains both ramps and plateau
  double encGradDur = FlowCompYesNo==Yes? EncGradDur:EncGradDur+rampTime;

  DB_MSG(("-->UpdateEchoTime\n"));

  if(PVM_NavPosMode != Nav_in_slice)
  {
  PVM_MinEchoTime =
    EffPulseDur    +
    rampTime       +  //falling ramp of slice grad
    encGradDur     +
    riseTime       +  //read-on ramp          +
    PVM_AcquisitionTime * PVM_EchoPosition / 100.0;
  }
  else
  {
  PVM_MinEchoTime =
    EffPulseDur    +
    rampTime       +  //falling ramp of slice grad
    RephSliceDur   +
    riseTime       +
    PVM_NavAQ      +  //AQ time for in_slice navigator
    0.1            +  //delay for scan shift
    encGradDur     +
    riseTime       +  //read-on ramp          +
    PVM_AcquisitionTime * PVM_EchoPosition / 100.0;
  }


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
  if(RespGate==Yes)
    minD0 = 0.21  /* ADC_END */ + igwt;
  else
    minD0 = 0.01  /* ADC_END */ + igwt;

  minTr =
  0.03                         + /*UpdateDynPars*/
	PVM_FatSupModuleTime         +
	PVM_SatTransModuleTime       +
	PVM_FovSatModuleTime         +
	PVM_InFlowSatModuleTime      +
        PVM_NavigatorModuleTime      +
	SliceSpoiler.dur             +
        riseTime                     +
	ExcPulse1.Length - EffPulseDur +
	PVM_EchoTime                 +
	PVM_AcquisitionTime *(100.0 - PVM_EchoPosition) / 100.0 +
	ReadSpoiler.dur              +
	riseTime                     +
        0.042                        +
	minD0;

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
    int max = PVM_EncMatrix[1]+PVM_DummyScans;
    int size = MAX_OF(256,max);
    PARX_change_dims("RFPhaseList",size);
    MRT_RFSpoilPhaseList(117,size,RFPhaseList,Yes);
  }

  DB_MSG(("<--UpdateRFSpoiling"));
}

void UpdateVisibility(void)
{
  DB_MSG(("-->UpdateVisibility"));

  if(SelfGating==No)
  {
    Oversampling=1;
    PVM_NMovieFrames=1;
    RespGate=No;
    ParxRelsMakeNonEditable("RespGate,Oversampling,PVM_NMovieFrames");
    ParxRelsShowInEditor("PVM_NAverages");
  }
  else
  {
    PVM_NAverages=1;
    ParxRelsMakeEditable("RespGate,Oversampling,PVM_NMovieFrames");
    ParxRelsHideInEditor("PVM_NAverages");
  }

  DB_MSG(("<--UpdateVisibility"));
}

static double minFlowEncDur(double dim,
                     double pulseDel,  // time from pulse centre to onset of
                                       // falling slice ramp
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

  double ramp = CFG_GradientRampTime()+CFG_InterGradientWaitTime();
  double gradDel = CFG_GradientDelayTime();
  //one lobe, counted from onset of first ramp to end of pleateau
  double encDur = 4*CFG_GradientRiseTime(); // initial value, will be incremented later
  double gval[2], gvalSl[2];
  double increment = 0.2;
  int i, grad_exceeded = 0;
  double phaseIntegral;
  double l1r, l2r, l1s, l2s, l1p, l2p; // lobe durations on r/p/s


  encDur -= increment;
  do
  {
    encDur += increment;

    findLobes(encDur, pulseDel,
              &l1r, &l2r, &l1p, &l2p, &l1s, &l2s);

    grad_exceeded = 0;
    MRT_FlowCompGradRead(ramp,
                     l1r,
                     l2r,
                     PVM_AcquisitionTime*PVM_EchoPosition/100,
                     readGrad,
                     gval);


    for(i=0;i<2;i++)
      if(fabs(gval[i])>ReadDephGradLim)
        grad_exceeded = 1;

    MRT_FlowCompGradSlice(ramp,
                      l1s,
                      l2s,
                      pulseDel+gradDel,
                      sliceGrad,
                      gvalSl);


    for(i=0;i<2;i++)
      if(fabs(gvalSl[i])>ExcSliceRephGradLim)
        grad_exceeded = 1;

    phaseIntegral = 1e5/(2*PVM_GradCalConst*resol[1]);

    MRT_FlowCompGradPhase(ramp,
                      l1p,
                      l2p,
                      phaseIntegral,
                      gval);


    for(i=0;i<2;i++)
      if(fabs(gval[i])>Phase2DGradLim)
        grad_exceeded = 1;


    if(dim>2)
    {
      phaseIntegral = 1e5/(2*PVM_GradCalConst*resol[2]);

      MRT_FlowCompGradPhase(ramp,
                        l1p,
                        l2p,
                        phaseIntegral,
                        gval);
      for(i=0;i<2;i++)
        if(fabs(gvalSl[i]+gval[i])>ExcSliceRephGradLim)
          grad_exceeded = 1;
    }


  } while (grad_exceeded && encDur<200);


  DB_MSG(("<-- minFlowEncDur"));

  return encDur;
}

static void findLobes(double encDur, double pulseDel,
               double *l1r, double *l2r,
               double *l1p, double *l2p,
               double *l1s, double *l2s)
{
  // divide endDur to lobes (r,p,s) in the optimal, feasible way

  double rampT = CFG_GradientRampTime();

  MRT_FlowCompTimingRead(rampT, encDur,
                         PVM_AcquisitionTime*PVM_EchoPosition/100,
                         l1r, l2r);


  MRT_FlowCompTimingPhase(rampT, encDur, l1p, l2p);


  MRT_FlowCompTimingSlice(rampT, encDur, pulseDel, l1s, l2s);


  *l2r = encDur - *l1r;
  *l2p = encDur - *l1p;
  *l2s = encDur - *l1s;

}

