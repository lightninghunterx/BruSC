/****************************************************************
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
  int dimRange[2] = { 2,3 };
  int lowMat[3]   = { 16, 16, 8 };
  int upMat[3]    = { 512, 512, 256 };
  int defaultMat[3] = {96, 64, 32};


  DB_MSG(( "--> initMeth\n" ));


  /*
   * ShowParam initialization
   */

  ShowAllParsRange();

  /*
   *  initialization of PVM_Nuc1
   */

  STB_InitNuclei(1);

  /*
   *  initialize local gradient parameter 
   */

  GradStabTimeRange();


  DeriveGainsRange();

  /*
   *  init 3D phase encoding parameters
   */


  Phase3DGradLim     = 57.0;
  InitPhase3DEncoding(ShowAllPars);

  ParxRelsMakeNonEditable("Phase3DGradDur"); /* ,ReadDephTime"); */

  
  /*
   * Slice selection parameter initialization
   */
  
  ExcSliceGradLim     = 100.0;
  ExcSliceRephGradLim = 100.0;

  InitSliceSelection(ShowAllPars);

  PackDelRange();

  /* 
   * RF Pulse initialization
   */
  
  InitRFPulses();


  /* segments */
  NSegmentsRange();


  /* Encoding */
  STB_InitEncoding( 2, dimRange, lowMat, upMat, defaultMat);  

  /* Initialisation of geometry parameters */ 
  STB_InitImageGeometry();

  /* 
   * Initialisation of modules 
   */

  STB_InitEpi(UserSlope);
  STB_InitFatSupModule();
  STB_InitTriggerModule();
  STB_InitFovSatModule();
  STB_InitDiffusionPreparation(Yes);
  STB_InitDummyScans(1000.0);
  STB_InitDriftComp(On);

  /* initialize mapshim parameter class */
  STB_InitMapShim();
  
  /*
   *  initialize standard imaging parameters NA TR TE
   */

  EffSwRange();

  RepetitionTimeRange();
  AveragesRange();
  RepetitionsRange();
  EchoTimeRange();

  /* initialization method specific reconstruction */
  RecoMethModeRange();

  ParxRelsMakeNonEditable("PVM_MinEchoTime,PVM_EchoPosition");

  // constrain ATS to reference position
  if(!ParxRelsParHasValue("PVM_AtsBedPositionMode")  ||
     PVM_AtsBedPositionMode==ATS_POS_CURRENT)
  {
    PVM_AtsBedPositionMode=ATS_POS_REFERENCE;
  }
  STB_InitAtsReferencePosition();


  /* 
   * Once all parameters have initial values, the backbone is called
   * to assure they are consistent 
   */
  
  backbone();
 

  DB_MSG(( "<-- initMeth\n" ));

}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/


