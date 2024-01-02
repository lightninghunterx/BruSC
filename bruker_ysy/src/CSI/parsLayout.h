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

enum_attributes AVERAGING_MODE
{
  display_name Weighted_av "Weighted";
  display_name Standard_av "Standard";
};

/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/

#include "LocalizationLayout.h"

pargroup
{
  EncGradDur;
  Spoiler;
} Sequence_Details;

pargroup
{
    NavParameters;
    PVM_NavOnOff;
    RetroFrequencyLock_OnOff;
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
  Suppression;
  PVM_MotionSupOnOff;
  PVM_OvsOnOff;
  OuterVolumeSuppression;
  PVM_DecOnOff;
  Decoupling_Parameters;
  PVM_NoeOnOff;
  NOE_Parameters;
  PVM_TriggerModule;
  Trigger_Parameters;
  PVM_FatSupOnOff;
  Fat_Sup_Parameters;
  PVM_FovSatOnOff;
  Fov_Sat_Parameters;
} Preparation;

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
  Spectroscopy;
  Encoding;
  Sequence_Details;
  ImageGeometry;
  Preparation;
  AverageList;
  AverageListSum;   
  AverageMode;
  EffResolution;
  ArrayPhaseParameters;
  Localization;
  Optimize;
  MapShim;
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
  RetroFrequencyLock_OnOff;
  KFiltering;
}attributes
{
  display_name "Reconstruction Options";
}MethodRecoGroup;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



