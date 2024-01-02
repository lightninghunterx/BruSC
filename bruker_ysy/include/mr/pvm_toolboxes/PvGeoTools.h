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

#ifndef PVGEOMRTOOLS_H
#define PVGEOMRTOOLS_H

#ifndef CPROTO

#include "common/pvm_toolboxes/PvGeoTools.h"

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
/* gen/src/prg/pvm_toolboxes/PvGeoTools/packages.c                       */
/*************************************************************************/
/** Redimensions all the array and matrix parameters required for the definition of slice
    packages.

  @PVM_PARS
  <table>
  <tr><th> Parameter                    </th><th> Size                      </th></tr>
  <tr><td> @c PVM_SPackArrGradOrient    </td><td> <c> PVM_NSPacks[3][3] </c></td></tr>
  <tr><td> @c PVM_SPackArrNSlices       </td><td> @c  PVM_NSPacks           </td></tr>
  <tr><td> @c PVM_SPackArrPhase1Offset  </td><td> @c  PVM_NSPacks           </td></tr>
  <tr><td> @c PVM_SPackArrPhase2Offset  </td><td> @c  PVM_NSPacks           </td></tr>
  <tr><td> @c PVM_SPackArrReadOffset    </td><td> @c  PVM_NSPacks           </td></tr>
  <tr><td> @c PVM_SPackArrReadOrient    </td><td> @c  PVM_NSPacks           </td></tr>
  <tr><td> @c PVM_SPackArrSliceDistance </td><td> @c  PVM_NSPacks           </td></tr>
  <tr><td> @c PVM_SPackArrSliceGap      </td><td> @c  PVM_NSPacks           </td></tr>
  <tr><td> @c PVM_SPackArrSliceGapMode  </td><td> @c  PVM_NSPacks           </td></tr>
  <tr><td> @c PVM_SPackArrSliceOffset   </td><td> @c  PVM_NSPacks           </td></tr>
  <tr><td> @c PVM_SPackArrSliceOrient   </td><td> @c  PVM_NSPacks           </td></tr>
  </tr>
  </table>  

  @PVM_TBCALL
  - @ref UT_RedimensionListOf1d

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @c PVM_NSPacks must be initialized.

  @PVM_ERROR
  - @c PVM_NSPacks is not initialized.
  - @c PVM_NSPacks < @c 1

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE void GTB_RedimSPackPars(void);




/** Initializes the parameters associated with slice packages. 

  Also used by @em gradEcho. @n
  In scope initialization means setting parameters their defaults if they
  have not been initialized, or range checking those which are set. 
  Intialization also means setting the dimensions of the arrays and matrices
  that define the slice packages, see @ref GTB_RedimSPackPars for a list of 
  these parameters. 

  @PVM_PARS   
  <table>
  <tr><th> Parameter                               </th><th> Default  </th>
      <th> Range          </th></tr>
  <tr><td> @c PVM_MaxNSlices                       </td><td> @c 128   </td>
      <td> @c 1 to @c 128 </td></tr>
  <tr><td> @c PVM_NSPacks                          </td><td> @c 1     </td>
      <td> @c 1 to @c PVM_MAXNSPacks </td></tr>
  <tr><td> @c PVM_SPackNdx                         </td><td> @c 1     </td>
      <td> @c 1 to @c PVM_NSPacks    </td></tr>
  <tr><td> Elements of @c PVM_SPackArrSliceGapMode </td><td> @c Contiguous </td><td></td></tr>
  <tr><td> @c PVM_SPackArrSliceOrient              </td><td> @c Axial </td>     <td></td></tr>
  <tr><td> @c PVM_SPackArrReadOrient               </td><td> @c L_R   </td>     <td></td></tr>
  </tr>
  </table>  
 
  @PVM_TBCALL
  - @ref GTB_RedimSPackPars
  - @ref GTB_ConstrainSPackArrNSlices
 
  @PVM_RELCALL
  NONE
  
  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref GTB_ConstrainSPackArrNSlices, @ref GTB_RedimSPackPars

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE void GTB_InitSPackPars(void);





/** Constrains the total number of slices.

  @PVM_PARS
  NONE
   
  @PVM_TBCALL
  - @ref GTB_SPackNSlicesWellDefined

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nPacks < @c 1
  - @a sPacksNSlices is a @c NULL pointer
  - @a sPacksNSlices badly defined, see @ref GTB_SPackNSlicesWellDefined
  - @a maxNSlices < @c 1
  - @a nPacks > @a maxNSlices

  @PVM_EXAMPLE
  @code
  GTB_ConstrainSPackArrNSlices(PVM_NSPacks,
                               PVM_MaxNSlices,
                               PVM_SPackArrNSlices);
  @endcode			       
			       
  @param nPacks            Number of slice packages
  @param maxNSlices        Maximum number of slices
  @param sPacksNSlices     Number of slices in each package,
                           @c int array of size @a nPacks

  @ingroup PVM_GTB_INT
*/ 
DLL_INTERFACE void GTB_ConstrainSPackArrNSlices(int  nPacks,
                                                int  maxNSlices,
                                                int *sPacksNSlices);




