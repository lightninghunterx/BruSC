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
 * $Id$
 *
 * ***************************************************************/

/* ***************************************************************/
/*	PARAMETER CLASS	               			       	 */
/* ***************************************************************/

/*--------------------------------------------------------------*
 * Definition of the PV class...
 *--------------------------------------------------------------*/

pargroup
{
  ExcPulse1;
} RF_Pulses;

pargroup
{
  NDummyScans;
  EncGradDur;
  PVM_EffSWh;
  PVM_AcquisitionTime;
  PVM_EchoPosition;
  ReadGrad;
  ReadDephGrad;
  ReadSpoilGradDur;
  ReadSpoilGrad;
  Phase2DGrad;
  Phase3DGrad;
  ExcSliceGrad;		 	 
  ExcSliceRephGrad; 
}SequenceDetails;

extend pargroup
{
  PVM_EchoTime;
  PVM_MinEchoTime;
  PVM_RepetitionTime;
  PVM_NEchoImages;
  PVM_NAverages;
  PVM_ScanTimeStr;
  PVM_ScanTime;
  PVM_DeriveGains;
  RF_Pulses;
  Nuclei;
  SequenceDetails;
  Encoding;
  ImageGeometry;
} MethodClass;


/* ***************************************************************/
/*	E N D   O F   F I L E					 */
/* ***************************************************************/

