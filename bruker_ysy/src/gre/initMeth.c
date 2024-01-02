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

#define DEBUG	    0
#define DB_MODULE   0
#define DB_LINE_NR  0


#include "method.h"

void initMeth()
{
  int dimRange[2]   = { 2, 3 };
  int lowMat[3]     = { 32, 8, 8 };
  int upMat[3]      = { 1024, 1024, 256 };
  int defaultMat[3] = { 128, 128, 64 };

  DB_MSG(( "--> initMeth" ));


  /* initialization of PVM_Nucleus1                             */
  STB_InitNuclei(1);

  /* Initialisation of rf pulse parameters                      */
 
  /* pulse declared in parsDefinition.h: ExcPulse1 
   * We initialise it to default name, 
   * 3000.0 Hz, and 30 deg flip angle.
   * In addition we declare ExcPulse1Shape to be handled together
   * with pulse struct ExcPulse1. Only if a double array parameter 
   * is provided as shape, the pulse supports calculated shapes.
   */
  
  STB_InitRFPulse("ExcPulse1",      // name of pulse structure
                  0,                // name of pulse list parameter - not defined
                  0,                // name of pulse amplitude parameter - not defined
                  "ExcPulse1Shape", // name of pulse shape (for calc. pulses)
                  0,                // used for excitation
                  "gauss.exc",      // default shape
                  3000.0,           // default bandwidth
                  30.0);            // default pulse angle

  // check valid range for this specific pulse see parsRelations.c
  ExcPulse1Range();
  
  /* initialize duration of phase encoding gradient             */
  EncGradDurRange();

  /* init frequency encoding parameters                         */
  EffSWhRange();

  /* init encoding parameters                                   */
  STB_InitEncoding( 2, dimRange, lowMat, upMat, defaultMat);

  /* init geometry parameters                                   */
  STB_InitImageGeometry();

  /* initialize standard imaging parameters NA TR TE DS         */
  RepetitionTimeRange();
  AveragesRange();
  EchoTimeRange();
  if(ParxRelsParHasValue("NDummyScans") == No)
    NDummyScans = 0;
  if(ParxRelsParHasValue("PVM_DeriveGains") == No)
    PVM_DeriveGains = Yes;
 
  /* init read spoiler parameters                               */
  ReadSpoilGradDurRange();
  if(!ParxRelsParHasValue("ReadSpoilGrad"))
    ReadSpoilGrad = 40.0;
 
  /* Visibility of parameters                                   */
  ParxRelsMakeNonEditable("PVM_EchoPosition,"
                          "PVM_AcquisitionTime,"
                          "PVM_MinEchoTime");
 
  /* Once all parameters have initial values, the backbone is 
   * called to assure they are consistent                       */
  
  backbone();
 
  DB_MSG(( "<-- initMeth" ));
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/


