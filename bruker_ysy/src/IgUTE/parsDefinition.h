/* ***************************************************************
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
 * ***************************************************************/



/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/

int    parameter {editable false;}RampPoints;
int    parameter {editable false;}ReadDephPoints;
double parameter {editable false;}RampCompTime;
double parameter {editable false;}GrShapeT1;
double parameter {editable false;}GrShapeT2;
double parameter {editable false;}GrShapeT3;
double parameter {editable false;}GrShapeG2;
double parameter {editable false;}GradAmpR[];
double parameter {editable false;}GradAmpP[];
double parameter {editable false;}GradShape1[];
double parameter {editable false;}GradShape2[];
double parameter {editable false;}GrShape1Time;
double parameter {editable false;}GrShape2Time;
double parameter {editable false;}GradRes;
double parameter {editable false;}Grid;

double parameter {editable false;}ReadDephTime;
double parameter {editable false;}ReadDephGrad;
double parameter {editable false;}PhaseDephGrad;
double parameter {editable false;}ReadDephGradLim;
double parameter {editable false;}ReadGradLim; 
double parameter {editable false;}ReadGrad;
double parameter {editable false;}PhaseGrad;
double parameter {editable false;}TeFillDelay;

int parameter 
{
  relations backbone;
}GradShape1Size;

int parameter 
{
  relations backbone;
}GradShape2Size;

int parameter
{
  display_name "Projections";
  format "%d";
  relations backbone;
}NPro;

double parameter
{
  display_name "Polar Undersampling";
  format "%.2f";
  relations ProUnderRel;
}ProUndersampling;

YesNo parameter
{
  display_name "Minimize TE";
  relations backbone;
}YesNoMinEchoTime;

YesNo parameter
{
  display_name "Gradient Synchronization";
  short_description "Synchronization of the sequence with the clock of the gradient amplifier.";
  relations GradSyncRel;
}GradSync;

YesNo parameter
{
  display_name "Ramp Sampling Compensation";
  short_description "Compensation of resolution loss caused by ramp sampling.";
  relations backbone;
}RampCompYN;

YesNo parameter 
{
  display_name "B0 Correction";
}B0Corr;

double parameter
{
  display_name "Read Spoiling";
  short_description "Readout gradient spoiling after acquisition.";
  units "{cycles}/{pixel}";
  format "%.1f";
  minimum 0.0;
  maximum 100.0;
  relations backbone;
} ReadSpoiling;

double parameter ReadSpoilDur;

// slice selection parameters
double parameter ExcSliceGradLim;
double parameter ExcSliceRephGradLim;
double parameter ExcSliceGrad;

PVM_SPOILER_TYPE parameter 
{
  display_name "Slice Spoiler";
  relations SliceSpoilerRel;
}SliceSpoiler;

RAMP_MODE parameter
{
  display_name "Ramp Mode";
  relations    RampModeRel;
} RampMode;

double parameter
{
  display_name "Effective Slew Rate";
  relations RampModeRel;
  units "%";
  format "%f";
}EffSlewRate;

double parameter
{
  display_name "Ramp Time";
  relations RampModeRel;
  units "ms";
  format "%.3f";
}Ramptime;

// excitation pulse parameters
PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse Shape";
  relations    ExcPulse1EnumRel;
}ExcPulse1Enum;


PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse";
  relations    ExcPulse1Rel;
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

// parameters for self-gating
int parameter
{
  display_name "Oversampling";
  short_description "Rerun of each phase encoding step: should be set high enough to sufficiently sample cardiac and respiratory cycles.";
  minimum 1 outofrange nearestval;
  maximum 20000 outofrange nearestval;
  relations backbone;
}Oversampling;

int parameter
{
  display_name "Cardiac Period";  
  units "ms";
  editable false;
}CardiacPeriod;

int parameter
{
  display_name "Heart Rate";  
  short_description "Expected heart rate of the animal under investigation in beats per minute.";
  units "{bpm}";
  minimum 1 outofrange nearestval;
  maximum 10000 outofrange nearestval;
  relations CalcFreq;
}HeartRate;

int parameter
{
  display_name "Respiration Period";  
  units "ms";
  editable false;
}RespPeriod;

int parameter
{
  display_name "Respiration Rate";  
  short_description "Expected respiration rate of the animal under investigation in beats per minute.";
  units "{bpm}";
  minimum 1 outofrange nearestval; 
  maximum 5000 outofrange nearestval;
  relations CalcFreq;
}RespRate;

