/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2016 - 2017
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVCFGMRTOOLS_H
#define PVCFGMRTOOLS_H

#ifndef CPROTO

#include "common/pvm_toolboxes/PvCfgTools.h"
#include "PvGenericServer2Mgr/MR/PreempParameters.hh"
#include "PvGenericServer2Mgr/MR/PreempParameterDefines.h"


#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

/// DLL interface libParx identifiers for Windows.
#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#  ifdef PvCfgMriTools_INTERNAL
#    define DLL_INTERFACE __declspec(dllexport)
#  else
#    define DLL_INTERFACE __declspec(dllimport)
#  endif
#elif defined (__GNUC__) && __GNUC__ >= 4
#    define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#    define DLL_INTERFACE
#endif



/** Returns the upper limit of an anti alias factor.

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

  @return Maximum anti alias factor

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_MaxAntiAliasFactor(void);




/* gen/src/prg/pvm_toolboxes/PvCfgTools/datastation.c */

/** Returns @c Yes for a data station configuration, @c No otherwise.

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @return  @c Yes for a  data station, @c No otherwise

  @todo Incomplete documentation: PVM_PARS, PVM_PRECOND, PVM_ERROR

  @ingroup PVM_CFG
*/
DLL_INTERFACE YesNo CFG_IsDatastation(void);




/* gen/src/prg/pvm_toolboxes/PvCfgTools/gradients.c */

/** Maximally achievable gradient duty cycle.

  The maximally achievable duty cycle on a gradient channel is the ratio of
  - the maximum power that can theoretically be achieved if the maximum current \f$I_{max}\f$
    was applied, and
  - the maximum allowed power \f$P_{max}\f$:

  \f[DC_{max}[\%] = 100.0\,\frac{1.2\,R \cdot I_{max}^2}{P_{max}}\f]

  where

  @li \f$R\f$ = resistance at \f$20\,^{\circ}{\rm C}\f$,
  @li the factor 1.2 takes into account a 20% increase of the resistance with the temperature
      increasing up to \f$60\,^{\circ}{\rm C}\f$,
  @li \f$I_{max}\f$ includes the gradient calibration constant of the respective channel.

  The actual gradient duty cycle \f$DC\f$ for a (sufficiently short) time interval \f$T\f$ can
  be calculated according to

  \f[DC[\%] = DC_{max} \cdot \frac{t}{T} \cdot G^2\f]

  where

  @li \f$t\f$ = gradient-on time in interval,
  @li \f$G\f$ = gradient strength averaged over \f$t\f$ and normalised with maximum gradient
      strength.

  For most gradient systems the sum of \f$DC\f$ over all three channels must not exceed 100%
  when integrated over a 1 sec interval. @n
  For some gradient systems the channels are independent (e.g. BGA38S) and \f$DC\f$ must not
  exceed 100% for each channel.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Invalid input value
  - Deriving gradient values failed

  @PVM_EXAMPLE
  @code
  double maxDutyCycle = CFG_GradientMaxDutyCycle("mean");
  @endcode

  @param[in] chan Gradient channel
                  - @c "X", @c "Y", or @c "Z"
                  - @c "max"  = maximum of all channels
                  - @c "mean" = mean over all channels

  @return Maximum gradient duty cycle [%]

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_GradientMaxDutyCycle(const char *chan);




/** Retrieves the gradient calibration constant appropriate to the given nucleus.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nucStr is a NULL pointer
  - @a nucStr is a NULL string
  - nucleus is not known

  @PVM_EXAMPLE
  @code
  double bandwidth = gradAmp / 100.0 * CFG_GradCalConst(PVM_Nucleus1) * PVM_Fov[0];
  @endcode

  @param[in] nucStr    Name of an isotope as defined in the nuclei table

  @return Gradient calibration constant (Hz/mm)

  @see @ref CFG_MaxGradientStrength

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_GradCalConst(const char *nucStr);



/** Retrieves the gradient ramp time.

  In case  PREEMP_ramp_mode ==  Constant_time_xyz or  Shaped_ramp_xyz the maximum of
  PREEMP_ramp_time_x, PREEMP_ramp_time_y, and PREEMP_ramp_time_z will be returned.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - No or illegal system information about gradient ramp time available.
  - @c PREEMP_ramp_mode = @c Ramp_off
  - No or illegal values of @c PREEMP_ramp_time in case @c PREEMP_ramp_mode = @c Constant_time or
    @c Shaped_ramp
  - No or illegal values of @c PREEMP_ramp_time_x, @c PREEMP_ramp_time_y, or @c PREEMP_ramp_time_z in
    case @c PREEMP_ramp_mode = @c Constant_time_xyz or @c Shaped_ramp_xyz

  In these cases the preemphasis file must be corrected.

  @return Duration of a gradient ramp (ms) or @c 1.0 in case of an error condition

  @see @ref CFG_GradientRiseTime, @ref CFG_GradientDelayTime,
       @ref CFG_MinGradientPulseTime, @ref CFG_InterGradientWaitTime, @ref CFG_GradientShapeResolution

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_GradientRampTime(void);



/** Retrieves the gradient rise time.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - No or illegal system information about gradient rise time available.

  @return Duration of gradient rise time (ms) or  @c 1.0 in case of an error

  @see @ref CFG_GradientRampTime, @ref CFG_GradientDelayTime,
       @ref CFG_MinGradientPulseTime, @ref CFG_InterGradientWaitTime, @ref CFG_GradientShapeResolution

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_GradientRiseTime(void);




/** Calculates the gradient delay time.

  This is defined as the half of the difference between gradient rise time and gradient
  ramp time.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_GradientRampTime
  - @ref CFG_GradientRiseTime

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - No or illegal information about gradient ramp and rise time.

  @return Duration of the gradient delay (ms) or @c 0.0 in case of an error

  @see @ref CFG_GradientRampTime, @ref CFG_GradientRiseTime,
       @ref CFG_MinGradientPulseTime, @ref CFG_InterGradientWaitTime, @ref CFG_GradientShapeResolution

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_GradientDelayTime(void);




/** Retrieves the inter-gradient wait time.

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

  @return Duration of the inter-gradient wait time (ms)

  @see @ref CFG_GradientRampTime, @ref CFG_GradientRiseTime, @ref CFG_GradientDelayTime,
       @ref CFG_MinGradientPulseTime, @ref CFG_GradientShapeResolution

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_InterGradientWaitTime(void);




/** Calculates the minimum duration of a gradient pulse.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_InterGradientWaitTime
  - @ref CFG_GradientRampTime

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @return Minimum duration of a gradient pulse (ms)

  @see @ref CFG_GradientRampTime, @ref CFG_GradientRiseTime, @ref CFG_GradientDelayTime,
       @ref CFG_InterGradientWaitTime, @ref CFG_GradientShapeResolution

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_MinGradientPulseTime(void);




/** Delivers the minimum gradient dwell time.

  This function is intended to constrain the time resolution of user defined gradient waveforms
  to hardware capabilities.

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

  @return Minimum gradient dwell time [ms]

  @see @ref CFG_GradientRampTime, @ref CFG_GradientRiseTime, @ref CFG_GradientDelayTime,
       @ref CFG_MinGradientPulseTime, @ref CFG_InterGradientWaitTime

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_GradientShapeResolution(void);





/** Returns the maximum gradient strength (mT/m)

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_GradCalConst

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @return Gradient strength (mT/m) at 100% gradient power

  @see @ref CFG_GradCalConst, @ref CFG_GradientMaxDutyCycle

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_MaxGradientStrength(void);




/** Retrieves the system status string.

  Copies the current study specific system status string to @a string.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The argument @a string must be of length @c PVSYSINFOCHANSTATE_LEN.

  @PVM_ERROR
  - Getting status string failed

  @PVM_EXAMPLE
  @code
  char status[PVSYSINFOCHANSTATE_LEN];
  CFG_StatusString(status);
  @endcode

  @returns The status string as std::string

  @see @ref CFG_ShimStatusString

  @ingroup PVM_CFG
*/
DLL_INTERFACE void CFG_StatusString(char *string);




