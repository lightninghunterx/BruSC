/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002-2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2002-2003 Bruker BioSpin MRI GmbH";

#define DEBUG    0
#define DB_MODULE  0
#define DB_LINE_NR  0




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
  int dimRange[2] = { 2,3 };
  int lowMat[3]   = { 32, 8, 8 };
  int upMat[3]    = { 512, 512, 256 };
  int defaultMat[3] = {128, 128, 32};

  DB_MSG(( "--> initMeth\n" ));

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
                  90.0);            // default pulse angle

  // check valid range for this specific pulse see parsRelations.c
  ExcPulse1Range();


  /* Encoding */
  STB_InitEncoding( 2, dimRange, lowMat, upMat, defaultMat);

  /* Initialisation of geometry parameters */
  STB_InitImageGeometry();
  STB_InitAtsReferencePosition();

  /* Init Bandwidth */
  EffSWhRange();

  /* initialize diffusion parameters */
  STB_InitDiffusionPreparation(Yes);

  /* initialize preparation module parameters */
  STB_InitFatSupModule();
  STB_InitFovSatModule();
  STB_InitTriggerModule();

  /* initialize dummy scans */
  STB_InitDummyScans(1000.0);

  /* initialize mapshim parameter class */
  STB_InitMapShim();

  /* initialize standard imaging parameters */
  EncGradDurRange();
  ExcSliceRephTimeRange();
  RepetitionTimeRange();
  AveragesRange();
  repetitionsRange();
  EchoTimeRange();
  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
    PVM_DeriveGains = Yes;

  /* initialization method specific reconstruction */
  RecoMethModeRange();
  
  /*
   * Once all parameters have initial values, the backbone is called
   * to assure they are consistent
   */

  backbone();


  DB_MSG(( "<-- initMeth\n" ));

}

/****************************************************************/
/*    E N D   O F   F I L E        */
/****************************************************************/
