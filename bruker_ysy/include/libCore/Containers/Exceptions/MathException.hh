//==============================================================================
// Copyright (c) 2009 - 2018
//
// Bruker BioSpin AG
// MRI Division
// CH-8117 Fällanden, Switzerland
//
// All Rights Reserved
//==============================================================================

#ifndef LIB_CORE_CONTAINERS_MATH_EXCEPTION_H
#define LIB_CORE_CONTAINERS_MATH_EXCEPTION_H

#include <string>
#include "libCore/Base/ExceptionBase.hh"

namespace core
{
    //--------------------------------------------------------------------------
    // Interface of class MathException
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Containers/Exceptions/MathException.hh"
     * Mathematics exception.
     * A concrete exception class; the argument leading to the problem,
     * given as a parameter to the constructor, is a C string.
     *
     * @author Christian Werner
     * @since ParaVision 6.Alpha.0.72
     */
    class MathException : public ExceptionBase
    {
    public:
        /**
         * Constructor.
         *
         * @param className The class name of the object throwing the exception.
         * @param message   A message string.
         */
        inline MathException(std::string className, std::string message)
        : ExceptionBase(className, "Math exception raised: "),
        message_(message)
        {
        }

        /** Copy constructor. */
        inline MathException(const MathException &rhs)
                : ExceptionBase(rhs), message_(rhs.message_)
        {
        }

        /** Destructor. */
        virtual ~MathException() throw()
        {
        }

    protected:
        /**
         * Helper to display the description message.
         * See this exception's base class, <tt>ExceptionBase</tt>,
         * for a detailed description.
         */
        virtual std::ostream &write(std::ostream &ostream) const override
        {
            ExceptionBase::write(ostream);
            return ostream <<message_;
        }

    private:
        /// Deleted assignment operator (because of const member)
        MathException & operator=(const MathException &) = delete;

        /** The string argument leading to the exception. */
        const std::string message_;
    };
}

#endif

//==========================================================================EOF=
