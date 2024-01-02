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

#ifndef PVGEOTOOLS_H
#define PVGEOTOOLS_H

#include "bruktyp.h"
#include "PvGeoClasses.hh"
#include "subject/subjtyp.h"

#ifndef CPROTO

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

/// DLL interface libParx identifiers for Windows.
#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#  ifdef PvGeoTools_INTERNAL
#    define DLL_INTERFACE __declspec(dllexport)
#  else
#    define DLL_INTERFACE __declspec(dllimport)
#  endif
#elif defined (__GNUC__) && __GNUC__ >= 4
#    define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#    define DLL_INTERFACE
#endif



/*************************************************************************/
/* gen/src/prg/pvm_toolboxes/PvGeoTools/InplaneGeoTools.c                */
/*************************************************************************/
/* range checkers and relations */
DLL_INTERFACE void GTB_InplaneGeoSpatialMatrixLimits (void);


/** Initializes spatial matrix limits.

  This function sets @c PVM_InplaneGeoSpatialMatrixLimits. This parameter
  serves as a storage area of values for @c PVM_MinMatrix and
  @c PVM_MaxMatrix, to restore limits, initialized in the @c initMeth
  of a method (usually done with routine @ref STB_InitEncoding).

  The elements of @c PVM_InplaneGeoSpatialMatrixLimits are set according
  to the arguments in the following way:

  - Element 0 - 2 = @a lowMat[0] - @a lowMat[2] (>= @c CFG_MinImagingSpatialMatrixSize)
  - Element 3 - 5 = @a upMat[0] - @a upMat[2] (<= @c CFG_MaxImagingSpatialMatrixSize)

  @PVM_PARS
  - @c PVM_InplaneGeoSpatialMatrixLimits

  @PVM_TBCALL
  - @ref CFG_MinImagingSpatialMatrixSize
  - @ref CFG_MaxImagingSpatialMatrixSize

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Each element of @a lowMat must be greater or equal to @ref CFG_MinImagingSpatialMatrixSize.
  - Each element of @a upMat must be lower or equal to @ref CFG_MaxImagingSpatialMatrixSize.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  int loMat[3] = {16, 16, 8};
  int upMat[3] = {512, 512, 64};

  if (GTB_InitSpatialMatrixLimits(loMat, upMat) == No)
  {
    UT_ReportError("illegal values of matrix size arguments");
  }
  @endcode

  @param lowMat    3-element @c int array containing minimum spatial matrix sizes
  @param upMat     3-element @c int array containing maximum spatial matrix sizes

  @return @c Yes if arguments are in valid range, @c No otherwise

  @see @ref STB_InitEncoding

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE YesNo GTB_InitSpatialMatrixLimits(int *lowMat,
                                  int *upMat);

/** Checks dimension and ranges of a spatial matrix.

  Dependent on argument @a spatDim, the dimension and ranges for
  - @c PVM_MinMatrix
  - @c PVM_MaxMatrix
  - @c PVM_Matrix

  are checked to lie within the limits specified during initialization of the spatial imaging
  matrix sizes. This is done usually when initialising or updating the encoding group.

  @PVM_PARS
  - @c PVM_MinMatrix
  - @c PVM_MaxMatrix
  - @c PVM_Matrix

  @PVM_TBCALL
  - @ref UT_IntArraySetInRangeArray

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @ref GTB_InitSpatialMatrixLimits has been called successfully.

  @PVM_ERROR
  - Illegal value of argument @a spatDim
  - @ref GTB_InitSpatialMatrixLimits has not been called successfully before.

  @PVM_EXAMPLE
  @code
  GTB_UpdateSpatialMatrixRanges(PTB_GetSpatDim());
  @endcode

  @param spatDim    Spatial dimensionality (@c 1, @c 2, or @c 3)

  @return @c Yes = no error occured

  @see @ref STB_InitEncoding, @ref STB_UpdateEncoding, @ref GTB_InitSpatialMatrixLimits

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE YesNo GTB_UpdateSpatialMatrixRanges(const int spatDim);




/** Constrains the isotropic state dependent on spatial matrix ranges.

  Checks if an @c Isotropic_matrix is possible or not. The arguments @a minMatrix, @a maxMatrix,
  and @a spatMatrix are checked to be consistant: Each element of @a spatMatrix must lie within
  the range of @a minMatrix and @a maxMatrix. The imaging dimensionality is checked to lie
  within the range of @c 1-3. If the minimum and maximum matrix sizes do not allow isotropic
  resolution, the isotropic state is changed to @c Isotropic_None.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_MinImagingSpatialMatrixSize

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Legal values of arguments, the array arguments have to have a size of @a dim.

  @PVM_ERROR
  - Illegal values of arguments @a minMatrix, @a maxMatrix, and @a spatMatrix
  - Illegal dimensionality argument @a dim
  - Argument @a iState should be one of the states @c Isotropic_Matrix, @c Isotropic_All,
    @c Isotropic_Fov,  @c Isotropic_Resolution, @c Isotropic_None

  @PVM_EXAMPLE
  @code
  PV_ISOTROPIC isotropicState = PVM_Isotropic;

  GTB_ConstrainIsotropicState(&isotropicState,
                              PTB_GetSpatDim(),
                              PVM_MinMatrix,
                              PVM_MaxMatrix,
                              PVM_Matrix);
  @endcode

  @param iState        Points to a variable of type @c PV_ISOTROPIC
  @param dim           Spatial dimensionality
  @param minMatrix     Minimum spatial matrix size, must have at least @a dim elements
  @param maxMatrix     Maximum spatial matrix size, must have at least @a dim elements
  @param spatMatrix    Spatial matrix size, must have at least @a dim elements

  @return Yes in case of changed argument @a iState or errors

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE YesNo GTB_ConstrainIsotropicState(PV_ISOTROPIC *iState,
                                                const int    dim,
                                                const int    *minMatrix,
                                                const int    *maxMatrix,
                                                int          *spatMatrix);




/** Calculates spatial resolution from matrix and FOV.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_MinImagingSpatialMatrixSize
  - @ref CFG_MaxImagingSpatialMatrixSize

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Illegal dimension argument @a dim (out of range @c 1-3)
  - Illegal elements of argument @a matrix (out of range as defined  y
    @ref CFG_MinImagingSpatialMatrixSize and @ref CFG_MaxImagingSpatialMatrixSize)
  - Elements of @a fov are less or equal to @c 0.0

  @PVM_EXAMPLE
  @code
  GTB_FovToSpatResol(PTB_GetSpatDim(),
                     PVM_Matrix,
                     PVM_Fov,
                     PVM_SpatResol);
  @endcode

  @param dim          Spatial dimensionality
  @param matrix       Spatial imaging matrix, expects an int array of size
                      @a dim
  @param fov          Field of view (mm), a double array of size @a dim is
                      expected.
  @param spatResol    Spatial resolution (mm/pt), a double array of size
                      @a dim is expected.

  @return @c Yes = no error occured

  @ingroup PVM_GTB
*/
DLL_INTERFACE YesNo GTB_FovToSpatResol(const int     dim,
                                       const int    *matrix,
                                       const double *fov,
                                       double       *spatResol);





/** Sets up an isotropic matrix within legal ranges.

  Dependent on argument @a refNdx (reference index) the corresponding array
  element of argument @a matrix is checked to lie within the limits defined
  by @a minMatrix and @a maxMatrix. If so, all elements of @a matrix are
  set to the value of this array element.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_MinOfIntArray
  - @ref UT_MaxOfIntArray
  - @ref CFG_MinImagingSpatialMatrixSize
  - @ref CFG_MaxImagingSpatialMatrixSize

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Illegal spatial dimension (argument @a spatDim)
  - Illegal reference index (argument @a refNdx)
  - Illegal values in argument @a minMatrix, @a maxMatrix, and @a matrix

  @PVM_EXAMPLE
  @code
  int indexEdited = UT_RequestIndex(0);

  GTB_IsotropicRequestForMatrix(PVM_Matrix,
                                PVM_MinMatrix,
                                PVM_MaxMatrix,
                                PTB_GetSpatDim(),
                                indexEdited);
  @endcode

  @param matrix       Spatial matrix size, array of length @a spatDim
                      expected
  @param minMatrix    Minimal spatial matrix size, array of length
                      @a spatDim expected
  @param maxMatrix    Maximal spatial matrix size, array of length
                      a spatDim expected
  @param spatDim      Spatial dimensionality, expected to be @c 1, @c 2 or
                      @c 3
  @param refNdx       Reference index expected to lie within @c 1 -
                      @a spatDim

  @return @c Yes in case matrix elements have been changed

  @see @ref GTB_IsotropicRequestForResolution, @ref GTB_IsotropicRequestForFov

  @ingroup PVM_GTB
*/
DLL_INTERFACE YesNo GTB_IsotropicRequestForMatrix(int       *matrix,
                                                  const int *minMatrix,
                                                  const int *maxMatrix,
                                                  int        spatDim,
                                                  int        refNdx);





