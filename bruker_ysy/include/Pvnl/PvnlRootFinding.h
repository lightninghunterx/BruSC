/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2015
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVNL_ROOTFINDING_H
#define PVNL_ROOTFINDING_H

#include "Pvnl/PvnlComplex.h"
#include "Pvnl/PvnlGeneral.h"
#include "PvUtil/PvExcept.hh"
#include "Pvnl/DLLMacro.h"

/** @file
 *  @defgroup PvnlRootFinding PvnlRootFinding
 *  Provides functions for finding roots of polynomials
 */
/*@{*/

    /*--------------------------------------------------------------*
     * Definitions
     *--------------------------------------------------------------*/

    /*--------------------------------------------------------------*
     * prototype definition with doxygen documentation
     *--------------------------------------------------------------*/

    /**
     * @ingroup PvnlRootFinding
     * @brief Find roots of polynomial with complex coefficients by calculating
     * the eigenvalues of its companion matrix.
     *
     * This function finds the complex roots of a polynomial of the form
     * \f$ c_n z^n + ... + c_1 z + c_0 = 0\f$, where the coefficients \f$c\f$
     * can be complex, by calculating the eigenvalues of its companion matrix.
     * For details, see "Numerical recipes in C", Second Edition, Cambridge
     * University Press.
     *
     * The eigenvalues are found using the LAPACK function ZGEEV, which first
     * balances the companion matrix to improve its condition.
     *
     * @param n (input/output parameter) As input parameter, n specifies the
     * order of the polynomial. As output parameter, n contains the number of
     * roots found. This can deviate from the order of the polynomial, if some
     * of the first coefficients are (numerically) zero.
     * @param coeffs array of complex polynomial coefficients, its length must
     *        be equal to n+1. The order of the coefficients is
     *        \f$c_n ... c_0\f$
     * @param roots array of complex roots, its length must be equal to the
     *        order of the polynomial. The actual number of roots stored in
     *        this array is equal to the output value of n.
     *
     * @throws PvStdException, when the algorithm fails.
     */
    DLL_INTERFACE void PvnlFindRootsComplexEv(unsigned int &n, const PvnlComplex* coeffs, PvnlComplex* roots);

/*@}*/


#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
