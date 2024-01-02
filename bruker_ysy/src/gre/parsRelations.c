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

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0



#include "method.h"

/* =============================================================
 * Default relations and range checking functions of parameters
 * that appear editable in the method editor
 * ============================================================= */

/* ============================================================
 * Globally defined parameters: The relation has been redirected 
 * (see callbackDefs.h)
 * ============================================================ */

/* ------------------------------------------------------------
 * Repetition Time TR
 * ------------------------------------------------------------ */

void RepetitionTimeRange(void)
{  
  DB_MSG(("-->RepetitionTimeRange"));

  if(ParxRelsParHasValue("PVM_RepetitionTime")==No)
  {
    PVM_RepetitionTime = 100.0;
  }
  else
  {
    PVM_RepetitionTime = MAX_OF(1e-3,PVM_RepetitionTime);
  }
  
  DB_MSG(("<--RepetitionTimeRange"));
}

void RepetitionTimeRel(void)
{
  DB_MSG(("-->RepetitionTimeRel"));

  RepetitionTimeRange();
  backbone();

  DB_MSG(("<--RepetitionTimeRel"));
}

/* -------------------------------------------------------------
 * Averages NA
 * ------------------------------------------------------------*/

void AveragesRange(void)
{ 
  DB_MSG(("-->AveragesRange"));

  if(ParxRelsParHasValue("PVM_NAverages")==No)
  {
    PVM_NAverages = 1;
  }
  else
  {
    PVM_NAverages = MAX_OF(1,PVM_NAverages);
  }
 
  DB_MSG(("<--AveragesRange"));
}

void AveragesRel(void)
{
  DB_MSG(("-->AveragesRel"));

  AveragesRange();
  backbone();

  DB_MSG(("<--AveragesRel"));
}

/* ------------------------------------------------------------
 * Echo Time TE
 * -----------------------------------------------------------*/
 
void EchoTimeRange(void)
{  
  DB_MSG(("-->EchoTimeRange"));

  if(ParxRelsParHasValue("PVM_EchoTime")==No)
  {
    PVM_EchoTime = 6.0;
  }
  else
  {
    PVM_EchoTime = MAX_OF(1e-3,PVM_EchoTime);
  }
  
  DB_MSG(("<--EchoTimeRange"));
}

void EchoTimeRel(void)
{
  DB_MSG(("-->EchoTimeRel"));

  EchoTimeRange();
  backbone();

  DB_MSG(("<--EchoTimeRel"));
}


/* -------------------------------------------------------------
 * Bandwidth of data sampling PVM_EffSWh
 * ------------------------------------------------------------*/

void EffSWhRange(void)
{
  DB_MSG(("-->EffSWhRange"));
  
  double minmax[2]={2000,1000000};

  if(!ParxRelsParHasValue("PVM_EffSWh"))
    PVM_EffSWh=50000;

  MRT_GetNearestBandwidthInHz("PVM_EffSWh",minmax);

  DB_MSG(("-->EffSWhRange"));
}

void EffSWhRel(void)
{
  DB_MSG(("-->EffSWhRel"));

  EffSWhRange();
  backbone();

  DB_MSG(("<--EffSWhRel"));
}


/* =============================================================
 * Locally (i.e. in parsDefinition.h) defined parameters
 * ============================================================= */

/* -------------------------------------------------------------
 * Duration of Encoding Gradient
 * ------------------------------------------------------------- */

void EncGradDurRange(void)
{
  DB_MSG(("-->EncGradDurRange"));
  
  double min = 2*CFG_GradientRiseTime();

  if(!ParxRelsParHasValue("EncGradDur"))
  {
    EncGradDur=MAX_OF(2.0,min);
  }
  else
  {
    EncGradDur=MAX_OF(EncGradDur,min);
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


/* -----------------------------------------------------------
 * Duration of read spoiling gradient
 * ----------------------------------------------------------*/

void ReadSpoilGradDurRange(void)
{
  DB_MSG(("-->ReadSpoilGradDurRange"));
  
  double min = 2.0*CFG_GradientRiseTime();

  if (!ParxRelsParHasValue("ReadSpoilGradDur"))
  {
    ReadSpoilGradDur = MAX_OF(min,1.0);
  }
  else
  {
    ReadSpoilGradDur = MAX_OF(ReadSpoilGradDur,min);
  }

  DB_MSG(("<--ReadSpoilGradDurRange"));
}

void ReadSpoilGradDurRel(void)
{
  DB_MSG(("-->ReadSpoilGradDurRel"));

  ReadSpoilGradDurRange();
  backbone();

  DB_MSG(("<--ReadSpoilGradDurRel"));
}


/* ===================================================================
 * Relation of ExcPulse1
 * 
 * All pulses of type PVM_RF_PULSE must have relations like this.
 *
 * IMPORTANT: this function should not be invoked in the backbone!
 ====================================================================*/

void ExcPulse1Relation(void)
{
  DB_MSG(("-->ExcPulse1Relation"));
    
  /* Check the values of ExcPulse1 */
  
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

 DB_MSG(("<--ExcPulse1Range")); 
}
