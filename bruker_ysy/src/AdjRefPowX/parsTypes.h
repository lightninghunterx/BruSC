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
 *
 * $Locker$
 * $Id$
 * $Id$
 *
 *
 ****************************************************************/

/****************************************************************/
/*	TYPEDEF's						*/
/****************************************************************/
typedef enum
  {
    CMPLX_RECO_MODE,
    MAGN_RECO_MODE
  } REFPOWX_RECO_MODE;

typedef enum
  {
    CONST_PULSE_POW,
    CONST_PULSE_DUR
  } REFPOWX_SCAN_MODE;

typedef enum
  {
    search_90,
    search_180
  } REFPOWX_SEARCH_MODE;

typedef enum
  {
    none,
    phases_cor,
    phases_uncor,
    spectra
  } REFPOWX_LOG;

typedef struct
{
  char nuc[8];
  double val;
  char stat[256];
} REFPOW_TABLE;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
