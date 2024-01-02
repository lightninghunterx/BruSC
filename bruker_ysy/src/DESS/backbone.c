
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

#define CONF_MAP_ADJ_NAME "B0Map_Standard"
#define CONF_MAP_ADJ_NAME_DESS "B0Map_DESS"

#ifndef CPROTO
#include <sstream>
#include "PvUtil/PvExcept.hh"
#endif


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
    NULL);

  PVM_EchoPosition = 50;

  /* update parameters controlling the data sampling period   */

  MRT_UpdateDigPars(&PVM_EffSWh,
                     PVM_EncMatrix[0],
                    &PVM_AntiAlias[0],
                    &PVM_AcquisitionTime);

  /* update excitation pulse                                  */

  UpdateRFPulses();


  /* general features of the method                           */

  PVM_NEchoImages = 2;


  /* set limits for read, phase and slice gradients            */

  ControlGradientLimits(PVM_MajSliceOri);


  /* calculate minima for FOV and slice thickness              */

  UpdateGeometryMinima(minFov,
                       &minSliceThick);


  /* update geometry parameters                                */

  int dim=PTB_GetSpatDim();

  // only one package if 3D
  int maxPackages = dim>2? 1:0;

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
  
  /* handling of modules */
  STB_UpdateTriggerModule();
  if(PVM_TriggerModule == On)
    PVM_TriggerMode = per_Slice;
  STB_UpdateEvolutionModule();


  DESS_RF_cycle = Yes; // at the moment only this works (RF 0-0))

  if (DESS_RF_cycle==Yes)
  {
    RFPhaseList[0] = 0;
    RFPhaseList[1] = 0;
  }
  else
  {
    RFPhaseList[0] = 0;
    RFPhaseList[1] = 180;
  }

  /* Calculate read and slice gradients */
  UpdateReadSliceGradients();

  /* Sequence elements, TE, TR, total time: */
  UpdateSequenceTiming();

  /* Dummy Scans */
  STB_UpdateDummyScans(PVM_RepetitionTime);

  /* calculate dephasing and phase-encoding gradients         */
  UpdatePhaseGradients();

  /* calculate frequency offsets                              */
  UpdateFrequencyOffsets();

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  /* Set ACQP parameters */
  SetBaseLevelParam();
  SetAdjustmentRequests();

  /* Set RECO parameters */
  FieldMapImages=1;
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

  EncGradDur_min = EncGradDur;

  DB_MSG(("minimum EncGradDur = %f", EncGradDur));

  /* Update TE with the mimimum EncGradDur */

  UpdateEchoTime();

  /* If there is some freedom, make EncGradDur longer
     (to avoid unnecessarily strong phase gradients),
     but not longer than allowed by gradient resolution */

  extensionPossible = PVM_EchoTime - PVM_MinEchoTime;
  extensionAllowed  = PTB_GetSpatDim()>1? MRT_MaxEncodingTime(PVM_Fov[1], PVM_GradCalConst)-EncGradDur :20;

  DB_MSG(("ext possible = %f, allowed = %f", extensionPossible, extensionAllowed));

  extension = MIN_OF(extensionPossible, extensionAllowed);
  extension = MAX_OF(extension, 0);

  DB_MSG(("extension = %f",extension));

  EncGradDur += extension;
  TeFillDelay = PVM_EchoTime - PVM_MinEchoTime - extension;

  /* look for the minimum read spoiler time */
  double MaxAmp = 50.0;
  MinSpoilerDuration = 1e3*abs(DESS_ReadSpoiling-1.0)/(PVM_GradCalConst*PVM_SpatResol[0]*PVM_EncZf[0]*MaxAmp/100.0);
  MinSpoilerDuration = MAX_OF(MinSpoilerDuration, 2.0*CFG_GradientRiseTime());

  if(DESS_ReadSpoiling == 0.0)
  {
    DESS_ReadSpoilGrad = -ReadGrad*(PVM_AcquisitionTime+CFG_GradientRiseTime())/MinSpoilerDuration;
    if(ReadGrad-DESS_ReadSpoilGrad > 100)
    {
      DESS_ReadSpoilGrad = - (100 - ReadGrad);
      MinSpoilerDuration = -ReadGrad*(PVM_AcquisitionTime+CFG_GradientRiseTime())/DESS_ReadSpoilGrad;
    }
  }

  DESS_spoiler_duration=MinSpoilerDuration;

  // Balance the slice gradient
  // UpdateSlicePreGrad();
  // find min TE2 and update TE2
  UpdateTE2();

  /* Find min TR and update TR */
  UpdateRepetitionTime();

  UpdateDESSread();
  UpdateDESSslice();

  /* Calculate total experiment time */
  UpdateTotalTime();
}