/** Calculates the slice distances and gaps for the slice packages.

  @PVM_PARS
  NONE
   
  @PVM_TBCALL
  NONE
 
  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a sPackArrSliceGapMode, @a sPackArrSliceDist, and @a sPackArrSliceGap 
    must be dimensioned according to @a nPacks.

  @PVM_ERROR    
  - @a nPacks < @c 1
  - @a sPackArrSliceGapMode is a @c NULL pointer.
  - @a sPackArrSliceDist is a @c NULL pointer.
  - @a sPackArrSliceGap is a @c NULL pointer.
  - @a sliceThickness is less or equal to @c 0.0.

  @PVM_EXAMPLE
  @code
  GTB_SliceDistances(PVM_NSPacks,
                     PVM_SliceThick,
                     PVM_SPackArrSliceGapMode,
                     PVM_SPackArrSliceDistance,
                     PVM_SPackArrSliceGap);
  @endcode			       
		      
  @param nPacks                  Number of slice packs to be considered
  @param sliceThickness          Slice thickness
  @param sPackArrSliceGapMode    Gap modes (@c contiguous or @c non_contiguous)
  @param sPackArrSliceDist       Slice distances
  @param sPackArrSliceGap        Slice gaps

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE void GTB_SliceDistances(int               nPacks,
                                      double            sliceThickness,
                                      PV_SLICE_GAP_MODE *sPackArrSliceGapMode,
                                      double            *sPackArrSliceDist,
                                      double            *sPackArrSliceGap);



/** Calculates the offset of a slice within a slice package. 
  
  The slice offset is calculated according to the formula:
  \f[offset = sliceDistance \times \left(\frac{sliceIndex-nSlices}{2} + 0.5\right)\f]
  Thus the slices in a package are always centered symmetrically around the middle of the
  package.

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
  // get all slice offsets in first package
  
  int nSlices = PVM_SPackArrNSlices[0];
  double distance = PVM_SPackArrSliceDistance[0];
  
  for (int i = 0; i < nSlices; i++)
  {
    offset[i] = GTB_SliceOffsetInPackage(
                  i,
                  distance,
                  nSlices);
  }
  @endcode			       
		      
  @param sliceIndex       Index of the slice
  @param sliceDistance    Distance between slice centers (mm)
  @param nSlices          Number of slices in the package

  @return Offset of a slice within a slice package (mm)

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE double GTB_SliceOffsetInPackage(int    sliceIndex,
                                              double sliceDistance,
                                              int    nSlices);




/** Tests whether the array defining the number of slices in each package is well defined.

  @PVM_PARS
  NONE
   
  @PVM_TBCALL
  - @ref UT_MinOfIntArray
 
  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a sPackNSlices must be dimensioned to @a nPacks, see @ref GTB_RedimSPackPars.

  @PVM_ERROR      
  - @a nPacks < @c 1
  - @a sPackNSlices is @c NULL pointer.
  - Some packages have less than one slice. 
  
  @PVM_EXAMPLE
  @code
  GTB_SPackNSlicesWellDefined(PVM_NSPacks, PVM_SPackArrNSlices);
  @endcode			       

  @param nPacks          Number of slice packages
  @param sPackNSlices    Number of slices in each package, int array of size @a nPacks
  
  @return @c Yes = array is well defined, otherwise an error occurs (i.e. @c No is never returned)

  @see @ref GTB_RedimSPackPars

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE YesNo GTB_SPackNSlicesWellDefined(int nPacks,
                                  int *sPackNSlices);




/** Calculates the total number of slices defined in the slice packs.

  @PVM_PARS
  NONE
   
  @PVM_TBCALL
  - @ref UT_SumOfIntArray
 
  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a sPackNSlices must be dimensioned to @a nPacks.

  @PVM_ERROR      
  - @a nPacks < @c 1
  - @a sPackNSlices is @c NULL pointer.
  - Some packages have less than one slice.
  
  @PVM_EXAMPLE
  @code
  int nSlices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);
  @endcode			       

  @param nPacks          Number of slice packs to be considered
  @param sPackNSlices    int array containing the slice count per pack

  @return Total number of slices, @c 0 in case of an error

  @ingroup PVM_GTB
*/
DLL_INTERFACE int GTB_NumberOfSlices(int  nPacks,
                                     int *sPackNSlices);




