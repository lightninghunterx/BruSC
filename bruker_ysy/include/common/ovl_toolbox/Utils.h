/****************************************************************
 *
 * Copyright (c) 1995-2018
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVOVLTOOLS_UTILS_H
#define PVOVLTOOLS_UTILS_H

#ifndef CPROTO

#include <vector>
#include <string>

#include "Parx/publicTypes.h"
#include "common/ovl_toolbox/DLLMacro.h"

/*==============================================================*/
/** @file
 *  @addtogroup PvOvlTools ParaVision overlay toolbox
 *  <p>This utility library contains general functions for:</p>
 *  - general utility functions used in overlays, methods.
 */
/*@{*/


/* gen/src/prg/ovl_toolbox/Utils.c */


/** Initializes a scalar dynamic enum parameter with a value set.
 *  The new value of the dynamic enum is the old value.
 *  If the old value is not anymore in the enum value set the
 *  \a defaultVal is used. The \a defaultValue is also used if
 *  \a forceDefault is \c Yes (independent of the old enumeration
 *  value). If neither the old value nor the 
 *  \a defaultVal is in the new enumeration value set the first
 *  value of the enumeration set is used.
 *  
 *  @param[in] parname parameter name of dynamic enum.
 *  @param[in] dynEnumVals dynamic enum value set. Each entry of the set
 *         has the format ValueName[\\Display Name], e.g. if using 
 *         the enum value name "FirstValue" and the value display name
 *         "This is the first value" the entry should be (in C Notation)
 *         "FirstValue\\This is the first value". If a display name is not
 *         specified the enum value name is used as display name. In this
 *         case the entry may be  simply "FirstName".
 *  @param[in] defaultVal Default new value of dynamic enum, maybe \c 0.
 *  @param[in] forceDefault Indicates if the default value is used 
           independent of the old enumeration value.
 *  @return A value is returned if the enumeration parameter has a new 
 *          value set. 2 is returned if the default value was set, 1 is
 *          returned if the original value was set, and 0 is returned if
 *          the first value from the value set was set.
 *  @throws PvException if it was not possible to set the new value set or a 
 *          new value.
 */
DLL_INTERFACE int PvOvlUtilInitDynEnum(
    const char * parname, const std::vector<std::string> & dynEnumVals,
    const char * defaultVal, bool forceDefault);

/** Initializes a scalar dynamic enumeration parameter with a value set.
 *  The new value of the dynamic enumeration is the old value.
 *  If the old value is not anymore in the enumeration value set the
 *  \a defaultVal is used. The \a defaultValue is also used if
 *  \a forceDefault is \c true (independent of the old enumeration
 *  value). If neither the old value nor the 
 *  \a defaultVal are in the new enumeration value set the first
 *  value of the enumeration set is used.
 *  
 *  @param parname parameter name of dynamic enumeration.
 *  @param dynEnumVals List of dynamic enumeration values. Each entry of the set
 *         is a pair of the value name and display name.
 *  @param defaultVal Default new value of dynamic enumeration, maybe \c nullptr.
 *  @param forceDefault Indicates that the default value is used 
           independent of the old enumeration value.
 *  @return A value is returned if the enumeration parameter has a new 
 *          value set. 2 is returned if the default value was set, 1 is
 *          returned if the original value was set, and 0 is returned if
 *          the first value from the value set was set.
 *  @throws PvException if it was not possible to set the new value set or a 
 *          new value.
 */
DLL_INTERFACE int PvOvlUtilInitDynEnum(const char * parname, 
    const std::vector<std::pair<std::string, std::string>> & dynEnumVals,
    const char * defaultVal, bool forceDefault);

/** Initialize a scalar dynamic enumeration parameter with directory names.
 *  Scan the path in \a pathName and use the sub directories
 *  as enumeration values.
 *  @param[in] parname Parameter name of dynamic enumeration.
 *  @param[in] pathName Path to use sub directories as enumeration values.
 *  @param[in] defaultVal Default path, will be set if it is in the 
 *              new dynamic enumeration set.
 *  @return Number of enumeration values bound to the parameter or -1 on error.
 *  @exception PvException if an array parameter was given, this routine 
 *         works only for scalar dynamic enumerations.
 */
