/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1995-2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef LIBPSTUL_H
#define LIBPSTUL_H

#include "modality.h"

/** 
 *  @ingroup PvParx
 *  @defgroup libPsTul Parx client utility functions.
 *  This utility library contains functions that should not be used
 *  in overlays and overlay extensions (methods).
 */
/*@{*/

#ifdef DLL_INTERFACE
#undef DLL_INTERFACE
#endif

/// DLL interface libPsTul identifiers for Windows.
#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#ifdef libPsTul_INTERNAL
#define DLL_INTERFACE __declspec(dllexport)
#else
#define DLL_INTERFACE __declspec(dllimport)
#endif
#elif defined (__GNUC__) && __GNUC__ >= 4
#define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#define DLL_INTERFACE
#endif 

/*--------------------------------------------------------------*
 * prototype files...
 *--------------------------------------------------------------*/

#ifndef CPROTO

/* gen/src/lib/Parx/libPsTul/DataSetPsid.c */
/** Set the default parameter space mode.
    @param mode Dataset parameter space key suffix.
    @return old parameter space mode.
 */
DLL_INTERFACE int PvSetDsetPsMode(int mode);

/// Return default parameter space key suffix for creation purposes.
DLL_INTERFACE int PvGetDsetPsMode();

/** Set the default opening mode for a parameter space.
    If this function is not called the default is prototyping mode.
    @param[in] mode If \c Yes the dataset parameter spaces are opened using
        prototyping mode else in normal mode.
    @return The old prototyping mode setting.
    @see PvGetDsetPrototypingMode
 */
DLL_INTERFACE bool PvSetDsetPrototypingMode(bool mode);

/// Return the default opening mode for not completed datasets.
DLL_INTERFACE bool PvGetDsetPrototypingMode();


/** 
 * Select a dataset as CPR dataset.
 * The given \c procnoPath is the new CPR dataset. If the new dataset is not
 * the current dataset a parameter space for this dataset is opened using
 * the current parameter space and opening mode. The dataset is registered in 
 * the CPR object file.
 * @see PvGetDsetPsMode
 * @see PvGetDsetPrototypingMode
 * @param procnoPath The dataset in this path is the new CPR dataset.
 * @param cprObjFile The CPR object file where the dataset is registered.
 * @return The parameter space id for the new CPR dataset.
 */
DLL_INTERFACE int PvCprSelectObject(const char * procnoPath, const char * cprObjFile);

/** 
 * Create a parameter space from a dataset described in the CURD parameters in
 * a CPR object file
 * 
 * @param cprObjFile Path to the CPR object file with the CURD parameters.
 * @return Parameter space id of a dataset from the given CURD parameters.
 */
DLL_INTERFACE int PvCprSetObject(const char * cprObjFile);


/* gen/src/lib/Parx/libPsTul/PvCprPsid.c */

/** Get the parameter space id for the CPR dataset.
    @return The parameter space identifier for the CPR dataset.
 */
DLL_INTERFACE int PvGetCprPsid(void);

/** 
 * Set the parameter space id for the CPR dataset and save as current CRP object.
 * @param psid Parameter space from which the CPR dataset is derived.
 * @param cprObjFile Path to the current CPR object file.
 */
DLL_INTERFACE void PvSetCprPsid(int psid, const char * cprObjFile);

/// Close the parameter space for the CPR dataset.
DLL_INTERFACE void PvDestroyCprPsid(void);

/* gen/src/lib/Parx/libPsTul/WorkPs.c */


/** 
 * Create a temporary parameter space for datasets and a given modality.
 *  A temporary parameter space is created and all dataset overlays
 *  are pushed into the parameter space. This parameter space can be used for
 *  datasets in all status.
 * @param modalityType The modality for the parameter space.
 *  @return An identifier to newly created temporary parameter space.
 *  @exception PvException if it is not possible to create the 
 *             parameter space.
 */
DLL_INTERFACE int CreateTemporaryPs(PvModalityType modalityType);

/** Create a temporary parameter space for completed datasets.
 *  A temporary parameter space is created and all dataset overlays
 *  are pushed into the parameter space. This parameter space can only 
 *  be used for completed datasets.
 * @param modalityType The modality for the parameter space.
 *  @return An identifier to a new temporary parameter space.
 *  @exception PvException if it is not possible to create the 
 *                         parameter space.
 */
DLL_INTERFACE int CreateTemporaryROPs(PvModalityType modalityType);

/** Create a temporary uninitialized parameter space.
 *  A temporary parameter space is created and overlays are not pushed into
 *  this parameter space.
 * @param modalityType The modality for the parameter space.
 *  @return An identifier to new temporary parameter space..
 *  @exception PvException if it is not possible to create the parameter space.
 */
DLL_INTERFACE int CreateUninitializedTemporaryPs(
    PvModalityType modalityType);

/** Destroy temporary uninitialized parameter space.
 *  Temporary parameter spaces created by CreateTemporaryPs, 
 *  CreateTemporaryROPs, and CreateUninitializedTemporaryPs are closed
 *  using this function.
 *  @see CreateTemporaryPs
 *  @see CreateTemporaryROPs
 *  @see CreateUninitializedTemporaryPs
 *  @param psid pointer to an existing temporary parameter space identifier.
 *  @exception PvException if it is not possible to close the parameter space.
 */
DLL_INTERFACE void DestroyTemporaryPs(int psid);

#endif

/*@}*/

#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
