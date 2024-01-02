/****************************************************************
 *
 * Copyright (c) 2003 - 2019
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVNL_MATRIX_H
#define PVNL_MATRIX_H

#ifndef CPROTO
#include <memory>
#include <vector>
#endif

#include "Pvnl/PvnlGeneral.h"
#include "Pvnl/PvnlComplex.h"
#include "Pvnl/DLLMacro.h"

#ifndef CPROTO

/** @file
 *  @defgroup PvnlMatrix PvnlMatrix
 *  Matrix implementation, basic algebra and functions.
 */
/*@{*/


/*--------------------------------------------------------------*
 * Definitions
 *--------------------------------------------------------------*/

/* DEFINITION OF BLOCK AND MATRIX TYPE */
/** Definition of block structure where data is stored.
 */
class PvnlBlock
{
public:
    /** create a new PvnlBlock */
    PvnlBlock(pvnlFieldType newFieldType, pvnlDataType newDataType,
            unsigned int allocMem, const std::vector<unsigned int>& newSize);

    /** destroy this PvnlBlock */
    ~PvnlBlock();

    /** get number of elements in this PvnlBlock. */
    unsigned int getNumOfElements() const;

    /** field type of data (PVNL_REAL, PVNL_COMPLEX) */
    pvnlFieldType fieldType;

    /** data type of data (PVNL_SINGLEPREC, PVNL_DOUBLEPREC, PVNL_BYTE) */
    pvnlDataType dataType;

    /** number of dimensions */
    unsigned int dim;

    /** array of sizes for each dimension */
    std::vector<unsigned int> size;
    
    /** array with number of elements in dimensions < actual dimension */
    std::vector<unsigned int> elementNumber;

    /** pointer to data */
    union
    {
        PVNL_FLOAT* floatData;
        PVNL_DOUBLE* doubleData;
        PVNL_CHAR* charData;
    } data;

    /** flag which indicates if the block has allocated own memory for data or if the data pointer has been attached to other memory */
    pvnlBool hasOwnData;

private:
    /** deny copy construcion */
    PvnlBlock(const PvnlBlock&) = delete;

    /** deny copy assignment */
    PvnlBlock& operator=(const PvnlBlock&) = delete;
};

/** A matrix class for use in the ParaVision numeric library.
 *
 * PvnlMatrix represents an n-dimensional matrix with access to real or complex
 * data of type float, double or char.
 *
 * Default-constructed PvnlMatrix instances are empty (equivalent to the
 * previous <code>PvnlMatrix* newMatrix = NULL</code>).
 *
 * Use one of the PvnlMatrixNew functions to initialize a matrix (dimension,
 * sizes, element type) and optionally allocate memory for it. If allocated, the
 * memory will be automatically freed by the PvnlMatrix object, there is no need
 * to call PvnlMatrixDelete any more.
 *
 * If no memory has been allocated in PvnlMatrixNew, external data that matches
 * the PvnlMatrix size can be attached via PvnlMatrixAttachMem. In this case,
 * the caller must take care of freeing the external memory.
 *
 * Multiple PvnlMatrix objects can share the same data, where the different
 * PvnlMatrix "views" on the data may have different sizes (sub-matrix view).
 * Use PvnlMatrixGenView to initialize a PvnlMatrix with a view on an existing
 * PvnlMatrix. The memory used will be freed automatically once the last
 * PvnlMatrix object sharing the data is destroyed.
 *
 * Thread-safety: NOT thread-safe.
 */
class DLL_INTERFACE PvnlMatrix
{
public:
    /** create an empty PvnlMatrix */
    PvnlMatrix();

    /** destroy this PvnlMatrix */
    ~PvnlMatrix();

    /** is this matrix empty (un-initialized) */
    bool empty() const;

    /** reset this matrix to empty (un-initialized) */
    void reset();

    /** get number of elements */
    unsigned int getNumOfElements() const;

    /** compare this matrix' size to another matrix.
     * @param other other matrix
     * @return true, if all sizes are identical.
     */
    pvnlBool compSize(const PvnlMatrix& other) const;

    /** pointer to data block */
    std::shared_ptr<PvnlBlock> dataBlock;

    /** number of visible dimensions */
    unsigned int dim;

    /** array (size: <CODE>dim</CODE>) where visible dimensions relative to dimensions of the data block are listed */
    std::vector<unsigned int> visibleDim;

