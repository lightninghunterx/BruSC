/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2006
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 *
 * ***************************************************************/
static const char resid[] = "$Id$ (C) 2006 Bruker BioSpin MRI GmbH";


#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	0



#include "method.h"

void InitRFPulses(void)
{
  DB_MSG(("-->InitRFPulses"));

  DeriveGainsRange();
  VoxPul1Range();
  VoxPul2Range();
  VoxPul3Range();

  if(!ParxRelsParHasValue("LockVoxPulAmpls"))
    LockVoxPulAmpls = No;

  DB_MSG(("<--InitRFPulses"));
}

void UpdateRFPulses(void)
{

  DB_MSG(("-->UpdateRFPulses"));

  STB_UpdateRFPulse("VoxPul1", 1, PVM_DeriveGains, Conventional);
  STB_UpdateRFPulse("VoxPul2", 1, PVM_DeriveGains, Conventional);
  STB_UpdateRFPulse("VoxPul3", 1, PVM_DeriveGains, Conventional);

  if(PVM_DeriveGains==Yes)
  {
    ParxRelsHideInEditor("VoxPul1Ampl");
    ParxRelsHideInEditor("VoxPul2Ampl");
    ParxRelsHideInEditor("VoxPul3Ampl");
    ParxRelsHideInEditor("LockVoxPulAmpls");
  }
  else
  {
    ParxRelsShowInEditor("VoxPul1Ampl");
    ParxRelsShowInEditor("VoxPul2Ampl");
    ParxRelsShowInEditor("VoxPul3Ampl");
    ParxRelsShowInEditor("LockVoxPulAmpls");
  }

  ParxRelsShowInFile("VoxPul1Ampl");
  ParxRelsShowInFile("VoxPul2Ampl");
  ParxRelsShowInFile("VoxPul3Ampl");

  DB_MSG(("<--UpdateRFPulses"));
}



void VoxPul1Range(void)
{
  DB_MSG(("-->VoxPul1Range"));


  if(!ParxRelsParHasValue("VoxPul1"))
  {

    STB_InitRFPulse("VoxPul1",      // name of pulse structure
                    "VoxPul1Enum",  // name of pulse list parameter
                    "VoxPul1Ampl",  // name of pulse amplitude parameter
                    "VoxPul1Shape", // name of pulse shape (for calc. pulses)
                    0,              // used for excitation
                    "Calculated",  // default shape
                    5600.0,         // default bandwidth
                    90.0);          // default pulse angle
  }
  
  /* allowed flipangle for this pulse */
  VoxPul1.Flipangle = MIN_OF(MAX_OF(1.0,VoxPul1.Flipangle),90.0);

  DB_MSG(("<--VoxPul1Range"));
}

void VoxPul2Range(void)
{
  DB_MSG(("-->VoxPul2Range"));


  if(!ParxRelsParHasValue("VoxPul2"))
  {

    STB_InitRFPulse("VoxPul2",      // name of pulse structure
                    "VoxPul2Enum",  // name of pulse list parameter
                    "VoxPul2Ampl",  // name of pulse amplitude parameter
                    "VoxPul2Shape", // name of pulse shape (for calc. pulses)
                    0,              // used for excitation
                    "Calculated",  // default shape
                    5600.0,         // default bandwidth
                    90.0);          // default pulse angle
  }
  
  /* allowed flipangle for this pulse */
  VoxPul2.Flipangle = MIN_OF(MAX_OF(1.0,VoxPul2.Flipangle),90.0);

  DB_MSG(("<--VoxPul2Range"));
}

void VoxPul3Range(void)
{
  DB_MSG(("-->VoxPul3Range"));


  if(!ParxRelsParHasValue("VoxPul3"))
  {

    STB_InitRFPulse("VoxPul3",      // name of pulse structure
                    "VoxPul3Enum",  // name of pulse list parameter
                    "VoxPul3Ampl",  // name of pulse amplitude parameter
                    "VoxPul3Shape", // name of pulse shape (for calc. pulses)
                    0,              // used for excitation
                    "Calculated",  // default shape
                    5600.0,         // default bandwidth
                    90.0);          // default pulse angle
  }
     
  /* allowed flipangle for this pulse */
  VoxPul3.Flipangle = MIN_OF(MAX_OF(1.0,VoxPul3.Flipangle),90.0);

  DB_MSG(("<--VoxPul3Range"));
}