DLL_INTERFACE int PvOvlUtilInitDynEnumWithSubdirNames(
    const char * parname, const char * pathName, const char * defaultVal);

/** Initialize a scalar dynamic enumeration parameter with file names. 
 *  Scan the path in \a pathName and use the file names
 *  as enumeration values.
 *  @param[in] enumParName Parameter name of dynamic enumeration.
 *  @param[in] pathName    Path to use sub directories as enumeration values.
 *  @param[in] ext The filename extension. Only file with this extension
 *    are added to the enumeration value set. If the \c . is missing at the
 *    beginning of the extension it is added the the tests. If \a ext is 0 
 *    all files are added
 *  @param[in] removeExt Indicates if the extension should be removed before
 *    adding to the enumeration value set. This parameter has no meaning if
 *    \a ext is 0.
 *  @param defaultVal Default value for the enumeration, will only be set if
 *          value is in list. 
 *  @return  Number of enumeration values bound to the parameter 
 *            or -1 on error.
 *  @exception PvException if an array parameter was given, this routine 
 *         works only for scalar dynamic enumerations.
 */
DLL_INTERFACE int PvOvlUtilInitDynEnumWithRegFileNames(
    const char * enumParName, const char * pathName, const char * ext,
    YesNo removeExt, const char * defaultVal);

/** Get study path of current dataset attached to parameter space.
 *  @param[out] path memory to store data study path + suffix, 
 *              PATH_MAX elements must be allocated.
 *  @param[in] len length of \a path string including terminating character 
 *  @param[in] suffix suffix of study path.
 *  @return  The length of the constructed path.
 *  @retval -1 if \a path is \c 0.
 *  @retval -2 if the parameter space is not bound to a dataset.
 *  @retval -3 If the length \a len is not sufficient to hold
 *        the constructed path.
 */
DLL_INTERFACE int PvOvlUtilGetStudyPath(char * path, int len,
    const char * suffix);

/** Get experiment path of current dataset attached to parameter space.
 *  @param[out] path memory to store data experiment path + suffix, 
 *              PATH_MAX elements must be allocated.
 *  @param[in] len length of \a path string including terminating character 
 *  @param[in] suffix suffix of experiment path.
 *  @return  The length of the constructed path.
 *  @retval -1 if \a path is \c 0.
 *  @retval -2 if the parameter space is not bound to a dataset.
 *  @retval -3 If the length \a len is not sufficient to hold
 *        the constructed path.
 */
DLL_INTERFACE int PvOvlUtilGetExpnoPath(char * path, int len,
    const char * suffix);

/** Get procno path of current dataset attached to parameter space.
 *  @param[out] path memory to store data procno path + suffix, 
 *              PATH_MAX elements must be allocated.
 *  @param[in] len length of \a path string including terminating character 
 *  @param[in] suffix suffix of procno path.
 *  @retval -1 if \a path is \c 0.
 *  @retval -2 if the parameter space is not bound to a dataset.
 *  @retval -3 If the length \a len is not sufficient to hold
 *        the constructed path.
 */
DLL_INTERFACE int PvOvlUtilGetProcnoPath(char * path, int len,
    const char * suffix);


/** Delivers the dB value corresponding to 0W peak amplitude
    @return attenuation in dB
 */
DLL_INTERFACE double PvOvlUtilZeroWattDb(void);

/** Delivers the peak amplitude in W that corresponds to a 0dB attenuation 
    @return peak amplitude in W for 0dB attenuation
 */

DLL_INTERFACE double PvOvlUtilZeroDbWatt(void);

/** Converts an attenuation in dB into a peak power in W. For values out of 
    range the minimum or max power will be delivered.

    @param attn attenuation in dB
    @return peak power in W
 */
DLL_INTERFACE double PvOvlUtilConvDbToWatt(double attn);

/** Converts a peak power in W  into an attenuation in dB. For values out
    of range the 0W or the max supported peak power (not HW dependent)
    is delivered.

    @param power peak power in W
    @return attenuation in dB
 */
DLL_INTERFACE double PvOvlUtilConvWattToDb(double power);

/*@}*/

#endif
#endif


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
