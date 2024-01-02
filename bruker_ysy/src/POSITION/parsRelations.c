/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002 - 2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2002 - 2003 Bruker BioSpin MRI GmbH";

#define DEBUG		0


#include "method.h"


/*==========================================================
 *
 *  examples for relations concearning special pulses and
 *  pulselists
 *
 *==========================================================*/

/*===============================================================
 * ExcPulse1EnumRelation
 * Relation of ExcPulse1Enum (a dynamic enmueration parameter which
 * allows to select one of the existing  pulses)
 *===============================================================*/

void ExcPulse1EnumRelation(void)
{
  DB_MSG(("-->ExcPulse1EnumRelation"));

  UT_SetRequest("ExcPulse1Enum");
  backbone();

  DB_MSG(("<--ExcPulse1EnumRelation"));
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

void ExcPulse1Relation(void)
{
  DB_MSG(("-->ExcPulse1Relation"));

  UT_SetRequest("ExcPulse1");

  /* Check the values of ExcPulse */
  ExcPulse1Range();

  backbone();

  DB_MSG(("<--ExcPulse1Relation"));
}


void ExcPulse1Range(void)
{
  DB_MSG(("-->ExcPulse1Range"));

  double dval=ExcPulse1.Flipangle;

  ExcPulse1.Flipangle = MIN_OF(90.0,MAX_OF(dval,1.0));

  DB_MSG(("<--ExcPulseRange"));
}


/*===============================================================
 *
 * Range checking routine for parameter PVM_NAverages
 *
 *==============================================================*/


void Local_NAveragesRange(void)
{
  int ival;
  DB_MSG(("-->Local_NAveragesRange\n"));

  /*
   *  Range check of PVM_NAverages: prevent it to be negative or 0
   */

  if(ParxRelsParHasValue("PVM_NAverages") == No)
  {
    PVM_NAverages = 1;
  }

  ival = PVM_NAverages;
  PVM_NAverages = MAX_OF(ival,1);


  DB_MSG(("<--Local_NAveragesRange\n"));

}

void Local_NAveragesHandler(void)
{

  DB_MSG(("-->Local_NAveragesRange\n"));


  Local_NAveragesRange();

  /*
   *   Averages range check is finished, handle the request by
   *   the method:
   */


  backbone();


  DB_MSG(("<--Local_NAveragesRange\n"));
  return;
}
void GradientDirectionRange(void)
{
  if(ParxRelsParHasValue("GradientDirection") == No)
    GradientDirection = Z_dir;

  switch (GradientDirection)
  {
   case Z_dir:
   break;
   case X_dir:
   break;
   case Y_dir:
   break;
   default:
   GradientDirection = Z_dir;
   break;
  }

}

void GradientDirectionRel(void)
  {
   GradientDirectionRange();
   backbone();
  }

/* rangechecking and redirected relations of PVM_EffSWh */

void EffSWhRange(void)
{
  DB_MSG(("-->EffSWhRange"));
  double minmax[2]={2000,1000000};

  if(!ParxRelsParHasValue("PVM_EffSWh"))
    PVM_EffSWh=50000;

  MRT_GetNearestBandwidthInHz("PVM_EffSWh",minmax);

  DB_MSG(("-->EffSWhRange"));
  return;
}

void EffSWhRel(void)
{
  DB_MSG(("-->EffSWhRel"));

  EffSWhRange();
  backbone();

  DB_MSG(("-->EffSWhRel"));
  return;
}

/*==============================================================
 * relation of NDummyScans
 *==============================================================*/


void dsRelations(void)
{

  DB_MSG(("-->dsRelations\n"));

  dsRange();
  backbone();

  DB_MSG(("<--dsRelations\n"));

}

void dsRange(void)
{
  if(ParxRelsParHasValue("NDummyScans") == No)
    NDummyScans = 0;
  else
    NDummyScans = MAX_OF(0, NDummyScans);
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/








