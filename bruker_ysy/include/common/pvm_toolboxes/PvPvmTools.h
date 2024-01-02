/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1995-2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVPVMTOOLS_H
#define PVPVMTOOLS_H

#ifndef CPROTO

#include <vector>
#include "PvAdj/adjManagerTypes.h"
#include "pvidl.h"

#ifdef DLL_INTERFACE
#undef DLL_INTERFACE
#endif

/// Hidden interface definition
#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#ifdef PvPvmTools_INTERNAL
#define DLL_INTERFACE __declspec(dllexport)
#else
#define DLL_INTERFACE __declspec(dllimport)
#endif
#elif defined (__GNUC__) && __GNUC__ >= 4
#define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#define DLL_INTERFACE
#endif


/* gen/src/prg/pvm_toolboxes/PvPvmTools/dimensions.c */

/** Initializes or reinitializes @c PVM_SpatDimEnum.

  This is according to the recipe described for @ref PTB_InitSpatDim.

  @PVM_PARS
  The following arrays are redimensioned to current number of dimensions:
  - @c PVM_Fov
  - @c PVM_FovCm
  - @c PVM_MinFov
  - @c PVM_AntiAlias
  - @c PVM_MaxAntiAlias
  - @c PVM_SpatResol
  - @c PVM_Matrix
  - @c PVM_MinMatrix
  - @c PVM_MaxMatrix

  @PVM_TBCALL
  - @ref PTB_InitSpatDim

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  PTB_SetImagingDims(2, 3);
  @endcode

  @param lowestDim     Lowest dimension to which a dimension can be set
  @param highestDim    Highest dimension to which a dimension can be set

  @return Current number of dimensions

  @see @ref PTB_InitSpatDim, @ref PTB_SetSpatDim, @ref PTB_GetSpatDim,
       @ref MRT_SetSpectrocopyDims, @ref MRT_InitSpecDim, @ref MRT_SetSpecDim, @ref MRT_GetSpecDim

  @todo @n
        Check which of the related Set/Init functions are actually required.

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_SetImagingDims(int lowestDim,
    int highestDim);


/** Initializes or reinitializes @c PVM_SpatDimEnum for dimensions lying between @a lowestDim
    and @a highestDim.

  If @a lowestDim is equal to @a highestDim @c PVM_SpatDimEnum is made non-editable. If
  @c PVM_SpatDimEnum has the state <c> NO VALUE </c> it is set to the lowest applicable
  dimension, otherwise its value is confined to lie within the initialized range.

  Note that @a lowestDim and @a highestDim are constrained to lie within the range @c 0 to @c 3.

  @PVM_PARS
  - @c PVM_SpatDimEnum

  @PVM_TBCALL
  - @ref PTB_GetSpatDim
  - @ref PTB_SetSpatDim

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  PTB_InitSpatDim(2, 3);
  @endcode

  @param lowestDim     Lowest dimension to which a dimension can be set
  @param highestDim    Highest dimension to which a dimension can be set

  @return Current number of dimensions

  @see @ref PTB_SetImagingDims, @ref PTB_SetSpatDim, @ref PTB_GetSpatDim,
       @ref MRT_SetSpectrocopyDims, @ref MRT_InitSpecDim, @ref MRT_SetSpecDim, @ref MRT_GetSpecDim

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_InitSpatDim(int lowestDim,
    int highestDim);

/** Sets the parameter @c PVM_SpatDimEnum to @c dimValue.

  If @c dimValue lies outside the legal range it will be set according to the recipe described
  for the function @ref UT_SetInLimits.

  @PVM_PARS
  - @c PVM_SpatDimEnum

  @PVM_TBCALL
  - @ref PTB_GetSpatDim

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  Legal dimension range must have been set with @ref PTB_InitSpatDim.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  PTB_SetSpatDim(2);
  @endcode

  @param dimensions    Number of dimensions

  @return Dimension number that was set

  @see @ref PTB_SetImagingDims, @ref PTB_InitSpatDim, @ref PTB_GetSpatDim,
       @ref MRT_SetSpectrocopyDims, @ref MRT_InitSpecDim, @ref MRT_SetSpecDim, @ref MRT_GetSpecDim

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_SetSpatDim(int dimensions);

/** Retrieves the current value of @c PVM_SpatDimEnum as an int.

  @PVM_PARS
  NONE

  @PVM_RELCALL
  NONE

  @PVM_TBCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  int dim = PTB_GetSpatDim();
  @endcode

  @return Number of dimensions

  @see @ref PTB_SetImagingDims, @ref PTB_InitSpatDim, @ref PTB_SetSpatDim,
       @ref MRT_SetSpectrocopyDims, @ref MRT_InitSpecDim, @ref MRT_SetSpecDim, @ref MRT_GetSpecDim

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_GetSpatDim(void);



/** Maximum possible size of a PVM array parameter per dimension.

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

  @return Maximum number of elements per dimension

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_MaxArraySize(void);




/* gen/src/prg/pvm_toolboxes/PvPvmTools/DynFileEnum.c */

/** Initialise dynamic enum with sub-directory names.

  @PVM_PARS
  - Parameter given by @a enumParName

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
  int size = PTB_InitDynEnumWithSubdirNames("SystemStatusEnum",
                                            "/opt/PV7/conf/instr/Datastation/preemp");
  @endcode

  @param[in] enumParName   Parameter name of enum
  @param[in] pathBaseName  Path where subdirectories are located

  @return Number of entries found

  @see @ref PTB_InitDynEnumWithRegFileNames

  @ingroup PVM_PTB_INT
 */
DLL_INTERFACE int PTB_InitDynEnumWithSubdirNames(const char *const enumParName,
                                                 const char *const pathBaseName);




/** Initialise dynamic enum with file names.

  @PVM_PARS
  - Parameter given by @a enumParName

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
  int size = PTB_InitDynEnumWithRegFileNames("PVM_PreemphasisFileEnum",
                                             "/opt/PV7/conf/instr/Datastation/preemp/BFR_BGA12S2_0000");
  @endcode

  @param[in] enumParName   Parameter name of enum
  @param[in] pathBaseName  Path where files are located

  @return Number of entries found

  @see @ref PTB_InitDynEnumWithSubdirNames

  @ingroup PVM_PTB_INT
 */
DLL_INTERFACE int PTB_InitDynEnumWithRegFileNames(const char *const enumParName,
                                                  const char *const pathBaseName);




/* gen/src/prg/pvm_toolboxes/PvPvmTools/PtbAdj.c */

#define RG_ADJNAME "RCVR_Standard"

