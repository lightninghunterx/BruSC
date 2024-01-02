/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2011
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
double parameter RephSliceDur;
double parameter TeFillDelay;
double parameter TrFillDelay;
double parameter {relations backbone;}Rew2DGrad;
double parameter {relations backbone;}Rew3DGrad;
double parameter {relations backbone;}RewGradDur;
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

YesNo parameter
{
  display_name "RF Spoiling";
  relations backbone;
}RFSpoiling;

double parameter RFPhaseList[];

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
  display_name "Segments";
  short_description "In case of Segments > 1, only a part of k-space is acquired inside the Oversampling loop before moving to the next segment.";
  minimum 1 outofrange nearestval;
  maximum 1024 outofrange nearestval;
  relations    backbone;
}Nsegments;

SEGMENTING_MODE parameter
{
  display_name "Segmenting Mode";
  short_description "Way phase encoding is performed in case of segmentation.";
  relations    backbone;
} Seg_mode;

YesNo parameter
{
  display_name "Angio Mode";
  short_description "Inflow contrast is achieved by changing the loop structure in multi-slice experiments.";
  relations backbone;
}AngioMode;

/* parameters for flow compensation */

YesNo parameter
{
  display_name "Flow Compensation";
  relations backbone;
}FlowCompYesNo;

double parameter ReadFcGrad[2];
double parameter Phase2DFcGrad[2];
double parameter Phase3DFcGrad[2];
double parameter ExcSliceFcGrad[2];
double parameter P2Shape[];
double parameter P3Shape[];
double parameter RShape[];
double parameter SShape[];
int parameter ShapeSize;

/* special parameters for reconstruction */
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

YesNo parameter
{
  display_name "Respiration Gating";
  short_description "If physical respiratory gating is applied, retrospective gating is only used for cardiac phases.";
  relations backbone;
}RespGate;

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
  minimum 0.1 outofrange nearestval;
  maximum 1 outofrange nearestval;
}FreezePhaseRange;

int parameter
{
    display_name "Movie Frames";
    short_description "Number of movie frames to be reconstructed from data.";
    relations MovieFramesRelations;
} OutputMovieFrames;

YesNo parameter
{
  display_name "Online Navigator Signal";
  short_description "Display time course of navigator signal during setup mode.";
  relations backbone;
}NavSigSetupYesNo;

int parameter {relations backbone;}TransActionBlGSP;

YesNo parameter IgRecoDebug;

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
enum_attributes SEGMENTING_MODE
{
  display_name sequential  "Sequential";
  display_name interleaved "Interleaved";
};

enum_attributes ANALYSIS_MODE
{
  display_name CardiacResp  "Cardiac and Respiration";
  display_name OnlyCardiac "Only Cardiac";
  display_name OnlyResp "Only Respiration";
};
/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

