/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2002 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0


#include "method.h"




/*===============================================================
 * ExcPulse1EnumRelation
 * Relation of ExcPulse1Enum (a dynamic enmueration parameter which
 * allows to select one of the existing  pulses)
 *===============================================================*/

void ExcPulse1EnumRelation(void)
{
  DB_MSG(("-->ExcPulse1EnumRelation"));

  UT_SetRequest("ExcPulse1Enum");
  backbone();
  
  DB_MSG(("<--ExcPulse1EnumRelation"));                                       
}

/*===============================================================
 * ExcPulse1AmplRel
 * Relation of ExcPulseAmpl 
 * This parameter is used in the setup parameter card to adjust
 * the RF pulse amplitude manually
 *===============================================================*/

void ExcPulse1AmplRel(void)
{
  DB_MSG(("-->ExcPulse1AmplRel"));
  UT_SetRequest("ExcPulse1Ampl");
  HandleRFPulseAmplitude();
  DB_MSG(("-->ExcPulse1AmplRel"));
}

void HandleRFPulseAmplitude(void)
{
  DB_MSG(("-->HandleRFPulseAmplitude"));

  STB_UpdateRFShapeAmplitude("ExcPulse1Ampl",No);
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]");

  DB_MSG(("<--HandleRFPulseAmplitude"));
}


/* ===================================================================
 * Relation of ExcPulse
 * 
 * All pulses of type PVM_RF_PULSE must have relations like this.
 * However, if you clone this funtion for a different pulse parameter
 * remember to replace the param name in the call to UT_SetRequest!
 *
 * IMPORTANT: this function should not be invoked in the backbone!
 ====================================================================*/

void ExcPulse1Relation(void)
{
  DB_MSG(("-->ExcPulse1Relation"));
  
  /*
   * Tell the request handling system that the parameter
   * ExcPulse has been edited 
   */
  
  UT_SetRequest("ExcPulse1");
  
  /* Check the values of ExcPulse */
  
  ExcPulse1Range();
  
  /* 
   * call the backbone; further handling will take place there
   * (by means of STB_UpdateRFPulse)  
   */
  
  backbone();
  
  DB_MSG(("<--ExcPulse1Relation"));
}



void ExcPulse1Range(void)
{
  DB_MSG(("-->ExcPulse1Range"));
  
  // range checker fields to be controlled may be
  // .Length  
  // .Bandwidth
  // .Flipangle
  // .Calculated
  // .Sharpness
  // .Flatness   
 double dval=ExcPulse1.Flipangle;
  
 ExcPulse1.Flipangle = MIN_OF(90.0,MAX_OF(dval,1.0));

 DB_MSG(("<--ExcPulseRange")); 
}

/* rangechecking and redirected relations of PVM_EffSWh */

void EffSWhRange(void)
{
  DB_MSG(("-->EffSWhRange"));
  double minmax[2]={2000,1000000};

  if(!ParxRelsParHasValue("PVM_EffSWh"))
    PVM_EffSWh=100000;

  MRT_GetNearestBandwidthInHz("PVM_EffSWh",minmax);

  DB_MSG(("-->EffSWhRange"));
  return;
}

void EffSWhRel(void)
{
  DB_MSG(("-->EffSWhRel"));

  EffSWhRange();
  backbone();

  DB_MSG(("-->EffSWhRel"));
  return;
}

/* relations of read/slice spoiler */
void ReadSpoilerRel(void)
{
  DB_MSG(("-->ReadSpoilerRel"));
  UT_SetRequest("ReadSpoiler");
  backbone();
  DB_MSG(("<--ReadSpoilerRel"));
}

void SliceSpoilerRel(void)
{
  DB_MSG(("-->SliceSpoilerRel"));
  UT_SetRequest("SliceSpoiler");
  backbone();
  DB_MSG(("<--SliceSpoilerRel"));
}

void RecoMethModeVisPar(void)
{
  DB_MSG(("-->RecoMethModeVisPar"));

  if(TimeGridding ==Yes)
    ParxRelsShowInEditor("GaussWidth");
  else
    ParxRelsHideInEditor("GaussWidth");

  if(RespGate == Yes)
    AnalysisOfMotion = OnlyCardiac; 

  const char *const heartList = "HeartRate,CardiacPeriod,FreezePhase0,FreezePhaseRange0,HeartRateRecoResult";
  const char *const respList  = "RespRate,RespPeriod,FreezePhase,FreezePhaseRange,RespRateRecoResult"; 
  ParxRelsShowInEditor(heartList);
  ParxRelsShowInEditor(respList);

  switch(AnalysisOfMotion)
  {
  case 0:
    break;
  case 1:
    MovieType=Cardiac;
    ParxRelsHideInEditor(respList);
    break; 
  case 2:
    MovieType=Respiration;
    ParxRelsHideInEditor(heartList);
    break; 
  }
  
  const char *const parList = \
    "OutputMovieFrames,MovieType,HeartRate,RespRate,FreezePhase0,FreezePhaseRange0,"
    "FreezePhase,FreezePhaseRange,TimeGridding,GaussWidth,AnalysisOfMotion";

  if(SelfGating == No)
    ParxRelsMakeNonEditable(parList);
  else
    ParxRelsMakeEditable(parList);

  DB_MSG(("<--RecoMethModeVisPar"));
}

void MovieFramesRelations(void)
{
    if (OutputMovieFrames < 1)
        OutputMovieFrames = 1;
    else if (OutputMovieFrames > Oversampling)
        OutputMovieFrames = Oversampling;
    SetRecoParam ();

}

void MyRgInitRel(void)
{
  DB_MSG(("-->MyRgInitRel"));

  if(NavSigSetupYesNo==Yes)
    NavSigSetupYesNo=No;
  
  // deactivate online trigger decision for RG adjustment
  if(RespGate==Yes)
    RespGate=No;  
  
  if(AngioMode==Yes)
  {
    /* initialize 1 slice per package, upadte without angio mode
       for RG adjustment
    */
    int packs=PVM_NSPacks;
    int *ppersl=PVM_SPackArrNSlices;
    for(int i=0; i<packs;i++)
      ppersl[i]=1;
    AngioMode=No;
  }

  backbone();
 
  PpgTTLOut = No; 

  ParxRelsParRelations("PVM_AutoRgInitHandler",Yes);
  DB_MSG(("<--MyRgInitRel"));
}

void SetBeforeAcquisition(void)
{
  DB_MSG(("-->SetBeforeAcquisition"));
  
  //redirecting PVM_ScanHandler to local method requires to call
  //modality-specific start-settings!
  ParxRelsParRelations("PVM_AcqScanHandler", MODALITY_HIERARCHY);

  if((PVM_AcqScanHandler == SETUP_EXPERIMENT) && (NavSigSetupYesNo==Yes))
  {
    int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
    ACQ_jobs[0].transactionBlocks = nSlices * TransActionBlGSP;
    ACQ_jobs[1].transactionBlocks = nSlices * TransActionBlGSP;
    ACQ_SetupPipeJobSettings[0].nAccumScans = nSlices * TransActionBlGSP;
    ACQ_SetupPipeJobSettings[1].nAccumScans = nSlices * TransActionBlGSP;
  }

  if((PVM_AcqScanHandler == SCAN_EXPERIMENT) && (IgTTLOut == Yes))
    PpgTTLOut = Yes;
  else
    PpgTTLOut = No;

  DB_MSG(("<--SetBeforeAcquisition"));
}
/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/








