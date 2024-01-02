/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2006
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

static const char resid[] = "$Id$ (C) 2006 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	0



/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "method.h"


void initMeth()
{
  DB_MSG(("-->initMeth"));    
    
  /*
   *  initialization of PVM_Nuc1
   */
  STB_InitNuclei(1);
 
  /*
   * init modules
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
 
  InitRFPulses();

  STB_InitAtsReferencePosition();
  /* 
   *  setup for a spectroscopy method
   */
  STB_InitVoxelGeometry( 1);
  
  /* 
   * init the Outer Volume suppression 
   * with 100% maximum gradient strength 
   */
  STB_InitOvsModule( 100.0, PVM_VoxMinDistance );
    

  /* 
   * init the ws modules without application of RF pulses
   */
  STB_InitWsModule (PVM_NVoxels);
    

  /* 
   * init the optimisation  modules
   */
  InitOptimization();

 
  /*
   * init the trigger module
   */
  PVM_TriggerMode = per_Slice;
  STB_InitTriggerModule();
  ParxRelsMakeNonEditable("PVM_TriggerMode");

  /* 
   * init the localization parameters
   */
  InitSteamLocalization();
  

  /*
   * initialize standard measurement parameters
   */
  
  if(ParxRelsParHasValue("PVM_RepetitionTime") == No)
    PVM_RepetitionTime = 2000.0;
  if(ParxRelsParHasValue("PVM_EchoTime") == No)
    PVM_EchoTime = 15.0;
  ParxRelsMakeNonEditable("PVM_MinEchoTime");

  if(ParxRelsParHasValue("PVM_NRepetitions") == No)
    PVM_NRepetitions = 1;

  AveragesRange();

  STB_InitMapShim();

  backbone();  
  
  DB_MSG(("<--initMeth"));    
  
}


/****************************************************************/
/*		L O C A L   F U N C T I O N S			*/
/****************************************************************/



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/

