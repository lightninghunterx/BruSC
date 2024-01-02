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

#ifndef PVUTILTOOLS_H
#define PVUTILTOOLS_H

#ifndef CPROTO
#include <vector>
#include "PvUtil/PvExcept.hh"
#include "PvUtil/PvUtil.h"
#include "Parx/Rels/Context.hh"
#include "libCore/Misc/IO/File.hh"

#include "common/PvTypes/commonTypes.h"

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

#if defined(HAS_WINDOWS_FEATURES)
#  ifdef PvUtilTools_INTERNAL
#    define DLL_INTERFACE __declspec(dllexport)
#  else
#    define DLL_INTERFACE __declspec(dllimport)
#  endif
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#  define DLL_INTERFACE
#endif



/* macro definitions */

/** Opens a binary file for portion-wise writing of double data.

  The file is only opened for a valid name.

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Opening file failed.

  @PVM_EXAMPLE
  @code
  UT_FILE_START(fp, "/tmp/testData")
  @endcode

  @param FILE_PT      Name of file pointer
  @param FILE_NAME    Name of file (including path)

  @see @ref UT_FILE_OUT, @ref UT_FILE_END

  @ingroup PVM_UT_INT
*/
#define UT_FILE_START(FILE_PT, FILE_NAME)                          \
FILE *FILE_PT = NULL;                                              \
if (FILE_NAME != NULL && strlen(FILE_NAME) > 0)                    \
{                                                                  \
  FILE_PT = fopen(FILE_NAME, "w");                                 \
  if (FILE_PT == NULL) UT_ReportError("Opening file failed.");     \
}

/** Writes a portion of double data to a binary file.

  @PVM_PRECOND
  - File was opened with @ref UT_FILE_START.

  @PVM_ERROR
  - Writing to file failed.

  @PVM_EXAMPLE
  @code
  UT_FILE_OUT(fp, data, 2)
  @endcode

  @param FILE_PT     Name of file pointer
  @param FILE_PAR    Name of parameter array
  @param FILE_N      Number of elements

  @see @ref UT_FILE_START, @ref UT_FILE_END

  @ingroup PVM_UT_INT
*/
#define UT_FILE_OUT(FILE_PT, FILE_PAR, FILE_N)                            \
if (FILE_PT != NULL)                                                      \
{                                                                         \
  if (fwrite(FILE_PAR, sizeof(double), FILE_N, FILE_PT) < (size_t)FILE_N) \
    UT_ReportError("Writing to file failed.");                            \
}

/** Closes a binary file used for portion-wise writing of double data.

  @PVM_PRECOND
  - File was opened with @ref UT_FILE_START.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  UT_FILE_END(fp)
  @endcode

  @param FILE_PT    Name of file pointer

  @see @ref UT_FILE_START, @ref UT_FILE_OUT

  @ingroup PVM_UT_INT
*/
#define UT_FILE_END(FILE_PT)                                       \
if (FILE_PT != NULL)                                               \
{                                                                  \
  fclose(FILE_PT);                                                 \
}

/** Opens an ASCII file for portion-wise writing of floating-point data.

  The file is only opened for a valid name.

  It will be created in @c PVHOME/prog/curdir/USER/ParaVision/\<process\>.

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Opening file failed.

  @PVM_EXAMPLE
  @code
  UT_DEBUG_START(Yes, fp, "/tmp/debugData")
  @endcode

  @param DEBUG_FLAG    @c Yes = do create debug file
  @param DEBUG_PT      Name of file pointer
  @param DEBUG_NAME    Name of file (without path)

  @see @ref UT_DEBUG_OUT, @ref UT_DEBUG_END

  @ingroup PVM_UT_INT
*/
#define UT_DEBUG_START(DEBUG_FLAG, DEBUG_PT, DEBUG_NAME)               \
FILE *DEBUG_PT = NULL;                                                 \
if (DEBUG_FLAG == Yes && DEBUG_NAME != NULL && strlen(DEBUG_NAME) > 0) \
{                                                                      \
  char debugName[PATH_MAX];                                            \
  PvPathMkCurdirPv(debugName, PATH_MAX, DEBUG_NAME);                    \
  DEBUG_PT = fopen(debugName, "w");                                    \
  if (DEBUG_PT == NULL) UT_ReportError("Opening file failed.");        \
}

/** Writes a portion of floating-point data to an ASCII file.

  One line per portion, with the elements separated by a space.

  @PVM_PRECOND
  - File was opened with @ref UT_DEBUG_START.

  @PVM_ERROR
  - Writing to file failed.

  @PVM_EXAMPLE
  @code
  UT_DEBUG_OUT(fp, data, 2)
  @endcode

  @param DEBUG_PT     Name of file pointer
  @param DEBUG_PAR    Name of parameter array
  @param DEBUG_N      Number of elements

  @see @ref UT_DEBUG_START, @ref UT_DEBUG_END

  @ingroup PVM_UT_INT
*/
#define UT_DEBUG_OUT(DEBUG_PT, DEBUG_PAR, DEBUG_N)                    \
if (DEBUG_PT != NULL)                                                 \
{                                                                     \
  for (int i__ = 0; i__ < DEBUG_N; i__++)                                   \
  {                                                                   \
    if (fprintf(DEBUG_PT, "%f ", DEBUG_PAR[i__]) == 0)                  \
      UT_ReportError("Writing to file failed.");                      \
  }                                                                   \
  fprintf(DEBUG_PT, "\n");                                            \
}

/** Closes a binary file used for portion-wise writing of double data.

  @PVM_PRECOND
  - File was opened with @ref UT_DEBUG_START.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  UT_DEBUG_END(fp)
  @endcode

  @param DEBUG_PT    Name of file pointer

  @see @ref UT_DEBUG_START, @ref UT_DEBUG_OUT

  @ingroup PVM_UT_INT
*/
#define UT_DEBUG_END(DEBUG_PT)                                        \
  if (DEBUG_PT != NULL)                                               \
  {                                                                   \
    fclose(DEBUG_PT);                                                 \
  }


/* /pv60/gen/src/prg/pvm_toolboxes/PvUtilTools/doubleArray.c */

/** Initializes a double array so that all indices receive the same given value.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr must be dimensioned to @a numElements.

  @PVM_ERROR
  The error conditions cause the parameters @c PVM_ErrorDetected and @c PVM_ErrorMessage
  to be set.
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  const int dim = PTB_GetSpatDim();
  PARX_change_dims("PVM_Fov", dim);
  UT_InitDoubleArray(dim, PVM_Fov, CFG_DefaultFov());
  @endcode

  @param      numElements    Number of elements to be set
  @param[out] srcArr         Double array
  @param      initValue      Value to which the elements should be set

  @see @ref UT_EuclideanNormOfDoubleArray,
       @ref UT_ScaleDoubleArray, @ref UT_DoubleArraySetInRange,
       @ref UT_InvertDoubleArray,
       @ref UT_NormalizeDoubleArray,
       @ref UT_InitIntArray

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_InitDoubleArray(int     numElements,
                                      double *srcArr,
                                      double  initValue);




/** Determines the maximum value of a double array.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  int index;
  double maxFov = UT_MaxOfDoubleArray(PTB_GetSpatDim(), PVM_Fov, &index);
  @endcode

  @param      numElements  Number of elements to be searched
  @param[in]  srcArr       Double array
  @param[out] maxIdx       (optional) Index of the element containing
                           the maximum. Will not be set if a @c NULL pointer
                           is parsed.

  @return Maximum value of the first @a numElements in the array

  @see @ref UT_MaxOfDoubleArray, @ref UT_MinOfDoubleArray,
       @ref UT_AbsMaxOfDoubleArray,
       @ref UT_DoubleArrayMax, @ref UT_DoubleArrayMin,
       @ref UT_DoubleArraySetInRange,
       @ref UT_MaxOfIntArray

  @ingroup PVM_UT
*/
DLL_INTERFACE double UT_MaxOfDoubleArray(int          numElements,
                                         const double *srcArr,
                                         int          *maxIdx = NULL);




/** Determines the minimum value of a double array.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  int index;
  double minFov = UT_MinOfDoubleArray(PTB_GetSpatDim(), PVM_Fov, &index);
  @endcode

  @param      numElements Number of elements to be searched
  @param[in]  srcArr      Double array
  @param[out] minIdx      (optional) Index of the element containing the minimum.
                          Will not be set if a @c NULL pointer is parsed.

  @return Minimum value of the first @a numElements in the array

  @see @ref UT_MaxOfDoubleArray, @ref UT_AbsMaxOfDoubleArray,
       @ref UT_DoubleArrayMax, @ref UT_DoubleArrayMin,
       @ref UT_DoubleArraySetInRange,
       @ref UT_MinOfIntArray

  @ingroup PVM_UT
*/
DLL_INTERFACE double UT_MinOfDoubleArray(int          numElements,
                                         const double *srcArr,
                                         int          *minIdx = NULL);




/** Determines the absolute maximum value of a double array.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  double maxGrad = UT_AbsMaxOfDoubleArray(PVM_SpiralNbOfGradientPoints, PVM_SpiralShape1);
  @endcode

  @param      numElements  Number of elements to be searched
  @param[in]  srcArr       Double array
  @param[out] maxIdx       (optional) Index of the element containing the maximum.
                           Will not be set if a @c NULL pointer is parsed.

  @return Absolute maximum value of the first @a numElements in the array

  @see @ref UT_MaxOfDoubleArray, @ref UT_MinOfDoubleArray,
       @ref UT_DoubleArrayMax, @ref UT_DoubleArrayMin,
       @ref UT_DoubleArraySetInRange,
       @ref UT_AbsMaxOfIntArray

  @ingroup PVM_UT
*/
DLL_INTERFACE double UT_AbsMaxOfDoubleArray(int    numElements,
                                            double *srcArr,
                                            int    *maxIdx = NULL);




/** Determines the euclidean norm of a double array.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  int dim = PARX_get_dim("PVM_FovSatGrad", 1);
  for(int i = 0; i < dim; i++)
  {
    double norm = UT_EuclideanNormOfDoubleArray(3, PVM_FovSatGrad[i]);
    UT_ScaleDoubleArray(3, PVM_FovSatGrad[i], 1.0 / norm, PVM_FovSatGrad[i]);
  }
  @endcode

  @param     numElements  Number of elements to be summed
  @param[in] srcArr       Double array

  @return Maximum value of the first @a numElements in the array

  @see @ref UT_InitDoubleArray,
       @ref UT_DoubleArrayMax, @ref UT_ScaleDoubleArray,
       @ref UT_InvertDoubleArray,
       @ref UT_NormalizeDoubleArray, @ref UT_ScalarProductDoubleArray

  @ingroup PVM_UT
*/
DLL_INTERFACE double UT_EuclideanNormOfDoubleArray(int    numElements,
                                                   double *srcArr);




/** Calculates an array that is the maximum of two arrays.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr, @a compArr, @a destArr must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.
  - @a compArr is a @c NULL pointer.
  - @a destArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  #define N 10
  double val1[N];
  double val2[N];
  double valMax[N];

  // fill arrays ...

  UT_DoubleArrayMax(N, val1, val2, valMax);
  @endcode

  @param      numElements  Number of array elements
  @param[in]  srcArr       First array to be compared
  @param[in]  compArr      Second array to be compared
  @param[out] destArr      Destination array containing elements that
                           are max of the two arrays

  @see @ref UT_MaxOfDoubleArray, @ref UT_MinOfDoubleArray,
       @ref UT_AbsMaxOfDoubleArray,
       @ref UT_DoubleArrayMin, @ref UT_DoubleArraySetInRange

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_DoubleArrayMax(int    numElements,
                                     double *srcArr,
                                     double *compArr,
                                     double *destArr);




/** Scales a double array.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr, @a destArr must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.
  - @a destArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  int dim = PARX_get_dim("PVM_FovSatGrad", 1);
  for(int i = 0; i < dim; i++)
  {
    double norm = UT_EuclideanNormOfDoubleArray(3, PVM_FovSatGrad[i]);
    UT_ScaleDoubleArray(3, PVM_FovSatGrad[i], 1.0 / norm, PVM_FovSatGrad[i]);
  }
  @endcode

  @param      numElements   Number of array elements
  @param[in]  srcArr        Array containing the source data
  @param      scalingFactor Scalar double scaling factor
  @param[out] destArr       Destination array containing the scaled
                            data after function call

  @see @ref UT_InitDoubleArray, @ref UT_EuclideanNormOfDoubleArray,
       @ref UT_DoubleArrayMax,
       @ref UT_DoubleArraySetInRange, @ref UT_InvertDoubleArray,
       @ref UT_NormalizeDoubleArray

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_ScaleDoubleArray(int    numElements,
                                       double *srcArr,
                                       double scalingFactor,
                                       double *destArr);




/** Calculates an array that is the minimum of two arrays.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr, @a compArr, @a destArr must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.
  - @a compArr is a @c NULL pointer.
  - @a destArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  #define N 10
  double val1[N];
  double val2[N];
  double valMin[N];

  // fill arrays ...

  UT_DoubleArrayMin(N, val1, val2, valMin);
  @endcode

  @param      numElements Number of array elements
  @param[in]  srcArr      First array to be compared
  @param[in]  compArr     Second array to be compared
  @param[out] destArr     Destination array containing elements that are
                          min of the two arrays

  @see @ref UT_MaxOfDoubleArray, @ref UT_MinOfDoubleArray,
       @ref UT_AbsMaxOfDoubleArray,
       @ref UT_DoubleArrayMax, @ref UT_DoubleArraySetInRange

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_DoubleArrayMin(int    numElements,
                                     double *srcArr,
                                     double *compArr,
                                     double *destArr);




/** Checks the range of array elements.

  Each entry of a source array is checked to lie within the ranges defined by a min- and
  a max-array.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr, @a minArr, @a maxArr, @a destArr must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.
  - @a minArr is a @c NULL pointer.
  - @a maxArr is a @c NULL pointer.
  - @a destArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  #define N 10
  double val[N];
  double valMin[N];
  double valMax[N];
  double valInRange[N];

  // fill arrays ...

  UT_DoubleArraySetInRange(N, val, valMin, valMax, valInRange);
  @endcode

  @param      numElements Number of array elements
  @param[in]  srcArr      Source array
  @param[in]  minArr      Array containing minimum values
  @param[in]  maxArr      Array containing maximum values
  @param[out] destArr     Destination array

  @see @ref UT_InitDoubleArray, @ref UT_ScaleDoubleArray, @ref UT_InvertDoubleArray,
       @ref UT_NormalizeDoubleArray,
       @ref UT_IntArraySetInRange, @ref UT_IntArraySetInRangeArray

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_DoubleArraySetInRange(int    numElements,
                                            double *srcArr,
                                            double *minArr,
                                            double *maxArr,
                                            double *destArr);




/** Inverts (changes sign) a double array.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  // invert first column of a 3x3 orientation matrix
  double oriMatrix[3][3];
  // ...
  UT_InvertDoubleArray(3, oriMatrix[0]);
  @endcode

  @param         numElements    Number of elements to be inverted
  @param[in,out] srcArr         Double array

  @see @ref UT_InitDoubleArray,
       @ref UT_EuclideanNormOfDoubleArray,
       @ref UT_ScaleDoubleArray,
       @ref UT_NormalizeDoubleArray

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_InvertDoubleArray(int    numElements,
                                        double *srcArr);




/** Normalizes a double array.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  // normalize first column of a 3x3 orientation matrix
  double oriMatrix[3][3];
  // ...
  UT_NormalizeDoubleArray(3, oriMatrix[0]);
  @endcode

  @param         numElements    Number of elements to be normalized
  @param[in,out] srcArr         Double array

  @return @c Yes = array norm is > 0, i.e. normalization was performed

  @see @ref UT_InitDoubleArray,
       @ref UT_EuclideanNormOfDoubleArray,
       @ref UT_ScaleDoubleArray, @ref UT_InvertDoubleArray

  @ingroup PVM_UT
*/
DLL_INTERFACE YesNo UT_NormalizeDoubleArray(int    numElements,
                                            double *srcArr);




