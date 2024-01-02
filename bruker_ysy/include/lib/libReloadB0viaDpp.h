/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2006-2010
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

/* prevent double includes */
#ifndef LIBRELOADB0VIADPP_H
#define LIBRELOADB0VIADPP_H

#include "acqutyp.h"

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

/// DLL interface libParx identifiers for Windows.
#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#  ifdef libReloadB0viaDpp_INTERNAL
#    define DLL_INTERFACE __declspec(dllexport)
#  else
#    define DLL_INTERFACE __declspec(dllimport)
#  endif
#elif defined (__GNUC__) && __GNUC__ >= 4
#    define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#    define DLL_INTERFACE
#endif 



/*:=INFO=:*********************************************************
 * 
 * Header : libReloadB0viaDpp.h
 *
 * Description : interface to reload B0 via DPP
 *    
 *:=info=:*********************************************************/

DLL_INTERFACE const char *ReloadB0Init(int psid); 
DLL_INTERFACE const char *ReloadB0(double deltaB0); 
DLL_INTERFACE const char *ReloadB0viaDppInit(int *value);
DLL_INTERFACE const char *ReloadB0viaDpp(int *value);
DLL_INTERFACE void ReloadB0viaDppFinish(void);

DLL_INTERFACE const char *GetNDppShims(int *ndppshims); 
DLL_INTERFACE const char *GetDppShimChannels(SHIM_PAR *channels);

DLL_INTERFACE const char *GetDppShims(int *values);
DLL_INTERFACE const char *GetDppShim(SHIM_PAR channel, int *value);
DLL_INTERFACE const char *GetDppNShimSets(int *nshimsets);
DLL_INTERFACE const char *GetDppShimSets(int *values);
DLL_INTERFACE const char *GetDppShimListIndex(int *shimlistindex);

DLL_INTERFACE const char *ReloadDppShims(int *values);
DLL_INTERFACE const char *ReloadDppShim(SHIM_PAR channel, int *value);
DLL_INTERFACE const char *ReloadDppNShimSets(int *nshimsets);
DLL_INTERFACE const char *ReloadDppShimSets(int *values);
DLL_INTERFACE const char *ReloadDppShimListIndex(int *shimlistindex);

DLL_INTERFACE const char *GetDppNRotMat(int *ndpprotmat);
DLL_INTERFACE const char *GetDppRotMatList(double *values);
DLL_INTERFACE const char *ReloadDppNRotMat(int *ndpprotmat);
DLL_INTERFACE const char *ReloadDppRotMatList(double *values);
#endif
