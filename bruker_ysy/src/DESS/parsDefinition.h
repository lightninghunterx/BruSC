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
double parameter OneRepTime;
double parameter RFPhaseList[2];

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
  display_name "RF Pulse Amplitude";
  relations ExcPulse1AmplRel;
}ExcPulse1Ampl;

double parameter
{
  relations backbone;
}ExcPulse1Shape[];


/* ---------------------------------------------------------
 * remaining local method parameters
 * --------------------------------------------------------*/

double parameter
{
  display_name "Read Dephase Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ReadDephGrad;

double parameter
{
  display_name "Max Read Dephase Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ReadDephGradLim;

double parameter
{
  display_name "Read Gradient";
  format "%f";
  units "%";
  relations backbone;
}ReadGrad;

double parameter
{
  display_name "Max Read Gradient";
  format "%f";
  units "%";
  relations backbone;
}ReadGradLim;

double parameter
{
  display_name "2D Phase Gradient";
  format "%f";
  units "%";
  relations backbone;
}Phase2DGrad;

double parameter
{
  display_name "Max. 2D Phase Gradient";
  format "%f";
  units "%";
  relations backbone;
}Phase2DGradLim;

double parameter
{
  display_name "3D Phase Gradient";
  format "%f";
  units "%";
  relations backbone;
}Phase3DGrad;

double parameter
{
  display_name "Max. 3D Phase Gradient";
  format "%f";
  units "%";
  relations backbone;
}Phase3DGradLim;

double parameter
{
  display_name "Exc. Slice Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ExcSliceGrad;

double parameter
{
  display_name "Max. Exc. Slice Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ExcSliceGradLim;

double parameter
{
  display_name "Exc. Slice Reph. Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ExcSliceRephGrad;

double parameter
{
  display_name "Max. Exc. Slice Reph. Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ExcSliceRephGradLim;

double parameter
{
  display_name "Encoding Duration";
  short_description "Duration of encoding gradient.";
  relations backbone;
  units "ms";
  format "%.3f";
}EncGradDur;

double parameter {relations backbone;}Rew2DGrad;
double parameter {relations backbone;}Rew3DGrad;
double parameter {relations backbone;}RewGradDur;

/* new parameters for DESS */
double parameter
{
  display_name "TE1";
  short_description "Time from rf pulse to 1st echo.";
  relations DESS_TE1_Rel;
  format "%f";
  units  "ms";
  editable false;
}DESS_TE1;

double parameter
{
  display_name "TE2";
  short_description "Time from 2nd echo to following rf pulse.";
  relations DESS_TE2_Rel;
  format "%.3f";
  units  "ms";
}DESS_TE2;

double parameter
{
  display_name "Minimum TE2";
  short_description "Minimum time for TE2.";
  relations backbone;
  editable false;
  format "%.3f";
  units  "ms";
}DESS_TE2_min;

double parameter
{
  display_name "Read Spoiling";
  short_description "Spoiling in read direction.";
  relations backbone;
  units "{cycles}/{pixel}";
  format "%.1f";
  minimum 0.0;
  maximum 20.0;
}DESS_ReadSpoiling;

double parameter
{
  display_name "Read Spoiler Amp";
  short_description "Amplitude of the spoiler in read direction.";
  relations backbone;
  format "%f";
  units  "%";
  minimum -50.0;
  maximum 50.0;
  editable false;
}DESS_ReadSpoilGrad;

double parameter
{
  display_name "Slice Spoiling";
  short_description "Spoiling in slice direction.";
  relations backbone;
  units "{cycles}/{pixel}";
  format "%.1f";
  minimum 0.0;
  maximum 20.0;
}DESS_SliceSpoiling;

double parameter
{
  display_name "Slice Spoiler Amp";
  short_description "Amplitude of the spoiler in slice direction.";
  relations backbone;
  format "%f";
  units  "%";
  editable false;
}DESS_SliceSpoilGrad;

YesNo parameter
{
  display_name "Equal Echo Times";
  short_description "If set to Yes: TE1 = TE2.";
  relations backbone;
}DESS_equal_TEs;

YesNo parameter
{
  display_name "Read Spoiler Flat";
  short_description "If set to Yes: Read Spoiler Amplitude = Read Gradient Amplitude.";
  relations backbone;
}DESS_spoiler_flat;

YesNo parameter
{
  display_name "RF cycle 0-0";
  short_description "Yes: 0-0 instead of 0-180.";
  relations backbone;
}DESS_RF_cycle;

YesNo parameter
{
  relations FWInphaseRel;
  display_name "Fat/Water Inphase Condition";
}FWInphase;

double parameter TeFillDelay;
double parameter DESS_spoiler_duration;
double parameter DESS_TE2Delay;
double parameter DESS_SlicePreGradAmp;
double parameter DESS_RewDur;
double parameter DESS_ReadRew;
double parameter DESS_Phase2Rew;
double parameter DESS_Phase3Rew;

/* new parameters for reconstruction */
RecoMeth_MODE parameter
{
  display_name "Reconstruction Mode";
  short_description "Switches between standard and field map reconstruction.";
  relations SetRecoParam;
}RecoMethMode;

int parameter
{
  relations backbone;
} FieldMapImages;

double parameter
{
  display_name "Map S/N";
  format "%.0f";
  relations MapSnrRel;
}MapSnr;


int parameter RunningAdj;
SHIM_CHANNEL parameter ShimsetArray[];

double parameter EchoDelay;
double parameter EffPulseDur;
double parameter MinSpoilerDuration;
double parameter EncGradDur_min;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

