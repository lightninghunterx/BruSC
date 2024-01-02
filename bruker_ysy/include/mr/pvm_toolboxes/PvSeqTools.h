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

#ifndef PVSEQMRTOOLS_H
#define PVSEQMRTOOLS_H

#ifndef CPROTO

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

/// DLL interface libParx identifiers for Windows.
#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#  ifdef PvSeqTools_INTERNAL
#    define DLL_INTERFACE __declspec(dllexport)
#  else
#    define DLL_INTERFACE __declspec(dllimport)
#  endif
#elif defined (__GNUC__) && __GNUC__ >= 4
#    define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#    define DLL_INTERFACE
#endif



/* range checkers and relations */
DLL_INTERFACE void STB_ExSlicePulseLength (void);
DLL_INTERFACE void STB_ExSlicePulseLengthRels (void);
DLL_INTERFACE void STB_ExSliceBandWidth (void);
DLL_INTERFACE void STB_ExSliceBandWidthRels (void);
DLL_INTERFACE void STB_ExSliceRephaseFactor (void);
DLL_INTERFACE void STB_ExSliceRephaseFactorRels (void);
DLL_INTERFACE void STB_ExSliceRampDownTime (void);
DLL_INTERFACE void STB_ExSliceRampDownTimeRels (void);
DLL_INTERFACE void STB_ExSliceRampDownIntegral (void);
DLL_INTERFACE void STB_ExSliceRampDownIntegralRels (void);
DLL_INTERFACE void STB_ExSliceRephaseRampUpTime (void);
DLL_INTERFACE void STB_ExSliceRephaseRampUpTimeRels (void);
DLL_INTERFACE void STB_ExSliceRephaseRampUpIntegral (void);
DLL_INTERFACE void STB_ExSliceRephaseRampUpIntegralRels (void);
DLL_INTERFACE void STB_ExSliceRephaseRampDownTime (void);
DLL_INTERFACE void STB_ExSliceRephaseRampDownTimeRels (void);
DLL_INTERFACE void STB_ExSliceRephaseRampDownIntegral (void);
DLL_INTERFACE void STB_ExSliceRephaseRampDownIntegralRels (void);
DLL_INTERFACE void STB_MinExSliceRephaseTime (void);
DLL_INTERFACE void STB_MinExSliceRephaseTimeRels (void);
DLL_INTERFACE void STB_ExSliceRephaseTime (void);
DLL_INTERFACE void STB_ExSliceRephaseTimeRels (void);
DLL_INTERFACE void STB_LimExSliceGradient (void);
DLL_INTERFACE void STB_LimExSliceGradientRels (void);
DLL_INTERFACE void STB_ExSliceGradient (void);
DLL_INTERFACE void STB_ExSliceGradientRels (void);
DLL_INTERFACE void STB_LimExSliceRephaseGradient (void);
DLL_INTERFACE void STB_LimExSliceRephaseGradientRels (void);
DLL_INTERFACE void STB_ExSliceRephaseGradient (void);
DLL_INTERFACE void STB_ExSliceRephaseGradientRels (void);
DLL_INTERFACE void STB_ExSliceRampDownWaitTime (void);
DLL_INTERFACE void STB_ExSliceRampDownWaitTimeRels (void);
DLL_INTERFACE void STB_SliceBandWidthScale (void);
DLL_INTERFACE void STB_SliceBandWidthScaleRels (void);



/**************************************************************************/
/*          gen/src/prg/pvm_toolboxes/mr/PvSeqTools/StbRfPulses.c         */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_RFPulseRange (PVM_PULSE *);
DLL_INTERFACE void STB_RFPulsePropertyRange (PVM_PULSE_PROPERTIES *);
DLL_INTERFACE void STB_RFPulseCorrRange (PVM_PULSE_CORRECTIONS *);
DLL_INTERFACE void STB_ExcPulseAngleRange (void);
DLL_INTERFACE void STB_ExcPulseAngle (void);



/**************************************************************************/
/*         gen/src/prg/pvm_toolboxes/mr/PvSeqTools/SingleRFPulses.c       */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_RFPulseAmplTypRange (const char *const );
DLL_INTERFACE int STB_HandleRFPulseAmpl (const char *const , const double);




/** Default range checking function for single RF-pulses.

  Range checking function for parameter of type @c PVM_RF_PULSE_TYPE.
  All members of the struct parameter argument pulse points are checked
  to be inside legal ranges.

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
  STB_CheckRFPulse(&ExcPulse);
  @endcode

  @param[in,out] pulse Pointer to a @c PVM_RF_PULSE_TYPE parameter

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_CheckRFPulse(PVM_RF_PULSE_TYPE *const pulse);




/** Initialises a single RF-Pulse.

  Function to be used in the method initialization part. All members
  of the pulse struct (given as an argument) are range-checked. After
  range checking of the default values (given as argument) their
  values will be copied into the propriate structmembers, if they have
  a legal value. Illegal values of the arguments (e.g. negative pulse
  durations,non existant shapefiles etc.) are replaced with legal
  default values. In this case, the return value will be @c Yes.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_GetPulseClassification
  - @ref CFG_RFPulseDefaultAttenuation

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
    STB_InitRFPulse(
      &ExcPulse,
      CFG_RFPulseDefaultShapename(LIB_EXCITATION),
      1.0,
      30.0);
  @endcode

  @param[out] pulse     Single RF-pulse parameter which should be initialised
  @param[in]  shapename Filename of the default RF-pulse waveform
  @param pulselength    Default length of the RF-pulse [ms]
  @param flipAngle      Default flip angle of the RF-pulse [deg]


  @return @c Yes if one of the default values given in the argument list have illegal ranges.
          In this case reasonable values are initialised.

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_InitRFPulse( PVM_RF_PULSE_TYPE *const pulse,
                                     const char        *const shapename,
                                         double               pulselength,
                                         double               flipAngle);




/** Single RF-pulse update for F1 channel.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref STB_SpecRFPulseUpdate

  @PVM_PRECOND
  See @ref STB_SpecRFPulseUpdate

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  YesNo referenceAvailable;
  double referenceAttenuation = 30.0;

  STB_UpdateRFPulse(
    "ExcPulse",
    &ExcPulse,
    referenceAvailable,
    referenceAttenuation);
  @endcode

  @param[in] parname           Name of the @c PARX pulse parameter given as a string
  @param[in,out] pulse         Pointer to the @c PARX pulse parameter
  @param referenceAvailable    Enables attenuation and flip angle calculation based on the
                               value of argument @a referenceAtt
  @param referenceAtt          Reference attenuation of a 90 deg 1 ms rectangular pulse

  @return @c No if an error occured, @c Yes in all other cases

  @see @ref STB_SpecRFPulseUpdate

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_UpdateRFPulse(const char *const        parname,
                                      PVM_RF_PULSE_TYPE *const pulse,
                                      YesNo                    referenceAvailable,
                                      const double             referenceAtt);




/** Specific single RF-pulse update for F1 channel.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref STB_SpecRFPulseUpdate

  @PVM_PRECOND
  See @ref STB_SpecRFPulseUpdate

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  YesNo referenceAvailable;
  double referenceAttenuation = 30.0;

  STB_SpecUpdateRFPulse(
    "ExcPulse",
    &ExcPulse,
    "FlipAngle",
    referenceAvailable,
    referenceAttenuation);
  @endcode

  @param[in]     parname            Name of the @c PARX pulse parameter given as a string
  @param[in,out] pulse              Pointer to the @c PARX pulse parameter
  @param[in]     member             String containing a struct member name of the pulse parameter
  @param         referenceAvailable Enables attenuation and flip angle calculation based on the
                                    value of argument @a referenceAtt
  @param         referenceAtt       Reference attenuation of a 90 deg 1 ms rectangular pulse

  @return @c No if an error occured, @c Yes in all other cases

  @see @ref STB_SpecRFPulseUpdate

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_SpecUpdateRFPulse(const char *const        parname,
                                          PVM_RF_PULSE_TYPE *const pulse,
                                          const char *const        member,
                                          YesNo                    referenceAvailable,
                                          const double             referenceAtt);




/** Single RF-pulse update on logical channel.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref STB_SpecRFPulseUpdate

  @PVM_PRECOND
  See @ref STB_SpecRFPulseUpdate

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  YesNo referenceAvailable;
  double referenceAttenuation = 30.0;

  STB_RFPulseUpdate(
    "ExcPulse",
    &ExcPulse,
    1,
    referenceAvailable,
    referenceAttenuation);
  @endcode

  @param[in] parname           Name of the @c PARX pulse parameter given as a string
  @param[in,out] pulse         Pointer to the @c PARX pulse parameter
  @param channel               Number of logical channel on which the pulse is applied
  @param referenceAvailable    Enables attenuation and flip angle calculation based on the value
                               of argument @a referenceAtt
  @param referenceAtt          Reference attenuation of a 90 deg 1 ms rectangular pulse

  @return @c No if an error occured, @c Yes in all other cases

  @see @ref STB_GetRefAtt, @ref STB_UpdateRFPulse, @ref STB_SpecRFPulseUpdate

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_RFPulseUpdate(const char *const        parname,
                                      PVM_RF_PULSE_TYPE *const pulse,
                                      const int                channel,
                                      YesNo                    referenceAvailable,
                                      const double             referenceAtt);




/** Specific single RF-pulse update on logical channel.

  Handles the relation between different members of the pulse struct (given as argument). The
  calculation of the attenuation for a given flip angle will be done only, if a reference
  attenuation is available as indicated by the arguments (@a referenceAvailable and
  @a referenceAtt) and if the value of @a referenceAtt has a legal range. If the calculated
  attenuation is lower than the return value of @ref CFG_MinRFAttenuation(@a channel), the
  flip angle  will be constrained. In case of illegal system configuration or illegal values of
  argument @a channel the RF attenuation is set to the highest possible value and in case of a
  flip angle derivation, the flip angle is set close to @c 0.0!

  The default behaviour may be controlled by the string argument @a member. If its value
  matches one of the name of the pulse struct members, the update will be done as if this
  member would have been edited. If not, the function acts in the same way as
  @ref STB_RFPulseUpdate.

  In detail following actions are performed:

  - Update of the pulse length in response to a bandwidth request
  - Update of the bandwidth in response to a pulse length request
  - Update of the pulse properties in response to a shapename request
  - Update of bandwidth, attenuation in response to appropriate correction requests

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_MinRFAttenuation
  - @ref CFG_RFPulseHighestAttenuation
  - STB_RFPulseRange
  - STB_RFPulsePropertyRange
  - STB_RFPulseCorrRange
  - @ref UT_GetRequest
  - @ref UT_HandleRequest
  - @ref UT_ClearRequest
  - @ref MRT_RFPulseBandwidthToPulseLength
  - @ref MRT_RFPulseAngle
  - @ref MRT_RFPulseAttenuation

  @PVM_PRECOND
  - This function can only handle specific requests, if the argument pulse points to a @c PARX
    parameter, whose relation calls the toolbox function @ref UT_SetRequest.
  - The @c PARX parameter name given as argument parname must coincide with the address of this
    parameter given as argument pulse. Otherwise the correct request handling may not be
    possible!

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  YesNo referenceAvailable;
  double referenceAttenuation = 30.0;

  STB_SpecRFPulseUpdate(
    "ExcPulse",
    &ExcPulse,
    1,
    "FlipAngle",
    referenceAvailable,
    referenceAttenuation);
  @endcode

  @param[in]     parname               Name of the @c PARX pulse parameter given as a string
  @param[in,out] pulse                 Pointer to the @c PARX pulse parameter
  @param         channel               Number of logical channel on which the pulse is applied
  @param[in]     member                String containing a struct member name of the pulse parameter
  @param         referenceAvailable    Enables attenuation and flip angle calculation based on the value
                                       of argument @a referenceAtt
  @param         referenceAtt          Reference attenuation of a 90 deg 1 ms rectangular pulse

  @return @c No if an error occured, @c Yes in all other cases

  @see @ref STB_GetRefAtt, @ref STB_UpdateRFPulse

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_SpecRFPulseUpdate(const char *const        parname,
                                          PVM_RF_PULSE_TYPE *const pulse,
                                          const int                channel,
                                          const char *const        member,
                                          YesNo                    referenceAvailable,
                                          const double             referenceAtt);




/** Initialises a dynamic enum for valid excitation pulse shapes.

  Initialises a dynamic enum containing all pulse shapes valid for <b>spin excitation</b>.
  The number of valid shapes depends  on the content of the default waveform directory in
  <c>$DU/exp/stan/nmr/lists/wave</c>. This function is intended to be used in the method
  initialization part.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_LibPulseEnumRange

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The string given as argument must be a parameter name of type @c PV_PULSE_LIST.

  @PVM_ERROR
  - The behaviour of this function is undefined, if its argument is not a @c PARX parameter
    name of type @c PV_PULSE_LIST.

  @PVM_EXAMPLE
  @code
  STB_InitExcPulseEnum("ExcPulseEnum");
  @endcode

  @param[in] pulseEnumParName    String containing a parameter of type @c PV_PULSE_LIST

  @return Number of RF-pulse shapes for excitation found in wavedirectory or @c -1 if no one
          has been found.

  @see @ref STB_UpdateExcPulseEnum, @ref STB_UpdateExcPulseName

  @ingroup PVM_STB
*/
DLL_INTERFACE int STB_InitExcPulseEnum(const char *const pulseEnumParName);




/** Updates a dynamic enum for valid excitation pulse shapes.

  Updates a dynamic enum containing all pulse shapes valid for <b>spin excitation</b>. The
  shapename (given as argument) will be translated into the appropriate enum state and the value
  of the enum (given as pointer argument @a pulseEnum) will be updated.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_GetPulseClassification
  - @ref CFG_LibPulseEnumRange

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Argument @a pulseEnumParName must be the name of the @c PARX parameter of type
    @c PV_PULSE_LIST argument @a pulseEnum points to.
  - The @c PARX parameter with the name given as argument @a pulseEnumParName should have been
    initialised (refer to function @ref STB_InitExcPulseEnum).

  @PVM_ERROR
  - The behaviour of this function is undefined, if argument @a pulseEnumParName is not the
    @c PARX parameter of type @c PV_PULSE_LIST argument @a pulseEnum points to.

  @PVM_EXAMPLE
  @code
  STB_UpdateExcPulseEnum("ExcPulseEnum",
                         &ExcPulseEnum,
                         ExcPulse.Filename,
                         ExcPulse.Classification);
  @endcode

  @param[in]     pulseEnumParName    String containing a parameter of type @c PV_PULSE_LIST
  @param[in,out] pulseEnum           Pointer to the @c PARX parameter with the name @a pulseEnumParName
  @param[in]     shapename           Name of the pulse waveform file
  @param         shapeClass          Classification of the pulse waveform

  @return @c No if the update of the enum fails

  @see @ref STB_InitExcPulseEnum, @ref STB_UpdateExcPulseName

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_UpdateExcPulseEnum(const char *const              pulseEnumParName,
                                           PV_PULSE_LIST *const           pulseEnum,
                                           const char *const              shapename,
                                           const PVM_PULSE_CLASSIFICATION shapeClass);




/** Converts the dynamic enum parameter state into a shape filename.

  Converts the value of a dynamic excitation pulse list parameter (name given as argument
  @a pulseEnumParName, value given as pointer argument @a pulseEnum) into a shape filename.
  This filename is copied into argument @a shapename, if it is not identical to @c USER_PULSE,
  the classifcation is copied into argument @a classification.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_LibPulseEnumRange

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Argument @a pulseEnumParName must be the name of the @c PARX parameter of type
    @c PV_PULSE_LIST argument @a pulseEnum points to.
  - The @c PARX parameter with the name given as argument @a pulseEnumParName should have been
    initialised (refer to function @ref STB_InitExcPulseEnum).

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_UpdateExcPulseName("ExcPulseEnum",
                         &ExcPulseEnum,
                         ExcPulse.Filename,
                         &ExcPulse.Classification);
  @endcode

  @param[in]  pulseEnumParName    String containing a parameter of type @c PV_PULSE_LIST
  @param[in]  pulseEnum           Pointer to the @c PARX parameter with the name @a pulseEnumParName
  @param[out] shapename           Name of the pulse waveform file
  @param      classification      Classification of the pulse waveform

  @return @c No if an error occured

  @see @ref STB_InitExcPulseEnum, @ref STB_UpdateExcPulseName

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_UpdateExcPulseName(const char               *const pulseEnumParName,
                                           PV_PULSE_LIST            *const pulseEnum,
                                           char                     *const shapename,
                                           PVM_PULSE_CLASSIFICATION *const classification);




/** Initialises a dynamic enum for valid refocussing pulse shapes.

  Initialises a dynamic enum containing all pulse shapes valid for <b>spin refocussing</b>.
  The number of valid shapes depends on the content of the default waveform directory in
  $DU/exp/stan/nmr/lists/wave. This function is intended to be used in the method
  initialization part.

  @PVM_PARS
  NONE

  @PVM_PRECOND
  - The string given as argument @b must be a parameter name of type @c PV_PULSE_LIST.

  @PVM_TBCALL
  - @ref CFG_LibPulseEnumRange

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  - The behaviour of this function is undefined, if its argument is not a @c PARX parameter
    name of type @c PV_PULSE_LIST.

  @PVM_EXAMPLE
  @code
  STB_InitRfcPulseEnum("RefPulseEnum");
  @endcode

  @param[out] pulseEnumParName    String containing a parameter of type @c PV_PULSE_LIST

  @return Number of RF-pulse shapes for refocussing found in wavedirectory or @c -1 if no one
          has been found.

  @see @ref STB_UpdateRfcPulseEnum, @ref STB_UpdateRfcPulseName

  @ingroup PVM_STB
*/
DLL_INTERFACE int STB_InitRfcPulseEnum(const char *const pulseEnumParName);




/** Updates a dynamic enum for valid refocussing pulse shapes.

  Updates a dynamic enum containing all pulse shapes valid for <b>spin refocussing</b>. The
  shapename (given as argument) will be translated into the appropriate enum state and the value
  of the enum (given as pointer argument @a pulseEnum) will be updated.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_LibPulseEnumRange

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Argument @a pulseEnumParName must be the name of the @c PARX parameter of type
    @c PV_PULSE_LIST argument @a pulseEnum points to.
  - The @c PARX parameter with the name given as argument @a pulseEnumParName should have been
    initialised (refer to function @ref STB_InitRfcPulseEnum).

  @PVM_ERROR
  - The behaviour of this function is undefined, if argument @a pulseEnumParName is not the
    @c PARX parameter of type @c PV_PULSE_LIST argument @a pulseEnum points to.

  @PVM_EXAMPLE
  @code
  STB_UpdateRfcPulseEnum("RefPulseEnum",
                         &RefPulseEnum,
                         RefPulse.Filename,
                         RefPulse.Classification);
  @endcode

  @param[in]     pulseEnumParName    String containing a parameter of type @c PV_PULSE_LIST
  @param[in,out] pulseEnum           Pointer to the @c PARX parameter with the name @a pulseEnumParName
  @param[in]     shapename           Name of the pulse waveform file
  @param         shapeClass          Classification of the pulse waveform

  @return @c No if the update of the enum fails

  @see @ref STB_InitRfcPulseEnum, @ref STB_UpdateRfcPulseName

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_UpdateRfcPulseEnum(const char    *const           pulseEnumParName,
                                           PV_PULSE_LIST *const           pulseEnum,
                                           const char    *const           shapename,
                                           const PVM_PULSE_CLASSIFICATION shapeClass);




/** Converts the dynamic enum parameter state into a shape filename.

  Converts the value of a dynamic refocussing pulse list parameter (name given as argument
  @a pulseEnumParName, value given as pointer argument @a pulseEnum) into a shape filename.
  This filename is copied into argument @a shapename, if it is not identical to @c USER_PULSE,
  the classifcation is copied into argument @a classification.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_LibPulseEnumRange

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Argument @a pulseEnumParName must be the name of the @c PARX parameter of type
    @c PV_PULSE_LIST argument @a pulseEnum points to.
  - The @c PARX parameter with the name given as argument @a pulseEnumParName should have been
    initialised (refer to function @ref STB_InitRfcPulseEnum).

  @PVM_EXAMPLE
  @code
  STB_UpdateRfcPulseName("RefPulseEnum",
                         &RefPulseEnum,
                         RefPulse.Filename,
                         &RefPulse.Classification);
  @endcode

  @param[in]  pulseEnumParName    String containing a parameter of type @c PV_PULSE_LIST
  @param[in]  pulseEnum           Pointer to the @c PARX parameter with the name @a pulseEnumParName
  @param[out] shapename           Name of the pulse waveform file
  @param      classification      Classification of the pulse waveform

  @return @c No if an error occured

  @see @ref STB_InitRfcPulseEnum, @ref STB_UpdateRfcPulseEnum

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_UpdateRfcPulseName(const char               *const pulseEnumParName,
                                           PV_PULSE_LIST            *const  pulseEnum,
                                           char                     *const shapename,
                                           PVM_PULSE_CLASSIFICATION *const classification);




/** Initialises a dynamic enum for valid inversion pulse shapes.

  Initialises a dynamic enum containing all pulse shapes valid for <b>spin inversion</b>.
  The number of valid shapes depends on the content of the default waveform directory in
  <c>$DU/exp/stan/nmr/lists/wave</c>. This function is intended to be used in the method
  initialization part.

  @PVM_PARS
  NONE

  @PVM_PRECOND
  - The string given as argument @b must be a parameter name of type @c PV_PULSE_LIST.

  @PVM_TBCALL
  - @ref CFG_LibPulseEnumRange

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  - The behaviour of this function is undefined, if its argument is not a @c PARX parameter
    name of type @c PV_PULSE_LIST.

  @PVM_EXAMPLE
  @code
  STB_InitInvPulseEnum("InvPulseEnum");
  @endcode

  @param[in] pulseEnumParName    String containing a parameter of type @c PV_PULSE_LIST

  @return Number of RF-pulse shapes for inversiob found in wavedirectory or @c -1 if no one
          has been found.

  @see @ref STB_UpdateInvPulseEnum, @ref STB_UpdateInvPulseName

  @ingroup PVM_STB
*/
DLL_INTERFACE int STB_InitInvPulseEnum(const char *const pulseEnumParName);




/** Updates a dynamic enum for valid inversion pulse shapes.

  Updates a dynamic enum containing all pulse shapes valid for <b>spin inversion</b>. The
  shapename (given as argument) will be translated into the appropriate enum state and the value
  of the enum (given as pointer argument @a pulseEnum) will be updated.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_LibPulseEnumRange

  @PVM_PRECOND
  - Argument @a pulseEnumParName must be the name of the @c PARX parameter of type
    @c PV_PULSE_LIST argument @a pulseEnum points to.
  - The @c PARX parameter with the name given as argument @a pulseEnumParName should have been
    initialised (refer to function @ref STB_InitInvPulseEnum).

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  - The behaviour of this function is undefined, if argument @a pulseEnumParName is not the
    @c PARX parameter of type @c PV_PULSE_LIST argument @a pulseEnum points to.

  @PVM_EXAMPLE
  @code
  STB_UpdateInvPulseEnum("InvPulseEnum",
                         &InvPulseEnum,
                         InvPulse.Filename,
                         InvPulse.Classification);
  @endcode

  @param[in]  pulseEnumParName    String containing a parameter of type @c PV_PULSE_LIST
  @param[out] pulseEnum           Pointer to the @c PARX parameter with the name @a pulseEnumParName
  @param[in]  shapename           Name of the pulse waveform file
  @param      shapeClass          Classification of the pulse waveform

  @return @c No if the update of the enum fails

  @see @ref STB_InitInvPulseEnum, @ref STB_UpdateInvPulseName

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_UpdateInvPulseEnum(const char    *const           pulseEnumParName,
                                           PV_PULSE_LIST *const           pulseEnum,
                                           const char    *const           shapename,
                                           const PVM_PULSE_CLASSIFICATION shapeClass);




/** Converts the dynamic enum parameter state into a shape filename.

  Converts the value of a dynamic inversion pulse list parameter (name given as argument
  @a pulseEnumParName, value given as pointer argument @a pulseEnum) into a shape filename.
  This filename is copied into argument @a shapename, if it is not identical to @c USER_PULSE,
  the classifcation is copied into argument @a classification.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_LibPulseEnumRange

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Argument @a pulseEnumParName must be the name of the @c PARX parameter of type
    @c PV_PULSE_LIST argument @a pulseEnum points to.
  - The @c PARX parameter with the name given as argument @a pulseEnumParName should have been
    initialised (refer to function @ref STB_InitInvPulseEnum).

  @PVM_EXAMPLE
  @code
  STB_UpdateInvPulseName("InvPulseEnum",
                         &InvPulseEnum,
                         InvPulse.Filename,
                         &InvPulse.Classification);
  @endcode

  @param[in]  pulseEnumParName    String containing a parameter of type @c PV_PULSE_LIST
  @param[in]  pulseEnum           Pointer to the @c PARX parameter with the name @a pulseEnumParName
  @param[out] shapename           Name of the pulse waveform file
  @param      classification      Classification of the pulse waveform

  @return @c No if an error occured

  @see @ref STB_InitInvPulseEnum, @ref STB_UpdateInvPulseEnum

  @todo Incomplete documentation: PVM_PARS, PVM_RELCALL, PVM_ERROR, PVM_EXAMPLE

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_UpdateInvPulseName(const char *const               pulseEnumParName,
                             PV_PULSE_LIST *const            pulseEnum,
                             char *const                     shapename,
                             PVM_PULSE_CLASSIFICATION *const classification);




/** Delivers spatial resolution.

  The spatial resolution in mm is delivered by this function. The dimension of the
  result vector is controlled by the number of spatial dimensions. A length of 0
  indicates an unlocalized experiments.


  @PVM_PARS
  State of the encoding parameter group.

  @PVM_TBCALL
  NONE


  @PVM_ERROR
  - illegal state of encoding parameter group resulting
    in illegal value for spatial resolution

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - the encoding parameter group must have been updated.


  @PVM_EXAMPLE
  @code
  //(...)
  // update encoding parameter group: precondition for
  // call of STB_GetSpatResol()


  STB_UpdateEncoding(
    NULL,
    SEG_SEQUENTIAL,
    Yes,
    Yes,
    Yes,
    &PVM_EchoPosition);


  // (...)
  std::vector<double> resol=STB_GetSpatResol();

  // spatial dimensions can be tested e.g. in this way:
  if( resol.size() == 0)
  {
    // no spatial experiment
    ...
  }

  switch(resol.size())
  {
     case 0:
        // no spatial dimension, maybe spectroscopic experiment
        break;
     case 1:
        // resol[0] : spatial resolution in 1st dimension (read)
        break;
     case 2:
        // resol[1] : spatial resolution in 2nd dimension (phase)
     case 3:
        // resol[2] : spatial resolution in 3rd dimension (slice)
        break;
     default:
        // illegal state should never occur
        break;
   }

  @endcode

  @return vector of size spatial dimensions, elements in mm

  @see @ref STB_UpdateEncoding

  @todo Incomplete documentation:, PVM_PARS, PVM_RELCALL, PVM_ERROR, PVM_EXAMPLE

  @ingroup PVM_STB
*/

DLL_INTERFACE std::vector<double> STB_GetSpatResol(void);


/* gen/src/prg/pvm_toolboxes/mr/PvSeqTools/inplaneGeoAtoms.c */

/* range checkers and relations */
DLL_INTERFACE void STB_SpatDimEnumRange (void);
DLL_INTERFACE void STB_SpatDimEnum (void);
DLL_INTERFACE void STB_DimEnumRange (void);
DLL_INTERFACE void STB_DimEnum (void);
DLL_INTERFACE void STB_SpatDimRange (void);
DLL_INTERFACE void STB_SpatDim (void);
DLL_INTERFACE void STB_IsotropicRange (void);
DLL_INTERFACE void STB_Isotropic (void);
DLL_INTERFACE void STB_IsotropicFovRes (void);
DLL_INTERFACE void STB_MatrixRange (void);
DLL_INTERFACE void STB_Matrix (void);
DLL_INTERFACE void STB_MaxMatrixRange (void);
DLL_INTERFACE void STB_MaxMatrix (void);
DLL_INTERFACE void STB_MinMatrixRange (void);
DLL_INTERFACE void STB_MinMatrix (void);
DLL_INTERFACE void STB_AntiAliasRange (void);
DLL_INTERFACE void STB_AntiAlias (void);
DLL_INTERFACE void STB_MaxAntiAliasRange (void);
DLL_INTERFACE void STB_MaxAntiAlias (void);
DLL_INTERFACE void STB_SpatResolRange (void);
DLL_INTERFACE void STB_SpatResol (void);
DLL_INTERFACE void STB_MinSpatResolutionRange (void);
DLL_INTERFACE void STB_MinSpatResolution (void);
DLL_INTERFACE void STB_FovRange (void);
DLL_INTERFACE void STB_Fov (void);
DLL_INTERFACE void STB_FovCmRange (void);
DLL_INTERFACE void STB_FovCm (void);
DLL_INTERFACE void STB_MinFovRange (void);
DLL_INTERFACE void STB_MinFov (void);



/** Initialisation of geometry parameters

  Initialiser for the <em>Image Geometry</em> parameter group which includes
  both inplane and slice geometry parameters.

  Note that the parameters @c PVM_SpatDimEnum, @c PVM_Matrix, and @c PVM_AntiAlias
  belong to the @em Encoding parameter group.

  @PVM_PARS
  - @c PVM_IsotropicFovRes
  - @c PVM_SpatResol
  - @c PVM_Fov
  - @c PVM_FovCm
  - @c PVM_MinFov
  - See @ref STB_InitSliceGeoPars

  @PVM_TBCALL
  - @ref PTB_GetSpatDim
  - @ref UT_InitDoubleArray
  - @ref STB_InitSliceGeoPars

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateImageGeometry

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitImageGeometry(void);




/** Update of geometry parameters

  Updater for the <em>Image Geometry</em> parameter group which includes
  both inplane and slice geometry parameters.

  Note that the parameters @c PVM_SpatDimEnum, @c PVM_Matrix, and @c PVM_AntiAlias
  belong to the @em Encoding parameter group.

  @PVM_PARS
  - @c PVM_Isotropic
  - @c PVM_SpatResol
  - @c PVM_MinSpatResolution
  - @c PVM_Fov
  - @c PVM_FovCm
  - @c PVM_MinFov
  - @c PVM_IsotropicFovRes
  - @c PVM_SliceGeoObj
  - @c PVM_NSPacks
  - @c PVM_SPackArr*
  - @c PVM_SliceThick
  - See @ref STB_UpdateSliceGeoPars

  @PVM_TBCALL
  - @ref UT_GetRequest
  - @ref GTB_HandleGeoCuboid
  - @ref UT_ClearRequest
  - @ref GTB_GeoObjForceZeroGap
  - @ref CFG_DefaultFov
  - @ref GTB_IsotropicRequestForResolution
  - @ref GTB_FovToSpatResol
  - @ref GTB_IsotropicRequestForFov
  - @ref GTB_GeoObjExtentKeepAllRatios
  - @ref GTB_GeoObjFreezeExt0Ext1Ratio
  - @ref GTB_GeoObjExtentKeepAllRatios
  - @ref STB_UpdateSliceGeoPars
  - @ref UT_ArrParsHaveSameDimension

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The <em>Image Geometry</em> parameter group must have been initialised with @ref STB_InitImageGeometry.
  - Size of @a imageMatrix must be at least @a dim.

  @PVM_ERROR
  - Illegal values of argument @a minFov.

  @PVM_EXAMPLE
  @code
  double minFov[3];

  // ... determine minFov ...

  STB_UpdateImageGeometry(PTB_GetSpatDim(),
                          PVM_Matrix,
                          minFov);
  @endcode

  @param     dim            Spatial dimension
  @param[in] imageMatrix    Image matrix size for all dimensions
  @param[in] minFov         Minimum FOV in all spatial dimensions
  @param     maxSlices      Constrains the total number of slices. For values less or equal to
                            @c 0 as well as values greater than the return value of
                            @ref CFG_MaxSlices, the return value of @ref CFG_MaxSlices is used
                            for the constrain.
  @param maxNSPacks         Constrains the number of slice packages. For values less or equal
                            to @c 0 as well as values greater than @ref CFG_MaxSlicePackages,
                            the latter value is used for the constrain.
  @param maxSlicePerPack    Constrains the number of slices per packages. For values less or
                            equal to @c 0 as well as values greater than @ref CFG_MaxSlices,
                            the latter value is used for the constrain.
  @param minSliceThick      Constrains the minimum slice thickness (mm) and sets
                            @c PVM_MinSliceThick. For values less or equal to @c 0 a value of
                            @a 0.001 mm is used.
  @param sliceFovRatio      Ratio of slice thickness and FOV in third dimension

  @see @ref STB_InitImageGeometry

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateImageGeometry(int           dim,
                                           const int    *imageMatrix,
                                           const double *minFov,
                                           int           maxSlices        = 0,
                                           int           maxNSPacks       = 1,
                                           int           maxSlicePerPack  = 1,
                                           double        minSliceThick    = 0.0,
                                           double        sliceFovRatio    = 1.0);





/** Initialization routine of inplane geometry parameters.

  @PVM_H1{Note}
  It is advised to use the initialiser @ref STB_InitImageGeometry for the new <em>Image Geometry</em>
  parameter group instead which includes both inplane and slice geometry parameters.

  This function initialises parameters for

  - spatial dimensionality
  - field of view
  - resolution

  This function is intended to be used in the method initialization part. With the call of this
  function, method specific constraints (given by appropriate values of the arguments) to the
  matrix sizes and the spatial dimensionality are initialised. The parameters initialised by
  this function are interface parameters to the geometry editor. It defines furthermore the
  visibility and editability of the parameters listed below. The dimensionality, represented by
  the state of parameter @c PVM_SpatDimEnum is constrained to the range of argument
  @a spatialDimRange (@a spatialDimRange[0] <= dim <= @a spatialDimRange[1]). It is
  possible to initialise pure 1D, 2D, or 3D spatial dimensionality by appropriate setting of
  argument values.

  @PVM_PARS
  - @c PVM_SpatDimEnum
  - @c PVM_Matrix
  - @c PVM_MinMatrix
  - @c PVM_MaxMatrix
  - @c PVM_MaxAntiAlias
  - @c PVM_AntiAlias
  - @c PVM_Isotropic
  - @c PVM_SpatResol
  - @c PVM_MinSpatResolution
  - @c PVM_Fov
  - @c PVM_FovCm
  - @c PVM_MinFov

  @PVM_TBCALL
  - @ref CFG_DefaultImagingSpatialMatrixSize
  - @ref UT_InitIntArray
  - @ref CFG_DefaultFov
  - @ref UT_InitDoubleArray
  - @ref PTB_InitSpatDim
  - @ref PTB_SetSpatDim

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Matrix constrain arguments
    - @a lowerMatrixSize (@a upperMatrixSize) must be 3 element integer arrays containing the
      lowest (highest) spatial matrix size for each spatial dimension.
    - Each element of argument @a lowerMatrixSize must be less or equal to the same element of
      argument @a upperMatrixSize.
    - Each element of arguments @a lowerMatrixSize and @a upperMatrixSize must be less or equal
      to the value given by @ref CFG_MaxImagingSpatialMatrixSize.
    - Each element of arguments @a lowerMatrixSize and @a upperMatrixSize must be greater or
      equal to the value given by @ref CFG_MinImagingSpatialMatrixSize.

  - Dimension constrain arguments
    - @a spatialDimRange is a fixed 2 element integer array.
    - @c 1 <= @a spatialDimRange[0] < @c 3
    - @a spatialDimRange[0] <= @a spatialDimRange[1]
    - @a spatialDimRange[0] <= @a diminit <= @a spatialDimRange[1]

  @PVM_ERROR
  - Illegal values of argument @a dimInit (@c 0 < @a dimInit <= @c 3)
  - Illegal values for elements of argument @a spatialDimRange
  - Illegal values for elements of argument @a lowerMatrixSize
  - Illegal values for elements of argument @a upperMatrixSize

  @PVM_EXAMPLE
  @code
  int dimRange[2] = {2, 2};
  int lowMat[3]   = {32, 32, 8};
  int upMat[3]    = {2048, 2048, 256};

  STB_InitStandardInplaneGeoPars(2, dimRange, lowMat, upMat, No);
  @endcode

  @param diminit            Default spatial dimension if @c PVM_SpatDimEnum has no legal value
  @param spatialDimRange    Two element integer array containing the lowest
                            (@a spatialDimRange[0]) and highest (@a spatialDimRange[1])
                            spatial dimensionality
  @param lowerMatrixSize    Tree element integer array containing the lowest spatial matrix
                            size for each spatial dimension
  @param upperMatrixSize    Tree element integer array containing the highest spatial matrix
                            size for each spatial dimension
  @param unitFovCm          Determines whether the FOV parameters shown in the method are
                            displayed in units of cm (@c Yes) or mm (@c No)

  @return @c Yes

  @see @ref STB_StandardInplaneGeoParHandler

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_InitStandardInplaneGeoPars( int    diminit,
                                                    int   *spatialDimRange,
                                                    int   *lowerMatrixSize ,
                                                    int   *upperMatrixSize,
                                                    YesNo  unitFovCm);




/** Inplane geometry parameter group handler.

  @PVM_H1{Note}
  It is advised to use the updater @ref STB_UpdateImageGeometry for the new <em>Image Geometry</em>
  parameter group instead which includes both inplane and slice geometry parameters.

  This function handles the dependencies of parameters listed below as well as their update in
  response to user input. The FOV parameters are constrained to the values of @a minFov given
  as double array. These values have to be calculated in the methodHandler. The argument
  @a maxReadAntiAlias is usually set to @c 2.0.
  The parameters handled by this function are interface parameters to
  the geometry editor. The matrix sizes as well as the anti alias factors are constrained in a
  way that the resultant dataset size does not exceed the maximum data set size.

  @PVM_PARS
  - @c PVM_SpatDimEnum
  - @c PVM_Matrix
  - @c PVM_MinMatrix
  - @c PVM_MaxMatrix
  - @c PVM_MaxAntiAlias
  - @c PVM_AntiAlias
  - @c PVM_Isotropic
  - @c PVM_SpatResol
  - @c PVM_MinSpatResolution
  - @c PVM_Fov
  - @c PVM_FovCm
  - @c PVM_MinFov

  @PVM_TBCALL
  - @ref PTB_GetSpatDim
  - @ref CFG_MaxAntiAliasFactor
  - @ref CFG_DefaultImagingSpatialMatrixSize
  - @ref GTB_ConstrainIsotropicState
  - @ref GTB_IsotropicRequestForMatrix
  - @ref CFG_DefaultFov
  - @ref GTB_IsotropicRequestForFov
  - @ref GTB_IsotropicRequestForResolution
  - @ref GTB_FovToSpatResol

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The inplane geometry parameters should have been initialised with a call of
    @ref STB_InitStandardInplaneGeoPars.

  @PVM_ERROR
  - Illegal values of argument @a minFov
  - Illegal values of argument @a maxReadAntiAlias
  - No value of @c PVM_SpatDimEnum

  @PVM_EXAMPLE
  @code
  double minFov[3];

  // ... determine minFov ...

  STB_StandardInplaneGeoParHandler(minFov, 2.0);
  @endcode

  @param minFov              Fixed three element double array containing the minimum FOV in all
                             spatial dimensions
  @param maxReadAntiAlias    Maximum anti alias factor in read direction

  @return @c Yes

  @see @ref PTB_GetSpatDim, @ref STB_InitStandardInplaneGeoPars

  @todo Incomplete documentation: PVM_EXAMPLE

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_StandardInplaneGeoParHandler(const double *minFov,
                                                     const double  maxReadAntiAlias);




/** Inplane geometry parameter group handler.

  This function is equivalent to @ref STB_StandardInplaneGeoParHandler.

  @see @ref STB_StandardInplaneGeoParHandler

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_UpdateStandardInplaneGeoPars(const double *minFov,
                                                     const double  maxReadAntiAlias);




/**************************************************************************/
/*           gen/src/prg/pvm_toolboxes/mr/PvSeqTools/SliceGeoAtoms.c      */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE YesNo STB_SliceSchemeRange (PV_SLICE_SCHEME_TYPE *);
DLL_INTERFACE YesNo STB_SlorTypeRange (SLOR_TYPE *);
DLL_INTERFACE YesNo STB_ReorTypeRange (PV_REOR_TYPE *);
DLL_INTERFACE YesNo STB_SliceGapModeTypeRange (PV_SLICE_GAP_MODE *);
DLL_INTERFACE void STB_MaxNSlicesRange (void);
DLL_INTERFACE void STB_MaxNSlices (void);
DLL_INTERFACE void STB_NSPacksRange (void);
DLL_INTERFACE void STB_NSPacks (void);
DLL_INTERFACE void STB_SPackArrNSlicesRange (int);
DLL_INTERFACE void STB_SPackArrNSlices (void);
DLL_INTERFACE void STB_ObjOrderSchemeRange (void);
DLL_INTERFACE void STB_ObjOrderScheme (void);
DLL_INTERFACE void STB_ObjOrderListRange (void);
DLL_INTERFACE void STB_ObjOrderList (void);
DLL_INTERFACE void STB_MajSliceOriRange (void);
DLL_INTERFACE void STB_MajSliceOriRel (void);
DLL_INTERFACE YesNo STB_SPackArrGradOrientRange (void);
DLL_INTERFACE void STB_SPackArrGradOrient (void);
DLL_INTERFACE void STB_SPackArrSliceOrientRange (void);
DLL_INTERFACE void STB_SPackArrSliceOrient (void);
DLL_INTERFACE void STB_SPackArrReadOrientRange (void);
DLL_INTERFACE void STB_SPackArrReadOrient (void);
DLL_INTERFACE void STB_MinSliceThickRange (void);
DLL_INTERFACE void STB_MinSliceThick (void);
DLL_INTERFACE void STB_SliceThickRange (void);
DLL_INTERFACE void STB_SliceThick (void);
DLL_INTERFACE void STB_SPackArrSliceGapModeRange (void);
DLL_INTERFACE void STB_SPackArrSliceGapMode (void);
DLL_INTERFACE void STB_SPackArrSliceGapRange (double);
DLL_INTERFACE void STB_SPackArrSliceGap (void);
DLL_INTERFACE void STB_SPackArrSliceDistanceRange (void);
DLL_INTERFACE void STB_SPackArrSliceDistance (void);
DLL_INTERFACE void STB_SPackArrSliceOffsetRange (void);
DLL_INTERFACE void STB_SPackArrSliceOffset (void);
DLL_INTERFACE void STB_SPackArrReadOffsetRange (void);
DLL_INTERFACE void STB_SPackArrReadOffset (void);
DLL_INTERFACE void STB_SPackArrPhase0OffsetRange (void);
DLL_INTERFACE void STB_SPackArrPhase0Offset (void);
DLL_INTERFACE void STB_SPackArrPhase1OffsetRange (void);
DLL_INTERFACE void STB_SPackArrPhase1Offset (void);
DLL_INTERFACE void STB_SPackArrPhase2OffsetRange (void);
DLL_INTERFACE void STB_SPackArrPhase2Offset (void);
DLL_INTERFACE void STB_SliceOffsetRange (void);
DLL_INTERFACE void STB_SliceOffset (void);
DLL_INTERFACE void STB_EffSliceOffsetRange (void);
DLL_INTERFACE void STB_EffSliceOffset (void);
DLL_INTERFACE void STB_ReadOffsetRange (void);
DLL_INTERFACE void STB_ReadOffset (void);
DLL_INTERFACE void STB_EffReadOffsetRange (void);
DLL_INTERFACE void STB_EffReadOffset (void);
DLL_INTERFACE void STB_Phase0OffsetRange (void);
DLL_INTERFACE void STB_Phase0Offset (void);
DLL_INTERFACE void STB_Phase1OffsetRange (void);
DLL_INTERFACE void STB_Phase1Offset (void);
DLL_INTERFACE void STB_Phase2Offset (void);
DLL_INTERFACE void STB_EffPhase2OffsetRange (void);
DLL_INTERFACE void STB_EffPhase0OffsetRange (void);
DLL_INTERFACE void STB_EffPhase0Offset (void);
DLL_INTERFACE void STB_EffPhase1OffsetRange (void);
DLL_INTERFACE void STB_EffPhase1Offset (void);
DLL_INTERFACE void STB_Phase2OffsetRange (void);
DLL_INTERFACE void STB_EffPhase2Offset (void);
DLL_INTERFACE void STB_SliceOffsetHzRange (void);
DLL_INTERFACE void STB_SliceOffsetHz (void);
DLL_INTERFACE void STB_ReadOffsetHzRange (void);
DLL_INTERFACE void STB_ReadOffsetHz (void);
DLL_INTERFACE void STB_Phase1OffsetHzRange (void);
DLL_INTERFACE void STB_Phase1OffsetHz (void);
DLL_INTERFACE void STB_Phase2OffsetHzRange (void);
DLL_INTERFACE void STB_Phase2OffsetHz (void);
DLL_INTERFACE void STB_SliceOrientRange (void);
DLL_INTERFACE void STB_SliceOrient (void);
DLL_INTERFACE void STB_ReadOrientRange (void);
DLL_INTERFACE void STB_ReadOrient (void);
DLL_INTERFACE void STB_SliceAngleLHRange (void);
DLL_INTERFACE void STB_SliceAngleLH (void);
DLL_INTERFACE void STB_SliceAngleAHRange (void);
DLL_INTERFACE void STB_SliceAngleAH (void);
DLL_INTERFACE void STB_SliceAngleALRange (void);
DLL_INTERFACE void STB_SliceAngleAL (void);
DLL_INTERFACE void STB_ReadAngle (void);
DLL_INTERFACE void STB_SPackNdxRange (void);
DLL_INTERFACE void STB_SPackNdx (void);
DLL_INTERFACE void STB_SPackNSlicesRange (void);
DLL_INTERFACE void STB_SPackNSlices (void);
DLL_INTERFACE void STB_SPackSliceOffsetCm (void);
DLL_INTERFACE void STB_SPackReadOffsetCm (void);
DLL_INTERFACE void STB_SPackPhase1OffsetCm (void);
DLL_INTERFACE void STB_SPackPhase2OffsetCm (void);
DLL_INTERFACE void STB_SPackSliceOffset (void);
DLL_INTERFACE void STB_SPackReadOffset (void);
DLL_INTERFACE void STB_SPackPhase1Offset (void);
DLL_INTERFACE void STB_SPackPhase2Offset (void);
DLL_INTERFACE void STB_SPackSliceDistanceRange (void);
DLL_INTERFACE void STB_SPackSliceDistance (void);
DLL_INTERFACE void STB_SPackSliceGapRange (void);
DLL_INTERFACE void STB_SPackSliceGap (void);
DLL_INTERFACE void STB_SPackSliceGapModeRange (void);
DLL_INTERFACE void STB_SPackSliceGapMode (void);




