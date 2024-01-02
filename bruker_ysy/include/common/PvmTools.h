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

#ifndef CMN_PVMTOOLS_H
#define CMN_PVMTOOLS_H

/* --------------------------------------------------------------
 *   Include files...
 *--------------------------------------------------------------*/

#include "pvmachine.h"
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#ifndef CPROTO
#include <algorithm>
#include <numeric>
#include <functional>
#endif

/*--------------------------------------------------------------*
 * include files required at the start by Parx...
 *--------------------------------------------------------------*/

#include "debugdef.h"
#include "imagetyp.h"
#include "Parx/Parx.h"

/*--------------------------------------------------------------*
 * Uxnmr include files...
 *--------------------------------------------------------------*/
#include "common/acqtyp.h"

#include "common/pvm_toolboxes/PvUtilTools.h"
#include "common/pvm_toolboxes/PvCfgTools.h"
#include "common/pvm_toolboxes/PvSeqTools.h"
#include "common/pvm_toolboxes/PvPvmTools.h"
#include "common/pvm_toolboxes/PvGeoTools.h"
#include "common/pvm_toolboxes/PvGeoClasses.hh"
#include "common/ovl_toolbox/VisuTools.h"
#include "common/ovl_toolbox/Utils.h"
#include "common/ovl_toolbox/AdjTools.h"
#include "common/ovl_toolbox/AtsSysInfo.h"
#include "common/ovl_toolbox/StringBuffer.h"

#include "generated/VisuIds.h"

#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
