/* ***************************************************************
 *
 * Copyright (c) 2013 - 2018
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 * ***************************************************************/

static const char resid[] = "$Id$ (C) 2004 - 2018 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE       0
#define DB_LINE_NR	0

#include "method.h"
#include "common/ovl_toolbox/SysInfo_Common.h"

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
  
  if(!ParxRelsParHasValue("B0Corr"))
    B0Corr=Yes;

  GradSyncRange();
  ProUnderRange();
  EchoPositionRange();
  RampModeInitRel();

  GradRes = MAX_OF(CFG_GradientShapeResolution(),0.002);
    
  /* init geometry parameters */
  STB_InitAtsReferencePosition();
  STB_InitImageGeometry();
  
  /* geometry editor */
  PTB_SetGeoObjShape("PVM_SliceGeoObj", Cylinder_In_GobjShape);

  /* initialize local and redirected parameters */
  if(ParxRelsParHasValue("PVM_NMovieFrames") == No)
  {
    PVM_NMovieFrames = 8; 
    OutputMovieFrames = 8;
  }

  if(!ParxRelsParHasValue("Oversampling"))
    Oversampling=80;

  if(!ParxRelsParHasValue("SelfGating"))
    SelfGating=Yes;

  if(!ParxRelsParHasValue("PVM_NAverages"))
    PVM_NAverages=1;
 
  if(!ParxRelsParHasValue("AddCentralPoints"))
    AddCentralPoints=0;

  if(ParxRelsParHasValue("PVM_NRepetitions") == No)
    PVM_NRepetitions = 1;

  if(ParxRelsParHasValue("YesNoMinEchoTime") == 0)
    YesNoMinEchoTime = Yes;

  RepetitionTimeRange();

  /* Initialisation of spoilers */
  if(!ParxRelsParHasValue("ReadSpoiling"))
    ReadSpoiling = 1.0;
  MRT_InitSpoiler("SliceSpoiler");


  /* Initialization of trajectory module */
  STB_InitTraj(1, 1, 1, 1, No, 2, Yes, 0, NULL, No, 0,
    "GradShape2", NULL, NULL,
    "GradAmpR","GradAmpP");

  /* Initialisation of modules */
  STB_InitFovSatModule();
  STB_InitFatSupModule();
  STB_InitSatTransModule();
  STB_InitDummyScans(1000.0);
  STB_InitTriggerModule(); //only used for trajectory module

  /* Initialisation of navigator module */
  STB_InitNavModule();
  /* Initialisation of drift compensation module */
  STB_InitDriftComp();

  /* initialize mapshim parameter class */
  STB_InitMapShim();

  /* initialization of special reconstruction parameters */
  if (!ParxRelsParHasValue("MovieType"))
    MovieType=Cardiac;
  if(!ParxRelsParHasValue("AnalysisOfMotion"))
    AnalysisOfMotion=CardiacResp; 
  if(!ParxRelsParHasValue("NavigatorAnalysis"))
    NavigatorAnalysis=Magnitude;  
  if (!ParxRelsParHasValue("TimeGridding"))
    TimeGridding=No;
  if(!ParxRelsParHasValue("HeartRate"))
    HeartRate=400;
  if(!ParxRelsParHasValue("RespRate"))
    RespRate=65;
  if(!ParxRelsParHasValue("FreezePhase0"))
    FreezePhase0=0.0;
  if(!ParxRelsParHasValue("FreezePhaseRange0"))
    FreezePhaseRange0=1.0;
  if(!ParxRelsParHasValue("FreezePhase"))
    FreezePhase=0.15;
  if(!ParxRelsParHasValue("FreezePhaseRange"))
    FreezePhaseRange=0.7;
  if(ParxRelsParHasValue("GaussWidth") == No)
    GaussWidth=1.0;
  if(!ParxRelsParHasValue("IgRecoDebug"))
    IgRecoDebug=No;
  if(!ParxRelsParHasValue("CompressedSensing"))
    CompressedSensing=No;  

  if(SysInfo_StudyModalityActive(PT_Modality))
    PET_MR_Study = Yes;
  else
    PET_MR_Study = No;
  
  if(!ParxRelsParHasValue("StoreOutputFiles"))
  {
    if(PET_MR_Study == Yes)
      StoreOutputFiles=Yes;
    else
      StoreOutputFiles=No;
  }
  
  if(!ParxRelsParHasValue("IgTTLOut"))
  {
    if(PET_MR_Study == Yes)
      IgTTLOut = Yes;
    else
      IgTTLOut = No;
  }

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


