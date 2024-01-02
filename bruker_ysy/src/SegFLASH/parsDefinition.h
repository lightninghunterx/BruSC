/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2020
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
double parameter {units "ms";} EchoDelay;
double parameter {units "ms";} EffPulseDur;


double parameter
{
  display_name "Minimum Echo Repetition Time";
  relations backbone;
  format "%.2f";
  units "ms";
  editable false;
}MinEchoRepTime;

double parameter
{
  display_name "Echo Repetition Time";
  relations backbone;
  format "%.2f";
  units "ms";
  editable false;
}EchoRepTime;

int parameter
{
  display_name "Segment Size";
  short_description "Number of echoes per segment.";
  minimum 1 outofrange nearestval;
  maximum 1024 outofrange nearestval;
  relations backbone;
} SegmentSize;

double parameter
{
  display_name "Segment Duration";
  short_description "Time needed for the Segment and the Movie Frames.";
  format "%.2f";
  units "ms";
}SegmentDuration;

double parameter
{
  display_name "Segment Repetition Time";
  short_description "Segment Repetition Time.";
  relations backbone;
  format "%.2f";
  units "ms";
}SegmentRepTime;

double parameter MinSegmentRepTime;

double parameter
{ display_name "Frame Repetition Time";
  //short_description "";
  minimum 0.0 outofrange nearestval;
  maximum 10000.0 outofrange nearestval;
  relations backbone;
  format "%.2f";
  units "ms";
}FrameRepTime;

/* parameters for inversion */
enum_attributes Inversion_MODE
{
  display_name SliceSelInversion  "Slice-selective Inversion";
  display_name NonSelInversion    "Non-selective Inversion";
};

OnOff parameter
{
  display_name "Inversion";
  relations backbone;
}InversionOnOff;

Inversion_MODE parameter
{
  display_name "Inversion Mode";
  relations backbone;
}InversionMode;

/* parameters for spoiler gradients */
PVM_SPOILER_TYPE parameter
{
  display_name "Read Spoiler";
  relations ReadSpoilerRel;
}ReadSpoiler;

PVM_SPOILER_TYPE parameter
{
  display_name "Slice Spoiler";
  relations SliceSpoilerRel;
}SliceSpoiler;

PVM_SPOILER_TYPE parameter
{
  display_name "Segment Spoiler";
  relations SegSpoilerRel;
}SegSpoiler;

double parameter
{
  display_name "Inter Slice Delay";
  relations backbone;
  units "ms";
  format "%.2f";
}SliceSegDur;

/* parameters for hf pulse */
PV_PULSE_LIST parameter
{
  display_name "Excitation Pulse Shape";
  relations ExcPulse1EnumRelation;
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

YesNo parameter
{
  display_name "RF Spoiling";
  relations backbone;
}RFSpoiling;

double parameter RFPhaseList[];



/* ---------------------------------------------------------
 * remaining local method parameters
 * --------------------------------------------------------*/


/* parameters for flow compensation */
OnOff parameter
{
  display_name "Flow Compensation";
  relations backbone;
}FlowCompOnOff;

YesNo parameter FlowCompLongTEScan;

double parameter {units "%"; format "%.6f";}ReadFcGrad[2];
double parameter {units "%"; format "%.6f";}Phase2DFcGrad[2];
double parameter {units "%"; format "%.6f";}Phase3DFcGrad[2];
double parameter {units "%"; format "%.6f";}ExcSliceFcGrad[2];

double parameter P2Shape[];
double parameter P3Shape[];
double parameter RShape[];
double parameter SShape[];
int parameter ShapeSize;

/* parameters for trigger */
OnOff parameter
{
  display_name      "Trigger";
  short_description "Wait for external trigger pulse.";
  relations         TriggerOnOffRel;
} TriggerOnOff;

YesNo parameter
{
  display_name "Trigger Acquisition";
  relations AcqTriggerRel;
} AcqTrigger;

YesNo parameter
{
  display_name "Trigger Inversion";
  relations InvTriggerRel;
} InvTrigger;

double parameter
{
  display_name      "Delay";
  short_description "Delay for acquisition trigger.";
  format            "%.3f";
  units             "ms";
  minimum           0.03 outofrange nearestval;
  maximum           10000.0 outofrange nearestval;
  relations         backbone;
}AcqTriggerDelay;

double parameter
{
  display_name      "Delay";
  short_description "Delay for inversion trigger.";
  format            "%.3f";
  units             "ms";
  minimum           0.03 outofrange nearestval;
  maximum           10000.0 outofrange nearestval;
  relations         backbone;
} InvTriggerDelay;

double parameter
{
  display_name      "Module Duration";
  units             "ms";
  format            "%.3f";
  editable          false;
} AcqTriggerModuleTime;

double parameter
{
  display_name      "Module Duration";
  units             "ms";
  format            "%.3f";
  editable          false;
} InvTriggerModuleTime;

double parameter
{
  display_name      "Pulse Duration";
  units             "ms";
  format            "%.3f";
  editable          false;
} TriggerDur;

/* parameters for read gradient */
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


/* parameters for phase gradient */
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
  display_name "Encoding Duration";
  short_description "Duration of encoding gradient.";
  relations backbone;
  units "ms";
  format "%.3f";
}EncGradDur;

double parameter
{
  short_description "Helper variable to catch rounding effect.";
  units "ms";
  format "%.6f";
}RdVar;

double parameter {relations backbone;}Rew2DGrad;
double parameter {relations backbone;}Rew3DGrad;
double parameter {relations backbone;}RewGradDur;

/* parameters for slice gradient */
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


double parameter TeFillDelay;
double parameter TrFillDelay;

/* fix delay of 10 us for list handling in ppg (.inc, .store, etc) */
double parameter FixDelay_1;

double parameter ModulesTimeToKzero;

/* new parameters for SWI Reconstruction */
RecoMeth_MODE parameter
{
  display_name "Reconstruction Mode";
  short_description "Switches between standard and susceptibility-weighted imagig reconstruction.";
  relations SetRecoParam;
}RecoMethMode;

int parameter RecoReformatSerialData[];
int parameter RecoReformatSize[];
int parameter RecoSortSerialData[];
int parameter SortArgs[];

SegmentSlice_ORDER parameter
{
  display_name "Segment Slice Mode";
  short_description "Serial: all segments of a slice, then next slice. Alternate: a segment for all slices, then next segment.";
  relations SegmentSliceOrderRels;
}SegmentSliceOrder;

MASK_MODE parameter
{
  display_name "Weighting Mode";
  short_description "Selection of the way phase information will be used to influence image intensity.";
  relations MaskModeRel;
}WeightingMode;

double parameter
{
  display_name "Mask Weighting";
  short_description "Strength of the weighting mask function.";
  relations MaskWeightRange;
  format "%.2f";
}MaskWeighting;

double parameter
{
  display_name "Gauss Broadening";
  short_description "Defines the broadening (smoothing) effect of the Gauss filter.";
  relations GaussBroadRange;
  format "%.2f";
  units "mm";
}GaussBroadening;

YesNo parameter
{
  display_name "Flow Comp direction";
  style inline_array;
  short_description "Direction of Flow Compensation.";
  relations backbone;
}FlowCompDirection[3];

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

