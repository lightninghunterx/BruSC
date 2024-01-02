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

#if DEBUG
#define DEB_FPRINT(file, array, nelem)  \
{\
int ix; \
FILE *dfp; \
if((dfp=fopen(file,"w"))!=NULL) \
{ \
for(ix=0; ix<(nelem); ix++) fprintf(dfp, " %g\n",(double) array[ix] );\
fclose(dfp); \
} \
}
#else
#define DEB_FPRINT(file, array, nelem) ;
#endif

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
                      double *l1s, double *l2s,
                      double *l1e, double *l2e);

void makeShape(double *buf, int n, double ramp, double lobe1, double lobe2,
               double amp1, double amp2);

void backbone(void)
{
  double minFov[3];
  double minSliceThick;
  int dim;

  DB_MSG(("-->backbone"));

  /* update nuclei parameter group                            */

  STB_UpdateNuclei(No);

  /* Limitation of FourierFlow to 2D */
  if(FlowMode == FourierFlowImaging)
    PTB_SetSpatDim(2);

  dim=PTB_GetSpatDim();

  /* update encoding parameter group                          */

  STB_UpdateEncoding(
    NULL,
    SEG_SEQUENTIAL,
    Yes,
    Yes,
    Yes,
    &PVM_EchoPosition);

  /* update parameters controlling the data sampling period   */

  MRT_UpdateDigPars(&PVM_EffSWh,
                     PVM_EncMatrix[0],
                    &PVM_AntiAlias[0],
                    &PVM_AcquisitionTime);

  /* update excitation pulse                                  */

  UpdateRFPulses();


  /* general features of the method */

  PVM_NEchoImages = 1;


  /* set limits for read, phase and slice gradients            */

  ControlGradientLimits(PVM_MajSliceOri);


  /* calculate minima for FOV and slice thickness             */

  UpdateGeometryMinima(minFov,
                       &minSliceThick);


  /* update geometry parameters                               */

  //
  int maxPackages = 1; //limited by reconstruction

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
  
  /* update slice spoiler */
  double mindurSlice = 1.5*CFG_GradientRiseTime();
  double spoilerThick = dim>2 ? PVM_SpatResol[2]*PVM_EncZf[2] : PVM_SliceThick;
  MRT_UpdateSpoiler("SliceSpoiler",2.0,ExcSliceGradLim,mindurSlice,PVM_GradCalConst,spoilerThick);

  /* handling of modules */
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, spoilerThick);
  STB_UpdateSatTransModule(PVM_DeriveGains);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  STB_UpdateFlowSaturationModule(PVM_Nucleus1,PVM_DeriveGains);
  STB_UpdateTriggerModule();
  STB_UpdateEvolutionModule();

  UpdateMovie();
  UpdateRFSpoiling();

  /* Calculate read and slice gradients */
  UpdateReadSliceGradients();

  /* Sequence elements, TE, TR, total time: */
  UpdateSequenceTiming();
  STB_UpdateDummyScans(PVM_RepetitionTime);

  /* calculate dephasing and phase-encoding gradients         */
  UpdatePhaseGradients();


  // previous call also included UpdateFlow; we know FlowEncLoop and can:
  UpdateTotalTime();

  /* calculate frequency offsets                              */
  UpdateFrequencyOffsets();

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
   ReadGrad and ExcSliceGrad must be already set.
   ------------------------------------------------------*/
{
  double extension;

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

  DB_MSG(("minimum EncGradDur = %f", EncGradDur));

  /* Update TE with this EncGradDur */

  UpdateEchoTime();


  /* If there is some freedom, make EncGradDur longer */

  extension = PVM_EchoTime - PVM_MinEchoTime;
  extension = MAX_OF(extension, 0);

  DB_MSG(("extension = %f",extension));

  EncGradDur += extension;

  RewindGradDur = EncGradDur/2;  // this is sufficient for equal-lobe flow comp

  /* Other sequence elements, not involved in TE, e.g. spoilers  */
  double mindurRead =  RewindGradDur+CFG_GradientRiseTime();
  MRT_UpdateSpoiler("ReadSpoiler",2.0,Phase2DGradLim,mindurRead,PVM_GradCalConst,PVM_SpatResol[0]*PVM_EncZf[0]);

  /* inversion recovery removed */
  PVM_InversionTime = 0.0;


  /* Find min TR and update TR */
  UpdateRepetitionTime();

  /* Calculate total experiment time */
  // UpdateTotalTime(); moved to end of backbone, when we know FlowEncLoop
}




void UpdateRepetitionTime(void)
{
  int nSlices;

  DB_MSG(("-->UpdateRepetitionTime"));


  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  /* min TR in a movie: */
  PVM_MinRepetitionTime = nSlices * minLoopRepetitionTime();

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
    /* TR is one movie frame */
    TimeForMovieFrames = PVM_RepetitionTime * PVM_NMovieFrames;
    TotalTime =  TimeForMovieFrames * FlowEncLoop * PVM_EncMatrix[1] * PVM_NAverages;
  }

  if( dim >2 )
    TotalTime = TotalTime * PVM_EncMatrix[2];

  /* time for one repetition */
  OneRepTime = TotalTime/1000.0;

  TotalTime = TotalTime * PVM_EvolutionCycles + PVM_EvolutionModuleTime;
  TotalTime = TotalTime * PVM_SatTransRepetitions * PVM_NRepetitions;

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

}