/** Appends adjustment.

  This function should be called in methods that support additional adjustment
  procedures that should be executed automatically (@a type = @c per_scan) or
  on demand (@a type = @c on_demand). A call of this function makes the adjustment
  available in the adjustment platform. Subsequent calls of this function define
  the order of @c per_scan adjustments as specified by argument @a type. Any other
  type will have no effect on the adjustment lists.

  @PVM_H1{Implementation}
  If the order and amount of @c per_scan adjustments should be controlled on method
  level, follow these steps:
  - Call @ref PTB_ClearAdjustments
  - Call @ref PTB_AppendAdjustment with arguments dependent on available method
    specific adjustments
  - Use @ref PTB_AppendConfiguredAdjustment if adjustments that have been configured
    should be used

  @PVM_PARS
  - Internal parameters of the adjustment manager

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
  // clears the per_scan adjustment list
  PTB_ClearAdjustments();

  // appends trajectory adjustment at end of list
  AdjPerformListType type=on_demand;
  if( (PVM_TrajAdjMode == Traj_Per_Scan ||
      (PVM_TrajAdjMode == Traj_If_Required && PVM_TrajUpToDate == No))
  {
     type=per_scan;
  }

  PTB_AppendAdjustment("Trajectory",
                       "Trajectory Measurement",
                       "Measurement of the k-space trajectory",
                       type);

  // appends receiver gain adjustment at end of list
  PTB_AppendConfiguredAdjustment(per_scan,RCVR);
  @endcode

  @param name             Adjustment name specified as string used to identify
                          the adjustment
                          during execution
  @param displayName      Name that is shown inside the adjustment platform
  @param tooltip          Short description of the adjustment
  @param type             Must be either @c per_scan or @c on_demand

  @param protocol         Optional argument specifying the adjustment protocol name
                          (an empty string will be treated as method specific
                          adjustment)
  @param methodContext    Optional argument specifying a special context that may be
                          used as key to restore adjusted parameters or to control
                          the behavior of the adjustment at execution time.
 * @param isVisible       Indicates if the parameter is shown in the
 *                        adjustment platform and instruction list.
 *                        The parameter is only used if \a type if \c true
 *                        since \c on_demand and \c per_study adjustments
 *                        cannot be invisible.

  @return Possible return values are
          - @c 0 on success
          - Non-zero for types other than @c per_scan or @c on_demand

  @see @ref PTB_AppendConfiguredAdjustment, @ref PTB_ClearAdjustments

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_AppendAdjustment(const char *const name,
                                       const char *const displayName,
                                       const char *const tooltip,
                                       AdjPerformListType type,
                                       const char *const protocol = 0,
                                       const char *const methodContext = 0,
                                       bool isVisible = true);


DLL_INTERFACE void PTB_AppendOnDemandAdjustment(const char *const name,
        const char *const displayName,
        const char *const tooltip,
        AdjResultType result_type,
        const char *const protocol=0,
        const char *const methodContext=0,
        bool isVisible=true);

/**
 * Appends configured adjustment from the given order list (scan or study).
 * This adjustment is called if all adjustment of a given type (scan or study)
 * and a given category should be appended to the p
 * @param srcType Configured adjustment is retrieved from the given order list (scan, study).
 * @param destType Adjustment is put into the method specific per_scan or on_demand
 *      list.
 * @param adjName The name of the adjustment.
 * @param isMethodSpecific Indicates if adjustment is method specific.
 * @return 0 on success, -1 on error.
 */
DLL_INTERFACE int PTB_AppendOrderAdjustment(AdjPerformListType srctype,
                                            AdjPerformListType destType,
                                            const char * adjName);




/** Appends configured adjustment.

  This function should be called if adjustments that belong to the adjustment
  configuration should be executed @c per_scan or @c on_demand. Subsequent calls
  of this function define the order of @c per_scan adjustments as specified by
  argument @a type. Any other type will have no effect on the adjustment lists.

  @PVM_H1{Implementation}
  If the order and amount of @c per_scan adjustments should be controlled on
  method level, follow these steps:
  - Call @ref PTB_ClearAdjustments
  - Call @ref PTB_AppendConfiguredAdjustment
  - Use @ref PTB_AppendAdjustment to append method specific adjustments

  @PVM_PARS
  - Internal parameters of the adjustment manager

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
  // clears the per_scan adjustment list
  PTB_ClearAdjustments();

  // appends trajectory adjustment at end of list
  AdjPerformListType type=on_demand;
  if( (PVM_TrajAdjMode == Traj_Per_Scan ||
      (PVM_TrajAdjMode == Traj_If_Required && PVM_TrajUpToDate == No))
  {
     type=per_scan;
  }

  PTB_AppendAdjustment("Trajectory",
                      "Trajectory Measurement",
                      "Measurement of the k-space trajectory",
                      type);

  // appends receiver gain adjustment at end of list
  PTB_AppendConfiguredAdjustment(per_scan,RG_ADJNAME);
  @endcode

  @param type             Must be either @c per_scan or @c on_demand
  @param name             Mandatory argument specifying the adjustmentname
                          (an empty string will be treated as method specific
                          adjustment)
  @param methodContext    Optional argument specifying a special context that
                          may be used as
                          key to restore adjusted parameters or to control the
                          behavior of the adjustment at execution time.

  @return Possible return values are
         - @c 0 on success
         - < @c 0 for types other than @c per_scan or @c on_demand
         - > @c 0 if adjustment was not found in list of configured adjustments

  @see @ref PTB_AppendAdjustment, @ref PTB_ClearAdjustments

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_AppendConfiguredAdjustment(AdjPerformListType type,
                                                 const char *const name,
                                                 const char *const methodContext = 0);




/** Clears adjustment list.

  This function should be called if the list of @c on_demand or @c per_scan
  adjustments should be cleared to be build up specifically in the method.
  Dependent on (optional) argument @c clearall (default: No) the list of @c per_scan
  adjustments is cleared completely (clearall = Yes) or it is cleared and configured
  mandatory per_scan adjustments are prepended.




  @PVM_H1{Implementation}
  If the order and amount of @c per_scan adjustments should be controlled on method
  level, follow these steps:
  - Call @ref PTB_ClearAdjustments
  - Call @ref PTB_AppendConfiguredAdjustment
  - Use @ref PTB_AppendAdjustment to append method specific adjustments

  @PVM_PARS
  - Internal parameters of the adjustment manager

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
  // clears the per_scan adjustment list
  PTB_ClearAdjustments();

  // appends trajectory adjustment at end of list
  AdjPerformListType type=on_demand;
  if( (PVM_TrajAdjMode == Traj_Per_Scan ||
      (PVM_TrajAdjMode == Traj_If_Required && PVM_TrajUpToDate == No))
  {
     type=per_scan;
  }

  PTB_AppendAdjustment("Trajectory",
                       "Trajectory Measurement",
                       "Measurement of the k-space trajectory",
                        type);

  // appends receiver gain adjustment at end of list
  PTB_AppendConfiguredAdjustment(per_scan,RCVR);
  @endcode

  @param clearall all adjustments are cleared if set to Yes (default = No, see above)
  @see @ref PTB_AppendAdjustment, @ref PTB_AppendConfiguredAdjustment

  @ingroup PVM_PTB
 */
DLL_INTERFACE void PTB_ClearAdjustments(YesNo clearall = No);





/** Registers GOP adjustment results.

  This function should be called in the redirected relation of
  @c PVM_GopAdjResultHandler if the dataset that is acquired within a GOP adjustment
  should be loaded in the database of @em ParaVision (e.g. to visualize it).

  @PVM_PARS
  - Internal parameters of the adjustment manager

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
  PTB_RegisterGopAdj();
  @endcode

  @ingroup PVM_PTB
 */
DLL_INTERFACE void PTB_RegisterGopAdj(void);




/** Delivers the name of the current subadjustment.

  This function delivers the name of the subadjustment of the adjustment for which
  the result handler relations are being called (@c SHIM, @c FREQ, @c TRANSM,
  @c RCVR, @c USERADJ1, @c USERADJ2, @c USERADJ3). It is intended to be called in
  a redirected parameter relation of @c PVM_AdjResultHandler. With help of this
  function the method is able to distinguish which subadjustment has been performed
  and to react to its result specifically.

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

  @return Currently required adjustment category

  @see @ref PTB_GetAdjCategory, @ref PTB_AdjMethSpec, @ref PTB_GetAdjParList,
       @ref PTB_GetAdjContext

  @ingroup PVM_PTB
 */
DLL_INTERFACE const char *PTB_GetAdjResultSubProcess(void);




/** Delivers the name of a running subadjustment.

  This function delivers the name of the subadjustment of the
  adjustment if called in the redirected relation of @c PVM_AdjHandler.

  With help of this function the method is able to distinguish which
  subadjustment is going to be executed. If no subadjustment is running
  an empty string is returned.


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

  @return name of the running adjustment subprocess

  @see @ref PTB_AdjMethSpec, @ref PTB_GetAdjParList,
       @ref PTB_GetAdjContext

  @ingroup PVM_PTB
*/
DLL_INTERFACE const char *PTB_GetAdjSubProcess(void);




