/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$(C) 2002 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0


#include "method.h"


void initMeth()
{
  DB_MSG(( "-->initMeth\n" ));

  
  /*  Initialize NA ( see code in parsRelations ) */
  AverageModeRange();
 
  Local_NAveragesRange();
    
  
  /* Initialisation of rf pulse parameters */
  STB_InitRFPulse("ExcPulse1",      // name of pulse structure
                  "ExcPulse1Enum",  // name of pulse list parameter
                  "ExcPulse1Ampl",  // name of pulse amplitude parameter
                  "ExcPulse1Shape", // name of pulse shape (for calc. pulses)
                  0,                // used for excitation
                  "Calculated",     // default shape
                  5000.0,           // default bandwidth
                  30.0);            // default pulse angle

  // check valid range for this specific pulse see parsRelations.c
  ExcPulse1Range();
  
  /* Initialisation of nucleus */  
  STB_InitNuclei(1);
  
  if(ParxRelsParHasValue("PVM_NRepetitions") == No)
    PVM_NRepetitions = 1;
 
  if(ParxRelsParHasValue("PVM_RepetitionTime") == No)
    PVM_RepetitionTime = 1000.0;
  if(ParxRelsParHasValue("PVM_EchoTime") == No)
    PVM_EchoTime = 5.0;

  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
    PVM_DeriveGains = Yes;
  
  /* Initialisation of spoilers */
  MRT_InitSpoiler("Spoiler");

  
  /* duration of phase encoding */
  if(ParxRelsParHasValue("EncGradDur") == No)
    EncGradDur = 0.5;
 
  /* Initialisation of geometry parameters */
  {
    int dimRange[2]   = { 1, 3 };
    int lowMat[3]     = { 8, 8, 8 };
    int upMat[3]      = { 64, 64, 32 };
    int defaultMat[3] = { 32, 32, 16 };
  
    STB_InitEncoding( 2, dimRange, lowMat, upMat, defaultMat);  
  }
  
  STB_InitImageGeometry();

  ParxRelsMakeNonEditable("EffResolution");  

  /* initialisation of spectroscopy */

  STB_InitSpectroscopy( 1, 1, 1 ,  PVM_Nucleus1 , 200.0, 200000 );
  
  /* Initialisation of modules */

  STB_InitFovSatModule();
  
  InitLocalization();

  STB_NoeOnOffRange();
  STB_DecOnOffRange();
  
  STB_InitFatSupModule();

  STB_InitDecModule();
  STB_InitNoeModule();

  STB_InitNavModule(Yes, Off, 128, 20000);
  RetroFrequencyLockRange();

  STB_InitDriftComp();

  STB_InitWsModule (PVM_NVoxels);

  /* 
   * init the Outer Volume suppression 
   * with 100% maximum gradient strength 
   */
  STB_InitOvsModule( 100.0, PVM_VoxMinDistance );
    

 /*
   * init the trigger module
   */
  STB_InitTriggerModule();

 
  /* Dummy Scan Module  */
  STB_InitDummyScans(0.0);
 
  /* initialize mapshim parameter class */
  STB_InitMapShim(Yes); //DynamicShim

  STB_InitAtsReferencePosition();

  backbone();
  
  DB_MSG(("<--initMeth\n"));
  
}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/









