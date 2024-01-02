/****************************************************************
 *
 * Copyright (c) 1995-2019
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

/*--------------------------------------------------------------*
 * prevent multiple includes
 *--------------------------------------------------------------*/
#ifndef PARX_PARX_H
#define PARX_PARX_H

/**
   @defgroup PvParx PARX -- Extended Parameter Handling.
   This group contains all types and interface for the parameter
   handler in ParaVision.
 */

#include "Parx/publicTypes.h"
#include "Parx/Common/CommonTypes.hh"

/*--------------------------------------------------------------*
 * function prototypes
 *--------------------------------------------------------------*/

#define INCLUDE_FROM_PARX_H
#if defined(InParxOvl)
#    include "Parx/Rels/libParxRels.h"
#    include "Parx/Rels/ovlinterface.h"
#else
#    include "Parx/Client/libParx.h"
#endif
#undef INCLUDE_FROM_PARX_H

#endif /* ifndef PARX_PARX_H_ */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
