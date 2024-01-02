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


#include "flowDefinition.h"

/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/
double parameter OneRepTime;

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
 display_name "Time for Movie";
 units "ms";
 format "%.2f";
 relations backbone;
 editable false;
}TimeForMovieFrames;

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
  display_name "Rewinding Duration";
  short_description "Duration of encoding-rewinding gradient.";
  relations backbone;
  units "ms";
  format "%.3f";
}RewindGradDur;

RecoMeth_MODE parameter
{
  display_name "Reconstruction Mode";
  short_description "Switches between FLOWMAP and Magnitude reconstruction.";
  relations SetRecoParam;
}RecoMethMode;

double parameter ReadFcGrad[2];
double parameter Phase2DFcGrad[2];
double parameter Phase3DFcGrad[2];
double parameter ExcSliceFcGrad[2];

double parameter TrFillDelay;

// shapes for flow compensation
double parameter P2Shape[];//phase enc
double parameter P3Shape[];//phase enc
double parameter RShape[]; //read dephasing
double parameter SShape[]; //slice rephasing
double parameter EShape[]; //flow encoding
int    parameter ShapeSize;
double parameter EchoDelay;
double parameter EffPulseDur;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

