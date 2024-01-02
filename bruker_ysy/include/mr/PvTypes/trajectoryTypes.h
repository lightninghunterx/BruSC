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

#ifndef MR_TRAJECTORY_TYPES_H
#define MR_TRAJECTORY_TYPES_H

/* part of Trajectory toolbox

  Define types.

*/

/** Name of the trajectory adjustment.
    @ingroup PVM_DEFINE
*/
#define TRAJ_ADJ_NAME "Trajectory"

/** Trajectory interleave mode.
    Determines how many interleaves are measured.
    @ingroup PVM_ENUM
*/
typedef enum
{
  Traj_IntAll,            /**< All interleaves */
  Traj_IntReduced         /**< Reduced number of interleaves */
} TrajInterleaveMode;

/** Trajectory adjustment mode.
    Determines when trajectory is measured.
    @ingroup PVM_ENUM
*/
typedef enum
{
  Traj_Adj_Auto,          /**< Automatic (if required) */
  Traj_Adj_Force,         /**< Force (perform always) */
  Traj_Adj_Skip           /**< Skip (do not perform) */
} TrajAdjMode;

/** Trajectory reconstruction mode.
    Determines trajectory used for reconstruction.
    @ingroup PVM_ENUM
*/
typedef enum
{
  Traj_Measured,          /**< Measured trajectory */
  Traj_Theoretical        /**< Theoretical trajectory */
} TrajRecoMode;

/** Trajectory adjustment status.
    Status of the last trajectory adjustment.
    @ingroup PVM_ENUM
*/
typedef enum
{
  Traj_StatusNone,        /**< Adjustment was not performed */
  Traj_StatusRunning,     /**< Adjustment is running */
  Traj_StatusSuccessful,  /**< Adjustment was successful */
  Traj_StatusRepaired,    /**< Adjustment result was repaired after failure */
  Traj_StatusFailed       /**< Adjustment failed */
} TrajStatus;

/** Trajectory display mode.
    Determines type of data to be displayed (values depend on settings).
    @ingroup PVM_ENUM
*/
typedef dynenum TrajDisplayMode;

#endif