void UpdateGeometryMinima(double *minFov, double *minSliceThick)
{
  int dim;

  DB_MSG(("-->UpdateGeometryMinima"));

  dim=PTB_GetSpatDim();

  minFov[0]      = PVM_EffSWh /
                   (1e-2*ReadGradLim * PVM_GradCalConst);
 
  *minSliceThick = ExcPulse1.Bandwidth /
                   (1e-2*ExcSliceGradLim * PVM_GradCalConst);

  if(dim >= 2)
    minFov[1] = minFov[0]/4;

  if(dim >= 3)
    minFov[2] = *minSliceThick;

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


/* -------------------------------------------------------------------------------------
   calculates phase encoding, rad deph, slice reph graidents,
   makes update of flow parameters,
   and produces the gradient shapes
   ----------------------------------------------------------------------------------*/
void UpdatePhaseGradients()
{

  DB_MSG(("-->UpdatePhaseGradients"));

  /* Calculation of phase-encoding,
     dephasing and rephasing gradients.

     (ReadGrad, ExcSliceGrad, EchoDelay, EffPulseDur,
      EncGradDur and RewindGradDur must be calculated before)       */

  double ramp = CFG_GradientRampTime();
  double gradDel = CFG_GradientDelayTime();
  double l1r, l2r, l1s, l2s, l1p, l2p, l1e, l2e; // lobe durations on r/p/s


  int dim = PTB_GetSpatDim();
  double phaseIntegral;

  Phase2DGrad = PTB_GetSpatDim() > 1 ?
    MRT_PhaseGrad(RewindGradDur, PVM_Matrix[1], PVM_Fov[1], PVM_GradCalConst) : 0.0;

  Phase3DGrad = PTB_GetSpatDim() == 3 ?
    MRT_PhaseGrad(RewindGradDur, PVM_Matrix[2], PVM_Fov[2], PVM_GradCalConst) : 0.0;

  findLobes(EncGradDur, EffPulseDur,
            &l1r, &l2r, &l1p, &l2p, &l1s, &l2s, &l1e, &l2e);


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
  DB_MSG(("gdw = %f", gradDwell));
  ShapeSize = floor(EncGradDur/gradDwell);
  if(ShapeSize<10)
  {
    char msg[200];
    sprintf(msg, "FcFLASH: min grad dwell too high (%f us)", 1000*gradDwell);
    UT_ReportError(msg);
  }

  PARX_change_dims("RShape", ShapeSize);
  PARX_change_dims("P2Shape", ShapeSize);
  PARX_change_dims("P3Shape", ShapeSize);
  PARX_change_dims("SShape", ShapeSize);
  PARX_change_dims("EShape", ShapeSize);

  memset(RShape, 0, ShapeSize*sizeof(int));
  memset(P2Shape, 0, ShapeSize*sizeof(int));
  memset(P3Shape, 0, ShapeSize*sizeof(int));
  memset(SShape, 0, ShapeSize*sizeof(int));
  memset(EShape, 0, ShapeSize*sizeof(int));


  makeShape(RShape, ShapeSize, ramp, l1r, l2r, ReadFcGrad[0]/100, ReadFcGrad[1]/100 );
  makeShape(P2Shape, ShapeSize, ramp, l1p, l2p, Phase2DFcGrad[0]/100, Phase2DFcGrad[1]/100 );
  makeShape(P3Shape, ShapeSize, ramp, l1p, l2p, Phase3DFcGrad[0]/100, Phase3DFcGrad[1]/100 );
  makeShape(SShape, ShapeSize, ramp, l1s, l2s, ExcSliceFcGrad[0]/100, ExcSliceFcGrad[1]/100 );


  //we know ReadFcGrad etc, and we can now call
  UpdateFlow(PVM_SliceThick,PTB_GetSpatDim(), EncGradDur);

  //now we know FlowEncGradientAmp:
  makeShape(EShape, ShapeSize, ramp, l1e, l2e, -FlowEncGradientAmp/100, FlowEncGradientAmp/100 );

  // testing EShape
  DEB_FPRINT("/tmp/eshape",EShape, ShapeSize);
#if DEBUG
  double mom=0, *es=EShape;
  int i, n=ShapeSize;
  for(i=0;i<n;i++)
  {
    mom += i*es[i];
  }
  mom *= pow(EncGradDur/ShapeSize, 2.0); // 1st moment for unit amp, in ms^2
  double shift = 2*M_PI*(PVM_GradCalConst*1e-3)*mom*(FlowRange*1e-2);
  DB_MSG(("Flow shift: %f x PI", shift/M_PI));
 #endif



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
      Phase2DGradLim    =
      Phase3DGradLim    = 100;
    ExcSliceRephGradLim = 100.0 - TWISTER_MARGIN;
  }
  else
  {
    /* Gradient limits in % of max. Value 57 (1/sqrt(3))
       is needed for arbitrary oblique slices. */
    ReadDephGradLim     =
      Phase2DGradLim      =
      Phase3DGradLim      = 57;
    ExcSliceRephGradLim = 57.0 - TWISTER_MARGIN;
  }

  ReadGradLim        = 100.0;
  ExcSliceGradLim    = 100.0;

  DB_MSG(("-->ControlGradientLimits"));
}

