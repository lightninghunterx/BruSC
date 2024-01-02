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

#ifndef PVSEQTOOLS_H
#define PVSEQTOOLS_H

#ifndef CPROTO

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif
#if defined(HAS_WINDOWS_FEATURES)
#  ifdef PvSeqTools_INTERNAL
#    define DLL_INTERFACE __declspec(dllexport)
#  else
#    define DLL_INTERFACE __declspec(dllimport)
#  endif
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#  define DLL_INTERFACE
#endif

#include <vector>
DLL_INTERFACE void STB_ImportHandler(void);
DLL_INTERFACE bool CMN_ImportID(void);
DLL_INTERFACE void CMN_InstructionHandler(void);
DLL_INTERFACE void CMN_AcqScanHandler(void);
DLL_INTERFACE void CMN_AdjHandler(void);
DLL_INTERFACE void CMN_AdjResultHandler(void);
DLL_INTERFACE void CMN_GopAdjResultHandler(void);



/** Restores parameters adjusted as <em> Study </em> result.

    This function restores common study result parameters. If they are adjusted
    their value is set to the last actual valid adjustment, otherwise
    they are set to "no value".

    @PVM_PARS
    - PVM_AtsReferencePositon
    @PVM_TBCALL
    - @ref STB_InitAtsReferencePosition
    @PVM_RELCALL
    NONE
    @PVM_PRECOND
    NONE

    @PVM_ERROR
    NONE

    @PVM_EXAMPLE
    @code
    CMN_RestoreStudyResultPars();
    @endcode
    @ingroup PVM_STB_INT

 */
DLL_INTERFACE void CMN_RestoreStudyResultPars(void);



/** Handling routine of load protocol action.

    This function is called if a protocol is loaded into the examination card.

    @PVM_H1{Actions}


    @PVM_H2{Animal Transport System handling}
    For systems equipped with an animal transport system (ATS) storage of the 
    actual position as reference is deactivated.

  @PVM_PARS
  - @c PVM_AtsChangeRefPos (set to No)
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @param readOnly flag indicating whether the dataset is read only or not.

  @ingroup PVM_STB
*/
DLL_INTERFACE void CMN_PostReadMethodProtocol(bool readOnly);
DLL_INTERFACE void STB_AtsInit(bool readOnly);
DLL_INTERFACE void STB_AtsInit(void);
DLL_INTERFACE void STB_AtsNpos(void);
DLL_INTERFACE void STB_AtsOverlap(void);
DLL_INTERFACE void STB_AtsCentralSetup(void);
DLL_INTERFACE void STB_AtsHandler(void);
DLL_INTERFACE void STB_AtsUpdate(std::vector<double> *res=0);
DLL_INTERFACE YesNo STB_AtsAdj(void);
DLL_INTERFACE YesNo STB_HandleAts(const char *const purpose);
DLL_INTERFACE void STB_AtsHandleAdjResult(void);
DLL_INTERFACE void STB_AtsHandleGopAdjResult(void);



DLL_INTERFACE int STB_TransferAtsOffsetToGeometry(const double atsoffs,
                                                  bool shift,
                                                  const double centeroffs);
DLL_INTERFACE void STB_GeoObjHandler (void);
DLL_INTERFACE void STB_AtsRefGeoObj(void);
DLL_INTERFACE bool STB_AtsRefGeoObjRange(void);
DLL_INTERFACE void STB_AtsRefGeoCub(void);



DLL_INTERFACE void STB_AtsRefPosHandler(void);
DLL_INTERFACE double STB_AtsRefGeoCubRange(void);
DLL_INTERFACE double STB_AtsRefGeoCubRange(double offset, bool force=false);
DLL_INTERFACE void STB_AtsBedPositionMode(void);
DLL_INTERFACE bool STB_AtsChangeRefPosRange(void);
DLL_INTERFACE void STB_AtsChangeRefPos(void);