/** Returns the system status string.

  Gets the current study specific system status string from the
  configuration.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Getting status string failed

  @PVM_EXAMPLE
  @code
  std::string status = CFG_StatusString();
  @endcode

  @param[out] string    Character string of length @c PVSYSINFOCHANSTATE_LEN

  @see @ref CFG_ShimStatusString

  @ingroup PVM_CFG
*/
DLL_INTERFACE const std::string CFG_StatusString();



/** Retrieves the state of the shim system.

  Copies the state of the shim system to @a statstr.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The argument @a statstr must be of length PVSYSINFOCHANSTATE_LEN.

  @PVM_ERROR
  - Getting status string failed

  @PVM_EXAMPLE
  @code
  CFG_ShimStatusString(PVM_ShimCoeffHwStatus);
  @endcode

  @param[out] statstr    Chararacter string of size PVSYSINFOCHANSTATE_LEN

  @see @ref CFG_StatusString

  @ingroup PVM_CFG
*/
DLL_INTERFACE void CFG_ShimStatusString(char *statstr);




/** Get maximum possible value for a shim channel.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Getting maximum shim value failed

  @PVM_EXAMPLE
  @code
  int maxShimVal = 0;
  for(int i = 0; i < 128; i++)
  {
    maxShimVal = MAX_OF(maxShimVal, CFG_MaxShimValue(i));
  }
  @endcode

  @param shim    Number of shim channel

  @return Maximum shim value [shim units]

  @ingroup PVM_CFG
*/
DLL_INTERFACE int CFG_MaxShimValue(int shim);




/** Delivers the max number of shim channels.

  The max number of shim channels that can be handled by the software is returned.

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

  @return Max number of shim channels that can be handled by the software

  @ingroup PVM_CFG
*/
DLL_INTERFACE int CFG_NShims(void);




/** Convert shim name to the @c SHIM_PAR enum state.

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
  SHIM_PAR shim;
  CFG_ShimName2ShimEnum("Z2", &shim);
  @endcode

  @param[in]  snam   Shim name
  @param[out] senum  Shim enum value

  @return
  - @c 0 = shim was found
  - @c 1 = shim was not found and @a senum was set to @c Shim_Z0

  @ingroup PVM_CFG
*/
DLL_INTERFACE int CFG_ShimName2ShimEnum(const char *const  snam,
                                        SHIM_PAR          *senum);




/** Initialise shim configuration parameters.

  @PVM_PARS
  - Parameters given by @a stateparn, @a chanparn, and @a sensparn

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
  CFG_InitShimConfigPars("PVM_ShimCoeffHwStatus",
                         "ShimId",
                         "ShimSens",
                         Yes);
  @endcode

  @param[in] stateparn  Parameter name for shim state
  @param[in] chanparn   Parameter name for shim ids
  @param[in] sensparn   Parameter name for shim sensitivities
  @param     rawshim    @c Yes = raw mode is available

  @return @c Yes = initialisation successful, else @c No

  @ingroup PVM_CFG_INT

*/
DLL_INTERFACE YesNo CFG_InitShimConfigPars(const char *stateparn,
                                           const char *chanparn,
                                           const char *sensparn,
                                           YesNo       rawshim);




/* /pv60/gen/src/prg/pvm_toolboxes/PvCfgTools/nuclei.c */

/** Determines the ratio of the gyromagnetic ratio of the given nucleus to that of protons.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nucStr is a @c NULL pointer.
  - @a nucStr is a @c NULL string.
  - Gamma ratio for nucleus could not be determined.

  @PVM_EXAMPLE
  @code
  double gamma = 42.5764 * CFG_GammaRatio(PVM_Nucleus1);
  @endcode

  @param nucStr    Name of an isotope as defined in the nuclei table

  @return Gamma ratio or @c 1.0 in case of an error

  @see @ref CFG_BasicFrequency, @ref CFG_StandardFreq

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_GammaRatio(const char *nucStr);




/** Retrieves basic proton frequency of the system.

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

  @return Configuration frequency for 1H (MHz)

  @see @ref CFG_GammaRatio, @ref CFG_StandardFreq

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_BasicFrequency(void);




/** Retrieves the standard basic frequency of the given nucleus.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nucStr is a @c NULL pointer.
  - @a nucStr is a @c NULL string.
  - @a nucStr not found.

  @PVM_EXAMPLE
  @code
  double freq1H = CFG_StandardFreq("1H");
  @endcode

  @param nucStr    Name of an isotope as defined in the nuclei table

  @return Standard frequency for selected nucleus [MHz]

  @see @ref CFG_GammaRatio, @ref CFG_BasicFrequency

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_StandardFreq(const char *nucStr);




/** Looks up the given nucleus in the nuclei table.

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
  if (CFG_IsNucleusKnown(nucName) == No)
  {
    strcpy(nucName, "1H");
  }
  @endcode

  @param nucName    Name of an isotope as defined in the nuclei table

  @return  @c Yes if the nucleus is known, @c No otherwise

  @ingroup PVM_CFG
*/
DLL_INTERFACE YesNo CFG_IsNucleusKnown(const char *nucName);




/** Returns number of transmit channels.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Number of transmit channels could not be determined.

  @return  Number of transmit channels

  @ingroup PVM_CFG
*/
DLL_INTERFACE int CFG_GetNumberOfTransmitChannels(void);




/** Returns number of receive channels.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Number of receive channels could not be determined.

  @return  Number of receive channels

  @ingroup PVM_CFG
*/
DLL_INTERFACE int CFG_GetNumberOfReceiveChannels(void);




/* /pv60/gen/src/prg/pvm_toolboxes/PvCfgTools/rfPulses.c */

/* range checkers and relations */
DLL_INTERFACE YesNo CFG_RFPulseModeRange(const int);




/** Delivers a save default RF-pulse attenuation for the first logical frequency channel.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_RFPulseDefaultPower

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @return RF-pulse attenuation (dB)

  @see @ref CFG_MaxRFPowerShape, @ref CFG_MaxRFPower, @ref CFG_CalcMaxRFPower,
       @ref CFG_MinCWRFAttenuation, @ref CFG_MaxCWRFPower, @ref CFG_RFPulseDefaultPower

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_RFPulseDefaultAttenuation(void);




/** Delivers a default RF-pulse shape name.

  Dependent on the desired classification, specified by argument, a shapename is delivered.

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
  Initialisation of an excitation pulse
  @code
  STB_InitRFPulse("ExcPul",        // name of pulse structure
                  "ExcPulseEnum",  // name of pulse list parameter
                  "ExcPulseAmpl",  // name of pulse amplitude parameter
                  "ExcPulseShape", // name of pulse shape (for calc. pulses)
                  0,               // used for excitation
                  CFG_RFPulseDefaultShapename(LIB_EXCITATION), // default shape
                  2000.0,          // default bandwidth
                  90.0);           // default pulse angle
  @endcode

  @param exmRequest    Desired classification of the RF-pulse represented as
                       @c PVM_PULSE_CLASSIFICATION

  @return Pointer to a string containing the desired RF waveform (including correct extension)

  @see @ref CFG_RFPulseMaxShapenameLength, @ref CFG_RFPulseShapenameWellFormed

  @ingroup PVM_CFG
*/
DLL_INTERFACE const char *CFG_RFPulseDefaultShapename(const PVM_PULSE_CLASSIFICATION exmRequest);



