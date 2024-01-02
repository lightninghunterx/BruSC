/* **************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2013
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id
 *
 * ***************************************************************/

/* **************************************************************/
/*	TYPEDEF's						*/
/* **************************************************************/

typedef enum
  {
    constant_slope,
    constant_time
  } RAMP_MODE;

typedef enum
  {
    Cardiac,
    Respiration
  }MOVIE_TYPE;

typedef enum
  {
    CardiacResp,
    OnlyCardiac,
    OnlyResp
  }ANALYSIS_MODE;

typedef enum
  {
    Magnitude,
    Phase,
    Auto
  }SIGNAL_TYPE;

/* **************************************************************/
/*	E N D   O F   F I L E					*/
/* **************************************************************/
