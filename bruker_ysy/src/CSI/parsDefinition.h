/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999-2002
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

#include "LocalizationPars.h"


// parameter for weighted averaging:

int parameter AverageList[];
int parameter AverageListSum;

double parameter
{
  display_name "Effective Resolution";
  short_description "Spatial response width including filter effect.";
  units "mm";
  style inline_array;
  maxdim [once 3];
} EffResolution[];

AVERAGING_MODE parameter
{
  display_name "Averaging Mode";
  relations AverageModeRels;
} AverageMode;

PVM_SPOILER_TYPE parameter
{
  display_name "Spoiler Gradient";
  relations SpoilerRel;
}Spoiler;

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
  display_name "1D Phase Gradient";
  format "%f";
  units "%";
  relations backbone;
}Phase1DGrad;

double parameter
{
  display_name "Max. 1D Phase Gradient";
  format "%f";
  units "%";
  relations backbone;
}Phase1DGradLim;


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

OnOff parameter
{
    display_name "Retro Frequency Lock";
    relations RetroFrequencyLockRelation;
} RetroFrequencyLock_OnOff;

OnOff parameter
{
    display_name "k-Space Filter";
    short_description "Applies a Hamming window to k-space to reduce ringing.";
    relations KFilteringRelation;
} KFiltering;

double parameter TrFillDelay; /* placeholder, no relations */

double parameter EffPulseDur;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

