/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002-2004
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 * ***************************************************************/

static const char resid[] = "$Id$ (C) 2004 Bruker BioSpin MRI GmbH";

#define DEBUG       0
#define DB_MODULE   0
#define DB_LINE_NR  0


#include "method.h"


void initMeth()
{
  DB_MSG(("-->initMeth"));


  /* nucleus */

  STB_InitNuclei(1);

  /* RF Pulse */
  
  InitRFPulses();

  /* local parameters */
  
  EndOfScan        = 0.01;
  GradMaxDutyCycle = CFG_GradientMaxDutyCycle("mean");
      
  if (!ParxRelsParHasValue("PVM_EffSWh"))
  {
    PVM_EffSWh = 100000.0;
  }
  
  if (!ParxRelsParHasValue("MinimumTR"))
  {
    MinimumTR = No;
  }

  if (!ParxRelsParHasValue("AcqDelay"))
  {
    AcqDelay = 0.0;
  }
  
  if (!ParxRelsParHasValue("OverSampling"))
  {
    OverSampling = 4;
  }
  
  if (!ParxRelsParHasValue("ProUnderSampling"))
  {
    ProUnderSampling = 1.0;
  }
  if (!ParxRelsParHasValue("MinimumUnderSampling"))
  {
    MinimumUnderSampling = Yes;
  }
  if (!ParxRelsParHasValue("HalfAcquisition"))
  {
    HalfAcquisition = No;
  }
  
  if (!ParxRelsParHasValue("Spoiling"))
  {
    Spoiling = 1.5;
  }
  if (!ParxRelsParHasValue("Reorder"))
  {
    Reorder = No;
  }
  if (!ParxRelsParHasValue("GradOff"))
  {
    GradOff = No;
  }
  
  if (!ParxRelsParHasValue("UseOrigin"))
  {
    UseOrigin = No;
  }
  
  RecoSkipRange();
    
  if (!ParxRelsParHasValue("RecoT2Filter"))
  {
    RecoT2Filter = 0.0;
  }
    
  if (!ParxRelsParHasValue("RecoOffResonance"))
  {
    RecoOffResonance = 0.0;
  }
  
  if (!ParxRelsParHasValue("RecoOnline"))
  {
    RecoOnline = Yes;
  }
    
  if (!ParxRelsParHasValue("RecoUseOrigin"))
  {
    RecoUseOrigin = No;
  } 

  if (!ParxRelsParHasValue("RecoScaleOrigin"))
  {
    RecoScaleOrigin = 1.0;
  } 

  if (!ParxRelsParHasValue("RecoProCorr"))
  {
    RecoProCorr = ProCorr_None;
  } 

  if (!ParxRelsParHasValue("RecoOver"))
  {
    RecoOver = 1;
  } 

  if (!ParxRelsParHasValue("RecoRegularise"))
  {
    RecoRegularise = 0.0;
  } 

  if (!ParxRelsParHasValue("RecoStorePro"))
  {
    RecoStorePro = No;
  } 


  /* geometry */
  
  {
    int dimRange[2] = {3, 3};
    int loMat[3] = {8, 8, 8};
    int upMat[3] = {512, 512, 512};
    int dfMat[3] = {128, 128, 128};
    
    STB_InitEncoding(3, dimRange, loMat, upMat, dfMat); 
  }
  
  ParxRelsMakeNonEditable("PVM_SliceThick");
  ParxRelsMakeNonEditable("PVM_ObjOrderScheme");
  ParxRelsMakeNonEditable("PVM_NSPacks");
  ParxRelsMakeNonEditable("PVM_SPackArrNSlices");
  ParxRelsMakeNonEditable("PVM_SPackArrSliceGapMode");
  ParxRelsMakeNonEditable("PVM_SPackArrSliceGap");
  ParxRelsMakeNonEditable("PVM_SPackArrSliceDistance");
  
  STB_InitImageGeometry();
  ParxRelsMakeNonEditable("PVM_AntiAlias");
  PTB_SetGeoObjShape("PVM_SliceGeoObj", Ellipsoid_In_GobjShape);
  
  /* standard imaging parameters */

  if (!ParxRelsParHasValue("PVM_NRepetitions"))
  {
    PVM_NRepetitions = 1;
  }
  PVM_NEchoImages     = 1;
  PVM_NMovieFrames    = 1;
  PVM_MotionSupOnOff  = On;
  if (!ParxRelsParHasValue("PVM_RepetitionTime"))
  {
    PVM_RepetitionTime = 2.5;
  }
  if (!ParxRelsParHasValue("PVM_NAverages"))
  {
    PVM_NAverages = 1;
  }
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_AcquisitionTime");

  /* modules */
  STB_InitDummyScans(1000.0);
  STB_InitTriggerModule();
  STB_InitFatSupModule();
  STB_InitFovSatModule();
  STB_InitSegmentationModule();
  STB_InitEvolutionModule();

  /* initialize mapshim parameter class */
  STB_InitMapShim();

  /* GS parameters */  
  
  ACQ_SetupDim = 1;
  ParxRelsParRelations("ACQ_SetupDim", Yes);
  ACQ_SetupOnlineReco = Yes;
  ParxRelsParRelations("ACQ_SetupOnlineReco", Yes);
  ACQ_SetupRecoDisplay = Yes;
  ParxRelsParRelations("ACQ_SetupRecoDisplay", Yes);
  ACQ_SetupImageType = MAGNITUDE_IMAGE;
  ParxRelsParRelations("ACQ_SetupImageType", Yes);

  STB_InitAtsReferencePosition();
  /* make consistent */
  
  backbone();
 
  DB_MSG(("<--initMeth"));
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/


