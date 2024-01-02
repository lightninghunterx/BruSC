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

  DB_MSG(("-->backbone"));

  /* update nuclei parameter group                            */

  STB_UpdateNuclei(No);

  /* update spectroscopy (bandwidth, spectral size, digitizer)   */
  STB_UpdateSpectroscopy( PVM_Nucleus1 );

  YesNo yeYeYe[3] = {Yes, Yes, Yes};

  STB_UpdateEncodingSpecial(
     0,
     0,
     No,
     NULL,    //segSize
     NULL,    //segMode
     NULL,    //ppi
     NULL,    //ppi ref
     NULL,    //pft
     yeYeYe); //zf


  /* update the averaging list */
  updateAveraging();

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

  int dim = PTB_GetSpatDim();

  int maxPackages = 10;  //reasonable limit
  int maxPerPackage = 0;
  if (LocalizationMode == Voxel_PRESS || dim>2)
  {
    maxPerPackage=1;
    maxPackages=1;
  }

  STB_UpdateAtsReferencePosition("PVM_SliceGeoObj");

  STB_UpdateImageGeometry(dim,
                          PVM_Matrix,
                          minFov,
                          0,
                          maxPackages,
                          maxPerPackage,
                          minSliceThick,
                          1.0);

  LocalizationHandler();

  /* update slice spoiler */
  MRT_UpdateSpoiler("Spoiler",3.0,ExcSliceGradLim,2*CFG_GradientRiseTime(),PVM_GradCalConst,PVM_SliceThick);

  /* effective resolution */
  updateEffResol(dim);

  updateModules();

  /* Calculate read and slice gradients */
  UpdateReadSliceGradients();

  /* Sequence elements, TE, TR, total time: */
  UpdateSequenceTiming();

  /* updating Dummy Scans afte RepetitionTime */
  STB_UpdateDummyScans(PVM_RepetitionTime);

  /* calculate dephasing and phase-encoding gradients         */
  UpdatePhaseGradients();

  /* calculate frequency offsets                              */
  UpdateFrequencyOffsets();

  /* DriftCompensation */
  STB_UpdateDriftComp(PVM_RepetitionTime);

  /* set up adjustment list */
  SetAdjustments();

  /* update mapshim parameter class */
  if (LocalizationMode == Voxel_PRESS)
    STB_UpdateMapShim(PVM_Nucleus1,"PVM_VoxelGeoObj");
  else
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
  double minEnc1d, minEnc2d=0, minEnc3d=0, minRephSlice, minEncGradDur;
  int spatDim = PTB_GetSpatDim();

  /* Part of the exctiation pulse to be refocused: */
  EffPulseDur = ExcPulse1.Length * (ExcPulse1.Rpfac/100);

  /* Minimum durations of all phase-gradient pulses */
  minEnc1d = MRT_EncodingTime(PVM_SpatResol[0],
                              PVM_GradCalConst*Phase1DGradLim/100);

  minEnc2d = spatDim>1?
    MRT_EncodingTime(PVM_SpatResol[1], PVM_GradCalConst*Phase2DGradLim/100) : 0;

  minEnc3d = spatDim>2?
    MRT_EncodingTime(PVM_SpatResol[2], PVM_GradCalConst*Phase3DGradLim/100) : 0;

  minRephSlice = MRT_DephaseTime(EffPulseDur,
                                 CFG_GradientRiseTime(),
                                 ExcSliceGrad,
                                 ExcSliceRephGradLim);


  /* In this sequence all phase-gradient pulses are
     simultaneous with duration EncGradDur. We set its minimum
     to the common (longest) minimum: */

  minEncGradDur = MAX_OF( MAX_OF(minEnc2d ,  minEnc3d),
                       MAX_OF(minRephSlice ,minEnc1d ) );


  /* MinEncGradDur should also contain one ramp, thus: */

  minEncGradDur = MAX_OF(EncGradDur, CFG_GradientRiseTime());

  /* Setting encoding duration to minimum
     (could be replaced by restrition to min.) */

  EncGradDur = minEncGradDur;

  /* Update TE  */

  UpdateEchoTime();

  /* Update modules dependent on TE (none here) */

  /* Find min TR and update TR */
  UpdateRepetitionTime();

  /* Calculate total experiment time */
  UpdateTotalTime();
}



