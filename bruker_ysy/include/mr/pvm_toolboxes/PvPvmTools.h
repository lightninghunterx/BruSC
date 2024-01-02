/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVPVMMRTOOLS_H
#define PVPVMMRTOOLS_H

#ifndef CPROTO

#include "common/pvm_toolboxes/PvPvmTools.h"
#include "pvidl.h"

#ifdef DLL_INTERFACE
#undef DLL_INTERFACE
#endif

/// Hidden interface definition
#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#ifdef PvPvmTools_INTERNAL
#define DLL_INTERFACE __declspec(dllexport)
#else
#define DLL_INTERFACE __declspec(dllimport)
#endif
#elif defined (__GNUC__) && __GNUC__ >= 4
#define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#define DLL_INTERFACE
#endif 


/*   temporarily emptied, can be deleted ... maybe */





#endif

#endif


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