void UpdateTE2( void )
{
  double EncGradDur_max,
    riseTime = CFG_GradientRiseTime(),
    igwt = CFG_InterGradientWaitTime(),
    minD0 = 0.01  /* ADC_END */ + igwt;

  DB_MSG(("-->UpdateTE2\n"));


  DESS_TE2_min =
    0.03                                            +  //UpdateDynPars
    ExcPulse1.Length * (1-(ExcPulse1.Rpfac/100))    +
    riseTime                                        +
    minD0                                           +
    EncGradDur_min                                  +
    2*riseTime                                      +
    PVM_AcquisitionTime * (1-(PVM_EchoPosition / 100)) ;


  if(FWInphase==Yes)
  {
    double cshiftsp,mesp,dval;

    cshiftsp = fabs(STB_GetFreqOffset(1,4.7)-STB_GetFreqOffset(1,1.2));
    cshiftsp = 1e3/cshiftsp;


    /* find minimum possible echo time mesp that matches
       fat water inphase condition
    */

    dval=cshiftsp;
    mesp=DESS_TE2_min;
    while(dval < mesp) dval+=cshiftsp;
    mesp=dval;

    /* round to closest echo time that is multiple of
       chemical shift related echospacing
    */
    double esp=DESS_TE2,csp2=cshiftsp/2;

    esp=MAX_OF(esp,mesp);

    for(dval=mesp;fabs(esp-dval)>csp2;dval+=cshiftsp);

    /* round to 1us accuracy */

    DESS_TE2 = ((int)(dval*1000.0 +0.5))/1000.0;

  }

  if (DESS_equal_TEs)
  {
    ParxRelsMakeNonEditable("DESS_TE2");
    DESS_TE2 = MAX_OF(DESS_TE1, DESS_TE2_min);
    if (DESS_TE2 > DESS_TE1)
    {
      EncGradDur = EncGradDur + (DESS_TE2 - DESS_TE1);
      PVM_EchoTime = DESS_TE1 = DESS_TE2 ;
    }
  }
  else
  {
    ParxRelsMakeEditable("DESS_TE2");
    DESS_TE2 = MAX_OF(DESS_TE2, DESS_TE2_min);
  }


  DESS_RewDur = EncGradDur_min;
  EncGradDur_max = PTB_GetSpatDim()>1? MRT_MaxEncodingTime(PVM_Fov[1], PVM_GradCalConst):20;
  DESS_TE2Delay = 0.0;

  if (DESS_TE2 > DESS_TE2_min )
  {
    DESS_RewDur = EncGradDur_min + DESS_TE2 - DESS_TE2_min;

    if (DESS_RewDur > EncGradDur_max)
    {
      DESS_TE2Delay = DESS_RewDur - EncGradDur_max;
      DESS_RewDur = EncGradDur_max;
    }
  }

  double rise = CFG_GradientRiseTime();

  DESS_ReadRew =     MRT_DephaseGrad(DESS_RewDur, EchoDelay,   rise, ReadGrad);
  DESS_SlicePreGradAmp = MRT_DephaseGrad(DESS_RewDur, ExcPulse1.Length/2.0, rise, ExcSliceGrad);
  DESS_Phase2Rew = PTB_GetSpatDim() > 1 ?
    MRT_PhaseGrad(DESS_RewDur, PVM_Matrix[1], PVM_Fov[1], PVM_GradCalConst) : 0.0;
  DESS_Phase3Rew = PTB_GetSpatDim() == 3 ?
    MRT_PhaseGrad(DESS_RewDur, PVM_Matrix[2], PVM_Fov[2], PVM_GradCalConst) : 0.0;

  DB_MSG(("<--UpdateTE2\n"));
}


