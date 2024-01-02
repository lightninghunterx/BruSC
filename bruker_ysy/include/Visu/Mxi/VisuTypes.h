/****************************************************************
 *
 * Copyright (c) 2018
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef MXI_VISUTYPES_H
#define MXI_VISUTYPES_H

#include "Visu/Common/VisuTypes.h"

/** Definition of receive and transmit coil type */
enum VisuCoilType
{
    /** A body coil. */
    BODY_COIL,

    /** A volume coil for head, extremity, etc. */
    VOLUME_COIL,

    /** A surface coil. */
    SURFACE_COIL,

    /** A multi coil. */
    MULTICOIL
};

/** Structure to describe multi coils. */
struct VisuMultiCoilStruct
{
    /** Name of the coil. */
    char name[VISU_MAX_NAME_LEN];

    /** Indicates if this coil is used in the scan. */
    YesNo active;
};



#endif /* VISUTYPES_H */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



