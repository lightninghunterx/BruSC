/****************************************************************
 *
 * Copyright (c) 1999-2018
 * Bruker BioSpin MRI GmbH 
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/


#ifndef ADJMANAGERTYPES_H
#define ADJMANAGERTYPES_H

#include "bruktyp.h"
#include "PvUtil/TimeDefs.h"
#include "adjBaseTypes.h"
#include "generated/DataPath.h"
#include "generated/KeyUids.h"

/****************************************************************/
/* TYPEDEF ENUMs						*/
/****************************************************************/


/****************************************************************/
/* TYPEDEF STRUCTS						                                  */
/****************************************************************/

/** Definition of hardware chain indices for an adjustment. */
typedef struct AdjChainNdx
{
    /// first element in chain list.
    int first;

    ///number of elements in chain list.
    int len;    
} ADJ_CHAIN_NDX;


/** Definition of chains for adjustment. */
typedef struct AdjHardwareContext
{
    /** Hardware context key. */
    char key[ADJ_HWCONTEXT_KEY_LENGTH];  
    /** Hardware contxt value. */
    char context[ADJ_HWCONTEXT_LENGTH]; 
} ADJ_HARDWARE_CONTEXT;


/** The adjustment context */
typedef struct AdjContextType
{
    /** Adjustment name. */
    char name[ADJ_NAME_LENGTH];

    /** Adjustment display name. */
    char displayName[ADJ_DISPL_NAME_LENGTH];

    /** Adjustment description. */
    char description[ADJ_DESCR_LENGTH];

    /** Adjustment protocol.
        If \#METHOD\# the actual scan method is used.
     */
    char protocol[ADJ_PROTOCOL_LENGTH];

    /** Dummy value for compatibility to PV-360.1.0. */
    char dummy[ADJ_NAME_LENGTH];

    /** The method context of the adjustment. */
    char methodContext[ADJ_CONTEXT_LENGTH];

    /** 
     * The default result type for the adjustment result parameters if
     * performed as on-demand adjustment.
     */
    AdjResultType onDemandResultType;
    
    /** Indicates if adjustment is a configured adjustment. */
    YesNo isConfigured;
    
    /** Indicates if adjustment should respect hardware events. */
    YesNo respectHardwareEvents;

    /** Subprocess indicator. */
    char subprocess[ADJ_SUBPROCESS_LENGTH];

    /** Indicates in which context the adjustment handler was called. */
    ADJ_HANDLER_CONTEXT handlerContext;
    
    /** Indicates if the adjustment is visible in the GUI. */
    YesNo isVisible;

 } AdjContext;

/** Definition of sub-adjustments. */
typedef struct AdjSubAdjustmentType
{
    /** Name of sub-adjustment. */
    char name[ADJ_SUBPROCESS_LENGTH];
    
    /** 
     * Name of a configured adjustment that is used
     * as sub-adjustment.
     * If empty the sub-adjustment is method specific.
     * CAUTION: This adjustment cannot be an adjustment that 
     * has itself sub-adjustments. 
     */
    char adjName[ADJ_NAME_LENGTH];
    
    /**
     * The method context of the sub-adjustment.
     * if empty the context from the parameter adjustment is used.
     */
    char methodContext[ADJ_CONTEXT_LENGTH];
} AdjSubAdjType;


/** The adjustment state result. */
typedef struct 
{
    /** The adjustment context created this result. */
    AdjContext adjustment;          

    /** Date and time of this adjustment. */
    pvtime_t time; 

    /** The adjustment study identification. */
    char studyUid[de_bruker_mri_generated_uid_CStrIsoUidLen];

    /** The adjustment experiment number. */
    int expno; 

    /** The adjustment execution state. */
    AdjExecutionState execState; 

    /** The result identifier. */
    char resultId[ADJ_RESULTID_LENGTH];

    /** The method that created this result. */
    char methodName[ADJ_METHOD_LENGTH];
} AdjResult;

#endif /* ADJMANAGERTYPES_H */


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
