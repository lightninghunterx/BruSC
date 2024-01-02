/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002-2003
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

double parameter OneRepTime;

double parameter
{
  display_name "Read Dephase Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ReadDephGrad;

double parameter
{
  display_name "Max Read Dephase Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ReadDephGradLim;

double parameter
{
  display_name "Read Dephase Time";
  format "%f";
  units  "ms";
  editable false;
  relations backbone;
}ReadDephTime;

double parameter
{
  display_name "Read Gradient";
  format "%f";
  units "%";
  relations backbone;
}ReadGrad;

double parameter
{
  display_name "Max Read Gradient";
  format "%f";
  units "%";
  relations backbone;
}ReadGradLim;

double parameter
{
  display_name "Encoding Duration";
  short_description "Duration of encoding gradient.";
  relations EncGradDurRel;
  units "ms";
  format "%.3f";
}EncGradDur;

double parameter
{
  display_name "2D Phase Gradient";
  format "%f";
  units "%";
  relations backbone;
}Phase2DGrad;

double parameter
{
  display_name "Max. 2D Phase Gradient";
  format "%f";
  units "%";
  relations backbone;
}Phase2DGradLim;

double parameter
{
  display_name "3D Phase Gradient";
  format "%f";
  units "%";
  relations backbone;
}Phase3DGrad;

double parameter
{
  display_name "Max. 3D Phase Gradient";
  format "%f";
  units "%";
  relations backbone;
}Phase3DGradLim;

double parameter
{
  display_name "Exc. Slice Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ExcSliceGrad;

double parameter
{
  display_name "Max. Exc. Slice Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ExcSliceGradLim;

double parameter
{
  display_name "Exc. Slice Reph. Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ExcSliceRephGrad;

double parameter
{
  display_name "Max. Exc. Slice Reph. Gradient";
  format "%f";
  units  "%";
  relations backbone;
}ExcSliceRephGradLim;

double parameter
{
  display_name "Exc. Slice Reph. Time";
  format "%f";
  units "ms";
  relations ExcSliceRephTimeRel;
}ExcSliceRephTime;

PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse Shape";
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
  relations backbone;
}ExcPulse1Shape[];


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

