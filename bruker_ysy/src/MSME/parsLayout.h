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
  PVM_NEchoImages;
  ConstNEchoes;
  NEchoesPerEchoImage;
  NEchoes;
  EffectiveTE;
} Echoes;

pargroup
{

  DummyScans_Parameters;
 
  PVM_TriggerModule;
  Trigger_Parameters;

  PVM_EvolutionOnOff;
  Evolution_Parameters;

  PVM_SelIrOnOff;
  Selective_IR_Parameters;

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

  DriftComp_Parameters;
 
} Preparation;

extend pargroup
{
  PVM_EchoTime;
  EffectiveTE;
  PVM_RareFactor;
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

extend pargroup
{
  MethodAtsRecoGroup;
} attributes
{
  display_name "Reconstruction Options";
}MethodRecoGroup;

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



