/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2013
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


/* gradient amplitudes (outside cmpg period) */

double parameter ReadGradient;
double parameter ExcSliceGradient;
double parameter RefSliceGradient;
double parameter SliceDephGradient;
double parameter ReadDephGradient;
double parameter MinRecoveryTime;
double parameter SpoilingRead;   /* read spoiler in cycles par mm */
double parameter SpoilingSlice;  /* slice spoiler in cycles par mm */

double parameter
{
  display_name "Recovery Time";
  format "%.2f";
  units "ms";
  relations backbone;
} RecoveryTime;

double parameter
{
  display_name "Repetition Time";
  format "%.3f";
  units "ms";
  editable false;
  relations backbone;
} MultiRepTime[];

/* gradient shapes (cpmg period) */

double parameter ReadShape[];
double parameter PhaseShape[];
double parameter SliceShape[];
double parameter SliceEncShape[];
int parameter
{
  relations backbone;
} ShapeSize;

RAMPFORM parameter
{
  relations RampFormRels;
} RampForm;

double parameter
{
  display_name "Gradient Stabilization Time";
  relations GradStabRels;
  format "%.3f";
  units "ms";
} GradStab;

/* CPMG spoiling levels */

YesNo parameter
{
  display_name "Automatic Spoiler Setting";
  relations backbone;
} AutoSpoil;

double parameter
{
  display_name "CPMG Read-Spoiling";
  format "%.2f";
  units "{cycles}/{Pixel}";
  relations SpoilingReadPxRels;
} SpoilingReadPx;

double parameter
{
  display_name "CPMG Slice-Spoiling";
  format "%.2f";
  units "{cycles}/{Sl}";
  relations SpoilingSliceSlRels;
} SpoilingSliceSl;

double parameter
{
  display_name "Usable Slew Rate";
  format "%.2f";
  units "%";
  relations SlewRatePerCentRels;
} SlewRatePerCent;


/* remaining RARE parameters */


double parameter
{
  display_name "Slice Bandwidth Scaling";
  format "%.2f";
  units "%";
  relations SliceBandWidthScaleRels;
} SliceBandWidthScale;

double parameter
{
  display_name "Effective echo time";
  format "%.2f";
  units "ms";
  relations localEchoRels;
} EffectiveTE[];

PVM_SPOILER_TYPE parameter
{
  display_name "Repetition Spoiler";
  relations RepetitionSpoilerRel;
}RepetitionSpoiler;


/* excitation pulse */

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

/* refocusing pulse */

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

int parameter
{
  display_name "Number of echoes";
} NEchoes;



/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/




