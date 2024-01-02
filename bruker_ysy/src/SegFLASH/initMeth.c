/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002 - 2010
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$(C) 2002 Bruker BioSpin MRI GmbH";

#define DEBUG     0
#define DB_MODULE  0
#define DB_LINE_NR	0

#include "method.h"

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
  int dimRange[2]   = {    2,    3      };
  int lowMat[3]     = {   32,   16,   8 };
  int upMat[3]      = { 2048, 2048, 512 };
  int defaultMat[3] = {  128,  128,  64 };
  
  
  DB_MSG(( "-->SegFLASH:initMeth\n" ));


  /*  initialize local and redirected parameters */
  NAveragesRange();

  /* Method specific initialisation */
  if(ParxRelsParHasValue("PVM_EchoTime") == No)
    PVM_EchoTime = 4.0;

  if (ParxRelsParHasValue("EchoRepTime") == No)
    EchoRepTime = 10.0;

  if(ParxRelsParHasValue("SegmentRepTime") == No)
    SegmentRepTime = 100.0;

  if(ParxRelsParHasValue("PVM_RepetitionTime") == No)
    PVM_RepetitionTime = 100.0;

  if(ParxRelsParHasValue("SegmentSize") == No)
     SegmentSize = 1;

  if(ParxRelsParHasValue("PVM_NMovieFrames") == No)
    PVM_NMovieFrames = 1;

  if(ParxRelsParHasValue("FrameRepTime") == No)
  FrameRepTime=10.0;

  if(!ParxRelsParHasValue("SegmentSliceOrder"))
    SegmentSliceOrder = Alternate;

  ParxRelsShowInFile("SegmentSliceOrder");

  if(!ParxRelsParHasValue("InversionOnOff"))
  {
    InversionOnOff=Off;
    ParxRelsHideInEditor("InversionMode");
  } 
  ParxRelsShowInFile  ("InversionMode");

  InversionTimeRange();
  
  ParxRelsMakeNonEditable("SegmentDuration");

  if(ParxRelsParHasValue("PVM_NRepetitions") == No)
    PVM_NRepetitions = 1;

  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
    PVM_DeriveGains = Yes;

  /* encoding group */
  STB_InitEncoding( 2, dimRange, lowMat, upMat, defaultMat);
  
  /* Initialisation of rf pulse parameters */
  /*
   * 1: pulses declared in parDefinitions.h 
   * in this case - ExcPulse1. We initalise it to default name, 
   * 5000.0 Hz, and 30 deg flip angle
   * In addition we declare ExcPulse1Enum ExcPulse1A^mpl and ExcPulse1Shape
   * to be handled together with pulse struct ExcPulse1. Only if a double
   * array parameter is provided as shape, the pulse supports calculated 
   * shapes.
   */
  
  STB_InitRFPulse("ExcPulse1",      // name of pulse structure
                  "ExcPulse1Enum",  // name of pulse list parameter
                  "ExcPulse1Ampl",  // name of pulse amplitude parameter
                  "ExcPulse1Shape", // name of pulse shape (for calc. pulses)
                  0,                // used for excitation
                  "Calculated",     // default shape
                  3000.0,           // default bandwidth
                  30.0);            // default pulse angle

  // check valid range for this specific pulse see parsRelations.c
  ExcPulse1Range();
  
  /* Initialisation of nucleus */  
  STB_InitNuclei(1);

  /* Initialisation of geometry parameters */
  STB_InitImageGeometry();
  
   /* Initialisation of spoilers */
  MRT_InitSpoiler("ReadSpoiler");
  MRT_InitSpoiler("SliceSpoiler");
  MRT_InitSpoiler("SegSpoiler");

  if (!ParxRelsParHasValue("FlowCompOnOff"))
    FlowCompOnOff = Off;

  if (!ParxRelsParHasValue("FlowCompDirection"))
  {
    FlowCompDirection[0]=Yes;
    FlowCompDirection[1]=Yes;
    FlowCompDirection[2]=Yes;
  }

  if (ParxRelsParHasValue("PVM_MotionSupOnOff") == 0)
     PVM_MotionSupOnOff = Off;

  EffSWhRange();
  
  /* Initialisation of modules */
  STB_InitFatSupModule();
  STB_InitSatTransModule();
  STB_InitFovSatModule();
  STB_InitFlowSaturationModule();
  STB_InitTaggingModule();
  STB_InitEvolutionModule();
  STB_InitSelIrModule();
  STB_InitDriftComp(On);

  InitTriggerModule();

  if(!ParxRelsParHasValue("RFSpoiling"))
    RFSpoiling=No;

  if(!ParxRelsParHasValue("PVM_DummyScans"))
  {
    PVM_DummyScans=6;
    STB_InitDummyScans( PVM_DummyScans * SegmentRepTime );
  }

  /* initialize mapshim parameter class */
  STB_InitMapShim();

  /* initialization of method specific reconstruction */
  if(ParxRelsParHasValue("RecoMethMode") == No)
    RecoMethMode=Default;
  if(ParxRelsParHasValue("WeightingMode") == No)
    WeightingMode=positive_mask;

  GaussBroadRange();
  MaskWeightRange();

  /* Visibility of parameters */
  ParxRelsMakeNonEditable("PVM_EchoPosition");
  ParxRelsMakeNonEditable("PVM_MinEchoTime,PVM_AcquisitionTime");
  ParxRelsMakeNonEditable("EncGradDur");

  /* initialize method ats parameter group */
  STB_AtsInit();

  /* 
   * Once all parameters have initial values, the backbone is called
   * to assure they are consistent 
   */

  backbone();

  DB_MSG(("<--FLASH:initMeth\n"));
  
}

void InitTriggerModule(void)
{
  DB_MSG(("-->InitTriggerModule"));
  TriggerDur = 0.01;

  if (!ParxRelsParHasValue("TriggerOnOff"))
  {
    TriggerOnOff = Off;

    AcqTrigger=No;
    AcqTriggerModuleTime=0.0;

    InvTrigger=No;
    InvTriggerModuleTime=0.0;

    ParxRelsHideGroupInEditor("Acq_Trigger_Parameters");
    ParxRelsHideGroupInEditor("Inv_Trigger_Parameters");
  }

  if (!ParxRelsParHasValue("AcqTriggerDelay"))
  {
    AcqTriggerDelay = 1.0;
  }

  if (!ParxRelsParHasValue("InvTriggerDelay"))
  {
    InvTriggerDelay = 1.0;
  }

  DB_MSG(("-->InitTriggerModule"));
}


/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
