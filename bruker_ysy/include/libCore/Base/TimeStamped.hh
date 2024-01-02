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
#ifndef LIB_CORE_BASE_TIME_STAMPED_H
#define LIB_CORE_BASE_TIME_STAMPED_H

#include "libCore/Base/TimeStamp.hh"
#include "libCore/Base/internal.hh"

namespace core
{
    //--------------------------------------------------------------------------
    // Interface of class TimeStamped
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Base/TimeStamped.hh"
     * Base class for timestamped objects.
     * Descendants of this class "know" about the time of their last
     * modification.
     * This is useful for helper objects such as matrices. Pipeline stages,
     * however, do not derive from this class, they use one or two timestamp
     * object(s) directly through composition.
     *
     * @version $Id$
     * @author Christian Werner
     * @since ParaVision 6.Alpha.0.72
     */
    class DLL_INTERFACE TimeStamped
    {
    public:
        /**
         * Default constructor.
         * Creates a timestamped object with initial modification time set to
         * the current time. Thus, new objects always are outdated and will take
         * part of the next update.
         */
        TimeStamped(void);

        /**
         * Copy constructor.
         * The value of the time stamp gets copied.
         * @param rhs Instance to copy from.
         */
        TimeStamped(const TimeStamped& rhs);

        /**
         * Assignment operator.
         * The value if the time stamp gets copied.
         * @param rhs Instance to get atribute values from.
         */
        TimeStamped &operator=(const TimeStamped& rhs);

        /** Reference counted object style destructor. */
        virtual void deleteMe(void);

        /** Get the time of the last modification. */
        inline uint64_t getMTime(void) const
        {
            return mTime_.getMTime();
        }

        /**
         * Mark this instance as modified.
         * This sets the object's modification time to the current logical time.
         */
        virtual void modified(void);

    protected:
        /**
         * Virtual destructor allows for subclassing.
         * As for all timestamped objects, it is protected and should remain so
         * in derived classes. <code>deleteMe()</code> is the preferred way to
         * destroy such objects.
         */
        virtual ~TimeStamped()
        {}

        /** This instance's modification time. */
        TimeStamp mTime_;
    };
}

#endif

//==========================================================================EOF=
