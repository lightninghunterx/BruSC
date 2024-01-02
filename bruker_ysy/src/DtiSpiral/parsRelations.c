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
#define DB_MODULE  0
#define DB_LINE_NR 0

#include "method.h"
#include <lib/PvSysinfoClient.h>

/* =============================================================
 * Global parameters
 * ============================================================= */

void EffSWhRange(void)
{
  DB_MSG(("-->EffSWhRange"));
  double minmax[2]={1000.0, 1000000.0};

  if (!ParxRelsParHasValue("PVM_EffSWh"))
    PVM_EffSWh = 200000.0;

  MRT_GetNearestBandwidthInHz("PVM_EffSWh", minmax);

  DB_MSG(("<--EffSWhRange"));
}

void EffSWhRel(void)
{
  DB_MSG(("-->EffSWhRel"));

  EffSWhRange();
  backbone();

  DB_MSG(("<--EffSWhRel"));
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

void HandleRFPulseAmplitudes(void)
{
  DB_MSG(("-->HandleRFPulseAmplitude"));

  STB_UpdateRFShapeAmplitude("ExcPulAmpl", No);
  ATB_SetRFPulse("ExcPul", "ACQ_RfShapes[0]");

  DB_MSG(("<--HandleRFPulseAmplitude"));
}


void HandleScan(void)
{
  std::string modpar=UT_RelContext_ParName(true,false,true);

  if(!modpar.find("PVM_AcqScanHandler"))
    ParxRelsParRelations("PVM_AcqScanHandler", MODALITY_HIERARCHY);

    STB_SpiralCreateShapes();
}


/* relations of TensorReconstruction parameters */

void RecoMethModeRel(void)
{
  DB_MSG(("-->RecoMethModeRel\n"));
  
  RecoMethModeRange();
  
  if(RecoMethMode==Default)
    ParxRelsHideInEditor("TraceWeight");
  else
    ParxRelsShowInEditor("TraceWeight");
  
  DB_MSG(("<--RecoMethModeRel\n"));  
}

void RecoMethModeRange(void)
{
  DB_MSG(("-->RecoMethModeRange\n")); 
    
  if(!ParxRelsParHasValue("TraceWeight"))
    TraceWeight = 1000.0;
  else
    TraceWeight = MAX_OF(0.0,TraceWeight);
  
  if(!ParxRelsParHasValue("RecoMethMode"))
    RecoMethMode=Default;

  // Require at least 7 diffusion experiments, from which 6 or more have different
  // diffusion directions (at least nominally).
  if((!ParxRelsParHasValue("PVM_DwAoImages") || !ParxRelsParHasValue("PVM_DwNDiffDir") ||
      PVM_DwNDiffDir + PVM_DwAoImages < 7 || PVM_DwNDiffDir < 6))
  {
    RecoMethMode = Default;
    ParxRelsMakeNonEditable("RecoMethMode");
  }
  else
  {
    ParxRelsMakeEditable("RecoMethMode");
  }
 
  DB_MSG(("<--RecoMethModeRange\n"));
}
