//==============================================================================
// Copyright (c) 2009 - 2018
//
// Bruker BioSpin AG
// MRI Division
// CH-8117 Fällanden, Switzerland
//
// All Rights Reserved
//==============================================================================
#ifndef LIB_CORE_MATH_MATRIX3X3_H
#define LIB_CORE_MATH_MATRIX3X3_H

#include <cstring>
#include <array>
#include "libCore/Containers/Exceptions/MathException.hh"
#include "libCore/Base/TimeStamped.hh"
#include "libCore/Base/internal.hh"

namespace core
{
    // forward
    class Matrix4x4;
    class Matrix3x3;
    class MathException;

    //--------------------------------------------------------------------------
    // Interface of non-member functions
    //--------------------------------------------------------------------------

    /**
     * Insertion operator for @c Matrix3x3 .
     * @param ostream The output stream to write into.
     * @param matrix  A reference to an instance of a 3 by 3 matrix.
     * @return        The output stream.
     */
    DLL_INTERFACE std::ostream &operator<<(std::ostream &ostream, const Matrix3x3 &matrix);

    /**
     * Extraction operator for <code>Matrix3x3</code>.
     * @param istream The input stream to extract from.
     * @param matrix  A reference to an instance of a 3 by 3 matrix.
     * @return        The input stream.
     */
    DLL_INTERFACE std::istream &operator>>(std::istream &istream, Matrix3x3 &matrix);

    //--------------------------------------------------------------------------
    // Interface of class Matrix3x3
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Math/Matrix3x3.hh"
     * This concrete class implements 3x3 matrices.
     * @c Matrix3x3  is a class to represent and manipulate 3x3 matrices.
     *
     * @author Christian Werner
     * @since ParaVision 6.Alpha.0.57
     * @see Matrix4x4
     * @see Transform
     */
    class DLL_INTERFACE Matrix3x3 : public TimeStamped
    {
        friend class Matrix4x4;
        friend std::ostream &operator<<(
            std::ostream &ostream, const Matrix3x3 &matrix);
        friend std::istream &operator>>(
            std::istream &istream, Matrix3x3 &matrix);
    public:
        /**
         * Default constructor.
         * Constructs a 3x3 identity matrix.
         */
        Matrix3x3(void);

        /**
         * Constructor with 9 elements.
         * @param m00 Element in row 0, column 0.
         * @param m01 Element in row 0, column 1.
         * @param m02 Element in row 0, column 2.
         * @param m10 Element in row 1, column 0.
         * @param m11 Element in row 1, column 1.
         * @param m12 Element in row 1, column 2.
         * @param m20 Element in row 2, column 0.
         * @param m21 Element in row 2, column 1.
         * @param m22 Element in row 2, column 2.
         */
        Matrix3x3(double m00, double m01, double m02,
                  double m10, double m11, double m12,
                  double m20, double m21, double m22);

        /**
         * Constructor with a 2D array.
         * @param array Array of three by three elements. The array will be
         *              copied.
         */
        Matrix3x3(const double array[3][3]);

        /**
         * Constructor with an 1D array.
         * @param array Array of nine elements, to be filled into matrix in
         *              row-major order. The array will be copied.
         */
        Matrix3x3(const double *const array);

        /**
         * Copy constructor.
         * Makes a deep copy of the 3x3 <code>rhs</code> matrix object.
         */
        Matrix3x3(const Matrix3x3& rhs);

        /**
         * Cross copy constructor.
         * Makes a deep copy of the upper left nine elements of the 4x4
         * <code>rhs</code> matrix object.
         */
        Matrix3x3(const Matrix4x4& rhs);

        /**
         * Set all elements of the matrix to the given value.
         * @param element Value to set.
         * @return        This instance.
         */
        Matrix3x3 & operator=(double element);

        /**
         * Assignment operator for 3x3 matrix.
         * @param rhs Instance to copy state from.
         * @return    This instance.
         */
        inline Matrix3x3& operator=(const Matrix3x3& rhs)
        {
            if(this == &rhs) return *this;
            memcpy(element_, rhs.element_, 9 * sizeof(double));
            return *this;
        }