/** Delivers the max number of different RF shapes.

  The max number @c N of RF-pulse shapes that can be used in a pulse program dependent on
  different spn (n = @c 0,..., <c>N-1</c>) is returned.

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

  @return max number of RF-pulse shapes.

  @ingroup PVM_CFG
*/
DLL_INTERFACE int CFG_RFPulsesMaxNumberOfPulses(void);




/**  Scans all files in wave directory, builds up the internal linked pulse list.

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

  @ingroup PVM_CFG
*/
DLL_INTERFACE void CFG_UpdateRFPulseList(void);




/** Checks extension of RF-pulse waveform files.

  The extension is checked to match the required RF-pulse excitation mode.

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
  const char ext[] = ".rfc";
  EXCITATION_MODE mode = Excitation;

  if (CFG_RFPulseExtensionWellFormed(mode, ext) == No)
  {
    UT_ReportError("extension doesn't match excitation mode");
  }
  @endcode

  @param     exMode    Excitation mode of RF-pulse
  @param[in] ext       String containing extension (inclucing dot @c ".")

  @return @c Yes in case of matching excitation mode, else @c No

  @see @ref CFG_RFPulseRephfacWellFormed, @ref CFG_RFPulseShapenameWellFormed

  @ingroup PVM_CFG
*/
DLL_INTERFACE YesNo CFG_RFPulseExtensionWellFormed(const EXCITATION_MODE exMode,
                                                   const char *ext);




/** Delivers pulse classification of shaped RF-pulse.

  The shape name specified by argument is checked and the classification as specified in the
  shape header is returned in argument @a classification.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_RFPulseShapenameWellFormed
  - @ref CFG_UpdateRFPulseList

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Shape name must be well-formed, see @ref CFG_RFPulseShapenameWellFormed
  - Shape must be PPG usable
  - Shape must have valid excitation mode

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  // parameter definition
  char parameter PulseName[16];
  PVM_PULSE_CLASSIFICATION parameter PulseClassification;
  ...

  if (CFG_GetPulseClassification(PulseName, &PulseClassification) == No)
  {
    ParxRelsResetPar("PulseClassification");
  }
  @endcode

  @param[in]  shapename          Name of an RF waveform file
  @param[out] classification     Pointer to a variable or type @c PVM_PULSE_CLASSIFICATION

  @return @c Yes in case of success, @c No in case preconditions are not fulfilled (see above)

  @see @ref CFG_GetRotAngle, @ref CFG_RFPulseGetPowIntFac

  @ingroup PVM_CFG
*/
DLL_INTERFACE YesNo CFG_GetPulseClassification(const char *const         shapename,
                                               PVM_PULSE_CLASSIFICATION *classification);




/** Delivers the minimum possible duration of an RF-pulse.

  Note: The minimum duration of an RF-pulse is also dependent on the number of shape points.

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

  @return  Minimum possible RF-pulse length (ms)

  @see @ref CFG_RFPulseGetMinPulseLength

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_RFPulseMinDuration(void);




/** Delivers the minimum possible RF shape dwell time.

  The dwell time is defined as the minimum time difference of 2 complex shape points.

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

  @return  Minimum possible RF-pulse length (ms)

  @see @ref CFG_RFPulseGetMinPulseLength

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_RFPulseMinPulseDwellTime(void);




/** Delivers the minimum duration of an RF-pulse waveform.

  It is dependent on the number of complex shape points specified as argument. In case of
  illegal values (negative or @c 0), it returns @ref CFG_RFPulseMinDuration.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_RFPulseMinPulseDwellTime
  - @ref CFG_RFPulseMinDuration

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  double minLength = CFG_RFPulseGetMinPulseLength(512);
  @endcode

  @param npoints    Number of complex waveform points

  @return Minimum possible RF-pulse length (ms)

  @see @ref CFG_RFPulseMinPulseDwellTime, @ref CFG_RFPulseMinDuration

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_RFPulseGetMinPulseLength(const int npoints);




/** Delivers the min possible bandwidth factor of an RF-pulse.

  Dependent on the excitation mode as specified by argument the bandwidth factor (kHz*ms) is
  returned. This function is used to check information stored in RF waveform files.

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
  double minBwFac = CFG_RFPulseMinBandwidthFactor(Excitation);
  @endcode

  @param mode    Excitation mode of the RF waveform

  @return Minimum bandwidth factor (kHz*ms)

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_RFPulseMinBandwidthFactor(EXCITATION_MODE mode);




/** Checks the rephase factor of an RF-pulse waveform.

  Dependent on the desired excitation mode the rephase factor is checked to lie in a legal
  range. The rephase factor of an RF-pulse waveform defines the amount of pulse length (%)
  that is required to be refocused by a gradient integral of appropriate polarity.

  - Excitation: Rephase factor is expected to lie within [@c 0.0 ; @c 100.0]
  - Refocussing: Rephase factor is expected to lie within [@c -100.0 ; @c 100.0]
  - Inversion: Rephase factor is expected to lie within [@c -100.0 ; @c 100.0]

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
  if (CFG_RFPulseRephfacWellFormed(ExcPulse.Rpfac, excitation) == No)
  {
    ExcPulse.Rpfac = MIN_OF(MAX_OF(ExcPulse.Rpfac, 0.0), 100.0);
  }
  @endcode

  @param fac     Rephase factor (%)
  @param mode    Excitation mode of the RF waveform

  @return @c Yes in case @a fac lies within legal range else @c No

  @see @ref CFG_RFPulseExtensionWellFormed, @ref CFG_RFPulseShapenameWellFormed

  @ingroup PVM_CFG
*/
DLL_INTERFACE YesNo CFG_RFPulseRephfacWellFormed(double          fac,
                                                 EXCITATION_MODE mode);



