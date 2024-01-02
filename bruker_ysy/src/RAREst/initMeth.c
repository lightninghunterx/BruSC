/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002-2004
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$(C) 2002-2004 Bruker BioSpin MRI GmbH";

#define DEBUG       0
#define DB_MODULE   1
#define DB_LINE_NR  1


#include "method.h"

/*:=MPB=:=======================================================*
 *
 * Global Function: initMeth
 *
 * Description: This procedure is implicitly called when this
 *  method is selected.
 *
 * Error History:
 *
 * Interface:              */

void initMeth()
/*:=MPE=:=======================================================*/
{
  int dimRange[2] = { 2,2 };
  int lowMat[3]   = { 32, 8, 8 };
  int upMat[3]    = { 2048, 2048, 256 };
  int defaultMat[3]    = { 128, 128, 32 };


  DB_MSG(("-->initMeth\n"));


  /*  Initialize NA ( see code in parsRelations ) */
  Local_NAveragesRange();
  if(ParxRelsParHasValue("PVM_NRepetitions") == No)
    PVM_NRepetitions = 1;

  /* init grad stab time */
  GradStabRange();
  RampFormRange();

  SlewRatePerCentRange();


  if(ParxRelsParHasValue("PVM_RepetitionTime") == No)
    PVM_RepetitionTime = 3000.0;
  if(ParxRelsParHasValue("PVM_EchoTime") == No)
    PVM_EchoTime = 5.0;
  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
    PVM_DeriveGains = Yes;


  /* init slice scaling */
  SliceBandWidthScaleRange();

  /* Initialisation of nucleus */
  STB_InitNuclei(1);

   /* Initialisation of rf pulse parameters */

  STB_InitRFPulse("ExcPulse1",      // name of pulse structure
                  "ExcPulse1Enum",  // name of pulse list parameter
                  "ExcPulse1Ampl",  // name of pulse amplitude parameter
                  "ExcPulse1Shape", // name of pulse shape (for calc. pulses)
                  0,                // used for excitation
                  "Calculated",     // default shape
                  2000.0,           // default bandwidth
                  90.0,             // default pulse angle
                  2);               // sharpness

  // check valid range for this specific pulse see parsRelations.c
  ExcPulse1Range();

  STB_InitRFPulse("RefPulse1",      // name of pulse structure
                  "RefPulse1Enum",  // name of pulse list parameter
                  "RefPulse1Ampl",  // name of pulse amplitude parameter
                  "RefPulse1Shape", // name of pulse shape (for calc. pulses)
                  1,                // used for refocusing
                  "Calculated",     // default shape
                  2000.0,           // default bandwidth
                  180.0,            // default pulse angle
                  2);               // sharpness

  RefPulse1Range();

  EffSWhRange();

  /* Encoding.*/
  if(!ParxRelsParHasValue("PVM_EncPft"))
    InitEncoding = Yes;
  else
    InitEncoding = No;

  STB_InitEncoding( 2, dimRange, lowMat, upMat, defaultMat);


  /* Initialisation of geometry parameters */
  STB_InitImageGeometry();

  /* echo parameters */

  if(ParxRelsParHasValue("PVM_RareFactor") == No)
    PVM_RareFactor = 128;
  if(ParxRelsParHasValue("PVM_NEchoImages") == No)
    PVM_NEchoImages = 1;
  PVM_NEchoScan1 = 1;
  PVM_NEchoScan2 = 1;

  ParxRelsMakeNonEditable("NEchoes,PVM_NEchoScan1,PVM_NEchoScan2,PVM_EchoPosition");
  ParxRelsMakeNonEditable("ShapeSize");

  /* spoilers */
  if (!ParxRelsParHasValue("AutoSpoil"))
    AutoSpoil = Yes;
  SpoilingReadPxRange();
  SpoilingSliceSlRange();
  MRT_InitSpoiler("RepetitionSpoiler");

  /* activating motion suppression */
  if (ParxRelsParHasValue("PVM_MotionSupOnOff") == 0)
     PVM_MotionSupOnOff = Off;

  /* Initialisation of modules */
  STB_InitFatSupModule();
  STB_InitSatTransModule();
  STB_InitFovSatModule();
  STB_InitTriggerModule();
  STB_InitEvolutionModule();
  STB_InitSelIrModule();
  STB_InitDummyScans(1000.0);
  STB_InitTaggingModule();
  STB_InitDriftComp(On);

  /* initialize mapshim parameter class */
  STB_InitMapShim();
  
  /* initialize method ats parameter group */
  STB_AtsInit();

  /*
   * Once all parameters have initial values, the backbone is called
   * to assure they are consistent
   */


  backbone();

  DB_MSG(( "<--initMeth\n" ));

}




/****************************************************************/
/*    E N D   O F   F I L E        */
/****************************************************************/