void UpdateRepetitionTime(void)
{
  int nSlices;

  DB_MSG(("-->UpdateRepetitionTime"));


  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );


  /* min TR: */
  PVM_MinRepetitionTime = nSlices * minLoopRepetitionTime();

  if(PVM_NavOnOff == On)
    PVM_MinRepetitionTime+=(0.2 + PVM_NavigatorModuleTime);

  PVM_RepetitionTime = MAX_OF(PVM_MinRepetitionTime, PVM_RepetitionTime);

  DB_MSG(("<--UpdateRepetitionTime"));
  return;
}



/* calculates PVM_ScanTimeStr and TimeForMovieFrames */

void UpdateTotalTime(void)
{
  double TotalTime=0;

  // AverageListSum conatins all dimensions and averages, thus:
  TotalTime = PVM_RepetitionTime * AverageListSum * PVM_NRepetitions;

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

}

void UpdateGeometryMinima( double *minFov,
                           double *minSliceThick)
{
  int i,dim;


  DB_MSG(("-->UpdateGeometryMinima"));

  dim=PTB_GetSpatDim();

  /* abitrary choice: min fov = 1 mm;
     (duration of encoding adapts to fov) */

  for(i=0; i<dim; i++)
    minFov[i] = 1.0;

  if (LocalizationMode==Slice_SE)
  {
    *minSliceThick = MAX_OF(ExcPulse1.Bandwidth, RefPulse1.Bandwidth) /
                  (1e-2*ExcSliceGradLim * PVM_GradCalConst);
  }
  else
  {
  *minSliceThick = ExcPulse1.Bandwidth /
                  (1e-2*ExcSliceGradLim * PVM_GradCalConst);
  }

  DB_MSG(("<--UpdateGeometryMinima"));
}