void UpdateMovie(void)
{
  if (FlowMode == FourierFlowImaging)
    PVM_NMovieFrames=1;

  if(PVM_NMovieFrames == 1)
  {
    ParxRelsHideInEditor("TimeForMovieFrames");
  }
  else
  {
    ParxRelsShowInEditor("TimeForMovieFrames");
  }
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
    EncGradDur     + //enc. time (ramp+plateau
    riseTime       + //read-on ramp          +
    EchoDelay;


  PVM_EchoTime = MAX_OF(PVM_EchoTime, PVM_MinEchoTime);

  DB_MSG(("<--UpdateEchoTime\n"));
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

  minTr =
	PVM_FatSupModuleTime          +
	PVM_SatTransModuleTime        +
	PVM_FovSatModuleTime          +
	PVM_InFlowSatModuleTime       +
	SliceSpoiler.dur              +
  riseTime                      +
	ExcPulse1.Length - EffPulseDur+
	PVM_EchoTime - EchoDelay      +
	PVM_AcquisitionTime           +
	ReadSpoiler.dur               +
	riseTime                      +
  0.005                         + /*freqRX*/
  0.03                          + /*UpdateDynPars*/
  0.005                         + /*movie Label*/
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


static double minFlowEncDur(double dim,
                     double pulseDel,  // time from pulse centre to onset of
                                       // falling slice ramp
                     double readGrad,
                     double sliceGrad,
                     const double *resol )
/* -------------------------------------------------
   min duration of encoding, read-deph and slice rephasing
   (flow compensated) counted from onser of first enc ramp
   to onset of last enc ramps, assuming equal durations of
   both lobes. PVM_AcquisitionTime and PVM_EchoPosition must
   be set before.
   -------------------------------------------------*/
{

  // margin ofr MRI gradients 1.0=all; the rest is for flow grad
#define MARGIN 0.8

  double ramp = CFG_GradientRampTime()+CFG_InterGradientWaitTime();
  double gradDel = CFG_GradientDelayTime();
  //one lobe, conuted from onset of first ramp to end of pleateau
  double encDur = 4*CFG_GradientRiseTime(); // initial value, will be incremented later
  double gval[2], gvalSl[2];
  double increment = 0.2;
  int i, grad_exceeded = 0;
  double phaseIntegral;
  double l1r, l2r, l1s, l2s, l1p, l2p, l1e, l2e; // lobe durations on r/p/s


  encDur -= increment;
  do
  {
    encDur += increment;

    findLobes(encDur, pulseDel,
              &l1r, &l2r, &l1p, &l2p, &l1s, &l2s, &l1e, &l2e);

    grad_exceeded = 0;
    MRT_FlowCompGradRead(ramp,
                     l1r,
                     l2r,
                     PVM_AcquisitionTime*PVM_EchoPosition/100,
                     readGrad,
                     gval);


    for(i=0;i<2;i++)
      if(fabs(gval[i])>ReadDephGradLim*MARGIN)
        grad_exceeded = 1;

    MRT_FlowCompGradSlice(ramp,
                      l1s,
                      l2s,
                      pulseDel+gradDel,
                      sliceGrad,
                      gvalSl);


    for(i=0;i<2;i++)
      if(fabs(gvalSl[i])>ExcSliceRephGradLim*MARGIN)
        grad_exceeded = 1;

    phaseIntegral = 1e5/(2*PVM_GradCalConst*resol[1]);

    MRT_FlowCompGradPhase(ramp,
                      l1p,
                      l2p,
                      phaseIntegral,
                      gval);


    for(i=0;i<2;i++)
      if(fabs(gval[i])>Phase2DGradLim*MARGIN)
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
        if(fabs(gvalSl[i]+gval[i])>ExcSliceRephGradLim*MARGIN)
          grad_exceeded = 1;
    }


  } while (grad_exceeded && encDur<200);


  DB_MSG(("<-- minFlowEncDur"));

  return encDur;
}

static void findLobes(double encDur, double pulseDel,
                      double *l1r, double *l2r, //re deph
                      double *l1p, double *l2p, //ph enc
                      double *l1s, double *l2s, //sl reph
                      double *l1e, double *l2e) //flow enc
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

  // equal lobes for flow encoding:
  *l1e = *l2e = encDur/2;

}
