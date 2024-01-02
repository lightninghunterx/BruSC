/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001-2003
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

#include "Phase3DPars.h"
#include "SliceSelPars.h"
#include "RFPulsePars.h"


double parameter OneRepTime;

int parameter 
{
  display_name "Segments";
  short_description "Number of Segments." ;
  relations NSegmentsRels;
} NSegments;

YesNo parameter
{
  display_name "Show Noneditable Pars";
  relations ShowAllParsRel;
}ShowAllPars;

double parameter
{
  display_name "Gradient Stabilization Time";
  relations GradStabTimeRel;
  units "ms";
  format "%f";
}GradStabTime;

/*
 *  parameters to pass information between different 
 *  routines. Since only the method may change their value,
 *  they are redirected to the backbone routine.
 */

double parameter
{
  relations backbone;
} MinTE1;

double parameter
{
  relations backbone;
}MinTE2;



double parameter
{
  display_name "Echo Time";
  units "ms";
  format "%.3f";
  relations EchoTimeRel;
} EchoTime;

double parameter
{
  display_name "Delay Between Volumes";
  format "%.2f";
  units "ms";
  relations PackDelRel;
} PackDel;

/* parameters for DTI Reconstruction */
RecoMeth_MODE parameter
{
  display_name "Reconstruction Mode";
  short_description "Switches between standard and diffusion tensor reconstruction.";
  relations RecoMethModeRel;
}RecoMethMode;

double parameter
{
  display_name "Trace Weight";
  short_description "Specifies the factor b in the weight exp(-b*trace/3) for trace weighted images.";
  format "%.2f";
  units "s/mm2";
  relations RecoMethModeRel;  
}TraceWeight;

/****************************************************************/
/* TYPE NAMES                                                   */
/****************************************************************/
enum_attributes RecoMeth_MODE
{
  display_name Default  "Default";
  display_name TensorReconstruction "Tensor Reconstruction";
};

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