/**************************************************************************/
/*        gen/src/prg/pvm_toolboxes/mr/PvSeqTools/SliceGeoHandler.c       */
/**************************************************************************/

/** Initialises the slice geometry parameter group.

  @PVM_H1{Note}
  It is advised to use the initialiser @ref STB_InitImageGeometry for the new <em>Image Geometry</em>
  parameter group instead which includes both inplane and slice geometry parameters.

  With help of this function, all slice geometry parameters are checked to lie in a legal range
  as specified by arguments and system constraints. For a detailed description and parameter
  list refer to @ref STB_UpdateSliceGeoPars.

  @PVM_PARS
  See @ref STB_UpdateSliceGeoPars

  @PVM_TBCALL
  - @ref STB_UpdateSliceGeoPars

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_InitSliceGeoPars(0, 0, 0);
  @endcode

  @param maxSlices          Constrains the total number of slices. For values less or equal to
                            @c 0 as well as values greater than the return value of
                            @ref CFG_MaxSlices, the return value of @ref CFG_MaxSlices is used
                            for the constrain.
  @param maxNSPacks         Constrains the number of slice packages. For values less or equal
                            to @c 0 as well as values greater than the return value of
                            @ref CFG_MaxSlicePackages, the return value of @ref CFG_MaxSlices
                            is used for the constrain.
  @param maxSlicePerPack    Constrains the number of slices per packages. For values less or
                            equal to @c 0 as well as values greater than the return value of
                            @ref CFG_MaxSlices, the return value of @ref CFG_MaxSlices is used
                            for the constrain.

  @return @c Yes if a range check of the arguments results in different values as specified
          else @c No

  @see  @ref CFG_MaxSlices, @ref CFG_MaxSlicePackages, @ref STB_UpdateSliceGeoPars

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_InitSliceGeoPars(int maxSlices,
                           int maxNSPacks,
                           int maxSlicePerPack);




/** Updates the slice geometry parameter group.

  @PVM_H1{Note}
  It is advised to use the updater @ref STB_UpdateImageGeometry for the new <em>Image Geometry</em>
  parameter group instead which includes both inplane and slice geometry parameters.

  This updater checks the consistancy of the slice geometry parameter group as listed below.
  - According to the arguments (see below) it constraints the total number of slices, the total
    number of slice packages and the total number of slices per package. For values of
    arguments less or equal to @c 0 the constraints are performed according to system limits
    delivered by toolbox functions @ref CFG_MaxSlices and @ref CFG_MaxSlicePackages.
  - It controls the visibility, editability, and the storage in the method file of the
    parameters listed below.
  - Parameters with prefix @c PVM_SPackArr are checked to have @c PVM_NSPacks entries.
  - The position and orientation of individual slices is derived from the orientation and
    position of the related slice packages. In case of assymetric gradient coils, slice read
    and phase offsets are corrected and the result is stored in special parameters, intended to
    be used for the calculation of frequency lists and/or reco rotation.
  - The parameters @c PVM_SliceOffsetHz and @c PVM_ReadOffsetHz are dimensioned to hold the
    frequency offsets for all slices. The final values have to be calculated with the help of
    routine MRT_FrequencyOffsetList in the method code using the information about the slice
    gradient strength and the gamma of the nucleus of interest.

  @PVM_PARS
  - Parameters defining the number of slices and slice packages
    - @c PVM_MaxNSlices
    - @c PVM_NSPacks
    - @c PVM_SPackArrNSlices
  - Parameters defining the excitation order of slices
    - @c PVM_ObjOrderScheme
    - @c PVM_ObjOrderList
  - Slice Package Orientation Parameters
    - @c PVM_SPackArrGradOrient
    - @c PVM_SPackArrSliceOrient
    - @c PVM_SPackArrReadOrient
  - Parameters defining the common slice thickness
    - @c PVM_MinSliceThick
    - @c PVM_SliceThick
  - Parameters defining the coordinates of slice packages
    - @c PVM_SPackArrSliceGapMode
    - @c PVM_SPackArrSliceGap
    - @c PVM_SPackArrSliceDistance
    - @c PVM_SPackArrSliceOffset
    - @c PVM_SPackArrReadOffset
    - @c PVM_SPackArrPhase0Offset
    - @c PVM_SPackArrPhase1Offset
    - @c PVM_SPackArrPhase2Offset
  - Parameters defining the coordinates of slices. These parameters are dimensioned with the
    total number of slices which is the sum of all entries of parameter @c PVM_SPackArrNSlices:
    - @c PVM_SliceOffset     (as derived from packages)
    - @c PVM_ReadOffset      (as derived from packages)
    - @c PVM_Phase0Offset    (as derived from packages, same as @c PVM_ReadOffset)
    - @c PVM_Phase1Offset    (as derived from packages)
    - @c PVM_Phase2Offset    (as derived from packages)
    - @c PVM_EffSliceOffset  (@c PVM_SliceOffset corrected in case of assymetric gradient coils)
    - @c PVM_EffReadOffset   (@c PVM_ReadOffset corrected in case of assymetric gradient coils)
    - @c PVM_EffPhase0Offset (same as @c PVM_EffReadOffset)
    - @c PVM_EffPhase1Offset (@c PVM_Phase1Offset corrected in case of assymetric gradient
                              coils)
    - @c PVM_EffPhase2Offset (@c PVM_Phase2Offset corrected in case of assymetric gradient
                              coils)
    - @c PVM_SliceOffsetHz
    - @c PVM_ReadOffsetHz

  @PVM_TBCALL
  - @ref CFG_MaxSlices
  - @ref CFG_MaxSlicePackages
  - @ref GTB_ReadOrientToGradOrient
  - @ref GTB_SetStandardOrientation
  - @ref UT_ArrParsHaveSameDimension
  - @ref UT_ClearRequest
  - @ref UT_GetRequest
  - @ref UT_HandleRequest
  - @ref UT_RequestIndex
  - @ref UT_RequestParName
  - @ref UT_UniqueIntArray

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  double minThickness;

  // ... determine minThickness ...

  STB_UpdateSliceGeoPars(0, 0, 0, minThickness);
  @endcode

  @param maxSlices          Constrains the total number of slices. For values less or equal to
                            @c 0 as well as values greater than the return value of
                            @ref CFG_MaxSlices, the return value of @ref CFG_MaxSlices is used
                            for the constrain.
  @param maxNSPacks         Constrains the number of slice packages. For values less or equal
                            to @c 0 as well as values greater than the return value of
                            @ref CFG_MaxSlicePackages, the return value of @ref CFG_MaxSlices
                            is used for the constrain.
  @param maxSlicePerPack    Constrains the number of slices per packages. For values less or
                            equal to @c 0 as well as values greater than the return value of
                            @ref CFG_MaxSlices, the return value of @ref CFG_MaxSlices is used
                            for the constrain.
  @param minSliceThick      Constrains the minimum slice thickness (unit mm) and sets
                            @c PVM_MinSliceThick. For values less or equal to @c 0 a value of
                            @a 0.001 mm is used.

  @return @c Yes if a range check of the arguments results in different values as specified
          else @c No

  @see  @ref CFG_MaxSlices, @ref CFG_MaxSlicePackages, @ref STB_InitSliceGeoPars

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_UpdateSliceGeoPars(int    maxSlices,
                                           int    maxNSPacks,
                                           int    maxSlicePerPack,
                                           double minSliceThick);




/**************************************************************************/
/*           gen/src/prg/pvm_toolboxes/mr/PvSeqTools/userType.c           */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_UserTypeRange (void);
DLL_INTERFACE void STB_UserType (void);
DLL_INTERFACE void STB_DefaultUserTypeHandler (void);




/**************************************************************************/
/*           gen/src/prg/pvm_toolboxes/mr/PvSeqTools/FatSupHandler.c      */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_FatSupRelations(void);
DLL_INTERFACE void STB_FatSupSpoilRel(void);
DLL_INTERFACE void STB_FatSupBandWidth(void);
DLL_INTERFACE void STB_FatSupPulRange(void);
DLL_INTERFACE void STB_FatSupPul(void);
DLL_INTERFACE void STB_FatSupPulEnum(void);
DLL_INTERFACE void STB_FatSupPulAmpl(void);

/** Initialises the parameters of the <em> Fat Suppression </em> preparation module.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Switch         </td> <td>@c PVM_FatSupOnOff          </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Parameter group</td> <td>@c Fat_Sup_Parameters       </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitFatSupModule   </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateFatSupModule </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_FatSupHandler        </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>BaseLevel      </td> <td>@ref ATB_SetFatSupBaselevel </td> <td>@c BaseLevelRelations.c</td></tr>
  <tr><td>Duration       </td> <td>@c PVM_FatSupModuleTime     </td> <td>@c backbone()          </td></tr>
  <tr><td>Include        </td> <td>@c FatSup.mod or @n
                                   @c PrepModulesHead.mod      </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Subroutine     </td> <td>@c FatSup                   </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Parameter card </td> <td>@c FatSup.xml               </td> <td><c>\<method\>.xml</c>  </td></tr>
  <tr><td>Setup card     </td> <td>@c FatSupSetup.xml          </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>

  @PVM_PARS
  - @c PVM_FatSupOnOff
  - @c PVM_FatSupRampTime
  - @c PVM_FatSupGradWait
  - @c PVM_FatSupPul
  - @c PVM_FatSupPulEnum
  - @c PVM_FatSupPulAmpl
  - @c PVM_FatSupPulShape
  - @c PVM_FatSupBandWidth
  - @c PVM_FatSupSpoil

  @PVM_TBCALL
  - @ref CFG_AmplifierEnable
  - @ref CFG_GradientRampTime
  - @ref CFG_InterGradientWaitTime
  - @ref STB_InitRFPulse
  - @ref STB_GetFreqOffset
  - @ref MRT_InitSpoiler

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateFatSupModule, @ref ATB_SetFatSupBaselevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitFatSupModule(void);

/** Updates the parameters of the <em> Fat Suppression </em> preparation module.

  The module will only be activated if @a nucStr is @c "1H".

  @PVM_PARS
  - @c PVM_FatSupPerform
  - @c PVM_FatSupPul
  - @c PVM_FatSupPulAmpl
  - @c PVM_FatSupPulShape
  - @c PVM_FatSupBandWidth
  - @c PVM_FatSupFL
  - @c PVM_FatSupSpoil
  - @c PVM_FatSupModuleTime

  @PVM_TBCALL
  - @ref STB_UpdateRFPulse
  - @ref STB_GetFreqOffset
  - @ref MRT_UpdateSpoiler
  - @ref CFG_GradientRiseTime

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The module must have been initialised with @ref STB_InitFatSupModule.
  - The nuclei parameter group must have been updated.

  @PVM_ERROR
  - Invalid nucleus in @a nucStr.

  @PVM_EXAMPLE
  @code
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, PVM_SpatResol[0]);
  @endcode

  @param[in] nucStr       nucleus
  @param     deriveGains  (optional) @c Yes = gains will be derived from reference power (default)
  @param     spoilerThick (optional) Scale basis for the spoiling efficiency

  @see @ref STB_InitFatSupModule, @ref ATB_SetFatSupBaselevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateFatSupModule(const char *const nucStr, YesNo deriveGains = Yes, double spoilerThick=1);




/**************************************************************************/
/*          gen/src/prg/pvm_toolboxes/mr/PvSeqTools/FwsHandler.c          */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_FwsOnOffRel(void);
DLL_INTERFACE void STB_FwsChemShiftRel(void);
DLL_INTERFACE void STB_FwsAngleRel(void);
DLL_INTERFACE void STB_FwsLoopRel(void);
DLL_INTERFACE void STB_FwsKernelSizeRel(void);
DLL_INTERFACE void STB_FwsKernelSizeRange(void);
DLL_INTERFACE void STB_FwsGaussBroadeningRel(void);
DLL_INTERFACE void STB_FwsGaussBroadeningRange(void);
DLL_INTERFACE void STB_FwsRecoModeRel(void);
DLL_INTERFACE void STB_FwsImagesRel(void);

/** Initialises the parameters of the <em> Fat Water Separation </em> module.
  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Switch         </td> <td>@c PVM_FwsOnOff         </td> <td>@c         </td></tr>
  <tr><td>Parameter group</td> <td>@c Fws_Parameters       </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Reco parameter group</td> <td>@c Fws_RecoParameters</td> <td>@c parsLayout.h      </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitFwsModule  </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateFwsModule</td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_FwsHandler       </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>Reco Handler   </td> <td>@c PVM_FwsRecoHandler   </td> <td>@c callbackDefs.h      </td></tr>

  <tr><td>BaseLevel      </td> <td>@ref ATB_SetFwsBaseLevel</td> <td>@c BaseLevelRelations.c</td></tr>
  <tr><td>Reco BaseLevel </td> <td>@ref ATB_SetFwsRecoParam</td> <td>@c RecoRelations.c</td></tr>
  <tr><td>Reco Network   </td> <td>@ref ATB_FwsRecoDerive  </td> <td>@c RecoRelations.c</td></tr>

  <tr><td>Timing         </td> <td>@c PVM_FwsEchoShift     </td> <td>@c backbone()          </td></tr>
  <tr><td>Include        </td> <td>@c Fws.mod              </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Subroutines    </td> <td>@c FwsDelX, FwsLoop     </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Parameter card </td> <td>@c Fws.xml              </td> <td><c>\<method\>.xml</c>  </td></tr>
  <tr><td>Reco card      </td> <td>@c FwsReco.xml          </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>

In addition to the default module elements, this module provides a reconstruction parameter group.
The group reco handler of this module is typically redirected to @c SetRecoParam() which has to call @c ATB_SetFwsRecoParam() before setting the default reconstruction parameters.
After the default reconstruction network has been built up, the network has to be modified using the @c ATB_FwsRecoDerive() function.


  @PVM_PARS
  - @c PVM_FwsOnOff
  - @c PVM_FwsChemShift
  - @c PVM_FwsAngle
  - @c PVM_FwsEchoShift
  - @c PVM_FwsLoop             (number of loop executions)
  - @c PVM_FwsRecoMode
  - @c PVM_FwsKernelSize
  - @c PVM_FwsGaussBroadening
  - @c PVM_FwsSpatShift        (spatial shift between fat and water)
  - @c PVM_FwsSnr              (SNR threshold for mask)
  - @c PVM_FwsSigLevel         (amplitude threshold for mask)
  - @c PVM_FwsImages           (number of reconstructed images)



  @PVM_TBCALL

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateFwsModule, @ref ATB_SetFwsBaseLevel, @ref ATB_SetFwsRecoParam

  @ingroup PVM_STB

*/
DLL_INTERFACE void STB_InitFwsModule(void);

/** Updates the parameters of the <em> Fat Water Separation </em> module.

*/
DLL_INTERFACE void STB_UpdateFwsModule(double FrqWork);




/**************************************************************************/
/*          gen/src/prg/pvm_toolboxes/mr/PvSeqTools/SelIrHandler.c        */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_SelIrSpoilerRel (void);
DLL_INTERFACE void STB_SelIrGradLimit (void);
DLL_INTERFACE void STB_SelIrGradLimitRange (void);
DLL_INTERFACE void STB_SelIrGrad (void);
DLL_INTERFACE void STB_SelIrGradRange (void);
DLL_INTERFACE void STB_SelIrInvTime (void);
DLL_INTERFACE void STB_SelIrInvTimeRange (void);
DLL_INTERFACE void STB_SelIrOnOff (void);
DLL_INTERFACE void STB_SelIrOnOffRange (void);
DLL_INTERFACE void STB_SelIrNSlabs (void);
DLL_INTERFACE void STB_SelIrNSlabsRange (void);
DLL_INTERFACE void STB_SelIrSlabThick (void);
DLL_INTERFACE void STB_SelIrSlabThickRange (void);
DLL_INTERFACE void STB_SelIrMinThick (void);
DLL_INTERFACE void STB_SelIrMinThickRange (void);
DLL_INTERFACE void STB_SelIrSlabOffset (void);
DLL_INTERFACE void STB_SelIrSlabOffsetRange (void);
DLL_INTERFACE void STB_SelIrPulEnumRel (void);
DLL_INTERFACE void STB_SelIrPulRange (void);
DLL_INTERFACE void STB_SelIrPulRel (void);
DLL_INTERFACE void STB_SelIrPulAmpl (void);

/** Initialises the parameters of the <em> Selective Inversion Recovery </em> preparation module.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Switch         </td> <td>@c PVM_SelIrOnOff           </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Parameter group</td> <td>@c Selective_IR_Parameters  </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitSelIrModule    </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateSelIrModule  </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_SelIrHandler         </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>BaseLevel      </td> <td>@ref ATB_SetSelIrBaseLevel  </td> <td>@c BaseLevelRelations.c</td></tr>
  <tr><td>Duration       </td> <td>@c PVM_SelIrModuleTime      </td> <td>@c backbone()          </td></tr>
  <tr><td>Include        </td> <td>@c SliceSelIr.mod or @n
                                   @c PrepModulesHead.mod      </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Subroutine     </td> <td>@c SliceSelIr               </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Parameter card </td> <td>@c SelIr.xml               </td> <td><c>\<method\>.xml</c>  </td></tr>
  <tr><td>Setup card     </td> <td>@c SelIrSetup.xml        </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>


  @PVM_PARS
  - @c PVM_SelIrInvTime
  - @c PVM_SelIrNSlabs
  - @c PVM_SelIrSlabThick
  - @c PVM_SelIrSlabOffset
  - @c PVM_SelIrOffset
  - @c PVM_SelIrMinThick
  - @c PVM_SelIrGrad
  - @c PVM_SelIrPul
  - @c PVM_SelIrPulEnum
  - @c PVM_SelIrPulAmpl
  - @c PVM_SelIrPulShape
  - @c PVM_SelIrSpoiler

  @PVM_TBCALL
  - @ref STB_InitRFPulse
  - @ref MRT_InitSpoiler

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateSelIrModule, @ref ATB_SetSelIrBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitSelIrModule(void);


/** Updates the parameters of the <em> Selective Inversion Recovery </em> preparation module.

  The <em> slice selective inversion recovery </em> module can be inserted into a multi-slice
  sequence to perform a time-optimized inversion. The module inverts all specified slices in a
  sequential manner, before the sequence excites these slices in the same order. The updater
  calculates the frequency offset for the inversion RF-pulses, the amplitude and duration of
  the spoiler gradient and the delays for inversion, PVM_InterInvDelay and PVM_InterInvEndDelay.
  Argument sliceSegDur controls the temporal separation of the inversion pulses, argument
  fixedTime controls the point of time within the imaging part at which the inversion time
  PVM_SelIrInvTime is reached. In product methods it is usually set to the excitation pulse.

  @image html STB_SelIrModule.png

  @PVM_PARS
  - @c PVM_SelIrPul
  - @c PVM_SelIrPulEnum
  - @c PVM_SelIrPulAmpl
  - @c PVM_SelIrPulShape
  - @c PVM_SelIrNSlabs
  - @c PVM_SelIrSlabThick
  - @c PVM_SelIrSlabOffset
  - @c PVM_SelIrOffsetHz
  - @c PVM_SelIrFL
  - @c PVM_SelIrMinThick
  - @c PVM_SelIrGrad
  - @c PVM_SelIrSpoiler
  - @c PVM_SelIrModuleTime

  @PVM_TBCALL
  - @ref STB_UpdateRFPulse
  - @ref MRT_UpdateSpoiler
  - @ref CFG_AmplifierEnable
  - @ref CFG_GradientRiseTime

  @PVM_PRECOND
  - The module must have been initialised with @ref STB_InitSelIrModule.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_UpdateSelIrModule(PVM_SliceThick,PVM_SliceOffset,nSlices,&SliceSegDur,0,fixedTime,PVM_DeriveGains);
  @endcode

  @param  minSlabThick     Minimum thickness of the inversion slabs
  @param  slabOffset       Offset of each inversion slab in millimeters
  @param  invSlabNumber    Number of inverion slabs
  @param  sliceSegDur      Duration of one inversion segment (time between excitations of
                           consecutive slices)
  @param  packageNumber    Pointer to the required number of slice packages
  @param  fixedtime        Fixed time at the beginning of the sequence (outside of the
                           <em> inversion </em> module) to be included in the inversion time.
  @param  deriveGains      @c Yes = gains will be derived from reference power

  @return @c Yes if the required number of packages or the @a sliceSegDur were not accepted and
          have been changed by the module, otherwise @c No.

  @see @ref STB_InitSelIrModule, @ref ATB_SetSelIrBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_UpdateSelIrModule(double        minSlabThick,
                                          const double *slabOffset,
                                          int           invSlabNumber,
                                          double       *sliceSegDur,
                                          int          *packageNumber,
                                          double        fixedtime,
                                          YesNo         deriveGains=Yes);




/**************************************************************************/
/*       gen/src/prg/pvm_toolboxes/mr/PvSeqTools/MagTransHandler.c        */
/**************************************************************************/
// Module deprecated & Replaced by SatTrans module
// Init & update functions remain to show pop-up when used in customer
// code of older PV versions

DLL_INTERFACE void STB_InitMagTransModule(void);


DLL_INTERFACE void STB_UpdateMagTransModule(YesNo calcPow=Yes,
                              double spatExtent=0.0);



/* -----------------------------------------------------------
   gen/src/prg/pvm_toolboxes/PvSeqTools/SatTransHandler.c
------------------------------------------------------------ */


/* range checkers and relations */
DLL_INTERFACE void STB_SatTransCommonRels(void);
DLL_INTERFACE void STB_SatTransType(void);
DLL_INTERFACE void STB_SatTransFreqUnit(void);
DLL_INTERFACE void STB_SatTransPulseEnum(void);
DLL_INTERFACE void STB_SatTransPulseRange (void);
DLL_INTERFACE void STB_SatTransPulse (void);
DLL_INTERFACE void STB_SatTransSpoil(void);
DLL_INTERFACE void STB_SatTransRefgain (void);
DLL_INTERFACE void STB_SatTransRefgainRange (void);
DLL_INTERFACE void STB_SatTransSetFrequencies (void);
DLL_INTERFACE void STB_SatTransPulseAmpl(void);
DLL_INTERFACE void ATB_SetSatTransBaseLevel(void);


/** Initialises the parameters of the <em> saturation transfer </em> module.

  This module supports two different saturation transfer mechanisms:
  Magnetization Transfer Contrast (MTC) and Chemical Exchange Saturation Transfer (CEST).

  Both versions of saturation transfer apply a series of rf pulses at frequencies to
  be specified in parameter card SatTrans.xml. While for MTC only one frequency can be
  specified (as an offset to the working frequency) CEST usually uses a number of frequencies
  stored in list @c PVM_SatTransFL of size @c PVM_SatTransRepetition.


  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Switch         </td> <td>@c PVM_SatTransOnOff           </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Parameter group</td> <td>@c Sat_Transfer_Parameters     </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitSatTransModule    </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateSatTransModule  </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_SatTransHandler         </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>BaseLevel      </td> <td>@ref ATB_SetSatTransBaseLevel  </td> <td>@c BaseLevelRelations.c</td></tr>
  <tr><td>Duration       </td> <td>@c PVM_SatTransModuleTime      </td> <td>@c backbone()          </td></tr>
  <tr><td>Include        </td> <td>@c SatTrans.mod or @n
                                   @c PrepModulesHead.mod         </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Subroutines    </td> <td>@c SatTransInit @n
                                   @c SatTrans @n
                                   @c SatTransInc                </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Parameter card </td> <td>@c SatTrans.xml                </td> <td><c>\<method\>.xml</c>  </td></tr>
  <tr><td>Setup card     </td> <td>@c SatTransSetup.xml           </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>


  Additional adaptions within the method:
  - Baselevel  parameter @c NR has to factor in @c PVM_SatTransRepetition. This is usually done in
    toolbox @c ATB_SetNR(...).
  - Total scan time has to account for increase of repetition number.
  - Total scan size of all (!) acquisition jobs may need to be adapted (usually in
    SetAcquisitionParameters()).
  - In pulse program:
     - Subroutine @c SatTransInit() resets the list counter; to be placed into the ppg's head.
     - Subroutine @c SatTransInc() sets the next offset frequency; to be placed in front of NR-loop.
       If the pulse program uses a different counter than NR to loop over repetitions it may has to
       be multiplied by @c PVM_SatTransRepetitions in BaseLevelRelations.
     - Subroutine @c SatTrans() plays out saturation pulses; to be placed at appropriate position
       within ppg's body.



  @PVM_PARS


  - @c PVM_SatTransOnOff
  - @c PVM_SatTransType
  - @c PVM_SatTransRFType
  - @c PVM_SatTransPulse
  - @c PVM_SatTransPulseEnum
  - @c PVM_SatTransPulseAmpl
  - @c PVM_SatTransPulseShape
  - @c PVM_SatTransPulseAmpl_uT
  - @c PVM_SatTransInterPulseDelay
  - @c PVM_SatTransNPulses
  - @c PVM_SatTransSpoil
  - @c PVM_SatTransRefScan
  - @c PVM_SatTransPowerList
  - @c PVM_SatTransModuleTime
  - @c PVM_SatTransFreqInput
  - @c PVM_SatTransFreqUnit
  - @c PVM_SatTransFreqValues
  - @c PVM_SatTransFreqRanges
  - @c PVM_SatTransFL
  - @c PVM_SatTransRepetitions
  - @c PVM_SatTransModuleMeanRFPower


  @PVM_TBCALL
    - @ref STB_InitRFPulse
    - @ref MRT_InitSpoiler

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateSatTransModule, @ref ATB_SetSatTransBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitSatTransModule(void);


/** Updates the parameters of the <em> saturation transfer </em> module.

  1. Sets PVM_SatTransRepetitions to number of specified offset frequencies
     (size of PVM_SatTransFreqValues) +1 if reference scan is used).
     Use this parameter in method's backbone for total scan time calculation
     and in baselevel relations to adapt NR. @n
  2. Sets up PVM_SatTransFL, the list of offset frequencies in [Hz]. This is
     the list used in SatTrans.mod. @n
  3. Sets up the rf saturation transfer pulse and the power list
     (@c PVM_SatTransPowerList) with a zero pulse amplitude for the reference scan. @n
  4. Updates and checks the gradient spoiler pulse. @n
  5. Updates module duration, mean rf power and parameters' UI visibility. @n

  @PVM_PARS
  Only members of the @c Sat_Transfer_Parameters group

  @PVM_TBCALL
  - @ref STB_UpdateRFPulse
  - @ref MRT_UpdateSpoiler
  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The following parameters are used and must have values:
    - @c PVM_SliceThick
    - PVM_Nucleus1 (update of Nuclei pargroup before call of
      this function)


  @PVM_ERROR
  - @c PVM_SliceThick (if spatExtent is not specified or <= 0)
       has no value

  @PVM_EXAMPLE
  @code
  // void call, activates RF pulse amplitude calculation,
  // uses PVM_SliceThick as spatial dimension

  STB_UpdateSatTransModule();

  // call with all arguments
  double minext=PVM_VoxArrSize[0][0];

  for(int k=1;k<3;k++)
  {
    minext=MIN_OF(minext,PVM_VoxArrSize[0][k]);
  }

  STB_UpdateSatTransModule(PVM_DeriveGains,minext);


  @endcode

  @param deriveAmpl (optional): If "yes" (default) the power for the rf pulse is
                                derived from the reference power.
  @param spatExtent (optional): The spatial dimension used to calculate the
                                spoiling capacity. Use the smallest dimension
                                of the Slice / Voxel dimension. If not specified
                                PVM_SliceThick is taken.

  @see @ref STB_InitSatTransModule, @ref ATB_SetSatTransBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateSatTransModule( YesNo deriveAmpl=Yes, double spatExtent=0.0 );




/**************************************************************************/
/*        gen/src/prg/pvm_toolboxes/mr/PvSeqTools/SatSlicesHandler.c      */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_FovSatThick(void);
DLL_INTERFACE void STB_FovSatOffset(void);
DLL_INTERFACE void STB_FovSatRelations(void);
DLL_INTERFACE void STB_FovSatSpoilRel(void);
DLL_INTERFACE void STB_FovSatSliceOri(void);
DLL_INTERFACE void STB_FovSatPulRange(void);
DLL_INTERFACE void STB_FovSatPul(void);
DLL_INTERFACE void STB_FovSatPulEnum(void);
DLL_INTERFACE void STB_FovSatPulAmpl(void);
DLL_INTERFACE void STB_FovSatGeoObjRel(void);
DLL_INTERFACE void STB_FovSatGeoCubRel(void);
DLL_INTERFACE void STB_FovSatHandler(void);

/** Initialises the parameters of the <em>FOV saturation</em> preparation module.

  Application of saturation slices to restrict the FOV.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Switch         </td> <td>@c PVM_FovSatOnOff          </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Parameter group</td> <td>@c Fov_Sat_Parameters       </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitFovSatModule   </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateFovSatModule </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_FovSatHandler        </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>BaseLevel      </td> <td>@ref ATB_SetFovSatBaseLevel </td> <td>@c BaseLevelRelations.c</td></tr>
  <tr><td>Duration       </td> <td>@c PVM_FovSatModuleTime     </td> <td>@c backbone()          </td></tr>
  <tr><td>Include        </td> <td>@c FovSat.mod or @n
                                   @c PrepModulesHead.mod      </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Subroutine     </td> <td>@c FovSat                   </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Parameter card </td> <td>@c FovSat.xml               </td> <td><c>\<method\>.xml</c>  </td></tr>
  <tr><td>Setup card     </td> <td>@c FovSatSetup.xml          </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>

  @PVM_PARS
  - @c PVM_FovSatAmpEnable
  - @c PVM_FovSatRampTime
  - @c PVM_FovSatGradWait
  - @c PVM_FovSatSatOnOff
  - @c PVM_FovSatNSlices
  - @c PVM_FovSatSliceOri
  - @c PVM_FovSatSliceOriMat
  - @c PVM_FovSatSliceVec
  - @c PVM_FovSatThick
  - @c PVM_FovSatOffset
  - @c PVM_FovSatGrad
  - @c PVM_FovSatSpoil
  - @c PVM_FovSatPul
  - @c PVM_FovSatPulEnum
  - @c PVM_FovSatPulAmpl
  - @c PVM_FovSatPulShape
  - @c PVM_FovSatGeoCub

  @PVM_TBCALL
  - @ref CFG_AmplifierEnable
  - @ref CFG_GradientRampTime
  - @ref CFG_InterGradientWaitTime
  - @ref CFG_DefaultSliceThickness
  - @ref STB_InitRFPulse
  - @ref MRT_InitSpoiler

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateFovSatModule, @ref ATB_SetFovSatBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitFovSatModule(void);

/** Updates the parameters of the <em>FOV saturation</em> preparation module.

  @PVM_PARS
  - @c PVM_FovSatPul
  - @c PVM_FovSatGrad
  - @c PVM_FovSatSliceOri
  - @c PVM_FovSatSliceOriMat
  - @c PVM_FovSatSliceVec
  - @c PVM_FovSatThick
  - @c PVM_FovSatOffset
  - @c PVM_FovSatFL
  - @c PVM_FovSatModuleTime
  - @c PVM_FovSatGeoCub

  @PVM_TBCALL
  - @ref CFG_IsNucleusKnown
  - @ref PTB_RemoveGeoObject
  - @ref PTB_InsertGeoObject
  - @ref STB_UpdateRFPulse
  - @ref GTB_GeoObjFreezeExtent0
  - @ref GTB_GeoObjFreezeExtent1
  - @ref MRT_SliceThickness
  - @ref PTB_InsertTransferFeature
  - @ref MRT_UpdateSpoiler
  - @ref CFG_GradientRiseTime

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The nuclei parameter group must have been updated.

  @PVM_ERROR
  - Invalid nucleus in @a nucStr.

  @PVM_EXAMPLE
  @code
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  @endcode

  @param[in] nucStr       nucleus
  @param     deriveGains  (optional) @c Yes = gains will be derived from reference power (default)

  @see @ref STB_InitFovSatModule, @ref ATB_SetFovSatBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateFovSatModule(const char *nucStr, YesNo deriveGains = Yes);




/**************************************************************************/
/*         gen/src/prg/pvm_toolboxes/mr/PvSeqTools/FlowSatHandler.c       */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_FlowSatPulEnumRel (void);
DLL_INTERFACE void STB_FlowSatPulRange (void);
DLL_INTERFACE void STB_FlowSatPulRel (void);
DLL_INTERFACE void STB_FlowSatPulAmplRel (void);
DLL_INTERFACE void STB_FlowSatGeoObjRel (void);
DLL_INTERFACE void STB_FlowSatGeoCubRel (void);
DLL_INTERFACE void STB_InFlowSatSpoilerRel (void);
DLL_INTERFACE void STB_InFlowSatGradLimit (void);
DLL_INTERFACE void STB_InFlowSatGradLimitRange (void);
DLL_INTERFACE void STB_InFlowSatGrad (void);
DLL_INTERFACE void STB_InFlowSatGradRange (void);
DLL_INTERFACE void STB_InFlowSatOnOff (void);
DLL_INTERFACE void STB_InFlowSatOnOffRange (void);
DLL_INTERFACE void STB_InFlowSatThick (void);
DLL_INTERFACE void STB_InFlowSatThickRange (void);
DLL_INTERFACE void STB_InFlowSatMinThick (void);
DLL_INTERFACE void STB_InFlowSatMinThickRange (void);
DLL_INTERFACE void STB_InFlowSatSliceGapRel (void);
DLL_INTERFACE void STB_InFlowSatSliceGapRange (void);
DLL_INTERFACE void STB_InFlowSatPosRel (void);
DLL_INTERFACE void STB_FlowSatHandler (void);

