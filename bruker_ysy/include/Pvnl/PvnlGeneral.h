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

#ifndef PVNL_GENERAL_H
#define PVNL_GENERAL_H

#include <float.h>

/** @file
 *  @defgroup PvnlGeneral PvnlGeneral
 *  Basic definitions for the numerical library.
 */
/*@{*/


/*--------------------------------------------------------------*
 * Definitions
 *--------------------------------------------------------------*/


/** Defines an alias for single numerical precision.
*/
#ifndef PVNL_FLOAT
#define PVNL_FLOAT float
#endif

/** Defines an alias for double numerical precision.
*/
#ifndef PVNL_DOUBLE
#define PVNL_DOUBLE double
#endif

/** Defines an alias for one byte data type.
*/
#ifndef PVNL_CHAR
#define PVNL_CHAR char
#endif


/** Defines constant for epsilon in the double precision case.
*/
#ifndef PVNL_DBL_EPSILON
#define PVNL_DBL_EPSILON DBL_EPSILON
#endif

/** Defines constant for the smallest positive floating point number in the double precision case.
*/
#ifndef PVNL_DBL_MIN
#define PVNL_DBL_MIN DBL_MIN
#endif

/** Defines constant for epsilon in the single precision case.
*/
#ifndef PVNL_FLT_EPSILON
#define PVNL_FLT_EPSILON FLT_EPSILON
#endif

/** Defines constant for the smallest positive floating point number in the single precision case.
*/
#ifndef PVNL_FLT_MIN
#define PVNL_FLT_MIN FLT_MIN
#endif

/** Defines weather BLAS should be used for carrying out linear algebra operations
*/
#ifndef PVNL_USE_BLAS
#define PVNL_USE_BLAS
#endif
#undef PVNL_USE_BLAS

/** Defines the different possible field types.
*/
typedef enum {PVNL_REAL=1, PVNL_COMPLEX=2} pvnlFieldType;


/** Defines the different possible data types.
*/
typedef enum {PVNL_SINGLEPREC=1, PVNL_DOUBLEPREC=2, PVNL_BYTE=3} pvnlDataType;

/** Defines a boolean data type
*/
typedef enum {PVNL_FALSE=0, PVNL_TRUE=1} pvnlBool;



/*@}*/


#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
