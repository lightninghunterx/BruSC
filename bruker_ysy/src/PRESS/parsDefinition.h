


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
/* local parameter						*/
/****************************************************************/

/* timing parameter */
EchoModeType parameter
{
    display_name "TE Calculation Mode";
    relations backbone;
} EchoTimeMode;

double parameter
{
    display_name "TE 1";
    units "ms";
    format "%.2f";
    relations backbone;
} TE1;


double parameter
{
    display_name "TE 2";
    units "ms";
    format "%.2f";
    relations backbone;
} TE2;

double parameter
{
    display_name "Spoiler Duration";
    units "ms";
    format "%.2f";
    relations backbone;
} SpoilerDuration[2][1];

OnOff parameter 
{
    display_name "Calculate Spoiler";
    relations backbone;
} CalcSpoiler;

double parameter
{
    display_name "Spoiler Strength";
    units "%";
    format "%.2f";
    relations backbone;
} SpoilerStrengthArr[2][3];

double parameter
{
    display_name "Spoiler Strength";
    units "%";
    format "%.2f";
    relations backbone;
} SpoilerStrength;

double parameter 
{
    display_name "Gradient Stabilization";
    short_description "Delay between gradient ramps and RF pulses.";
    units "ms";
    format "%.2f";
    relations backbone;
} GradStabDelay;
double parameter SliceRephase;
double parameter AddDelay[4];




/* rf pulses for localized excitation */

/* Pulse 1 */
PV_PULSE_LIST parameter
{
  display_name "RF Pulse 1 Shape";
  relations    VoxPul1EnumRelation;
}VoxPul1Enum;

PVM_RF_PULSE parameter
{
  display_name "RF Pulse 1";
  relations VoxPul1Relation;
}VoxPul1;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "RF Pulse 1 Amplitude";
  relations VoxPul1AmplRel;
}VoxPul1Ampl;

double parameter
{
  relations backbone;
}VoxPul1Shape[];


/* Pulse 2 */
PV_PULSE_LIST parameter
{
  display_name "RF Pulse 2 Shape";
  relations    VoxPul2EnumRelation;
}VoxPul2Enum;

PVM_RF_PULSE parameter
{
  display_name "RF Pulse 2";
  relations VoxPul2Relation;
}VoxPul2;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "RF Pulse 2 Amplitude";
  relations VoxPul2AmplRel;
}VoxPul2Ampl;

double parameter
{
  relations backbone;
}VoxPul2Shape[];

/* Pulse 3 */
PV_PULSE_LIST parameter
{
  display_name "RF Pulse 3 Shape";
  relations    VoxPul3EnumRelation;
}VoxPul3Enum;

PVM_RF_PULSE parameter
{
  display_name "RF Pulse 3";
  relations VoxPul3Relation;
}VoxPul3;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "RF Pulse 3 Amplitude";
  relations VoxPul3AmplRel;
}VoxPul3Ampl;

double parameter
{
  relations backbone;
}VoxPul3Shape[];


/****************************************************************/
/*	Adjustment Parameters	    			*/
/****************************************************************/

/* auto adjustments */
int parameter
{
    relations AutoSFCounterRel;
} AutoSFCounter;


/* manual adjustments */

VSEL_ManAdjType parameter
{
    display_name "Manual Adjustment";
    relations OPT_ManAdjustmentRel;
} OPT_ManAdjustment;

double parameter
{
    display_name "FOV";
    units "mm";
    format "%.2f";
    relations OPT_FOVRel;
} OPT_FOV;
double parameter OPT_GradReadDephaseDelay;
double parameter OPT_GradStrengthRead;
double parameter OPT_GradStrengthReadDephase;

YesNo parameter 
{
  display_name "Keep Pulse Ampl. Ratio";
  relations LockVoxPulAmplsRelation;
} LockVoxPulAmpls;

int parameter AverageList[];
double parameter TrDel;
/****************************************************************/
/*	Optimization Parameters	    			*/
/****************************************************************/

/* eddy current compensation */
OnOff parameter
{
    display_name "Eddy Current Compensation";
    short_description "Uses reference scan to compensate for eddy current induced phase distortions.";
    relations SetRecoParam;
} Edc_OnOff;

YesNo parameter
{
    display_name "Manual Filter Setting";
    short_description "Deactivates automatic determination of Gaussian filter width for water line extraction.";
    relations SetRecoParam;
} EdcManualFilter;

double parameter
{
    display_name "Filter Width";
    short_description "Width of Gaussian filter to extract water line.";
    units "Hz";
    format "%.2f";
    minimum 1.0 outofrange nearestval;
    maximum 100000.0 outofrange nearestval;
    //Reco Param -> no call of backbone!
} EdcFilterWidthHz;


OnOff parameter
{
  display_name "Retro Frequency Lock";
  relations RetroFrequencyLockRelation;
} RetroFrequencyLock_OnOff;

// parameter for shimming and frq adjustment
double parameter
{
  editable false;
  format "%.3e";
  display_name "Normalized Area";
}NormArea[][];

double parameter
{
  editable false;
  display_name "Line Width";
  format "%6.2f";
  units "Hz";
} LineWidth[][][]; // nlevel,spect,chan


OnOff parameter
{
  display_name "Calc Line Width";
  relations CalcLineWidthRelation;
} CalcLineWidth;

double parameter Peak[1][];
double parameter Noise[1][];
/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/