/** Initializes and checks parameters of type PV_PULSE_LIST.

  Dynamic enumeration parameters of type PV_PULSE_LIST are initialized with lists of RF
  pulses in the waveform directory that match a specific excitation mode.
  Argument @a listparn must be a the name of a PV_PULSE_LIST parameter, @a mode
  must be either 0 (excitation) 1 (refocusing) or 2 (inversion). The function

  - builds up dynamic enumeration entries of all RF pulses for the selected mode
    (extension @c ".exc" , @c ".rfc" , @c ".inv" is removed) and as last element the key
    word "Calculated".
  - initializes a default state if the parameter has no value.
  - checks whether the actual state of the parameter matches the required mode.

  @PVM_PARS
  Parameter specified by name (argument @a listparn)

  @PVM_TBCALL
  - @ref CFG_UpdateRFPulseList

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  At least RF pulses delivered with the software need to be installed in the
  waveform directory.

  @PVM_ERROR
  All errors of this function start with message "CFG_PulseFileListRange: Fatal Error"
  - Argument check
    - invalid value of argument @a mode message:@n
      "Argument 2 illegal mode (value <val>"
    - argument @a listparn is not a parameter name, message:@n
      "Argument 1 (value <listparn> ) is not a parameter name"
    - argument @a listparn is an array parameter, message:@n
      "Arg 1 (<listparn>) is not a scalar parameter"
    - argument @a listparn is not a parameter of type PV_PULSE_LIST, message:@n
      "Arg 1 (<listparn>) is not a parameter of type PVM_PULSE_LIST"
  - RF pulse installation
    - No pulses of the required specification found in waveform directory, message:@n
      "Incorrect shape waveform installation, no valid pulses for <mode> found"

  @PVM_EXAMPLE
  Code example of a parameter definition and a range checking function of
  a  PV_PULSE_LIST parameter. Note: this function is implicitly used by
  STB_UpdateRFPulse and needs not to be used in method code.

  @code
  // parameter definition (e.g. parsDefinition.h)

  PV_PULSE_LIST parameter
  {
    display_name "Refocusing pulse list";
    relations    RFCPulseListRel;
  }RFCPulseList;

  // range checking function using CFG_PulseFileListRange

  void RFCPulseListRange(void)
  {
    CFG_PulseFileListRange("RFCPulseList",1); // checks refocusing pulses
  }

  void RFCPulseListRel(void)
  {
    RFCPulseListRange();
    backbone();
  }
  @endcode

  @param[in] listparn  Name of a @c PVM_PULSE_LIST parameter
  @param     mode indicator of pulse excitation mode
             - 0 = excitation
             - 1 = refocusing
             - 2 = inversion

  @return 0

  @see STB_UpdateRFPulse

  @todo Review, Documentation of type PV_PULSE_LIST

  @ingroup PVM_CFG_INT
*/
DLL_INTERFACE int CFG_PulseFileListRange(const char *const listparn, int mode);




