/****************************************************************
 *
 * Copyright (c) 1995-2020
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 ****************************************************************/

#ifndef PVOVLTOOLS_ADJ_H
#define PVOVLTOOLS_ADJ_H


#ifndef CPROTO

#ifdef DLL_INTERFACE
#undef DLL_INTERFACE
#endif

#include "pvmachine.h"
#include <vector>
#include "PvUtil/DataPathType.h"
#include "PvAdj/AdjUtils.hh"
#include "PvAdj/adjManagerTypes.h"
#include "PvAdj/ResultParsType.h"
#include "common/ovl_toolbox/DLLMacro.h"

struct AdjConfigType;

namespace PvAdj
{
    class HardwareContext;
}

/*==============================================================*/
/** @file
 * @defgroup PvOvlTools ParaVision overlay toolbox
 *  <p>This utility library contains general functions for:</p>
 *  - utility routines to handle adjustment results
 */
/*@{*/

/** 
 * Delivers actual adjustment configuration.
 * @throws PvException in case of a PARX
 */
DLL_INTERFACE const PvAdj::AdjConfiguration * PvAdjManGetCurrentConfiguration(void);


/** 
 * Set adjusted result parameter list.
 * The parameter \c AdjParameterListSize is set to \c cnt and
 * the \c list is copied into the parameter \c AdjParameterList.
 * @param list The list of adjusted parameters.
 * @param cnt Number of adjusted parameters.
 * @param isScanInstr Indicates if a scan instrument adjustment is set up,
 * @return 0 if ok, -1 on error.
 * @throws PvException in case of a PARX error or if one element
 *    in the list is a global result.
 */
DLL_INTERFACE int PvAdjManSetAdjParameterList(const ADJ_RESULT_PAR * list,
    int cnt, bool isScanInstr = false);

/** 
 * Append an adjusted result parameter to the result parameter list.
 * The list must be set by \c PvAdjManSetAdjParameterList before using
 * this function.
 * @param elem An adjusted parameter.
 * @param isScanInstr Indicates if a scan instrument adjustment is set up,
 * @return 0 if ok, -1 on error.
 * @throws PvException in case of a PARX error.
 */
DLL_INTERFACE int PvAdjManAppendAdjParameterList(const ADJ_RESULT_PAR * elem,
    bool isScanInstr = false);

/** Initialize an empty adjustment context. 
 *  @param adj adjustment context to initialize.
 */
DLL_INTERFACE void PvAdjManInitContext(AdjContextType * adj);


/** Sets if a new experiment is requested for the adjustment.
    @param newExpno If \c Yes a new experiment is used for
    the adjustment, if \c No the current experiment is used.
 */
DLL_INTERFACE void PvAdjManRequestNewExpno(bool newExpno);

/** Return path to the adjustment method result directory.
 *  @return a directory path or 0 if
 *           - this routine in called outside of an adjustment
 *           - or PvAdjManStorePar was not called before.
 */
DLL_INTERFACE const char * PvAdjManGetStudyResultPath();



/**
 * Get all method contexts for a given method.
 * @param method The method name.
 * @param mehtContexts A list of all method contexts is set, empty
 *     if no method contexts are registered for this method.
 * @throws PvException on error.
 */
DLL_INTERFACE void PvAdjManGetValidMethodContexts(
    std::vector<std::string> & methContexts,
    const char * method);

/** Restore adjusted information into the dataset parameter space.
 *  This routine can be used inside and outside an adjustment.
 *  @param parname parameter name to restore.
 *  @param method  adjustment method that adjusted the parameter. 
 *                 may be 0, then the last successful adjustment
 *                 that this parameter adjusted is used.
 *  @param specialRequest Special request that the adjustment manager must 
 *         handle, it will be compared to the method context.
 *  @param resultType If not \c 0 fill it with the location of the result,
 *         i.e. global, user, or study result.
 *  @return 0 if restore was successful, negative on error.
 */
DLL_INTERFACE int PvAdjManRestorePar(const char * parname,
    const char * method,
    const char * specialRequest,
    AdjResultType * resultType);

/** Restore adjusted information into the dataset parameter space.
 *  This routine can be used inside and outside an adjustment.
 *  @param parname parameter name to restore.
 *  @param method  adjustment method that adjusted the parameter. 
 *                 may be 0, then the last successful adjustment
 *                 that this parameter adjusted is used.
 *  @param specialRequest Special request that the adjustment manager must 
 *         handle, it will be compared to the method context.
 *  @param resultType If not \c 0 fill it with the location of the result,
 *         i.e. global, user, or study result.
 *  @param refHwContext A reference hardware context used for the search.
 *  @return 0 if restore was successful, negative on error.
 */
DLL_INTERFACE int PvAdjManRestoreParFromHwContext(
    const char * parname, const char * method,
    const char * specialRequest, AdjResultType * resultType,
    const PvAdj::HardwareContext & refHwContext);

/** Get the result dataset path of a scan adjustment.
    The path is constructed using the entries of \c res. It is tested if this
    path exists.
    @param path memory to fill the constructed path.
    @param len the maximum length of the memory for the path.
    @param dtype Type of the dataset path to be constructed (.e.g. study, 
        experiment, or processing path).
    @param res The scan adjustment result path description.
    @param resType The type of the adjustment result (.e.g. study, user, or
        global result).
    @param suffix suffix to be added to the constructed path separated by a 
        \c '/'. If \c 0 the path without suffix is returned.
    @return  The length of the constructed path.
    @retval -1 if \c pathname is \c 0.
    @retval -2 if the parameter space is not bound to a .
    @retval -3 if the length \a len is not sufficient to hold
        the constructed path.
    @retval -4 if the result dataset PROCNO path does not exist. In this
        case the result of the scan adjustment is invalid.

 */