/** Sets up an isotropic resolution.

  Dependent on argument @a refNdx (reference index) the corresponding array element of
  argument @a resolution is used as reference and transferred to the remaining elements of
  argument @a resolution in case this is compatible with the specified minimum FOV. Each
  element of FOV is then calculated from the desired spatial resolution and the imaging matrix
  specified as argument @a matrix.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref GTB_FovToSpatResol

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Illegal value of argument @a spatDim
  - Illegal value of argument @a refNdx
  - Error conditions of @ref GTB_FovToSpatResol

  @PVM_EXAMPLE
  @code
  int indexEdited = UT_RequestIndex(0);

  GTB_IsotropicRequestForResolution(PVM_Fov,
                                    PVM_MinFov,
                                    PVM_SpatResol,
                                    PVM_Matrix,
                                    PTB_GetSpatDim(),
                                    indexEdited);
  @endcode

  @param fov           Field of view (mm), double array of size @a spatDim expected
  @param minFov        Minimum field of view (mm), double array of size @a spatDim expected
  @param resolution    Spatial resolution (mm/pt), double array of size @a spatDim expected
  @param matrix        Spatial matrix, int array of size @a spatDim expected
  @param spatDim       Spatial dimensionality (@c 1, @c 2 or @c 3 expected)
  @param refNdx        Reference index

  @return @c Yes in case @a fov entries have been changed, else @c No

  @see @ref GTB_IsotropicRequestForMatrix, @ref GTB_IsotropicRequestForFov

  @ingroup PVM_GTB
*/
DLL_INTERFACE YesNo GTB_IsotropicRequestForResolution(double    *fov,
                                                      double    *minFov,
                                                      double    *resolution,
                                                      const int *matrix,
                                                      const int  spatDim,
                                                      const int  refNdx);





/** Sets up an isotropic FOV.

  Dependent on argument @a refNdx (reference index) the corresponding array element of argument
  @a fov is used as reference and transferred to the remaining elements of argument
  @a resolution in case this is compatible with the specified minimum FOV.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_MaxOfDoubleArray

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Illegal value of argument @a spatDim
  - Illegal value of argument @a refNdx
  - Array elements of @a minFov <= @c 0.0

  @PVM_EXAMPLE
  @code
  int indexEdited = UT_RequestIndex(0);

  GTB_IsotropicRequestForFov(PVM_Fov,
                             PVM_MinFov,
                             PTB_GetSpatDim(),
                             indexEdited);
  @endcode

  @param fov        Field of view (mm), double array of size @a spatDim expected
  @param minFov     Minimum field of view (mm), double array of size @a spatDim expected
  @param spatDim    Spatial dimensionality (@c 1, @c 2 or @c 3 expected)
  @param refNdx     Reference index

  @return @c Yes in case @a fov entries have been changed

  @see @ref GTB_IsotropicRequestForMatrix, @ref GTB_IsotropicRequestForResolution

  @ingroup PVM_GTB
*/
DLL_INTERFACE YesNo GTB_IsotropicRequestForFov(double       *fov,
                                               const double *minFov,
                                               const int     spatDim,
                                               const int     refNdx);




/** Returns major slice orientation index from slice vector.

  The index belongs to the component of the slice vector with the maximum absolute value.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_GetNdxOfMaxArr
  - @ref GTB_GetSliceOrientFromSliceNdx

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  for (i = 0; i < PVM_FovSatNSlices; i++)
  {
    PVM_FovSatSliceOrient[i] = GTB_GetSliceOrientFromVec(PVM_FovSatSliceVec + i * 3);
  }
  @endcode

  @param sliceVector    3-dimensional double vector containing the slice orientation
                        (e.g. 3rd row of an orientation matrix)

  @return Major slice orientation represented as @c SLOR_TYPE (@c sagittal, @c coronal or
          @c axial)

  @see @ref UT_GetNdxOfMaxArr, @ref GTB_GetSliceOrientFromSliceNdx

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE SLOR_TYPE GTB_GetSliceOrientFromVec(double *sliceVector);



/*************************************************************************/
/* gen/src/prg/pvm_toolboxes/PvGeoTools/utilities.c                      */
/*************************************************************************/

/** Constrain to fields-of-view in the various axes according to the
    @a isotropicConstraint flag/enum and the minimum field-of-view.

  @a isotropicConstraint takes the values:
  - @c Isotropic_Fov
  - @c Isotropic_Resolution
  - @c Isotropic_Matrix
  - @c Isotropic_All
  - @c Isotropic_None

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_MaxOfDoubleArray

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a minFov is @c NULL pointer.
  - @a fov is a @c NULL pointer.
  - @a fovCm is a @c NULL pointer.
  - @a matrix is a @c NULL pointer.
  - @a spatDim is out of range.
  - @a refNdx is out of range.

  @PVM_EXAMPLE
  @code
  GTB_ConstrainFov(PVM_Isotropic, PVM_MinFov, PVM_Fov, PVM_FovCm,
                   PVM_Matrix, PTB_GetSpatDim(), 0 );
  @endcode

  @param isotropicConstraint    Required constraint
  @param minFov                 Array of size @a spatDim containing the minimum
                                field-of-view for each dimension (mm)
  @param fov                    Array of size @a spatDim containing the desired
                                fields-of-view (mm). The values in @a fov are
                                modified by this function.
  @param fovCm                  Array of size @a spatDim containing the desired
                                fields-of-view but in cm.
  @param matrix                 Array containing the matrix sizes in each dimension.
                                The values will be modified by this function.
  @param spatDim                Number of dimensions to be considered
  @param refNdx                 Dimension to be taken as reference

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE void GTB_ConstrainFov( PV_ISOTROPIC  isotropicConstraint,
                                     double       *minFov,
                                     double       *fov,
                                     double       *fovCm,
                                     int          *matrix,
                                     int           spatDim,
                                     int           refNdx);





/** Creates a slice order list on the basis of the @a sliceScheme enumeration.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nSlices < @c 1
  - @a sliceOrder is @c NULL pointer.

  @PVM_EXAMPLE
  @code
  int nslices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);

  GTB_SliceOrder(nslices,
                 PVM_ObjOrderScheme,
                 PVM_ObjOrderList);
  @endcode

  @param nSlices          Number of slices
  @param sliceScheme      Enum defining the slice excitation scheme
  @param[out] sliceOrder  Array of size @a nSlices defining the excitation
                          order after call of this routine

  @ingroup PVM_GTB_INT
 */
DLL_INTERFACE void GTB_SliceOrder(int                   nSlices,
                                  PV_SLICE_SCHEME_TYPE  sliceScheme,
                                  int                  *sliceOrder);





