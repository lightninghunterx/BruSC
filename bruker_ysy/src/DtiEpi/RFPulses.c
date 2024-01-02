/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/


static const char resid[] = "$Id$ (C) 2003 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0



#include "method.h"



void InitRFPulses(void)
{
  DB_MSG(("-->InitRFPulses\n"));

  /* Initialisation of rf pulse parameters */
  
  STB_InitRFPulse("ExcPul",        // name of pulse structure
                  "ExcPulseEnum",  // name of pulse list parameter
                  "ExcPulseAmpl",  // name of pulse amplitude parameter
                  "ExcPulseShape", // name of pulse shape (for calc. pulses)
                  0,               // used for excitation
                  "Calculated",    // default shape
                  2000.0,          // default bandwidth
                  90.0);           // default pulse angle
  ExcPulRange();

  DB_MSG(("<--InitRFPulses\n"));
}


void UpdateRFPulses(YesNo deriveGains)
{
  DB_MSG(("-->UpdateRFPulses\n"));
  
  STB_UpdateRFPulse("ExcPul",1,deriveGains,Conventional);

  if(deriveGains==Yes)
  {
    ParxRelsHideInEditor("ExcPulseAmpl");
  }
  else
  {
    ParxRelsShowInEditor("ExcPulseAmpl");
  }
    
  DB_MSG(("<--UpdateRFPulses\n"));
}



/* -----------------------------------------------------------
   Relations of ExcPulse
 
   -----------------------------------------------------------*/
   
void ExcPulseEnumRelation(void)
{
  DB_MSG(("-->ExcPulseEnumRelation"));

  UT_SetRequest("ExcPulseEnum");
  backbone();
  
  DB_MSG(("<--ExcPulseEnumRelation"));                                       
}

void ExcPulRelation(void)
{
  DB_MSG(("-->ExcPulRelation"));
  
  UT_SetRequest("ExcPul");
  ExcPulRange();
  backbone();
  
  DB_MSG(("<--ExcPulRelation"));
}

void ExcPulRange(void)
{
  DB_MSG(("-->ExcPulse1Range"));
    
 ExcPul.Flipangle = MIN_OF(90.0,MAX_OF(ExcPul.Flipangle,1.0));

 DB_MSG(("<--ExcPulseRange")); 
}

void ExcPulseAmplRel(void)
{
  DB_MSG(("-->ExcPulseAmplRel"));
  
  UT_SetRequest("ExcPulseAmpl");
  HandleRFPulseAmplitudes();
  
  DB_MSG(("-->ExcPulseAmplRel"));
}

void HandleRFPulseAmplitudes(void)
{
  DB_MSG(("-->HandleRFPulseAmplitude"));

  STB_UpdateRFShapeAmplitude("ExcPulseAmpl",Yes);
  ATB_SetRFPulse("ExcPul","ACQ_RfShapes[0]");

  DB_MSG(("<--HandleRFPulseAmplitude"));
}





void DeriveGainsRange(void)
{
  DB_MSG(("-->DeriveGainsRange\n"));

  if(ParxRelsParHasValue("PVM_DeriveGains")==No)
  {
    PVM_DeriveGains = Yes;
  }
  else
  {
    switch(PVM_DeriveGains)
    {
    case No:
      break;
    default:
      PVM_DeriveGains = Yes;
    case Yes:
      break;
    }
  }


  DB_MSG(("<--DeriveGainsRange\n"));
  return;
}

void DeriveGainsRel(void)
{
  DB_MSG(("-->DeriveGainsRel\n"));

  DeriveGainsRange();
  backbone();

  DB_MSG(("<--DeriveGainsRel\n"));
  return;
}