void UpdateReadSliceGradients(void)
{
  DB_MSG(("-->UpdateReadSliceGradients"));

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

  int dim = PTB_GetSpatDim();

  ExcSliceRephGrad = MRT_DephaseGrad(EncGradDur,
                                 EffPulseDur,
                                 CFG_GradientRiseTime(),
                                 ExcSliceGrad);

  Phase1DGrad = MRT_PhaseGrad(EncGradDur,
                                PVM_Matrix[0],
                                PVM_Fov[0],
                                PVM_GradCalConst);

  if(dim > 1)
  {
    Phase2DGrad = MRT_PhaseGrad(EncGradDur,
                                PVM_Matrix[1],
                                PVM_Fov[1],
                                PVM_GradCalConst);
  }
  else
  {
    Phase2DGrad = 0;
  }

  if(dim == 3)
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

void UpdateFrequencyOffsets( void )
{
  int spatDim;
  int i,nslices;

  spatDim = PTB_GetSpatDim();
  nslices = GTB_NumberOfSlices(PVM_NSPacks,PVM_SPackArrNSlices);

  MRT_FrequencyOffsetList(nslices,
                          PVM_EffReadOffset,
                          0.0,
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
  STB_UpdateRFPulse("RefPulse1",1,PVM_DeriveGains,Conventional);

  if(PVM_DeriveGains==Yes)
  {
    ParxRelsHideInEditor("ExcPulse1Ampl");
    ParxRelsHideInEditor("RefPulse1Ampl");
  }
  else
  {
    ParxRelsShowInEditor("ExcPulse1Ampl");
    if (LocalizationMode != Slice_FID)
      ParxRelsShowInEditor("RefPulse1Ampl");
    else
      ParxRelsHideInEditor("RefPulse1Ampl");
  }


  ParxRelsShowInFile("ExcPulse1Ampl");
  ParxRelsShowInFile("RefPulse1Ampl");

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
    Phase1DGradLim      =
    Phase2DGradLim      =
    Phase3DGradLim      =
    ExcSliceRephGradLim =
    ExcSliceGradLim     = 100.0;
  }
  else
  {
    /* Gradient limits in % of max. Value 57 (1/sqrt(3))
       is needed for arbitrary oblique slices. */
    Phase1DGradLim      =
    Phase2DGradLim      =
    Phase3DGradLim      =
    ExcSliceRephGradLim = 57.0;

  }
    ExcSliceGradLim     = 100.0;



  DB_MSG(("-->ControlGradientLimits"));
}



/* Calculates PVM_MinEchoTime and restrict PVM_EchoTIme.
   EffPulseDur, EncGradDur must be set before */
void UpdateEchoTime( void )
{
  double riseTime = CFG_GradientRiseTime();

  DB_MSG(("-->UpdateEchoTime\n"));

  switch (LocalizationMode)
  {
    case Slice_FID:
      MinTE1_1=
        ExcPulse1.Length/2.0     +
        riseTime                 +
        EncGradDur               +
        riseTime;

      PVM_MinEchoTime = MinTE1_1;
      break;
    case Slice_SE:

      MinTE1_1 =
        ExcPulse1.Length/2.0     +
        riseTime                 +
        RefSpoiler.dur +
        riseTime +
        RefPulse1.Length/2.0;

      MinTE1_2 =
        RefPulse1.Length/2.0 +
        RefSpoiler.dur +
        riseTime +
        EncGradDur +
        riseTime;

      PVM_MinEchoTime=2.0*MAX_OF(MinTE1_1,MinTE1_2);
      break;

    case Voxel_PRESS:

      MinTE1_1 =
        ExcPulse1.Length/2.0     +
        riseTime                 +
        RefSpoiler.dur +
        riseTime +
        RefPulse1.Length/2.0;

      MinTE1_2 =
        RefPulse1.Length/2.0 +
        RefSpoiler.dur +
        riseTime;

      MinTE2_1=
        RefSpoiler.dur +
        riseTime +
        RefPulse1.Length/2.0;

      MinTE2_2 =
        RefPulse1.Length/2.0 +
        RefSpoiler.dur +
        riseTime +
        EncGradDur +
        riseTime;

      PVM_MinEchoTime=(2.0*MAX_OF(MinTE1_1,MinTE1_2)) + (2.0*MAX_OF(MinTE2_1,MinTE2_2));
      break;
  }

  PVM_EchoTime = MAX_OF(PVM_EchoTime, PVM_MinEchoTime);

  DB_MSG(("<--echoTimeRels\n"));
}

double minLoopRepetitionTime(void)
/* ---------------------------------------------------------
this function returns the minimum duration of the innermost
pulse program loop (here: slice loop)
----------------------------------------------------------*/
{
  double minTr, riseTime = CFG_GradientRiseTime();

  double dynshim_event=0;
  if(PVM_DynamicShimEnable==Yes)
    dynshim_event = PVM_DynamicShimEventDuration;

  minTr =
    0.03                         + //UpdateDynPars
    dynshim_event +
    0.01                         +
    PVM_TriggerModuleTime        +
    PVM_NoeModuleTime            +
    PVM_WsModuleDuration         +
    PVM_FovSatModuleTime         +
    PVM_FatSupModuleTime         +
    Spoiler.dur                  +
    riseTime                     +
    ExcPulse1.Length/2.0         +
    PVM_EchoTime                 +
    PVM_SpecAcquisitionTime      +
    1.0;

  return minTr;
}


void LocalDecNoeHandling(void)
{
  DB_MSG(("-->LocalDecNoeHandling"));

  if((PVM_DecOnOff == On ) || (PVM_NoeOnOff == On))
  {
    if(PVM_NumberOfNuclei!=2)
    {
      STB_InitNuclei(2);
      if(PVM_NumberOfNuclei < 2)
      {
        /* system configuration: 2nd transmit channel not available */
        PVM_DecOnOff = PVM_NoeOnOff = Off;
      }
    }
  }
  else if( (PVM_DecOnOff ==Off ) && (PVM_NoeOnOff == Off))
  {
    if(PVM_NumberOfNuclei!=1)
    {
      STB_InitNuclei(1);
    }
  }

  STB_UpdateDecModule(PVM_Nucleus2,PVM_SpecAcquisitionTime);
  STB_UpdateNoeModule(PVM_Nucleus2);

  if( (PVM_DecOnOff ==On ) && (PVM_NoeOnOff == On)
      && PVM_DecMode == Composite_Pulse && PVM_NoeMode == Composite_Pulse_Noe)
  {
    /* both modules use F2 channel and share therefor the CPD pulse
       element duration PCPD[1] so the element durations have to be matched */

    if(PVM_NoePulseElementDuration != PVM_DecPulseElementDuration)
    {
      PVM_NoePulseElementDuration = PVM_DecPulseElementDuration;
      STB_UpdateNoeModule(PVM_Nucleus2);
    }
  }

  DB_MSG(("<--LocalDecNodHandling"));
}

void updateModules(void)
{

  if (LocalizationMode == Voxel_PRESS)
  {
    PVM_FovSatOnOff=Off;
    ParxRelsHideGroupInEditor("Fov_Sat_Parameters");
    ParxRelsHideInEditor("PVM_FovSatOnOff");
    ParxRelsShowGroupInEditor("OuterVolumeSuppression");
    ParxRelsShowInEditor("PVM_OvsOnOff");
  } else {
    PVM_OvsOnOff=Off;
    ParxRelsShowGroupInEditor("Fov_Sat_Parameters");
    ParxRelsShowInEditor("PVM_FovSatOnOff");
    ParxRelsHideGroupInEditor("OuterVolumeSuppression");
    ParxRelsHideInEditor("PVM_OvsOnOff");
  }

  STB_UpdateOvsModule( PVM_VoxMinDistance,
                       PtrType1x3 PVM_VoxArrSize,
                       PtrType1x3 PVM_VoxArrPositionRPS,
                       PVM_NVoxels,
                       PVM_DeriveGains,
                       PVM_Nucleus1);

  /* handling of FovSat module */
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);

  /*Handle Fat Suppression*/
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, PVM_SliceThick);

  /*
   * handle water suppression
   */
  double excDelay= PVM_FovSatModuleTime+PVM_FatSupModuleTime+Spoiler.dur+CFG_GradientRiseTime()+ExcPulse1.Length/2.0; //ms
  STB_UpdateWsModule( PVM_OvsModuleDuration,
                      PVM_NVoxels,
                      excDelay);

  LocalDecNoeHandling();

  /* update of navigator module */
  STB_UpdateNavModule();  

  if (PVM_NavOnOff == On)
  {
    ParxRelsMakeEditable("PVM_DriftCompYesNo");
  }
  else
  {
    PVM_DriftCompYesNo=No;
    ParxRelsMakeNonEditable("PVM_DriftCompYesNo");
  }

  /*handling of trigger module*/
  STB_UpdateTriggerModule();
}


