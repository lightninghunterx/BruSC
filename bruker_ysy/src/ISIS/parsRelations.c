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

#include "method.h"

/*
 * Local parameter relations
 */

void AveragesRange(void)
{
  DB_MSG(("-->AveragesRange"));

  if(!ParxRelsParHasValue("PVM_NAverages"))
  {
    PVM_NAverages=1;
  }
  else
  {
    PVM_NAverages=MAX_OF(1,PVM_NAverages);
  }

  DB_MSG(("<--AveragesRange"));
}

void AveragesRel(void)
{
  DB_MSG(("-->AveragesRel"));

  AveragesRange();
  backbone();
  DB_MSG(("-->AveragesRel"));
  return;
}


/* RF Pulses*/

void InvPul1EnumRelation(void)
{
  DB_MSG(("-->InvPul1EnumRelation"));

  UT_SetRequest("InvPul1Enum");
  backbone();

  DB_MSG(("<--InvPul1EnumRelation"));
}

void ExcPul1EnumRelation(void)
{
  DB_MSG(("-->ExcPul1EnumRelation"));

  UT_SetRequest("ExcPul1Enum");
  backbone();

  DB_MSG(("<--ExcPul1EnumRelation"));
}

void InvPul1AmplRel(void)
{
  DB_MSG(("-->InvPul1AmplRel"));
  UT_SetRequest("InvPul1Ampl");
  HandlePulsesAmplitude();
  DB_MSG(("-->InvPul1AmplRel"));
}

void ExcPul1AmplRel(void)
{
  DB_MSG(("-->ExcPul1AmplRel"));
  UT_SetRequest("ExcPul1Ampl");
  HandlePulsesAmplitude();
  DB_MSG(("-->ExcPul1AmplRel"));
}
void HandlePulsesAmplitude(void)
{
  DB_MSG(("-->HandlePulsesAmplitude"));
  STB_UpdateRFShapeAmplitude("InvPul1Ampl,ExcPul1Ampl",No);
  ATB_SetRFPulse("InvPul1","ACQ_RfShapes[1]");
  ATB_SetRFPulse("ExcPul1","ACQ_RfShapes[0]");
  DB_MSG(("<--HandlePulsesAmplitude"));
}

void InvPul1Relation(void)
{
  DB_MSG(("-->InvPul1Relation"));
  UT_SetRequest("InvPul1");
  InvPul1Range();
  backbone();
  DB_MSG(("<--InvPul1Relation"));
}

void ExcPul1Relation(void)
{
  DB_MSG(("-->ExcPul1Relation"));
  UT_SetRequest("ExcPul1");
  ExcPul1Range();
  backbone();
  DB_MSG(("<--ExcPul1Relation"));
}

void InvPul1Range(void)
{
  DB_MSG(("-->InvPul1Range"));
  double dval=InvPul1.Flipangle;
  InvPul1.Flipangle = MIN_OF(270.0,MAX_OF(dval,1.0));
  DB_MSG(("<--InvPul1Range"));
}

void ExcPul1Range(void)
{
  DB_MSG(("-->ExcPul1Range"));
  double dval=ExcPul1.Flipangle;
  ExcPul1.Flipangle = MIN_OF(180.0,MAX_OF(dval,1.0));
  DB_MSG(("<--ExcPul1Range"));
}


void DeriveGainsRel(void)
{
  DB_MSG(("-->DeriveGainsRel"));

  DeriveGainsRange();
  backbone();

  DB_MSG(("<--DeriveGainsRel"));
  return;
}

void DeriveGainsRange(void)
{
  DB_MSG(("-->DeriveGainsRange"));

  if(!ParxRelsParHasValue("PVM_DeriveGains"))
  {
    PVM_DeriveGains = Yes;
  }
  else
  {
    switch(PVM_DeriveGains)
    {
      default:
        PVM_DeriveGains=Yes;
      case Yes: case No:
        break;
    }
  }

  DB_MSG(("<--DeriveGainsRange"));
}

void IsisSpoilDurRels(void)
{
  DB_MSG(("-->IsisSpoilDurRels"));

  IsisSpoilDurRange();
  IsisSpoilPRels();
  backbone();

  DB_MSG(("<--IsisSpoilDurRels"));
}

