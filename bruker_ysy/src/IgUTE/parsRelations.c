/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2013
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 * ***************************************************************/


static const char resid[] = "$Id$ (C) 2004 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"
#include <lib/PvSysinfoClient.h>

/*===============================================================
 * ExcPulse1EnumRel
 * Relation of ExcPulse1Enum (a dynamic enmueration parameter which
 * allows to select one of the existing  pulses)
 *===============================================================*/

void ExcPulse1EnumRel(void)
{
  DB_MSG(("-->ExcPulse1EnumRel"));

  UT_SetRequest("ExcPulse1Enum");
  backbone();

  DB_MSG(("<--ExcPulse1EnumRel"));
}

/*===============================================================
 * ExcPulse1AmplRel
 * Relation of ExcPulseAmpl
 * This parameter is used in the setup parameter card to adjust
 * the RF pulse amplitude manually
 *===============================================================*/

void ExcPulse1AmplRel(void)
{
  DB_MSG(("-->ExcPulse1AmplRel"));
  UT_SetRequest("ExcPulse1Ampl");
  HandleRFPulseAmplitude();
  DB_MSG(("-->ExcPulse1AmplRel"));
}

void HandleRFPulseAmplitude(void)
{
  DB_MSG(("-->HandleRFPulseAmplitude"));

  STB_UpdateRFShapeAmplitude("ExcPulse1Ampl",No);
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]");

  DB_MSG(("<--HandleRFPulseAmplitude"));
}


/* ===================================================================
 * Relation of ExcPulse
 *
 * All pulses of type PVM_RF_PULSE must have relations like this.
 * However, if you clone this funtion for a different pulse parameter
 * remember to replace the param name in the call to UT_SetRequest!
 *
 * IMPORTANT: this function should not be invoked in the backbone!
 ====================================================================*/

void ExcPulse1Rel(void)
{
  DB_MSG(("-->ExcPulse1Rel"));

  /*
   * Tell the request handling system that the parameter
   * ExcPulse has been edited
   */

  UT_SetRequest("ExcPulse1");

  /* Check the values of ExcPulse */

  ExcPulse1Range();

  /*
   * call the backbone; further handling will take place there
   * (by means of STB_UpdateRFPulse)
   */

  backbone();

  DB_MSG(("<--ExcPulse1Rel"));
}

void ExcPulse1Range(void)
{
  DB_MSG(("-->ExcPulse1Range"));

  // range checker fields to be controlled may be
  // .Length
  // .Bandwidth
  // .Flipangle
  // .Calculated
  // .Sharpness
  // .Flatness
 double dval=ExcPulse1.Flipangle;

 ExcPulse1.Flipangle = MIN_OF(90.0,MAX_OF(dval,1.0));

 DB_MSG(("<--ExcPulseRange"));
}

/*--------------------------------------------------------
 *
 * single parameter range checkers and relations
 *
 *-------------------------------------------------------*/


void RepetitionTimeRange(void)
{

  DB_MSG(("-->RepetitionTimeRange"));

  if(ParxRelsParHasValue("PVM_RepetitionTime")==No)
  {
    PVM_RepetitionTime = 6.0;
  }
  else
  {
    PVM_RepetitionTime = MAX_OF(1e-3,PVM_RepetitionTime);
  }


  DB_MSG(("<--RepetitionTimeRange"));
  return;
}

void RepetitionTimeRel(void)
{
  DB_MSG(("-->RepetitionTimeRel"));

  RepetitionTimeRange();
  backbone();

  DB_MSG(("<--RepetitionTimeRel"));
  return;
}

/*==============================================================
 * method specific range checkers and relations
 *==============================================================*/


void ProUnderRel(void)
{
  DB_MSG(("-->ProUnderRel"));

  ProUnderRange();
  backbone();

  DB_MSG(("<--ProUnderRel"));
  return;
}

void ProUnderRange(void)
{
  DB_MSG(("-->ProUnderRange"));

  if(!ParxRelsParHasValue("ProUndersampling"))
  {
    ProUndersampling = 1.0;
  }
  else
  {
    ProUndersampling = MAX_OF(MIN_OF(512.0,ProUndersampling),1.0);
  }

  DB_MSG(("<--ProUnderRange"));
  return;
}


void RampModeRel (void)
{

  RampModeInitRel();
  backbone();
}

void RampModeInitRel (void)
{
  if(ParxRelsParHasValue("RampMode") == No)
    RampMode = constant_slope;

  SlewRateRange();
  ConstantTimeRange();

  switch(RampMode)
  {
  default:
  case constant_time:
    ParxRelsHideInEditor("EffSlewRate");
    ParxRelsHideInFile("EffSlewRate");
    ParxRelsShowInEditor("Ramptime");
    ParxRelsShowInFile("Ramptime");
    break;
  case constant_slope:
    ParxRelsHideInEditor("Ramptime");
    ParxRelsHideInFile("Ramptime");
    ParxRelsShowInEditor("EffSlewRate");
    ParxRelsShowInFile("EffSlewRate");
    break;
  }
}

void SlewRateRel(void)
{
  SlewRateRange();
  backbone();
}

void SlewRateRange(void)
{
  DB_MSG(("-->EffSlewRateRange"));

  if(ParxRelsParHasValue("EffSlewRate") == 0)
  {
    EffSlewRate = 100.0;
  }
  else
  {
    EffSlewRate = MIN_OF(MAX_OF(10.0,EffSlewRate),100.0);
  }

  DB_MSG(("<--EffSlewRateRange"));
  return;
}

