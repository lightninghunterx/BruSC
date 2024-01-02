/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999-2007
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

/****************************************************************/
/*  PARAMETER CLASSES                 */
/****************************************************************/


/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/

pargroup
{
  PVM_EffSWh;
  PVM_EchoPosition;
  EncGradDur;
  PVM_AcquisitionTime;
  ReadSpoiler;
  SliceSpoiler;
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
  PVM_NEchoImages;
  EchoAcqMode;
  EchoSpacing;
  MinEchoSpacing;
  EffectiveTE;
} Echoes;

pargroup
{
  DummyScans_Parameters;

  PVM_EvolutionOnOff;
  Evolution_Parameters;

  PVM_TriggerModule;
  Trigger_Parameters;

  PVM_TaggingOnOff;
  Tagging_Parameters;

  PVM_SelIrOnOff;
  Selective_IR_Parameters;

  PVM_BlBloodOnOff;
  BlackBlood_Parameters;

  PVM_FatSupOnOff;
  Fat_Sup_Parameters;

  PVM_SatTransOnOff;
  Sat_Transfer_Parameters;

  PVM_FovSatOnOff;
  Fov_Sat_Parameters;

  PVM_InFlowSatOnOff;
  Flow_Sat_Parameters;

  PVM_MotionSupOnOff;

  RFSpoiling;

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
  Echoes;
  Sequence_Details;
  ImageGeometry;
  Preparation;
  MapShim;
  MethodAtsGroup;
} MethodClass;

// parameters that should be tested after any editing
conflicts
{
  PVM_EchoTime;
  PVM_RepetitionTime;
  PVM_Fov;
  PVM_SliceThick;
};

// parameters for reconstruction
extend pargroup
{
  RecoMethMode;
  WeightingMode;
  MaskWeighting;
  GaussBroadening;
  MethodAtsRecoGroup;
}attributes
{
  display_name "Reconstruction Options";
}MethodRecoGroup;

/****************************************************************/
/*  E N D   O F   F I L E          */
/****************************************************************/
