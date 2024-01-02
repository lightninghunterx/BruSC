
/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2006-2011
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


#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	1


#include "method.h"



/* 
 * method backbone 
 */


void backbone(void)
{
  double PulseBandwidthArray[3];
  DB_MSG(("entering backbone()"));

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

  DB_MSG(("Calling Voxel handler"));

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

  /* 
   * handle Press timing & gradients
   */
  pressHandler(80.0);


  /* 
   * handle setup & auto routines
   */    
  OptHandler();

  /* 
   * calculate repetition time
   */

  RepetitionTimeRels();

  /* DriftCompensation (after update of TR)*/
  STB_UpdateDriftComp(PVM_RepetitionTime);

  SetBaseLevelParameter();
  SetRecoParam();
}


/* 
 * subroutines called by the method handler 
 */


/* calculates timing amplitudes and min TE of PRESS volume selection */

void pressHandler ( double maxSpoilerStrength )
{
    int i, j;
    double  minGradDelay, ad, minTE1, minTE2;
    double riseT = CFG_GradientRiseTime();
    
    /*
     * simple parameter relations 
     */
    minGradDelay = 2.0*riseT;
    PVM_NAverages = MAX_OF(1, PVM_NAverages);
    SpoilerStrength = MIN_OF( maxSpoilerStrength, SpoilerStrength);
    GradStabDelay = MAX_OF(0.0, GradStabDelay );

    for ( i = 0; i < 2; i++ )
    {
	SpoilerDuration[i][0] = MAX_OF(minGradDelay, SpoilerDuration[i][0]);
	for ( j = 0; j < 3; j++ )
	{
	    SpoilerStrengthArr[i][j] = MAX_OF(-maxSpoilerStrength, SpoilerStrengthArr[i][j]);
	    SpoilerStrengthArr[i][j] = MIN_OF( maxSpoilerStrength, SpoilerStrengthArr[i][j]);
	}
    }   

    /* calculate SpoilerStrength */
    if (CalcSpoiler == On )
    {
      double fac=1.0/sqrt(2.0);
      SpoilerStrengthArr[0][0] =  fac*SpoilerStrength;
      SpoilerStrengthArr[0][1] = -fac*SpoilerStrength;
      SpoilerStrengthArr[0][2] =  fac*SpoilerStrength;
      SpoilerStrengthArr[1][0] =  1.0;
      SpoilerStrengthArr[1][1] =  SpoilerStrength;
      SpoilerStrengthArr[1][2] =  SpoilerStrength;
    }


    /* 
     * handle timing 
     */

    /* check the rephasing */
    ad = PVM_VoxArrGradient[0][0]*((VoxPul1.Length+riseT)/2.0+GradStabDelay);
    SpoilerDuration[0][0] = MAX_OF( SpoilerDuration[0][0], 
                                    ad/100.0+riseT );
    SliceRephase = -ad/(SpoilerDuration[0][0]-riseT);
    if ( (SpoilerStrengthArr[0][0]+SliceRephase) > maxSpoilerStrength )
      SpoilerStrengthArr[0][0] = maxSpoilerStrength - SliceRephase;
    if ( (SpoilerStrengthArr[0][0]+SliceRephase) < -maxSpoilerStrength )
      SpoilerStrengthArr[0][0] = -maxSpoilerStrength - SliceRephase;
    

    /* calculate EchoTimes */
    minTE1 = 
    ( VoxPul1.Length/2.0                  //p1/2 
	  + GradStabDelay                       //d6
	  + riseT                               //d7
	  + SpoilerDuration[0][0]               //d8 + d4
	  + GradStabDelay                       //d6
    + VoxPul2.Length/2.0 ) * 2.0;         //p2/2

    minTE2 = 
    ( VoxPul3.Length/2.0                  //p3/2
      + GradStabDelay                     //d6
      + SpoilerDuration[1][0]             //d10+d11     
	  + 0.020) * 2.0;
    
    
    
    switch ( EchoTimeMode )
    {
    case MinTE:
	TE1 = minTE1;
	TE2 = minTE2;
	PVM_EchoTime = minTE1+minTE2;
	break;

    case Equalise:
	PVM_EchoTime = MAX_OF(PVM_EchoTime, 2.0*MAX_OF(minTE1, minTE2));
	TE1 = PVM_EchoTime/2.0;
	TE2 = PVM_EchoTime/2.0;
	break;

    case FixTE1:
	TE1 = MAX_OF(minTE1, TE1);
	PVM_EchoTime = MAX_OF(PVM_EchoTime, TE1+minTE2);
	TE2 = PVM_EchoTime-TE1;
	break;

    case FixTE2:
	TE2 = MAX_OF(TE2, minTE2);
	PVM_EchoTime = MAX_OF( PVM_EchoTime, minTE1+TE2);
	TE1 = PVM_EchoTime - TE2;
	break;
    }

    AddDelay[0] = 
	TE1/2.0 - minTE1/2.0;

    AddDelay[1] = 
	TE1/2.0
	- VoxPul2.Length/2.0
  - GradStabDelay             //d6
  - SpoilerDuration[0][0];    //d8+d9

    AddDelay[2] = 
      TE2/2.0 - minTE2/2.0;  //assuming symmetry between TE2_1 and TE2_2

    AddDelay[3] = AddDelay[2];
    

    /* 
     * handle layout
     */
    if ( CalcSpoiler == On )
    {
	ParxRelsHideInEditor("SpoilerStrengthArr");
	ParxRelsShowInEditor("SpoilerStrength");
    }
    else
    {
	ParxRelsShowInEditor("SpoilerStrengthArr");
	ParxRelsHideInEditor("SpoilerStrength");
    }
    switch (EchoTimeMode)
    {
    case Equalise:
	ParxRelsMakeNonEditable("TE1");
	ParxRelsMakeNonEditable("TE2");
	ParxRelsMakeEditable("PVM_EchoTime");
	break;
    case FixTE1:
	ParxRelsMakeEditable("TE1");
	ParxRelsMakeNonEditable("TE2");	
	ParxRelsMakeEditable("PVM_EchoTime");
	break;
    case FixTE2:
	ParxRelsMakeEditable("TE2");
	ParxRelsMakeNonEditable("TE1");
	ParxRelsMakeEditable("PVM_EchoTime");
	break;	
    default:
	ParxRelsMakeNonEditable("TE1");
	ParxRelsMakeNonEditable("TE2");
	ParxRelsMakeNonEditable("PVM_EchoTime");
    }
    
}