/** Initializes or reinitializes @c PVM_Matrix, @c PVM_MinMatrix, and @c PVM_MaxMatrix.

  @PVM_PARS
  - @c PVM_Matrix
  - @c PVM_MinMatrix
  - @c PVM_MaxMatrix

  @PVM_TBCALL
  - @ref UT_RedimensionListOf1d
  - @ref UT_IntArraySetInRangeArray

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Illegal values for argument @a numdims

  @PVM_EXAMPLE
  @code
  GTB_InitAndConstrainMatrix(PTB_GetSpatDim());
  @endcode

  @param numDims    Number of spatial dimensions (@c 1 - @c 3)

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE void GTB_InitAndConstrainMatrix(int numDims);



/** The given matrix transforms the modality coordinate system to the
  patient coordinate system.

  This function delivers a matrix represented as 9-element double array
  that transforms a vector from the modality coordinate system into the patient
  coordinate system. The matrix can be obtained from the 9-element array by
  reshaping it using row-major ordering (C-style).
  The transform is dependent on the subject position of the current study,
  which the function automatically detects.
  If the optional boolean argument @c dicom is equal to No, the transform goes
  from the modality coordinate system to the ParaVision patient system. If it
  is equal to Yes, then it goes to the Dicom patient system. The ParaVision
  patient system differes from the Dicom patient system, by a minus sign in the
  first two coordinates. The default value is No.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  GTB_ObjPosMatrix

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The object position has to be specified during study setup.

  @PVM_ERROR
  - Related subject parameters SUBJECT_study_instrument_position,
  and has no value

  @PVM_EXAMPLE
  @code
  double obj_ori[9];

  // get the transform from modality to ParaVision patient coordinate system for Head_Supine positioning
  GTB_ObjPosMatrix(obj_ori);

  // get the transform from modality to Dicom patient coordinate system, for Head_Supine positioning
  GTB_ObjPosMatrix(obj_ori, Yes);

  @endcode

  @param m     transformation matrix represented as 9-element vector
  @param dicom flag that determines the desired patient coordinate system


  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_ObjPosMatrix(double *m, YesNo dicom=No);





/** The given matrix transforms the modality coordinate system to the
  patient coordinate system.

  This function delivers a matrix represented as 9-element double array
  that transforms a vector from the modality coordinate system into the patient
  coordinate system. The matrix can be obtained from the 9-element array by
  reshaping it using row-major ordering (C-style).
  The transform is dependent on the subject position, which can be specified by
  @c ppos. Possible values are:
  Head_Supine,
  Head_Prone,
  Head_Left,
  Head_Right,
  Foot_Supine,
  Foot_Prone,
  Foot_Left,
  Foot_Right,
  Left_Supine,
  Left_Prone,
  Right_Supine,
  Right_Prone,
  Anterior_Left,
  Anterior_Right,
  Posterior_Left,
  Posterior_Right.
  If the optional boolean argument @c dicom is equal to No, the transform goes
  from the modality coordinate system to the ParaVision patient system. If it
  is equal to Yes, then it goes to the Dicom patient system. The ParaVision
  patient system differes from the Dicom patient system, by a minus sign in the
  first two coordinates. The default value is No.

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

  @PVM_EXAMPLE
  @code
  double obj_ori[9];

  // get the transform from modality to ParaVision patient coordinate system for Head_Supine positioning
  GTB_ObjPosMatrix(Head_Supine, obj_ori);

  // get the transform from modality to Dicom patient coordinate system, for Head_Supine positioning
  GTB_ObjPosMatrix(Head_Supine,obj_ori,Yes);

  @endcode

  @param ppos  subject position
  @param m     transformation matrix represented as 9-element vector
  @param dicom flag that determines the desired patient coordinate system


  @ingroup PVM_GTB
*/
DLL_INTERFACE void GTB_ObjPosMatrix(SubjectInstrPosType ppos, double *m, YesNo dicom=No);



/*************************************************************************/
/* gen/src/prg/pvm_toolboxes/PvGeoTools/GeoObj.c                         */
/*************************************************************************/
/** Geometry: Handle geometry object after its modification.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_GetRequest
  - @ref UT_ClearRequest
  - @ref GTB_CheckPlanningMode

  @PVM_RELCALL
  - Cuboid parameter associated with @a gobjnam

  @PVM_PRECOND
  - The geometry object @a gobjnam must have been created with @ref PTB_InsertGeoObject.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In the relation of @c PVM_SliceGeoObj
  @code
  UT_SetRequest(("PVM_SliceGeoObj"));
  GTB_HandleGeoObject("PVM_SliceGeoObj");
  @endcode

  @param gobjnam  parameter name of geometry object

  @return
  - @c 0 = successful
  - @c -1 = parameter @a gobjnam does not exist
  - @c -2 = parameter @a gobjnam has no value
  - @c -3 = parameter @a gobjnam has incorrect size
  - @c -4 = cuboid parameter associated with @a gobjnam does not exist
  - @c -5 = value of @a gobjnam could not be obtained

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_HandleGeoObject (const char *const gobjnam);





/** Geometry: Handle a geometry cuboid after its modification.

  @PVM_PARS
  - Parameter given by @a gcnam

  @PVM_TBCALL
  - @ref UT_GetRequest
  - @ref UT_RelContext_ParOrigDim
  - @ref UT_RequestParOrigValue

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The geometry cuboid @a gcnam must exist.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In the relation of the cuboid @c PVM_SliceGeo
  @code
  // PVM_SliceGeo has been changed in the geometry editor
  GTB_HandleGeoCuboid("PVM_SliceGeo", PVM_SliceGeoObj.handle);
  UT_ClearRequest();
  @endcode

  @param gcnam  parameter name of geometry cuboid
  @param mask   bit mask holding geometry constraints

  @return
  - @c >= 0: mask after constraining operations
  - @c -1 = parameter @a gcnam does not exist
  - @c -2 = parameter @a gcnam has no value
  - @c -3 = parameter @a gcnam has incorrect size

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_HandleGeoCuboid (const char *const gcnam,
                                       const int         mask);





/** Geometry: Get index after transposition.

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

  @PVM_EXAMPLE
  @code
  int iTransp = GTB_TranspositionIndex(i, cuboidPack[0].surrounding.transp);
  @endcode

  @param ntind          index without taking into account transposition (@c 0-@c 2)
  @param transposition  transposition to be applied to index (@c 0-@c 3)

  @return
  - >= 0: index after taking into account transposition
  - -1 = invalid values of input parameters

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE int GTB_TranspositionIndex(int ntind, int transposition);




/** Geometry: Determines if the geometry editor is currently in planning mode.

  Being in planning mode means that the user makes interactive modifications in the geometry editor
  using the mouse while holding the left mouse button pressed. The planning mode is left when
  the mouse button is released.

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

  @return @c Yes = geometry editor is currently in planning mode, else @c No

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE YesNo GTB_GeoEditorPlanningMode(void);




/** Geometry: Call relations of a geometry group handler depending on the current situation.

  This function is to be called in the relation of a cuboid.
  Either the default or the redirected relation of a group handler associated with
  a geometry object is called, depending on:
  - The geometry editor is in planning mode or not.
  - The geometry object @a gobjnam is currently dealt as a slave for dependencies.
  - The geometry object @a gobjnam is a master of other objects.
    In this case, also the relations of the slave objects will be called.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - If dependencies should be applied, the default relation of the group handler must be prepared for this feature.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In the relation of @c PVM_VoxelGeoObj
  @code
  GTB_GeoObjRelationCalls("PVM_VoxelGeoObj", "PVM_VoxCallBack");
  @endcode

  @param gobjnam  parameter name of geometry object
  @param handler  associated group handler

  @ingroup PVM_GTB
*/
DLL_INTERFACE void GTB_GeoObjRelationCalls(const char *const gobjnam, const char *const handler);




