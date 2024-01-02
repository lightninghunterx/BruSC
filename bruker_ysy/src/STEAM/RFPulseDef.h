
/****************************************************************
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
 ****************************************************************/


/*pulse 1*/

PV_PULSE_LIST parameter
{
  display_name "RF Pulse 1";
  relations    VoxPul1EnumRel;
}VoxPul1Enum;


PVM_RF_PULSE parameter
{
  display_name "Pulse 1";
  relations VoxPul1Rel;
}VoxPul1;


PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "RF Pulse 1 Amplitude";
  relations VoxPul1AmplRel;
}VoxPul1Ampl;


double parameter
{
  relations backbone;
}VoxPul1Shape[];


/*pulse2*/

PV_PULSE_LIST parameter
{
  display_name "RF Pulse 2";
  relations    VoxPul2EnumRel;
}VoxPul2Enum;


PVM_RF_PULSE parameter
{
  display_name "Pulse 2";
  relations VoxPul2Rel;
}VoxPul2;


PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "RF Pulse 2 Amplitude";
  relations VoxPul2AmplRel;
}VoxPul2Ampl;


double parameter
{
  relations backbone;
}VoxPul2Shape[];


/*pulse3*/

PV_PULSE_LIST parameter
{
  display_name "RF Pulse 3";
  relations    VoxPul3EnumRel;
}VoxPul3Enum;


PVM_RF_PULSE parameter
{
  display_name "Pulse 3";
  relations VoxPul3Rel;
}VoxPul3;


PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "RF Pulse 3 Amplitude";
  relations VoxPul3AmplRel;
}VoxPul3Ampl;


double parameter
{
  relations backbone;
}VoxPul3Shape[];


YesNo parameter 
{
  display_name "Lock Amplitudes";
  short_description "Locks relative amlitude ratio of the voxel pulses.";
  relations LockVoxPulAmplsRelation;
} LockVoxPulAmpls;

