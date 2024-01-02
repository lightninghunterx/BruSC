/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002 -2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2002 Bruker BioSpin MRI GmbH";

#define DEBUG 0


/****************************************************************/
/****************************************************************/
/*		I N T E R F A C E   S E C T I O N		*/
/****************************************************************/
/****************************************************************/

/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "method.h"

/****************************************************************/
/*	I M P L E M E N T A T I O N   S E C T I O N		*/
/****************************************************************/


/****************************************************************/
/*		G L O B A L   F U N C T I O N S			*/
/****************************************************************/


/* ------------------------------------------------------------
  backbone
  The main part of method code. The consitency of all parameters is checked
  chere, relations between them are resolved and, finally, functions setting
  the base level parameters are called.
  --------------------------------------------------------------*/
void backbone( void )
{
  DB_MSG(("--> backbone()"));

  /* Nucleus and  PVM_GradCalConst
     are handled by this funtion: */
  STB_UpdateNuclei(Yes);

  /* handle RF pulse */
  UpdateRFPulses();

  PVM_NEchoImages = NEchoes;

  /* Spoiler */
  if(SpoilerOnOff == Off)
  {
    ParxRelsHideInEditor("Spoiler_duration");
    ParxRelsHideInFile("Spoiler_duration");
    ParxRelsHideInEditor("Spoiler_strength");
    ParxRelsHideInFile("Spoiler_strength");
  }
  else
  {
    ParxRelsShowInEditor("Spoiler_duration");
    ParxRelsShowInFile("Spoiler_duration");
    ParxRelsShowInEditor("Spoiler_strength");
    ParxRelsShowInFile("Spoiler_strength");
  }

  Spoiler_duration = MAX_OF(MIN_OF(Spoiler_duration,15.0),2*CFG_GradientRiseTime());
  Spoiler_strength = MAX_OF(MIN_OF(Spoiler_strength,90),0);

  /* ------------- spectroscopy part ----------------------- */
  STB_UpdateSpectroscopy( PVM_Nucleus1 );

  /* update Encoding pargroup for multi receiver experiment */
  STB_UpdateEncoding();

  /* echo time and repetition time */
  timingRels();

  /* handling of modules */
  STB_UpdateTriggerModule();
  PVM_TriggerMode = per_PhaseStep;
  STB_UpdateDummyScans(PVM_RepetitionTime);

  /* set up adjustment list */
  SetAdjustments();

  /* set baselevel acquisition parameter */
  SetBaseLevelParam();

  /* set baselevel reconstruction parameter */
  SetRecoParam();

  DB_MSG(("<-- backbone()"));
}


/****************************************************************/
/*	         L O C A L   F U N C T I O N S			*/
/****************************************************************/



void timingRels( void )
{
  DB_MSG(("--> timingRels\n"));

  double SpoilingTime = (SpoilerOnOff == Off) ? 0.0 : Spoiler_duration;

  /* minimum time between from middle of exc. pulse to middle of ref. pulse */
  MinTE1 = ExcPulse1.Length/2          +
           0.01                        +
           SpoilingTime                +
           RefPulse1.Length/2;

  /* minimum time between from middle of ref. pulse to middle adc */
  MinTE2 = RefPulse1.Length/2          +
           SpoilingTime                +
           0.03                        +    //10u + 20u (minimum D3)
           PVM_SpecAcquisitionTime / 2;


  PVM_MinEchoTime = MAX_OF(MinTE1, MinTE2);
  EchoSpacing = MAX_OF(2 * PVM_MinEchoTime, EchoSpacing);


  PVM_MinRepetitionTime =
    0.03                          + /* UpdateDynPars */
    ExcPulse1.Length / 2          +
    NEchoes * EchoSpacing         +
    PVM_SpecAcquisitionTime / 2   +
    EchoSpacing / 2 - MinTE2      +
    0.03;

  PVM_RepetitionTime = MAX_OF(PVM_RepetitionTime, PVM_MinRepetitionTime);

  /** Calculate Total Scan Time and Set for Scan Editor **/

  int dim = MRT_GetSpecDim();
  double TotalTime = PVM_RepetitionTime*PVM_NAverages;
  for(int i = 1; i < dim; i++)
     TotalTime *= PVM_SpecMatrix[i];

  TotalTime *= PVM_NRepetitions;

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);


  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<-- timingRels"));
}

/*--------------------------------------------------------
 * Routine to update RF pulse parameters
 *-------------------------------------------------------*/
void UpdateRFPulses()
{
  DB_MSG(("--> UpdateRFPulses"));

  /* Excitation */
  STB_UpdateRFPulse("ExcPulse1",1,PVM_DeriveGains,Conventional);
  STB_UpdateRFPulse("RefPulse1",1,PVM_DeriveGains,Conventional);

  if(PVM_DeriveGains==Yes)
  {
    ParxRelsHideInEditor("ExcPulse1Ampl,RefPulse1Ampl");
  }
  else
  {
    ParxRelsShowInEditor("ExcPulse1Ampl,RefPulse1Ampl");
  }

  ParxRelsShowInFile("ExcPulse1Ampl,RefPulse1Ampl");

  DB_MSG(("<-- UpdateRFPulses"));

  return;
}

void SetAdjustments(void )
{
  DB_MSG(("-->SetAdjustments"));

  PTB_ClearAdjustments();
  STB_ArrayPhaseAppendAdjustment(PVM_EncNReceivers, No);
  PTB_AppendOrderAdjustment(per_scan, per_scan, RG_ADJNAME);

  DB_MSG(("<--SetAdjustments"));

}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