/** Calculates the local offset of each defined slice according to the receipe described for
    @ref GTB_SliceOffsetInPackage.

  @PVM_PARS
  NONE
   
  @PVM_TBCALL
  - @ref GTB_SliceOffsetInPackage
  
  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a sPackNSlices and @a sPackSliceDistances must be sized according to @a nPacks.
  - @a sliceOffsets must be sized according to the total number of slices, 
    see @ref GTB_NumberOfSlices.

  @PVM_ERROR      
  - @a nPacks < @c 1
  - @a sPackNSlices is @c NULL pointer.
  - Some packages have less than one slice. 
  - @a sPackSliceDistances is a @c NULL pointer.
  - @a sliceOffsets is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  int totalSlices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);
  double *sliceArrOffs = (double *) calloc(totalSlices, sizeof(double));

  GTB_SliceOffsetsInPackages(PVM_NSPacks,
                             PVM_SPackArrNSlices,
                             PVM_SPackArrSliceDistance,
                             sliceArrOffs);
  @endcode			
         
  @param nPacks                 Number of defined slice packs
  @param sPackNSlices           Array containing the number of slices in each pack
  @param sPackSliceDistances    Array containing the slice distances for each pack (mm)
  @param[out] sliceOffsets      Array to receive the local offset of each slice (mm)

  @see @ref GTB_SliceOffsetInPackage

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE void GTB_SliceOffsetsInPackages(int     nPacks,
                                              int    *sPackNSlices,
                                              double *sPackSliceDistances,
                                              double *sliceOffsets);



/*************************************************************************/
/* gen/src/prg/pvm_toolboxes/PvGeoTools/slice.c                          */
/*************************************************************************/

/** Redimensions all array parameters that refer to spatial and frequency 
    offsets.

  @PVM_PARS
  <table>
  <tr><th> Parameter              </th><th> Size       </th></tr>
  <tr><td> @c PVM_SliceOffsetHz   </td><td> @a nSlices </td></tr>
  <tr><td> @c PVM_SliceOffset     </td><td> @a nSlices </td></tr>
  <tr><td> @c PVM_ReadOffsetHz    </td><td> @a nSlices </td></tr>
  <tr><td> @c PVM_ReadOffset      </td><td> @a nSlices </td></tr>
  <tr><td> @c PVM_Phase1Offset    </td><td> @a nSlices </td></tr>
  <tr><td> @c PVM_EffPhase1Offset </td><td> @a nSlices </td></tr>
  <tr><td> @c PVM_Phase2Offset    </td><td> @a nSlices </td></tr>
  <tr><td> @c PVM_EffPhase2Offset </td><td> @a nSlices </td></tr>
  </tr>
  </table>   

  @PVM_TBCALL
  - @ref UT_RedimensionListOf1d
 
  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR      
  - @a nSlices < @c 1

  @PVM_EXAMPLE
  @code
  int totalSlices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);
  GTB_SliceOffsetsInPackages(totalSlices);
  @endcode			
         
  @param nSlices    Total number of slices

  @ingroup PVM_GTB_INT    
 */
DLL_INTERFACE void GTB_RedimOffsets(int nSlices);







