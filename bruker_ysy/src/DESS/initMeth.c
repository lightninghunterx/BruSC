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

#define DEBUG		0
#define DB_MODULE	0
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
  int dimRange[2] = { 1,3 };
  int lowMat[3]   = { 32, 32, 8 };
  int upMat[3]    = { 2048, 2048, 512 };
  int defaultMat[3] = {128, 128, 64};
  
  
  DB_MSG(( "-->DESS:initMeth\n" ));
    
   
  /* encoding group */
  STB_InitEncoding( 2, dimRange, lowMat, upMat, defaultMat);  

  /* Initialisation of rf pulse parameters */
    
  /*
   * 1: pulses declared in parDefinitions.h 
   * in this case - ExcPulse1. We initalise it to default name, 
   * 5000.0 Hz, and 30 deg flip angle
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
                  "Calculated",     // default shape
                  3000.0,           // default bandwidth
                  30.0);            // default pulse angle

  // check valid range for this specific pulse see parsRelations.c
  ExcPulse1Range();
  
  /* Initialisation of nucleus */  
  STB_InitNuclei(1);

  /* Initialisation of geometry parameters */
  STB_InitImageGeometry();
  
  /* 1: method specific initialisation */
  if(!ParxRelsParHasValue("PVM_NRepetitions"))
    PVM_NRepetitions = 1;
  if(!ParxRelsParHasValue("PVM_NAverages"))
    PVM_NAverages = 1;
  if(!ParxRelsParHasValue("PVM_RepetitionTime"))
    PVM_RepetitionTime = 10.0;
  if(!ParxRelsParHasValue("PVM_EchoTime"))
    PVM_EchoTime = 2.5;
  if(!ParxRelsParHasValue("PVM_DeriveGains"))
    PVM_DeriveGains = Yes;
  if(!ParxRelsParHasValue("DESS_spoiler_flat"))
    DESS_spoiler_flat = No;
  if(!ParxRelsParHasValue("DESS_ReadSpoiling"))
    DESS_ReadSpoiling = 2.0;
  if(!ParxRelsParHasValue("DESS_SliceSpoiling"))
    DESS_SliceSpoiling = 2.0;
  
  DESS_TE1_Range();
  DESS_TE2_Range();
  FWInphaseRange();

  EffSWhRange();

  /* Initialisation of modules */
  STB_InitTriggerModule();
  STB_InitEvolutionModule();
  STB_InitDummyScans(1000.0);

  /* initialize mapshim parameter class */
  STB_InitMapShim();

  /* initialize method ats parameter group */
  STB_AtsInit();

  /* initialization of method specific reconstruction */
  if(!ParxRelsParHasValue("RecoMethMode"))
    RecoMethMode=Default;
  
  MapSnrRange();
  
  /* Visibility of parameters */
  ParxRelsMakeNonEditable("PVM_EchoPosition");
  ParxRelsMakeNonEditable("PVM_MinEchoTime,PVM_AcquisitionTime");
  ParxRelsMakeNonEditable("EncGradDur"); 
  ParxRelsMakeNonEditable("PVM_MinRepetitionTime");
  
  // no running adjustment    
  RunningAdj=0;

  
  /* 
   * Once all parameters have initial values, the backbone is called
   * to assure they are consistent 
   */

  backbone();
  
  DB_MSG(("<--DESS:initMeth\n"));
  
}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/