        /**
         * Assignment operator for 4x4 matrix.
         * The upper left nine elements of <code>rhs</code>, are copied, the
         * rest is ignored.
         */
        Matrix3x3& operator=(const Matrix4x4& rhs);

        /**
         * Equality comparison operator.
         * @param rhs Instance to compare to.
         * @return    @c true if all elements are equal.
         */
        inline bool operator==(const Matrix3x3& rhs) const
        {
            if (this == &rhs) return true;
            for (size_t i = 0; i < 3; i++)
                for (size_t j = 0; j < 3; j++)
                    if (element_[i][j] != rhs.element_[i][j]) return false;
            return true;
        }

        /** Inequality comparison operator. */
        inline bool operator!=(const Matrix3x3& rhs) const
        {
            return !(*this == rhs);
        }

        /**
         * Get the 3x3 identity matrix.
         * @return The 3x3 identity matrix.
         */
        static const Matrix3x3 &Identity(void);

        /**
         * Row access operator (non-const).
         * @param row A row address, starting at zero.
         * @return    The selected row.
         */
        inline double *operator[](unsigned int row)
        {
            return &(element_[row][0]);
        }

        /**
         * Row access operator (const).
         * @param row A row address, starting at zero.
         * @return    The selected row.
         */
        inline const double *operator[](unsigned int row) const
        {
            return &(element_[row][0]);
        }

        /**
         * Row access function.
         * @param row A row address, starting at zero.
         * @return    The selected row.
         */
        inline const double *getRow(unsigned int row) const
        {
            return &(element_[row][0]);
        }

        /**
         * Matrix inversion. Use transposition instead for orthogonal matrices.
         * Calculated as @f$\mathcal{A}^{-1} = \frac{1}{ \textrm{det}
         * \mathcal{A}} \textrm{adjoint} \mathcal{A}@f$.
         *
         *
         * @param in  The matrix to invert.
         * @param out The inverted matrix.
         */
        static void Invert(const Matrix3x3 &in, Matrix3x3 & out)
            throw(MathException);

        /**
         * In-place matrix inversion. Use transposition instead for orthogonal
         * matrices.
         * Calculated as @f$\mathcal{A}^{-1} = \frac{1}{ \textrm{det}
         * \mathcal{A}} \textrm{adjoint} \mathcal{A}@f$.
         */
        inline void invert(void)
        {
            Invert(*this, *this);
        }

        /**
         * Matrix transposition.
         * @param in  The matrix to transpose.
         * @param out The inverted transposed.
         */
        static void Transpose(const Matrix3x3 &in, Matrix3x3 &out);

        /** In-place transposition. */
        inline void transpose(void)
        {
            Transpose(*this, *this);
        };

        /**
         * Point multiplication from right.
         * Multiply this matrix by a point (in homogeneous coordinates) and
         * return the result. The <code>in[4]</code> and
         * <code>out[4]</code> arrays must both be allocated but they can be
         * the same array.
         * @param in The point to be multiplied from right.
         * @param out The result of the multiplication.
         */
        void multiplyPoint(const double in[4], double out[4]) const;

        /**
         * Point multiplication from right.
         * Multiply this matrix by a point (in homogeneous coordinates) and
         * return the result. The <code>in</code> and
         * <code>out</code> can be the same array.
         * @param in The point that if multiplied from right.
         * @param out The result point.
         */
        void multiplyPoint(const std::array<double, 4> & in,
            std::array<double, 4> & out) const;

        /**
         * Point multiplication from right.
         * Multiply this matrix by a point (3 elements) and return the result.
         * The <code>in[3]</code> and <code>out[3]</code> arrays
         * must both be allocated but they can be the same array.
         * @param in The point to be multiplied from right.
         * @param out The result of the multiplication.
         */
        void multiplyPoint3(const double in[3], double out[3]) const;

        /**
         * Point multiplication from right.
         * Multiply this matrix by a point (3 elements) and return the result.
         * The <code>in</code> and <code>out</code> arrays can be the same array.
         * @param in The point to be multiplied from right.
         * @param out The result of the multiplication.
         */
        void multiplyPoint3(const std::array<double, 3> & in,
            std::array<double, 3> & out) const;

