/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2020
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
#include "SliceSelPars.h"


char parameter AdjProtName[64];

int parameter
{
  minimum 1 outofrange nearestval;
  relations ExcPulseRelation;
} NExcSteps;

PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse Shape";
  relations    ExcPulseEnumRelation;
}ExcPulseEnum;


PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse";
  editable false;
}ExcPulse;


OnOff parameter
{
  display_name "Minimize TE";
  short_description "Keeps TE at minimum.";
  relations AutoEchoTimerRel;
  editable true;
}AutoEchoTimer;

double parameter
{
  units "ms";
  editable false;
  minimum 0.0;
} TETRFillDelay[];


int parameter AverageList[];


// parameters for CalcLineWidth etc
double parameter
{
  editable false;
  format "%.3e";
  display_name "Normalized td-Area";
}NormArea[][];


// list of exc pulse lengths:
double parameter
{ display_name "Pulse Lengths";
  format "%.2f";
  minimum 1    outofrange nearestval;
  maximum 10000 outofrange nearestval;
  //  style inline_array;
  units "us";
  relations ExcPulseRelation;
} ExcDurList[];


double parameter
{ // element [0,1]: start,end-value
  display_name "Pulse Length Range";
  short_description "Set delays as 'Start | End' - value.";
  style inline_array;
  widget spinner;
  format "%.2f";
  units "us";
  minimum 1 outofrange nearestval;
  maximum 10000 outofrange nearestval;
  relations ExcPulseRelation;
} ExcDurRange[2];


double parameter
{ display_name "Pulse Powers";
  format "%.4f";
  minimum 0    outofrange nearestval;
  maximum 1000 outofrange nearestval;
  //  style inline_array;
  units "W";
  relations ExcPulseRelation;
} ExcPowList[];


double parameter
{ // element [0,1]: start,end-value
  display_name "Pulse Power Range";
  short_description "Set delays as 'Start | End' - value.";
  style inline_array;
  widget spinner;
  format "%.4f";
  units "W";
  minimum 0 outofrange nearestval;
  maximum 1000 outofrange nearestval;
  relations ExcPulseRelation;
} ExcPowRange[2];


double parameter
{
  editable false;
  short_description "Pulse Voltages.";
  units "V";
} ExcVoltList[];

int parameter { minimum 0; } NoiseScan;


//for the MethodRecoFilter:
int parameter RecoCounter;
int parameter {relations RecoBufferRelation;} RecoBuffer;

//to store the areas of ft peaks
double parameter PkArea[];

//to store the areas of ft peaks
double parameter {editable false; units "Hz";} PkZeroCross;

//to store index of maximum ft peak area
int parameter PkMaxIdx;

//to interpolate betw indices for search_180 mode
double parameter PkMaxIdxFrac;

double parameter
{
  display_name "Integration Range";
  short_description "Narrows spectral width over which to integrate.";
  units "Hz";
  format "%.0f";
  style inline_array;
  relations PkRangeRelation;
} PkRange[2];

REFPOWX_SEARCH_MODE parameter
{
  display_name "Search";
  short_description "Tries to find value for either 90- or 180-degree passage.";
  relations SearchModeRelation;
} SearchMode;

enum_attributes REFPOWX_SEARCH_MODE
{
  display_name search_90 "90 degrees   ";
  display_name search_180 "180 degrees";
};

double parameter
{ // [0,1]: start,end-value
  display_name "Search Range";
  short_description "Restricts region where to search for 90 or 180 degree signal passage.";
  style inline_array;
  widget spinner;
  units "us";
  format "%.2f";
  relations SearchRangeRelation;
} SearchDurRange[2];

double parameter
{ //[0,1]: start,end-value
  display_name "Search Range";
  short_description "Restricts region where to search for 90 or 180 degree signal passage.";
  style inline_array;
  widget spinner;
  units "V";
  format "%.4f";
  editable true;
  relations SearchRangeRelation;
} SearchVoltRange[2];

double parameter
{ //[0,1]: start,end-value
  display_name "Search Range";
  short_description "Restricts region where to search for 90 or 180 degree signal passage.";
  style inline_array;
  widget spinner;
  units "W";
  format "%.4f";
  editable true;
  relations SearchRangeRelation;
} SearchPowRange[2];

//for phase correcting complex ft-data
double parameter
{
  editable false;
  format "%.3f";
  style inline_array;
} PhaseCorrVals[2];


// for reco:
REFPOWX_RECO_MODE parameter
{
  editable true;
  relations RecoModeRelation;
} RecoMode;


enum_attributes REFPOWX_RECO_MODE
{
  display_name CMPLX_RECO_MODE  "Complex";
  display_name MAGN_RECO_MODE "Magnitude";
};


REFPOWX_SCAN_MODE parameter
{
  editable true;
  short_description "Define parameter to be varied.";
  display_name "Scan Mode";
  relations ScanModeRelation;
} ScanMode;


enum_attributes REFPOWX_SCAN_MODE
{
  display_name CONST_PULSE_POW "Pulse Length        ";
  display_name CONST_PULSE_DUR "Pulse Power";
};


YesNo  parameter { editable false; } AdjRunning;
YesNo  parameter { editable false; } InAdjPlatform;

char parameter CurrentTxState[PVSYSINFOCHANSTATE_LEN];


REFPOWX_LOG parameter
{
  editable true;
  short_description "Debug data logger for AdjRefPowX.";
  display_name "Log";
  //  relations ScanModeRelation;
} AdjRefPowXLog;


enum_attributes REFPOWX_LOG
{
  display_name none         "None";
  display_name phases_cor   "Phases, unwrapped";
  display_name phases_uncor "Phases";
  display_name spectra      "Spectra";
};


/****************************************************************/
YesNo parameter
{
  editable true;
  display_name "Skip Sub-Adjustments";
  short_description "Skips Rg- & Rx-phase-adjustment.";
  relations backbone;
} SkipSubAdjustments;

int parameter
{
  display_name "RG Auto Counter";
  relations RgAutoCounterRel;
}RgAutoCounter;

double parameter
{
  editable false;
  format "%.3e";
  display_name "Normalized fd-Area";
}NormFtArea[][];

double parameter RgMagn[];
int    parameter RgScanCounter;
int    parameter RgMagnTndncy;
double parameter RgResult;

/****************************************************************/
//for PDF report
double parameter {editable false; units "%" ;} RefPowDelta;
double parameter {editable false; units "us";} SamplingResolution;

double parameter {editable false; units "W" ;} AdjReport_TxMaxAllowedCWPower;
double parameter {editable false; units "W" ;} AdjReport_TxMaxAllowedPeakPower;
double parameter {editable false; units "ms";} AdjReport_TxMaxAllowedPeakDur;
char   parameter {editable true;} NumRes[12];
char   parameter {editable true;} NumRes1[12];
char   parameter {editable true;} NumRes2[12];
char   parameter {editable false;} AdjReport_Message[1024];
char   parameter {editable false;} Messages[][1024];
double parameter {editable false;} AdjReport_Plot_X_Vals[][];
double parameter {editable false;} AdjReport_Plot_Y_Vals[][];
int    parameter {editable false;} AdjReport_NbrElemRxCoil;
int    parameter {editable false;} AdjReport_NbrElemTxCoil;
YesNo  parameter {editable false;} SearchBox;
YesNo  parameter AdjReport_RenderFlags[];

REFPOW_TYPE parameter
{
  short_description "Mirror par of Reference Power for reporting.";
} StudyRefPow;

REFPOW_TABLE parameter RefPowTable[];
/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

