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

#ifndef PVNL_LINALG_H
#define PVNL_LINALG_H

#include "Pvnl/PvnlGeneral.h"
#include "Pvnl/PvnlError.h"
#include "Pvnl/DLLMacro.h"

/** @file
 *  @defgroup PvnlLinalg PvnlLinalg
 *  Provides basic linear algebra routings (partially based on LAPACK).
 *
 *  @note There are two different conventions concerning memory storage of a matrix as a 2D array.
 *        - Row-major order, as used in C.\n
 *          The column elements of a matrix row immediately follow each other in the memory.\n
 *          Functions using this order end with a capital <CODE>C</CODE>.
 *        - Column-major order, as used in Fortran, LAPACK, Matlab.\n
 *          The row elements of a matrix column immediately follow each other in the memory.\n
 *          This is the default convention used if the function is not marked with a capital <CODE>C</CODE>.
 */
/*@{*/




/*--------------------------------------------------------------*
 * Definitions
 *--------------------------------------------------------------*/

/*--------------------------------------------------------------*
 * prototype definition with doxygen documentation
 *--------------------------------------------------------------*/

/** Multiplies complex matrices ( <CODE>C = A * B</CODE>).
 *  Matrices must be ordered column after column in interleaved format (real complex real complex ...) .
 *  @param C resulting product matrix (size: <CODE>m</CODE> x  <CODE>n</CODE>).
 *  @param A first matrix factor (size: <CODE>m</CODE> x  <CODE>k</CODE>).
 *  @param B second matrix factor (size: <CODE>k</CODE> x  <CODE>n</CODE>).
 *  @param m number of rows of matrix <CODE>C</CODE> and matrix <CODE>A</CODE> (in units of complex numbers !).
 *  @param n number of columns of matrix <CODE>C</CODE> and matrix <CODE>B</CODE> (in units of complex numbers !).
 *  @param k number of columns in matrix <CODE>A</CODE> and number of rows in matrix <CODE>B</CODE> (in units of complex numbers !).
 */
DLL_INTERFACE void PvnlLinalgMatMultComplex(PVNL_DOUBLE* C, PVNL_DOUBLE* A, PVNL_DOUBLE* B, long m, long n, long k);

/** Multiplies real matrices ( <CODE>C = A * B</CODE>).
 *  Matrices must be ordered column after column .
 *  @param C resulting product matrix (size: <CODE>m</CODE> x  <CODE>n</CODE>).
 *  @param A first matrix factor (size: <CODE>m</CODE> x  <CODE>k</CODE>).
 *  @param B second matrix factor (size: <CODE>k</CODE> x  <CODE>n</CODE>).
 *  @param m number of rows of matrix <CODE>C</CODE> and matrix <CODE>A</CODE>.
 *  @param n number of columns of matrix <CODE>C</CODE> and matrix <CODE>B</CODE>.
 *  @param k number of columns in matrix <CODE>A</CODE> and number of rows in matrix <CODE>B</CODE> .
 */
DLL_INTERFACE void PvnlLinalgMatMultReal(PVNL_DOUBLE* C, PVNL_DOUBLE* A, PVNL_DOUBLE* B, long m, long n, long k);

/** Calculates the least square solution <CODE>C</CODE> of a complex linear system  <CODE>A * C = B</CODE>.
 *  (matrices have to be in interleaved format (real complex real complex ...).
 *  The system has to be equally or overdetermined, i.e. <CODE>m</CODE> >= <CODE>n</CODE>
 *  @param A <CODE>m</CODE> x <CODE>n</CODE> matrix.
 *  @param B <CODE>m</CODE> x <CODE>nrhs</CODE> matrix.
 *  @param C <CODE>n</CODE> x <CODE>nrhs</CODE> matrix (solution matrix).
 *  @param m number of rows of the system (in units of complex numbers !).
 *  @param n number columns of <CODE>A</CODE> and number of rows of <CODE>C</CODE> (in units of complex numbers !).
 *  @param nrhs number of columns on the right hand side (matrix <CODE>B</CODE>) (in units of complex numbers !).
 *  @param rcond threshold for treating singular values as zero (if negative, machine precision is used).
 *  @return error code.
 */
DLL_INTERFACE int PvnlLinalgSolvePinvSvdComplex(PVNL_DOUBLE* A, PVNL_DOUBLE* B, PVNL_DOUBLE* C, long m, long n, long nrhs, PVNL_DOUBLE rcond);

/** Calculates the least square solution <CODE>C</CODE> of a real valued linear system  <CODE>A * C = B</CODE>.
 *  The system has to be equally or overdetermined, i.e. <CODE>m</CODE> >= <CODE>n</CODE>
 *  @param A <CODE>m</CODE> x <CODE>n</CODE> matrix.
 *  @param B <CODE>m</CODE> x <CODE>nrhs</CODE> matrix.
 *  @param C <CODE>n</CODE> x <CODE>nrhs</CODE> matrix (solution matrix).
 *  @param m number of rows of the system (in units of complex numbers !).
 *  @param n number columns of <CODE>A</CODE> and number of rows of <CODE>C</CODE> (in units of complex numbers !).
 *  @param nrhs number of columns on the right hand side (matrix <CODE>B</CODE>) (in units of complex numbers !).
 *  @param rcond threshold for treating singular values as zero (if negative, machine precision is used).
 *  @param chi if a pointer != NULL is specified, then the remaining difference
 *             is stored in the chi per rhs. A value of 0.0 is returned if
 *             the system is not overdetermined or if
 *             the rank of  <CODE>A</CODE> is less than  <CODE>n</CODE> .
 *  @return error code.
 */