DLL_INTERFACE int PvAdjManGetProcnoResultPath(
    char * path, int len, DataPathType dtype, const AdjProcnoResult & res,
    AdjResultType resType, const char * suffix);


/** Append an adjustment to an adjustment list.
    The adjustment types \c per_scan and \c on_demand have corresponding
    adjustment lists. This function adds the given adjustment into these 
    lists. 
    @param[in] adjType The type of the adjustment.
    @param[out] element The adjustment context to be added.
    @throws PvException in case of an error. If \c adjType is \c per_study
       an exception is thrown because per_study adjustments cannot be 
       configured using these lists. 
    @see PvAdjListPerpendAdjustment
 */
DLL_INTERFACE void PvAdjListAppendAdjustment(AdjPerformListType adjType,
    const AdjContextType & element);

/** Append a configured adjustment to an adjustment list.
    The adjustment types \c per_scan and \c on_demand have corresponding
    adjustment lists. This function adds the given adjustment into these 
    lists. 
    @param[in] adjType The type of the adjustment.
    @param[out] element The configured adjustment to be added.
    @throws PvException in case of an error. If \c adjType is \c per_study
       an exception is thrown because per_study adjustments cannot be 
       configured using these lists. 
    @see PvAdjListPerpendAdjustment
 */
DLL_INTERFACE void PvAdjListAppendAdjustment(AdjPerformListType adjType,
    const AdjConfigType & element);

/** Add an adjustment at the beginning or an adjustment list.
    The adjustment types \c per_scan and \c on_demand have corresponding
    adjustment lists. This function adds the given adjustment into these 
    lists. 
    @param[in] adjType The type of the adjustment.
    @param[out] element The adjustment context to be added.
    @throws PvException in case of an error. If \c adjType is \c per_study
       an exception is thrown because per_study adjustments cannot be 
       configured using these lists. 
    @see PvAdjListAppendAdjustment
 */
DLL_INTERFACE void PvAdjListPrependAdjustment(AdjPerformListType adjType,
    const AdjContextType & element);

/** Clear the corresponding adjustment list. 
    
    Meaning of argument \c clearall = \c No:

    If \c adjType is \c per_scan adjustments the \c per_scan adjustment
    list is built up by all adjustments of the configured \c per_scan 
    order list with \c isMandatory flag set to \c Yes

    For \c adjType is \c on_demand the \c on_demand adjustment list is built
    up by all adjustments of the known adjustment list with \c isOndemand flag
    set to Yes in the adjustment configuration.

    @param[in] adjType The type of the adjustment list.
    @param[in] clearall (default Yes) if No some adjustments are inserted  
    @throws PvException in case of an error or if \c adjType is
        \c per_study.
    @see PvAdjListPrependAdjustment
    @see PvAdjListAppendAdjustment
 */
DLL_INTERFACE void PvAdjListClear(AdjPerformListType adjType, bool clearall = true);

/**
 * Find adjustments in the per_study or per_scan order list.
 * @param adjType Specify the list of adjustments.
 * @param name  The name of the adjustment. 
 * @return The configured adjustment configuration, {@code 0} if adjustment
 *    is not found.
 * @throws PvException on error.
 */
DLL_INTERFACE const AdjConfigType * PvAdjFindConfiguredOrderAdjustments(
    AdjPerformListType adjType, const char * name);

/** Find a configured adjustment by name.
    @param[in] name The adjustment name.
    @return a pointer to the adjustment with given category and name,
       \c 0 if such an adjustment does not exist.
    @throws PvException in case of an error (e.g. config overlay
        not pushed).
 */
DLL_INTERFACE const AdjConfigType * PvAdjFindConfiguredAdjustment(const char * name);

/**
 * Set the parameter \c AdjConfigurationMode from a modality study parameter.
 * 
 * If \c AdjConfigurationMode does not exit nothing happens. If the dataset 
 * loaded in a way that parameters can be changed for acquisition the relation 
 * of \c AdjConfigurationMode is performed after the setting.
 * 
 * @param studyPar The modality specific study parameter with the adjustment
 * configuration mode. 
 */
DLL_INTERFACE void PvAdjSetConfigurationModePar(const char * studyPar);


/**
 * Set the value set of adjustment configuration into a dynamic enumeration.
 * @param dynEnumName The dynamic enumeration parameter name.
 * @param initConfig Re-initialize the configuration structures.
 * @retval 1 if a list of adjustment configuration sets exist
 * @retval 0 if a 
 */
DLL_INTERFACE int PvAdjSetConfigurationEnumValueSet(
    const char * dynEnumName, bool initConfig);

/**
 * Get the original dataset path for the adjustment
 * 
 * @return The dataset path for which the adjustment platform was started. If 
 * not in the adjustment platform return the dataset path for which the 
 * adjustment will be started.
 */
DLL_INTERFACE AdjProcnoResult PvAdjGetOriginalDatasetPath();

#endif /* CPROTO */

/*@}*/

#endif


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
