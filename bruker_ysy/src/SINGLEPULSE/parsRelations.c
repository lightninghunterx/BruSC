/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002 -2003
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
#define DB_MODULE	1
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



/* ------------------------------------------------------------
   relations of DeadTime
   -------------------------------------------------------------*/
void DeadTimeRels(void)
{
  DeadTimeRange();
  backbone();
}

void DeadTimeRange(void)
{
  if(ParxRelsParHasValue("DeadTime") == No)
    DeadTime = 0.05;
  DeadTime = MAX_OF(0.02, DeadTime);
  DeadTime = MIN_OF(5.0, DeadTime);
}

void updateDeadTime(void)
{
  double min_us;

  /* The minimum delay between RF pulse and ACQ_START is given by the
   * base-level parameter DE. This parameter was set during the update of
   * spectorscopy. */

  min_us = DE+1.0; //+1us margin

  DeadTime =  MAX_OF(DeadTime, min_us*1e-3);
}

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

  /* Tell the request handling system that the parameter
     ExcPulse1 has been edited */
  UT_SetRequest("ExcPulse1");

  /* Check the values of ExcPulse1 */
  ExcPulse1Range();

  /* call the backbone; further handling will take place there
     (by means of STB_UpdateRFPulse)  */

  backbone();

  DB_MSG(("-->ExcPulse1Relation"));
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

  DB_MSG(("<--ExcPulse1Range"));
}


/****************************************************************/
/*	         L O C A L   F U N C T I O N S			*/
/****************************************************************/


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


void LocalAdjHandler(void)
{
  DB_MSG(("-->LocalAdjHandler"));

  if (Yes==STB_AtsAdj()) {
      return;
  }

  if(No == PTB_AdjMethSpec())
  {
    DB_MSG(("<--HandleAdjustmentRequests: no method specific adjustment desired"));
    ParxRelsParRelations("PVM_AdjHandler",Yes);
    return;
  }

  /* array phase adjustment */
  if (STB_ArrayPhaseHandleAdjustmentRequests() == 1)
  {
    PVM_NAverages = 1;
    ParxRelsParRelations("PVM_NAverages",No);
    backbone();
    return;
  }

  /* receiver gain adjustment */
  const char * adjName = PTB_GetAdjName();
  if(!strcmp(adjName, RG_ADJNAME))
  {
    PVM_NAverages = 1;
    backbone();
    ParxRelsParRelations("PVM_AdjHandler", Yes);
  }

  DB_MSG(("<--LocalAdjHandler"));
}

void LocalAdjResultHandler()
{
  ParxRelsParRelations("PVM_AdjResultHandler",MODALITY_HIERARCHY);
  backbone();
}


/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
