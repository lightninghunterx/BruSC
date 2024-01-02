/****************************************************************
 *
 * Copyright (c) 2015 - 2018
 * Bruker BioSpin MRI GmbH 
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/


#ifndef ADJCONFIGTYPES_H
#define ADJCONFIGTYPES_H

#include "bruktyp.h"
#include "adjBaseTypes.h"
#include "Parx/Dynenum.h"

/** Adjustment configuration type */
typedef struct AdjConfigType
{
    /** Adjustment name. */
    char name[ADJ_NAME_LENGTH];

    /** Adjustment display name. */
    char displayName[ADJ_DISPL_NAME_LENGTH];

    /** Adjustment description. */
    char description[ADJ_DESCR_LENGTH];

    /** Dummy value for compatibility to PV-360.1.0. */
    char dummy[ADJ_NAME_LENGTH];

    /** Adjustment protocol.
        If \#METHOD\# the actual scan method is used.
     */
    char protocol[ADJ_PROTOCOL_LENGTH];

   /** 
     * The default result type for the adjustment result parameters if
     * performed as on-demand adjustment.
     */
    AdjResultType onDemandResultType;
    
    /** Indicates if adjustment should be in on-demand list. */
    YesNo onDemandFlag;
    
    /** Indicates if hardware events are respected during configuration. */
    YesNo respectHardwareEvents;

    /** Perform order number in study order list. */
    int studyOrderNdx;

    /** Perform order number in scan order list. */
    int scanOrderNdx;

    /** Indicates if scan adjustment is mandatory in method specific scan order list. */
    YesNo scanMandatoryFlag;

    /** Indicates if adjustment is visible in adjustment platform. */
    YesNo isVisible;

} ADJ_CONFIG_TYPE;

/** The adjustment configuration enumeration. */
typedef dynenum ADJ_CONFIG_MODE;


#endif /* ADJMANAGERTYPES_H */


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
