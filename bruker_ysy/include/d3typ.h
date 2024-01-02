/****************************************************************
 *
 * $Source$
 * 
 * Copyright (c) 1988-2017
 * Bruker BioSpin MRI GmbH
 * D7512 Rheinstetten, Germany
 * 
 * All Rights Reserved 
 * 
 * $Id$
 *
 ****************************************************************/

#ifndef D3TYP_H
#define D3TYP_H

/*--------------------------------------------------------------*
 * This file contains typedefs for the D3 parameter groups...
 *--------------------------------------------------------------*/
typedef enum
{
  orig,
  intp,
  surf,
  proj
} ORIG_TYPE;

#define ip_float ip_int

typedef enum
{
  ip_bit,
  ip_byte,
  ip_u_byte,
  ip_short,
  ip_u_short,
  ip_int,
  ip_u_int
/* 
  this hack is to make floating point reco possible without disturbing xtip
  remove the comment and the define above when changing xtip
  ip_float
*/
} DATTYPE_TYPE;

typedef enum
{
  x,
  y,
  z,
  t
} PRI_TYPE;

typedef enum
{
  slices,
  echoes,
  ms_me,
  project,
  sl_tseq,
  pr_tseq,
  tseq_pr
} SEQTYPE_TYPE;

#endif /* D3TYP_H */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
