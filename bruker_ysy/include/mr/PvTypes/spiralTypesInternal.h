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

  Define internal types.
  
*/

#ifndef MR_SPIRAL_INT_TYPES_H
#define MR_SPIRAL_INT_TYPES_H

/** Spiral design parameters
  @ingroup PVM_STRUCT
*/
typedef struct
{
  double b;                         /**< parameter for angle function in domain 0 (constant) */
  SpiralDesign design;              /**< design of spiral (two or three domains)             */
  double gradConst;                 /**< gradient scaling constant [1/rad/mm]                */
  double omega;                     /**< constant with constraint for domain 0 [rad]         */
  double beta;                      /**< constant with constraint for domain 1 [rad/ms^2]    */
  double kappa;                     /**< constant with constraint for domain 2 [rad/ms]      */
  double tTrans1;                   /**< transition time domain 0 -> domain 1 [ms]           */
  double tTrans2;                   /**< transition time domain 1 -> domain 2 [ms]           */
  double angleTrans1;               /**< angle at transition point 1 [rad]                   */
  double angleTrans2;               /**< angle at transition point 2 [rad]                   */
  double dt;                        /**< gradient dwell time [ms]                            */
  int    n;                         /**< # spiral gradient points                            */
} SpiralDesignPars;

/** Properties of a Spiral gradient shape
  @ingroup PVM_STRUCT
*/
typedef struct
{
  double scale;                     /**< gradient scaling                                    */

  /* number of gradient points for individual spiral parts                                   */
  int    nNav;                      /**< number of points for navigator                      */
  int    nSpiral;                   /**< number of points for spiral shape                   */
  int    nDown;                     /**< number of points for ramp down                      */
  int    nEnd;                      /**< number of points after spiral                       */
  int    nFlow;                     /**< number of points for flow compensation (spiral-in)  */
  int    nExtra;                    /**< number of points for extra at end (spiral-in)       */

  /* details for k compensation */
  int    nRampComp;                 /**< compensation ramp duration                 */
  int    nTopComp;                  /**< compensation flat top duration             */
  double gxComp;                    /**< compensation X gradient amplitude [kHz/mm] */
  double gyComp;                    /**< compensation Y gradient amplitude [kHz/mm] */

  /* details for flow compensation */
  int    nRampFlow;                 /**< flow ramp duration                 */
  int    nTopFlow;                  /**< flow flat top duration             */
  double gxFlow;                    /**< flow X gradient amplitude [kHz/mm] */
  double gyFlow;                    /**< flow Y gradient amplitude [kHz/mm] */
} SpiralShapePars;

#endif