/** Delivers the name of the current adjustment.

  This function delivers the name of the subadjustment of the adjustment for
  which the result handler relations are being called (@c SHIM, @c FREQ,
  @c TRANSM, @c RCVR, @c OTHER, @c USERADJ1, @c USERADJ2, @c USERADJ3). It is
  intended to be called in a redirected parameter relation of
  @c PVM_AdjResultHandler. With help of this function the method is able to
  distinguish which subadjustment has been performed and to react to its result
  specifically.

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

  @return Currently required adjustment category

  @see @ref PTB_GetAdjCategory, @ref PTB_AdjMethSpec, @ref PTB_GetAdjParList,
       @ref PTB_GetAdjContext

  @ingroup PVM_PTB
 */
DLL_INTERFACE const char *PTB_GetAdjResultName(void);




/** Delivers the current adjustment name.

  This function delivers the current required adjustment name. It is intended to
  be called in a  redirected parameter relation of @c PVM_AdjHandler. With help
  of this function the method is able to test whether it is responsible to perform
  the adjustment or not.

  If the method is not able to perform the desired adjustment it should call the
  relation of @c PVM_AdjHandler forcing the default behavior:
  <code>ParxRelsParRelations("PVM_AdjHandler", Yes)</code>.

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

  @return Currently required adjustment name

  @see @ref PTB_GetAdjCategory, @ref PTB_AdjMethSpec, @ref PTB_GetAdjParList,
       @ref PTB_GetAdjContext

  @ingroup PVM_PTB
 */
DLL_INTERFACE const char *PTB_GetAdjName(void);




/** Delivers information about method specific adjustments.

  This function returns @c Yes in case a method specific adjustment is required.
  It is intended to be called in a redirected parameter relation of
  @c PVM_AdjHandler. With help of this function the method is able to test whether
  it is responsible to perform the adjustment or not.

  If the method is not able to perform the desired adjustment it should call the
  relation of @c PVM_AdjHandler forcing the default behavior:
  <code>ParxRelsParRelations("PVM_AdjHandler", Yes)</code>.

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

  @return  @c Yes if a method specific adjustment is required else @c No

  @see @ref PTB_GetAdjCategory, @ref PTB_GetAdjName, @ref PTB_GetAdjParList,
       @ref PTB_GetAdjContext

  @ingroup PVM_PTB
 */
DLL_INTERFACE YesNo PTB_AdjMethSpec(void);




/** Delivers information about the adjustment protocol used.

  This function returns the name of the adjustment protocol. It is intended to
  be called in a redirected parameter relation of @c PVM_AdjHandler.

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

  @return  @c Yes if a method specific adjustment is required else @c No

  @see @ref PTB_GetAdjCategory, @ref PTB_GetAdjName, @ref PTB_GetAdjParList,
       @ref PTB_GetAdjContext

  @ingroup PVM_PTB
 */
DLL_INTERFACE const char *PTB_GetAdjProtocol(void);




/** Get method context of adjustment.

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

  @return  Method context of adjustment handler

  @see @ref PTB_GetAdjCategory, @ref PTB_GetAdjName, @ref PTB_AdjMethSpec,
       @ref PTB_GetAdjParList

  @ingroup PVM_PTB
 */
DLL_INTERFACE const char *PTB_GetAdjContext(void);




/** Writes special information about the adjustment.

  This function is used to write information about the adjustment result of
  an adjustment method. This information may be used to retrieve adjusted
  results in subsequent scans. The function is intended to be used during an
  adjustment in the redirected relation of
  @c PVM_AdjHandler.

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
  PTB_SetRunAdjContext("GLOBAL SHIM");
  @endcode

  @param contx    String containing the information that should be passed

  @return @c Yes if the string could be passed, @c No in case it was too long

  @see  @ref PTB_SetAdjContext

  @ingroup PVM_PTB
 */
DLL_INTERFACE YesNo PTB_SetRunAdjContext(const char *const contx);

/** Checks if an adjustment subprocess is running.

  This function is used to retrieve information about an adjustment subprocess that might be
  executed during an adjustment. The subprocess name must be specified by argument. In case
  @c 0 or an empty string is provided as subprocess argument the return value is @c Yes in case
  no subprocess is running.

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
  if (PTB_CheckSubProcess(0) == Yes)
  {
    // init subprocesses
    ...
  }
  else if (PTB_CheckSubProcess("Traj") == Yes)
  {
    // adjust trajectory
    ...
  }
  else if (PTB_CheckSubProcess("Rg") == Yes)
  {
    // adjust RG only
    ...
  }
  else
  {
    // unknown sub-adjustment
  }
  @endcode

  @param procnam    Name of the subprocess that should be checked. A @c 0 or empty string is
                    allowed to check whether no subprocess is running.

  @return @c Yes (@c No) if the subprocess specified by argument is (not) running

  @see  @ref PTB_InitSubProcess

  @ingroup PVM_PTB
 */
DLL_INTERFACE YesNo PTB_CheckSubProcess(const char *const procnam);

