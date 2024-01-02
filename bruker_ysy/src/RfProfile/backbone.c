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


/* ------------------------------------------------------------ 
  backbone 
  The main part of method code. The consitency of all parameters is checked
  chere, relations between them are resolved and, finally, functions setting
  the base level parameters are called.
  --------------------------------------------------------------*/
void backbone( void )
{

  double minFov[2] = {1e-3,1e-3};
  
  DB_MSG(("Entering RfProfile:backbone"));


  /* Nucleus and  PVM_GradCalConst
     are handled by this funtion: */
  STB_UpdateNuclei(Yes);
 
  DB_MSG(("nucleus ok"));


  /* handle RF pulse */   

  UpdateRFPulses();

  ProfileRel();
  
  PVM_AntiAlias[0]=1.0;

  PVM_AtsBedPositionMode=ATS_POS_REFERENCE;
  STB_UpdateAtsReferencePosition("");
  LocalGeometryMinimaRels(minFov);

  PVM_Fov[0]=MAX_OF(minFov[0],PVM_Fov[0]);

  MRT_UpdateDigPars(&PVM_EffSWh,
                     PVM_Matrix[0],
                     PVM_AntiAlias,
                    &PVM_AcquisitionTime);


  /* update Encoding pargroup for multi receiver experiment */
  STB_UpdateEncoding();


  /* FREQUENCY ENCODING GRADIENT */
  ReadGradient = MRT_ReadGrad(PVM_EffSWh, PVM_Fov[0], PVM_GradCalConst);
 
  /* Calculating of Spoiling */
  Spoiler_duration = MIN_OF(MAX_OF(0.5, Spoiler_duration),15.0);
  Spoiler_duration2=Spoiler_duration;
  double riseT = CFG_GradientRiseTime();
  
  if(ProfileMode == Excitation_Profile)
  {
    Spoiler_strength = (   ReadGradient * (PVM_AcquisitionTime/2
		                     + Spoiler_duration - SPExamExc.Length/2)
                       ) / ( Spoiler_duration2 + riseT );

    if(Spoiler_strength > 90.0)
    {
      Spoiler_strength = 90.0;
      Spoiler_duration2 = ( ( ReadGradient*(  PVM_AcquisitionTime/2 
                                            + Spoiler_duration 
                                            - SPExamExc.Length/2  )
                            ) / Spoiler_strength
                          ) - riseT;
    }
  }
  else
  {
    Spoiler_strength = ( ReadGradient*(PVM_AcquisitionTime/2+Spoiler_duration))/
                         (Spoiler_duration2+riseT); 
    if(Spoiler_strength > 90.0)
    {
      Spoiler_strength = 90.0;
      Spoiler_duration2 = (( ReadGradient*(PVM_AcquisitionTime/2+Spoiler_duration))/
                             Spoiler_strength)-riseT;
    }
  }

  PVM_NEchoImages = 1;

  echoTimeRels();

  /* repetition time */
  repetitionTimeRels();
 
  /* set baselevel acquisition parameter */
  SetBaseLevelParam();

  /* set baselevel reconstruction parameter */
  SetRecoParam();

  DB_MSG(("Exiting einpuls:backbone"));
}



/****************************************************************/
/*	         L O C A L   F U N C T I O N S			*/
/****************************************************************/

void echoTimeRels( void )
{
  DB_MSG(("-->echoTimeRels\n"));
  double excplen = 0.0,rfcplen = 0.0;


  switch(ProfileMode)
  {
    case Refocusing_Profile:
      excplen=HardPulse.Length/2;
      rfcplen=SPExamRfc.Length/2;
      break;
    case Inversion_Profile:
    case Saturation_Profile:
      excplen=HardPulse.Length/2;
      rfcplen=HardPulse.Length;
      break;
    case Excitation_Profile:
      excplen=SPExamExc.Length/2;
      rfcplen=HardPulse.Length/2;
      break;
  }




  /* TE limit due to 90-180 events */
  double igwt = CFG_InterGradientWaitTime();
  double riseT = CFG_GradientRiseTime(); 

  MinTE1 = 
    (excplen                   +
     riseT                     +
     0.1                       +
     riseT                     +
     Spoiler_duration2         +
     riseT                     +
     igwt                      +
     riseT                     +
     rfcplen ) *2.0;
  
  /* TE limit due to 180-180 events */  

  MinTE2 = 
    (rfcplen                     +
     riseT                       +
     igwt                        +
     riseT                       +
     Spoiler_duration            +
     PVM_AcquisitionTime/2) *2.0;
  
  PVM_MinEchoTime = MAX_OF(MinTE1,MinTE2);
  PVM_EchoTime = MAX_OF(PVM_MinEchoTime,PVM_EchoTime);
  
  DB_MSG(("<--echoTimeRels\n"));
}

