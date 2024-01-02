/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2007
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

/****************************************************************/
/* INCLUDE FILES						*/
/****************************************************************/

/* =============================================================
 * Definition of local method parameters. Parameters that are
 * not editable and calculated inside the methods backbone such
 * as gradient values will have the backbone routine as default
 * relations. Only parameters that appear editable in the method
 * editor need a specific parameter relation and range checking
 * function.
 * ============================================================*/

/* -------------------------------------------------------
 * Frequency encoding parameter 
 * ------------------------------------------------------*/

double parameter
{
  display_name "Read Dephase Gradient";
  format "%f";
  units  "%";
  editable false;
  relations backbone;
}ReadDephGrad;

double parameter
{
  display_name "Read Gradient";
  format "%f";
  units "%";
  editable false;
  relations backbone;
}ReadGrad;

/* ----------------------------------------------------------
 * Read spoiler parameter
 * ---------------------------------------------------------*/

double parameter
{
  display_name "Read Spoiler";
  units "%";
  format "%.3f";
  minimum 0 outofrange nearestval;
  maximum 60 outofrange nearestval;
  short_description "Read spoiler gradient strength (limited to range: 0% ... 60%).";
  relations backbone;
}ReadSpoilGrad;

double parameter
{
  display_name "Read Spoiler Duration";
  units "ms";
  format "%.3f";
  relations ReadSpoilGradDurRel;
}ReadSpoilGradDur;


/* ----------------------------------------------------------
 * Phase encoding parameter
 * ---------------------------------------------------------*/

double parameter
{
  display_name "2D Phase Gradient";
  format "%f";
  units "%";
  editable false;
  relations backbone;
}Phase2DGrad;

double parameter
{
  display_name "3D Phase Gradient";
  format "%f";
  units "%";
  editable false;
  relations backbone;
}Phase3DGrad;


/* ---------------------------------------------------------------
 * Slice selection parameter
 * --------------------------------------------------------------*/

double parameter
{
  display_name "Exc. Slice Gradient";
  format "%f";
  units  "%";
  editable false;
  relations backbone;
}ExcSliceGrad;

double parameter
{
  display_name "Exc. Slice Reph. Gradient";
  format "%f";
  units  "%";
  editable false;
  relations backbone;
}ExcSliceRephGrad;


/* ----------------------------------------------------------
 * Definition of RF Pulse parameter
 * ---------------------------------------------------------*/

PVM_RF_PULSE parameter
{
  display_name "Excitation Pulse";
  relations    ExcPulse1Relation;
}ExcPulse1;

double parameter
{
  relations backbone;
}ExcPulse1Shape[];

/* ---------------------------------------------------------
 * remaining local method parameters
 * --------------------------------------------------------*/

double parameter
{
  display_name "Encoding Gradient Duration";
  relations EncGradDurRel;
  units "ms";
  format "%.3f";
}EncGradDur;

int parameter 
{
  display_name "Dummy Scans";
  relations backbone;
  minimum 0 outofrange nearestval;
} NDummyScans;


/* ---------------------------------------------------------
 * auxiliary method parameters
 * --------------------------------------------------------*/
double parameter {editable false;}ReadDephGradLim;
double parameter {editable false;}ReadGradLim;
double parameter {editable false;}Phase2DGradLim;
double parameter {editable false;}Phase3DGradLim;
double parameter {editable false;}ExcSliceGradLim;
double parameter {editable false;}ExcSliceRephGradLim;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/