/** Initializes adjustment subprocesses.

  This function is used to initialize adjustment subprocesses within an adjustment. The
  adjustment results of consecutive subprocesses are available for all following subprocesses.
  The subprocesses are specified as names that are separated by a colon "," in string
  argument @a proclis. On success the number of subprocesses that are initialized are returned
  as int value. A value less than @c 1 indicates an error condition. Calling this function with
  @c 0 argument or an empty string will deactivate subprocesses. Specifying only one subprocess
  will be allowed but useless.

  @PVM_H2{Note}
  Each subprocess will be done in a separate parameter space, within a subprocess the method
  may change the acquisition mode, the original mode will be restored at the end of the
  adjustment subprocess chain.

  This function is intended to be used in the redirected relation of @c PVM_AdjHandler.

  @PVM_PARS
  - Internal parameters of the adjustment manager overlay

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - Relation of internal parameters of the adjustment manager overlay

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  Extend the receiver adjustment to subprocesses

  @code
  const char *const sublis="subproc1,subproc2,subproc3";

  (..)

  switch(PTB_GetAdjCategory)
  {
  case RCVR:
  if(Yes==PTB_CheckSubProcess(0))
  {
  if(3 != PTB_InitSubProcess(sublis))
  {
  ( some error handling code )
  }
  else
  {
   // initialization of subprocesses succeded, return to adjmanager
   return;
  }
  }

  if(Yes==PTB_CheckSubProcess("subproc1"))
  {
   // actions to do in first subprocess
  }
  else if(Yes==PTB_CheckSubProcess("subproc2"))
  {
   // actions to do in second subprocess
  }
  else if(Yes==PTB_CheckSubProcess("subproc3"))

   // actions to do in third subprocess
  }
  break;
  // other adjustments
  }
  @endcode

  @param proclis    Colon separated list of adjustment subprocess names

  @return Number of successfully initialized subprocesses

  @see  @ref PTB_CheckSubProcess

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_InitSubProcess(const char *const proclis);


/** Initializes adjustment subprocesses.

  This function is used to initialize adjustment subprocesses within an
  adjustment. The adjustment results of consecutive subprocesses are
  available for all following subprocesses.
  The subprocesses are specified as vector of type @a AdjSubAdjType .
  On success the number of subprocesses that are initialized are returned
  as int value. A value less than @c 1 indicates an error condition.

  @PVM_H2{Note}
  Each subprocess will be done in a separate parameter space, within a subprocess the method
  may change the acquisition mode, the original mode will be restored at the end of the
  adjustment subprocess chain.

  This function is intended to be used in the redirected relation of @c PVM_AdjHandler.

  @PVM_PARS
  - Internal parameters of the adjustment manager overlay

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - Relation of internal parameters of the adjustment manager overlay

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  Extend the receiver adjustment to subprocesses

  @code


  if(Yes==PTB_CheckSubProcess(0))
  {
    AdjSubAdjType csubadj;

    std::vector<AdjSubAdjType>  subadjlist;
    csubadj = { "Subadj1","","" };
    subadjlist.push_back(csubadj);
    csubadj = { "Subadj2","RCVR_Standard","special rg"};
    subadjlist.push_back(csubadj);
    PTB_InitSubProcess(subadjlist);
    return;
  }
  (..)

  if(Yes==PTB_CheckSubProcess("Subadj1"))
  {
   // actions to do in first subprocess
  }
  else if(Yes==PTB_CheckSubProcess("Subadj2"))
  {
   // actions to do in special receiver adjustment
  }


  @endcode

  @param adjvec vector of type @c AdjSubAdjType

  @return Number of successfully initialized subprocesses

  @see  @ref PTB_CheckSubProcess

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_InitSubProcess(std::vector<AdjSubAdjType> &adjvec);




/** Enables execution of configured adjustments inside subadjustments.

    This function is intended to be executed inside a redirected relation of
    @c PVM_AdjHandler after initialization of subadjustments using
    @ref PTB_InitSubProcess . This enables execution of adjustments by other
    methods. The list of adjustments (referenced by names initialized by
    @ref PTB_InitSubProcess ) is scanned for names starting with a string
    that matches argument @c sadjn . If found, a configured
    adjustment (i.e. existing inside the list of known adjustments of
    the currently active adjustment configuration) as specified by argument
    @c configadjn is executed as subadjustment. If set, a context
    information that can be accssible and interpreted by the configured
    adjustment to control specific aspects of the adjustment algorithm is
    provided by artument @c context.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_TBCALL
  - @ref PTB_AdjAvailable

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  Subadjustments have to be initialized with @ref PTB_InitSubProcess

  @PVM_ERROR
  - unknown adjustment name (wrong argument @c configadjn
    @ref PTB_AdjAvailable returns No)

  @PVM_EXAMPLE
  The following example shows how to start a protocol based global
  frequency adjustment inside a subadjustment list. This code must
  be executed in a redirected relation of PVM_AdjHandler. An example
  adjustment @c ThisMethodAdj sets up the subadjustment list.


  @code

  // check if the method adjustment is required

  if(!strcmp("ThisMethodAdj",PTB_GetAdjName()))
  {
    // define subadjustments
    const char *const sublis="subproc1,FreqAdj1,subproc2,Freqadj2,subproc3";

    // check if subjadjustments are initialized

    if(Yes==PTB_CheckSubProcess(0))
    {
      // subprocess initialization required
      PTB_InitSubProcess(sublis);
      // here is the right place to call PTB_ConfigSubAdj

      int retval=PTB_ConfigSubAdj("FreqAdj",
                                  "FREQ_Standard",
                                  "");

      // retval is now 2 (see sublis above)
      // return, adjustment manager gets now notification about
      // subprocesses, the next call will go into the else branch

      return;
    }
    else
    {
       if(Yes==PTB_CheckSubProcess("subproc1"))
       {
          // handle first subprocess
          (...)
       }
       if(Yes==PTB_CheckSubProcess("subproc2"))
       {
          // handle second subprocess
          (...)
       }
       if(Yes==PTB_CheckSubProcess("subproc3"))
       {
          // handle third subprocess
          (...)
       }
       // note: the FreqX subadjustments are executed
       // in a different method
    }
  } // Adjustment ThisMethodAdj and its subadjustments are handled now

  // handle other method specific adjustments
  (...)


  @endcode

  @param  sadjn  Name of the subadjustment that should execute the
                 configured adjustment
  @param  configadjn Name of the configured adjustment

  @param  context Additional information that might control the configured
                 adjustment specifically-
  @return Number of subprocesses that are handled by the configured adjustment

  @see  @ref PTB_CheckSubProcess @ref PTB_InitSubProcess

  @ingroup PVM_PTB
*/
DLL_INTERFACE int PTB_ConfigSubAdj(const char *const sadjn,
                                   const char *const configadjn,
                                   const char *const context=0);


/** Delivers information whether adjustment is started.

  This function may be used within the redirected relation of @c PVM_AdjHandler
  to determine whether an adjustment should be prepared to be started. In case
  the method should react specifically to generate the data for the adjustment
  algorithm, these specific changes should be performed for a return value @c Yes.

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

  @return @c Yes = adjustment is started, else @c No

  @ingroup PVM_PTB
 */
DLL_INTERFACE YesNo PTB_AdjustmentStarting(void);




/** Delivers reason of relation call of PVM_AdjHandler.

  This function may be used within the redirected relation of @c PVM_AdjHandler to determine
  whether its relation has been called because the adjustment platform has been opened or because
  an adjustment that is started should be initialized. In the latter case the method may react
  specifically to generate the data for the adjustment algorithm.

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
  // inside redirected relation of PVM_AdjHandler
  ADJ_HANDLER_CONTEXT mode;

  if(PTB_AdjHandlerCall(&mode))
  {
      switch(mode)
      {
        default:
        case HANDLE_ACQUISITION:
            // some code to prepare the method to the specific adjustment

            break;
        case HANDLE_PLATFORM:
           // code to prepare the result parameters visible in the result
           // parameter card
           break;
      }
  }


  @endcode

  @param  mode in case of return value 1 it is set to @c HANDLE_ACQUISITION or
          @c HANDLE_PLATFORM dependent on the reason of the @c PVM_AdjHandler relation
          call
  @return @c 1 if @c PVM_AdjHandler has a value else 0



  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_AdjHandlerCall(ADJ_HANDLER_CONTEXT *mode);




/** Retrieves information about available configured adjustments.

  This functions may be used to make a specific method feature
  dependent on a dedicated adjustment configuration.
  Its return value indicates whether an adjustment with name @em adjName
  is configured.
  An adjustment is configured if the pool of known adjustment contains
  this adjustment (specified by name).
  Additional information is given optionally by arguments @em type
  (adjustment type @c per_scan, @c per_study, @c on_demand), and flag
  @em mandatory (in case a @c per_scan adjustment is mandatory or not).

  Mandatory @c per_scan adjustments will not be cleared by
  @ref PTB_ClearAdjustments called before a method sets up its own
  @c per_scan adjustment list using @ref PTB_AppendAdjustment or
  @ref PTB_AppendConfiguredAdjustment. Mandatory registered adjustments
  are prepended (in the configured order) into the on_demand or
  per_scan adjustment list.



  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - internal errors to be reported to hotline.

  @PVM_EXAMPLE
  This example shows how to make a method feature dependent on a valid
  adjustment configuration. Only if in the per_scan list of adjustments
  a shim procedure is configured mandatory the shim requirements may be
  realized when starting a measurment with scan or continue:

  @code
  // ask for a SHIM adjustment in the per_scan adjustment list,require
  // a standard adjustment.

  AdjPerformListType adjtyp;
  YesNo isMandatory;

  YesNo available =
  PTB_AdjAvailable("SHIM_Scan",
                   &adjtyp,
                   &isMandatory);        // per_study, per_scan or on_demand


  if(!(Yes==available     &&
       per_scan == adjtyp &&
       Yes  == mandatory ))
  {
     // study shim or mapshim not possible in this case
     // deactivate shim handling
     PVM_ReqShimEnum=Current_Shim;
  }

  @endcode

  @param[in]  adjName   string (max. length ADJ_NAME_LENGTH)
  @param[out] type      type of the adjustment
  @param[out] mandatory flag indicating @c mandatory per_scan adjustments
  @return Yes in case the adjustment is configured else No.

  @ingroup PVM_PTB
*/
DLL_INTERFACE YesNo PTB_AdjAvailable(const char * const adjName,
                       AdjPerformListType *type=0,
                       YesNo *mandatory=0);


/** Sets specific method context in adjustment.

  This functions may be used to set a specific method context
  in adjustments to be used as key in 
  @ref PvAdjManRestoreParFromHwContext to restore adjustment results.
  This function has only effect if called in redirected relations of 
  PVM_AdjHandler.


  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - illegal string size

  @PVM_EXAMPLE

  @code
  const char *const metodContext="Specific Restore Feature";

  bool failed=PTB_AdjSetMethodContext(methodContext);

  if(failed)
  {
     std::ostringstream oerr;
     oerr << "Failed to set method context: " << methodContext;
     UT_ReportError(oerr.str().c_str());
  }

  @endcode

  @param[in]  methodContext  string (max. length ADJ_CONTEXT_LENGTH)
  @return true in case of error.

  @ingroup PVM_PTB
*/
DLL_INTERFACE bool PTB_AdjSetMethodContext(const char *const methodContext);


