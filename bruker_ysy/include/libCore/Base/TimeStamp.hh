//==============================================================================
// $Source$
//
// Copyright (c) 1997 - 2017
//
// Bruker BioSpin AG
// MRI Division
// CH-8117 Fällanden, Switzerland
//
// All Rights Reserved
//
// $Id$
//==============================================================================
#ifndef LIB_CORE_BASE_TIMESTAMP_H
#define LIB_CORE_BASE_TIMESTAMP_H

#include <cstdint>
#include "libCore/Base/internal.hh"

namespace core
{
    //--------------------------------------------------------------------------
    // Interface of class TimeStamp
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Base/TimeStamp.hh"
     * Record modification and/or execution time.
     * <code>CoreTimeStamp</code> records a unique time when the method
     * <code>modified()</code> is executed. This time is guaranteed to be
     * monotonically increasing across all instances of this class.
     *
     * <b>Usage</b>
     *
     * Classes use this object to record modification and/or execution time.
     * There is built in support for the binary <code><</code> and
     * <code>></code> comparison operators between two
     * <code>TimeStamp</code> objects.
     *
     * <b>Wrap-around</b>
     *
     * It is possible for this number to wrap around back to zero. This should
     * only happen for processes that have been running for a very long time,
     * while constantly changing objects within the program. When this does
     * occur, the typical consequence should be that some objects will update
     * themselves when really they don't need to.
     *
     * @version $Id$
     * @author Christian Werner
     * @since ParaVision 6.Alpha.0.72
     */
    class DLL_INTERFACE TimeStamp
    {
    public:

        /** Default constructor. */
        TimeStamp(void) : modifiedTime_(0)
        {}

        /** Copy constructor. */
        explicit TimeStamp(const TimeStamp &rhs)
            : modifiedTime_(rhs.modifiedTime_)
        {}

        /** Assignment operator. */
        TimeStamp & operator=(const TimeStamp &rhs)
        {
            modifiedTime_ = rhs.modifiedTime_;
            return *this;
        }

        /**
         * Set this object's time to the current time.
         * The current time is just a monotonically increasing 64-bit integer,
         * it is the same among all instances of this class.
         */
        void modified(void);

        /** Return this object's modification time. */
        inline uint64_t getMTime() const
        {
            return modifiedTime_;
        }

        /**
         * Greater than operator for modification times.
         * @param  rhs Another time stamp.
         * @return     <code>true</code> if <code>this</code> is greater than
         *             <code>rhs</code>; <code>false</code> otherwise.
         */
        inline bool operator>(const TimeStamp& rhs) const
        {
            return (modifiedTime_ > rhs.modifiedTime_);
        }

        /**
         * Less than operator for modification times.
         * @param  rhs Another time stamp.
         * @return     <code>true</code> if <code>this</code> is less than
         *             <code>rhs</code>; <code>false</code> otherwise.
         */
        inline bool operator<(const TimeStamp& rhs) const
        {
            return (this->modifiedTime_ < rhs.modifiedTime_);
        }

        /** Conversion to a number. */
        inline operator uint64_t() const
        {
            return modifiedTime_;
        }

    private:
        /** This instance's modification time. */
        uint64_t modifiedTime_;
    };
}

#endif

//==========================================================================EOF=
