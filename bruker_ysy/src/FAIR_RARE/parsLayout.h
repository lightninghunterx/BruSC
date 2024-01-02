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
  RampForm;
  SlewRatePerCent;
  GradStab;
  PVM_AcquisitionTime;
  AutoSpoil;
  RepetitionSpoiler;
  SpoilingReadPx;
  SpoilingSliceSl;
  ShapeSize;
} Sequence_Details;

pargroup
{
  SliceBandWidthScale;
  ExcPulse1Enum;
  ExcPulse1;
  ExcPulse1Ampl;
  RefPulse1Enum;
  RefPulse1;
  RefPulse1Ampl;
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
 
  PVM_MotionSupOnOff;
 
} Preparation;

extend pargroup
{
  PVM_EchoTime;
  EffectiveTE;
  RecoveryTime;
  PVM_RepetitionTime;
  MultiRepTime;
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_RareFactor;
  PVM_NEchoImages;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_DeriveGains;
  RF_Pulses;
  Nuclei;
  Encoding;
  FairParameters;
  Sequence_Details;
  StandardInplaneGeometry;
  StandardSliceGeometry;
  Preparation;
  MapShim;
} MethodClass;

// parameters to be checked afer any editing
conflicts
{
  PVM_EchoTime;
  PVM_RepetitionTime;
  PVM_Fov;
  PVM_SliceThick;
};

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



