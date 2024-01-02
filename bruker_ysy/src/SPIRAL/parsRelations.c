/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2007
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 * ***************************************************************/


static const char resid[] = "$Id$ (c) 2007 Bruker BioSpin MRI GmbH";

#define DEBUG      0

#include "method.h"
#include <lib/PvSysinfoClient.h>

/* =============================================================
 * Global parameters
 * ============================================================= */

void EffSWhRange(void)
{
  DB_MSG(("-->EffSWhRange"));
  double minmax[2]={2000,1000000};

  if(!ParxRelsParHasValue("PVM_EffSWh"))
    PVM_EffSWh=200000;

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

void MovieFramesRange(void)
{
  DB_MSG(("-->MovieFramesRange"));

  if (!ParxRelsParHasValue("PVM_NMovieFrames"))
  {
    PVM_NMovieFrames = 1;
  }

  PVM_NMovieFrames = MAX_OF(1, MIN_OF(100, PVM_NMovieFrames));

  DB_MSG(("<--MovieFramesRange"));
}

void MovieFramesRel(void)
{
  DB_MSG(("-->MovieFramesRel"));

  MovieFramesRange();
  backbone();

  DB_MSG(("<--MovieFramesRel"));
}

void NAveragesRange(void)
{
  DB_MSG(("-->NAveragesRange\n"));

  if(ParxRelsParHasValue("PVM_NAverages") == No)
  {
    PVM_NAverages = 1;
  }

  if (PVM_NAverages > 1)
  {
    ParxRelsShowInEditor("PVM_MotionSupOnOff");
  }
  else
  {
    ParxRelsHideInEditor("PVM_MotionSupOnOff");
  }

  DB_MSG(("<--AveragesRange\n"));
}

void NAveragesRels(void)
{
  DB_MSG(("-->NAveragesRels"));

  NAveragesRange();

  backbone();

  DB_MSG(("<--NAveragesRels"));
}

/* =============================================================
 * Local parameters
 * ============================================================= */

void MinimumTERel(void)
{
  DB_MSG(("-->MinimumTERel"));

  ParxRelsAddConflictSource(NULL, "PVM_EchoTime");
  backbone();

  DB_MSG(("<--MinimumTERel"));
}

void SliceSpoilerRel(void)
{
  DB_MSG(("-->SliceSpoilerRel"));

  UT_SetRequest("SliceSpoiler");
  backbone();

  DB_MSG(("<--SliceSpoilerRel"));
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

/* -----------------------------------------------------------
 * RF pulse related parameters
 * ----------------------------------------------------------*/

void ExcPulEnumRel(void)
{
  DB_MSG(("-->ExcPulEnumRel"));

  UT_SetRequest("ExcPulEnum");
  backbone();

  DB_MSG(("<--ExcPulEnumRel"));
}

void ExcPulRange(void)
{
  DB_MSG(("-->ExcPulRange"));

  ExcPul.Flipangle = MIN_OF(90.0, ExcPul.Flipangle);

  DB_MSG(("<--ExcPulRange"));
}

void ExcPulRel(void)
{
  DB_MSG(("-->ExcPulRel"));

  UT_SetRequest("ExcPul");
  ExcPulRange();
  backbone();

  DB_MSG(("<--ExcPulRel"));
}

void ExcPulAmplRel(void)
{
  DB_MSG(("-->ExcPulAmplRel"));

  UT_SetRequest("ExcPulAmpl");
  HandleRFPulseAmplitudes();

  DB_MSG(("-->ExcPulAmplRel"));
}

void RefPulEnumRel(void)
{
  DB_MSG(("-->RefPulEnumRel"));

  UT_SetRequest("RefPulEnum");
  backbone();

  DB_MSG(("<--RefPulEnumRel"));
}

void RefPulRel(void)
{
  DB_MSG(("-->RefPulRel"));

  UT_SetRequest("RefPul");
  RefPulRange();
  backbone();

  DB_MSG(("<--RefPulRel"));
}

void RefPulRange(void)
{
  DB_MSG(("-->RefPulRange"));

  RefPul.Flipangle = MIN_OF(180.0, RefPul.Flipangle);

  DB_MSG(("<--RefPulRange"));
}

void RefPulAmplRel(void)
{
  DB_MSG(("-->RefPulAmplRel"));

  UT_SetRequest("RefPulAmpl");
  HandleRFPulseAmplitudes();

  DB_MSG(("-->RefPulAmplRel"));
}

void HandleRFPulseAmplitudes(void)
{
  DB_MSG(("-->HandleRFPulseAmplitude"));

  STB_UpdateRFShapeAmplitude("ExcPulAmpl,RefPulAmpl", No);
  ATB_SetRFPulse("ExcPul", "ACQ_RfShapes[0]");
  ATB_SetRFPulse("RefPul", "ACQ_RfShapes[1]");

  DB_MSG(("<--HandleRFPulseAmplitude"));
}


void HandleScan(void)
{

  std::string modpar=UT_RelContext_ParName(true,false,true);

  if(!modpar.find("PVM_AcqScanHandler"))
    ParxRelsParRelations("PVM_AcqScanHandler", MODALITY_HIERARCHY);
  STB_SpiralCreateShapes();
}