/*************************************************************************/
/* gen/src/prg/pvm_toolboxes/PvGeoTools/GeoObjBitHandling.c              */
/*************************************************************************/
/** Geometry: Checks ratio planning mode of a geometry object.

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  GTB_CheckRatioMode(&PVM_SliceGeoObj);
  @endcode

  @param gobj geometry object

  @return
  - @c 0 = ratio planning mode is correct
  - @c 1 = ratio planning mode was corrected

  @see @ref GTB_CheckPlanningMode

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE int GTB_CheckRatioMode(GEO_OBJECT *const gobj);

/** Geometry: Checks planning mode of a geometry object.

  The return value indicates error conditions and success or modification of the geometric
  behavior:

  <table>
  <tr><th> Return value     </th><th>                                       </th></tr>
  <tr><td> @c 0             </td><td> Planning mode is correct              </td></tr>
  <tr><td> @c 1             </td><td> Planning mode has been corrected      </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value) </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  GTB_CheckPlanningMode(&PVM_SliceGeoObj);
  @endcode

  @param gobj geometry object

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref PTB_InsertGeoObject, @ref GTB_CheckRatioMode

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE int GTB_CheckPlanningMode (GEO_OBJECT *const gobj);




/** Geometry: Checks planning mode of a geometry object.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT.

  The return value indicates error conditions and success or modification of the geometric
  behavior:

  <table>
  <tr><th> Return value     </th><th>                                       </th></tr>
  <tr><td> @c 0             </td><td> Planning mode is correct              </td></tr>
  <tr><td> @c 1             </td><td> Planning mode has been corrected      </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value) </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  GTB_CheckPlanningMode("PVM_SliceGeoObj");
  @endcode

  @param gobjname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref PTB_InsertGeoObject

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_CheckPlanningMode(const char *const gobjname);




/** Geometry: Delivers planning mode of a geometry object.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                       </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed         </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value) </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  int mask = 0;
  GTB_GeoObjGetPlanningMode("PVM_SliceGeoObj", &mask);
  @endcode

  @param gobjname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param[out] mode   Planning mode

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref PTB_InsertGeoObject

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjGetPlanningMode(const char *const gobjname,
                              int               *mode);

/** Geometry: Initializes a planning mode of a geometry object.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                       </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed         </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value) </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  During initialisation of the image geometry
  @code
  GTB_GeoObjInitDefaultPlanningMode("PVM_SliceGeoObj");
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)

  @return @c 0 or 1 on success, negative values in case of error conditions (see above)

  @see @ref PTB_InsertGeoObject

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjInitDefaultPlanningMode(const char *const parname);

/** Geometry: Freezes shift along first direction of a geometry object.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                       </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed         </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value) </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method not allowing off-centre in the first dimension
  @code
  GTB_GeoObjFreezePos0("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos1, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueThickness, @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent1, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent1, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri1, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeMajorOri, @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForceZeroGap, @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjFreezePos0(const char *const parname,
                         YesNo             clear);

/** Geometry: Freezes shift along second direction of a geometry object.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry object description for planar object </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method not allowing off-centre in the second dimension
  @code
  GTB_GeoObjFreezePos1("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueThickness, @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent1, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent1, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri1, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeMajorOri, @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForceZeroGap, @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjFreezePos1 (const char *const parname,
                          YesNo             clear);

/** Geometry: Freezes shift along third direction of a geometry object.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry object description for planar object </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method not allowing off-centre in the third dimension
  @code
  GTB_GeoObjFreezePos2("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos1,
       @ref GTB_GeoObjUniqueThickness, @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent1, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent1, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri1, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeMajorOri, @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForceZeroGap, @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjFreezePos2 (const char *const parname,
                                        YesNo             clear);




/** Geometry: Forces same thickness of subcuboids in arrays of cuboids or planar objects.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. For geomety objects describing a planar
  object, all objects are forced to have the same thickness. If argument @a clear is set to
  @c Yes the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method forcing same cuboid thickness
  @code
  GTB_GeoObjUniqueThickness("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos1, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent1, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent1, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri1, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeMajorOri, @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForceZeroGap, @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjUniqueThickness (const char *const parname,
                                             YesNo             clear);




/** Geometry: Forces same number of subcuboids in arrays of cuboids.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method forcing same number of subcuboids
  @code
  GTB_GeoObjUniqueSubc("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos1, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueThickness,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent1, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent1, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri1, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeMajorOri, @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForceZeroGap, @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjUniqueSubc(const char *const parname,
                                       YesNo             clear);




/** Geometry: Fixed extent along first direction of the geometry object.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method requiring a fixed FOV in the first dimension
  @code
  GTB_GeoObjFreezeExtent0("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos1, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueThickness, @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent1, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent1, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri1, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeMajorOri, @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForceZeroGap, @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjFreezeExtent0 (const char *const parname,
                                           YesNo             clear);




/** Geometry: Fixed extent along second direction of the geometry object.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method requiring a fixed FOV in the second dimension
  @code
  GTB_GeoObjFreezeExtent1("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos1, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueThickness, @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent1, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri1, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeMajorOri, @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForceZeroGap, @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjFreezeExtent1 (const char *const parname,
                                           YesNo             clear);




/** Geometry: Fixed extent along third direction of the geometry object.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method requiring a fixed extent in the third dimension
  @code
  GTB_GeoObjFreezeExtent2("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos1, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueThickness, @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent1,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent1, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri1, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeMajorOri, @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForceZeroGap, @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjFreezeExtent2 (const char *const parname,
                                           YesNo             clear);




/** Geometry: Fixed ratio of first and second extent of geometry object.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method not allowing the user to change the ratio of the FOV sizes
  @code
  GTB_GeoObjFreezeExt0Ext1Ratio("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos1, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueThickness, @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent1, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent1, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri1, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeMajorOri, @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForceZeroGap, @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjFreezeExt0Ext1Ratio (const char *const parname,
                                                 YesNo             clear);

/** Geometry: Forces equal first extents of all array elements of geometric objects.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method
  @code
  GTB_GeoObjUniqueExtent0("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos1, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueThickness, @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent1, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent1, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri1, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeMajorOri, @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForceZeroGap, @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjUniqueExtent0 (const char *const parname,
                                           YesNo             clear);




/** Geometry: Forces equal second extents of all array elements of geometric objects.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method
  @code
  GTB_GeoObjUniqueExtent1("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos1, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueThickness, @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent1, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri1, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeMajorOri, @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForceZeroGap, @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjUniqueExtent1 (const char *const parname,
                                           YesNo             clear);




/** Geometry: Forces equal third extents of all array elements of geometric objects.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method
  @code
  GTB_GeoObjUniqueExtent2("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos1, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueThickness, @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent1, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent1,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri1, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeMajorOri, @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForceZeroGap, @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjUniqueExtent2 (const char *const parname,
                                           YesNo             clear);




/** Geometry: Freezes the first direction of geometric objects.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method not allowing the user to change the orientation in the first dimension
  @code
  GTB_GeoObjFreezeOri0("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos1, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueThickness, @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent1, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent1, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri1, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeMajorOri, @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForceZeroGap, @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjFreezeOri0 (const char *const parname,
                                        YesNo             clear);





/** Geometry: Freezes the second direction of geometric objects.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method not allowing the user to change the orientation in the second dimension
  @code
  GTB_GeoObjFreezeOri1("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos1, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueThickness, @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent1, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent1, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeMajorOri, @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForceZeroGap, @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjFreezeOri1 (const char *const parname,
                                        YesNo             clear);




/** Geometry: Freezes the third direction of geometric objects.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method not allowing the user to change the orientation in the third dimension
  @code
  GTB_GeoObjFreezeOri2("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos1, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueThickness, @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent1, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent1, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri1,
       @ref GTB_GeoObjFreezeMajorOri, @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForceZeroGap, @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjFreezeOri2 (const char *const parname,
                                        YesNo             clear);





/** Geometry: Forces any direction of geometric objects parallel to the gradient coordinate
              system.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method allowing only major slice orientations
  @code
  GTB_GeoObjFreezeMajorOri("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos1, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueThickness, @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent1, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent1, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri1, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForceZeroGap, @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjFreezeMajorOri (const char *const parname,
                                            YesNo             clear);




/** Geometry: Allows rotations only around one axis.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method
  @code
  GTB_GeoObjFreezeRotAlongAxis("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos1, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueThickness, @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent1, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent1, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri1, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeMajorOri,
       @ref GTB_GeoObjForceZeroGap, @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjFreezeRotAlongAxis (const char *const parname,
                                                YesNo             clear);




/** Geometry: Forces zero gap between subcuboids.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method not allowing gaps between slices
  @code
  GTB_GeoObjForceZeroGap("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos1, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueThickness, @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent1, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent1, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri1, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeMajorOri, @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForcePositiveGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjForceZeroGap (const char *const parname,
                                          YesNo             clear);




/** Geometry: Forces positive gap between subcuboids.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In @c initMeth of a method not allowing slice overlap
  @code
  GTB_GeoObjForcePositiveGap("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjFreezePos0, @ref GTB_GeoObjFreezePos1, @ref GTB_GeoObjFreezePos2,
       @ref GTB_GeoObjUniqueThickness, @ref GTB_GeoObjUniqueSubc,
       @ref GTB_GeoObjFreezeExtent0, @ref GTB_GeoObjFreezeExtent1, @ref GTB_GeoObjFreezeExtent2,
       @ref GTB_GeoObjFreezeExt0Ext1Ratio,
       @ref GTB_GeoObjUniqueExtent0, @ref GTB_GeoObjUniqueExtent1, @ref GTB_GeoObjUniqueExtent2,
       @ref GTB_GeoObjFreezeOri0, @ref GTB_GeoObjFreezeOri1, @ref GTB_GeoObjFreezeOri2,
       @ref GTB_GeoObjFreezeMajorOri, @ref GTB_GeoObjFreezeRotAlongAxis,
       @ref GTB_GeoObjForceZeroGap

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjForcePositiveGap (const char *const parname,
                                              YesNo             clear);




/** Geometry: Editor planning mode, modification of thickness.

  Controls behavior of geometry editor. Modification of the extent of geometry objects along
  the third direction modifies the subcuboid thickness at constant gap. The name of the
  parameter used to control the geometry object has to be specified by argument. It is checked
  to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes the functionality is
  disabled. In this case a default planning mode is established that agrees with the currently
  defined constraints.

  Other planning modes are deactivated.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  GTB_GeoObjExtentThickness("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjInsertSubc, @ref GTB_GeoObjExtentKeepGapThickRatio,
       @ref GTB_GeoObjExtentGap, @ref GTB_GeoObjExtentKeepExt0Ext1Ratio, @ref GTB_GeoObjExtentKeepAllRatios,
       @ref GTB_GeoObjInsertObject

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjExtentThickness (const char *const parname,
                                             YesNo             clear);




/** Geometry: Editor planning mode, insertion of subcuboids.

  Controls behavior of geometry editor. Modification of the extent of geometry objects along
  the third direction modifies the number of subcuboids keeping gap and thidkness. The name of
  the parameter used to control the geometry object has to be specified by argument. It is
  checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes the functionality
  is disabled. In this case a default planning mode is established that agrees with the
  currently defined constraints.

  Other planning modes are deactivated.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  GTB_GeoObjInsertSubc("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjExtentThickness, @ref GTB_GeoObjExtentKeepGapThickRatio,
       @ref GTB_GeoObjExtentGap, @ref GTB_GeoObjExtentKeepExt0Ext1Ratio, @ref GTB_GeoObjExtentKeepAllRatios,
       @ref GTB_GeoObjInsertObject

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjInsertSubc (const char *const parname,
                                        YesNo             clear);




/** Geometry: Editor planning mode, expand gap and thickness.

  Controls behavior of geometry editor. Modification of the extent of geometry objects along
  the third direction modifies gap and thickness keeping the gap/thickness ratio. The name of
  the parameter used to control the geometry object has to be specified by argument. It is
  checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes the functionality
  is disabled. In this case a default planning mode is established that agrees with the
  currently defined constraints.

  Other planning modes are deactivated.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  GTB_GeoObjExtentKeepGapThickRatio("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjExtentThickness, @ref GTB_GeoObjInsertSubc,
       @ref GTB_GeoObjExtentGap, @ref GTB_GeoObjExtentKeepExt0Ext1Ratio, @ref GTB_GeoObjExtentKeepAllRatios,
       @ref GTB_GeoObjInsertObject

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjExtentKeepGapThickRatio (const char *const parname,
                                                     YesNo             clear);




/** Geometry: Editor planning mode, expand gap.

  Controls behavior of geometry editor. Modification of the extent of geometry objects along
  the third direction modifies gap keeping the thickness constant. The name of
  the parameter used to control the geometry object has to be specified by argument. It is
  checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes the functionality
  is disabled. In this case a default planning mode is established that agrees with the
  currently defined constraints.

  Other planning modes are deactivated.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  GTB_GeoObjExtentGap("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjExtentThickness, @ref GTB_GeoObjInsertSubc, @ref GTB_GeoObjExtentKeepGapThickRatio,
       @ref GTB_GeoObjExtentKeepExt0Ext1Ratio, @ref GTB_GeoObjExtentKeepAllRatios,
       @ref GTB_GeoObjInsertObject

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjExtentGap (const char *const parname,
                                       YesNo             clear);




/** Geometry: Keeps first and second extension of cuboid constant.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled. Otherwise, if set, the state produced by
  @ref GTB_GeoObjExtentKeepAllRatios is deactivated.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  GTB_GeoObjExtentKeepExt0Ext1Ratio("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjExtentThickness, @ref GTB_GeoObjInsertSubc, @ref GTB_GeoObjExtentKeepGapThickRatio,
       @ref GTB_GeoObjExtentGap, @ref GTB_GeoObjExtentKeepAllRatios,
       @ref GTB_GeoObjInsertObject

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjExtentKeepExt0Ext1Ratio (const char *const parname,
                                                     YesNo             clear);




/** Geometry: Keeps ratio of all extents constant.

  The name of the parameter used to control the geometry object has to be specified by
  argument. It is checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes
  the functionality is disabled. Otherwise, if set, the state produced by
  @ref GTB_GeoObjExtentKeepExt0Ext1Ratio is deactivated.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  <tr><td> @c -4            </td><td> Geometry description of planar objects        </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  GTB_GeoObjExtentKeepAllRatios("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be deactivated (@c Yes)
                    or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjExtentThickness, @ref GTB_GeoObjInsertSubc, @ref GTB_GeoObjExtentKeepGapThickRatio,
       @ref GTB_GeoObjExtentGap, @ref GTB_GeoObjExtentKeepExt0Ext1Ratio,
       @ref GTB_GeoObjInsertObject

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjExtentKeepAllRatios (const char *const parname,
                                                 YesNo             clear);




/** Geometry: Editor planning mode, insertion of cuboids or planar objects.

  Controls behavior of geometry editor. Modification of the extent of geometry objects along
  the third direction will create new objects along this direction. The name of
  the parameter used to control the geometry object has to be specified by argument. It is
  checked to be of type @c GEO_OBJECT. If argument @a clear is set to @c Yes the functionality
  is disabled. In this case a default planning mode is established that agrees with the
  currently defined constraints.

  Other planning modes are deactivated.

  The return value indicates error conditions and success or modification of the geometric
  behavior:
  <table>
  <tr><th> Return value     </th><th>                                               </th></tr>
  <tr><td> @c 0             </td><td> Behavior has already been established         </td></tr>
  <tr><td> @c 1             </td><td> Behavior successfully changed                 </td></tr>
  <tr><td> @c -1            </td><td> Illegal parameter name                        </td></tr>
  <tr><td> @c -2            </td><td> Illegal parameter type                        </td></tr>
  <tr><td> @c -3            </td><td> Parmameter not initialized (no value)         </td></tr>
  </tr>
  </table>

  @PVM_PARS
  - Specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameter must have a value.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  GTB_GeoObjInsertObject("PVM_SliceGeoObj", No);
  @endcode

  @param parname    Name of the geometry object (4th argument of
                    @ref PTB_InsertGeoObject)
  @param clear      Flag indicating whether the functionality should be
                    deactivated (@c Yes) or activated (@c No)

  @return @c 0 on success, negative values in case of error conditions (see above)

  @see @ref GTB_GeoObjExtentThickness, @ref GTB_GeoObjInsertSubc,
  @ref GTB_GeoObjExtentKeepGapThickRatio,
  @ref GTB_GeoObjExtentGap, @ref GTB_GeoObjExtentKeepExt0Ext1Ratio,
  @ref GTB_GeoObjExtentKeepAllRatios

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjInsertObject(const char *const parname,
                                         YesNo             clear);



/*************************************************************************/
/* gen/src/prg/pvm_toolboxes/PvGeoTools/GeoObjDependencies.c             */
/*************************************************************************/
/** Initialise dependencies between two geometry objects.

  The dependency functionality serves to make certain properties of a geometry
  object (slave) dependent on another geometry object (master). Examples are
  cuboid orientation or offsets.

  @PVM_H1{Implementation}
  For implementing dependencies in a method, follow the instructions below
  and find more details in the cited functions. Note, that the geometry object
  must have been prepared for this feature.
  An example is the voxel geometry.
  - Use @ref GTB_GeoObjDependencyInit to initialise dependencies between two
    geometry objects, either once in @c initMeth or with each parameter update
    in @c backbone if dependencies change based on parameter settings. In the
    latter case, @ref GTB_GeoObjDependencyRemove can be used to remove all
    dependencies between two objects.
  - The dependencies are specified with @ref GTB_GeoObjDependencySet, just after
    initialisation.
  - In the @c backbone, the calling order of the updaters related to the geometry
    objects must agree with their
    master-slave hierarchy, i.e. the master updater must be called before the
    updater of the associated slave .

  During the initialisation with the present function, basic features of the
  relation between the two objects are set. In case the selected master-slave
  couple already exists, it is reset and specified with  the new features.

  The following rules apply:
  - A slave can have only one master.
  - There must not be a circular relation.

  @PVM_H1{Number of slaves per master}
  The argument @a nCuboids gives the number of slave cuboids per master cuboid.
  The following cases possible:
  - @a nCuboids > @c  0 : The size of the slave cuboid pack is set automatically in
    @ref GTB_GeoObjDependencyApply.
  - @a nCuboids = @c  0 : The number of slaves is the number the of sub-cuboids in
    the master cuboid. The size of the slave cuboid pack is set automatically in
    @ref GTB_GeoObjDependencyApply.
  - @a nCuboids = @c -1 : Only one slave cuboid exists which is assigned to the first
    master cuboid.
  - @a nCuboids = @c -2 : The value is taken from the ratio of number of slave and
    master cuboids (default), i.e. @a nCuboids = # slaves / # masters. This requires
    the number of slave cuboids to be set @em before calling @ref GTB_GeoObjDependencyApply.

  @PVM_PARS
  - @c PVM_GeoObjDependencies

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Geometry objects given in @a master and @a slave must exist.

  @PVM_ERROR
  - Incorrect input arguments.
  - An object does not exist.
  - A slave already has another master.
  - A circular relation has been created.

  @PVM_EXAMPLE
  Initialisation in @c initMeth
  @code
  // initialise
  int id = GTB_GeoObjDependencyInit("PVM_SliceGeoObj", "PVM_VoxelGeoObj");

  // voxel has same orientation as slice
  GTB_GeoObjDependencySet(id, GeoDep_Ori);
  @endcode

  @param master        parameter name of master object
  @param slave         parameter name of slave object
  @param nCuboids      (optional) number of slave cuboids assigned to one master cuboid
                       - @c  0 = number of sub-cuboids in master
                       - @c -1 = only one slave cuboid for first master
                       - @c -2 = match to master number

  @return identifier of master-slave couple

  @see @ref GTB_GeoObjDependencyNCuboids, @ref GTB_GeoObjDependencyRemove,
  @ref GTB_GeoObjDependencySet

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_GeoObjDependencyInit(
  const char *const master,
  const char *const slave,
  int nCuboids = 1);

/** Remove dependencies between two geometry objects.

  All dependencies for the given couple of geometry objects are removed.
  This function is required if the dependencies are only applied for certain settings
  of method parameters.

  @PVM_PARS
  - @c PVM_GeoObjDependencies

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Incorrect input arguments.
  - An object does not exist.

  @PVM_EXAMPLE
  @code
  GTB_GeoObjDependencyRemove("PVM_SliceGeoObj", "PVM_VoxelGeoObj");
  @endcode

  @param master        parameter name of master object
  @param slave         parameter name of slave object

  @see @ref GTB_GeoObjDependencyInit, @ref GTB_GeoObjDependencyNCuboids, @ref GTB_GeoObjDependencySet

  @ingroup PVM_GTB
*/
DLL_INTERFACE void GTB_GeoObjDependencyRemove( const char *const master,
                                               const char *const slave);