/** Calculates the scalar product of two double arrays.

  @PVM_PARS
  NONE

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr1 and @a srcArr2 must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr1 is a @c NULL pointer.
  - @a srcArr2 is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  double vec1[3];
  double vec2[3];
  // ...
  double scalarProduct = UT_ScalarProductDoubleArray(3, vec1, vec2);
  @endcode

  @param     numElements  Number of elements to be inverted
  @param[in] srcArr1      Double array
  @param[in] srcArr2      Double array

  @return Scalar product

  @ingroup PVM_UT
*/
DLL_INTERFACE double UT_ScalarProductDoubleArray(int    numElements,
                                                 double *srcArr1,
                                                 double *srcArr2);




/** Interpolates between double arrays with equidistant values starting concurrently.

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
  UT_InterpolateDoubleArray(nGrad, nAq, dwGrad, dwAq, shapeGrad, shapeAq);
  @endcode

  @param      ns    # Points in source array
  @param      nt    # Points in destination array
  @param      ds    Interval size of source array
  @param      dt    Interval size of destination array
  @param[in]  ys    Values of source array (must be of length @a ns)
  @param[out] yt    Values of destination array (must be of length @a nt)

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE void UT_InterpolateDoubleArray(int    ns,
                                             int    nt,
                                             double ds,
                                             double dt,
                                             double *ys,
                                             double *yt);




/* /pv60/gen/src/prg/pvm_toolboxes/PvUtilTools/files.c */

/** Checks if file can be accessed for reading.

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
  const char *filename = "/tmp/test.dat";
  if (UT_FileAccessForRead(filename) == No)
  {
    UT_ReportError("file cannot be accessed for reading");
  }
  @endcode

  @param[in] filepath filename including path

  @return @c Yes = file can be accessed for reading, else @c No

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE YesNo UT_FileAccessForRead(char *filepath);




/** Writes a double array into a debug file.

  The file is written in ASCII format to the directory
  @c PVHOME/prog/curdir/USER/ParaVision/\<process\>.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Invalid input parameters
  - Opening file failed.

  @PVM_EXAMPLE
  @code
  UT_WriteDoubleArrayToDebugFile("debugData", data, n);
  @endcode

  @param[in] fileName    Name of output file
  @param[in] arr         Array with double values (at least @a nRow times @a nCol)
  @param     nRow        Number of rows
  @param     nCol        Number of columns

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE void UT_WriteDoubleArrayToDebugFile(const char   *fileName,
                                                  const double *arr,
                                                  const int    nRow,
                                                  const int    nCol = 1);




/** Writes an integer array into a debug file.

  The file is written in ASCII format to the directory
  @c PVHOME/prog/curdir/USER/ParaVision/\<process\>.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Invalid input parameters
  - Opening file failed.

  @PVM_EXAMPLE
  @code
  UT_WriteIntArrayToDebugFile("debugData", data, n);
  @endcode

  @param[in] fileName    Name of output file
  @param[in] arr         Array with integer values (at least @a nRow times @a nCol)
  @param     nRow        Number of rows
  @param     nCol        Number of columns

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE void UT_WriteIntArrayToDebugFile(const char *fileName,
                                               const int  *arr,
                                               const int  nRow,
                                               const int  nCol = 1);



/** Writes an array to a file.

  The file is written in ASCII format to the specified file.
  Note: The directory must exist otherwise an exception is thrown.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Invalid input parameters
  - Opening file failed.

  @PVM_EXAMPLE
  @code
  unsigned int length = 128;
  std::vector<double> array(128, 0);
  UT_WriteArrayToFile("/tmp/debugData.txt", array.data(), length);
  @endcode

  @param[in] fileName    Name of output file including path
  @param[in] arr         Array with values of length @a nRow times @a nCol
  @param[in] nRow        Number of rows
  @param[in] nCol        Number of columns (Default = 1)

  @ingroup PVM_UT_INT
*/
template<typename T>
DLL_INTERFACE void UT_WriteArrayToFile(
  const char * fileName,
  T * arr,
  size_t nRow,
  size_t nCol = 1
)
{
  int ret = core::File::WriteDataToAscii(fileName, arr, nRow, nCol);

  if (ret != 0) {
    PvStdException("In function \"UT_WriteArrayToFile\": Failed to write file.");
  }
}



/* /pv60/gen/src/prg/pvm_toolboxes/PvUtilTools/intArray.c */

/** Initializes an int array so that all indices receive the same given value.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  int maxAbsVals[N_SHIMS];
  double maxVal = CFG_MaxShimValue(0);
  UT_InitIntArray(N_SHIMS, maxAbsVals, maxVal);
  @endcode

  @param      numElements  Number of elements to be set
  @param[out] srcArr       Integer array
  @param      initValue    Value to which the elements should be set

  @see @ref UT_AbsOfIntArray, @ref UT_SumOfIntArray, @ref UT_InitDoubleArray

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_InitIntArray(int numElements,
                                   int *srcArr,
                                   int initValue);




/** Determines the maximum value of an int array.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  int index;
  int maxMatrix = UT_MaxOfIntArray(PTB_GetSpatDim(), PVM_Matrix, &index);
  @endcode

  @param      numElements Number of elements to be searched
  @param[in]  srcArr      Integer array
  @param[out] maxIdx      (optional) Index of the element containing the maximum.
                          Will not be set if a @c NULL pointer is parsed.

  @return Maximum value of the first @a numElements in the array

  @see @ref UT_MinOfIntArray, @ref UT_AbsMaxOfIntArray, @ref UT_MaxOfDoubleArray

  @ingroup PVM_UT
*/
DLL_INTERFACE int UT_MaxOfIntArray(int       numElements,
                                   const int *srcArr,
                                   int       *maxIdx = NULL);




/** Determines the minimum value of an int array.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  int index;
  int minMatrix = UT_MinOfIntArray(PTB_GetSpatDim(), PVM_Matrix, &index);
  @endcode

  @param      numElements Number of elements to be searched
  @param[in]  srcArr      Integer array
  @param[out] minIdx      (optional) Index of the element containing the minimum.
                          Will not be set if a @c NULL pointer is parsed.

  @return Minimum value of the first @a numElements in the array

  @see @ref UT_MaxOfIntArray, @ref UT_AbsMaxOfIntArray, @ref UT_MinOfDoubleArray

  @ingroup PVM_UT
*/
DLL_INTERFACE int UT_MinOfIntArray(int       numElements,
                                   const int *srcArr,
                                   int       *minIdx = NULL);




/** Determine the absolute maximum value of an int array.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  int k_range = UT_AbsMaxOfIntArray(PVM_EncMatrix[0], PVM_EncSteps0);
  @endcode

  @param      numElements Number of elements to be searched
  @param[in]  srcArr      Integer array
  @param[out] maxIdx      (optional) Index of the element containing the maximum.
                          Will not be set if a @c NULL pointer is parsed.

  @return Maximum value of the first @a numElements in the array

  @see @ref UT_MaxOfIntArray, @ref UT_MinOfIntArray, @ref UT_AbsMaxOfDoubleArray

  @ingroup PVM_UT
*/
DLL_INTERFACE int UT_AbsMaxOfIntArray(int numElements,
                                      int *srcArr,
                                      int *maxIdx = NULL);




/** Sets all the elements of an int array to their absolute value.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  #define N 10
  int val[N];
  ...

  UT_AbsOfIntArray(N, val);
  @endcode

  @param         numElements   Number of elements to be searched
  @param[in,out] srcArr        Integer array

  @see @ref UT_InitIntArray, @ref UT_SumOfIntArray

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_AbsOfIntArray(int numElements,
                                    int *srcArr);




/** Sums the elements of an int array.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  int totalSlices = UT_SumOfIntArray(PVM_NSPacks, PVM_SPackArrNSlices);
  @endcode

  @param     numElements Number of elements to be summed
  @param[in] srcArr      Integer array

  @return Sum of the first @a numElements in the array

  @see @ref UT_InitIntArray, @ref UT_AbsOfIntArray

  @ingroup PVM_UT
*/
DLL_INTERFACE int UT_SumOfIntArray(int numElements,
                                   int *srcArr);




/** Determines whether an int array is populated with unique values.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  int totalSlices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);
  if (UT_UniqueIntArray(totalSlices, PVM_ObjOrderList) == No)
  {
    UT_ReportError("objects in order list are not unique");
  }
  @endcode

  @param     numElements  Number of elements to be considered
  @param[in] srcArr       Int array to be tested

  @return @c Yes if the values are unique otherwise @c No

  @ingroup PVM_UT
*/
DLL_INTERFACE YesNo UT_UniqueIntArray(int numElements,
                                      int *srcArr);




/** Writes a destination array based on a source array that lies within the range
    defined by @a maxVal and @a minVal.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_SwapInt

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr and @a destArr must be dimensioned to @a numElements.
  - If @a hgNdxs is supplied it must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.
  - @a destArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  UT_IntArraySetInRange(PTB_GetSpatDim(),
                        PVM_MinMatrix,
                        2, 32768,
                        PVM_MinMatrix);
  @endcode

  @param      numElements Number of elements contained in each of the arrays @a srcArr,
                          @a destArr, and @a chgNdxs
  @param[in]  srcArr      Int array of size @a numElements containing the source data
  @param      minVal      Minimum value of the range
  @param      maxVal      Maximum value of the range
  @param[out] destArr     Int array of size @a numElements containing the destination data
  @param[out] chgNdxs     (optional) If parsed as a @c NULL pointer it has no function,
                          otherwise its elements are set to @c Yes or @c No depending on whether
                          they had to be changed.

  @return @c Yes if one or more elements had to be modified

  @see @ref UT_IntArraySetInRangeArray, @ref UT_DoubleArraySetInRange

  @ingroup PVM_UT
*/
DLL_INTERFACE YesNo UT_IntArraySetInRange(int   numElements,
                                          int   *srcArr,
                                          int   minVal,
                                          int   maxVal,
                                          int   *destArr,
                                          YesNo *chgNdxs = NULL);




/** Writes a destination array based on a source array that lies within
    the range defined by arrays of maxima and minima. If either @a minArr
    or @a maxArr may be @c NULL pointers the function delivers the min of
    @a srcArr and @a maxArr or the max of @a srcArr and @a minArr, respectively.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a srcArr, @a minArr,@a maxArr, and @a destArr must be dimensioned to
       @a numElements.
  - If @a hgNdxs is supplied it must be dimensioned to @a numElements.

  @PVM_ERROR
  - @a numElements < @c 1
  - @a srcArr is a @c NULL pointer.
  - Both @a minArr and @a maxArr are @c NULL pointers.
  - @a destArr is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  UT_IntArraySetInRangeArray(PTB_GetSpatDim(),
                             PVM_Matrix,
                             PVM_MinMatrix,
                             PVM_MaxMatrix,
                             PVM_Matrix);
  @endcode

  @param      numElements Number of array elements
  @param[in]  srcArr      Array containing the source data
  @param[in]  minArr      Array containing the minimum values
  @param[in]  maxArr      Array containing the maximum values
  @param[out] destArr     Array of destination values containing the
                          checked elements @param[out] chgNdxs
                          (optional) If parsed as a @ NULL pointer it
                          has no function, otherwise its elements are
                          set to @c Yes or @c No depending on whether
                          they had to be changed.

  @return @c Yes if one or more elements had to be modified

  @see @ref UT_IntArraySetInRange, @ref UT_DoubleArraySetInRange

  @ingroup PVM_UT
*/
DLL_INTERFACE YesNo UT_IntArraySetInRangeArray(int   numElements,
                                               int   *srcArr,
                                               int   *minArr,
                                               int   *maxArr,
                                               int   *destArr,
                                               YesNo *chgNdxs = NULL);




