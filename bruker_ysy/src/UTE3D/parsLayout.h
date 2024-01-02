/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2009
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
  PVM_EffSWh;
  PVM_AcquisitionTime;
  Spoiling;
  RampCompYN;
  RampPoints;
  PostPoints;
  VarRampTimeYN;
  EffSlewRate;
  RampTime;
  GradSync;
  GradRes;
  PVM_MinFov;
  RecoOnline;
  DualEcho;
  FWInphase;
  StabDelay;
  MinStabDelay;
  EchoSpacing;
  MinEchoSpacing;
  EffectiveTE;
  GradShape1;
}
attributes
{
  display_name "Sequence Details";
} Sequence_Details;

pargroup
{
  DummyScans_Parameters;

  PVM_TriggerModule;
  Trigger_Parameters;

  PVM_FovSatOnOff;
  Fov_Sat_Parameters;

  PVM_FatSupOnOff;
  Fat_Sup_Parameters;

  PVM_SatTransOnOff;
  Sat_Transfer_Parameters;

  PVM_SegmentationOnOff;
  Segmentation_Parameters;

  DriftComp_Parameters;

}Preparation;

extend pargroup
{
  YesNoMinEchoTime;
  PVM_EchoTime;
  PVM_MinEchoTime;
  PVM_RepetitionTime;
  NPro;
  ProUndersampling;
  MinimumUnderSampling;
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
  Preparation;
  TrajParameters;
  MapShim;
} MethodClass;

pargroup
{
  B0Corr;
  TrajRecoParameters;
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
  ProUndersampling;
};

/* ***************************************************************/
/*	E N D   O F   F I L E					 */
/* ***************************************************************/