double parameter
{
  display_name "Cardiac Frequency";  
  editable false;
  relations CalcFreq;
}CardFreq;

double parameter
{
  display_name "Respiration Frequency";  
  editable false;
  relations CalcFreq;
}RespFreq;

YesNo parameter
{
  display_name "Self-Gating";
  short_description "When deselected, only a default reconstruction without data sorting will be carried out.";
  relations backbone;
}SelfGating;

MOVIE_TYPE parameter
{
  display_name "Movie Type";
  relations RecoMethModeVisPar;
}MovieType;

ANALYSIS_MODE parameter
{
  display_name "Analysis of Motion";
  relations RecoMethModeVisPar;
}AnalysisOfMotion;

SIGNAL_TYPE parameter
{
  display_name "Navigator Analysis";
  short_description "The navigator analysis can be based on magnitude or phase data. Auto mode chooses the result with the best SNR.";
}NavigatorAnalysis;

YesNo parameter
{
  display_name "Time Gridding";
  short_description "Turns on time interpolation of movie frames with a  Gaussian kernel.";
  relations RecoMethModeVisPar;
}TimeGridding;

double parameter
{
  display_name "Gauss Width";
  short_description "Width of Gaussian kernel used for time interpolation.";
  format "%.3f";
  minimum 0 outofrange nearestval;
  maximum 20 outofrange nearestval;
}GaussWidth;

double parameter
{
  display_name "Gate Start";
  short_description "Starting point of gating window used for reconstruction.";
  format "%.3f";
  minimum 0 outofrange nearestval;
  maximum 1 outofrange nearestval;
}FreezePhase0;

double parameter
{
  display_name "Gate Range";
  short_description "Range of gating window used for reconstruction.";
  format "%.3f";
  minimum 0.1 outofrange nearestval;
  maximum 1 outofrange nearestval;
}FreezePhaseRange0;

double parameter
{
  display_name "Gate Start";
  short_description "Starting point of gating window used for reconstruction.";
  format "%.3f";
  minimum 0 outofrange nearestval;
  maximum 1 outofrange nearestval;
}FreezePhase;

double parameter
{
  display_name "Gate Range";
  short_description "Range of gating window used for reconstruction.";
  format "%.3f";
  minimum 0 outofrange nearestval;
  maximum 1 outofrange nearestval;
}FreezePhaseRange;

int parameter
{
    display_name "Movie Frames";
    short_description "Number of movie frames to be reconstructed from data.";
    relations MovieFramesRelations;
} OutputMovieFrames;

int parameter {relations backbone;}TransActionBlGSP;

YesNo parameter IgRecoDebug;

int parameter
{
  display_name "Additional Central Points";
  short_description "Number of additional central points used for navigator.";
  minimum 0 outofrange nearestval;
  maximum 32 outofrange nearestval; 
  relations backbone; 
}AddCentralPoints;

/* result parameters for reconstruction */
double parameter 
{
 editable false;
 relations CardFreqResultRel;
}CardFreqResult;

double parameter 
{
  editable false;
  relations RespFreqResultRel;
}RespFreqResult;

int parameter
{
  display_name "Heart Rate";  
  short_description "Heart rate calculated by reconstruction.";
  units "{bpm}";
  editable false; 
}HeartRateRecoResult;

int parameter
{
  display_name "Respiration Rate";  
  short_description "Respiration rate calculated by reconstruction.";
  units "{bpm}";
  editable false;
}RespRateRecoResult;

YesNo parameter
{
  display_name "Store Output Files";
  short_description "If activated, reconstruction output files will be created in the procno directory.";
}StoreOutputFiles;

/* parameters for PET/MR study */
YesNo parameter
{
  display_name "PET MR Study";
  editable false;
}PET_MR_Study;

YesNo parameter
{
  display_name "TTL Out";
  relations backbone;
}IgTTLOut;

YesNo parameter
{
  display_name "PPG TTL Out";
  editable false;
}PpgTTLOut;

double parameter
{
  display_name "TTL Duration";  
  units "ms"; 
  editable false;
}IgTTLDuration;

YesNo parameter
{
    display_name "Compressed Sensing"; 
    short_description "Activates an iterative reconstruction.";
    relations SetRecoParam;
}CompressedSensing;

/****************************************************************/
/* TYPE NAMES                                                   */
/****************************************************************/
enum_attributes ANALYSIS_MODE
{
  display_name CardiacResp  "Cardiac and Respiration";
  display_name OnlyCardiac "Only Cardiac";
  display_name OnlyResp "Only Respiration";
};
/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/


