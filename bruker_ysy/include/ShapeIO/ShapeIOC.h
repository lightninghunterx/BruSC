/*
 *******************************************************************
 *
 * $Source: /pr/CvsTree/pr/gen/h/ShapeIO/ShapeIOC.h,v $
 *
 * Copyright (c) 1996
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: ShapeIOC.h,v 1.32 2016/04/13 11:45:17 Marina.Maurer Exp $
 *
 *******************************************************************
*/

#ifndef SHAPE_IO_C_H
#define SHAPE_IO_C_H

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif
#if defined(DLL_LINKAGE) && !defined(CPROTO)
#   ifdef ShapeIO_INTERNAL
#      define DLL_INTERFACE __declspec(dllexport)
#   else
#      define DLL_INTERFACE __declspec(dllimport)
#   endif
#else
#   define DLL_INTERFACE
#endif


/** C wrapper functions for the ShapeIO class operations to handle shaped pulses */


#ifdef __cplusplus
extern "C" {
#endif


/**
 * reads shape in specified file into internal memory
 *
 * \param fileName full path of the shape file
 *
 * \return pointer to the amplitude array of size nPoints
 * \return nPoints number of points
 * \return phasePtr pointer to the phase array of size nPoints
 * 	   if (*phasePtr == 0) no phase data is present (gradient)
 * \return minAmp minimum amplitude of the shape
 * \return maxAmp maximum amplitude of the shape
 * \return integFac integral factor of the shape
 */
extern DLL_INTERFACE const double* readShapeC (
	const char* fileName, int* nPoints, const double** phasePtr,
	double* minAmp, double* maxAmp, double* integFac);


/**
 * writes amplitude and phase data to specified file
 * if a shape is present in memory its excitation mode is taken
 *
 * or
 *
 * writes shape in internal memory to specified file
 *
 * does not change the shape held in memory
 *
 * \param fileName full path of the shape file
 * \param dataPtr pointer to the 2*nPoints array of amplitude and phase values
 * 	  or nPoints array of amplitude values (gradient)
 * 	  if (dataPtr==0) shape held in memory is written to disc
 * \param nPoints number of points
 * \param phaseData 0 - no phase data (gradient), !0 - phase data
 * \param bwFac bandwidth factor of the shape
 * \param shapeMode number of offset frequencies the shape is modulated
 *
 * \return 0 if operation failed, 1 if it succeeded
 */
extern DLL_INTERFACE int writeShapeC (
	const char* fileName, const double* dataPtr, int nPoints,
	int phaseData, double bwFac, int shapeMode);


/**
 * generates the specified shape into internal memory
 *
 * \param shapeName name of the shape to generate
 * \param params parameters of the shape
 * \param phasedata 0 - no phase data (gradient), !0 - phase data
 *
 * \return pointer to the amplitude array of size 2*nPoints or
 * 	   nPoints (gradient)
 */
extern DLL_INTERFACE const double* generateShapeC (
	const char* shapeName, const char* params, int phaseData);


/**
 * analyses the shape in specified file
 *
 * or
 *
 * analyses the shape held in memory
 *
 * does not change the shape held in memory
 *
 * \param fileName name of the shape file
 * 	  if (fileName==0) shape held in memory is analyzed
 * \param command analyze operation
 * \param params parameters for the operation
 *
 * \return number of result parameters
 * \return result results of the calculations
 */
extern DLL_INTERFACE int analyzeShapeC (
	const char* fileName, const char* command, const char* params,
	char* result);


/**
 * reads and manipulates the shape in specified file
 *
 * or
 *
 * manipulates the shape held in memory
 *
 * \param fileName name of the shape file
 * 	  if (fileName==0) shape held in memory is manipulated
 * \param command manipulation operation
 * \param params parameters for the operation
 * \param freqList parameters for the operation
 *        if (freqList==0) no freqList is used.
 *
 * \return pointer to the amplitude array of size nPoints
 * \return phasePtr pointer to the phase array of size nPoints
 * 	   if (*phasePtr == 0) no phase data is present (gradient)
 * \return nPoints number of points
 * \return shapeMode number of offset frequencies the shape is modulated with
 */
extern DLL_INTERFACE const double* manipulateShapeC (
	const char* fileName, const char* command, const char* params,
	const char* freqList, const double** phasePtr, int* nPoints,
	int* shapeMode);


/**
 * adds 2 shapes and writes the result to disc
 * leaving shape held in memory unchanged
 *
 * or
 *
 * adds 2 shapes and stores result into memory
 *
 * \param inputFile1 name of the 1st shape file
 * \param inputFile2 name of the 2nd shape file
 * \param outputFile name of the output file
 * 	  if (outputFile==0) result is put into memory
 *
 * \return 0 in case of success
 */
extern DLL_INTERFACE int addShapesC (
	char* inputFile1, char* inputFile2, char* outputFile);


/**
 * adds several shapes and writes the result to disc
 * leaving shape held in memory unchanged
 *
 * or
 *
 * adds several shapes and stores result into memory
 *
 * \param inputFiles space or comma separated file names
 * \param outputFile name of the output file
 *	if (outputFile==0) result is put into memory
 * \param scaleF array of size numberOfFiles with scale factors
 * \param alignment 0, 1, 2 - left, center, right alignment

 *
 * \return 0 in case of success
 */
extern DLL_INTERFACE int addShapesMultC (
	const char* inputFiles, const char* outputFile,
	double* scaleF, int alignment);


/**
 * merges 2 shapes and writes the result to disc
 * leaving shape held in memory unchanged
 *
 * or
 *
 * merges 2 shapes and stores result into memory
 *
 * \param inputFile1 name of the 1st shape file
 * \param inputFile2 name of the 2nd shape file
 * \param outputFile name of the output file
 * 	  if (outputFile==0) result is put into memory
 *
 * \return 0 in case of success
 */
extern DLL_INTERFACE int mergeShapesC (
	char* inputFile1, char* inputFile2, char* outputFile);


/**
 * merges several shapes and writes the result to disc
 * leaving shape held in memory unchanged
 *
 * or
 *
 * merges several shapes and stores result into memory
 *
 * \param inputFiles space or comma separated file names
 * \param outputFile name of the output file
 * 	  if (outputFile==0) result is put into memory
 *
 * \return 0 in case of success
 */
extern DLL_INTERFACE int mergeShapesMultC (
	const char* inputFiles, char* outputFile);


/**
 * returns the integral factor of the shape in specified file
 *
 * or
 *
 * returns the integral factor of the shape held in memory
 *
 * does not change the shape held in memory
 *
 * \param fileName name of the shape file
 * 	  if (fileName==0) shape held in memory is analyzed
 *
 * \return integral factor of the shape
 */
extern DLL_INTERFACE double getIntegFacC (
	const char* fileName);


/**
 * clears the shape from internal memory if present
 */
extern DLL_INTERFACE void deleteShapeC ();


/**
 * Depricated! Use readShapeC instead !
 *
 * reads ASCII formated shape file
 * removes any shapes from memory and keeps the read shape in memory
 *
 * \param fileName full path of the shape file
 * \param exMode excitation mode of the shape
 * \param shapeMode number of offset frequencies the shape is modulated with
 *
 * \return pointer to the amplitude array of size 2*nPoints or
 * 	   nPoints (gradient)
 * \return nPoints number of points
 * \return phasedata 0 - no phase data (gradient), !0 - phase data
 */
extern DLL_INTERFACE const double* readShapeASCIIC (
	const char* fileName, int* nPoints, int* phaseData,
	int exMode, int mode);


#ifdef __cplusplus
}
#endif

#endif /* SHAPE_IO_C_H */