/** Constrains spatial imaging matrix size and anti alias factor. 

  Dependent on the spatial dimensionality as defined by argument @a dim and the desired
  isotropic state defined by argument @a iState, the spatial matrix size and the anti alias
  factor are controlled to prevent a dataset size exceeding the reconstruction capabilities.
  In case the dataset is too large the anti aliasing
  factors are reduced (from highest to lowest dimension) and if the dataset is still to large,
  the spatial matrix (from highest to lowest dimension) is reduced.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_MaxIsotropicSpatialMatrixSize

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Legal range of function arguments. Array size of arguments @a matrix and @a antiAlias are
    expected to be identical to argument @a dim.

  @PVM_ERROR
  - Illegal value of spatial dimension argument @a dim
  - Illegal value of imaging matrix size
  - Illegal value of anti aliasing factors
  - Illegal value of isotropic state (only @c Isotropic_Matrix, @c Isotropic_All, 
    @c Isotropic_Fov, @c Isotropic_Resolution, @c Isotropic_None allowed)
  
  @PVM_EXAMPLE
  @code
  GTB_ConstrainAntiAliasAndMatrix(PTB_GetSpatDim(),
                                  PVM_Matrix,
                                  PVM_MaxAntiAlias,
                                  Isotropic_None);
  @endcode			
         
  @param dim          Spatial dimensionality and array size of argument @a matrix and 
                      @a antiAlias
  @param matrix       Spatial imaging matrix, array of size @a dim
  @param antiAlias    Anti aliasing factor, array of size @a dim
  @param iState       Isotropic state (defines whether isotropic matrix size is required)

  @return 
  - @c -1 in case of an error
  - @c 0 if no constrain is necessary
  - @c 1 if @a antiAlias and/or @a matrix have been changed

  @see @ref CFG_MinImagingSpatialMatrixSize

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE int GTB_ConstrainAntiAliasAndMatrix(const int      dim,
                                                  int           *matrix, 
                                                  double        *antiAlias, 
                                                  PV_ISOTROPIC   iState);



/*************************************************************************/
/* gen/src/prg/pvm_toolboxes/PvGeoTools/ModuleGeoTools.c                 */
/*************************************************************************/


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

/** Converts index of max slice vector component into major slice orientation.
 
  The index is expected to be the component of the slice vector with the maximum absolute
  value. The major slice orientation is returned as a state of enum type @c SLOR_TYPE
  (@c sagittal, @c coronal or @c axial).

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
  int sIdx = UT_GetNdxOfMaxArr(sliceVector, 3);
  SLOR_TYPE slOrient = GTB_GetSliceOrientFromSliceNdx(sIdx);
  @endcode			
  
  @param sliceNdx    Index of the max component of a slice vector (@c 0-2)

  @return Major slice orientation represented as state of enum type @c SLOR_TYPE
          (for any illegal value of @a sliceNdx, @c axial is returned)

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE SLOR_TYPE GTB_GetSliceOrientFromSliceNdx(int sliceNdx);

/** Sets the flow saturation slice orientation from the imaging slice orientation.

  @PVM_PARS
  - <c> PVM_InFlowSatSliceOrient[][] </c>

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The following parameters are used and must have values:
    - @c PVM_InFlowSatNSlices
    - @c PVM_SliceOrient

  @PVM_ERROR
  NONE    

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE void GTB_InFlowSatSetOrientFromSlice(void);

/** Gets the flow saturation slice side name from the saturation slice orientation and the
    saturation slice side.

  @PVM_PARS
  - <c> PVM_FlowSaturationSide[] </c>

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The following parameters are used and must have values:
    - @c PVM_InFlowSatSide
    - @c PVM_FovSatNSlices
    - @c PVM_InFlowSatSliceOrient

  @PVM_ERROR
  NONE    

  @ingroup PVM_GTB_INT   
*/
DLL_INTERFACE void GTB_InFlowSatSetSliceSideName(void);

