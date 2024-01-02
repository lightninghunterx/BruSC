//==============================================================================
// $Source$
//
// Copyright (c) 2000 - 2016
//
// Bruker BioSpin AG
// MRI Division
// CH-8117 Fällanden, Switzerland
//
// All Rights Reserved
//
// $Id$
//==============================================================================
#ifndef LIB_CORE_BASE_CLEANUP_H
#define LIB_CORE_BASE_CLEANUP_H

namespace core
{
    //--------------------------------------------------------------------------
    // Interface of class core::NoCleanup<T>
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Base/Cleanup.hh"
     * Do-nothing payload cleanup trait.
     * This is a class template to be bound to a container's payload type. It
     * just does nothing.
     * <br>
     * <b>Usage</b>
     * <br>
     * This class can be applied either as a stateless trait template where just
     * <code>Destroy()</code> is called, or as trait object, where the instances
     * are used through calls to <code>destroy()</code>.
     * <br>
     * <b>Customizing Cleanup</b>
     * <br>
     * If some specialized  cleanup function is needed to fit your purpose,
     * then write a cleanup class similar to this one; including a customized
     * <code>Cleanup()</code> member.
     * You cannot rely on derivation, because the virtual function
     * mechanism prevents inlining the cleanup function -- which would render
     * useless this whole framework.
     *
     * @version $Id$
     * @author  Christian Werner
     * @since VIPER libCore/Base R3.1.1
     * @see core::ScalarCleanup
     * @see core::VectorCleanup
     * @see core::CleanupCStr
     */
    template <class T>
    class NoCleanup
    {
    public:
        /** Constructor (dummy -- does nothing). */
        inline explicit NoCleanup(bool = true)
        {}

        /** Assignment operator. */
        inline NoCleanup &operator=(const NoCleanup &)
        {
            return *this;
        }

        /**
         * Static destroy function.
         * It's a dummy -- it just does nothing. We're doing call by reference
         * here to get around copy constructor calls.
         */
        static inline void Destroy(T&)
        {}

        /**
         * Destroy member function.
         * It's a dummy -- it just does nothing. We're doing call by reference
         *  here to get around copy constructor calls.
         */
        inline void destroy(T&)
        {}
    };

    //--------------------------------------------------------------------------
    // Interface of class core::ScalarCleanup<T>
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Base/Cleanup.hh"
     * Scalar payload cleanup trait.
     * This is a class template to be bound to a container's payload type. It
     * destroys a payload instance with the <code>delete</code> operator.
     * <br>
     * <b>Usage</b>
     * <br>
     * This class can be applied either as a stateless traits template where just
     * <code>Destroy()</code> is called, or as traits object, where the instances
     * hold a local enable flag and are used through calls to
     * <code>destroy()</code>.
     * <br>
     * <b>Customizing Cleanup</b>
     * <br>
     * If some specialized  cleanup function is needed to fit your purpose,
     * then write a cleanup class similar to this one; including a customized
     * <code>Cleanup()</code> member.
     * You cannot rely on derivation, because the virtual function
     * mechanism prevents inlining the cleanup function --- which would render
     * useless this whole framework.
     *
     * @version $Id$
     * @author  Christian Werner
     * @since VIPER libCore/Base R3.1.1
     * @see core::NoCleanup
     * @see core::VectorCleanup
     * @see Core::CleanupCStr
     */
    template <class T>
    class ScalarCleanup
    {
    public:
        /** Constructor. */
        inline explicit ScalarCleanup(bool enabled=true) : enabled_(enabled)
        {}

        /** Assignment operator. */
        inline ScalarCleanup &operator=(const ScalarCleanup &rhs)
        {
            if (this != &rhs) enabled_ = rhs.enabled_;
            return *this;
        }

        /** Destroy member function. */
        inline void destroy(T pointee)
        {
            if (enabled_) delete pointee;
        }

        /** Static destroy function. */
        static inline void Destroy(T pointee)
        {
            delete pointee;
        }

