/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2004 - 2011
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
#include <libCore/Math/Math.hh>


/*--------------------------------------------------------
 *
 * method specific range checkers and relations
 *
 *-------------------------------------------------------*/

void MyRgInitRel(void)
{
  DB_MSG(("-->MyRgInitRel"));
  
  InitBeforeAcquisition(1, 1);
  
  ACQ_SetupOnlineReco = No;

  ParxRelsParRelations("PVM_AutoRgInitHandler",Yes);
  
  DB_MSG(("<--MyRgInitRel"));
}

void BandWidthRel(void)
{
  DB_MSG(("-->BandWidthRel"));

  UT_SetRequest("PVM_EffSWh");
  backbone();
  
  DB_MSG(("<--BandWidthRel"));
}

void HalfAcqRel(void)
{
  DB_MSG(("-->HalfAcqRel"));

  /* avoid extra acquisition */
  if (HalfAcquisition == No)
  {
    UseOrigin = No;
  }
  
  backbone();

  DB_MSG(("<--HalfAcqRel"));
}