/* gen/src/prg/pvm_toolboxes/PvPvmTools/VisuUtils.c */

/** Sets the Id of an existing frame group in post processing parameters.

  CAUTION: After setting the Id, the loop changes its identity.

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
  PTB_VisuSetLoopId(PV_IDL_CONSTANT(visuid_framegroup_CYCLE), PV_IDL_CONSTANT(visuid_framegroup_CARDIAC_MOVIE));
  @endcode

  @param[in]  oldId    Existing Id of a Visu frame group
  @param[in]  newId    New identifier of the Visu frame group

  @return @c 0 if everything is ok else @c -1

@see @ref MRT_VisuSetEchoLoopId,
     @ref MRT_VisuSetMovieLoopId, @ref PTB_VisuSetLoopName, @ref MRT_VisuSetEchoLoopName,
     @ref MRT_VisuSetMovieLoopName, @ref PTB_VisuSetLoopDependentParameter,
     @ref PTB_VisuRemoveLoopDependentParameter, @ref PTB_VisuInsertLoop

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_VisuSetLoopId(const char *oldId,
    const char *newId);





/** Sets the name/comment of a frame group in post processing parameters.

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
  PTB_VisuSetLoopName(PV_IDL_CONSTANT(visuid_framegroup_IRMODE), "Inversion Mode");
  @endcode

  @param[in] id      Identification of the frame group
  @param[in] name    New name/comment of the frame group

  @return @c 0 if everything is ok else @c -1

@see @ref PTB_VisuSetLoopId
     @ref PTB_VisuSetLoopDependentParameter,
     @ref PTB_VisuRemoveLoopDependentParameter, @ref PTB_VisuInsertLoop

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_VisuSetLoopName(const char *id,
    const char *name);




/** Sets a dependent parameter for a given loop.

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
  PTB_VisuSetLoopDependentParameter(PV_IDL_CONSTANT(visuid_framegroup_MOVIE),
                                    "VisuAcqInversionTime",
                                    0);
  @endcode

  @param[in] loopId      Identifier of loop to set dependent parameter
  @param[in] parName     Name of dependent parameter
  @param     startNdx    Start ndx of dependent values

  @return @c 0 if everything is ok else @c -1

@see @ref PTB_VisuSetLoopId,
     @ref PTB_VisuSetLoopName,
     @ref PTB_VisuRemoveLoopDependentParameter, @ref PTB_VisuInsertLoop

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_VisuSetLoopDependentParameter(const char *loopId,
                                                    const char * parName,
                                                    int startNdx);



/** Removes a dependent parameter for a given loop.

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
  PTB_VisuRemoveLoopDependentParameter(PV_IDL_CONSTANT(visuid_framegroup_MOVIE),
                                       "VisuFGElemComment",
                                       Yes);
  @endcode

  @param[in] loopId          Identifier of the loop
  @param[in] parName         Name of dependent parameter to remove
  @param     adaptDepVals    Adapt the dependent parameter

  @return Start ndx in @a parName array if everything is ok else @c -1

@see @ref PTB_VisuSetLoopId, @ref PTB_VisuSetLoopName,
     @ref PTB_VisuSetLoopDependentParameter, @ref PTB_VisuInsertLoop

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_VisuRemoveLoopDependentParameter(const char * loopId,
                                                       const char * parName,
                                                       YesNo adaptDepVals);




/** Inserts a new frame group.

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
  PTB_VisuInsertLoop(PV_IDL_CONSTANT(visuid_framegroup_IRMODE),
                     PV_IDL_CONSTANT(visuid_framegroup_MOVIE),
                     FairTIR_NExp,
                     "Inversion");
  @endcode

  @param[in] beforeId    Existing group identifier of frame group before the
                         insertion @n. If empty new frame group is group @c 0
  @param[in] id          Group Id of inserted frame group
  @param     len         Number of frame group element
  @param[in] comment     Group comment of inserted frame group (maybe @c 0)

@see @ref PTB_VisuSetLoopId, @ref PTB_VisuSetLoopName,
     @ref PTB_VisuSetLoopDependentParameter,
     @ref PTB_VisuRemoveLoopDependentParameter

  @ingroup PVM_PTB
 */
DLL_INTERFACE void PTB_VisuInsertLoop(const char * beforeId,
                                      const char * id,
                                      int len,
                                      const char * comment);




/** Change number of elements of a given frame group.

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
  PTB_VisuChageFGLen(PV_IDL_CONSTANT(visuid_framegroup_DIFFUSION), PVM_DwNDiffExp);
  @endcode

  @param[in] id     Frame group Id
  @param     newLen New number of frame group element, must be greater 0.

  @return @c 0 on success, @c -1 on error

  @see @ref PTB_VisuSetFGElementComment

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_VisuChangeFGLen(const char * id, int newLen);




/** Sets comment for frame group elements

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
  char comments[80];
  memset(comments, 0, 80);
  strcpy(comments, "Selective Inversion Mode");
  strcpy(&comments[40], "Non-selective Inversion Mode");
  PTB_VisuSetFGElementComment(PV_IDL_CONSTANT(visuid_framegroup_IRMODE),
                              comments,
                              2,
                              40);
  @endcode

  @param id            Frame group Id
  @param comment       Comment array (one dimensional)
  @param commentLen    Number of comments
  @param stringLen     Maximum length of one comment

  @see @ref PTB_VisuChangeFGLen

  @ingroup PVM_PTB
 */
DLL_INTERFACE void PTB_VisuSetFGElementComment(const char * id,
                                               const char * comment,
                                               int commentLen,
                                               int stringLen);




/* gen/src/prg/pvm_toolboxes/PvPvmTools/geoObj.c */