void UpdateDESSread(void)
{

  DESS_spoiler_duration = MinSpoilerDuration + (PVM_RepetitionTime - PVM_MinRepetitionTime);

  if(DESS_spoiler_flat)
  {
    DESS_ReadSpoilGrad = ReadGrad ;
    DESS_ReadSpoiling=1.0+DESS_ReadSpoilGrad*PVM_GradCalConst*PVM_SpatResol[0]*PVM_EncZf[0]*(DESS_spoiler_duration*1e-3)/100.0;
    ParxRelsMakeNonEditable("DESS_ReadSpoiling");
  }
  else
  {
    if(DESS_ReadSpoiling == 0.0)
    {
      DESS_ReadSpoilGrad = -ReadGrad*(PVM_AcquisitionTime+CFG_GradientRiseTime())/DESS_spoiler_duration;
    }
    else
    {
      DESS_ReadSpoilGrad=(DESS_ReadSpoiling-1.0)/(PVM_GradCalConst*PVM_SpatResol[0]*PVM_EncZf[0]*(DESS_spoiler_duration*1e-3)/100.0);
    }
    ParxRelsMakeEditable("DESS_ReadSpoiling");
  }
}

void UpdateDESSslice(void)
{
  double slice_resol;

  if (PTB_GetSpatDim() == 3)
    slice_resol = PVM_SpatResol[2]*PVM_EncZf[2];
  else
    slice_resol = PVM_SliceThick;

  DESS_SliceSpoilGrad=DESS_SliceSpoiling/(PVM_GradCalConst*slice_resol*(DESS_spoiler_duration*1e-3)/100.0);
  if(DESS_SliceSpoilGrad > 50.0)
  {
    DESS_SliceSpoilGrad = 50.0;
    DESS_SliceSpoiling = DESS_SliceSpoilGrad*(PVM_GradCalConst*slice_resol*(DESS_spoiler_duration*1e-3)/100.0);
  }
}


void UpdateRepetitionTime(void)
{
  DB_MSG(("-->UpdateRepetitionTime"));

  PVM_MinRepetitionTime = minLoopRepetitionTime();

  PVM_RepetitionTime=MAX_OF(PVM_MinRepetitionTime,PVM_RepetitionTime);

  DB_MSG(("<--UpdateRepetitionTime"));
  return;
}


/* calculates PVM_ScanTimeStr */