/** Delivers the highest RF-pulse attenuation.

  The return value of this function may be used to set the .power fields of @c TPQQ structs.

  Outdated for pulses with amplitudes specified with peak power in watt

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

  @return Max attenuation (dB)

  @todo reference to PvUtilConvWattToDb required

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE double CFG_RFPulseHighestAttenuation(void);




/** Lowest RF attenuation.

  @PVM_USEINSTEAD{CFG_MaxRFPowerShape}

  Delivers the lowest RF-pulse attenuation that can be tolerated by the hardware connected to
  the 1st logical channel F1.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_MinRFAttenuation(1,0,0,1.0);

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @return Lowest attenuation (dB), in case of errors return value of
          @ref CFG_RFPulseHighestAttenuation

  @see @ref CFG_MinRFAttenuation, @ref CFG_RFPulseHighestAttenuation

  @todo reference to PvOvlUtilConvWattToDb required

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE double CFG_RFPulseLowestAttenuation(void);




/** Lowest RF attenuation (channel specific).

  @PVM_USEINSTEAD{CFG_MaxRFPowerShape}

  Delivers the lowest RF-pulse attenuation (dB) that can be tolerated by the hardware
  connected to the logical channel specified by argument. In case of errors the hardware
  constraints may not be performed and an error condition will be printed out in the messages
  window of the system control tool and in the ParaVision starting shell.

  ATTENTION: Hardware damage is possible.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  @ref CFG_MaxRFPowerShape  PvOvlUtilConvWattToDb

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  @see CFG_MaxRFPowerShape

  @param channel      Logical transmit channel number. This channel is checked to be usable for
                      RF transmission. If @a channel is less than @c 1 the minimum possible
                      attenuation is returned. If @a channel is greater than the number of the
                      transmit channels according to the current coil setup the highest
                      possible attenuation is returned.
  @param nuc          Nucleus that is used on specified channel. In case of illegal or no value
                      of this argument the max power of the amplifier is assumed to be
                      available on the transmit coil, otherwise a frequency dependent damping
                      may be considered (if calibrated).
  @param shapename    Name of the RF waveform that is used for the RF-pulse.
                      If valid (i.e. shape is present in waveform directory) the power integral
                      of the shape is considered in the calculation.
  @param plen         RF-pulse length (ms), for values less or equal @c 0.0 the length and
                      shape are not considered in the calculation. ATTENTION: Possible damage
                      of the hardware is possible for shapes of long duration.


  @return Lowest attenuation (dB). In case of errors return the lowest possible attenuation.

  @todo review, reference to PvOvlUtilConvWattToDb

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE double CFG_MinRFAttenuation(int         channel,
                                          const char *nuc,
                                          const char *shapename,
                                          double      plen);




/** Delivers maximum RF pulse power for shape in waveform directory.

  Delivers the maximum RF-pulse power (Watt) that can be transmited
  to the hardware routed on logical channel specified by argument @a channel.

  In case the required system information is not available, some limitations
  are not considered and a warning will be written using PARX_sprintf.

  ATTENTION: Hardware damage is possible.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_RFPulseGetPowIntFac
  - @ref CFG_MaxRFPower

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  See @ref CFG_MaxRFPower

  @PVM_EXAMPLE
  @code
  double maxPower = CFG_MaxRFPowerShape(1, PVM_Nucleus1, ExcPulse.Filename, ExcPulse.Length);
  @endcode

  @param channel       Logical transmit channel number. This channel is checked to be usable for
                       RF transmission. If @a channel is less than @c 1 the minimum possible
                       attenuation is returned. If @a channel is greater than the number of the
                       transmit channels according to the current coil setup the highest
                       possible attenuation is returned.
  @param[in] nuc       Nucleus that is used on specified channel. In case of illegal or no value
                       of this argument the max power of the amplifier is assumed to be
                       available on the transmit coil, otherwise a frequency dependent damping
                       may be considered (if calibrated).
  @param[in] shapename Name of the RF waveform that is used for the RF-pulse.
                       If valid (i.e. shape is present in waveform directory) the power integral
                       of the shape is considered in the calculation.
  @param pulselength   RF-pulse length (ms), for values less or equal @c 0.0 the length and
                       shape are not considered in the calculation. ATTENTION: Possible damage
                       of the hardware damage is for shapes of long duration.


  @return Maximum RF pulse power (Watt).

  @see @ref CFG_MaxRFPower, @ref CFG_CalcMaxRFPower,
       @ref CFG_MinCWRFAttenuation, @ref CFG_MaxCWRFPower, @ref CFG_RFPulseDefaultPower,
       @ref CFG_RFPulseDefaultAttenuation

  @todo to be reviewed

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_MaxRFPowerShape(int         channel,
                                         const char *nuc,
                                         const char *shapename,
                                         double      pulselength);




/** Delivers maximum RF pulse amplitude (Watt).

    The maximum RF pulse amplitude is dependent on the duration (@a pulselength in ms)
    and power integral (@a powint \f$\in\f$ ]0.0,1.0])
    of the RF pulse as well as the hardware limits of the transmit chain that
    is specified by argument @a channel (logical channel).
    The power integral of RF shapes can be calculated using
    @ref CFG_RFPulseGetPowIntFac for library shapes installed in the waveform
    directory or @ref MRT_RFPulseGetIntFac for calculated waveforms stored
    in a double array parameter.


  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_CalcMaxRFPower

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  Routine must be used after updating the Nuclei parameter group.
  Valid system configuration.

  @PVM_ERROR

  @PVM_H2{Illegal or insufficient system configuration}

  The following list of errors indicate an invalid system configuration
  that needs to be reported to the hotline:

  - Illegal state for number of tx channels
  - Max number of tx channels == 0
  - Failed to retrieve max peak power pulselength
  - Illegal system information about max peak power pulselength
    (correction of coil configuration required)
  - Failed to retrieve max possible peak power
  - Retrieved illegal value of max possible RF power
  - Failed to retrieve max coil peak power
  - Retrieved illegal max coil peak power
  - Failed to retrieve max coil CW power
  - Retrieved illegal max coil CW power

  @PVM_H2{Check of arguments}

  These messages indicate a programming error.

  - Illegal channel argument specified: if the specified channel is larger than
    the maximum logical channel that is supported by the current coil configuration.
  - pulse duration @a pulselength < 0.0 ms

  If argument @a errmsg is not a 0 pointer the error condition is written into the string errmsg points to and 0.0 is returned. If errmsg is a 0 pointer, error conditions above will be reported with UT_ReportError
  causing a runtime error.

  @PVM_H2{Warnings}

  In case system information is missing the following warnings are printed
  with PARX_sprintf:

  -  No information about  max peak power pulselength: The transition to the
     CW limitation cannot be calculated, possible hardware damage for long RF
     pulse durations may occur.
     Repair: specify correct value in coil configuration
  -  No information about max possible RF power available (0.0W):
     RF pulse amplitude is constrained to 0W. Reason:
     The RF amplifier has not been detected or calibrated properly.
     Repair: contact service personnal.
  -  No information about max coil peak power: 0.0W
     The RF pulse peak power limitation of the coil is missing, neither a peak
     power nor a CW power limitation will be performed, possible hardware
     damage may occur.
     Repair: specify correct value in coil configuration.
  -  No information about max coil CW power (0.0W)
     The RF pulse CW power limitation of the coil is missing, a CW power
     limitation will not be performed, possible hardware
     damage may occur for long RF pulses.
     Repair: specify correct value in coil configuration.

  @PVM_EXAMPLE
  @code
  double maxPower = CFG_MaxRFPower(1, PVM_Nucleus1, 1.0, ExcPulse.Length);
  @endcode

  @param channel    The logical channel the RF pulse should be transmitted. (must
                    correspond to the related line in the pulse program). Values
                    \f$\le\f$ 0 result in a return value of 0W. Values larger than
                    the maximum possible logical channel that is supported by the
                    currently chosen coil configuration will cause a runtime error.
  @param[in] nuc    The nucleus that is excited at the logical channel (use
                    @c PVM_NucleusX, with X=channel), after updating Nuclei
                    parameter.

  @param powint     The power integral of the RF pulse shape. Values outside the
                    interval ]0.0,1.0] are treated as 1.0 (worst case scenario).

  @param pulselength Duration of the RF pulse shape (in ms). A value \f$\ge\f$ 0.0
                     is allowed (for @a pulselength = 0.0 ms max allowed peak
                     power is returned without possible CW limitation).

  @param[out] errmsg (optional) If specified a string that contains error conditions
                     if occured (length of 256 characters required)

  @see @ref CFG_MaxRFPowerShape, @ref CFG_CalcMaxRFPower,
       @ref CFG_MinCWRFAttenuation, @ref CFG_MaxCWRFPower, @ref CFG_RFPulseDefaultPower,
       @ref CFG_RFPulseDefaultAttenuation

  @todo Review

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_MaxRFPower(int         channel,
                                    const char *nuc,
                                    double      powint,
                                    double      pulselength,
                                    char       *errmsg=0);




/** Calculates maximum RF pulse peak power (amplitude).

  The maximum RF pulse amplitude (units W) depends on the duration (@a pulselength
  in ms), the power integral factor (@a powint \f$\in\f$ ]0.0,1.0], for definition
  see @ref MRT_RFPulseGetIntFac ) and hardware limits of the currently routed transmit
  channel such as maximum peak power of the transmit chain (@a maxpchain units W),
  peak power limitation of the RF coil (@a maxpcoil units W), continuous wave power
  limitation of the transmit chain (@a maxcwpow units W) and the maximum duration
  of a block pulse at maximum peak power of the RF coil (@a maxpowplen units ms).

  The limitation is based on the dissipated RF energy during the RF pulse. With

  @li  @a pulselength \f$\tau_p\f$
  @li  @a powint \f$W_p\f$
  @li  @a maxpchain \f$P_{Tx}\f$
  @li  @a maxpcoil \f$P_{Coil}\f$
  @li  @a maxcwpow \f$P_{CW}\f$
  @li  @a maxpowplen \f$\tau_{Max}\f$
  @li  maximum peak power \f$P_{Max}\f$ and
  @li  \f$T_{eff} \equiv  \frac{\tau_{Max}*P_{Coil}}{P_{Tx}*W_p}\f$

  Three power regimes can be distinguished:

  @PVM_H2{Maximum Power Regime}

  \f$ 0 \le \tau_p \le T_{eff}
        \Rightarrow P_{Max} = \min (P_{Coil},P_{Tx}) \f$

  @PVM_H2{Intermediate Power Regime}

  \f$ \tau_p > T_{eff} \f$ and \f$ P_{Max} > P_{CW}
         \Rightarrow P_{Max} = \min (P_{Coil},P_{Tx})*\frac{T_{eff}}{\tau_p}\f$

  @PVM_H2{CW Power Regime}

  \f$ P_{Max} = P_{CW}\f$

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  Nonzero values of arguments @a maxpowplen @a maxpcoil @a maxpchain @a maxcwpow requried to consider
  CW power limitations, otherwise routine returns @a maxpchain and prints a message using @c PARX_sprintf.

  @PVM_ERROR
  @li @a pulselength < 0.0 : "illegal pulse duration"
  @li @a powint \f$\ni\f$ ]0.0,1.0] : "illegal power integral"
  @li @a maxpchain < 0.0 W : "illegal max power"
  @li @a maxpcoil < 0.0 W : "illegal max peak power for coil"
  @li @a maxcwpow < 0.0 W : "illegal max cw power"
  @li @a maxpowplen < 0.0 ms: "illegal max power pulse duration"

  If argument @a errmsg is not a @c NULL pointer, the error condition is written into the string
  @a errmsg points to and 0.0 is returned. If @a errmsg is a @c NULL pointer, error conditions
  above will be reported with @ref UT_ReportError causing a runtime error.

  @PVM_EXAMPLE
  @code

  // PVM_RFPulse1 in this example defined as parameter of type PVM_RF_PULSE
  // retrieve maximum pulse peak power (i.e. max. pulse amplitude)

  // example with errstring string

  char errstring[256]; // important less than 256 bytes will cause a memory failure

  double maxpulseampl=0.0
  double maxpchain=0.0:
  double maxpcoil=0.0;
  double maxcwpow=0.0;
  double maxpowplen=0.0;

  // get values ...

  maxpulseampl=CFG_CalcMaxRFPower(RFPulse1.Length, // pulse length in ms
                                  RFPulse1.Pint,   // pulse shape power integral
                                  maxpchain,
                                  maxpcoil,
                                  maxcwpow,
                                  maxpowplen,
                                  errstring);

  // react on error:

  if(*errstring)
  {
     char errmsg[512];
     snprintf(errmsg,511,"Call of CFG_CalcMaxRFPower failed with message\n%s",errstring);
     errmsg[511]='\0';
     UT_ReportError(errmsg);  // code stops here with message in errmsg string
  }

  // example without using errstring, will cause a runtime error:

  maxpulseampl=CFG_CalcMaxRFPower(-1.0,            // illegal value of pulse length
                                  RFPulse1.Pint,
                                  maxpchain,
                                  maxpcoil,
                                  maxcwpow,
                                  maxpowplen);

  // Code stops at this point, with error message of CFG_CalcMaxRFPower
  @endcode

  @param pulselength  duration of RF pulse (unit ms)
  @param powint  the power integral factor \f$\in\f$ ]0.0,1.0]
  @param maxpchain  maximum peak power of the transmit chain (units W)
  @param maxpcoil   peak power limitation of the RF coil (units W)
  @param maxcwpow   continuous wave power limitation of the transmit chain (units W)
  @param maxpowplen maximum duration of a blockpulse at maximum
                    peak power of the RF coil (units ms)
  @param errmsg  If specified a string that contains error conditions if occured (length of 256 characters required)

  @return maximum possible pulse peak amplitude (in W)

  @see @ref MRT_RFPulseGetIntFac, @ref CFG_MaxRFPowerShape, @ref CFG_MaxRFPower,
       @ref CFG_MinCWRFAttenuation, @ref CFG_MaxCWRFPower, @ref CFG_RFPulseDefaultPower,
       @ref CFG_RFPulseDefaultAttenuation

  @todo
  - review
  - discuss open documentation of SysInfo functions to make example meaningful
  - reference to PARX_sprintf not resolved

  @ingroup PVM_CFG
*/

