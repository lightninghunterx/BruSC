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

/* part of Spiral toolbox 

  Define public types.
  
*/

#ifndef MR_SPIRAL_TYPES_H
#define MR_SPIRAL_TYPES_H

/** Spiral trajectory mode
@ingroup PVM_ENUM
 */
typedef enum
{
  SPIRAL_IN,   /**< Inwards (ending in k-space centre)      */
  SPIRAL_OUT   /**< Outwards (starting from k-space centre) */
} SpiralMode;

/** Spiral design type
@ingroup PVM_ENUM
 */
typedef enum
{
  OptimumDomains,  /**< Use design with optimum performance (faster)         */
  TwoDomains,      /**< Two-domain design (slewrate, amplitude)              */
  ThreeDomains     /**< Three-domain design (frequency, slewrate, amplitude) */
} SpiralDesign;

/** Spiral navigator mode
@ingroup PVM_ENUM
 */
typedef enum
{
  SpiralNav_None,          /**< No navigator                  */
  SpiralNav_ZeroOrder,     /**< Zero-order phase correction   */
  SpiralNav_FirstOrder     /**< First-order phase correction  */
} SpiralNavMode;

#endif