/* -------------------------------------------------------------
   Redimension and calculate AverageList, which contains numbers
   of averages for each phase encoding step
   -------------------------------------------------------------*/
void updateAveraging(void)
{
  int p0, p1, p2, na=PVM_NAverages, *list, s0=PVM_EncMatrix[0],
    s1=1, s2=1, sum=0, zero = 0,
    *step0 = PVM_EncSteps0, *step1 = &zero, *step2=&zero;

  if(PTB_GetSpatDim()>1)
  {
    s1 = PVM_EncMatrix[1];
    step1 = PVM_EncSteps1;
  }

  if(PTB_GetSpatDim()>2)
  {
    s2 = PVM_EncMatrix[2];
    step2 = PVM_EncSteps2;
  }

  PARX_change_dims("AverageList", s0*s1*s2);
  list = AverageList;

  if(AverageMode == Weighted_av)
  {
    for(p2=0; p2<s2; p2++)
      for(p1=0; p1<s1; p1++)
        for(p0=0; p0<s0; p0++)
        {
          *list = (int)(1.5 +
                        (na-1)*0.125*
                        (1+cos(2*M_PI*step0[p0]/(double)s0))*
                        (1+cos(2*M_PI*step1[p1]/(double)s1))*
                        (1+cos(2*M_PI*step2[p2]/(double)s2))     );
          sum += *list;
          list++;
        }
    AverageListSum=sum;
  }
  else
  {
    for(p0=0; p0<s0*s1*s2; p0++)
      list[p0] = na;
    AverageListSum = s0*s1*s2*na;
  }
}


void updateEffResol(int dim)
{
  int i;
  double apodizationBroadening  = 1.5; // Hamming

  PARX_change_dims("EffResolution", dim);
  for(i=0; i<dim; i++)
  {
    EffResolution[i] = PVM_SpatResol[i] *
                       PVM_EncZf[i] *
                       apodizationBroadening;
  }
}

void SetAdjustments(void )
{
  DB_MSG(("-->SetAdjustments"));

  PTB_ClearAdjustments();

  if (LocalizationMode == Voxel_PRESS)
    STB_ArrayPhaseAppendAdjustment(PVM_EncNReceivers, No,0, &PVM_VoxelGeoCub[0]);
  else
    STB_ArrayPhaseAppendAdjustment(PVM_EncNReceivers, No,0,
                                   &PVM_SliceGeo[0], PARX_get_dim("PVM_SliceGeo",1));

  PTB_AppendOrderAdjustment(per_scan, per_scan, RG_ADJNAME);

  DB_MSG(("<--SetAdjustments"));
}

