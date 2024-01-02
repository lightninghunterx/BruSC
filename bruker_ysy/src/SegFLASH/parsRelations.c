/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2020
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




/*==========================================================
 *
 *  examples for relations concearning special pulses and 
 *  pulselists
 *
 *==========================================================*/



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

/*===============================================================
 *
 * Range checking routine for parameter PVM_NAverages
 *
 *==============================================================*/


void NAveragesRange(void)
{
  DB_MSG(("-->NAveragesRange\n"));
  
  /* 
   *  Range check of PVM_NAverages: prevent it to be negative or 0
   */
  
  if(ParxRelsParHasValue("PVM_NAverages") == No)
  {
    PVM_NAverages = 1;
  }
  
  if (PVM_NAverages > 1)
  {
    //averaging possible only with NAE loop
    PVM_MotionSupOnOff = On;
    ParxRelsShowInEditor("PVM_MotionSupOnOff");
    ParxRelsMakeNonEditable("PVM_MotionSupOnOff");
  }
  else
  {
    ParxRelsHideInEditor("PVM_MotionSupOnOff");
  }

  DB_MSG(("<--NAveragesRange\n"));
}

void NAveragesRels(void)
{

  DB_MSG(("-->NAveragesRels\n"));


  NAveragesRange();

  /*
   *   Averages range check is finished, handle the request by
   *   the method:
   */
  
  backbone();
    
  DB_MSG(("<--NAveragesRels\n"));
  return;
}

void SegmentSliceOrderRels(void)
{
  DB_MSG(("-->SegmentSliceOrderRels\n"));
  
  backbone();
  DB_MSG(("<--SegmentSliceOrderRels\n"));
  return;
}
/*==================================================
 *     relation to update Echotime from scan editor
 *==================================================*/

void EffSWhRange(void)
{
  DB_MSG(("-->EffSWhRange"));
  double minmax[2]={2000,1000000};

  if(!ParxRelsParHasValue("PVM_EffSWh"))
    PVM_EffSWh=50000;

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

/*==================================================
 *     relations for Inversion Recovery
 *==================================================*/
void InversionTimeRel(void)
{
    DB_MSG(("-->InversionTimeRel"));

    InversionTimeRange();

    if(PVM_SelIrOnOff==On)
      PVM_SelIrInvTime = PVM_InversionTime;

    backbone();

    DB_MSG(("-->InversionTimeRel"));

}
/*==================================================
 *     relations for Inversion
 *==================================================*/
void InversionTimeRange(void)
{
    if(!ParxRelsParHasValue("PVM_InversionTime"))
       PVM_InversionTime = 0.0;

    PVM_InversionTime = MAX_OF(PVM_InversionTime,0.0);
}
/*==================================================
 *     relations for Trigger
 *==================================================*/
void AcqTriggerRel(void)
{
    if (AcqTrigger == No && InvTrigger == No)
      TriggerOnOff = Off;

    if (AcqTrigger == Yes)
      TriggerOnOff = On;

    backbone();
}

void InvTriggerRel(void)
{
    if (AcqTrigger == No && InvTrigger == No)
      TriggerOnOff = Off;

    if (InvTrigger == Yes)
      TriggerOnOff = On;

    backbone();
}

void TriggerOnOffRel(void)
{
  if (TriggerOnOff == Off)
  {
    AcqTrigger = No;
    InvTrigger = No;
  }
  else
  {
    AcqTrigger = Yes;
  }

  backbone();
}
/*==================================================
 *     relations for Read/Slice Spoiler
 *==================================================*/
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

void SegSpoilerRel(void)
{
  DB_MSG(("-->SegSpoilerRel"));
  UT_SetRequest("SegSpoiler");
  backbone();
  DB_MSG(("<--SegSpoilerRel"));
}

void RecoMethModeVisPar(void)
{
  DB_MSG(("-->RecoMethModeVisPar"));

  if(RecoMethMode==SWI)
  {
   ParxRelsShowInEditor("WeightingMode,GaussBroadening");
   if(WeightingMode==phase_image)
     ParxRelsHideInEditor("MaskWeighting");
   else
     ParxRelsShowInEditor("MaskWeighting"); 
  }
  else
    ParxRelsHideInEditor("WeightingMode,GaussBroadening,MaskWeighting");

  DB_MSG((">--RecoMethModeVisPar"));
}

void MaskModeRel(void)
{
  DB_MSG(("-->MaskModeRel"));
  
  if(WeightingMode==phase_image)
    ParxRelsHideInEditor("MaskWeighting");
  else
    ParxRelsShowInEditor("MaskWeighting"); 
  SetRecoParam();

  DB_MSG(("<--MaskModeRel"));
}

void GaussBroadRange(void)
{
  double max;

  DB_MSG(("-->GaussBroadRange"));

  if(ParxRelsParHasValue("GaussBroadening") == No)
    GaussBroadening=1.0;

  max=PTB_GetSpatDim()>1? MAX_OF(PVM_Fov[0],PVM_Fov[1]) : PVM_Fov[0];
  if(PTB_GetSpatDim()==3)
    max=MAX_OF(max,PVM_Fov[2]);
  
  GaussBroadening = MIN_OF(MAX_OF(0,GaussBroadening),max);

  DB_MSG(("<--GaussBroadRange"));
}

void MaskWeightRange(void)
{
  DB_MSG(("-->MaskWeightRange"));

  if(ParxRelsParHasValue("MaskWeighting") == No)
    MaskWeighting=4.0;

  MaskWeighting = MIN_OF(MAX_OF(0,MaskWeighting),20);

  DB_MSG(("<--MaskWeightRange"));
}

void MyRgInitRel(void)
{
  DB_MSG(("-->MyRgInitRel"));
    
  InversionOnOff = Off;

  SegmentSize=1;
  PVM_NMovieFrames = 1;

  if (SegmentSliceOrder == Serial)
  {
    SegmentSliceOrder = Alternate;
  }

  backbone();

  ParxRelsParRelations("PVM_AutoRgInitHandler",Yes);

  DB_MSG(("<--MyRgInitRel"));
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/