DLL_INTERFACE double CFG_CalcMaxRFPower(double pulselength,
                                        double powint,
                                        double maxpchain,
                                        double maxpcoil,
                                        double maxcwpow,
                                        double maxpowplen,
                                        char  *errmsg=0);




/** Lowest CW RF attenuation (channel specific).

  Delivers the lowest continuous wave (CW) RF-pulse attenuation that can be tolerated by
  the hardware connected to the logical channel specified by argument. In case of errors the
  hardware constraints may not be performed and an error condition will be printed out in the
  messages window of the system control tool and in the ParaVision starting shell.

  ATTENTION: Hardware damage is possible.

  In case of errors the value of @ref CFG_RFPulseHighestAttenuation will be retured. In this
  case an error condition will be printed out in the messages window of the system control tool
  and in the ParaVision starting shell.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_MaxCWRFPower

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  double minAtt = CFG_MinCWRFAttenuation(1, PVM_Nucleus1);
  @endcode

  @param channel      Logical transmit channel number. This channel is checked to be usable for
                      RF transmission. If @a channel is less than @c 1 the minimum possible
                      attenuation is returned. If @a channel is greater than the number of the
                      transmit channels according to the current coil setup the highest
                      possible attenuation is returned.
  @param[in] nuc      Nucleus that is used on specified channel. In case of illegal or no value
                      of this argument the max power of the amplifier is assumed to be
                      available on the transmit coil, otherwise a frequency dependent damping
                      may be considered (if calibrated).

  @return Lowest attenuation (dB). In case of errors return the highest possible attenuation.

  @see @ref CFG_MaxRFPowerShape, @ref CFG_MaxRFPower, @ref CFG_CalcMaxRFPower,
       @ref CFG_MaxCWRFPower, @ref CFG_RFPulseDefaultPower,
       @ref CFG_RFPulseDefaultAttenuation

  @ingroup PVM_CFG
 */
DLL_INTERFACE double CFG_MinCWRFAttenuation(int        channel,
                                            const char *nuc);




/** Delivers the max length of an RF-pulse waveform filename.

  The return value may be used to check the length of waveform filenames. Longer filenames are
  ignored by all functions handling RF-pulse waveform files.

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

  @return Max length of shape names (chars, including @c 0)

  @see @ref CFG_RFPulseMaxShapenameLength, @ref CFG_RFPulseShapenameWellFormed

  @ingroup PVM_CFG
*/
DLL_INTERFACE int CFG_RFPulseMaxShapenameLength(void);




/** Tests whether the filename of shape is well formed.

  A valid shape name must
  - have a maximum of one extension (separated by a dot)
  - contain only alphanumeric characters except an underline or one dot (used to mark an
    extension)
  - have a maximum number of @ref CFG_RFPulseMaxShapenameLength() characters
  - not be named @c USER_PULSE nor @c Calculated

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
  if (CFG_RFPulseShapenameWellFormed(ExcPulse.Shape) == No)
  {
    UT_ReportError("shape name not well formed");
  }
  @endcode

  @param[in] shname    String containing the shape name to be examined

  @return @c Yes = shape file name is well formed, else @c No

  @see @ref CFG_RFPulseDefaultShapename, @ref CFG_RFPulseMaxShapenameLength,
       @ref CFG_RFPulseExtensionWellFormed, @ref CFG_RFPulseRephfacWellFormed


  @ingroup PVM_CFG
*/
DLL_INTERFACE YesNo CFG_RFPulseShapenameWellFormed(const char *const shname);




/** Delivers user defined RF pulse string indicator.

  To build up lists of RF pulses that match specific properties (excitation,
  refocusing, inversion) pulses in the waveform directory are scanned, the
  result is used to build up dynamic enumeration lists of type @c PV_PULSE_LIST. Pulses
  that may be used in pulse programs but the required properties are not
  stored in the waveform file are marked as @c "USER_PULSE". This function
  returns this string that is reserved for such pulses. Pulses with the same
  name in the waveform directory are ignored.

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
  Function needs not to be called in method code. It is used internally
  to exclude special filenames of RF pulses.

  @code
  const char *userpulsename=CFG_RFPulseSpecialUserPulseName();
  @endcode

  @return string @c "USER_PULSE"

  @todo review Documentation of type PV_PULSE_LIST

  @see @ref CFG_RFPulseSpecialCalcPulseName

  @ingroup PVM_CFG_INT
*/
DLL_INTERFACE const char *CFG_RFPulseSpecialUserPulseName(void);




/** Delivers a calculated RF pulse string indicator.

  To build up lists of RF pulses that match specific properties (excitation,
  refocusing, inversion) pulses in the waveform directory are scanned, the
  result is used to build up dynamic enumeration lists of type @c PV_PULSE_LIST.
  Pulses that are calculated and the resultant waveform is stored in separate
  parameters are marked as @c "Calculated". This function
  returns this string that is reserved for such pulses. Pulses with the same
  name in the waveform directory are ignored.

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
  Function needs not to be called in method code. It is used internally
  to exclude special filenames of RF pulses.

  @code
  const char *calcpulsename=CFG_RFPulseSpecialCalcPulseName();
  @endcode

  @return string @c "Calculated"

  @todo review  Documentation of type PV_PULSE_LIST

  @see @ref CFG_RFPulseSpecialUserPulseName

  @ingroup PVM_CFG_INT
*/

DLL_INTERFACE const char *CFG_RFPulseSpecialCalcPulseName(void);




/** Updates the low level RF-pulse properties of a RF waveform file.

  The waveform directory is scanned for the RF waveform as specified in the .Filename field of
  the @a property struct argument and the remaining low level pulse properties are set
  according to information stored in the waveform file.

  @PVM_PARS
  - Parameter given by @a property

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The waveform filename (including extension) should be set in the @c .Filename field of
    argument @a property.
  - The desired RF-pulse classification should be set in the Classification field.

  @PVM_ERROR
  - Insufficient number of RF-pulse shapes are installed in the waveform directory.
  - The desired RF-pulse classification is set to an illegal value.

  @PVM_EXAMPLE
  @code
  for (int i = 0; i < PVM_SlRFPulseArrSize; i++)
  {
    CFG_UpdateRFPulseProperties(&PVM_SlRFPulsePropertyArr[i], Yes);
    STB_RFPulsePropertyRange(&PVM_SlRFPulsePropertyArr[i]);
  }
  @endcode

  @param[in] property Pointer to a struct of type @c PVM_PULSE_PROPERTIES
  @param refresh      If set to @c Yes also for user pulses all properties fields that can be
                      set according to shape header information will be updated.

  @return @c No in case of errors, else @c Yes

  @ingroup PVM_CFG_INT
*/
DLL_INTERFACE YesNo CFG_UpdateRFPulseProperties(PVM_PULSE_PROPERTIES *property,
                                                YesNo                refresh);




