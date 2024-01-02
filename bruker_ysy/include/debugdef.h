/****************************************************************
 *
 * Copyright (c) 1995-2018
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

#ifndef DEBUGDEF_H
#define DEBUGDEF_H

#include <assert.h>

#ifndef CPROTO
#include <sstream>
#endif
#include "libLogger/DLLMacro.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Set DBMSG logger output to a file are standard output.
     * @param fname If \a fname is a file the trace output is stored in the file.
     *    if \a fname is \c 0 then the trace is print to the standard output.
     */
    DLL_INTERFACE void PvTraceSetFile(const char * fname);
    
    /**
     * Print the current stack trace to the standard output.
     * The function \a PvTracePrintStacktrace is also on the stack trace.
     */
    DLL_INTERFACE void PvTracePrintStacktrace(void);
    
    /**
     * Indicates if the DBMSG trace logger would really log.
     * @return Indication if the trace logger would output anything.
     */
    DLL_INTERFACE bool PvTraceWouldLog();
    
    /**
     * Output the trace messages using the DBMSG logger.
     * The interface is a printf(3) interface.
     * @param fname The filename where the message was generated.
     * @param line The line number where the message was generated.
     * @param format The printf(3) format.
     * @param ... A printf(3) argument list.
     */
    DLL_INTERFACE void PvTraceOutput(const char * fname, int line,
        const char * format, ...)
#ifdef	__GNUC__
#if	__GNUC__ >= 3
    __attribute__ ((format (printf, 3, 4)))
#endif
#endif
    ;
  
#ifdef USE_PVTRACE_VINTERFACE
    
#include <stdarg.h>
    /**
     * Output the trace messages using the DBMSG logger.
     * The interface is a vprintf(3) interface.
     * @param fname The filename where the message was generated.
     * @param line The line number where the message was generated.
     * @param format The vprintf(3) format.
     * @param ap The vprintf(3) argument list.
     */
    DLL_INTERFACE void PvTraceVOutput(const char * fname, int line,
        const char *format, va_list ap);
#endif

#ifdef __cplusplus
}
#endif

/*--------------------------------------------------------------*
 * If the NDEBUG symbol is defined
 * - define an ASSERT macro that won't produce any code
 * - define a DB_MSG macro that won't produce any code
 *--------------------------------------------------------------*/

#ifdef NDEBUG
#  undef ASSERT
#  define ASSERT(EX) ((void)0)

#  undef DB_MSG
#  define DB_MSG(msg) ((void)0)

#  undef DB_MSG_S
#  define DB_MSG_S(msg) ((void)0)

#else /* !NDEBUG */

/*
  Define an ASSERT macro that creates a stack trace.
*/
#  ifdef HAS_WINDOWS_FEATURES
#    define ASSERT assert
#  else /* !HAS_WINDOWS_FEATURES */
#    define ASSERT(d) do { if (!(d)) { PvTracePrintStacktrace(); assert(d); } } while(0)
#  endif /* HAS_WINDOWS_FEATURES */

/*
  The following macro DB_MSG use the
  following definitions. If they are not initialized set them to 0.
*/

#  ifndef DEBUG
#    define DEBUG 0
#  endif


/*
  Define the DB_MSG macro to print the debugging message
  if DEBUG is 1 and NDEBUG is not defined.
*/
#  if DEBUG
#    undef DB_MSG
#    define DREF(...) __VA_ARGS__
#    define DB_MSG(msg) do { if (PvTraceWouldLog()) PvTraceOutput(__FILE__, __LINE__, DREF msg); } while(0)
#    define DB_MSG_S(msg) do { if (PvTraceWouldLog()) { std::ostringstream _os; _os msg; PvTraceOutput(__FILE__, __LINE__, "%s", _os.str().c_str()); } } while(0)
#  else /* !DEBUG */
#    undef DB_MSG
#    define DB_MSG(msg) ((void)0)
#    define DB_MSG_S(msg) ((void)0)
#  endif


#endif /* !NDEBUG */

#endif /* !DEBUGDEF_H */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