void UpdateTotalTime(void)
{
  int dim = PTB_GetSpatDim();
  double TotalTime=0;

  if( dim >1 )
  {
    int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
    TotalTime = (PVM_RepetitionTime * PVM_EncMatrix[1])*nSlices*PVM_NAverages;
  }

  if( dim >2 )
    TotalTime = TotalTime * PVM_EncMatrix[2];

  /* time for one repetition */
  OneRepTime = TotalTime/1000.0;

  TotalTime = TotalTime * PVM_EvolutionCycles + PVM_EvolutionModuleTime;
  TotalTime = TotalTime * PVM_NRepetitions;

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

  minFov[0]     = PVM_EffSWh /
                  (1e-2*ReadGradLim * PVM_GradCalConst);

  *minSliceThick = ExcPulse1.Bandwidth /
                  (1e-2*ExcSliceGradLim * PVM_GradCalConst);

  if(dim >= 2)
    minFov[1] = minFov[0]/8;

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

void UpdatePhaseGradients()
{

  DB_MSG(("-->UpdatePhaseGradients"));

  /* Calculation of phase-encoding,
     dephasing and rephasing gradients.

     (ReadGrad, ExcSliceGrad, EchoDelay, EffPulseDur,
      and EncGradDur must be calculated before)       */

  double rise = CFG_GradientRiseTime();

  ReadDephGrad =     MRT_DephaseGrad(EncGradDur, EchoDelay,   rise, ReadGrad);

  ExcSliceRephGrad = MRT_DephaseGrad(EncGradDur, EffPulseDur, rise, ExcSliceGrad);

  Phase2DGrad = PTB_GetSpatDim() > 1 ?
                MRT_PhaseGrad(EncGradDur, PVM_Matrix[1], PVM_Fov[1], PVM_GradCalConst) : 0.0;

  Phase3DGrad = PTB_GetSpatDim() == 3 ?
                MRT_PhaseGrad(EncGradDur, PVM_Matrix[2], PVM_Fov[2], PVM_GradCalConst) : 0.0;

  Rew2DGrad = PTB_GetSpatDim() > 1 ?
                MRT_PhaseGrad(RewGradDur, PVM_Matrix[1], PVM_Fov[1], PVM_GradCalConst) : 0.0;

  Rew3DGrad = PTB_GetSpatDim() == 3 ?
                MRT_PhaseGrad(RewGradDur, PVM_Matrix[2], PVM_Fov[2], PVM_GradCalConst) : 0.0;

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

  DB_MSG(("-->ControlGradientLimits"));
}



/* Calculates PVM_MinEchoTime and restrict PVM_EchoTIme.
   EffPulseDur, EncGradDur must be set before */
void UpdateEchoTime( void )
{

  double riseTime = CFG_GradientRiseTime();

  DB_MSG(("-->UpdateEchoTime\n"));

  PVM_MinEchoTime =
    EffPulseDur    +
    riseTime       + //falling ramp of slice grad
    EncGradDur     + //enc. time (ramp+plateau
    riseTime       + //min te-filling (end ramp of encoding)
    riseTime       + //read-on ramp          +
    EchoDelay;


  if(FWInphase==Yes)
  {
    double cshiftsp,mesp,dval;

    cshiftsp = fabs(STB_GetFreqOffset(1,4.7)-STB_GetFreqOffset(1,1.2));
    cshiftsp = 1e3/cshiftsp;


    /* find minimum possible echo time mesp that matches
       fat water inphase condition
    */

    dval=cshiftsp;
    mesp=PVM_MinEchoTime;
    while(dval < mesp) dval+=cshiftsp;
    mesp=dval;

    /* round to closest echospacing that is multiple of
       chemical shift related echospacing
    */
    double esp=PVM_EchoTime,csp2=cshiftsp/2;

    esp=MAX_OF(esp,mesp);

    for(dval=mesp;fabs(esp-dval)>csp2;dval+=cshiftsp);

    /* round to 1us accuracy */

    PVM_EchoTime = ((int)(dval*1000.0 +0.5))/1000.0;

  }
  else
  {
    PVM_EchoTime = MAX_OF(PVM_EchoTime, PVM_MinEchoTime);
  }

  DESS_TE1 = PVM_EchoTime;

  DB_MSG(("<--UpdateEchoTime\n"));
}


double minLoopRepetitionTime(void)
/* ---------------------------------------------------------
this function returns the minimum duration of the innermost
pulse program loop
----------------------------------------------------------*/
{
  double minTr,
    riseTime = CFG_GradientRiseTime(),
    igwt = CFG_InterGradientWaitTime(),
    minD0 = 0.01  /* ADC_END */ + igwt;

  minTr =
    0.03                           + //UpdateDynPars
    riseTime                       +
    ExcPulse1.Length - EffPulseDur +
    PVM_EchoTime - EchoDelay       +
    PVM_AcquisitionTime            +
    MinSpoilerDuration             +
    riseTime                       +
    PVM_AcquisitionTime            +
    riseTime                       +
    DESS_RewDur                    +
    riseTime                       +
    DESS_TE2Delay +  minD0;

  return minTr;
}


// adjustment handling
void SetAdjustmentRequests(void)
{
    DB_MSG(("-->SetAdjustmentRequests"));


    PTB_ClearAdjustments();
    if(RecoMethMode==FieldMap)
    {
         PTB_AppendAdjustment("B0Map_Current_DESS",
                             "B0 Map DESS (curr)",
                             "Map of the B0 distribution in measurement object",
                             on_demand);
    }

    PTB_AppendConfiguredAdjustment(per_scan,RG_ADJNAME);

}

void HandleAdjustmentRequests(void)
{
  YesNo adjPossible=No;


  DB_MSG(("-->HandleAdjustmentRequests"));

  if (Yes==STB_AtsAdj()) {
      return;
  }

  const char *const curadjname=PTB_GetAdjName();

  if (0 == strcmp(curadjname, RG_ADJNAME))
  {
    ParxRelsParRelations("PVM_AdjHandler",Yes);
    adjPossible = Yes;
  }
  else if(!strcmp(curadjname,"B0Map_Current_DESS") ||
         !strcmp(curadjname,CONF_MAP_ADJ_NAME) ||
         !strcmp(curadjname,CONF_MAP_ADJ_NAME_DESS))
  {
    //FmapRunningAdj=FM_B0MAP;
    /* force RecoMethMode to FieldMap */
    RecoMethMode=FieldMap;
    ParxRelsMakeNonEditable("RecoMethMode");

    AdjEnableGlobalResultSave=No;
    AdjEnableUserResultSave=No;
    HandleMapMeasurement();
    adjPossible = Yes;
  }

  if(adjPossible == No)
  {
    PARX_sprintf("<--HandleAdjustmentRequests: Unknown adjustment required");
    /* make illegal adjustment request fail by setting ACQ_SetupAutoName to empty string */
    ACQ_SetupAutoName[0]='\0';
  }
  else
  {
      RunningAdj=1;
  }

  DB_MSG(("<--HandleAdjustmentRequests"));
}


void HandleMapMeasurement(void)
{
  DB_MSG(("-->HandleMapMeasurement"));

  static const ADJ_RESULT_PAR gopAdjPars[] =
    {
      { scan_result, "ACQ_jobs"  },
      { study_result, "PVM_StudyB0Map"  },
      { study_result, "PVM_B0MapShimset"  }
    };


  if(PTB_CheckSubProcess(0)==Yes) // no subprocess initialized
  {
    PvAdjManSetAdjParameterList(gopAdjPars, ARRAY_LEN(gopAdjPars));
    MRT_AdjSetHwScope(SHIM_CHAIN,0);
    if(No==PTB_AdjMethSpec())
    {
      ParxRelsResetPar("PVM_AutoRg");
      ATB_SetReceiverGains();
    }
    if(2!=PTB_InitSubProcess("RgAdj,B0Map"))
    {
      // indicates an error in PTB_InitSubProcess
      UT_ReportError("FieldMap: Unable to initialize subprocesses"
                     " for map measurement");
    }
  }
  else
  {
    if(PTB_CheckSubProcess("RgAdj"))
    {
      DB_MSG(("setting up RG adjustment for map"));

      ATB_RgAdjSetGsPars("PVM_AutoRgCounter");
    }
    else if(PTB_CheckSubProcess("B0Map"))
    {
      PvAdjManRequestNewExpno(Yes);
      PvAdjManSetAdjParameterList(gopAdjPars, ARRAY_LEN(gopAdjPars));
      MRT_AdjSetHwScope(SHIM_CHAIN,0);

      // prepare unique shimset
      std::ostringstream oerrst;
      try
      {

        std::vector<double> actshim;
        ATB_GetActualShim(actshim);
        PARX_change_dims("PVM_B0MapShimset",actshim.size());
        std::copy(actshim.begin(),actshim.end(),PVM_B0MapShimset);
      }
      catch (const PvException &ex)
      {
        oerrst << "FieldMap : Map measurement: Failed to retrieve "
               << "actual state of shim hardware!"
               << std::endl << "Reason:" << std::endl << ex.what();
        UT_ReportError(oerrst.str().c_str());
      }

      DB_MSG(("-->Last subadjustment store parameters"));
    }// end of handling B0 map
  }

  return;
  DB_MSG(("<--HandleMapMeasurement"));
}

void HandleGopAdjResult(void)
{
  DB_MSG(("-->HandleMapGopAdjResult"));
  int failed=0, overflowDetected=0;

  std::ostringstream oerrst;
  // here parameters besides AdjProcnoResult may be derived

  YesNo *overflow=ACQ_adc_overflow;
  int dim=(int)PARX_get_dim("ACQ_adc_overflow",1);

  for(int i=0;i<dim;i++)
  {
    if(overflow[i]==Yes)
    {
      overflowDetected=1;
      oerrst << "overflow on channel "
             << i+1 << std::endl;
    }
  }

  if(overflowDetected)
  {
    snprintf(AdjErrorComment,255,"FieldMap: Map measurement failed\n%s",
             oerrst.str().c_str());
    AdjPerformState=adj_failed;

  }
  else
  {
    // assure current shim setting is stored as adjustment result
    try
    {

      std::vector<double> actshim;
      ATB_GetActualShim(actshim);
      PARX_change_dims("PVM_B0MapShimset",actshim.size());
      std::copy(actshim.begin(),actshim.end(),PVM_B0MapShimset);
      // makes the map available for database

      PTB_RegisterGopAdj();
      AdjPerformState=adj_successful;
    }
    catch (const PvException &ex)
    {
      oerrst << "DESS : Map measurement: Failed to retrieve "
             << "actual state of shim hardware as adj. result!"
             << std::endl << "Reason:" << std::endl << ex.what();
      failed=1;
    }
  }// end of shim set adj result handling

  if(failed)
  {
    UT_ReportError(oerrst.str().c_str());
  }


  DB_MSG(("<--HandleMapGopAdjResult"));
  return;
}
