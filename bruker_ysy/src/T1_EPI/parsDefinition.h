/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2008
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


double parameter IROffset0;
double parameter ExSliceGradient;
double parameter ExSliceRephaseGradient;
double parameter RfcSpoilerStrength;
double parameter RfcSpoilerDuration;
int parameter Sli_ppg;
int parameter Rep_ppg;

double parameter SliceGradRatio;
double parameter ReadGradient;
double parameter MinTE_left;
double parameter MinTE_right;

double parameter MinIRSpacing;
double parameter MinIROffset;
double parameter MinIROffset0;
double parameter MinIROffset1;


double parameter 
{
  display_name "Rephasing/encoding time";
  relations RephaseTimeRels;
  units "ms";
  format "%.2f";
} RephaseTime;


int parameter 
{
  display_name "Segments";
  short_description "Number of segments." ;
  relations NSegmentsRels;
} NSegments;

double parameter
{
  display_name "Echo Time";
  units "ms";
  format "%.3f";
  relations backbone;
} InpEchoTime;

int parameter 
{
  display_name "Slices";
  short_description "Number of slices." ; 
  relations InpSlicesRelation;
} InpSlices;


double parameter
{
  display_name "Offset";
  short_description "Time from inversion to first excitation." ;
  units "ms";
  format "%.3f";
  relations IROffsetRelation; 
} IROffset;

double parameter
{
  display_name "Recovery Time";
  short_description "Inversion time; [slice][time point]." ;
  units "ms";
  format "%.3f";
  relations backbone;
} IRTime[][];

double parameter
{
  display_name "Spacing";
  short_description "Inversion time spacing." ; 
  units "ms";
  format "%.3f";
  relations IRSpacingRelation; 
} IRSpacing;

int parameter
{
  display_name "Time Points";
  short_description "Number of IR Points." ; 
  relations NIRPointsRelation;  
} NIRPoints;

int parameter
{
  display_name "Slice Group Size";
  relations SliceGroupSizeRelation;  
} SliceGroupSize;

YesNo parameter
{
  display_name "Slice Group Rotation";  
  relations RotationRelation;
} SliceGroupRotation;

double parameter
{
  display_name "Delay between volumes";
  format "%.2f";
  units "ms";
  relations PackDelRelation;
} PackDel;

PVM_SPOILER_TYPE parameter 
{
  display_name "Slice Spoiler";
  relations SliceSpoilerRel;
}SliceSpoiler;

PVM_SPOILER_TYPE parameter 
{
  display_name "Inversion Spoiler";
  relations InvSliceSpoilerRel;
}InvSliceSpoiler;

PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse Shape";
  relations    ExcPulseEnumRelation;
}ExcPulseEnum;

PV_PULSE_LIST parameter
{
  display_name "Refocusing Pulse Shape";
  relations    RefPulseEnumRelation;
}RefPulseEnum;

PV_PULSE_LIST parameter
{
  display_name "Inversion Pulse Shape";
  relations    InvPulseEnumRelation;
}InvPulseEnum;

PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse";
  relations    ExcPulRelation;
}ExcPul;

PVM_RF_PULSE parameter
{
  display_name "Refocusing Pulse";
  relations    RefPulRelation;
}RefPul;

PVM_RF_PULSE parameter
{
  display_name "Inversion Pulse";
  relations    InvPulRelation;
}InvPul;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Excitation Pulse Amplitude";
  relations ExcPulseAmplRel;
}ExcPulseAmpl;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Refocusing Pulse Amplitude";
  relations RefPulseAmplRel;
}RefPulseAmpl;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Inversion Pulse Amplitude";
  relations InvPulseAmplRel;
}InvPulseAmpl;

double parameter
{
  editable false;
}ExcPulseShape[];

double parameter
{
  editable false;
}RefPulseShape[];

double parameter
{
  editable false;
}InvPulseShape[];

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