        /**
         * Point multiplication from left.
         * Multiply a point (in homogeneous coordinates) by this matrix,
         * and return the result in result. The <code>in[4]</code> and
         * <code>out[4]</code> arrays must both be allocated but they can be
         * the same array.
         * @param in The point to be multiplied from left.
         * @param out The result of the multiplication.
         */
        void pointMultiply(const double in[4], double out[4]) const;

        /**
         * Point multiplication from left.
         * Multiply a point (in homogeneous coordinates) by this matrix,
         * and return the result. The <code>in</code> and
         * <code>out</code> arrays can be the same array.
         * @param in The point to be multiplied from left.
         * @param out The result of the multiplication.
         */
        void pointMultiply(const std::array<double, 4> & in,
            std::array<double, 4> & out) const;

        /**
         * Point multiplication from left.
         * Multiply a point (3 elements) by this matrix, and return the result
         * in result. The <code>in[3]</code> and <code>result[3]</code> arrays
         * must both be allocated but they can be the same array.
         * @param in The point to be multiplied from left.
         * @param out The result of the multiplication.
         */
        void point3Multiply(const double in[3], double out[3]) const;

        /**
         * Point multiplication from left.
         * Multiply a point (3 elements) by this matrix, and return the result
         * in result. The <code>in</code> and <code>out</code> arrays can be the
         * same array.
         * @param in The point to be multiplied from left.
         * @param out The result of the multiplication.
         */
        void point3Multiply(const std::array<double, 3> & in,
            std::array<double, 3> & out) const;

        /**
         * Compute adjoint.
         * Calculates the adjoint of a 3x3 matrix:
         * Let @f$a_{ij}@f$ denote the minor determinant of matrix
         * @f$\mathcal{A}@f$ obtained by deleting the ith row and jth column
         * from @f$\mathcal{A}@f$.
         * Let @f$ b_{ij} = (-1)^{i+j} a_{ij}@f$.
         * The matrix @f$\mathcal{B} = (b_{ij})@f$ is the adjoint of
         * @f$\mathcal{A}@f$.
         *
         * @param in  The original matrix.
         * @param out The adjoint of @c in .
         */
        static void Adjoint(const Matrix3x3 &in,Matrix3x3 &out);

        /** Compute the determinant of the matrix and return it. */
        double determinant(void) const;

        /**
         * Multiply matrices @c lhs and @c rhs and store the result in
         * @c result .
         */
        static void Multiply(const Matrix3x3 &lhs, const Matrix3x3 &rhs,
                             Matrix3x3 &result);

        /**
         * Set a matrix element.
         * Sets the element row, col in the matrix to the given value.
         *
         * @param row   The row address starting at zero.
         * @param col   The column address starting at zero.
         * @param value The value the element is set to.
         */
        void setElement(unsigned int row, unsigned int col, double value);

        /**
         * Get a matrix element.
         * Returns the element i,j from the matrix.
         *
         * @param row The row address starting at zero.
         * @param col The column address starting at zero.
         * @return    The specified element's value.
         */
        double getElement(unsigned int row, unsigned int col) const;

        /**
         * Return the matrix (internal allocation).
         * @return A 3x3 matrix as double type array in row-major order. The
         *         memory is to be released by the caller.
         */
        double *getMatrix(void) const;

        /**
         * Get the matrix (external allocation).
         * @param matrix A preallocated nine-element array where a 3x3 matrix
         *               will be stored in row-major order. Required to be
         *               non-<tt>nullptr</tt>.
         */
        void getMatrix(double *matrix) const;

        /**
         * Test orthonormal matrix properties.
         * To pass the test, the matrix has to be orthogonal, it must not
         * change coordinate system handedness, and the transformed base
         * vectors have to be of unit length.
         * @param testEps The epsilon value for the test operations.
         * @exception MathException is thrown if the test fails.
         */
        void testOrthonormalMatrix(double testEps) const throw(MathException);

    private:
        /** The 2D array of the 9 elements. */
        double element_[3][3];

        /** 3x3 identity matrix. */
        static const Matrix3x3 Identity_;
    };

}

#endif

//==========================================================================EOF=
