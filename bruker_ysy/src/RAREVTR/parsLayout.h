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
  EncGradDur;
  PVM_AcquisitionTime;
  ReadDephDur;
  PVM_MinEchoTime;
  PVM_MinFov;
  PVM_MinSliceThick;
  RepetitionSpoiler;
  EchoSpoilingAuto;
  EchoSpoilingSlice;
  EchoSpoilingSliceEff;                         
  EchoSpoilingRead;
  EchoSpoilingReadEff;
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
  RefPulse1Enum;
  RefPulse1;
  RefPulse1Ampl;
} 
attributes
{
  display_name "RF Pulses";
} RF_Pulses;


pargroup
{

  DummyScans_Parameters;

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

  PVM_MotionSupOnOff;
  
  PVM_FlipBackOnOff;
 
} Preparation;

extend pargroup
{
  PVM_EchoTime;
  EffectiveTE;
  PVM_RareFactor;
  MultiRepTime;
  PVM_NAverages;
  T1Exp;
  RepTimeMode;
  EstT1Value;
  MinT1RepTime;
  PVM_NRepetitions;
  PVM_NEchoImages;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_DeriveGains;
  RF_Pulses;
  Nuclei;
  Encoding;
  Sequence_Details;
  ImageGeometry;
  Preparation;
  MapShim;
  MethodAtsGroup;
} MethodClass;

// parameters to be checked afer any editing
conflicts
{
  PVM_EchoTime;
  MultiRepTime;
  MinT1RepTime;
  PVM_Fov;
  PVM_SliceThick;
  PVM_NEchoImages;
};

// parameters for reconstruction
extend pargroup
{
  MethodAtsRecoGroup;
}attributes
{
  display_name "Reconstruction Options";
}MethodRecoGroup;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



