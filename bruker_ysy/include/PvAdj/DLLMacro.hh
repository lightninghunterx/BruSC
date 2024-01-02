//==============================================================================
// $Source$
//   File to create __dllspec(dllimport) 
//
// Copyright (c) 2015
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//==============================================================================

#if defined(DLL_INTERFACE)
#   undef DLL_INTERFACE
#endif

#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#   ifdef libPvAdj_INTERNAL
#    define DLL_INTERFACE __declspec(dllexport)
#  else
#    define DLL_INTERFACE __declspec(dllimport)
#  endif
#elif defined (__GNUC__) && __GNUC__ >= 4
#    define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#    define DLL_INTERFACE
#endif 

//==========================================================================EOF=

