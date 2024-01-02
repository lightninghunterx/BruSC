/****************************************************************
 *
 * Copyright (c) 2015 - 2019
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVNL_THRESHOLDING_H
#define PVNL_THRESHOLDING_H

#include <vector>
#include "Pvnl/DLLMacro.h"

/** @file
 *  @defgroup PvnlThresholding PvnlThresholding
 *  Provides functions for thresholding
 */
/*@{*/

 /*--------------------------------------------------------------*
  * Definitions
  *--------------------------------------------------------------*/

 /*--------------------------------------------------------------*
  * prototype definition with doxygen documentation
  *--------------------------------------------------------------*/

/**
 * @ingroup PvnlThresholding
 * @brief Automatic threshold computation using the Otsu algorithm.
 *
 * For details on the Otsu algorithm see: Nobuyuki
 * Otsu (1979). "A threshold selection method from gray-level histograms".
 * IEEE Trans. Sys., Man., Cyber. 9 (1): 62-66.
 *
 * @param data     (in) PvnlMatrix containing the data.
 *
 * @param nbins    (in) Number of bins that shall be used for histogram creation.
 *
 * @param thresh   (out) The given double will be replaced with the calculated
 *                 threshold.
 *
 * @return         PVNL_SUCCESS if function finished successfully.
 */
DLL_INTERFACE int PvnlOtsuThreshold(const PvnlMatrix& data, int nbins, double& thresh);



/**
 * @ingroup PvnlThresholding
 * @brief Automatic threshold computation using the Otsu algorithm.
 *
 * For details on the Otsu algorithm see: Nobuyuki
 * Otsu (1979). "A threshold selection method from gray-level histograms".
 * IEEE Trans. Sys., Man., Cyber. 9 (1): 62-66.
 *
 * @param data     (in) const pointer to the data.
 * @param ndata    (in) length of data.
 * @param nbins    (in) Number of bins that shall be used for histogram creation.
 * @param thresh   (out) will be set to the calculated threshold.
 *
 * @return         PVNL_SUCCESS if function finished successfully.
 */
template<typename T>
DLL_INTERFACE int PvnlOtsuThreshold(const T * data, size_t ndata, int nbins,
        double& thresh);

/**
 * @ingroup PvnlThresholding
 * @brief Automatic threshold computation using the Otsu algorithm.
 *
 * For details on the Otsu algorithm see: Nobuyuki
 * Otsu (1979). "A threshold selection method from gray-level histograms".
 * IEEE Trans. Sys., Man., Cyber. 9 (1): 62-66.
  *
 * @param histogram  (in) histogram of the data to be thresholded
 * @param binCenters (in) bin centers for the histogram
 * @param thresh     (out) will be set to the calculated threshold.
 * @return           PVNL_SUCCESS if function finished successfully.
 */
DLL_INTERFACE int PvnlOtsuThreshold(const std::vector<double>& histogram,
        const std::vector<double>& binCenters, double& thresh);

 /**
  * @ingroup PvnlThresholding
  * @brief Automatic threshold computation using the Li algorithm.
  *
  * For details on the Li algorithm see: Li, CH & Tam,
  * PKS (1998), "An Iterative Algorithm for Minimum Cross Entropy Thresholding",
  * Pattern Recognition Letters 18(8): 771-776
  *
  * @param data     (in) PvnlMatrix containing the data. The data in
  *                 data.dataBlock is not modified after this function returns.
  *
  * @param thresh   (out) The given double will be replaced with the calculated
  *                 threshold.
  *
  * @return         (out) PVNL_SUCCESS if function finished successfully.
  */
 DLL_INTERFACE int PvnlLiThreshold(const PvnlMatrix& data, double& thresh);



/**
  * @ingroup PvnlThresholding
  * @brief Mask creation based on given data and a given threshold.
  *
  * @param data     (in) PvnlMatrix containing the data.
  *
  * @param mask     (out) PvnlMatrix receiving the mask.
  *
  * @param thresh   (out) The mask is set to one for each entry of the data that
  *                 exceedes the given threshold.
  *
  * @return         (out) PVNL_SUCCESS if function finished successfully.
  */
 DLL_INTERFACE int PvnlCreateMask(const PvnlMatrix& data, PvnlMatrix& mask, double thresh);



 /**
  * @ingroup PvnlThresholding
  * @brief Automatic threshold computation using the Li algorithm.
  *
  * For details on the Li algorithm see: Li, CH & Tam,
  * PKS (1998), "An Iterative Algorithm for Minimum Cross Entropy Thresholding",
  * Pattern Recognition Letters 18(8): 771-776
  *
  * @param data     (in) Data as std::vector. CAUTION, data is modified
  *                 after this function returns.
  *
  * @param thresh   (out) The given double will be replaced with the calculated
  *                 threshold.
  *
  * @return         (out) PVNL_SUCCESS if function finished successfully.
  */
 template<typename T>
 DLL_INTERFACE int PvnlLiThreshold(std::vector<T>& data, double& thresh);

 
/*@}*/


#endif // PVNL_THRESHOLDING_H

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
