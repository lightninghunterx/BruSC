/****************************************************************
 *
 * Copyright (c) 1995 - 2019
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef LIBPVVERSION_H
#define LIBPVVERSION_H

#ifdef DLL_INTERFACE
#undef DLL_INTERFACE
#endif

#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#ifdef libPvVersion_INTERNAL
#define DLL_INTERFACE __declspec(dllexport)
#else
#define DLL_INTERFACE __declspec(dllimport)
#endif
#elif defined (__GNUC__) && __GNUC__ >= 4
#define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#define DLL_INTERFACE
#endif 

#include <time.h>

#define VERSION_COMPILE_FORMAT "%Y%m%d-%H%M%S"
#define VERSION_COMPILE_DISPLAY_FORMAT "%Y-%m-%d %H:%M:%S"


#ifdef __cplusplus
extern "C" {
#endif

    /** @file
     *  @defgroup PvVersion ParaVision version library.
     *  <p>
     *  <b>Multiple Implementations!</b>
     *  <br>
     *  Implementations exist in C (here) as well as in Java 
     *  (<code>de.bruker.mri.lib.libcore.misc.system</code> package).
     */
    /*@{*/

    /**
     * Version Type enumeration.
     * Order is important.
     */
    typedef enum PvVersionType {
        /** Release Version. */
        Release,

        /** Alpha version. */
        Alpha,

        /** Beta version. */
        Beta,

        /** Release Patchlevel version. */
        Patchlevel,

        /** Release Candidate version. */
        ReleaseCandidate,

        /** Unknown Type. */
        VersionTypeUnknown,
    } PV_VERSION_TYPE;

    /// Return name of manufacturing company.
    DLL_INTERFACE const char * PvVersionGetCompanyName(void);

    /// Return the product name (ParaVision).
    DLL_INTERFACE const char * PvVersionGetProductName(void);

    /// Return the short product name (PV).
    DLL_INTERFACE const char * PvVersionGetShortProductName(void);

    /** Get a string with the current ParaVision version.
     *  A string of the form 
     *  <code>Major[.Type].Minor[.Revision][plPatchLevel]</code>
     *  is returned in the variable <code>version</code>.
     *  @param version The version string to return.
     *  @param maxLen Maximum length of version string memory.
     *  @return Length of filled  version string.
     */
    DLL_INTERFACE int PvVersionGetString(char * version, int maxLen);
    
    /** Get a string with the current ParaVision version.
     *  A string of the form 
     *  <code>Product-Major[.Type].Minor[.Revision][plPatchLevel]</code>
     *  is returned in the variable <code>version</code>.
     *  @param version The version string to return.
     *  @param maxLen Maximum length of version string memory.
     *  @return Length of filled  version string.
     */
    DLL_INTERFACE int PvVersionGetProductString(char * version, int maxLen);
    
    /** 
     * Get a the display string for the current ParaVision version.
     *  @param version The version string to return.
     *  @param maxLen Maximum length of version string memory.
     *  @return Length of filled  version string.
     */
    DLL_INTERFACE int PvVersionGetDisplayString(char * version, int maxLen);

    /** Returns the version creation time. */
    DLL_INTERFACE time_t PvVersionGetCreationTime(void);

    /** Returns version creation time string.
     @param[out] date Memory to hold the version date.
     @param[in] datelen Number of bytes allocated.
     @return Number of characters placed in \c date, not including the 
     terminating null byte. 0 if it was not able to place the version 
     number.
     */
    DLL_INTERFACE int PvVersionGetCreationTimeString(char * date, int datelen);

    /** Return version type. */
    DLL_INTERFACE enum PvVersionType PvVersionGetType(void);

    DLL_INTERFACE const char * PvVersionGetTypeString(void);

    /** Return the version type number. */
    DLL_INTERFACE int PvVersionGetTypeNumber(void);

    /** Return major version number. */
    DLL_INTERFACE int PvVersionGetMajor(void);

    /** Return minor version number. */
    DLL_INTERFACE int PvVersionGetMinor(void);

    /** Return revision number. */
    DLL_INTERFACE int PvVersionGetRevision(void);
    
    /** Get the version tag. */
    DLL_INTERFACE const char * PvVersionGetTag(void);
    
    /** Get the version branch. */
    DLL_INTERFACE const char * PvVersionGetBranch(void);

    /** Return the patchlevel. */
    DLL_INTERFACE int PvVersionGetPatchlevel(void);

    /** Return version name, empty string if not set. */
    DLL_INTERFACE const char * PvVersionGetName(void);

    /** Returns 1 if current version is a release version else 0. */
    DLL_INTERFACE int PvVersionIsRelease(void);

    /** Returns the version creation time. */
    DLL_INTERFACE time_t PvVersionGetCommitTime(void);

    /** 
     * Returns compile / commit  time string.
     * @param[out] t Memory to hold the compile / commit time.
     * @param[in] tlen Number of bytes allocated.
     * @return Number of characters placed in \c t, not including the 
     * terminating null byte. 0 if it was not able to place the version 
     * number.
     */
    DLL_INTERFACE int PvVersionGetCommitTimeString(char * t, int tlen);

    /**
     * Return the commit identification hash.
     * @return The commit identification hash for the version, empty string if 
     * unknown or version was not created from a commit but from a modified files.
     */
    DLL_INTERFACE const char * PvVersionGetCommitHash(void);


    /** @} */

#ifdef __cplusplus
}
#endif

#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

