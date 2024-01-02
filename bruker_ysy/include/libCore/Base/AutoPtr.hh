//==============================================================================
// Copyright (c) 1999 - 2018
//
// Bruker BioSpin AG
// MRI Division
// CH-8117 Fällanden, Switzerland
//
// All Rights Reserved
//==============================================================================
#ifndef LIB_CORE_BASE_AUTO_PTR
#define LIB_CORE_BASE_AUTO_PTR

#include "libCore/Base/Cleanup.hh"

namespace core
{
    //---------------------------------------------------------------------------
    // Implementation of class AutoPtr
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Base/AutoPtr.hh"
     * Auto pointer template.
     * Derived from version 3.12 of the SGI implementation of the STL; parts
     * requiring member templates as well as the <code>std</code> namespace are
     * omitted.
     *
     * <b>Traits</b>
     *
     * The optional second template parameter selects a trait template suited to
     * delete the pointee. Operator <code>delete</code> is used by the default
     * trait <code>core::ScalarCleanup</code>; use
     * <code>core::VectorCleanup</code> for pointees requiring
     * <code>delete[]</code>.
     *
     * Write your own trait for pointee classes requiring a specialized approach
     * to destruction (such as singletons or reference counted objects). The
     * trait class has to provide a public static <code>Delete()</code> member
     * function working on its single argument of pointee type as required.
     *
     * Refer to <b>Traits: The else-if-then of Types</b> by Andrei Alexandrescu
     * in C++ Report, 12(4), April 2000.
     *
     * <b>Original Copyright</b>
     *
     * <i>Copyright (c) 1997
     * Silicon Graphics Computer Systems, Inc.
     *
     * Permission to use, copy, modify, distribute and sell this software and its
     * documentation for any purpose is hereby granted without fee, provided that
     * the above copyright notice appear in all copies and that both that
     * copyright notice and this permission notice appear in supporting
     * documentation.
     * Silicon Graphics makes no representations about the suitability of this
     * software for any purpose.  It is provided "as is" without express or
     * implied warranty.</i>
     *
     * @author  Christian Werner
     * @since Viper libCore/Base R2
     *
     * @see core::ArrayAutoPtr
     * @see core::AutoPtrCStr
     * @see core::AutoPtrJNI
     * @see core::ScalarCleanup
     * @see core::VectorCleanup
     * @see core::CleanupCStr
     */
    template <typename T, class TR=ScalarCleanup<T*> > class AutoPtr
    {
    private:
        /** Pointee. */
        T* pointee_;

    public:
        /**
         * Constructor.
         * @param pointee A heap-allocated object to hold, defaults to
         *                <code>nullptr</code>. The manner of object allocation
         *                must be compatible to the cleanup trait.
         */
        inline explicit AutoPtr(T* pointee = 0): pointee_(pointee)
        {}

        /**
         * Copy constructor.
         * Pointee ownership is transferred from the right hand side to the new
         * instance.
         * @param rhs The right hand side instance.
         */
        inline AutoPtr(AutoPtr& rhs): pointee_(rhs.release())
        {}

        /**
         * Assignment operator.
         * The left hand side's payload is disposed of. Pointee ownership is
         * transferred from the right hand side to the left hand side instance.
         * Assignment to self is dealt with properly.
         * @param rhs The right hand side instance.
         */
        inline AutoPtr& operator=(AutoPtr& rhs)
        {
            if (&rhs != this)
            {
                if (pointee_) TR::Destroy(pointee_);
                pointee_ = rhs.release();
            }
            return *this;
        }

        /**
         * Non-virtual destructor.
         * The current pointee is disposed of properly. Ok, that's what auto
         * pointers are all about in the first place: An auto pointer on the
         * stack guards a pointer to some resource, which is freed automatically
         * as soon as the pointer instance goes out of scope.
         */
        inline ~AutoPtr()
        {
            if (pointee_) TR::Destroy(pointee_);
        }

        /** Dereferencing operator. */
        inline T& operator*(void) const
        {
            return *pointee_;
        }

        /** Arrow operator. */
        inline T* operator->(void) const
        {
            return pointee_;
        }

        /**
         * Get the current pointee.
         * This call's result is exactly equivalent to the arrow operator's.
         * @return This instance's payload.
         */
        inline T* get(void) const
        {
            return pointee_;
        }

        /** Relinquish ownership of the current pointee. */
        inline T* release()
        {
            T* tmp = pointee_;
            pointee_ = 0;
            return tmp;
        }

        /**
         * Reset the auto pointer.
         * Deletes the object currently pointed to, assigns a new one if
         * appropriate.
         * @param pointee Another object to hold, defaults to
         *                <code>nullptr</code>.
         */
        inline void reset(T* pointee = 0)
        {
            if (pointee_) TR::Destroy(pointee_);
            pointee_ = pointee;
        }
    };

