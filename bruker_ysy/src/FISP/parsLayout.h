/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2013
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
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
  FlipAngMz;
  NumPrep;
} 
attributes
{
  display_name "Starter Sequence";
} StarterSequence;

pargroup
{
  PVM_NMovieFrames;
  Seg_time;
  Const_separation;
  Seg_Sep_time;
  Seg_Sep_Fra_time;  
} Movie;

pargroup               
{
  Ssfp_preparation;
  StarterSequence;
  Alpha2_Prep_time;
  DummyScans_Parameters;
  Prescan_Delay;
  FISP_inversion_enable;
  PVM_InversionTime;
  Inv_Rep_time;    
  Movie;
  PVM_EvolutionOnOff;
  Evolution_Parameters;
  PVM_TriggerModule;
  Trigger_Parameters;
  RFSpoilerOnOff;
} Preparation;

pargroup
{
  ExcPulse1Enum;
  ExcPulse1;
  ExcPulse1Ampl;
  InvPulse1Enum;
  InvPulse1;
  InvPulse1Ampl;    
} 
attributes
{
  display_name "RF Pulses";
} RF_Pulses;

pargroup
{
  PVM_EffSWh;
  PhaseAdvance;
  PVM_EchoPosition;
  EncGradDur;
  PVM_AcquisitionTime;
  PVM_MinFov;
  PVM_MinSliceThick;
  SliceSpoiler;
  KeepSteadyState3D;
}
attributes
{
  display_name "Sequence Details";
} Sequence_Details;

extend pargroup
{
  Ssfp;
  PVM_EchoTime;
  PVM_RareFactor;
  PVM_RepetitionTime;
  Nsegments;
  Seg_mode; 
  YesNoMinScanRepTime;
  Scan_RepetitionTime;
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
  MethodAtsGroup;
} MethodClass;

// parameters that should be tested after any editing
conflicts
{
  PVM_EchoTime;
  PVM_RepetitionTime;
  PVM_Fov;
  PVM_SliceThick;
};

extend pargroup
{
  MethodAtsRecoGroup;
} attributes
{
  display_name "Reconstruction Options";
}MethodRecoGroup;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

