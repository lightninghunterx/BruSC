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


  DB_MSG(( "--> initMeth()" ));

  
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
  Local_NAveragesRange();

  if(ParxRelsParHasValue("PVM_NRepetitions") == No)
  PVM_NRepetitions = 1;


  /* Initialisation of rf pulse parameters */

  /*
   * 1: pulses declared in parDefinitions.h 
   * in this case - ExcPulse1. We initalise it to default name, 
   * 5000.0 Hz, and 20 deg flip angle
   * In addition we declare ExcPulse1Enum ExcPulse1Ampl and ExcPulse1Shape
   * to be handled together with pulse struct ExcPulse1. Only if a double
   * array parameter is provided as shape, the pulse supports calculated 
   * shapes.
   */
  
  STB_InitRFPulse("ExcPulse1",      // name of pulse structure
                  "ExcPulse1Enum",  // name of pulse list parameter
                  "ExcPulse1Ampl",  // name of pulse amplitude parameter
                  "ExcPulse1Shape", // name of pulse shape (for calc. pulses)
                  0,                // used for excitation
                  "bp.exc",         // default shape
                  10000.0,          // default bandwidth
                  20.0);            // default pulse angle

  /* check valid range for this specific pulse see parsRelations.c*/
  ExcPulse1Range();

 
  /* Initialise PVM_DeriveGains*/
  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
    PVM_DeriveGains = Yes;
  
  RetroFrequencyLockRelation();


  /* initialisation of spectroscopy */
  STB_InitSpectroscopy( 1, 1, 1 ,  PVM_Nucleus1 , 200.0, 1000000 );


  /* Initialize multi receiver parameters */
  
  STB_InitEncoding();

 
  /* Initialize Timing */
  if(ParxRelsParHasValue("PVM_RepetitionTime") == No)
    PVM_RepetitionTime = 1000.0;

  /* Initialisation of modules */
  STB_InitWsModule (1);
  STB_InitFovSatModule();
  STB_InitSatTransModule();
  STB_InitFatSupModule();
  STB_InitDummyScans(0.0);

  STB_InitNavModule(Yes, Off, 128, 20000);
  RetroFrequencyLockRange();

  STB_InitDriftComp();

  CalcLineWidthRange();

  PVM_TriggerMode = per_Slice;
  STB_InitTriggerModule();
  ParxRelsMakeNonEditable("PVM_TriggerMode");

  STB_InitMapShim();

  /* initialisation of DeadTime */
  DeadTimeRange();

  STB_InitRefScan(0); //RefScan via job0
  if(ParxRelsParHasValue("Edc_OnOff") == No)
    Edc_OnOff = Off;
  if(ParxRelsParHasValue("EdcManualFilter") == No)
    EdcManualFilter = No;  
  if(ParxRelsParHasValue("EdcFilterWidthHz") == No)
    EdcFilterWidthHz = 120;  
 

  STB_InitAtsReferencePosition();
  InitSliceSelection();
  /* Once all parameters have initial values, the backbone is called
     to assure they are consistent */
  backbone();
 
  DB_MSG(( "--> initMeth()" ));

}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
