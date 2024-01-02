/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2008
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/

pargroup
{
  Isis_D0;
  Isis_D4;
  Isis_D6;
  Isis_D10;
  Isis_D11;
  Isis_D13;
  Isis_D14;
  Isis_P0;
  Isis_P1;
  Isis_Gr1;
  Isis_Gr2;
  Isis_Gr3;
  Isis_Gr4;
  Isis_Gp1;
  Isis_Gp2;
  Isis_Gp3;
  Isis_Gp4;
  Isis_Gs1;
  Isis_Gs2;
  Isis_Gs3;
  Isis_Gs4;
} PPGparameters;

pargroup
{
  IsisSpoilDur;
  IsisSpoilPerc1;
  IsisSpoilPerc2;
  IsisSpoilPerc3;
  IsisSpoilCyMm1;
  IsisSpoilCyMm2;
  IsisSpoilCyMm3;
} Spoiler_Gradients;

pargroup
{
  IsisModDur;
  Spoiler_Gradients;
  IsisStabDur;
  DeadTime;
} Isis;

pargroup
{
  IsisInvBandwidthPpm;
  InvPul1Enum;
  InvPul1;
  InvPul1Ampl;
  IsisSpecExcBandwidthPpm;
  ExcPul1Enum;
  ExcPul1;
  ExcPul1Ampl;

} RF_Pulses;

pargroup
{
  IsisManAdjustment;
  IsisFov;
  PVM_Matrix;
  PVM_EffSWh;
  MapShim;
} Optimize;

pargroup 
{
  DummyScans_Parameters;
  PVM_TriggerModule;
  Trigger_Parameters;
  PVM_SatTransOnOff;
  Sat_Transfer_Parameters;
  Suppression;
  PVM_OvsOnOff;
  IsisNumOvsCycles;
  OuterVolumeSuppression;
  PVM_DecOnOff;
  Decoupling_Parameters;
  PVM_NoeOnOff;
  NOE_Parameters;
} Preparation;

extend pargroup
{
  PVM_RepetitionTime;
  PVM_NAverages;
  IsisNAverages;
  AverageList;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_DeriveGains;
  RF_Pulses;
  Nuclei;
  Isis;
  Spectroscopy;
  Voxel_Geometry;
  IsisSpatialDispl;
  Preparation;
  Optimize;
  Encoding;
  PPGparameters;
  ArrayPhaseParameters;
} MethodClass;

// parameters that should be tested after any editing
conflicts
{
  PVM_RepetitionTime;
  PVM_VoxArrSize;
};

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
