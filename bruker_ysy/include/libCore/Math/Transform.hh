//==============================================================================
// Copyright (c) 1997 - 2018
//
// Bruker BioSpin AG
// MRI Division
// CH-8117 Fällanden, Switzerland
//
// All Rights Reserved
//==============================================================================
#ifndef LIB_CORE_MATH_TRANSFORM_H
#define LIB_CORE_MATH_TRANSFORM_H

#include "libCore/Base/TimeStamped.hh"
#include "libCore/Math/Matrix4x4.hh"
#include "libCore/Base/internal.hh"

namespace core
{
    //--------------------------------------------------------------------------
    // Interface of class Transform
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Math/Transform.hh"
     * A general matrix transformation class.
     *
     * <b>Multiple Implementations!</b>
     *
     * Implementations of this class exist in C++ (libCore/Math library) as
     * well as in Java (Libraries module).
     *
     * <tt>Transform</tt> maintains a stack of 4x4 transformation matrices.
     * A variety of methods are provided to manipulate the translation, scale,
     * and rotation components of the matrix. Methods operate on the matrix at
     * the top of the stack.
     * It is very important to realize that this class performs all of its
     * operations in a right handed coordinate system with right handed
     * rotations.
     * Some other graphics libraries use left handed coordinate systems and
     * rotations.
     *
     * @author Christian Werner
     * @since ParaVision 6.Alpha.0.72
     * @see Matrix3x3
     * @see Matrix4x4
     */
    class DLL_INTERFACE Transform : public TimeStamped
    {
    public:
        /**
         * Default constructor.
         * Constructs a transform and sets the following defaults:
         * <tt>preMultiplyFlag = 1</tt>
         * <tt>stackSize = 10</tt>.
         * It then creates a 4x4 identity matrix as the top matrix on the stack.
         */
        Transform(void);

        /**
         * Constructor with 4x4 matrix.
         * Constructs a transform and sets the following defaults:
         * <tt>preMultiplyFlag =  1</tt>
         * <tt>stackSize       = 10</tt>.
         * It then copies the given matrix onto the top of the stack.
         * @param matrix Initial 4x4 matrix, a deep copy is made.
         */
        Transform(const Matrix4x4 &matrix);

        /**
         * Constructor with 3x3 matrix.
         * Constructs a transform and sets the following defaults:
         * <tt>preMultiplyFlag =  1</tt>
         * <tt>stackSize       = 10</tt>.
         * It then copies the given matrix onto the top of the stack.
         * @param matrix Initial 3x3 matrix, a deep copy is made. The
         *               translational coefficients are initialized to zero.
         */
        Transform(const Matrix3x3 &matrix);

        /**
         * Copy constructor.
         * Creates an instance of @c Transform and then copies its instance
         * variables from the values in @c rhs .
         */
        explicit Transform(const Transform& rhs);

        /** Destructor. */
        ~Transform();

        /**
         * Assignment operator.
         * @param rhs Instance to copy state from.
         * @return This instance.
         */
        Transform &operator=(const Transform &rhs);

        /**
         * Identity.
         * Creates an identity matrix and makes it the current transformation
         * matrix.
         */
        void identity(void);

        /**
         * Pop the matrix stack.
         * Deletes the transformation on the top of the stack and sets the top
         * to the next transformation on the stack.
         */
        void pop(void);

        /**
         * Push the matrix stack.
         * Pushes the current transformation matrix onto the transformation
         * stack.
         */
        void push(void);

        /**
         * Set to post multiplication.
         * Sets the internal state of the transform to post multiply. All
         * subsequent matrix operations will occur after those already
         * represented in the current transformation matrix.
         * The default setting is pre-multiplication.
         */
        void postMultiply(void);

        /**
         * Set to pre-multiplication.
         * Sets the internal state of the transform to pre-multiply. All
         * subsequent matrix operations will occur before those already
         * represented in the current transformation matrix.
         * Pre-multiplication is the default.
         */
        void preMultiply(void);

