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
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$(C) 2002 Bruker BioSpin MRI GmbH";


#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"






/*===========================================================
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
  
  // range checker fields to be controlled may be
  // .Length  
  // .Bandwidth
  // .Flipangle
  // .Calculated
  // .Sharpness
  // .Flatness   
 double dval=ExcPulse1.Flipangle;
  
 ExcPulse1.Flipangle = MIN_OF(90.0,MAX_OF(dval,1.0));

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
  
  /* 
   *  Tell the request handling system that the parameter
   *  RefPulse has been edited 
   */

  UT_SetRequest("RefPulse1");

  /* Check the values of RefPulse1 */
  RefPulse1Range();
  
  /* 
   * call the backbone; further handling will take place 
   * there (by means of STB_UpdateRFPulse)  
   */
  
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


/* =============================================================
 * relations of EffectiveTE (local parameter)
 * ============================================================ */
void effTErels(void)
{
  double denom;

  DB_MSG(("-->effTErels\n"));

  if(ParxRelsParHasValue("PVM_EncCentralStep1") && PVM_EncCentralStep1>=1)
    denom = PVM_EncCentralStep1;
  else
    denom = 1.0;
  
  if(denom)
    PVM_EchoTime = EffectiveTE[0]/denom;
  backbone();     

  DB_MSG(("<--effTErels\n"));
}





/*===============================================================
 *
 * Range checking routine for parameter PVM_NAverages
 *
 *==============================================================*/

void NAveragesRange(void)
{
  DB_MSG(("-->NAveragesRange\n"));
  
  
  if(ParxRelsParHasValue("PVM_NAverages") == No)
  {
    PVM_NAverages = 1;
  }
  
  if (PVM_NAverages > 1)
  {
    ParxRelsShowInEditor("PVM_MotionSupOnOff");
  }
  else
  {
    ParxRelsHideInEditor("PVM_MotionSupOnOff");
  }

  DB_MSG(("<--AveragesRange\n"));
}



void NAveragesRels(void)
{

  DB_MSG(("-->NAveragesRels\n"));


  NAveragesRange();

  /*
   *   Averages range check is finished, handle the request by
   *   the method:
   */
  
  backbone();
  
  
  DB_MSG(("<--NAveragesRels\n"));
  return;
}


/* rangechecking and redirected relations of PVM_EffSWh */

void EffSWhRange(void)
{
  DB_MSG(("-->EffSWhRange"));
  double minmax[2]={2000,1000000};

  if(!ParxRelsParHasValue("PVM_EffSWh"))
    PVM_EffSWh=100000;

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

/* relation of repetition spoiler */
void RepetitionSpoilerRel(void)
{
  DB_MSG(("-->RepetitionSpoilerRel"));
  UT_SetRequest("RepetitionSpoiler");
  backbone();
  DB_MSG(("<--RepetitionSpoilerRel"));
}

void MyRgInitRel(void)
{
  int i,dim;
  
  DB_MSG(("-->MyRgInitRel"));

  /* perform RG adjustment with largest TR */
  dim = (int) PARX_get_dim("MultiRepTime",1);
  UT_MaxOfDoubleArray(dim,MultiRepTime,&i);
  MultiRepTime[0] = MultiRepTime[i];
  RepTimeMode =Enter_RepTime;
  T1Exp = 1;
  backbone();

  /* force default behaviour */
  ParxRelsParRelations("PVM_AutoRgInitHandler",Yes);

  DB_MSG(("<--MyRgInitRel"));
}
/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
