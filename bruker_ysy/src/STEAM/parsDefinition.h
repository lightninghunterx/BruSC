/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2005
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/



/****************************************************************/
/* local parameter						*/
/****************************************************************/

/* rf pulses for localized excitation */
#include "RFPulseDef.h"
#include "steamLocPars.h"


/* ***************************************************************/
/*	Auto Adjustment Parameter	    			*/
/* ***************************************************************/

int parameter
{
  relations AutoSFCounterRel;
} AutoSFCounter;

/* manual adjustments parameter */

VSEL_ManAdjType parameter
{
  display_name "Manual Adjustment";
  relations OPT_ManAdjustmentRel;
} OPT_ManAdjustment;

double parameter
{
  display_name "FOV";
  units "mm";
  format "%.2f";
  relations OPT_FOVRel;
} OPT_FOV;

OnOff parameter
{
  display_name "Retro Frequency Lock";
  relations RetroFrequencyLockRelation;
} RetroFrequencyLock_OnOff;

OnOff parameter
{
  display_name "Eddy Current Compensation";
  short_description "Uses reference scan to compensate for eddy current induced phase distortions.";
  relations SetRecoParam;
} Edc_OnOff;

YesNo parameter
{
  display_name "Manual Filter Setting";
  short_description "Deactivates automatic determination of Gaussian filter width for water line extraction.";
  relations SetRecoParam;
} EdcManualFilter;
 	 
double parameter
{
  display_name "Filter Width";
  short_description "Width of Gaussian filter to extract water line.";
  units "Hz";
  format "%.2f";
  minimum 1.0 outofrange nearestval;
  maximum 100000.0 outofrange nearestval;
  //Reco Param -> no call of backbone!
} EdcFilterWidthHz;


int parameter AverageList[];

// parameter for shimming and frq adjustment
double parameter
{
  editable false;
  format "%.3e";
  display_name "Normalized Area";
}NormArea[][];

double parameter
{
  editable false;
  display_name "Line Width";
  format "%6.2f";
  units "Hz";
} LineWidth[][][]; // nlevel,spect,chan

OnOff parameter
{
  display_name "Calc Line Width";
  relations CalcLineWidthRelation;
} CalcLineWidth;

double parameter Peak[1][];
double parameter Noise[1][];


/* ***************************************************************/
/*	E N D   O F   F I L E					 */
/* ***************************************************************/















