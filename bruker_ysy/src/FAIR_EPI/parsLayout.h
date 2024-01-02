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
  ExSliceRephaseTime;
  SliceSpoiler;
}
attributes
{
  display_name "Sequence Details";
} Sequence_Details;

pargroup
{
  BwScale;
  ExcPulse1Enum;
  ExcPulse1;
  ExcPulse1Ampl;
  ExcPulse1Shape;

  RefPulse1Enum;
  RefPulse1;
  RefPulse1Ampl;
  RefPulse1Shape;
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

  PVM_FovSatOnOff;
  Fov_Sat_Parameters;

  PVM_SatTransOnOff;
  Sat_Transfer_Parameters;

} Preparation;


pargroup
{
  PVM_EchoTime1;
  PVM_EchoTime;
  PVM_NEchoImages;
} ScanEditorInterface;


extend pargroup
{
  PVM_EffSWh;
  EchoTime;
  PVM_MinEchoTime;
  NSegments;
  RecoveryTime;
  PVM_RepetitionTime;
  MultiRepTime;
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_SignalType;
  PVM_DeriveGains;
  Encoding;
  FairParameters;
  EPI_Parameters;
  RF_Pulses;
  Nuclei;
  Sequence_Details;
  ImageGeometry;
  Preparation;
  MapShim;
  ScanEditorInterface;
} MethodClass;

// parameters that should be tested after any editing
conflicts
{
  EchoTime;
  PVM_RepetitionTime;
  PVM_Fov;
  PVM_SliceThick;
};

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



