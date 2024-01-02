//==============================================================================
// Copyright (c) 1997 - 2018
//
// Bruker BioSpin AG
// MRI Division
// CH-8117 Fällanden, Switzerland
//
// All Rights Reserved
//==============================================================================
#ifndef LIB_CORE_VECMATH_H
#define LIB_CORE_VECMATH_H

#include <cassert>
#include <cstdlib>

#include "libCore/Containers/Exceptions/MathException.hh"
#include "libCore/Math/Math.hh"
#include "libCore/Base/internal.hh"

namespace core
{
    //--------------------------------------------------------------------------
    // Interface of class VecMath
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Math/VecMath.hh"
     * Static utility class collecting mathematical operations.
     * @c core::VecMath provides methods to perform common math operations.
     * These include constants such as Pi; conversion from degrees to radians;
     * vector operations such as dot and cross products and vector norm; matrix
     * determinant for 2x2 and 3x3 matrices; and random number generation.
     *
     * @author Christian Werner
     * @since ParaVision 6.Alpha.0.72
     */
    class DLL_INTERFACE VecMath
    {
    public:

        //----------------------------------------------------------------------
        // constants
        //----------------------------------------------------------------------

        /** Degrees to radians. */
        static const double DegreesToRadians;

        /** Radians to degrees. */
        static const double RadiansToDegrees;

        //----------------------------------------------------------------------
        // some common methods
        //----------------------------------------------------------------------

        /**
         * Multiply a vector with a scalar.
         * @param scalar Scalar to multiply with.
         * @param vector Vector to multiply.
         * @param out    Output.
         */
        static inline void SMult(double scalar, const double vector[3],
        double out[3])
        {
            out[0] = scalar * vector[0];
            out[1] = scalar * vector[1];
            out[2] = scalar * vector[2];
        }

        /**
         * Negate a vector.
         * @param in  Input vector.
         * @param out Output vector.
         */
        static inline void Negate(const double in[3], double out[3])
        {
            out[0] = -in[0];
            out[1] = -in[1];
            out[2] = -in[2];
        }

        /** Dot product of two 3-vectors. */
        static inline double Dot(const double lhs[3], const double rhs[3])
        {
            return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
        }


        /**
         * Cross product of two 3-vectors (double version).
         * @param lhs Left hand side input.
         * @param rhs Right hand side output.
         * @param out Output.
         */
        static void Cross(
            const double lhs[3], const double rhs[3], double out[3]);

        /**
         * Compute the norm of a 3-vector.
         * @param vector A length three vector, it is left unchanged.
         * @return       The norm of the vector.
         */
        static inline double Norm(const double vector[3])
        {
            return std::sqrt(vector[0] * vector[0] + vector[1] * vector[1]
                    + vector[2] * vector[2]);
        }

        /**
         * Normalize (in place) a 3-vector.
         * @param vector A length three vector which will be normalized.
         * @return       The norm of the vector.
         */
        static inline double Normalize(double vector[3])
        {
            double den;
            if ((den = VecMath::Norm(vector)) != 0.0)
            {
                for (size_t idx = 0; idx < 3; idx++)
                {
                    vector[idx] = vector[idx] / den;
                }
            }
            return den;
        }

        /**
         * Compute distance squared between two points.
         * @param lhs A first point as length three vector.
         * @param rhs A second point.
         * @return    The square of the distance between @c lhs and
         *           <tt>rhs</tt>.
         */
        static inline double Distance2BetweenPoints(const double lhs[3],
        const double rhs[3])
        {
            const double diff0 = lhs[0] - rhs[0];
            const double diff1 = lhs[1] - rhs[1];
            const double diff2 = lhs[2] - rhs[2];
            return diff0 * diff0 + diff1 * diff1 + diff2 * diff2;
        }

        /**
         * Test, whether two vectors in 3D space are approximately normal to
         * each other.
         *
         * @param lhs       First length three vector.
         * @param rhs       Second length three vector.
         * @param tolerance Angle tolerance in radians, should not be larger
         *                  than the equivalent of a few degrees (calculation is
         *                  not accurate outside this range).
         * @return          @c true if the vectors are approximately normal.
         */
        static bool IsApproxNormal(const double *lhs, const double *rhs,
                double tolerance);

