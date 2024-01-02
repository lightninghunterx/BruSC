/****************************************************************
 *
 * $Source$
 * 
 * Copyright (c) 1988-2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 * 
 * All Rights Reserved 
 * 
 * $Id$
 *
 ****************************************************************/

#ifndef MR_PROCTYP_H
#define MR_PROCTYP_H


/*--------------------------------------------------------------*
 * The following typedef's are used to define several parameters
 * in the PROC parameter class.
 *--------------------------------------------------------------*/
typedef enum
{
  _321,
  _312
} AQORDER_TYPE;

typedef enum
{
  No_BC,
  SINGLE_BC,
  QUAD_BC,
  SPOL_BC,
  QPOL_BC,
  SFIL_BC,
  QFIL_BC
} BC_TYPE;

typedef enum
{
  RAW,
  PROC
} DATMOD_TYPE;

typedef enum
{
  No_FT,
  ISR,
  IQC,
  IQR,
  FSC,
  FSR,
  FQC,
  FQR,
  ISC
} FT_mod_TYPE;

typedef enum
{
  QF,
  QSEQ,
  TPPI,
  RSH
} MC2_TYPE;

typedef enum
{
  GLOBAL,
  PREG,
  IREG,
  PIREG,
  SREG,
  PSREG,
  NOISE
} PSCAL_TYPE;

typedef enum
{
  POSITIVE,
  NEGATIVE,
  BOTH
} PSIGN_TYPE;

typedef enum
{
  NO_SYM,
  SYM,
  SYMA,
  SYMJ
} SYMM_TYPE;

#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
