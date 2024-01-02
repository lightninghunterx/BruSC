/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2004
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/



/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/

int    parameter {editable false;}RampPoints;
int    parameter {editable false;}ReadDephPoints;
int    parameter {editable false;}PostPoints;
double parameter {editable false;}RampCompTime;
double parameter {editable false;}GrShapeT1;
double parameter {editable false;}GrShapeT2;
double parameter {editable false;}GrShapeT3;
double parameter {editable false;}GrShapeG2;
double parameter {editable false;}GradAmpR[];
double parameter {editable false;}GradAmpP[];
double parameter {editable false;}GradShape1[];
double parameter {editable false;}GradShape2[];
double parameter {editable false;}GrShape1Time;
double parameter {editable false;}GrShape2Time;
double parameter {editable false;}GradRes;
double parameter {editable false;}Grid;

double parameter {editable false;}ReadDephTime;
double parameter {editable false;}ReadDephGrad;
double parameter {editable false;}PhaseDephGrad;
double parameter {editable false;}ReadDephGradLim;
double parameter {editable false;}ReadGradLim; 
double parameter {editable false;}ReadGrad;
double parameter {editable false;}PhaseGrad;
double parameter {editable false;}TeFillDelay;
double parameter {editable false;}PhaseList[];

int parameter 
{
  relations backbone;
}GradShape1Size;

int parameter 
{
  relations backbone;
}GradShape2Size;

ACQ_MODE parameter
{
  display_name "Acquisition Mode";
  relations backbone;
} AcqMode;

int parameter
{
  display_name "Projections";
  format "%d";
  relations backbone;
}NPro;

double parameter
{
  display_name "Polar Undersampling";
  format "%.2f";
  relations ProUnderRel;
}ProUndersampling;

YesNo parameter
{
  display_name "Minimize TE";
  relations backbone;
}YesNoMinEchoTime;

YesNo parameter
{
  display_name "Gradient Synchronization";
  short_description "Synchronization of the sequence with the clock of the gradient amplifier.";
  relations GradSyncRel;
}GradSync;

YesNo parameter
{
  display_name "Ramp Sampling Compensation";
  short_description "Compensation of resolution loss caused by ramp sampling.";
  relations backbone;
}RampCompYN;

YesNo parameter 
{
  display_name "B0 Correction";
}B0Corr;

double parameter
{
  display_name "Read Spoiling";
  short_description "Readout gradient spoiling after acquisition.";
  units "{cycles}/{pixel}";
  format "%.1f";
  minimum 0.0;
  maximum 100.0;
  relations backbone;
} ReadSpoiling;

double parameter ReadSpoilDur;

// slice selection parameters
double parameter ExcSliceGradLim;
double parameter ExcSliceRephGradLim;
double parameter ExcSliceGrad;

PVM_SPOILER_TYPE parameter 
{
  display_name "Slice Spoiler";
  relations SliceSpoilerRel;
}SliceSpoiler;

RAMP_MODE parameter
{
  display_name "Ramp Mode";
  relations    RampModeRel;
} RampMode;

double parameter
{
  display_name "Effective Slew Rate";
  relations RampModeRel;
  units "%";
  format "%f";
}EffSlewRate;

double parameter
{
  display_name "Ramp Time";
  relations RampModeRel;
  units "ms";
  format "%.3f";
}Ramptime;

YesNo parameter
{
  display_name "Slice Gradient Alternation";
  relations backbone;
}SliceAlter;

// excitation pulse parameters
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

double parameter
{
  relations backbone;
}ExcPulse1Shape[];

double parameter {relations backbone;} FreqList1[];

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/


