/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2008
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * 
 *
 * ***************************************************************/

/***************************************************************/
/* local parameter						*/
/****************************************************************/

/* 
 * parameter used as delays, 
 * pulse duration and gradient amplitudes in pulse program 
 */

/* Delays */
double parameter 
{
	relations backbone;
} Isis_D0;


double parameter 
{
	relations backbone;
} Isis_D4;

double parameter 
{
	relations backbone;
} Isis_D6;

double parameter 
{
	relations backbone;
} Isis_D10;

double parameter 
{
	relations backbone;
} Isis_D11;

double parameter 
{
	relations backbone;
} Isis_D13;

double parameter 
{
	relations backbone;
} Isis_D14;



/* Pulse Durations */
double parameter 
{
	relations backbone;
} Isis_P0;

double parameter 
{
	relations backbone;
} Isis_P1;



/* Pulse Bandwidth */
double parameter 
{
  display_name "Inversion Bandwidth";
  relations backbone;
  units "[ppm]";
  format "%.2f";
} IsisInvBandwidthPpm;

double parameter 
{
  display_name "Excitation Bandwidth";
  relations backbone;
  units "[ppm]";
  format "%.2f";
} IsisSpecExcBandwidthPpm;


/* Gradient Amplitudes */
double parameter 
{
	relations backbone;
} Isis_Gr1;

double parameter 
{
	relations backbone;
} Isis_Gr2;

double parameter 
{
	relations backbone;
} Isis_Gr3;

double parameter 
{
	relations backbone;
} Isis_Gr4;


double parameter 
{
	relations backbone;
} Isis_Gp1;

double parameter 
{
	relations backbone;
} Isis_Gp2;

double parameter 
{
	relations backbone;
} Isis_Gp3;

double parameter 
{
	relations backbone;
} Isis_Gp4;


double parameter 
{
	relations backbone;
} Isis_Gs1;

double parameter 
{
	relations backbone;
} Isis_Gs2;

double parameter 
{
	relations backbone;
} Isis_Gs3;

double parameter 
{
	relations backbone;
} Isis_Gs4;


/* ISIS Module Parameters */
double parameter
{
  display_name "Spatial Shift";
  units "mm/[ppm]";
  format "%.2f";
  relations backbone;
  maxdim [once 3];
  style inline_array;
} IsisSpatialDispl[];

double parameter
{
  display_name "ISIS Module Duration";
  units "ms";
  format "%.2f";
  relations backbone;
} IsisModDur;

double parameter
{
  display_name "ISIS Spoiler Duration";
  format "%.2f";
  units "ms";
  relations IsisSpoilDurRels;
} IsisSpoilDur;

double parameter
{
  display_name "Spoiler Strength Dir1";
  format "%.2f";
  units "%";
  relations IsisSpoilPRels;
} IsisSpoilPerc1;

double parameter
{
  display_name "Spoiler Strength Dir2";
  format "%.2f";
  units "%";
  relations IsisSpoilPRels;
} IsisSpoilPerc2;

double parameter
{
  display_name "Spoiler Strength Dir3";
  format "%.2f";
  units "%";
  relations IsisSpoilPRels;
} IsisSpoilPerc3;

double parameter
{
  display_name "Spoiler Strength Dir1";
  format "%.2f";
  units "{cycles}/mm";
  relations IsisSpoilCRels;
} IsisSpoilCyMm1;

double parameter
{
  display_name "Spoiler Strength Dir2";
  format "%.2f";
  units "{cycles}/mm";
  relations IsisSpoilCRels;
} IsisSpoilCyMm2;

double parameter
{
  display_name "Spoiler Strength Dir3";
  format "%.2f";
  units "{cycles}/mm";
  relations IsisSpoilCRels;
} IsisSpoilCyMm3;

double parameter
{
  display_name "Gradient Stabilization";
  short_description "Delay between inversion spoiler and excitation pulse.";
  format "%.3f";
  units "ms";
  relations IsisStabDurRels;
} IsisStabDur;

double parameter
{
  display_name "Pre-Acquisition Delay";
  short_description "Delay between excitation pulse and acquisition.";
  relations DeadTimeRels;
  format "%.3f";
  units "ms";
} DeadTime;

/* Pipeline Acquisition Parameters */
int parameter
{
  display_name "Total ISIS Averages";
} IsisNAverages;

/* RF Pulses for Localization & Excitation */
PV_PULSE_LIST parameter
{
  display_name "Exc. Pulse Shape";
  relations    ExcPul1EnumRelation;
}ExcPul1Enum;

PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse";
  relations ExcPul1Relation;
}ExcPul1;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Exc. Pulse Amplitude";
  relations ExcPul1AmplRel;
}ExcPul1Ampl;

double parameter
{
  relations backbone;
}ExcPul1Shape[];

PV_PULSE_LIST parameter
{
  display_name "Inv. Pulse Shape";
  relations    InvPul1EnumRelation;
}InvPul1Enum;

PVM_RF_PULSE parameter
{
    display_name "Inversion Pulse";
    relations InvPul1Relation;
}InvPul1;

PVM_RF_PULSE_AMP_TYPE parameter
{
  display_name "Inv. Pulse Amplitude";
  relations InvPul1AmplRel;
}InvPul1Ampl;

double parameter
{
  relations backbone;
}InvPul1Shape[];

/* manual adjustments parameter */
ISIS_ManAdjType parameter
{
    display_name "Adjustment Mode";
    relations IsisManAdjustmentRels;
} IsisManAdjustment;

double parameter
{
    display_name "Field of View";
    units "mm";
    format "%.2f";
    relations IsisFovRels;
} IsisFov;

/* specific preparation parameters */
YesNo parameter	 
{	 
  display_name "Adjust Inversion Pulses?";	 
  relations backbone;	 
} IsisAdjInvPulses;	 
 	 
YesNo parameter	 
{	 
  display_name "Adjust OVS Pulses";	 
} IsisAdjOvsPulses;

int parameter
{
  display_name "Number of OVS Cycles";
  relations NumOvsCyclesRels;
} IsisNumOvsCycles;

int parameter IsisNumAddOvsCycles;

int parameter AverageList[];


/* ***************************************************************/
/*	E N D   O F   F I L E					 */
/* ***************************************************************/
