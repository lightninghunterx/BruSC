//==============================================================================
// Copyright (c) 1997 - 2020
//
// Bruker Switzerland AG
// CH-8117 Fällanden, Switzerland
//
// All Rights Reserved
//==============================================================================
#ifndef LIB_CORE_MATH_MATRIX4X4_H
#define LIB_CORE_MATH_MATRIX4X4_H

#include <cstring>
#include "libCore/Containers/Exceptions/MathException.hh"
#include "libCore/Base/TimeStamped.hh"
#include "libCore/Base/internal.hh"

namespace core
{
    class Matrix3x3;
    class Matrix4x4;
    class Transform;

    /**
     * Insertion operator for @c Matrix4x4 .
     * @param ostream The output stream to write into.
     * @param matrix  A reference to an instance of a 4 by 4 matrix.
     * @return        The output stream.
     */
    DLL_INTERFACE std::ostream &operator<<(std::ostream &ostream,
            const Matrix4x4 &matrix);

    //--------------------------------------------------------------------------
    // Interface of class Matrix4x4
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Math/Matrix4x4.hh"
     * This concrete class implements 4x4 matrices and operations on them.
     * @c Matrix4x4 is a class to represent and manipulate 4x4 matrices.
     * Specifically, it is designed to work on 4x4 transformation matrices
     * found in 3D rendering using homogeneous coordinates @f$[x y z w]@f$.
     *
     * @author Christian Werner
     * @since Viper libVip/Pipeline R2, libCore/Misc R5.2.0
     * @see Matrix3x3
     * @see Transform
     */
    class DLL_INTERFACE Matrix4x4 : public TimeStamped
    {
        friend class Transform;
        friend class Matrix3x3;
        friend std::ostream &operator<<(
                std::ostream &ostream, const Matrix4x4 &matrix);
        friend std::istream &operator>>(
                std::istream &istream, Matrix4x4 &matrix);
    public:
        /**
         * Default constructor.
         * Constructs a 4x4 identity matrix.
         */
        Matrix4x4(void);

        /**
         * Constructor with 16 elements.
         * @param m00 Element in row 0, column 0.
         * @param m01 Element in row 0, column 1.
         * @param m02 Element in row 0, column 2.
         * @param m03 Element in row 0, column 3.
         * @param m10 Element in row 1, column 0.
         * @param m11 Element in row 1, column 1.
         * @param m12 Element in row 1, column 2.
         * @param m13 Element in row 1, column 3.
         * @param m20 Element in row 2, column 0.
         * @param m21 Element in row 2, column 1.
         * @param m22 Element in row 2, column 2.
         * @param m23 Element in row 2, column 3.
         * @param m30 Element in row 3, column 0.
         * @param m31 Element in row 3, column 1.
         * @param m32 Element in row 3, column 2.
         * @param m33 Element in row 3, column 3.
         */
        Matrix4x4(
            double m00, double m01, double m02, double m03,
            double m10, double m11, double m12, double m13,
            double m20, double m21, double m22, double m23,
            double m30, double m31, double m32, double m33);

        /**
         * Constructor with a 2D array.
         * @param array Array of four by four elements. The array will be
         *              copied.
         */
        Matrix4x4(const double array[4][4]);

        /**
         * Constructor with an 1D array.
         * @param array Array of sixteen elements, to be filled into matrix in
         *              row-major order. The array will be copied.
         */
        Matrix4x4(const double *const array);

        /**
         * Copy constructor.
         * Makes a deep copy of the 4x4 @c rhs matrix object.
         */
        Matrix4x4(const Matrix4x4& rhs);

        /**
         * Cross copy constructor.
         * Makes a deep copy of the 3x3 @c rhs matrix object; the remaining
         * elements are set to zero with the exception of the lower right corner
         * which is set to one.
         */
        Matrix4x4(const Matrix3x3& rhs);

        /**
         * Set all elements of the matrix to the given value.
         * @param element Value to set.
         * @return        This instance.
         */
        Matrix4x4 & operator=(double element);

        /**
         * Assignment operator for 4x4 matrix.
         * @param rhs Instance to copy state from.
         * @return    This instance.
         */
        inline Matrix4x4& operator=(const Matrix4x4& rhs)
        {
            if (this == &rhs) return *this;
            memcpy(element_, rhs.element_, 16 * sizeof(double));
            return *this;
        }

        /**
         * Assignment operator for 3x3 matrix.
         * The upper left nine elements are set to the value of <tt>rhs</tt>,
         * the rest stays unchanged.
         */
        Matrix4x4& operator=(const Matrix3x3& rhs);

        /**
         * Equality comparison operator.
         * @param rhs Instance to compare to.
         * @return    @c true if all elements are equal.
         */
        inline bool operator==(const Matrix4x4& rhs) const
        {
            if (this == &rhs) return true;
            for (size_t i = 0; i < 4; i++)
            {
                for (size_t j = 0; j < 4; j++)
                {
                    if (element_[i][j] != rhs.element_[i][j]) return false;
                }
            }
            return true;
        }

        /** Inequality comparison operator. */
        inline bool operator!=(const Matrix4x4& rhs) const
        {
            return !(*this == rhs);
        }

        /**
         * Get the 4x4 identity matrix.
         * @return The 4x4 identity matrix.
         */
        static const Matrix4x4 &Identity(void);

        /**
         * Row access operator (non-constant).
         * @param row A row address, starting at zero.
         * @return    The selected row.
         */
        inline double *operator[](unsigned int row)
        {
            return &(element_[row][0]);
        }