/** Get enum values of library RF pulses.

  @PVM_PARS
  - Parameter given by @a pulseEnumParName

  @PVM_TBCALL
  - @ref CFG_UpdateRFPulseList

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a pulseEnumParName is an illegal parameter name

  @PVM_EXAMPLE
  @code
  int nExcPulses = CFG_LibPulseEnumRange("ExcPulseEnum", LIB_EXCITATION);
  @endcode

  @param[in] pulseEnumParName Name of pulse enum parameter
  @param     ClassRequest     Pulse classification

  @return
  - Number of enum values
  - @c -1 = @a ClassRequest does not indicate a library pulse or no pulse is available

  @ingroup PVM_CFG
*/
DLL_INTERFACE int CFG_LibPulseEnumRange(const char *const              pulseEnumParName,
                                        const PVM_PULSE_CLASSIFICATION ClassRequest);




/** Delivers rot angle as stored in RF waveform file.

  The waveform filename should be given as argument. In case of illegal name or illegal waveform
  information an angle of @c -1.0 is returned.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Filename should be set.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  PVM_PULSE pul;
  PVM_PULSE_PROPERTIES pro;
  ...

  CFG_GetRotAngle(pro.Filename, &pul.FlipAngle);
  @endcode

  @param[in]  filename    Name of a RF waveform file stored in waveform directory
  @param[out] angle       Pointer to double value that is set to the angle

  @return @c Yes in case the information is available, else @c No

  @see @ref CFG_RFPulseGetPowIntFac, @ref CFG_GetPulseClassification

  @ingroup PVM_CFG
*/
DLL_INTERFACE YesNo CFG_GetRotAngle(const   char *const filename,
                                    double *const       angle);




/** RF waveform specific power integral ratio.

  Delivers the power integral ratio (integral over squared waveform compared  to block pulse)

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
  double pif = 1.0;
  CFG_RFPulseGetPowIntFac(ExcPulse.Filename, &pif);
  @endcode

  @param[in]  filename    Name of RF pulse waveform file
  @param[out] pif         Pointer to double delivered power integral factor

  @return
  - @c Yes = information is available
  - @c No = argument @a pif was not changed

  @see @ref CFG_GetRotAngle, @ref CFG_GetPulseClassification, @ref MRT_RFPulseGetIntFac

  @ingroup PVM_CFG
*/
DLL_INTERFACE YesNo CFG_RFPulseGetPowIntFac(const char *filename,
                                            double     *pif);




/** Constrains the attenuation correction fields of RF-pulses.

  This function is called whenever attenuation correction fields of RF-pulse parameters are
  edited and constrain the allowed range of attenuation corrections.

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
  double attcorr = 0.0;
  attcorr = CFG_AttCorrRange(attcorr);
  @endcode

  @param attcorr    Value of attenuation correction to be checked

  @return Checked @a attcorr value

  @ingroup PVM_CFG_INT
*/
DLL_INTERFACE double CFG_AttCorrRange(const double attcorr);




/** Delivers a safe default RF-pulse power (Watt).

  The value corresponds to the maximum power of the logical transmit channel (specified by
  argument) attenuated by 36dB.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_RFPulseDefaultPower

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  // parameter definition
  double parameter
  {
    display_name "Initial Power";
    units "W";
    relations InitPowRel;
  } InitPow;

  ...

  // in relation InitPowRel
  if (!ParxRelsParHasValue("InitPow"))
  {
    InitPow = CFG_RFPulseDefaultPower();
  }
  @endcode

  @param chan (optional) Logical transmit channel number. Default = @c 1.

  @return RF-pulse power (Watt)

  @see @ref CFG_MaxRFPowerShape, @ref CFG_MaxRFPower, @ref CFG_CalcMaxRFPower,
       @ref CFG_MinCWRFAttenuation, @ref CFG_MaxCWRFPower,
       @ref CFG_RFPulseDefaultAttenuation

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_RFPulseDefaultPower(int chan = 1);





/** Highest CW RF-pulse power (channel specific).

  Delivers the highest continuous wave (CW) RF-pulse power that can be tolerated by the
  hardware connected to the logical channel specified by argument.

  ATTENTION: Hardware damage is possible.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  In case of errors the
  hardware constraints may not be performed and an error condition will be printed out in the
  messages window of the system control tool and in the ParaVision starting shell.
  - retrieved illegal state for max allowed CW power
  - retrieved illegal value for max allowed CW power
  - retrieved illegal state for max allowed peak power
  - retrieved illegal value for max allowed peak power

  @PVM_EXAMPLE
  @code
  double maxCwPow = CFG_MaxCWRFPower(1, PVM_Nucleus1);
  @endcode

  @param channel     Logical transmit channel number. This channel is checked to be usable for
                     RF transmission. If @a channel is greater than the number of the transmit
                     channels according to the current coil setup a low (save) value is
                     returned to prevent coil damage.
  @param[in] nuc     Nucleus that is used on specified channel. In case of illegal or no value
                     of this argument the max power of the amplifier is assumed to be available
                     on the transmit coil, otherwise a frequency dependent damping may be
                     considered (if calibrated).


  @return Highest CW power (Watt). In case of errors return @c 0.0.

  @see @ref CFG_MaxRFPowerShape, @ref CFG_MaxRFPower, @ref CFG_CalcMaxRFPower,
       @ref CFG_MinCWRFAttenuation, @ref CFG_RFPulseDefaultPower,
       @ref CFG_RFPulseDefaultAttenuation

  @todo description incorrect!

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_MaxCWRFPower(int        channel,
                        const char *nuc);





/* gen/src/prg/pvm_toolboxes/PvCfgTools/machine.c */










/** Delivers the number of receivers installed in the system.

  Note: The number of receivers that can be accessed on method level depends on the active coil
        configuration.

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

  @return Number of installed receivers

  @see @ref ATB_SetDigPars, @ref CFG_NFreqChannels

  @ingroup PVM_CFG
*/
DLL_INTERFACE int CFG_NReceivers(void);





/** Delivers the number of available frequency channels.

  This function is used to deliver the number of different frequency channels that are
  available on the system.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - A valid system configuration is required. In case of a datastation @c 1 is returned.

  @PVM_ERROR
  NONE

  @return Number of frequency channels represented as int

  @see @ref CFG_NFreqChannels

  @ingroup PVM_CFG
*/
DLL_INTERFACE int CFG_NFreqChannels(void);