        /**
         * Test, whether two vectors in 3D space are approximately parallel to
         * each other.
         *
         * @param lhs       First length three vector.
         * @param rhs       Second length three vector.
         * @param tolerance Angle tolerance in radians, should not be larger
         *                  than the equivalent of a few degrees (calculation is
         *                  not
         *                  accurate outside this range).
         * @return          @c true if the vectors are approximately parallel.
         */
        static bool IsApproxParallel(const double *lhs, const double *rhs,
                double tolerance);

        //----------------------------------------------------------------------
        // special methods for 2D operations
        //----------------------------------------------------------------------

        /**
         * Dot product of two 2-vectors.
         * The third (z) component is ignored.
         */
        static inline double Dot2D(const double lhs[3], const double rhs[3])
        {
            return lhs[0] * rhs[0] + lhs[1] * rhs[1];
        }

        /**
         * Compute the norm of a 2-vector.
         * @param vector A vector, it is left unchanged. The z-component is
         *               ignored.
         * @return       The norm of the vector.
         */
        static inline double Norm2D(const double vector[3])
        {
            return std::sqrt(vector[0] * vector[0] + vector[1] * vector[1]);
        }

        /**
         * Normalize (in place) a 2-vector.
         * @param vector A vector, which will be normalized. The z-component is
         *               ignored.
         * @return       The norm of the vector.
         */
        static inline double Normalize2D(double vector[3])
        {
            double den;
            if ((den = Norm2D(vector)) != 0.0)
            {
                for (size_t idx = 0; idx < 2; idx++) vector[idx] /= den;
            }
            return den;
        }

        //----------------------------------------------------------------------
        // special methods for ND operations
        //----------------------------------------------------------------------

        /**
         * Compute the norm of a n-vector.
         * @param vector A length @c length vector, it is left unchanged.
         * @param length The length of the vector.
         * @return       The norm of the vector.
         */
        static inline double NormND(const double *vector, unsigned int length)
        {
            double norm2 = 0.0;
            for (size_t idx = 0; idx < length; idx++)
            {
                norm2 += vector[idx] * vector[idx];
            }
            return std::sqrt(norm2);
        }

        /**
         * Compute the sum of the elements of a n-vector.
         * @param vector A length @c length vector of any type, it is left
         *               unchanged.
         * @param length The length of the vector.
         * @return       The sum of the vector elements.
         */
        template<class T>
        static inline T SumND(const T *vector, unsigned int length)
        {
            T sum = 0;
            for (size_t idx = 0; idx < length; idx++)
            {
                sum += vector[idx];
            }
            return sum;
        }

        //----------------------------------------------------------------------
        // matrix stuff
        //----------------------------------------------------------------------

        /**
         * Compute the determinant of 2x2 matrix.
         * Two columns of the matrix are input.
         * @param col1 The left (first) column.
         * @param col2 The right (second) column.
         * @return     The 2x2 matrix determinant.
         */
        static inline double Determinant2x2(const double col1[2],
        const double col2[2])
        {
            return (col1[0] *col2[1] - col2[0] * col1[1]);
        }

        /**
         * Calculate the determinant of a 2x2 matrix.
         * This computes @f$\vert\begin{array}{cc}a&b\\c&d\end{array}\vert@f$.
         * @return The 2x2 matrix determinant.
         */
        static inline double Determinant2x2(double a, double b, double c, double d)
        {
            return (a * d - b * c);
        }

        /**
         * Compute the determinant of 3x3 matrix.
         * Three columns of the matrix are input.
         * @param col1 The left (first) column.
         * @param col2 The middle (second) column.
         * @param col3 The right (third) column.
         * @return     The 3x3 matrix determinant.
         */
        static inline double Determinant3x3(const double col1[3],
        const double col2[3], const double col3[3])
        {
            return col1[0] * col2[1] * col3[2]
                + col2[0] * col3[1] * col1[2] + col3[0] * col1[1] * col2[2]
                - col1[0] * col3[1] * col2[2] - col2[0] * col1[1] * col3[2]
                - col3[0] * col2[1] * col1[2];
        }