/* /pv60/gen/src/prg/pvm_toolboxes/PvUtilTools/oriMatrix.c */

/** Initializes an orientation matrix to unity.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a oriMatrix must be dimensioned to [3][3]

  @PVM_ERROR
  - @a oriMatrix == @c NULL pointer
  - @a oriMatrix[0] == @c NULL pointer

  @PVM_EXAMPLE
  @code
  PARX_change_dims("ACQ_grad_matrix", 1, 3, 3);
  UT_InitOriMatrix(PtrType3x3 ACQ_grad_matrix[0]);
  @endcode

  @param[out] oriMatrix    3 by 3 orientation matrix

  @see @ref UT_CopyOriMatrix

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_InitOriMatrix(double (*oriMatrix)[3][3]);




/** Copies a source orientation matrix to a destination orientation matrix.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a oriMatrix must be dimensioned to [3][3]
  - @a dstMatrix must be dimensioned to [3][3]

  @PVM_ERROR
  - @a oriMatrix == @c NULL pointer
  - @a oriMatrix[0] == @c NULL pointer
  - @a dstMatrix == @c NULL pointer
  - @a dstMatrix[0] == @c NULL pointer

  @PVM_EXAMPLE
  @code
  UT_CopyOriMatrix(PtrType3x3 PVM_SPackArrGradOrient[0], PtrType3x3 ACQ_grad_matrix[0]);
  @endcode

  @param[in]  oriMatrix    3 by 3 orientation matrix
  @param[out] dstMatrix    Destination 3 by 3 orientation matrix

  @see @ref UT_InitOriMatrix

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_CopyOriMatrix(double (*oriMatrix)[3][3],
                                    double (*dstMatrix)[3][3]);




/** Inserts a column vector into a column of an orientation matrix.

  An orientation matrix array is always defined as follows:
  @code
  double parameter
  {
    ...
	 }OriMatrix[][3][3];
  }
  @endcode
  It is intended to hold the matrix to convert the imaging coordinate
  system (r,p,s) for each slice package into the patient coordinate
  system (x,y,z). (The conversion into the gradient coordinate system is
  done in reaction to the patient position.) The value of these matrices
  is intended to be passed to the @c ACQ_grad_matrix parameter.
  The definition of row and column vectors in this matrix can be done in
  two ways:
  - From a technical point of view <c>OriMatrix[0][0]</c> is resolved to the pointer
    of the first @b row vector of the first orientation matrix.
  - The matrix multiplication however is executed using the transposed orientation matix, so in
    fact <c>OriMatrix[0][0]</c> corresponds in this way to a @b column vector.

  @PVM_H2{Note}
  Using the standard way of slice geometry handling, this function does not have to be used!

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a oriMatrix must be dimensioned to [3][3]
  - @a colVector must be dimensioned to [3]

  @PVM_ERROR
  - @a colNumber < @c 0
  - @a colNumber > @c 2
  - @a colVector == @c NULL pointer
  - @a oriMatrix == @c NULL pointer
  - @a oriMatrix[0] == @c NULL pointer

  @PVM_EXAMPLE
  Replaces the third column vector of the third entry of parameter @c OriMatrix (please note,
  that the cast is mandatory, @c PtrType3x3 may be used instead) by the values of @c ColVec
  (which has to be defined as a three element double array).
  @code
  // define parameters
  double parameter OriMatrix[][3][3];
  double parameter ColVec[3];
  ...

  // insert column
  UT_OriMatrixInsertColumn( (double (*)[3][3]) OriMatrix[0], ColVec,  2);
  @endcode

  @param[in,out]  oriMatrix    3 by 3 orientation matrix
  @param[in]      colVector    3 element column vector
  @param          colNumber    Number of a column in the orientation matrix

  @see @ref UT_OriMatrixExtractColumn, @ref UT_OriMatrixMaxOfColumn,
       @ref UT_OriMatrixAbsMaxOfColumn, @ref UT_OriMatrixInvertColumn, @ref UT_OriMatrixColumnsNormalized,
       @ref UT_OriMatrixNormalizeColumns

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE void UT_OriMatrixInsertColumn(double (*oriMatrix)[3][3],
                                            double  *colVector,
                                            int      colNumber);




/** Extracts a column vector from a column of an orientation matrix.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a oriMatrix must be dimensioned to [3][3]
  - @a colVector must be dimensioned to [3]

  @PVM_ERROR
  - @a colNumber < @c 0
  - @a colNumber > @c 2
  - @a colVector == @c NULL pointer
  - @a oriMatrix == @c NULL pointer
  - @a oriMatrix[0] == @c NULL pointer

  @PVM_EXAMPLE
  Extract third column
  @code
  // define parameters
  double parameter OriMatrix[][3][3];
  double parameter ColVec[3];
  ...

  // extract column
  UT_OriMatrixExtractColumn( (double (*)[3][3]) OriMatrix[0], ColVec,  2);
  @endcode

  @param[in]  oriMatrix    3 by 3 orientation matrix
  @param[out] colVector    3 element column vector
  @param      colNumber    Number of a column in the orientation matrix

  @see @ref UT_OriMatrixInsertColumn, @ref UT_OriMatrixMaxOfColumn,
       @ref UT_OriMatrixAbsMaxOfColumn, @ref UT_OriMatrixInvertColumn, @ref UT_OriMatrixColumnsNormalized,
       @ref UT_OriMatrixNormalizeColumns

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE void UT_OriMatrixExtractColumn(double (*oriMatrix)[3][3],
                                             double  *colVector,
                                             int      colNumber);




/** Determines the maximum value and its index in a given column of an orientation matrix.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_MaxOfDoubleArray

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a oriMatrix must be dimensioned to [3][3]

  @PVM_ERROR
  - @a colNumber < @c 0
  - @a colNumber > @c 2
  - @a maxIndexInCol == @c NULL pointer
  - @a oriMatrix == @c NULL pointer
  - @a oriMatrix[0] == @c NULL pointer

  @PVM_EXAMPLE
  Get maximum of third column
  @code
  // define parameters
  double parameter OriMatrix[][3][3];
  ...

  // get maximum
  int index;
  double maxCol = UT_OriMatrixMaxOfColumn( (double (*)[3][3]) OriMatrix[0], 2, &index);
  @endcode

  @param[in]  oriMatrix        3 by 3 orientation matrix
  @param      colNumber        Number of a column in the orientation matrix
  @param[out] maxIndexInCol    (optional) Index of the maximum value in a column (@c NULL = not requested)

  @return Maximum value in a column

  @see @ref UT_OriMatrixInsertColumn, @ref UT_OriMatrixExtractColumn,
       @ref UT_OriMatrixAbsMaxOfColumn, @ref UT_OriMatrixInvertColumn, @ref UT_OriMatrixColumnsNormalized,
       @ref UT_OriMatrixNormalizeColumns

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE double UT_OriMatrixMaxOfColumn(double (*oriMatrix)[3][3],
                                             int     colNumber,
                                             int     *maxIndexInCol = NULL);




/** Determines the maximum absolute value (and index) in a given column of an orientation
    matrix.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_AbsMaxOfDoubleArray

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a oriMatrix must be dimensioned to [3][3]

  @PVM_ERROR
  - @a colNumber < @c 0
  - @a colNumber > @c 2
  - @a maxIndexInCol == @c NULL pointer
  - @a oriMatrix == @c NULL pointer
  - @a oriMatrix[0] == @c NULL pointer

  @PVM_EXAMPLE
  Get absolute maximum of third column
  @code
  // define parameters
  double parameter OriMatrix[][3][3];
  ...

  // get maximum
  int index;
  double maxCol = UT_OriMatrixAbsMaxOfColumn( (double (*)[3][3]) OriMatrix[0], 2, &index);
  @endcode

  @param[in]  oriMatrix        3 by 3 orientation matrix
  @param      colNumber        Number of a column in the orientation matrix
  @param[out] maxIndexInCol    (optional) Index of the maximum value in a column (@c NULL = not requested)

  @return Maximum value in a column

  @see @ref UT_OriMatrixInsertColumn, @ref UT_OriMatrixExtractColumn, @ref UT_OriMatrixMaxOfColumn,
       @ref UT_OriMatrixInvertColumn, @ref UT_OriMatrixColumnsNormalized,
       @ref UT_OriMatrixNormalizeColumns

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE double UT_OriMatrixAbsMaxOfColumn(double (*oriMatrix)[3][3],
                                                int     colNumber,
                                                int     *maxIndexInCol = NULL);




/** Inverts a column in an orientation matrix.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_InvertDoubleArray

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a oriMatrix must be dimensioned to [3][3]

  @PVM_ERROR
  - @a colNumber < @c 0
  - @a colNumber > @c 2
  - @a maxIndexInCol == @c NULL pointer
  - @a oriMatrix == @c NULL pointer
  - @a oriMatrix[0] == @c NULL pointer

  @PVM_EXAMPLE
  Invert third column
  @code
  // define parameters
  double parameter OriMatrix[][3][3];
  ...

  // invert
  double maxCol = UT_OriMatrixInvertColumn( (double (*)[3][3]) OriMatrix[0], 2);
  @endcode

  @param[in,out] oriMatrix        3 by 3 orientation matrix
  @param         colNumber        Number of a column in the orientation matrix

  @see @ref UT_OriMatrixInsertColumn, @ref UT_OriMatrixExtractColumn, @ref UT_OriMatrixMaxOfColumn,
       @ref UT_OriMatrixAbsMaxOfColumn, @ref UT_OriMatrixColumnsNormalized,
       @ref UT_OriMatrixNormalizeColumns

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE void UT_OriMatrixInvertColumn(double (*oriMatrix)[3][3],
                                            int    colNumber);




/** Normalize columns of an orientation matrix.

  Different from @ref UT_OriMatrixNormalizeColumns this function does not create errors.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_EuclideanNormOfDoubleArray
  - @ref UT_ScaleDoubleArray

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a oriMatrix must be dimensioned to [3][3]

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  // define parameters
  double parameter OriMatrix[][3][3];
  ...

  // normalize
  UT_OriMatrixColumnsNormalized( (double (*)[3][3]) OriMatrix[0]);
  @endcode

  @param[in,out] oriMatrix        3 by 3 orientation matrix

  @return @c Yes = normalization was performed, else @c No (norm was zero)

  @see @ref UT_OriMatrixInsertColumn, @ref UT_OriMatrixExtractColumn, @ref UT_OriMatrixMaxOfColumn,
       @ref UT_OriMatrixAbsMaxOfColumn, @ref UT_OriMatrixInvertColumn,
       @ref UT_OriMatrixNormalizeColumns

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE YesNo UT_OriMatrixColumnsNormalized(double (*oriMatrix)[3][3]);




/** Normalize columns of an orientation matrix.

  Different from @ref UT_OriMatrixColumnsNormalized this function may create errors.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_NormalizeDoubleArray

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a oriMatrix must be dimensioned to [3][3]

  @PVM_ERROR
  - @a oriMatrix == @c NULL pointer
  - @a oriMatrix[0] == @c NULL pointer

  @PVM_EXAMPLE
  @code
  // define parameters
  double parameter OriMatrix[][3][3];
  ...

  // normalize
  UT_OriMatrixNormalizeColumns( (double (*)[3][3]) OriMatrix[0]);
  @endcode

  @param[in,out] oriMatrix        3 by 3 orientation matrix

  @return @c Yes = normalization was performed, else @c No (norm was zero)

  @see @ref UT_OriMatrixInsertColumn, @ref UT_OriMatrixExtractColumn, @ref UT_OriMatrixMaxOfColumn,
       @ref UT_OriMatrixAbsMaxOfColumn, @ref UT_OriMatrixInvertColumn, @ref UT_OriMatrixColumnsNormalized

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE YesNo UT_OriMatrixNormalizeColumns(double (*oriMatrix)[3][3]);




/** Make sure that orientation matrix conforms to standards.

  - Matrix is normalized.
  - If not orthonormal, it is set to identity.
  - The maximum value of each column is made positive (i.e. restrict orientation to positive half sphere).

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_OriMatrixNormalizeColumns
  - @ref UT_OriMatrixIsOrthoNormal
  - @ref UT_OriMatrixSetIdentity
  - @ref UT_AbsMaxOfDoubleArray
  - @ref UT_OriMatrixInvertColumn

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a oriMatrix must be dimensioned to [3][3]

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  UT_OriMatrixStandardOrientation(PtrType3x3 ACQ_grad_matrix[0]);
  @endcode

  @param[in,out] oriMatrix        3 by 3 orientation matrix

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE void UT_OriMatrixStandardOrientation(double (*oriMatrix)[3][3]);




/** Test if matrix is orthogonal.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_EuclideanNormOfDoubleArray
  - @ref UT_ScalarProductDoubleArray

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a oriMatrix must be dimensioned to [3][3]

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  // define parameters
  double parameter OriMatrix[][3][3];
  ...

  if (UT_OriMatrixIsOrthogonal( (double (*)[3][3]) OriMatrix[0]) == No)
  {
    UT_ReportError("orientation matrix is not orthogonal");
  }
  @endcode

  @param[in] oriMatrix        3 by 3 orientation matrix

  @return @c Yes = matrix ist orthogonal, else @c No

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE YesNo UT_OriMatrixIsOrthogonal(double (*oriMatrix)[3][3]);




/** Test if matrix is orthonormal (orthogonal and normalized).

  Different from @ref UT_OriMatrixWellDefined this function does not create errors.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_EuclideanNormOfDoubleArray
  - @ref UT_OriMatrixIsOrthogonal

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a oriMatrix must be dimensioned to [3][3]

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  // define parameters
  double parameter OriMatrix[][3][3];
  ...

  if (UT_OriMatrixIsOrthoNormal( (double (*)[3][3]) OriMatrix[0]) == No)
  {
    UT_ReportError("orientation matrix is corrupt");
  }
  @endcode

  @param[in] oriMatrix        3 by 3 orientation matrix

  @return @c Yes = matrix ist orthonormal, else @c No

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE YesNo UT_OriMatrixIsOrthoNormal(double (*oriMatrix)[3][3]);




/** Test if matrix is orthonormal (orthogonal and normalized).

  Different from @ref UT_OriMatrixIsOrthoNormal this function normalizes the matrix
  and may also create errors.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_OriMatrixNormalizeColumns
  - @ref UT_OriMatrixIsOrthogonal

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a oriMatrix must be dimensioned to [3][3]

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  // define parameters
  double parameter OriMatrix[][3][3];
  ...

  if (UT_OriMatrixWellDefined( (double (*)[3][3]) OriMatrix[0]) == No)
  {
    UT_ReportError("orientation matrix is corrupt");
  }
  @endcode

  @param[in,out] oriMatrix        3 by 3 orientation matrix

  @return @c Yes = matrix ist orthonormal, else @c No

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE YesNo UT_OriMatrixWellDefined(double (*oriMatrix)[3][3]);




/** Set orientation matrix to identity.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a oriMatrix must be dimensioned to [3][3]

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  // define parameters
  double parameter OriMatrix[][3][3];
  ...

  UT_OriMatrixSetIdentity( (double (*)[3][3]) OriMatrix[0]);
  @endcode

  @param[out] oriMatrix        3 by 3 orientation matrix

  @see @ref UT_InitOriMatrix

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE void UT_OriMatrixSetIdentity(double (*oriMatrix)[3][3]);




/** Angulation of an orientation matrix.

  Angle between normal vector of slice plane and closest axis.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_AbsMaxOfDoubleArray
  - @ref UT_Degrees

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a oriMatrix must be dimensioned to [3][3]

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  ACQ_slice_angle[0] = UT_OriMatrixAngulation(PtrType3x3 PVM_SPackArrGradOrient[0]);
  @endcode

  @param[in] oriMatrix        3 by 3 orientation matrix

  @return slice angle [deg]

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE double UT_OriMatrixAngulation(double (*oriMatrix)[3][3]);




/** Extends an array of 3x3 matrices.

  If an array of 3x3 matrices has been redimensioned to a larger array size, each new element
  is initialized with the matrix of the reference index.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a newSize and @a oldSize have to be > @c 0
  - @a newSize must be > @a oldSize
  - @a mat is expected to be an array of 3x3 matrices with @a newSize elements

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  int dim = PARX_get_dim("PVM_SPackArrNSlices", 1);
  UT_Extend3x3Matrix(PtrType3x3 PVM_SPackArrGradOrient[0],
                     PVM_NSPacks,
                     dim,
                     -1);
  @endcode

  @param[in,out] mat          Array of 3x3 matrices
  @param         newSize      New matrix array size
  @param         oldSize      Old matrix array size
  @param         reference    Reference index for initialization of new array elements
                              - < 0: array element @a oldSize-1 is used

  @return @c Yes in case of success and legal arguments else @c No

  @ingroup PVM_UT_INT
 */
