/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002-2004
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 * ***************************************************************/

static const char resid[] = "$Id$ (C) 2004 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE       0
#define DB_LINE_NR	0

#include "method.h"


void initMeth()
/*:=MPE=:=======================================================*/
{
  int dimRange[2]   = {2, 2};
  int lowMat[3]     = {16, 16, 16};
  int upMat[3]      = {1024, 1024, 512};
  int defaultMat[3] = {128, 128, 128}; 

  DB_MSG(( "--> initMeth" ));

  /* Initialisation of nucleus */  
  STB_InitNuclei(1);
  
  /* initialize rf pulse parameters */  
  STB_InitRFPulse("ExcPulse1",      // name of pulse structure
                  "ExcPulse1Enum",  // name of pulse list parameter
                  "ExcPulse1Ampl",  // name of pulse amplitude parameter
                  "ExcPulse1Shape", // name of pulse shape (for calc. pulses)
                  0,                // used for excitation
                  "Calculated",     // default shape
                  8000.0,           // default bandwidth
                  15.0,             // default pulse angle
                  2);               // default pulse sharpness

  // check valid range for this specific pulse 
  ExcPulse1Range();
  
  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
    PVM_DeriveGains = Yes;
  
  EffSWhRange(); 

  /* encoding group */
  STB_InitEncoding(2, dimRange, lowMat, upMat, defaultMat);  

  /* initialize local parameters */
  if(!ParxRelsParHasValue("RampCompYN"))
    RampCompYN=Yes;

  if(ParxRelsParHasValue("AcqMode") == No)
    AcqMode = FID;

  if(ParxRelsParHasValue("SliceAlter") == 0)
    SliceAlter = No;

  if(!ParxRelsParHasValue("B0Corr"))
    B0Corr=Yes;

  GradSyncRange();
  ProUnderRange();
  EchoPositionRange();
  RampModeInitRel();

  GradRes = MAX_OF(CFG_GradientShapeResolution(),0.002);
    
  /* init geometry parameters */
  STB_InitImageGeometry();
  
  /* geometry editor */
  PTB_SetGeoObjShape("PVM_SliceGeoObj", Cylinder_In_GobjShape);

  /* initialize standard imaging parameters NA TR TE */
  if(ParxRelsParHasValue("PVM_NMovieFrames") == No)
    PVM_NMovieFrames = 1;
 
  if(ParxRelsParHasValue("PVM_NRepetitions") == No)
    PVM_NRepetitions = 1;

  if(ParxRelsParHasValue("YesNoMinEchoTime") == 0)
    YesNoMinEchoTime = Yes;

  if (ParxRelsParHasValue("PVM_MotionSupOnOff") == 0)
     PVM_MotionSupOnOff = Off;

  RepetitionTimeRange();
  AveragesRange();

  /* Initialisation of spoilers */
  if(!ParxRelsParHasValue("ReadSpoiling"))
    ReadSpoiling = 1.0;
  MRT_InitSpoiler("SliceSpoiler");


  /* Initialization of trajectory module */
  STB_InitTraj(1, 1, 1, 1, No, 2, Yes, 0, NULL, No, 0,
    "GradShape2", NULL, NULL,
    "GradAmpR","GradAmpP");

  /* Initialisation of modules */
  STB_InitTriggerModule();
  STB_InitFovSatModule();
  STB_InitFatSupModule();
  STB_InitSatTransModule();
  STB_InitDummyScans(1000.0);

  /* initialize mapshim parameter class */
  STB_InitMapShim();

  ParxRelsMakeNonEditable("PVM_MinEchoTime,NPro,PVM_AntiAlias");

  /* 
   * Once all parameters have initial values, the backbone is called
   * to assure they are consistent 
   */
  
  backbone();
 

  DB_MSG(( "<-- initMeth" ));

}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/


