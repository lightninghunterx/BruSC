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

#ifndef PVNL_COMPLEX_H
#define PVNL_COMPLEX_H

#include "Pvnl/PvnlGeneral.h"
#include "Pvnl/DLLMacro.h"

/** @file
 *  @defgroup PvnlComplex PvnlComplex
 *  Complex number implementation, basic algebra and functions.
 */
/*@{*/


/*--------------------------------------------------------------*
 * Definitions
 *--------------------------------------------------------------*/


/* DEFINITION OF COMPLEX NUMBER TYPE */

/** Definition of complex number type. */
typedef struct
{
  /** Real part of complex number. */
  PVNL_DOUBLE real;
  /** Imaginary part of complex number. */
  PVNL_DOUBLE imag;
}
PvnlComplex;


/*--------------------------------------------------------------*
 * prototype definition with doxygen documentation
 *--------------------------------------------------------------*/


/* GENERATING COMPLEX NUMBERS */

/** Generates a complex number from real and imaginary part.
 *  @param x real part of complex number.
 *  @param y imaginary part of complex number.
 *  @return complex number <CODE>(x + i*y)</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexCart (PVNL_DOUBLE x, PVNL_DOUBLE y);

/** Generates a complex number from modulus and argument.
 *  @param r modulus of complex number.
 *  @param phi argument of complex number (in radians).
 *  @return complex number <CODE>(r*cos(phi) + i*r*sin(phi))</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexPolar (PVNL_DOUBLE r, PVNL_DOUBLE phi);



/* RETREIVING BASIC PROPERTIES */

/** Returns the real part of a complex number.
 *  @param z complex number.
 *  @return real part <CODE>x</CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlComplexReal (PvnlComplex z);

/** Returns the imaginary part of a complex number.
 *  @param z complex number.
 *  @return imaginary part <CODE>y</CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlComplexImag (PvnlComplex z);

/** Returns the modulus of a complex number.
 *  @param z complex number.
 *  @return modulus <CODE>r</CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlComplexAbs (PvnlComplex z);

/** Returns the square modulus of a complex number.
 *  @param z complex number.
 *  @return square modulus <CODE>r^2</CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlComplexAbs2 (PvnlComplex z);

/** Returns the logarithm of the modulus of a complex number.
 *  @param z complex number.
 *  @return logarithm of modulus <CODE>log(r)</CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlComplexLogAbs (PvnlComplex z);

/** Returns the argument of a complex number.
 *  @param z complex number.
 *  @return argument <CODE>phi</CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlComplexArg (PvnlComplex z);



/* BASIC ARITHMETICS */

/** Addition of two complex numbers.
 *  @param a first addend.
 *  @param b second addend.
 *  @return sum <CODE>a+b</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexAdd (PvnlComplex a, PvnlComplex b);

/** Subtraction of two complex numbers.
 *  @param a minuend.
 *  @param b subtrahend.
 *  @return difference <CODE>a-b</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexSub (PvnlComplex a, PvnlComplex b);

/** Multiplication of two complex numbers.
 *  @param a first factor.
 *  @param b second factor.
 *  @return product <CODE>a*b</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexMult (PvnlComplex a, PvnlComplex b);

/** Division of two complex numbers.
 *  @param a dividend.
 *  @param b divisor.
 *  @return quotient <CODE>a/b</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexDiv (PvnlComplex a, PvnlComplex b);

/** Add real number to complex number.
 *  @param a complex addend.
 *  @param x real addend.
 *  @return complex sum <CODE>a+x</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexAddReal (PvnlComplex a, PVNL_DOUBLE x);

/** Subtraction of real number from complex number.
 *  @param a complex minuend.
 *  @param x real subtrahend.
 *  @return complex difference <CODE>a-x</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexSubReal (PvnlComplex a, PVNL_DOUBLE x);

/** Multiplication of complex with real number.
 *  @param a complex factor.
 *  @param x real factor.
 *  @return complex product <CODE>a*x</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexMultReal (PvnlComplex a, PVNL_DOUBLE x);

/** Division of complex number by real number.
 *  @param a complex dividend.
 *  @param x real divisor.
 *  @return quotient <CODE>a/x</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexDivReal (PvnlComplex a, PVNL_DOUBLE x);

/** Add imaginary number to complex number.
 *  @param a complex addend.
 *  @param y imaginary addend.
 *  @return complex sum <CODE>a+i*y</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexAddImag (PvnlComplex a, PVNL_DOUBLE y);

/** Subtraction of imaginary number from complex number.
 *  @param a complex minuend.
 *  @param y imaginary subtrahend.
 *  @return complex difference <CODE>a-i*y</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexSubImag (PvnlComplex a, PVNL_DOUBLE y);

/** Multiplication of complex with imaginary number.
 *  @param a complex factor.
 *  @param y imaginary factor.
 *  @return complex product <CODE>a*i*y</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexMultImag (PvnlComplex a, PVNL_DOUBLE y);

/** Division of complex number by imaginary number.
 *  @param a complex dividend.
 *  @param y imaginary divisor.
 *  @return quotient <CODE>a/(i*y)</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexDivImag (PvnlComplex a, PVNL_DOUBLE y);

/** Calculation of complex conjugate.
 *  @param a complex number.
 *  @return complex conjugate <CODE>a*</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexConj (PvnlComplex a);

/** Calculation of inverse value.
 *  @param a complex number.
 *  @return inverse value <CODE>a^(-1)</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexInv (PvnlComplex a);

/** Calculation of negative value.
 *  @param a complex number.
 *  @return negative value <CODE>-a</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexNeg (PvnlComplex a);


/* BASIC COMPLEX FUNCTIONS */

/** Calculation of complex square root.
 *  @param a complex number.
 *  @return complex square root <CODE>sqrt(a)</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexSqrt (PvnlComplex a);

/** Calculation of complex power of complex number.
 *  @param a complex basis.
 *  @param b complex exponent.
 *  @return complex power <CODE>a^b</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexPow (PvnlComplex a, PvnlComplex b);

/** Calculation of real power of complex number.
 *  @param a complex basis.
 *  @param x real exponent.
 *  @return power <CODE>a^x</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexPowReal (PvnlComplex a, PVNL_DOUBLE x);

/** Calculation of complex exponential.
 *  @param a complex number.
 *  @return complex exponential <CODE>exp(a)</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexExp (PvnlComplex a);

/** Calculation of complex logarithm.
 *  @param a complex number.
 *  @return complex logarithm <CODE>log(a)</CODE>.
 */
DLL_INTERFACE PvnlComplex PvnlComplexLog (PvnlComplex a);

/* Interface operating on floating point arrays */

/** Multiplication of two complex numbers.
 *  @param c product <CODE>a*b</CODE>.
 *  @param a first factor.
 *  @param b second factor.
 */
DLL_INTERFACE void PvnlComplexMult2 (PVNL_DOUBLE* c, PVNL_DOUBLE* a, PVNL_DOUBLE* b);

/** Multiplication of two complex numbers conjugate factor a.
 *  @param c product <CODE>a*b</CODE>.
 *  @param a first factor.
 *  @param b second factor.
 */
DLL_INTERFACE void PvnlComplexMultConj2 (PVNL_DOUBLE* c, PVNL_DOUBLE* a, PVNL_DOUBLE* b);


/*@}*/


#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