/** Returns max length of a geometry object parameter name.

  This function returns the maximum length (including @c 0) of a parameter name used to handle
  a geometry object.

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

  @return Length of parameter name represented as int

  @see @ref PTB_MaxGeoIdLength,
       @ref PTB_MaxGeoLabelLength

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_MaxGeoParnameLength(void);




/** Returns max length of a geometry object identifier.

  This function returns the maximum length (including @c 0) of a geometry object identifier.

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

  @return Length geo object identifier string represented as int

  @see @ref PTB_MaxGeoParnameLength,
       @ref PTB_MaxGeoLabelLength

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_MaxGeoIdLength(void);




/** Returns max length of a geometry object axis lable length.

  This function returns the maximum length (including @c 0) of a geometry axis lable.

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

  @return Length geo object lable represented as int

  @see @ref PTB_MaxGeoParnameLength, @ref PTB_MaxGeoIdLength,


  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_MaxGeoLabelLength(void);



/** Registers a new geometry object.

  This function has to be used to register a geometry object that should be
  handled and displayed by the geometry editor.

  Legal values of arguments are expected and checked, possible error states
  are returned as nonzero return values. In case of illegal states, @c PVM_GeoObj
  remains unchanged and the geometry object is not handled by the geometry editor.

  For argument @a geo_order: @n
  In case of either an empty string, a @c NULL pointer, a string that does not
  specify a parameter name or that specifies a parameter that is not an @c int array,
  detailed information about the ordering of the subcuboids (i.e. slices in case of
  slice geometry) will not be displayed and changes of the order may not be done on
  geometry editor level.

  If the required geometry object is already registered with the same properties as
  specified by arguments the function returns @c 0 without any change. In case it is
  not registered, a new entry of @c PVM_GeoObj is created.

  @PVM_PARS
  - @c PVM_GeoObj
  - Parameter specified as argument @a obj_parname

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Illegal arguments (return values > @c 0):
    - Illegal string length of argument @a id or @c NULL pointer (return value @c 1)
    - Illegal string length of argument @a subid (return value @c 2)
    - Illegal @a obj_parname (No parameter name, wrong string length. This parameter
      has to be of type @c GEO_OBJECT and is mandatory (return value @c 4)
    - Illegal @a geo_parname (No parameter name, wrong string length. This parameter
      has to be of type @c GEO_CUBOID_PACK or @c GEO_PLANAR and is mandatory (return
      value @c 5)
    - Illegal @a upd_contr (No parameter name)
  - Illegal state of @c PVM_GeoObj (return values < @c 0)
    - @c PVM_GeoObj has more than one entry of name @a obj_parname (return value @c -1)
    - @c PVM_GeoObj as an entry of name @a obj_parname, but the related parameter has
      no value (return value @c -3)
    - @c PVM_GeoObj as an entry of name @a obj_parname with different geometry object
      properties as specified by arguments (return value @c -4)

  @PVM_EXAMPLE
  @code
  const char *const label[3] = {"1;first pulse", "2;second pulse", "3;third pulse"};

  PTB_InsertGeoObject("VOXEL",
                      GeoCuboidPackId,
                      Standard_GobjShape,
                      "PVM_VoxelGeoObj",
                      "PVM_VoxelGeoCub",
                      0,
                      label,
                      Yes);
  @endcode

  @param[in] id            String specifying the displayed geometry object (must not
                           contain prefix @c BRUKER_)
  @param     type          Enum specifying the object type (either @c GeoCuboidPackId
                           or @c GeoPlanarId)
  @param     shape         Enum specifying the object shape (either
                           @c Standard_GobjShape,
                           @c Cylinder_In_GobjShape, @c Cylinder_Circ_GobjShape,
                           @c Ellipsoid_In_GobjShape, or @c Ellipsoid_Circ_GobjShape)
  @param[in] obj_parname   String specifying the parameter name that is used to store
                           the geometry object properties. The display name of this
                           parameter appears in the geometry editor.
  @param[in] geo_parname   String specifying the parameter name that is used to
                           transfer the geometry from geometry editor to methods and
                           vice versa
  @param[in] geo_order     String specifying the parameter name that is used to
                           specify the order of subcuboids
  @param[in] labels        Labels used to identify each dimension in the geometry
                           editor (@c NULL = use default labels). @n
                           Each label string consists of a short and a long
                           description, separated by a semi-colon:
                           <c>\<s_desc\>;\<l_desc\></c>, where
                           - @c s_desc: short description (mandatory, max. two
                             characters)
                           - @c l_desc: long description (optional)
  @param[in] show          Default visibility in geometry editor

  @return @c 0 in case of success; other error states see above

  @see @ref PTB_RemoveGeoObject, @ref PTB_SetGeoObjShape

  @todo Check whether the referenced parameters are unique

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_InsertGeoObject(const char *const id,
                                      GeoObjType type,
                                      GeoObjShapeType shape,
                                      const char *const obj_parname,
                                      const char *const geo_parname,
                                      const char *const geo_order,
                                      const char *const labels[3] = NULL,
                                      YesNo show = No);

/** Removes a geometry object.

  This function has to be used to unregister a geomety object that is handled and
  displayed by the geometry editor.

  Legal values of arguments are expected and checked. In case of illegal states,
  @c PVM_GeoObj remains unchanged and the geometry object is not handled by the
  geometry editor.

  @PVM_PARS
  - @c PVM_GeoObj
  - Parameter specified as argument @a obj_parname

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
  PTB_RemoveGeoObject(""PVM_VoxelGeoCub");
  @endcode

  @param geo_parname    String specifying the parameter name that is used to
                        transfer the geometry from geometry editor to methods
                        and vice versa

  @see @ref PTB_InsertGeoObject, @ref PTB_SetGeoObjShape

  @ingroup PVM_PTB
 */
DLL_INTERFACE void PTB_RemoveGeoObject(const char *const geo_parname);




/** Changes the shape of a geometry object.

  This function has to be used to modify the way how a geometry object is displayed
  in the geometry editor.

  Legal values of arguments are expected and checked. In case of illegal states,
  i.e. the specified parameter name is not a geometry object, nothing is changed
  and error codes are returned (see below).

  For planar objects, the second argument is ignored and the shape is set to
  @c Standard_GobjShape.

  @PVM_PARS
  - The parameter name as specified by argument

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  For spiral imaging
  @code
  PTB_SetGeoObjShape("PVM_SliceGeoObj", Cylinder_In_GobjShape);
  @endcode
  For UTE
  @code
  PTB_SetGeoObjShape("PVM_SliceGeoObj", Ellipsoid_In_GobjShape);
  @endcode


  @param obj_parname    String specifying the parameter name that is used to store
                        the geometry object properties
  @param shape          Enum specifying the object shape with these possible values:
                        - @c Standard_GobjShape,
                        - @c Cylinder_In_GobjShape
                        - @c Cylinder_Circ_GobjShape
                        - @c Ellipsoid_In_GobjShape
                        - @c Ellipsoid_Circ_GobjShape

  @return
  - Error code @c 1 is returned in case the string argument @a obj_parname is not a
    parameter name.
  - Error code @c 2 is returned in case the string argument @a obj_parname is not of
    type @c GEO_OBJECT.
  - Error code @c 0 is returned in case of no errors

  @see @ref PTB_InsertGeoObject, @ref PTB_RemoveGeoObject

  @ingroup PVM_PTB
 */
DLL_INTERFACE int PTB_SetGeoObjShape(const char *const obj_parname,
    GeoObjShapeType shape);




/* gen/src/prg/pvm_toolboxes/PvPvmTools/ParamImport.c */

/** Inserts a transfer feature for parameter import.

  This function is used to enable the import and export of certain parameters for
  a method. It inserts a feature into the list of transferable features.

  @PVM_PARS
  - @c PVM_ExportHandler
  - @c PVM_ImportHandler

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - A feature with the Id specified as argument already exists, but context and
    description are not identical.

  @PVM_EXAMPLE
  Offer local parameter class for transfer
  @code
  PTB_InsertTransferFeature("MyTaggingParID",
                            "", // empty context
                            "Tagging Parameters");
  @endcode

  @param id             Unique identifier of the transfer feature
                        - Use internal Id for Bruker defined transfer features
  @param context        Specifies special conditions for the data transfer
                        - May be empty
  @param description    Text in the import GUI
                        - Mandatory for user defined transfer features
                        - Ignored for Bruker defined transfer features

  @return @c Yes = transfer feature was inserted @n
          @c No = transfer feature already exists

  @see @ref PTB_RemoveTransferFeature, @ref PTB_SetImportList

  @ingroup PVM_PTB
 */
DLL_INTERFACE YesNo PTB_InsertTransferFeature(const char *const id,
                                              const char *const context,
                                              const char *const description);




/** Removes a transfer feature for parameter import.

  This function is used to disable the import and export of certain parameters for
  a method. It removes a feature from the list of transferable features.

  @PVM_PARS
  - @c PVM_ExportHandler
  - @c PVM_ImportHandler

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
  PTB_RemoveTransferFeature("MyTaggingParID");
  @endcode

  @param id    Unique identifier of the transfer feature
               - Use internal Id for Bruker defined transfer features

  @return @c Yes = transfer feature was removed @n
          @c No = transfer feature was not found

  @see @ref PTB_InsertTransferFeature, @ref PTB_SetImportList

  @ingroup PVM_PTB
 */
DLL_INTERFACE YesNo PTB_RemoveTransferFeature(const char *const id);




