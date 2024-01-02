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


int parameter 
{
  display_name "Spectral size";
  relations SpecSizeRels;
} SpecSize;

double parameter 
{
  display_name "Spectral resolution";
  units "Hz";
  relations backbone;
} SpecResol;

double parameter 
{
  display_name "Spectral range";
  units "Hz";
  relations backbone;
} SpecBand;

double parameter 
{
  display_name "Spectral range";
  units "[ppm]";
  relations backbone;
} SpecBandPpm;



double parameter ExSliceGradient;
double parameter ExSliceRephaseGradient;
double parameter RfcSpoilerStrength;
double parameter RfcSpoilerDuration;
double parameter OneRepTime;
double parameter SliceGradRatio;
double parameter ReadGradient;
double parameter MinTE_left;
double parameter MinTE_right;


double parameter 
{
  display_name "Rephasing/encoding time";
  relations RephaseTimeRels;
  units "ms";
  format "%.2f";
} RephaseTime;


int parameter 
{
  display_name "Segments";
  short_description "Number of Segments.";
  relations NSegmentsRels;
} NSegments;

double parameter
{
  display_name "Echo Time";
  units "ms";
  format "%.3f";
  relations backbone;
} EchoTime;

double parameter
{
  display_name "Delay between volumes";
  format "%.2f";
  units "ms";
  relations PackDelRelation;
} PackDel;

PVM_SPOILER_TYPE parameter 
{
  display_name "Slice Spoiler";
  relations SliceSpoilerRel;
}SliceSpoiler;

PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse Shape";
  relations    ExcPulseEnumRelation;
}ExcPulseEnum;

PV_PULSE_LIST parameter
{
  display_name "Refocusing Pulse Shape";
  relations    RefPulseEnumRelation;
}RefPulseEnum;

PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse";
  relations    ExcPulRelation;
}ExcPul;

PVM_RF_PULSE parameter
{
  display_name "Refocusing Pulse";
  relations    RefPulRelation;
}RefPul;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Excitation Pulse Amplitude";
  relations ExcPulseAmplRel;
}ExcPulseAmpl;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Refocusing Pulse Amplitude";
  relations RefPulseAmplRel;
}RefPulseAmpl;

double parameter
{
  editable false;
}ExcPulseShape[];

double parameter
{
  editable false;
}RefPulseShape[];

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

