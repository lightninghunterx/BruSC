/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2006-2007
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 *
 * ***************************************************************/
static const char resid[] = "$Id$ (C) 2007 Bruker BioSpin MRI GmbH";

#define DEBUG		   0
#define DB_MODULE	 1
#define DB_LINE_NR 0


#include "method.h"



/* 
 * method backbone 
 */

void backbone(void)
{
  double PulseBandwidthArray[3];
  DB_MSG(("-->backbone"));

  /*
   * Nucleus related 
   */
  STB_UpdateNuclei(Yes); 
    

  /*
   * spectroscopic data acquisition
   */
  STB_UpdateSpectroscopy( PVM_Nucleus1 );



  /*
   * update Encoding pargroup for multi receiver experiment
   */
  STB_UpdateEncoding();

  /*
   * handle TuneShim
   */    

  ConstrainTuneShim();

  /* 
   * handle voxel pulses
   */
  UpdateRFPulses();
  PulseBandwidthArray[0] = VoxPul1.Bandwidth;
  PulseBandwidthArray[1] = VoxPul2.Bandwidth;
  PulseBandwidthArray[2] = VoxPul3.Bandwidth;
    
  /* 
   * handle voxel geometry
   */
  STB_UpdateAtsReferencePosition("PVM_VoxelGeoObj");
  STB_UpdateVoxelGeometry( PulseBandwidthArray, PVM_Nucleus1 );

  /*
   * update mapshim parameter group
   */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_VoxelGeoObj");

  updateModules();

  /*
   * update ReferenceScan pargroup
   */
  STB_UpdateMCRefScan(PVM_EncNReceivers);


  /* set up adjustment list */
  SetAdjustments();


  /*  handle manual adjustments */
  OptHandler();

  /* 
   * handle STEAM timing & gradients (after OptHandler)
   */
  UpdateSteamLocalization(PVM_Nucleus1,
                          PtrType1x3 PVM_VoxArrSize,
                          PtrType1x3 PVM_VoxArrGradient,
                          &PVM_EchoTime,
                          &PVM_MinEchoTime,
                          &VoxPul1,
                          &VoxPul2,
                          &VoxPul3,
                          &OPT_ManAdjustment,
                          &OPT_FOV);

  /* 
   * calculate repetition time
   */
  UpdateRepetitionTime();


  /* DriftCompensation (after update of TR)*/
  STB_UpdateDriftComp(PVM_RepetitionTime);

  SetBaseLevelParameter();
  SetRecoParam();

  DB_MSG(("<--backbone"));
}


/* 
 * subroutines called by the method handler 
 */


void LocalDecNoeHandling(void)
{
  DB_MSG(("-->LocalDecNoeHandling"));
  
  if((PVM_DecOnOff == On ) || (PVM_NoeOnOff == On))
  {
    if(PVM_NumberOfNuclei!=2)
    {
      STB_InitNuclei(2);
      if(PVM_NumberOfNuclei < 2)
      {
        /* system configuration: 2nd transmit channel not available */
        PVM_DecOnOff = PVM_NoeOnOff = Off;
      }
    }
  }
  else if( (PVM_DecOnOff ==Off ) && (PVM_NoeOnOff == Off))
  {
    if(PVM_NumberOfNuclei!=1)
    {
      STB_InitNuclei(1);
    }
  }
  
  STB_UpdateDecModule(PVM_Nucleus2,PVM_SpecAcquisitionTime);
  STB_UpdateNoeModule(PVM_Nucleus2);
  
  if( (PVM_DecOnOff ==On ) && (PVM_NoeOnOff == On)
      && PVM_DecMode == Composite_Pulse && PVM_NoeMode == Composite_Pulse_Noe)
  {
    /* both modules use F2 channel and share therefor the CPD pulse 
       element duration PCPD[1] so the element durations have to be matched */
    
    if(PVM_NoePulseElementDuration != PVM_DecPulseElementDuration)
    {
      PVM_NoePulseElementDuration = PVM_DecPulseElementDuration;
      STB_UpdateNoeModule(PVM_Nucleus2);
    }
  }
  
  
  DB_MSG(("<--LocalDecNodHandling"));
}

void updateModules(void)
{
  DB_MSG(("-->updateModules"));

  /* 
   * outer volume supression handler 
   * if NVoxels > 1, PVM_VoxMinDistance is the limiting factor
   * for the saturation slice thickness.
   */

  STB_UpdateOvsModule( PVM_VoxMinDistance, 
                       PtrType1x3 PVM_VoxArrSize,
                       PtrType1x3 PVM_VoxArrPositionRPS,
                       PVM_NVoxels,
                       PVM_DeriveGains,
                       PVM_Nucleus1);

  /* 
   * handle water suppression
   */
  double excDelay= CFG_GradientRiseTime()+StGStabD+VoxPul1.Length/2.0; //ms
  STB_UpdateWsModule( PVM_OvsModuleDuration,
                      PVM_NVoxels,
                      excDelay);

  LocalDecNoeHandling();
  
  updateOptimization();

  DB_MSG(("<--updateModules"));
}

