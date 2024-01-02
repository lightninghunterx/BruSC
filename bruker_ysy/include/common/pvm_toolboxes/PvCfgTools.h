/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVCFGTOOLS_H
#define PVCFGTOOLS_H

#ifndef CPROTO

#include "common/PvTypes/commonTypes.h"

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

/// DLL interface libParx identifiers for Windows.
#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#  ifdef PvCfgTools_INTERNAL
#    define DLL_INTERFACE __declspec(dllexport)
#  else
#    define DLL_INTERFACE __declspec(dllimport)
#  endif
#elif defined (__GNUC__) && __GNUC__ >= 4
#    define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#    define DLL_INTERFACE
#endif



/** Returns maximum size of a dataset (MByte).

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @return Maximum size of a dataset (MByte) represented as double value

  @ingroup PVM_CFG_INT
*/
DLL_INTERFACE double CFG_MaxDatasetSize(void);




/** Returns maximum isotropic spatial matrix size

  This matrix size is dependent on the spatial dimensionality of the experiment.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Illegal dimension argument. Valid values: @c 1, @c 2 or @c 3

  @PVM_EXAMPLE
  @code
  int maxMatrix = CFG_MaxIsotropicSpatialMatrixSize(PTB_GetSpatDim());
  @endcode

  @param dim    Spatial dimension

  @return Maximum isotropic spatial matrix size represented as integer value

  @see @ref CFG_DefaultImagingSpatialMatrixSize,
       @ref CFG_MaxImagingSpatialMatrixSize, @ref CFG_MinImagingSpatialMatrixSize

  @ingroup PVM_CFG
*/
DLL_INTERFACE int CFG_MaxIsotropicSpatialMatrixSize(int dim);




/** Returns a default value for the spatial imaging matrix size.

  This function should be used to initialize the elements of the spatial imaging matrix.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @return Default value for the spatial imaging matrix size for each spatial
 *  dimension.

  @see @ref CFG_MaxIsotropicSpatialMatrixSize,
       @ref CFG_MaxImagingSpatialMatrixSize, @ref CFG_MinImagingSpatialMatrixSize,
       @ref CFG_DefaultSliceThickness, @ref CFG_DefaultExtent
 *
 * @throws PvException on error.

  @ingroup PVM_CFG
*/
DLL_INTERFACE void CFG_DefaultImagingSpatialMatrixSize(int sizes[3]);




/** Returns the maximal allowed spatial matrix size.

  Returns the maximal allowed value for an element of a spatial matrix array represented as
  integer.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @return Maximal allowed spatial matrix size for each spatial dimension.

  @see @ref CFG_MaxIsotropicSpatialMatrixSize, @ref CFG_DefaultImagingSpatialMatrixSize,
       @ref CFG_MinImagingSpatialMatrixSize
 *
 * @throws PvException on error.

  @ingroup PVM_CFG
*/
DLL_INTERFACE void CFG_MaxImagingSpatialMatrixSize(int sizes[3]);




/** Returns the minimal allowed spatial matrix size.

  Returns the minimal allowed value for an element of a spatial matrix array represented as
  integer.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @return Minimal allowed spatial matrix size for each spatial dimension.

  @see @ref CFG_MaxIsotropicSpatialMatrixSize, @ref CFG_DefaultImagingSpatialMatrixSize,
       @ref CFG_MaxImagingSpatialMatrixSize
 *
 * @throws PvException on error,

  @ingroup PVM_CFG
*/
DLL_INTERFACE void CFG_MinImagingSpatialMatrixSize(int sizes[3]);



/** Delivers the max possible number of slice packages.

  Note: The max number of slice packages is also dependent on the total number of
  slices. The value returned by this function assumes that the total number of slices
  (dependent on the number of slices per slice package) is not exceeded.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @return Max number of slice packages that can be handled by the software

  @see @ref CFG_MaxSlices

  @ingroup PVM_CFG
*/
DLL_INTERFACE int CFG_MaxSlicePackages(void);




/** Delivers the max possible number of slices.

  The value returned by this  function is the max value of slices that can be
  handled by the software.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @return Max number of slices that can be handled by the software

  @see @ref CFG_MaxSlicePackages

  @ingroup PVM_CFG
*/
DLL_INTERFACE int CFG_MaxSlices(void);




/**  Returns a default slice thickness (mm).

  This slice thickness is usable to initialize an appropriate parameter.
  The value is adapted to the instrument.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_DefaultExtent

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @return Default slice thickness represented as double

  @see @ref CFG_DefaultExtent, @ref CFG_DefaultImagingSpatialMatrixSize

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_DefaultSliceThickness(void);





/*********************************************************************************/
/* gen/src/prg/pvm_toolboxes/PvCfgTools/utilities.c                              */
/*********************************************************************************/

/** Returns the current instrument name.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @return Name of the instrument in use.

  @ingroup PVM_CFG
*/
DLL_INTERFACE const char *CFG_InstrumentName(void);




/**
 * Returns a default field of view (FOV)(mm).
 *
 * This field of view (FOV) is usable to initialize an appropriate
 * parameter. The values are adapted to the instrument.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @param[out] extent  Default extent for each spatial dimensions in [mm].

  @see @ref CFG_DefaultSliceThickness, @ref CFG_DefaultImagingSpatialMatrixSize

 *
 * @throws PvException in case of an error.

  @ingroup PVM_CFG
*/
DLL_INTERFACE void CFG_DefaultExtent(double extent[3]);


/**
 * Returns the instrument offset for each spatial directions in mm.
 *


  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @return The offset of the instrument to the modality coordinate system
 * origin.

 * @throws PvException in case of an error.

 *   @ingroup PVM_CFG
*/
DLL_INTERFACE void CFG_GetInstrumentOffset(double offset[3]);



#endif
#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