DLL_INTERFACE int PvnlLinalgSolvePinvSvdReal(PVNL_DOUBLE* A, PVNL_DOUBLE* B, PVNL_DOUBLE* C,
			       long m, long n, long nrhs, PVNL_DOUBLE rcond,
			       PVNL_DOUBLE* chi);



/** Calculates the least square solution <CODE>C</CODE> of a real valued linear system  <CODE>A * C = B</CODE>.
 *  The system has to be equally or overdetermined, i.e. <CODE>m</CODE> >= <CODE>n</CODE>
 *  @param A <CODE>m</CODE> x <CODE>n</CODE> matrix.
 *  @param B <CODE>m</CODE> x <CODE>nrhs</CODE> matrix.
 *  @param C <CODE>n</CODE> x <CODE>nrhs</CODE> matrix (solution matrix).
 *  @param m number of rows of the system (in units of complex numbers !).
 *  @param n number columns of <CODE>A</CODE> and number of rows of <CODE>C</CODE> (in units of complex numbers !).
 *  @param nrhs number of columns on the right hand side (matrix <CODE>B</CODE>) (in units of complex numbers !).
 *  @param rcond threshold for treating singular values as zero (if negative, machine precision is used).
 *  @param chi if a pointer != NULL is specified, then the remaining difference
 *             is stored in the chi per rhs.  A value of 0.0 is returned if
 *             the system is not overdetermined or if
 *             the rank of  <CODE>A</CODE> is less than  <CODE>n</CODE> .
 *  @param rank rank of the pseudoinverse
 *  @param S if a pointer != NULL is specified, then S stores the
 *         <CODE>rank</CODE> singular values (size <CODE>n</CODE>,
 *         <CODE>S[i]=0.0</CODE> for
 *         <CODE>i</CODE> >= <CODE>rank </CODE> <= <CODE>n</CODE>)
 *  @param V if a pointer != NULL, the transposed
 *          <CODE>n</CODE> x <CODE>n</CODE>
 *         right hand singular eigenvector matrix  is stored
 * *       in column major order
 *
 *  @return error code.
 */
DLL_INTERFACE int PvnlLinalgSolvePinvSvdRealSV(PVNL_DOUBLE* A,
                                 PVNL_DOUBLE* B,
                                 PVNL_DOUBLE* C,
                                 long m,
                                 long n,
                                 long nrhs,
                                 PVNL_DOUBLE rcond,
                                 PVNL_DOUBLE* chi,
                                 long *const rank,
                                 PVNL_DOUBLE* S,
                                 PVNL_DOUBLE* V);



/** Returns SVD (singular value decomposition) and pseudoinverse of a real valued matrix <CODE>A</CODE>.
 *  \f[ A = U S V^{T}\f]\n
 *  \f[ A^{+} = V S^{-1} U^{T}\f]\n
 *  @param A      On entry, <CODE>m</CODE> x <CODE>n</CODE> matrix with <CODE>m >= n</CODE>.\n
 *                On exit, <CODE>n</CODE> x <CODE>m</CODE> pseudo inverse matrix <CODE>A+</CODE>.
 *  @param S      On exit, <CODE>n</CODE> element vector of singular values.
 *  @param U      On exit, <CODE>m</CODE> x <CODE>n</CODE> (eco==0:<CODE>m</CODE> x <CODE>m</CODE>) left orthogonal matrix.
 *  @param VT     On exit, <CODE>n</CODE> x <CODE>n</CODE> transposed right orthogonal matrix containing singular vectors.
 *  @param m      number of rows of <CODE>A</CODE>.
 *  @param n      number columns of <CODE>A</CODE>.
 *  @param rcond  threshold for truncating singular values (if negative, machine precision is used).
 *  @param type   type of algorithm: \n
 *                0 = slower, but requires less memory, \n
 *                1 = faster, but requires more memory.
 *  @param eco    economic version (eco != 0) of U
 *  @return error code.
 *  @note <CODE>A,A+,U,VT</CODE> are stored in column major order.
 */

DLL_INTERFACE int PvnlLinalgPinvSvdVSUReal(PVNL_DOUBLE* A,
                             PVNL_DOUBLE* S,
                             PVNL_DOUBLE* U,
                             PVNL_DOUBLE* VT,
                             long m,
                             long n,
                             PVNL_DOUBLE rcond,
                             unsigned long type,
                             unsigned eco);


/** Calculates the pseudo inverse of a real valued matrix <CODE>A</CODE> using singular value decomposition (SVD).
 *  @param A      On entry, <CODE>m</CODE> x <CODE>n</CODE> matrix with <CODE>m >= n</CODE>.\n
 *                On exit, <CODE>n</CODE> x <CODE>m</CODE> pseudo inverse matrix.
 *  @param m      number of rows of <CODE>A</CODE>.
 *  @param n      number columns of <CODE>A</CODE>.
 *  @param rcond  threshold for truncating singular values (if negative, machine precision is used).
 *  @param type   type of algorithm: \n
 *                0 = slower, but requires less memory, \n
 *                1 = faster, but requires more memory.
 *  @return error code.
 *  @note Uses C-like row-major order for array storage.
 */
DLL_INTERFACE int PvnlLinalgPinvSvdRealC(PVNL_DOUBLE* A, long m, long n, PVNL_DOUBLE rcond, unsigned long type);

/*@}*/


#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