void IsisSpoilDurRange(void)
{
  double rise;

  DB_MSG(("-->IsisSpoilDurRange"));

  rise = CFG_GradientRiseTime();
  IsisSpoilDur = MAX_OF(IsisSpoilDur,rise);
  IsisSpoilDur = MIN_OF(IsisSpoilDur,10.0);

  DB_MSG(("<--IsisSpoilDurRange"));
}

void IsisSpoilPRels(void)
{
  double gcc;

  DB_MSG(("-->IsisSpoilPRels"));

  IsisSpoilPerc1 = MAX_OF(IsisSpoilPerc1,0.0);
  IsisSpoilPerc1 = MIN_OF(IsisSpoilPerc1,57.0);
  IsisSpoilPerc2 = MAX_OF(IsisSpoilPerc2,0.0);
  IsisSpoilPerc2 = MIN_OF(IsisSpoilPerc2,57.0);
  IsisSpoilPerc3 = MAX_OF(IsisSpoilPerc3,0.0);
  IsisSpoilPerc3 = MIN_OF(IsisSpoilPerc3,57.0);

  gcc = CFG_GradCalConst(PVM_Nucleus1);

  IsisSpoilCyMm1 =
    gcc * IsisSpoilPerc1/100.0 * IsisSpoilDur/1000.0;
  IsisSpoilCyMm2 =
    gcc * IsisSpoilPerc2/100.0 * IsisSpoilDur/1000.0;
  IsisSpoilCyMm3 =
    gcc * IsisSpoilPerc3/100.0 * IsisSpoilDur/1000.0;

  backbone();

  DB_MSG(("<--IsisSpoilPRels"));
}

void IsisSpoilCRels(void)
{
    double maxspoil, gcc;

    DB_MSG(("-->IsisSpoilCRels"));

    gcc = CFG_GradCalConst(PVM_Nucleus1);
    maxspoil = gcc * 0.5 * IsisSpoilDur/1000.0;

    IsisSpoilCyMm1 = MAX_OF(IsisSpoilCyMm1,0.0);
    IsisSpoilCyMm1 = MIN_OF(IsisSpoilCyMm1,maxspoil);
    IsisSpoilCyMm2 = MAX_OF(IsisSpoilCyMm2,0.0);
    IsisSpoilCyMm2 = MIN_OF(IsisSpoilCyMm2,maxspoil);
    IsisSpoilCyMm3 = MAX_OF(IsisSpoilCyMm3,0.0);
    IsisSpoilCyMm3 = MIN_OF(IsisSpoilCyMm3,maxspoil);

    IsisSpoilPerc1 = (100.0 * 1000.0 * IsisSpoilCyMm1) /
			(IsisSpoilDur * gcc);
    IsisSpoilPerc2 = (100.0 * 1000.0 * IsisSpoilCyMm2) /
			(IsisSpoilDur * gcc);
    IsisSpoilPerc3 = (100.0 * 1000.0 * IsisSpoilCyMm3) /
			(IsisSpoilDur * gcc);

    backbone();

    DB_MSG(("<--IsisSpoilCRels"));
}


void IsisStabDurRels(void)
{
  DB_MSG(("-->IsisStabDurRels"));

	IsisStabDurRange();
	backbone();

  DB_MSG(("<--IsisStabDurRels"));
}

void IsisStabDurRange(void)
{

  DB_MSG(("-->IsisStabDurRange"));

  if(ParxRelsParHasValue("IsisStabDur") == No)
    IsisStabDur = 2.0;

	IsisStabDur = MAX_OF(IsisStabDur,0.01);
	IsisStabDur = MIN_OF(IsisStabDur,10.0);

  DB_MSG(("<--IsisStabDurRange"));
}


/* ------------------------------------------------------------
   relations of DeadTime
   -------------------------------------------------------------*/

void DeadTimeRels(void)
{
  DB_MSG(("-->DeadTimeRels"));

  DeadTimeRange();
  backbone();

  DB_MSG(("<--DeadTimeRels"));
  return;
}

void DeadTimeRange(void)
{
  DB_MSG(("-->DeadTimeRange"));

  if(ParxRelsParHasValue("DeadTime") == No)
    DeadTime = 0.05;

  /* The minimum delay between RF pulse and ACQ_START is given by 20us
   * which are included in the acqdec-subroutine before acquisition.
   * (these 20us include the DE delay).
   * 1us is added to avoid rounding problems -> min=21us */

  double min = 0.021;
  DeadTime = MAX_OF(DeadTime, min);
  DeadTime = MIN_OF(5.0, DeadTime);

  DB_MSG(("<--DeadTimeRange"));
  return;
}


