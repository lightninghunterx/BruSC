/****************************************************************
 *
 * Copyright (c) 2019
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef COMMON_RECOTYP_H
#define COMMON_RECOTYP_H

// options for Pvnl Fourier Transformation, not necessarily restricted to MRI

typedef enum
{
    NO_QOPTS,
    COMPLEX_CONJUGATE,
    QUAD_NEGATION,
    CONJ_AND_QNEG
} RECO_QOPTS_TYPE;


typedef enum
{
    LS_CGNR,
    LS_KACZMARZ,
    LS_BLOCK_KACZMARZ,
    LS_SL0,
    LS_STOMP,
    LS_GFPC,
    LS_THRLW,
    LS_NON_LIN_CG,
    LS_FISTA
} RECO_LINEARSOLVER_TYPE;


typedef enum
{
    LT_DENSE,
    LT_SPARSE,
    LT_FT,
    LT_SPARSE_FT,
    LT_DCT,
    LT_DWT,
    LT_IDENTITY,
    LT_COMPOSITION,
    LT_DIAG,
    LT_SPARSE_DCT
} RECO_LINEARTRAFO_TYPE;



#endif // COMMON_RECOTYP_H

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