    /** array  (size: <CODE>dataBlock->dim</CODE>) containing first element in each dimension relative to data block */
    std::vector<unsigned int> start;

    /** array (size: <CODE>dataBlock->dim</CODE>) containing increment in each dimension relative to data block */
    std::vector<unsigned int> step;

    /** array (size: <CODE>dataBlock->dim</CODE>) containing number of elements in each dimension relative to data block */
    std::vector<unsigned int> size;

    /** matrix of pointers */
    std::vector<char*> index;
private:
    /** deny copy construcion */
    PvnlMatrix(const PvnlMatrix&) = delete;

    /** deny copy assignment */
    PvnlMatrix& operator=(const PvnlMatrix&) = delete;
};

/*--------------------------------------------------------------*
 * prototype definition with doxygen documentation
 *--------------------------------------------------------------*/


/* GENERATING NEW MATRICES */

/** Generates matrix structure and allocates memory (if desired).
 *  @param newMatrix PvnlMatrix to initialize.
 *  @param newFieldType field type of new matrix.
 *  @param newDataType data type of new matrix.
 *  @param allocMem (<CODE>pvnlBool</CODE> variable indicating if memory for this matrix shall be allocated or not (possible values: <CODE>PVNL_FALSE</CODE> or <CODE>PVNL_TRUE</CODE>).
 *  @param newDim number of dimensions of new matrix.
 *  @param ... list (<CODE>newDim</CODE> entries) of numbers of elements for each dimension.
 *  @throws PVNL_ERROR (to PvnlErrorHandler) or exception on error
 */
DLL_INTERFACE void PvnlMatrixNew (PvnlMatrix& newMatrix, pvnlFieldType newFieldType, pvnlDataType newDataType, pvnlBool allocMem, unsigned int newDim, ...);

/** Generates matrix structure and allocates memory (if desired).
 *  @param newMatrix PvnlMatrix to initialize.
 *  @param newFieldType field type of new matrix.
 *  @param newDataType data type of new matrix.
 *  @param allocMem (<CODE>pvnlBool</CODE> variable indicating if memory for this matrix shall be allocated or not (possible values: <CODE>PVNL_FALSE</CODE> or <CODE>PVNL_TRUE</CODE>).
 *  @param newDim number of dimensions of new matrix.
 *  @param newSize array (<CODE>newDim</CODE> entries) of numbers of elements for each dimension.
 *  @throws PVNL_ERROR (to PvnlErrorHandler) or exception on error
 */
DLL_INTERFACE void PvnlMatrixNewDirect (PvnlMatrix& newMatrix, pvnlFieldType newFieldType, pvnlDataType newDataType, pvnlBool allocMem, const std::vector<unsigned int>& newSize);

/** Generates matrix structure, allocates memory and sets all matrix entries to zero.
 *  @param newMatrix PvnlMatrix to initialize.
 *  @param newFieldType field type of new matrix.
 *  @param newDataType data type of new matrix.
 *  @param newDim number of dimensions of new matrix.
 *  @param ... list (<CODE>newDim</CODE> entries) of numbers of elements for each dimension.
 *  @throws PVNL_ERROR (to PvnlErrorHandler) or exception on error
 */
DLL_INTERFACE void PvnlMatrixNewZero (PvnlMatrix& newMatrix, pvnlFieldType newFieldType, pvnlDataType newDataType, unsigned int newDim, ...);