/** Initialises the parameters of the <em> Flow Saturation</em> preparation module.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Switch         </td> <td>@c PVM_InFlowSatOnOff                </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Parameter group</td> <td>@c Flow_Sat_Parameters               </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitFlowSaturationModule    </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateFlowSaturationModule </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_FlowSatHandler                </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>BaseLevel      </td> <td>@ref ATB_SetFlowSaturationBaseLevel  </td> <td>@c BaseLevelRelations.c</td></tr>
  <tr><td>Duration       </td> <td>@c PVM_InFlowSatModuleTime           </td> <td>@c backbone()          </td></tr>
  <tr><td>Include        </td> <td>@c FlowSat.mod or @n
                                   @c PrepModulesHead.mod               </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Subroutine     </td> <td>@c FlowSat, FlowSatInc, FlowSatInit  </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Parameter card </td> <td>@c FlowSat.xml                       </td> <td><c>\<method\>.xml</c>  </td></tr>
  <tr><td>Setup card     </td> <td>@c FlowSatSetup.xml                  </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>

  @PVM_PARS
  - @c PVM_InFlowSatOnOff
  - @c PVM_FlowSatPul
  - @c PVM_FlowSatPulEnum
  - @c PVM_FlowSatPulAmpl
  - @c PVM_FlowSatPulShape
  - @c PVM_InFlowSatNSlices
  - @c PVM_InFlowSatThick
  - @c PVM_InFlowSatGrad
  - @c PVM_InFlowSatSliceGap
  - @c PVM_InFlowSatPos
  - @c PVM_InFlowSatSpoil


  @PVM_TBCALL
  - @ref STB_InitRFPulse
  - @ref MRT_InitSpoiler

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateFlowSaturationModule, @ref ATB_SetFlowSaturationBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitFlowSaturationModule(void);

/** Updates the parameters of the <em> Flow Saturation</em> preparation module.

  @PVM_PARS
  - @c PVM_FlowSatPul
  - @c PVM_InFlowSatSliceGap
  - @c PVM_InFlowSatPos
  - @c PVM_InFlowSatSpoil
  - @c PVM_InFlowSatNSlices
  - @c PVM_InFlowSatThick
  - @c PVM_InFlowSatGrad
  - @c PVM_InFlowSatModuleTime

  @PVM_TBCALL
  - @ref CFG_IsNucleusKnown
  - @ref STB_UpdateRFPulse
  - @ref PTB_RemoveGeoObject
  - @ref PTB_InsertGeoObject
  - @ref MRT_SliceThickness
  - @ref MRT_UpdateSpoiler
  - @ref CFG_AmplifierEnable
  - @ref CFG_GradientRiseTime
  - @ref CFG_GradCalConst

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The following parameters are used and must have values:
    - @c PVM_NSPacks
    - @c PVM_SPackArrNSlices
    - @c PVM_ObjOrderList
    - @c PVM_EffSliceOffset
    - @c PVM_SliceThick
    - @c PVM_SPackArrSliceGap
  - The nuclei parameter group must have been updated.

  @PVM_ERROR
  - Invalid nucleus in @a nucStr.

  @PVM_EXAMPLE
  @code
  STB_UpdateFlowSaturationModule(PVM_Nucleus1, PVM_DeriveGains);
  @endcode

  @param  nucStr       nucleus
  @param  deriveGains  (optional) @c Yes = gains will be derived from reference power (default)


  @see @ref STB_InitFlowSaturationModule, @ref ATB_SetFlowSaturationBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateFlowSaturationModule(const char *const nucStr,YesNo deriveGains = Yes);


/**************************************************************************/
/*           gen/src/prg/pvm_toolboxes/mr/PvSeqTools/OvsHandler.c         */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_OvsOnOffRange (void);
DLL_INTERFACE void STB_OvsOnOff (void);
DLL_INTERFACE void STB_OvsGapToVoxelRange (void);
DLL_INTERFACE void STB_OvsGapToVoxel (void);
DLL_INTERFACE void STB_OvsSliceThicknessRange (void);
DLL_INTERFACE void STB_OvsSliceThickness (void);
DLL_INTERFACE void STB_OvsMaxGradientRange (void);
DLL_INTERFACE void STB_OvsMaxGradient (void);
DLL_INTERFACE void STB_OvsSpoilerStrengthRange (void);
DLL_INTERFACE void STB_OvsSpoilerStrength (void);
DLL_INTERFACE void STB_OvsSpoilerDurationRange (void);
DLL_INTERFACE void STB_OvsSpoilerDuration (void);
DLL_INTERFACE void STB_OvsPulse1Range (void);
DLL_INTERFACE void STB_OvsPulse1 (void);
DLL_INTERFACE void STB_OvsPulse1Ampl (void);
DLL_INTERFACE void STB_OvsPulse1Enum (void);
DLL_INTERFACE void STB_HandleOvsPulseAmplitude(void);

/** Initialises the parameters of the <em> outer volume suppression </em> module.

    Used to improve the definition of a cubic voxel in localized spectroscopy by
    applying saturation slabs at each side of the volume. For free adjustable slice
    orientations, use @ref FovSat. The OVS can be applied interleaved during the
    @ref WaterSup module by using @c subr WsOvs.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Switch         </td> <td>@c PVM_OvsOnOff           </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Parameter group</td> <td>@c OuterVolumeSuppression </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitOvsModule    </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateOvsModule  </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_OvsHandler         </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>BaseLevel      </td> <td>@ref ATB_SetOvsBaseLevel  </td> <td>@c BaseLevelRelations.c</td></tr>
  <tr><td>Duration       </td> <td>@c PVM_OvsModuleDuration  </td> <td>@c backbone()          </td></tr>
  <tr><td>Include        </td> <td>@c WsOvs.mod or @n
                                   @c PrepModulesHead.mod    </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Subroutine     </td> <td>@c Ovs or @c WsOvs        </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Parameter card </td> <td>@c Ovs.xml                </td> <td><c>\<method\>.xml</c>  </td></tr>
  <tr><td>Setup card     </td> <td>@c OvsSetup.xml           </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>

  @PVM_PARS
  - @c PVM_OvsMaxGradient
  - @c PVM_OvsOnOff
  - @c PVM_OvsPulse1
  - @c PVM_OvsPulseEnum1
  - @c PVM_OvsSliceThickness
  - @c PVM_OvsGapToVoxel
  - @c PVM_OvsSpoilerStrength
  - @c PVM_OvsSpoilerDuration

  @PVM_TBCALL
  - @ref STB_InitRFPulse

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_InitOvsModule( 100.0, PVM_VoxMinDistance );
  @endcode


  @param maxGradient    Maximum gradient strength
  @param minDistance    Minimum distance between voxels in case of multi voxel
                        acquisition

  @see @ref STB_UpdateOvsModule, @ref ATB_SetOvsBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitOvsModule( double maxGradient, double minDistance);

/** Updates the parameters of the <em> outer volume suppression </em> module.

  The @c PVM_OvsOnOff parameter is  not included in the @c OuterVolumeSuppression
  class. In most cases they are directly included in the @c method class.

  @PVM_PARS
  - @c PVM_OvsModuleDuration
  - @c PVM_OvsPulse
  - @c PVM_OvsPulseEnum
  - @c PVM_OvdGapToVoxel
  - @c PVM_OvsConflict
  - @c PVM_OvsSliceTickness
  - @c PVM_OvsSpoilerDuration
  - @c PVM_OvsSpoilerStrength
  - @c PVM_OvsChemShiftDisplacement
  - @c PVM_OvsFrequencyList

  @PVM_TBCALL
  - @ref STB_GetRefAtt
  - @ref STB_CheckRFPulse
  - @ref STB_UpdateRFPulse
  - @ref STB_UpdateExcPulseEnum
  - @ref ATB_GetTransmitterFreq
  - @ref CFG_GradCalConst

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The following parameters are used and must have values:
    - @c PVM_GradCalConst
    - @c PVM_RiseTime
  - The nuclei parameter group must have been updated.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_UpdateOvsModule( PVM_VoxMinDistance,
                       PtrType1x3 PVM_VoxArrSize,
                       PtrType1x3 PVM_VoxArrPositionRPS,
                       PVM_NVoxels,
                       PVM_DeriveGains,
                       PVM_Nucleus1);
  @endcode


  @param minDistance       Minimum distance between two voxels in multi voxels case
  @param voxelSize         Voxel size array
  @param voxelPosition     Voxel position array
  @param nvoxels           Number of voxels
  @param methodGainFlag   @c Yes to derive OVS pulse gains from reference gain, @c No otherwise.
  @param nucStr            Nucleus


  @see @ref STB_InitOvsModule, @ref ATB_SetOvsBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateOvsModule( double            minDistance,
                                        double          (*voxelSize)[3],
                                        double          (*voxelPosition)[3],
                                        int               nvoxels,
                                        YesNo             methodGainFlag,
                                        const char *const nucStr);




/**************************************************************************/
/*            gen/src/prg/pvm_toolboxes/mr/PvSeqTools/WsHandler.c         */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_WsModeRange (void);
DLL_INTERFACE void STB_WsMode (void);
DLL_INTERFACE void STB_WsOnOffRange (void);
DLL_INTERFACE void STB_WsDeriveGainMode (void);
DLL_INTERFACE void STB_WsDeriveGainModeRange (void);
DLL_INTERFACE void STB_WsOnOff (void);
DLL_INTERFACE void STB_WsOffsetHzRange (void);
DLL_INTERFACE void STB_WsOffsetHz (void);
DLL_INTERFACE void STB_WsOffsetppmRange (void);
DLL_INTERFACE void STB_WsOffsetppm (void);
DLL_INTERFACE void STB_WsOffsetPPMRange (void);
DLL_INTERFACE void STB_WsOffsetPPM (void);
DLL_INTERFACE void STB_WsCalcSpoilerRange (void);
DLL_INTERFACE void STB_WsCalcSpoiler (void);
DLL_INTERFACE void STB_WsCalcTimingRange (void);
DLL_INTERFACE void STB_WsCalcTiming (void);
DLL_INTERFACE void STB_WsMeanT1Range (void);
DLL_INTERFACE void STB_WsMeanT1 (void);
DLL_INTERFACE void STB_WsLockGainsRange(void);
DLL_INTERFACE void STB_WsLockGains(void);
DLL_INTERFACE void STB_WsSpoilerStrengthRange (void);
DLL_INTERFACE void STB_WsSpoilerStrength (void);
DLL_INTERFACE void STB_WsBandwidthRange (void);
DLL_INTERFACE void STB_WsBandwidth (void);
DLL_INTERFACE void STB_ChSpoilerStrengthRange (void);
DLL_INTERFACE void STB_ChSpoilerStrength (void);
DLL_INTERFACE void STB_ChInterPulseDelayRange (void);
DLL_INTERFACE void STB_ChInterPulseDelay (void);
DLL_INTERFACE void STB_ChSpoilerOnDurationRange (void);
DLL_INTERFACE void STB_ChSpoilerOnDuration (void);
DLL_INTERFACE void STB_ChPul1Range (void);
DLL_INTERFACE void STB_ChPul1 (void);
DLL_INTERFACE void STB_ChPul1Enum (void);
DLL_INTERFACE void STB_ChPul1Ampl (void);
DLL_INTERFACE void STB_ChPul2Range (void);
DLL_INTERFACE void STB_ChPul2 (void);
DLL_INTERFACE void STB_ChPul2Enum (void);
DLL_INTERFACE void STB_ChPul2Ampl (void);
DLL_INTERFACE void STB_ChPul3Range (void);
DLL_INTERFACE void STB_ChPul3 (void);
DLL_INTERFACE void STB_ChPul3Enum (void);
DLL_INTERFACE void STB_ChPul3Ampl (void);
DLL_INTERFACE void STB_HandleChPulAmplitude(void);
DLL_INTERFACE void STB_VpSpoilerStrengthRange (void);
DLL_INTERFACE void STB_VpSpoilerStrength (void);
DLL_INTERFACE void STB_VpInterPulseDelayRange (void);
DLL_INTERFACE void STB_VpInterPulseDelay (void);
DLL_INTERFACE void STB_VpSpoilerOnDurationRange (void);
DLL_INTERFACE void STB_VpSpoilerOnDuration (void);
DLL_INTERFACE void STB_VpPul1Range (void);
DLL_INTERFACE void STB_VpPul1 (void);
DLL_INTERFACE void STB_VpPul1Enum (void);
DLL_INTERFACE void STB_VpPul1Ampl (void);
DLL_INTERFACE void STB_VpPul2Range (void);
DLL_INTERFACE void STB_VpPul2 (void);
DLL_INTERFACE void STB_VpPul2Enum (void);
DLL_INTERFACE void STB_VpPul2Ampl (void);
DLL_INTERFACE void STB_HandleVpPulAmplitude(void);
DLL_INTERFACE void STB_WsDeriveGainsRange (void);
DLL_INTERFACE void STB_WsDeriveGains (void);

/** Initialises the parameters of the <em> solvent suppression </em> module.

  The solvent suppression module is used to suppress water (or other solvent) line in spectroscopy. Three modes - NO_SUPPRESSION, VAPOR and CHESS -can be selected with the @c PVM_WsMode parameter.

If the @ref OVS module is activated, it is applied interleaved  between the solvent suppression pulses.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Switch         </td> <td>@c PVM_WsMode               </td> <td> <c>\<method\>.xml</c></td></tr>
  <tr><td>Parameter group</td> <td>@c Suppression              </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitWsModule       </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateWsModule     </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c   PVM_WsHandler          </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>BaseLevel      </td> <td>@ref ATB_SetWsBaseLevel     </td> <td>@c BaseLevelRelations.c</td></tr>
  <tr><td>Duration       </td> <td>@c   PVM_WsModuleDuration   </td> <td>@c backbone()          </td></tr>
  <tr><td>Include        </td> <td>@c   WsOvs.mod or @n
                                   @c PrepModulesHead.mod      </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Subroutine     </td> <td>@c WsOvs                    </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Parameter card </td> <td>@c Suppression.xml          </td> <td><c>\<method\>.xml</c>  </td></tr>
  <tr><td>Setup card     </td> <td>@c SuppressionSetup.xml     </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>

Note, that the module is de-/activated by the parameter @c PVM_WsMode.
The parameter @c PVM_WsOnOff controls the application of the RF pulses only, while keeping the module activated. This maintains the sequence structure for setup and debugging purposes.

  @PVM_PARS
  - @c PVM_WsOffsetHz
  - @c PVM_WsOffsetPPM
  - @c PVM_ChPul1
  - @c PVM_ChPul1Enum
  - @c PVM_ChPul2
  - @c PVM_ChPul2Enum
  - @c PVM_ChPul3
  - @c PVM_ChPul3Enum
  - @c PVM_ChInterPulseDelay
  - @c PVM_ChSpoilerOnDuration
  - @c PVM_ChSpoilerStrength
  - @c PVM_VpPul1
  - @c PVM_VpPul1Enum
  - @c PVM_VpPul2
  - @c PVM_VpPulse2Enum
  - @c PVM_VpInterPulseDelay
  - @c PVM_VpSpoilerOnDuration
  - @c PVM_VpSpoilerStrength
  - @c PVM_WsModuleDuration

  @PVM_TBCALL
  - STB_WsOnOffRange
  - STB_WsModeRange
  - STB_WsMeanT1Range
  - STB_WsOffsetHzRange
  - STB_WsOffsetppmRange
  - STB_WsCalcSpoilerRange
  - STB_WsCalcTimingRange
  - STB_WsSpoilerStrengthRange
  - STB_WsBandwidthRange
  - STB_WsDeriveGainModeRange

  @PVM_RELCALL
  NONE

  @PVM_PRECOND

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_InitWsModule (PVM_NVoxels);
  @endcode

  @param nOffsets    Number of voxels

  @todo Incomplete documentation: PVM_PRECOND

  @see @ref STB_UpdateWsModule, @ref ATB_SetWsBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitWsModule (int nOffsets);



/** Updates the parameters of the <em> solvent suppression </em> module.

  @PVM_PARS
  - @c PVM_WsDeriveGains
  - @c PVM_WsOffsetHz
  - @c PVM_WsOffsetPPM
  - @c PVM_ChPul1
  - @c PVM_ChPul1Enum
  - @c PVM_ChPul2
  - @c PVM_ChPul2Enum
  - @c PVM_ChPul3
  - @c PVM_ChPulse3Enum
  - @c PVM_ChInterPulseDelay
  - @c PVM_ChSpoilerStrength
  - @c PVM_ChSpoilerOnDuration
  - @c PVM_ChppdD1
  - @c PVM_ChppdD2
  - @c PVM_VpPul1
  - @c PVM_VpPul1Enum
  - @c PVM_VpPul2
  - @c PVM_VpPul2Enum
  - @c PVM_VpInterPulseDelay
  - @c PVM_VpSpoilerStrength
  - @c PVM_VpSpoilerOnDuration
  - @c PVM_VpppgD1
  - @c PVM_VpppgD2
  - @c PVM_WsModuleDuration

  @PVM_TBCALL
  - @ref STB_UpdateRFPulse
  - @ref ATB_GetTransmitterFreq
  - @ref UT_GetRequest

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The following parameter is used and must have a value:
    - @c PVM_RiseTime
  - The nuclei parameter group must have been updated.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_UpdateWsModule(PVM_OvsModuleDuration, PVM_NVoxels);
  @endcode

  @param ovsDuration       Duration of the <em> outer volume suppression
                           </em> module if it is used
  @param nOffsets          Number of voxels

  @param excDelay          Delay in [ms] between end of the solvent suppression module and the excitation (center of excitation pulse). This delay is included in the last last InterPulseDelay of the module. The argument is optional and its default value is 0.0.

  @see @ref STB_InitWsModule, @ref ATB_SetWsBaseLevel


  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateWsModule(double ovsDuration, int nOffsets, double excDelay = 0.0);




/**************************************************************************/
/*           gen/src/prg/pvm_toolboxes/mr/PvSeqTools/DecHandler.c         */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_DecOnOffRelation (void);
DLL_INTERFACE void STB_DecOnOffRange (void);
DLL_INTERFACE void STB_DecModeRelation (void);
DLL_INTERFACE void STB_DecModeRange (void);
DLL_INTERFACE void STB_DecPulseEnumRelation (void);
DLL_INTERFACE void STB_DecPulseEnumRange (void);
DLL_INTERFACE void STB_DecUserPulseRelation (void);
DLL_INTERFACE void STB_DecUserPulseRange (void);
DLL_INTERFACE void STB_DecTimeRelation (void);
DLL_INTERFACE void STB_DecTimeRange (void);
DLL_INTERFACE void STB_DecDurRelation (void);
DLL_INTERFACE void STB_DecDurRange (void);
DLL_INTERFACE void STB_DeriveDecPowerYesNoRelation (void);
DLL_INTERFACE void STB_DeriveDecPowerYesNoRange (void);
DLL_INTERFACE void STB_DecPowerRelation (void);
DLL_INTERFACE void STB_DecPowerRange (void);
DLL_INTERFACE void STB_DecNutationFreqRelation (void);
DLL_INTERFACE void STB_DecNutationFreqRange (void);
DLL_INTERFACE void STB_DecPulseElementDurationRelation (void);
DLL_INTERFACE void STB_DecPulseElementDurationRange (void);
DLL_INTERFACE void STB_DecOffsetppmRelation (void);
DLL_INTERFACE void STB_DecOffsetppmRange (void);
DLL_INTERFACE void STB_DecReferenceppmRelation (void);
DLL_INTERFACE void STB_DecReferenceppmRange (void);
DLL_INTERFACE void STB_DecCycleDurRelation (void);

/** Initialises the parameter of the @em decoupling module.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Switch         </td> <td>@c PVM_DecOnOff             </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Parameter group</td> <td>@c Decoupling_Parameters    </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitDecModule      </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateDecModule    </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c   PVM_DecHandler         </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>BaseLevel      </td> <td>@ref ATB_SetDecBaseLevel    </td> <td>@c BaseLevelRelations.c</td></tr>
  <tr><td>Include        </td> <td>@c acqdec.mod               </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Subroutine     </td> <td>@c acqdec_jobs              </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Parameter card </td> <td>@c Decoupling.xml           </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>

  @PVM_PARS
   - @c PVM_DecMode
   - @c PVM_DecPower
   - @c PVM_DecTime
   - @c PVM_DecPulseEnum
   - @c PVM_DecPulseElementDuration
   - @c PVM_DeriveDecPowerYesNo
   - @c PVM_DecNutationFreq
   - @c PVM_DecOffsetppm
   - @c PVM_DecReferenceppm
   - @c PVM_DecReferenceMHz
   - @c PVM_DecFreqMHz

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c STB_DecModeRange
  - @c STB_DecPowerRange
  - @c STB_DecTimeRange
  - @c STB_DecPulseEnumRange
  - @c STB_DecUserPulseRange
  - @c STB_DecPulseElementDurationRange
  - @c STB_DeriveDecPowerYesNoRange
  - @c STB_DecNutationFreqRange
  - @c STB_DecOffsetppmRange
  - @c STB_DecReferenceppmRange

  @PVM_ERROR
  NONE

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitDecModule (void);

/** Updates the parameters of the @em decoupling  module.

  The decoupling power and duration are calculated from the derive gain mode and the decoupling
  mode. The decoupling frequency value is calculated for the decoupling nucleus from its value
  in ppm.

  @PVM_PARS
   - @c PVM_DecMode
   - @c PVM_DecPower
   - @c PVM_DecTime
   - @c PVM_DecDuration
   - @c PVM_DecPulseEnum
   - @c PVM_DecPulseElementDuration
   - @c PVM_DeriveDecPowerYesNo
   - @c PVM_DecNutationFreq
   - @c PVM_DecOffsetppm
   - @c PVM_DecReferenceppm
   - @c PVM_DecReferenceMHz
   - @c PVM_DecFreqMHz

  @PVM_TBCALL
  - @ref ATB_GetTransmitterFreq
  - @ref UT_GetRequest
  - @ref STB_GetRefAtt
  - @ref MRT_RFPulseAttenuation

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The nuclei parameter group must have been updated.

  @PVM_ERROR
  NONE

  @param nucStr     Decoupling nucleus
  @param acqtime    Acquisition time

  @todo Incomplete documentation: PVM_EXAMPLE

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateDecModule (const char *const nucStr,
                          double            acqtime);





/**************************************************************************/
/*           gen/src/prg/pvm_toolboxes/mr/PvSeqTools/NoeHandler.c         */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_NoeOnOffRelation (void);
DLL_INTERFACE void STB_NoeOnOffRange (void);
DLL_INTERFACE void STB_NoeModeRelation (void);
DLL_INTERFACE void STB_NoeModeRange (void);
DLL_INTERFACE void STB_NoePulseEnumRelation (void);
DLL_INTERFACE void STB_NoePulseEnumRange (void);
DLL_INTERFACE void STB_NoeUserPulseRelation (void);
DLL_INTERFACE void STB_NoeUserPulseRange (void);
DLL_INTERFACE void STB_NoeTimeRelation (void);
DLL_INTERFACE void STB_NoeTimeRange (void);
DLL_INTERFACE void STB_DeriveNoePowerYesNoRelation (void);
DLL_INTERFACE void STB_DeriveNoePowerYesNoRange (void);
DLL_INTERFACE void STB_NoePowerRelation (void);
DLL_INTERFACE void STB_NoePowerRange (void);
DLL_INTERFACE void STB_NoeNutationFreqRelation (void);
DLL_INTERFACE void STB_NoeNutationFreqRange (void);
DLL_INTERFACE void STB_NoePulseElementDurationRelation (void);
DLL_INTERFACE void STB_NoePulseElementDurationRange (void);
DLL_INTERFACE void STB_NoeOffsetppmRelation (void);
DLL_INTERFACE void STB_NoeOffsetppmRange (void);
DLL_INTERFACE void STB_NoeReferenceppmRelation (void);
DLL_INTERFACE void STB_NoeReferenceppmRange (void);
DLL_INTERFACE void STB_NoeCycleDurRelation (void);

/** Initialises the parameters of the <em> Nuclear Overhauser Enhancement </em> module.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Switch         </td> <td>@c PVM_NoeOnOff             </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Parameter group</td> <td>@c NOE_Parameters           </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitNoeModule      </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateNoeModule    </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c   PVM_NoeHandler         </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>BaseLevel      </td> <td>@ref ATB_SetNoeBaseLevel    </td> <td>@c BaseLevelRelations.c</td></tr>
  <tr><td>Duration       </td> <td>@c PVM_NoeModuleTime        </td> <td>@c backbone()          </td></tr>
  <tr><td>Include        </td> <td>@c Noe.mod  or @n
                                   @c PrepModulesHead.mod      </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Subroutine     </td> <td>@c Noe                      </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Parameter card </td> <td>@c Noe.xml                  </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>

  @PVM_PARS
  - @c PVM_NoeMode
  - @c PVM_NoePower
  - @c PVM_NoeTime
  - @c PVM_NoePulseEnum
  - @c PVM_NoePulseElementDuration
  - @c PVM_DeriveNoePowerYesNo
  - @c PVM_NoeNutationFreq
  - @c PVM_NoeOffsetppm
  - @c PVM_NoeReferenceppm
  - @c PVM_NoeReferenceMHz
  - @c PVM_NoeFreqMHz

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c STB_NoeModeRange
  - @c STB_NoePowerRange
  - @c STB_NoeTimeRange
  - @c STB_NoePulseEnumRange
  - @c STB_NoeUserPulseRange
  - @c STB_NoePulseElementDurationRange
  - @c STB_DeriveNoePowerYesNoRange
  - @c STB_NoeNutationFreqRange
  - @c STB_NoeOffsetppmRange
  - @c STB_NoeReferenceppmRange

  @PVM_ERROR
  NONE

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitNoeModule (void);

/** Updates the parameters of the <em> Nuclear Overhauser Enhancement </em> module.

  The irradiation power and the module duration are calculated from the derive gain mode
  and the noe mode. The irradiation frequency value is calculated for the nucleus from its
  value in ppm.

  @PVM_PARS
  - @c PVM_NoeMode
  - @c PVM_NoePower
  - @c PVM_NoeTime
  - @c PVM_NoeModuleTime
  - @c PVM_NoePulseEnum
  - @c PVM_NoePulseElementDuration
  - @c PVM_DeriveNoePowerYesNo
  - @c PVM_NoeNutationFreq
  - @c PVM_NoeOffsetppm
  - @c PVM_NoeReferenceppm
  - @c PVM_NoeReferenceMHz
  - @c PVM_NoeFreqMHz

  @PVM_TBCALL
  - @ref ATB_GetTransmitterFreq
  - @ref STB_GetRefAtt
  - @ref MRT_RFPulseAttenuation

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The nuclei parameter group must have been updated.

  @PVM_ERROR
  NONE

  @param  nucStr    Decoupling nucleus

  @todo Incomplete documentation: PVM_EXAMPLE

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateNoeModule (const char *const nucStr);





/**************************************************************************/
/*        gen/src/prg/pvm_toolboxes/mr/PvSeqTools/EvolutionHandler.c      */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_EvolutionRelations(void);

/** Initialises the parameters of the @em Evolution module.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Switch         </td> <td>@c PVM_EvolutionOnOff          </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Parameter group</td> <td>@c Evolution_Parameters        </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitEvolutionModule   </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateEvolutionModule </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_EvolutionHandler        </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>BaseLevel      </td> <td>@ref ATB_EvolutionSetTimePoints @n
                                   @ref ATB_SetNr                 <td>@c BaseLevelRelations.c</td></tr>

  <tr><td>Duration       </td> <td>@c PVM_EvolutionModuleTime     </td> <td>@c backbone()          </td></tr>
  <tr><td>Include        </td> <td>@c Evolution.mod or @n
                                   @c PrepModulesHead.mod         </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Subroutine     </td> <td>@c Evolution                   </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Parameter card </td> <td>@c Evolution.xml               </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>

  Set @c NR including the number of evolution cycles given by @c PVM_EvolutionCycles by using @ref ATB_SetNR, e.g.
  @code
  ATB_SetNR(PVM_NRepetitions * PVM_EvolutionCycles);
  @endcode

  Note that in this module time parameters are given in sec, not in msec as usual.

  @PVM_PARS
  - @c PVM_EvolutionOnOff
  - @c PVM_EvolutionDelay
  - @c PVM_EvolutionTime
  - @c PVM_NEvolutionCycles
  - @c PVM_EvolutionMode

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c STB_EvolutionOnOffRange
  - @c STB_EvolutionModeRange
  - @c STB_EvolutionDelayRange
  - @c STB_EvolutionTimeRange
  - @c STB_NEvolutionCyclesRange

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateEvolutionModule, @ref ATB_EvolutionSetTimePoints

  @todo Problematic issues
  - The time parameters are given in sec, not in msec as usual.
  - The @c NR loop is utilised for evolutions which prevents using the module in methods expanding this loop
    for other purposes.

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitEvolutionModule(void);

/** Updates the parameters of the @em Evolution module.

  @PVM_PARS
  - @c PVM_NEvolutionCycles
  - @c PVM_EvolutionCycles
  - @c PVM_EvolutionTime
  - @c PVM_EvolutionDelay
  - @c PVM_EvolutionModuleTime

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Initialisation of @em Evolution must have been performed with @ref STB_InitEvolutionModule.

  @PVM_ERROR
  NONE

  @param evolDuration (optional) Total module duration (sum of all delays) [ms], as also given by @c PVM_EvolutionModuleTime

  @see @ref STB_InitEvolutionModule, @ref ATB_EvolutionSetTimePoints

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateEvolutionModule(double *evolDuration = NULL);


/**************************************************************************/
/*         gen/src/prg/pvm_toolboxes/mr/PvSeqTools/BlBloodHandler.c       */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_BlBloodSpoilerRel (void);
DLL_INTERFACE void STB_BlBloodGradLimit (void);
DLL_INTERFACE void STB_BlBloodGradLimitRange (void);
DLL_INTERFACE void STB_BlBloodGrad (void);
DLL_INTERFACE void STB_BlBloodGradRange (void);
DLL_INTERFACE void STB_BlBloodInvTime (void);
DLL_INTERFACE void STB_BlBloodInvTimeRange (void);
DLL_INTERFACE void STB_BlBloodOnOff (void);
DLL_INTERFACE void STB_BlBloodOnOffRange (void);
DLL_INTERFACE void STB_BlBloodNSlabs (void);
DLL_INTERFACE void STB_BlBloodNSlabsRange (void);
DLL_INTERFACE void STB_BlBloodSlabThick (void);
DLL_INTERFACE void STB_BlBloodSlabThickRange (void);
DLL_INTERFACE void STB_BlBloodMinThick (void);
DLL_INTERFACE void STB_BlBloodMinThickRange (void);
DLL_INTERFACE void STB_BlBloodSlabOffset (void);
DLL_INTERFACE void STB_BlBloodSlabOffsetRange (void);
DLL_INTERFACE void STB_BlBloodPulEnumRel (void);
DLL_INTERFACE void STB_BlBloodPulRange (void);
DLL_INTERFACE void STB_BlBloodPulRel (void);
DLL_INTERFACE void STB_BlBloodPulAmpl (void);

/** Initialises the parameters of the <em> Black Blood </em> preparation module.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Switch         </td> <td>@c PVM_BlBloodOnOff           </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Parameter group</td> <td>@c BlackBlood_Parameters      </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitBlBloodModule    </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateBlBloodModule  </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_BlBloodHandler         </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>BaseLevel      </td> <td>@ref ATB_SetBlBloodBaseLevel  </td> <td>@c BaseLevelRelations.c</td></tr>
  <tr><td>Duration       </td> <td>@c PVM_BlBloodModuleTime      </td> <td>@c backbone()          </td></tr>
  <tr><td>Include        </td> <td>@c BlBlood.mod or @n
                                   @c PrepModulesHead.mod        </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Subroutine     </td> <td>@c BlBlood                    </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Parameter card </td> <td>@c BlackBlood.xml             </td> <td><c>\<method\>.xml</c>  </td></tr>
  <tr><td>Setup card     </td> <td>@c BlackBloodSetup.xml        </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>


  @PVM_PARS
  - @c PVM_BlBloodOnOff
  - @c PVM_BlBloodInvTime
  - @c PVM_BlBloodPul
  - @c PVM_BlBloodPulEnum
  - @c PVM_BlBloodPulAmpl
  - @c PVM_BlBloodPulShape
  - @c PVM_BlBloodNSlabs
  - @c PVM_BlBloodSlabThick
  - @c PVM_BlBloodSlabOffset
  - @c PVM_BlBloodOffset
  - @c PVM_BlBloodMinThick
  - @c PVM_BlBloodGrad
  - @c PVM_BlBloodGradLimit
  - @c PVM_BlBloodSpoiler


  @PVM_TBCALL
  - @ref STB_InitRFPulse
  - @ref MRT_InitSpoiler

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateBlBloodModule, @ref ATB_SetBlBloodBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitBlBloodModule(void);




/** Updates the parameters of the <em> saturation pulse preparation </em> module.

  The updater calculates the slab offsets, the gradient and spoiler gradient amplitude and the
  delays for inversion. The pulse gain is calculated depending on the @c DeriveGainMode
  parameter.

  @PVM_PARS
  - @c PVM_BlBloodPul
  - @c PVM_BlBloodPulAmpl
  - @c PVM_BlBloodPulShape
  - @c PVM_BlBloodNSlabs
  - @c PVM_BlBloodSlabThick
  - @c PVM_BlBloodSlabOffset
  - @c PVM_BlBloodOffsetHz
  - @c PVM_BlBloodFL
  - @c PVM_BlBloodMinThick
  - @c PVM_BlBloodGrad
  - @c PVM_BlBloodSpoiler
  - @c PVM_BlBloodModuleTime

  @PVM_TBCALL
  - @ref STB_UpdateRFPulse
  - @ref CFG_AmplifierEnable
  - @ref CFG_GradientRiseTime
  - @ref MRT_UpdateSpoiler

  @PVM_PRECOND
 - The module must have been initialised with @ref STB_InitBlBloodModule.

  @PVM_ERROR
  - If more than one slice package is used for inversion

  @PVM_EXAMPLE
  @code
  STB_UpdateBlBloodModule(&slabthick,PVM_SPackArrSliceOffset,1,fixedTime,PVM_SliceThick,PVM_DeriveGains);
  @endcode

  @param  minSlabThick     Minimum thickness of each inversion slab
  @param  slabOffset       Offset of each slab in millimeters
  @param  invSlabNumber    Only 1 slice package for inversion is supported
  @param  fixedtime        Fixed time at the beginning of the sequence to be included in the
                           inversion time.
  @param  spoilerThick     Scale basis for the spoiling efficiency e.g. slice thickness of the method
  @param  deriveGains      (optional) @c Yes = gains will be derived from reference power (default)

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateBlBloodModule(double  *minSlabThick,
                                           double  *slabOffset,
                                           int      invSlabNumber,
                                           double   fixedtime,
                                           double   spoilerThick=1,
                                           YesNo    deriveGains=Yes);




/**************************************************************************/
/*          gen/src/prg/pvm_toolboxes/mr/PvSeqTools/TaggingHandler.c      */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_TaggingSpoilerRel (void);
DLL_INTERFACE void STB_TaggingDirRel (void);
DLL_INTERFACE void STB_TaggingDirRange (void);
DLL_INTERFACE void STB_TaggingModeRel (void);
DLL_INTERFACE void STB_TaggingModeRange (void);
DLL_INTERFACE void STB_TaggingDistanceRel (void);
DLL_INTERFACE void STB_TaggingDistanceRange (void);
DLL_INTERFACE void STB_TaggingMinDistanceRel (void);
DLL_INTERFACE void STB_TaggingMinDistanceRange (void);
DLL_INTERFACE void STB_TaggingThickRel (void);
DLL_INTERFACE void STB_TaggingThickRange (void);
DLL_INTERFACE void STB_TaggingPulseElement (void);
DLL_INTERFACE void STB_TaggingPulseElementRange (void);
DLL_INTERFACE void STB_TaggingOnOffRel (void);
DLL_INTERFACE void STB_TaggingOnOffRange (void);
DLL_INTERFACE void STB_TaggingAngleRel (void);
DLL_INTERFACE void STB_TaggingAngleRange (void);
DLL_INTERFACE void STB_TaggingGradientStrengthRel (void);
DLL_INTERFACE void STB_TaggingGradientStrengthRange (void);
DLL_INTERFACE void STB_TaggingOffset1Rel (void);
DLL_INTERFACE void STB_TaggingOffset1Range (void);
DLL_INTERFACE void STB_TaggingOffset2Rel (void);
DLL_INTERFACE void STB_TaggingOffset2Range (void);
DLL_INTERFACE void STB_TaggingDelayRel (void);
DLL_INTERFACE void STB_TaggingDelayRange (void);
DLL_INTERFACE void STB_TaggingGridDelayRel (void);
DLL_INTERFACE void STB_TaggingGridDelayRange (void);
DLL_INTERFACE void STB_TaggingPulEnumRel (void);
DLL_INTERFACE void STB_TaggingPulRange (void);
DLL_INTERFACE void STB_TaggingPulRel (void);
DLL_INTERFACE void STB_TaggingPulAmpl (void);

