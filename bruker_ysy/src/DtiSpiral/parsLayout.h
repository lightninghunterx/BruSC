/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2007
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

/*--------------------------------------------------------------*
 * Group Definitions
 *--------------------------------------------------------------*/

pargroup
{
  ExcPulEnum;
  ExcPul;
  ExcPulAmpl;
  ExcPulShape;
}
attributes
{
  display_name "RF Pulses";
} RFPulses;

pargroup
{
  DummyScans_Parameters;
  PVM_MotionSupOnOff;
  PVM_TriggerModule;
  Trigger_Parameters;
  PVM_TriggerOutOnOff;
  TriggerOut_Parameters;
  PVM_TaggingOnOff;
  Tagging_Parameters;
  PVM_FatSupOnOff;
  Fat_Sup_Parameters;
  PVM_FovSatOnOff;
  Fov_Sat_Parameters;
} attributes
{
  display_name "Preparation";
} Preparation;

pargroup
{
  SliceGrad;
  SliceGradLim;
  SliceRephGrad;
  SliceRephGradLim;
  RephGradDur;
  PVM_AcquisitionTime;
  SliceSpoiler;
  FreqSwitch;
  RiseT;
  GradWait;
  GradSync;
  Phase3dGrad;
  Phase3DGradLim;
  TEWait;
  TEWaitRfc;
  TRWait;
} attributes
{
  display_name "Sequence";
} Sequence;


extend pargroup
{
  PVM_EffSWh;
  PVM_EchoTime;
  PVM_MinEchoTime;
  PVM_RepetitionTime;
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_DeriveGains;
  RFPulses;
  Nuclei;
  SpiralParameters;
  Diffusion;
  TrajParameters;
  Sequence;
  Preparation;
  Encoding;
  ImageGeometry;
  MapShim;
} MethodClass;

// parameters for reconstruction 
extend pargroup
{
  RecoMethMode;
  TraceWeight;
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
  PVM_SliceThick;
};

/* ***************************************************************/
/*  E N D   O F   F I L E                                        */
/* ***************************************************************/

