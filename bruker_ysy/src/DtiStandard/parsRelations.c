/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2003-2007
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/


static const char resid[] = "$Id$ (C) 2003-2007 Bruker BioSpin MRI GmbH";

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

 ExcPulse1.Flipangle = MIN_OF(180.0,MAX_OF(dval,1.0));

 DB_MSG(("<--ExcPulseRange"));
}


/*--------------------------------------------------------
 *
 * single parameter range checkers and relations
 *
 *-------------------------------------------------------*/

void RepetitionTimeRange(void)
{

  DB_MSG(("-->RepetitionTimeRange\n"));

  if(ParxRelsParHasValue("PVM_RepetitionTime")==No)
  {
    PVM_RepetitionTime = 500.0;

  }
  else
  {
    PVM_RepetitionTime = MAX_OF(1e-3,PVM_RepetitionTime);
  }


  DB_MSG(("<--RepetitionTimeRange\n"));
  return;
}

void RepetitionTimeRel(void)
{
  DB_MSG(("-->RepetitionTimeRel\n"));

  RepetitionTimeRange();
  backbone();

  DB_MSG(("<--RepetitionTimeRel\n"));
  return;
}

void AveragesRange(void)
{

  DB_MSG(("-->AveragesRange\n"));

  if(ParxRelsParHasValue("PVM_NAverages") == No)
  {
    PVM_NAverages = 1;
  }

  if (PVM_NAverages > 1)
  {
    ParxRelsShowInEditor("PVM_MotionSupOnOff");
  }
  else
  {
    ParxRelsHideInEditor("PVM_MotionSupOnOff");
  }

  DB_MSG(("<--AveragesRange\n"));
  return;
}

void AveragesRel(void)
{
  DB_MSG(("-->AveragesRel\n"));

  AveragesRange();
  backbone();

  DB_MSG(("<--AveragesRel\n"));
  return;
}



void EchoTimeRange(void)
{

  DB_MSG(("-->EchoTimeRange\n"));

  if(ParxRelsParHasValue("PVM_EchoTime")==No)
  {
    PVM_EchoTime = 20.0;
  }
  else
  {
    PVM_EchoTime = MAX_OF(1e-3,PVM_EchoTime);
  }


  DB_MSG(("<--EchoTimeRange\n"));
  return;
}

void EchoTimeRel(void)
{
  DB_MSG(("-->EchoTimeRel\n"));

  EchoTimeRange();
  backbone();

  DB_MSG(("<--EchoTimeRel\n"));
  return;
}



void repetitionsRange(void)
{
  DB_MSG(("-->repetitionsRange"));

  if(!ParxRelsParHasValue("PVM_NRepetitions"))
  {
    PVM_NRepetitions = 1;
  }
  else
  {
    PVM_NRepetitions=MAX_OF(1,PVM_NRepetitions);
  }

  DB_MSG(("<--repetitionsRange"));

}

void repetitionsRel(void)
{
  DB_MSG(("-->repetitionsRel"));
  repetitionsRange();
  backbone();
  DB_MSG(("<--repetitionsRel"));

}


void EncGradDurRange(void)
{
  DB_MSG(("-->EncGradDurRange"));

  double min = 2*CFG_GradientRiseTime() +
               CFG_InterGradientWaitTime();

  if(ParxRelsParHasValue("EncGradDur")==No)
  {
    EncGradDur = MAX_OF(min,0.8);
  }
  else
  {
    EncGradDur = MAX_OF(min,EncGradDur);
  }

  DB_MSG(("<--EncGradDurRange"));
}


void EncGradDurRel(void)
{
  DB_MSG(("-->EncGradDurRel"));

  EncGradDurRange();
  backbone();

  DB_MSG(("<--EncGradDurRel"));
}

void ExcSliceRephTimeRange(void)
{
  double min;
  DB_MSG(("-->ExcSliceRephTimeRange\n"));

  min = 2*CFG_GradientRiseTime() +
        CFG_InterGradientWaitTime();

  if(ParxRelsParHasValue("ExcSliceRephTime")==No)
  {
    ExcSliceRephTime = MAX_OF(1.5,min);
  }
  else
  {
    ExcSliceRephTime = MAX_OF(ExcSliceRephTime,min);
  }


  DB_MSG(("<--ExcSliceRephTimeRange\n"));
  return;
}

void ExcSliceRephTimeRel(void)
{
  DB_MSG(("-->ExcSliceRephTimeRel\n"));

  ExcSliceRephTimeRange();
  backbone();

  DB_MSG(("<--ExcSliceRephTimeRel\n"));
  return;
}

/* rangechecking and redirected relations of PVM_EffSWh */

void EffSWhRange(void)
{
  DB_MSG(("-->EffSWhRange"));

  double minmax[2] = {2000, 1000000};

  if(!ParxRelsParHasValue("PVM_EffSWh")) {
    PVM_EffSWh = 50000;
  }

  MRT_GetNearestBandwidthInHz("PVM_EffSWh", minmax);

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


void handleRgAdjustment(void)
{
  DB_MSG(("-->handleRgAdjustment"));

  if (Yes==STB_AtsAdj()) {
      return;
  }

  if(Yes == PTB_AdjMethSpec() &&
     0 == strcmp(PTB_GetAdjName(), RG_ADJNAME))
  {
    PVM_DwMeasMode = 0;
    PVM_DwAoImages = 0;
    PVM_DwNDiffExpEach = 1;
    PVM_DwNDiffDir=1;
    PVM_DwBvalEach[0] = 0.0;
    backbone();
    ParxRelsParRelations("PVM_AdjHandler",Yes);
  }


  DB_MSG(("<--handleRgAdjustment"));
  return;
}


/* relations of TensorReconstruction parameters */

void RecoMethModeRel(void)
{
  DB_MSG(("-->RecoMethModeRel\n"));
  
  RecoMethModeRange();
  
  if(RecoMethMode==Default)
    ParxRelsHideInEditor("TraceWeight");
  else
    ParxRelsShowInEditor("TraceWeight");
  
  DB_MSG(("<--RecoMethModeRel\n"));  
}

void RecoMethModeRange(void)
{
  DB_MSG(("-->RecoMethModeRange\n")); 
    
  if(!ParxRelsParHasValue("TraceWeight"))
    TraceWeight = 1000.0;
  else
    TraceWeight = MAX_OF(0.0,TraceWeight);
  
  if(!ParxRelsParHasValue("RecoMethMode"))
    RecoMethMode=Default;
  
  // Require at least 7 diffusion experiments, from which 6 or more have different
  // diffusion directions (at least nominally).
  if((!ParxRelsParHasValue("PVM_DwAoImages") || !ParxRelsParHasValue("PVM_DwNDiffDir") ||
      PVM_DwNDiffDir + PVM_DwAoImages < 7 || PVM_DwNDiffDir < 6))
  {
    RecoMethMode = Default;
    ParxRelsMakeNonEditable("RecoMethMode");
  }
  else
  {
    ParxRelsMakeEditable("RecoMethMode");
  }

  DB_MSG(("<--RecoMethModeRange\n"));
}
