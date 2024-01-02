//===============================================================
//
// $Source$
//   Adjustment result administration class
//
// Copyright (c) 2015
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//
//===============================================================

#ifndef PVMRADJ_MRADJMANAGERTYPES_HH
#define PVMRADJ_MRADJMANAGERTYPES_HH

#include "PvAdj/adjBaseTypes.h"

/** The hardware scope identifiers defined for an adjustment.
    If the hardware scope is different to the one the method requested
    the adjustment is performed again or a result is not considered for usage.
 */
typedef enum HwScopeEnum
{
    /// The transmitter chain settings are used as hardware scope.
    TRANS_CHAIN,

    /// The receiver chain settings are used as hardware scope.
    REC_CHAIN,

    /// The gradient chain settings are used as hardware scope.
    GRAD_CHAIN,

    /// The shim chain settings are used as hardware scope.
    SHIM_CHAIN,

    /// The Rf-State is used as hardware scope.
    RF_STATE,
            
    /// The physical transmitter chain settings are used as hardware scope.
    PHYS_TRANS_CHAIN,
            
    /// The physical receiver chain settings are used as hardware scope.
    PHYS_REC_CHAIN,
} HW_SCOPE_ENUM;

/** Identification of a hardware chain. */
typedef struct AdjChainDef
{
    /// Type of hardware scope.
    HW_SCOPE_ENUM hwScope;

    /// The hardware channel, only relevant for \c TRANS_CHAIN and \c GRAD_CHAIN.
    int channel;
} ADJ_CHAIN_DEF;

/** Definition of chains for adjustment. */
typedef struct AdjChainContext
{
    ADJ_CHAIN_DEF chain; /* chain definition */
    char context[ADJ_HWCONTEXT_LENGTH]; /* chain context. */
} ADJ_CHAIN_CONTEXT;


#endif // #ifndef PVMRADJ_MRADJMANAGERTYPES_HH

//***************************************************************/
//	E N D   O F   F I L E					*/
//***************************************************************/
