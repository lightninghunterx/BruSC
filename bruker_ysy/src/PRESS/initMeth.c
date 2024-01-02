/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999-2004
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 *
 ****************************************************************/
static const char resid[] = "$Id$ (C) 2004 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	1



/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "method.h"



void initMeth()
/*:=MPE=:=======================================================*/
{
  DB_MSG(("entering initMeth()"));


  /*
   * initialize NOE and Decoupling parameter group
   */
  STB_NoeOnOffRange();
  STB_DecOnOffRange();
  STB_InitDecModule();
  STB_InitNoeModule();
  LocalDecNoeHandling();

  /* Init Navigator Module */
  STB_InitNavModule(Yes, Off, 128, 20000);
  RetroFrequencyLockRange();
  STB_InitDriftComp();

  CalcLineWidthRange();
    
  /*
   * init the spectroscopic dataAcquisition
   */    
  STB_InitSpectroscopy( 1, 1, 1, PVM_Nucleus1, 200.0, 125000.0 );


  /*
   * initialize multi receiver parameters
   */
  STB_InitEncoding();

  STB_InitRefScan(0);  //RefScan via job0
  if(ParxRelsParHasValue("Edc_OnOff") == No)
    Edc_OnOff = Off;
  if(ParxRelsParHasValue("EdcManualFilter") == No)
    EdcManualFilter = No;  
  if(ParxRelsParHasValue("EdcFilterWidthHz") == No)
    EdcFilterWidthHz = 120;  

  /*
   * Dummy Scan Module
   */
  STB_InitDummyScans(0.0);


  /* 
   * rf pulses for localised excitation 
   */
  if(!ParxRelsParHasValue("VoxPul1"))
  {
    STB_InitRFPulse("VoxPul1",      // name of pulse structure
                    "VoxPul1Enum",  // name of pulse list parameter
                    "VoxPul1Ampl",  // name of pulse amplitude parameter
                    "VoxPul1Shape", // name of pulse shape (for calc. pulses)
                    0,                  // used for excitation
                    "Calculated",      // default shape
                    5600.0,             // default bandwidth
                    90.0);              // default pulse angle
  }

  if(!ParxRelsParHasValue("VoxPul2"))
  {
    STB_InitRFPulse("VoxPul2",      // name of pulse structure
                    "VoxPul2Enum",  // name of pulse list parameter
                    "VoxPul2Ampl",  // name of pulse amplitude parameter
                    "VoxPul2Shape", // name of pulse shape (for calc. pulses)
                    1,                  // used for refocusing
                    "Calculated",      // default shape
                    3400.0,             // default bandwidth
                    180.0);             // default pulse angle
  }

  if(!ParxRelsParHasValue("VoxPul3"))
  {
    STB_InitRFPulse("VoxPul3",      // name of pulse structure
                    "VoxPul3Enum",  // name of pulse list parameter
                    "VoxPul3Ampl",  // name of pulse amplitude parameter
                    "VoxPul3Shape", // name of pulse shape (for calc. pulses)
                    1,                  // used for excitation
                    "Calculated",      // default shape
                    3420.0,             // default bandwidth
                    180.0);             // default pulse angle
  }


  DeriveGainsRange();

  // check valid range for pulses, see parsRelations.c
  VoxPul1Range();
  VoxPul2Range();
  VoxPul3Range();

  if(!ParxRelsParHasValue("LockVoxPulAmpls"))
    LockVoxPulAmpls = No;


  STB_InitAtsReferencePosition();
  /* 
   *  setup for a spectroscopy method
   */
  STB_InitVoxelGeometry(1);
    
   
  /* 
   * init the Outer Volume suppression 
   * with 100% maximum gradient strength 
   */
  STB_InitOvsModule( 100.0, PVM_VoxMinDistance );
    

  /* 
   * init the ws modules
   */
  STB_InitWsModule (PVM_NVoxels);
    

  /* 
   * init the optimisation  modules
   */
  InitOptimization();

  PVM_TriggerMode = per_Slice;
  STB_InitTriggerModule();
  ParxRelsMakeNonEditable("PVM_TriggerMode");

  /* 
   * init the local parameter
   */
  pressInit();

  AveragesRange();

  if(ParxRelsParHasValue("PVM_NRepetitions") == No)
    PVM_NRepetitions = 1;

  STB_InitMapShim();

  backbone();
}


/****************************************************************/
/*		L O C A L   F U N C T I O N S			*/
/****************************************************************/

void pressInit( void )
{
    
  if ( ParxRelsParHasValue("EchoTimeMode") == No )
  {
    int i, j;
    PVM_RepetitionTime = 2000.0;
    PVM_EchoTime = 20.0;
    GradStabDelay = 0.0;
    EchoTimeMode = FixTE1;
    SpoilerStrength = MIN_OF(80.0, floor( 100.0*10000.0/3.0/PVM_GradCalConst+0.5));
	
    CalcSpoiler = On;
    for ( i = 0; i < 2; i++ )
    {
	    SpoilerDuration[i][0] = 3.0;
	    for ( j = 0; j < 3; j++ )
        SpoilerStrengthArr[i][j] = 0.0;
    }
  }
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");
}


/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/