void repetitionTimeRels( void )
{
  DB_MSG(("--> minRepetitionTimeRels"));

  double TotalTime, MinInversionTime = 0.0;
  double excplen =  0.0, invplen = 0.0;
  double riseT = CFG_GradientRiseTime();
  double igwt  = CFG_InterGradientWaitTime();

  switch(ProfileMode)
  {
  case Excitation_Profile:
    excplen=SPExamExc.Length/2;
    invplen=0.0;
    ParxRelsHideInEditor("InversionTime");
    break;
  case Refocusing_Profile:
    excplen=HardPulse.Length/2;
    invplen=0.0;
    ParxRelsHideInEditor("InversionTime");
    break;
  case Inversion_Profile:
    excplen=HardPulse.Length/2;
    invplen=SPExamInv.Length;
    ParxRelsShowInEditor("InversionTime");
    break;
  case Saturation_Profile:
    excplen=HardPulse.Length/2;
    invplen=SPExamExc.Length;
    ParxRelsShowInEditor("InversionTime");
    break;
  }

  TotalTime = 0.0;

  PVM_MinRepetitionTime = (PVM_EchoTime             +
                           PVM_AcquisitionTime/2    +
                           1.08                     + /* sum of fixed values in ppg */
                           2*riseT                  +
                           excplen                  +
                           1.0                     ); /* min d0 */


  if(Preparation == Yes)
  {
    MinInversionTime = invplen/2 + 5.0 + riseT + riseT + excplen + igwt;

    InversionTime = MAX_OF( MinInversionTime, InversionTime ); 

    PVM_MinRepetitionTime = PVM_MinRepetitionTime + InversionTime 
                                  /* add left half of inv pulse */
                            + riseT + invplen/2 
                                  /* subtract right half of exc pulse
                                   already counted in previous MinTR */
                            - riseT - excplen;
  }
                                  

  PVM_RepetitionTime = ( PVM_RepetitionTime < PVM_MinRepetitionTime ? 
			 PVM_MinRepetitionTime : PVM_RepetitionTime );
  
  /** Calculate Total Scan Time and Set for Scan Editor **/ 

  TotalTime = PVM_RepetitionTime*PVM_NAverages*PVM_NRepetitions;

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);

  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<-- repetitionTimeRels"));
}






void LocalGeometryMinimaRels(double *min_fov)
{
  DB_MSG(("-->LocalGeometryMinimaRels\n"));
    
  
  /* Gradient limits in % of max. Value */
 double LimReadGradient = 90.0; 

  /* Calculate the minimum field of view in the read direction */
  min_fov[0] = MRT_MinReadFov( PVM_EffSWh,
			       1.0,
			       LimReadGradient,
			       LimReadGradient,  
			       PVM_GradCalConst );
        
 
  DB_MSG(("<--LocalGeometryMinimaRels\n"));
}


void UpdateRFPulses()
{

  DB_MSG(("-->UpdateRFPulses"));

  // DeriveExaminedPulseGain 



  STB_UpdateRFPulse("SPExamExc",1,DeriveExaminedPulseGain, PulseAlg);
  STB_UpdateRFPulse("SPExamRfc",1,DeriveExaminedPulseGain, PulseAlg );
  STB_UpdateRFPulse("SPExamInv",1,DeriveExaminedPulseGain, PulseAlg );
  STB_UpdateRFPulse("HardPulse",1,DeriveHardPulseGain, Conventional );


  DB_MSG(("<--UpdateRFPulses"));
  return;
}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/





