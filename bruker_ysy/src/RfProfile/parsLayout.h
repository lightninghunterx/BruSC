/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999-2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

/****************************************************************/
/*	PARAMETER CLASSES				       	*/
/****************************************************************/


/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/

pargroup
{
  HardPulse;
  HardPulseAmpl;
  SPExamExcEnum;
  SPExamExcAmpl;
  SPExamExc;
  SPExamRfcEnum;
  SPExamRfcAmpl;
  SPExamRfc;
  SPExamInvEnum;
  SPExamInvAmpl;
  SPExamInv;
  PulseAlg;
}
attributes
{
  display_name "RF Pulses";
} RF_Pulses;

pargroup
{
  PVM_EffSWh;
  PVM_Matrix;
  ReadGradient;
  ReadDephGradient;
  PVM_Fov;
  Spoiler_duration;
  InversionTime;
} 
attributes
{
  display_name "Sequence Details";
} Sequence_Details;

extend pargroup
{
  PVM_EchoTime;
  PVM_RepetitionTime;
  PVM_NAverages;
  PVM_NRepetitions;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  GradientDirection;
  ProfileMode;
  RF_Pulses;
  DeriveExaminedPulseGain;
  DeriveHardPulseGain;
  Nuclei;
  Sequence_Details;
  Encoding;
  Ndummy;
} MethodClass;


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



