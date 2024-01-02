/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2006
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

#include "steamLocLayout.h"

/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/
pargroup
{
  PVM_RefScanYN;
  ReferenceScan; //param group
  NavParameters; 
  PVM_NavOnOff;
  RetroFrequencyLock_OnOff;
  OPT_ManAdjustment;
  OPT_FOV;
  Tune_Shim;
  MapShim;
  DriftComp_Parameters;
} Optimize;


pargroup 
{
  DummyScans_Parameters;
  Suppression;
  PVM_OvsOnOff;
  OuterVolumeSuppression;
  PVM_DecOnOff;
  Decoupling_Parameters;
  PVM_NoeOnOff;
  NOE_Parameters;
  PVM_TriggerModule;
  Trigger_Parameters;
}Preparation;


extend pargroup
{
  PVM_EchoTime;
  PVM_MinEchoTime;
  PVM_RepetitionTime;
  StTM;
  PVM_NAverages;
  AverageList;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_DeriveGains;
  RFPulses;
  Nuclei;
  Steam;
  Spectroscopy;
  Voxel_Geometry;
  Preparation;
  Optimize;
  Encoding;
  PPGparameters;
  ArrayPhaseParameters;
} MethodClass;

// parameters that should be tested after any editing
conflicts
{
  PVM_EchoTime;
  PVM_RepetitionTime;
  PVM_VoxArrSize;
};

// parameters for reconstruction 
pargroup
{
  RetroFrequencyLock_OnOff;
  Edc_OnOff;
  EdcManualFilter;
  EdcFilterWidthHz;
  CalcLineWidth;
  LineWidth;
}attributes
{
  display_name "Reconstruction Options";
}MethodRecoGroup;


pargroup
{
  NormArea;
  LineWidth;
  Peak;
  Noise;
  PVM_FrqAdj1H;
  PVM_FrqAdj1HPpm;
  PVM_NucleiPpmRef;
  AdjErrorComment;
  AutoSFCounter;
  PVM_TuneShimCounter;
}LocShimFrqAdjGroup;


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/