/** Delivers a valid receiver gain (RG) value.

  This function is used to derive valid values of the receiver gain (i.e. values that are
  realized on hardware level.
  Dependent on the desired operation mode (given as argument @a mode) either
  - the closet value (@a mode = @c 2)
  - a value that is less than or equal (@a mode = @c 0)
  - a value that is higher or equal (@a mode = @c 1)

  compared to the input value (argument @a inRg) is returned.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - A valid system configuration is required.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  PVM_AutoRgInitVal = CFG_RgValue(PVM_AutoRgInitVal, 2);
  @endcode

  @param inRg    Required receiver gain
  @param mode    Flag controlling the operation mode of this function (see above).
                 @c 0, @c 1, and @c 2 are supported, other values default to @c 2.

  @return Valid receiver gain

  @see @ref CFG_MaxRg, @ref CFG_MinRg, @ref CFG_RgInitVal

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_RgValue(double inRg,
                   int    mode);





/** Delivers the largest RG value.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_RgValue

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - A valid system configuration is required.

  @PVM_ERROR
  NONE

  @return Largest receiver gain value

  @see @ref CFG_RgValue, @ref CFG_MinRg, @ref CFG_RgInitVal

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_MaxRg(void);





/** Delivers the smallest RG value.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_RgValue

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - A valid system configuration is required.

  @PVM_ERROR
  NONE

  @return Smallest receiver gain value

  @see @ref CFG_RgValue, @ref CFG_MaxRg, @ref CFG_RgInitVal

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_MinRg(void);





/** Delivers a starting value for RG adjustments.

  Dependent on the electronic a starting value for the RG adjustment is delivered.
  This value is adapted so that noise is digitized with sufficient bit resolution.
  Increasing RG above this value will not increase bit resolution or S/N.

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

  @return Valid receiver gain value

  @see @ref CFG_RgValue, @ref CFG_MaxRg, @ref CFG_MinRg

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_RgInitVal(void);





/** Delivers size of RG table (valid entries within uxnmr.par:RG_tableA)

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

  @returns valid entries of RG table (0, in case no valid entries found)

  @see @ref CFG_RgValue, @ref CFG_MaxRg, @ref CFG_MinRg, @ref CFG_RgTable

  @ingroup PVM_CFG
*/
DLL_INTERFACE unsigned int CFG_RgTableSize(void);





/** Delivers RG table (valid entries within uxnmr.par:RG_tableA)

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

  @param gainTable  Table to be filled
  @param maxsize    size of Table to be filled (to avoid memory violation)
                    If (size > maxsize), the remaining values are not delivered.
  @returns valid entries of RG table (0, in case no valid entries found)

  @see @ref CFG_RgValue, @ref CFG_MaxRg, @ref CFG_MinRg, @ref CFG_RgTableSize

  @ingroup PVM_CFG
*/
DLL_INTERFACE unsigned int CFG_RgTable(double *gainTable, unsigned int maxsize);




/** Delivers the instrument type.

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

  @return @c CONFIG_instrument_type

  @see @ref CFG_Digitizer, @ref CFG_DigFilter, @ref CFG_InstrumentName

  @ingroup PVM_CFG
*/
DLL_INTERFACE INSTRUMENT_TYPE CFG_InstrumentType(void);




/** Delivers Yes if system type is ICON.

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

  @return @c YesNo

  @see @ref @ref CFG_isIcon

  @ingroup PVM_CFG
*/
DLL_INTERFACE YesNo CFG_isIcon(void);





/* gen/src/prg/pvm_toolboxes/PvCfgTools/tcu.c */

/** Minimum possible delay in pulse programs.

  Returns the minimum possible delay (ms) usable in  pulse programs. Delays shorter than
  this value will be treated as zero delays.

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

  @return Minimum delay (ms)

  @see @ref CFG_AmplifierEnable

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_MinDelay(void);





/** Returns the delay necessary to prepare the HF amplifier.

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

  @return Delay necessary to prepare the HF amplifier [ms]

  @see @ref CFG_MinDelay

  @ingroup PVM_CFG
*/
DLL_INTERFACE double CFG_AmplifierEnable(void);




/* gen/src/prg/pvm_toolboxes/PvCfgTools/AdjVals.c */

/* range checkers and relations */
DLL_INTERFACE void CFG_RefAttRange(void);





/** Delivers special reference attenuation for nucleus of interest.

  The special reference attenuation (1ms 90 deg pulse) will be delivered
  -# if it has been adjusted for the nucleus as specified by the argument
  -# if the status specified by argument @a stat matches the information delivered by the
     adjustment routine.

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
  Retrieves a global reference attenuation.
  @code
    YesNo available = No;
    double refAtt;

    ...
    STB_UpdateNuclei(Yes);
    ...

    available = CFG_GetSpecRefAtt(PVM_Nucleus1,"global",&refAtt);
  @endcode

  @param nuc       String containing the nucleus of interest
  @param stat      String containing information about special requirements for the reference
                   attenuation
  @param refAtt    Pointer to double value. It will be modified to the value of reference
                   attenuation only if a return value of @c Yes is returned.

  @return  @c Yes if the requested gain has been adjusted

  @see @ref ATB_SetRefAtt

  @ingroup PVM_CFG
*/
DLL_INTERFACE YesNo CFG_GetSpecRefAtt(const   char *const nuc,
                                      const   char *const stat,
                                            double *const refAtt);





/** Delivers a global reference attenuation for nucleus of interest.

  @PVM_USEINSTEAD{STB_GetRefAtt}

  The reference attenuation (1ms 90 deg pulse) will be delivered if it has been adjusted for
  the nucleus as specified by the argument. The logical channel 1 is assumed to be the transmit
  channel.

  @PVM_DEACT
  20100101

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_GetSpecRefAtt

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Function should be called after an update of the nuclei group.

  @PVM_ERROR
  NONE

  @param nuc    String containing the nucleus of interest
  @param att    Pointer to double value. It will be modified to the value of reference
                attenuation only if a return value of @c Yes is returned.

  @return  @c Yes if the requested gain has been adjusted

  @see @ref ATB_SetRefAtt, @ref CFG_GetSpecRefAtt, @ref STB_GetRefAtt

  @todo Incomplete documentation: PVM_EXAMPLE

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE YesNo CFG_GetGlobRefAtt(const char *const nuc,
                                      double *const     att);





/** Delivers a reference attenuation (dB) for a 90 deg rectangular RF-pulse of 1 ms duration for
  protons.

  @PVM_USEINSTEAD{STB_GetRefAtt}

  @param refGain    Points to double variable where the reference attenuation should be stored

  @return  @c Yes if the attenuation was successfully adjusted by the spectrometer,
           @c No otherwise.

  @see @ref STB_GetRefAtt

  @todo Incomplete documentation: PVM_PARS, PVM_TBCALL, PVM_RELCALL,
                                  PVM_PRECOND, PVM_ERROR, PVM_EXAMPLE

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE YesNo CFG_RFPulseGetGlobalReferenceAttenuation(double *const refGain);





/** Converts shim ID to spherical function ID.

    The spherical function identifier is stored into the entry
    of the shim identifier specified as offset between 0 and N_SHIMS
    of argument skey. A value of -1 is set if no spherical function
    is available.


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

  // get spherical function index and name

  int spfIdKey[N_SHIMS];
  CFG_ShimId2SpherId(spfIdKey);

  int sind,spfind;
  char shimName[128];
  char sphericalName[128];

  for(sind=0;sind <N_SHIMS;sind++)
  {
    spfind=spfIdKey[sind];  // index of PvnlSpherical function

    if(spfind >= 0)
    {
       strncpy(sphericalName,PvnlSphercalFuncName(spfind),128);
    }
    else
    {
       strncpy(sphericalName,"Not Available",128);
    }

    strncpy(shimName,SysInfoShimIdToName(sind));
  }
  @endcode

  @param skey  int array of size N_SHIMS

  @ingroup PVM_CFG_INT
*/
DLL_INTERFACE void CFG_ShimId2SpherId( int *skey);

/** Provides job title of standard acquisition.


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
  char * jobname=CFG_DefaultJobTitle();
  printf("Job title: %s",jobname);

  @endcode


  @ingroup PVM_CFG
*/
DLL_INTERFACE const char* CFG_DefaultJobTitle(void);


#endif
#endif

/****************************************************************/
/* E N D   O F   F I L E */
/****************************************************************/