/** Calculates the absolute offset in mm and Hz from the relative gap for the flow saturation
    slices. 

  @PVM_PARS
  - <c> PVM_InFlowSatSliceOffset[][] </c>
  - <c> PVM_InFlowSatOffsetHz[][] </c>

  @PVM_TBCALL
  - @ref GTB_NumberOfSlices

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The following parameters are used and must have values:
    - @c PVM_NSPacks
    - @c PVM_SPackArrNSlices
    - @c PVM_ObjOrderList
    - @c PVM_EffSliceOffset
    - @c PVM_InFlowSatSide
    - @c PVM_InFlowSatThick
    - @c PVM_SliceThick
    - @c PVM_InFlowSatGap
    - @c PVM_FlowSatPulse.Bandwidth

  @PVM_ERROR
  NONE    

  @ingroup PVM_GTB_INT     
*/
DLL_INTERFACE void GTB_InFlowSatGetSliceSideOffset(void);




/*************************************************************************/
/* gen/src/prg/pvm_toolboxes/PvGeoTools/SliceGeoTools.c                  */
/*************************************************************************/

/** Determines major read and slice orientation from gradient matrix array. 

  Checks the array of gradient orientation matrices and derives the major read and slice
  orientations form it. For further details see @ref GTB_GradOrientToSliceReadOrient.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref GTB_GradOrientToSliceReadOrient

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Error conditions of @ref GTB_GradOrientToSliceReadOrient  

  @PVM_EXAMPLE
  @code
  GTB_GradOrientArrToSliceReadOrientArr(
    PtrType3x3 PVM_SPackArrGradOrient[0],
    PVM_SPackArrSliceOrient,
    PVM_SPackArrReadOrient,
    PVM_NSPacks);
  @endcode			
					
  @param oriMatrix    Array of size @a nPackages x 3 x 3 orientation matrices
  @param sor          Array of size @a nPackages containing slice orientations
  @param reor         Array of size @a nPackages containing read orientations
  @param nPackages    Number of packages

  @return @c Yes in case of success

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE YesNo GTB_GradOrientArrToSliceReadOrientArr(double       (*oriMatrix)[3][3],
                                                          SLOR_TYPE     *sor,
                                                          PV_REOR_TYPE  *reor,
                                                          const int      nPackages);

/** Determines major read and slice orientation from gradient matrix. 

  The max components of read and slice orientation are determined (using
  @ref GTB_GradOrientToReadPhaseSliceIndex) and the major slice and read orientation are
  derived.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref GTB_GradOrientToReadPhaseSliceIndex

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Argument @a oriMatrix points to a well formed orientation matrix.

  @PVM_ERROR
  - Failure of @ref GTB_GradOrientToReadPhaseSliceIndex

  @PVM_EXAMPLE
  @code
  double (*oriMatrix)[3][3] = {{0.0, 1.0, 0.0},{0.0, 0.0, -1.0},{-1.0, 0.0, 0.0}};
  PV_REOR_TYPE reor;
  SLOR_TYPE    slor;
  
  GTB_GradOrientToSliceReadOrient(oriMatrix, &slor, &reor);
  @endcode			
					
  @param[in] oriMatrix  3x3 double matrix containing an orthonormal gradient orientation matrix
  @param[out] sor       Pointer to variable of type @c SLOR_TYPE (set to @c axial, @c sagittal or
                        @c coronal respectively)
  @param[out] reor      Pointer to variable of type @c PV_REOR_TYPE (set to @c L_R, @c A_P or 
                        @c H_F respectively)

  @return @c Yes in case of success, else @c No
  
  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE YesNo GTB_GradOrientToSliceReadOrient(double      (*oriMatrix)[3][3],
                                                    SLOR_TYPE    *sor,
                                                    PV_REOR_TYPE *reor);

/** Determines indices of max components for read phase and slice vectors. 

  The array of orientation matrices specified by argument is checked and the abs 
  max of the component for the read, phase, and slice vector is determined and stored into
  argument @a rindex, @a pindex or @a sindex. This function is used to determine the major read
  and slice orientations.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_OriMatrixAbsMaxOfColumn

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Well formed orientation matrix (must be orthonormal)

  @PVM_ERROR
  - Illegal formed orientation matrix as detected by @ref UT_OriMatrixAbsMaxOfColumn

  @PVM_EXAMPLE
  @code
  double (*oriMatrix)[3][3] = {{0.0, 1.0, 0.0},{0.0, 0.0, -1.0},{-1.0, 0.0, 0.0}};
  int ri=0, pi=1, si=2;
  
  GTB_GradOrientToReadPhaseSliceIndex(oriMatrix, &ri, &pi, &si);
  @endcode			
					
  @param oriMatrix 3x3 double matrix containing an orthonormal gradien orientation matrix
  @param rindex    Pointer to int variable containing max read index
  @param pindex    Pointer to int variable containing max phase index
  @param sindex    Pointer to int variable containing max slice index

  @return @c Yes in case of success
  
  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE YesNo GTB_GradOrientToReadPhaseSliceIndex(double   (*oriMatrix)[3][3],
                                                        int *const rindex,
                                                        int *const pindex,
                                                        int *const sindex);




/** Sets up a standard gradient orientation matrix. 

  Dependent on a desired major slice and read orientation as specified by arguments @a slorRequest and
  @a reorRequest the corresponding orientation matrix is set up. In case of illegal combinations of
  slice and read orientations, a default read orientation for the given slice orientation is set up.

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
  GTB_SetStandardOrientation(PtrType3x3 PVM_SPackArrGradOrient[0],
                             PVM_SPackArrSliceOrient[0],
                             PVM_SPackArrReadOrient[0]))
  @endcode			
  
  @param oriMatrix      3x3 double matrix containing an orthonormal gradient orientation matrix
  @param slorRequest    Slice orientation variable
  @param reorRequest    Pointer to a read orientation variable changed by this function in case
                        it is not compatible with the requested slice orientation @a slorRequest

  @return @c Yes in case of success, @c No in case of illegal @a slorRequest

  @ingroup PVM_GTB
*/
DLL_INTERFACE YesNo GTB_SetStandardOrientation(double      (*oriMatrix)[3][3],
                                               SLOR_TYPE     slorRequest,
                                               PV_REOR_TYPE *reorRequest);




