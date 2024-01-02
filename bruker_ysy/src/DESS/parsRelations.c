/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002 - 2003
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


void MyRgInitRel(void)
{
  DB_MSG(("-->MyRgInitRel"));
  
  const int dim = PTB_GetSpatDim();
  if(dim > 1)
  {
    PVM_EncMatrix[1] = 32;
    SetBaseLevelParam();
    SetRecoParam();
  }

  ParxRelsParRelations("PVM_AutoRgInitHandler",Yes);

  DB_MSG(("<--MyRgInitRel"));
}


/* relations of DESS parameters */

void DESS_TE1_Range(void)
{
  DB_MSG(("-->DESS_TE1_Range"));
  
  if(ParxRelsParHasValue("DESS_TE1") == No)
  {
    DESS_TE1 = 2.5;
  }
  else
  {
    DESS_TE1=MAX_OF(MIN_OF(DESS_TE1,100.0),0.1);
  }
  
  DB_MSG(("<--DESS_TE1_Range"));
}

void DESS_TE1_Rel(void)
{
    DESS_TE1_Range();
    backbone();
}

void DESS_TE2_Range(void)
{
  DB_MSG(("-->DESS_TE2_Range"));
  
  if(ParxRelsParHasValue("DESS_TE2") == No)
  {
    DESS_TE2 = 2.5;
  }
  else
  {
    DESS_TE2 = MAX_OF(MIN_OF(DESS_TE2,100.0),0.1);
  }
  
  DB_MSG(("<--DESS_TE2_Range"));
}

void DESS_TE2_Rel(void)
{
    DESS_TE2_Range();
    backbone();
}

void MapSnrRange(void)
{

  if(!ParxRelsParHasValue("MapSnr"))
  {
    MapSnr=5;
  }
  else
  {
    MapSnr=MIN_OF(MAX_OF(MapSnr,2),1000);
  }

}

void MapSnrRel(void)
{
  MapSnrRange();
}

void FWInphaseRange(void)
{
  if(!ParxRelsParHasValue("FWInphase"))
  {
    FWInphase=Yes;
  }
}

void FWInphaseRel(void)
{
  FWInphaseRange();
  backbone();
}


/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/








