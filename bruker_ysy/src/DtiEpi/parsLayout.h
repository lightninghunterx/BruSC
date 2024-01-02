/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

/****************************************************************/
/*	PARAMETER CLASE_EPIS				       	*/
/****************************************************************/

#include "Phase3DLayout.h"
#include "SliceSelLayout.h"
#include "RFPulseLayout.h"



/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/

pargroup
{
  ShowAllPars;
  GradStabTime;
  Phase3DEncoding;
  SliceSelection;
}
attributes
{
  display_name "Sequence Details";
}SequenceDetails;


pargroup
{
  DummyScans_Parameters;
  PVM_TriggerModule;
  Trigger_Parameters;
  PVM_FatSupOnOff;
  Fat_Sup_Parameters;
  PVM_FovSatOnOff;
  Fov_Sat_Parameters;
  DriftComp_Parameters;
} Preparation;

extend pargroup
{
  PVM_EffSWh;
  EchoTime;
  PVM_EchoTime;    
  PVM_MinEchoTime;
  NSegments;
  PVM_RepetitionTime;
  PackDel;
  PVM_NEchoImages; 
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_DeriveGains;
  Diffusion;
  Encoding;
  EPI_Parameters;
  RF_Pulses;
  Nuclei;
  SequenceDetails;
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

// parameters that should be tested after any editing
conflicts
{
  EchoTime;
  PVM_RepetitionTime;
  PVM_Fov;
  PVM_SliceThick;
};


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

