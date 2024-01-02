/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2000 - 2015
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVUTIL_DATAPATHTYPE_H
#define PVUTIL_DATAPATHTYPE_H


/** 
 * @file
 * @addtogroup PvUtil 
 */
/*@{*/

/** Request types to parse data set paths.
    Used as argument to <code>ParseDataPathname(E)/ParseFramePathname(E)</code>.
 */
typedef enum DataPathType
{
    /** The path to be parsed is minimum a study path. */
    IsNamePath,
    /** The path to be parsed is minimum an experiment path. */
    IsExpnoPath,
    /** The path to be parsed is minimum a processing path. */
    IsProcnoPath,
    /** The path to be parsed is a precessing path with appended frame number. */
    IsFramePath
} DatPathLenT;

/** Result code of the data path parsing functions. */
enum DataPathResultCode
{
    /** Error code: path to parse is not an experiment path. */
    ExpnoPathNotFound = -1,
    /** Error code: path to parse is not a processing path. */
    ProcnoPathNotFound = -2,
    /** Error code: path to parse is not a processing with appended frame no. */
    FramePathNotFound = -3,
    /** Error code: path to parse is not a study path. */
    NamePathNotFound = -4,
    /** Error code: One of the  path length to a component  is too small. */
    PathBufferTooSmall = -5,
    /** Error code: path to parse is relative and not absolute. */
    RelativePathError = -6,
    /** Error code: path to parse is not a data set path. */
    NoDataPath = -7,
    /** Error code: Invalid argument of parse function found. */
    BadPathArguments = -8,
    /** Error code: Old style data set path requested but not found. */
    NoOldStyleDataPath = -9,
    /** Error code: Experiment number is out of range. */
    ExpnoOutOfRange = -10,
    /** Error code: Processing number is out of range. */
    ProcnoOutOfRange = -11,

    /** Indication that a name path is found. */
    NamePathFound = 4,
    /** Indication that an experiment path is found. */
    ExpnoPathFound = 5,
    /** Indication that a processing path is found. */
    ProcnoPathFound = 6,
    /** Indication that a processing path with appended frame number is found. */
    FramePathFound = 7
};

/*@}*/


#endif /* PVUTIL_DATAPATHTYPE_H */


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/


