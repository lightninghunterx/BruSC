/****************************************************************
 *
 * Copyright (c) 2006 - 2019
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVNL_UNWRAP_H
#define PVNL_UNWRAP_H

#include "Pvnl/PvnlGeneral.h"
#include "Pvnl/PvnlComplex.h"
#include "Pvnl/PvnlMatrix.h"
#include "Pvnl/DLLMacro.h"

/** @file
 *  @defgroup PvnlUnwrap PvnlUnwrap
 *  Unwrapping of phase difference maps.
 */
//@{


/*--------------------------------------------------------------*
 * Definitions
 *--------------------------------------------------------------*/

/*--------------------------------------------------------------*
 * prototype definition with doxygen documentation
 *--------------------------------------------------------------*/


/** Unwraps phase difference map in several iterations treating noisy regions last. Requires 3-dimensional data. For 2D data create 3D matrix and set size of 3rd dimension to 1. (Reference: R. Cusack and N. Papadakis, New Robust 3-D Phase Unwrapping Algorithms: Application to Magnetic Field Mapping and Undistorting Echoplanar Images, NeuroImage 16, 754-764 (2002))
 *  @param mPhaseMap Pointer to phase difference map to be unwrapped.
 *  @param mSignalMap Pointer to signal map, defining which voxels the algorithm processes. CAUTION: This matrix is changed. If pointer is NULL all voxels are processed.
 *  @param seedPointX Start flooding from this voxel.
 *  @param seedPointY Start flooding from this voxel.
 *  @param seedPointZ Start flooding from this voxel. Set all three seed point values to 0 to trigger automatic search for seed point.
 *  @param phaseWrap Defines phase wrap to be unwrapped. Typically it is set to PI (for double precision use M_PI defined in math.h). Then all adjacent voxels having an absolute phase difference greater than PI are going to be unwrapped.
 *  @param noiseBlur Kernel size for blurring estimated noise map.
 *  @param thresIter Number of noise threshold iterations.
 *  @param thresMax	Iterate up to maximum noise level. thresMax is an absolute value and depends on noiseBlur: if you increase noiseBlur you should also increase thresMax. For example the noise map values after blurring are in range 0..138 for noiseBlur = 3. So in this case you should choose thresMax between 0 and 138. Choosing a value above 138 disables this functionality for noiseBlur = 3.
 *  @param signalRatio Return warning if resulting phase map has less processed voxels. This parameter is in range 0..1. For example if you set signalRatio to 0.2 you will receive the return value PVNL_E_WARN if only 20% of the total number of voxels was processed by the unwrapping algorithm.
 *  @return error code.
 */
DLL_INTERFACE int PvnlUnwrapUnwrap (PvnlMatrix& mPhaseMap,
	PvnlMatrix& mSignalMap,
   	unsigned int seedPointX,
   	unsigned int seedPointY,
   	unsigned int seedPointZ,
   	const PVNL_DOUBLE phaseWrap,
   	const unsigned int noiseBlur,
   	const unsigned int thresIter,
	PVNL_DOUBLE thresMax,
   	const PVNL_DOUBLE signalRatio);


/** Estimates where image has signal and where there is only background noise. Assumes there is no signal in two of the cube's corners.
 *  @param mSignalMap Pointer to resulting real char 3D matrix.
 *  @param m Pointer to double 3D matrix as input.
 *  @param snr Multipication factor which defines multiple of background noise as signal.
 *  @param cornerPart Estimate noise in cube corners up to this fraction of image size (in range <CODE>0..1</CODE>).
 *  @param signalLevel Sets an additional instensity threshold of signalLevel*effMax, where effMax is average of
10% strongest pixels. Default (when this argument is ommitted) is zero.
 *  @return error code.
 */
DLL_INTERFACE int PvnlUnwrapSignalMap (PvnlMatrix& mSignalMap,
   	PvnlMatrix& m,
   	const PVNL_DOUBLE snr,
    const PVNL_DOUBLE cornerPart,
    const PVNL_DOUBLE signalLevel = 0);


//@}

#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
