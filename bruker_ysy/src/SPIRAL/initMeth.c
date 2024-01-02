/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2007
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 * ***************************************************************/

static const char resid[] = "$Id$ (c) 2007 Bruker BioSpin MRI GmbH";

#define DEBUG       0


#include "method.h"


void initMeth()
{
  DB_MSG(( "-->initMeth" ));

  /* hardware settings */

  MinDelay   = 0.001;
  FreqSwitch = 0.01;
  RiseT      = CFG_GradientRiseTime();
  GradWait   = CFG_InterGradientWaitTime();

  /* local parameters */

  if (!ParxRelsParHasValue("PVM_SignalType"))
  {
    PVM_SignalType = SignalType_Fid;
  }

  if (!ParxRelsParHasValue("MinimiseTE"))
  {
    MinimiseTE = Yes;
  }

  GradSyncRange();

  /* nucleus */

  STB_InitNuclei(1);

  /* RF pulse */

  if (!ParxRelsParHasValue("PVM_DeriveGains"))
  {
    PVM_DeriveGains = Yes;
  }

  STB_InitRFPulse("ExcPul",
                  "ExcPulEnum",
                  "ExcPulAmpl",
                  "ExcPulShape",
                  0,
                  "Calculated",
                  2000.0,
                  30.0);
  STB_InitRFPulse("RefPul",
                  "RefPulEnum",
                  "RefPulAmpl",
                  "RefPulShape",
                  1,
                  "Calculated",
                  2000.0,
                  180.0);
  ExcPulRange();
  RefPulRange();

  /* spoiling */

  MRT_InitSpoiler("SliceSpoiler");

  /* acquisition */

  EffSWhRange();

  /* geometry */
  {
    int dimRange[2] = {2, 3};
    int loMat[3]    = {16, 16, 8};
    int upMat[3]    = {512, 512, 512};
    int dfMat[3]    = {64, 64, 16};

    STB_InitEncoding(2, dimRange, loMat, upMat, dfMat);
  }

  STB_InitImageGeometry();


  /* standard imaging parameters */

  if (!ParxRelsParHasValue("PVM_RepetitionTime"))
  {
    PVM_RepetitionTime = 1000.0;
  }
  if (!ParxRelsParHasValue("PVM_NRepetitions"))
  {
    PVM_NRepetitions = 1;
  }
  if (!ParxRelsParHasValue("PVM_EchoTime"))
  {
    PVM_EchoTime = 5.0;
  }
  if (!ParxRelsParHasValue("PVM_MotionSupOnOff"))
  {
    PVM_MotionSupOnOff = Off;
  }
  NAveragesRange();
  MovieFramesRange();
  PVM_NEchoImages = 1;

  /* display-only parameters */

  ParxRelsMakeNonEditable("PVM_MinEchoTime");
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  /* spiral module */

  STB_InitSpiral();

  /* trajectory module */

  double scaleInt[9] = {1.0, 1.0, 0.0,
                       -1.0, 1.0, 0.0,
                        0.0, 0.0, 0.0};
  STB_InitTraj(1, 2, 3, 0, No, 2, Yes, 0, NULL, No, 2,
    "PVM_SpiralShape1", "PVM_SpiralShape2", NULL,
    "PVM_SpiralInterleavCos", "PVM_SpiralInterleavSin", NULL,
    "PVM_SpiralInterleavSin", "PVM_SpiralInterleavCos", NULL,
    NULL, NULL, NULL,
    scaleInt);

  /* modules */

  STB_InitFatSupModule();
  STB_InitSatTransModule();
  STB_InitFovSatModule();
  STB_InitTaggingModule();
  STB_InitEvolutionModule();
  STB_InitTriggerModule();
  STB_InitTriggerOutModule();
  STB_InitDummyScans(0.0);

  /* mapshim */
  STB_InitMapShim();

  /* make consistent */

  backbone();

  DB_MSG(( "<--initMeth" ));
}

/****************************************************************/
/*  E N D   O F   F I L E                                       */
/****************************************************************/


