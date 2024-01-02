/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2011
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
#include "common/ovl_toolbox/SysInfo_Common.h"

/*:=MPB=:=======================================================*
 *
 * Global Function: initMeth
 *
 * Description: This procedure is implicitly called when this
 *	method is selected.
 *
 * Error History:
 *
 * Interface:							*/

void initMeth()
/*:=MPE=:=======================================================*/
{
  int dimRange[2] = { 2,3 };
  int lowMat[3]   = { 32, 32, 8 };
  int upMat[3]    = { 2048, 2048, 512 };
  int defaultMat[3] = {128, 128, 64};
  
  
  DB_MSG(( "-->IgFLASH:initMeth\n" ));
  
  
  /* encoding group */
  STB_InitEncoding( 2, dimRange, lowMat, upMat, defaultMat);  

  /* Initialisation of rf pulse parameters */
  STB_InitRFPulse("ExcPulse1",      // name of pulse structure
                  "ExcPulse1Enum",  // name of pulse list parameter
                  "ExcPulse1Ampl",  // name of pulse amplitude parameter
                  "ExcPulse1Shape", // name of pulse shape (for calc. pulses)
                  0,                // used for excitation
                  "Calculated",     // default shape
                  3000.0,           // default bandwidth
                  30.0,             // default pulse angle
                  2);               // default pulse sharpness

  // check valid range for this specific pulse see parsRelations.c
  ExcPulse1Range();  
  
  /* Initialisation of nucleus */  
  STB_InitNuclei(1);

  /* Initialisation of geometry parameters */
  STB_InitImageGeometry();
  
  /*  initialize local and redirected parameters */
  if(ParxRelsParHasValue("PVM_RepetitionTime") == No)
    PVM_RepetitionTime = 10.0;
  if(ParxRelsParHasValue("PVM_EchoTime") == No)
    PVM_EchoTime = 3.0;
  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
    PVM_DeriveGains = Yes;
  if(ParxRelsParHasValue("PVM_NRepetitions") == No)
    PVM_NRepetitions = 1;
  if(ParxRelsParHasValue("PVM_NMovieFrames") == No)
  {
    PVM_NMovieFrames = 8; 
    OutputMovieFrames = 8;
  }
  if(!ParxRelsParHasValue("Oversampling"))
    Oversampling=80;
  if(!ParxRelsParHasValue("PVM_NAverages"))
    PVM_NAverages=1;
  if(ParxRelsParHasValue("Nsegments") == No)
    Nsegments = 1;
  if(ParxRelsParHasValue("Seg_mode") == No) 
    Seg_mode = sequential;
  if(!ParxRelsParHasValue("NavSigSetupYesNo"))
    NavSigSetupYesNo=No;
  if(!ParxRelsParHasValue("SelfGating"))
    SelfGating=Yes;
  if(!ParxRelsParHasValue("AngioMode"))
    AngioMode=No;
  if(!ParxRelsParHasValue("FlowCompYesNo"))
    FlowCompYesNo=No;
  if(!ParxRelsParHasValue("CompressedSensing"))
    CompressedSensing=No;

  /* Initialisation of spoilers */
  MRT_InitSpoiler("ReadSpoiler");
  MRT_InitSpoiler("SliceSpoiler");
  
  EffSWhRange();
 
  /* Initialisation of modules */
  STB_InitFatSupModule();
  STB_InitSatTransModule();
  STB_InitFovSatModule();
  STB_InitFlowSaturationModule();
  if(!ParxRelsParHasValue("RFSpoiling"))
    RFSpoiling=No;
  STB_InitDummyScans(1000.0);
  
  /* Initialisation of navigator module */
  STB_InitNavModule();
  /* Initialisation of drift compensation module */
  STB_InitDriftComp();
 
  /* initialize mapshim parameter class */
  STB_InitMapShim();

  /* initialization of special reconstruction parameters */
  if(!ParxRelsParHasValue("MovieType"))
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
  if(!ParxRelsParHasValue("RespGate"))
    RespGate=No;
  if(!ParxRelsParHasValue("IgRecoDebug"))
    IgRecoDebug=No;

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
  
  /* Visibility of parameters */
  ParxRelsMakeNonEditable("PVM_EchoPosition");
  ParxRelsMakeNonEditable("PVM_MinEchoTime,PVM_AcquisitionTime");
  ParxRelsMakeNonEditable("EncGradDur");
 
  STB_InitAtsReferencePosition();

  /* 
   * Once all parameters have initial values, the backbone is called
   * to assure they are consistent 
   */
  
  
  backbone();
  
  
  DB_MSG(("<--IgFLASH:initMeth\n"));
  
}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/