/** Prepares transfer feature for parameter import.

  This function is used to define the parameters that are transfered with a
  certain transfer feature.

  @PVM_PARS
  - @c PVM_ImportList

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - function must be called only in (redirected) relation of
    parameter PVM_ImportID

  @PVM_ERROR
  - Illegal values in list content
  - Primary parameter relation not from parameter PVM_ImportID

  @PVM_EXAMPLE
  Prepare import of EPI parameter group
  @code

  // assuming redirection of PVM_ImportID to function MyImportHandler

  void MyImportHandler(void)
  {
     // react on special parameter import
     if(!strcmp(PVM_ImportID,"MyEpiParImportID"))
     {
        // PVM_Matrix and PVM_EffSWh are transferred (without handling)
        // then epi parameter group is transferred and handled (call
        // relation of PVM_EpiHandler)

        PARIMPORT_LIST_TYPE myIlist[] =
        {
          {"PVM_Matrix",     "PVM_Matrix", "",               No},
          {"PVM_EffSWh",     "PVM_EffSWh", "",               No},
          {"EPI_Parameters", "",           "PVM_EpiHandler", No}
         };
         PTB_SetImportList(myIlist, 3);
     }
     else
     {
       // call the default relation of PVM_ImportID
       // to handle standard parameter import
       STB_ImportID();
     }
  }
  @endcode

  @param ilist    List of parameters to be transfered with the following elements
                  per entry:
                  - Parameter name / parameter class used to retrieve parameter
                    values (mandatory)
                  - Parameter name used to store the source values in the destination
                    dataset (empty string is replaced by source name, ignored for
                    parameter class)
                  - Handler parameter for the transfer
                    (relation called in the target parameter space after the actual
                    transfer)
                  - @c No = call redirected relation, @c Yes = call default relation
  @param size     Number of entries in @a ilist

  @see @ref PTB_InsertTransferFeature, @ref PTB_RemoveTransferFeature

  @ingroup PVM_PTB
 */
DLL_INTERFACE void PTB_SetImportList(const PARIMPORT_LIST_TYPE *ilist,
    const int size);



/**
Stores parameters in such a way that other methods within the same study can
access them on demand using <c>PTB_RestoreStudyResult()</c>.

Say method A measures data which is written to a method parameter (e.g.
@c PVM_ExampleCalib) and
a subsequent method B needs that data to derive e.g. a acquisition parameter
from it. In method A one could call
<c>PTB_StoreStudyResult("PVM_ExampleCalib")</c>,
which would store the content of this parameter to a certain file within the
study folder.

Calling <c>PTB_RestoreStudyResult("PVM_ExampleCalib")</c> in method B
would restore the saved parameter, i.e. the parameter @c PVM_ExampleCalib
can be used within method B and will contain what was previously calibrated
in method A.

<b>Important additions:</b>
- This function currently only works in the MR modality
- Calling the function only "queues" the request for parameter storage. The
actual storage process (i.e. writing to disc) is performed after the
scan is completed.

@PVM_PARS
NONE

@PVM_TBCALL
NONE

@PVM_RELCALL
NONE

@PVM_PRECOND
<b>This function currently only works in the MR modality.</b>

@PVM_ERROR
NONE

@PVM_EXAMPLE
@code
std::vector<std::string> pars = {"PVM_DummyPar1", "PVM_DummyPar2"};
PTB_StoreStudyResult(pars, false);
PTB_StoreStudyResult("PVM_DummyPar3", true);
@endcode

@param[in] parnames
A vector containing the parameters names (as strings) that shall be stored.
@param[in] append
If @c true, the specified parameter is appendend to previously stored
parameters. Only a single parameter name is accepted in this case. If @c false,
the given list of names is stored and previously specified parameters
are overwritten.

@return False in case of an error and true otherwise.

@throws PvStdException if one of the parameter names does not exist.
@throws PvStdException if one tries to append more than one parameters.

@ingroup PVM_PTB
*/
DLL_INTERFACE bool PTB_StoreStudyResult(std::vector<std::string> parnames, bool append);



/**
Store a parameters in such a way that other methods within the same study can
access them on demand using <c>PTB_RestoreStudyResult()</c>.

Calls <c>PTB_StoreStudyResult(std::vector<std::string> parnames, bool append)</c>
with a single entry.

<b>This function currently only works in the MR modality.</b>

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
PTB_StoreStudyResult("PVM_DummyPar1", false);
@endcode

@param[in] parnames
The parameter name that shall be stored.
@param[in] append
If @c true, the specified parameter is appendend to previously stored
parameters, otherwise the given parameter is stored and previously specified
parameters are overwritten.

@return False in case of an error and true otherwise.

@throws PvStdException if one of the parameter names does not exist.
@throws PvStdException if one tries to append more than one parameters.

@ingroup PVM_PTB
*/
DLL_INTERFACE bool PTB_StoreStudyResult(std::string parname, bool append);



/**
Restores a parameter that was previously stored using
<c>PTB_StoreStudyResult()</c>.

See <c>PTB_StoreStudyResult(std::vector<std::string> parnames, bool append)</c>
for additional explanations.

Contrary to <c>PTB_StoreStudyResult()</c>, this function works in all
modalities.

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
PTB_RestoreStudyResult("PVM_DummyPar1");
@endcode

@param[in] parname
The parameter name that shall be restored.

@return False in case of an error and true otherwise.

@throws PvStdException if the parameter name does not exist.

@ingroup PVM_PTB
*/
DLL_INTERFACE bool PTB_RestoreStudyResult(std::string parname);



/**
  Takes a path in form of a AdjProcnoResultType and returns the full path
  to the procno as a string.

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

  @param[in] res
  The path as std::string.

  @return A string containing the full path to the procno.

  @ingroup PVM_PTB
*/
DLL_INTERFACE std::string PTB_ProcnoResultType2Path(AdjProcnoResultType& res);



/**
  Takes a procno path in form of a ProcnoResultType and returns the full path
  to the procno as a string.

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

  @param[in] res
  The path as std::string.

  @return A string containing the full path to the procno.

  @ingroup PVM_PTB
*/
DLL_INTERFACE std::string PTB_ProcnoResultType2Path(ProcnoResultType& res);



/**
  Takes the path to the procno and returns a AdjProcnoResultType containing the
  path information.

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

  @param[in] path
  The path as a char array.

  @return A AdjProcnoResultType containing the full path to the procno.

  @throws PvStdException when the path could not be parsed.

  @ingroup PVM_PTB
*/
DLL_INTERFACE AdjProcnoResultType PTB_Path2ProcnoResultType(const char* dpath);



/**
  Takes the path to the procno and returns a AdjProcnoResultType containing the
  path information.

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

  @param[in] path
  The path as a string.

  @return A parameter of type AdjProcnoResultType containing the full path to
  the procno.

  @throws PvStdException when the path could not be parsed.

  @ingroup PVM_PTB
*/
DLL_INTERFACE AdjProcnoResultType PTB_Path2ProcnoResultType(std::string& dpath);

/** Writes parameter groups into new subdirectory of actual experiment.
    This function may be used in reconstruction networks or adjustments to
    store the actual state of parameters grouped in separate parameter groups
    specified by argument @c grouplist and @c cnt . If started inside
    adjustments the experiment directory from which the adjustment has been
    started or from which the adjustment platform has been entered (parent
    dataset).
    Caution: attempts to overwrite the standard directory structure are not
             prevented and may cause severe problems with data handling.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - empty string @c subdirn (arg 4)
  - id pointing to a negative integer
  - 0 poiter or empty string a @c filename (arg 5)
  - failure of directory creation
  - failure of pargroup storage (e.g. illegal pargroup names in \
    argument @c grouplist

  @PVM_EXAMPLE
  The following code would store parameter groups @c ProfileReportGroup ,
  @c SliceSelection and @c MethodClass into a file @c Results within a
  subdirectory of the current expno (inprocnodir == No). Function creates
  (if not existant) directory @c PowAdjustment and numbered subdirectories
  ( @c id is not 0). Subsequent calls increase the subdirectory number
  (@c keepId == No).

  @code
  int id=1;
  const char *const grouplist[]=      \
    {
      "ProfileReportGroup",
      "SliceSelection",
      "MethodClass"
    };


  PTB_StoreInParentDset(No,
                        grouplist,
                        3,
                        "PowAdjustment",
                        "Results",
                        &id,
                        No);

  @endcode

  @param[in ] inprocnodir if set to Yes, directory is stored as subdirectory of
              pdata (path of reconstructed data)
  @param[in ] grouplist array of strings specifiying the name of
              parameter groups to be stored in file
  @param[in ] ngroups number of array entries in array @c grouplist
  @param[in ] subdirn (optional) if nonzero name of a subdirectory that is
              created (if not existant) to store the file
  @param[in ] filename name of parameter file to be created
  @param[out] id if nonzero pointer, pointer of a non-negative integer
              that is used to create numbered subdirectories for subsequent
              calls. Id is set to the smalles non-existing numbered
              subdirectory that is created.
  @param[in]  keepid If set to Yes, numbered subdirectory is used, possible
              content of this directory may be overwritten.
  @param[out] outpath (optional) if not zero: absolute path 
  @return     nonzero value in case of error

  @ingroup PVM_PTB_INT
*/
DLL_INTERFACE int PTB_StoreInParentDset( YesNo inprocnodir,
                                         const char *const *grouplist,
                                         int cnt,
                                         const char *const subdirn,
                                         const char *const filename,
                                         int *id,
                                         YesNo keepId,
                                         std::string *outpath=0);

