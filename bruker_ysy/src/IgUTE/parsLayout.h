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

/* ***************************************************************/
/*	PARAMETER CLASS	               			       	 */
/* ***************************************************************/


/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/
pargroup
{	 
  RampMode; 
  EffSlewRate;
  Ramptime;
}SliceSelection;

pargroup
{
  PVM_EffSWh;
  PVM_AcquisitionTime;
  SliceSelection;
  ReadSpoiling;
  SliceSpoiler;
  RampCompYN;
  RampPoints;
  ReadDephPoints;
  GradSync;
  PVM_MinFov;
  PVM_MinSliceThick;
  GradShape2;
  GradRes;
  Oversampling;
  SelfGating;
  AddCentralPoints;
  DriftComp_Parameters;
  PET_MR_Study;
  IgTTLOut;
  IgTTLDuration;
}
attributes
{
  display_name "Sequence Details";
} Sequence_Details;

pargroup
{
  ExcPulse1Enum;
  ExcPulse1;
  ExcPulse1Ampl;
} 
attributes
{
  display_name "RF Pulses";
} RF_Pulses;

pargroup
{
  DummyScans_Parameters;
  PVM_FovSatOnOff;
  Fov_Sat_Parameters;
  PVM_FatSupOnOff;
  Fat_Sup_Parameters;
  PVM_SatTransOnOff;
  Sat_Transfer_Parameters;
  PVM_NMovieFrames;
  PVM_NavOnOff;
  NavParameters;
  PVM_TriggerModule;
  Trigger_Parameters; 
}Preparation;

extend pargroup
{
  YesNoMinEchoTime;
  PVM_EchoTime;
  PVM_MinEchoTime;
  PVM_RepetitionTime;
  PVM_NAverages;
  NPro;
  ProUndersampling;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_DeriveGains;
  RF_Pulses;
  Nuclei;
  Encoding;
  Sequence_Details;
  ImageGeometry;
  Preparation;
  TrajParameters;
  MapShim;
} MethodClass;

pargroup
{
  B0Corr;
  TrajRecoParameters;
  HeartRate;
  CardFreq;
  RespRate;
  RespFreq;
  CardiacPeriod;
  RespPeriod;
  HeartRateRecoResult;
  RespRateRecoResult;
  MovieType;
  AnalysisOfMotion;
  NavigatorAnalysis;
  TimeGridding;
  GaussWidth;
  FreezePhase0;
  FreezePhaseRange0;
  FreezePhase;
  FreezePhaseRange;
  OutputMovieFrames;
  StoreOutputFiles;
  IgRecoDebug;
  CompressedSensing;
}
attributes
{
  display_name "Reconstruction Options";
}MethodRecoGroup;

/*--------------------------------------------------------------*
 * Conflicts
 *--------------------------------------------------------------*/
conflicts
{
  PVM_EchoTime;
  PVM_RepetitionTime;
  PVM_Fov;
  PVM_SliceThick;
};

/* ***************************************************************/
/*	E N D   O F   F I L E					 */
/* ***************************************************************/

