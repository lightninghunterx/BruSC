/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2010
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2010 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"


void initMeth( void )
/*:=MPE=:=======================================================*/
{
  DB_MSG(( "--> initMeth()\n" ));

  ParxRelsMakeEditable("PVM_FrqAdj1H,PVM_FrqAdj1HPpm,PVM_NucleiPpmRef");
  ParxRelsMakeNonEditable("PVM_AcquisitionTime");

  EffSWhRange();
  AcqSizeRange();

  STB_InitNuclei(1);

  if(!ParxRelsParHasValue("PVM_TriggerModule"))
  {
    PVM_TriggerModule=Off;
  }
  PVM_TriggerMode   = per_PhaseStep;
  ParxRelsMakeNonEditable("PVM_TriggerMode");

  STB_InitTriggerModule();

  FreqResRange();
  NexcRange();

  ParxRelsMakeNonEditable("PVM_EffSWh,AcqSize,FreqAdjRange");
 
  if(!ParxRelsParHasValue("PVM_RepetitionTime"))
  {
    PVM_RepetitionTime= 300.0;
  }

  /* Initialize multi receiver parameters */
  
  STB_InitEncoding();

  // initialize parameters of result parameter card

  if(!ParxRelsParHasValue("O1"))
  {
    O1=0.0;
  }

  STB_InitAtsReferencePosition();

  // prevent reaction outside adjustment platform

  ParxRelsMakeNonEditable("ApplyManAdjOffset,FrqResetCfg");
  STB_InitRFPulse("ExcPulse1",
                  "ExcPulse1Enum",
                  "ExcPulse1Ampl",
                  0,
                  0,
                  "bp.exc",
                  15000.0,
                  90.0);
 
  UpdateRfPulsePars();

  if (CFG_isIcon() == Yes)
    PVM_DeMagnetizeOnOff = On;
  else
    PVM_DeMagnetizeOnOff = Off;

  DB_MSG(( "<-- initMeth()\n" ));

}

