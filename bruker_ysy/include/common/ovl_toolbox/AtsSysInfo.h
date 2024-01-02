/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2016-2017
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef ATS_SYSINFO_H
#define ATS_SYSINFO_H

#include "Parx/publicTypes.h"
#include "common/ovl_toolbox/DLLMacro.h"

#ifndef CPROTO

/*==============================================================*/
/** @file
 *  @addtogroup PvOvlTools ParaVision overlay toolbox
 *  <p>This sysinfo library contains general functions for all modalities:</p>
 *  - hardware information used in overlays, methods.
 */
/*@{*/

/** 
 * Get if the ATS should be used for an experiment.
 * @return <code>true</code> if the ATS should be used for an experiment.
 */
DLL_INTERFACE bool SysInfo_UseAts();

/**
 * Get the minimum ATS position in ATS coordinate system.
 * @return The minimum ATS position in mm.
 * @throws PvException in case of an error.
 */
DLL_INTERFACE double SysInfo_AtsMinPosition();

/**
 * Get the maximum ATS position in ATS coordinate system.
 * @return The maximum ATS position in mm.
 * @throws PvException in case of an error.
 */
DLL_INTERFACE double SysInfo_AtsMaxPosition();

/**
 * Get the maximum fault for repositioning of the ATS.
 * @return The maximum fault in mm.
 * @throws PvException in case of an error.
 */
DLL_INTERFACE double SysInfo_AtsPrecision();

/*@}*/

#endif
#endif


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
