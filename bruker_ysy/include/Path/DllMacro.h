/*
 *******************************************************************
 *
 * $Source: /bscl/CvsTree/bscl/gen/h/Path/DllMacro.h,v $
 *
 * Copyright (c) 2000
 * BRUKER ANALYTIK GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: DllMacro.h,v 1.1 2000/02/07 17:37:10 ah Exp $
 *
 *******************************************************************
 */

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

#if defined(DLL_LINKAGE) && !defined(CPROTO)
#   ifdef libPath_INTERNAL
#      define DLL_INTERFACE __declspec(dllexport)
#   else
#      define DLL_INTERFACE __declspec(dllimport)
#   endif
#else
#   define DLL_INTERFACE
#endif
