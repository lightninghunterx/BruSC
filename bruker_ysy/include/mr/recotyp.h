/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1995-2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef RECOTYP_H
#define RECOTYP_H

#include "imagetyp.h"
#include "common/recotyp.h"
#include "libRecoTools/RecoStageTyp.h"
/*--------------------------------------------------------------*
 * The following typedef's are used to define several parameters
 * in the RECO parameter class.
 *--------------------------------------------------------------*/

typedef enum
  {
  NO_REORDERING,
  REV_ALT_ROWS
  } RECO_INP_REORDER;

typedef enum
  {
  NO_WDW,
  EXPONENTIAL,
  GAUSSIAN,
  SINE,
  SINE_SQUARED,
  SINC,
  SINC_SQUARED,
  TRAPEZOID,
  TRAFICANTE,
  TRAFICANTE_SQUARED,
  SQUARE,
  USER_DEF_WDW
  } RECO_WDW_TYPE;

typedef enum
  {
  NO_FT,
  REAL_FT,
  REAL_FFT,
  COMPLEX_FT,
  COMPLEX_FFT,
  REAL_IFT,
  REAL_IFFT,
  COMPLEX_IFT,
  COMPLEX_IFFT
  } RECO_FT_TYPE;

typedef enum
  {
  NO_PC,
  FIRST_ORDER_PC
  } RECO_PC_TYPE;

typedef struct
  {
  double	pc0;
  double	pc1;
  } RECO_1ORD_COEFFS;

typedef enum
  {
  NO_PPC,
  AUTO_PPC,
  MANUAL_PPC
  } RECO_PPC_TYPE;


typedef enum
  {
  ABSOLUTE_MAPPING,
  PERCENTILE_MAPPING,
  USER_RANGE_MAPPING,
  PER_OBJECT_MAPPING,
  USER_SCALE_MAPPING
  } RECO_MAP_TYPE;

typedef enum
  {
      SumOfSquares,
      ShuffleImages,
      AddImages
  } RECO_COMBINE_TYPE;  

/* Grappa processing mode. */
typedef enum
  {
      GrappaProcStandard,         /* coefficients are determined and applied directly */
      GrappaProcCalibrate,        /* coefficients are calibrated and stored */
      GrappaProcApply             /* coefficients are restored and applied to the data */
  } RECO_GRAPPA_PROC_MODE;

/* trajectory types affecting density correction */
typedef enum
{
  RecoRegridNTrajAny,    /* any trajectory (only with density post-correction; requires PVMUTE or PVMSPIRAL license) */
  RecoRegridNTrajRadial, /* radial trajectory (rho density pre-correction, requires PVMUTE license) */
  RecoRegridNTrajSpiral  /* spiral trajectory (jacobian density pre-correction, requires PVMSPIRAL license) */
} RecoRegridNTrajTypes;

/* types of density correction */
typedef enum
{
  RecoRegridNDCNone,     /* no density correction */
  RecoRegridNDCPost,     /* density post-correction */
  RecoRegridNDCPre,      /* density pre-correction using provided trajectory */
  RecoRegridNDCPrePost   /* both density pre- and post-correction */
} RecoRegridNDCTypes;

/* types of time series phase correction */
typedef enum
{
  RecoCorrPhaseOff,      /* no phase correction */
  RecoCorrPhaseIn,       /* time series inside slice loop */
  RecoCorrPhaseOut       /* time series outside slice loop */
} RecoCorrPhaseTypes;

#endif /* _RECOTYP_H_ */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