/** Initializes the ATS reference position group.

    This function initializes the reference position parameter group for the 
    animal transport system @em ATS .

    The reference position is adjusted and restored according to the state of
    @c PVM_AtsChangeRefPos and @c PVM_AtsBedPositionMode in a study. The 
    reference position itself is visible in the geometry editor as separate 
    geometry object if @em ATS support is activated during study setup. 

  @PVM_H2{Implementation}
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Parameter group</td> <td>@c MethodAtsRefPosGroup             </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitAtsReferencePosition   </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateAtsReferencePosition </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_AtsRefPosHandler             </td> <td>@c callbackDefs.h      </td></tr>
  </table>

  With parameter @c PVM_AtsBedPositionMode of type @ref ATS_POS_POLICY 
  the following positions of the
  ATS can be selected for a scan:

  - @c Current (@c ATS_POS_CURRENT) - the bed position at scan start time is used
  - @c Image @c Center (@c ATS_POS_IMAGE_CENTER) - aligns the component of the 
       selected slice package / geometry object along the @em ATS with the 
       modality center (e.g. gradient zeropoint in MR modality)
  - @c Reference (@c ATS_POS_REFERENCE) - the reference position that has been
        adjusted at scan start.

  The control of the @em ATS position during scan start is derived from this state 
  by @ref STB_AtsAdj .


  @ref STB_UpdateAtsReferencePosition

  @PVM_PARS
  - @c PVM_AtsReferencePosition (restored according to last adjusted state)
  - @c PVM_AtsBedPositionMode 
  - @c PVM_AtsChangeRefPos
  - @c PVM_AtsRefGeoObj (geometry object)
  - @c PVM_AtsRefGeoCub (geometry editor interface parameter)
  

  @PVM_TBCALL
  - STB_InitAtsReferencePosition()

  @param readOnly if false @c STB_InitAtsReferencePosition() is called

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitAtsReferencePosition(bool readOnly);

/** Initializes the ATS reference position group.

    This function initializes the reference position parameter group for the 
    animal transport system @em ATS .

    The reference position is adjusted and restored according to the state of
    @c PVM_AtsChangeRefPos and @c PVM_AtsBedPositionMode in a study. The 
    reference position itself is visible in the geometry editor as separate 
    geometry object if @em ATS support is activated during study setup. 

  @PVM_H2{Implementation}
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Parameter group</td> <td>@c MethodAtsRefPosGroup             </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitAtsReferencePosition   </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateAtsReferencePosition </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_AtsRefPosHandler             </td> <td>@c callbackDefs.h      </td></tr>
  </table>

  With parameter @c PVM_AtsBedPositionMode of type @ref ATS_POS_POLICY 
  the following positions of the
  ATS can be selected for a scan:

  - @c Current (@c ATS_POS_CURRENT) - the bed position at scan start time is used
  - @c Image @c Center (@c ATS_POS_IMAGE_CENTER) - aligns the component of the 
       selected slice package / geometry object along the @em ATS with the 
       modality center (e.g. gradient zeropoint in MR modality)
  - @c Reference (@c ATS_POS_REFERENCE) - the reference position that has been
        adjusted at scan start.

  The control of the @em ATS position during scan start is derived from this state 
  by @ref STB_AtsAdj .


  @ref STB_UpdateAtsReferencePosition

  @PVM_PARS
  - @c PVM_AtsReferencePosition (restored according to last adjusted state)
  - @c PVM_AtsBedPositionMode 
  - @c PVM_AtsChangeRefPos
  - @c PVM_AtsRefGeoObj (geometry object)
  - @c PVM_AtsRefGeoCub (geometry editor interface parameter)
  

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE


  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitAtsReferencePosition(void);



/** Updates the @c  MethodAtsRefPosGroup group (@c std::string interface).

  After the call of this funtion all members of the group
  @c MethodAtsRefPosGroup have up-to-date values. If @em ATS support is
  activated for a study, the actual reference position is visualized in the geometry 
  editor.

  @PVM_PARS
  - @c PVM_AtsReferencePosition (restored according to last adjusted state)
  - @c PVM_AtsBedPositionMode 
  - @c PVM_AtsChangeRefPos
  - @c PVM_AtsRefGeoObj (geometry object)
  - @c PVM_AtsRefGeoCub (geometry editor interface parameter)

  @PVM_TBCALL
  - @ref PTB_InsertGeoObject
  - @ref PTB_RemoveGeoObject
  - @ref PTB_StoreStudyResult
 
  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE
 
 @PVM_ERROR
  For nonzero argument @c gobject type of parameter specified as name is not
  of type @ref GEO_OBJECT .

  @PVM_EXAMPLE

  @code
  std::string gobject("PVM_SliceGeoObj");
  STB_UpdateAtsReferencePosition(&gobject);
  @endcode

  @see @ref STB_InitAtsReferencePosition

  @ingroup PVM_STB

  @param gobject (optional) pointer to a string containing the name of geometry
         object that is used to derive the image center (0 pointer de-activates this
         bed position mode)

  @return true if the reference position has been changed 
*/
DLL_INTERFACE bool STB_UpdateAtsReferencePosition(const std::string *gobject=0);



/** Updates the @c  MethodAtsRefPosGroup group (@c c @c string interface).

    Version with string pointer argument.

    @see STB_UpdateAtsReferencePosition
    @see STB_InitAtsReferencePosition

    @code
    STB_UpdateAtsReferencePosition("PVM_SliceGeo");
    @endcode
  For nonzero argument @c gobject type of parameter specified as name is not
  of type @ref GEO_OBJECT .


    @param gobject string containing the name of geometry
           object that is used to derive the image center (0 pointer de-activates this
           bed position mode).

  @return value of @ref  STB_UpdateAtsReferencePosition (std::string interface)
 */
