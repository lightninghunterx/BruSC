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
  RefPulEnum;
  RefPul;
  RefPulAmpl;
  RefPulShape;
}
attributes
{
  display_name "RF Pulses";
} RFPulses;

pargroup
{
  DummyScans_Parameters;
  PVM_MotionSupOnOff;
  PVM_NMovieFrames;
  PVM_TriggerModule;
  Trigger_Parameters;
  PVM_TriggerOutOnOff;
  TriggerOut_Parameters;
  PVM_TaggingOnOff;
  Tagging_Parameters;
  PVM_FatSupOnOff;
  Fat_Sup_Parameters;
  PVM_SatTransOnOff;
  Sat_Transfer_Parameters;
  PVM_FovSatOnOff;
  Fov_Sat_Parameters;
  PVM_EvolutionOnOff;
  Evolution_Parameters;
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
  RfcSpoilGrad;
  RfcSpoilDur;
  TEWait;
  TEWaitRfc;
  TRWait;
} attributes
{
  display_name "Sequence";
} Sequence;

pargroup
{
  TrajRecoParameters;
} MethodRecoGroup;

extend pargroup
{
  PVM_EffSWh;
  PVM_SignalType;
  PVM_EchoTime;
  PVM_MinEchoTime;
  MinimiseTE;
  PVM_RepetitionTime;
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_DeriveGains;
  RFPulses;
  Nuclei;
  SpiralParameters;
  TrajParameters;
  Sequence;
  Preparation;
  Encoding;
  ImageGeometry;
  MapShim;
} MethodClass;

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