/** Set number of slave cuboids for dependencies between two geometry objects.

  Change the number of slave cuboids assigned to a master cuboid which was initialised
  with the argument @a nCuboids of @ref GTB_GeoObjDependencyInit.

  @PVM_PARS
  - @c PVM_GeoObjDependencies

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Dependency must have been created with @ref GTB_GeoObjDependencyInit.

  @PVM_ERROR
  - Incorrect input arguments.
  - An object does not exist.
  - Dependency couple does not exist.

  @PVM_EXAMPLE
  @code
  GTB_GeoObjDependencyNCuboids("PVM_SliceGeoObj", "PVM_VoxelGeoObj", 2);
  @endcode

  @param master        parameter name of master object
  @param slave         parameter name of slave object
  @param nCuboids      (optional) number of slave cuboids assigned to one master cuboid
                       - @c  0 = number of sub-cuboids in master
                       - @c -1 = only one slave cuboid for first master
                       - @c -2 = match to master number

  @see @ref GTB_GeoObjDependencyInit, @ref GTB_GeoObjDependencyRemove, @ref GTB_GeoObjDependencySet

  @ingroup PVM_GTB
*/
DLL_INTERFACE void GTB_GeoObjDependencyNCuboids( const char *const master,
                                                 const char *const slave,
                                                 int nCuboids = 1);




