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
double parameter ExcSliceGrad;
double parameter ReadDephGrad;
double parameter SliceRephGrad;
double parameter ReadGrad;
double parameter Phase2DGrad;
double parameter Phase3DGrad;
double parameter TeFillDelay1;
double parameter TeFillDelay2;
double parameter ReadDephDur;
double parameter OneRepTime;
double parameter SliceSpoilGradL;
double parameter SliceSpoilGradR;
double parameter ReadSpoilGradL;
double parameter ReadSpoilGradR;
double parameter EchoDelay;
double parameter EffPulseDur;
double parameter ReadDephGradLim;
double parameter ReadGradLim;
double parameter Phase2DGradLim;
double parameter Phase3DGradLim;
double parameter ExcSliceGradLim;
double parameter ExcSliceRephGradLim;

double parameter
{
  display_name "Required Slice Spoiling";
  short_description "Required dephasing by slice spoiler.";
  relations backbone;
  units "{cycles}";
  format "%.3f";
  minimum 0.0 outofrange nearestval;
  maximum 20.0 outofrange nearestval;
} EchoSpoilingSlice;

double parameter
{
  display_name "Effective Slice Spoiling";
  short_description "Effective dephasing by slice spoiler.";
  relations backbone;
  units "{cycles}";
  format "%.3f";
} EchoSpoilingSliceEff;

double parameter
{
  display_name "Required Read Spoiling";
  short_description "Required dephasing by read spoiler.";
  relations backbone;
  units "{cycles}";
  format "%.3f";
  minimum 0.0 outofrange nearestval;
  maximum 20.0 outofrange nearestval;
} EchoSpoilingRead;

double parameter
{
  display_name "Effective Read Spoiling";
  short_description "Effective dephasing by read spoiler.";
  relations backbone;
  units "{cycles}";
  format "%.3f";
} EchoSpoilingReadEff;


YesNo parameter
{
  display_name "Auto Echo Spoiler";
  short_description "Automatic setting of echo spoiler.";
  relations backbone;
} EchoSpoilingAuto;

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
  display_name "Effective echo time";
  format "%.2f";
  units "ms";
  relations effTErels;
} EffectiveTE[];

PVM_SPOILER_TYPE parameter
{
  display_name "Repetition Spoiler";
  relations RepetitionSpoilerRel;
}RepetitionSpoiler;


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
  display_name "Number of Echoes";
} NEchoes;

double parameter
{
  display_name "Inter Slice Delay";
  relations backbone;
  units "ms";
  format "%.2f";
}SliceSegDur;

double parameter SliceSegDelay;
double parameter MinSliceSegDur;

/* parameters specific for RAREVTR */
int parameter
{
  display_name "T1 Experiments";
  short_description "Number of T1 experiments.";
  minimum 1 outofrange nearestval;
  maximum 50 outofrange nearestval;
  relations backbone;
}T1Exp;

REP_MODE parameter
{
  display_name "Repetition Time Mode";
  relations backbone;
}RepTimeMode;

double parameter
{
  display_name "Estimated T1";
  relations backbone;
  format "%.2f";
  units "ms";
  minimum 1 outofrange nearestval;
  maximum 50000 outofrange nearestval;
} EstT1Value;

double parameter
{
  display_name "Minimum Repetition Time";
  relations backbone;
  format "%.3f";
  units "ms";
} MinT1RepTime;

double parameter
{
  display_name "Repetition Time";
  format "%.3f";
  units "ms";
  relations backbone;
} MultiRepTime[];

/****************************************************************/
/* TYPE NAMES                                                   */
/****************************************************************/
enum_attributes REP_MODE
{
  display_name Calculate_RepTime  "Calculate";
  display_name Enter_RepTime      "Enter";
};

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/




