/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2007
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id: ";


#define DEBUG    0
#define DB_MODULE  0
#define DB_LINE_NR  0

#include "method.h"



/*===========================================================
 *
 * RF PULSES
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
  HandleRFPulseAmplitudes();
  DB_MSG(("-->ExcPulse1AmplRel"));
}

void HandleRFPulseAmplitudes(void)
{
  DB_MSG(("-->HandleRFPulseAmplitude"));

  STB_UpdateRFShapeAmplitude("ExcPulse1Ampl,RefPulse1Ampl",No);
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]");
  ATB_SetRFPulse("RefPulse1","ACQ_RfShapes[1]");

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
  DB_MSG(("-->ExcPulse1Relation\n"));

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

  DB_MSG(("<--ExcPulseRelation\n"));
}



void ExcPulse1Range(void)
{
  DB_MSG(("-->ExcPulse1Range"));

  // fields to be controlled may be
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




/* --------------------------------------------------------------
   Relation of RefPulseEnum
   --------------------------------------------------------------*/

void RefPulse1EnumRelation(void)
{
  DB_MSG(("-->RefPulse1EnumRelation"));

  UT_SetRequest("RefPulse1Enum");
  backbone();

  DB_MSG(("<--RefPulse1EnumRelation"));
}

/* --------------------------------------------------------------
   Relation of RefPulse1Ampl
   --------------------------------------------------------------*/

void RefPulse1AmplRel(void)
{
  DB_MSG(("-->RefPulse1AmplRel"));
  UT_SetRequest("RefPulse1Ampl");
  HandleRFPulseAmplitudes();
  DB_MSG(("-->RefPulse1AmplRel"));
}


/* -----------------------------------------------------------
   Relation of RefPulse
   -----------------------------------------------------------*/
void RefPulse1Relation(void)
{
  DB_MSG(("-->RefPulse1Relation\n"));

  /*
   *  Tell the request handling system that the parameter
   *  RefPulse has been edited
   */

  UT_SetRequest("RefPulse1");

  /* Check the values of RefPulse1 */
  RefPulse1Range();

  /*
   * call the backbone; further handling will take place
   * there (by means of STB_UpdateRFPulse)
   */

  backbone();

  DB_MSG(("<--RefPulseRelation\n"));
}

void RefPulse1Range(void)
{
  DB_MSG(("-->RefPulse1Range"));

  double dval=RefPulse1.Flipangle;

  RefPulse1.Flipangle = MIN_OF(270,MAX_OF(dval,1));

  DB_MSG(("<--RefPulse1Range\n"));
}

/* =============================================================
 * relations of EffectiveTE (local parameter)
 * ============================================================ */

void localEchoRels(void)
{
  double denom;

  DB_MSG(("-->localEchoRels\n"));

  if(ParxRelsParHasValue("PVM_EncCentralStep1") && PVM_EncCentralStep1>=1)
    denom = PVM_EncCentralStep1;
  else
    denom = 1.0;

  if(denom)
    PVM_EchoTime = EffectiveTE[0]/denom;
  backbone();

  DB_MSG(("<--localEchoRels\n"));
}

/*===============================================================
 *
 * Range checking routine for parameter PVM_NAverages
 *
 *==============================================================*/


void Local_NAveragesRange(void)
{
  int ival;
  DB_MSG(("-->Local_NAveragesRange\n"));

  /*
   *  Range check of PVM_NAverages: prevent it to be negative or 0
   */

  if(ParxRelsParHasValue("PVM_NAverages") == No)
  {
    PVM_NAverages = 1;
  }

  ival = PVM_NAverages;
  PVM_NAverages = MAX_OF(ival,1);

  if (PVM_NAverages > 1)
  {
    ParxRelsShowInEditor("PVM_MotionSupOnOff");
  }
  else
  {
    ParxRelsHideInEditor("PVM_MotionSupOnOff");
  }

  DB_MSG(("<--Local_NAveragesRange\n"));

}



void Local_NAveragesHandler(void)
{

  DB_MSG(("-->Local_NAveragesRange\n"));


  Local_NAveragesRange();

  /*
   *   Averages range check is finished, handle the request by
   *   the method:
   */


  backbone();


  DB_MSG(("<--Local_NAveragesRange\n"));
  return;
}




/*===========================================================
 * The update of slice geometry is done in backbone.
 * This function connects the Fov in 3rd spatial dimension
 * to the slice thickness.
 *==========================================================*/

