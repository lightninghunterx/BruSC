/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2020
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

#include "SliceSelLayout.h"

/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/

pargroup
{
  ExcPulseEnum;
  ExcPulse;
  ExcDurList;
  ExcDurRange;
  ExcPowList;
  ExcPowRange;
  ExcVoltList;
}
attributes
{
  display_name "RF Pulses";
} RF_Pulses;


pargroup
{
  PhaseCorrVals;
  PkRange;
  PkArea;
  PkMaxIdx;
  PkMaxIdxFrac;
  PkZeroCross;
  SearchDurRange;
  SearchPowRange;
  SearchVoltRange;
}
attributes
{
  display_name "AdjX Parameters";
} AdjX_Parameters;


pargroup
{
  PVM_EchoTime;
  AutoEchoTimer;
  PVM_NAverages;
  AverageList;
  PVM_NRepetitions;
  NExcSteps;
  PVM_RepetitionTime;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_NumberOfNuclei;
  ScanMode;
  NoiseScan;
  RF_Pulses;       //grp
  AdjX_Parameters; //grp
  AdjRefPowXLog;
  Messages;
  SkipSubAdjustments;
}
attributes
{
  display_name "Sequence Details";
} Sequence_Details;



pargroup
{
  DummyScans_Parameters;

  PVM_TriggerModule;
  Trigger_Parameters;

  PVM_FatSupOnOff;
  Fat_Sup_Parameters;

  PVM_FovSatOnOff;
  Fov_Sat_Parameters;

  Suppression;

  PVM_DecOnOff;
  Decoupling_Parameters;

  PVM_NoeOnOff;
  NOE_Parameters;

  SliceSelOnOff;
  Slice_Sel_Parameters;

  DriftComp_Parameters;
} Preparation;



extend pargroup
{
  PVM_DeriveGains;
  Nuclei;
  Spectroscopy;
  Preparation;
  Encoding;
  Sequence_Details;
  ArrayPhaseParameters;
  MapShim;
  PVM_CfgInfoMrGroup;
} MethodClass;


conflicts
{
  PVM_RepetitionTime;
  PVM_RefScanYN;
  Sat_Transfer_Parameters;
  SliceSelOnOff;
  SearchMode;
  RecoMode;
  ScanMode;
};


// parameters for reconstruction
pargroup
{
  PhaseCorrVals;
  PkRange;
  PkArea;
  PkMaxIdx;
  PkMaxIdxFrac;
  PkZeroCross;
  Messages;
  AdjRefPowXLog;
  RecoMode;
  ScanMode;
  SearchMode;
  SearchDurRange;
  SearchPowRange;
  SearchVoltRange;
  PVM_NumberOfNuclei;
}attributes
{
  display_name "Reconstruction Options";
} MethodRecoGroup;


pargroup
{
  Method;
  MethManagGroup;
  MethodRecoGroup;
  PVM_DeriveGains;
  Spectroscopy;
  Encoding;
  Sequence_Details;
  ArrayPhaseParameters;
  MapShim;
  PVM_CfgInfoMrGroup;

  //all  prep pars but not trigger
  DummyScans_Parameters;

  PVM_FatSupOnOff;
  Fat_Sup_Parameters;

  PVM_FovSatOnOff;
  Fov_Sat_Parameters;

  Suppression;

  PVM_DecOnOff;
  Decoupling_Parameters;

  PVM_NoeOnOff;
  NOE_Parameters;

  SliceSelOnOff;
  Slice_Sel_Parameters;
}
attributes
{
  // post read is needed since loadMeth is only called for
  // MethodClass automatically.
  postread_proc methodProtocolPostRead;

} MethodProtocolGroup;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
