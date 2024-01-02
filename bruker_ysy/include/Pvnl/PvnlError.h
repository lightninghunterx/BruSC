/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVNL_ERROR_H
#define PVNL_ERROR_H

#include "Pvnl/DLLMacro.h"

/** @file
 *  @defgroup PvnlError PvnlError
 *  Complex number implementation, basic algebra and functions.
 */
/*@{*/


/*--------------------------------------------------------------*
 * Definitions
 *--------------------------------------------------------------*/

/** Definition of error codes */
enum {
  PVNL_FAILURE  = -1,
  PVNL_SUCCESS  = 0,
  PVNL_E_NOMEM,              /* pvnlMalloc failed */
  PVNL_E_MEM,                /* memory access error */
  PVNL_E_INVAL,              /* invalid input value */
  PVNL_E_WRONGMATTYPE,       /* wrong matrix type */
  PVNL_E_NOMATDATA,          /* matrix block has no data memory */
  PVNL_E_EOF,                /* end of file reached */
  PVNL_E_MATMISMATCH,        /* matrices do not match for desired operation */
  PVNL_E_NOINPLACE,          /* operation cannot work in place */
  PVNL_E_FFTSIZE,            /* FFT not possible with the size of this matrix dimension */
  PVNL_E_WRONGARRAYSIZE,      /* array size(s) are wrong */
  PVNL_E_NOCONVERGE,          /* algorithm failed to converge */
  PVNL_E_WARN                 /* algorithm completed but produced poor results */
};


/*--------------------------------------------------------------*
 * prototype definition with doxygen documentation
 *--------------------------------------------------------------*/


/* Providing infrastructure for replacing error handling functions */

/** Definition of fuction prototype for error handling function */
typedef void pvnlErrorHandlerT (const char* errorText, const char* file, int line, int pvnlErrorCode);

/** Replaces the standard error handler by a user defined one.
 *  @param newHandler pointer to the user defined error handler.\n
 *                    The value NULL will use the default error handler that prints an error message and returns.
 *  @return a pointer to the former error handler.
 */
DLL_INTERFACE pvnlErrorHandlerT* PvnlSetErrorHandler (pvnlErrorHandlerT* newHandler);

/** Replaces the standard error handler by a default handler.
 *  The default error handler prints an error message and returns.
 *  @return a pointer to the former error handler.
 */
DLL_INTERFACE pvnlErrorHandlerT* PvnlSetDefaultErrorHandler (void);

/** Replaces the standard error handler by a handler throwing an exception.
 *  The exception error handler throws a PvStdException with details of the 
 *  error which has occurred.
 *  @return a pointer to the former error handler.
 */
DLL_INTERFACE pvnlErrorHandlerT* PvnlSetExceptionErrorHandler (void);

/** Turns error handling off.
 *  @return a pointer to the former error handling function.
 */
DLL_INTERFACE pvnlErrorHandlerT* PvnlTurnErrorHandlerOff (void);



/*@}*/


#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
