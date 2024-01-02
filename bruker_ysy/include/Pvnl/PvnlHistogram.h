/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2015 - 2017
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVNL_HISTOGRAM_H
#define PVNL_HISTOGRAM_H

#include <vector>

#include "PvUtil/PvExcept.hh"
#include "Pvnl/DLLMacro.h"

/** @file
 *  @defgroup PvnlHistogram PvnlHistogram
 *  Provides functions for creating histograms
 */
/*@{*/

    /*--------------------------------------------------------------*
     * Definitions
     *--------------------------------------------------------------*/

    /*--------------------------------------------------------------*
     * prototype definition with doxygen documentation
     *--------------------------------------------------------------*/

    /**
     * @ingroup PvnlHistogram
     * @brief Calculate a histogram for given data.
     *
     * This function calculates a histogram of an array of input data, with a
     * given number of bins.
     *
     * The histogram can be returned as cumulative histogram, and/or normalized
     * to the largest value, so that the maximum value is equal to 1.0, also the
     * centers of the histogram bins can be returned.
     *
     * Optionally, the range of the histogram can be given. If data points
     * exceed this range, they are counted as (lower and high) outliers. if no
     * range is given, the range is determined from the minimum and maximum of
     * the input data, which is then divided equally into <code>numBins</code>
     * bins.
     *
     * The function can be called multiple times with the same histogram to add
     * more data to the histogram. In this case the number of bins, the minimum
     * and the maximum must be given and be constant for all calls, otherwise
     * the result is meaningless. Also, in this case, cumulation and
     * normalization must only be done in the very last call.
     *
     * @param numBins           (in) number of bins of the histogram
     * @param numDataPoints     (in) number of data points. Can be zero, e.g.,
     *                          if an input histogram should only be cumulated
     *                          or normalized.
     * @param data              (in) pointer to data. Can be NULL, if
     *                          <code>numDataPoints</code> is zero.
     * @param histogram         (in/out) pointer to a vector of counts in each
     *                          bin. Will be resized to match numBins. Can
     *                          already contain counts from previous calls of
     *                          PvnlHistogram. Optional parameter, default is
     *                          NULL. If NULL, no histogram is calculated.
     * @param binCenters        (out) pointer to vector of histogram bin centers.
     *                          Will be resized to match numBins. Optional
     *                          parameter, default is NULL. If NULL, bin centers
     *                          are not returned.
     * @param cumulative        (in) Return a cumulative histogram. Optional,
     *                          default is false
     * @param normalized        (in) Return a normalized histogram. Optional,
     *                          default is false.
     * @param min               (in) minimum of histogram range. Optional,
     *                          default is nan, in which case the minimum of the
     *                          range will be set to the minimum of the data.
     * @param min               (in) maximum of histogram range. Optional,
     *                          default is nan, in which case the maximum of the
     *                          range will be set to the maximum of the data.
     * @param outliersLow       (out) pointer to a counter of low outliers. The
     *                          pointee will be increased by the number of
     *                          <code>data</code> values less than <code>min</code>.
     * @param outliersHigh      (out) pointer to a counter of high outliers. The
     *                          pointee will be increased by the number of
     *                          <code>data</code> values greater than <code>max</code>.
     * @throws                  PvStdException, when the algorithm fails.
     */
    template<typename T>
    DLL_INTERFACE void PvnlHistogram(
            int numBins,
            size_t numDataPoints,
            const T* data,
            std::vector<double>* histogram = NULL,
            std::vector<double>* binCenters = NULL,
            bool cumulative = false,
            bool normalized = false,
            double min = NAN,
            double max = NAN,
            unsigned long* outliersLow = NULL,
            unsigned long* outliersHigh = NULL);

/*@}*/


#endif // PVNL_HISTOGRAM_H

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