/** Attaches memory to the data pointer in the corresponding <CODE>dataBlock</CODE>. (IMPORTANT: The matrix is required to have been generated using the <CODE>PvnlMatrixNew</CODE> command with the argument <CODE>allocMem</CODE> set to <CODE>PVNL_FALSE</CODE>. Furthermore the size and the type of the memory attached must meet the descriptions in the corresponding <CODE>dataBlock</CODE>.)
 *  @param m PvnlMatrix to which to attach the memory.
 *  @param memPtr pointer on memory.
 *  @param memSize size (in bytes) of the memory block <CODE>memPtr</CODE> points to.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixAttachMem (PvnlMatrix& m, void* memPtr, unsigned int memSize);

/** Generates matrix structure looking onto a subset of an existing matrix.
 *  @param dest PvnlMatrix to initialize.
 *  @param source already existing matrix.
 *  @param ... list describing subset (start, step and stop values for each dimension)
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixGenView(PvnlMatrix& dest, const PvnlMatrix& source, ...);

/** Generates matrix structure looking onto a subset of an existing matrix.
 *  @param dest PvnlMatrix to initialize.
 *  @param source already existing matrix.
 *  @param start array (<CODE>source->dim</CODE> entries) describing start value for each dimension.
 *  @param step array (<CODE>source->dim</CODE> entries) describing step value for each dimension.
 *  @param stop array (<CODE>source->dim</CODE> entries) describing stop value for each dimension.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixGenViewDirect(PvnlMatrix& dest, const PvnlMatrix& source, 
        const std::vector<unsigned int> start,
        const std::vector<unsigned int> step,
        const std::vector<unsigned int> stop);

/** Generates matrix of pointers referring to data. Returns PVNL_SUCCESS if index already exists. Usage: (1) call PvnlMatrixGenIndex(m); (2) make cast, in three dimensional case: PVNL_DOUBLE*** mIndex = (PVNL_DOUBLE***)m->index; (3) access elements, in three dimensional case: PVNL_DOUBLE element = mIndex[1][2][3], (4) remove index by calling PvnlMatrixRemoveIndex(m) (this is optional - index is removed on matrix deletion).
 *  @param m matrix for which to generate the index.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixGenIndex(PvnlMatrix& m);

/** Delete index generated with <CODE>PvnlMatrixGenIndex</CODE>. This function is called by PvnlMatrixDelete(m).
 *  @param m matrix for which to delete the index.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixRemoveIndex(PvnlMatrix& m);


/* SETTING AND GETTING VALUES */

/** Sets the value of an entry of a real matrix.
 *  @param m matrix in which to set the value.
 *  @param value real value to set.
 *  @param ... list of coordinates where to set the value.
 */
DLL_INTERFACE void PvnlMatrixSetReal (PvnlMatrix& m, PVNL_DOUBLE value, ...);

/** Gets the value of an entry of a real matrix.
 *  @param m matrix in which to get the value.
 *  @param ... list of coordinates where to get the value.
 *  @return real value retreived from matrix.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlMatrixGetReal (const PvnlMatrix& m, ...);

/** Sets the value of an entry of a complex matrix.
 *  @param m matrix in which to set the value.
 *  @param value complex value to set.
 *  @param ... list of coordinates where to set the value.
 */
DLL_INTERFACE void PvnlMatrixSetComplex (PvnlMatrix& m, PvnlComplex value, ...);

/** Gets the value of an entry of a complex matrix.
 *  @param m matrix in which to get the value.
 *  @param ... list of coordinates where to get the value.
 *  @return complex value retreived from matrix.
 */
DLL_INTERFACE PvnlComplex PvnlMatrixGetComplex (const PvnlMatrix& m, ...);

/** Sets the values of all entries of a real matrix.
 *  @param m matrix in which to set the values.
 *  @param value real value to set.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixSetAllReal (PvnlMatrix& m, PVNL_DOUBLE value);

/** Sets the values of all entries of a complex matrix.
 *  @param m matrix in which to set the values.
 *  @param value complex value to set.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixSetAllComplex (PvnlMatrix& m, PvnlComplex value);

/** Sets all matrix elements to zero.
 *  @param m matrix in which to set elements.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixSetZero (PvnlMatrix& m);

/** Sets the values of all entries of a matrix from an array. In the array the values have to be shuffled for complex matrices (real, imaginary, real, imaginary, ...)
 *  @param m matrix in which to set the values.
 *  @param values pointer to array where values are stored.
 *  @param arrayMemSize memory size (in bytes) of the array.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixSetAllFromArray (PvnlMatrix& m, const void* values, unsigned int arrayMemSize);

/** Stores the values of all entries of a matrix in an array. In the array the values will be shuffled for complex matrices (real, imaginary, real, imaginary, ...)
 *  @param values pointer to array where values will be stored.
 *  @param m matrix from which to store the values.
 *  @param arrayMemSize memory size (in bytes) of the array.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixStoreAllInArray (PVNL_DOUBLE* values, const PvnlMatrix& m, unsigned int arrayMemSize);

/** Reads the matrix content from an ASCII file.
 *  @param m matrix in which to set the values.
 *  @param fileName name of file where data is stored.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixReadFileAsc (PvnlMatrix& m, const char* fileName);

/** Reads the matrix content from a binary file (32bit signed integer).
 *  @param m matrix in which to set the values.
 *  @param fileName name of file where data is stored.
 *  @param dataFormat input format (0: 32bit signed integer, 1: 16bit signed integer)
 *  @param endian byte order of data in file (0: little endian, 1: big endian)
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixReadFileBin(PvnlMatrix& m, const char* fileName, unsigned int dataFormat, unsigned int endian);

/** Writes the matrix content to an ASCII file.
 *  @param fileName name of file where to store data.
 *  @param m matrix from which to write data.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixWriteFileAsc (const char* fileName, const PvnlMatrix& m);

/** Writes the matrix content to a binary file (32bit signed integer).
 *  @param fileName name of file where to store data.
 *  @param m matrix from which to write data.
 *  @param dataFormat output format (0: 32bit signed integer, 1: 16bit signed integer, 2: 32bit float)
 *  @param endian byte order of data in file to write (0: little endian, 1: big endian)
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixWriteFileBin(const char* fileName, const PvnlMatrix& m, unsigned int dataFormat, unsigned int endian);

/** Writes the matrix content to stdout
 *  @param m matrix from which to write data.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixPrint(const PvnlMatrix& m);

/** Copies data from one matrix into another.
 *  @param result matrix where to copy data to.
 *  @param a matrix where to copy data from.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixCopy (PvnlMatrix& result, const PvnlMatrix& a);



/* RETREIVING BASIC PROPERTIES */

