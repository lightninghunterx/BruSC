/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2005
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
  AcqSize;
  PVM_AcquisitionTime;
  PVM_EffSWh;
  FreqOffsList;
}Sequence_Details;

pargroup
{
  NChan;
  Nexc;
  FreqRes;
  FreqAdjRange;
  FreqOffsList;
  AcqSize;
  ExcPulse1;
  FreqAdjCounter;
  OFrq;
  WOFrq;
  Peak;
  Noise;
  PWidth;
  PVM_Nucleus1;
  PVM_FrqAdj1H;
  PVM_FrqRefPpm;
  PVM_FrqWorkPpm;
}AdjResultReportGroup;

extend pargroup
{
  Nuclei;
  PVM_RepetitionTime;
  Nexc;
  FreqAdjRange;
  FreqRes;
  ExcPulse1Enum;
  ExcPulse1Ampl;
  RfPulseDur;
  ExcPulse1;
  Sequence_Details;
  Encoding;
  PVM_TriggerModule;
  Trigger_Parameters;
} MethodClass;

/* following pargroups should not be stored in protocol because
   they are inherited by methods calling the adjustment:

   o Nuclei
   o Trigger_Parameters
   o PVM_TriggerModule
   o Encoding

   These parameters and pargroups are not stored in protocol but
   they are stored in the method file to restore the state of these
   parameters in a ready scan 
*/


pargroup
{
  Method;
  MethManagGroup;
  MethodRecoGroup;
/*   Nuclei; */
  PVM_RepetitionTime;
  Nexc;
  FreqAdjRange;
  FreqRes;
  ExcPulse1Enum;
  ExcPulse1Ampl;
  RfPulseDur;
  ExcPulse1;
  Sequence_Details;
/*   Encoding; */
  PVM_TriggerMode;
  PVM_TriggerDelay;
/*   Trigger_Parameters; */
}
attributes
{
    // post read is needed since loadMeth is only called for 
    // MethodClass automatically.
    postread_proc methodProtocolPostRead;
} MethodProtocolGroup;



/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