/** Initialises the parameters of the <em> Tagging </em> preparation module.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Switch         </td> <td>@c PVM_TaggingOnOff           </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Parameter group</td> <td>@c Tagging_Parameters         </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitTaggingModule    </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateTaggingModule  </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_TaggingHandler         </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>BaseLevel      </td> <td>@ref ATB_SetTaggingBaseLevel  </td> <td>@c BaseLevelRelations.c</td></tr>
  <tr><td>Duration       </td> <td>@c PVM_TaggingModuleTime      </td> <td>@c backbone()          </td></tr>
  <tr><td>Include        </td> <td>@c Tagging.mod or @n
                                   @c PrepModulesHead.mod        </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Subroutine     </td> <td>@c Tagging                    </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Parameter card </td> <td>@c Tagging.xml                </td> <td><c>\<method\>.xml</c>  </td></tr>
  <tr><td>Setup card     </td> <td>@c TaggingSetup.xml           </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>

  @PVM_PARS
  - @c PVM_TaggingPul
  - @c PVM_TaggingPulEnum
  - @c PVM_TaggingPulAmpl
  - @c PVM_TaggingPulShape
  - @c PVM_TaggingMode
  - @c PVM_TaggingDistance
  - @c PVM_TaggingThick
  - @c PVM_TaggingPulseNumber
  - @c PVM_TaggingOffset1
  - @c PVM_TaggingOffset2
  - @c PVM_TaggingModuleTime

  @PVM_TBCALL
  - @ref STB_InitRFPulse
  - @ref MRT_InitSpoiler

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateTaggingModule, @ref ATB_SetTaggingBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitTaggingModule(void);

/** Updates the parameters of the <em> Tagging </em> preparation module.
  @PVM_USEINSTEAD{STB_UpdateTaggingModule}

  @PVM_DEACT
  20170101

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE void STB_UpdateTaggingModule(double            *fov,
                                           int               *matrix,
                                           double            spoilerThick=1,
                                           YesNo             deriveGains=Yes);

/** Updates the parameters of the <em> Tagging </em> preparation module.

  @PVM_PARS
  - @c PVM_TaggingPul
  - @c PVM_TaggingPulEnum
  - @c PVM_TaggingPulAmpl
  - @c PVM_TaggingPulShape
  - @c PVM_TaggingFL
  - @c PVM_TaggingDistance
  - @c PVM_TaggingInterPulseDelay
  - @c PVM_TaggingPulseNumber
  - @c PVM_TaggingPulseElement
  - @c PVM_TaggingModuleTime

  @PVM_TBCALL
  - @ref STB_UpdateRFPulse
  - @ref MRT_UpdateSpoiler
  - @ref CFG_GradientRiseTime

  @PVM_PRECOND
  - The module must have been initialised with @ref STB_InitTaggingModule.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_UpdateTaggingModule(PVM_Fov,PVM_Matrix,PVM_SpatResol[0],PVM_DeriveGains);
  @endcode

  @param dim       Number of spatial dimensions
  @param fov       Image field of view array
  @param matrix    Image matrix array
  @param  spoilerThick     Scale basis for the spoiling efficiency
  @param  deriveGains      (optional) @c Yes = gains will be derived from reference power (default)

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateTaggingModule(int      dim,
                                           double  *fov,
                                           int     *matrix,
                                           double   spoilerThick=1,
                                           YesNo    deriveGains=Yes);


/**************************************************************************/
/*          gen/src/prg/pvm_toolboxes/mr/PvSeqTools/TriggerHandler.c      */
/**************************************************************************/

/* range checkers and relations */

DLL_INTERFACE void STB_TriggerRelations(void);

/** Initialises the parameters of the <em> trigger preparation </em> module.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Switch         </td> <td>@c PVM_TriggerModule        </td> <td>@c parsLayout.h      </td></tr>
  <tr><td>Parameter group</td> <td>@c Trigger_Parameters       </td> <td>@c parsLayout.h      </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitTriggerModule  </td> <td>@c initMeth()        </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateTriggerModule</td> <td>@c backbone()        </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_TriggerHandler       </td> <td>@c callbackDefs.h    </td></tr>
  <tr><td>Duration       </td> <td>@c PVM_TriggerModuleTime    </td> <td>@c backbone()        </td></tr>
  <tr><td>Include        </td> <td>@c Trigger.mod or @n
                                   @c PrepModulesHead.mod      </td> <td><c>\<method\>.ppg</c></td></tr>
  <tr><td>Subroutine     </td> <td>@c TriggerSlice @n
                                   @c TriggerPhase             </td> <td><c>\<method\>.ppg</c></td></tr>
  <tr><td>Parameter card </td> <td>@c Trigg.xml                </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>

  @PVM_PARS
  - @c PVM_TriggerDur
  - @c PVM_TriggerModule
  - @c PVM_TriggerDelay
  - @c PVM_TriggerMode

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateTriggerModule, @ref STB_InitTriggerOutModule

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitTriggerModule(void);

/** Updates the parameters of the <em> trigger preparation </em> module.

  @PVM_PARS
  - @c PVM_TriggerModuleTime

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Module must have been initialised with @ref STB_InitTriggerModule.

  @PVM_ERROR
  NONE

  @return Module duration [ms]

  @see @ref STB_InitTriggerModule, @ref STB_UpdateTriggerOutModule

  @ingroup PVM_STB
*/
DLL_INTERFACE double STB_UpdateTriggerModule(void);

/* range checkers and relations */
DLL_INTERFACE void STB_TriggerOutRelations(void);

/** Initialises the parameters of the <em> trigger-out </em> module.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Switch         </td> <td>@c PVM_TriggerOutOnOff         </td> <td>@c parsLayout.h      </td></tr>
  <tr><td>Parameter group</td> <td>@c TriggerOut_Parameters       </td> <td>@c parsLayout.h      </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitTriggerOutModule  </td> <td>@c initMeth()        </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateTriggerOutModule</td> <td>@c backbone()        </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_TriggerOutHandler       </td> <td>@c callbackDefs.h    </td></tr>
  <tr><td>Duration       </td> <td>@c PVM_TriggerOutModuleTime    </td> <td>@c backbone()        </td></tr>
  <tr><td>Include        </td> <td>@c TriggerOut.mod or @n
                                   @c PrepModulesHead.mod         </td> <td><c>\<method\>.ppg</c></td></tr>
  <tr><td>Subroutine     </td> <td>@c TriggerOutStart @n
                                   @c TriggerOutSlice @n
                                   @c TriggerOutVolume            </td> <td><c>\<method\>.ppg</c></td></tr>
  <tr><td>Parameter card </td> <td>@c TriggOut.xml                </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>

  @PVM_PARS
  - @c PVM_TriggerOutDur
  - @c PVM_TriggerOutModule
  - @c PVM_TriggerOutDelay
  - @c PVM_TriggerOutMode

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateTriggerOutModule, @ref STB_InitTriggerModule

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitTriggerOutModule(void);

/** Updates the parameters of the <em> trigger-out </em> module.

  @PVM_PARS
  - @c PVM_TriggerOutModuleTime

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Module must have been initialised with @ref STB_InitTriggerOutModule.

  @PVM_ERROR
  NONE

  @see @ref STB_InitTriggerOutModule, @ref STB_UpdateTriggerModule

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateTriggerOutModule(void);

/**************************************************************************/
/*           gen/src/prg/pvm_toolboxes/mr/PvSeqTools/spectroscopy.c       */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_SpecDimEnum (void);
DLL_INTERFACE void STB_SpecMatrix (void);
DLL_INTERFACE void STB_SpecOffsetHz (void);
DLL_INTERFACE void STB_SpecOffsetppm (void);
DLL_INTERFACE void STB_SpecDwellTime (void);
DLL_INTERFACE void STB_SpecSW (void);
DLL_INTERFACE void STB_SpecSWH (void);
DLL_INTERFACE void STB_SpecNomRes (void);
DLL_INTERFACE void STB_SpecAcquisitionTime (void);

/** Intialisation of spectroscopy parameters.

  This function initialises parameters for
  - spectral dimensionality
  - matrix size
  - spectral resolution, etc.

  This function is intended for the the method initialization part. With the call of this
  function all parameters of the spectroscopy acquisition group obtain consistent values and
  allowed ranges.

  @PVM_PARS
  - @c PVM_SpecDimEnum;
  - @c PVM_SpecAcquisitionTime;
  - @c PVM_SpecOffsetHz;
  - @c PVM_SpecOffsetppm;
  - @c PVM_SpecMatrix;
  - @c PVM_SpecSWH;
  - @c PVM_SpecSW;
  - @c PVM_SpecDwellTime;
  - @c PVM_SpecNomRes;

  @PVM_TBCALL
  - @ref STB_InitDigPars
  - @ref STB_UpdateSpectroscopy

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_InitSpectroscopy(1, 1, 1, PVM_Nucleus1, 200.0, 1000000.0);
  @endcode

  @param     dim        Default number of dimensions
  @param     minDim     Min number of dimensions
  @param     maxDim     Max number of dimensions
  @param[in] nucleus    Nucleus name
  @param     minSWH     Max allowed bandwidth in Hz
  @param     maxSWH     Min allowed bandwidth in Hz

  @see STB_UpdateSpectroscopy

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitSpectroscopy(int         dim,
                          int         minDim,
                          int         maxDim,
                          const char* nucleus,
                          double      minSWH,
                          double      maxSWH);

/** Updating spectroscopy parameters.

  This function handles interdependencies between parameters listed below.

  @PVM_PARS
  - @c PVM_SpecDimEnum;
  - @c PVM_SpecAcquisitionTime;
  - @c PVM_SpecOffsetHz;
  - @c PVM_SpecOffsetppm;
  - @c PVM_SpecMatrix;
  - @c PVM_SpecSWH;
  - @c PVM_SpecSW;
  - @c PVM_SpecDwellTime;
  - @c PVM_SpecNomRes;

  @PVM_TBCALL
  - @ref MRT_GetSpecDim
  - @ref UT_GetRequest
  - @ref ATB_GetTransmitterFreq
  - @ref STB_UpdateDigPars
  - @ref UT_ArrParsHaveSameDimension

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The spectroscopy parameters must have been initialised with @ref STB_InitSpectroscopy.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_UpdateSpectroscopy(PVM_Nucleus1);
  @endcode

  @param[in] nucleus     Nucleus name

  @see @ref STB_InitSpectroscopy

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateSpectroscopy(const char* nucleus);

/**************************************************************************/
/*              gen/src/prg/pvm_toolboxes/mr/PvSeqTools/voxel.c           */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_VoxCallBack(void);
DLL_INTERFACE void STB_NVoxels (void);
DLL_INTERFACE void STB_VoxMinSize (void);
DLL_INTERFACE void STB_VoxMinDistance (void);
DLL_INTERFACE void STB_VoxArrSize (void);
DLL_INTERFACE void STB_VoxSize (void);
DLL_INTERFACE void STB_VoxArrGradOrient (void);
DLL_INTERFACE void STB_VoxArrPosition (void);
DLL_INTERFACE void STB_VoxPosition (void);
DLL_INTERFACE void STB_VoxExcOrder (void);
DLL_INTERFACE void STB_VoxMethodType (void);


/** Intialisation of voxel geometry parameters.

  @PVM_PARS
  - @c PVM_MaxNVoxels
  - @c PVM_NVoxels
  - @c PVM_VoxArrGradOrient
  - @c PVM_VoxArrSize
  - @c PVM_VoxArrPosition
  - @c PVM_VoxMaxGradient
  - @c PVM_VoxelGeoObj
  - @c PVM_VoxelGeoCub


  @PVM_TBCALL
  - @ref PTB_InsertGeoObject
  - @ref GTB_GeoObjInitDefaultPlanningMode

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_InitVoxelGeometry(1);
  @endcode

  @param maxNVoxels  Maximum number of voxels allowed for the method

  @see STB_UpdateVoxelGeometry

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitVoxelGeometry(int maxNVoxels);

/** Updating voxel geometry parameters.

  This function handles interdependencies between parameters describing voxel geometry.

  @PVM_PARS
  - @c PVM_NVoxels
  - @c PVM_VoxMinSize
  - @c PVM_VoxArrSize
  - @c PVM_VoxArrGradOrient
  - @c PVM_VoxArrPosition
  - @c PVM_VoxArrFrequency
  - @c PVM_VoxArrGradient
  - @c PVM_VoxMaxGradient;
  - @c PVM_VoxArrCSDisplacement
  - @c PVM_VoxelGeoCub

  @PVM_TBCALL
  - @ref UT_GetRequest
  - @ref GTB_HandleGeoCuboid
  - @ref UT_ClearRequest
  - @ref CFG_GradCalConst
  - @ref ATB_GetTransmitterFreq
  - @ref PTB_MaxGeoLabelLength

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The voxel parameters must have been initialised with @ref STB_InitVoxelGeometry.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  const double pulseBandwidthArray[3];

  pulseBandwidthArray[0] = VoxPulse1.Bandwidth;
  pulseBandwidthArray[1] = VoxPulse2.Bandwidth;
  pulseBandwidthArray[2] = VoxPulse3.Bandwidth;

  STB_UpdateVoxelGeometry(pulseBandwidthArray, PVM_Nucleus1);
  @endcode

  @param[in] pulseBandwidthArray    3-element array specifying bandwidths of the selection pulses [Hz]
  @param[in] nucleus                Name of the nucleus

  @see @ref STB_InitVoxelGeometry

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateVoxelGeometry(double* pulseBandwidthArray,
                             char*   nucleus);




/**************************************************************************/
/*             gen/src/prg/pvm_toolboxes/mr/PvSeqTools/epi.c              */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_CommonEpiRels (void);
DLL_INTERFACE void STB_EpiGradSync (void);
DLL_INTERFACE void STB_EpiRampMode (void);
DLL_INTERFACE void STB_EpiRampForm (void);
DLL_INTERFACE void STB_EpiEchoPosition (void);
DLL_INTERFACE void STB_EpiGradDwellTime (void);
DLL_INTERFACE void STB_EpiRampTime (void);
DLL_INTERFACE void STB_EpiDephaseTime (void);
DLL_INTERFACE void STB_EpiBlipTime (void);
DLL_INTERFACE void STB_EpiAcqDelayTrim (void);
DLL_INTERFACE void STB_EpiReadDephTrim (void);
DLL_INTERFACE void STB_EpiBlipAsym (void);
DLL_INTERFACE void STB_EpiReadAsym (void);
DLL_INTERFACE void STB_EpiMaxOrder (void);
DLL_INTERFACE void STB_EpiMaxOrderRange (void);
DLL_INTERFACE void STB_EpiDriftCorr (void);
DLL_INTERFACE void STB_EpiDriftCorrRange (void);
DLL_INTERFACE void STB_EpiGrappaThresh (void);
DLL_INTERFACE void STB_EpiGrappaThreshRange (void);

/** Initialises the @c EPI_Parameters group.

  This function should be used in the initialization of methods using the @c EPI_Parameters
  group. It assigns initial values to all group members when they have no value.

  @PVM_PARS
  - All members of the @c EPI_Parameters pargroup

  @PVM_TBCALL
  - @ref STB_InitDigPars

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_InitEpi(UserSlope);
  @endcode

  @param rampMode    Default ramp mode (@c UserRamp, @c UserSlope, @c SystemRamp)

  @return @c 0 if everything is OK

  @see @ref STB_EpiUpdate, @ref STB_EpiCheckTrajectory, @ref STB_EpiHandleTrajAdjResults,
       @ref ATB_EpiSetBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE int STB_InitEpi(EpiRampMode rampMode);




/** Updates the @c EPI_Parameters group.

  This function should be called in methods using the @em epi.mod module to update the
  parameter group @c EPI_Parameters. After the call of this function all members of the
  @c EPI_Parameters group have up-to-date values. In particular, parameters @c PVM_EpiEchoDelay
  and @c PVM_EpiModuleTime obtain values which can be used in the method to calculate the TE
  and TR, respectively.

  This function tries to set the module parameters for  the specified FOV,
  and returns @c 0 if this is possible. Otherwise, it sets the FOV
  to what is possible and returns @c 1. In this case the method should react properly by updating
  the geometry parameters.

  In case illegal values of arguments are specified @c -1 is returned, a consistent state of
  the parameters may not be guaranteed in this case.


  @PVM_PARS
  - All members of the @c EPI_Parameters pargroup

  @PVM_TBCALL
  - @ref CFG_GradientRampTime
  - @ref CFG_InterGradientWaitTime

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The @em EPI parameters must have been initialised with @ref STB_InitEpi.

  @PVM_ERROR
  - Illegal arguments.

  @PVM_EXAMPLE
  @code
  double minFov[3] = {1e-3, 1e-3, 1e-3};
  int nSegments;

  STB_UpdateEncodingForEpi(&nSegments, Yes, No, Yes);

  STB_EpiUpdate(No,
                PTB_GetSpatDim(),
                PVM_EncMatrix,
                PVM_AntiAlias,
                PVM_Fov,
                minFov,
                PVM_EffSWh,
                PVM_GradCalConst,
                PVM_NEchoImages * GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices),
                nSegments,
                PVM_EncCentralStep1,
                PVM_EncPpi[1],
                PVM_EncNReceivers);
  @endcode

  @param epsi                    @c Yes for EPSI measurements, @c No otherwise
  @param dim                     Number of spatial dimensions
  @param[in] matrix_size         Nominal acq. matrix size (must have @a dim entries)
  @param[in] antiAlias           Aliasing factors (@a dim entries)
  @param[in,out] fov             Required FOV in units of mm; may change if too small
  @param[out] minFov             Minimum FOV allowed by the @em EPI module
  @param[in] bandwidth           Pointer to desired required bandwidth
  @param gradCalConst            Gradient calibration constant in units of Hz/mm
  @param nImagesPerRepetition    Number of images per repetition
  @param nShots                  Required number of shots (interleaves), must previously
                                 negotiated with @ref STB_UpdateEncoding
  @param centralEcho             Number of echo (counting from 1) touching
                                 <em>k</em>-space centre
  @param acceleration            Parallel acceleration
  @param nReceivers              Number of receive channels currently used
  @param tempGridding            activates temporal gridding on 1us, optional, default=No

  @return
  - @c -1 in case of an error condition due to illegal arguments
  - @c 0 if specified FOV is accepted
  - @c 1 if FOV has been changed

  @see @ref STB_InitEpi, @ref STB_EpiCheckTrajectory, @ref STB_EpiHandleTrajAdjResults,
       @ref ATB_EpiSetBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE int STB_EpiUpdate(YesNo      epsi,
                                int        dim,
                                const int *matrix_size,
                                double    *antiAlias,
                                double    *fov,
                                double    *minFov,
                                double    *bandwidth,
                                double     gradCalConst,
                                int        nImagesPerRepetition,
                                int        nShots,
                                int        centralEcho,
                                int        acceleration,
                                int        nReceivers,
                                YesNo      tempGridding = No);




/** Tests whether the @em EPI trajectory is up-to-date.

  This function should be called in @em EPI methods using trajectory measurements. This
  function tests whether imaging parameters have changed that determine the measured
  trajectory. If this is the case the parameter @c PVM_EpiTrajAdjMeasured is set to @c No.
  The visibility of the parameters in the @em EPI trajectory subclass is controlled accordingly.

  The relevant parameters besides the interface parameters are:

  @c PVM_EpiRampMode, @c PVM_EpiRampForm, @c PVM_EpiRampTime, @c PVM_EpiRampComp

  Names of the context parameters: @n
  @c PVM_EpiTrajAdjReadvec[], @c PVM_EpiTrajAdjRampmode, @c PVM_EpiTrajAdjRampform,
  @c PVM_EpiTrajAdjRamptime, @c PVM_EpiTrajAdjComp, @c PVM_EpiTrajAdjFov0,
  @c PVM_EpiTrajAdjMatrix0, @c PVM_EpiTrajAdjBw

  @PVM_PARS
  - @c PVM_EpiTrajAdjMeasured
  - @c PVM_EpiTrajAdjkx
  - @c PVM_EpiTrajAdjb0

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @em EPI parameters must be up-to-date.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_EpiCheckTrajectory(PVM_Fov[0], PVM_EffSWh, PVM_SPackArrGradOrient[0][0]);
  @endcode

  @param     fovRead       Field-of-view (in mm)
  @param     effSWh        Effective bandwidth
  @param[in] readVector    Pointer to read gradient vector

  @see @ref STB_InitEpi, @ref STB_EpiUpdate, @ref STB_EpiHandleTrajAdjResults,
       @ref ATB_EpiSetBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_EpiCheckTrajectory(double  fovRead,
                                          double  effSWh,
                                          double *readVector);




/** Stores @em EPI trajectory context parameters.

  This function should be called in @em EPI methods using trajectory measurements. This
  function stores imaging parameters that determine the measured trajectory.

  Names of the context parameters: see parameter list below.

  @c PVM_EpiTrajAdjReadvec[], @c PVM_EpiTrajAdjRampmode, @c PVM_EpiTrajAdjRampform,
  @c PVM_EpiTrajAdjRamptime, @c PVM_EpiTrajAdjComp, @c PVM_EpiTrajAdjFov0,
  @c PVM_EpiTrajAdjMatrix0, @c PVM_EpiTrajAdjBw

  @PVM_PARS
  - @c PVM_EpiTrajAdjReadvec[]
  - @c PVM_EpiTrajAdjRampmode
  - @c PVM_EpiTrajAdjRampform
  - @c PVM_EpiTrajAdjRamptime
  - @c PVM_EpiTrajAdjComp
  - @c PVM_EpiTrajAdjFov0
  - @c PVM_EpiTrajAdjMatrix0
  - @c PVM_EpiTrajAdjBw

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
  STB_EpiHandleTrajAdjResults(PVM_Fov[0], PVM_EffSWh, PVM_SPackArrGradOrient[0][0]);
  @endcode

  @param fovRead       Field-of-view (in mm)
  @param effSWh        Effective bandwidth
  @param readVector    Pointer to read gradient vector

  @see @ref STB_InitEpi, @ref STB_EpiUpdate, @ref STB_EpiCheckTrajectory,
       @ref ATB_EpiSetBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_EpiHandleTrajAdjResults(double   fovRead,
                                               double   effSWh,
                                               double  *readVector);

/** Constrains Geometry for @em EPI.

  This function should be called in @em EPI before the final image geometry
  is updated. Due to an oscillating readout gradient shifts along the
  readout direction are restricted. Dependent on the actual chosen image geometry
  the position of the slice packages are constrained within the legal range.
  For systems equipped with an Animal Transport System ( @em ATS), the position
  constraints are adapted to the animal bed position during scan time.


  Names of the context parameters: see parameter list below.

  @c PVM_EpiTrajAdjReadvec[], @c PVM_EpiTrajAdjRampmode, @c PVM_EpiTrajAdjRampform,
  @c PVM_EpiTrajAdjRamptime, @c PVM_EpiTrajAdjComp, @c PVM_EpiTrajAdjFov0,
  @c PVM_EpiTrajAdjMatrix0, @c PVM_EpiTrajAdjBw

  @PVM_PARS
  - @c PVM_AtsBedPositionMode
  - @c PVM_SPackArrReadOffset


  @PVM_TBCALL
  @ref STB_UpdateAtsReferencePosition
  @ref STB_TransferImagGeo

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  
  (...)
  double max_read_shift=(PVM_Fov[0] / 2)*(PVM_AntiAlias[0] - 1.0);
  STB_EpiConstrainGeometry(max_read_shift);
  STB_UpdateImageGeometry(dim, 
                          PVM_Matrix,
                          minFov,
                          0, // total slices (no restr)
                          maxPackages,
                          maxPerPackage,
                          minThickness,
                          1.0);
  (...)
  @endcode

  @param max_shift_read  maximum shift in readout direction (in mm)

  @see @ref STB_UpdateAtsReferencePosition, @ref STB_EpiUpdate, @ref STB_UpdateImageGeometry

  @ingroup PVM_STB
*/
DLL_INTERFACE bool STB_EpiConstrainGeometry(double  max_shift_read);


/**************************************************************************/
/*             gen/src/prg/pvm_toolboxes/mr/PvSeqTools/Spiral.c           */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_SpiralCommonRels(void);
DLL_INTERFACE void STB_SpiralGradDwellTimeRange (void);
DLL_INTERFACE void STB_SpiralGradDwellTime (void);

/** Initialises the @c SpiralParameters group.

  This function should be called in methods using the @em Spiral module to
  initialise the parameter group @c SpiralParameters. It assigns default
  values to all editable group members in case they do not yet have a value.

  @PVM_H1{Description of the @em Spiral module}

  @PVM_H2{Modes}
  The two basic modes for spiral acquisition are spiral-out and spiral-in.
  Commonly, spiral-out is used where @e k = @c 0 is acquired at the beginning.
  With spiral-in, @e k = @c 0 is acquired at the end, leading to a longer echo
  time without increasing the module time. In case of moving spins, the latter
  mode requires a compensation of the  flow-moment.

  @PVM_H2{Timing}
  The timing of a spiral acquisition is described by the following parameters
  which are either given in milliseconds or as the number of acquisition samples.
  Gradients before or after the spiral are used to drive to the required values of
  @e g and @e k and for flow compensation in case of spiral-in.
  - @c PVM_SpiralModuleTime - Total duration of @em Spiral module [ms]
  - @c PVM_SpiralEchoTime   - Time between start of module and acquisition of @e k = @c 0 [ms]
  - @c PVM_SpiralNavSize    - Initial navigator data [samples]
  - @c PVM_SpiralPreSize    - Gradients applied before start of spiral [samples]
  - @c PVM_SpiralSize       - Actual spiral acquisition [samples]
  - @c PVM_SpiralPostSize   - Gradients after spiral [samples]

  @image html SpiralTiming.png

  @PVM_H2{Implementation}
  The following steps must be performed to use the @em Spiral module in a method:
  - Create the method's pulse program as follows:
    - Include the @em Spiral module: <c>\#include <Spiral.mod></c>.
    - Insert <c>subr spiralAcq(RecPh, RefPh)</c> for acquisition of a @em Spiral
      readout with given receiver and reference phase.
    - Insert <c>subr spiralLoop(labelStart)</c> for incrementing the interleave
      counter and looping to @c labelStart.
  - Insert the parameter group @c SpiralParameters into a class in @c parsLayout.h.
  - Include the toolbox functions @ref STB_InitSpiral (after @ref STB_InitImageGeometry),
    @ref STB_SpiralUpdateGeometry, @ref STB_UpdateSpiral, and @ref ATB_SpiralSetRecoPars
    at the appropriate locations.
  - In @c callbackDefs.h redirect
    - @c PVM_SpiralHandler to the method's @c backbone,
    - @c PVM_AcqScanHandler to @ref STB_SpiralCreateShapes.
  - Use @c PVM_SpiralEchoTime, @c PVM_SpiralModuleTime, and @c PVM_SpiralNbOfInterleaves to
    calculate echo, repetition, and total scan time.
    The overall maximum gradient amplitude applied is @c PVM_SpiralGradientLimit.
  - Optionally include the trajectory measurement as described in @ref STB_InitTraj.
    Here, the string parameter @c PVM_SpiralContext, which is a unique identifier of the
    current gradient shape, can be used to decide if a trajectory measurement must be repeated.
  - Use @ref STB_SpiralTrajectory to calculate the theoretical trajectory for preparing the
    reconstruction.
  - Set up the reconstruction network by calling @ref ATB_SpiralSetupReco.
  - Insert @c Spiral.xml to the "Sequence" parameter card in the method's xml-file for the
    parameter layout.

  @PVM_H2{Debug Output}
  If <c>PVM_SpiralDebug = Yes</c> the following debug output in ASCII format is created in
  a subdirectory of <c>PVHOME/prog/curdir/USER/ParaVision</c>:
  <table>
  <tr><th>Filename</th><th>Description</th><th>Unit</th><th>Creating Function</th></tr>
  <tr><td>@c spiralGx<br>
          @c spiralGy</td>
      <td>Gradient values to be played out</td>
      <td>Fraction of maximum gradient amplitude<br>[<c>-1..1</c>]</td>
      <td>@ref STB_UpdateSpiral</td>
  </tr>
  <tr><td>@c spiralKt</td>
      <td>Theoretical <em>k</em>-space trajectory for both dimensions<br>
          (excluding the durations for navigator and ramping down)</td>
      <td>Normalised k-values<br>[<c>-0.5..0.5</c>]</td>
      <td>@ref STB_SpiralTrajectory</td>
  </tr>
  <tr><td>@c spiralDC</td>
      <td>Density pre-correction data (binary)<br>
          (only sample points used for reconstruction)</td>
      <td>a.u.</td>
      <td>Image reconstruction</td>
  </tr>
  </table>
  Note, that setting <c>PVM_SpiralDebug = Yes</c> can slow down the parameter update considerably,
  in particular for large matrices. Therefore, this setting should only be used for debugging
  purposes and never stored in protocols.

  @PVM_H2{Example}
  See the @em Spiral method for an example implementation.

  @PVM_PARS
  - All editable members of the @c SpiralParameters group
  - @c PVM_SliceGeoObj (set constraints)
  - @c PVM_SliceGeo (set maximum number of array elements to 1)

  @PVM_TBCALL
  - @ref GTB_GeoObjFreezeExt0Ext1Ratio
  - @ref GTB_GeoObjFreezePos0
  - @ref GTB_GeoObjFreezePos1
  - @ref PTB_SetGeoObjShape

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The image geometry must have been initialised.

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateSpiral, @ref STB_SpiralUpdateGeometry, @ref STB_SpiralTrajectory,
       @ref STB_SpiralCreateShapes, @ref ATB_SpiralSetRecoPars, @ref ATB_SpiralSetupReco

  @todo
  - @PVM_H2{Method}
    - protocol SPIRAL_bas: use default initialisation
    - DtiSpiral
      - no choice of signal type (part of diffusion module)
      - modules: trigger, fat sup, and saturation
  - @PVM_H2{Problems}
    - Pharmascan, spiral-in, 100 kHz, 4 shots: heavy artefacts
    - Artefact with 2-shot (on uI system, probably residual B0 eddy currents).
    - Clarify why duration of three-domain becomes NaN (currently fixed with exception).
  - @PVM_H2{Developments}
    - Static B0 off-resonance correction.
    - Variable density (+ use as navigator).

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitSpiral(void);

/** Updates the @c SpiralParameters group.

  This function should be called in methods using the @em Spiral module to update the parameter
  group @c SpiralParameters. After the call of this funtion all members of the group
  @c SpiralParameters have up-to-date values. In particular, @c PVM_SpiralEchoTime and
  @c PVM_SpiralModuleTime obtain values which can be used in the method to calculate TE and TR,
  respectively.

  If <c>PVM_SpiralDebug = Yes</c> the following debug output in ASCII format is created in
  a subdirectory of <c>PVHOME/prog/curdir/USER/ParaVision</c>:<br>
  The files @c spiralGx and @c spiralGy contain the gradient values to be played out.

  @PVM_PARS
  - All members of the @c SpiralParameters group
  - All members of the @c Digitizer group

  @PVM_TBCALL
  - @ref CFG_GradientRampTime
  - @ref PTB_MaxArraySize
  - @ref STB_SpiralCreateShapes
  - @ref STB_UpdateDigPars
  - @ref STB_SpecUpdateDigPars

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The dimension of the geometry must be >= @c 2.
  - Isotropic geometry is assumed for the two spiral dimensions (use @ref STB_SpiralUpdateGeometry).
  - The inplane geometry must be up-to-date.
  - The nucleus parameter group must be up-to-date (@c PVM_GradCalConst must be set).
  - A valid bandwidth value must be obtained by calling
    @code
    STB_UpdateDigPars(&PVM_EffSWh, PVM_EncMatrix[0] * PVM_EncMatrix[1], NULL, NULL);
    @endcode

  @PVM_ERROR
  - Matrix size was reduced due to limited gradient vector length.
  - Input parameters are not set correctly.

  @PVM_EXAMPLE
  @code
  STB_UpdateSpiral(PVM_EncMatrix[0], PVM_EffSWh, PVM_Fov[0]);
  @endcode

  @param matrix         Matrix size
  @param bandWidth      Effective acquisition bandwidth [Hz]
  @param fov            FOV [mm] (must be within constraints)

  @see @ref STB_InitSpiral, @ref STB_SpiralUpdateGeometry, @ref STB_SpiralTrajectory,
       @ref STB_SpiralCreateShapes, @ref ATB_SpiralSetRecoPars, @ref ATB_SpiralSetupReco

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateSpiral(int    matrix,
                      double bandWidth,
                      double fov);

/** Updates the geometry for the @c SpiralParameters group.

  This function should be called in methods using the @em Spiral module to update the geometry
  appropriately. It must be called in the method's backbone before updating the @em ImageGeometry group.
  It makes sure that
  - the first two dimensions have isotropic geometry (FOV and resolution),
  - no off-centres are allowed for the first slice-package.

  @PVM_PARS
  - The first two dimensions of
    - @c PVM_Fov
    - @c PVM_SpatResol
  - <c>PVM_SPackArrReadOffset[0]</c>
  - <c>PVM_SPackArrPhase1Offset[0]</c>

  @PVM_TBCALL
  - @ref GTB_IsotropicRequestForFov
  - @ref GTB_IsotropicRequestForResolution

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The dimension of the geometry must be >= @c 2.
  - The image size (matrix) must be isotropic in the first two dimensions.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_SpiralUpdateGeometry(PVM_Matrix, minFov);
  @endcode

  @param[in] matrix  image size (must be isotropic in first two dimensions)
  @param[in] minFov  minimum field of view

  @see @ref STB_InitSpiral, @ref STB_UpdateSpiral, @ref STB_SpiralTrajectory,
       @ref STB_SpiralCreateShapes, @ref ATB_SpiralSetRecoPars, @ref ATB_SpiralSetupReco

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_SpiralUpdateGeometry(int *matrix, double *minFov);

/** Creates theoretical @em Spiral trajectory for parameter group @c SpiralParameters.

  This function should be called in methods using the @em Spiral module to create the
  theoretical trajectory. It can be stored in parameters and/or written to a file to be used
  by the reconstruction.

  If <c>PVM_SpiralDebug = Yes</c> the following debug output in ASCII format is created in
  a subdirectory of <c>PVHOME/prog/curdir/USER/ParaVision</c>:<br>
  The file @c spiralKt contains the theoretical trajectory for both dimensions,
  excluding the durations for navigator and ramping down.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The toolbox function @ref STB_UpdateSpiral must have been called.

  @PVM_ERROR
   - Input parameters are not set correctly.

  @PVM_EXAMPLE
  @code
  STB_SpiralTrajectory("/tmp/traj", NULL, NULL, 0, nSpiral);
  @endcode

  @param[in]  fileName   Name of the file for writing the trajectory data
                         (@c NULL = do not write to file)
  @param[out] k1         Parameter for storing the trajectory in first dimension
                         (@c NULL = do not store in parameter)
  @param[out] k2         Parameter for storing the trajectory in second dimension
                         (@c NULL = do not store in parameter)
  @param      nInt       Number of interleaves (@c 0 = all)
  @param      nSpiral    Number of samples per interleave (@c 0 = all)

  @see @ref STB_InitSpiral, @ref STB_UpdateSpiral, @ref STB_SpiralUpdateGeometry,
       @ref STB_SpiralCreateShapes, @ref ATB_SpiralSetRecoPars, @ref ATB_SpiralSetupReco

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_SpiralTrajectory(char   *fileName,
                          double *k1     = NULL,
                          double *k2     = NULL,
                          int    nInt    = 0,
                          int    nSpiral = 0);

/** Creates spiral gradient shapes for the parameter group @c SpiralParameters.

  This function is required to create the @em Spiral gradient shapes in methods using the
  @em Spiral module. In @c callbackDefs.h the relations of @c PVM_AcqScanHandler must be
  redirected to this function.

  @PVM_PARS
  - @c PVM_SpiralShape1
  - @c PVM_SpiralShape2

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The toolbox function @ref STB_UpdateSpiral must have been called.

  @PVM_ERROR
  NONE

  @see @ref STB_InitSpiral, @ref STB_UpdateSpiral, @ref STB_SpiralTrajectory,
       @ref STB_SpiralUpdateGeometry, @ref ATB_SpiralSetRecoPars,
       @ref ATB_SpiralSetupReco

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_SpiralCreateShapes(void);




/**************************************************************************/
/*           gen/src/prg/pvm_toolboxes/mr/PvSeqTools/Trajectory.c         */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_TrajCommonRels(void);
DLL_INTERFACE void STB_TrajCommonRecoRels(void);
DLL_INTERFACE void STB_TrajExcPulseEnumRel(void);
DLL_INTERFACE void STB_TrajExcPulseRange(void);
DLL_INTERFACE void STB_TrajExcPulseRel(void);
DLL_INTERFACE void STB_TrajRfcPulseEnumRel(void);
DLL_INTERFACE void STB_TrajRfcPulseRange(void);
DLL_INTERFACE void STB_TrajRfcPulseRel (void);
DLL_INTERFACE void STB_TrajDisplayModeRange(YesNo enableResponse, int display);
DLL_INTERFACE void STB_TrajGeoObjRel(void);
DLL_INTERFACE void STB_TrajGeoCubRel(void);
DLL_INTERFACE void STB_TrajHandler(void);

