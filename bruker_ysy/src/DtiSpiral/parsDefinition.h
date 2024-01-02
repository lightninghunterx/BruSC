/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2007 - 2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

/****************************************************************/
/* INCLUDE FILES                                                */
/****************************************************************/

PVM_SPOILER_TYPE parameter
{
  display_name "Slice Spoiler";
  relations SliceSpoilerRel;
} SliceSpoiler;

/*** RF pulses ***/

PV_PULSE_LIST parameter
{
  display_name "Excitation Shape";
  short_description "Shape of excitation pulse.";
  relations ExcPulEnumRel;
} ExcPulEnum;

PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse";
  short_description "Excitation pulse parameters.";
 relations ExcPulRel;
} ExcPul;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Excitation Pulse Amplitude";
  relations ExcPulAmplRel;
} ExcPulAmpl;

double parameter
{
  editable false;
} ExcPulShape[];

/*** internal ***/

double parameter
{
  display_name "Slice Gradient";
  short_description "Amplitude of slice gradient.";
  format "%.2f";
  units  "%";
  editable false;
} SliceGrad;

double parameter
{
  display_name "Maximum Slice Gradient";
  short_description "Maximum amplitude of slice gradient.";
  format "%.2f";
  units  "%";
  editable false;
} SliceGradLim;

double parameter
{
  display_name "Rephasing Gradient";
  short_description "Amplitude of slice rephasing gradient.";
  format "%.2f";
  units  "%";
  editable false;
} SliceRephGrad;

double parameter
{
  display_name "Maximum Rephasing Gradient";
  short_description "Maximum amplitude of excitation slice rephasing gradient.";
  format "%.2f";
  units  "%";
  editable false;
} SliceRephGradLim;

double parameter
{
  display_name "Maximum Phase Gradient";
  short_description "Maximum amplitude of phase encoding gradient.";
  format "%.2f";
  units  "%";
  editable false;
} Phase3DGradLim;

double parameter
{
  display_name "Rephasing Duration";
  short_description "Duration of excitation slice rephasing gradient.";
  units "ms";
  format "%.3f";
  editable false;
} RephGradDur;

double parameter
{
  short_description "Minimum delay in pulse program.";
  units "ms";
  format "%.3f";
  editable false;
} MinDelay;

double parameter
{
  short_description "Duration of frequency switching.";
  units "ms";
  format "%.3f";
  editable false;
} FreqSwitch;

double parameter
{
  short_description "Gradient rise time.";
  units "ms";
  format "%.3f";
  editable false;
} RiseT;

double parameter
{
  short_description "Inter-gradient wait time.";
  editable false;
} GradWait;

double parameter
{
  short_description "Amplitude of 3D phase encoding gradient.";
  units "%";
  format "%.1f";
  editable false;
} Phase3dGrad;

double parameter
{
  short_description "Wait time to realise TE.";
  units "ms";
  format "%.3f";
  editable false;
} TEWait;

double parameter
{
  short_description "Wait time to realise TE after refocusing pulse.";
  units "ms";
  format "%.3f";
  editable false;
} TEWaitRfc;

double parameter
{
  short_description "Wait time to realise TR.";
  units "ms";
  format "%.3f";
  editable false;
} TRWait;

double parameter
{
  short_description "Duration of one repetition.";
  units "s";
  format "%.3f";
  editable false;
} OneRepTime;

YesNo parameter
{
  display_name "Gradient Synchronization";
  short_description "Synchronization of the sequence with the clock of the gradient amplifier.";
  relations GradSyncRel;
}GradSync;

/* parameters for DTI Reconstruction */
RecoMeth_MODE parameter
{
  display_name "Reconstruction Mode";
  short_description "Switches between standard and diffusion tensor reconstruction.";
  relations RecoMethModeRel;
}RecoMethMode;

double parameter
{
  display_name "Trace Weight";
  short_description "Specifies the factor b in the weight exp(-b*trace/3) for trace weighted images.";
  format "%.2f";
  units "s/mm2";
  relations RecoMethModeRel;
}TraceWeight;


double parameter EffPulseDur;
double parameter MinFov[3];
double parameter MinSliceThick;

/****************************************************************/
/* TYPE NAMES                                                   */
/****************************************************************/
enum_attributes RecoMeth_MODE
{
  display_name Default  "Default";
  display_name TensorReconstruction "Tensor Reconstruction";
};

/****************************************************************/
/*  E N D   O F   F I L E	                                */
/****************************************************************/

