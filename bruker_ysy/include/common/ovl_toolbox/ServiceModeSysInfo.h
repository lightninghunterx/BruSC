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

#ifndef SERVICE_MODE_SYSINFO_H
#define SERVICE_MODE_SYSINFO_H

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
 * Get if the service mode is currently active.
 * @return <code>true</code> if the service mode is currently active,
 *         <code>false</code> otherwise.
 * @throws PvException In case of an error.
 */
DLL_INTERFACE bool SysInfo_ServiceModeActive();

/*@}*/

#endif
#endif


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
