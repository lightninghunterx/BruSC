/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2013
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/



/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/

double parameter ReadGrad;
double parameter SliceGrad;
double parameter ReadGradLim;
double parameter ReadDephGrad;
double parameter SliceGradLim;
double parameter SliceRephaseGrad;
double parameter ReadGradRatio;
double parameter SliceGradRatio;
double parameter Phase3dInteg;
double parameter Phase2dInteg;
double parameter Phase2DGrad;
double parameter Phase3DGrad;
double parameter TotalPhase2Offset[];
double parameter Alpha2Delay;
double parameter Alpha2SliceRephGrad;
double parameter FID_delay;
double parameter ECHO_delay;
double parameter FlipAngMzPow1;
double parameter FlipAngMzPow2;
double parameter OneRepTime;
double parameter ReadDephaseTime;
double parameter SliceRephaseTime;
double parameter PrepPulseShapeList[][];
double parameter Local_MinFov[3];
double parameter MinInversionTime;
double parameter MinSliceThick;

ACQSHAPE parameter PrepPulseList[];

double parameter
{
  display_name "Encoding Duration";
  short_description "Duration of encoding gradient.";
  relations backbone;
  units "ms";
  format "%.3f";
}EncGradDur;

PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse Shape";
  relations    ExcPulse1EnumRelation;
} ExcPulse1Enum;

PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse";
  relations    ExcPulse1Relation;
} ExcPulse1;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "RF Pulse Amplitude";
  relations ExcPulse1AmplRel;
}ExcPulse1Ampl;

double parameter
{
  relations backbone;
}ExcPulse1Shape[];

PV_PULSE_LIST parameter
{
  display_name "Inversion Pulse Shape";
  relations    InvPulse1EnumRelation;
} InvPulse1Enum;

PVM_RF_PULSE parameter
{
  display_name "Inversion Pulse";
  relations    InvPulse1Relation;
} InvPulse1;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "RF Pulse Amplitude";
  relations InvPulse1AmplRel;
}InvPulse1Ampl;

double parameter
{
  relations backbone;
}InvPulse1Shape[];

SSFP_MODE parameter
{
  display_name "SSFP Mode";
  relations    backbone;
} Ssfp;

int parameter
{
  display_name "Segments";
  short_description "Number of segments.";
  minimum 1 outofrange nearestval;
  maximum 1024 outofrange nearestval;
  relations    backbone;
} Nsegments;

SEGMENTING_MODE parameter
{
  display_name "Segmenting Mode";
  relations    backbone;
} Seg_mode;


double parameter
{
  display_name "Frame Separation";
  short_description "Frame separation in units of segment time.";
  format       "%.2f";
  minimum 0.0 outofrange nearestval;
  maximum 10000.0 outofrange nearestval;
  relations    backbone;
} Const_separation;

double parameter
{
  display_name "RF Phase Advance";
  units        "deg";
  format       "%.2f";
  minimum 0.0 outofrange nearestval;
  maximum 360.0 outofrange nearestval;
  relations    backbone;
} PhaseAdvance;

double parameter
{
  display_name "Segment Time";
  short_description "Segment time for one frame without separation.";
  units        "ms";
  format       "%.2f";
  relations    backbone;
} Seg_time;

double parameter
{
  display_name "One Frame with Separation";
  units        "ms";
  format       "%.2f";
  relations    backbone;
} Seg_Sep_time;

double parameter
{
  display_name "Movie Cycle";
  units        "ms";
  format       "%.2f";
  relations    backbone;
} Seg_Sep_Fra_time;

double parameter
{
  display_name "Time for Preparation";
  units        "{x TR}";
  format       "%.2f";
  relations    backbone;
} Alpha2_Prep_time;

double parameter
{
  display_name "Preparation Duration";
  units        "ms";
  format       "%.2f";
  relations    backbone;
} Prescan_Delay;

PVM_SPOILER_TYPE parameter
{
  display_name "Slice Spoiler";
  relations SliceSpoilerRel;
}SliceSpoiler;

PVM_SPOILER_TYPE parameter
{
  display_name "Preparation Spoiler";
  relations backbone;
}PrepSpoiler;

OnOff parameter
{
  display_name  "RF Spoiling";
  relations backbone;
}RFSpoilerOnOff;


double parameter
{
  display_name "Scan Repetition Time";
  units        "ms";
  format       "%.2f";
  relations    InvRepTimeRel;
} Inv_Rep_time;

double parameter
{
  display_name "Scan Repetition Time";
  units        "ms";
  format       "%.2f";
  relations    ScanRepTimeRel;
} Scan_RepetitionTime;

YesNo parameter
{
  display_name "Minimize Scan Repetition";
  short_description "Minimizes scan repetition time.";
  relations backbone;
}YesNoMinScanRepTime;

double parameter
{
  display_name "Total Scan Time";
  units        "ms";
  format       "%.2f";
  relations    backbone;
} Total_scan_time;

YesNo parameter
{
  display_name "Inversion";
  relations    backbone;
} FISP_inversion_enable;

int parameter
{
  relations backbone;
} SeparationEchoes;

double parameter
{
    display_name "Flip Angle for Mz";
    units "deg";
    format "%.2f";
    minimum 2.0 outofrange nearestval;
    maximum 88.0 outofrange nearestval;
    relations backbone;
} FlipAngMz;

int parameter
{
  display_name "Preparations for Mxy";
  relations NumPrepRel;
  format "%d";
}NumPrep;

SSFPPreparation_MODE parameter
{
  display_name "SSFP Preparation";
  relations SSFPPrepRel;
} Ssfp_preparation;

YesNo parameter
{
  display_name "Steady State 3D";
  short_description "Keeps the steady state for 3D and limits slice packages and segmentation.";
  relations    backbone;
} KeepSteadyState3D;

/****************************************************************/
/* TYPE NAMES                                                   */
/****************************************************************/
enum_attributes SSFP_MODE
{
  display_name TRUE_FISP  "TrueFISP";
  display_name FID        "FID";
  display_name ECHO       "ECHO";
};

enum_attributes SSFPPreparation_MODE
{
  display_name Alpha_2            "Alpha/2";
  display_name Starter_Sequence   "Starter Sequence";
  display_name No_Preparation     "None";
};

enum_attributes SEGMENTING_MODE
{
  display_name sequential  "Sequential";
  display_name interleaved "Interleaved";
};
/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/













