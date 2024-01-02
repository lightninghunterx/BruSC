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
/*	PARAMETER CLASSES				       	*/
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

  PVM_EvolutionOnOff;
  Evolution_Parameters;

  PVM_TriggerModule;
  Trigger_Parameters;
 
} Preparation;

pargroup
{
    DESS_TE1;
    DESS_TE2;
    DESS_TE2_min;
    DESS_equal_TEs;
    DESS_spoiler_flat;
    DESS_RF_cycle;
    DESS_ReadSpoiling;
    DESS_ReadSpoilGrad;
    DESS_SliceSpoiling;
    DESS_SliceSpoilGrad;
    PVM_MinRepetitionTime;
    PVM_MinEchoTime;
    FWInphase;
}
attributes
{
  display_name "DESS";
} DESS;

extend pargroup
{
  PVM_EchoTime;
  PVM_RepetitionTime;
  PVM_NEchoImages;
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_DeriveGains;
  RF_Pulses;
  Nuclei;
  Encoding;
  DESS;
  Sequence_Details;
  ImageGeometry;
  Preparation;
  MapShim;
  FieldMapImages;
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
pargroup
{
  RecoMethMode;
  MapSnr;
  MethodAtsRecoGroup;
}attributes
{
  display_name "Reconstruction Options";
}MethodRecoGroup;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