/** Writes parameter groups into new subdirectory of actual experiment.
    This function may be used in reconstruction networks or adjustments to
    store the actual state of parameters grouped in separate parameter groups
    specified by argument @c grouplist . If started inside
    adjustments the experiment directory from which the adjustment has been
    started or from which the adjustment platform has been entered (parent
    dataset).
    Caution: attempts to overwrite the standard directory structure are not
             prevented and may cause severe problems with data handling.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - empty string @c subdirn (arg 4)
  - id pointing to a negative integer
  - 0 poiter or empty string a @c filename (arg 5)
  - failure of directory creation
  - failure of pargroup storage (e.g. illegal pargroup names in \
    argument @c grouplist

  @PVM_EXAMPLE
  The following code would store parameter groups @c ProfileReportGroup ,
  @c SliceSelection and @c MethodClass into a file @c Results within a
  subdirectory of the current expno (inprocnodir == No). Function creates
  (if not existant) directory @c PowAdjustment and numbered subdirectories
  ( @c id is not 0). Subsequent calls increase the subdirectory number
  (@c keepId == No).

  @code
  int id=1;
  const char *const grouplist[]=      \
    {
      "ProfileReportGroup",
      "SliceSelection",
      "MethodClass"
    };


  PTB_StoreInParentDset(No,
    {"ProfileReportGroup", "SliceSelection", "MethodClass"},
    "PowAdjustment",
    "Results",
    &id,
    No);

  @endcode

  @param[in ] inprocnodir if set to Yes, directory is stored as subdirectory of
              pdata (path of reconstructed data)
  @param[in ] grouplist array of strings specifying the name of
              parameter groups to be stored in file
  @param[in ] subdirn (optional) if nonzero name of a subdirectory that is
              created (if not existent) to store the file
  @param[in ] filename name of parameter file to be created
  @param[out] id if nonzero pointer, pointer of a non-negative integer
              that is used to create numbered subdirectories for subsequent
              calls. Id is set to the smallest non-existing numbered
              subdirectory that is created.
  @param[in]  keepid If set to Yes, numbered subdirectory is used, possible
              content of this directory may be overwritten.
  @param[out] outpath (optional) if not zero: absolute path 
  @return     nonzero value in case of error

  @ingroup PVM_PTB
*/
DLL_INTERFACE int PTB_StoreInParentDset( YesNo inprocnodir,
    const std::vector<std::string> &grouplist,
    const char *const subdirn,
    const char *const filename,
    int *id,
    YesNo keepId,
    std::string *outpath=0);


/** Provide information about transfer of reference position.

    This function provides information whether or not a transfer of the
    actual reference position to geometry objects had beed performed.

    @PVM_PARS
    - PVM_RefPosTransferred
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
    // marks setting of refernce position
    // technically: sets PVM_RefPosTransferred to Yes
    PTB_RefPosTransferred(true);

    // retrieves state of PVM_RefPosTransferred
    bool transferred=PTB_RefPosTransferred();
    if(transferred)
     // ... some actions done

    @endcode
    @ingroup PVM_PTB_INT
    @param setstate argument controlling state of PVM_RefPosTransferred
           (false leaves parameter unchanged, true changes it to Yes) 
    @return current state of PVM_RefPosTransferred (returns false
            if it has no value)
 */
DLL_INTERFACE bool PTB_RefPosTransferred(bool setstate=false);

DLL_INTERFACE bool PTB_TransferAtsReferencePosition(bool callMethodHandler=true);
DLL_INTERFACE bool PTB_TransferAtsReferencePosition(const char *const gcname,
                                                    const double atspos,
                                                    bool shift=true);
DLL_INTERFACE bool PTB_TransferAtsReferencePosition(const char *const gcname,
                                                    const double refpos[3],
                                                    bool shift=true);

/**  Common actions after load of method class .

  This function is called as post-read action for pargroup MethodClass
  (i.e. the method file has been read from expno directory).
  General actions are implemented there.



  @PVM_PARS
  - reactions on deprecated parameters
  - forced default initialization of new parameters

  @PVM_TBCALL
  NONE

  @PVM_PRECOND

  MethodClass pargroup has been read.

  

  @PVM_EXAMPLE
  @code


  // for methods extending MethodClass, code e.g. in parsLayout.h or elsewhere
  // with declaration of postread routine:

  extend pargroup
  {
    Method;
    (...)
  }
  attributes
  {
     init_proc     initMethod;
     postread_proc handleProtocolParameters;
  }MethodClass;

  // definition of postread routine 

  void handleProtocolParameters(bool readOnly)
  {
    DB_MSG(("--> %s handleProtocolParameters( %s )",GetMethodEnumAsString(),
            readOnly ? "true":"false"));

    // calls  MRT_PostReadMethodGroup(readonly)
    // calls  PTB_PostReadMethodGroup(readonly)

    ParxRelsGroupRoutine("MethodClass",MODALITY_HIERARCHY,GroupPostRead);
    // method specific tasks

    if(readOnly)
    {
        return;
    }

    initMethod(readOnly);
    DB_MSG(("<-- %s initMethod( %s )",GetMethodEnumAsString(),
            readOnly ? "true":"false"));
  }

  @endcode

  @param readOnly flag indicating whether the dataset is read only or not.


  @ingroup PVM_PTB

 */
DLL_INTERFACE void PTB_PostReadMethodGroup(bool readonly);

DLL_INTERFACE void PTB_AtsBedPositionModeRange(void);

/**  Provides information about adjusted parameter in PVM_AdjResultHandler relation.

     This function provides information whether a parameter (specified by name 
     in argument @c parname ) has been edited. Return value is only @c true if
     called in a (possibly redirected) relation of PVM_AdjResultHandler and 
     parameter has been adjusted.


  @PVM_PARS
  NONE
  @PVM_TBCALL
  NONE

  @PVM_PRECOND
  NONE

  

  @PVM_EXAMPLE
  @code

  (...)
  bool deriveFromCuboid = PTB_ParameterAdjusted("PVM_SliceGeo");

  if(deriveFromCuboid == true)
  {
     // transfer state of cuboid to geometry parameter group
     ...
  }


  @endcode

  @param parname : name of parameter
  @return @c true if called in relation of PVM_AdjResultHandler and parameter 
          has been adjusted else @c false

  @ingroup PVM_PTB
 */
DLL_INTERFACE bool PTB_ParameterAdjusted(const char *parname);





/* Definitions for deprecated functions. */
#ifdef METHCOMP

/** Deprecated function name for @ref PTB_VisuSetLoopDependentParameter. */
#define PTB_VisuSetLoopDependantParameter(loopId,parName,startNdx) PTB_VisuSetLoopDependentParameter((loopId),(parName),(startNdx))

/** Deprecated function name for @ref PTB_VisuRemoveLoopDependentParameter. */
#define PTB_VisuRemoveLoopDependantParameter(loopId,parName,adaptDepVals) PTB_VisuRemoveLoopDependentParameter((loopId),(parName),(adaptDepVals))

/** Deprecated function name for @ref PTB_VisuChangeFGLen. */
#define PTB_VisuDecreaseFGLen(id, newLen) PTB_VisuChangeFGLen((id), (newLen))






#endif

#endif
#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
