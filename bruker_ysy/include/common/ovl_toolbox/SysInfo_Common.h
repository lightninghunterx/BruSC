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

#ifndef COMMON_SYSINFO_H
#define COMMON_SYSINFO_H

#ifndef CPROTO

#include <vector>
#include "Parx/publicTypes.h"
#include "common/ovl_toolbox/DLLMacro.h"
#include "modality.h"


/*==============================================================*/
/** @file
 *  @addtogroup PvOvlTools ParaVision overlay toolbox
 *  <p>This sysinfo library contains general functions for all modalities:</p>
 *  - hardware information used in overlays, methods.
 */
/*@{*/

/** 
 * Get if specified modality is currently available in study.
 * @param  modality modality type that should be checked for
 * @return <code>true</code> if the specified modality is currently available,
 *         <code>false</code> otherwise.
 * @throws PvException In case of an error.
 */
DLL_INTERFACE bool SysInfo_StudyModalityActive(PvModalityType modality);

/** 
 * Get vector of modalities  currently available in study.
 * @return vector of type <code>PvModalityType</code> .
 * @throws PvException In case of an error.
 */
DLL_INTERFACE const std::vector<PvModalityType> SysInfo_ActiveStudyModalities(void);





/*@}*/

#endif
#endif


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