DLL_INTERFACE bool STB_UpdateAtsReferencePosition(const char *const gobject);

/** Algorithm to handle single and multi-position experiments.

    Sets or retrieves current ATS position for single and multi-position
    experiments and executes data acquisition and - reconstruction of
    multi-position acquisition in separate single position GOP adjustments.
    Dataset paths of multi-position acquisition are stored
    in parameter PVM_AtsDatasets.
    This function may be used only in a redirected relation of
    PVM_AdjHandler!

  @PVM_PARS
  - @c PVM_AtsDatasets

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  The central imaging point has to be defined.

  @return Yes if the adjustment is handled


  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_AtsAdj(void);



/** Constrains position of geometry object to modality center.

    This function updates @c  MethodAtsRefPosGroup group constraining  
    @c PVM_AtsBedPositionMode to @c ATS_POS_IMAGE_CENTER or 
    @c ATS_POS_REFERENCE .
    
    If @em ATS (Animal Transport System) support is activated in the study 
    the bed position is
    fixed according to the state of @c PVM_AtsBedPositionMode and the 
    geometry can be constrained to assure a zero offset to the modality
    center at this bed position.
    If no @em ATS support is activated the bed position is assumed to be 0.

    Argument @c gobject (parameter name, with parameter type @ref GEO_OBJECT)
    references a parameter of type @ref GEO_CUBOID_PACK that defines the
    position of the geometry object as shown in geometry editor. Arguments
    @c zero_pos0 ( @c zero_pos1 , @c zero_pos2 ) define which component of
    the position (in the acquisition coordinate system) should be set to the
    modality center (in case of MR modality: gradient isocenter).


  @PVM_PARS

  - see @ref STB_InitAtsReferencePosition
  - parameter of type @c GEO_CUBOID_PACK referenced by arg. @c gobject


  @PVM_TBCALL

  - @ref STB_UpdateAtsReferencePosition
 
  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE
 
 @PVM_ERROR
  Valid argument @c gobject
    - must have a value
    - parameter type is @ref GEO_OBJECT 
    - no array parameter

  @PVM_EXAMPLE

  @code


  bool changed=false;
  // for mr modality: this assures a zero read offset:
  changed = STB_ConstrainGeoObjectPosition("PVM_SliceGeoObj",
                                           true,      // zero read component
                                           false,     // phase component unchanged
                                           false);    // slice component unchanged
  if(changed)
  {
    ... // specific actions for constrained positions
  }                                         

  double minFov[3],minSliceThick;
  int maxPackages,MaxPerPackage;

  (...) // calculate minima

  // update geometry
  STB_UpdateImageGeometry(dim,
                          PVM_Matrix,
                          minFov,
                          0, // total slices (no restr)
                          maxPackages,
                          maxPerPackage,
                          minSliceThick,
                          1.0); // sliceFovRatio in 3D


  @endcode

  @code

  // example to constrain slice offset for nonselective excitation
  // assuming YesNo parameter SliceSelective
  bool changed=false;
  if(SliceSelective==No)
  {
     changed=STB_ConstrainGeoObjectPosition("PVM_SliceGeoObj",
                                             false,    // read component unchanged
                                             false,    // phase component unchanged
                                             true);    // zero slice component
  }


  double minFov[3],minSliceThick;
  int maxPackages,MaxPerPackage;

  (...) // calculate minima

  // update geometry
  STB_UpdateImageGeometry(dim,
                          PVM_Matrix,
                          minFov,
                          0, // total slices (no restr)
                          maxPackages,
                          maxPerPackage,
                          minSliceThick,
                          1.0); // sliceFovRatio in 3D

  @endcode


  @see STB_UpdateAtsReferencePosition

  @ingroup PVM_STB

  @param gobject string containing the name of a parameter of type GEO_OBJECT
  @param zero_pos0 flag to force zero modality offset along 
         first acquisition direction
  @param zero_pos1 flag to force zero modality offset along 
         second acquisition direction
  @param zero_pos2 flag to force zero modality offset along 
         third acquisition direction

  @return true if the position has been changed according to constraints.
*/
DLL_INTERFACE bool STB_ConstrainGeoObjectPosition(const char *const gobject,
                                                  bool zero_pos0,
                                                  bool zero_pos1,
                                                  bool zero_pos2);



DLL_INTERFACE void CMN_StudyACMapRel(void);



DLL_INTERFACE void CMN_StudyACMapCheck(void);

DLL_INTERFACE void CMN_StudyIGAssignmentRel(void);

DLL_INTERFACE void CMN_StudyIGAssignmentCheck(void);

DLL_INTERFACE void CMN_ReportHandler(void);

#endif
#endif
