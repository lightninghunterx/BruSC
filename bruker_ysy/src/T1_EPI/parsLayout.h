/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2008
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
  InpSlices;
  IROffset; 
  IRSpacing;
  NIRPoints;
  SliceGroupSize;
  SliceGroupRotation;
  IRTime;
  InvSliceSpoiler;
}
attributes
{
  display_name "Inversion Recovery";
} IR_Parameters;


pargroup
{
  PVM_EffSWh; 
  RephaseTime;
  PVM_MinFov;
  PVM_MinSliceThick;
  SliceSpoiler;
}
attributes
{
  display_name "Sequence Details";
} Sequence_Details;

pargroup
{
  ExcPulseEnum;
  ExcPul;
  ExcPulseAmpl;
  ExcPulseShape;
  RefPulseEnum;
  RefPul;
  RefPulseAmpl;
  RefPulseShape;
  InvPulseEnum;
  InvPul;
  InvPulseAmpl;
  InvPulseShape;
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

  PVM_TaggingOnOff;
  Tagging_Parameters;

  PVM_FatSupOnOff;
  Fat_Sup_Parameters;

  PVM_SatTransOnOff;
  Sat_Transfer_Parameters;

  PVM_FovSatOnOff;
  Fov_Sat_Parameters;

  PVM_TriggerOutOnOff;
  TriggerOut_Parameters;

} Preparation;

extend pargroup
{
  PVM_EffSWh;
  InpEchoTime;
  PVM_MinEchoTime;
  PVM_EchoTime;
  NSegments;
  PVM_RepetitionTime;
  PackDel;
  PVM_NEchoImages;
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_SignalType;
  PVM_DeriveGains;
  Encoding;
  IR_Parameters;
  EPI_Parameters;
  RF_Pulses;
  Nuclei;
  Sequence_Details;
  ImageGeometry;
  Preparation;
  MapShim;
} MethodClass;

// parameters that should be tested after any editing
conflicts
{
  PVM_SignalType;
  InpEchoTime;
  PVM_RepetitionTime;
  PVM_Fov;
  PVM_SliceThick;
  IRSpacing;
};


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



