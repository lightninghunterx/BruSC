/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1995-2014
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVOVLTOOLS_ACQ_H
#define PVOVLTOOLS_ACQ_H
#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

#include "pvmachine.h"
#include "mr/ovl_toolbox/DLLMacro.h"


#ifndef CPROTO
/*==============================================================*/
/** @file
 * @defgroup PvOvlTools ParaVision overlay toolbox
 *  <p>This utility library contains general functions for:</p>
 *  - derivation of subchannel selection (ACQ_ReceiverSelect, ACQ_TxSguSelect)

/**
 * This function sets the base level parameter ACQ_ReceiverSelect by analyzing
 * the scan configuration parameters CONFIG_SCAN_coil_configuration, 
 * CONFIG_SCAN_operation_mode, and CONFIG_SCAN_RxCoilsSelect.
 */
DLL_INTERFACE void Derive_ACQ_ReceiverSelect();

/**
 * This function sets the base level parameter ACQ_TxSguSelect by analyzing
 * the scan configuration parameters CONFIG_SCAN_coil_configuration, 
 * CONFIG_SCAN_operation_mode, and CONFIG_SCAN_TxCoilsSelect.
 */
DLL_INTERFACE void Derive_ACQ_TxSguSelect();

#endif /* CPROTO */

/*@}*/

#endif


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