        /**
         * Row access operator (constant).
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
         * Matrix inversion.
         * Use transposition instead for rotation-only matrices.
         * Calculated as @f$\mathcal{A}^{-1} = \frac{1}{ \textrm{det}
         * \mathcal{A}} \textrm{adjoint} \mathcal{A}@f$.
         *
         * @param in  The matrix to invert.
         * @param out The inverted matrix.
         */
        static void Invert(const Matrix4x4 &in, Matrix4x4 &out)
            throw(MathException);

        /**
         * In-place matrix inversion.
         * Use transposition instead for rotation-only matrices.
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
        static void Transpose(const Matrix4x4 &in, Matrix4x4 &out);

        /** In-place transposition. */
        inline void transpose(void)
        {
            Transpose(*this, *this);
        }

        /**
         * Transform a point or vector into the coordinate system represented by
         * this matrix. The <tt>input[4]</tt> and <tt>output[4]</tt> arrays must
         * both be allocated but they can be the same array.
         * @param input  Point or vector to transform.
         * @param output Point or vector in local coordinates.
         * @see #xformOutOfFrame
         */
        void xformIntoFrame(const double input[4], double output[4]) const;

        /**
         * Transform a point or vector out of the coordinate system represented
         * by this matrix. The <tt>input[4]</tt> and <tt>output[4]</tt> arrays must
         * both be allocated but they can be the same array.
         * @param input  Point or vector in local coordinates.
         * @param output Transformed point or vector.
         * @see #xformIntoFrame
         */
        void xformOutOfFrame(const double input[4], double output[4]) const;

        /**
         * Point multiplication from right.
         * For coordinate transforms, use <tt>xformIntoFrame</tt> instead.
         * Multiply this matrix by a point (in homogeneous coordinates), and
         * return the result in <tt>out</tt>. The <tt>in[4]</tt> and
         * <tt>out[4]</tt> arrays must both be allocated but they can be the
         * same array.
         */
        void multiplyPoint(const double in[4], double out[4]) const;

        /**
         * Point multiplication from left.
         * For coordinate transforms, use <tt>xformOutOfFrame</tt> instead.
         * Multiply a point (in homogeneous coordinates) by this matrix, and
         * return the result in <tt>out</tt>. The <tt>in[4]</tt> and
         * <tt>out[4]</tt> arrays must both be allocated but they can be the
         * same array.
         */
        void pointMultiply(const double in[4], double out[4]) const;

        /**
         * Compute adjoint.
         * Calculates the adjoint of a 4x4 matrix:
         * Let @f$a_{ij}@f$ denote the minor determinant of matrix
         * @f$\mathcal{A}@f$ obtained by deleting the ith row and jth column
         * from @f$\mathcal{A}@f$.
         * Let @f$ b_{ij} = (-1)^{i+j} a_{ij}@f$.
         * The matrix @f$\mathcal{B} = (b_{ij})@f$ is the adjoint of
         * @f$\mathcal{A}@f$.
         *
         * @param in  The original matrix.
         * @param out The adjoint of <tt>in</tt>.
         */
        static void Adjoint(const Matrix4x4 &in, Matrix4x4 &out);

        /** Compute the determinant of the matrix and return it. */
        double determinant(void) const;

        /**
         * Multiply matrices @c lhs and @c rhs and store the result in
         * @c result .
         */
        static void Multiply(
                const Matrix4x4 &lhs, const Matrix4x4 &rhs, Matrix4x4 &result);

        /**
         * Set a matrix element.
         * Sets the element i,j in the matrix to the given value.
         *
         *  @param row   The row address starting at zero.
         *  @param col   The column address starting at zero.
         *  @param value The value the element is set to.
         */
        void setElement(unsigned int row, unsigned int col, double value);

        /**
         * Get a matrix element.
         * Returns the element row, col from the matrix.
         *
         *  @param row The row address starting at zero.
         *  @param col The column address starting at zero.
         *  @return    The specified element's value.
         */
        double getElement(unsigned int row, unsigned int col) const;

        /**
         * Return the matrix (internal allocation).
         * @return A 4x4 matrix as double type array in row-major order. The
         *         memory is to be released by the caller.
         */
        double *getMatrix(void) const;

        /**
         * Get the matrix (external allocation).
         * @param matrix A preallocated sixteen-element array where a 4x4 matrix
         *               will be stored in row-major order. Required to be
         *               non-<tt>nullptr</tt>.
         */
        void getMatrix(double *matrix) const;

        /**
         * Return the orientation part of the matrix (internal allocation).
         * @return A 3x3 orientation matrix (upper left corner of the current
         *         matrix) as double type array in row-major order. The memory
         *         is to be released by the caller.
         */
        double *getOrientationMatrix(void) const;

        /**
         * Get the orientation part of the matrix (external allocation).
         * @param orient A preallocated nine-element array where a 3x3
         *               orientation matrix (upper left corner of the current
         *               matrix) will be stored in row-major order. Required to
         *               be non-<tt>nullptr</tt>.
         */
        void getOrientationMatrix(double *orient) const;

        /**
         * Test orthonormal matrix properties; only checks the 3x3 elements in
         * the upper left corner of the current matrix.
         * To pass the test, the matrix has to be orthogonal, it must not
         * change coordinate system handedness, and the transformed base
         * vectors have to be of unit length.
         * @param testEps The epsilon value for the test.
         * @exception MathException is thrown if the test fails.
         */
        void testOrthonormalMatrix(double testEps) const throw(MathException);

    private:
        /** The 2D array of the 16 elements. */
        double element_[4][4];

        /** 4x4 identity matrix. */
        static const Matrix4x4 Identity_;
    };

    //--------------------------------------------------------------------------
    // Interface of non-member functions
    //--------------------------------------------------------------------------

}

#endif

//==========================================================================EOF=