        /**
         * Calculate the determinant of a 3x3 matrix.
         * This computes @f$\vert\begin{array}{ccc}a_1&b_1&c_1\\a_2&b_2&c_2\\a_3&b_3&c_3\end{array}\vert@f$.
         * @param a1 First element in first row.
         * @param a2 Second element in first row.
         * @param a3 Third element in first row.
         * @param b1 First element in second row.
         * @param b2 Second element in second row.
         * @param b3 Third element in second row.
         * @param c1 First element in third row.
         * @param c2 Second element in third row.
         * @param c3 Third element in third row.
         * @return The 3x3 matrix determinant.
         */
        static inline double Determinant3x3(double a1, double a2, double a3,
        double b1, double b2, double b3,
        double c1, double c2, double c3)
        {
            return a1 * Determinant2x2(b2, b3, c2, c3)
                    - b1 * Determinant2x2(a2, a3, c2, c3)
                    + c1 * Determinant2x2(a2, a3, b2, b3);
        }

        /**
         * Test an orthonormal matrix (nine elements).
         * To pass the test, the matrix has to be orthogonal, it must not
         * change coordinate system handedness, and the transformed base
         * vectors have to be of unit length.
         *
         * @param m00 First element in the first row.
         * @param m01 Second element in the first row.
         * @param m02 Third element in the first row.
         * @param m10 First element in the second row.
         * @param m11 Second element in the second row.
         * @param m12 Third element in the second row.
         * @param m20 First element in the third row.
         * @param m21 Second element in the third row.
         * @param m22 Third element in the third row.
         * @param eps Tolerance value.
         * @exception MathException is thrown if the test fails.
         */
        static void TestOrthonormalMatrix(double m00, double m01, double m02,
                double m10, double m11, double m12, double m20, double m21,
                double m22, double eps) throw (MathException);

        /**
         * Test an orthonormal matrix (length nine array).
         * To pass the test, the matrix has to be orthogonal, it must not
         * change coordinate system handedness, and the transformed base
         * vectors have to be of unit length.
         *
         * @param matrix Matrix to test; an array of nine values in row major
         *               order. Required to be non-<tt>nullptr</tt>.
         * @param eps    Tolerance value.
         * @exception MathException is thrown if the test fails.
         */
        static inline void TestOrthonormalMatrix(const double *matrix,
                double eps) throw (MathException)
        {
            assert(matrix);
            return TestOrthonormalMatrix(
                    matrix[0], matrix[1], matrix[2],
                    matrix[3], matrix[4], matrix[5],
                    matrix[6], matrix[7], matrix[8],
                    eps);
        }

        /**
         * Multiply a 3-element row vector by a 3-by-3 matrix.
         * Multiplication from right.
         * @param in0    Input vector, first coordinate.
         * @param in1    Input vector, second coordinate.
         * @param in2    Input vector, third coordinate.
         * @param matrix 3-by-3 matrix.
         * @param out0   Output vector, first coordinate.
         * @param out1   Output vector, second coordinate.
         * @param out2   Output vector, third coordinate.
         */
        static void MultiplyPoint(double in0, double in1, double in2,
                const double *matrix, double &out0, double &out1, double &out2);

        /**
         * Multiply a 3-by-3 matrix by a 3-element column vector.
         * Multiplication from left.
         * @param matrix 3-by-3 matrix.
         * @param in0    Input vector, first coordinate.
         * @param in1    Input vector, second coordinate.
         * @param in2    Input vector, third coordinate.
         * @param out0   Output vector, first coordinate.
         * @param out1   Output vector, second coordinate.
         * @param out2   Output vector, third coordinate.
         */
        static void PointMultiply(const double *matrix, double in0, double in1,
                double in2, double &out0, double &out1, double &out2);

        //----------------------------------------------------------------------
        // Random number generation
        //----------------------------------------------------------------------

        /**
         * Provide a seed value to the random number generator.
         * @param seed Seed value.
         */
        inline static void RandomSeed(unsigned int seed)
        {
            srand(seed);
        }

        /**
         * Get a random number.
         * @return The next pseudo-random number.
         */
        inline static int Random()
        {
            return rand();
        }

        /**
         * Generate a random number.
         * Returns a random number between @c min and <tt>max</tt>.
         */
        template<class T>
        inline T Random(T min, T max)
        {
            return min + Random() * (max - min);
        }

    private:
        /** Inaccessible and unimplemented constructor: abstract class. */
        VecMath();

        /** Seed for random numbers. */
        static int64_t Seed_;
    };
}

#endif

//==========================================================================EOF=