        /**
         * Rotate about the x axis.
         * Creates an x rotation matrix and concatenates it with the current
         * transformation matrix.
         *
         * @param angle The rotation angle specified in degrees.
         */
        void rotateX(double angle);

        /**
         * Rotate about the y axis.
         * Creates a y rotation matrix and concatenates it with the current
         * transformation matrix.
         *
         * @param angle The rotation angle specified in degrees.
         */
        void rotateY(double angle);

        /**
         * Rotate about the z axis.
         * Creates a z rotation matrix and concatenates it with the current
         * transformation matrix.
         *
         * @param angle The rotation angle specified in degrees.
         */
        void rotateZ(double angle);

        /**
         * Arbitrary rotation.
         * Creates a matrix that rotates about an axis through the origin and
         * @c axisX , @c axisY , and @c axisZ . It then concatenates this matrix
         * with the current transformation matrix.
         *
         * @param angle The rotation angle in degrees.
         * @param axisX X-axis coordinate of second rotation axis point.
         * @param axisY Y-axis coordinate of second rotation axis point.
         * @param axisZ Z-axis coordinate of second rotation axis point.
         */
        void rotateWXYZ(double angle, double axisX, double axisY,
                double axisZ) throw (MathException);

        /**
         * Scale.
         * Scales the current transformation matrix in the axis directions.
         * A scale factor of zero will automatically be replaced with one.
         *
         * @param scaleX X-axis scale factor.
         * @param scaleY Y-axis scale factor.
         * @param scaleZ Z-axis scale factor.
         */
        void scale(double scaleX, double scaleY,
                double scaleZ) throw (MathException);

        /**
         * Translate.
         * Translate the current transformation matrix by the given vector.
         *
         * @param transX Translation x-axis component.
         * @param transY Translation y-axis component.
         * @param transZ Translation z-axis component.
         */
        void translate(double transX, double transY, double transZ);

        /** Transpose the current transformation matrix. */
        inline void transpose(void) const
        {
            getTranspose(**stack_);
        }

        /**
         * Obtain the transpose of the current transformation matrix.
         * @param transpose Matrix to copy the transpose into.
         */
        void getTranspose(Matrix4x4& transpose) const;

        /** Invert the current transformation matrix. */
        void inverse(void);

        /**
         * Obtain the inverse of the current transformation matrix.
         * @param inverse Matrix to copy the inverse into.
         */
        void getInverse(Matrix4x4& inverse) const;

        /**
         * Get the x, y, z orientation angles from the transformation matrix.
         * Retrieves Tait-Brian angles in y-x-z sequence.
         * @param orientation A preallocated array of three floating point
         *                    values used to pass back the result.
         */
        void getOrientation(double *orientation) const;

        /**
         * Get the x, y, z orientation angles from the transformation matrix.
         * Retrieves Tait-Brian angles in y-x-z sequence.
         * @param angleX Returns the x-axis angle.
         * @param angleY Returns the y-axis angle.
         * @param angleZ Returns the z-axis angle.
         */
        void getOrientation(double& angleX, double& angleY,
                double& angleZ) const;

        /**
         * Get rotation axis and angle from the transformation matrix.
         * The rotation axis will always point into the positive half space.
         * @param angle Returns the rotation angle in degrees.
         * @param axisX Returns the rotation vector x-axis component.
         * @param axisY Returns the rotation vector y-axis component.
         * @param axisZ Returns the rotation vector z-axis component.
         */
        void getOrientationWXYZ(double &angle, double& axisX, double& axisY,
                double& axisZ) const;

        /**
         * Get rotation axis and angle from the transformation matrix.
         * The rotation axis will always point into the positive half space.
         * @param wxyz A preallocated length 4 vector, used to return the
         *             rotation angle in degrees and the rotation axis.
         */
        void getOrientationWXYZ(double *wxyz) const;

        /**
         * Get the current position.
         * Returns the position from the current transformation matrix as an
         * array of three floating point numbers. This is simply returning the
         * translation component of the 4x4 matrix.
         * @param position A preallocated array of three floating point values
         *                 used to pass back the result.
         */
        void getPosition(double *position) const;

