/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2004
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVNL_ULP_H
#define PVNL_ULP_H

#include "Pvnl/DLLMacro.h"

/** @file
 *  @defgroup PvnlUtil PvnlUtil
 *  Some functions to calculate the size of an ulp (Unit in the Last Place).
 */
/*@{*/

/*--------------------------------------------------------------*
 * prototype definition with doxygen documentation
 *--------------------------------------------------------------*/

/**
 * Get the next smaller representable float value for the given float value.
 *
 * @param f The float value for which the next smaller representable
 *          float value should be returned.
 *
 * @return  The next smaller representable float value for the given
 *          float value.
 */
DLL_INTERFACE float prevFloat(float f);

/**
 * Get the next larger representable float value for the given float value.
 *
 * @param f The float value for which the next larger representable
 *          float value should be returned.
 *
 * @return  The next larger representable float value for the given
 *          float value.
 */
DLL_INTERFACE float nextFloat(float f);

/**
 * Get the size of an ulp (Unit in the Last Place) of the argument.
 * An ulp of a float value is the positive distance between this
 * floating-point value and the float value next larger in magnitude.
 * Note that for non-NaN x, ulp(-x) == ulp(x).
 *
 * @param f The floating-point value whose ulp is to be returned.
 *
 * @return  The size of an ulp of the argument.
 */
DLL_INTERFACE float ulpf(float f);

/**
 * Get the next smaller representable double value for the given double value.
 *
 * @param d The double value for which the next smaller representable
 *          double value should be returned.
 *
 * @return  The next smaller representable double value for the given
 *          double value.
 */
DLL_INTERFACE double prevDouble(double d);

/**
 * Get the next larger representable double value for the given double value.
 *
 * @param d The double value for which the next larger representable
 *          double value should be returned.
 *
 * @return  The next larger representable double value for the given
 *          double value.
 */
DLL_INTERFACE double nextDouble(double d);

/**
 * Get the size of an ulp (Unit in the Last Place) of the argument.
 * An ulp of a double value is the positive distance between this
 * floating-point value and the double value next larger in magnitude.
 * Note that for non-NaN x, ulp(-x) == ulp(x).
 *
 * @param d The doubleing-point value whose ulp is to be returned.
 *
 * @return  The size of an ulp of the argument.
 */
DLL_INTERFACE double ulpd(double d);

/*@}*/

#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
