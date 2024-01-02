/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2008
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/
static const char resid[] = "$Id$ (C) 2008 Bruker BioSpin MRI GmbH";

#define DEBUG		    0
#define DB_MODULE	  0
#define DB_LINE_NR	0

/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/
#include "method.h"

/****************************************************************/
/*		L O C A L   F U N C T I O N S			*/
/****************************************************************/

void initMeth()
{
  DB_MSG(("-->initMeth"));    
  /*
   * Set Version number of current method
   */
  

  /*
   *  initialization of PVM_Nuc1
   */

  STB_InitNuclei(1);

  /* initialize repetition time*/
  if(ParxRelsParHasValue("PVM_RepetitionTime") == No)
    PVM_RepetitionTime = 2000.0;   

  /*
   * initialize Neo and Decoupling parameter group
   */
  STB_NoeOnOffRange();
  STB_DecOnOffRange();
  
  STB_InitDecModule();
  STB_InitNoeModule();

  /*
   * init magnetization transfer module
   */
  STB_InitSatTransModule();

  /*
   * init the local adjustments available
   */

  LocalDecNoeHandling();

  /*
   * init the trigger module
   */
  PVM_TriggerMode = per_Slice;
  STB_InitTriggerModule();
  ParxRelsMakeNonEditable("PVM_TriggerMode");

  /*
   * init the spectroscopic dataAcquisition
   */    
  STB_InitSpectroscopy( 1, 1, 1, PVM_Nucleus1, 200.0, 125000.0 );
  
  /* Initialize multi receiver parameters */
  
  STB_InitEncoding();


  /*
   * Init of RF Pulses
   */
  
  STB_InitRFPulse("InvPul1",      // name of pulse structure
                  "InvPul1Enum",  // name of pulse list parameter
                  "InvPul1Ampl",  // name of pulse amplitude parameter
                  "InvPul1Shape", // name of pulse shape (for calc. pulses)
                  2,                // used for excitation
                  "Calculated",     // default shape
                  3000.0,           // default bandwidth
                  180.0);            // default pulse angle

  // check valid range for this specific pulse see parsRelations.c
  InvPul1Range();

  STB_InitRFPulse("ExcPul1",      // name of pulse structure
                  "ExcPul1Enum",  // name of pulse list parameter
                  "ExcPul1Ampl",  // name of pulse amplitude parameter
                  "ExcPul1Shape", // name of pulse shape (for calc. pulses)
                  0,                // used for excitation
                  "bp32",            // default shape
                  6000.0,           // default bandwidth
                  90.0);            // default pulse angle
  ExcPul1Range();
  
  DeriveGainsRange();


  /* Show/Hide of ISIS Parameters */  
  ParxRelsMakeNonEditable("IsisModDur");
  ParxRelsMakeNonEditable("IsisNAverages");
  PARX_change_dims("IsisSpatialDispl",3);
  ParxRelsMakeNonEditable("IsisSpatialDispl");
  ParxRelsMakeNonEditable("IsisInvBandwidthPpm");
  ParxRelsMakeNonEditable("IsisSpecExcBandwidthPpm");

  /* Initialisation of nucleus and frequency */  
  STB_InitNuclei(1);
  
  UpdateRFPulses();

  /* 
   *  setup for a spectroscopy method
   */
   
  STB_InitVoxelGeometry(1);
  
  /* 
   * init the Outer Volume suppression 
   * with 100% maximum gradient strength 
   */
  STB_InitOvsModule( 100.0, PVM_VoxMinDistance );
    
  /* 
   * init the ws modules without application of RF pulses
   */

  STB_InitWsModule (PVM_NVoxels);
  /* 
   * init spoiler parameters
   */

  if(ParxRelsParHasValue("IsisSpoilDur") == No)
  	IsisSpoilDur = 2.0;
  if(ParxRelsParHasValue("IsisSpoilPerc1") == No)
  	IsisSpoilPerc1 = 0.0;
  if(ParxRelsParHasValue("IsisSpoilPerc2") == No)
  	IsisSpoilPerc2 = 0.0;
  if(ParxRelsParHasValue("IsisSpoilPerc3") == No)
  	IsisSpoilPerc3 = 0.0;
  if(ParxRelsParHasValue("IsisSpoilCyMm1") == No)
  	IsisSpoilCyMm1 = 5.0;
  if(ParxRelsParHasValue("IsisSpoilCyMm2") == No)
  	IsisSpoilCyMm2 = 5.0;
  if(ParxRelsParHasValue("IsisSpoilCyMm3") == No)
  	IsisSpoilCyMm3 = 5.0;



  /* 
   * init preparation parameters
   */
  if(ParxRelsParHasValue("IsisAdjInvPulses") == No)	 
    IsisAdjInvPulses = No;	 
  if(ParxRelsParHasValue("IsisAdjOvsPulses") == No)	 
    IsisAdjOvsPulses = No;

  NumOvsCyclesRange();

  STB_InitDummyScans(0.0);

  /* 
   * init optimization parameters
   */
  IsisManAdjustmentRange();
  IsisFovRange();
  PARX_change_dims("PVM_Matrix", 1);
  PVM_Matrix[0] = 128;
  if(!ParxRelsParHasValue("PVM_EffSWh")) //swh for ManAdj
    PVM_EffSWh = 50000.0;

  AveragesRange();

  if(ParxRelsParHasValue("PVM_NRepetitions") == No)
    PVM_NRepetitions = 1;

  STB_InitAtsReferencePosition();
  STB_InitMapShim();


  backbone();
  DB_MSG(("<--initMeth"));    
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
