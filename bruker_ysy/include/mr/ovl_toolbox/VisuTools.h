/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1995-2017
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVOVLTOOLSMR_VISU_H
#define PVOVLTOOLSMR_VISU_H

#ifndef CPROTO

#include "mr/ovl_toolbox/DLLMacro.h"

struct VisuFrameGroupType;
struct VisuDepValsType;

/* string definitions to be used with toolbox functions */


/*==============================================================*/
/** @file
 *  @addtogroup PvOvlToolsMr ParaVision Mr overlay toolbox
 *  <p>This utility library contains general functions for:</p>
 *  - setting @b VISU parameters from within method code
 */
/*@{*/



/**
 * Get a representative transposition value.
 * If more than one transposition for a dataset exists, 0 is returned and
 * \c VisuCoreTransposition is set for every frame of the dataset.
 * @param frameCount Number of frames in the dataset.
 * @return The representative transposition value.
 */
DLL_INTERFACE int VisuDeriveGetTransposeNdx(int frameCount);

/**
 * Return is different RECO transposition values exist in a dataset.
 * @return \c Yes if different RECO transposition values exist else \c false.
 */
DLL_INTERFACE YesNo VisuDeriveHaveDiffTransposition();

/**
 * Set a spatial dimension order array.
 * The spatial dimension is set using ACQ_dim_desc.
 * @param spatDimOrder Array of 3 elements allocated by the caller. The indices
 *    of the spatial dimensions are stored in the correct order in the array.
 * @param transposition The transposition used for the creation of the spatial
 *    dimension order array.
 * @param dim The dimension of the dataset.
 */
DLL_INTERFACE void VisuDeriveCreateSpatialDimOrder(
    int * spatDimOrder, int transposition, int dim);

/**
 * Indicates if spatial dimensions exist in a dataset.
 * Uses \c ACQ_dim_desc to see if spatial dimensions exist.
 * @param dimCnt The number of all dimension elements. If 
 * <code>dimCnt == -1</code> the dimension of \c ACQ_dim_desc is used. 
 * @return \c Yes if spatial dimensions exist else \c false.
 */
DLL_INTERFACE YesNo VisuDeriveHasSpatialDimensions(int dimCnt);


/**
 * Derive a complex frame group and add it to the frame group array.
 * @param orderDesc List of frame group descriptions.
 * @param fgNdx index of complex element.
 * @return \c fgNdx if no complex element is inserted, else <code>fgNdx+1</code>.
 */
DLL_INTERFACE int VisuDeriveCreateComplexFG(VisuFrameGroupType * orderDesc, int fgNdx);
/*@}*/


#endif
#endif


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
