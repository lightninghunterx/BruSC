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
double parameter Spoiler_strength;
double parameter MinTE1;
double parameter MinTE2;
double parameter Spoiler_duration2;
YesNo  parameter Preparation;

ProfileModeType parameter
{
  display_name "Profile Mode";
  relations   backbone;
} ProfileMode;

// Excitation pulse
PV_PULSE_LIST parameter
{
  display_name "Exc Pulse Shape";
  relations    SPExamExcEnumRel;
}SPExamExcEnum;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Exc. Pulse Ampl.";
  relations SPExamExcAmplRel;
}SPExamExcAmpl;


PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse";
  relations    SPExamExcRel;
}SPExamExc;

double parameter
{
  relations backbone;
}SPExamExcShape[];


// Refocusing pulse

PV_PULSE_LIST parameter
{
  display_name "Rfc Pulse Shape";
  relations    SPExamRfcEnumRel;
}SPExamRfcEnum;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Rfc. Pulse Ampl.";
  relations SPExamRfcAmplRel;
}SPExamRfcAmpl;

PVM_RF_PULSE parameter
{
  display_name "Refocussing Pulse";
  relations    SPExamRfcRel;
}SPExamRfc;

double parameter
{
  relations backbone;
}SPExamRfcShape[];
// Inversion pulse

PV_PULSE_LIST parameter
{
  display_name "Inv Pulse Shape";
  relations    SPExamInvEnumRel;
}SPExamInvEnum;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Inv. Pulse Ampl.";
  relations SPExamInvAmplRel;
}SPExamInvAmpl;

PVM_RF_PULSE parameter
{
  display_name "Inversion Pulse";
  relations    SPExamInvRel;
}SPExamInv;

double parameter
{
  relations backbone;
}SPExamInvShape[];

// hard pulse#

PVM_RF_PULSE parameter
{
  display_name "Hard Pulse";
  relations    HardPulseRel;
}HardPulse;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Hard Pulse Ampl.";
  relations HardPulseAmplRel;
}HardPulseAmpl;


int parameter
{
  display_name "Dummy Scans";
  relations NdummyRel;
}Ndummy;

double parameter
{
  display_name "Read Gradient";
  units        "%";
  relations    backbone;
} ReadGradient;

double parameter
{
  display_name "Read Dephase Gradient";
  units        "%";
  relations    backbone;
} ReadDephGradient;

double parameter    
{
  display_name "Spoiler Duration";
  units        "ms";
  format       "%.2f";
  relations    backbone;
} Spoiler_duration;

double parameter    
{
  display_name "Inversion/Saturation Delay";
  units        "ms";
  format       "%.2f";
  relations    backbone;
} InversionTime;

GRAD_DIRECTION parameter
{
  display_name "Gradient Direction";
  relations  GradientDirectionRel;
}GradientDirection;

YesNo parameter
{
    display_name "Derive Examined Pulse Ampl";
    relations backbone;
} DeriveExaminedPulseGain;

YesNo parameter
{
    display_name "Derive Hard Pulse Ampl";
    relations backbone;
} DeriveHardPulseGain;

SHAPE_ALG_TYPE parameter
{
  display_name "Algorithm for calculated pulses";
  relations PulseAlgRel;
} PulseAlg;

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

