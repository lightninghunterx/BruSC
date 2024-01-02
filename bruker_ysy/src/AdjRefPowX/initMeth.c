/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2020
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2001 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

/****************************************************************/
/****************************************************************/
/*		I N T E R F A C E   S E C T I O N		*/
/****************************************************************/
/****************************************************************/

/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "method.h"

/****************************************************************/
/*	I M P L E M E N T A T I O N   S E C T I O N		*/
/****************************************************************/


/****************************************************************/
/*		G L O B A L   F U N C T I O N S			*/
/****************************************************************/


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


  DB_MSG(( "-->initMeth()" ));


  /*
   * initialize NOE and Decoupling parameter group
   */

  STB_NoeOnOffRange();
  STB_DecOnOffRange();

  /*
   * init decoupling module
   */

  STB_InitDecModule();

  /*
   * init noe module
   */

  STB_InitNoeModule();

  /*
   * initialize the nuclei dependent on NOE or Decoupling
   */

  LocalDecNoeHandling();

  /*  Initialize method parameters*/
  if(ParxRelsParHasValue("PVM_NAverages") == No)
    PVM_NAverages = 1;

  if(ParxRelsParHasValue("PVM_NRepetitions") == No)
    PVM_NRepetitions = 1;

  if(ParxRelsParHasValue("NExcSteps") == No)
    NExcSteps = 1;

  if(ParxRelsParHasValue("NoiseScan") == No)
    NoiseScan = 0;

  if(ParxRelsParHasValue("AutoEchoTimer") == No)
  {
    AutoEchoTimer = On;
    ParxRelsMakeNonEditable("PVM_EchoTime");
  }

  /* -------------------------------------------------------------------
   Initializing rf pulse:
   Most of the arguments don't have a relevance anymore in this method
   becasue pulse durations and powers are lists in ppg as their values
   are changed with NR.
   --------------------------------------------------------------------*/

  STB_InitRFPulse("ExcPulse",       // name of pulse structure
                  "ExcPulseEnum",   // name of pulse list parameter
                  0,                // no pulse ampl parameter
                  0,                // no calc pulses
                  0,                // used for excitation
                  "bp.exc",         // default shape
                  10000.0,          // default bandwidth
                  20.0);            // default pulse angle

  /* check valid range for this specific pulse see parsRelations.c*/
  ExcPulseRangeCheck();

  InitSliceSelection();

  if(ParxRelsParHasValue("ScanMode") == No)
    ScanMode = CONST_PULSE_POW;
  ScanModeCheck();

  /* Initialise PVM_DeriveGains*/
  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
    PVM_DeriveGains = Yes;

  /* initialisation of spectroscopy */
  STB_InitSpectroscopy( 1, 1, 1 ,  PVM_Nucleus1 , 200.0, 1000000 );

  STB_InitEncoding();

  /* Initialize Timing */
  if(ParxRelsParHasValue("PVM_EchoTime") == No)
    PVM_EchoTime = 0.021;

  if(ParxRelsParHasValue("PVM_RepetitionTime") == No)
    PVM_RepetitionTime = 1000.0;

  if(!ParxRelsParHasValue("TETRFillDelay"))
  {
    PARX_change_dims("TETRFillDelay", 1);
    TETRFillDelay[0]=0.0;
  }

  /* Initialize modules */
  STB_InitWsModule (1);
  STB_InitFovSatModule();
  STB_InitFatSupModule();
  STB_InitDummyScans(PVM_RepetitionTime);

  //  STB_InitNavModule(Yes, Off, 128, 20000);

  STB_InitDriftComp(On);

  PVM_TriggerMode = per_Slice;
  STB_InitTriggerModule();
  ParxRelsMakeNonEditable("PVM_TriggerMode");

  STB_InitMapShim();

  if (ParxRelsParHasValue("RecoMode") == No)
    RecoMode = CMPLX_RECO_MODE;

  if (ParxRelsParHasValue("SearchMode") == No)
    SearchMode = search_90;

  SearchRangeRelation();

  if (ParxRelsParHasValue("PkMaxIdx") == No)
    PkMaxIdx = 0;

  if (ParxRelsParHasValue("PkMaxIdxFrac") == No)
    PkMaxIdxFrac = 0.0;

  if (ParxRelsParHasValue("SkipSubAdjustments") == No)
    SkipSubAdjustments = No;

  if(!ParxRelsParHasValue("InAdjPlatform"))
    InAdjPlatform=No;

  if(!ParxRelsParHasValue("AdjRunning"))
    AdjRunning=No;

  if(!ParxRelsParHasValue("AdjRefPowXLog"))
    AdjRefPowXLog=none;

  STB_InitAtsReferencePosition();

  AdjProtName[0]='\0';

  PkRangeRelation();

  if(!ParxRelsParHasValue("PhaseCorrVals"))
  {
    PhaseCorrVals[0]=0.0;
    PhaseCorrVals[1]=0.0;
  }


  if(!ParxRelsParHasValue("PhaseCorrVals"))
  {
    PhaseCorrVals[0]=0.0;
    PhaseCorrVals[1]=0.0;
  }

  if(!ParxRelsParHasValue("Messages"))
  {
    PARX_change_dims("Messages", 1, 1024);
    strcpy(Messages[0], "\0");
  }


  AdjRunning=No;

  /* Once all parameters have initial values, the backbone is called
     to assure they are consistent */
  backbone();

  DB_MSG(( "<--initMeth()" ));
}


/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