/* -------------------------------------------------------------
   relations of IsisNumOvsCycles
   -------------------------------------------------------------*/

void NumOvsCyclesRels(void)
{
  DB_MSG(("-->NumOvsCyclesRels"));

  NumOvsCyclesRange();
  backbone();

  DB_MSG(("<--NumOvsCyclesRels"));
  return;
}

void NumOvsCyclesRange(void)
{
  DB_MSG(("-->NumOvsCyclesRange"));

  if(ParxRelsParHasValue("IsisNumOvsCycles") == No)
    IsisNumOvsCycles = 0;

  IsisNumOvsCycles = MAX_OF(1,IsisNumOvsCycles);
  IsisNumOvsCycles = MIN_OF(20,IsisNumOvsCycles);

  DB_MSG(("<--NumOvsCyclesRange"));
  return;
}


/* default relations and range checking for Optimization parameters */

void IsisManAdjustmentRange(void)
{
  DB_MSG(("-->IsisManAdjustmentRange"));

  if(!ParxRelsParHasValue("IsisManAdjustment"))
  {
    IsisManAdjustment = NoManAdjustment;
  }
  else
  {
    switch(IsisManAdjustment)
    {
      case InvPulse1:
      case InvPulse2:
      case InvPulse3:
      case OvsPulse1:
      case OvsPulse2:
      case OvsPulse3:
        break;
      default:
        IsisManAdjustment = NoManAdjustment;
      case NoManAdjustment:
        break;
    }
  }

  DB_MSG(("<--IsisManAdjustmentRange"));
  return;
}

void IsisManAdjustmentRels(void)
{
  DB_MSG(("-->IsisManAdjustmentRels"));

  IsisManAdjustmentRange();
  backbone();

  DB_MSG(("<--IsisManAdjustmentRels"));
  return;
}

void IsisFovRange(void)
{
  DB_MSG(("-->IsisFovRange"));

  if(!ParxRelsParHasValue("IsisFov"))
  {
    double defext[3];
    CFG_DefaultExtent(defext);
    IsisFov=defext[0];
  }
  else
  {
    /* constrain FOV to be posititve */
    IsisFov=MAX_OF(0.001,IsisFov);
  }

  DB_MSG(("-->IsisFovRange"));
  return;
}

void IsisFovRels(void)
{
  DB_MSG(("-->IsisFovRels"));

  IsisFovRange();
  backbone();

  DB_MSG(("<--IsisFovRels"));
}

void HandleAdjustmentRequests(void)
{
  YesNo adjPossible=No;
  DB_MSG(("-->HandleAdjustmentRequests"));

  if (Yes==STB_AtsAdj()) {
      return;
  }

  /* array phase adjustment */
  if (STB_ArrayPhaseHandleAdjustmentRequests() == 1)
  {
    /*Turn water suppression pulses off*/
    PVM_WsOnOff = Off;
    ParxRelsParRelations("PVM_WsOnOff",No);
    PVM_NAverages = 1;
    ParxRelsParRelations("PVM_NAverages",No);
    adjPossible = Yes;
  }

  if(No == PTB_AdjMethSpec())
  {
    DB_MSG(("<--HandleAdjustmentRequests: no method specific adjustment desired"));
    ParxRelsParRelations("PVM_AdjHandler",Yes);
    return;
  }

  if(Yes==PTB_AdjMethSpec() &&
     0 == strcmp(PTB_GetAdjName(), RG_ADJNAME))
  {
      PVM_DummyScans=0;
      ParxRelsParRelations("PVM_DummyScans",No);
      PVM_NAverages=1;
      ParxRelsParRelations("PVM_NAverages",No);
      ParxRelsParRelations("PVM_AdjHandler",Yes);
      adjPossible = Yes;
  }
  else
      return;

  if(adjPossible == Yes)
  {
    IsisManAdjustment =  NoManAdjustment;
    backbone();
  }
  else
  {
    PARX_sprintf("Unknown Adjustment required for method PRESS");
    /* make illegal adjustment request fail by setting ACQ_SetupAutoName to empty string */
    ACQ_SetupAutoName[0]='\0';
  }

  DB_MSG(("<--HandleAdjustmentRequests"));
  return;
}