void UpdateRFPulses( void )
{
  DB_MSG(("-->UpdateRFPulses"));

  STB_UpdateRFPulse("VoxPul1", 1, PVM_DeriveGains, Conventional);
  STB_UpdateRFPulse("VoxPul2", 1, PVM_DeriveGains, Conventional);
  STB_UpdateRFPulse("VoxPul3", 1, PVM_DeriveGains, Conventional);

  if(PVM_DeriveGains==Yes)
  {
    ParxRelsHideInEditor("VoxPul1Ampl");
    ParxRelsHideInEditor("VoxPul2Ampl");
    ParxRelsHideInEditor("VoxPul3Ampl");
    ParxRelsHideInEditor("LockVoxPulAmpls");
  }
  else
  {
    ParxRelsShowInEditor("VoxPul1Ampl");
    ParxRelsShowInEditor("VoxPul2Ampl");
    ParxRelsShowInEditor("VoxPul3Ampl");
    ParxRelsShowInEditor("LockVoxPulAmpls");
  }

  ParxRelsShowInFile("VoxPul1Ampl");
  ParxRelsShowInFile("VoxPul2Ampl");
  ParxRelsShowInFile("VoxPul3Ampl");


  DB_MSG(("<--UpdatePulses"));
}

void LocalDecNoeHandling(void)
{
 if((PVM_DecOnOff == On ) || (PVM_NoeOnOff == On))
 {
   if(PVM_NumberOfNuclei!=2)
   {
     STB_InitNuclei(2);
     if(PVM_NumberOfNuclei < 2)
     {
       /* system configuration does not support 2nd RF channel */
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


 if( (PVM_DecOnOff ==On ) && (PVM_NoeOnOff == On) &&
     PVM_DecMode == Composite_Pulse && PVM_NoeMode == Composite_Pulse_Noe)
 {
   /* both modules use F2 channel and share therefor the CPD pulse 
      element duration PCPD[1] so the element durations have to be matched */
   
   if(PVM_NoePulseElementDuration != PVM_DecPulseElementDuration)
   {
     PVM_NoePulseElementDuration = PVM_DecPulseElementDuration;
     STB_UpdateNoeModule(PVM_Nucleus2);
   }
 }
 
}

void updateModules(void)
{
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
  double excDelay= CFG_GradientRiseTime()+GradStabDelay+VoxPul1.Length/2.0; //ms
  STB_UpdateWsModule( PVM_OvsModuleDuration,
                      PVM_NVoxels,
                      excDelay);

  LocalDecNoeHandling();

  updateOptimization();
}

/* calculates min repetition time and total scantime */


void RepetitionTimeRels(void)
{
  double  modulesDuration=0, minTR=0, TotalTime=0;

  DB_MSG(("-->RepetitionTimeRels"));

  /* calculate repetition time */
  
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

  double aqq=(OPT_ManAdjustment == NoManAdjustment)?PVM_SpecAcquisitionTime:PVM_AcquisitionTime;

  minTR =
      0.03                      //UpdateDynPars
    + 1.0
    + PVM_TriggerModuleTime
    + PVM_NoeModuleTime
    + modulesDuration
    + CFG_GradientRiseTime()    //d4
    + GradStabDelay             //d6
    + VoxPul1.Length/2.0        //p1/2
    + PVM_EchoTime
    + aqq
    + CFG_GradientRiseTime()   //d4
    + 1.0;

  if(PVM_NavOnOff == On)
    minTR+=(0.02 + PVM_NavigatorModuleTime);

  PVM_RepetitionTime = MAX_OF( PVM_RepetitionTime, minTR);
  TrDel = PVM_RepetitionTime - minTR;

  /* updating Dummy Scans afte RepetitionTime and before TotalScanTime */
  STB_UpdateDummyScans(PVM_RepetitionTime);  

  /* Calculate Total Scan Time and Set for Scan Editor */
  
  DB_MSG(("-->TotalScanTimeRels\n"));
  TotalTime = 0.0; 

  TotalTime = PVM_RepetitionTime * PVM_NAverages * PVM_NRepetitions;
 
  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  
  DB_MSG(("<--RepetitionTimeRels"));
  return;
}


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

    STB_ArrayPhaseAppendAdjustment(PVM_EncNReceivers, No, 0, &PVM_VoxelGeoCub[0]);
  
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
