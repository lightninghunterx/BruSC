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
double parameter ReadGradRatio;

double parameter
{
  display_name "Read Spoiler Duration";
  format "%.2f";
  units "ms";
  relations backbone;
} ReadSpoilerDuration;

double parameter
{
  display_name "Read Spoiler Strength";
  format "%.1f";
  units "%";
  relations backbone;
} ReadSpoilerStrength;

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
  display_name "Read Gradient";
  format "%f";
  units "%";
  maximum 100;
  relations backbone;
}ReadGradient;

double parameter
{
  display_name "Max Read Gradient";
  format "%f";
  units "%";
  relations backbone;
}ReadGradientLim;

GRAD_DIRECTION parameter
{
  display_name "Gradient Direction";
  relations  GradientDirectionRel;
}GradientDirection;

double parameter
{
  display_name "Gradient Current";
/*  relations GradientCurrentRel;****/
  format "%.2f";
  units "%{ of maximum}";
}GradientCurrent;

int parameter
{
  display_name "Dummy Scans";
  relations dsRelations;
} NDummyScans;

double parameter GradVec[3];
/****************************************************************/
/* TYPE NAMES                                                   */
/****************************************************************/

enum_attributes GRAD_DIRECTION
{
  display_name X_dir   "X";
  display_name Y_dir   "Y";
  display_name Z_dir   "Z";
};

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

