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
#define DB_MODULE	1
#define DB_LINE_NR	1

/*:=INFO=:*******************************************************
 *
 * Description :
 *   it contains functions necessary for initialisation of the
 *	onepulse method.
 *
 *::=info=:******************************************************/

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
  int dimRange[2] = { 2,2 };
  int lowMat[3] = { 16, 16, 8 };
  int upMat[3]  = { 512, 512, 256 };
  int defaultMat[3] = {96, 96, 64};
 

  DB_MSG(( "Entering FAIR_EPI2:initMeth()" ));


  /* initialise NSegments */
  NSegmentsRange();

  /*  Initialize effective bandwidth */
  EffSWhRange();

  /* Encoding */
  STB_InitEncoding( 2, dimRange, lowMat, upMat, defaultMat);  

  /*  Initialize NA ( see code in parsRelations ) */
  Local_NAveragesRange();

  /*Initialisation of recovery time*/
  if(ParxRelsParHasValue("RecoveryTime") == No)
      RecoveryTime = 3000;

  /*Initialisation of repetitions time*/
  if(ParxRelsParHasValue("PVM_RepetitionTime") == No)
      PVM_RepetitionTime = 3000;

  /*Initialisation of automatic derive RF gains*/
  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
      PVM_DeriveGains = Yes;

  /* Initialisation of signal type */
  if(ParxRelsParHasValue("PVM_SignalType") == No)
      PVM_SignalType = SignalType_Echo;

  /* Bandwidth Scaling Factor */
  BwScaleRange();

  /* Initialisation of rf pulse parameters */

  STB_InitRFPulse("ExcPulse1",      // name of pulse structure
                  "ExcPulse1Enum",  // name of pulse list parameter
                  "ExcPulse1Ampl",  // name of pulse amplitude parameter
                  "ExcPulse1Shape", // name of pulse shape (for calc. pulses)
                  0,                // used for excitation
                  "Calculated",     // default shape
                  2000.0,           // default bandwidth
                  90.0);            // default pulse angle

  // check valid range for this specific pulse see parsRelations.c
  ExcPulse1Range();

  STB_InitRFPulse("RefPulse1",      // name of pulse structure
                  "RefPulse1Enum",  // name of pulse list parameter
                  "RefPulse1Ampl",  // name of pulse amplitude parameter
                  "RefPulse1Shape", // name of pulse shape (for calc. pulses)
                  1,                // used for refocusing
                  "Calculated",     // default shape
                  2000.0,           // default bandwidth
                  180.0);           // default pulse angle

  RefPulse1Range();
  
  /* Initialisation of nucleus and frequency */  
  STB_InitNuclei(1);

  /* Initialisation of geometry parameters */
  STB_InitImageGeometry();
  
  ExSliceRephaseTimeRange();
  
  /* Initialisation of spoilers */
  MRT_InitSpoiler("SliceSpoiler");
 
  /* Initialisation of modules */
  STB_InitEpi(UserSlope);
  STB_InitFairModule();
  STB_InitFatSupModule();
  STB_InitSatTransModule();
  STB_InitFovSatModule();
  STB_InitTriggerModule();
  STB_InitDummyScans(0.0);
 
  /* initialize mapshim parameter class */
  STB_InitMapShim();

  /* Visibility of Scan Editor parameters */
  ParxRelsShowInEditor("PVM_EchoTime1,PVM_EchoTime,PVM_NEchoImages");
  ParxRelsHideGroupInEditor("ScanEditorInterface");
  ParxRelsShowInFile("PVM_EchoTime1,PVM_EchoTime,PVM_NEchoImages");
  ParxRelsMakeNonEditable("PVM_EchoTime1,PVM_EchoTime,PVM_MinEchoTime");
  ParxRelsMakeNonEditable("PVM_NEchoImages,PVM_EchoPosition");

  // constrain ATS to reference position
  if(!ParxRelsParHasValue("PVM_AtsBedPositionMode")  ||
     PVM_AtsBedPositionMode==ATS_POS_CURRENT)
  {
    PVM_AtsBedPositionMode=ATS_POS_REFERENCE;
  }
  STB_InitAtsReferencePosition();

  /* Once all parameters have initial values, the backbone is called
     to assure they are consistent */
  backbone();
 

  DB_MSG(( "Exiting FAIR_EPI2:initMeth()" ));

}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/









