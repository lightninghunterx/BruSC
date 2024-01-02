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
double parameter EchoDelay;
double parameter EffPulseDur;


PVM_SPOILER_TYPE parameter
{
  display_name "Read Spoiler";
  relations ReadSpoilerRel;
}ReadSpoiler;

PVM_SPOILER_TYPE parameter
{
  display_name "Slice Spoiler";
  relations SliceSpoilerRel;
}SliceSpoiler;

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


double parameter
{
  display_name "Inter Slice Delay";
  relations backbone;
  units "ms";
  format "%.2f";
}SliceSegDur;

double parameter SliceSegDelay;
double parameter MinSliceSegDur;
double parameter SliceSegEndDelay;

YesNo parameter
{
  display_name "RF Spoiling";
  relations backbone;
}RFSpoiling;

double parameter RFPhaseList[];

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
  display_name "Read Rephase Time";
  format "%f";
  units "ms";
  relations backbone;
}RephaseTime;

double parameter
{
  display_name "Read Rephase Grad ";
  format "%.2f";
  units "%";
  relations backbone;
}RephaseGrad;

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

double parameter
{
  display_name "First Echo Time";
  format "%.2f";
  units "ms";
} FirstEchoTime;

double parameter
{
  display_name "Effective Echo Time";
  format "%.2f";
  units "ms";
  editable false;
} EffectiveTE[];

double parameter
{
  display_name "Echo Spacing";
  format "%.2f";
  units "ms";
  relations backbone;
} EchoSpacing;

double parameter
{
  display_name "Min Echo Spacing";
  format "%.2f";
  units "ms";
  editable false;
} MinEchoSpacing;

ECHOACQMODE parameter
{
  display_name "Acquisition Mode";
  relations EchoAcqModeRel;
} EchoAcqMode;


double parameter TeFillDelay; /* placeholder, no relations */
double parameter TrFillDelay;
double parameter EchoFillDelay;
double parameter MultiEchoTime;

/* new parameters for SWI Reconstruction */
RecoMeth_MODE parameter
{
  display_name "Reconstruction Mode";
  short_description "Switches between standard and susceptibility-weighted imagig reconstruction.";
  relations SetRecoParam;
}RecoMethMode;

MASK_MODE parameter
{
  display_name "Weighting Mode";
  short_description "Selection of the way phase information will be used to influence image intensity.";
  relations MaskModeRel;
}WeightingMode;

double parameter
{
  display_name "Mask Weighting";
  short_description "Strength of the weighting mask function.";
  relations MaskWeightRange;
  format "%.2f";
}MaskWeighting;

double parameter
{
  display_name "Gauss Broadening";
  short_description "Defines the broadening (smoothing) effect of the Gauss filter.";
  relations GaussBroadRange;
  format "%.2f";
  units "mm";
}GaussBroadening;


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