DLL_INTERFACE YesNo UT_Extend3x3Matrix(double    (*mat)[3][3],
                                       const int newSize,
                                       const int oldSize,
                                       int       reference);




/* /pv60/gen/src/prg/pvm_toolboxes/PvUtilTools/parameters.c */

/** Constructs a dynamic numeric enum with numeric entries dependent on a formatting string.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a enumName is a @c NULL pointer.
  - @a enumName is an empty string.
  - @a numIndices < @c 1
  - @a formatStr is a @c NULL pointer.
  - @a formatStr is an empty string.
  - @a enumStrLen < @c 2

  @PVM_EXAMPLE
  @code
  UT_MakeNumericEnum("PVM_SpatDimEnum", 1, 3, "%dD", 10 );
  @endcode

  @param[in] enumName      Name of a PARX enum parameter
  @param     startIdx      Starting index
  @param     numIndices    Number of enum entries
  @param[in] formatStr     Format string
  @param     enumStrLen    Maximum length of an enum string

  @see @ref UT_MakeDimEnum, @ref UT_DimEnumRange, @ref UT_DimEnumValueToInt

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE void UT_MakeNumericEnum(const char *enumName,
                                      int startIdx,
                                      int numIndices,
                                      const char *formatStr,
                                      int enumStrLen);




/** Constructs a dynamic dim enum.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_SetRangeInLimitsInt
  - @ref UT_MakeNumericEnum

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a enumName is a @c NULL pointer.
  - @a enumName is an empty string.
  - @a lowDimLimit < @c 0
  - @a highDimLimit < @c 0

  @PVM_EXAMPLE
  @code
  int numEntries = UT_MakeDimEnum("PVM_SpatDimEnum",
                                  1, 3,
                                  0, 3 );
  @endcode

  @param[in] enumName        Name of a PARX enum parameter
  @param     lowestDim       Lowest dimension to which a dimension can be set
  @param     highestDim      Highest dimension to which a dimension can be set
  @param     lowDimLimit     Lowest permissable value of a dimension.
  @param     highDimLimit    Highest permissible value of a dimension

  @return Number of indices in the enum

  @see @ref UT_MakeNumericEnum, @ref UT_DimEnumRange, @ref UT_DimEnumValueToInt

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE int UT_MakeDimEnum(const char *enumName,
                                 int lowestDim,
                                 int highestDim,
                                 int lowDimLimit,
                                 int highDimLimit);




/** Determines the valid range of the dimensions of a dim enum.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a enumName is a @c NULL pointer.
  - @a enumName is an empty string.
  - @a lowestDim is a @c NULL pointer.
  - @a highestDim is a @c NULL pointer.

  @PVM_EXAMPLE
  @code
  int dimLow, dimHigh;
  UT_MakeNumericEnum("PVM_SpatDimEnum", &dimLow, &dimHigh);
  @endcode

  @param[in]  enumName        Name of a PARX enum parameter
  @param[out] lowestDim       Lowest dimension to which a dimension can be set
  @param[out] highestDim      Highest dimension to which a dimension can be set

  @see @ref UT_MakeNumericEnum, @ref UT_MakeDimEnum, @ref UT_DimEnumValueToInt

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE void UT_DimEnumRange(const char *enumName,
                                   int        *lowestDim,
                                   int        *highestDim);




/** Converts the integer value of a dimension to an int representing the equivalent enum value.

  @PVM_H1{Warning}
  The value returned here is not generally equal to the integer representing the dimension.
  For example, if the @c DimEnum is initialized to have the values 2D and 3D then the @a dimValues
  representing these possible enum values are 2 and 3 respectively whereas the enum int values
  will be 0 and 1 respectively.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_DimEnumRange
  - @ref UT_SetInLimits

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a enumName is a @c NULL pointer.
  - @a enumName is an empty string.

  @PVM_EXAMPLE
  @code
  PVM_SpatDimEnum = (DIM_ENUM_TYPE)UT_DimEnumValueToInt("PVM_SpatDimEnum", 2);
  @endcode

  @param[in] enumName    Name of the enum
  @param     dimValue    Integer value of dimension

  @return Enum string index

  @see @ref UT_MakeNumericEnum, @ref UT_MakeDimEnum, @ref UT_DimEnumRange

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE int UT_DimEnumValueToInt(const char *enumName,
                                       int        dimValue);




/** Grows or allocates a scratch buffer.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_FreeScratchBuffer

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a bufferSize < @c 0
  - @a newBufferSize < @c 0
  - @a theBuffer could not be allocated.

  @PVM_EXAMPLE
  @code
  int parNameLength = 100;
  int bufferSize = 0;
  char *buffer = NULL;

  UT_MakeScratchBuffer(&bufferSize, &buffer, parNameLength + 1);
  @endcode

  @param[in,out]  bufferSize       Current buffer size
  @param[out]     theBuffer        Buffer
  @param          newBufferSize    New buffer size

  @return @c Yes if @a theBuffer is allocated or @c No for a @c NULL pointer

  @see @ref UT_FreeScratchBuffer

  @ingroup PVM_UT
*/
DLL_INTERFACE YesNo UT_MakeScratchBuffer(int  *bufferSize,
                                         char **theBuffer,
                                         int  newBufferSize);




/** Frees a scratch buffer and sets it to a @c NULL pointer.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a theBuffer was not allocated.

  @PVM_EXAMPLE
  @code
  int parNameLength = 100;
  int bufferSize = 0;
  char *buffer = NULL;

  UT_MakeScratchBuffer(&bufferSize, &buffer, parNameLength + 1);
  ...

  UT_FreeScratchBuffer(&buffer);
  @endcode

  @param[in,out] theBuffer        Buffer to be freed

  @see @ref UT_MakeScratchBuffer

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_FreeScratchBuffer(char **theBuffer);




/** Retrieves the number of dims of a PARX parameter.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a parName is a @c NULL pointer.
  - @a parName is an empty string.

  @PVM_EXAMPLE
  @code
  if (UT_GetNumDims("PVM_SPackArrGradOrient") != 3)
  {
    UT_ReportError("dimensionality of gradient matrix corrupt");
  }
  @endcode

  @param[in] parName    Parameter name

  @return Number of dimensions

  @ingroup PVM_UT
*/
DLL_INTERFACE int UT_GetNumDims(const char * parName);




/** Redimensions a comma separated list of one dimensional PARX parameters.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_MakeScratchBuffer
  - @ref UT_GetNumDims
  - @ref UT_FreeScratchBuffer

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a newDim < @c 1
  - Parameter does not exist.
  - Parameter is not 1d.

  @PVM_EXAMPLE
  @code
  UT_RedimensionListOf1d(PTB_GetSpatDim(),
                         "PVM_MinMatrix,"
                         "PVM_MaxMatrix,"
                         "PVM_Matrix");
  @endcode

  @param     newDim         New size of the arrays
  @param[in] parNameList    Comma separated list of PARX parameters

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE void UT_RedimensionListOf1d(int        newDim,
                                          const char *parNameList);




/* /pv60/gen/src/prg/pvm_toolboxes/PvUtilTools/stringUtils.c */

/** Checks whether a string ends with a specified extension.

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
  Remove extension in name of pulse file
  @code
  char *extPointer = UT_StringExtension(pulseFileName, ".exc");
  if (extPointer)
  {
    *extPointer='\0';
  }
  @endcode

  @param[in] str    String to be examined
  @param[in] ext    String of an extension

  @return Pointer to the last occurence of @a ext in string, or @c NULL if not found

  @see @ref UT_FindElemInString

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE char *UT_StringExtension(char              *str,
                                       const char *const ext);




/** Finds an element in an elementlist.

  Finds the first occurrence of an element @a elem (represented as a string) in a list of
  elements @a elemList. @a elemList is represented as a string in which elements are separated
  by a separator @a delim.

  @PVM_PARS
  - Argument @a index

  @PVM_TBCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  For the following elemList
  @code
  const char *const ElemList = "this,is,an,element,list";
  @endcode
  The call of
  @code
    int index;

    UT_FindElemInString("an",ElemList,',',&index);
  @endcode
  returns @c Yes and the argument @a index has the value @c 3.

  @param[in]  elem        Element name represented as a string
  @param[in]  elemList    Element list of element strings separated by @a delim
  @param      delim       Delimiter used to separate different elements in an element list
                          represented as char
  @param[out] index       Pointer to an integer value where the position of the element in the
                          element list is stored (first element is element @c 1)

  @return @c Yes if the element has been found, @c No if the element is not in the element list
          or the element list is not built up correctly. In this case @c -1 is assigned to the
          parameter index.

  @see @ref UT_StringExtension

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE YesNo UT_FindElemInString(const char *const elem,
                                        const char *const elemList,
                                        const char        delim,
                                        int *const        index);



/* /pv60/gen/src/prg/pvm_toolboxes/PvUtilTools/handlerUtils.c */

/** Tells the request handling mechanism which parameter has been edited.

  Sometimes, especially in functions handling the interdependences of parameters describing a
  common higher level functionality (<b>parameter groups</b>) it is necessary to distinguish
  - which parameter
  - which array element of a parameter
  - which struct member of a parameter

  has been modified in response to a user request in order to implement a specific software
  behaviour. With help of this function the edit process of a user (independent whether it took
  place due to changes in method editor or using shell commands) is labelled and all relevant
  information is stored internally and made accessible using UT- request handling toolbox
  funcions listed below.

  Since the first software action in response to a user request is a call of the parameter
  relation, this function is intended to be used in the default relation of a parameter. The
  information about edit processes is available
  - until @ref UT_SetRequest is called again,
  - until a call of @ref UT_ClearRequest.

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
  Parameter definition:
  @code
  PVM_RF_PULSE_TYPE parameter
  {

    display_name "My RF Pulse";
    relations "My_RfPulse_Relation";

  }My_RfPulse;
  @endcode

  Parameter relation:
  @code
  void My_RfPulse_Relation(void)
  {
  ...
     UT_SetRequest("My_RfPulse");
  ...
  }
  @endcode

  @param[in] parname    Terminated string containing the variable name of the parameter
                        (in difference to the display name)

  @see @ref UT_GetRequest, @ref UT_ClearRequest, @ref UT_HandleRequest, @ref UT_RequestParName,
       @ref UT_RequestIndexDim, @ref UT_RequestIndex

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_SetRequest(const char *const parname);

/** Clears information stored by request handling mechanism.

  With help of this function information about an edit process of a parameter (stored using
  function @ref UT_SetRequest) is cleared. After a call of @ref UT_ClearRequest,
  @ref UT_GetRequest will always deliver a return value of @c No. This function is intended to
  be used in parameter group handling functions after all interdependences of the parameter
  group members are resolved successfully.

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

  @see @ref UT_GetRequest, @ref UT_SetRequest, @ref UT_HandleRequest, @ref UT_RequestParName,
       @ref UT_RequestIndexDim, @ref UT_RequestIndex

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_ClearRequest(void);




/** Tells the request handling mechanism that a request is handled.

  This function is intended to be used if @ref UT_GetRequest returns @c Yes and this request
  should be handled by this function. The usage of @ref UT_HandleRequest is optional, if it is
  used, @ref UT_ClearRequest should be called if the request is handled (i.e. a parameter group
  has been updated correctly in response to the request). @n
  For detailled information see documentation of @ref UT_GetRequest.

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

  @return @c No in case the request is already handled (by a previous call of this function)
          else @c Yes

  @see @ref UT_GetRequest, @ref UT_SetRequest, @ref UT_ClearRequest, @ref UT_RequestParName,
       @ref UT_RequestIndexDim, @ref UT_RequestIndex

  @ingroup PVM_UT
*/
DLL_INTERFACE YesNo UT_HandleRequest(void);




