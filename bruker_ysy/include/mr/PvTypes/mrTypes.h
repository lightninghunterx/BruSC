/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef MR_MR_TYPES_H
#define MR_MR_TYPES_H

#include "common/PvTypes/commonTypes.h"
#include "mr/PvTypes/pvmTypes.h"
#include "mr/PvTypes/internalPvmTypes.h"
#include "mr/acqtyp.h"
#include "mr/proctyp.h"

/****************************************************************/
/* TYPEDEF ENUMs						*/
/****************************************************************/

typedef enum {
    GRAD_COIL_UNKNOWN,
    GRAD_COIL_BGA09,
    GRAD_COIL_BGA105,
    GRAD_COIL_BGA12,
    GRAD_COIL_BGA20
} GRAD_COIL_TYPE;


#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
