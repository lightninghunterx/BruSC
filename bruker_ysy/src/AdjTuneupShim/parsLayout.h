/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999-2003
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


/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/

pargroup
{
  BasicShim;
  BasicShimAmp;
  TestDynamicShims;
  ShimIncAuto;
  DefaultDiameter;
  SphereDiameter;
  MaxDsIncr;
  FieldDev;
  ScIncPerc;
  ScIncHz;
  ScIncAmp;
  DynShimExpTyp;
  DynShimAcqExperiment;
  NDynShims;
  NShimPS;
  DynShimSumCurr;
  DynShimPower;
  DynamicShims;
  Selected_ShimChannel;
  SelectShimCoilIncrements;
  DynShimLoopsize;
  ShimPlateau;
  ShimRamptime;
}DynShimPars;

pargroup
{
    NBShims;
    ShimMaxSPS;
    ShimMaxCoil;
    Testshimsets;
    ScIncPerc;
    ScIncHz;
    ScIncAmp;
    ShimIncAuto;
    DefaultDiameter;
    SphereDiameter;
    MaxDsIncr;
    ShimIDs;
    BasicShim;
    BasicShimAmp;
    NFIDSteadyState;
    NFID;
    NShimFID;
    FIDArea;
    FIDFoffset;
    FIDPeakwidth;
    BaseFIDResult;
    ShimFIDResults;
    ShimFIDTestSuccess;
    TestSucceeded;
    TestMessages;
    PVM_CfgInfoMrGroup;
    ShimCoeffHwStatus;
}TestShimEffectPars;


pargroup
{
  DeadTime;
}
attributes
{
  display_name "Sequence Details";
} Sequence_Details;

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

extend pargroup
{
  PVM_RepetitionTime;
  PVM_NAverages;
  PVM_NRepetitions;
  DummyScans_Parameters;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_DeriveGains;
  RF_Pulses;
  Nuclei;
  Spectroscopy;
  Encoding;
  Sequence_Details;
  ArrayPhaseParameters;
  MapShim;
  NFID;
  DynShimPars;
} MethodClass;

// parameters that should be tested after any editing
conflicts
{
  PVM_RepetitionTime;
};

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