/** Determines if a request for a certain parameter has been set.

  This is an overloaded implementation of @ref UT_GetRequest with a simplified interface.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_GetRequest

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  double BandWidthLast = PVM_EffSWh;
  if (UT_GetRequest("PVM_EffSWh") == Yes)
  {
    UT_RequestParOrigValue(&BandWidthLast);
    UT_ClearRequest();
  }
  @endcode

  @param[in]  par   parameter name for which request should be tested

  @return @c Yes = request found, else @c No

  @see @ref UT_SetRequest, @ref UT_GetRequest, @ref UT_ClearRequest, @ref UT_HandleRequest,
       @ref UT_RequestParName, @ref UT_RequestIndexDim, @ref UT_RequestIndex

  @ingroup PVM_UT
*/
DLL_INTERFACE YesNo UT_GetRequest(const char *const par);





/** Determines if a request for one parameter in a list has been set.

  This is an overloaded implementation of @ref UT_GetRequest with a simplified interface.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_GetRequest

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  int parIndex;
  if (UT_GetRequest("PVM_EchoTime, PVM_RepetitionTime, PVM_EffSWh", &parIndex) == Yes)
  {
    switch (parIndex)
    {
      ...
    }
    UT_ClearRequest();
  }
  @endcode

  @param[in]  parlist   Nonempty string of comma separated parameter variable names
  @param[out] parindex  Index of parameter in list for which request has been found

  @return @c Yes = request found, else @c No

  @see @ref UT_SetRequest, @ref UT_ClearRequest, @ref UT_HandleRequest, @ref UT_RequestParName,
       @ref UT_RequestIndexDim, @ref UT_RequestIndex

  @ingroup PVM_UT
*/
DLL_INTERFACE YesNo UT_GetRequest(const char *const parlist,
                                  int *const        parindex);




/** Delivers name and struct level of a modified parameter.

  This is an overloaded implementation of @ref UT_GetRequest, providing different kind of information.

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
  int structLevel;
  if (UT_GetRequest("PVM_SliceGeo", NULL, &structLevel) == Yes)
  {
      ...
  }
  @endcode

  @param[in]  parlist   Nonempty string of comma separated parameter variable names
  @param[out] parind    Index of parameter in list for which request has been found
                        (@c NULL if only one parameter in list)
  @param[out] slevel    Struct level of modification

  @return @c Yes = request found, else @c No

  @see @ref UT_SetRequest, @ref UT_ClearRequest, @ref UT_HandleRequest, @ref UT_RequestParName,
       @ref UT_RequestIndexDim, @ref UT_RequestIndex

  @ingroup PVM_UT
*/
DLL_INTERFACE YesNo UT_GetRequest(const char *const parlist,
                                  int *const        parind,
                                  int *const        slevel);




/** Delivers information stored by the request handling mechanism.

  Sometimes, especially in functions handling the interdependences of parameters describing a
  common higher level functionality (<b>parameter groups</b>) it is necessary to distinguish
  - which parameter
  - which array element of a parameter
  - which struct member of a parameter

  has been modified in response to a user request in order to implement a specific software
  behaviour. With help of @ref UT_SetRequest, the edit process of a user (independent whether
  it took place due to changes in method editor or using shell commands) is labelled and all
  relevant information is stored internally and made accessible using UT- request handling
  toolbox funcions listed below.

  This function is intended to be used in functions resolving the interdependences of
  parameters.

  @PVM_H2{Information delivered by this function}
  -# Return value: A value of @c Yes is returned if
    - argument @a handlerParList is a non empty string containing a comma separated list of
      parameter names with at least one element.
    - @ref UT_SetRequest has been called previously with an argument containing one of the
      parameter names specified in the comma separated list @a handlerParList (see argument
      desrciption below).
  -# Argument @a parIndex. If the function returnes @c Yes, the position of the parameter name
     inside the list @a handlerParList is stored in the variable @a parIndex points to (first
     element == @c 1). This argument might be used in the handler function to treat a parameter
     specific software behaviour in a switch statement.
  -# Argument @a member. If one or more parameters contained in the @a handlerParList is a
     struct parameter and argument @a memberList is a non empty comma separated list of struct
     member names, the position of the struct member name inside the list @a memberList is
     stored in the variable @a member points to.

  @PVM_H2{Note}
  There are more overloaded implementations of this function, requiring less input parameters.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - If the conditions of a @c Yes return value (see above) are met, but a successful handling
    of the request was not possible due to errors.

  @PVM_EXAMPLE
  @PVM_H2{Handling a parameter group}
  A parameter group consisting of three parameters should be handled by a function.

  - Parameter group:
    @code
    pargroup
    {
      SliceGradients;
      RfPulses;
      SliceVec;
    }SliceSelection;
    @endcode

  - Parameter definition:
    @code
    PVM_RF_PULSE_TYPE parameter
    {
      display_name "Radio Frequency Pulses";
      relations "RFPulsesRel";
    }RFPulses[3];

    double parameter
    {
      display_name "Slice Gradients";
      relations "SliceGradRel";
    }SliceGrad[3];

    double parameter
    {
      display_name "Slice Vectors";
      relations "SliceVecRel";
    }SliceVec[3][3];
    @endcode

  - Parameter relations:
    @code
    void RFPulsesRel(void)
    {
    ...
       UT_SetRequest("RFPulses");
    ...
    }

    void RFPulsesRel(void)
    {
    ...
       UT_SetRequest("RFPulses");
    ...
    }

    void SliceGradRel(void)
    {
    ...
       UT_SetRequest("SliceGrad");
    ...
    }

    void SliceVecRel(void)
    {
    ...
       UT_SetRequest("SliceVec");
    ...
    }
    @endcode

  - Usage of @ref UT_GetRequest in a group handling function:
    @code

    void HandleSliceSelection(void)
    {
      int parNo, structNo;

      const char *const parlist = "SliceGradients,RFPulses,SliceVec";
      const char *const stuctlist "FlipAngle,Attenuation,Length,Bandwidth";

      YesNo requestDetected;

      (...)

        requestDetected = UT_GetRequest(parlist,&parNo,structlist,&structNo);

      if(requestDetected == Yes)
      {
        switch(parNo)
        {
        case 1: // Par SliceGradients has been edited
          (...)
            break;
        case 2: // Par RFPulses has been edited
          (...)
            break;
        case 3: // Par SliceVec has been edited
          (...)
            break;
        default:
            break;
        }
      }

      (...)

      if(parNo == 2)
      {
        switch(structNo)
        {
        case 1: // member FlipAngle has been edited
            (...)
              break;
        case 2: // member Attenuation has been edited
            (...)
              break;
        case 3: // member Length has been edited
            (...)
              break;
        case 4: // member Bandwidth has been edited
            (...)
              break;
        default: // one or more members have been edited e.g. via shell command
        }
      }

      (...)
         if( requestDetected == Yes)
            UT_ClearRequest();// request has been handled correctly
    }
    @endcode

  @param[in]  handlerParList   Nonempty string of comma separated parameter variable names
  @param[out] parIndex         Pointer to an integer variable (for usage see example above)
  @param[in]  memberList       String of comma separated struct member names (might be empty if no
                               struct member is in @a memberlist)
  @param[out] member           Pointer to an integer variable (for usage see example above)
                               parameter (in difference to the display name)

  @return @c Yes = request found, else @c No

  @see @ref UT_SetRequest, @ref UT_ClearRequest, @ref UT_HandleRequest,
       @ref UT_RequestParName, @ref UT_RequestIndexDim, @ref UT_RequestIndex

  @ingroup PVM_UT
*/
DLL_INTERFACE YesNo UT_GetRequest(const char *const handlerParList,
                                  int *const        parIndex,
                                  const char *const memberList,
                                  int *const        member);




/** Delivers the parameter name stored in request handling mechanism.

  This function returns the parameter name of the last @ref UT_SetRequest call.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @ref UT_SetRequest has been called before.
  - @ref UT_ClearRequest has not been called.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  char parName[128];
  if (UT_RequestParName(parName) == Yes)
  {
    DB_MSG(("parameter changed: %s", parName));
    UT_ClearRequest();
  }
  @endcode

  @param name    Character array (long enough to hold a parameter name, a size of 128 is
                 recommended)

  @return @c Yes in case a parameter name is available, @c No otherwise

  @see @ref UT_GetRequest, @ref UT_SetRequest, @ref UT_ClearRequest, @ref UT_HandleRequest,
       @ref UT_RequestIndexDim, @ref UT_RequestIndex

  @ingroup PVM_UT
*/
DLL_INTERFACE YesNo UT_RequestParName(char *name);




/** Delivers information about indices stored in request handling mechanism.

  This function is intended to be used after a call of @ref UT_GetRequest in case it is
  necessary to get information about (multiple) indexed array parameters. It returns the number
  of indices necessary to address the value changed by an edit process. @n
  A return value of @c 0 indicates that
  - no array parameter has been edited,
  - no special element of an array parameter has been edited (e.g. if the complete parameter
    array has been modified using the single parameter editor or a shell command).

  In the example shown in @ref UT_GetRequest, the maximum possible return value of
  @ref UT_RequestIndexDim returns
  - @c 2 if (for instance) SliceVec[0][2] has been edited,
  - @c 1 if (for instance)  SliceVec[1] has been edited in the single parameter editor of
    ScanControl or using a shell command,
  - @c 0 if (for instance)  SliceVec has been edited in the single parameter editor of
    ScanControl or using a shell command.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @ref UT_SetRequest has been called before.
  - @ref UT_ClearRequest has not been called.

  @PVM_ERROR
  NONE

  @return  Number of indices necessary to address the value changed by an edit process

  @see @ref UT_GetRequest, @ref UT_SetRequest, @ref UT_ClearRequest, @ref UT_HandleRequest,
       @ref UT_RequestParName, @ref UT_RequestIndex

  @ingroup PVM_UT
*/
DLL_INTERFACE int UT_RequestIndexDim(void);




/** Delivers indices stored in request handling mechanism.

  This function is intended to be used after a call of @ref UT_GetRequest in case it is
  necessary to get information about (multiple) indexed array parameters.
  @c UT_RequestIndex(i) with @c 0 <= @c i < @ref UT_RequestIndexDim returns the value of the
  i-th index of a (multiple) indexed parameter array element necessary to address the element
  which has been modified due to a user request. @n
  It returns @c -1 in case
  - no array parameter has been edited,
  - no special element of an array parameter has been edited (e.g. if the complete parameter
    array has been modified using the single parameter editor or a shell command),
  - argument @a dim is less than @c 0,
  - argument @a dim is greater than or equal to the return value of @ref UT_RequestIndexDim.

  In the example shown in @ref UT_GetRequest an edit process of <c>SliceVec[1][2]</c> results in the
  following return values:
  - @c UT_RequestIndex(0) = @c 1
  - @c UT_RequestIndex(1) = @c 2
  - @c UT_RequestIndex(2) = @c -1

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @ref UT_SetRequest has been called before.
  - @ref UT_ClearRequest has not been called.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  int requestIndex;
  YesNo requestFound = UT_GetRequest("PVM_Fov, PVM_SpatResol", &requestIndex);
  if (requestFound == Yes)
  {
    // find array index of changed element
    int indexEdited = UT_RequestIndex(0);
  }
  @endcode

  @param dim    Dimension to be examined (see example above)

  @return Value of the i-th index of a (multiple) indexed parameter array element

  @see @ref UT_GetRequest, @ref UT_SetRequest, @ref UT_ClearRequest, @ref UT_HandleRequest,
       @ref UT_RequestParName, @ref UT_RequestIndexDim

  @ingroup PVM_UT
*/
DLL_INTERFACE int UT_RequestIndex(const int dim);




