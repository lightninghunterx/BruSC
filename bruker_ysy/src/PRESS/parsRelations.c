
/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2006-2007
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
static const char resid[] = "$Id$ (C) 2007 Bruker BioSpin MRI GmbH";


#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	1


#include "method.h"


/*
 * simple local parameter relations 
 */

void AveragesRange(void)
{
  DB_MSG(("-->AveragesRange"));

  if(!ParxRelsParHasValue("PVM_NAverages"))
  {
    PVM_NAverages=1;
  }
  else
  {
    int actna;
    actna=PVM_NAverages;
    PVM_NAverages=MAX_OF(1,actna);
  }

  DB_MSG(("<--AveragesRange"));
}

void AveragesRel(void)
{
  DB_MSG(("-->AveragesRel"));

  AveragesRange();
  backbone();
  DB_MSG(("-->AveragesRel"));
}

/*===============================================================
 * VoxPulXEnumRelations
 * Relation of VoxPulXEnum (a dynamic enmueration parameter which
 * allows to select one of the existing  pulses)
 *===============================================================*/

void VoxPul1EnumRelation(void)
{
  DB_MSG(("-->VoxPul1EnumRelation"));

  UT_SetRequest("VoxPul1Enum");
  backbone();
  
  DB_MSG(("<--VoxPul1EnumRelation"));                                       
}

void VoxPul2EnumRelation(void)
{
  DB_MSG(("-->VoxPul2EnumRelation"));

  UT_SetRequest("VoxPul2Enum");
  backbone();
  
  DB_MSG(("<--VoxPul2EnumRelation"));                                       
}

void VoxPul3EnumRelation(void)
{
  DB_MSG(("-->VoxPul3EnumRelation"));

  UT_SetRequest("VoxPul3Enum");
  backbone();
  
  DB_MSG(("<--VoxPul3EnumRelation"));                                       
}


/*===============================================================
 * VoxPulXAmplRel
 * Relation of VoxPulXAmpl
 * This parameter is used in the setup parameter card to adjust
 * the RF pulse amplitude manually
 *===============================================================*/

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


/* ===================================================================
 * Relation of VoxPulX
 * 
 * All pulses of type PVM_RF_PULSE must have relations like this.
 * However, if you clone this funtion for a different pulse parameter
 * remember to replace the param name in the call to UT_SetRequest!
 *
 * IMPORTANT: this function should not be invoked in the backbone!
 ====================================================================*/

void VoxPul1Relation(void)
{
  DB_MSG(("-->VoxPul1Relation"));
  UT_SetRequest("VoxPul1");
  VoxPul1Range();
  backbone();
  DB_MSG(("<--VoxPul1Relation"));
}

void VoxPul1Range(void)
{
  DB_MSG(("-->VoxPul1Range"));
  double dval=VoxPul1.Flipangle;
  VoxPul1.Flipangle = MIN_OF(145.0,MAX_OF(dval,1.0));
  DB_MSG(("<--VoxPul1Range")); 
}


void VoxPul2Relation(void)
{
  DB_MSG(("-->VoxPul2Relation"));
  UT_SetRequest("VoxPul2");
  VoxPul2Range();
  backbone();
  DB_MSG(("<--VoxPul2Relation"));
}

void VoxPul2Range(void)
{
  DB_MSG(("-->VoxPul2Range"));
  double dval=VoxPul2.Flipangle;
  VoxPul2.Flipangle = MIN_OF(225.0,MAX_OF(dval,1.0));
  DB_MSG(("<--VoxPul1Range")); 
}

void VoxPul3Relation(void)
{
  DB_MSG(("-->VoxPul3Relation"));
  UT_SetRequest("VoxPul3");
  VoxPul3Range();
  backbone();
  DB_MSG(("<--VoxPul3Relation"));
}

void VoxPul3Range(void)
{
  DB_MSG(("-->VoxPul3Range"));
  double dval=VoxPul3.Flipangle;
  VoxPul3.Flipangle = MIN_OF(225.0,MAX_OF(dval,1.0));
  DB_MSG(("<--VoxPul1Range")); 
}

/* 
 * subroutines called by the method handler 
 */


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
