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
/* INCLUDE FILES						*/
/****************************************************************/


#include "testproc.def"

PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse";
  relations    ExcPulse1EnumRelation;
}ExcPulse1Enum;


PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse";
  relations    ExcPulse1Relation;
}ExcPulse1;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "RF Pulse Amplitude";
  relations ExcPulse1AmplRel;
}ExcPulse1Ampl;

double parameter
{
  display_name "Pre-Acquisition Delay";
  relations DeadTimeRels;
  format "%.3f";
  units "ms";
} DeadTime;


// startup tuneup shim handling parameter

double parameter LoadedShimset[];


char parameter
{
  display_name "Startup Shim Name";
}StartShimName[64];

void parameter
{
  display_name "Deletes Tuneup Shim";
  short_description "Deletes Tuneup shim.";
  relations DeleteTuneupShimRel;
}DeleteTuneupShim;

void parameter
{
  display_name "Add Startup Shim";
  short_description "Stores actual shim as Startup shim.";
  relations AddStartShimRel;
}AddStartupShim;

void parameter
{
  display_name "Deletes Startup Shim";
  short_description "Deletes Startup shim.";
  relations DeleteStartShimRel;
}DeleteStartupShim;

STORE_SHIM_TYPE parameter
{
  display_name "Store Shim as";
  relations StoreShimRel;
} StoreShim;


// parameter for frequency offset and normalized area filter

// (channel averaged) weighted peak width
double parameter Wpwidth[1][1];
// (channel averaged) weighted frequency offset
double parameter WfrqOffs[1];
// normalized area 1 spectrum, dimensioned to number of channels
double parameter Narea[1][];



/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

