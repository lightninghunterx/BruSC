 /****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999-2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2003 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"


/*===============================================================
 * ExcPulse1EnumRelation
 * Relation of ExcPulse1Enum (a dynamic enumeration parameter which
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


/*** Inversion Pulse ***/

void InvPulse1EnumRelation(void)
{
  DB_MSG(("-->InvPulse1EnumRelation"));

  UT_SetRequest("InvPulse1Enum");
  backbone();
  
  DB_MSG(("<--InvPulse1EnumRelation"));                                       
}


void InvPulse1AmplRel(void)
{
  DB_MSG(("-->InvPulse1AmplRel"));
  UT_SetRequest("InvPulse1Ampl");
  HandleInvPulseAmplitude();
  DB_MSG(("-->InvPulse1AmplRel"));
}

void HandleInvPulseAmplitude(void)
{
  DB_MSG(("-->HandleInvPulseAmplitude"));

  STB_UpdateRFShapeAmplitude("InvPulse1Ampl",No);
  ATB_SetRFPulse("InvPulse1","ACQ_RfShapes[4]");

  DB_MSG(("<--HandleInvPulseAmplitude"));
}


void InvPulse1Relation(void)
{
  DB_MSG(("-->ExcPulse1Relation"));
    
  UT_SetRequest("InvPulse1");
  InvPulse1Range();
  backbone();
  
  DB_MSG(("<--ExcPulse1Relation"));
}

void InvPulse1Range(void)
{
  DB_MSG(("-->InvPulse1Range"));
  
  double dval=InvPulse1.Flipangle;
  
  InvPulse1.Flipangle = MIN_OF(180.0,MAX_OF(dval,180.0));

  DB_MSG(("<--InvPulse1Range")); 
}


/* ---------------------------------------------------------------
   redirected relations of PVM_RepetitionTime 
   --------------------------------------------------------------*/
void repTimeRels(void)
{
  /* PVM_EchoTime = PVM_RepetitionTime/2.0 - PVM_AcquisitionTime*(50.0-PVM_EchoPosition)/100.0; */
  
  if(Ssfp==TRUE_FISP)
  {
    PVM_EchoTime = PVM_RepetitionTime/2.0;
  }
  backbone();
}

void InvRepTimeRel(void)
{
  DB_MSG(("-->InvRepTimeRel"));
  
  UT_SetRequest("Inv_Rep_time");
  backbone();

  DB_MSG(("<--InvRepTimeRel")); 
}

void ScanRepTimeRel(void)
{
  DB_MSG(("-->ScanRepTimeRel"));
  
  UT_SetRequest("Scan_RepetitionTime");
  backbone();

  DB_MSG(("<--ScanRepTimeRel")); 
}

void LocalSWhRange(void)
{
  DB_MSG(("-->LocalSWhRange"));

  double minmax[2]={2000,1000000};

  if(!ParxRelsParHasValue("PVM_EffSWh"))
    PVM_EffSWh=100000;

  MRT_GetNearestBandwidthInHz("PVM_EffSWh",minmax);

  DB_MSG(("<--LocalSWhRange"));
}

void LocalSWhRels(void)
{
  DB_MSG(("-->LocalSWhRel"));

  LocalSWhRange();
  backbone();

  DB_MSG(("<--LocalSWhRel"));
  return;
}

void MyEchoPosRange(void)
{
  DB_MSG(("-->MyEchoPosRange"));
  if(!ParxRelsParHasValue("PVM_EchoPosition"))
  {
    PVM_EchoPosition=50.0;
  }
  else
  {
    double dval;
    
    dval = PVM_EchoPosition;
    PVM_EchoPosition = MIN_OF(MAX_OF(dval,0),50);
  }

  DB_MSG(("<--MyEchoPosRange"));
}


void MyEchoPosRel(void)
{
  DB_MSG(("-->MyEchoPosRel"));
  MyEchoPosRange();
  backbone();
  
  DB_MSG(("<--MyEchoPosRel"));
}


void NumPrepRange(void)
{
  DB_MSG(("-->NumPrepRange"));
  if (!ParxRelsParHasValue("NumPrep"))
  {
    NumPrep = 8;
  }
  else
  {
    NumPrep = MAX_OF(0,NumPrep);
  }
  DB_MSG(("<--NumPrepRange: %d", NumPrep));

}

void NumPrepRel(void)
{
  NumPrepRange();
  backbone();
}


void SSFPPrepRange(void)
{
  DB_MSG(("-->SSFPPrepRange"));

  if(!ParxRelsParHasValue("Ssfp_preparation"))
  {
    Ssfp_preparation = Starter_Sequence;
  }

  switch(Ssfp_preparation)
  {
  default:
    Ssfp_preparation = Alpha_2;
  case Alpha_2:
    ParxRelsShowInEditor("Alpha2_Prep_time");
    ParxRelsHideGroupInEditor("StarterSequence");
    ParxRelsShowInEditor("PVM_DummyScans,PVM_DummyScansDur");
    break;
  case No_Preparation:
    ParxRelsHideGroupInEditor("StarterSequence");
    ParxRelsShowInEditor("PVM_DummyScans,PVM_DummyScansDur");
    ParxRelsHideInEditor("Alpha2_Prep_time");
    break;

  case Starter_Sequence:
    ParxRelsShowGroupInEditor("StarterSequence");
    ParxRelsHideInEditor("PVM_DummyScans,PVM_DummyScansDur");
    ParxRelsHideInEditor("Alpha2_Prep_time");
     break;
  }

  DB_MSG(("<--SSFPPrepRange"));
}

void SSFPPrepRel(void)
{
  DB_MSG(("-->SSFPPrepRel"));

  SSFPPrepRange();
  backbone();

  DB_MSG(("<--SSFPPrepRel"));
}


void SliceSpoilerRel(void)
{
  DB_MSG(("-->SliceSpoilerRel"));
  UT_SetRequest("SliceSpoiler");
  backbone();
  DB_MSG(("<--SliceSpoilerRel"));
}

void MyRgInitRel(void)
{
  DB_MSG(("-->MyRgInitRel"));
 
  PVM_RareFactor = 1;
  SetBaseLevelParam();
  SetRecoParam();
  
  ParxRelsParRelations("PVM_AutoRgInitHandler",Yes);
  
  DB_MSG(("<--MyRgInitRel"));
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/


