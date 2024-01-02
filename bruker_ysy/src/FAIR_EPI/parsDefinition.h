/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999-2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/


/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/


double parameter SliceGradRatio;
double parameter ExSliceBandWidth;
double parameter ExSliceRephaseGradient;
double parameter ExSliceGradient;
double parameter ExSlicePulseLength;
double parameter ExSliceRephaseFactor;
double parameter ReadGradient;
double parameter MinTE_left;
double parameter MinTE_right;
double parameter MinRecoveryTime;

double parameter
{
  display_name "Recovery Time";
  format "%.2f";
  units "ms";
  minimum 1 outofrange nearestval;
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

int parameter 
{
  display_name "Segments";
  short_description "Number of segments." ;
  relations NSegmentsRels;
} NSegments;

double parameter
{
  display_name "Echo Time";
  units "ms";
  format "%.2f";
  relations backbone;
} EchoTime;

double parameter
{
  display_name "ExSliceRephaseTime";
  units "ms";
  format "%.2f";
  relations ExSliceRephaseTimeRel;
} ExSliceRephaseTime;

PVM_SPOILER_TYPE parameter 
{
  display_name "Slice Spoiler";
  relations SliceSpoilerRel;
}SliceSpoiler;

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


double parameter
{
  display_name "Bandwidth Scaling Factor";
  relations BwScaleRel;
  units "%";
  format "%f";
}BwScale;

/****************************************************************/
/* TYPE NAMES                                                   */
/****************************************************************/

enum_attributes SIGNAL_TYPE
{
  display_name FID_signal         "FID";
  display_name SPINECHO_signal    "Spinecho";
};

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

