/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2004
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

#define DEBUG       0
#define DB_MODULE   0
#define DB_LINE_NR  0


#include "method.h"


void InitRFPulses(void)
{
  DB_MSG(("-->InitRFPulses"));
  
  if (!ParxRelsParHasValue("PVM_DeriveGains"))
  {
    PVM_DeriveGains = Yes;
  }
  
  if (!ParxRelsParHasValue("ExcPulseAuto"))
  {
    ExcPulseAuto = Yes;
  }
  
  STB_InitRFPulse("ExcPul",
                  NULL,
                  "ExcPulAmpl",
                  NULL,
                  0,
                  "bp.exc",
                  1.28e6,     // length = 1 us
                  5.0);
                  
  ExcPulRange();

  DB_MSG(("<--InitRFPulses"));
}


void UpdateRFPulses(void)
{
  DB_MSG(("-->UpdateRFPulses"));
  
  STB_UpdateRFPulse("ExcPul", 1, PVM_DeriveGains);
  DB_MSG(("ExcPul.Bandwidth = %f", ExcPul.Bandwidth));
  
  if (ExcPulseAuto && ExcPul.Bandwidth < PVM_EffSWh)
  {
    ExcPul.Bandwidth = PVM_EffSWh;
    UT_ClearRequest();
    STB_UpdateRFPulse("ExcPul", 1, PVM_DeriveGains);
    DB_MSG(("ExcPul.Bandwidth = %f", ExcPul.Bandwidth));
  }
  
  if (PVM_DeriveGains == Yes)
  {
    ParxRelsHideInEditor("ExcPulAmpl");
  }
  else
  {
    ParxRelsShowInEditor("ExcPulAmpl");
  }

  DB_MSG(("<--UpdateRFPulses"));
}

void ExcPulRel(void)
{
  DB_MSG(("-->ExcPulRel"));

  UT_SetRequest("ExcPul");
  ExcPulRange();
  backbone();

  DB_MSG(("<--ExcPulRel"));
}

void ExcPulRange(void)
{
  DB_MSG(("-->ExcPulRange"));
  
  ExcPul.Flipangle = MIN_OF(90.0, ExcPul.Flipangle);
  strcpy(ExcPul.Shape, "bp.exc");
  
  DB_MSG(("<--ExcPulRange"));  
}

void ExcPulAmplRel(void)
{
  DB_MSG(("-->ExcPulAmplRel"));
  
  UT_SetRequest("ExcPulAmpl");
  STB_UpdateRFShapeAmplitude("ExcPulAmpl", No);
  ATB_SetRFPulse("ExcPul", "ACQ_RfShapes[0]");
  
  DB_MSG(("<--ExcPulAmplRel"));  
}