    //--------------------------------------------------------------------------
    // Implementation of class AutoPtrCStr
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Base/AutoPtr.hh"
     * Auto pointer for C strings.
     * Convenience class as shorthand notation for the auto pointer class
     * template applied to <code>const char*</code> strings.
     *
     * @version $Id$
     * @author  Christian Werner
     * @since Viper libCore/Base R3.1.1
     * @see core::AutoPtr
     */
    typedef AutoPtr<const char, CleanupCStr > AutoPtrCStr;

    //--------------------------------------------------------------------------
    // Implementation of class core::ArrayAutoPtr
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Base/AutoPtr.hh"
     * Auto pointer for arrays of objects.
     * Convenience class equivalent to the auto pointer class template used with
     * a vector cleanup trait (i.e., the array is freed using
     * <code>delete[]</code>.
     *
     * Caution: this class cannot be used for arrays of dynamically allocated
     * <code>char*</code> strings, as the individual strings cannot be disposed
     * of by calling <code>delete[]</code> on the whole array.
     *
     * @version $Id$
     * @author  Christian Werner
     * @since   ParaVision 6.Alpha.0.144
     * @see core::AutoPtr
     */
    template <typename T> class ArrayAutoPtr {

    private:
        /** Pointee. */
        T* pointee_;

    public:
        /**
         * Constructor.
         * @param pointee A heap-allocated array to hold, defaults to
         *                <code>nullptr</code>. Allocation must use C++
         *                <code>new[]</code>, rather than C
         *                <code>malloc()</code>.
         */
        inline explicit ArrayAutoPtr(T* pointee = 0): pointee_(pointee)
        {}

        /**
         * Copy constructor.
         * Pointee ownership is transferred from the right hand side to the new
         * instance.
         * @param rhs The right hand side instance.
         */
        inline ArrayAutoPtr(ArrayAutoPtr& rhs): pointee_(rhs.release())
        {}

        /**
         * Assignment operator.
         * The left hand side's payload is disposed of. Pointee ownership is
         * transferred from the right hand side to the left hand side instance.
         * Assignment to self is dealt with properly.
         * @param rhs The right hand side instance.
         * @return Reference to changed \c this.
         */
        inline ArrayAutoPtr& operator=(ArrayAutoPtr& rhs)
        {
            if (&rhs != this)
            {
                if (pointee_) VectorCleanup<T*>::Destroy(pointee_);
                pointee_ = rhs.release();
            }
            return *this;
        }

        /**
         * Non-virtual destructor.
         * The current pointee is disposed of properly. Ok, that's what auto
         * pointers are all about in the first place: An auto pointer on the
         * stack guards a pointer to some resource, which is freed automatically
         * as soon as the pointer instance goes out of scope.
         */
        inline ~ArrayAutoPtr()
        {
            if (pointee_) VectorCleanup<T*>::Destroy(pointee_);
        }

        /// Dereferencing operator.
        inline T& operator*(void) const
        {
            return *pointee_;
        }

        /// Arrow operator.
        inline T* operator->(void) const
        {
            return pointee_;
        }

        /**
         * Get the current pointee.
         * This call's result is exactly equivalent to the arrow operator's.
         * @return This instance's payload.
         */
        inline T* get(void) const
        {
            return pointee_;
        }

        /// Relinquish ownership of the current pointee.
        inline T* release()
        {
            T* tmp = pointee_;
            pointee_ = 0;
            return tmp;
        }

        /**
         * Reset the auto pointer.
         * Deletes the object currently pointed to, assigns a new one if
         * appropriate.
         * @param pointee Another object to hold, defaults to
         *                <code>nullptr</code>.
         */
        inline void reset(T* pointee = 0)
        {
            if (pointee_) VectorCleanup<T*>::Destroy(pointee_);
            pointee_ = pointee;
        }
    };
}

#endif

//==========================================================================EOF=
