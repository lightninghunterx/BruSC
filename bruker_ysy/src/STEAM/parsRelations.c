/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2006-2007
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 *
 * ***************************************************************/
static const char resid[] = "$Id$ (C) 2007 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	0


#include "method.h"

/*
 * simple local parameter relations 
 */

void AveragesRange(void)
{
  DB_MSG(("-->AveragesRange"));

  if(!ParxRelsParHasValue("PVM_NAverages"))
  {
    PVM_NAverages=1;
  }
  else
  {
    int actna;
    actna=PVM_NAverages;
    PVM_NAverages=MAX_OF(1,actna);
  }

  DB_MSG(("<--AveragesRange"));
}

void AveragesRel(void)
{
  DB_MSG(("-->AveragesRel"));

  AveragesRange();
  backbone();
  DB_MSG(("<--AveragesRel"));
}

void CalcLineWidthRange(void)
{
  if(!ParxRelsParHasValue("CalcLineWidth"))
     CalcLineWidth=Off;
}

void CalcLineWidthRelation(void)
{
  CalcLineWidthRange();
  SetRecoParam();
  //call of backbone not allowed for reco params
}



/*Navigator Relations and RangeChecker */

/*local Relation to react on Navigator Params for Reco*/

void RetroFrequencyLockRange(void)
{
  if(!ParxRelsParHasValue("RetroFrequencyLock_OnOff"))
     RetroFrequencyLock_OnOff=Off;
}

void RetroFrequencyLockRelation(void)
{
  RetroFrequencyLockRange();
  //call of backbone not allowed for reco params
}