/** Initialises the @c TrajParameters group of the @em Trajectory module.

  This function should be used in the initialisation part of a method.
  It assigns default values to all editable group members in case they do
  not yet have a value.

  @PVM_H1{Description of the @em Trajectory module}
  The @em Trajectory module serves to measure <em>k</em>-space trajectories
  created by gradient pulses or shapes. It is implemented as a method-specific
  adjustment.

  @PVM_H2{Principle}
  The general principle is to collect signal from off-centred spins while playing
  out the gradient shape in the same dimension. The localisation is performed by
  exciting two opposite, off-centred slices.

  Although the gradients are described - as usual - in the RPS volume coordinate
  system, the actual measurement is executed separately for all involved physical
  gradients directly in the XYZ object coordinate system. Thus, the effects of
  different gradient delays can be fully detected.
  Note that the mis-scaling of a gradient channel cannot be detected in this way
  as the test  slices will be shifted, thus compensating for the modified test gradient.

  @PVM_H2{References}
  - Duyn JH, Yang Y, Frank JA, van der Veen JW, Simple correction method for <em>k</em>-space
    trajectory deviations in MRI, JMR 132 (1998) 150-153
  - Zhang Y, Hetherington HP, Stokely EM, Mason GF, Twieg DB, A novel <em>k</em>-space
    trajectory measurement technique, MRM 39 (1998) 999-1004
  - Boernert P, Schomberg H, Aldefeld B, Groen J, Improvements in spiral MR imaging, MAGMA 9
    (1999) 29-41

  @PVM_H2{Modes}
  For a useful trajectory measurement, the relative timing of gradient shape and acquisition
  should be identical to that used for the actual measurement. There are two possibilities
  provided to describe the application of the gradient together with the acquisition.The
  related modes are set during initialisation of the module with the argument @a gradientMode.
  - With @a gradientMode = @c 0 the gradient shape can be described in an arbitrary way.
    However, this mode requires definitions on the pulse program level and has a few
    restrictions.
  - With @a gradientMode = @c 1 or @c 2 only minimal modifications of the method's pulse program
    are required. The gradients to be tested must be described completely by a single shape
    with additional interleave weightings. All gradient information is provided in PVM parameter arrays.
  - For @a gradientMode = @c 1 one shape is provided for all dimensions (e.g. Radial) whereas
    for @a gradientMode = @c 2 an individual shape is required for each dimension
    (e.g. Spiral).

  Moreover, the @a interleaveMode handles which interleaves should be measured. If the
  acquisition consists of multiple interleaves, the trajectory can be measured for all of them.
  Alternatively, in order to save time, a reduced number of interleaves is measured and the
  trajectory is calculated for the remaining ones.

  Note, that only one orientation is supported, hence e.g. multiple slice packages with different
  orientation cannot be handled.

  @PVM_H2{Timing}
  Different timing aspects of the sequence can be handled via arguments of the toolbox
  functions. All these arguments are given in units of number of acquisition samples.
  - @c nSamp - Total number of acquired data points.
  - @c nJunk - Initial data which is skipped both in the actual measurement and in the
               trajectory, e.g. navigator data. At the end of this interval the trajectory
               must be at <em>k</em> = @c 0.
  - @c nUsed - Part of the data and trajectory actually used for image reconstruction.
  - @c nPre  - Initial part of the gradient shape used e.g. to reach a certain
               <em>k</em>-space position.
  - @c nPost - Final part of gradient shape used e.g. to go to G = @c 0 or to travel back to
               <em>k</em> = 0.

  @image html TrajTiming.png

  @PVM_H2{Implementation}
  The following steps must be performed to use the @em Trajectory module in a method:
  - Set up the method's pulse program as follows:
    -# Call <c>preset off</c>.
    -# Insert <c>\#include <MRI.include></c>.
    -# For @a gradientMode = @c 0 create subroutines used by the @em Trajectory module:
       - <c>trajTestOn(phase RecPH, phase RefPH)</c>
         for acquisition while playing out all test gradients on R, P, and S at the same time
       - <c>trajTestOff(phase RecPH, phase RefPH)</c>
         for reference acquisition without gradients
       - <c>trajIncr()</c>
         for incrementing the interleave pointer (only required for @a interleaveMode = @c 2)
       - The given receiver and reference phases are used for the acquisition according to
         <c>ADC_INIT(RecPH, RefPH)</c>.
       - Example:
         @code
         #define SPIRAL_GR PVM_SpiralShape1() * PVM_SpiralInterleavCos * 100 - PVM_SpiralShape2() * PVM_SpiralInterleavSin * 100
         #define SPIRAL_GP PVM_SpiralShape1() * PVM_SpiralInterleavSin * 100 + PVM_SpiralShape2() * PVM_SpiralInterleavCos * 100

         subroutine trajTest(phase RecPH, phase RefPH)
         {
           ADC_INIT(RecPH, RefPH)
           SpirD0 ADC_START grad_shape {SPIRAL_GR, SPIRAL_GP, 0}
           SpirD1           grad_off
           SpirD2 ADC_END
         }

         subroutine trajTestOff(phase RecPH, phase RefPH)
         {
           ADC_INIT(RecPH, RefPH)
           SpirD0 ADC_START
           SpirD1
           SpirD2 ADC_END
         }

         subroutine trajIncr()
         {
           PVM_SpiralInterleavCos.inc
           PVM_SpiralInterleavSin.inc
         }
         @endcode
    -# Insert <c>\#include <Trajectory.mod></c>.
    -# The alternative trajectory or actual acquisitions are performed by inserting
       @code
         if (PVM_TrajAdjustmentRunning == Yes)
         {
           subr trajPerform()
         }
         else
         {
           ; complete standard pulse program parts of the method
         }
       @endcode
  - For @a gradientMode > @c 0, for the actual acquisition, gradient shape and acquisition
    must start concurrently, ideally according to the following example:
    @code
    ADC_INIT(RecPH, RefPH)
    d0 ADC_START grad_shape {Shape1(), Shape2(), 0}
    d1           grad_off
    d2 ADC_END
    @endcode
  - In @c parsLayout.h insert the parameter group
    - @c TrajParameters into @c MethodClass or another suitable group
    - @c TrajRecoParameters into the @c MethodRecoGroup
  - In @c loadMeth.h call @c SetRecoParam() when @c MethodRecoGroup was loaded.
  - Include the toolbox functions @ref STB_InitTraj and @ref STB_UpdateTraj.
  - Include the toolbox function @ref STB_TrajAppendAdjustment as described there.
  - In @c callbackDefs.h redirect
    - @c PVM_AdjHandler to @ref STB_TrajHandleAdjustmentRequests or to a method specific
      adjustment handler function, where this function is called initially.
    - @c PVM_AdjResultHandler to @ref ATB_TrajHandleAdjustmentResults or to a method
      specific adjustment result handler function, where this function is called initially.
    - @c PVM_TrajHandler to the method's @c backbone
    - @c PVM_TrajRecoHandler to @c SetRecoParam
    - @c PVM_TrajAdjustmentCounter to a method specific adjustment handler function,
      e.g. @c TrajAdjustmentCounterRels, in case additional result parameters should be handled.
      This handler may look as follows:
      @code
      void TrajAdjustmentCounterRels(void)
      {
        // call default relations
        ParxRelsParRelations("PVM_TrajAdjustmentCounter", Yes);

        // process data after successful adjustment
        if (PVM_TrajAdjustmentCounter == 0)
        {
          // process data and store in additional result parameters
          // ...

          // in case of a bad result, return from adjustment with failure
          ATB_TrajSetAdjustmentStatus(0, "No valid result for EPI");
        }

        // handle un-successful adjustment
        else if (PVM_TrajAdjustmentCounter == -1)
        {
          // create theoretical data and store in additional result parameters
          // ...

          // set status to "repaired"
          ATB_TrajSetAdjustmentStatus(1);
        }
      }
      @endcode
  - In @c RecoRelations.c, in case the relation of @c RecoUserUpdate is redirected to
    @c DeriveReco, call @ref ATB_TrajSetupReco.
    Otherwise, redirect @c RecoUserUpdate to @ref ATB_TrajSetupReco in @c callbackDefs.h.
  - The method must perform acquisition with the baselevel setting <c>ACQ_scan_shift = -1</c>.
  - After the measurement, the XYZ trajectory data are available in @c PVM_TrajKx/y/z and the B0
    components in @c PVM_TrajBx/y/z. They can be transferred to the RPS system and prepared
    for the reconstruction by using @ref STB_TrajCreateTrajectory. The B0 eddy current
    correction can be applied by using @ref ATB_TrajInsertB0Correction.
  - The function @ref ATB_TrajUpToDate indicates if a valid trajectory is available for
    the current context.
  - In the method's xml-file for the parameter layout include
    - @c Trajectory.xml in the @em Sequence parameter card
    - @c TrajectoryResult.xml on the highest level
    - @c TrajectoryReco.xml on the highest level
  - For @a gradientMode = @c 0 the theoretical gradients in the RPS system must/may be provided
    in @c PVM_TrajGx/y/z
    - Mandatory for @a display = @c 4 and/or @a repairResult = @c Yes
    - Optional for displaying the theoretical together with the measured results
    - Units [kHz/mm]
    - Must contain all measured interleaves but no junk data
    - Values on acquisition grid of size (@a nSamp - @a nJunk) * @a nInt
      (given by @c PVM_TrajResultSize after calling @ref STB_UpdateTraj)
    - @c PVM_TrajGy only for @a nDim > @c 1, @c PVM_TrajGz only for @a nDim > @c 2
    - Required after sequence start, i.e. may be created during updating or in the redirected
      relation of @c PVM_AcqScanHandler

  @PVM_H2{Debug Output}
  If <c>PVM_TrajDebug = Yes</c> the following debug output in ASCII format is created in
  a subdirectory of <c>PVHOME/prog/curdir/USER/ParaVision</c>:
  <table>
  <tr><th>Filename</th><th>Description</th><th>Unit</th><th>Created</th></tr>
  <tr><td>@c trajKx<br>
          @c trajKy<br>
          @c trajKz</td>
      <td>Unprocessed measured XYZ trajectory<br>
          (including all acquired samples)</td>
      <td>k-values<br>[1/mm]</td>
      <td>After adjustment</td>
  </tr>
  <tr><td>@c trajBx<br>
          @c trajBy<br>
          @c trajBz</td>
      <td>Unprocessed measured XYZ B0 phase error<br>
          (including all acquired samples)</td>
      <td>rad</td>
      <td>After adjustment</td>
  </tr>
  <tr><td>@c trajKm</td>
      <td>Processed trajectory for all RPS dimensions<br>
          (excluding pre- and post-samples)</td>
      <td>k-values (see @a kUnits)<br>[1/mm] or normalised [<c>-0.5..0.5</c>]</td>
      <td>By @ref STB_TrajCreateTrajectory</td>
  </tr>
  <tr><td>@c trajBm</td>
      <td>Processed B0 phase error added from all RPS dimensions<br>
          (excluding pre- and post-samples)</td>
      <td>Complex-valued correction factors</td>
      <td>By @ref STB_TrajCreateTrajectory</td>
  </tr>
  <tr><td>@c trajDisp</td>
      <td>Display output (binary)<br>
          real part = measured, imaginary part = theoretical<br>
          (with junk data being removed)</td>
      <td>Depends on displayed quantity (see also @a kUnits)</td>
      <td>During adjustment</td>
  </tr>
  </table>

  For simulating the trajectory adjustment, perform the following steps:
  - Enter the adjustment platform
  - Open the @em Trajectory adjustment
  - In the single parameter editor search for @c PVM_TrajAdjustmentCounter
  - Set the parameter by clicking the "No Value" field
  - Do a right-click on @em Trajectory in the adjustment list and select "Simulate Adjustment"
  - Note: for correct simulation, parameters should not be changed in the adjustment platform
    but on the method level.

  @PVM_H2{Example}
  Find an example of using the @em Trajectory module in the method @em SPIRAL.

  @PVM_PARS
  - Members of the @c TrajParameters group

  @PVM_TBCALL
  - @ref CFG_AmplifierEnable
  - @ref CFG_GradientRiseTime
  - @ref CFG_InterGradientWaitTime
  - @ref STB_InitRFPulse
  - @ref PTB_InsertGeoObject
  - @ref GTB_GeoObjFreezePos0
  - @ref GTB_GeoObjFreezePos1
  - @ref GTB_GeoObjFreezePos2
  - @ref GTB_GeoObjFreezeOri0
  - @ref GTB_GeoObjFreezeOri1
  - @ref GTB_GeoObjFreezeOri2
  - @ref GTB_GeoObjUniqueSubc
  - @ref GTB_GeoObjForcePositiveGap
  - @ref GTB_GeoObjDependencyInit
  - @ref GTB_GeoObjDependencySet
  - @ref PTB_MaxGeoLabelLength

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Input parameters are not set correctly.

  @PVM_EXAMPLE
  @code
  double scaleInt[9] = {1.0, 1.0, 0.0,
                       -1.0, 1.0, 0.0,
                        0.0, 0.0, 0.0};
  STB_InitTraj(1, 2, 3, 0, No, 1, Yes, 0, NULL, No, 2,
       "PVM_SpiralShape1", "PVM_SpiralShape2", NULL,
       "PVM_SpiralInterleavCos", "PVM_SpiralInterleavSin", NULL,
       "PVM_SpiralInterleavSin", "PVM_SpiralInterleavCos", NULL,
       NULL, NULL, NULL,
       scaleInt);
  @endcode

  @param kUnits              Determines units of the trajectory
                             - @c 0 = physical units<br>
                                      \f$k\f$ will be in [1/mm] with \f$k_{max} = 1 / (2 \cdot
                                      resolution [mm])\f$
                             - @c 1 = normalised units<br>
                                      \f$k\f$ will be normalised with \f$k_{max} = 0.5\f$<br>
                                      as required for regridding with @em RegridN
  @param gradientMode        Determines how the tested gradients are described
                             - @c 0 = general (no specific description of gradients)
                             - @c 1 = one shape for all dimensions
                             - @c 2 = one shape per dimension
  @param interleaveMode      Determines for how many interleaves a separate trajectory is
                             measured
                             - @c 0 = no interleaves used
                             - @c 1 = reduced number of interleaves (@a gradientMode = @c 0:
                               not available, @c 1: one interleave, @c 2: each shape for each axis)
                             - @c 2 = all interleaves
                             - @c 3 = user selection from @c 1 and @c 2 (default; only for
                               @a gradientMode > @c 0)
  @param signalType          Initialisation value for signal type
                             - @c 0 = use current value (default)
                             - @c 1 = FID
                             - @c 2 = spin echo
  @param enableResponse      Enables frequency response as display output
                             - For @a gradientMode = @c 0 input gradients must be provided in @c PVM_TrajGx/y/z
                             - Default = @c No
  @param display             Initialisation value for display output
                             - @c 0 = use current value (default)
                             - @c 1 = trajectory
                             - @c 2 = gradient
                             - @c 3 = gradient spectrum
                             - @c 4 = frequency response (only for @a enableResponse = @c Yes)
  @param handleAdjRequests   Determines if other adjustment requests are handled
                             - @c Yes = call default adjustment handler for non-method-specific
                               and receiver gain adjustments (default)
                               (to be used for immediate redirection of @c PVM_AdjHandler)
                             - @c No = only handle trajectory adjustment
                               (to be used if more method-specific adjustments are added)
  @param nResultPars         Number of additional result parameters (default = @c 0)
  @param[in] resultPars      List of additional result parameters to be handled (default = @c NULL)
  @param repairResult        Determines behaviour in case the adjustment fails
                             - @c No = conclude with failure (default)
                             - @c Yes = provide theoretical trajectory and conclude with success
                               - For @a gradientMode = @c 0 input gradients must be provided in @c TrajGx/y/z
                               - Note: a previously measured valid trajectory has priority over the repaired result
  @param callAcqUpdate       Determines when the redirected relations of @c PVM_AcqScanHandler
                             should be called
                             - Only for methods with redirections of the scan handler
                               @c PVM_AcqScanHandler
                             - The relations usually update parameters, e.g. gradient shapes,
                               just before the acquisition.
                             - If activated, call is also executed in
                               @ref STB_TrajCreateTrajectory
                             - Possible values:
                               - @c 0 = never (default)
                               - @c 1 = before trajectory adjustment
                               - @c 2 = before trajectory and receiver gain adjustment
                               - @c 3 = before all method specific adjustments
  @param[in] shape1          Gradient shape 1 (required for @a gradientMode > @c 0)
  @param[in] shape2          Gradient shape 2 (required for @a gradientMode > @c 1 and
                             @a nDim > @c 1)
  @param[in] shape3          Gradient shape 3 (required for @a gradientMode > @c 1 and
                             @a nDim > @c 2)
  @param[in] int1r           Interleave weighting for shape 1 on R (required for
                             @a gradientMode > @c 0)
  @param[in] int1p           Interleave weighting for shape 1 on P (required for
                             @a gradientMode > @c 0, and @a nDim > @c 1)
  @param[in] int1s           Interleave weighting for shape 1 on S (required for
                             @a gradientMode > @c 0, and @a nDim > @c 2)
  @param[in] int2r           Interleave weighting for shape 2 on R (required for
                             @a gradientMode = @c 2, and @a nDim > @c 1)
  @param[in] int2p           Interleave weighting for shape 2 on P (required for
                             @a gradientMode = @c 2, and @a nDim > @c 1)
  @param[in] int2s           Interleave weighting for shape 2 on S (required for
                             @a gradientMode = @c 2, and @a nDim > @c 1)
  @param[in] int3r           Interleave weighting for shape 3 on R (required for
                             @a gradientMode = @c 2, and @a nDim > @c 2)
  @param[in] int3p           Interleave weighting for shape 3 on P (required for
                             @a gradientMode = @c 2, and @a nDim > @c 2)
  @param[in] int3s           Interleave weighting for shape 3 on S (required for
                             @a gradientMode = @c 2, and @a nDim > @c 2)
  @param scaleInt            Scaling of each interleave weighting in above order
                             - Optional
                             - Only for @a gradientMode = @c 2
                             - May save parameter arrays for interleaves
                             - E.g. for spirals

  @PVM_H2{Notes}
  - Interleave weightings must be scaled to have maximum 1.
  - Shapes can be of arbitrary scaling but must be in agreement with @a gradAmp in
    @ref STB_UpdateTraj.
  - Dimensions: R | r = read, P | p = phase, S | s = slice.
  - The parameters <c>shape*</c> and <c>int*</c> are given as the names of the respective
    PVM parameters.
  - The parameters <c>int*</c> are only required for @a interleaveMode = @c 2 or @c 3,
    or when @ref STB_TrajCreateTrajectory is used.
  - The parameters <c>shape*</c>, <c>int*</c>, and @c scaleInt are not required for
    @a gradientMode = @c 0.
  - Otherwise, set non-required or optional parameters to @c NULL.

  @see @ref STB_UpdateTraj, @ref STB_TrajAppendAdjustment,
       @ref STB_TrajHandleAdjustmentRequests, @ref STB_TrajCreateTrajectory,
       @ref ATB_TrajSetupReco, @ref ATB_TrajInsertB0Correction, @ref ATB_TrajSetAdjustmentStatus,
       @ref ATB_TrajHandleAdjustmentResults, @ref ATB_TrajUpToDate

  @todo
  - @PVM_H2{Open issues}
    - Many (4-5) shots required for RG
      - Check after first average?
      - Use lower start value?
    - Trajectory sometimes corrupt in early parts
      - Wrong unwrapping due to large changes?
      - Use higher sampling?
  - @PVM_H2{Improvements}
    - Hardware scope set by @ref PvAdjManSetHwScope should include preemphasis and @c COILDEL.
    - Enable changing order of RPS (see PDF notes in project folder)?
    - Check quality of trajectory from
      - Magnitude
      - Difference theoretical - measured (after shift correction)
    - Provide way of removing parts of data for shift correction as determined by @ref STB_TrajCreateTrajectory
      - E.g. by using RecoStabCorrFilter
      - SPIRAL: done by @ref ATB_SpiralSetupReco
      - UTE: currently no shift is allowed
    - Slice positions: enable setting the two locations separately?
    - Multiple slices
      - With same TR per slice
      - Use highest SNR or weighted fit
    - Automatic averaging depending on SNR

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitTraj(int        kUnits,
                  int        gradientMode,
                  int        interleaveMode    = 3,
                  int        signalType        = 0,
                  YesNo      enableResponse    = No,
                  int        display           = 0,
                  YesNo      handleAdjRequests = Yes,
                  int        nResultPars       = 0,
                  const char *resultPars[]     = NULL,
                  YesNo      repairResult      = No,
                  int        callAcqUpdate     = 0,
                  const char *shape1           = NULL,
                  const char *shape2           = NULL,
                  const char *shape3           = NULL,
                  const char *int1r            = NULL,
                  const char *int1p            = NULL,
                  const char *int1s            = NULL,
                  const char *int2r            = NULL,
                  const char *int2p            = NULL,
                  const char *int2s            = NULL,
                  const char *int3r            = NULL,
                  const char *int3p            = NULL,
                  const char *int3s            = NULL,
                  double     scaleInt[9]       = NULL);

/** Updates the @c TrajParameters group (deprecated).

  @PVM_USEINSTEAD{STB_UpdateTraj}

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE void STB_UpdateTraj(double *resol,
                    double *zeroFill,
                    double *fov,
                    int    nDim,
                    int    nInt,
                    int    nSamp,
                    int    nJunk,
                    double dwAcq,
                    double *gradMax,
                    double (*gradOrient)[3][3],
                    OnOff  trig = Off,
                    YesNo  gradsync = No,
                    double echoPos = 0.5,
                    double acqDel = 0.0,
                    double dwGrad = 0.0,
                    double *gradAmp = NULL);

/** Updates the @c TrajParameters group.

  This function should be called in methods using the @em Trajectory module to update the
  parameter group @c TrajParameters. After the call of this function all members of the group
  @c TrajParameters have up-to-date values.

  @PVM_PARS
  - Members of the @c TrajParameters group

  @PVM_TBCALL
  - @ref UT_GetRequest
  - @ref UT_ClearRequest
  - @ref GTB_HandleGeoCuboid
  - @ref GTB_GeoObjDependencySlave
  - @ref GTB_GeoObjDependencyApply
  - @ref GTB_GeoObjDependencyIsSlave
  - @ref GTB_GeoObjFreezeExtent2
  - @ref GTB_GeoObjExtentGap
  - @ref GTB_GeoObjExtentKeepExt0Ext1Ratio
  - @ref STB_UpdateRFPulse
  - @ref MRT_MinSliceThickness
  - @ref MRT_SliceGrad
  - @ref MRT_DephaseTime
  - @ref MRT_DephaseGrad

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The geometry must be up-to-date.
  - The nucleus parameter group must be up-to-date (@c PVM_GradCalConst must be set).
  - It is assumed that acquisition is performed on the F1 channel.

  @PVM_ERROR
  - Input parameters are not set correctly.

  @PVM_EXAMPLE
  @code
  double gradMax[2] = {PVM_SpiralGradientLimit, PVM_SpiralGradientLimit};

  STB_UpdateTraj(PVM_SpatResol, PVM_EncZf, "PVM_SliceGeo", 2, PVM_SpiralNbOfInterleaves,
                 PVM_DigNp, PVM_SpiralNavSize, PVM_DigDw, gradMax, PtrType3x3 PVM_SPackArrGradOrient[0],
                 PVM_TriggerModule, 0.5, PVM_DigEndDelOpt, PVM_SpiralGradDwellTime * 1e-3);
  @endcode

  @param[in] resol      Reconstructed spatial resolution [mm]
  @param[in] zeroFill   Zero filling factor
                        - @c NULL = @c 1.0 for all dimensions
  @param[in] cuboidname Parameter name of geometry object (e.g. PVM_SliceGeo)
  @param     nDim       Number of dimensions (@c 1, @c 2, or @c 3)
  @param     nInt       Number of interleaves (only important if initialised with interleaves)
  @param     nSamp      Number of acquired complex sampling points per readout
  @param     nJunk      Number of initial sampling points that must be skipped per readout
                        (e.g. navigator data)
  @param     dwAcq      Acquisition dwell time [ms]
  @param[in] gradMax    Maximum gradient amplitude per dimension [%]
                        - Required to set slice position appropriately
  @param     trig       Handles triggering
                        - @c On = perform triggering on level
                        - @c Off = no triggering (default)
  @param     gradsync   YesNo option to synchronize sequence with hardware clock
  @param     echoPos    Echo position of spin echo [fraction of readout duration]
                        - @c 0.5 = centred (default)
  @param     acqDel     Delay between acquisition and end of acquisition [ms]
                        - Only for @a gradientMode > @c 0
                        - Should allow for additional samples due to digital filtering
                        - At least one gradient rise rime will be used (gradient is switched off)
  @param     dwGrad     Gradient dwell time [ms]
                        - Only for @a gradientMode > @c 0
  @param[in] gradAmp    Gradient amplitude per dimension [%]
                        - Only for @a gradientMode > @c 0
                        - Shapes will be multiplied with this value
                        - Set values to @c 1.0 in case final amplitudes are given by the shapes
                        - @c NULL = 100 %

  @see @ref STB_InitTraj, @ref STB_TrajAppendAdjustment, @ref STB_TrajHandleAdjustmentRequests,
       @ref STB_TrajCreateTrajectory, @ref ATB_TrajSetupReco, @ref ATB_TrajInsertB0Correction,
       @ref ATB_TrajSetAdjustmentStatus, @ref ATB_TrajHandleAdjustmentResults, @ref ATB_TrajUpToDate

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateTraj(double *resol,
                                  double *zeroFill,
                                  const char *const cuboidname,
                                  int nDim,
                                  int nInt,
                                  int nSamp,
                                  int nJunk,
                                  double dwAcq,
                                  double *gradMax,
                                  OnOff trig,
                                  YesNo gradsync,
                                  double echoPos,
                                  double acqDel,
                                  double dwGrad,
                                  double *gradAmp=NULL);
/** Creates trajectory to be used by reconstruction.

  This function should be called after measuring a trajectory with the @em Trajectory module.
  The trajectory is analysed for a possible gradient delay and written to a file to be used by
  the reconstruction.

  This function may also be used to transfer the results originally acquired in the XYZ system
  to the RPS system.

  If <c>PVM_TrajDebug = Yes</c> the following debug output in ASCII format is created in
  a subdirectory of <c>PVHOME/prog/curdir/USER/ParaVision</c>:<br>
  The files @c trajKm and @c trajBm contain the processed trajectory and B0 components for all
  dimensions, excluding pre- and post-samples.

  @PVM_PARS
  - <c>PVM_TrajInt*</c>

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c PVM_AcqScanHandler if activated in @ref STB_InitTraj (@a callAcqUpdate > @c 0)
    and if actually required (@a gradientMode > @c 0, and @a *nUsed = @c 0 or @a nShift > @c 0)

  @PVM_PRECOND
  - The toolbox function @ref STB_UpdateTraj must have been called.
  - The trajectory must have been measured.
  - Initial navigator data must not be included in the trajectory.

  @PVM_ERROR
  - Input parameters are not set correctly.
  - Calling for incorrect mode.

  @PVM_EXAMPLE
  @code
  STB_TrajCreateTrajectory("/tmp/traj", "/tmp/b0",
                           &nPre, &nSpiral, &nPost,
                           (int)(0.3 / PVM_DigDw), PVM_SpiralPointsPerRotation);
  @endcode

  @param[in]     kName  Name of the file for writing the trajectory data
                        - @c NULL = do not store trajectory in file
  @param[in]     bName  Name of the file for writing the B0 correction
                        - Only for @a nUsed > @c 0
                        - @c NULL = do not store B0 correction in file
  @param[in,out] nPre   Number of samples before range used for reconstruction
                        - E.g. ramping up to start of spiral
                        - After navigator
                        - For one interleave
                        - On return: + shift
  @param[in,out] nUsed  Number of samples used for reconstruction
                        - For one interleave
                        - @c 0 = calculate theoretical trajectory and no B0 (only for
                          @a gradientMode > @c 0)
                        - On return: possibly reduced for too large shift
  @param[out]    nPost  Number of samples to be skipped at the end
                        - For one interleave
  @param         nShift Maximum shift [samples]
                        - Should represent the maximum expected gradient delay
                        - @c 0 = no shift correction
  @param         nTest  Testing range for shift [samples]
                        - Should represent a reasonable fraction of an interleave readout
                        - Internally limited to samples per interleave - @a nShift
                        - Only used for @a nShift > @c 0
  @param         storeInPar @c Yes = Store result in arrays @a kr, @a kp, @a ks, @a br, @a bp, @a bs
  @param[in,out] kr     Trajectory in R
                        - Input if @a gradientMode = @c 0 and @a nShift > @c 0: @a nTest theoretical values of the first interleave
                        - Output if @a storeInPar = @c Yes: size must be sufficient to store @a nUsed points for all interleaves
  @param[in,out] kp     Trajectory in P (@a nDim > @c 1)
  @param[in,out] ks     Trajectory in S (@a nDim > @c 2)
  @param[out]    br     B0 error in R
                        - @c NULL = do not store B0 error in parameter
  @param[out]    bp     B0 error in P (@a nDim > @c 1)
  @param[out]    bs     B0 error in S (@a nDim > @c 2)

  @see @ref STB_InitTraj, @ref STB_UpdateTraj, @ref STB_TrajAppendAdjustment,
       @ref STB_TrajHandleAdjustmentRequests, @ref ATB_TrajSetupReco,
       @ref ATB_TrajInsertB0Correction, @ref ATB_TrajSetAdjustmentStatus,
       @ref ATB_TrajHandleAdjustmentResults, @ref ATB_TrajUpToDate

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_TrajCreateTrajectory(const char *kName,
                              const char *bName,
                              int        *nPre,
                              int        *nUsed,
                              int        *nPost,
                              int        nShift     = 0,
                              int        nTest      = 0,
                              YesNo      storeInPar = No,
                              double     *kr        = NULL,
                              double     *kp        = NULL,
                              double     *ks        = NULL,
                              double     *br        = NULL,
                              double     *bp        = NULL,
                              double     *bs        = NULL);

/** Setup of the trajectory measurement as an adjustment.

  This function should be called in methods using the @em Trajectory module with the parameter
  group @c TrajParameters. It appends the trajectory as a method-specific adjustment.

  @PVM_H1{Implementation}
  In the methods's @c backbone the following steps must be performed after calling
  @ref STB_UpdateTraj :
  - Call @ref PTB_ClearAdjustments
  - Call @ref STB_TrajAppendAdjustment
  - Append further required adjustments by calling @ref PTB_AppendAdjustment
    (e.g. receiver gain)

  @PVM_PARS
  - @c PVM_TrajUpToDate
  - @c PVM_TrajAdjMode
  - @c PVM_TrajAdjustmentRunning

  @PVM_TBCALL
  - @ref PTB_AppendAdjustment

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @ref STB_UpdateTraj must have been called.
  - @ref PTB_ClearAdjustments must have been called.

  @PVM_ERROR
 - Input parameter is not set correctly.

  @PVM_EXAMPLE
  @code
  PTB_ClearAdjustments();
  STB_TrajAppendAdjustment(PVM_SpiralContext);
  PTB_AppendConfiguredAdjustment(per_scan, RCVR);
  @endcode

  @param[in] context Context string
                     - For <c>PVM_TrajAdjMode = Traj_Adj_Auto</c> the measurement is repeated
                       if the context has changed.
                     - Internally, a trajectory context and a context related to the first slice
                       package is added.
                     - @c "" = no method context is provided (use only internal context).
                     - @c NULL = do not use context mechanism.

  @see @ref STB_InitTraj, @ref STB_UpdateTraj, @ref STB_TrajHandleAdjustmentRequests,
       @ref STB_TrajCreateTrajectory, @ref ATB_TrajSetupReco, @ref ATB_TrajInsertB0Correction,
       @ref ATB_TrajSetAdjustmentStatus, @ref ATB_TrajHandleAdjustmentResults, @ref ATB_TrajUpToDate

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_TrajAppendAdjustment(const char *context);

/** Handles an adjustment request for the @em Trajectory module.

  In @c callbackDefs.h redirect @c PVM_AdjHandler to this function.<br>
  If the method uses more adjustments, redirect @c PVM_AdjHandler to the respective method
  function that calls this handler first. From its return value it can be determined if the
  request could be handled.<br>
  For updating parameters before adjustments, the redirected relation of @c PVM_AcqScanHandler
  is called depending on the value of @c PVM_TrajCallAcqUpdate.

  @PVM_PARS
  - @c PVM_TrajAdjustmentRunning

  @PVM_TBCALL
  - @ref PTB_AdjMethSpec
  - @ref PTB_GetAdjCategory
  - @ref PTB_GetAdjName
  - @ref ATB_RgAdjSetGsPars

  @PVM_RELCALL
  - @c PVM_AdjHandler
  - @c PVM_AcqScanHandler if activated in @ref STB_InitTraj (@a callAcqUpdate > @c 0)

  @PVM_PRECOND
  - The parameter group @c TrajParameters must be up-to-date.

  @PVM_ERROR
  NONE

  @return @c Yes = request was handled<br>
          @c  No = request is still to be handled

  @see @ref STB_InitTraj, @ref STB_UpdateTraj, @ref STB_TrajAppendAdjustment,
       @ref STB_TrajCreateTrajectory, @ref  ATB_TrajSetupReco, @ref ATB_TrajInsertB0Correction,
       @ref ATB_TrajSetAdjustmentStatus, @ref ATB_TrajHandleAdjustmentResults, @ref ATB_TrajUpToDate

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_TrajHandleAdjustmentRequests (void);




/**************************************************************************/
/*            gen/src/prg/pvm_toolboxes/mr/PvSeqTools/nuclei.c            */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_Nucleus1Enum (void);
DLL_INTERFACE void STB_Nucleus1 (void);
DLL_INTERFACE void STB_Nucleus2Enum (void);
DLL_INTERFACE void STB_Nucleus2 (void);
DLL_INTERFACE void STB_Nucleus3Enum (void);
DLL_INTERFACE void STB_Nucleus3 (void);
DLL_INTERFACE void STB_Nucleus4Enum (void);
DLL_INTERFACE void STB_Nucleus4 (void);
DLL_INTERFACE void STB_Nucleus5Enum (void);
DLL_INTERFACE void STB_Nucleus5 (void);
DLL_INTERFACE void STB_Nucleus6Enum (void);
DLL_INTERFACE void STB_Nucleus6 (void);
DLL_INTERFACE void STB_Nucleus7Enum (void);
DLL_INTERFACE void STB_Nucleus7 (void);
DLL_INTERFACE void STB_Nucleus8Enum (void);
DLL_INTERFACE void STB_Nucleus8 (void);
DLL_INTERFACE void STB_NumberOfNucleiRange (void);
DLL_INTERFACE void STB_NumberOfNuclei (void);
DLL_INTERFACE void STB_NucleiPpmWork(void);
DLL_INTERFACE void STB_NucleiPpmRef(void);

/** Initialization of nuclei.

  This function initialises parameters belonging to the @c Nuclei pargroup and should be used
  in method initialization. It makes the required number of nuclei visible, and assigns their
  initial values.

  @PVM_PARS
  - @c PVM_NumberOfNuclei;

  @PVM_TBCALL
  - @ref STB_InitFrqPars
  - @ref STB_UpdateNuclei

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_InitNuclei(1);
  @endcode

  @param numberOfNuclei    Required number of nuclei

  @see @ref STB_UpdateNuclei

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitNuclei(int numberOfNuclei);

/** Update of nuclei.

  Updates parameters of the @c Nuclei pargroup. Number of nuclei is limited to the available
  number of transmit channels on the system. For each nucleus, its name (e.g. @c PVM_Nucleus1)
  is derived from the enum value (e.g. @c PVM_Nucleus1Enum), or, if the enum is
  @c OTHER_NUCLEUS, the corresponding name parameter is made editable. In this case it is
  checked if the nucleus name is legal; otherwise @c 1H is assigned.

  Additionally, @c PVM_GradCalConst (Hz/mm) will be calculated based on the gamma of the first
  nucleus and the maximum gradient amplitude.

  @PVM_PARS
  - @c PVM_Nucleus1Enum
  - @c PVM_Nucleus1
  - @c PVM_Nucleus2Enum
  - @c PVM_Nucleus2
  - etc. up to the required number of nuclei
  - @c PVM_NumberOfNuclei (if illegal)
  - @c PVM_GradCalConst

  @PVM_RELCALL
  NONE

  @PVM_TBCALL
  - @ref CFG_GetNumberOfTransmitChannels
  CFG_GradCalConst

  @PVM_PRECOND
  - The nuclei parameters must have been initialised with @ref STB_InitNuclei.

  @PVM_ERROR
  - Updating reference power failed.
  - Updating transmit channel scaling failed.

  @PVM_EXAMPLE
  @code
  STB_UpdateNuclei(No);
  @endcode

  @param enableHomo  If @c No, choosing identical nuclei will be impossible
  @param restPow     (optional - used in adjustment methods only) Yes = restore study reference power (default = Yes)
  @param restTxChan  (optional - used in adjustment methods only) Yes = restore amplitude und phase of (parallel)  transmit channels (default = Yes)

  @see @ref STB_InitNuclei

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateNuclei(YesNo enableHomo, YesNo restPow=Yes, YesNo restTxChan=Yes);

/** Determine nucleus name for a given channel.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The nucleus for the channel @a i must be available in @c PVM_Nucleus\<i\>.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  char *nuc = NULL;
  STB_NucleusName(&nuc, 1);
  @endcode

  @param[out] nname  Pointer which will be set to string containing the nucleus name
  @param      i      Channel number >= @c 1

  @ingroup PVM_STB_INT
*/
DLL_INTERFACE void STB_NucleusName(char **nname,
                     int  i);

/* range checkers and relations */
DLL_INTERFACE void STB_RefPowCh1 (void);
DLL_INTERFACE void STB_RefPowCh2 (void);
DLL_INTERFACE void STB_RefPowCh3 (void);
DLL_INTERFACE void STB_RefPowCh4 (void);
DLL_INTERFACE void STB_RefPowCh5 (void);
DLL_INTERFACE void STB_RefPowCh6 (void);
DLL_INTERFACE void STB_RefPowCh7 (void);
DLL_INTERFACE void STB_RefPowCh8 (void);
DLL_INTERFACE void STB_RefPowMod1 (void);
DLL_INTERFACE void STB_RefPowMod2 (void);
DLL_INTERFACE void STB_RefPowMod3 (void);
DLL_INTERFACE void STB_RefPowMod4 (void);
DLL_INTERFACE void STB_RefPowMod5 (void);
DLL_INTERFACE void STB_RefPowMod6 (void);
DLL_INTERFACE void STB_RefPowMod7 (void);
DLL_INTERFACE void STB_RefPowMod8 (void);
DLL_INTERFACE void STB_RefPowStateRel (void);

/** Deliveres a global reference attenuation for nucleus of interest.

  The reference attenuation (attenuation of a 1ms 90 deg block pulse) is given in the argument
  @a att. It is either the result of the automatic RF gain adjustment or the value specified by
  the user in the @c Nuclei parameter class. This depends on the value of @c PVM_RefAttMod<x>
  for the channel for which this nucleus is selected (@c x = channel). If the desired nucleus
  is assigned to more than one transmit channel, the first matching channel is examined
  (see also channel specific version of @ref STB_GetRefAtt).

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref STB_GetRefPower

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The function should be called after an update of the nuclei group.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  YesNo refAttIsAvailable = No;
  double referenceAttenuation = 0;

  if (PVM_DeriveGains == Yes)
  {
    refAttIsAvailable = STB_GetRefAtt(PVM_Nucleus1, &referenceAttenuation);
  }
  else
  {
    refAttIsAvailable = No;
  }
  @endcode

  @param nuc        String containing the nucleus of interest
  @param att        To double value. It will be modified to the value of reference attenuation
                    only if a return value of @c Yes is returned.

  @return @c Yes if the requested gain is available

  @see @ref ATB_SetRefAtt, @ref CFG_GetSpecRefAtt

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_GetRefAtt(const char *const nuc,
                    double            *att);

/** Deliveres a global reference attenuation for logical transmit channel and nucleus of
    interest.

  Deliveres a global reference attenuation for nucleus of interest assigned to the channel
  @a chan given by argument. The reference attenuation (attenuation of a 1ms 90 deg block pulse)
  is given in the argument @a att. It is either the result of the automatic RF gain adjustment
  or the value specified by the user in the @c Nuclei parameter class. This depends on the
  value of @c PVM_RefAttMod<x> for the channel for which this nucleus is selected
  (@c x = channel).

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_GetSpecRefAtt

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The function should be called after an update of the nuclei group.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  YesNo refAttIsAvailable = No;
  double referenceAttenuation = 0;

  if (PVM_DeriveGains == Yes)
  {
    refAttIsAvailable = STB_GetRefAtt(1, PVM_Nucleus1, &referenceAttenuation);
  }
  else
  {
    refAttIsAvailable = No;
  }
  @endcode

  @param chan       Logical transmit channel nucleus is assigned to.
  @param nuc        String containing the nucleus of interest if a 0 pointer is passed.
                    The nucleus assigned to the channel specified by argument @a chan
                    is not examined.
  @param att        To double value. It will be modified to the value of reference attenuation
                    only if a return value of @c Yes is returned.

  @return @c Yes if the requested gain is available and the nucleus (if given by argument)
          matches the nucleus that is assigned to the logical transmit channel


  @see @ref ATB_SetRefAtt, @ref CFG_GetSpecRefAtt

  @todo Incomplete documentation: PVM_EXAMPLE

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_GetRefAtt(const int         chan,
                    const char *const nuc,
                    double            *att);

/** Get reference power for a channel.

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
  double refpow = 0.0;
  YesNo refPowAvailable = STB_GetRefPower(1, &refpow);
  @endcode

  @param      chan    Channel number <c>[1..8]</c>
  @param[out] refpow  Reference power [W]

  @return
  - @c Yes = successful
  - @c No = @a chan out of range <c>[1..8]</c>

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_GetRefPower(int chan,
                      double *const refpow);





/**************************************************************************/
/*           gen/src/prg/pvm_toolboxes/mr/PvSeqTools/utilities.c          */
/**************************************************************************/

/** Checks a gradient ramp time.

  Checks its argument to be positive. Negative values will be initialised with a default
  value delivered by @ref CFG_GradientRampTime.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_GradientRampTime

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  PVM_MagTransRampTime = STB_InitGradientRamp(PVM_MagTransRampTime);
  @endcode

  @param rampRequest    Ramp time to be checked

  @return Range checked argument

  @see @ref CFG_GradientRampTime

  @ingroup PVM_STB
*/
DLL_INTERFACE double STB_InitGradientRamp(double rampRequest);

