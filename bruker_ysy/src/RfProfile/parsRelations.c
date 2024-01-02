/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002 -2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2002 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0


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


/****************************************************************/
/*	         L O C A L   F U N C T I O N S			*/
/****************************************************************/



void Local_NAveragesRange(void)
{
  int ival;
  DB_MSG(("Entering Local_NAveragesRange"));
  
  /* 
   *  Range check of PVM_NAverages: prevent it to be negative or 0
   */

  if(ParxRelsParHasValue("PVM_NAverages") == No)
    {
      PVM_NAverages = 1;
    }

  ival = PVM_NAverages;
  PVM_NAverages = MAX_OF(ival,1);
  
  DB_MSG(("Exiting Local_NAveragesRange"));

}


void Local_NAveragesHandler(void)
{

  DB_MSG(("Exiting Local_NAveragesHandler with value %d",PVM_NAverages));

  Local_NAveragesRange();

  /*
   *   Averages range check is finished, handle the request by
   *   the method:
   */

  
  backbone();


  DB_MSG(("Exiting Local_NAveragesHandler with value %d",PVM_NAverages));
  return;
}


void NrepRange(void)
{
  if(ParxRelsParHasValue("PVM_NRepetitions") ==No)
  {
    PVM_NRepetitions = 1;
  }    
  else
  {
     PVM_NRepetitions = MAX_OF(1,PVM_NRepetitions);
  }

}

void NrepRel(void)
{
  NrepRange();
  backbone();
}


void NdummyRange(void)
{
  if(!ParxRelsParHasValue("Ndummy"))
  {
    Ndummy = 0;
  }
  else
  {
    Ndummy = MAX_OF(0,Ndummy);
  }
}


void NdummyRel(void)
{
  NdummyRange();
  backbone();

}

void GradientDirectionRange(void)
{
  if(ParxRelsParHasValue("GradientDirection") == No)
    GradientDirection = Z_dir;

  switch (GradientDirection)  
  {
   case Z_dir:
   break;
   case X_dir:
   break;
   case Y_dir:
   break;
   default:
   GradientDirection = Z_dir;
   break;
  }

}

void GradientDirectionRel(void)
  {
   GradientDirectionRange();
   backbone();
  }



/* rangechecking and redirected relations of PVM_EffSWh */

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



void ProfileRel(void)
{
  DB_MSG(("-->ProfileRel"));

  if(!ParxRelsParHasValue("ProfileMode"))
    ProfileMode = Excitation_Profile;
  else
  {
    switch(ProfileMode)
    {
    default:
      ProfileMode=Excitation_Profile;
    case Saturation_Profile:
    case Excitation_Profile:
      ParxRelsMakeEditable("SPExamExc,SPExamExcEnum");
      ParxRelsMakeNonEditable("SPExamRfc,SPExamRfcEnum,SPExamInv,SPExamInvEnum");
      if(DeriveExaminedPulseGain==No)
      {
        ParxRelsMakeEditable("SPExamExcAmpl");
      }
      else
      {
        ParxRelsMakeNonEditable("SPExamExcAmpl");
      }
      ParxRelsMakeNonEditable("SPExamRfcAmpl,SPExamInvAmpl");
      break;
    case Refocusing_Profile:
      ParxRelsMakeEditable("SPExamRfc,SPExamRfcEnum");
      ParxRelsMakeNonEditable("SPExamExc,SPExamExcEnum,SPExamInv,SPExamInvEnum");
      if(DeriveExaminedPulseGain==No)
      {
        ParxRelsMakeEditable("SPExamRfcAmpl");
      }
      else
      {
        ParxRelsMakeNonEditable("SPExamRfcAmpl");
      }
      ParxRelsMakeNonEditable("SPExamExcAmpl,SPExamInvAmpl");
      break;
    case Inversion_Profile:
      ParxRelsMakeEditable("SPExamInv,SPExamInvEnum");
      ParxRelsMakeNonEditable("SPExamExc,SPExamExcEnum,SPExamRfc,SPExamRfcEnum");
      if(DeriveExaminedPulseGain==No)
      {
        ParxRelsMakeEditable("SPExamInvAmpl");
      }
      else
      {
        ParxRelsMakeNonEditable("SPExamInvAmpl");
      }
      ParxRelsMakeNonEditable("SPExamExcAmpl,SPExamRfcAmpl");
      break;
    }
  }
 
  if(DeriveHardPulseGain==No)
  {
    ParxRelsMakeEditable("HardPulseAmpl");
  }
  else
  {
    ParxRelsMakeNonEditable("HardPulseAmpl");
  }


  if(ProfileMode == Inversion_Profile || ProfileMode ==Saturation_Profile)
  {
    Preparation = Yes;
  }  
  else
  {
    Preparation = No;
  }


  DB_MSG(("<--ProfileRel"));
  return;
}