/** Set dependencies between two geometry objects.

  The following dependencies are available:
   <table>
  <tr><th>Dependency</th>                <th>Description</th>                                <th>@a valuePar (default)</th></tr>
  <tr><td>@c GeoDep_Transp</td>          <td>same transposition</td>                         <td>-</td></tr>
  <tr><td>@c GeoDep_Ori</td>             <td>same orientation</td>                           <td>-</td></tr>
  <tr><td>@c GeoDep_OriRot90Read</td>    <td>orientation rotated by 90 deg around read</td>  <td>-</td></tr>
  <tr><td>@c GeoDep_OriRot90Phase</td>   <td>orientation rotated by 90 deg around phase</td> <td>-</td></tr>
  <tr><td>@c GeoDep_ExtentRead</td>      <td>same extent in read dimension</td>              <td>optional factor (@c 1.0)</td></tr>
  <tr><td>@c GeoDep_ExtentPhase</td>     <td>same extent in phase dimension</td>             <td>optional factor (@c 1.0)</td></tr>
  <tr><td>@c GeoDep_ExtentSlice</td>     <td>same extent in slice dimension</td>             <td>optional factor (@c 1.0)</td></tr>
  <tr><td>@c GeoDep_OffsetRead</td>      <td>fixed offset in read dimension</td>             <td>optional offset (@c 0.0)</td></tr>
  <tr><td>@c GeoDep_OffsetPhase</td>     <td>fixed offset in phase dimension</td>            <td>optional offset (@c 0.0)</td></tr>
  <tr><td>@c GeoDep_OffsetSlice</td>     <td>fixed offset in slice dimension</td>            <td>optional offset (@c 0.0)</td></tr>
  <tr><td>@c GeoDep_OffsetFlexRead</td>  <td>flexible offset in read dimension</td>          <td>offset</td></tr>
  <tr><td>@c GeoDep_OffsetFlexPhase</td> <td>flexible offset in phase dimension</td>         <td>offset</td></tr>
  <tr><td>@c GeoDep_OffsetFlexSlice</td> <td>flexible offset in slice dimension</td>         <td>offset</td></tr>
  <tr><td>@c GeoDep_IncludeRead</td>     <td>included in read dimension</td>                 <td>-</td></tr>
  <tr><td>@c GeoDep_IncludePhase</td>    <td>included in phase dimension</td>                <td>-</td></tr>
  <tr><td>@c GeoDep_IncludeSlice</td>    <td>included in slice dimension</td>                <td>-</td></tr>
  </tr>
  </table>

  For geometry objects that do not use the dimension names @c Read, @c Phase, and @c Slice,
  these labels correspond to first, second and third dimension.

  The following rules apply separately for each slave cuboid:
  - Setting dependencies must occur in the the general order given in the table.
    However, the dimension order does not matter.
  - Setting @c GeoDep_Transp requires that transpositions are actually supported for the slave object.
  - Only one @c GeoDep_Ori is accepted.
  - @c GeoDep_OffsetFlex means that the offset can be manipulated in the geometry editor and is
    transferred to the associated PVM parameter.
  - Only one of the dependencies @c GeoDep_Offset, @c GeoDep_OffsetFlex or @c GeoDep_Include can be set
    per dimension.
  - Usually, it is reasonable that setting @c GeoDep_Include is preceded by setting @c GeoDep_Ori.
  - The PVM parameter given by @a valuePar must be of type @c double and is provided by name.
    It can also be a @c double array where each entry corresponds to a slave cuboid.
    In case it is optional and not provided, a default value is used.
  - In case a dependency of type @c GeoDep_Ori is combined with @c GeoDep_Offset or @c GeoDep_OffsetFlex,
    it will be made sure that no offset changes occur when the orientation is changed.
    This avoids awkward behavour of the free dimensions.

  @PVM_PARS
  - @c PVM_GeoObjDependencies

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @ref GTB_GeoObjDependencyInit must have been called.
  - The PVM parameter given in @a valuePar must exist.

  @PVM_ERROR
  - Incorrect input arguments.
  - Incorrect calling order.
  - Incompatible dependencies.
  - Maximum number of dependencies exceeded.

  @PVM_EXAMPLE
  Setting up dependencies in @c initMeth
  @code
  // initialise
  int id = GTB_GeoObjDependencyInit("PVM_SliceGeoObj", "PVM_VoxelGeoObj");

  // voxel has same orientation as slice
  GTB_GeoObjDependencySet(id, GeoDep_Ori);

  // voxel has same extent as slice in slice dimension
  GTB_GeoObjDependencySet(id, GeoDep_ExtentSlice);

  // voxel position in slice as given by PVM parameter "VoxelOffset"
  VoxelOffset = 10.0;
  GTB_GeoObjDependencySet(id, GeoDep_OffsetSlice, -1, "VoxelOffset");
  @endcode

  @param id          identifier of master-slave couple
  @param dependency  type of dependency as described above
  @param cuboidIndex (optional) index of slave cuboid
                     - -1 = all (default)
  @param valuePar    (optional) name of parameter providing values required for dependency
                     - default = @c NULL

  @see GTB_GeoObjDependencyInit, @ref GTB_GeoObjDependencyNCuboids, @ref GTB_GeoObjDependencyRemove

  @ingroup PVM_GTB
*/
DLL_INTERFACE void GTB_GeoObjDependencySet( int id,
                                            GeoObjDependencyType dependency,
                                            int cuboidIndex = -1,
                                            const char *const valuePar = NULL);




