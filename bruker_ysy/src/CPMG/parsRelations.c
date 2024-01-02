/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002 -2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2002 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0


/****************************************************************/
/****************************************************************/
/*		I N T E R F A C E   S E C T I O N		*/
/****************************************************************/
/****************************************************************/

/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "method.h"

/****************************************************************/
/*	I M P L E M E N T A T I O N   S E C T I O N		*/
/****************************************************************/


/****************************************************************/
/*		G L O B A L   F U N C T I O N S			*/
/****************************************************************/


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
  HandleRFPulseAmplitudes();
  DB_MSG(("-->ExcPulse1AmplRel"));
}

void HandleRFPulseAmplitudes(void)
{
  DB_MSG(("-->HandleRFPulseAmplitude"));

  STB_UpdateRFShapeAmplitude("ExcPulse1Ampl,RefPulse1Ampl",No);
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]");
  ATB_SetRFPulse("RefPulse1","ACQ_RfShapes[1]");

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
  DB_MSG(("-->ExcPulse1Relation\n"));

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

  DB_MSG(("<--ExcPulseRelation\n"));
}

void ExcPulse1Range(void)
{
  DB_MSG(("-->ExcPulse1Range"));

  // fields to be controlled may be
  // .Length
  // .Bandwidth
  // .Flipangle
  // .Calculated
  // .Sharpness
  // .Flatness
 double dval=ExcPulse1.Flipangle;

 ExcPulse1.Flipangle = MIN_OF(180.0,MAX_OF(dval,1.0));

 DB_MSG(("<--ExcPulseRange"));
}

/* --------------------------------------------------------------
   Relation of RefPulseEnum
   --------------------------------------------------------------*/

void RefPulse1EnumRelation(void)
{
  DB_MSG(("-->RefPulse1EnumRelation"));

  UT_SetRequest("RefPulse1Enum");
  backbone();

  DB_MSG(("<--RefPulse1EnumRelation"));
}

/* --------------------------------------------------------------
   Relation of RefPulse1Ampl
   --------------------------------------------------------------*/

void RefPulse1AmplRel(void)
{
  DB_MSG(("-->RefPulse1AmplRel"));
  UT_SetRequest("RefPulse1Ampl");
  HandleRFPulseAmplitudes();
  DB_MSG(("-->RefPulse1AmplRel"));
}

/* -----------------------------------------------------------
   Relation of RefPulse
   -----------------------------------------------------------*/
void RefPulse1Relation(void)
{
  DB_MSG(("-->RefPulse1Relation\n"));

  UT_SetRequest("RefPulse1");
  RefPulse1Range();
  backbone();

  DB_MSG(("<--RefPulseRelation\n"));
}

void RefPulse1Range(void)
{
  DB_MSG(("-->RefPulse1Range"));

  double dval=RefPulse1.Flipangle;

  RefPulse1.Flipangle = MIN_OF(270,MAX_OF(dval,1));

  DB_MSG(("<--RefPulse1Range\n"));
}

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


void NrepRange(void)
{
  if(ParxRelsParHasValue("PVM_NRepetitions") ==No)
  {
    PVM_NRepetitions = 1;
  }
  else
  {
     PVM_NRepetitions = MAX_OF(1,PVM_NRepetitions);
  }

}

void NrepRel(void)
{
  NrepRange();
  backbone();
}

void SpecHandler(void)
{
  DB_MSG(("-->SpecHandler\n"));

  backbone();

  DB_MSG(("<--SpecHandler\n"));
}

void LocalAdjHandler(void)
{
  DB_MSG(("-->LocalAdjHandler"));

  if (Yes==STB_AtsAdj()) {
      return;
  }

  /* array phase adjustment */
  if(STB_ArrayPhaseHandleAdjustmentRequests() == 1)
  {
    PVM_NAverages = 1;
    NEchoes = 1;
    backbone();
    return;
  }

  /* non-method-specific adjustment */
  if (PTB_AdjMethSpec() == No)
  {
    ParxRelsParRelations("PVM_AdjHandler", Yes);
    return;
  }

  /* receiver gain adjustment */
  if (0 == strcmp(PTB_GetAdjName(), RG_ADJNAME))
  {
    ParxRelsParRelations("PVM_AdjHandler", Yes);
  }

  DB_MSG(("<--LocalAdjHandler"));
}

void SliceSpoilerRel(void)
{
  DB_MSG(("-->SliceSpoilerRel"));
  UT_SetRequest("SliceSpoiler");
  backbone();
  DB_MSG(("<--SliceSpoilerRel"));
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
