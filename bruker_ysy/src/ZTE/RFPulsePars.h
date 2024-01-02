/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2004
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/


/*
 *  Excitation pulse parameter
 */
 
YesNo parameter
{
  display_name "Excitation Pulse Auto";
  short_description "Pulse setting appropriate for acquistion bandwidth.";
  relations backbone;
} ExcPulseAuto;

PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse";
  short_description "Excitation hard pulse.";
  relations ExcPulRel;
} ExcPul;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Excitation Pulse Amplitude";
  relations ExcPulAmplRel;
} ExcPulAmpl;