/** Calculates a min grad pulse time for given ramptimes.

  Calculates a min duration of a gradient pulse. Arguments are expected to be legal values for
  a ramp up and ramp down time in units of ms.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_InterGradientWaitTime

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Arguments are expected to be legal values for a ramp up and ramp down time in units of ms.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  double min = STB_MinGradPulseTime(PVM_MagTransRampTime ,PVM_MagTransRampTime);
  @endcode

  @param rampUp      Duration of 1st ramp in ms
  @param rampDown    Duration of 2nd ramp in ms

  @return Duration of min grad pulse time [ms]

  @see @ref CFG_InterGradientWaitTime

  @ingroup PVM_STB
*/
DLL_INTERFACE double STB_MinGradPulseTime(double rampUp,
                            double rampDown);

/** Checks legal value of gradient amplitude.

  Checks its argument to lie within [@c 0.0,@c 100.0].

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
  PVM_MagTransSpoilGradLimit = STB_InitGradientAmplitude(PVM_MagTransSpoilGradLimit);
  @endcode

  @param request    Gradient amplitude to be checked [%]

  @return Checked argument

  @ingroup PVM_STB
*/
DLL_INTERFACE double STB_InitGradientAmplitude(double request);




/**************************************************************************/
/*          gen/src/prg/pvm_toolboxes/mr/PvSeqTools/preemphasis.c         */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE int STB_PreemphasisHandler (void);
DLL_INTERFACE void STB_PreemphasisFileEnum (void);
DLL_INTERFACE void STB_PreemphasisFileEnumRange (void);
DLL_INTERFACE void STB_PreemphasisSpecial (void);
DLL_INTERFACE void STB_PreemphasisSpecialRange (void);
DLL_INTERFACE void STB_RampTime (void);

/** Initialises parameters of the @c PVM Preemphasis pargroup.

  @PVM_PARS
  - @c PVM_PreemphasisSpecial
  - @c PVM_PreemphasisFileEnum

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdatePreemphasis

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitPreemphasis(void);

/** Updates parameters of the @c PVM Preemphasis pargroup.
  The state of @c PVM_PreemphasisFileEnum represents a valid preemphasis file in the standard
  preemphasis file location of the current active system status. All files that contain the
  appropriate information and in addition do not contain special characters inside the filename
  (only alphanumeric characters and "-" and "_" are allowed).

  @PVM_PARS
  - @c PVM_PreemphasisSpecial     YesNo parameter used to enable selection of special
                                  preemphasis file
  - @c PVM_PreemphasisFileEnum    Parameter representing valid preemphasis files present in the
                                  standard preemphasis file location of the current active
                                  gradient status
  - @c ACQ_preemp_file

  @PVM_TBCALL
  - @ref CFG_StatusString

  @PVM_RELCALL
  - Relations of @c ACQ_preemp_file

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @return @c Yes

  @see @ref STB_InitPreemphasis, @ref CFG_StatusString

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_UpdatePreemphasis(void);




/**************************************************************************/
/*             gen/src/prg/pvm_toolboxes/mr/PvSeqTools/seDiffPrep.c       */
/*             gen/src/prg/pvm_toolboxes/mr/PvSeqTools/steDiffPrep.c      */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_DwMinTmRange (void);
DLL_INTERFACE void STB_DwMinTm (void);
DLL_INTERFACE void STB_DwTmRange (void);
DLL_INTERFACE void STB_DwTm (void);
DLL_INTERFACE void STB_TmDwVarSpoilGrad (void);
DLL_INTERFACE void STB_TmDwSliceSpoilGradDurRange (void);
DLL_INTERFACE void STB_TmDwSliceSpoilGradDur (void);
DLL_INTERFACE void STB_TmDwSliceSpoilGradLimRange (void);
DLL_INTERFACE void STB_TmDwSliceSpoilGradLim (void);
DLL_INTERFACE void STB_TmDwSliceSpoilGradRange (void);
DLL_INTERFACE void STB_TmDwSliceSpoilGrad (void);
DLL_INTERFACE void STB_HandleSteDwVisibility (YesNo);



/**************************************************************************/
/*      gen/src/prg/pvm_toolboxes/mr/PvSeqTools/diffusionPreparation.c    */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_DwPeriodsRange (void);
DLL_INTERFACE void STB_DwPeriods (void);
DLL_INTERFACE void STB_DwMinPeriodsRange (void);
DLL_INTERFACE void STB_DwMinPeriods (void);
DLL_INTERFACE void STB_DwMinPerSepRange (void);
DLL_INTERFACE void STB_DwMinPerSep (void);
DLL_INTERFACE void STB_DwPerSepRange (void);
DLL_INTERFACE void STB_DwPerSep (void);
DLL_INTERFACE int STB_DwGradTrajRange (void);
DLL_INTERFACE void STB_DwGradTraj (void);
DLL_INTERFACE int STB_DwEffGradTrajRange (void);
DLL_INTERFACE void STB_DwEffGradTraj (void);
DLL_INTERFACE void STB_DwDeriveGainsRange (void);
DLL_INTERFACE void STB_DwDeriveGains (void);
DLL_INTERFACE int STB_DwDgSwitchRange (void);
DLL_INTERFACE void STB_DwDgSwitch (void);
DLL_INTERFACE int STB_DwMeasModeRange (void);
DLL_INTERFACE void STB_DwMeasMode (void);
DLL_INTERFACE int STB_DiffPrepModeRange (void);
DLL_INTERFACE void STB_DiffPrepMode (void);
DLL_INTERFACE void STB_DwRfcAdiab(void);
DLL_INTERFACE void STB_DwRfcPulse1EnumRange (void);
DLL_INTERFACE void STB_DwRfcPulse1Enum (void);
DLL_INTERFACE void STB_DwRfcPulse1Range (void);
DLL_INTERFACE void STB_DwRfcPulse1 (void);
DLL_INTERFACE void STB_DwRfcPulse1Ampl (void);
DLL_INTERFACE void STB_DwRfcPulse1Shape (void);
DLL_INTERFACE void STB_DwExcPulse1EnumRange (void);
DLL_INTERFACE void STB_DwExcPulse1Enum (void);
DLL_INTERFACE void STB_DwExcPulse1Range (void);
DLL_INTERFACE void STB_DwExcPulse1 (void);
DLL_INTERFACE void STB_DwExcPulse1Ampl (void);
DLL_INTERFACE void STB_DwExcPulse1Shape (void);
DLL_INTERFACE void STB_DwVisibleParsRange (void);
DLL_INTERFACE void STB_DwVisiblePars (void);
DLL_INTERFACE void STB_DwUsedSliceThickRange (void);
DLL_INTERFACE void STB_DwUsedSliceThick (void);
DLL_INTERFACE void STB_DwD (void);
DLL_INTERFACE void STB_DwModDur (void);
DLL_INTERFACE void STB_DwModDurRange (void);
DLL_INTERFACE void STB_DwModEchDelRange (void);
DLL_INTERFACE void STB_DwModEchDel (void);
DLL_INTERFACE void STB_TeDwSliceSpoilGradDurRange (void);
DLL_INTERFACE void STB_TeDwSliceSpoilGradDur (void);
DLL_INTERFACE void STB_DwSliceGradDurRange (void);
DLL_INTERFACE void STB_DwSliceGradDur (void);
DLL_INTERFACE void STB_DwSliceGradRange (void);
DLL_INTERFACE void STB_DwSliceGrad (void);
DLL_INTERFACE void STB_DwSliceGradLimRange (void);
DLL_INTERFACE void STB_DwSliceGradLim (void);
DLL_INTERFACE void STB_TeDwSliceSpoilGradRange (void);
DLL_INTERFACE void STB_TeDwSliceSpoilGrad (void);
DLL_INTERFACE void STB_TeDwSliceSpoilGradLimRange (void);
DLL_INTERFACE void STB_TeDwSliceSpoilGradLim (void);
DLL_INTERFACE void STB_DwGradDurRange (void);
DLL_INTERFACE void STB_DwGradDur (void);
DLL_INTERFACE void STB_DwGradSepRange (void);
DLL_INTERFACE void STB_DwGradSep (void);
DLL_INTERFACE void STB_DwGradPulseDelayRange (void);
DLL_INTERFACE void STB_DwGradPulseDelay (void);
DLL_INTERFACE void STB_DwGradSepDelayRange (void);
DLL_INTERFACE void STB_DwGradSepDelay (void);
DLL_INTERFACE void STB_DwEnterDiffDirRange (void);
DLL_INTERFACE void STB_DwEnterDiffDir (void);
DLL_INTERFACE void STB_DwDirectScaleRange (void);
DLL_INTERFACE void STB_DwDirectScale (void);
DLL_INTERFACE void STB_DwNDiffDirRange (void);
DLL_INTERFACE void STB_DwNDiffDir (void);
DLL_INTERFACE void STB_DwNDiffExpEachRange (void);
DLL_INTERFACE void STB_DwNDiffExpEach (void);
DLL_INTERFACE void STB_DwNDiffExpRange (void);
DLL_INTERFACE void STB_DwNDiffExp (void);
DLL_INTERFACE void STB_DwAoImagesRange (void);
DLL_INTERFACE void STB_DwAoImages (void);
DLL_INTERFACE void STB_DwDirRange (void);
DLL_INTERFACE void STB_DwDir (void);
DLL_INTERFACE void STB_DwSpDirRange (void);
DLL_INTERFACE void STB_DwSpDir (void);
DLL_INTERFACE void STB_DwSpDir2Range (void);
DLL_INTERFACE void STB_DwSpDir2 (void);
DLL_INTERFACE void STB_DwDirInputRange (void);
DLL_INTERFACE void STB_DwDirInput (void);
DLL_INTERFACE void STB_DwGradAmpRange (void);
DLL_INTERFACE void STB_DwGradAmp (void);
DLL_INTERFACE void STB_DwMaxBvalRange (void);
DLL_INTERFACE void STB_DwMaxBval (void);
DLL_INTERFACE void STB_DwBvalEachRange (double);
DLL_INTERFACE void STB_DwBvalEach (void);
DLL_INTERFACE void STB_DwGradStrRange (void);
DLL_INTERFACE void STB_DwGradStr (void);
DLL_INTERFACE void STB_DwEffGradStrRange (void);
DLL_INTERFACE void STB_DwEffGradStr (void);
DLL_INTERFACE void STB_DwEffBvalRange (void);
DLL_INTERFACE void STB_DwEffBval (void);
DLL_INTERFACE void STB_DwBMatRange (void);
DLL_INTERFACE void STB_DwBMat (void);
DLL_INTERFACE void STB_DwGradVecRange (void);
DLL_INTERFACE void STB_DwGradVec (void);
DLL_INTERFACE void STB_DwGradReadRange (void);
DLL_INTERFACE void STB_DwGradRead (void);
DLL_INTERFACE void STB_DwGradPhaseRange (void);
DLL_INTERFACE void STB_DwGradPhase (void);
DLL_INTERFACE void STB_DwGradSliceRange (void);
DLL_INTERFACE void STB_DwGradSlice (void);
DLL_INTERFACE double STB_DwMinGSwitchTime (void);
DLL_INTERFACE void STB_DwCalcDirs(void);

/** Initialises all parameters of the @c Diffusion class.

  Initialization of the diffusion pargroup parameters, checks whether these parameters are
  within a legal range, provides default values in case they are not initialised. The
  initialization of the @c DW_SliceSelection parameters is dependent on the legal
  (range checked) value of parameter @c PVM_DiffPrepMode. The interdependencies of these
  parameters are not resolved by this function which is intended to be called in the
  initMeth routine of a method.

  @PVM_PARS
  - Parameters of @c DW_SliceSelection pargroup
    - @c PVM_DwTm
    - @c PVM_DwMinTm
    - @c PVM_DwExcPulse1Enum
    - @c PVM_DwExcPulse1
    - @c PVM_DwRfcPulse1Enum
    - @c PVM_DwRfcPulse1
    - @c PVM_DwSliceGradDur
    - @c PVM_DwSliceGrad
    - @c PVM_DwSliceGradLim
    - @c PVM_TeDwSliceSpoilGradDur
    - @c PVM_TeDwSliceSpoilGrad
    - @c PVM_TeDwSliceSpoilGradLim
    - @c PVM_TmDwSliceSpoilGradDur
    - @c PVM_TmDwSliceSpoilGrad
    - @c PVM_TmDwSliceSpoilGradLim

  - Parameters of main @c Diffusion pargroup
    - @c PVM_DiffPrepMode
    - @c PVM_DwUsedSliceThick
    - @c PVM_DwVisiblePars
    - @c PVM_DwGradDur
    - @c PVM_DwGradSep
    - @c PVM_DwDirectScale
    - @c PVM_DwMeasMode
    - @c PVM_DwNDiffDir
    - @c PVM_DwNDiffExpEach
    - @c PVM_DwAoImages
    - @c PVM_DwEnterDiffDir
    - @c PVM_DwDirInput
    - @c PVM_DwDir
    - @c PVM_DwDgSwitch
    - @c PVM_DwMaxBval
    - @c PVM_DwBvalEach
    - @c PVM_DwGradAmp
    - @c PVM_DwGradStr
    - @c PVM_DwEffGradStr

  -  Parameters of the @c DiffOutput pargroup
    - @c PVM_DwNDiffExp
    - @c PVM_DwBMat
    - @c PVM_DwEffBval
    - @c PVM_DwGradVec
    - @c PVM_DwGradRead
    - @c PVM_DwGradPhase
    - @c PVM_DwGradSlice
    - @c PVM_DwPeriods
    - @c PVM_DwMinPeriods
    - @c PVM_DwPerSep
    - @c PVM_DwMinPerSep
    - @c PVM_DwGradPulseDelay
    - @c PVM_DwGradSepDelay
    - @c PVM_DwModDur
    - @c PVM_DwModEchDel

  @PVM_TBCALL
  - Range checking functions for each of the parameters listed above. For each parameter
    @c PVM_xxx listed above a function @c STB_xxxRange is defined which is used to ensure
    legal values. Each of these functions modifies only the parameter it is defined for.
  - @ref STB_InitRFPulse

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Legal values of the argument @a allVisible (@c Yes, @c No). Any different value will be
    treated as @c No.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_InitDiffusionPreparation(Yes);
  @endcode

  @param allVisible    Specifies whether the non editable parameters are shown in the method
                       editor or not. This is done by passing the value to parameter
                       @c PVM_DwVisiblePars.

  @see @ref STB_UpdateDiffusionPreparation, @ref ATB_DwAcq

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitDiffusionPreparation(YesNo allVisible);

/** Updates the parameters of the @c Diffusion pargroup.

  Creates a consistant parameter state of these parameters. All interdependencies are resolved
  by this function. Also the visibility, editability, and storage in the method file (for
  protocol generation) is handled dependent on the selected preparation and measurement mode.
  Please note, the direction of the diffusion sensitizing gradients is only dependent on the
  value of parameter @c PVM_DwDir. In case @c PVM_DwEnterDiffDir is set to @c Yes, the values
  of @c PVM_DwDirInput will be passed to @c PVM_DwDir according to the state of
  @c PVM_DwDirectScale (in case of @c No,the norm of each direction vector is constrained to be
  @c 1.0). The direction information provided by @c PVM_DwDir and the amplitude information
  defined by @c PVM_DwGradAmp will be passed to @c PVM_DwGradVec, @c PVM_DwGradRead,
  @c PVM_DwGradPhase, and @c PVM_DwGradSlice respectively which are used in the pulseprogram
  part of the diffusion module.

  Please note
  - Parameters of the @c DwOutput class will be set according to the consistant parameter state
    of the parameters in the @c DW_SliceSelection and the main @c Diffusion pargroup.
  - If @c PVM_DwDirectScale is set to @c Yes only one slice package may be specified. An
    appropriate constain should be done using @ref STB_UpdateSliceGeoPars in the methods
    @c backbone routine.

  This function is intended to be used in the method's @c backbone.

  In detail
  - Correct setting of dimensionality parameters according to the specified measurement modes
    as defined by @c PVM_DwMeasMode
  - Proper dimensionality of array parameters according to the number of diffusion experiments
    as specified by
    - @c PVM_DwNDiffDir
    - @c PVM_DwNDiffExpEach
    - @c PVM_DwAoImages
  - b-value and b-matrix calculations

  @PVM_PARS
  - Parameters of @c DW_SliceSelection pargroup
    - @c PVM_DwTm
    - @c PVM_DwMinTm
    - @c PVM_DwExcPulse1Enum
    - @c PVM_DwExcPulse1
    - @c PVM_DwExcPulse1Ampl
    - @c PVM_DwRfcPulse1Enum
    - @c PVM_DwRfcPulse1
    - @c PVM_DwRfcPulse1Ampl
    - @c PVM_DwSliceGradDur
    - @c PVM_DwSliceGrad
    - @c PVM_DwSliceGradLim
    - @c PVM_TeDwSliceSpoilGradDur
    - @c PVM_TeDwSliceSpoilGrad
    - @c PVM_TeDwSliceSpoilGradLim
    - @c PVM_TmDwSliceSpoilGradDur
    - @c PVM_TmDwSliceSpoilGrad
    - @c PVM_TmDwSliceSpoilGradLim

  - Parameters of main @c Diffusion pargroup
    - @c PVM_DiffPrepMode
    - @c PVM_DwUsedSliceThick
    - @c PVM_DwVisiblePars
    - @c PVM_DwDeriveGains
    - @c PVM_DwGradDur
    - @c PVM_DwGradSep
    - @c PVM_DwDirectScale
    - @c PVM_DwMeasMode
    - @c PVM_DwNDiffDir
    - @c PVM_DwNDiffExpEach
    - @c PVM_DwAoImages
    - @c PVM_DwEnterDiffDir
    - @c PVM_DwDirInput
    - @c PVM_DwDir
    - @c PVM_DwDgSwitch
    - @c PVM_DwMaxBval
    - @c PVM_DwBvalEach
    - @c PVM_DwGradAmp
    - @c PVM_DwGradStr
    - @c PVM_DwEffGradStr

  - Parameters of the @c DiffOutput pargroup
    - @c PVM_DwNDiffExp
    - @c PVM_DwBMat
    - @c PVM_DwEffBval
    - @c PVM_DwGradVec
    - @c PVM_DwGradRead
    - @c PVM_DwGradPhase
    - @c PVM_DwGradSlice
    - @c PVM_DwPeriods
    - @c PVM_DwMinPeriods
    - @c PVM_DwPerSep
    - @c PVM_DwMinPerSep
    - @c PVM_DwGradPulseDelay
    - @c PVM_DwGradSepDelay
    - @c PVM_DwModDur
    - @c PVM_DwModEchDel

  @PVM_TBCALL
  - Range checking functions of diffusion parameters above
  - @ref CFG_IsNucleusKnown
  - @ref CFG_GradCalConst
  - @ref STB_UpdateRFPulse

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The argument of the diffusion updating function should have their legal and final values
    (see below).

  @PVM_ERROR
  - Illegal values of @c PVM_DiffPrepMode will be corrected to SpinEcho mode an appropriate
    message will be printed out in the shell start window of ParaVision. No call of
    @ref UT_ReportError.

  @PVM_EXAMPLE
  @code
   // backbones code
   // (...)

  sliceThick = MAX_OF(PVM_SliceThick,minSliceThick);

  UpdateMinEchoTime(&minte1,&minte2);
  prepEchoTime = PVM_EchoTime;

  status = STB_UpdateDiffusionPreparation(&sliceThick,
					  PVM_DeriveGains,
					  PVM_Nucleus1,
					  PtrType3x3 PVM_SPackArrGradOrient[0],
					  minte1,
					  minte2,
					  &prepEchoTime);

  if(status == 1)
  {
    DB_MSG(("DIFFUSION UPDATER: changed min slice thickness"));
    minSliceThick=sliceThick;
    if(dim==3)
    {
      minFov[2]=minSliceThick;
    }
  }

    // (...)
  @endcode

  @param[in,out] sliceThick    Points to a double value containing the desired slice  thickness. In
                               case this thickness is less than the minimum slice thickness as
                               calculated by the slice selective excitation/refocussing of the
                               @c Diffusion class, the value will be modified to the minimum possible
                               slice thickness. This will result in a return value of @c 1.
                               A value \f$\le\f$ @c 0 is allowed, in this case no slice selection gradient
                               is switched during the RF-pulse.
  @param         deriveGains   (Legal values @c Yes @c No, other values default to @c No)
                               A call with value @c Yes will force the attenuation of the pulse to
                               be derived from its flip angle.
  @param[in,out] nuc           String containing the nucleus name. Any illegal value defaults
                               to protons (1H).
  @param[in]     gradMatrix    Array containing the gradient matrix of an imaging experiment. This
                               value is used only in case @c PVM_DwDirectScale is set to @c Yes to
                               transform the diffusion gradient directions into the rps slice
                               coordinate system for b-matrix calculations.
  @param         ted1          Total time before the diffusion module in which the magnetization
                               used to produce images is transversal (e.g. mid of excitation pulse
                               until start of @em dti module).
  @param         ted2          Total time after the diffusion module needed by the sequence in order
                               to acquire the central echo signal produced by the @c dti module end
                               of diffusion module.
  @param[in,out] prepEchoTime  Points to a double value which should be initialised with the
                               requested echo time of the diffusion preparation. If the requested
                               echo time is larger than the echo time needed by the diffusion module
                               to realize the timing of the diffusion gradient switching
                               (defined by @c PVM_DwGradDur and @c PVM_DwGradSep), this time will be
                               filled up by delays of the @c Diffusion module. The minimum
                               preparation echo time needed to realize the timing will be returned in
                               this argument.

  @return 1 in case the slice thickness had to be modified

  @see   @ref STB_UpdateSliceGeoPars, @ref STB_InitDiffusionPreparation, @ref ATB_DwAcq

  @ingroup PVM_STB
*/
DLL_INTERFACE int STB_UpdateDiffusionPreparation(double *const sliceThick,
                                   YesNo         deriveGains,
                                   char          *nuc,
                                   double        (*gradMatrix)[3][3],
                                   double        ted1,
                                   double        ted2,
                                   double        *prepEchoTime);




/**************************************************************************/
/*             gen/src/prg/pvm_toolboxes/mr/PvSeqTools/encoding.c         */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_EncCommonRels(void);
DLL_INTERFACE void STB_EncZf(void);
DLL_INTERFACE void STB_EncPpi(void);
DLL_INTERFACE void STB_EncPft(void);
DLL_INTERFACE void STB_EncPftOverscans(void);

/** Initialises the @em Encoding group for spectroscopy methods.

  This function should be called in spectroscopy methods using the
  @em Encoding group, which contains parameters handling multiple receivers.

  @PVM_PARS
  - @c PVM_EncSpectroscopy

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateEncoding, @ref STB_UpdateEncodingSpecial, @ref STB_UpdateEncodingForEpi

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitEncoding(void);

/** Initialises the @em Encoding group.

  This function should be called in imaging methods using the @em Encoding group.

  @PVM_PARS
  - @c PVM_EncSpectroscopy
  - @c PVM_SpatDimEnum
  - @c PVM_DefMatrix
  - @c PVM_Matrix
  - @c PVM_MaxAntiAlias
  - @c PVM_AntiAlias

  @PVM_TBCALL
  - @ref CFG_DefaultImagingSpatialMatrixSize
  - @ref CFG_MaxAntiAliasFactor

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Incorrect input parameters

  @PVM_EXAMPLE
  @code
  int dimRange[2] = {2, 3};
  int loMat[3] = {8, 8, 8};
  int upMat[3] = {256, 256, 256};
  int dfMat[3] = {256, 128, 32};

  STB_InitEncoding(2, dimRange, loMat, upMat, dfMat);
  @endcode

  @param     diminit            Default spatial dimension if @c PVM_SpatDimEnum has no legal value
  @param[in] spatialDimRange    Two element integer array containing the lowest
                                (@a spatialDimRange[0]) and highest (@a spatialDimRange[1])
                                spatial dimensionality
  @param[in] lowerMatrixSize    Three-element integer array containing the lowest image matrix
                                size for each spatial dimension
  @param[in] upperMatrixSize    Three-element integer array containing the highest image matrix
                                size for each spatial dimension
  @param[in] defaultMatrixSize  Three-element integer array containing the default image matrix
                                size for each spatial dimension

  @see @ref STB_UpdateEncoding, @ref STB_UpdateEncodingSpecial, @ref STB_UpdateEncodingForEpi

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitEncoding(int diminit,
                                    int *spatialDimRange,
                                    int *lowerMatrixSize ,
                                    int *upperMatrixSize,
                                    int *defaultMatrixSize);




/** Updates the @em Encoding group (special variant).

  @PVM_USEINSTEAD{STB_UpdateEncodingSpecial}

  @todo Incomplete documentation: PVM_DEACT, PVM_PARS, PVM_TBCALL, PVM_RELCALL,
                                  PVM_PRECOND, PVM_ERROR, PVM_EXAMPLE, param, return
  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE int STB_UpdateEncodingSpecial(int       dimension,
                                            const int *imageSize,
                                            double    *antiAlias,
                                            int       *segmentSize,
                                            const     SEG_MODE *segmode,
                                            const     YesNo *piAllow,
                                            const     YesNo *piRefAllow,
                                            const     YesNo *pfAllow,
                                            const     YesNo *zfAllow,
                                            const     YesNo *adjustStart);




/** Updates the @em Encoding group (special variant).

  This function should be called in imaging methods using the @em Encoding group,
  which contains parameters describing the encoding strategy, including:
  - spatial dimensionality
  - image matrix size (@c PVM_Matrix)
  - anti-aliasing factor (@c PVM_AntiAlias)
  - interpolation by zero-filling (ZF)
  - partial Fourier (PF)
  - parallel imaging (PI)
  - segmentation

  This function should only be called in methods which need some non-standard encoding modes:
  - non-standard encoding in read dimension (PF)
  - non-standard encoding in other than 1st phase dimension (PI, PF)
  - no frequency encoding (e.g. SPI, CSI)
  - non-Cartesian encoding (e.g. Spiral, Radial)
  Standard methods should use the function @ref STB_UpdateEncoding, which has a simplified
  interface.

  After the call of this function, all group members have up-to-date values.
  In particular, @c PVM_Matrix contains the image matrix size,
  @c PVM_EncMatrix  the effective acquisition matrix size
  that is needed to obtain an image with the specified size using the specified
  accelerations,
  and @c PVM_EncValues? contain the values for the phase encoding gradient functions.
  The number of spatial dimensions can be obtained by @ref PTB_GetSpatDim.

  @PVM_PARS
  - @c PVM_SpatDimEnum
  - @c PVM_Matrix
  - @c PVM_AntiAlias
  - @c PVM_EncZf
  - @c PVM_EncPft
  - @c PVM_EncPftOverscans
  - @c PVM_EncPpi
  - @c PVM_EncPpiRefLines
  - @c PVM_EncOrder
  - @c PVM_EncStart
  - @c PVM_EncMatrix
  - @c PVM_EncTotalAccel
  - @c PVM_EncSteps0
  - @c PVM_EncSteps1
  - @c PVM_EncSteps2
  - @c PVM_EncCentralStep0
  - @c PVM_EncCentralStep1
  - @c PVM_EncCentralStep2
  - @c PVM_EncValues0
  - @c PVM_EncValues1
  - @c PVM_EncValues2

  @PVM_TBCALL
  - @ref CFG_NReceivers
  - @ref MRT_RareEncodingStart

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The @em Encoding group must have been initialised with @ref STB_InitEncoding.

  @PVM_ERROR
  - Incorrect input parameters
  - An error was returned by a @em SysInfo function
  - More coil elements than receivers were detected

  @PVM_EXAMPLE
  3D radial encoding (e.g. ZTE)
  @code
  STB_UpdateEncodingSpecial(3, 3, Yes);
  @endcode

  2D Spiral with optional phase encoding in 3rd dimension
  @code
  SEG_MODE seg_mode      = SEG_SEQUENTIAL;
  const YesNo piAllow    = Yes;
  const YesNo piRefAllow = Yes;
  const YesNo zfAllow    = Yes;
  const YesNo autoStart  = Yes;

  STB_UpdateEncodingSpecial(
      2,
      2,
      Yes,
      NULL,
      &seg_mode,
      &piAllow,
      &piRefAllow,
      NULL,
      &zfAllow,
      &autoStart);
  @endcode

  @param         dimNonPhase   Number of spatial dimensions which are not phase-encoded,
                               for which holds:
                               - are always the leading dimensions
                               - are usually of identical image size
                               - phase encoding options will be disabled (PI, PF, encoding order)
                               - the same ZF factor will be used (only for @a dimNonPhase > @c 1)
  @param         dimIsoMatrix  Number of dimensions with isotropic image size (matrix)
  @param         evenIsoMatrix @c Yes = make isotropic image size an even number
  @param[in,out] segSize        Table of required segment sizes (RARE factors)
                               - may be changed
                               - @c NULL = all @c 1
  @param         segMode       Table of segmentation mode
                               - @c SEG_SEQUENTIAL or @c SEG_INTERLEAVED
                               - @c NULL = all @c SEG_SEQUENTIAL
  @param         piAllow       Table, where @c Yes = PI is allowed
                               - @c NULL = all @c No
  @param         piRefAllow    Table, where @c Yes = PI reference lines are allowed
                               - @c NULL = all @c No
  @param         pfAllow       Table, where @c Yes = PF is allowed
                               - @c NULL = all @c No
  @param         zfAllow       Table, where @c Yes = ZF is allowed
                               - @c NULL = all @c No
  @param         adjustStart   Table, where @c Yes = <em>k</em>-space start should be adjusted
                               when segmenting
                               - @c NULL = all @c No
  @param         encodingScheme @c encodingScheme < 3, traditional encoding 
                                - encodingScheme >= 3 (3 or 4), 3D phase encoding
  @param[out]    echoPosition  Determines if PF is allowed in the read dimension
                               - @c NULL = do not allow
                               - otherwise: allow and return echo position in this parameter
                                 (only for @a dimNonPhase = @c 1)

  @PVM_H2{Note}
  The parameters no. 4 - 10 are tables for the phase-encoded dimensions and must have the size
  @a dimension - @a dimNonPhase.

  @return
          - @c 0 = all elements of @a segSize were accepted
          - @c 1 = at last one element of @a segSize was changed

  @see @ref STB_InitEncoding, @ref STB_UpdateEncoding, @ref STB_UpdateEncodingForEpi

  @ingroup PVM_STB
*/
DLL_INTERFACE int STB_UpdateEncodingSpecial(int             dimNonPhase,
                                            int             dimIsoMatrix,
                                            YesNo           evenIsoMatrix,
                                            int            *segSize      = NULL,
                                            const SEG_MODE *segMode      = NULL,
                                            const YesNo    *piAllow      = NULL,
                                            const YesNo    *piRefAllow   = NULL,
                                            const YesNo    *pfAllow      = NULL,
                                            const YesNo    *zfAllow      = NULL,
                                            const YesNo    *adjustStart  = NULL,
                                            double         *echoPosition = NULL,
                                            int            encodingScheme= 0);




/** Updates the @em Encoding group (deprecated).

  @PVM_USEINSTEAD{STB_UpdateEncoding}

  @todo Incomplete documentation: PVM_DEACT, PVM_PARS, PVM_TBCALL, PVM_RELCALL,
                                  PVM_PRECOND, PVM_ERROR, PVM_EXAMPLE, param, return
  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE int STB_UpdateEncoding(int       dimension,
                                     const int *imageSize,
                                     double    *antiAlias,
                                     int       *segmentSize,
                                     SEG_MODE  segMode,
                                     YesNo     piAllow,
                                     YesNo     piRefAllow,
                                     YesNo     pfAllow,
                                     int       encodingScheme = 0);




/** Updates the @em Encoding group for spectroscopy methods.

  This function should be called in spectroscopy methods using the @em Encoding group,
  which contains parameters handling multiple receivers.

  @PVM_PARS
  - @c PVM_EncAvailReceivers
  - @c PVM_EncUseMultiRec
  - @c PVM_EncActReceivers

  @PVM_TBCALL
  - @ref CFG_NReceivers

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The spectroscopy version of @ref STB_InitEncoding must have been called.

  @PVM_ERROR
  - An error was returned by a @em SysInfo function.
  - More coil elements than receivers were detected.

  @see @ref STB_InitEncoding, @ref STB_UpdateEncodingSpecial, @ref STB_UpdateEncodingForEpi

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateEncoding(void);




/** Updates the @em Encoding group.

  This function should be called in imaging methods using the @em Encoding group,
  which contains parameters describing the encoding strategy, including:
  - spatial dimensionality
  - image matrix size (@c PVM_Matrix)
  - anti-aliasing factor (@c PVM_AntiAlias)
  - interpolation by zero-filling (ZF)
  - partial Fourier (PF)
  - parallel imaging (PI)
  - segmentation

  This function should be called in standard methods:
  - Cartesian, linear encoding
  - frequency encoding in first dimension (without PF)
  - no parallel acquisition, no PF in other than 1st phase dimension
  - no EPI
  Methods which need some non-standard encoding modes should use
  @ref STB_UpdateEncodingSpecial, which has a more general interface.
  @em EPI module-based methods should use @ref STB_UpdateEncodingForEpi.

  After the call of this function, all group members have up-to-date values.
  In particular, @c PVM_EncMatrix contains the image matrix size,
  @c PVM_EncMatrix  the effective acquisition matrix size
  that is needed to obtain an image with the specified size using the specified
  accelerations,
  and @c PVM_EncValues contains the values for the phase encoding gradient function.
  The number of spatial dimensions can be obtained by @ref PTB_GetSpatDim.

  @PVM_PARS
  - @c PVM_SpatDimEnum
  - @c PVM_Matrix
  - @c PVM_AntiAlias
  - @c PVM_EncZf
  - @c PVM_EncPft
  - @c PVM_EncPftOverscans
  - @c PVM_EncPpi
  - @c PVM_EncPpiRefLines
  - @c PVM_EncOrder
  - @c PVM_EncStart
  - @c PVM_EncMatrix
  - @c PVM_EncTotalAccel
  - @c PVM_EncSteps1
  - @c PVM_EncSteps2
  - @c PVM_EncCentralStep1
  - @c PVM_EncCentralStep2
  - @c PVM_EncValues1
  - @c PVM_EncValues2
  - @c PVM_EncAvailReceivers
  - @c PVM_EncUseMultiRec
  - @c PVM_EncActReceivers

  @PVM_TBCALL
  - @ref STB_UpdateEncodingSpecial
  - @ref CFG_NReceivers
  - @ref MRT_RareEncodingStart

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The @em Encoding group must have been initialised with @ref STB_InitEncoding.

  @PVM_ERROR
  - An error was returned by a @em SysInfo function.
  - More coil elements than receivers were detected.

  @PVM_EXAMPLE
  @code
  STB_UpdateEncoding(
    NULL,
    SEG_SEQUENTIAL,
    Yes,
    Yes,
    Yes,
    int);
  @endcode

  @param[in,out] segSize        Pointer to required segment size (RARE factor)
                                - affects 2nd dimension (@em Phase)
                                - may be changed
                                - @c NULL = all @c 1
  @param         segMode        Segmentation mode
                                - affects 2nd dimension (@em Phase)
                                - @c SEG_SEQUENTIAL or @c SEG_INTERLEAVED
  @param         piAllow        @c Yes = PI is allowed
                                - affects 2nd dimension (@em Phase)
  @param         piRefAllow     @c Yes = PI reference lines are allowed
                                - affects 2nd dimension (@em Phase)
  @param         pfAllow        @c Yes = PF is allowed
                                - affects 2nd and 3rd dimension (@em Phase and @em Slice)
  @param         encodingScheme @c encodingScheme < 3, traditional encoding 
                                - encodingScheme >= 3 (3 or 4), 3D phase encoding
  @param[out]    echoPosition   Determines if PF is allowed in the 1st dimension (@em Read)
                                - @c NULL = do not allow (default)
                                - otherwise: allow and return echo position in this parameter

  @return
          @c 0 = @a segSize was accepted
          @c 1 = @a segSize was changed

  @see @ref STB_InitEncoding, @ref STB_UpdateEncodingSpecial, @ref STB_UpdateEncodingForEpi

  @ingroup PVM_STB
*/
DLL_INTERFACE int STB_UpdateEncoding(int       *segSize,
                       SEG_MODE  segMode,
                       YesNo     piAllow,
                       YesNo     piRefAllow,
                       YesNo     pfAllow,
                       double    *echoPosition = NULL,
                       int       encodingScheme = 0);




/** Updates the @em Encoding group (deprecated).

  @PVM_USEINSTEAD{STB_UpdateEncoding}

  @todo Incomplete documentation: PVM_DEACT, PVM_PARS, PVM_TBCALL, PVM_RELCALL,
                                  PVM_PRECOND, PVM_ERROR, PVM_EXAMPLE, param, return
  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE int STB_UpdateEncodingForEpi(int       dimension,
                             const int *imageSize,
                             double    *antiAlias,
                             int       *numberOfSegments,
                             YesNo     piAllow,
                             YesNo     piRefAllow,
                             YesNo     pfAllow);



/** Updates the @em Encoding group for @em EPI-type methods.

  This function should be called in methods using the @em Encoding group, which contains
  parameters describing the encoding strategy, including:
  - spatial dimensionality
  - image matrix size (@c PVM_Matrix)
  - anti-aliasing factor (@c PVM_AntiAlias)
  - interpolation by zero-filling (ZF)
  - partial Fourier (PF)
  - parallel imaging (PI)
  - segmentation

  This function should be called in standard methods (with Cartesian, linear encoding, no
  parallel acquisition, no partial FT in the 3rd dimension, no @em EPI). Methods which need
  some non-standard encoding modes should use @ref STB_UpdateEncodingSpecial, which has a more
  general interface. @em EPI module-based methods should use @ref STB_UpdateEncodingForEpi.

  This function is a special variant of @ref STB_UpdateEncoding adapted for
  @em EPI experiments. It sets the segmenting mode to interleaved, disables automatic encoding
  start adjustment, and accepts the number of segments (shots) instead of segment size.

  After the call of this function, all group members have up-to-date values.
  In particular, @c PVM_EncMatrix contains the effective acquisition matrix size
  that is needed to obtain an image with the specified size using the specified accelerations,
  and @c PVM_EncValues contains the values for the phase encoding gradient function.

  @PVM_PARS
  - @c PVM_EncZf
  - @c PVM_EncPft
  - @c PVM_EncPftOverscans
  - @c PVM_EncPpi
  - @c PVM_EncPpiRefLines
  - @c PVM_EncOrder
  - @c PVM_EncStart
  - @c PVM_EncMatrix
  - @c PVM_EncTotalAccel
  - @c PVM_EncSteps1
  - @c PVM_EncSteps2
  - @c PVM_EncCentralStep1
  - @c PVM_EncCentralStep2
  - @c PVM_EncValues1
  - @c PVM_EncValues2

  @PVM_TBCALL
  - @ref STB_UpdateEncodingSpecial
  - @ref CFG_NReceivers
  - @ref MRT_RareEncodingStart

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The @em Encoding group must have been initialised with @ref STB_InitEncoding.

  @PVM_ERROR
  - An error was returned by a @em SysInfo function.
  - More coil elements than receivers were detected.

  @PVM_EXAMPLE
  @code
  STB_UpdateEncoding(
    PTB_GetSpatDim(),
    PVM_Matrix,
    PVM_AntiAlias,
    NULL,
    SEG_SEQUENTIAL,
    Yes,
    Yes,
    Yes);
  @endcode

  @param[in,out] numberOfSegments    Pointer to number of interleaved segments (shots)
                                     - may be changed
  @param         piAllow             @c Yes = PI is allowed
  @param         piRefAllow          @c Yes = PI reference lines are allowed
  @param         pfAllow             @c Yes = PF is allowed
  @param         encodingScheme @c encodingScheme < 3, traditional encoding 
                                - encodingScheme >= 3 (3 or 4), 3D phase encoding
  @PVM_H2{Note}
  All parameters affect the 2nd dimension, the @em Phase dimension, which is the 1st
  phase-encoded dimension.

  @return
          @c 0 = @a numberOfSegments was accepted
          @c 1 = @a numberOfSegments was changed

  @see @ref STB_InitEncoding, @ref STB_UpdateEncoding, @ref STB_UpdateEncodingSpecial

  @ingroup PVM_STB
*/
DLL_INTERFACE   int STB_UpdateEncodingForEpi(int      *numberOfSegments,
                                             YesNo     piAllow,
                                             YesNo     piRefAllow,
                                             YesNo     pfAllow,
                                             int       encodingScheme = 0);