/** Documentation missing.

  Never used.

  @todo Incomplete documentation: PVM_PARS, PVM_TBCALL, PVM_RELCALL,
                                  PVM_PRECOND, PVM_ERROR, return

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE int UT_RequestSmemberDim(void);




/** Documentation missing.

  Never used.

  @todo Incomplete documentation: PVM_PARS, PVM_TBCALL, PVM_RELCALL,
                                  PVM_PRECOND, PVM_ERROR, PVM_EXAMPLE, param, return

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE YesNo UT_RequestSmemberName(char *SmemberName);

/** Documentation missing.

  @todo Incomplete documentation: PVM_PARS, PVM_TBCALL, PVM_RELCALL,
                                  PVM_PRECOND, PVM_ERROR

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE void UT_PrintRequestInfo(void);

/** Documentation missing.

  Only used by internal test.

  @todo Incomplete documentation: PVM_PARS, PVM_TBCALL, PVM_RELCALL,
                                  PVM_PRECOND, PVM_ERROR, PVM_EXAMPLE, param, return

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE int UT_OutputRequestInfo(char *out,
                                       int  maxLen);




/** Documentation missing.

  Request handling: Delivers name and struct level of a modified parameter.

  @todo Incomplete documentation: PVM_PARS, PVM_TBCALL, PVM_RELCALL,
                                  PVM_PRECOND, PVM_ERROR, PVM_EXAMPLE, param, return

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE YesNo UT_GetSmemberRequest(const char *const smemlist,
                                         int *const        smem,
                                         const int         slevel,
                                         const YesNo       qualified);




/** Documentation missing.

  @todo Incomplete documentation: PVM_PARS, PVM_TBCALL, PVM_RELCALL,
                                  PVM_PRECOND, PVM_ERROR, PVM_EXAMPLE, param, return

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE int UT_RequestStructElemDim(const int  level,
                                          const char *name,
                                          int        *iarr,
                                          const int  iasize);




/** Documentation missing.

  @todo Incomplete documentation: PVM_PARS, PVM_TBCALL, PVM_RELCALL,
                                  PVM_PRECOND, PVM_ERROR, PVM_EXAMPLE, param, return

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE int UT_RequestParOrigValueSize(size_t *const siz);




/** Documentation missing.

  @todo Incomplete documentation: PVM_PARS, PVM_TBCALL, PVM_RELCALL,
                                  PVM_PRECOND, PVM_ERROR, return

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE YesNo UT_RequestParOrigValueAvailable(void);




/** After changing a parameter, get its original value.

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
  if (UT_GetRequest("PVM_DummyScansDur") == Yes)
  {
    // PVM_DummyScans is changed by at least 1 in the indicated direction

    double dummyScansDurLast;

    if (UT_RequestParOrigValue(&dummyScansDurLast) == Yes)
    {
      if (PVM_DummyScansDur > dummyScansDurLast)
      {
        PVM_DummyScans += 1;
      }
      else if (PVM_DummyScansDur < dummyScansDurLast)
      {
        PVM_DummyScansDur -= 1;
      }
    }
  }
  @endcode

  @param[out] buf buffer where original value will be written,
                  must have the same type as the changed parameter

  @return @c Yes = value could be determined, else @c No

  @see @ref UT_RequestParOrigDim

  @ingroup PVM_UT
*/
DLL_INTERFACE YesNo UT_RequestParOrigValue(void *buf);




/** After changing a parameter, get dimension of original parameter value.

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
  int newDim = PARX_get_dim(PVM_EncMatrix, 1);
  int origDim = 0;
  if (UT_RequestParOrigDim(0, &origDim) == Yes && newDim != origDim)
  {
    ...
  }
  @endcode

  @param      dim    The dimension index, beginning with 0
  @param[out] dimval The number of elements in a dimension, 0 if it is not an
                     array or dimension does not contain any elements

  @return @c Yes = dimension could be determined, else @c No

  @see @ref UT_RequestParOrigValue

  @ingroup PVM_UT
*/
DLL_INTERFACE YesNo UT_RequestParOrigDim(int dim,
                           int *dimval);




/** Documentation missing.

  @todo Incomplete documentation: PVM_PARS, PVM_TBCALL, PVM_RELCALL,
                                  PVM_PRECOND, PVM_ERROR, PVM_EXAMPLE, param

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE void UT_RequestAddConflictSource(const char *parname);




/** Documentation missing.

  @todo Incomplete documentation: PVM_PARS, PVM_TBCALL, PVM_RELCALL,
                                  PVM_PRECOND, PVM_ERROR, PVM_EXAMPLE, param

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE void UT_RequestRemoveConflictSource(const char *parname);




/* /pv60/gen/src/prg/pvm_toolboxes/PvUtilTools/moduleUtils.c */

/** Returns the index of the maximum of a double array.

  @PVM_USEINSTEAD{UT_MaxOfDoubleArray}

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @return Index of maximum value

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE int UT_GetNdxOfMaxArr(double *array,
                                    int    arrayDim);




/** Returns the minimum of a double array.

  @PVM_USEINSTEAD{UT_MinOfDoubleArray}

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @return Minimum of double array

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE double UT_GetMinOfDoubleArray(double *d_arr,
                                            int    n_elem);




/** Returns the maximum of a double array.

  @PVM_USEINSTEAD{UT_MaxOfDoubleArray}

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @return Maximum of double array

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE double UT_GetMaxOfDoubleArray(double *d_arr,
                                            int     n_elem);




/** Calculates the norm of a vector.

  @PVM_USEINSTEAD{UT_EuclideanNormOfDoubleArray}

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @return Norm of vector

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE double  UT_GetNormOfVec(double *vecTor);




/** Normalizes a vector.

  @PVM_USEINSTEAD{UT_NormalizeDoubleArray}

  @PVM_TBCALL
  - @ref UT_GetNormOfVec

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @see @ref UT_GetNormOfVec

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE void UT_NormalizeVec(double *vecTor);




/** Calculate determinant of a 3x3 matrix.

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
  double det;
  double oriMat[9];

  ...

  UT_Det3x3(oriMat, &det);
  @endcode

  @param[in]  mv  3x3 matrix in 9-element array
  @param[out] res determinant

  @return
  - @c 0 = successful
  - @c 1 = @a mv = @c NULL

  @ingroup PVM_UT
*/
DLL_INTERFACE int UT_Det3x3(double *mv,
                            double *const res);




/* /pv60/gen/src/prg/pvm_toolboxes/PvUtilTools/utilities.c */

/**  Returns methods manager version.

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE int UT_Version(void);




/** Converts an angle from degrees to radians.

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
  double invAngleRad = UT_Radians(180.0);
  @endcode

  @param angleDeg    Angle expressed in degrees

  @return Angle converted to radians

  @see @ref UT_Degrees

  @ingroup PVM_UT
*/
DLL_INTERFACE double UT_Radians(double angleDeg);




/** Converts an angle from radians to degrees.

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
  double invAngleDeg = UT_Degrees(M_PI);
  @endcode

  @param angleRad   Angle expressed in radians

  @return Angle converted to degrees

  @see @ref UT_Radians

  @ingroup PVM_UT
*/
DLL_INTERFACE double UT_Degrees(double angleRad);




/** Constrains an integer range with some limits.

  If @a lowIntValue exceeds @a highIntValue the two are swapped. @a lowIntValue and
  @a highIntValue are constrained to lie within @a lowerIntLimit and @a higherIntLimit.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_SetInLimits
  - @ref UT_SwapInt

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  const int lowLim = 1, highLim = 10;
  int lowVal = 7, highVal = -3;

  UT_SetRangeInLimitsInt(&lowVal, &highVal, lowLim, highLim);

  // now lowVal = 1, highVal = 7
  @endcode

  @param[in,out] lowIntValue       int defining the lowest value in a range
  @param[in,out] highIntValue      int defining the highest value in a range
  @param         lowerIntLimit     int defining the lower limit of a range
  @param         upperIntLimit     int defining the upper limit of a range

  @see @ref UT_SetInLimits

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_SetRangeInLimitsInt(int *lowIntValue,
                                          int *highIntValue,
                                          int lowerIntLimit,
                                          int upperIntLimit);




/** Constrains an int value to lie within given limits.

  If too high it is set to the upper limit, if too low it is set to the lower limit. The lower
  and upper limits are swapped internally if necessary.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref UT_SwapInt

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  const int lowLim = 1, highLim = 10;
  int val = -3;

  UT_SetInLimits(&val, lowLim, highLim);

  // now val = 1
  @endcode

  @param[in,out] intValue          int value
  @param         lowerIntLimit     int defining the lower limit of a range
  @param         upperIntLimit     int defining the upper limit of a range

  @return Constrained value

  @see @ref UT_SetRangeInLimitsInt

  @ingroup PVM_UT
*/
DLL_INTERFACE int UT_SetInLimits(int intValue,
                                 int lowerIntLimit,
                                 int upperIntLimit);




/** Swaps two integer values.

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
  const int a = 1, b = 10;

  UT_SwapInt(&a, &b);

  // now a = 10, b = 1
  @endcode

  @param[in,out] aIntValue    int value
  @param[in,out] bIntValue    int value

  @see @ref UT_SwapDouble

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_SwapInt(int *aIntValue,
                              int *bIntValue);




/** Swaps two double values.

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
  const double a = 1.5, b = 10.5;

  UT_SwapDouble(&a, &b);

  // now a = 10.5, b = 1.5
  @endcode

  @param aDoubleValue    double value
  @param bDoubleValue    double value

  @see @ref UT_SwapInt

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_SwapDouble(double *aDoubleValue,
                                 double *bDoubleValue);




/** Reports a PVM toolbox or method error.

  If a serious error occurs in a PVM toolbox or method, it should be reported with this
  function. An exception is created and the given error message will be displayed.
  The parameter @c PVM_ErrorMessage may be used as a container for the error string.

  @PVM_PARS
  - @c PVM_ErrorMessage

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a errorStr is a null pointer
  - @a errorStr is an empty string

  @PVM_EXAMPLE
  @code
  const double maxVal = 100.0;
  double val = 200.0;

  if (val > maxVal)
  {
    sprintf(PVM_ErrorMessage, "value %.2f exceeds limt %.2f", val, maxVal);
    UT_ReportError(PVM_ErrorMessage);
  }
  @endcode

  @param errorStr    Error message to be reported

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_ReportError(const char *errorStr);




/** Tests whether a nucleus name is well formed.

  @PVM_USEINSTEAD{CFG_IsNucleusKnown}

  A nucleus name is composed of a string of digits followed by a string of letters in which
  only the first letter is uppercase. If the flag @a changeCase is set to @c Yes any character
  if incorrect case will be converted to the appropriate case and @a nucName will be modifed.

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @param nucName
  @param changeCase

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE YesNo UT_IsNucleusNameWellFormed(char  *nucName,
                                               YesNo changeCase);




/** Checks if different arrays have the same dimension.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Illegal array parameter

  @PVM_EXAMPLE
  @code
  if (UT_ArrParsHaveSameDimension("PVM_FovSatSliceOrient,"
                                  "PVM_FovSatThick,"
                                  "PVM_FovSatOffset") == No)
  {
    UT_RedimensionListOf1d(PVM_FovSatNSlices,
                           "PVM_FovSatSliceOrient,"
                           "PVM_FovSatThick,"
                           "PVM_FovSatOffset");
  }
  @endcode

  @param[in] parlist comma-separated list of parameter names

  @return @c Yes = same dimension, else @c No

  @ingroup PVM_UT
*/
DLL_INTERFACE YesNo UT_ArrParsHaveSameDimension(const char *const parlist);




/** Documentation missing.

  Never used.

  @todo Incomplete documentation: PVM_PARS, PVM_TBCALL, PVM_RELCALL,
                                  PVM_PRECOND, PVM_ERROR, PVM_EXAMPLE, param, return

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE const char *UT_GetActiveStructMember(const char *const activeQualif);

/** Documentation missing.

  Never used.

  @todo Incomplete documentation: PVM_PARS, PVM_TBCALL, PVM_RELCALL,
                                  PVM_PRECOND, PVM_ERROR, PVM_EXAMPLE, param, return

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE YesNo UT_GetActiveArrayIndex(const char *const activeQualif,
                                           int *const        arrInd);




/** Checks if @a stringArr contains requested string.

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
  #define N 3
  const char *names[N] = {"Felix", "Peter", "Paul"};
  int i = UT_StringIsInStringArr("Peter", names, N);

  // result: i = 1;
  @endcode

  @param[in]  stringRequest  String to be tested for
  @param[in]  stringArr      Array of strings
  @param      arrSize        Number of strings in array

  @return
  - Array index where string was found
  - @c -1 if not found

  @ingroup PVM_UT
*/
DLL_INTERFACE int UT_StringIsInStringArr(const char *const stringRequest,
                                         const char *const *stringArr,
                                         const int         arrSize);




/** Returns the raw data size (MB).

  Assuming 4 byte per real data point the dataset size is calculated according to the matrix
  entries (argument @a matrix), the array of anti aliasing factors (argument @a antiAlias),
  the number of repetitions (argument @a repetitions) and the number of images (argument
  @a images).

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Legal values of arguments

  @PVM_ERROR
  - Argument @a dim outside of range [@c 1;@c 3]
  - Argument @a repetitions or images less than @c 1
  - Array elements of @a matrix less than @c 1
  - Array elements of @a antiAlias less than @c 1

  @PVM_EXAMPLE
  @code
  if (UT_RawDataSize(PTB_GetSpatDim(), PVM_Matrix, PVM_AntiAlias, 1, NI) > CFG_MaxDatasetSize())
  {
    UT_ReportError("data size exceeds possible maximum");
  }
  @endcode

  @param dim            Dimensionality of arrays @a matrix and @a antiAlias
  @param matrix         Imaging matrix (arraysize @a dim)
  @param antiAlias      Anti aliasing factors (arraysize @a dim)
  @param repetitions    Number of repetitions
  @param images         Number of images

  @return Raw dataset size (MB)

  @ingroup PVM_UT_INT
*/
DLL_INTERFACE double UT_RawDataSize(const int dim,
                                    int       *matrix,
                                    double    *antiAlias,
                                    int       repetitions,
                                    int       images);




/** Converts scan time into a string.

  This function may be used to convert a scan time (ms) into a string representing the scan
  time in hours, min, sec, and ms.

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
  double TotalTime = PVM_RepetitionTime * PVM_EncMatrix[1];
  UT_ScanTimeStr(PVM_ScanTimeStr, TotalTime);
  @endcode

  @param timeStr     Points to a string variable to be set by this function
                     (at least 16 characters including @c 0)
  @param timeMsec    Calculated scan time (ms)

  @ingroup PVM_UT
*/
DLL_INTERFACE void UT_ScanTimeStr(char   *timeStr,
                                  double timeMsec);




