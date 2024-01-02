/****************************************************************
 *
 * Copyright (c) 2008 - 2018
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2008-2018 Bruker BioSpin MRI GmbH";

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
  int dimRange[2] = { 2,3 };
  int lowMat[3] = { 16, 16, 8 };
  int upMat[3]  = { 512, 512, 256 };
  int defaultMat[3] = {96, 64, 32};

  DB_MSG(( "Entering epi2:inistMeth()" ));

  /*  Initialize NA ( see code in parsRelations ) */
  Local_NAveragesRange();

  RephaseTimeRange();

  /* Number of slices */
  
  InpSlicesRange();

  /* Number of slices measured in one loop (for one inversion) */
  
  SliceGroupSizeRange();  

  /* IRSpacing: Time between successive IR slice images */ 

  IRSpacingRange();

  /* IROffset: Time between Inversion pulse and first IR slice image */

  IROffsetRange();

  /* NIRPoints: Number of measurement points (per slice) after inversion */

  NIRPointsRange();

  /* segments */
  NSegmentsRange();

  /* Encoding */
  STB_InitEncoding( 2, dimRange, lowMat, upMat, defaultMat);  

  /*Initialisation of repetitions time*/
  if(ParxRelsParHasValue("PVM_RepetitionTime") == No)
      PVM_RepetitionTime = 3000;

  /*Initialisation of automatic derive RF gains*/
  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
      PVM_DeriveGains = Yes;

  /* Initialisation of signal type */
  PVM_SignalType = SignalType_Fid;

  /* Initialisation of rf pulse parameters */
  
  STB_InitRFPulse("ExcPul",        // name of pulse structure
                  "ExcPulseEnum",  // name of pulse list parameter
                  "ExcPulseAmpl",  // name of pulse amplitude parameter
                  "ExcPulseShape", // name of pulse shape (for calc. pulses)
                  0,               // used for excitation
                  "Calculated", // default shape
                  2000.0,          // default bandwidth
                  20.0);           // default pulse angle
  ExcPulRange();

  STB_InitRFPulse("RefPul",        // name of pulse structure
                  "RefPulseEnum",  // name of pulse list parameter
                  "RefPulseAmpl",  // name of pulse amplitude parameter
                  "RefPulseShape", // name of pulse shape (for calc. pulses)
                  1,               // used for refocusing
                  "Calculated", // default shape
                  2000.0,          // default bandwidth
                  180.0);          // default pulse angle
  RefPulRange();

  STB_InitRFPulse("InvPul",        // name of pulse structure
                  "InvPulseEnum",  // name of pulse list parameter
                  "InvPulseAmpl",  // name of pulse amplitude parameter
                  "InvPulseShape", // name of pulse shape (for calc. pulses)
                  2,               // used for inversion
                  "Calculated", // default shape
                  5000.0,          // default bandwidth
                  180.0);          // default pulse angle
  InvPulRange();


  /* Initialisation of nucleus and frequency */  
  STB_InitNuclei(1);


  /* ExSliceRephaseTime = 1.5; */

  /* Initialisation of the delay between the slices packages */  
  PackDelRange();
   
  /* Initialisation of spoilers */
  MRT_InitSpoiler("SliceSpoiler");
  MRT_InitSpoiler("InvSliceSpoiler");

  /* other parameters */
  if(ParxRelsParHasValue("SliceGroupRotation") == No)
    SliceGroupRotation = No;

  /* Initialisation of geometry parameters */
  STB_InitImageGeometry();
  
  /* initializtion of bandwidth */
  LocalSWhRange();

  /* Initialisation of modules */
  STB_InitEpi(UserSlope);
  STB_InitFatSupModule();
  STB_InitSatTransModule();
  STB_InitFovSatModule();
  STB_InitTriggerModule();
  STB_InitTriggerOutModule();
  STB_InitTaggingModule();
  STB_InitDummyScans(0.0);

  /* initialize mapshim parameter class */
  STB_InitMapShim();

  /* Visibility of Scan Editor parameters */
  ParxRelsShowInEditor("PVM_EchoTime,IRSpacing,SliceGroupRotation");
  ParxRelsShowInFile("PVM_EchoTime,IRSpacing,SliceGroupRotation");
  ParxRelsMakeNonEditable("PVM_EchoTime,PVM_MinEchoTime,PVM_EchoPosition");
  
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
 

  DB_MSG(( "Exiting epi2:initMeth()" ));

}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/









