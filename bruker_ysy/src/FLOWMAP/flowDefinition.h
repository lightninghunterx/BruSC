/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2006
 * Bruker Biospin MRI GMBH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

FLOW_MODE parameter
{
  display_name "Mode";
  short_description "Flow encoding mode.";
  relations FlowModeRel;
} FlowMode;

FLOW_ENC_DIR parameter
{
  display_name "Direction";  
  short_description "Flow encoding direction.";
  relations FlowEncodingDirectionRel;
} FlowEncodingDirection;

int parameter 
{
  display_name "No. of Flow Encoding Directions";
  relations FlowEncDirNumberRel;
} FlowEncDirNumber;

double parameter
{
  display_name "Velocity Range";
  relations FlowRangeRel;
  format "%.2f";
  units  "cm/s";
} FlowRange;

double parameter
{
  display_name "Velocity Resolution";
  relations FlowResolutionRel;
  format "%.2f";
  units  "cm/s";
} FlowResolution;

double parameter
{
  display_name "Velocity Resolution Limit";
  relations FlowMaxResolutionRel;
  format "%.2f";
  units  "cm/s";
} FlowMaxResolution;

OnOff parameter
{
display_name "Twister Gradient ";
  relations FlowTwisterRel;
}FlowTwister;

double parameter
{
  display_name "Twister Effect";
  relations FlowTwisterGradientRel;
  format "%.2f";
  units "mm";
} FlowTwisterGradient;

double parameter TwisterGradStrength;
double parameter MaxTwisterStrength;

int parameter
{
  display_name "Flow Encoding Steps";
  relations FlowEncStepsRel; 
} FlowEncSteps;

double parameter
{
  display_name "Flow Zero-Fill Factor";
  relations FlowZeroFillRel;
} FlowZeroFillFactor;

double parameter FlowEncGradientAmp;

double parameter
{
  display_name "Min Velocity Range";
  relations MinFlowRangeRel;
  format "%.2f";
  units  "cm/s";
} MinFlowRange;
 

YesNo parameter 
{ 
  relations backbone;
} FlowHandler;

double parameter FlowEncGradMatrix[][3];

int parameter 
{
  display_name "Flow Encoding Steps";
  relations backbone;
} FlowEncLoop;

int parameter 
{
  relations backbone;
} FlowImages;

int parameter FlowSpatDim;

double parameter
{
 display_name "Signal Mask Threshold";
 format "%.2f";
 units  "%";
 relations SignalMaskRel;
}SignalMask;

double parameter FlowGradR[];
double parameter FlowGradP[];
double parameter FlowGradS[];
/****************************************************************/
/* TYPE NAMES                                                   */
/****************************************************************/

enum_attributes FLOW_ENC_DIR
{
  display_name SliceDirection  "Slice";
  display_name PhaseDirection  "Phase";
  display_name ReadDirection   "Read";
  display_name AllDirections   "All";
};