void VoxPul1Rel(void)
{
  DB_MSG(("-->VoxPul1Rel"));

  UT_SetRequest("VoxPul1");
  VoxPul1Range();
  backbone();

  DB_MSG(("<--VoxPul1Rel"));
}

void VoxPul2Rel(void)
{
  DB_MSG(("-->VoxPul2Rel"));

  UT_SetRequest("VoxPul2");
  VoxPul2Range();
  backbone();

  DB_MSG(("<--VoxPul2Rel"));
}

void VoxPul3Rel(void)
{
  DB_MSG(("-->VoxPul3Rel"));

  UT_SetRequest("VoxPul3");
  VoxPul3Range();
  backbone();

  DB_MSG(("<--VoxPul3Rel"));
}


void VoxPul1EnumRel(void)
{
  DB_MSG(("-->VoxPul1EnumRelation"));

  UT_SetRequest("VoxPul1Enum");
  backbone();
  
  DB_MSG(("<--VoxPul1EnumRelation"));                                       
}

void VoxPul2EnumRel(void)
{
  DB_MSG(("-->VoxPul2EnumRelation"));

  UT_SetRequest("VoxPul2Enum");
  backbone();
  
  DB_MSG(("<--VoxPul2EnumRelation"));                                       
}

void VoxPul3EnumRel(void)
{
  DB_MSG(("-->VoxPul3EnumRelation"));

  UT_SetRequest("VoxPul3Enum");
  backbone();
  
  DB_MSG(("<--VoxPul3EnumRelation"));                                       
}

void NavPulEnumRel(void)
{
  DB_MSG(("-->NavPulEnumRelation"));

  UT_SetRequest("NavPulEnum");
  backbone();
  
  DB_MSG(("<--NavPulEnumRelation"));                                       
}

void VoxPul1AmplRel(void)
{
  DB_MSG(("-->VoxPul1AmplRel"));
  UT_SetRequest("VoxPul1Ampl");
  HandleVoxPulAmplitude();
  DB_MSG(("-->VoxPul1AmplRel"));
}

void VoxPul2AmplRel(void)
{
  DB_MSG(("-->VoxPul2AmplRel"));
  UT_SetRequest("VoxPul2Ampl");
  HandleVoxPulAmplitude();
  DB_MSG(("-->VoxPul2AmplRel"));
}

void VoxPul3AmplRel(void)
{
  DB_MSG(("-->VoxPul3AmplRel"));
  UT_SetRequest("VoxPul3Ampl");
  HandleVoxPulAmplitude();
  DB_MSG(("-->VoxPul3AmplRel"));
}

void NavPulAmplRel(void)
{
  DB_MSG(("-->NavPulAmplRel"));
  UT_SetRequest("NavPulAmpl");
  HandleVoxPulAmplitude();
  DB_MSG(("-->NavPulAmplRel"));
}

void HandleVoxPulAmplitude(void)
{
  DB_MSG(("-->HandleVoxPulAmplitude"));
  STB_UpdateRFShapeAmplitude("VoxPul1Ampl,VoxPul2Ampl,VoxPul3Ampl", LockVoxPulAmpls);
  ATB_SetRFPulse("VoxPul1","ACQ_RfShapes[1]");
  ATB_SetRFPulse("VoxPul2","ACQ_RfShapes[2]");
  ATB_SetRFPulse("VoxPul3","ACQ_RfShapes[3]");
  DB_MSG(("<--HandleVoxPulAmplitude"));
}

void LockVoxPulAmplsRelation(void)
{
  DB_MSG(("-->LockVoxPulAmplsRelation"));
  /*HandleVoxPulAmplitude();*/
  DB_MSG(("-->LockVoxPulAmplsRelation"));
}


void DeriveGainsRel(void)
{
  DB_MSG(("-->DeriveGainsRel"));
  
  DeriveGainsRange();
  backbone();

  DB_MSG(("<--DeriveGainsRel"));
  return;
}

void DeriveGainsRange(void)
{
  DB_MSG(("-->DeriveGainsRange"));

  if(!ParxRelsParHasValue("PVM_DeriveGains"))
  {
    PVM_DeriveGains = Yes;
  }
  else
  {
    switch(PVM_DeriveGains)
    {
      default:
        PVM_DeriveGains=Yes;
      case Yes: case No:
        break;
    }
  }

  DB_MSG(("<--DeriveGainsRange"));
}
