/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2004
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/


/****************************************************************/
/* INCLUDE FILES                                                */
/****************************************************************/

#include "RFPulsePars.h"
#include "radial.h"


/****************************************************************/
/* DISPLAY PARAMETERS                                           */
/****************************************************************/

double parameter {editable false;}RiseTime;

/*** acquisition ***/

YesNo parameter
{
  display_name "Minimize TR";
  short_description "Use minimum repetition time.";
  relations backbone;
} MinimumTR;

double parameter
{
  display_name "Missing Data";
  short_description "Initial part of FID missing for reconstruction.";
  units "{dwells}";
  format "%.2f";
  editable false;
} MissingData;

// research
int parameter
{
  display_name "Oversampling";
  short_description "Acquisition oversampling factor.";
  minimum 2;
  maximum 8;
  relations backbone;
} OverSampling;

// research
double parameter
{
  display_name "Acquisition Delay";
  short_description "Additional delay between pulse and acquisition.";
  units "us";
  format "%.3f";
  minimum 0.0;
  maximum 100.0;
  relations backbone;
} AcqDelay;

// research
YesNo parameter
{
  display_name "Use Origin";
  short_description "Determine k=0 for improved reconstruction.";
  relations backbone;
} UseOrigin;


/*** radial projections ***/

double parameter
{
  display_name "Polar Undersampling";
  short_description "The k-space density is reduced by this factor.";
  format "%.3f";
  minimum 1;
  maximum 10;
  relations backbone;
} ProUnderSampling;

YesNo parameter
{
  display_name "Minimize Undersampling";
  short_description "Use minimum undersampling factor.";
  relations backbone;
} MinimumUnderSampling;

int parameter
{
  display_name "Projections";
  short_description "Total number of projections.";
  editable false;
  relations backbone;
} NPro;

// research
YesNo parameter
{
  display_name "Half Acquisition";
  short_description "Acquire only one hemisphere in k-space.";
  relations HalfAcqRel;
} HalfAcquisition;


/*** gradients ***/

double parameter
{
  display_name "Spoiling";
  short_description "Minimum readout gradient spoiling before RF pulse.";
  units "{cycles}";
  format "%.2f";
  minimum 0.0;
  maximum 100.0;
  relations backbone;
} Spoiling;

YesNo parameter
{
  display_name "Switch Gradient";
  short_description "Do not step but switch gradient (louder, but reduced duty cycle).";
  relations backbone;
} GradOff;

double parameter
{
  display_name "Gradient Strength";
  short_description "Maximum read gradient strength.";
  units "%";
  format "%.2f";
  editable false;
} ReadGrad;

double parameter
{
  display_name "Gradient Duty Cycle";
  short_description "Duty cycle of read gradient strength.";
  units "%";
  format "%.2f";
  editable false;
} GradDutyCycle;


/*** reconstruction ***/

YesNo parameter
{
  display_name "Online Reconstruction";
  short_description "Switch off to enable larger matrices.";
  relations backbone;
} RecoOnline;

int parameter
{
  display_name "Skip Initial Data";
  short_description "Initial part of FID skipped for reco.";
  units "{over-dwells}";
  minimum 0;
  relations RecoSkipRel;
} RecoSkip;

YesNo parameter
{
  display_name "Use Origin for Reco";
  short_description "Use magnitude of origin for reco.";
  relations SetRecoParam;
} RecoUseOrigin;

double parameter
{
  display_name "Scaling of Origin";
  short_description "Correction for background signal.";
  format "%.2f";
  minimum 0.1;
  maximum 10.0;
} RecoScaleOrigin;

ProCorrType parameter
{
  display_name "Projection Correction";
  short_description "Correction of projections for acquisition-related effects.";
} RecoProCorr;

int parameter
{
  display_name "Oversampling";
  short_description "Oversampling factor for SRF-optimised reconstruction.";
  minimum 1;
  maximum 16;
} RecoOver;

double parameter
{
  display_name "Regularisation";
  short_description "Condition number for regularisation of reconstruction.";
  format "%.2e";
  minimum 0.0;
  maximum 1.0;
} RecoRegularise;

double parameter
{
  display_name "T2* Estimate";
  short_description "The estimation is set after acquisition of k=0.";
  units "ms";
  format "%.3f";
  editable false;
} T2Estimate;

double parameter
{
  display_name "T2* Filter";
  short_description "Perform exponential filtering in reconstruction.";
  units "ms";
  format "%.3f";
  minimum 0.0;
  maximum 1000.0;
} RecoT2Filter;

