/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2005
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

double parameter
{
  display_name "RF Pulse Duration";
  relations RfPulseDurRel;
  format "%.3f";
  units "ms";
}RfPulseDur;


PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse Shape";
  relations    ExcPulse1EnumRel;
}ExcPulse1Enum;

PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse";
  relations    ExcPulse1Rel;
}ExcPulse1;


PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "RF Pulse Amplitude";
  relations ExcPulse1AmplRel;
}ExcPulse1Ampl;


int parameter
{
  display_name "Acquisition Size";
  relations AcqSizeRel;
}AcqSize;


double parameter
{
  display_name "Frequency Bands";
  units "Hz";
  short_description "Mid frequency for this excitation number.";
  editable false;
  relations backbone;
}FreqOffsList[];


int parameter
{
  display_name "Number of Excitations";
  relations NexcRel;
}Nexc;


double parameter
{
  display_name "Frequency Adjustment Range";
  units "Hz";
  format "%.0f";
  relations FreqAdjRangeRel;
}FreqAdjRange;


double parameter
{
  display_name "Resolution";
  units "Hz/{point}";
  format "%.1f";
  relations  FreqResRel;
}FreqRes;


int parameter 
{
  // relations FreqAdjCounterRel;
  relations SfAdjCounterRel;
}FreqAdjCounter;

YesNo  parameter SigFail[];

double parameter WOFrq[];
double parameter Peak[][];
double parameter Noise[][];    //
int    parameter NChan[2]; // stores number of channels (active, receive)

double parameter
{
  units "Hz";
  format "%6.2f";
}OFrq[][];


double parameter
{
  units "Hz";
  format "%6.2f";
}PWidth[][][]; // nlevel,spect,chan


void parameter
{
  display_name "Apply Manual Offset";
  relations ApplyOffsetRel;
  short_description "Add the manual offset to the working frequency.";
}ApplyManAdjOffset;


double parameter
{
  display_name "Reference Frequency";
  relations FrqRefRel;
  short_description "Adjusted reference frequency.";
  units "MHz";
  format "%.6f";
}FrqRef;


double parameter
{
  display_name "Reference Chemical Shift";
  relations FrqRefPpmRel;
  short_description "The ppm value of the reference frequency.";
  units "[ppm]";
}FrqRefPpm;


void parameter
{
  display_name "Reset to Configuration Frequency";
  relations FrqResetCfgRel;
  short_description "Reset of 1H reference frequency to system default.";
}FrqResetCfg;



double parameter
{
  //editable false;
  display_name "Normalized Area";
  format "%.3e";
}NormArea[][];
