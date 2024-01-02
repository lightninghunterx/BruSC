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

#include "Mdeft_parsDefinition.h"

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

int parameter
{
  display_name "Dummy Echoes";
  short_description "Number of Dummy Echoes.";
  relations deRelations;
}NDummyEchoes;

int parameter
{
  display_name "Segments";
  short_description "Number of Segments.";
  relations SegmNumberRelations;
}SegmNumber;

int parameter
{
  display_name "Segment Size";
  short_description "Number of echoes per segment.";
  minimum 1 outofrange nearestval;
  maximum 1024 outofrange nearestval;
  relations backbone;
} SegmentSize;

double parameter
{
  display_name "Segment Repetition Time";
  relations backbone;
  format "%.3f";
  units "ms";
}SegmRepTime;

double parameter MinSegmRepTime;

double parameter
{
  display_name "Segment Duration";
  /* relations SegmDurationRelations; */
  format "%.3f";
  units "ms";
}SegmDuration;

double parameter SegmPad;
double parameter PreEchoTime;
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


double parameter TeFillDelay; /* placeholder, no relations */
double parameter TrFillDelay;

double parameter
{
  display_name "Echo Repetition Time";
  relations backbone;
  format "%.2f";
  units "ms";
}EchoRepTime;

double parameter
{
  display_name "Echo Time";
  relations backbone;
  format "%.2f";
  units "ms";
}EchoTime;

double parameter MinEchoRepTime;


/****************************************************************/
/* TYPE NAMES                                                   */
/****************************************************************/

enum_attributes PREPARATION_TYPE
{
  display_name Inversion_prep  "Inversion";
  display_name MDEFT_prep      "MDEFT";
};

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

