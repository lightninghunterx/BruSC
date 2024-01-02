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
/*	PARAMETER CLASSES				       	*/
/****************************************************************/


/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/

pargroup
{
  PVM_EffSWh;
  PVM_EchoPosition;
  PVM_AcquisitionTime;
  PVM_MinEchoTime;
  EncGradDur;
  ReadSpoiler;
  SliceSpoiler;
  Oversampling;
  Nsegments;
  PVM_RareFactor;
  Seg_mode;
  SelfGating;
  RespGate;
  NavSigSetupYesNo;
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

  PVM_NMovieFrames;

  PVM_TriggerModule;
  Trigger_Parameters;

  PVM_FatSupOnOff;
  Fat_Sup_Parameters;

  PVM_SatTransOnOff;
  Sat_Transfer_Parameters;

  PVM_FovSatOnOff;
  Fov_Sat_Parameters;

  PVM_InFlowSatOnOff;
  Flow_Sat_Parameters;

  RFSpoiling;
  AngioMode;
  FlowCompYesNo;
  
} Preparation;

extend pargroup
{
  PVM_EchoTime;
  PVM_RepetitionTime;
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_DeriveGains;
  RF_Pulses;
  Nuclei;
  Encoding;
  Sequence_Details;
  ImageGeometry;
  NavParameters; 
  Preparation;
  MapShim;
} MethodClass;

// parameters that should be tested after any editing
conflicts
{
  PVM_EchoTime;
  PVM_RepetitionTime;
  PVM_Fov;
  PVM_SliceThick;
  PVM_SPackArrNSlices;
};


// parameters for reconstruction 
pargroup
{
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
}attributes
{
  display_name "Reconstruction Options";
}MethodRecoGroup;


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



