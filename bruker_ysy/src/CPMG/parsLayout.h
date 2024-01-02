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
 * $Id$
 *
 ****************************************************************/

/****************************************************************/
/*	PARAMETER CLASSES				       	*/
/****************************************************************/


/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/

pargroup
{
  DummyScans_Parameters;
  PVM_TriggerModule;
  Trigger_Parameters;
}Preparation;

pargroup
{
  ExcPulse1Enum;
  ExcPulse1;
  ExcPulse1Ampl;
  RefPulse1Enum;
  RefPulse1;
  RefPulse1Ampl;
} 
attributes
{
  display_name "RF Pulses";
} RF_Pulses;


extend pargroup
{
  EchoSpacing;
  NEchoes;
  PVM_RepetitionTime;
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_DeriveGains;
  RF_Pulses;
  Nuclei;
  Spectroscopy;
  Encoding;
  SpoilerOnOff;
  Spoiler_duration;
  Spoiler_strength;
  Preparation;
  ArrayPhaseParameters;
} MethodClass;


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