/** Sets the desired read orientation. 

  The actual orientation matrix as specified by argument is analysed to derive the
  actual major slice orientation. The desired read orientation is checked to be compatible with
  this slice orientation (if not, nothing is changed and @c No is returned). If the actual read
  orientation does not match the desired read orientation, the phase and read vectors of the
  gradient matrix are swapped.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Argument @a oriMatrix points to a well formed orientation matrix.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  GTB_ReadOrientToGradOrient(PtrType3x3 PVM_SPackArrGradOrient[0],
                             &PVM_SPackArrReadOrient[0]);
  @endcode			
  
  @param oriMatrix      3x3 double matrix containing an orthonormal gradient orientation matrix
  @param reorRequest    Required major read orientation

  @return @c Yes in case of success

  @ingroup PVM_GTB
*/
DLL_INTERFACE YesNo GTB_ReadOrientToGradOrient(double       (*oriMatrix)[3][3],
                                               PV_REOR_TYPE *reorRequest);





/*************************************************************************/
/* gen/src/prg/pvm_toolboxes/PvGeoTools/utilities.c                      */
/*************************************************************************/


/** Calculates the offsets from magnet isocenter, as well as the offsets due to gradient coil
    asymmetry, from the slice package definitions.

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
  int totalSlices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);
  double *sliceArrOffs = (double *) calloc(totalSlices, sizeof(double));
  
  GTB_Offsets(PVM_NSPacks,
              PVM_SPackArrNSlices,
              PtrType3x3 PVM_SPackArrGradOrient[0],
              PVM_SPackArrSliceOffset,
              sliceArrOffs,
              PVM_ZGradientOffset,  
              Logical_Slice_Grad,
              PVM_SliceOffset,
              PVM_EffSliceOffset);
  @endcode			
		    
  @param nPacks           Number of slice packages to be considered
  @param sPacksNSlices    Array containing the number of slices in each package
  @param gradMatrix       Gradient matrix of dimension @a nPacks*3*3 containing the gradient
                          matrices of each slice package
  @param packOffsets      Array of size @a nPacks containing the offsets of each packages (mm) 
  @param localOffsets     Array of local offsets to be added to @a packOffsets. The size of
                          this array is the total number of slices. This @c NULL pointer is
                          parsed the parameter is ignored!
  @param gradCoilOffset   Displacement of the z-gradient null from the magnet isocenter (mm)
  @param logicalGrad      Logical gradient axis to be considered. This enum takes the values:
			  - @c Logical_Read_Grad 
			  - @c Logical_Phase_Grad
			  - @c Logical_Slice_Grad
  @param offsets          This array is dimensioned to the total number of slices and receives
                          the resultant offsets (mm). These offsets are measured from the
                          magnet isocenter.
  @param totalOffsets     This array is dimensioned to the total number of slices and receives
                          the resultant total offsets (mm). These offsets  account for the
                          displacment of the z-gradient.

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE void GTB_Offsets(int                    nPacks,
                               int                   *sPacksNSlices,
                               double               (*gradMatrix)[3][3],
                               double                *packOffsets,
                               double                *localOffsets,
                               double                 gradCoilOffset,
                               PVM_LOGICAL_GRAD_TYPE  logicalGrad,  
                               double                *offsets,
                               double                *totalOffsets);




/** Calculates the offsets in the given phase encoding gradient from magnet isocenter as well
    as the effective offsets due to gradient coil asymmetry. 

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref GTB_Offsets

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  GTB_Offsets(PVM_NSPacks,
              PVM_SPackArrNSlices,
              PtrType3x3 PVM_SPackArrGradOrient[0],
              PVM_SPackArrSliceOffset,
              PVM_ZGradientOffset,  
              Logical_Slice_Grad,
              PVM_SliceOffset,
              PVM_EffSliceOffset);
  @endcode			
		    
  @param nPacks           Number of slice packages to be considered
  @param sPacksNSlices    Array containing the number of slices in each package
  @param gradMatrix       Gradient matrix of dimension @a nPacks*3*3 containing the gradient
                          matrices of each slice package
  @param packOffsets      Array of size @a nPacks containing the offsets of each packages (mm) 
  @param gradCoilOffset   Displacement of the z-gradient null from the magnet isocenter (mm)
  @param logicalGrad      Logical gradient axis to be considered. This enum takes the values:
			  - @c Logical_Read_Grad 
			  - @c Logical_Phase_Grad
			  - @c Logical_Slice_Grad
  @param offsets          This array is dimensioned to the total number of slices and receives
                          the resultant offsets (mm). These offsets are measured from the
                          magnet isocenter.
  @param totalOffsets     This array is dimensioned to the total number of slices and receives
                          the resultant total offsets (mm). These offsets  account for the
                          displacment of the z-gradient.

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE void GTB_OffsetsInPhase(int                    nPacks,
                                      int                   *sPacksNSlices,
                                      double               (*gradMatrix)[3][3],
                                      double                *packOffsets,
                                      double                 gradCoilOffset,
                                      PVM_LOGICAL_GRAD_TYPE  logicalGrad,  
                                      double                *offsets,
                                      double                *totalOffsets);




/** Calculates the offsets in the given read gradient from magnet isocenter as well as the
    frequency offsets taking gradient coil asymmetry into account.

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
  GTB_OffsetsInRead(PVM_NSPacks,
                    PVM_SPackArrNSlices,
                    PtrType3x3 PVM_SPackArrGradOrient[0],
                    PVM_SPackArrReadOffset,
                    PVM_ReadGradient,
                    PVM_GradCalConst,
                    PVM_ZGradientOffset,
                    Logical_Read_Grad,
                    PVM_ReadOffset,
                    PVM_ReadOffsetHz);
  @endcode			
			 
  @param nPacks           Number of slice packages to be considered
  @param sPacksNSlices    Array containing the number of slices in each package
  @param gradMatrix       Gradient matrix of dimension @a nPacks*3*3 containing the gradient
                          matrices of each slice package
  @param packOffsets      Array of size @a nPacks containing the offsets of each packages (mm) 
  @param grad             Read gradient strength [%]
  @param gradCalConst     Gradient calibration constant
  @param gradCoilOffset   Displacement of the z-gradient null from the magnet isocenter (mm)
  @param logicalGrad      Logical gradient axis to be considered. This enum takes the values:
			  - @c Logical_Read_Grad 
			  - @c Logical_Phase_Grad
			  - @c Logical_Slice_Grad
  @param offsets          This array is dimensioned to the total number of slices and receives
                          the resultant offsets (mm). These offsets are measured from the
                          magnet isocenter.
  @param totalOffsetsHz   This array is dimensioned to the total number of slices and receives
                          the resultant frequency offset.

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE void GTB_OffsetsInRead(int                    nPacks,
                                     int                   *sPacksNSlices,
                                     double               (*gradMatrix)[3][3],
                                     double                *packOffsets,
                                     double                 grad,
                                     double                 gradCalConst,
                                     double                 gradCoilOffset,
                                     PVM_LOGICAL_GRAD_TYPE  logicalGrad,  
                                     double                *offsets,
                                     double                *totalOffsetsHz);




/** Calculats the offsets in the given slice gradient from magnet isocenter as well as the
    frequency offsets taking gradient coil asymmetry into account.

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
  GTB_OffsetsInSlice(PVM_NSPacks,
                     PVM_SPackArrNSlices,
                     PtrType3x3 PVM_SPackArrGradOrient[0],
                     PVM_SPackArrSliceOffset,
                     PVM_SPackArrSliceDistance,
                     PVM_ExSliceGradient,
                     PVM_GradCalConst,
                     PVM_ZGradientOffset,
                     Logical_Slice_Grad,
                     PVM_SliceOffset,
                     PVM_SliceOffsetHz);
  @endcode			
  
  @param nPacks           Number of slice packages to be considered
  @param sPacksNSlices    Array containing the number of slices in each package
  @param gradMatrix       Gradient matrix of dimension @a nPacks*3*3 containing the gradient
                          matrices of each slice package
  @param packOffsets      Array of size @a nPacks containing the offsets of each packages (mm) 
  @param sPackSliceDist   Array of size @a nPacks containing the slice distances of each slice
                          package (mm)
  @param grad             Slice gradient strength [%]
  @param gradCalConst     Gradient calibration constant
  @param gradCoilOffset   Displacement of the z-gradient null from the magnet isocenter (mm)
  @param logicalGrad      Logical gradient axis to be considered. This enum takes the values:
			  - @c Logical_Read_Grad 
			  - @c Logical_Phase_Grad
			  - @c Logical_Slice_Grad
  @param offsets          This array is dimensioned to the total number of slices and receives
                          the resultant offsets (mm). These offsets are measured from the
                          magnet isocenter.
  @param totalOffsetsHz   This array is dimensioned to the total number of slices and receives
                          the resultant frequency offset.

  @ingroup PVM_GTB_INT
*/
DLL_INTERFACE void GTB_OffsetsInSlice(int                    nPacks,
                                      int                   *sPacksNSlices,
                                      double               (*gradMatrix)[3][3],
                                      double                *packOffsets,
                                      double                *sPackSliceDist,
                                      double                 grad,
                                      double                 gradCalConst,
                                      double                 gradCoilOffset,
                                      PVM_LOGICAL_GRAD_TYPE  logicalGrad,  
                                      double                *offsets,
                                      double                *totalOffsetsHz); 