/** Determination of the maximum of absolute matrix values.
 *  @param m matrix.
 *  @return value of maximum.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlMatrixMax (const PvnlMatrix& m);

/** Determination of the minimum of absolute matrix values.
 *  @param m matrix.
 *  @return value of minimum.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlMatrixMin (const PvnlMatrix& m);

/** Determination of the sum over all elements of absolute matrix values.
 *  @param m matrix.
 *  @return value of sum.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlMatrixSum (const PvnlMatrix& m);



/* BASIC ALGEBRA */

/** Elementwise addition of matrices.
 *  @param result matrix where to store the sum.
 *  @param a first addend matrix.
 *  @param b second addend matrix.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixAddElem (PvnlMatrix& result, const PvnlMatrix& a, const PvnlMatrix& b);

/** Elementwise subtraction of matrices.
 *  @param result matrix where to store the difference.
 *  @param a minuend matrix.
 *  @param b subtrahend matrix.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixSubElem (PvnlMatrix& result, const PvnlMatrix& a, const PvnlMatrix& b);

/** Elementwise multiplication of matrices.
 *  @param result matrix where to store the product.
 *  @param a first factor matrix.
 *  @param b second factor matrix.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixMultElem (PvnlMatrix& result, const PvnlMatrix& a, const PvnlMatrix& b);

/** Elementwise division of matrices.
 *  @param result matrix where to store the quotient.
 *  @param a dividend matrix.
 *  @param b divisor matrix.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixDivElem (PvnlMatrix& result, const PvnlMatrix& a, const PvnlMatrix& b);

/** Add real value to each element of matrix.
 *  @param result matrix where to store the sum.
 *  @param a matrix addend.
 *  @param r real addend.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixAddRealConst (PvnlMatrix& result, const PvnlMatrix& a, PVNL_DOUBLE r);

/** Subtract real value from each element of matrix.
 *  @param result matrix where to store the difference.
 *  @param a matrix minuend.
 *  @param r real subtrahend.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixSubRealConst (PvnlMatrix& result, const PvnlMatrix& a, PVNL_DOUBLE r);

/** Scale matrix with real value.
 *  @param result matrix where to store the product.
 *  @param a matrix.
 *  @param r real scaling factor.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixScaleReal (PvnlMatrix& result, const PvnlMatrix& a, PVNL_DOUBLE r);

/** Add complex value to each element of matrix.
 *  @param result matrix where to store the sum.
 *  @param a complex matrix addend.
 *  @param c complex addend.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixAddComplexConst (PvnlMatrix& result, const PvnlMatrix& a, PvnlComplex c);

/** Subtract complex value from each element of matrix.
 *  @param result matrix where to store the difference.
 *  @param a complex matrix minuend.
 *  @param c complex subtrahend.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixSubComplexConst (PvnlMatrix& result, const PvnlMatrix& a, PvnlComplex c);

/** Scale matrix with complex value.
 *  @param result matrix where to store the product.
 *  @param a complex matrix.
 *  @param c complex scaling factor.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixScaleComplex (PvnlMatrix& result, const PvnlMatrix& a, PvnlComplex c);

/** Elementwise inversion of matrices.
 *  @param result matrix where to store the inverted matrix values.
 *  @param a matrix in which to invert the values.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixInvElem (PvnlMatrix& result, const PvnlMatrix& a);

/** Calculate absolute values of matrix entries.
 *  @param result matrix where to store the absolute values.
 *  @param a matrix from which to calculate the absolute values.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixAbs (PvnlMatrix& result, const PvnlMatrix& a);

/** Calculate square roots of matrix entries.
 *  @param result matrix where to store the square roots.
 *  @param a matrix from which to calculate the square roots.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixSqrt (PvnlMatrix& result, const PvnlMatrix& a);

/** Elementwise phase difference of complex matrices.
 *  @param result real matrix where to store the phase difference.
 *  @param a phase minuend matrix.
 *  @param b phase subtrahend matrix.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixArgDiffElem (PvnlMatrix& result, const PvnlMatrix& a, const PvnlMatrix& b);


/* LINEAR ALGEBRA FUNCTIONALITY */

