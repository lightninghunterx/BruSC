
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



/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/
pargroup
{
    PVM_RefScanYN;
    ReferenceScan;
    NavParameters;
    PVM_NavOnOff;
    RetroFrequencyLock_OnOff;
    OPT_ManAdjustment;
    OPT_FOV;
    PVM_EffSWh;
    PVM_Matrix;
    MapShim;
    Tune_Shim;
    DriftComp_Parameters;
} Optimize;

pargroup
{
  DummyScans_Parameters;
  PVM_TriggerModule;
  Trigger_Parameters;
  Suppression;
  PVM_OvsOnOff;
  OuterVolumeSuppression;
  PVM_DecOnOff;
  Decoupling_Parameters;
  PVM_NoeOnOff;
  NOE_Parameters;
} Preparation;

pargroup
{
    VoxPul1Enum;
    VoxPul2Enum;
    VoxPul3Enum;
    VoxPul1;
    VoxPul2;
    VoxPul3;
    VoxPul1Ampl;
    VoxPul2Ampl;
    VoxPul3Ampl;

    LockVoxPulAmpls;
    
}
attributes
{
  display_name "RF Pulses";
} RFPulses;

pargroup
{
    TE1;
    TE2;
    EchoTimeMode;
    SpoilerDuration;
    GradStabDelay;
    CalcSpoiler;
    SpoilerStrength;
    SpoilerStrengthArr;
} Press;



extend pargroup
{
    PVM_EchoTime;
    PVM_RepetitionTime;
    PVM_NAverages;
    AverageList;
    PVM_NRepetitions;
    PVM_ScanTimeStr;
    PVM_ScanTime;
    PVM_DeriveGains;
    Press;
    RFPulses;
    Nuclei;
    Spectroscopy;
    Voxel_Geometry;
    Preparation;
    Optimize;
    Encoding;
    ArrayPhaseParameters;
} MethodClass;

// parameters that should be tested after any editing
conflicts
{
  PVM_EchoTime;
  PVM_RepetitionTime;
  PVM_VoxArrSize;
};

pargroup
{
  //  RecoPhaseChan;
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