    protected:
        /** Enabled flag. */
        bool enabled_;
    };

    //--------------------------------------------------------------------------
    // Interface of class core::VectorCleanup<T>
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Base/Cleanup.hh"
     * Vector payload cleanup trait.
     * This is a class template to be bound to a container's payload type. It
     * destroys a payload instance with the <code>delete[]</code> operator.
     * <br>
     * <b>Usage</b>
     * <br>
     * This class can be applied either as a stateless trait template where just
     * <code>Destroy()</code> is called, or as trait object, where the instances
     * hold a local enable flag and are used through calls to
     * <code>destroy()</code>.
     * <br>
     * <b>Customizing Cleanup</b>
     * <br>
     * If some specialized  cleanup function is needed to fit your purpose,
     * then write a cleanup class similar to this one; including a customized
     * <code>Cleanup()</code> member.
     * You cannot rely on derivation, because the virtual function
     * mechanism prevents inlining the cleanup function --- which would render
     * useless this whole framework.
     *
     * @version $Id$
     * @author  Christian Werner
     * @since VIPER libCore/Base R3.1.1
     * @see core::NoCleanup
     * @see core::ScalarCleanup
     * @see core::CleanupCStr
     */
    template <class T>
    class VectorCleanup
    {
    public:
        /** Constructor. */
        inline explicit VectorCleanup(bool enabled=true)
            : enabled_(enabled)
        {}

        /** Assignment operator. */
        inline VectorCleanup &operator=(const VectorCleanup &rhs)
        {
            if (this != &rhs) enabled_ = rhs.enabled_;
            return *this;
        }

        /** Static destroy function. */
        static inline void Destroy(T pointee)
        {
            delete[] const_cast<T>(pointee);
        }

        /** Destroy member function. */
        inline void destroy(T pointee)
        {
            if (enabled_) Destroy(pointee);
        }

    protected:
        /** Enabled flag. */
    bool enabled_;
    };

    //--------------------------------------------------------------------------
    // Interface of class core::CleanupCStr
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Base/Cleanup.hh"
     * Payload cleanup trait for C strings.
     * This is a class template to be bound to a container's payload type. It
     * destroys a <code>const char*</code> payload instance with the
     * <code>delete[]</code> operator.
     * <br>
     * <b>Customizing Cleanup</b>
     * <br>
     * If some specialized  cleanup function is needed to fit your purpose,
     * then write a cleanup class similar to this one; including a customized
     * <code>Cleanup()</code> member.
     * You cannot rely on derivation, because the virtual function
     * mechanism prevents inlining the cleanup function --- which would render
     * useless this whole framework.
     *
     * @version $Id$
     * @author  Christian Werner
     * @since VIPER libCore/Base R3.1.1
     * @see core::NoCleanup
     * @see core::ScalarCleanup
     * @see core::VectorCleanup
     */
    typedef class VectorCleanup<const char*> CleanupCStr;

    //--------------------------------------------------------------------------
    // Interface of class core::DeleteMeCleanup<T>
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Base/Cleanup.hh"
     * DeleteMe payload cleanup trait.
     * This is a class template to be bound to a container's payload type. It
     * destroys a payload instance with the <code>deleteMe()</code> operation
     * common to reference counted classes.
     *
     * @version $Id$
     * @author  Christian Werner
     * @since VIPER libCore/Base R3.4.1
     * @see core::NoCleanup
     * @see core::ScalarCleanup
     * @see core::CleanupCStr
     */
    template <class T>
    class DeleteMeCleanup
    {
    public:
        /** Constructor. */
        inline explicit DeleteMeCleanup(void)
        {}

        /** Static destroy function. */
        static inline void Destroy(T pointee)
        {
            pointee->deleteMe();
        }

        /** Destroy member function. */
        inline void destroy(T pointee)
        {
            Destroy(pointee);
        }
    };
}

#endif

//==========================================================================EOF=