/** Trigger the application of dependencies of all slaves for a given master.

  Call relations of the cuboids of all geometry objects which are slaves of the master.

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

  @PVM_EXAMPLE
  The reaction to changes of a master object in @ref GTB_GeoObjRelationCalls
  @code
  if (GTB_GeoObjDependencyIsMaster(gobjnam))
  {
    // make consistent by calling default relations
    ParxRelsParRelations(handler, Yes);
    // call slaves
    GTB_GeoObjDependencyCallSlaves(gobjnam);
    // full update if not in planning mode and not called as slave
    if (forceDefault == No)
    {
      ParxRelsParRelations(handler, No);
    }
  }
  @endcode

  @param geoObjName  parameter name of master geometry object

  @see @ref GTB_GeoObjDependencyInit, @ref GTB_GeoObjDependencyNCuboids, @ref GTB_GeoObjDependencyRemove,
       @ref GTB_GeoObjDependencySet,
       @ref GTB_GeoObjDependencyIsSlave, @ref GTB_GeoObjDependencyIsMaster, @ref GTB_GeoObjDependencySlave,
       @ref GTB_GeoObjDependencyApply

  @ingroup PVM_GTB
*/
DLL_INTERFACE void GTB_GeoObjDependencyCallSlaves(const char *geoObjName);




/** Determine if a geometry object has been initialised as a slave for dependencies.

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

  @PVM_EXAMPLE
  In @ref STB_UpdateVoxelGeometry, in order to transfer changes on PVM parameters to the cuboid
  @code
  if (UT_GetRequest("PVM_VoxelGeoCub"))
  {
    // changed directly
    GTB_HandleGeoCuboid("PVM_VoxelGeoCub", PVM_VoxelGeoObj.handle);
    GTB_GeoObjDependencyApply("PVM_VoxelGeoObj");
    transferVoxelGeoCub();
    UT_ClearRequest();
  }
  else if (GTB_GeoObjDependencySlave("PVM_VoxelGeoObj") || GTB_GeoObjDependencyIsSlave("PVM_VoxelGeoObj"))
  {
    // changed as slave or PVM parameters changed
    GTB_GeoObjDependencyApply("PVM_VoxelGeoObj");
    transferVoxelGeoCub();
  }
  @endcode

  @param geoObjName  parameter name of geometry object

  @see @ref GTB_GeoObjDependencyInit, @ref GTB_GeoObjDependencyNCuboids, @ref GTB_GeoObjDependencyRemove,
       @ref GTB_GeoObjDependencySet,
       @ref GTB_GeoObjDependencyCallSlaves, @ref GTB_GeoObjDependencyIsMaster, @ref GTB_GeoObjDependencySlave,
       @ref GTB_GeoObjDependencyApply

  @ingroup PVM_GTB
*/
DLL_INTERFACE YesNo GTB_GeoObjDependencyIsSlave(const char *geoObjName);




/** Determine if a geometry object has been initialised as a master for dependencies.

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

  @PVM_EXAMPLE
  In @ref GTB_GeoObjRelationCalls
  @code
  if (GTB_GeoObjDependencyIsMaster("PVM_VoxelGeoObj"))
  {
    // ...

    GTB_GeoObjDependencyCallSlaves("PVM_VoxelGeoObj");

    // ...
  }
  @endcode

  @param geoObjName  parameter name of geometry object

  @see @ref GTB_GeoObjDependencyInit, @ref GTB_GeoObjDependencyNCuboids, @ref GTB_GeoObjDependencyRemove,
       @ref GTB_GeoObjDependencySet,
       @ref GTB_GeoObjDependencyCallSlaves, @ref GTB_GeoObjDependencyIsSlave, @ref GTB_GeoObjDependencySlave,
       @ref GTB_GeoObjDependencyApply

  @ingroup PVM_GTB
*/
DLL_INTERFACE YesNo GTB_GeoObjDependencyIsMaster(const char *geoObjName);




/** Determine if a geometry object is currently handled as a slave in dependencies.

  This function is called in the updater of a geometry object to determine if the
  updater has been called as a reaction to a change of a master, and the current
  object is considered being a slave.

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

  @PVM_EXAMPLE
  In @ref STB_UpdateVoxelGeometry
  @code
  if (UT_GetRequest("PVM_VoxelGeoCub"))
  {
    // changed directly
    GTB_HandleGeoCuboid("PVM_VoxelGeoCub", PVM_VoxelGeoObj.handle);
    GTB_GeoObjDependencyApply("PVM_VoxelGeoObj");
    transferVoxelGeoCub();
    UT_ClearRequest();
  }
  else if (GTB_GeoObjDependencySlave("PVM_VoxelGeoObj") || GTB_GeoObjDependencyIsSlave("PVM_VoxelGeoObj"))
  {
    // changed as slave or PVM parameters changed
    GTB_GeoObjDependencyApply("PVM_VoxelGeoObj");
    transferVoxelGeoCub();
  }
  @endcode

  @param geoObjName  parameter name of geometry object

  @see @ref GTB_GeoObjDependencyInit, @ref GTB_GeoObjDependencyNCuboids, @ref GTB_GeoObjDependencySet,
       @ref GTB_GeoObjDependencyRemove,
       @ref GTB_GeoObjDependencyCallSlaves, @ref GTB_GeoObjDependencyIsSlave, @ref GTB_GeoObjDependencyIsMaster,
       @ref GTB_GeoObjDependencyApply

  @ingroup PVM_GTB
*/
DLL_INTERFACE YesNo GTB_GeoObjDependencySlave(const char *geoObjName);