/** Creates a slice ordered frequency list.

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
  - @a freqOffsetList is @c NULL pointer.
  - @a orderedOffsetList is @c NULL pointer.  

  @PVM_EXAMPLE
  @code
  int nslices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);
  
  GTB_OrderedFreqOffsetList(nslices, PVM_ObjOrderList, PVM_SliceOffsetHz, ACQ_O1_list);
  @endcode			
  
  @param nSlices              Total number of slices
  @param sliceOrder           Integer array of size @a nSlices containing the excitation order
                              of the slices
  @param freqOffsetList       Double array of size @a nSlices containing frequency offsets 
  @param orderedOffsetList    Double array of size @a nSlices containing reordered frequency
                              offsets after call of this routine

  @ingroup PVM_GTB
*/
DLL_INTERFACE void GTB_OrderedFreqOffsetList(int     nSlices,
                                             int    *sliceOrder,
                                             double *freqOffsetList,
                                             double *orderedOffsetList);             




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
  @param[out] sliceOrder  Array of size @a nSlices defining the excitation order after call of
                          this routine

  @ingroup PVM_GTB_INT
 */
DLL_INTERFACE void GTB_SliceOrder(int                   nSlices,
                                  PV_SLICE_SCHEME_TYPE  sliceScheme,
                                  int                  *sliceOrder);






#endif
#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
