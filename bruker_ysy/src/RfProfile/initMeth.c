/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2001 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	1


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


/*:=MPB=:=======================================================*
 *
 * Global Function: initMeth
 *
 * Description: This procedure is implicitly called when this
 *	method is selected.
 *
 * Error History: 
 *
 * Interface:							*/

void initMeth()
/*:=MPE=:=======================================================*/
{
  DB_MSG(( "Entering singlepulse:initMeth()" ));

 
  /*  Initialize PVM_NAverages PVM_NRepetitions see code in parsRelations.c */
  Local_NAveragesRange();
  NrepRange();

  if(ParxRelsParHasValue("PVM_EchoTime") == No)
    PVM_EchoTime = 10.0;

  /* Initialisation of nucleus */  
  if (ParxRelsParHasValue("PVM_Nucleus1") == 0)
   PVM_Nucleus1Enum = (STANDARD_NUCLEUS_TYPE)0;

  STB_InitNuclei(1);

  EffSWhRange();

  /* initialisation of multi receiver parameters */
  STB_InitEncoding();

  ParxRelsMakeNonEditable("PVM_EncActReceivers"); 

  if(ParxRelsParHasValue("GradientDirection") == No)
    GradientDirection = Z_dir;
  GradientDirectionRange();

  if(ParxRelsParHasValue("Spoiler_duration") == No)
    Spoiler_duration = 4.0;

  if(ParxRelsParHasValue("InversionTime") == No)  
     InversionTime = 10.0;

  if(!ParxRelsParHasValue("PVM_Matrix"))
    PVM_Matrix[0]=256;

  if(!ParxRelsParHasValue("PVM_Fov"))
    PVM_Fov[0]=50.0;

  if(!ParxRelsParHasValue("PVM_RepetitionTime"))
    PVM_RepetitionTime=500.0;

  NdummyRange();

  PulseAlgRange();

  STB_InitRFPulse("SPExamExc",
                  "SPExamExcEnum",
                  "SPExamExcAmpl",
                  "SPExamExcShape",
                  0,
                  "gauss.exc",
                  5000.0,
                  90.0);


  // call range checker of excitation pulse
  SPExamExcRange();

  STB_InitRFPulse("SPExamRfc",
                  "SPExamRfcEnum",
                  "SPExamRfcAmpl",
                  "SPExamRfcShape",
                  1,
                  "gauss.rfc",
                  5000.0,
                  180.0);


  // call range checker of refocussing pulse
  SPExamRfcRange();

  STB_InitRFPulse("SPExamInv",
                  "SPExamInvEnum",
                  "SPExamInvAmpl",
                  "SPExamInvShape",
                  2,
                  "gauss.inv",
                  5000.0,
                  180.0);


  // call range checker of refocussing pulse
  SPExamInvRange();


  ParxRelsMakeNonEditable("PVM_AtsBedPositionMode,PVM_AtsChangeRefPos");
  PVM_AtsBedPositionMode=ATS_POS_REFERENCE;
  PVM_AtsChangeRefPos=No;
  STB_InitAtsReferencePosition();



  STB_InitRFPulse("HardPulse",
                  0,               // no enum
                  "HardPulseAmpl", // ampl parameter
                  0,               // no shape parameter
                  0,               // excitation mode Excitation
                  "bp.exc",        // default shapename
                  20000.0,         // default bandwidth
                  90.0);           // default pulse angle





 
  /* Once all parameters have initial values, the backbone is called */
  backbone();



 

  DB_MSG(( "Exiting singlepulse:initMeth()" ));

}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/