void ConstantTimeRange(void)
{
  double rampT = CFG_GradientRampTime();

  DB_MSG(("-->ConstantTimeRange"));

  if(ParxRelsParHasValue("Ramptime") == 0)
  {
    Ramptime = rampT;
  }
  else
  {
    Ramptime = MIN_OF(MAX_OF(rampT, Ramptime), 4.0*rampT);
  }

  DB_MSG(("<--ConstantTimeRange"));
  return;
}

void UpdateAdjustments(void)
{
  DB_MSG(("-->UpdateAdjustments"));

  PTB_ClearAdjustments();
  STB_TrajAppendAdjustment("");
  PTB_AppendOrderAdjustment(per_scan, per_scan, RG_ADJNAME);

  DB_MSG(("<--UpdateAdjustments"));
}

// slice selection
void SliceSpoilerRel(void)
{
  DB_MSG(("-->SliceSpoilerRel"));
  UT_SetRequest("SliceSpoiler");
  backbone();
  DB_MSG(("<--SliceSpoilerRel"));
}

void EchoPositionRange(void)
{
  DB_MSG(("-->EchoPositionRange"));

  if(ParxRelsParHasValue("PVM_EchoPosition")==No)
  {
    PVM_EchoPosition=50.0;
  }
  else
  {
    PVM_EchoPosition = MAX_OF(MIN_OF(PVM_EchoPosition,50),20.0);
  }

  DB_MSG(("<--EchoPositionRange"));

}


void EffSWhRange(void)
{
  DB_MSG(("-->EffSWhRange"));
  double minmax[2]={2000,1000000};

  if(!ParxRelsParHasValue("PVM_EffSWh"))
    PVM_EffSWh=100000;

  MRT_GetNearestBandwidthInHz("PVM_EffSWh",minmax);

  DB_MSG(("<--EffSWhRange"));
}

void EffSWhRel(void)
{
  DB_MSG(("-->EffSWhRel"));

  EffSWhRange();
  backbone();

  DB_MSG(("<--EffSWhRel"));
}

void RecoMethModeVisPar(void)
{
  DB_MSG(("-->RecoMethModeVisPar"));

  if(TimeGridding ==Yes)
    ParxRelsShowInEditor("GaussWidth");
  else
    ParxRelsHideInEditor("GaussWidth");

  const char *const heartList = "HeartRate,CardiacPeriod,FreezePhase0,FreezePhaseRange0";
  const char *const respList  = "RespRate,RespPeriod,FreezePhase,FreezePhaseRange";
  ParxRelsShowInEditor(heartList);
  ParxRelsShowInEditor(respList);

  switch(AnalysisOfMotion)
  {
  case 0:
    break;
  case 1:
    MovieType=Cardiac;
    ParxRelsHideInEditor(respList);
    break;
  case 2:
    MovieType=Respiration;
    ParxRelsHideInEditor(heartList);
    break;
  }

  const char *const parList = \
    "OutputMovieFrames,MovieType,HeartRate,RespRate,FreezePhase0,FreezePhaseRange0,"
    "FreezePhase,FreezePhaseRange,TimeGridding,GaussWidth,AnalysisOfMotion";

  if(SelfGating == No)
    ParxRelsMakeNonEditable(parList);
  else
    ParxRelsMakeEditable(parList);

  DB_MSG(("<--RecoMethModeVisPar"));
}

void MovieFramesRelations(void)
{
    if (OutputMovieFrames < 1)
        OutputMovieFrames = 1;
    else if (OutputMovieFrames > Oversampling)
        OutputMovieFrames = Oversampling;
    SetRecoParam ();

}

void HandleAdjustmentRequests(void)
{
  STB_TrajHandleAdjustmentRequests();
}

void GradSyncRange(void)
{
  DB_MSG(("-->GradSyncRange"));

  if(ParxRelsParHasValue("GradSync") == 0)
    GradSync = No;
  
  // hardware supports synchronization
  double maxClock = 0.0;
  maxClock = SysInfo_GPSClockMax();
  if(maxClock == 0.0) 
    GradSync = No;

  DB_MSG(("<--GradSyncRange"));
}

void GradSyncRel(void)
{
  DB_MSG(("-->GradSyncRel"));

  GradSyncRange();
  backbone();

  DB_MSG(("<--GradSyncRel"));
}

void MyRgInitRel(void)
{
  DB_MSG(("-->MyRgInitRel"));
 
  PpgTTLOut = No; 

  ParxRelsParRelations("PVM_AutoRgInitHandler",Yes);
  DB_MSG(("<--MyRgInitRel"));
}


void SetBeforeAcquisition(void)
{
  DB_MSG(("-->SetBeforeAcquisition"));
  
  //redirecting PVM_ScanHandler to local method requires to call
  //modality-specific start-settings!
  ParxRelsParRelations("PVM_AcqScanHandler", MODALITY_HIERARCHY);

  if((PVM_AcqScanHandler == SCAN_EXPERIMENT) && (IgTTLOut == Yes))
    PpgTTLOut = Yes;
  else
    PpgTTLOut = No;

  DB_MSG(("<--SetBeforeAcquisition"));
}
