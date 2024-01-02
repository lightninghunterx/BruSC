/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/
 
static const char resid[] = "$Id$ (C) 2002 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

/*:=INFO=:*******************************************************
 *
 * Description :
 *   it contains functions necessary for responding to a 'load'
 *	of this method. This function will be automatically
 *	called in response to a method file for this method
 *	having been read.
 *
 *::=info=:******************************************************/

#include "method.h"


/**********************************************************************


		  TEMPLATE FOR SEQUENCE MODEL CONFORM
		        REQUEST HANDLING

     HERE:   Handling of PVM_NAverages


 *********************************************************************/


void Local_NAveragesRange(void)
{
  int ival;
  DB_MSG(("Entering Local_NAveragesRange"));
  
  /* 
   *  Range check of PVM_NAverages: prevent it to be negative or 0
   */

  if(ParxRelsParHasValue("PVM_NAverages") == No)
    {
      PVM_NAverages = 1;
    }

  ival = PVM_NAverages;
  PVM_NAverages = MAX_OF(ival,1);
  

  DB_MSG(("Exiting Local_NAveragesRange"));


}

void Local_NAveragesHandler(void)
{

  DB_MSG(("Exiting Local_NAveragesHandler with value %d",PVM_NAverages));

  Local_NAveragesRange();

  /*
   *   Averages range check is finished, handle the request by
   *   the method:
   */
  
  backbone();

  DB_MSG(("Exiting Local_NAveragesHandler with value %d",PVM_NAverages));
  return;
}

void EffSWhRange(void)
{
  DB_MSG(("-->EffSWhRange"));
  double minmax[2]={50000.0,1000000.0};

  if(!ParxRelsParHasValue("PVM_EffSWh"))
    PVM_EffSWh=200000.0;

  MRT_GetNearestBandwidthInHz("PVM_EffSWh",minmax);

  DB_MSG(("<--EffSWhRange"));
  return;
}
void EffSWhRels(void)
{
  DB_MSG(("-->EffSWhRels"));

  EffSWhRange();
  backbone();

  DB_MSG(("<--EffSWhRels"));
  return;
}

/* relations of NSegments */

void NSegmentsRels(void)
{
  NSegmentsRange();
  backbone();
}

void NSegmentsRange(void)
{
  if(!ParxRelsParHasValue("NSegments"))
    NSegments = 1;
  else
    NSegments = MAX_OF(1,NSegments);
}

/* Range checking and relation of Bandwidth Scaling Factor */

void BwScaleRange(void)
{
  DB_MSG(("-->BwScaleRange\n"));

  if(ParxRelsParHasValue("BwScale")==No)
  {
    BwScale=100.0;
  }
  else
  {
    BwScale=MAX_OF(MIN_OF(150,BwScale),50.0);
  }

  DB_MSG(("<--BwScaleRange\n"));
  return;
}

void BwScaleRel(void)
{
  DB_MSG(("-->BwScaleRel\n"));
  BwScaleRange();
  backbone();
  DB_MSG(("<--BwScaleRel\n"));
  return;
}

void SliceSpoilerRel(void)
{
  DB_MSG(("-->SliceSpoilerRel"));
  UT_SetRequest("SliceSpoiler");
  backbone();
  DB_MSG(("<--SliceSpoilerRel"));
}

void ExSliceRephaseTimeRange(void)
{
  DB_MSG(("-->ExSliceRephaseTimeRange"));
 
  if(!ParxRelsParHasValue("ExSliceRephaseTime"))
    ExSliceRephaseTime = 2.0;

  ExSliceRephaseTime = MAX_OF(ExSliceRephaseTime, 2*CFG_GradientRiseTime());
  
  DB_MSG(("<--ExSliceRephaseTimeRange"));
}

void ExSliceRephaseTimeRel(void)
{
  DB_MSG(("-->ExSliceRephaseTimeRel"));

  ExSliceRephaseTimeRange();
  backbone();

  DB_MSG(("<--ExSliceRephaseTimeRel"));
}
/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
