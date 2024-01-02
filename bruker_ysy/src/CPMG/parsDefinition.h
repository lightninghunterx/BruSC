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
/* INCLUDE FILES						*/
/****************************************************************/

double parameter MinTE1;
double parameter MinTE2;

PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse Shape";
  relations    ExcPulse1EnumRelation;
}ExcPulse1Enum;


PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse";
  relations    ExcPulse1Relation;
}ExcPulse1;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Exc Pulse Amplitude";
  relations ExcPulse1AmplRel;
}ExcPulse1Ampl;

double parameter
{
  relations backbone;
}ExcPulse1Shape[];

PV_PULSE_LIST parameter
{
  display_name "Refocusing Pulse Shape";
  relations    RefPulse1EnumRelation;
}RefPulse1Enum;


PVM_RF_PULSE parameter
{
  display_name "Refocusing Pulse";
  relations    RefPulse1Relation;
}RefPulse1;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Rfc Pulse Amplitude";
  relations RefPulse1AmplRel;
}RefPulse1Ampl;

double parameter
{
  relations backbone;
}RefPulse1Shape[];

double parameter
{
  display_name "Echo Spacing";
  relations backbone;
  format "%.3f";
  units "ms";
} EchoSpacing;

int parameter
{
  display_name "Echoes";
  minimum 1 outofrange nearestval;
  maximum 1024 outofrange nearestval;
  relations backbone;
} NEchoes;

OnOff parameter
{
  display_name "Spoiler";
  relations backbone;
} SpoilerOnOff;

double parameter
{
  display_name "Spoiler Duration";
  relations backbone;
  format "%.2f";
  units "ms";
} Spoiler_duration;

double parameter
{
  display_name "Spoiler Strength";
  relations backbone;
  format "%.1f";
  units "%";
} Spoiler_strength;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

