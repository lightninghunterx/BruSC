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


#include "method.h"

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

  STB_UpdateRFShapeAmplitude("ExcPulse1Ampl, RefPulse1Ampl",No);
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
  DB_MSG(("-->ExcPulse1Relation"));

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

/*===========================================================
 * The update of slice geometry is done in backbone.
 * This function connects the Fov in 3rd spatial dimension
 * to the slice thickness.
 *==========================================================*/


/* relations of Spoiler */
void SpoilerRel(void)
{
  DB_MSG(("-->SpoilerRel"));
  UT_SetRequest("Spoiler");
  backbone();
  DB_MSG(("<--SpoilerRel"));
}


void AverageModeRels(void)
{
  AverageModeRange();
  backbone();
}

void AverageModeRange(void)
{
  if(!ParxRelsParHasValue("AverageMode") ||
     (AverageMode != Weighted_av && AverageMode != Standard_av) )
  {
    AverageMode = Weighted_av;
  }
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


void KFilteringRange(void)
{
  if(!ParxRelsParHasValue("KFiltering"))
     KFiltering=On;
}

void KFilteringRelation(void)
{
  KFilteringRange();
  SetRecoParam();
  //call of backbone not allowed for reco params
}

/*===========================================================
 * Receiver gain adjustment.
 *==========================================================*/

void LocalAdjHandler(void)
{
  DB_MSG(("-->LocalAdjHandler"));

  if (Yes==STB_AtsAdj()) {
      return;
  }

  /* array phase adjustment */
  if (STB_ArrayPhaseHandleAdjustmentRequests() == 1)
  {
    // switch off water suppression here and call backbone
    PVM_WsOnOff = Off;
    ParxRelsParRelations("PVM_WsOnOff",No);
    PVM_NavOnOff = Off;
    ParxRelsParRelations("PVM_NavOnOff",Yes);
    PVM_NAverages = 1;
    ParxRelsParRelations("PVM_NAverages",No);
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
    PVM_NAverages = 1;
    ParxRelsParRelations("PVM_NAverages",No);
    backbone();
    ParxRelsParRelations("PVM_AdjHandler", Yes);
  }

  DB_MSG(("<--LocalAdjHandler"));
}


/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
