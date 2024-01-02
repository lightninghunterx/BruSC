/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2008
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




double parameter ExSliceGradient;
double parameter ExSliceRephaseGradient;
double parameter RfcSpoilerStrength;
double parameter RfcSpoilerDuration;

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
  short_description "Number of Segments." ;
  relations NSegmentsRels;
} NSegments;

double parameter
{
  display_name "Spin Echo Time";
  units "ms";
  format "%.3f";
  relations backbone;
} SpinEchoTime;

double parameter
{
  display_name "First Gradient Echo Time";
  units "ms";
  format "%.3f";
  relations backbone;
} FirstGradientEchoTime;

double parameter
{
  display_name "Gradient Echo Time";
  relations backbone;
} GradientEchoTime[];

double parameter
{
  display_name "Effective Echo Time";
  relations backbone;
} EffectiveEchoTime[];

double parameter
{
  display_name "Echo Time Increment";
  units "ms";
  format "%.3f";
  relations EchoTimeIncrementRelation;
} EchoTimeIncrement;

YesNo parameter
{
  display_name "Single Phase Correction";  
  relations SinglePhaseCorrectionRelation;
} SinglePhaseCorrection;

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