double parameter
{
  display_name "B0 Estimate";
  short_description "The estimation is set after acquisition of k=0.";
  units "Hz";
  format "%.2f";
  editable false;
} B0Estimate;

double parameter
{
  display_name "Off-resonance";
  short_description "Perform off-resonance demodulation.";
  units "Hz";
  format "%.2f";
  relations SetRecoParam;
} RecoOffResonance;

YesNo parameter
{
  display_name "Store Projections";
  short_description "Store intermediate 1D projections.";
} RecoStorePro;

double parameter
{
  display_name "Origin Amplitude";
  short_description "Amplitude at k=0 for each channel.";
  editable false;
} RecoAmp0[];

double parameter
{
  display_name "Origin Phase";
  short_description "Phase at k=0 for each channel.";
  editable false;
} RecoPhc0[];

double parameter
{
  display_name "T2 Factor";
  short_description "Stores T2 factor from acquisition of k=0 for each channel.";
  format "%.2f";
  editable false;
  relations RecoT2FactorRel;
} RecoT2Factor[];

double parameter
{
  display_name "B0 factor";
  short_description "Stores B0 factor from acquisition of k=0 for each channel.";
  format "%.2f";
  editable false;
  relations RecoB0FactorRel;
} RecoB0Factor[];

/****************************************************************/
/* INTERNAL GLOBAL PARAMETERS                                   */
/****************************************************************/

double parameter
{
  display_name "Gradient Duty Cycle";
  short_description "Maximum possible with respect to maximum allowed gradient duty cycle.";
  units "%";
  format "%.2f";
  editable false;
} GradMaxDutyCycle;

double parameter
{
  display_name "Gradient Settling";
  short_description "Gradient settling time before RF pulse.";
  units "ms";
  format "%.3f";
  editable false;
} GradSettle;

double parameter
{
  display_name "End of Scan";
  short_description "Time to wait after acquisition.";
  units "ms";
  format "%.3f";
  editable false;
} EndOfScan;

double parameter
{
  display_name "TR Delay";
  short_description "Waiting time to realise TR.";
  units "ms";
  format "%.3f";
  editable false;
} TRWait;

int parameter
{
  display_name "Acquired Points";
  short_description "Number of complex points per half radial acquisition.";
  editable false;
} NPoints;

int parameter
{
  display_name "Shots";
  short_description "Number of shots per projection.";
  editable false;
} NShots;

double parameter
{
  display_name "Signal Delay";
  short_description "Time of first signal acquired.";
  units "us";
  format "%.3f";
  editable false;
} AcqDelayTotal;

double parameter
{
  display_name "Gradient Read";
  short_description "Gradient amplitude in read dimension.";
  units "%";
  format "%.2f";
  editable false;
} GradAmpR;

double parameter
{
  display_name "Gradient Phase";
  short_description "Gradient amplitude in phase dimension.";
  units "%";
  format "%.2f";
  editable false;
} GradAmpP;

double parameter
{
  display_name "Gradient Slice";
  short_description "Gradient amplitude in slice dimension.";
  units "%";
  format "%.2f";
  editable false;
} GradAmpS;

double parameter
{
  display_name "Scaling Read";
  short_description "Read gradient scaling per projection.";
  format "%.2f";
  editable false;
  store false;
} GradR[];

double parameter
{
  display_name "Scaling Phase";
  short_description "Phase gradient scaling per projection.";
  format "%.2f";
  editable false;
  store false;
} GradP[];

double parameter
{
  display_name "Scaling Slice";
  short_description "Slice gradient scaling per projection.";
  format "%.2f";
  editable false;
  store false;
} GradS[];

double parameter
{
  short_description "Duration of one repetition.";
  units "s";
  format "%.3f";
  editable false;
} OneRepTime; 
            
// deprecated (only used for loading old protocols)
double parameter 
{
  editable false;
  visible false;
  store false;
} BandWidth;

/****************************************************************/
/* TYPE NAMES                                                   */
/****************************************************************/

enum_attributes ProCorrType
{
  display_name ProCorr_None   "None";
  display_name ProCorr_Pulse  "Pulse";
  display_name ProCorr_Filter "Filter";
  display_name ProCorr_Both   "Pulse + Filter";
};

/****************************************************************/
/*                    E N D   O F   F I L E                     */
/****************************************************************/


