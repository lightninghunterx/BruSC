/****************************************************************
 *
 * Copyright (c) 2009 - 2018
 * Bruker BioSpin MRI mbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 ****************************************************************/


#ifndef PVUTIL_TIMEDEFS_H
#define PVUTIL_TIMEDEFS_H

#ifndef CPROTO

#ifndef PARX_COMPILE_OVL
#include <cstdint>
#endif

/** @file
 *  @addtogroup PvUtil
 */

/*@{*/



/** Length of Iso8601 Time string */
#define PV_NORM_TIME_LEN  32

/** Time structure for ParaVision usage. */
typedef struct PvTimeStruct
{
    /** Time in seconds since January 1st, 1970, 00:00:00. */
    int64_t tsec;
    /** Milliseconds after tsec. */
    int msec;

    /** Offset of creating time zone in minutes east. */
    int tzoffset;
} pvtime_t;

/*@}*/

#endif

#endif