        /**
         * Return the x, y, z positions from the current transformation matrix.
         * This is simply returning the translation component of the 4x4 matrix.
         *
         * @param posX Returns the x-axis offset.
         * @param posY Returns the y-axis offset.
         * @param posZ Returns the z-axis offset.
         */
        void getPosition(double& posX, double& posY, double& posZ) const;

        /**
         * Get the current scale.
         * Returns the x, y, z scale factors of the current transformation
         * matrix as an array of three float numbers.
         * @param scale A preallocated array of three floating point values
         *              used to pass back the result.
         */
        void getScale(double *scale) const;

        /**
         * Return the x, y, z scale factors of the current transformation
         * matrix.
         * @param scaleX Returns the x-axis scale factor.
         * @param scaleY Returns the y-axis scale factor.
         * @param scaleZ Returns the z-axis scale factor.
         */
        void getScale(double& scaleX, double& scaleY, double& scaleZ) const;

        /**
         * Set the current matrix directly.
         * @param matrix Matrix to set.
         */
        inline void setMatrix(const Matrix4x4& matrix)
        {
            **stack_ = matrix;
        }

        /**
         * Return the current transformation matrix.
         * @return The current transformation matrix.
         */
        Matrix4x4& getMatrix(void) const
        {
            return **stack_;
        }

        /**
         * Return the current transformation matrix.
         * @param matrix Matrix to copy the current transformation matrix to.
         */
        inline void getMatrix(Matrix4x4& matrix) const
        {
            matrix = **stack_;
        }

        /**
         * Return the orientation part of the matrix (internal allocation).
         * @return A 3x3 matrix (upper left corner of the current matrix) as
         *         double type array in row-major order. The memory is to be
         *         released by the caller.
         */
        double *getOrientationMatrix(void) const;

        /**
         * Get the orientation part of the matrix (external allocation).
         * @param orient A preallocated nine-element array where a 3x3 matrix
         *               (upper left corner of the current matrix) will be
         *               stored in row-major order. Required to be
         *               non-<tt>nullptr</tt>.
         */
        void getOrientationMatrix(double *orient) const;

        /**
         * Concatenate 4x4.
         * Concatenates the 4x4 input matrix with the current transformation
         * matrix.
         * The resulting matrix becomes the new current transformation matrix.
         * The setting of the @c preMultiply_ flag determines whether
         * the matrix is pre or post concatenated.
         */
        void concatenate(const Matrix4x4 &matrix);

        /**
         * Concatenate 3x3.
         * Concatenates the 3x3 input matrix with the current transformation
         * matrix.
         * The resulting matrix becomes the new current transformation matrix.
         * The setting of the @c preMultiply_ flag determines whether
         * the matrix is pre or post concatenated.
         */
        void concatenate(const Matrix3x3 & matrix);

        /**
         * Point multiplication from left.
         * Left multiply the current matrix with a point.
         * The state of the transform instance stays unchanged.
         * In-place use is safe (e.g., @c in and @c out may be the same array).
         *
         * @param in  The point to multiply with in homogeneous coordinates.
         * @param out Returns the multiplication result in homogeneous
         *            coordinates.
         */
        void pointMultiply(const double in[4], double out[4]) const;

        /**
         * Point multiplication from right.
         * Right multiply the current matrix with a point.
         * The state of the transform instance stays unchanged.
         * In-place use is safe (e.g., @c in and @c out may be the same array).
         *
         * @param in  The point to multiply with in homogeneous coordinates.
         * @param out Returns the multiplication result in homogeneous
         *            coordinates.
         */
        void multiplyPoint(const double in[4], double out[4]) const;

    private:
        /** Flag whether pre or post multiplication is active. */
        bool preMultiplyFlag_;

        /** Stack size. */
        int stackSize_;

        /** Stack of transformation matrices. */
        Matrix4x4 **stack_;

        /** Pointer to stack bottom. */
        Matrix4x4 **stackBottom_;
    };
}

#endif

//==========================================================================EOF=
