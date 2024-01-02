/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2010 - 2017
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVMACHINE_H
#define PVMACHINE_H


#ifndef CPROTO
#include "machine.h"
#endif

/** Machine / OS specific ParaVison definitions. */


#ifdef HAS_WINDOWS_FEATURES
#define NOMINMAX
#define _USE_MATH_DEFINES
// Suppress C++ exception specification ignored except to indicate a function
// is not __declspec(nothrow)
#pragma warning( disable : 4290 )
// inheritance via dominance warning.
#pragma warning( disable : 4250 )
// conditional expression is constant.
#pragma warning( disable : 4127 )
#endif

#if  defined(__GNUC__) && !defined(CPROTO)
/** Prevent unused parameter warning. */
#  define UNUSED_PARAM __attribute__((unused))
/** Checks printf formats.
    @note: f is the position number of the format argument,
    d is the position number of the ... argument. In functions
    the count begins with 1, i non static c++ methods the count
    begins with 2.
*/
#  define VA_PRINTF_FORMAT(f,d)  __attribute__((format(printf, f, d)))

/** Checks if the last varargs argument is a pointer 0. */
#  if __GNUC__ >= 4
#    define SENTINEL __attribute__((sentinel))
#  else
#    define SENTINEL
#  endif
#else
#  define UNUSED_PARAM
#  define VA_PRINTF_FORMAT(f,d)
#  define SENTINEL
#endif



#ifndef CPROTO
#include "pvidl.h"

#ifdef HAS_WINDOWS_FEATURES
#define CONV_PTR_TO_FCN(f,p) (f)(p)
#define CONV_FCN_TO_VOID_PTR(p) (void *)(p)
#else /* !HAS_WINDOWS_FEATURES */

/** Convert a void * pointer into a function pointer. */
#define CONV_PTR_TO_FCN(f,p) (f)((uintptr_t)(p))

/** Convert a function pointer into a void * pointer. */
#define CONV_FCN_TO_VOID_PTR(p) (void *)((uintptr_t)(p))

#endif /* HAS_WINDOWS_FEATURES */

#ifndef pvmswin_INTERNAL
#include "generated/PvSystem.h"
#ifdef HAS_64BIT_FEATURES
#define DSO_EXT de_bruker_mri_generated_pvsystem_ShareLibExt64
#else /* !HAS_64BIT_FEATURES */
#define DSO_EXT de_bruker_mri_generated_pvsystem_ShareLibExt32
#endif /* HAS_64BIT_FEATURES */
#endif /* pvmswin_INTERNAL */

#ifdef HAS_64BIT_FEATURES
#define INT64_PRINTF_FORMAT "%ld"
#define UINT64_PRINTF_FORMAT "%lu"
#else /* !HAS_64BIT_FEATURES */
#define INT64_PRINTF_FORMAT "%lld"
#define UINT64_PRINTF_FORMAT "%llu"
#endif /* HAS_64BIT_FEATURES */

#endif /* CPROTO */

#if defined(__GNUC__)
#    define DLL_LOCAL __attribute__ ((visibility("hidden")))
#else
#    define DLL_LOCAL
#endif

#define PV_CXX_DELETE = delete
#define PV_CXX_OVERRIDE override

#ifdef HAS_WINDOWS_FEATURES
#include "pvmswin.h"
#endif

#ifdef HAS_WINDOWS_FEATURES
#define OS_FILE_SEP_CHR '\\'
#define OS_FILE_SEP_STR "\\"
#define OS_PATH_SEP_CHR ';'
#define OS_PATH_SEP_STR ";"
#else
#define OS_FILE_SEP_CHR '/'
#define OS_FILE_SEP_STR "/"
#define OS_PATH_SEP_CHR ':'
#define OS_PATH_SEP_STR ":"
#endif

#endif /* ifndef PVMACHINE_H */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