/* calculates min repetition time and total scantime */


void UpdateRepetitionTime(void)
{
  double  modulesDuration=0, minTR=0, TotalTime=0;
  DB_MSG(("-->UpdateRepetitionTime"));    
 
  /* calculate modules duration : ovs + ws */ 

  switch(PVM_WsMode)
  {
    case NO_SUPPRESSION:
      if(PVM_OvsOnOff == On)
        modulesDuration = 3.0 * PVM_OvsModuleDuration;
      else
        modulesDuration = 0.0;
      break;
    case VAPOR:
    case CHESS:
    default: 
      modulesDuration = PVM_WsModuleDuration;
      break;
  }



  minTR = (StDur+1.0) 
    + 0.03             //UpdateDynPars
    + 0.01             //fixed delays in ppg
    + modulesDuration
    + PVM_NoeModuleTime
    + PVM_TriggerModuleTime;

  if(PVM_NavOnOff == On)
    minTR+=(0.02 + PVM_NavigatorModuleTime);

  PVM_MinRepetitionTime = minTR;
  PVM_RepetitionTime = MAX_OF( PVM_RepetitionTime, minTR);

  /* updating Dummy Scans after RepetitionTime */
  STB_UpdateDummyScans(PVM_RepetitionTime);  

  /* Calculate Total Scan Time and Set for Scan Editor */
  TotalTime = PVM_RepetitionTime * PVM_NAverages * PVM_NRepetitions;
 
  /* display total scan time */
  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<--UpdateRepetitionTime"));
  return;
}

/* -------------------------------------------------------------------------------------
 * Constrain Tune_Shim parameter group to localized spectroscopy demands:
 * 1) Force adjustment of 1st order shims only (no frequency shift during adjustment,
 *    PVM_TuneShimSubset = Shim_1st_order)
 * 2) Force high order shims not to be reseted (e.g. keep 2nd order shims adjusted
 *    by other methods (PVM_TuneShimForceSubset is set to No)
 * 3) Adjust frequency after shimming (PVM_TuneShimAdjFreq set to Yes)
 * 4) Make parameter non editable.
 * Note: The consistancy of the parameter group is realized by calling the default
 *       relation of the group handler PVM_TuneShimHandler
 *
 * ------------------------------------------------------------------------------------- */


void ConstrainTuneShim(void)
{
  DB_MSG(("-->ConstrainTuneShim"));

  PVM_TuneShimSubset = Shim_1st_order;
  PVM_TuneShimForceSubset=No;
  PVM_TuneShimAdjFreq=Yes;

  ParxRelsParRelations("PVM_TuneShimHandler",Yes);


  ParxRelsMakeNonEditable("PVM_TuneShimSubset,"
                          "PVM_TuneShimForceSubset,"
                          "PVM_TuneShimAdjFreq");

  DB_MSG(("<--ConstrainTuneShim"));
}

void SetAdjustments(void )
{
  DB_MSG(("-->SetAdjustments"));

  PTB_ClearAdjustments();

  STB_ArrayPhaseAppendAdjustment(PVM_EncNReceivers, No, 0, &PVM_VoxelGeoCub[0]); // No -> do not handle other adjustments (eg RCVR) than ArrayPhase Adj.

  if(PVM_RefScanYN == Yes)
  {
    PTB_AppendAdjustment("ReferenceScan",
                         "Reference Scan",
                         "Acquisition of a not suppressed reference scan",
                         per_scan);
  }
  else
  {
    PTB_AppendAdjustment("ReferenceScan",
                         "Reference Scan",
                         "Acquisition of a not suppressed reference scan",
                         on_demand);
  }

  if(!strcmp(PVM_Nucleus1, "1H"))
    PTB_AppendAdjustment("AutoLocShim",
                         "Local Shim",
                         "Voxel specific shim adjustment",
                         on_demand);

  PTB_AppendAdjustment("AutoSF",
                       "Local Frequency",
                       "Voxel specific frequency adjustment",
                       on_demand);

  PTB_AppendOrderAdjustment(per_scan, per_scan, RG_ADJNAME);

  DB_MSG(("<--SetAdjustments"));
}

void updateOptimization(void)
{
  if (OPT_ManAdjustment != NoManAdjustment)
  {
    PVM_NavOnOff=Off;
    PVM_RefScanYN=No;
  }

  if (PVM_NavOnOff==On)
  {
    ParxRelsMakeEditable("PVM_DriftCompYesNo");
  }
  else
  {
    PVM_DriftCompYesNo=No;
    ParxRelsMakeNonEditable("PVM_DriftCompYesNo");
  }

  STB_UpdateNavModule();

  STB_UpdateTriggerModule();
}
