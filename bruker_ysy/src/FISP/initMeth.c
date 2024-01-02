 /****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999-2004
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2003 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	1




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
  int dimRange[2]   = {2,3};
  int lowMat[3]     = {32, 32, 8};
  int upMat[3]      = {2048, 2048, 512};
  int defaultMat[3] = {128, 128, 64};

  DB_MSG(( "Entering fisp:initMeth()\n" ));

  UT_ClearRequest();

  /* encoding group */
  STB_InitEncoding( 2, dimRange, lowMat, upMat, defaultMat);  

  /* Initialisation of nucleus */  
  STB_InitNuclei(1);

			  
  /*** Initialisation of rf pulse parameters ***/

  STB_InitRFPulse("ExcPulse1",      // name of pulse structure
                  "ExcPulse1Enum",  // name of pulse list parameter
                  "ExcPulse1Ampl",  // name of pulse amplitude parameter
                  "ExcPulse1Shape", // name of pulse shape (for calc. pulses)
                  0,                // used for excitation
                  "Calculated",     // default shape
                  3000.0,           // default bandwidth
                  60.0,             // default pulse angle
                  2);               // default pulse sharpness

  // check valid range for this specific pulse see parsRelations.c
  ExcPulse1Range();


  STB_InitRFPulse("InvPulse1",      // name of pulse structure
                  "InvPulse1Enum",  // name of pulse list parameter
                  "InvPulse1Ampl",  // name of pulse amplitude parameter
                  "InvPulse1Shape", // name of pulse shape (for calc. pulses)
                  2,                // used for inversion
                  "Calculated",     // default shape
                  5000.0,           // default bandwidth
                  180.0);           // default pulse angle

  // check valid range for this specific pulse see parsRelations.c
  InvPulse1Range();



  /* Initialisation of geometry parameters */
  STB_InitAtsReferencePosition();
  STB_InitImageGeometry();

  LocalSWhRange();

  ParxRelsMakeNonEditable("Seg_time");
  ParxRelsMakeNonEditable("Seg_Sep_time");
  ParxRelsMakeNonEditable("Seg_Sep_Fra_time");
  ParxRelsMakeNonEditable("Total_scan_time");
  ParxRelsMakeNonEditable("Alpha2_Prep_time");
  ParxRelsMakeNonEditable("Prescan_Delay");
  ParxRelsMakeNonEditable("EncGradDur");
  ParxRelsMakeNonEditable("PVM_EchoPosition");
   
  if(ParxRelsParHasValue("RFSpoilerOnOff") == 0)
    RFSpoilerOnOff = Off;
 
  if(ParxRelsParHasValue("Ssfp") == No)
    Ssfp = TRUE_FISP;

  if(ParxRelsParHasValue("Seg_mode") == No) 
    Seg_mode = sequential; 

  if(ParxRelsParHasValue("PVM_NRepetitions") == No) 
     PVM_NRepetitions = 1;

  if(ParxRelsParHasValue("PVM_InversionTime") == No)  
     PVM_InversionTime = 0.0;

  if(ParxRelsParHasValue("PVM_EchoTime") == No)  
    PVM_EchoTime = 2.5;

  if(ParxRelsParHasValue("FISP_inversion_enable") == No) 
     FISP_inversion_enable = No; 

  if(ParxRelsParHasValue("Nsegments") == No)
    Nsegments = 1;
    
  if(ParxRelsParHasValue("PVM_NMovieFrames") == No)
    PVM_NMovieFrames = 1;

  if(ParxRelsParHasValue("PVM_NAverages") == No)
    PVM_NAverages = 1;

  if(ParxRelsParHasValue("PhaseAdvance") == No)
    PhaseAdvance = 180;

  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
    PVM_DeriveGains = Yes;

  if(ParxRelsParHasValue("FlipAngMz") == No)
    FlipAngMz = 30;

  if(ParxRelsParHasValue("YesNoMinScanRepTime") == 0)
    YesNoMinScanRepTime = Yes;

  if(ParxRelsParHasValue("KeepSteadyState3D") == 0)
    KeepSteadyState3D = No;

  /* Initialisation of spoilers */
  MRT_InitSpoiler("PrepSpoiler");
  MRT_InitSpoiler("SliceSpoiler");
  
  /* Initialisation of modules */
  STB_InitDummyScans(1000.0);
  STB_InitTriggerModule();
  STB_InitEvolutionModule();

  /* initialize mapshim parameter class */
  STB_InitMapShim();

  MyEchoPosRange();
  SSFPPrepRange();
  NumPrepRange();

  /* initialize method ats parameter group */
  STB_AtsInit();
  
  backbone(); 

  DB_MSG(( "Exiting fisp:initMeth()" ));

}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/


