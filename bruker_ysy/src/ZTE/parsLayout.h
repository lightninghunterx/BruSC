/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002-2004
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

#include "RFPulseLayout.h"

/*--------------------------------------------------------------*
 * Group Definitions
 *--------------------------------------------------------------*/

pargroup
{
  RecoSkip;
  RecoUseOrigin;
  RecoScaleOrigin;
  RecoProCorr;
  RecoOver;
  RecoRegularise;
  RecoStorePro;
  T2Estimate;
  RecoT2Filter;
  B0Estimate;
  RecoOffResonance;
  RecoPhc0;
  RecoAmp0;
  RecoT2Factor;
  RecoB0Factor;
} MethodRecoGroup;

pargroup
{
  RecoOnline;
  MissingData;
  Spoiling;
  Reorder;
  GradOff;
  ReadGrad;
  GradDutyCycle;
  OverSampling;
  UseOrigin;
  AcqDelay;  
  HalfAcquisition;
  GradMaxDutyCycle;
  GradSettle;
  GradAmpR;
  GradAmpP;
  GradAmpS;
  GradR;
  GradP;
  GradS;
  EndOfScan;
  TRWait;
  NPoints;
  NShots;
  AcqDelayTotal;
  BandWidth; // deprecated (only used for loading old protocols)
} Sequence;

pargroup
{
  DummyScans_Parameters;
  PVM_FatSupOnOff;
  Fat_Sup_Parameters;
  PVM_FovSatOnOff;
  Fov_Sat_Parameters;
  PVM_SegmentationOnOff;
  Segmentation_Parameters;
  PVM_EvolutionOnOff;
  Evolution_Parameters;
  PVM_TriggerModule;
  Trigger_Parameters;
} Preparation;

extend pargroup
{
  PVM_EffSWh;
  PVM_AcquisitionTime;
  ProUnderSampling;
  MinimumUnderSampling;
  NPro;
  PVM_RepetitionTime;
  MinimumTR;
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_ScanTime;
  PVM_ScanTimeStr;
  PVM_DeriveGains;
  RF_Pulses;
  Sequence;
  ImageGeometry;
  Preparation;
  Nuclei;
  Encoding;
  PVM_MinRepetitionTime;
  MapShim;
} MethodClass;

/*--------------------------------------------------------------*
 * Conflicts
 *--------------------------------------------------------------*/

conflicts
{
  PVM_RepetitionTime;
  PVM_Fov;
  ProUnderSampling;
};

/* ***************************************************************/
/*	E N D   O F   F I L E					 */
/* ***************************************************************/

