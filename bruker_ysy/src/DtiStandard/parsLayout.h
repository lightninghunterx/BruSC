/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002-2003
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
  PVM_EffSWh;
  PVM_AcquisitionTime;
  PVM_EchoPosition;
  EncGradDur;
  ExcSliceRephTime;
  PVM_MinFov;
  PVM_MinSliceThick;
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

  PVM_TriggerModule;
  Trigger_Parameters;

  PVM_FatSupOnOff;
  Fat_Sup_Parameters;

  PVM_FovSatOnOff;
  Fov_Sat_Parameters;

  PVM_MotionSupOnOff;
}Preparation;

extend pargroup
{
  PVM_EchoTime;
  PVM_MinEchoTime;
  PVM_RepetitionTime;
  PVM_NEchoImages;
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_DeriveGains;
  Diffusion;
  RF_Pulses;
  Nuclei;
  Encoding;
  Sequence_Details;
  ImageGeometry;
  Preparation;
  MapShim;
} MethodClass;

// parameters for reconstruction 
extend pargroup
{
  RecoMethMode;
  TraceWeight;
}
attributes
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