// Excitation pulse

void SPExamExcRange(void)
{
  // range checker fields to be controlled may be
  // .Length  
  // .Bandwidth
  // .Flipangle
  // .Calculated
  // .Sharpness
  // .Flatness   

  double dval=SPExamExc.Flipangle;

  SPExamExc.Flipangle = MAX_OF(MIN_OF(360.0,dval),1.0);
  
}

void SPExamExcRel(void)
{
  UT_SetRequest("SPExamExc");
  SPExamExcRange();
  backbone();
}

void SPExamExcEnumRel(void)
{
  UT_SetRequest("SPExamExcEnum");
  backbone();
}

void SPExamExcAmplRel(void)
{
  UT_SetRequest("SPExamExcAmpl");
  RfPulseAmplRel();
}


// Refocussing pulse
void SPExamRfcRange(void)
{
  // range checker fields to be controlled may be
  // .Length  
  // .Bandwidth
  // .Flipangle
  // .Calculated
  // .Sharpness
  // .Flatness   

  double dval=SPExamRfc.Flipangle;

  SPExamRfc.Flipangle = MAX_OF(MIN_OF(360.0,dval),90.0);
  
}

void SPExamRfcRel(void)
{
  UT_SetRequest("SPExamRfc");
  SPExamRfcRange();
  backbone();
}

void SPExamRfcEnumRel(void)
{
  UT_SetRequest("SPExamRfcEnum");
  backbone();
}

void SPExamRfcAmplRel(void)
{
  UT_SetRequest("SPExamRfcAmpl");
  RfPulseAmplRel();
}

// Inversion pulse
void SPExamInvRange(void)
{
  // range checker fields to be controlled may be
  // .Length  
  // .Bandwidth
  // .Flipangle
  // .Calculated
  // .Sharpness
  // .Flatness   

  double dval=SPExamInv.Flipangle;

  SPExamInv.Flipangle = MAX_OF(MIN_OF(360.0,dval),90.0);
  
}

void SPExamInvRel(void)
{
  UT_SetRequest("SPExamInv");
  SPExamInvRange();
  backbone();
}

void SPExamInvEnumRel(void)
{
  UT_SetRequest("SPExamInvEnum");
  backbone();
}

void SPExamInvAmplRel(void)
{
  UT_SetRequest("SPExamInvAmpl");
  RfPulseAmplRel();
}

void HardPulseRange(void)
{
  DB_MSG(("-->HardPulseRange"));
  double dval=HardPulse.Flipangle;
  HardPulse.Flipangle=MAX_OF(MIN_OF(180.0,dval),45.0);
  sprintf(HardPulse.Shape,"bp.exc");
  DB_MSG(("<--HardPulseRange"));
}

void HardPulseRel(void)
{
  UT_SetRequest("HardPulse");
  HardPulseRange();
  backbone();
}

void HardPulseAmplRel(void)
{
  UT_SetRequest("HardPulseAmpl");
  RfPulseAmplRel();
}


void RfPulseAmplRel(void)
{
  STB_UpdateRFShapeAmplitude("HardPulseAmpl,"
                             "SPExamExcAmpl,"
                             "SPExamRfcAmpl,"
                             "SPExamInvAmpl",No);

  switch(ProfileMode)
  {
    default:
    case Saturation_Profile:
    case Excitation_Profile:
      ATB_SetRFPulse("SPExamExc","ACQ_RfShapes[0]");
      break;
    case Refocusing_Profile:
      ATB_SetRFPulse("SPExamRfc","ACQ_RfShapes[0]");
      break;
    case Inversion_Profile:
      ATB_SetRFPulse("SPExamInv","ACQ_RfShapes[0]");
      break;
  }

  ATB_SetRFPulse("HardPulse","ACQ_RfShapes[1]");
  ATB_SetRFPulse("HardPulse","ACQ_RfShapes[2]");


  return;
}

void PulseAlgRel(void)
{
  PulseAlgRange();
  backbone();
}


void PulseAlgRange(void)
{
  if(!ParxRelsParHasValue("PulseAlg"))
  {
    PulseAlg=Conventional;
  }
  else
  {
    switch(PulseAlg)
    {
      default:
        PulseAlg=Conventional;
      case Conventional:
      case AfpDown:
      case AfpUp:
        break;
    }
  }
}


/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/





