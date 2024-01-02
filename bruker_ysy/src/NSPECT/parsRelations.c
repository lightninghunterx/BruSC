/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002-2011
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
#include <libCore/Math/Math.hh>



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

  /* The minimum delay between RF pulse and ACQ_START is given by 20us
   * which are included in the acqdec-subroutine before acquisition.
   * (these 20us include the DE delay).
   * 1us is added to avoid rounding problems -> min=21us */

  double min = 0.021;
  DeadTime = MAX_OF(DeadTime, min);
  DeadTime = MIN_OF(5.0, DeadTime);
}

void updateDeadTime(void)
{
  /* The minimum delay between RF pulse and ACQ_START is given by 10us
   * which are included in the acqdec-subroutine before acquisition.
   * (these 10us include the DE delay).*/

  double min = 0.011 ;

  DeadTime =  MAX_OF(DeadTime, min);
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
 * Relation of ExcPulse1Ampl
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
  DB_MSG(("-->HandleExcPulse1Amplitude"));

  STB_UpdateRFShapeAmplitude("ExcPulse1Ampl",No);
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]");

  DB_MSG(("<--HandleExcPulse1Amplitude"));
}


/* ===================================================================
 * Relation of ExcPulse1
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

  /* non-method-specific adjustment */
  if (PTB_AdjMethSpec() == No)
  {
    DB_MSG(("No method specific adjustments"));
    ParxRelsParRelations("PVM_AdjHandler", Yes);
    return;
  }

  /* array phase adjustment */
  if (STB_ArrayPhaseHandleAdjustmentRequests() == 1)
  {
    DB_MSG(("ArrayPhaseAdjustment"));
    /*Turn water suppression pulses off*/
    PVM_WsOnOff = Off;
    ParxRelsParRelations("PVM_WsOnOff",No);
    PVM_NavOnOff = Off;
    ParxRelsParRelations("PVM_NavOnOff",Yes);
    Edc_OnOff = Off;
    ParxRelsParRelations("Edc_OnOff",Yes);
    PVM_NAverages = 1;
    ParxRelsParRelations("PVM_NAverages",No);
    backbone();
    return;
  }

  if(STB_RefScanHandleAdjustmentRequests() == 1)
  {
      PVM_DecPower = PVM_NoePower =
          CFG_RFPulseHighestAttenuation();
      PVM_WsOnOff = Off;
      ParxRelsParRelations("PVM_WsOnOff",No);
      Edc_OnOff = Off;
      ParxRelsParRelations("Edc_OnOff",No);
      PVM_NavOnOff = Off;
      ParxRelsParRelations("PVM_NavOnOff",No);
      backbone();
  }
  else if(0 == strcmp(PTB_GetAdjName(), RG_ADJNAME))
  {
      DB_MSG(("ReceiverGainAdjustment"));
      PVM_NAverages = 1;
      ParxRelsParRelations("PVM_NAverages",No);
      ParxRelsParRelations("PVM_AdjHandler", Yes);
      backbone();
  }



  DB_MSG(("<--LocalAdjHandler"));
}


/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