/** Transposition of 2D-matrix (no conjugation for complex matrices).
 *  @param result transposed matrix (size: n x m).
 *  @param a matrix which to transpose (size: m x n).
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixTrans (PvnlMatrix& result, const PvnlMatrix& a);

/** Transposition of 2D-matrix (with conjugation for complex matrices).
 *  @param result transposed matrix (size: n x m).
 *  @param a matrix which to transpose (size: m x n).
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixTransComplex (PvnlMatrix& result, const PvnlMatrix& a);

/** Calculation of matrix product (2D-matrix with 2D-matrix or 2D-matrix with 1D-matrix (from left or right)).
 *  @param prod matrix where to store matrix product
 *  @param a first factor matrix.
 *  @param b second factor matrix.
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixMult (PvnlMatrix& prod, const PvnlMatrix& a, const PvnlMatrix& b);

/** Calculation of FT of complex double precision matrix.
 *  @param m matrix on which to perform FT
 *  @param procDim dimension in which the FT is processed
 *  @param direction (-1 for forward and +1 for inverse FFT)
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixFT (PvnlMatrix& m, unsigned int procDim, int direction);

/** Calculation of FFT of complex double precision matrix.
 *  @param m matrix on which to perform FFT
 *  @param procDim dimension in which the FFT is processed (number of elements in this direction MUST be integer power of 2).
 *  @param direction (-1 for forward and +1 for inverse FFT)
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixFFT (PvnlMatrix& m, unsigned int procDim, int direction);

/** FFTShift of matrix.
 *  @param m matrix on which to perform FFTShift.
 *  @param procDim dimension in which the FFTShift is processed (number of elements in this direction MUST be integer power of 2).
 *  @return error code.
 */
DLL_INTERFACE int PvnlMatrixFFTShift (PvnlMatrix& m, unsigned int procDim);

/** Complex singular value decomposition (SVD)
 *  @param u complex matrix (m x m) to fill with the left unitary matrix
 *  @param s real vector (n) to fill with the singular values
 *  @param v complex matrix (n x n) to fill with the right unitary matrix
 *  @param a complex matrix (m x n, m>=n) on which to perform the SVD
 *  @return error code
 */
DLL_INTERFACE int PvnlMatrixSvd (PvnlMatrix& u, PvnlMatrix& s, PvnlMatrix& v, const PvnlMatrix& a);

/** Calculation of complex pseudo inverse (condition m>=n)
 *  @param result complex matrix (n x m) where to store the pseudoinverse
 *  @param a complex matrix (m x n, m>=n) of which to calculate the pseudoinverse (CAUTION: a is changed during the calculation!)
 *  @return error code
 */
DLL_INTERFACE int PvnlMatrixPinv (PvnlMatrix& result, PvnlMatrix& a);

/** Solves the system <CODE>Ax=b</CODE> by means of a pseudoinverse.
 *  @param resultVect complex vector <CODE>x</CODE> (n) where to store the solution
 *  @param a matrix <CODE>A</CODE> (m x n, m>=n) of which to calculate the pseudoinverse
 *  @param inVect vector <CODE>b</CODE> (m)
 *  @return error code
 */
DLL_INTERFACE int PvnlMatrixSolvePinv (PvnlMatrix& resultVect, const PvnlMatrix& a, const PvnlMatrix& inVect);

/*@}*/

#endif // CPROTO

#endif // PVNL_MATRIX_H

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