/** Delivers parameter type.
  This function should be used in functions using a string argument to retrieve
  or set parameter values. In this case it is necessary to check whether the parameter
  (as specified by @a parname) matches the expected type.
  In addition it retrieves in optional arguments

  - if the parameter is qualified
  - if the parameter is an array,
  - the dimensionality of the parameter.

  A parameter name is qualified, if elements of struct parameters of array parameters
  are explicitly specified (e.g. "RFPulse1.Length" for parameters of type PVM_RF_PULSE, or
  "ACQ_grad_matrix[0][1]"). The function does not change the state or value of the parameter.



  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE

  @PVM_H2{Example with struct parameter type}

  @code
  // assume parameter RfPulse1 is a parameter of type PVM_RF_PULSE
  int failed=0;
  char partype[128];
  YesNo isArray=No;
  YesNo qualified=No;
  YesNo isarray=No;
  int dimensions=0;
  char errormsg[512];

  failed=UT_GetParType("RfPulse1",partype,&qualified,&isArray,&dimensions);

  switch(failed)
  {
     case 0:       // function called correctly
         if(strcmp(partype,"PVM_RF_PULSE")  // test correct parameter type
         {

            snprintf(errormsg,511,"Parameter %s has wrong parameter type %s expected %s",
                              "RfPulse1",partype,"PVM_RF_PULSE");
            UT_ReportError(errormsg);
         }
         break;
     case -1:     // does not occur since parname and partype are not 0
         break;
     case 1:
         snprintf(errormsg,511,"Parameter %s does not exist","RfPulse1");
         UT_ReportError(errormsg);
         break;
  }

  // value of arguments now:
  // partype set to "PVM_RF_PULSE"
  // qualified set to No
  // isarray set to No
  // dimensions set to 0 since parameter is not an array

  // following call is same as UT_GetParType("RfPulse1",partype,0,0,0);
  failed=UT_GetParType("RfPulse1",partype);

  // following call is same as UT_GetParType("RfPulse1",partype,&qualified,0,0);
  failed=UT_GetParType("RfPulse1",partype,&qualified);

  // following call is same as UT_GetParType("RfPulse1",partype,&qualified,0);
  failed=UT_GetParType("RfPulse1",partype,&qualified,&isarray);

  // use following call if qualification is not of interest but check of array is desired
  failed=UT_GetParType("RfPulse1",partype,0,&isarray);

  @endcode

  @PVM_H2{Example with array parameter}

  @code
  int failed=0;
  char partype[128];
  YesNo isArray=No;
  YesNo qualified=No;
  YesNo isarray=No;
  int dimensions=0;
  char errormsg[512];

  // examine parameter PVM_DwBmat which is an double array of 3x3 matrices

  failed==UT_GetParType("PVM_DwBmat",partype,&qualified,&isArray,&dimensions);
  switch(failed)
  {
     case 0:       // function called correctly
         if(strcmp(partype,"double")  // test correct parameter type
         {

            snprintf(errormsg,511,"Parameter %s has wrong parameter type %s expected %s",
                              "PVM_DwBmat",partype,"double");
            UT_ReportError(errormsg);
         }
         break;
     case -1:     // does not occur since parname and partype are not 0
         break;
     case 1:      // does not occur since parname is name of existing parameter
         snprintf(errormsg,511,"Parameter %s does not exist","PVM_DwBMat");
         UT_ReportError(errormsg);
         break;
  }

  // value of arguments now:
  // partype set to "double"
  // qualified set to No
  // isarray set to Yes
  // dimensions set to 3


  // example of qualified parameter
  failed==UT_GetParType("PVM_DwBmat[0]",partype,&qualified,&isArray,&dimensions);

  // value of arguments now:
  // partype set to "double"
  // qualified set to Yes
  // isarray set to Yes
  // dimensions set to 3 (dimensionality according to parameter definition i.e. always not qualified)


  @endcode


  @param  parname   parameter name (specified as string) that should be examined
  @param  type      pointer to string of at least 128 characters
  @param  qualified (optional) pointer to a variable of type YesNo (ignored if 0 pointer)
  @param  isarray   (optional) pointer to a variable of type YesNo (ignored if 0 pointer)
  @param  dimensions (optional) pointer a variable of type int (ignored if 0 pointer)


  @return
  - @c 0  = correct function call
  - @c -1 = failure: 0 pointer argument of parname or type (should be reported with UT_ReportError)
  - @c 1  = argument parname is not a PARX parameter name (should be reported with UT_ReportError)


  @ingroup PVM_UT
*/
DLL_INTERFACE int UT_GetParType(const char *const parname,
                                char       *type,
                                YesNo      *qualified=0,
                                YesNo      *isarray=0,
                                int        *const dimensions=0);




/** Provides information about main memory of reconstruction computer.

    This function delivers the amount of memory that is available
    on the reconstruction computer. In case method code estimates the
    amount of memory that is necessary to reconstruct data, it can be
    decided whether an online reconstruction is possible
    (e.g. installed memory is sufficiently larger than required memory).

    The physical installed RAM may be exceeded if a swap partition
    on the reconstruction computer exists. This swap space is used
    if the memory required by processes running on the computer
    exceeds the RAM. Using swap space makes memory operations much
    slower, the performance of running processes will degrade. Thus
    information about swap space may be used to decide whether
    a reconstruction is possible at all, but the reconstruction
    process should be started offline.

    If specified, optional arguments @c free_phys_ram and @c free_swap
    are set to the currently available memory. Note this information
    depends on the amount of processes that are running at the time
    this function is executed and should therefore not be used during
    experiment setup since these values may change at experiment
    execution time.


  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  On error, -1 is returned, and  errno  is set appropriately.

  @PVM_EXAMPLE
  @code

  double requiredMem=0.0;

  // calculate required memory somehow in this example
  // expected in units of MB


  int64_t physram=0;

  int status=UT_AvailableMemory(&physram);

  if(status==-1)
  {
    char errstr[128],errreason[64];
    strerror_r(errno,errreason,64)M

    snprintf(errstr,128,
             "Failed to retrieve memory information\nReason\n%s",
             errreason);
    UT_ReportError(errstr);
  }

  // example to convert result into MB

  double availMem = physram/(1024.0*1024.0);

  // switch off online reconstruction if required memory
  // exceeds 80% of installed RAM

  if(mem_required > 0.8*availMem)
  {
     ACQ_ScanOnlineReco=No;
  }
  @endcode

  @param phys_ram    total amount of physical RAM (unit byte)
  @param total_swap  total amount of swap space (unit byte, optional)
  @param free_phys_ram amount of free physical RAM (unit byte, optional)
  @param free_swap   amount of free swap space (unit byte, optional)

  @return 0 on success, 1 if phys_ram is 0 pointer, -1 if acces of
          information failed

  @ingroup PVM_UT
*/
DLL_INTERFACE int UT_AvailableMemory(int64_t *const phys_ram,
                                     int64_t *const total_swap=0,
                                     int64_t *const free_phys_ram=0,
                                     int64_t *const free_swap=0);



/**
  Returns the parameter name that resulted in the current method call.

  Upon modifiying a parameter in the method user
  interface, its relation gets called. Calling UT_RelContext_ParName() within
  the method returns a string that contains either the parameter
  name of the modified parameter, the parameter qualifiers or both,
  depending on the input arguments.

  In case the function gets called when no parameter was modified,
  the function returns an empty string.

  <b>Important note:</b> Every time parameter relations are called,
  information like e.g. the parameter name or the previous value of the
  parameter, are stored in this relation context. In fact, PV differentiates
  between two different relation contexts, the primary relation context and the
  secondary relation context. When the user changes a parameter @c A in PV, the
  relation of this parameter is called. Both the primary and the secondary
  relation context will be identical and will contain information about @c A.
  Now, at some point in the method, the relation of another parameter @c B
  might be called using @c ParxRelsParRelations("B", ...). From that point on
  the secondary relation context will contain the information about @c B, while
  the primary relation context will still contain the information about @c A.
  By default, all @c UT_RelContext_Par... functions use the primary relation
  context.

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
  Lets assume the user modifies the first entry of a parameter array
  with the following definition:
  @code
  int parameter {
      relations backbone;
  } MyTestArray[3];
  @endcode

  We can then use the function in the <c>backbone()</c> (and all subsequent
  function calls) as follows:
  @code
  std::string parname = UT_RelContext_ParName(); // same as UT_RelContext_ParName(true, false);
  DB_MSG(("name = %s", parname.c_str()));
  // prints: "name = MyTestArray"

  std::string qualifiers = UT_RelContext_ParName(false, true);
  DB_MSG(("qualifier = %s", qualifiers.c_str()));
  // prints: "qualifier = [0]"

  std::string fullname = UT_RelContext_ParName(true, true);
  DB_MSG(("full name = %s", fullname.c_str()));
  // prints: "full name = MyTestArray[0]"
  @endcode

  @param[in] withName Specifies if the parameter name shall be included.
  @param[in] withQualifiers Specifies if the qualifiers shall be included in the output.
  @param[in] getPrimRelCntxt If true, the primary relation context will be used,
  otherwise the secondary relation context will be used.

  @return
  The modified parameter name as a @c std::string

  @throws PvStdException when both input arguments are false

  @ingroup PVM_UT
*/
DLL_INTERFACE std::string UT_RelContext_ParName(bool withName = true, bool withQualifiers = false, bool getPrimRelCntxt = true);



/**
  Gets the previous value of a modified parameter.

  Sometimes the method needs to react to a parameter change by the user
  depending on the value that the parameter had prior to its modification.
  The function retrieves this previous value.
  The meaning of the argument @c getPrimRelCntxt is explained in UT_RelContext_ParName().

  This is a template function, which means the <c>T</c> in the function signature
  needs to be replaced by the type of the changed parameter. All method parameter
  types are supported, i.e. int, int64_t, double, char arrays, enums, YesNo,
  OnOff and in addition <c>std::string</c>. While char arrays are supported,
  it's easy to make mistakes with them.
  Thus it's recommended to utilize <c>std::string</c>s instead (see example below).

  A few additional important notes:
  - Calling this function only makes sense after you know which parameter
  was modified. i.e. it's strongly recommended to use the result of
  <c>UT_RelContext_ParName()</c> and an if-statement first before calling
  <c>UT_RelContext_ParOrigValue()</c>.
  - The function always returns a single value, i.e. if an
  entry of an array parameter is changed by the user, the function only returns
  the previous value of the single array entry and NOT the complete array.
  - The function does check if the byte-size of the return type matches the
  byte-size of the parameter value type, so requesting a value of type
  <c>double</c> although the parameter is of type int, will result in an
  exception at run-time.
  - There's still a pitfall when different types have the same byte-size.
  Unfortunately this case is not detected, so e.g. requesting a
  <c>double</c> value although the parameter type is <c>int64_t</c> will simply
  give you a wrong value.

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
  Let's say we have an array parameter and a text field on a parameter card in
  PV with the following parameter definitions:
  @code
  int parameter {
      relations backbone;
  } MyTestArr[3];

  char parameter {
      relations backbone;
  } MyTestStr[100];
  @endcode
  The content of MyTestArr shall be <c>{5,6,7}</c> and MyTestStr contains
  "ABC".<br>
  Now, in ParaVision, the user changes the first entry of MyTestArr to 4
  and MyTestStr to "XYZ".
  As a result the relation of the respective parameter, namely <c>backbone()</c>
  gets called. In <c>backbone()</c> and all subsequent function calls
  we can do the following to get the previous values:
  @code
  std::string parname = UT_RelContext_ParName();

  if (parname == "MyTestArr")
  {
      int value = 0;
      UT_RelContext_ParOrigValue(value);
      DB_MSG(("value = %d", value)); // prints "value = 5"
      DB_MSG(("MyTestArr[0] = %d", MyTestArr[0])); // prints "MyTestArr[0] = 4"
  }

  if (parname == "MyTestStr")
  {
      std::string str1 = "";
      char str2[100] = "";
      UT_RelContext_ParOrigValue(str1);
      UT_RelContext_ParOrigValue(str2);
      DB_MSG(("str1 = %s", str1.c_str())); // prints "value = ABC"
      DB_MSG(("str2 = %s", str2)); // also prints "value = ABC"
      DB_MSG(("MyTestStr = %s", MyTestStr)); // prints "MyTestStr = XYZ"
  }
  @endcode
  Careful, using a <c>char*</c> in the example above or a <c>char[]</c> with a
  different length than 100 will result in an exception, so it's better to use a
  <c>std::string</c>.

  @param[out] value
  The previous value of a modified parameter.
  @param[in] getPrimRelCntxt If true, the primary relation context will be used,
  otherwise the secondary relation context will be used.

  @throws PvStdException when no parameter was modified
  @throws PvStdException when no previous value is available
  @throws PvStdException when the byte-size of parameter type and return
  type do not match
  @throws PvStdException when fetching the previous value from the
  parameter space fails.

  @ingroup PVM_UT
*/
template<typename T> void UT_RelContext_ParOrigValue(T& value, bool getPrimRelCntxt = true)
{
    const Parx::Relation::Context* RelCntxt = nullptr;

    if (getPrimRelCntxt) {
        RelCntxt = ParxRelsGetPrimaryRelationContext();
    }
    else {
        RelCntxt = ParxRelsGetRelationContext();
    }

    if (RelCntxt != nullptr) {
        if ( RelCntxt->hasOrigValue() ) {
            if ( sizeof(value) == RelCntxt->getOrigParSize() ) {
                bool flag = RelCntxt->getOrigParValue(&value);
                if (!flag) {
                    throw PvStdException("UT_RelContext_ParOrigValue(): "
                        "Could not get original parameter value.");
                }
            }
            else {
                throw PvStdException("UT_RelContext_ParOrigValue(): "
                    "The Number of bytes for the requested output type (%ld) does not match the available value (%d). "
                    "Maybe the type of your output variable does not fit the type of the parameter?",
                    sizeof(value), RelCntxt->getOrigParSize());
            }
        }
        else {
            throw PvStdException("UT_RelContext_ParOrigValue(): "
                "No original value of the parameter exists.");
        }
    }
    else {
        throw PvStdException("UT_RelContext_ParOrigValue(): "
            "No relation context available.");
    }
};