void localHandleSliceGeometry(void)
{
  DB_MSG(("-->localHandleSliceGeometry\n"));

  if(PTB_GetSpatDim()==3)
  {

    STB_IsotropicRange();

    switch(PVM_Isotropic)
    {
      case Isotropic_Fov:
      case Isotropic_All:
        PVM_Fov[0] =
        PVM_Fov[1] =
        PVM_Fov[2] = PVM_SliceThick;
        break;
      default:
        PVM_Fov[2] = PVM_SliceThick;
        break;
    }
  }

  backbone();


  DB_MSG(("<--localHandleSliceGeometry\n"));
  return;
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

void localInversionRel(void)
{
  DB_MSG(("-->localInversionRel"));
   if(!ParxRelsParHasValue("PVM_InversionTime"))
      PVM_InversionTime = 0.0;
   PVM_InversionTime = MAX_OF(PVM_InversionTime,0.0);
   PVM_SelIrInvTime = PVM_InversionTime;
   backbone();
  DB_MSG(("-->localInversionRel"));
}


void SliceBandWidthScaleRels(void)
{
  SliceBandWidthScaleRange();
  backbone();
}

void SliceBandWidthScaleRange(void)
{
   if(!ParxRelsParHasValue("SliceBandWidthScale"))
     SliceBandWidthScale = 100;
   else
   {
     SliceBandWidthScale = MAX_OF( SliceBandWidthScale, 50);
     SliceBandWidthScale = MIN_OF( SliceBandWidthScale, 150);
   }
}


void SpoilingReadPxRels(void)
{
  SpoilingReadPxRange();
  backbone();
}
void SpoilingReadPxRange(void)
{
  if(!ParxRelsParHasValue("SpoilingReadPx"))
    SpoilingReadPx = 1.0;
  else
  {
    SpoilingReadPx = MAX_OF(0,  SpoilingReadPx);
    SpoilingReadPx = MIN_OF(10, SpoilingReadPx);
  }
}

void SpoilingSliceSlRels(void)
{
  SpoilingSliceSlRange();
  backbone();
}
void SpoilingSliceSlRange(void)
{
  if(!ParxRelsParHasValue("SpoilingSliceSl"))
    SpoilingSliceSl = 2.0;
  else
  {
    SpoilingSliceSl = MAX_OF(0,  SpoilingSliceSl);
    SpoilingSliceSl = MIN_OF(10, SpoilingSliceSl);
  }
}

/* relation of repetition spoiler */
void RepetitionSpoilerRel(void)
{
  DB_MSG(("-->RepetitionSpoilerRel"));
  UT_SetRequest("RepetitionSpoiler");
  backbone();
  DB_MSG(("<--RepetitionSpoilerRel"));
}

void GradStabRels(void)
{
  GradStabRange();
  backbone();
}
void GradStabRange(void)
{
  if(!ParxRelsParHasValue("GradStab"))
    GradStab = 0.05;

  GradStab = MAX_OF(0,  GradStab);
  GradStab = MAX_OF(CFG_GradientDelayTime(),  GradStab);

  GradStab = MIN_OF(1.0, GradStab);

}

void SlewRatePerCentRels(void)
{
  SlewRatePerCentRange();
  backbone();
}

void SlewRatePerCentRange(void)
{
  if(!ParxRelsParHasValue("SlewRatePerCent"))
    SlewRatePerCent = 75;
  SlewRatePerCent = MAX_OF(SlewRatePerCent, 5.0);
  SlewRatePerCent = MIN_OF(SlewRatePerCent, 100.0);
}


void RampFormRels(void)
{
  RampFormRange();
  backbone();
}

void RampFormRange(void)
{
  if(!ParxRelsParHasValue("RampForm"))
    RampForm = LinearRamp;
}


void MyRgInitRel(void)
{

  DB_MSG(("-->MyRgInitRel"));

  /*
   * perform RG adjustment without inversion
   */

  PVM_FairMode = NONSELECTIVE;
  PVM_FairTIR_NExp = 1;
  backbone();
  ACQ_RfShapes[47].powerWatt = 0.0; /* inversion off */

  /* force default behaviour */
  ParxRelsParRelations("PVM_AutoRgInitHandler",Yes);


  DB_MSG(("<--MyRgInitRel"));
}
