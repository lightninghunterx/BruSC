/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002 - 2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$(C) 2002 - 2003 Bruker BioSpin MRI GmbH";

#define DEBUG		0


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
  int dimRange[2] = { 1,1 };
  int lowMat[3]   = { 32,32,32};
  int upMat[3]    = { 2048,256,256 };
  int defaultMat[3] = {128, 128, 64};

  DB_MSG(( "-->initMeth\n" ));


  /*  Initialize NA ( see code in parsRelations ) */

  Local_NAveragesRange();

  /* Initialisation of rf pulse parameters */

  STB_InitRFPulse("ExcPulse1",      // name of pulse structure
                  "ExcPulse1Enum",  // name of pulse list parameter
                  "ExcPulse1Ampl",  // name of pulse amplitude parameter
                  "ExcPulse1Shape", // name of pulse shape (for calc. pulses)
                  0,                // used for excitation
                  "bp.exc",         // default shape
                  5000.0,           // default bandwidth
                  30.0);            // default pulse angle

  // check valid range for this specific pulse see parsRelations.c
  ExcPulse1Range();

  /* Initialisation of nucleus */
  STB_InitNuclei(1);

  /* 1: method specific initialisation */

  if(ParxRelsParHasValue("PVM_RepetitionTime") == No)
    PVM_RepetitionTime = 500.0;
  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
    PVM_DeriveGains = Yes;

  /* Initialisation of spoilers */
  if(ParxRelsParHasValue("ReadSpoilerDuration") == No)
    ReadSpoilerDuration = 3.0* CFG_GradientRiseTime();
  if(ParxRelsParHasValue("ReadSpoilerStrength") == No)
    ReadSpoilerStrength = 33;

  ParxRelsMakeNonEditable("ReadSpoilerDuration");

 /* Encoding */
  STB_InitEncoding( 1, dimRange, lowMat, upMat, defaultMat);

  /* Initialisation of geometry parameters */
  if(ParxRelsParHasValue("PVM_Fov") == No)
  {
      double defext[3];
      CFG_DefaultExtent(defext);
      PVM_Fov[0]=1.5*defext[0];
  }
  STB_InitAtsReferencePosition();
  STB_InitImageGeometry();

  EffSWhRange();

  if(ParxRelsParHasValue("GradientDirection") == No)
    GradientDirection = Z_dir;
  GradientDirectionRange();

  ParxRelsMakeNonEditable("GradientCurrent");
  ParxRelsHideInEditor("PVM_Isotropic");

  backbone();

  DB_MSG(("<--initMeth\n"));

}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/