/**************************************************************************/
/*             gen/src/prg/pvm_toolboxes/mr/PvSeqTools/gradients.c        */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE GRAD_KSTYP STB_GradKsTypeRange (GRAD_KSTYP);

/** Set elements of @c SGT_TYPE parameter.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref STB_SgtTypeRange

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  const double gvec[3] = {0.0, 0.0, 1.0};
  int stat = STB_SgtTypeSetPar(&PVM_DwGradTraj[0],
                               PVM_DwGradPulseDelay[0],
                               CFG_GradientRiseTime(),
                               CFG_GradientRiseTime(),
                               PVM_DwGradDur[0],
                               UT_AbsMaxOfDoubleArray(PVM_DwNDiffExpEach, PVM_DwGradAmp),
                               PVM_DwDirectScale == No ? RPS_KS:PAT_KS,
                               gvec);
  @endcode

  @param[out] grad     Parameter whose elements will be set to given values
  @param      absTime
  @param      rampUp
  @param      rampDown
  @param      dur
  @param      plateau
  @param      ks
  @param[in]  vec

  @return Mask indicating which elements had to be modified during range checking

  @see @ref STB_SgtTypeRange, @ref STB_SgtTypeAddTime,
       @ref STB_SgtTypeCopyPar, @ref STB_PrintSGT, @ref STB_SgtTrans

  @todo Incomplete documentation: param

  @ingroup PVM_STB_INT
*/
DLL_INTERFACE int STB_SgtTypeSetPar(SGT_TYPE            *grad,
		      const double        absTime,
		      const double        rampUp,
		      const double        rampDown,
		      const double        dur,
		      const double        plateau,
		      const               GRAD_KSTYP ks,
		      const double *const vec);

/** Check elements of @c SGT_TYPE parameter.

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
  SGT_TYPE grad;

  // ... set values of grad ...

  int status = STB_SgtTypeRange(&grad);
  @endcode

  @param[in,out] grad Parameter whose elements will be set to given values

  @return Mask indicating which elements had to be modified

  @see @ref STB_SgtTypeSetPar, @ref STB_SgtTypeAddTime,
       @ref STB_SgtTypeCopyPar, @ref STB_PrintSGT, @ref STB_SgtTrans

  @ingroup PVM_STB_INT
*/
DLL_INTERFACE int STB_SgtTypeRange(SGT_TYPE *grad);

/** Add time values to @c SGT_TYPE parameter.

  Add values in a list of times to element @c st.

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
  #define N 3
  timeVal[M] = {1.0, 2.0, 3.0};
  SGT_TYPE grad;

  // ... set values of grad ...

  STB_SgtTypeAddTime(&grad, N, timeVal);
  @endcode

  @param[in,out] grad Parameter whose element @c st will be modified
  @param         size Number of time values
  @param         t    List of time values

  @see @ref STB_SgtTypeSetPar, @ref STB_SgtTypeRange,
       @ref STB_SgtTypeCopyPar, @ref STB_PrintSGT, @ref STB_SgtTrans

  @ingroup PVM_STB_INT
*/
DLL_INTERFACE void STB_SgtTypeAddTime(SGT_TYPE     *grad,
                                      int          size,
                                      const double t);



/** Copy an array of @c SGT_TYPE parameters.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Size of @a dest must be at least size of @a source.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  #define N 10
  SGT_TYPE grad1[N], grad2[N];

  // ... set values of grad1 ...

  STB_SgtTypeCopyPar(grad2, grad1, N);
  @endcode

  @param[out] dest   Destination parameter array
  @param[in]  source Source parameter array
  @param      size   Array size

  @see @ref STB_SgtTypeSetPar, @ref STB_SgtTypeRange, @ref STB_SgtTypeAddTime,
       @ref STB_PrintSGT, @ref STB_SgtTrans

  @ingroup PVM_STB_INT
*/
DLL_INTERFACE void STB_SgtTypeCopyPar(SGT_TYPE *dest,
                                      SGT_TYPE *source,
                                      int      size);



/**
  @todo Documentation missing

  @see @ref STB_SgtTypeSetPar, @ref STB_SgtTypeRange, @ref STB_SgtTypeAddTime,
       @ref STB_SgtTypeCopyPar, @ref STB_SgtTrans

  @ingroup PVM_STB_INT
*/
DLL_INTERFACE int STB_PrintSGT(SGT_TYPE          *gtr,
                               int               gtrsiz,
                               const char *const filename,
                               const double      dwell);



/**
  @todo Documentation missing.

  @see @ref STB_SgtTypeSetPar, @ref STB_SgtTypeRange, @ref STB_SgtTypeAddTime,
       @ref STB_SgtTypeCopyPar, @ref STB_PrintSGT

  @ingroup PVM_STB_INT
*/
DLL_INTERFACE void STB_SgtTrans(SGT_TYPE   *gtr,
                                int        gtrsiz,
                                GRAD_KSTYP kst,
                                double     **tmat,
                                double *ovec=0);

/**************************************************************************/
/*          gen/src/prg/pvm_toolboxes/mr/PvSeqTools/StbAdjTools.c         */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_AdjHandler (void);
DLL_INTERFACE void STB_UpdateAdjResultValues (void);
DLL_INTERFACE void STB_AutoRgInitVal (void);
DLL_INTERFACE void STB_AdjReportCreateRels();




/**************************************************************************/
/*            gen/src/prg/pvm_toolboxes/mr/PvSeqTools/TuneShim.c          */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_TuneShimIncIterRange (void);
DLL_INTERFACE void STB_TuneShimIncIter (void);
DLL_INTERFACE void STB_TuneShimAdjFreqRange (void);
DLL_INTERFACE void STB_TuneShimFreqOffsRange (void);
DLL_INTERFACE void STB_TuneShimAdjFreq (void);
DLL_INTERFACE void STB_TuneShimSubsetRange (void);
DLL_INTERFACE void STB_TuneShimSubset (void);
DLL_INTERFACE void STB_TuneShimForceSubset (void);
DLL_INTERFACE void STB_TuneShimForceSubsetRange (void);
DLL_INTERFACE void STB_TuneShimNShimRepRange (void);
DLL_INTERFACE void STB_TuneShimNShimRep (void);
DLL_INTERFACE void STB_TuneShimRepRange (void);
DLL_INTERFACE void STB_TuneShimRep (void);
DLL_INTERFACE void STB_TuneShimSetRange (SHIM_SUBSETS_TYPE);
DLL_INTERFACE void STB_TuneShimSet (void);
DLL_INTERFACE void STB_TuneShimRadiusRange (void);
DLL_INTERFACE void STB_TuneShimRadius (void);
DLL_INTERFACE void STB_TuneShimActStwRange(void);
DLL_INTERFACE void STB_TuneShimActStw(void);
DLL_INTERFACE void STB_TuneShimStepWidthRange(void);
DLL_INTERFACE void STB_TuneShimStepWidth(void);
DLL_INTERFACE void STB_TuneShimSkipShimming(void);
DLL_INTERFACE void STB_TuneShimDigFillLimitsRange(void);

/** Sets parameters defining the shim configuration.

  This functions retrieves shim specific ation about the currently active shim system and
  sets @c PARX parameters specified by parameter names.

  In detail the state of the shim system is retrieved and copied to a parameter named
  @a stateparn that must be a string parameter of length PVSYSINFOCHANSTATE_LEN.

  Information about available shim functions or raw shims is copied to a parameter named
  @a chanparn that must be an integer array of length N_SHIMS. Available shims are marked as
  nonzero entries in this parameter, the offset denotes the specific shim channel identifier.

  The max field variation of the shims is copied to a parameter named @a sensparn. This field
  variation corresponds to the max possible value of a shim function or max allowed current of
  a raw shim. It must be a 2-dimensional double array of size N_SHIMS x N_SHIMS. Each row is a
  spherical harmonic field expression of the related shim function (or raw shim) with
  coefficients expressed in Hz/cm^order. The order is characterized by the offset inside the
  vector.

  @PVM_PARS
  Function changes parameters with names specified by the string arguments.

  @PVM_TBCALL
  - @c SysInfo_RawShims, @c SysInfo_ShimFunctions
  - @c SysInfo_ShimMaxRawField
  - @c SysInfo_ShimMaxFuncField

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The string arguments must contain parameter names that have the correct type as described
    above.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  // during parameter definition
  int parameter ShimId [N_SHIMS];
  double parameter ShimSens[N_SHIMS][N_SHIMS];

  ...

  YesNo hwinfo = STB_InitShimConfigPars("PVM_ShimCoeffHwStatus",
                                        "ShimId",
                                        "ShimSens",
                                        Yes);
  @endcode

  @param[in] stateparn    Name of a string parameter of length PVSYSINFOCHANSTATE_LEN
  @param[in] chanparn     Name of a double array parameter of length N_SHIMS
  @param[in] sensparn     Name of a 2-dim double array parameter of lentgth N_SHIMS x N_SHIMS
  @param     rawshim      @c YesNo flag: If set to @c Yes, sensitivities and ID of shim coils instead
                          of shim functions are delivered.

  @return @c Yes in case the desired information is available and could be assigned to the
          parameters specified by arguments

  @see @ref STB_InitTuneShimConfigPars

  @ingroup PVM_STB
*/
DLL_INTERFACE YesNo STB_InitShimConfigPars(const char *stateparn,
                             const char *chanparn,
                             const char *sensparn,
                             YesNo      rawshim);

/** Initialises tune shim configuration parameters.

  @PVM_PARS
  - @c PVM_TuneShimInfoAvailable
  - @c PVM_TuneShimState
  - @c PVM_TuneShimFunctions
  - @c PVM_TuneShimSens

  @PVM_TBCALL
  - @ref STB_InitShimConfigPars

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_InitShimConfigPars, @ref STB_InitTuneShimPars

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitTuneShimConfigPars (void);

/** Initialises tune shim parameters.

  The tune shim parameters are initialised and checked to lie within their legal range. The
  consistancy of the parameters is not assured after a call of this function.

  @PVM_PARS
  - @c PVM_TuneShimSubset
  - @c PVM_TuneShimForceSubset
  - @c PVM_TuneShimNShimRep
  - @c PVM_TuneShimRep
  - @c PVM_TuneShimIncIter
  - @c PVM_TuneShimAdjFreq
  - @c PVM_TuneShimSetRange
  - @c PVM_TuneShimFreqOffs

  @PVM_TBCALL
  - Default range checking functions

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateTuneShimPars, @ref STB_InitTuneShimConfigPars

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitTuneShimPars(void);

/** Updates tune shim parameters.

  The tune shim parameters are initialised and checked to lie within their legal range. The
  consistancy of the parameters is assured after a call of this function. This functions serves
  also as default relation of the tune shim parameter group handler @c PVM_TuneShimHandler.

  @PVM_PARS
  - @c PVM_TuneShimSubset
  - @c PVM_TuneShimForceSubset
  - @c PVM_TuneShimNShimRep
  - @c PVM_TuneShimRep
  - @c PVM_TuneShimIncIter
  - @c PVM_TuneShimAdjFreq
  - @c PVM_TuneShimSetRange
  - @c PVM_TuneShimFreqOffs
  - @c PVM_TuneShimStepWidth

  @PVM_TBCALL
  - @ref STB_InitTuneShimPars

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_InitTuneShimPars, @ref STB_InitTuneShimConfigPars

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateTuneShimPars(void);




/**************************************************************************/
/*            gen/src/prg/pvm_toolboxes/mr/PvSeqTools/RefScan.c           */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_RefScanYNRange (void);
DLL_INTERFACE void STB_RefScanYN (void);
DLL_INTERFACE void STB_RefScanNARange (void);
DLL_INTERFACE void STB_RefScanNA (void);
DLL_INTERFACE void STB_RefScanPCRange (void);
DLL_INTERFACE void STB_RefScanPCYNRange (void);
DLL_INTERFACE void STB_RefScanPCYNRel (void);
DLL_INTERFACE void STB_RefScanStickFreqRange (void);
DLL_INTERFACE void STB_RefScanStickFreq (void);


/** Initialises the parameters of the @c ReferenceScan pargroup.

  A call of this routine assures legal starting values of the parameters listed below.

  Reference data can be acquired without water suppression via a @c ReferenceScan Adjustment.
  The data can be used for eddy current compensation.

  @PVM_PARS
  - @c PVM_RefScanYN
  - @c PVM_RefScanNA
  - @c PVM_RefScanRG
  - @c PVM_RefScanPC
  - @c PVM_RefScan

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @param job (optional) number of job with which reference scan is acquired (standard is no job (job=-1))

  @see  @ref STB_UpdateMCRefScan, @ref STB_UpdateRefScan

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitRefScan(int job = -1);

/** Updates the parameters of the ReferenceScan pargroup.

  A call of this routine resolves the relations between the parameters listed below. They
  control the acquisition of a reference scan adjustment. Argument @a nrec will be constrained
  to a legal range.

  @PVM_PARS
  - @c PVM_RefScanNA    Number of averages of the reference scan adjustment
  - @c PVM_RefScanRG    Receiver gain adjusted during the reference scan
  - @c PVM_RefScanPC    Complex phase correction values (real and imaginary part)
                        for each active receive channel

  @PVM_TBCALL
  - @ref CFG_NReceivers

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameters should have been initialised using  @ref STB_InitRefScan.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_UpdateMCRefScan(PVM_EncNReceivers);
  @endcode

  @param nrec    Number of active receivers

  @see  @ref STB_InitRefScan, @ref STB_UpdateRefScan

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateMCRefScan(int nrec);

/** Outdated - Updates the parameter of the @c ReferenceScan pargroup.

  Routine calls @ref STB_UpdateMCRefScan with @a nrec = @c 1.

  @PVM_PARS
  - @c PVM_RefScanNA    Number of averages of the reference scan adjustment
  - @c PVM_RefScanRG    Receiver gain adjusted during the reference scan
  - @c PVM_RefScanPC    Complex phase correction values (real and imaginary part)
                        for each active receive channel

  @PVM_TBCALL
  - @ref CFG_NReceivers

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameters should have been initialised using  @ref STB_InitRefScan.

  @PVM_ERROR
  NONE

  @see  @ref STB_UpdateMCRefScan, @ref STB_InitRefScan

  @ingroup PVM_STB
*/
void STB_UpdateRefScan(void);


/** Handles an adjustment request for the <em>Reference Scan</em> adjustment.

  In @c callbackDefs.h redirect @c PVM_AdjHandler to this function.

  If the method uses more adjustments or the method is to be modified before the
  <em>Reference Scan</em> adjustment, redirect @c PVM_AdjHandler to the respective
  method function that calls this handler. From its return value it can be
  determined if the request could be handled.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref PTB_AdjMethSpec
  - @ref PTB_GetAdjName

  @PVM_RELCALL
  - @c PVM_AdjHandler

  @PVM_PRECOND

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  void LocalAdjustmentResultHandler()
  {
    if (STB_RefScanHandleAdjustmentRequests() == 1)
    {
      // ... switch off water suppression here ...

      backbone();
    }
  }
  @endcode

  @return
  - @c 0 = request is still to be handled
  - @c 1 = reference scan request was handled


  @see @ref

  @ingroup PVM_STB
*/
DLL_INTERFACE int STB_RefScanHandleAdjustmentRequests(void);




/**************************************************************************/
/*              gen/src/prg/pvm_toolboxes/mr/PvSeqTools/shimCoeff.c       */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_ShimCoeffNcalRange (void);
DLL_INTERFACE void STB_ShimCoeffNcal (void);
DLL_INTERFACE void STB_ShimCoeffNcoeffRange (void);
DLL_INTERFACE void STB_ShimCoeffNcoeff (void);

DLL_INTERFACE void STB_ShimCoeffChan (void);

DLL_INTERFACE void STB_ShimCoeffId (void);
DLL_INTERFACE void STB_ShimCoeffCoeff (void);
DLL_INTERFACE void STB_ShimCoeffCoeffSD (void);
DLL_INTERFACE void STB_ShimCoeffHwStatus (void);

DLL_INTERFACE void STB_ShimCoeffHwMode (void);

DLL_INTERFACE void STB_ShimCoeffCalVersion (void);
DLL_INTERFACE void STB_ShimCoeffHandler (void);


/** Updates shim coefficients parameters.

  This function updates parameters of the shim coefficients parameter
  class (see below).  These parameters are the calibration result of a
  map based expression of shim fields in spherical harmonic
  coefficients. Dependent on argument @a restore parameters are
  restored (restore == yes) with respect to the current valid state of
  the shim hardware, on success the routine returns Yes in this
  case. If restore == No, array parameter are dimensioned according to
  arguments @a ncal and @a ncoeff.

  The routine sets PVM_ShimCoeffNcal to ncal, which must be > 1 and less
  than the number of supported shim channels
  ( as returned from CFG_NShims()) and PVM_ShimCoeffNcoeff to ncoeff
  which must be >= 9 and <= the maximum number of spherical harmonic
  coefficients supported by the software ( return value of
  PvnlSphericalMaxNFunc()).

  If the restored parameter have different values of @a ncoeff and @a ncal,
  array parameters are dimensioned according to these arguments and
  are inititialized with 0 values.

  @PVM_PARS
  - @c PVM_ShimCoeffNcal       : set to ncal
  - @c PVM_ShimCoeffNcoeff     : set to ncoeff
  - @c PVM_ShimCoeffChan       : dimensioned to ncal, elements set to Shim_Z
  - @c PVM_ShimCoeffHwStatus   : set to current hardware status
  - @c PVM_ShimCoeffHwMode     : set to PVSHIM_function_values
  - @c PVM_ShimCoeffCalVersion : set to empty string
  - @c PVM_ShimCoeffCoeff      : dimensioned to ncal x ncoeff set to 0.0
  - @c PVM_ShimCoeffCoeffSD    : dimensioned to ncal x ncoeff set to 0.0

  @PVM_TBCALL
  - @ref CFG_NShims
  - @ref PvnlSphericalMaxNFunc
  - CFG_ShimStatusString
  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - legal arguments of ncal and ncoeff

  @PVM_ERROR
  - illegal values of ncal and ncoeff

  @PVM_EXAMPLE
  @code
  if(No==STB_UpdateShimCoefficients(8,25,Yes))
  {
     // initialize shim coefficients with default values
  }
  @endcode

  @param[in] ncal  number of calibrated shim coils
  @param[in] ncoeff number of spherical harmonic coefficients
  @param[in] restore if Yes parameters are restored from calibration
             results  @return Yes on successful restore else No

  @ingroup PVM_STB_INT
*/
DLL_INTERFACE YesNo STB_UpdateShimCoefficients(int ncal,
                                 int ncoeff,
                                 YesNo restore);




/**************************************************************************/
/*               gen/src/prg/pvm_toolboxes/mr/PvSeqTools/csvol.c          */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_StudyShimRel (void);



/**************************************************************************/
/*             gen/src/prg/pvm_toolboxes/mr/PvSeqTools/GeoObjects.c       */
/**************************************************************************/

/** Handle constraints of the slice geometry depending on isotropy settings.

  @PVM_PARS
  - @c PVM_SliceGeoObj

  @PVM_TBCALL
  - @ref PTB_GetSpatDim
  - @ref GTB_CheckPlanningMode

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @ingroup PVM_STB_INT
*/
DLL_INTERFACE void STB_HandleIsotropicGeoEdConstraints(void);

/* range checkers and relations */
DLL_INTERFACE void STB_ImageGeometryHandler(void);
DLL_INTERFACE void STB_SliceGeometryHandler(void);
DLL_INTERFACE void STB_SliceGeoObjRel (void);
DLL_INTERFACE void STB_SliceGeoRel (void);
DLL_INTERFACE void STB_SliceGeoRange (void);

DLL_INTERFACE void STB_VoxelGeoObjRel(void);
DLL_INTERFACE void STB_VoxelGeoCubRel(void);




/**************************************************************************/
/*              gen/src/prg/pvm_toolboxes/mr/PvSeqTools/nucleiFrq.c       */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_FrqRef(void);
DLL_INTERFACE void STB_FrqWorkOffset(void);
DLL_INTERFACE void  STB_FrqWork(void);
DLL_INTERFACE void  STB_FrqRefPpm(void);
DLL_INTERFACE void  STB_FrqWorkOffsetPpm(void);
DLL_INTERFACE void  STB_FrqWorkPpm(void);

/** Initialise frequency parameters.

  @PVM_PARS
  - @c PVM_FrqAdj1HPpm

  @PVM_TBCALL
  - @ref MRT_NucleiPpmRefInit
  - @ref PTB_NucleiPpmWorkInit

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateFrqPars

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitFrqPars(void);

/** Delivers frequency offset for a given chemical shift.

  This function may be called to calculate the frequency offset (unit Hz) to the actual
  basic frequency of the logical channel (argument @a chan ) for a given ppm offset
  (argument @a ppm).

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  This function must be called after the update of the nuclei parameter group.

  @PVM_ERROR
  - illegal system configuration (failure of SysInfo_TxChannels )
  - illegal value of argument @a chan (must be less or equal to the number of available
    transmit channels (dependent on the currently routed RF coil and operation mode)
  - illegal state of the nuclei parameter group

  @PVM_EXAMPLE

  This example shows how to get the offset frequency of NAA at 2.01 ppm under the condition
  that the logical channel 1 is working at the proton frequency.

  @code

  double offsetNAA=STB_GetFreqOffset(1,2.01);


  @endcode

  @param chan logical frequency channel
  @param ppm  desired chemical shift offset
  @return frequency offset expressed in Hz


  @see STB_UpdateNuclei

  @ingroup PVM_STB
*/

DLL_INTERFACE double STB_GetFreqOffset(int chan, double ppm);

/** Update frequency parameters.

  @PVM_PARS
  - @c PVM_FrqRef
  - @c PVM_FrqWork
  - @c PVM_FrqRefPpm
  - @c PVM_FrqWorkPpm
  - @c PVM_FrqWorkOffset
  - @c PVM_FrqWorkOffsetPpm

  @PVM_TBCALL
  - @ref UT_GetRequest
  - @ref PTB_NucleiPpmRefGet
  - @ref PTB_NucleiPpmWorkSet
  - @ref CFG_GammaRatio
  - @ref UT_ClearRequest

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_InitFrqPars

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateFrqPars(void);




/**************************************************************************/
/*             gen/src/prg/pvm_toolboxes/mr/PvSeqTools/ParamImport.c      */
/**************************************************************************/

/* relations */
DLL_INTERFACE void STB_ImportID(void);




/**************************************************************************/
/*          gen/src/prg/pvm_toolboxes/mr/PvSeqTools/DriftComp.c           */
/**************************************************************************/

/* relations */
DLL_INTERFACE void STB_DriftCompCommonRels(void);
DLL_INTERFACE void STB_DriftCompYesNo(void);
DLL_INTERFACE void STB_DriftCompUpdateTime(void);
DLL_INTERFACE void STB_DriftCompUpdateTimeRange(double repeatTime);
DLL_INTERFACE void STB_DriftCompNavSWhRel(void);
DLL_INTERFACE void STB_DriftCompNavPulseRel(void);
DLL_INTERFACE void STB_DriftCompNavPulseRange(void);
DLL_INTERFACE void STB_DriftCompNavPulseEnumRel(void);
DLL_INTERFACE void STB_DriftCompNavSpoilRel(void);

/** Initialises the parameter group @em DriftComp_Parameters.

  This module provides a drift compensation via B0 correction. It analyzes
  navigator signals during online reconstruction with respect to frequency
  changes as they can occur for main magnet field drifts and corrects them
  via the @c reload @c B0 mechanism in the pulse program. The frequency
  changes are determined relative to the first navigator scan(s).

  The module can be setup to use navigator data which are either created by
  the module itself (NavOnOff: On) or by a separate JOB acquisition
  (navOnOff: Off). The intrinisc navigator uses a global excitation pulse.
  In this case the module sets up and controls the acquisition parameters
  automatically. Alternatively, separate navigator data can be fed into the
  module, e.g. created by the navigator module. This may be useful for more
  advanced requirements regarding the navigator (e.g. slice-selective navigator,
  positioning in Geometry Editor etc).

  @c PVM_DriftCompUpdateTime is a user-input parameter that can be changed in
  multiples of the repeat time of the @c reload @c B0 as given by the subroutine's
  position within the pulse program. As this repeat time may change with method
  parameters it needs to be updated in the backbone (call of STB_UpdateDriftComp).
  The module limits the lower update time to 75 ms. Navigator signals received
  within this minimum time are excluded from analysis to ensure transaction integrity.
  For systems using a physical B0 coil (instead of a FRED unit), the minimum is
  increased by an additional repeat time to assure settling of the B0 coil.
  For update times than minimum, the frequency is calculated from multiple navigator
  scans using linear regression. It may help to increase accuracy and/or stability.

  Logging can be activated by setting @c ACQ_DriftCompDebug to Yes in the Single
  Parameter editor (prototype mode required). This generates an output file @c b0offsets
  in the expno directory. The file contains the frequency offsets used for @c reload @c B0.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Parameter group</td> <td>@c DriftComp_Parameters        </td> <td>@c parsLayout.h          </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitDriftComp         </td> <td>@c initMeth()            </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateDriftCompModule </td> <td>@c backbone()            </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateDriftComp       </td> <td>@c backbone()            </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_DriftCompHandler        </td> <td>@c callbackDefs.h        </td></tr>
  <tr><td>BaseLevel      </td> <td>@ref ATB_SetDriftCompBaseLevel </td> <td>@c BaseLevelRelations.c  </td></tr>
  <tr><td>Duration       </td> <td>@c PVM_DriftCompModuleTime     </td> <td>@c backbone()            </td></tr>
  <tr><td>Include        </td> <td>@c DriftComp.mod or @n
                                   @c PrepModulesHead.mod         </td> <td><c>\<method\>.ppg</c>    </td></tr>
  <tr><td>Subroutine     </td> <td>@c DriftComp()                 </td> <td><c>\<method\>.ppg</c>    </td></tr>
  <tr><td>Parameter card </td> <td>@c PVM_DriftCompYesNo          </td> <td><c>\<method\>.xml</c>    </td></tr>
  <tr><td>Parameter card </td> <td>@c DriftComp.xml               </td> <td><c>\<method\>.xml</c>    </td></tr>
  </table>

  - This module consists of two updaters. Call @ref STB_UpdateDriftCompModule before
    updating the sequence timing. Consider the resulting module's duration in the calculation
    of min repetition time and scan duration dependig on where the module is called in the
    pulse program.
  - Call @ref STB_UpdateDriftComp after calculation of sequence timing to hand over the correct
    subroutine's repeat time.
  - Insert subroutine DriftComp() into the pulse program. If an external navigator signal is
    used it is recommended to place its acquisition closely after the subroutine DriftComp.
    This inreases the available duration for analysis.


  Internally, RecoDetermineB0 and RecoReloadB0 reconstruction filters are appended for
  calculation and setting of B0 values.

  @PVM_PARS
  - @c Members of the Drift Compensation module

  @PVM_TBCALL
  - @ref STB_InitRFPulse

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The pulse program needs to allow job acquisition
  - If a separate navigator signal generation is chosen then its acquisition must be realized
    independently

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_InitDriftComp(On);
  @endcode

  @param navOnOff  Internal or external navigator signal.

  @see @ref STB_UpdateDriftCompModule, @ref STB_UpdateDriftComp, @ref ATB_SetDriftCompBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitDriftComp(OnOff navOnOff=Off);



/** Updates the parameter group @em DriftComp_Parameters.

  This function should be called in methods using the @em DriftComp_Parameters group.
  The arguments handed over to the module are required to set up the intrinsic navigator
  and can be omitted if an external navigator is used.

  @PVM_PARS
  - @c   PVM_DriftCompModuleTime
  - @c   PVM_DriftCompNavPulse
  - @c   PVM_DriftCompNavSpoil
  - @c   PVM_DriftCompNavD

Members of the Drift Compensation module

  @PVM_TBCALL
  - @ref STB_UpdateRFPulse
  - @ref MRT_UpdateSpoiler
  - @ref CFG_GradientRiseTime()

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @ref STB_InitDriftComp must have been called.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_UpdateDriftCompModule(PVM_SliceThick, PVM_DeriveGains);
  @endcode

  @see @ref STB_InitDriftComp, @ref STB_UpdateDriftComp, @ref ATB_SetDriftCompBaseLevel

  @param  spoilerThick     Scale basis for the efficiency of an (optional) spoiler gradient.
  @param  deriveGains      (optional) @c Yes = gains will be derived from reference power (default)

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateDriftCompModule(double spoilerThick = 1.0, YesNo deriveGains=Yes);



/** Updates the parameter group @em DriftComp_Parameters.

  This function should be called in methods using the @em DriftComp_Parameters group.

  @PVM_PARS
  - @c PVM_DriftCompUpdateTime
  - @c PVM_DriftCompNSkip
  - @c PVM_DriftCompNAverages

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @ref STB_InitDriftComp must have been called.
  - @ref STB_UpdateDriftCompModule must have been called.
  - Repetition time must have been updated.

  @PVM_ERROR
  - Incorrect input parameter.

  @PVM_EXAMPLE
  @code
  STB_UpdateDriftComp(PVM_RepetitionTime);
  @endcode

  @param repeatTime  Repeating time of DriftComp subroutine in pulse program.

  @see @ref STB_InitDriftComp, @ref STB_UpdateDriftCompModule, @ref ATB_SetDriftCompBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateDriftComp(double repeatTime);




/**************************************************************************/
/*            gen/src/prg/pvm_toolboxes/mr/PvSeqTools/DummyScans.c        */
/**************************************************************************/

/* relations */
DLL_INTERFACE void STB_DummyScans(void);
DLL_INTERFACE void STB_DummyScansDur(void);

/** Initialise the parameter group @em DummyScans_Parameters.

  This function should be called in methods using the @em DummyScans_Parameters
  group, which handles the duration and number of initial dummy scans employed
  to reach a steady-state of the magnetisation.

  @PVM_H2{Using @em DummyScans_Parameters}
  - Include the parameter group @em DummyScans_Parameters directly or indirectly
    in @em MethodClass.
  - Call @ref STB_InitDummyScans during initialisation.
  - Call @ref STB_UpdateDummyScans from backbone after calculation of repetition
    time.
  - Redirect @c PVM_DummyScansHandler to backbone.
  - Insert the parameters @c PVM_DummyScans and @c PVM_DummyScansDur in the "Contrast"
    @c xml card.
  - Use @c PVM_DummyScans for setting up the number of dummy scans in the pulse program.

  @PVM_PARS
  - @c PVM_DummyScans
  - @c PVM_DummyScansDur

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Incorrect input parameter.

  @PVM_EXAMPLE
  @code
  STB_InitDummyScans(1000.0);
  @endcode

  @param dummyScansDur  duration of dummy scans used for initialisation (in
                        case no value exists) [ms]

  @see STB_UpdateDummyScans

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitDummyScans(double dummyScansDur);

/** Updates the parameter group @em DummyScans_Parameters.

  This function should be called in methods using the @em DummyScans_Parameters group.
  It will update the parameters handling the duration and number of initial dummy scans.

  @PVM_PARS
  - @c PVM_DummyScans
  - @c PVM_DummyScansDur

  @PVM_TBCALL
  - @ref UT_GetRequest
  - @ref UT_RequestParOrigValue

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @ref STB_InitDummyScans must have been called.
  - The repetition time must have been updated.

  @PVM_ERROR
  - Incorrect input parameter.

  @PVM_EXAMPLE
  @code
  STB_UpdateDummyScans(PVM_RepetitionTime);
  @endcode

  @param repetitionTime  RF pulse repetition time of the sequence
  @param averages        (optional) Number of averages inside dummy loop

  @see STB_InitDummyScans

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateDummyScans(double repetitionTime, int averages = 1);




/**************************************************************************/
/*          gen/src/prg/pvm_toolboxes/mr/PvSeqTools/RfPulseHandler.c      */
/**************************************************************************/

/** Initialise RF pulse parameters.

  This function should be used in the initMeth() routine of a method to initialise
  RF-pulse parameters. The first 4 (mandatory) arguments specify the parameter
  names of the shape parameters that handle RF-pulses within methods. The first
  argument specifies the name of a shape struct parameter of type @c PVM_RF_PULSE.
  The second argument specifies the name of an enum parameter of type @c PV_PULSE_LIST
  (an empty string or a @c NULL pointer is allowed), the third argument defines the name
  of a shape amplitude parameter of type @c PVM_RF_PULSE_AMP_TYPE (an empty string or a
  @c NULL pointer is allowed), the 4th argument specifies the name of a shape waveform
  parameter. This parameter will be used to store a calculated waveform (1 dimensional
  double array, an empty string or a @c NULL pointer is allowed, in this case calculated
  shapes are not supported).
  Each group of struct, shape list, amplitude, and waveform parameter must be unique
  i.e. it is not allowed to share waveform, amplitude or enum parameters between different
  RF pulse struct parameters.
  The 5th argument (@a type) specifies the shape type (excitation = @c 0, refocusing = @c 1,
  inversion = @c 2). This mandatory parameter controlls the shape algorithm that calculates
  the RF pulse waveform (calculated RF pulses), or restricts the waveform files to the subset
  of the desired type.

  Remaining arguments define a default shapename,
  a default pulse bandwidth and a default pulse angle (unit degree) and the default sharpness
  (meaningful for calculated shapes only). All parameters will be
  checked to have a value and the parameter values are checked to lie within a legal range.
  It is not possible to update an RF-pulse that has not been initialised with this function.

  @PVM_PARS
  - Parameter specified by arguments

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  The combination of shape struct, shape list, amplitude, and waveform parameter names
  must be unique. Thus this function must not have been called with one of the parameters.

  @PVM_ERROR

  Check of arguments:

  - Argument 1 (@a shapeStructParn):
    - is not a parameter name
    - parameter is not of type @c PVM_RF_PULSE
    - parameter is an array
  - Argument 2 (@a shapeEnumParn) optional
    - is not a parameter name
    - is not of type @c PV_PULSE_LIST
    - parameter is an array
  - Argument 3 (@a shapeAmplParn) optional
    - is not a parameter name
    - is not of type @c PVM_RF_PULSE_AMP_TYPE
    - parameter is an array
  - Argument 4 (@a shapeWaveParn) optional
    - is not a parameter name
    - is not a one dimensional double array
  - Argument 5 (@a type): must be @c 0 (excitation), @c 1 (refocusing) or @c 2 (inversion)

  If one of the parameters specified by arguments has been initialised with
  other parameters the combination of shape struct, enum, amplitude, and waveform
  parameters is checked to be unique.

  @PVM_EXAMPLE
  @code
  // initialises an refocusing rf pulse with 1.5 kHz bandwidth and 180 deg pulse angle
   STB_InitRFPulse("RFPulse1",
                  "RFPulse1List",
                  "RFPulse1Ampl",
                  "RFPulse1Waveform",
                  1,
                  "hermite",
                  1500.0,
                  1800);
  // initializes an excitation rf pulse with 2.0 kHz bandwidth, 30 deg flipangle and
  // sharpness 2
   STB_InitRFPulse("RFPulse2",
                  "RFPulse2List",
                  "RFPulse2Ampl",
                  "RFPulse2Waveform",
                  0,
                  "Calculated",
                  2000.0,
                  30,
                  2);


  // initialises a calculated excitation pulse with 2kHz bandwidth and 90 deg pulse angle
  STB_InitRFPulse("RFPulse3",
                  "RFPulse3List",
                  "RFPulse3Ampl",
                  "RFPulse3Waveform",
                  1);


  // initialises an inversion pulse, neither calculated pulses are possible
  // (no waveform parameter specified) nor a pulse list and amplitude is handled

  STB_InitRFPulse("RFPulse4",0,0,0,2);


  // this call will create a runtime error in this example, since RFPulse2List,
  // RFPulse2Ampl and RFPulse2Waveform have been already initialised with RFPulse2

  STB_InitRFPulse("RFPulse4",
                  "RFPulse2List",
                  "RFPulse2Ampl",
                  "RFPulse2Waveform",
                  1);



  @endcode

  @param[in] shapeStructParn    Name of parameter of type @c PVM_RF_PULSE containing the RF-pulse
                                structure that should be initialised (mandatory argument)
  @param[in] shapeEnumParn      Name of parameter of type @c PV_PULSE_LIST @c 0 or empty string
                                allowed)
  @param[in] shapeAmplParn      Name of parameter of type @c PVM_RF_PULSE_AMP_TYPE (@c 0 or empty
                                string allowed)
  @param[in] shapeWaveParn      Name of a 1D double array parameter that is used to store a
                                calculated shape waveform  (@c 0 or empty string allowed, in this
                                case calculated shapes are not supported for the related pulse
                                struct).
  @param     type               (Integer) Specifies the shape type
                                (excitation = @c 0, refocusing = @c 1, inversion = @c 2)
  @param[in] shapefile          Specifies a default shape waveform file (e.g. hermite without
                                extension), if not specified, @c Calculated is used. The keyword
                                @c Calculated is used for waveforms that are calculated within the
                                method code by the updating routine @ref STB_UpdateRFPulse.
  @param     bandwidth          Specifies the default bandwidth [Hz] (if not specified 2000 Hz).
  @param     fa                 Specifies the default pulse angle [degree] (if not specified 90 deg).
  @param     sharpness          Specifies the default sharpness of the spectral/spatial pulse
                                profile (if not specified 3).

  @see @ref STB_UpdateRFPulse, @ref STB_CheckRFPulse

  @ingroup PVM_STB
*/

DLL_INTERFACE int STB_InitRFPulse(const char *const shapeStructParn,
                    const char *const shapeEnumParn,
                    const char *const shapeAmplParn,
                    const char *const shapeWaveParn,
                    const int type,
                    const char *const shapefile="Calculated",
                    double bandwidth=2000.0,
                    double fa=90.0,
                    int sharpness=3);

/** Checks valid ranges for RF pulse parameters.

  @PVM_PARS
  - Parameter given by @a shapeStructParn

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
  STB_CheckRFPulse("ExcPulse");
  @endcode

  @param[in] shapeStructParn    Name of parameter of type @c PVM_RF_PULSE containing the
                                RF-pulse structure that should be checked.

  @see @ref STB_InitRFPulse, @ref STB_UpdateRFPulse

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_CheckRFPulse(const char *const shapeStructParn);

/** Update RF pulse parameters with shape algorithm.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Updating failed.

  @PVM_EXAMPLE
  @code
  STB_UpdateRFPulse("ExcPulse1", 1, PVM_DeriveGains, Conventional);
  @endcode

  @param[in] shapeStructParn    Name of parameter of type @c PVM_RF_PULSE containing
                                the RF-pulse structure that should be updates
  @param     chan               Channel number
  @param     deriveAmpl         @c Yes = derive power from reference power
  @param     shAlg              Shape algorithm
  @param[in] addpl

  @return
  - @c 0 = ok
  - @c 1 = error

  @see @ref STB_InitRFPulse, @ref STB_CheckRFPulse

  @todo Docu: parameter @a addpl

  @ingroup PVM_STB
*/
DLL_INTERFACE int STB_UpdateRFPulse(const char *const shapeStructParn,
                      const int chan,
                      YesNo deriveAmpl,
                      SHAPE_ALG_TYPE shAlg=Conventional,
                      const char *const addpl=0);


