/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2018
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
  DeadTime;
  EchoTime;
}
attributes
{
  display_name "Sequence Details";
} Sequence_Details;


pargroup
{
  NavParameters;
  PVM_NavOnOff;
  Edc_OnOff;
  RetroFrequencyLock_OnOff;
  PVM_RefScanYN;
  ReferenceScan;
  DriftComp_Parameters;
} Optimize;

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

  Suppression;

  PVM_DecOnOff;
  Decoupling_Parameters;

  PVM_NoeOnOff;
  NOE_Parameters;

  PVM_SatTransOnOff;
  Sat_Transfer_Parameters;

  SliceSelOnOff;
  Slice_Sel_Parameters;
} Preparation;

extend pargroup
{
  PVM_RepetitionTime;
  PVM_NAverages;
  AverageList;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_DeriveGains;
  RF_Pulses;
  Nuclei;
  Spectroscopy;
  Preparation;
  Optimize;
  Encoding;
  Sequence_Details;
  ArrayPhaseParameters;
  MapShim;
} MethodClass;

// parameters that should be tested after any editing
conflicts
{
  PVM_RepetitionTime;
  PVM_RefScanYN;
  Sat_Transfer_Parameters;
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


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