/** Apply all dependencies for a geometry object.

  All dependencies are applied where the given object is slave.

  Furthermore, the following modifications are performed:
  - Depending on the argument @a nCuboids of @ref GTB_GeoObjDependencyInit the size of the slave
    cuboid pack is set automatically.
  - The minimum and maximum array size of the slave cuboid pack is fixed to the current size
    to avoid user changes of this size in the geometry editor.
  - In case the optional argument @a valuePar of @ref GTB_GeoObjDependencySet is an array, it
    its size is set to the number of slave cuboids.

  @PVM_H1{Implementation of dependencies}
  The following steps must be prepared for a geometry object to enable using it as a master or slave
  for dependencies:
  - In the relation of the cuboid associated with the geometry object, the behaviour for the object being
    initialised as a master must be implemented
    @code
    // In the relation of PVM_VoxelGeoCub

    YesNo forceDefault = GTB_GeoEditorPlanningMode();

    if (GTB_GeoObjDependencySlave("PVM_VoxelGeoObj") == Yes)
    {
      // if called as a slave avoid full update
      forceDefault = Yes;
    }
    else
    {
      UT_SetRequest("PVM_VoxelGeoCub");
    }

    if (GTB_GeoObjDependencyIsMaster("PVM_VoxelGeoObj") == Yes)
    {
      // make consistent by calling default relations
      STB_VoxCallBack();
      // call slaves
      GTB_GeoObjDependencyCallSlaves("PVM_VoxelGeoObj");
      // full update if not in planning mode and not called as slave
      if (forceDefault == No)
      {
        backbone();
      }
    }
    else
    {
      if (forceDefault == Yes)
      {
        STB_VoxCallBack();
      }
      else
      {
        backbone();
      }
    }
    @endcode
    For parameters groups with a group handler, the above part can be replaced by calling @ref GTB_GeoObjRelationCalls.
    @code
    // In the relation of PVM_VoxelGeoCub

    GTB_GeoObjRelationCalls("PVM_VoxelGeoObj", "PVM_VoxCallBack");
    @endcode

  - In the group updater associated with the geometry object, the behaviour depending on the reason of the call
    must be implemented:
    - Check if the updater of this slave has been called as a reaction to a direct change (by using @ref UT_GetRequest),
      to a change of its master (by using @ref GTB_GeoObjDependencySlave), or to a change of a PVM parameter
      (only if the object is a slave which is checked with @ref GTB_GeoObjDependencyIsSlave).
    - Apply the dependencies by calling @ref GTB_GeoObjDependencyApply.
    - Transfer the cuboid settings to the PVM parameters as if the slave had been changed directly.

    @code
    // Beginning of STB_UpdateVoxelGeometry()

    if (UT_GetRequest("PVM_VoxelGeoCub"))
    {
      // changed directly
      GTB_HandleGeoCuboid("PVM_VoxelGeoCub", PVM_VoxelGeoObj.handle);
      GTB_GeoObjDependencyApply("PVM_VoxelGeoObj");
      transferVoxelGeoCub();
      UT_ClearRequest();
    }
    else if (GTB_GeoObjDependencySlave("PVM_VoxelGeoObj") || GTB_GeoObjDependencyIsSlave("PVM_VoxelGeoObj"))
    {
      // changed as slave or PVM parameters changed
      GTB_GeoObjDependencyApply("PVM_VoxelGeoObj");
      transferVoxelGeoCub();
    }
    @endcode

  @PVM_PARS
  - the object given by @a geoObjName

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - cuboid or sub-cuboid numbers do not match
  - number of cuboids exceeds allowed maximum

  @PVM_EXAMPLE
  See above.

  @param slaveName parameter name slave object

  @see @ref GTB_GeoObjDependencyInit, @ref GTB_GeoObjDependencyNCuboids, @ref GTB_GeoObjDependencySet,
       @ref GTB_GeoObjDependencyRemove,
       @ref GTB_GeoObjDependencyCallSlaves, @ref GTB_GeoObjDependencyIsSlave, @ref GTB_GeoObjDependencyIsMaster,
       @ref GTB_GeoObjDependencySlave

  @ingroup PVM_GTB

  @todo Open issues:
  - Ori + Offset: keep positions in free dimensions
    - only transposition (and slave transposition not adapted) > changes positions
  - Removing/initialising of slave cuboids should occur at positions where master cuboids were removed/inserted.
    However, the selection index of the master object is not sufficient as an information.
  - Implement use cases:
    - CSI
    - CASL
    - Shim volume: special case, includes all slice packages (i.e. # masters > # slaves, currently not allowed)
  - Adapt for old geometry: implement default handler of Inplane and Slice which only do apply and transfer
  - Test successive calls A>B>C
*/
DLL_INTERFACE void GTB_GeoObjDependencyApply(const char *slaveName);



/** initializes an element of cuboid package array.

This function initializes one
  @PVM_PARS
  none

  PVM_TBCALL
  - @ref CFG_DefaultFov
  - @ref CFG_DefaultSliceThickness
  - @ref CFG_MaxSlices

    @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code

  (...)

  // initialize first element of slice geometry cuboid package

  GTB_InitCuboidPack(PVM_SliceGeo);

  // initialize all elements of slice geometry cuboid package
  // using all default arguments

  double defaultExtent[3]={ 20.0,10.0,5.0 };
  int    nsubclimits[2]={ 2 , 8 };

  int dim=(int)PARX_get_dim("PVM_SliceGeo",1);
  for(int i=0;i<dim;i++)
  {
    GTB_InitCuboidPack(&PVM_SliceGeo[i],defaultExtent,nsublimits);
  }


  (...)

  @endcode

  @param  cp pointer to an element of a @c GEO_CUBOID_PACK array
  @param[in] defExtent (optional) 3-element double array. If specified
             the extent of the cuboid is initialized by this array.
             The elements are checked to be \f[ \ge \f] 0.001 mm.
             A 0 pointer initializes @c CFG_DefaultFov for extent 0 and 1
             and @c CFG_DefaultSliceThickness for extent 2.
  @param[in] nsubcRange (optional) 2-element int array. If specified
             nSubCuboidsMin is initialized with nsubcRange[0],
             nSubCuboidsMax is initialized with nsubcRange[1]. Argument
             nsubcRange[0] is constrained to be \f[ \ge 10^{-3}\f] mm.
             A 0 pointer initializes 1 for nSubCuboidsMin and @c CFG_MaxSlices
             for nSubCuboidsMax

  @ingroup PVM_GTB

 */
DLL_INTERFACE void GTB_InitCuboidPack(GEO_CUBOID_PACK *cp,
                                      double *defExtent=0,
                                      int *nsubcRange=0);



/** initializes geo cuboid as bounding box of other cuboids.
    This function may be used to initialize parameter of type
    GEO_CUBOID_PACK. The access to parameters is done using the parameter
    names as specified by string arguments dest (mandatory) and source
    (optional).

    If specified as argument (@e source) parameter dest (parameter name
    specified as string) is initialized as bounding box of all geometry
    cuboids defined by parameter source (parameter name specified as string)
    within the cuboid orientation of the first cuboid in array source.

    If no source cuboid array is specified, the dest cuboid array is
    initialized using GTB_InitCuboidPack


This function initializes one
  @PVM_PARS
  - parameter specified by argument dest,
    if specified, parameter with name stored in argument source is
    not changed.

  @PVM_TBCALL
  - @ref GTB_InitCuboidPack

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - arguments dest (mandatory) and source (optional) must be names of array
    parameters with type GEO_CUBOID_PACK
  - qualified parameter names (e.g. "PVM_SliceGeo[0]" are not allowed
  - if specified parameter with name stored in argument source must
    have a value

  @PVM_EXAMPLE
  @code

  // initializes the shim volume as bounding box of all slice packages
     in PVM_SliceGeo


  GTB_InitGeoCubeAsBBox("PVM_MapShimVolumes","PVM_SliceGeo");

  @endcode

  @param  dest
  @param  source
  @return 0 on success, nonzero values indicate an error condition.

  @ingroup PVM_GTB

 */
DLL_INTERFACE int GTB_InitGeoCubeAsBBox(const char *const dest,
                                        const char *const source=0);

#endif
#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
