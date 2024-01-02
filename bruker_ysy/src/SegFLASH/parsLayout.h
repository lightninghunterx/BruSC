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
  ReadSpoiler;
  SliceSpoiler;
  SegSpoiler;
  SegmentSliceOrder;
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
  TriggerDur;
  AcqTriggerDelay;
  AcqTriggerModuleTime;
}
attributes
{
  display_name "Acquisition Trigger";
} Acq_Trigger_Parameters;

pargroup
{  
  TriggerDur;
  InvTriggerDelay;
  InvTriggerModuleTime;
}
attributes
{
  display_name "Inversion Trigger";
} Inv_Trigger_Parameters;

pargroup
{
  FlowCompDirection;
}
attributes
{
  display_name "Flow Compensation";
} Flow_Comp_Parameters;

pargroup
{
  DummyScans_Parameters;
  
  PVM_NMovieFrames;
  FrameRepTime;

  PVM_EvolutionOnOff;
  Evolution_Parameters;

  TriggerOnOff;
  AcqTrigger;
  Acq_Trigger_Parameters;
  InvTrigger;
  Inv_Trigger_Parameters;

  FlowCompOnOff;
  Flow_Comp_Parameters;

  PVM_TaggingOnOff;
  Tagging_Parameters;

  InversionOnOff;
  InversionMode;
  PVM_SelIrOnOff;
  PVM_InterInvDelay;
  PVM_InterInvEndDelay;
  Selective_IR_Parameters;

  PVM_FatSupOnOff;
  Fat_Sup_Parameters;

  PVM_SatTransOnOff;
  Sat_Transfer_Parameters;

  PVM_FovSatOnOff;
  Fov_Sat_Parameters;

  PVM_InFlowSatOnOff;
  Flow_Sat_Parameters;

  DriftComp_Parameters;

  PVM_MotionSupOnOff;

  RFSpoiling;

} Preparation;

pargroup
{
  RecoReformatSize;
  RecoReformatSerialData;
  RecoSortSerialData;
  SortArgs;
} RecoPars;

extend pargroup
{
  PVM_EchoTime;
  MinEchoRepTime;
  EchoRepTime;
  SegmentRepTime;
  SegmentSize;
  SegmentDuration;
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
  MapShim;
  RecoPars; 
  MethodAtsGroup;
} MethodClass;

// parameters that should be tested after any editing
conflicts
{
  PVM_EchoTime;
  SegmentRepTime;
  SegmentSliceOrder;
  FrameRepTime;
  PVM_SelIrInvTime;
  PVM_Fov;
  PVM_SliceThick;
};

// parameters for reconstruction 
pargroup
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
/*	E N D   O F   F I L E					*/
/****************************************************************/



