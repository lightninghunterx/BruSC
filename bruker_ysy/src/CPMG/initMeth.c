/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2003
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
#define DB_MODULE	1
#define DB_LINE_NR	1


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


  DB_MSG(( "Entering singlepulse:initMeth()" ));


  /*  Initialize PVM_NAverages PVM_NRepetitions see code in parsRelations.c */
  Local_NAveragesRange();
  NrepRange();


  /* Initialisation of rf pulse parameters */
  
  STB_InitRFPulse("ExcPulse1",      // name of pulse structure
                  "ExcPulse1Enum",  // name of pulse list parameter
                  "ExcPulse1Ampl",  // name of pulse amplitude parameter
                  "ExcPulse1Shape", // name of pulse shape (for calc. pulses)
                  0,                // used for excitation
                  "bp.exc",     // default shape
                  5000.0,           // default bandwidth
                  90.0);            // default pulse angle

  // check valid range for this specific pulse see parsRelations.c
  ExcPulse1Range();

  STB_InitRFPulse("RefPulse1",      // name of pulse structure
                  "RefPulse1Enum",  // name of pulse list parameter
                  "RefPulse1Ampl",  // name of pulse amplitude parameter
                  "RefPulse1Shape", // name of pulse shape (for calc. pulses)
                  1,                // used for refocusing
                  "bp.exc",     // default shape
                  5000.0,           // default bandwidth
                  180.0);           // default pulse angle
  
  RefPulse1Range();



  /* Initialisation of nucleus */  
  if (ParxRelsParHasValue("PVM_Nucleus1") == 0)
    PVM_Nucleus1Enum = (STANDARD_NUCLEUS_TYPE)0;

  STB_InitNuclei(1);

  /* initialisation of spectroscopy */
  if(!ParxRelsParHasValue("PVM_SpecMatrix"))
    PVM_SpecMatrix[0]=256;
  if(!ParxRelsParHasValue("PVM_SpecSWH"))
    PVM_SpecSWH[0]= 25000;
  STB_InitSpectroscopy( 1, 1, 1 ,  PVM_Nucleus1 , 200.0, 1000000 );

  /* initializtion of multi receiver parameters */
  STB_InitEncoding();

  if(ParxRelsParHasValue("NEchoes") == No)
    NEchoes = 12;

  if(ParxRelsParHasValue("EchoSpacing") == No)
    EchoSpacing = 20.0;
  
  if(ParxRelsParHasValue("PVM_RepetitionTime") == No)
    PVM_RepetitionTime = 5000.0; 
    
  if(ParxRelsParHasValue("SpoilerOnOff") == 0)
    SpoilerOnOff = Off;

  if(ParxRelsParHasValue("PVM_DeriveGains") == 0)
    PVM_DeriveGains = Yes;

  if(ParxRelsParHasValue("Spoiler_duration") == No)
    Spoiler_duration = 1.0;

  if(ParxRelsParHasValue("Spoiler_strength") == No)
    Spoiler_strength = 10;
 

  /* Initialisation of modules */
  STB_InitTriggerModule();
  STB_InitDummyScans(0);

  /* Once all parameters have initial values, the backbone is called
     to assure they are consistent */
  backbone();
 

  DB_MSG(( "Exiting singlepulse:initMeth()" ));

}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/









