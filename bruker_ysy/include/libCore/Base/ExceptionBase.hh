//==============================================================================
// Copyright (c) 1998 - 2018
//
// Bruker BioSpin AG
// MRI Division
// CH-8117 Fällanden, Switzerland
//
// All Rights Reserved
//==============================================================================
#ifndef LIB_CORE_BASE_EXCEPTION_BASE_H
#define LIB_CORE_BASE_EXCEPTION_BASE_H

#include <exception>
#include "libCore/Base/Core.hh"
#include "libCore/Base/internal.hh"

namespace core
{
    class ContainerBase;

    //--------------------------------------------------------------------------
    // Declaration of class core::ExceptionBase
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Base/ExceptionBase.hh"
     * Container library exception base class.
     * This is the root of the container library's exception class hierarchy.
     * Concrete descendants of this abstract base class support the insertion
     * operator to create an error message.
     *
     * @version $Id$
     * @author Christian Werner
     * @since ParaVision 6.Alpha.0.72
     */
    class DLL_INTERFACE ExceptionBase : public std::exception
    {
        friend inline std::ostream &operator<<(
                std::ostream&, const ExceptionBase&);
    public:
        /** Constructor for iterator related exceptions. */
        explicit ExceptionBase(const Core::Object *object,
                std::string description);

        /** Constructor for container related exceptions. */
        explicit ExceptionBase(const ContainerBase *container,
                std::string description);

        /**
         * Constructor with class name.
         * Creates a basic exception object; a deep copy will be made of both
         * arguments.
         *
         * @param className   The class name of the object throwing the
         *                    exception.
         * @param description A description string for the exception.
         */
        explicit ExceptionBase(std::string className, std::string description);

        /** Copy constructor. */
        explicit ExceptionBase(const ExceptionBase &rhs);

        /** Assignment operator. */
        ExceptionBase& operator= (const ExceptionBase& rhs) throw();

        /**
         * Destructor.
         * Declared virtual to allow subclassing.
         */
        virtual ~ExceptionBase() throw();

        /**
         * Get a string describing what happened.
         * @return A descriptive string. Ownerships is retained, so the string
         *         may neither be altered nor freed by the caller.
         */
        virtual const char* what() const throw() override;

    protected:
        /**
         * Inaccessible helper to display the description message.
         * Never called directly, use the insertion operator overloaded for
         * this class.
         *
         * @param ostream The output stream to write into.
         * @return        The output stream.
         */
        virtual std::ostream &write(std::ostream &ostream) const;

    private:
        /** Class name of the object throwing the exception. */
        std::string className_;

        /** Exception description string. */
        std::string description_;

        /** Buffer for result of <code>what()</code>. */
        mutable std::string whatBuf_;
    };

    //--------------------------------------------------------------------------
    // Interface of non-member functions
    //--------------------------------------------------------------------------
    /**
     * <code>ExceptionBase</code> helper to overload the insertion
     * operator.
     * @param  ostream            The output stream to write into.
     * @param  containerException An instance of a descendant of the container
     *                            exception base class,
     *                            <code>ExceptionBase</code>.
     * @return                    The output stream.
     */
    inline std::ostream &operator<<(std::ostream &ostream,
            const ExceptionBase &containerException)
    {
        return containerException.write(ostream);
    }
}

#endif

//==========================================================================EOF=