template<> inline void UT_RelContext_ParOrigValue(std::string& value, bool getPrimRelCntxt)
{
    value = "";

    const Parx::Relation::Context* RelCntxt = nullptr;

    if (getPrimRelCntxt) {
        RelCntxt = ParxRelsGetPrimaryRelationContext();
    }
    else {
        RelCntxt = ParxRelsGetRelationContext();
    }

    if (RelCntxt != nullptr) {
        if ( RelCntxt->hasOrigValue() ) {
            int parsize = RelCntxt->getOrigParSize();
            char* tmp = new char[parsize];
            bool flag = RelCntxt->getOrigParValue(tmp);
            if (!flag) {
                delete tmp;
                throw PvStdException("UT_RelContext_ParOrigValue(): "
                    "Could not get original parameter value.");
            }
            else {
                value = tmp;
                delete tmp;
            }
        }
        else {
            throw PvStdException("UT_RelContext_ParOrigValue(): "
                "No original value of the parameter exists.");
        }
    }
    else {
        throw PvStdException("UT_RelContext_ParOrigValue(): "
            "No relation context available.");
    }
};



/**
Gets the dimensions of a parameter prior to its modification.

Calling this function when an array parameter was modified and
redimensioned, it
returns a vector containing the array dimensions before the parameter
was modified (see example).
The meaning of the argument @c getPrimRelCntxt is explained in UT_RelContext_ParName().

Important additional notes:
- Returns an empty vector if the parameter is not an array.
- Calling this function only makes sense if you know which parameter
was modified. i.e. it's strongly recommended to use the result of
<c>UT_RelContext_ParName()</c> and an if-statement first before calling
<c>UT_RelContext_ParOrigDim()</c>.
- For a struct array the function will only return the original dimensions
of the base element, i.e. a struct array parameter of the form
<c>PVM_Dummy[i][j].fieldA[n][m]</c>, will yield a vector of size 2 containing
the dimension of <c>"PVM_Dummy[i][j]"</c>.

@PVM_PARS
NONE

@PVM_TBCALL
UT_GetNumDims()

@PVM_RELCALL
NONE

@PVM_PRECOND
NONE

@PVM_ERROR
NONE

@PVM_EXAMPLE
In parsDefinition.h:
@code
int parameter {
  relations backbone;
} TestMatrix[][];
@endcode

In initMeth.c:
@code
PARX_change_dims("TestMatrix", 3, 3);
@endcode

In function backbone():
@code
std::vector<int> indices = UT_RelContext_ParArrayIndices();
int entered_value = TestMatrix[indices[0]][indices[1]];
DB_MSG(("Entered value = %d", entered_value));

if (entered_value > 0  && entered_value < 100) {
    std::vector<int> orig_dim = UT_RelContext_ParOrigDim();
    DB_MSG(("Original dimension of TestMatrix = [%d, %d]", orig_dim[0], orig_dim[1])); // prints "[3, 3]"

    PARX_change_dims("TestMatrix", entered_value, entered_value);
    DB_MSG(("Changed dimensions of TestMatrix to [%d, %d]", entered_value, entered_value)); // prints "[10, 10]" if user entered 10
}
@endcode
Please note, that @c UT_RelContext_ParOrigDim() needs to be called before
@c PARX_change_dims(), otherwise the original dimension is
lost.

@param[in] getPrimRelCntxt If true, the primary relation context will be used,
otherwise the secondary relation context will be used.

@return The parameter dimensions prior to its modification as a vector of
ints. Is empty when the parameter is not an array.

@throws PvStdException when no parameter was modified

@ingroup PVM_UT
*/
DLL_INTERFACE std::vector<int> UT_RelContext_ParOrigDim(bool getPrimRelCntxt = true);



/**
  Returns the array indices of a modified parameter.

  When dealing with complex parameters it is sometimes necessary to determine
  the exact element of the parameter that was modified by the user.
  To do this for arbitrary complicated parameters with e.g. multiple array
  dimensions and/or multiple struct levels we need a way to determine the array
  indices and the struct names of the modified parameter element.
  The meaning of the argument @c getPrimRelCntxt is explained in UT_RelContext_ParName().

  As an abstract
  example, consider a parameter <c>DummyPar</c> with the following structure:<br>
  <c>DummyPar[n].stuct_level_1.struct_level_2[m].struct_level_3</c><br>
  It has two array dimensions [n] and [m]
  and three struct levels: stuct_level_1, stuct_level_2 and stuct_level_3.

  UT_RelContext_ParArrayIndices() function gets the array indices of a modified parameter as a
  <c>std::vector</c> of <c>int</c>s, i.e. <c>n</c> and <c>m</c> in the abstract
  example above.

  To get the names of the struct levels see <c>UT_RelContext_ParStructNames()</c>.

  Please note:
  - Calling this function only makes sense after you know which parameter
  was modified. i.e. it's strongly recommended to use the result of
  UT_RelContext_ParName() and an if-statement first before calling it.
  - The function returns an empty vector in case no value was modified.
  Therefore, it's recommended to always check for non-emptyness of the
  resulting vector before trying to access its elements.
  - Array indices start with 0 NOT 1.

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
  Type definitions:
  @code
  struct StructTypeA {
      int int_field;
      double double_field;
  };

  struct StructTypeB {
      YesNo bool_field;
      char name_field[100];
  };

  struct StructTypeC {
      StructTypeA substructA[3];
      StructTypeB substructB[3];
  };
  @endcode

  Parameter definitions:
  @code
  int parameter MyTestArr[3][3];
  StructTypeC parameter MyTestStructArr[3];
  @endcode

  Lets say the user changed <c>MyTestArr[2][0]</c> in the method. Within the
  method code we can retrieve the array indices:
  @code
  std::string parname = UT_RelContext_ParName();

  if (parname == "MyTestArr")
  {
      std::vector<int> array_indices = UT_RelContext_ParArrayIndices();
      if (!array_indices.empty()) {
          DB_MSG(("MyTestArr[%d][%d] was changed.", array_indices[0], array_indices[1]));
          // prints: "MyTestArr[2][0] was changed."
      }
  }
  @endcode

  The second parameter is a bit more complex. It's an array of structs,
  where the struct elements of the array are arrays of structs themselves, thus
  it also has two array elements.<br>
  So let's assume the user changed the following struct field of the parameter
  in the method interface:<br>
  <c>MyTestStructArr[0].substructB[1].name_field</c><br>
  Again, we can retrieve the array indices:
  @code
  if (parname == "MyTestStructArr")
  {
      std::vector<int> array_indices = UT_RelContext_ParArrayIndices();
      if (!array_indices.empty()) {
          DB_MSG(("MyTestStructArr[%d].???[%d].??? was changed.", array_indices[0], array_indices[1]));
          // prints: "MyTestStructArr[0].???[1].??? was changed."
      }
  }
  @endcode
  You can combine the above with the result from <c>UT_RelContext_ParStructNames()</c>
  to fully determine which element of an arbitrarily complex parameter was modfied.

  @param[in] getPrimRelCntxt If true, the primary relation context will be used,
  otherwise the secondary relation context will be used.

  @return A vector of ints specifying the array indices corresponding to the
  modified parameter.

  @throws PvStdException when no parameter was modified

  @ingroup PVM_UT
*/
DLL_INTERFACE std::vector<int> UT_RelContext_ParArrayIndices(bool getPrimRelCntxt = true);



/**
  Returns the struct names of a modified parameter.

  When dealing with complex parameters it is sometimes necessary to determine
  the exact element of the parameter that was modified by the user.
  To do this for arbitrary complicated parameters with e.g. multiple array
  dimensions and/or multiple struct levels we need a way to determine the array
  indices and the struct names of the modified parameter element.
  The meaning of the argument @c getPrimRelCntxt is explained in UT_RelContext_ParName().

  As an abstract
  example, consider a parameter <c>DummyPar</c> with the following structure:<br>
  <c>DummyPar[n].stuct_level_1.struct_level_2[m].struct_level_3</c><br>
  It has two array dimensions [n] and [m]
  and three struct levels: stuct_level_1, stuct_level_2 and stuct_level_3.

  UT_RelContext_ParStructNames function gets the names of the struct
  levels of a modified parameter as a
  <c>std::vector</c> of <c>std::string</c>s, i.e. the names
  stuct_level_1, stuct_level_2 and stuct_level_3
  in the example above.<br>
  To get the array indices see <c>UT_RelContext_ParArrayIndices()</c>.

  Please note:
  - Calling this function only makes sense after you know which parameter
  was modified. i.e. it's strongly recommended to use the result of
  UT_RelContext_ParName() and an if-statement first before calling it.
  - The function returns an empty vector in case no value was modified.
  Therefore, it's recommended to always check for non-emptyness of the
  resulting vector before trying to access its elements.
  - Array indices start with 0 NOT 1.

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
  Type definitions:
  @code
  struct StructTypeA {
      int int_field;
      double double_field;
  };

  struct StructTypeB {
      YesNo bool_field;
      char name_field[100];
  };

  struct StructTypeC {
      StructTypeA substructA[3];
      StructTypeB substructB[3];
  };
  @endcode

  Parameter definitions:
  @code
  StructTypeA parameter MyTestStruct;
  StructTypeC parameter MyTestStructArr[3];
  @endcode

  Lets say the user changed <c>MyTestStruct.int_field</c> in the method. Within the
  method code we can retrieve the name of the modified struct field:
  @code
  std::string parname = UT_RelContext_ParName();

  if (parname == "MyTestStruct")
  {
      std::vector<std::string> struct_elem = UT_RelContext_ParStructNames();
      if (!struct_elem.empty()) {
          DB_MSG(("MyTestStruct.%s was changed.", struct_elem[0].c_str()));
          // prints: "MyTestStruct.int_field was changed."
      }
  }
  @endcode

  The second parameter is a bit more complex. It's an array of structs,
  where the struct elements of the array are arrays of structs themselves, thus
  it has two struct levels.<br>
  So let's assume the user changed the following struct field of the parameter
  in the method interface:<br>
  <c>MyTestStructArr[0].substructB[1].name_field</c><br>
  Again, we can retrieve the names of the struct levels as follows:
  @code
  if (parname == "MyTestStructArr")
  {
      std::vector<std::string> struct_elem = UT_RelContext_ParStructNames();
      if (!struct_elem.empty()) {
          DB_MSG(("MyTestStructArr[?].%s.[?].%s was changed.", struct_elem[0].c_str(), struct_elem[1].c_str()));
          // prints: "MyTestStructArr[?].substructB[?].name_field was changed."
      }
  }
  @endcode
  You can combine the above with the result from <c>UT_RelContext_ParArrayIndices()</c>
  to fully determine which element of an arbitrarily complex parameter was modfied.

  @param[in] getPrimRelCntxt If true, the primary relation context will be used,
  otherwise the secondary relation context will be used.

  @return A vector of std::strings specifying the names of the struct_levels of the
  modified parameter.

  @throws PvStdException when no parameter was modified

  @ingroup PVM_UT
*/
DLL_INTERFACE std::vector<std::string> UT_RelContext_ParStructNames(bool getPrimRelCntxt = true);

/** Calculates mean of double array.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a vec is a pointer to a double array
  - @a size is > 0


  @PVM_ERROR
  If the preconditions are not met.

  @PVM_EXAMPLE
  @code
  // call for a 2D double parameter

  double myarr[4]={ 1,2,3,4 };

  double mean,sd,adev,min,max;
  mean= UT_MeanOfDoubleArray(myarr,2,&sum,&sd,&adev,&min,&max);
  // other possible calls: calculate sum of elements only
  UT_MeanOfDoubleArray(myarr,2,&sum);

  // calculates standard deviation only
  UT_MeanOfDoubleArray(myarr,2,0,&sd);

  // calculates min and max array element only
  UT_MeanOfDoubleArray(myarr,2,0,0,0,&min,&max);


  @endcode

  @param[in] vec pointer to double array
  @param[in] size number of array elements
  @param      sum   (optional) set to the sum of all elements
  @param      sdev  (optional) set to the standard deviation of the mean
  @param      adev  (optional) set to the absolute deviation of the mean
  @param      min   (optional) set to the minimum of all elements
  @param      max   (optional) set to the maximum of all elements
  @return mean of double array

  @ingroup PVM_UT
*/
DLL_INTERFACE double UT_MeanOfDoubleArray(double *vec,
                                          int size,
                                          double *const sum=0,
                                          double *const sdev=0,
                                          double *const adev=0,
                                          double *const min=0,
                                          double *const max=0);

/** Calculates mean of double array parameter.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  @ref UT_MeanOfDoubleArray

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a parn (possibly qualified) must be a parameter name of a 1D double array.
  - number of elements of @a parn must be > 0
  - preconditions of @ref UT_MeanOfDoubleArray

  @PVM_ERROR
  If the preconditions are not met.

  @PVM_EXAMPLE
  @code
  // call for a 2D double parameter

    double mean,sd,adev,min,max;
    YesNo hasval=No

    hasval= UT_MeanOfDoublePar("NormArea[0]",&siz,&mean,&sum,&sd,&adev,&min,&max);
    // possible calls (assuming MyArrayPar is a 1D double array):
    hasval= UT_MeanOfDoublePar("MyArrayPar",&siz);
    UT_MeanOfDoublePar("MyArrayPar,0,&mean);

    // this will throw an error conditition if MyArrayPar is not of correct type
    hasval= UT_MeanOfDoublePar("MyArrayPar");

  @endcode

  @param[in]  parn  Name of a double array parameter
  @param[out] size  (optional) will be set to the number of elements
  @param      mean  (optional) set to the mean of all elements
  @param      sum   (optional) set to the sum of all elements
  @param      sdev  (optional) set to the standard deviation of the mean
  @param      adev  (optional) set to the absolute deviation of the mean
  @param      min   (optional) set to the minimum of all elements
  @param      max   (optional) set to the maximum of all elements
  @return Yes if parameter @a parn has a value else No

  @see @ref UT_MeanOfDoubleArray
  @ingroup PVM_UT
*/
DLL_INTERFACE YesNo UT_MeanOfDoublePar(const char *const parn,
                                       int *const size=0,
                                       double *const mean=0,
                                       double *const sum=0,
                                       double *const sdev=0,
                                       double *const adev=0,
                                       double *const min=0,
                                       double *const max=0);


#endif
#endif


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