/** Update RF pulse parameters with shape function.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Updating failed.

  @PVM_EXAMPLE
  @code
  STB_UpdateRFPulse("ExcPulse1", 1, PVM_DeriveGains, MRT_ConventionalRFShape);
  @endcode

  @param[in] shapeStructParn    Name of parameter of type @c PVM_RF_PULSE containing
                                the RF-pulse structure that should be updated
  @param     chan               Channel number
  @param     deriveAmpl         @c Yes = derive power from reference power
  @param     wavefunc           Function that calculates pulse shape
  @param[in] addpl              (optional) Colon separated list of parameter
                                names that are used to control or calculate
                                algorithm features


  @return
  - @c 0 = ok
  - @c 1 = error

  @see @ref STB_InitRFPulse, @ref STB_CheckRFPulse



  @ingroup PVM_STB
*/
DLL_INTERFACE int STB_UpdateRFPulse(const char *const shapeStructParn,
                                    int               chan,
                                    YesNo             deriveAmpl,
                                    SHAPE_ALG_PROC    wavefunc,
                                    const char *const addpl=0);

/** Update amplitudes of RF pulses.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @c NULL pointer for argument @a amplist
  - Failed to handle amplitude(s).

  @PVM_EXAMPLE
  @code
  STB_UpdateRFShapeAmplitude("ExcPulse1Ampl, RefPulse1Ampl", Yes);
  @endcode

  @param[in] amplist  List of parameters of type @c PVM_RF_PULSE_AMP_TYPE
  @param     combined @c Yes = Amplitude changes are applied for all pulses

  @return
  - @c 0 = ok
  - @c 1 = error

  @ingroup PVM_STB
*/
DLL_INTERFACE int STB_UpdateRFShapeAmplitude(const char *const amplist,YesNo combined);

/** Get maximum peak power of a RF pulse.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a shapeStructParn is a @c NULL pointer
  - @a shapeStructParn is no parameter name
  - @a shapeStructParn has wrong type
  - @a shapeStructParn has not been initialized
  - @a shapeStructParn qualified parameters are not handled
  - @a shapeStructParn
  - @a shapeStructParn

  @PVM_EXAMPLE
  @code
  double maxPower = STB_MaxRfPower("ExcPulse1");
  @endcode

  @param[in] shapeStructParn    Name of parameter of type @c PVM_RF_PULSE containing
                                the RF-pulse structure

  @return Maximum peak power [W]

  @ingroup PVM_STB
*/
DLL_INTERFACE double STB_MaxRfPower(const char *const shapeStructParn);



/**************************************************************************/
/*       gen/src/prg/pvm_toolboxes/mr/PvSeqTools/tx_coil_nuclei.c         */
/**************************************************************************/

DLL_INTERFACE void STB_TxCoilScMode1(void);
DLL_INTERFACE void STB_TxCoilScMode2(void);
DLL_INTERFACE void STB_TxCoilScMode3(void);
DLL_INTERFACE void STB_TxCoilScMode4(void);
DLL_INTERFACE void STB_TxCoilScMode5(void);
DLL_INTERFACE void STB_TxCoilScMode6(void);
DLL_INTERFACE void STB_TxCoilScMode7(void);
DLL_INTERFACE void STB_TxCoilScMode8(void);

DLL_INTERFACE void STB_TxCoilStat1(void);
DLL_INTERFACE void STB_TxCoilStat2(void);
DLL_INTERFACE void STB_TxCoilStat3(void);
DLL_INTERFACE void STB_TxCoilStat4(void);
DLL_INTERFACE void STB_TxCoilStat5(void);
DLL_INTERFACE void STB_TxCoilStat6(void);
DLL_INTERFACE void STB_TxCoilStat7(void);
DLL_INTERFACE void STB_TxCoilStat8(void);

DLL_INTERFACE void STB_TxCoilAmpScaling1(void);
DLL_INTERFACE void STB_TxCoilAmpScaling2(void);
DLL_INTERFACE void STB_TxCoilAmpScaling3(void);
DLL_INTERFACE void STB_TxCoilAmpScaling4(void);
DLL_INTERFACE void STB_TxCoilAmpScaling5(void);
DLL_INTERFACE void STB_TxCoilAmpScaling6(void);
DLL_INTERFACE void STB_TxCoilAmpScaling7(void);
DLL_INTERFACE void STB_TxCoilAmpScaling8(void);


DLL_INTERFACE void STB_TxCoilPhaseOffset1(void);
DLL_INTERFACE void STB_TxCoilPhaseOffset2(void);
DLL_INTERFACE void STB_TxCoilPhaseOffset3(void);
DLL_INTERFACE void STB_TxCoilPhaseOffset4(void);
DLL_INTERFACE void STB_TxCoilPhaseOffset5(void);
DLL_INTERFACE void STB_TxCoilPhaseOffset6(void);
DLL_INTERFACE void STB_TxCoilPhaseOffset7(void);
DLL_INTERFACE void STB_TxCoilPhaseOffset8(void);

/* gen/src/prg/pvm_toolboxes/mr/PvSeqTools/CASL.c */

/* range checkers and relations */
DLL_INTERFACE void STB_CaslCommonRels(void);
DLL_INTERFACE void STB_CaslRFPulRange(void);
DLL_INTERFACE void STB_CaslRFPulRel(void);
DLL_INTERFACE void STB_CaslRFPulAmplRel(void);
DLL_INTERFACE void STB_CaslLabelSliceRange(void);
DLL_INTERFACE void STB_CaslLabelSliceRel(void);
DLL_INTERFACE void STB_CaslLabelTimeRange(void);
DLL_INTERFACE void STB_CaslLabelTimeRel(void);
DLL_INTERFACE void STB_CaslPostLabelTimeRange(void);
DLL_INTERFACE void STB_CaslPostLabelTimeRel(void);
DLL_INTERFACE void STB_CaslLabelGradRange(void);
DLL_INTERFACE void STB_CaslLabelGradRel(void);
DLL_INTERFACE void STB_CaslGeoObjRel(void);
DLL_INTERFACE void STB_CaslGeoCubRel(void);
DLL_INTERFACE void STB_CaslRecoModeRel(void);
DLL_INTERFACE void STB_CaslRepAvRel(void);
DLL_INTERFACE void STB_CaslOutlierDetRel(void);
DLL_INTERFACE void STB_CaslRestoreIERel(void);

/** Initialises the parameters of the <em>CASL</em> preparation module.

  Application of continuous arterial spin labeling.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Parameter group</td> <td>@c CaslParameters              </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitCaslModule        </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateCaslModule      </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_CaslHandler             </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>BaseLevel      </td> <td>@ref ATB_SetCaslModuleBaseLevel</td> <td>@c BaseLevelRelations.c</td></tr>
  <tr><td>Duration       </td> <td>@c PVM_CaslModuleTime          </td> <td>@c backbone()          </td></tr>
  <tr><td>Include        </td> <td>@c Casl.mod                    </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Subroutine     </td> <td>@c CASL,casllopp,caslreset     </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Parameter card </td> <td>@c Casl.xml                    </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>

  Set @c NR including the number of CASL images given by @c PVM_CaslImages by using @ref ATB_SetNR, e.g.
  @code
  ATB_SetNR(PVM_NRepetitions * PVM_CaslImages);
  @endcode


  @PVM_PARS
  - @c PVM_CaslExpType
  - @c PVM_CaslAcqOrder
  - @c PVM_CaslLabelImages
  - @c PVM_CaslControlImages
  - @c PVM_CaslLabelCoil
  - @c PVM_CaslRFPowerYesNo
  - @c PVM_CaslLabelPos
  - @c PVM_CaslLabelSliceOffset
  - @c PVM_CaslLabelTime
  - @c PVM_CaslPostLabelTime
  - @c PVM_CaslLabelGradient
  - @c PVM_CaslGeoCub
  - @c PVM_CaslRFPul
  - @c PVM_CaslVec


  @PVM_TBCALL
  - @ref STB_InitRFPulse


  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  double refFreq = PVM_FrqRef[0];
  STB_InitCaslModule(refFreq);
  @endcode

  @param     refFrq    Reference frequency used for initialization of T1

  @see @ref STB_UpdateCaslModule, @ref ATB_SetCaslModuleBaseLevel

  @ingroup PVM_STB

 */
DLL_INTERFACE void STB_InitCaslModule(double refFreq);

/** Updates the parameters of the <em>CASL</em> preparation module.

  @PVM_PARS
  - All editable members of the @c CaslParameters group


  @PVM_TBCALL
  - @ref PTB_InsertGeoObject
  - @ref SysInfo_TxNChannels
  - @ref STB_InitNuclei
  - @ref STB_UpdateNuclei
  - @ref STB_UpdateRFPulse
  - @ref GTB_HandleGeoObject
  - @ref UT_SetRequest
  - @ref UT_ClearRequest


  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The nuclei parameter group must have been updated.

  @PVM_ERROR
  Wrong type of argument @c masterCub

  @PVM_EXAMPLE
  @code
  STB_UpdateCaslModule("PVM_SliceGeo");
  @endcode

  @param     masterCub name of geometry interface parameter that controls the 
             imaging slices (must be of type @ref GEO_CUBOID_PACK)
  @param     allowAdj (optional) YesNo option to allow derivation of control 
             frequency from MT adjustment
             (adjustment must be implemented separately, default=Yes)

  @see @ref STB_InitCaslModule, @ref ATB_SetCaslModuleBaseLevel

  @ingroup PVM_STB

 */
DLL_INTERFACE void STB_UpdateCaslModule(const char *const masterCub,   
                                        YesNo allowAdj = Yes);
/** Updates the parameters of the <em>CASL</em> preparation module (deprecated).

    This routine is deprecated and calls 
    STB_UpdateCaslModule("PVM_SliceGeo",allowAdj);
    @see STB_UpdateCaslModule

  @ingroup PVM_DEPRECATED

 */                                     
DLL_INTERFACE void STB_UpdateCaslModule(double slabOffset, YesNo allowAdj = Yes);


/** Updates geometry parameters of the <em>CASL</em> preparation module.

  @PVM_PARS
  - PVM_CaslGeoCub
  - PVM_CaslLabelSliceOffset
  - PVM_CaslVec

  @PVM_TBCALL
  - @ref PTB_InsertGeoObject
  - @ref GTB_HandleGeoObject


  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE
  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code

  double slaboffs=0.0;

  // set slaboffset ...


  YesNo fast_update = STB_UpdateCaslGeometry();

  if(fast_update==No)
     STB_UpdateCaslModule(slabOffset);
  @endcode

  @return Yes if only update of geometry parameters is required

  @see @ref STB_InitCaslModule, @ref STB_UpdateCaslModule, 
       @ref ATB_SetCaslModuleBaseLevel

  @ingroup PVM_STB

 */
DLL_INTERFACE YesNo STB_UpdateCaslGeometry(void);


/**************************************************************************/
/* gen/src/prg/pvm_toolboxes/mr/PvSeqTools/Navigator.c */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_NavCommonRels(void);
DLL_INTERFACE void STB_NavOnOffRel(void);
DLL_INTERFACE void STB_NavSWhRel(void);
DLL_INTERFACE void STB_NavPulseEnumRel(void);
DLL_INTERFACE void STB_NavPulseRel(void);
DLL_INTERFACE void STB_NavPulseRange(void);
DLL_INTERFACE void STB_NavGeoObjRel(void);
DLL_INTERFACE void STB_NavGeoCubRel(void);


/** Initialise the group NavParameters of the navigator module.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Switch         </td> <td>@c PVM_NavOnOff                </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Parameter group</td> <td>@c NavParameters               </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitNavModule         </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateNavModule       </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_NavHandler              </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>BaseLevel      </td> <td>@ref ATB_SetNavModuleBaseLevel </td> <td>@c BaseLevelRelations.c</td></tr>
  <tr><td>Duration       </td> <td>@c PVM_NavigatorModuleTime     </td> <td>@c backbone()          </td></tr>
  <tr><td>Include        </td> <td>@c Navigator.mod               </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Subroutine     </td> <td>@c Navigator or NavigatorGlobal</td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Parameter card </td> <td>@c Navigator.xml               </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>

  This function should be used in the initMeth() routine of a method to
  initialise NavParameters.

  @param navGlobal (optional) YesNo option to deactivate slice selectivity
  of the navigator if it is set to "Yes" (default=No)

  @param navOnOff (optional) OnOff parameter that determines the initialization
  value of PVM_NavOnOff (default=On)

  @param navPoints (optional) int parameter that determines the initialization
  value of PVM_NavPoints (default=32)

  @param navSWh (optional) double parameter that determines the initialization
  value of PVM_NavSWh (default=100000)

  @param navSpatEnc (optional) YesNo option to activate spatial encoding of 
  the navigator (in read direction) (default=No)

  @PVM_PRECOND

  @endcode

  @see @ref STB_UpdateNavModule, @ref ATB_SetNavModuleBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitNavModule(YesNo navGlobal = No, OnOff navOnOff = On, int navPoints=64, double navSWh=100000, YesNo navSpatEnc = No);



/** Updates the group NavParameters of the navigator module.

  This function should be used in the backbone() routine of a method to update
  the parameters of the navigator module.

  @see @ref STB_InitNavModule, @ref ATB_SetNavModuleBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateNavModule(void);



/* gen/src/prg/pvm_toolboxes/mr/PvSeqTools/ArrayPhase.c */

/** Append adjustment of receiver array channel phases.

  This adjustment module is intended for use with spectrocopic methods only.

  @PVM_H2{Implementation}
  The following steps must be performed to use the <em>Array Phase</em>
  adjustment in a method:
  - In @c parsLayout.h insert the parameter @c PVM_ArrayPhase.
  - In @c backbone() include the toolbox function @ref STB_ArrayPhaseAppendAdjustment
    - Call @ref PTB_ClearAdjustments
    - Call @ref STB_ArrayPhaseAppendAdjustment
    - Append further required adjustments by calling @ref PTB_AppendAdjustment
      (e.g. receiver gain)
    - In @c callbackDefs.h redirect @c PVM_AdjHandler to @ref STB_ArrayPhaseHandleAdjustmentRequests
      or to a method specific adjustment handler function, where this function is
      called initially.
    - After the measurement, the adjustment result is available in @c PVM_ArrayPhase.
      In @c SetRecoParam() you may use @ref ATB_ArrayPhaseSetRecoPhase to transfer the
      result to the related reco parameter.
    - In the method's xml-file for the parameter layout: include the parameter
      @c PVM_ArrayPhase on the Result card.

  @PVM_PARS
  - @c PVM_ArrayPhaseHandleAdjRequests
  - @c PVM_ArrayPhaseNChan
  - @c PVM_ArrayPhaseEchoPosition

  @PVM_RELCALL
  NONE

  @PVM_TBCALL
  - @ref PTB_AppendAdjustment

  @PVM_PRECOND
  The method must make sure that the following baselevel parameters are set before
  the adjustment is started (not yet required when calling this function):
  - @c NI
  - <c>ACQ_size[0]</c>

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  A)
  PTB_ClearAdjustments();
  STB_ArrayPhaseAppendAdjustment(PVM_EncNReceivers, No);
  PTB_AppendConfiguredAdjustment(per_scan, RCVR);

  B)
  STB_ArrayPhaseAppendAdjustment(PVM_EncNReceivers, No, 0.0, NULL);

  C)
  STB_ArrayPhaseAppendAdjustment(PVM_EncNReceivers, No, 0.0, &PVM_VoxelGeoCub[0]);

  D)
  STB_ArrayPhaseAppendAdjustment(PVM_EncNReceivers, No, 0.0, &PVM_SliceGeo[0], PARX_get_dim("PVM_SliceGeo",1));

  @endcode

  @param nChan               Number of active receiver channels
  @param handleAdjRequests   Determines if other adjustment requests are handled
                             - @c Yes = call default adjustment handler for non-method specific
                               and receiver gain adjustments (default)
                               (to be used for immediate redirection of @c PVM_AdjHandler)
                             - @c No = only handle array phase adjustment
                               (to be used if more method-specific adjustments are added)
  @param echoPosition        Relative position in readout where maximum signal is to be
                             expected [%], e.g.
                             - @c 0.0 = FID
                             - @c 50.0 = centred spin echo

  @param cuboid              Pointer to cuboid (e.g. voxel, FOV) for which the phases are adjusted
                             (used for adjustment context and restore option). Argument is optional and can be NULL.

  @param nCuboids            If cuboid has more than one element and more than the first element should be considered
                             for the adjustment context, the number of elements can be specified via nCuboids (optional, default=1).

  @see @ref STB_ArrayPhaseHandleAdjustmentRequests, @ref ATB_ArrayPhaseSetRecoPhase

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_ArrayPhaseAppendAdjustment(int    nChan,
                                    YesNo  handleAdjRequests = Yes,
                                    double echoPosition      = 0.0,
                                    GEO_CUBOID_PACK *cuboid  = NULL,
                                    int nCuboids = 1);

/** Handles an adjustment request for the <em>Array Phase</em> adjustment.

  In @c callbackDefs.h redirect @c PVM_AdjHandler to this function.

  If the method uses more adjustments or the method is to be modified before the
  <em>Array Phase</em> adjustment, redirect @c PVM_AdjHandler to the respective
  method function that calls this handler first. From its return value it can be
  determined if the request could be handled.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref PTB_AdjMethSpec
  - @ref PTB_GetAdjCategory
  - @ref PTB_GetAdjName
  - @ref ATB_RgAdjSetGsPars

  @PVM_RELCALL
  - @c PVM_AdjHandler

  @PVM_PRECOND
  - @ref STB_ArrayPhaseAppendAdjustment must have been called.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  void LocalAdjustmentResultHandler()
  {
    if (STB_ArrayPhaseHandleAdjustmentRequests() == 1)
    {
      // ... switch off water suppression here ...

      backbone();
    }
  }
  @endcode

  @return
  - @c 0 = request is still to be handled
  - @c 1 = array phase request was handled
  - @c 2 = receiver gain request was handled by calling default handler
  - @c 3 = non-method specific request was handled by calling default handler

  @see @ref STB_ArrayPhaseAppendAdjustment, @ref ATB_ArrayPhaseSetRecoPhase

  @ingroup PVM_STB
*/
DLL_INTERFACE int STB_ArrayPhaseHandleAdjustmentRequests(void);



DLL_INTERFACE int STB_ArrayPhaseHandleAdjustment(void);


/* gen/src/prg/pvm_toolboxes/mr/PvSeqTools/SegmentationHandler.c */

DLL_INTERFACE void STB_SegmentationRels(void);

/** Initialise the parameters of the Segmentation module.

  The Segmentation module enables segmented preparation for sequences
  with one acquisition loop. Possible candidates are 3D radial sequences
  such as @em ZTE or @em UTE3D.

  A steady-state sequence is interrupted periodically for a preparation
  block. Optionally after each preparation, dummy scans can be applied.

  @image html Segmentation.png "Segmentation module as implemented in
  the @em ZTE method"

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Switch         </td> <td>@c PVM_SegmentationOnOff          </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Parameter group</td> <td>@c Segmentation_Parameters        </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitSegmentationModule   </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateSegmentationModule </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_SegmentationHandler        </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>Total Duration </td> <td>@c PVM_SegmentationTotalTime      </td> <td>@c backbone()          </td></tr>
  <tr><td>Include        </td> <td>@c Segmentation.mod               </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Subroutines    </td> <td>See @c Segmentation.mod           </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Parameter card </td> <td>@c Segmentation.xml               </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>

  @PVM_PARS
  - @c PVM_SegmentationOnOff
  - @c PVM_SegmentationForceOn
  - @c PVM_SegmentationDur
  - @c PVM_SegmentationDummyDur
  - @c PVM_SegmentationMinDur
  - @c PVM_SegmentationModuleTime

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateSegmentationModule

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitSegmentationModule(void);

/** Update the parameters of the Segmentation module.

  @PVM_PARS
  - @c PVM_SegmentationOnOff
  - @c PVM_SegmentationForceOn
  - @c PVM_SegmentationDur
  - @c PVM_SegmentationShots
  - @c PVM_SegmentationSegments
  - @c PVM_SegmentationSegmentsN
  - @c PVM_SegmentationDummies
  - @c PVM_SegmentationDelay
  - @c PVM_SegmentationModuleTime
  - @c PVM_SegmentationTotalTime

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  The parameters must have been initialised by calling @ref STB_InitSegmentationModule.

  @PVM_ERROR
  Invalid input values.

  @PVM_EXAMPLE
  @code
  STB_UpdateSegmentationModule(
    (PVM_TriggerModule == On || PVM_FatSupOnOff == On || PVM_FovSatOnOff == On) ? Yes : No,
    NPro * PVM_NAverages,
    PVM_RepetitionTime,
    PVM_TriggerModuleTime + PVM_FatSupModuleTime + PVM_FovSatModuleTime);
  @endcode

  @param forceOn    @c Yes = switch segmentation on
  @param nShots     Number of acquisition shots
  @param shotDur    Duration per acquisition shot
  @param prepDur    Duration per preparation block

  @see @ref STB_InitSegmentationModule

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateSegmentationModule(
  YesNo  forceOn,
  int    nShots,
  double shotDur,
  double prepDur);


/** Initialize the parameter group @em Mapshim.

  This function should be called in routine initMeth() of methods using
  the  @em Mapshim parameter group. It will initialize all parameters of
  this group to legal values.


  @PVM_PARS
  - @c   PVM_MapShimReady
  - @c   PVM_MapShimLocShim
  - @c   PVM_MapShimStatus
  - @c   PVM_MapShimCalcStat
  - @c   PVM_MapShimNSets
  - @c   PVM_MapShimSets
  - @c   PVM_MapShimNVolumes
  - @c   PVM_MapShimVolDescr
  - @c   PVM_MapShimVolumes
  - @c   PVM_StudyB0Map
  - @c   PVM_StudyB0MapShimset
  - @c   PVM_ReqShimEnum
  - @c   PVM_MapShimUseShims
  - @c   PVM_MapShimVolDerive
  - @c   PVM_MapShimVolMargin
  - @c   PVM_MapShimVolShape
  - @c   PVM_MapShim
  - @c   PVM_DynamicShimEventDuration
  

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
 // for methods that don't support dynamic shimming call
  STB_InitMapShim();
 // or
 STB_InitMapShim(No);
 
 // methods that support dynamic shimming call
 STB_InitMapShim(Yes);
 
  @endcode

  @see @ref STB_UpdateMapShim
  @param method_supports_dynamc_shimming (optional, default is No) flag 
         indicating whether method supports dynamic shimming
 
  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitMapShim(YesNo method_supports_dynamic_shimming=No);

/** Updates the parameter group @em Mapshim.

  This function should be called in methods using the  @em Mapshim parameter
  group. It will update the parameter group according to its arguments.

  @PVM_H1{Description of the @em Mapshim parameter class}
  The mapsim parameter group controlls the shim requirements of a method.
  Dependent on the state of "Requested Shim" ( parameter PVM_ReqShimEnum )
  different actions are done before the (possibly scan specific) adjustments
  (e.g. receiver gain etc.) and the final data acquisition are started.

  @PVM_H2{Shim Requirements}

  - Map_Shim : A new shim is calculated for the shim volume (defined by
    PVM_MapShimVolumes and PVM_MapShimVolDescr) and loaded to the hardware,
    if the shim volume is bound to a different geometry object (slice geometry,
    or voxel geometry) it might be restricted in orientation and size according
    to the state of its master. The geometry that is used as master is specified
    as parameter name in string argument @em gobj.
  - Study_Shim: The result of the global FID based shimming procedur is
    restored and loaded to the hardware
  - Current_Shim: The shimset that is currently loaded on the hardware remains
    unchanged.

  These shim requirements control the adjustment behaviour of method AdjShim.

  @PVM_PARS
  - @c   PVM_MapShimReady
  - @c   PVM_MapShimLocShim
  - @c   PVM_MapShimStatus
  - @c   PVM_MapShimCalcStat
  - @c   PVM_MapShimNSets
  - @c   PVM_MapShimSets
  - @c   PVM_MapShimNVolumes
  - @c   PVM_MapShimVolDescr
  - @c   PVM_MapShimVolumes
  - @c   PVM_StudyB0Map
  - @c   PVM_StudyB0MapShimset
  - @c   PVM_ReqShimEnum
  - @c   PVM_MapShimUseShims
  - @c   PVM_MapShimVolDerive
  - @c   PVM_MapShimVolMargin
  - @c   PVM_MapShimVolShape

  @PVM_TBCALL
   - @ref GTB_InitGeoCubeAsBBox
  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The current adjustment configuration needs to provide a scan specific
    shim adjustment. In case no such adjustment is configured, the state
    of @em PVM_ReqShimEnum is fixed to @em Current_Shim in this case.
  - the type of parameter ( specified by name in optional argument gobj)
    that serves as geometry reference must be @em GEO_OBJECT
  - standard initialization by @ref STB_InitMapShim in initMeth()
  - relation redirection of parameter @em PVM_MapShimHandler in
    @em callbackDefs.h

  @PVM_ERROR
  - illegal values for argument nuc
  - illegal parameter type of optional argument gobj

  @PVM_EXAMPLE
  @code

  void backbone(void)
  {
  (...)
  // mandatory: first update nuclei parameter group

  STB_UpdateNuclei(No);
  (...)

  // simplest call, no geometry master object is specified
  // size and shape of shim volume may be changed freely

  STB_UpdateMapShim("PVM_Nucleus1");

  // use the slice geometry as master:
  // dependent on state of the mapsim parameters, the default
  // initialization of the shim volume and / or its size, orientation and
  // pos are influenced by the state of the slice geometry.
  // assure a final state of the slice geometry before calling
  // this function (update the parameter group of the required geometry
  // master before in backbone)


  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  // same call for voxel geometry
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_VoxelGeoObj");

  @endcode

  @param nuc The nucleus that is currently active on the F1 channel
  @param gobj name of a parameter of type

  @see @ref STB_InitMapShim

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateMapShim(const char *const nuc,
                       const char *const gobj=0);
DLL_INTERFACE void STB_MapShimLocShim(void);
DLL_INTERFACE void STB_ReqShimEnum(void);
DLL_INTERFACE void STB_MapShimUseShims(void);
DLL_INTERFACE void STB_MapShimReady(void);
DLL_INTERFACE void STB_MapShimStatus(void);
DLL_INTERFACE void STB_MapShimCalcStat(void);
DLL_INTERFACE void STB_MapShimSets(void);
DLL_INTERFACE void STB_MapShimNSets(void);
DLL_INTERFACE void STB_MapShimVolDescr(void);
DLL_INTERFACE void STB_MapShimVolumes(void);
DLL_INTERFACE void STB_MapShimNVolumes(void);
DLL_INTERFACE void STB_StudyB0Map(void);
DLL_INTERFACE void STB_StudyB0MapShimset(void);
DLL_INTERFACE void STB_MapShimHandler(void);
DLL_INTERFACE void STB_MapShimVolumes(void);
DLL_INTERFACE void STB_MapShimVolDerive(void);
DLL_INTERFACE void STB_MapShimVolMaster(void);
DLL_INTERFACE void STB_MapShimVolMargin(void);
DLL_INTERFACE void STB_MapShimVolShape(void);
DLL_INTERFACE void STB_DynamicShimEventDurationRange(void);
DLL_INTERFACE void STB_DynamicShimEventDurationRel(void);

/**************************************************************************/
/*         gen/src/prg/pvm_toolboxes/mr/PvSeqTools/FairHandler.c          */
/**************************************************************************/

/* range checkers and relations */
DLL_INTERFACE void STB_FairCommonRels(void);
DLL_INTERFACE void STB_FairSpoilerRel(void);
DLL_INTERFACE void STB_FairInvSlabThickRel(void);
DLL_INTERFACE void STB_FairInvPulRange(void);
DLL_INTERFACE void STB_FairInvPulRel(void);
DLL_INTERFACE void STB_FairInvPulEnumRel(void);
DLL_INTERFACE void STB_FairInvPulAmplRel(void);
/** Initialises the parameters of the <em> FAIR </em> preparation module.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Parameter group</td> <td>@c FairParameters             </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitFairModule       </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateFairModule     </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_FairHandler            </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>BaseLevel      </td> <td>@ref ATB_SetFairBaseLevel     </td> <td>@c BaseLevelRelations.c</td></tr>
  <tr><td>Include        </td> <td>@c Fair.mod                   </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Subroutine     </td> <td>@c FairPrep, FairRec, FairInc </td> <td><c>\<method\>.ppg</c>  </td></tr>
  <tr><td>Parameter card </td> <td>@c Fair.xml                   </td> <td><c>\<method\>.xml</c>  </td></tr>
  <tr><td>Setup card     </td> <td>@c FairSetup.xml              </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>

  @PVM_PARS
  - @c PVM_FairInvPulEnum
  - @c PVM_FairInvPul
  - @c PVM_FairSpoiler
  - @c PVM_FairMode
  - @c PVM_FairTIR
  - @c PVM_FairTIR_NExp
  - @c PVM_FairTIR_Mode
  - @c PVM_FairTIR_Inc
  - @c PVM_FairMaxTIR
  - @c PVM_FairTIR_Arr
  - @c PVM_FairRepSpacing
  - @c PVM_FairInvSlabMargin

  @PVM_TBCALL
  - @ref STB_InitRFPulse
  - @ref MRT_InitSpoiler

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_UpdateFairModule, @ref ATB_SetFairBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitFairModule(void);

/** Updates the parameters of the <em> FAIR </em> preparation module.

  @PVM_PARS
  - @c PVM_FairInvPul
  - @c PVM_FairInvSlabMargin
  - @c PVM_FairInvSlabThick
  - @c PVM_FairInvSlabGradient
  - @c PVM_FairInvSlabOffsetHz
  - @c PVM_FairInvGrad
  - @c PVM_FairMinTIR
  - @c PVM_FairTIR
  - @c PVM_FairTIR_Inc
  - @c PVM_FairTIR_Fac
  - @c PVM_FairTIR_Arr
  - @c PVM_FairMaxTIR
  - @c PVM_FairVdList1
  - @c PVM_FairVdList2
  - @c PVM_FairFreqList

  @PVM_TBCALL
  - @ref STB_UpdateRFPulse
  - @ref MRT_MinSliceThickness
  - @ref MRT_UpdateSpoiler
  - @ref CFG_GradientRiseTime

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The module must have been initialised with @ref STB_InitFairModule.
  - The image geometry must have been updated.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_UpdateFairModule(deriveGains, fixedTime, slicePackThick, slicePackOffset, spoilerThick);
  @endcode

  @param     deriveGains  @c Yes = gains will be derived from reference power
  @param     fixedTime  Time after FairPrep and the center of RF excitation
  @param     slicePackThick  Thickness of slice package
  @param     slicePackOffset  Offset of slice package
  @param     spoilerThick  Scale basis for the spoiling efficiency

  @see @ref STB_InitFairModule, @ref ATB_SetFairBaseLevel

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateFairModule( YesNo  deriveGains,
                                         double fixedTime,
                                         double slicePackThick,
                                         double slicePackOffset,
                                         double spoilerThick);

/** Restores parameters adjusted as <em> Study </em> result.

    This function restores study result parameters. If they are adjusted
    their value is set to the last actual valid adjustment, otherwise
    they are set to "no value".

    This list must be extended for further adjustments in future.
    This function is called within method manager code at the time
    of parameter space creation as well as for all opened parameter
    spaces at the end of an adjustment sequence as defined by the
    running scan ( within STB_UpdateAdjResultValues).

  @PVM_PARS
  - @c PVM_FrqAdj1H
  - @c PVM_FrqAdj1HPpm
  - @c PVM_NucleiPpmRef
  - @c PVM_AdjShimset
  - @c PVM_BasicShim


  @PVM_TBCALL
  - @ref MRT_NucleiPpmRefInit
  - @ref PvAdjManRestorePar
  - @ref ParxRelsResetPar

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  STB_RestoreStudyResultPars();
  @endcode


  @ingroup PVM_STB_INT
*/
DLL_INTERFACE void STB_RestoreStudyResultPars(void);

/** Initializes startup shim parameter group.

    This function restores startup shim condition and initializes the state
    of the startup shim parameter group that control the startup shim
    condition in the first examination of a study. This function should
    be called in the initialization of methods.

  @PVM_H2{Implementation}
  Follow the general implementation rules for modules using these elements:
  <table>
  <tr><th>Element</th><th>Name</th><th>Used in</th></tr>
  <tr><td>Parameter group</td> <td>@c StartupShims             </td> <td>@c parsLayout.h        </td></tr>
  <tr><td>Initialiser    </td> <td>@ref STB_InitStartupShims   </td> <td>@c initMeth()          </td></tr>
  <tr><td>Updater        </td> <td>@ref STB_UpdateStartupShims </td> <td>@c backbone()          </td></tr>
  <tr><td>Handler        </td> <td>@c PVM_StartupShimHandler   </td> <td>@c callbackDefs.h      </td></tr>
  <tr><td>BaseLevel      </td> <td> -none-                     </td> <td> -                     </td></tr>
  <tr><td>Duration       </td> <td> -none-                     </td> <td> -                     </td></tr>
  <tr><td>Parameter card </td> <td>@c StartupShim.xml          </td> <td><c>\<method\>.xml</c>  </td></tr>
  </table>


  @PVM_PARS
  - PVM_TuneupShimAvailable
  - PVM_TuneupShim
  - PVM_StartupShimAvailable
  - PVM_StartupShims
  - PVM_StartupShimNames

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - errors caused by invalid scanner configuration

  @PVM_EXAMPLE
  @code
  STB_InitStartupShims();
  @endcode

  @see @ref STB_UpdateStartupShims
  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitStartupShims(void);


/** Updates startup shim parameter group.

    This function restores startup shim condition and updates the state
    of the startup shim parameter group that control the startup shim
    condition in the first examination of a study. This function should
    be called in the updating routine (backbone) of methods.


  @PVM_PARS
  - PVM_TuneupShimAvailable
  - PVM_TuneupShim
  - PVM_StartupShimAvailable
  - PVM_StartupShims
  - PVM_StartupShimNames
  - PVM_StartupShimCond
  - PVM_StartupShimList

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - errors caused by invalid scanner configuration

  @PVM_EXAMPLE
  @code
  STB_UpdateStartupShims();
  @endcode

  @see @ref STB_InitStartupShims
  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_UpdateStartupShims(void);



/** Checks startup shim parameter group.

    This function checks the legal state of startup shims.

  @PVM_PARS
  - PVM_TuneupShimAvailable .
  - PVM_TuneupShim .
  - PVM_StartupShimAvailable
  - PVM_StartupShims .
  - PVM_StartupShimNames .
  - PVM_StartupShimCond
  - PVM_StartupShimList .

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - errors caused by invalid scanner configuration
  - illegal dimension of array parameters

  @PVM_EXAMPLE
  @code
  int nstartupshims=STB_CheckStartupShims();
  @endcode
  @param supshims number of shims that are supported by the actual hardware
  @return number of startup shims

  @see @ref STB_InitStartupShims
  @ingroup PVM_STB_INT
*/
DLL_INTERFACE int STB_CheckStartupShims(int supshims);


DLL_INTERFACE void STB_StartupShimCond(void);
DLL_INTERFACE void STB_StartupShimList(void);


DLL_INTERFACE void STB_UpdateGradientLinearityCorrection(void);

/** Initializes parameter for mapshim calculation report.

    This function initializes parameter used for mapshim report.

  @PVM_PARS
  - PVM_MapShimReportBasicShimStatistics .
  - PVM_MapShimReportBasicShimHistorgams .
  - PVM_MapShimReportCalcStatus .


  @PVM_TBCALL
  STB_InitMapShimReportPars(nbins,0);

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - no errors
  @PVM_EXAMPLE
  @code
  STB_InitMapShimReportPars(100);
  @endcode
  @param nbins number of bins for histograms (if <= 0, 100 is used)

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitMapShimReportPars(int nbins);

/** Initializes parameter for dynamic shim calculation report.

    This function initializes parameter used for mapshim report.

  @PVM_PARS
  - PVM_MapShimReportDynamicShimStatistics .
  - PVM_MapShimReportDynamicShimHistorgams .
  - PVM_MapShimReportNDynamicShims .
  - PVM_MapShimReportNShims .
  - PVM_MapShimReportShimProperties .
  - PVM_MapShimReportShimIdentifiers .
  - PVM_MapShimReportShimHardwareChainIdentifier .


  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - no errors
  @PVM_EXAMPLE
  @code
  STB_InitMapShimReportPars(5,100);
  @endcode
  @param ndynshims number of dynamic shims (if <= 0, 1 is used)
  @param nbins number of bins for histograms (if <= 0, 100 is used)

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_InitMapShimReportPars(int ndynshims, int nbins);


/** Transfers state of PVM_SliceGeo to slice package parameters.

    This function transfers the state of PVM_SliceGeo to slice package
    parameters.

  @PVM_PARS
  - Parameters defining the coordinates of slice packages
    - @c PVM_SPackArrSliceGapMode
    - @c PVM_SPackArrSliceGap
    - @c PVM_SPackArrSliceDistance
    - @c PVM_SPackArrSliceOffset
    - @c PVM_SPackArrReadOffset
    - @c PVM_SPackArrPhase0Offset
    - @c PVM_SPackArrPhase1Offset
    - @c PVM_SPackArrPhase2Offset


  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  @PVM_EXAMPLE
  @code
  STB_TransferImagGeo();
  @endcode

  @ingroup PVM_STB
*/
DLL_INTERFACE void STB_TransferImagGeo(void);




/** Constrain slice positions to gradient center.

    This function updates @c  MethodAtsRefPosGroup group constraining  
    @c PVM_AtsBedPositionMode to @c ATS_POS_IMAGE_CENTER or 
    @c ATS_POS_REFERENCE .
    
    If @em ATS (Animal Transport System) support is activated in the study
    the bed position is fixed according to the state of 
    @c PVM_AtsBedPositionMode and the geometry can be constrained to assure 
    a zero offset to the modality center at this bed position.
    If no @em ATS support is activated the bed position is assumed to be 0.
    Arguments @c zero_pos0 ( @c zero_pos1 , @c zero_pos2 ) define which 
    component of the position (in the acquisition coordinates system, i.e. 
    read/phase/slice) should be set to the modality center 
    (gradient isocenter).


  @PVM_PARS

  - see @ref STB_InitAtsReferencePosition
  - parameter @c PVM_SliceGeo
  - parameter of the image geometry (@ref STB_InitImageGeometry)

  @PVM_TBCALL

  - @ref STB_ConstrainGeoObjectPosition
  - @ref STB_UpdateAtsReferencePosition
  - @ref STB_TransferImagGeo
 
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
  changed = STB_ConstrainSlicePosition(true,      // zero read component
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
     changed=STB_ConstrainSlicePosition( false,    // read component unchanged
                                         false,    // phase component unchanged
                                         true);    // slice component unchanged
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




  bool changed=false;
  // for mr modality: this assures a zero read offset:
 
  @endcode


  @see STB_UpdateAtsReferencePosition

  @ingroup PVM_STB

  @param zero_pos0 flag to force zero modality offset along read direction
  @param zero_pos1 flag to force zero modality offset along phase direction
  @param zero_pos2 flag to force zero modality offset along slice direction

  @return true if the position has been changed according to constraints.
*/
DLL_INTERFACE bool STB_ConstrainSlicePosition(bool zero_pos0,
                                              bool zero_pos1,
                                              bool zero_pos2);


#endif
#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
