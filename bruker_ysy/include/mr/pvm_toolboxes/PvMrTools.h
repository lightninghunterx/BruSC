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

#ifndef PVMRTOOLS_H
#define PVMRTOOLS_H

#ifndef CPROTO

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

#if defined(HAS_WINDOWS_FEATURES)
#  ifdef PvMrTools_INTERNAL
#    define DLL_INTERFACE __declspec(dllexport)
#  else
#    define DLL_INTERFACE __declspec(dllimport)
#  endif
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#  define DLL_INTERFACE
#endif



/** Definition of a function pointer type for RF shape algorithms.

  Functions of this type are used to calculate RF pulse waveforms
  and handle structs of type @c PVM_RF_PULSE.

  @PVM_H1{General description of shape algorithm functions}

  A special group of functions of type @c SHAPE_ALG_PROC are used
  to calculate RF pulse shapes used in parameters of type @c PVM_RF_PULSE.
  In these functions a dedicated algorithm is controlled by values stored
  in the pulse shape structure (@a shapeStruct) and/or by a colon separated
  list of parameter names that are used for this purpose (@a addParnames).
  Functions of this type are used as argument for routine
  @ref STB_UpdateRFPulse to handle calculated shapes.

  @PVM_H2{Procedure}
  -# check of valid argument values
  -# calculation of normalized RF pulse shape (polar representation)
     and low level attributes ( <c> Type / Bwfac / Rphac </c> )
     based on high level attributes of pulse struct ( <c> Length / Bandwidth / Flipangle / Calculated /
     Sharpness </c>). Low level parameters may be used as input and output parameters
     dependent on the algorithm. The size of the double array containing the shape is controlled
     by the algorithm.
  -# calculation of low level attributes (<c> Pint / Sint </c> )

  @PVM_H2{Update of pulse structure}

  Each pulse shape algorithm is informed by argument @a handle about the currently modified
  struct element. The integer value may be interpreted as follows:

  <table>
  <tr><th> Value     </th><th>    Element edited     </th><th>   behavior                      </th></tr>
  <tr><td> 0         </td><td>    Not edited         </td><td>   default                       </td></tr>
  <tr><td> 1         </td><td>    <c>.Length     </c></td><td>   derive Bandwidth from Length  </td></tr>
  <tr><td> 2         </td><td>    <c>.Bandwidth  </c></td><td>   derive Length from Bandwidth  </td></tr>
  <tr><td> 3         </td><td>    <c>.Flipangle  </c></td><td>   derive Pow                    </td></tr>
  <tr><td> 4         </td><td>    <c>.Calculated </c></td><td>   not applicable                </td></tr>
  <tr><td> 5         </td><td>    <c>.Sharpness  </c></td><td>   controlls algorithm           </td></tr>
  <tr><td> 7         </td><td>    <c>.Bwfac      </c></td><td>   specific for shape algorithm  </td></tr>
  <tr><td> 8         </td><td>    <c>.Sint       </c></td><td>   ignored calculated in routine </td></tr>
  <tr><td> 9         </td><td>    <c>.Pint       </c></td><td>   ignored calculated in routine </td></tr>
  <tr><td> 10        </td><td>    <c>.Type       </c></td><td>   controlls algorithm           </td></tr>
  <tr><td> 11        </td><td>    <c>.Rphac      </c></td><td>   controlled by algorithm       </td></tr>
  <tr><td> 12        </td><td>    <c>.Pow        </c></td><td>   derive Flipangle              </td></tr>
  <tr><td> 13        </td><td>    <c>.Shape      </c></td><td>   not applicable                </td></tr>
  </table>

  To handle power limitations of the currently routed transmit chain that limits the maximum pulse
  amplitude, hardware limits are provided in additional arguments that may influence the calculated
  waveform.

  @param shapeStruct  Pointer to a variable of type @c PVM_RF_PULSE
  @param handle       Integer value indicating special requests (see table above)
  @param addParnames  string containing a colon separated list of parameter names that are
                      used to control the shape algorithm (@c NULL pointer allowed)
  @param pmax         Max possible RF power of the currently routed transmit channel (units W)
  @param pcoilmax     Max allowed RF power the currently routed coil may tolerate (units W)
  @param mpdur        Max duration of block pulse with max allowed power (units ms)
  @param pcwmax       Max allowed continuous wave (CW) power of the currently routed transmit
                      channel (units W dependent on coil and amplifier capabilities)
  @param pref         Reference power (power for 1ms 90 deg block pulse)
  @param setPower     Tells if the calculated pulse power should be set in shapeStruct.pow

  @return In general the return value of shape algorithms indicate whether the pulse struct
  parameter should be updated according to low level attributes calculated by the routine (return value 1).
  Routines returning @c 0 resolve any dependency of the pulse struct parameters.

  @see @ref MRT_ConventionalRFShape, @ref STB_UpdateRFPulse

  @ingroup PVM_MRT

  @todo decide references to SysInfo functions, decide general format of type definitions
*/
typedef int (* SHAPE_ALG_PROC)(PVM_RF_PULSE *const shapeStruct,
                               int handle,
                               const char *const addParnames,
                               double pmax,
                               double pcoilmax,
                               double mpdur,
                               double pcwmax,
                               double pref,
                               YesNo  setPower);

#define MRT_CPMG_NPOINTS 8


/**************************************************************************/
/*         gen/src/prg/pvm_toolboxes/mr/PvMrTools/digitizer.c             */
/**************************************************************************/


/** Calculates acquisiton duration.

  This function calculates the acquisition duration @acqDur using unaliased
  bandwidth @a sw, the anti-aliasing factor (@a alias) and the aliased number of
  points (@np).

  Before calculating  @acqDur it derives an aliased bandwidth (@a sw*alias)
  and sets it to the nearest value feasible by hardware. The unaliased bandwidth
  @a sw may be adapted accordingly.


  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The following assumptions or range checks of the arguments are made:
    - @a sw:    Unaliased bandwidth. @c 0 (@c NULL) pointer is not allowed.
    - @a np:    Aliased number of points (sw*alias). @c 0 (@c NULL) pointer is not
                allowed, it's value must be larger or equal to @c 1.
    - @a alias: Anti-alias factor: the FoV extension in read direction. Its value must be at least 1.0.
                1.0. @c 0 (@c NULL) pointer is allowed and defaults to a value of @c 1.0.
    - @a acqdur: The acqusition duration. @c 0 or (@c NULL) pointer allowed.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  MRT_UpdateDigPars(&PVM_EffSWh,
                     PVM_EncMatrix[0],
                    &PVM_AntiAlias[0],
                    &PVM_AcquisitionTime);
  @endcode

  @param[in,out] sw     Unaliased bandwidth.
  @param         np     Aliased number of complex datapoints. @a np data point are sampled with a
                        bandwidth of @a sw * @a alias.
  @param[in,out] alias  Anti aliasing or read extension factor.
  @param[out]    acqdur Calculated acquisition duration [ms].

  @see @ref MRT_SpecUpdateDigPars

  @ingroup PVM_MRT
*/
DLL_INTERFACE void MRT_UpdateDigPars( double* sw,          /* unaliased bw  */
                                      int     np,          /* unaliased size*/
                                      double* alias,       /* alias factor  */
                                      double* acqdur );    /* acquis. time  */


/** Derives aliased and unaliased matrix sizes.

  This function derives aliased and unaliased matrix sizes from (unaliased)
  bandwidth @a sw, acquisition duration and alias factor. Before calculation
  it checks if aliased bandwidth - @a sw*alias - is feasible by hardware and
  adapts it accordingly if necessary. It may also adapt unaliased bandwidth
  sw, alias factor @a alias or acquisisiton time @a acqdur to comply with the
  following conditions:

  @image html MRT_DigEquations.png

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The following assumptions or range checks of the arguments are made:
    - @a sw:     Unaliased bandwidth. @c 0 or @c NULL pointer is not allowed, its value must be positive.
    - @a np:     Unaliased number of (complex) points determined within this function. @c 0 (@c NULL)
                 pointer is allowed.
    - @a anp:    Aliased number of (complex) points determined within this function. @c 0 (@c NULL)
                 pointer is allowed.
    - @a alias:  Anti-alias factor: the FoV extension in read direction. Its value must be at least 1.0.
                 1.0. @c 0 (@c NULL) pointer is allowed and defaults to a value of @c 1.0.
    - @a acqdur: Acquisition duration. 0 (@c NULL) pointer is not allowed. Its value must be larger 0.0.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  MRT_SpecUpdateDigPars(&PVM_EffSWh,
                        &PVM_Matrix[0],
                        &PVM_EncMatrix[0]
                        &PVM_AntiAlias[0],
                        &PVM_AcquisitionTime);
  @endcode

  @param[in,out] sw     Unaliased band- or sweepwidth.
  @param[in,out] np     Unaliased number of complex datapoints
  @param[in,out] anp    Aliased number of complex datapoints. @a anp data point are sampled with a
                        bandwidth of @a sw * @a alias.
  @param[in,out] alias  Anti aliasing factor.
  @param[in,out] acqdur Duration of the acquisition interval (in ms).


  @see @ref MRT_UpdateDigPars

  @ingroup PVM_MRT
*/
DLL_INTERFACE void MRT_SpecUpdateDigPars( double* sw,      /* unaliased bw  */
                                          int*    np,      /* unaliased size*/
                                          int*    anp,     /* aliased size  */
                                          double* alias,   /* alias factor  */
                                          double* acqdur); /* acquis. time  */

//documentation needed
DLL_INTERFACE void MRT_GetDefaultRxFilterInfo ( RX_FILTER_INFO &info,
                                                const double& swh,
                                                const int scanSize,
                                                bool compensateGrpdly,
                                                RX_FILTER_SETTING* filterSetting = nullptr);  //cmei, test


/** Calculates the required wait time of the RX hardware filter

Group delay compensation requires a particular wait time between the end of an acquisition and a following receiver command (e.g. SWITCH_JOB, ACQ_INIT, etc.; note that the loop lable after the acquisition contains an implicit RG switching, too).
Note: The wait time is required starting from the actual end of the acquisition, independent from the position of the ADC_END command.
The wait time depends on acquisition bandwidth and filter selection.
This function returns the wait time for the filter that is selected automatically by the default policy for the given bandwidth, i.e. if the filter type is not changed explicitly.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  Final acquisition bandwidth has to be set before

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  double waittime = MRT_GetDefaultRxFilterWaitTime(PVM_EffSWh * PVM_AntiAlias[0], true);
  @endcode

  @param swh              Acquisition bandwidth (actual value, including AntiAliasing, etc.)
  @param compensateGrpdly compensate group delay? (true/false)

  @return wait time (\mus)

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_GetDefaultRxFilterWaitTime (const double& swh,
                                                bool compensateGrpdly);

/** Derives nearest bandwidth from a given value
  The parameter that is given in a string argument is set to the nearest bandwidth value.
  Using this function with string argument allows increasing/decreasing the bandwidth using the spinner
  since the search direction is automatically determined.

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  MRT_GetNearestBandwidthInHz("PVM_EffSWh",minmax);
  @endcode

  @param[in] swparn  parameter name as string containing currently chosen bandwidth (including aliasing, ramp sampling etc.) in [Hz]
  @param[in] minmax  2-element array containing minimum and maximum of allowed bandwidth
  @param[out]    void

  @see

  @ingroup PVM_MRT
*/
DLL_INTERFACE void MRT_GetNearestBandwidthInHz (const char * const swparn,
                                                const double minmax[2] = 0);


/** Derives nearest bandwidth from a given value

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  MRT_GetNearestBandwidthInHz(PVM_EffSWh,minmax);
  @endcode

  @param[in] sw  currently chosen bandwidth (including aliasing, ramp sampling etc.) in [Hz]
  @param[in] minmax  2-element array containing minimum and maximum of allowed bandwidth
  @param[in] searchDir specifies the search direction: -1: lower values, 0: nearest value, +1: higher values
  @param[out] new_sw   new bandwidth value

  @see

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_GetNearestBandwidthInHz (double sw,
                                                  const double minmax[2] = 0, int searchDir = 0);

/** Derives nearest bandwidth, ensuring a gridded readout duration

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  MRT_GetNearestBandwidthInHzOnGrid(&PVM_EffSWh,
                        PVM_Matrix[0],
                        1.0);
  @endcode

  @param[in] sw  currently chosen bandwidth (including aliasing, ramp sampling etc.) in [Hz]
  @param[in]     nPoints  actual number of complex datapoints (including aliasing, ramp sampling, etc.)
  @param[in]     grid     grid in [\mu s].
  @param[out]    new_sw   nearest bandwidth on grid

  @see

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_GetNearestBandwidthInHzOnGrid(double sw, int nPoints = 1, double grid = 1.0);


/**************************************************************************/
/*            gen/src/prg/pvm_toolboxes/PvMrTools/gradients.c             */
/**************************************************************************/
/** Calculates the gradient strength in Hz/mm.

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
  double gradStrength = MRT_AbsoluteGrad(ReadGrad, CFG_GradCalConst());
  @endcode

  @param grad            Gradient strength expressed as a percentage of the gradient
                         calibration constant
  @param gradCalConst    Gradient calibration constant (Hz/mm)

  @return Absolute gradient strength (Hz/mm)

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_AbsoluteGrad(double grad, double gradCalConst);




/** Calculates the frequency offset at a position.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref MRT_AbsoluteGrad

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @PVM_PRECOND
  NONE

  @PVM_EXAMPLE
  @code
  PVM_EffReadOffset[0] = MRT_FrequencyOffset(
                           PVM_ReadOffsetHz[0],
                           ReadGrad,
                           CFG_GradCalConst());
  @endcode

  @param offset          Offset from gradient center (mm)
  @param grad            Gradient strength expressed as a percentage of the gradient
                         calibration constant
  @param gradCalConst    Gradient calibration constant (Hz/mm)

  @return Frequency offset (Hz)

  @see @ref MRT_FrequencyOffsetList

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_FrequencyOffset(double offset,
                                         double grad,
                                         double gradCalConst);




/** Calculates a list of frequency offsets.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref MRT_FrequencyOffset

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  - @a nOffsets < @c 1
  - @a offsetList is a null pointer
  - @a offsetListHz is a null pointer

  @PVM_PRECOND
  - size of @a offsetList must be at least @a nOffsets
  - size of @a offsetListHz must be at least @a nOffsets

  @PVM_EXAMPLE
  @code
  int nslices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);

  MRT_FrequencyOffsetList(nslices,
                          PVM_EffReadOffset,
                          ReadGrad,
                          CFG_GradCalConst(),
                          PVM_ReadOffsetHz);
  @endcode

  @param      nOffsets        Number of frequency offsets to be calculated
  @param[in]  offsetList      List of offsets (mm)
  @param      grad            Gradient strength expressed as a percentage of the gradient
                              calibration constant
  @param      gradCalConst    Gradient calibration constant (Hz/mm)
  @param[out] offsetListHz    List of calculated frequency offsets

  @see MRT_FrequencyOffset

  @ingroup PVM_MRT
*/
DLL_INTERFACE void MRT_FrequencyOffsetList(int    nOffsets,
                                           double *offsetList,
                                           double grad,
                                           double gradCalConst,
                                           double *offsetListHz);




/** Calculates the normalized duration of a gradient pulse.

  This is defined as the duration of a pulse assuming that the ramps at both ends are
  instantaneous.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_EXAMPLE
  Calculate the normalised integral of the read dephase gradient
  @code
  double readDephInteg = MRT_NormGradPulseTime(PVM_ReadDephaseTime,
                                               PVM_ReadDephaseRampUpTime,
                                               PVM_ReadDephaseRampUpIntegral,
                                               PVM_ReadDephaseRampDownTime,
                                               PVM_ReadDephaseRampDownIntegral);
  @endcode

  @param gradPulseTime      Duration of a phase encoding gradient pulse including the duration
                            of both gradient ramps (ms)
  @param rampOnTime         Duration of the leading gradient ramp (ms)
  @param rampOnIntegral     Normalised integral of a ramp on
  @param rampOffTime        Duration of the trailing gradient ramp (ms)
  @param rampOffIntegral    Normalised integral of a ramp off

  @return Normalized duration of a gradient pulse

  @see @ref MRT_NormGradRampTime, @ref MRT_NormPhaseEncTime

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_NormGradPulseTime(double gradPulseTime,
                                           double rampOnTime,
                                           double rampOnIntegral,
                                           double rampOffTime,
                                           double rampOffIntegral);





/** Calculates the normalized duration of a gradient ramp.

  This is defined as the duration of the gradient assuming an instantaneous ramp.

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
  Calculate the normalised integral of the rising ramp of the read gradient
  preceding data collection
  @code
  double readRampInteg = MRT_NormGradRampTime(PVM_ReadRampUpTime,
                                              PVM_ReadRampUpIntegral);
  @endcode

  @param rampTime        Duration of the gradient ramp (ms)
  @param rampIntegral    Normalised integral of the ramp

  @return Normalized duration of the gradient (ms)

  @see @ref MRT_NormGradPulseTime, @ref MRT_NormPhaseEncTime

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_NormGradRampTime(double rampTime,
                                          double rampIntegral);




/** Calculates the amplitude of a flow encoding gradient pulse

    Calculates the amplitude of a bipolar gradient pulse that produces a given phase shift
    for a given velocity of flow along the gradient.
    It is assumed that the two gradient lobes have identical
    durations and oposite amplitudes, and that all ramp times are the same.

  @image html MRT_FlowEncGradient.png

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
  FlowEncGradientAmp = MRT_FlowEncGradient(
    EncTime,
    FlowRange,
    M_PI,
    CFG_GradientRampTime(),
    CFG_InterGradientWaitTime(),
    CFG_GradCalConst(PVM_Nucleus1));
  @endcode

  @param encDur       Duration of flow encoding [ms]
  @param velocity     Flow velocity [cm/s]
  @param phaseShift   Phase shift for this valocity [rad]
  @param ramp         Gradient ramp duration [ms]
  @param spacing      Time between lobes (zero for adjacent lobes) [ms]
  @param gradCalConst Maximum available gradient [Hz/mm]

  @return Gradient amplitude (positive lobe) in % of maximum.

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_FlowEncGradient(double encDur,
                                         double velocity,
                                         double phaseShift,
                                         double ramp,
                                         double spacing,
                                         double gradCalConst);




/** Calculates the amplitude of a dephasing gradient pulse.

  Finds the amplitude of a compensation pulse of a given effective duration (@a dephDur) that
  has the same integral as a gradient pulse that is switched on with a given effective ramp
  time (@a riseTime) to a given amplitude (@a gradOnAmp) and stays on for a given duration
  (@a gradOnTime). The result is given in the same units as @a gradOnTime. The function can be
  used to calculate readout gradient dephasing or slice gradient rephasing.

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
  ReadDephGrad = MRT_DephaseGrad(EncGradDur, PVM_AcquisitionTime * 0.5, CFG_GradientRiseTime(), ReadGrad);
  @endcode

  @param dephDur       Duration of the dephasing gradient pulse (ms)
  @param gradOnTime    Duration of the gradient to be compensated (ms)
  @param riseTime      Effective duration of the ramp leading from zero to the gradient that
                       has to be compensated (ms)
  @param gradOnAmp     Amplitude of gradient to be compensated (a.u.)

  @return Amplitude of dephasing gradient (units of @a gradOnAmp)

  @see @ref MRT_DephaseTime

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_DephaseGrad(double dephDur,
                                     double gradOnTime,
                                     double riseTime,
                                     double gradOnAmp);




/** Minimum dephasing time.

  Finds effective duration of a compensation pulse of a given amplitude (@a dephGrad) that has
  the same integral as a gradient pulse that is switched on with a given effective ramp time
  (@a riseTime) to a given amplitude (@a gradOnAmp) and stays on for a given duration
  (@a gradOnTime). Both grad-arguments can be given in arbitrary (but identical) units. May be
  used for read and slice gradient calculations.

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
  double minDephRead  = MRT_DephaseTime(PVM_AcquisitionTime * 0.5,
                                        CFG_GradientRiseTime(),
                                        ReadGrad,
                                        ReadDephGradLim);
  @endcode

  @param gradOnTime    Duration of the gradient to be compensated (ms)
  @param riseTime      Effective duration of the ramp leading from zero to the gradient that
                       has to be compensated (ms)
  @param gradOnAmp     Amplitude of the gradient to be compensated (a.u.)
  @param dephGrad      Amplitude of the compensating gradient (a.u.)

  @return Mimimum effective duration of the dephasing gradient pulse (ms)

  @see @ref MRT_DephaseGrad

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_DephaseTime(double gradOnTime,
                       double riseTime,
                       double gradOnAmp,
                       double dephGrad);




/** Duration of encoding gradient pulse.

  Calculates the effective duration (in ms) of a phase encoding pulse with given amplitude
  (@a gradAmp, Hz/mm) to reach a given resolution (@a resol, mm).

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
  Calculate minimum possible encoding time
  @code
  double minEncTime = MRT_EncodingTime(PVM_SpatResol[1], CFG_GradCalConst());
  @endcode

  @param resol    Spatial resolution [mm]
  @param gradAmp  Amplitude of phase encoding gradient [Hz/mm]

  @return Effective phase encoding duration [ms]

  @see @ref MRT_MaxEncodingTime

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_EncodingTime(double resol,
                                      double gradAmp);




/** Maximum encoding pulse duration.

  Calculates maximum duration of the encoding gradient that will not lead to gradient resolution
  problems.

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
  double maxEncTime = MRT_MaxEncodingTime(PVM_Fov[1], CFG_GradCalConst());
  @endcode

  @param fov           Field-of-view (mm)
  @param maxGradAmp    Maximum available gradient (Hz/mm)

  @return Maximum duration (ms)

  @see @ref MRT_EncodingTime

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_MaxEncodingTime(double fov,
                           double maxGradAmp);




/** Initializes and checks a gradient spoiler.

  Function to be used in the method initialization part. All members of the struct are
  initialized with default values or with the specified values given by arguments of this
  function.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Invalid parameter type of @a spoilerName.

  @PVM_EXAMPLE
  @code
  MRT_InitSpoiler("ReadSpoiler");
  @endcode

  @param spoilerName  Name of the spoiler parameter given as a string
  @param automatic    (optional) @c YesNo option for automatic setting of the spoiler attributes
                      (default = @c Yes)
  @param spoil        (optional) Defines the spoiling efficiency in cycles/image_element
                      (e.g. pixel in case of a read spoiler, or slice thickness in case of a slice spoiler)
                      (default = @c 1.0)
  @param dur          (optional) Duration of the spoiler
                      (default = @c 1.0)
  @param ampl         (optional) Amplitude of the spoiler
                      (default = @c 10.0)

  @see @ref MRT_UpdateSpoiler

  @ingroup PVM_MRT
*/
DLL_INTERFACE void MRT_InitSpoiler(const char *const spoilerName,
                                   YesNo             automatic = Yes,
                                   double            spoil     = 1.0,
                                   double            dur       = 1.0,
                                   double            ampl      = 10.0);




/** Updates a gradient spoiler.

  Function to be used in the @c backbone function to update a gradient spoiler.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Argument 1 - Invalid parameter type

  @PVM_EXAMPLE
  @code
  double minDurRead = MAX_OF(EncGradDur + CFG_GradientRiseTime(), PVM_DigEndDelOpt);
  MRT_UpdateSpoiler("ReadSpoiler", 1.0, Phase2DGradLim, minDurRead, CFG_GradCalConst(), PVM_SpatResol[0]);
  @endcode

  @param[in] spoilerName     Name of the spoiler parameter given as a string
  @param     spoiling        Desired spoiling efficiency in cycles/image_element (e.g. pixel in
                             case of a read spoiler, or slice thickness in case of a slice spoiler)
                             which should be realized in the automatic mode
  @param     maxgrad         Defines the maximam allowed gradient amplitude
  @param     mindur          Defines the minimum allowed duration
  @param     gradCalConst    Gradient calibration constant (Hz/mm)
  @param     resolution      Scale basis (image_element) for the spoiling efficiency (e.g. should
                             be image resolution for a read spoiler, or slice thickness for a slice
                             spoiler)

  @see @ref MRT_InitSpoiler

  @ingroup PVM_MRT
*/
DLL_INTERFACE void MRT_UpdateSpoiler(const char *const spoilerName,
                                     double            spoiling,
                                     double            maxgrad,
                                     double            mindur,
                                     double            gradCalConst,
                                     double            resolution);




/**************************************************************************/
/*           gen/src/prg/pvm_toolboxes/PvMrTools/phaseGradients.c         */
/**************************************************************************/
/** Calculates the effective phase encoding time.

  This is defined as the duration of a phase encoding gradient assuming that the ramps at both
  ends of the pulse are instantaneous.

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

  @param phaseEncTime       Duration of a phase encoding gradient pulse including the duration
                            of both gradient ramps (ms)
  @param rampOnTime         Duration of the leading gradient ramp (ms)
  @param rampOnIntegral     Normalised integral of a ramp on
  @param rampOffTime        Duration of the trailing gradient ramp (ms)
  @param rampOffIntegral    Normalised integral of a ramp off

  @return Normalized duration of a phase encoding gradient

  @see @ref MRT_NormGradRampTime, @ref MRT_NormGradPulseTime

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_NormPhaseEncTime(double phaseEncTime,
                                          double rampOnTime,
                                          double rampOnIntegral,
                                          double rampOffTime,
                                          double rampOffIntegral);




/** Calculates the field-of-view in a phase encoding dimension.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref MRT_Fov

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  Calculate minimum FOV in first phase dimension
  @code
  double minFov = MRT_PhaseFov(effEncGradDur,
                               PVM_EncMatrix[1],
                               100.0,
                               CFG_GradCalConst());
  @endcode

  @param normPhaseEncTime    Normalized duration of the phase encoding gradient assuming
                             instantaneous ramps (ms)
  @param acqSize             Number of data points to be acquired
  @param phaseGrad           Phase encoding gradient strength expressed as a percentage of the
                             gradient calibration constant
  @param gradCalConst        Gradient calibration constant (Hz/mm)

  @see @ref MRT_ReadFov, @ref MRT_MinReadFov,
       @ref MRT_SliceThickness, @ref MRT_MinSliceThickness

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_PhaseFov(double normPhaseEncTime,
                                  int    acqSize,
                                  double phaseGrad,
                                  double gradCalConst);




/** Calculates the phase encoding gradient strength.

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
  Phase2DGrad = MRT_PhaseGrad(EncGradDur, PVM_Matrix[1], PVM_Fov[1], CFG_GradCalConst());
  @endcode

  @param normPhaseEncTime    Normalized duration of the phase encoding gradient assuming
                             instantaneous ramps (ms)
  @param acqSize             Number of data points to be acquired
  @param phaseFov            Field-of-view in the phase encoding gradient axis (mm)
  @param gradCalConst        Gradient calibration constant (Hz/mm)

  @return Phase encoding gradient strength

  @see @ref MRT_MaxReadGrad, @ref MRT_ReadGradRatio, @ref MRT_ReadGrad,
       @ref MRT_ReadDephaseGrad, @ref MRT_MaxSliceGrad,
       @ref MRT_SliceGradRatio, @ref MRT_SliceGrad, @ref MRT_SliceRephaseGrad

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_PhaseGrad(double normPhaseEncTime,
                                   int    acqSize,
                                   double phaseFov,
                                   double gradCalConst);




/** Calculates the phase encoding gradient steps for first order flow compensation.

  The strength of the phase encoding gradient steps is calculated from the corresponding phase
  gradient integral in the case of first order flow compensation with two ramp times between
  gradient steps.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Length of @a phastrim must be at least 2.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  double gradVal[2];
  double ramp = CFG_GradientRampTime() + CFG_InterGradientWaitTime();
  double lobe = (EncGradDur - ramp) / 2;
  double phaseIntegral = 1.0e5 / (2.0 * CFG_GradCalConst() * PVM_SpatResol[1]);

  MRT_PhaseFlowComp(ramp, lobe, lobe, phaseIntegral, gradVal);
  @endcode

  @param      ramp        Ramp time [ms]
  @param      da          Duration of the first flow compensation point [ms]
  @param      db          Duration of the second flow compensation point [ms]
  @param      phaseint    Phase gradient integral [% * ms]
  @param[out] phastrim    Gradient amplitudes of the gradient points [%]

  @see @ref MRT_ReadFlowComp, @ref MRT_ShortReadFlowComp,
       @ref MRT_ShortPhaseFlowComp, @ref MRT_SliceFlowComp, @ref MRT_ShortSliceFlowComp

  @ingroup PVM_MRT
*/
DLL_INTERFACE void MRT_PhaseFlowComp(double ramp,
                                     double da,
                                     double db,
                                     double phaseint,
                                     double *phastrim);




/** Calculates the phase encoding gradient steps for first order flow compensation.

  The strength of the phase encoding gradient steps is calculated from the corresponding phase
  gradient integral in the case of first order flow compensation with one ramp time between
  gradient steps.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Length of @a phastrim must be at least 2.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  double gradVal[2];
  double ramp = CFG_GradientRampTime() + CFG_InterGradientWaitTime();
  double lobe = (EncGradDur - ramp) / 2;
  double phaseIntegral = 1.0e5 / (2.0 * CFG_GradCalConst() * PVM_SpatResol[1]);

  MRT_ShortPhaseFlowComp(ramp, lobe, lobe, phaseIntegral, gradVal);
  @endcode

  @param      ramp        Ramp time [ms]
  @param      da          Duration of the first flow compensation point [ms]
  @param      db          Duration of the second flow compensation point [ms]
  @param      phaseint    Phase gradient integral [% * ms]
  @param[out] phastrim    Amplitudes of the gradient points [%]

  @see @ref MRT_ReadFlowComp, @ref MRT_ShortReadFlowComp, @ref MRT_PhaseFlowComp,
       @ref MRT_SliceFlowComp, @ref MRT_ShortSliceFlowComp

  @ingroup PVM_MRT
*/
DLL_INTERFACE void MRT_ShortPhaseFlowComp(double ramp,
                                          double da,
                                          double db,
                                          double phaseint,
                                          double *phastrim);




/**************************************************************************/
/*         gen/src/prg/pvm_toolboxes/PvMrTools/readGradients.c            */
/**************************************************************************/

/** Calculates the maximum read gradient strength.

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
  double maxReadGrad = MRT_MaxReadGrad(90.0, 50.0, 1.0);
  @endcode

  @param limitingReadGrad       Limiting gradient strength that can be used for frequency
                                encoding expressed as percentage of the gradient calibration
                                constant. This parameter is required in order to impose a an
                                upper limit on the read gradient that is less than 100%, and is
                                a prerequisite for arbitary oblique imaging.
  @param limitingDephaseGrad    Limiting gradient strength that can be used for dephasing prior
                                to frequency encoding expressed as percentage of the gradient
                                calibration constant. This parameter is required in order to
                                impose a an upper limit on the read dephase gradient that is
                                less than 100%, and is a prerequisite for arbitary oblique
                                imaging.
  @param readGradRatio          Ratio of the strengths of the read gradient to read dephase
                                gradient

  @return Maximum read gradient strength

  @see @ref MRT_ReadGradRatio, @ref MRT_ReadGrad,
       @ref MRT_ReadDephaseGrad, @ref MRT_PhaseGrad, @ref MRT_MaxSliceGrad,
       @ref MRT_SliceGradRatio, @ref MRT_SliceGrad, @ref MRT_SliceRephaseGrad

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_MaxReadGrad(double limitingReadGrad,
                                     double limitingDephaseGrad,
                                     double readGradRatio);




/** Calculates the ratio of read gradient to read dephase gradient required to position an echo
    within the acquisition window correctly.

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
  @endcode

  @param acqTime               Duration of the acquisition window (ms)
  @param echoPosition          Position of the echo in the acquisition window expressed as a
                               percentage of the window from the window start
  @param acqStartDelay         Delay between the end of the read gradient ramp preceeding
                               acquisition and acquisition start (ms)
  @param normDephaseTime       Normalized duration of the dephasing gradient (ms)
  @param normReadRampUpTime    Normalized duration of the rising ramp ahead of sampling window
                               (ms)

  @return Read gradient ratio

  @see @ref MRT_MaxReadGrad, @ref MRT_ReadGrad,
       @ref MRT_ReadDephaseGrad, @ref MRT_PhaseGrad, @ref MRT_MaxSliceGrad,
       @ref MRT_SliceGradRatio, @ref MRT_SliceGrad, @ref MRT_SliceRephaseGrad

  @todo Incomplete documentation: PVM_EXAMPLE

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_ReadGradRatio(double acqTime,
                                       double echoPosition,
                                       double acqStartDelay,
                                       double normDephaseTime,
                                       double normReadRampUpTime);




/** Calculates the read gradient strength.

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
  ReadGrad = MRT_ReadGrad(PVM_EffSWh,
                          PVM_Fov[0],
                          CFG_GradCalConst());
  @endcode

  @param sweepWidthHz    Spectral width of the data acquisition (Hz)
  @param readFov         Field-of-view in the read gradient axis (mm)
  @param gradCalConst    Gradient calibration constant (Hz/mm)

  @return Read gradient strength as percentage of the @a gradCalConst

  @see @ref MRT_MaxReadGrad, @ref MRT_ReadGradRatio,
       @ref MRT_ReadDephaseGrad, @ref MRT_PhaseGrad, @ref MRT_MaxSliceGrad,
       @ref MRT_SliceGradRatio, @ref MRT_SliceGrad, @ref MRT_SliceRephaseGrad

  @ingroup PVM_MRT
 */
DLL_INTERFACE double MRT_ReadGrad(double sweepWidthHz,
                                  double readFov,
                                  double gradCalConst);




/** Calculates the read dephase gradient strength.

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
  ReadDephGrad = MRT_ReadDephaseGrad(readGradRatio, ReadGrad);
  @endcode

  @param readGradRatio    Ratio of the strengths of the read gradient to read dephase gradient
  @param readGrad         Read gradient strength expressed as a percentage of the gradient
                          calibration constant

  @return Read dephase gradient strength as percentage of gradient calibration constant

  @see @ref MRT_MaxReadGrad, @ref MRT_ReadGradRatio, @ref MRT_ReadGrad,
       @ref MRT_PhaseGrad, @ref MRT_MaxSliceGrad,
       @ref MRT_SliceGradRatio, @ref MRT_SliceGrad, @ref MRT_SliceRephaseGrad

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_ReadDephaseGrad(double readGradRatio,
                                         double readGrad);




/** Calculates the field-of-view in the read axis.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref MRT_Fov

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  Calculate minimum FOV
  @code
  double minFov = MRT_ReadFov(PVM_EffSWh,
                              100.0,
                              CFG_GradCalConst());
  @endcode

  @param sweepWidthHz    Spectral width of the data acquisition (Hz)
  @param readGrad        Read gradient strength expressed as a percentage of the gradient
                         calibration constant
  @param gradCalConst    Gradient calibration constant (Hz/mm)

  @return Field-of-view in the read gradient axis (mm)

  @see @ref MRT_MinReadFov, @ref MRT_PhaseFov,
       @ref MRT_SliceThickness, @ref MRT_MinSliceThickness

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_ReadFov(double sweepWidthHz,
                                 double readGrad,
                                 double gradCalConst);




/** Calculates the minimum field-of-view.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref MRT_MaxReadGrad
  - @ref MRT_ReadFov

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  double minFovRead = MRT_MinReadFov(PVM_EffSWh,
                                     readGradRatio,
                                     ReadGradLim,
                                     ReadDephGradLim,
                                     CFG_GradCalConst());
  @endcode

  @param sweepWidthHz           Spectral width of the data acquisition (Hz)
  @param readGradRatio          Ratio of the strengths of the read gradient to read dephase
                                gradient
  @param limitingReadGrad       Limiting gradient strength that can be used for frequency
                                encoding expressed as percentage of the gradient calibration
                                constant. This parameter is required in order to impose a an
                                upper limit on the read gradient that is less than 100%, and is
                                a prerequisite for arbitary oblique imaging.
  @param limitingDephaseGrad    Limiting gradient strength that can be used for dephasing prior
                                to frequency encoding expressed as percentage of the gradient
                                calibration constant. This parameter is required in order to
                                impose a an upper limit on the read dephase gradient that is
                                less than 100%, and is a prerequisite for arbitary oblique
                                imaging.
  @param gradCalConst           Gradient calibration constant (Hz/mm)

  @return Minimum field-of-view (mm)

  @see @ref MRT_ReadFov, @ref MRT_PhaseFov,
       @ref MRT_SliceThickness, @ref MRT_MinSliceThickness

  @ingroup PVM_MRT
 */
DLL_INTERFACE double MRT_MinReadFov(double sweepWidthHz,
                                    double readGradRatio,
                                    double limitingReadGrad,
                                    double limitingDephaseGrad,
                                    double gradCalConst);




/** Calculates the read gradient steps for first order flow compensation.

  The strength of the two dephasing gradient steps is calculated from the corresponding read
  gradient amplitude (third step) in the case of first order flow compensation with two ramp
  times between gradient steps.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Length of @a phastrim must be at least 2.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  double ramp = CFG_GradientRampTime() + CFG_InterGradientWaitTime();
  double lobe = (EncGradDur - ramp) / 2;

  MRT_ReadFlowComp(ramp,
                   lobe,
                   lobe,
                   PVM_AcquisitionTime,
                   PVM_EchoPosition,
                   CFG_GradientDelayTime(),
                   ReadGrad,
                   ReadFcGrad);
  @endcode

  @param      ramp        Ramp time [ms]
  @param      da          Duration of the first flow compensation step [ms]
  @param      db          Duration of the second flow compensation step [ms]
  @param      acq         Acquisition time [ms]
  @param      echopos     Echo position
  @param      dgh         Shift between ramp and gradient curent [ms]
  @param      readamp     Read gradient amplitude [a.u.]
  @param[out] readtrim    Amplitudes of the gradient points [a.u.]

  @see @ref MRT_ShortReadFlowComp, @ref MRT_PhaseFlowComp,
       @ref MRT_ShortPhaseFlowComp, @ref MRT_SliceFlowComp, @ref MRT_ShortSliceFlowComp

  @ingroup PVM_MRT
*/
DLL_INTERFACE void MRT_ReadFlowComp(double ramp,
                                    double da,
                                    double db,
                                    double acq,
                                    double echopos,
                                    double dgh,
                                    double readamp,
                                    double *readtrim);




/** Calculates the read gradient steps for first order flow compensation.

  The strength of the two dephasing gradient steps is calculated from the corresponding read
  gradient amplitude (third step) in the case of first order flow compensation with one ramp
  time between gradient steps.

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
  double ramp = CFG_GradientRampTime() + CFG_InterGradientWaitTime();
  double lobe = (EncGradDur - ramp) / 2;

  MRT_ShortReadFlowComp(ramp,
                       lobe,
                       lobe,
                       PVM_AcquisitionTime,
                       PVM_EchoPosition,
                       CFG_GradientDelayTime(),
                       ReadGrad,
                       ReadFcGrad);
  @endcode

  @param ramp        Ramp time
  @param da          Duration of the first flow compensation step
  @param db          Duration of the second flow compensation step
  @param acq         Acquisition time
  @param echopos     Echo position
  @param dgh         Shift between ramp and gradient curent
  @param readamp     Read gradient amplitude
  @param readtrim    Amplitudes of the gradient points

  @see @ref MRT_ReadFlowComp, @ref MRT_PhaseFlowComp,
       @ref MRT_ShortPhaseFlowComp, @ref MRT_SliceFlowComp, @ref MRT_ShortSliceFlowComp

  @ingroup PVM_MRT
*/
DLL_INTERFACE void MRT_ShortReadFlowComp(double ramp,
                                         double da,
                                         double db,
                                         double acq,
                                         double echopos,
                                         double dgh,
                                         double readamp,
                                         double *readtrim);




/**************************************************************************/
/*          gen/src/prg/pvm_toolboxes/PvMrTools/sliceGradients.c          */
/**************************************************************************/
/** Calculates the maximum gradient strength that can be used for slice selection.

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
  double maxSliceGrad = MRT_MaxSliceGrad(90.0, 90.0, 1.0);
  @endcode

  @param limitingSliceGrad      Limiting gradient strength that can be used for the slice
                                selection expressed as a percentage of the gradient calibration
                                constant. This parameter is required in order to impose an
                                upper limit on the slice gradient that is less than 100%, and it
                                is a prerequisite for arbitary oblique imaging.
  @param limitingRephaseGrad    Limiting gradient strength that can be used for the slice
                                selection rephasing expressed as a percentage of the gradient
                                calibration constant. This parameter is required in order to
                                impose an upper limit on the slice rephasing gradient that is
                                less than 100%, and it is a prerequisite for arbitary oblique
                                imaging.
  @param sliceGradRatio         Ratio of the strengths of the slice rephase gradient to slice
                                gradient. May be set to @c 0.0 when no rephasing is needed;
                                @a limitingRephaseGrad is then irrelevant.

  @return Maximum gradient strength expressed as a percentage of the gradient calibration
          constant.

  @see @ref MRT_MaxReadGrad, @ref MRT_ReadGradRatio, @ref MRT_ReadGrad,
       @ref MRT_ReadDephaseGrad, @ref MRT_PhaseGrad,
       @ref MRT_SliceGradRatio, @ref MRT_SliceGrad, @ref MRT_SliceRephaseGrad

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_MaxSliceGrad(double limitingSliceGrad,
                                      double limitingRephaseGrad,
                                      double sliceGradRatio);




/** Calculates the ratio of the strengths of the slice rephase gradient to the slice selection
    gradient that is required to achieve optimal slice rephasing.

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
  double sliceRampInteg = 0.5 *  PVM_RiseTime;
  double sliceRephInteg = RephaseTime - PVM_RiseTime;

  SliceGradRatio = MRT_SliceGradRatio( ExcPul.Length,
                                       ExcPul.Rpfac,
                                       0.0,
                                       sliceRampInteg,
                                       sliceRephInteg);
  @endcode

  @param pulseLength              Duration of the RF-pulse used for slice excitation (ms)
  @param rephaseFactor            Rephase factor for slice rephasing expressed as a percentage
                                  of the duration the RF-pulse length used for slice excitation.
  @param sliceRampDownDelay       Delay time between the end of the slice selection RF-pulse
                                  and the start of the gradient ramp down (ms)
  @param normSliceRampDownTime    Normalized duration of the descending ramp trailing the slice
                                  selection pulse (ms)
  @param normRephaseTime          Normalized duration of the slice rephase gradient (ms)

  @return Slice gradient ratio

  @see @ref MRT_MaxReadGrad, @ref MRT_ReadGradRatio, @ref MRT_ReadGrad,
       @ref MRT_ReadDephaseGrad, @ref MRT_PhaseGrad, @ref MRT_MaxSliceGrad,
       @ref MRT_SliceGrad, @ref MRT_SliceRephaseGrad

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_SliceGradRatio(double pulseLength,
                                        double rephaseFactor,
                                        double sliceRampDownDelay,
                                        double normSliceRampDownTime,
                                        double normRephaseTime);




/** Calculates the strength of the slice gradient.

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
  ExcSliceGrad = MRT_SliceGrad(ExcPulse1.Bandwidth,
                               PVM_SliceThick,
                               CFG_GradCalConst());
  @endcode

  @param sliceThickHz    Thickness of slice (Hz)
  @param sliceThick      Thickness of slice (mm)
  @param gradCalConst    Gradient calibration constant (Hz/mm)

  @return  Slice gradient strength expressed as a percentage of the gradient calibration
           constant

  @see @ref MRT_MaxReadGrad, @ref MRT_ReadGradRatio, @ref MRT_ReadGrad,
       @ref MRT_ReadDephaseGrad, @ref MRT_PhaseGrad, @ref MRT_MaxSliceGrad,
       @ref MRT_SliceGradRatio, @ref MRT_SliceRephaseGrad

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_SliceGrad(double sliceThickHz,
                                   double sliceThick,
                                   double gradCalConst);




/** Calculates the strength of the slice rephase gradient.

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
  ExSliceRephaseGradient = MRT_SliceRephaseGrad(SliceGradRatio, ExSliceGradient);
  @endcode

  @param sliceGradRatio    Ratio of the strength of the slice rephase gradient to the strength
                           of the slice gradient
  @param sliceGrad         Slice gradient strength expressed as a percentage of the gradient
                           calibration constant

  @return Rephase gradient strength expressed as a percentage of the gradient calibration
          constant

  @see @ref MRT_MaxReadGrad, @ref MRT_ReadGradRatio, @ref MRT_ReadGrad,
       @ref MRT_ReadDephaseGrad, @ref MRT_PhaseGrad, @ref MRT_MaxSliceGrad,
       @ref MRT_SliceGradRatio, @ref MRT_SliceGrad

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_SliceRephaseGrad(double sliceGradRatio,
                                          double sliceGrad);




/** Calculates the thickness of a slice.

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
  double minThick = MRT_SliceThickness(PVM_FovSatPul.Bandwidth,
                                       100.0,
                                       CFG_GradCalConst(nucStr));
  @endcode

  @param sliceThickHz    Thickness of slice (Hz)
  @param sliceGrad       Slice gradient strength expressed as a percentage of the gradient
                         calibration constant
  @param gradCalConst    Gradient calibration constant (Hz/mm)

  @return Thickness of the slice (mm)

  @see @ref MRT_ReadFov, @ref MRT_MinReadFov, @ref MRT_PhaseFov,
       @ref MRT_MinSliceThickness

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_SliceThickness(double sliceThickHz,
                                        double sliceGrad,
                                        double gradCalConst);




/** Calculates the minimum slice thickness.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref MRT_MaxSliceGrad
  - @ref MRT_SliceThickness

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  double minThickness = MRT_MinSliceThickness(ExcPul.Bandwidth,
                                              SliceGradRatio,
                                              LimExSliceGradient,
                                              LimExSliceRephaseGradient,
                                              CFG_GradCalConst());
  @endcode

  @param sliceThickHz           Thickness of the slice (Hz)
  @param sliceGradRatio         Ratio of the strengths of slice rephase gradient to slice
                                gradient. May be set to @c 0.0 when no rephasing is used;
                                @a limitingRephaseGrad is then irrelevant.
  @param limitingSliceGrad      Limiting gradient strength that can be used for the slice
                                selection expressed as a percentage of the gradient calibration
                                constant. This parameter is required in order to impose an
                                upper limit on the slice gradient that is less than 100%, and
                                it is a prerequisite for arbitary oblique imaging.
  @param limitingRephaseGrad    Limiting gradient strength that can be used for the slice
                                selection rephasing expressed as a percentage of the gradient
                                calibration constant. This parameter is required in order to
                                impose an upper limit on the slice rephasing gradient that is
                                less than 100%, and it is a prerequisite for arbitary oblique
                                imaging.
  @param gradCalConst           Gradient calibration constant (Hz/mm)

  @return Minimum slice thickness (mm)

  @see @ref MRT_ReadFov, @ref MRT_MinReadFov, @ref MRT_PhaseFov,
       @ref MRT_SliceThickness

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_MinSliceThickness(double sliceThickHz,
                                           double sliceGradRatio,
                                           double limitingSliceGrad,
                                           double limitingRephaseGrad,
                                           double gradCalConst);




/** Calculates the slice gradient steps for first order flow compensation.

  The strength of the two rephasing gradient steps is calculated from the corresponding slice
  gradient amplitude (first step) in the case of first order flow compensation with two ramp
  times between gradient steps.

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
  double ramp = CFG_GradientRampTime() + CFG_InterGradientWaitTime();
  double lobe = (EncGradDur - ramp) / 2;

  MRT_SliceFlowComp(ramp,
                    EffPulseDur,
                    lobe,
                    lobe,
                    CFG_GradientDelayTime(),
                    ExcSliceGrad,
                    ExcSliceFcGrad);
  @endcode

  @param      ramp         Ramp time [ms]
  @param      pulse        Part of the RF-pulse necessary to be rephased [ms]
  @param      da           Duration of the first flow compensation point [ms]
  @param      db           Duration of the second flow compensation point [ms]
  @param      dgh          Shift between ramp and gradient curent [ms]
  @param      sliceamp     Slice gradient amplitude [a.u.]
  @param[out] slicetrim    Amplitudes of the gradient points [a.u.]

  @see @ref MRT_ReadFlowComp, @ref MRT_ShortReadFlowComp, @ref MRT_PhaseFlowComp,
       @ref MRT_ShortPhaseFlowComp, @ref MRT_ShortSliceFlowComp

  @ingroup PVM_MRT
*/
DLL_INTERFACE void MRT_SliceFlowComp(double ramp,
                                     double pulse,
                                     double da,
                                     double db,
                                     double dgh,
                                     double sliceamp,
                                     double *slicetrim);




/** Calculates the slice gradient steps for first order flow compensation.

  The strength of the two rephasing gradient steps is calculated from the corresponding slice
  gradient amplitude (first step) in the case of first order flow compensation with one ramp
  time between gradient steps.

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
  double ramp = CFG_GradientRampTime() + CFG_InterGradientWaitTime();
  double lobe = (EncGradDur - ramp) / 2;

  MRT_ShortSliceFlowComp(ramp,
                         EffPulseDur,
                         lobe,
                         lobe,
                         CFG_GradientDelayTime(),
                         ExcSliceGrad,
                         ExcSliceFcGrad);
  @endcode

  @param      ramp         Ramp time [ms]
  @param      pulse        Part of the RF-pulse necessary to be rephased [ms]
  @param      da           Duration of the first flow compensation point [ms]
  @param      db           Duration of the second flow compensation point [ms]
  @param      dgh          Shift between ramp and gradient curent [ms]
  @param      sliceamp     Slice gradient amplitude [a.u.]
  @param[out] slicetrim    Amplitudes of the gradient points [a.u.]

  @see @ref MRT_ReadFlowComp, @ref MRT_ShortReadFlowComp, @ref MRT_PhaseFlowComp,
       @ref MRT_ShortPhaseFlowComp, @ref MRT_SliceFlowComp

  @ingroup PVM_MRT
*/
DLL_INTERFACE void MRT_ShortSliceFlowComp(double ramp,
                            double pulse,
                            double da,
                            double db,
                            double dgh,
                            double sliceamp,
                            double *slicetrim);




/**************************************************************************/
/*           gen/src/prg/pvm_toolboxes/PvMrTools/MrtRfPulse.c             */
/**************************************************************************/
/** Rephase duration of an RF pulse.

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
  double rephaseDur = MRT_RFPulseRephDur(ExcPul);
  @endcode

  @param[in] pulse  Pointer to RF pulse structure

  @return Rephase duration [ms]

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_RFPulseRephDur(PVM_RF_PULSE_TYPE *const pulse);




/** Rephase duration of an RF pulse.

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
  double rephaseDur = MRT_RFPulseRephDur(ExcPul);
  @endcode

  @param[in] pulse  Pointer to RF pulse structure

  @return Rephase duration [ms]

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_RFPulseRephDur(PVM_RF_PULSE *const pulse);





/** Perform correction for assymetric RF pulse shape.

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
  MRT_RFPulseAssymCorr(&PVM_DwExcPulse1, &ca, &cb, 50.0);
  @endcode

  @param[in]  pulse  Pointer to RF pulse structure
  @param[out] ca     First correction delay [ms]
  @param[out] cb     Second correction delay [ms]
  @param      center [%]

  @return Rephase duration [ms]

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_RFPulseAssymCorr(PVM_RF_PULSE_TYPE *const pulse,
                                          double *const            ca,
                                          double *const            cb,
                                          double                   center);




/** Perform correction for assymetric RF pulse shape.

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
  MRT_RFPulseAssymCorr(&PVM_DwExcPulse1, &ca, &cb, 50.0);
  @endcode

  @param[in]  pulse  Pointer to RF pulse structure
  @param[out] ca     First correction delay [ms]
  @param[out] cb     Second correction delay [ms]
  @param      center [%]

  @return Rephase duration [ms]

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_RFPulseAssymCorr(PVM_RF_PULSE *const pulse,
                                          double *const       ca,
                                          double *const       cb,
                                          double              center);




/** Calculates the RF power in dB according to specified pulse properties.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Reference attenuation must be available.

  @PVM_ERROR
  - Illegal value of argument @a integralRatio
  - Illegal value of argument @a attenuationCorrection
  - Illegal value of argument @a pulseLength
  - Illegal value of argument @a referenceAttenuation
  - Illegal value of argument @a pulseAngleRequest

  @PVM_EXAMPLE
  @code
  double att = MRT_RFPulseAttenuation(ExcPulse.IntegralRatio,
                                      0.0,
                                      ExcPulse.Length,
                                      referenceAttenuation,
                                      30.0);
  @endcode

  @param integralRatio         Integral ratio of RF pulse
  @param attenuationCorrection Pulse-specific attenuation correction [dB]
  @param pulseLength           Length of RF pulse [ms]
  @param referenceAttenuation  Reference attenuation for 1 ms 90 deg block pulse [dB]
  @param pulseAngleRequest     Requested flip angle [deg]

  @return RF power in dB

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_RFPulseAttenuation(const double  integralRatio,
                                            const double  attenuationCorrection,
                                            const double  pulseLength,
                                            const double  referenceAttenuation,
                                            const double  pulseAngleRequest);




/** Calculates the RF pulse angle from amplitude (power in W) and specified arguments

  For a given reference power @a refpow (units W) shape peak power @a pow (units W)
  pulse duration (@a length units ms) pulse shape integral (@a shapeInt) the pulse angle
  (angle in degree) is calculated according to the following formula @n

  \f$ angle = \sqrt{\frac{pow}{refpow}}*length*shapeInt*90 \f$


  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  Arguments in legal range (see below)

  @PVM_ERROR
  - shape integral @a shapeInt not in interval ]0.0,1.0]
  - pulse amplitude @a pow less than 0.0W
  - pulse length @a length less or equal to 0.0ms
  - reference power @a refpow less or equal to 0.0W

  If argument @a errmsg is not a @c NULL pointer, the error condition is written
  into the string @a errmsg points to and 0.0 is returned. If @a errmsg is a @c NULL
  pointer, error conditions above will be reported with @ref UT_ReportError
  causing a runtime error.

  @PVM_EXAMPLE
  In this example the shape integral @a shapeInt is retrieved using the <c>.Sint</c> field of
  a parameter of type @c PVM_RF_PULSE (in this example @c RFPulse1),
  it may be also calculated using @ref MRT_RFPulseGetIntFac for calculated shapes
  <c>.Length</c> and <c>.Pow</c> fields are used as @a length and @a pow argument. The return value
  is assigned to the <c>.Flipangle</c> field of @c RFPulse1.

  @code
  YesNo refPowAvailable=No;
  double refpow=0.0;
  char errstring[256];

  refPowAvailable=STB_GetRefPower(1,&refpow); // reference power of logical transmit channel 1

  if(refPowAvailable==Yes)  // important to assure nonzero value of refpow
  {
     RFPulse1.Flipangle=MRT_RFPulseAngle(RFPulse1.Sint,
                                         RFPulse1.Pow,
                                         RFPulse1.Length,
                                         refpow,
                                         errstring);
     // check error state
     if(*errstring) // error condition occured
     {
        UT_ReportError(errstring);
     }
  }

  // example without errstring would cause a runtime error because of illegal value of length

  RFPulse1.Flipangle=MRT_RFPulseAngle(RFPulse1.Sint,
                                      RFPulse1.Pow,
                                      -1.0,            // illegal value of length
                                      refpow);
  @endcode

  @param      shapeInt  Shape integral (real part of integral of normalized pulse waveform)
  @param      pow       RF pulse shape amplitude specified as peak power (units W)
  @param      length    RF pulse duration in ms
  @param      refpow    Reference amplitude (power of a 90 deg 1ms blockpulse)
  @param[out] errmsg    (optional) Buffer (256 characters used) containing error condition if occured

  @return RF pulse angle in degree (0.0) in case of errors

  @see @ref STB_GetRefPower, @ref MRT_RFPulsePower,
       @ref MRT_RFPulseGetIntFac, @ref STB_UpdateRFPulse

  @todo
  - review warning about power limitations
  - maybe other example

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_RFPulseAngle(double shapeInt, double pow, double length, double refpow, char *errmsg=0);




/** Calculates the flip angle according to specified parameters.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Reference attenuation must be available.

  @PVM_ERROR
  - Illegal value of argument @a integralRatio
  - Illegal value of argument @a attenuationCorrection
  - Illegal value of argument @a pulseLength
  - Illegal value of argument @a referenceAttenuation
  - Illegal value of argument @a pulseAngleRequest

  @PVM_EXAMPLE
  @code
  double flipAngle = MRT_RFPulseAngle(ExcPulse.IntegralRatio,
                                      0.0,
                                      ExcPulse.Length,
                                      referenceAttenuation,
                                      0.0);
  @endcode

  @param integralRatio         Integral ratio of RF pulse
  @param attenuationCorrection Pulse-specific attenuation correction [dB]
  @param pulseLength           Length of RF pulse [ms]
  @param referenceAttenuation  Reference attenuation for 1 ms 90 deg block pulse [dB]
  @param attenuationRequest    Requested attenuation [dB]

  @return Flip angle [deg]

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_RFPulseAngle(const double  integralRatio,
                                      const double  attenuationCorrection,
                                      const double  pulseLength,
                                      const double  referenceAttenuation,
                                      const double  attenuationRequest);




/**  Calculates the RF pulse amplitude (peak power in W) from desired pulse angle (in deg)

  For a given reference power @a refpow (units W) pulse angle @a angle (units deg)
  pulse duration (@a length units ms) pulse shape integral (@a shapeInt) the pulse amplitude
  pow (units W) is calculated according to the following formula @n

  \f$ pow = refpow *(\frac{angle}{length*90*shapeInt})^2 \f$

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  Arguments in legal range (see below)

  @PVM_ERROR
  - shape integral @a shapeInt not in interval ]0.0,1.0]
  - pulse angle @a angle less than 0.0 deg
  - pulse length @a length less or equal to 0.0 ms
  - reference power @a refpow less or equal to 0.0 W

  If argument @a errmsg is not a @c NULL pointer the error condition is written
  into the string @a errmsg points to and 0.0 is returned. If @a errmsg is a @c NULL
  pointer, error conditions above will be reported with @ref UT_ReportError
  causing a runtime error.

  @PVM_EXAMPLE
  In this example the shape integral @a shapeInt is retrieved using the <c>.Sint</c> field of
  a parameter of type PVM_RF_PULSE (in this example @c RFPulse1) ,
  it may be also calculated using @ref MRT_RFPulseGetIntFac for calculated shapes
  <c>.Length</c> and <c>.Flipangle</c> fields are used as @a length and @a angle argument, the
  return value is assigned to the <c>.Pow</c> field of @c RFPulse1.

  @code
  YesNo refPowAvailable=No;
  double refpow=0.0;
  char errstring[256];

  refPowAvailable=STB_GetRefPower(1,&refpow); // reference power of logical transmit channel 1

  if(refPowAvailable==Yes)  // important to assure nonzero value of refpow
  {
      RFPulse1.Pow=MRT_RFPulsePower(RFPulse1.Sint,
                                    RFPulse1.Flipangle,
                                    RFPulse1.Length,
                                    refpow);

     // check error state
     if(*errstring) // error condition occured
     {
        UT_ReportError(errstring);
     }
  }

  // example without errstring would cause a runtime error because of illegal value of length

  RFPulse1.Pow=MRT_RFPulsePower(RFPulse1.Sint,
                                RFPulse1.Flipangle,
                                -1.0,         // illegal value of length
                                refpow);
  @endcode

  @param      shapeInt Shape integral (real part of integral of normalized pulse waveform)
  @param      angle    RF pulse angle (units deg)
  @param      length   RF pulse duration in ms
  @param      refpow   Reference amplitude (power of a 90 deg 1ms blockpulse)
  @param[out] errmsg   (optional) Buffer (256 characters used) containing error condition if occured

  @return  RF pulse shape amplitude specified as peak power (units W)

  @see @ref MRT_RFPulseAngle, @ref MRT_RFPulseGetIntFac,
       @ref STB_UpdateRFPulse, @ref STB_GetRefPower

  @todo
  - review warning about power limitations
  - maybe other example

  @ingroup PVM_MRT
*/

DLL_INTERFACE double MRT_RFPulsePower(double shapeInt,double angle,double length,double refpow, char *errmsg=0);





/** Calculates shape integral and power integral of a calculated RF pulse shape.

  @PVM_H1{Definition of Shape integral}
  The shape integral \f$S_I\f$ (argument @a sint) of a complex waveform \f$W\f$ (argument @a shape )
  is defined as

  \f$S_I =  \frac{\sqrt{(\sum_{i=0}^{N-1} re(W_i))^2 + (\sum_{i=0}^{N-1} im(W_i))^2}}{N}\f$

  where \f$N\f$ is the number of complex points and @a shape contains the complex waveform with alternating
  real and imaginary part ( argument @a polar == @c No) or amplitude and phase (@a polar == @c Yes, phase in degree).

  @PVM_H1{Definition of Power integral}
  The power integral \f$P_I\f$ (argument @a pint ) is defined as

  \f$P_I = \frac{\sum_{i=0}^{N-1} W_i * W_i^*}{N}\f$

  @PVM_H1{Normalization condition}
  To assure a correct scaling of RF waveform amplitudes, the elements of the shape must fullfill the condition

  \f$ |W_i| \le 100.0 \; \forall i=0 \cdots N-1 \f$ and max\f$(|W_i|) = 100\f$ (accuracy 0.001)

  The shape integral \f$S_I\f$ (argument @a sint) is used to derive the pulse amplitude \f$P\f$ (units W)
  of a given RF pulse shape \f$W\f$ (argument @a shape) of pulse angle \f$\alpha\f$ and duration
  \f$\tau_p\f$ with reference power \f$ P_r\f$ ( refer to @ref MRT_RFPulsePower ), the power integral
  is used to calculate the maximum allowed pulse amplitude with respect to hardware protection
  (refer to @ref CFG_MaxRFPower).

  @PVM_H1{Note}
  Shape waveforms that are used inside pulseprograms require a normalized polar complex representation.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  The waveform @a shape specified either in polar or cartesian must meet the normalization condition.

  @PVM_ERROR
  NONE

  @PVM_H2{Nonzero return values indicate an illegal condition of the pulse shape}
  - Negative return values indicate an illegal call of the routine, arguments @a sint and @a pint remain unchanged.
  - Positive return values indicate a violation of the normalization condition or illegal complex representation
    of the shape, @a sint and @a pint are set to 1.0 in this case.

  @PVM_EXAMPLE
  @code
  // parameter RFShape is defined as double array ShapeSiz (int) is the size of the double array
  // NPoints (int) the number of complex shape points

  // set array length
  ShapeSiz = 2*NPoints;

  PARX_change_dims("RFShape",ShapeSiz);

  // calculate waveform RFShape  (...)

  // call routine in this way if RFShape is in polar complex representation

  int failed=0;
  double shint=0.0,powint=0.0;

  failed=MRT_RFPulseGetIntFac(RFShape,NPoints,&shint,&pint,Yes);

  switch(failed)
  {
    case 0:
      break;
    case -1: // NPoints <=0
      UT_ReportError("Illegal number of shape points");
      break;
    case 1:  // Negative shape amplitude
      UT_ReportError("Negative shape amplitude detected");
      break;
    case 2: // Maximum shape amplitude exceeds 100
      UT_ReportError("Illegal shape amplitude detected");
      break;
  }
  @endcode

  @param[in]  shape Double array containing complex waveform
  @param      siz   Number of complex shape points
  @param[out] sint  Pointer to double variable that stores shape integral
  @param[out] pint  Pointer to double variable that stores power integral
  @param      polar @c Yes / @c No = shape is in polar / cartesian complex representation

  @return
          - -1: either @a shape @a sint or @a pint are 0 pointer or @a siz \f$\le\f$ 1
          -  1: negative amplitude if polar == @c Yes
          -  2: normalization condition is violated
          -  0: successful call

  @todo review

  @ingroup PVM_MRT
*/
DLL_INTERFACE int MRT_RFPulseGetIntFac(double *shape,const int siz, double *const sint, double *const pint, YesNo polar);





/** Calculate length of an RF pulse from its bandwidth.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  Illegal arguments.

  @PVM_EXAMPLE
  @code
  PVM_PULSE             pulse;
  PVM_PULSE_PROPERTIES  property;
  PVM_PULSE_CORRECTIONS pulseCorr;

  ...

  pulse.PulseLength = MRT_RFPulseBandwidthToPulseLength(
                        property.BandwidthFactor,
                        pulseCorr.Bandwidth,
                        pulse.Bandwidth);
  @endcode

  @param bandwidthFactor           Pulse bandwidth factor
  @param bandwidthFactorCorrection Pulse bandwidth factor correction [%]
  @param bandwidthRequest          Requested bandwidth [kHz]

  @return

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_RFPulseBandwidthToPulseLength(const double bandwidthFactor,
                                                       const double bandwidthFactorCorrection,
                                                       const double bandwidthRequest);




/** Calculate bandwidth of an RF pulse from its length

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  Illegal arguments.

  @PVM_EXAMPLE
  @code
  PVM_PULSE             pulse;
  PVM_PULSE_PROPERTIES  property;
  PVM_PULSE_CORRECTIONS pulseCorr;

  ...

  pulse.Bandwidth = MRT_RFPulsePulseLengthToBandwidth(
                        property.BandwidthFactor,
                        pulseCorr.Bandwidth,
                        pulse.PulseLength);
  @endcode

  @param bandwidthFactor           Pulse bandwidth factor [kHz]
  @param bandwidthFactorCorrection Pulse bandwidth factor correction [%]
  @param pulseLengthRequest        Requested pulse length [ms]

  @return

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_RFPulsePulseLengthToBandwidth(const double bandwidthFactor,
                                                       const double bandwidthFactorCorrection,
                                                       const double pulseLengthRequest);




/** Calculate excitation profile of an RF pulse

  Uses the forward SLR transform to find the profile of transverse and
  longitudinal magnetization after the application of an arbitrary pulse shape
  to the intial z-magnetization vector.


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

  ...


  @endcode

  @param nTimePoints           number of pulse elements
  @param pulseAngles           flip angles of pulse elements in radians
  @param pulsePhases           phases of pulse elements in radians
  @param duration              pulse duration (sec or ms, see freqRange)
  @param freqRange             frequency range of observed profile (in Hz or kHz, depending on units of duration)
  @param nFreqPoints           number of calculated points within the frequency range
  @param mxy                   calculated complex profile of transverse magnetisation, must be allocated to 2*nFrequencyPoints
  @param mz                    calculated real profile of longitudinal magnetization, must be allocated to nFrequencyPoints


  @return 0 if OK


  @ingroup PVM_MRT
*/
DLL_INTERFACE int MRT_RFPulseExcProfile(int nTimePoints,
                                        const double *pulseAngles,
                                        const double *pulsePhases,
                                        double duration,
                                        double freqRange,
                                        int nFreqPoints,
                                        double *mxy,
                                        double *mz);




/** Calculate inverse Shinnar-LeRoux transform

  Uses the inverse SLR transform to find the RF shape producing a given excitation profile.
  Result is set in polar form (amplitude, phase, amplitude, phase, ...) in degrees.

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

  ...


  @endcode

  @param pulseType     1-small tipp, 2-90, 3-inver, 4-echo refoc, 5-sat
  @param flipAngle    flip angle in degree
  @param minPhase     1-minimum phase pulse (prefocused), 0 - symmetric pulse
  @param bwt          time-bandwidth products (sec*Hz)
  @param inRipple     % of ripple inside
  @param outRipple    % of ripple outside
  @param nSamples     pulse shape size (amplitudes and phases)
  @param result       resulting shape, must be allocated for 2*nSamples doubles
  @param nBands       number of symetric bands (default=1 for a standard pulse)
  @param bandSpc      spacing (centre-centre, default=0) of multiple bands as fraction of Nyquist frequency



  @ingroup PVM_MRT
*/
DLL_INTERFACE void MRT_InverseSLR(int pulseType,     // 1-small tipp, 2-90, 3-inver, 4-echo refoc, 5-sat
                    double flipAngle,  // deg
                    int minPhase,      // 1-min phase pulse (prefocused), 0 - symmetric pulse
                    double bwt,        // time-bandwidth products (sec*Hz)
                    double inRipple,   // % of ripple inside
                    double outRipple,  // % of ripple outside
                    int    nSamples,   // shape size
                    double *result,    // result, must be allocated for 2*nSample doubles
                    int nBands=1,      // number of bands
                    double bandSpc=0);  // band spacing as fraction of Nyquist




/**************************************************************************/
/* gen/src/prg/pvm_toolboxes/PvMrTools/shapes.c */
/**************************************************************************/
DLL_INTERFACE double ramp_lin(double x);
DLL_INTERFACE double ramp_05sin(double x);
DLL_INTERFACE double ramp2_05sin(double x);
DLL_INTERFACE double ramp_sin(double x);

/** Generates a ramp in gradient shape buffer.

  Generates a ramp of gradient in a buffer for a given starting and ending point. The shape of
  the ramp is determined by the function given in the argument @a rampfun. Standard ramps can be
  generated using the following arguments:
  - @c ramp_lin: linear
  - @c ramp_sin: sinusoidal (-pi/2 ... pi/2)
  - @c ramp_05sin: sinusoidal (0 ... pi/2)
  - @c ramp2_05sin: sinusoidal (-pi/2 ... 0)

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Size of @a buffer must be at least @a bufsiz.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  Linear ramp filling half of the shape buffer
  @code
  PARX_change_dims("GradShape", GradShapeSize);

  MRT_MakeRamp(GradShape,
               GradShapeSize,
               2.0,
               0.0,
               1.0,
               0.0,
               1.0,
	       ramp_lin);
  @endcode

  @param[out] buffer       Buffer in which the gradient shape is generated. It must be resized to
                           @a bufsize elements.
  @param      bufsize      Size of the buffer
  @param      totalTime    Total duration of the buffer
  @param      ton          Starting time of the designed ramp
  @param      toff         End time of the designed ramp
  @param      aon          Initial amplitude of the designed ramp
  @param      aoff         Final amplitude of the designed ramp
  @param[in]  rampfun      Name of a function determining the shape.
                           This must be a function returnig values from @c 0.0 to @c 1.0 for its
                           argument varying from @c 0.0 to @c 1.0. To obtain standard shapes, the
                           following predefined functions can be used:
                           @c ramp_lin, @c ramp_sin, @c ramp_05sin, @c ramp2_05sin

  @return Total time of the ramp shape

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_MakeRamp (double  *buffer,
                                   int     bufsize,
                                   double  totalTime,
                                   double  ton,
                                   double  toff,
                                   double  aon,
                                   double  aoff,
                                   double  (*rampfun)(double x));




/**************************************************************************/
/*              gen/src/prg/pvm_toolboxes/PvMrTools/cpmg.c                */
/**************************************************************************/
/** Calculates gradient points for one period of CPMG.

  @PVM_USEINSTEAD{MRT_CalculateCpmgPeriod}

  Calculates all gradient switching points (time and amplitudes) for one period of the CPMG
  sequence with given gradient slew rate and maxima. Calculates the minimum echo spacing which
  guarantees that the required spoilers and resolutions are achieved without breaking the
  gradient amplitude and slew rate limits, and restricts the required @a durPeriod to that.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @param slewRate            Maximum slew rate allowed simultaneously on all gradient channels
                             (Hz/mm/sec)
  @param maxGrad             Maximum gradient allowed simultaneously on all channels (Hz/mm)
  @param readGrad            Amplitude of the readout gradient (gr1 in Fig.1)
  @param sliceGrad           Amplitude of the slice slection gradient (gs0)
  @param durPulse            Duration of the refocusing pulse, i.e. of the flat top of the
                             slice selection gradient (ms)
  @param durAcq              Duration of the acquisition, i.e. of the flat top of the readout
                             gradient (ms)
  @param durGrid             Duration of a time grid unit (ms). All gradient switching points
                             must be placed on this grid.
  @param spoilRead           Spoiling strength of the readout channel (dispersion angle of
                             the transverse magnetization per distance unit produced by the
                             readout gradient from the start of the period till the echo
                             center) in cycles per mm. This parameter determines the integral
                             of the readout gradient from the beginning to the echo center
                             (kr1 in Fig.1).
  @param spoilSlice          Spoiling strength of the slice channel (dispersion angle of the
                             transverse magnetization per distance unit produced by the readout
		             gradient from the start of the period till the echo center) in
                             cycles per mm. This parameter determines the integral of the slice
                             gradient from the pulse center to the start of acquisition
                             (ks0 in Fig.1).
  @param spoilAsymRead       Strength (in cycles per mm) of an additional spoiler on the
                             readout channel added after the acquisition (kr2). The total
                             integral of the readout pulse should be 2*@a spoilRead +
                             @a spoilAsymRead. This spoiler may be used in UFLARE-type
                             experiments.
  @param spoilAsymSlice      Strength (in cycles per mm) of an additional spoiler on the slice
                             channel added after the acquisition (ks2). The total integral of
		             the readout pulse should be 2*@a spoilSlice + @a spoilAsymSlice.
                             This spoiler may be used in UFLARE-type experiments.
  @param resolPhase          Resolution in the readout direction (mm), determines the integral
                             of the maximum encoding pulse (kp) on the phase channel
  @param resolSlice          Resultion in the slice selection direction in case of 3D encoding
                             (zero for 2D), determines the integral of the maximum phase
                             encoding pulse on the slice channel (ks1)
  @param minMaxSliceScale    Array of two doubles giving the range of phase encoding in third
                             dim (between @c -1.0 and @c 1.0)
  @param tRead               (output) Table of gradient switching times (ms) for the read
                             channel, relative to the period origin. See Fig.1 for description.
                             In the figure, tr0 represents @a tRead[0], etc.
  @param tPhase              (output) Table of gradient switching times (ms) for the phase
                             channel (tp0 in Fig.1)
  @param tSlice              (output) Table of gradient switching times (ms) for the slice
                             channel (ts0 in Fig.1)
  @param gRead               (output) Table of gradient amplitudes (Hz/mm) for the read channel
                             (gr0 in Fig.1)
  @param gPhase              (output) Table of gradient amplitudes (Hz/mm) for the phase
                             channel (gp0 in Fig.1)
  @param gSlice              (output) Table of gradient amplitudes (Hz/mm) for the slice
                             channel (gs0 in Fig.1)
  @param gSliceEnc           (output) Table of gradient amplitudes (Hz/mm) for the 3D phase
                             encoding on the slice channel (gse0 in Fig.1)
  @param durPeriod           (in/out) Duration of the period of the sequence (echo spacing,
                             ms). At input this parameter contains the value needed by the
                             method. If this value is too short for the specified resolution,
                             it is increased to a minimum legal value (see return value). It is
                             rounded up to the specified time grid.

  @return
  - @c  0  echo spacing accepted @n
  - @c  1  echo spacing increased @n
  - @c -1  error

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE int MRT_CalculateCpmgPeriod(double       slewRate,
                                          double       maxGrad,
                                          double       readGrad,
                                          double       sliceGrad,
                                          double       durPulse,
                                          double       durAcq,
                                          double       durGrid,
                                          double       spoilRead,
                                          double       spoilSlice,
                                          double       spoilAsymRead,
                                          double       spoilAsymSlice,
                                          double       resolPhase,
                                          double       resolSlice,
                                          const double *minMaxSliceScale,
                                          double       *tRead,
                                          double       *tPhase,
                                          double       *tSlice,
                                          double       *gRead,
                                          double       *gPhase,
                                          double       *gSlice,
                                          double       *gSliceEnc,
                                          double       *durPeriod);




/** Calculates gradient points for one period of CPMG.

  Calculates all gradient switching points (time and amplitudes) for one period of the CPMG
  sequence with given gradient slew rate and maxima. Calculates the minimum echo spacing which
  guarantees that the required spoilers and resolutions are achieved without breaking the
  gradient amplitude and slew rate limits, and restricts the required @a durPeriod to that.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The input and arrays must have size @c MRT_CPMG_NPOINTS.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  double sliceScale[2];
  sliceScale[0] = UT_MinOfDoubleArray(PVM_EncMatrix[2], PVM_EncValues2);
  sliceScale[1] = UT_MaxOfDoubleArray(PVM_EncMatrix[2], PVM_EncValues2);

  double tRead[MRT_CPMG_NPOINTS], tPhase[MRT_CPMG_NPOINTS], tSlice[MRT_CPMG_NPOINTS],
         gRead[MRT_CPMG_NPOINTS], gPhase[MRT_CPMG_NPOINTS], gSlice[MRT_CPMG_NPOINTS];

  int ret = MRT_CalculateCpmgPeriod(
    SlewRatePerCent / 100 * CFG_GradCalConst() / CFG_GradientRampTime(),
    CFG_GradCalConst() * gradientLimit / 100,
    CFG_GradCalConst() * ReadGradient / 100,
    CFG_GradCalConst() * RefSliceGradient / 100,
    RefPulse1.Length + 2.0 * GradStab,      // add margins if needed
    PVM_AcquisitionTime + 2.0 * GradStab,   // add margins if needed
    CFG_GradientShapeResolution(),
    SpoilingRead,
    SpoilingSlice,
    0.0, // asym spoiler read
    0.0, // asym spoiler slice
    PVM_SpatResol[1],
    sliceScale,
    tRead,
    tPhase,
    tSlice,
    gRead,
    gPhase,
    gSlice,
    &PVM_EchoTime);
  @endcode

  @param slewRate            Maximum slew rate allowed simultaneously on all gradient channels
                             (Hz/mm/sec)
  @param maxGrad             Maximum gradient allowed simultaneously on all channels (Hz/mm)
  @param readGrad            Amplitude of the readout gradient (gr1 in Fig.1)
  @param sliceGrad           Amplitude of the slice slection gradient (gs0)
  @param durPulse            Duration of the refocusing pulse, i.e. of the flat top of the
                             slice selection gradient (ms)
  @param durAcq              Duration of the acquisition, i.e. of the flat top of the readout
                             gradient (ms)
  @param durGrid             Duration of a time grid unit (ms). All gradient switching points
                             must be placed on this grid.
  @param spoilRead           Spoiling strength of the readout channel (dispersion angle of
                             the transverse magnetization per distance unit produced by the
                             readout gradient from the start of the period till the echo
                             center) in cycles per mm. This parameter determines the integral
                             of the readout gradient from the beginning to the echo center
                             (kr1 in Fig.1).
  @param spoilSlice          Spoiling strength of the slice channel (dispersion angle of the
                             transverse magnetization per distance unit produced by the readout
		             gradient from the start of the period till the echo center) in
                             cycles per mm. This parameter determines the integral of the slice
                             gradient from the pulse center to the start of acquisition
                             (ks0 in Fig.1).
  @param spoilAsymRead       Strength (in cycles per mm) of an additional spoiler on the
                             readout channel added after the acquisition (kr2). The total
                             integral of the readout pulse should be 2*@a spoilRead +
                             @a spoilAsymRead. This spoiler may be used in UFLARE-type
                             experiments.
  @param spoilAsymSlice      Strength (in cycles per mm) of an additional spoiler on the slice
                             channel added after the acquisition (ks2). The total integral of
		             the readout pulse should be 2*@a spoilSlice + @a spoilAsymSlice.
                             This spoiler may be used in UFLARE-type experiments.
  @param resolPhase          Resolution in the readout direction (mm), determines the integral
                             of the maximum encoding pulse (kp) on the phase channel
  @param minMaxSliceScale    Array of two doubles giving the range of phase encoding in third
                             dim (between @c -1.0 and @c 1.0)
  @param[out] tRead          Table of gradient switching times (ms) for the read
                             channel, relative to the period origin. See Fig.1 for description.
                             In the figure, tr0 represents @a tRead[0], etc.
  @param[out] tPhase         Table of gradient switching times (ms) for the phase channel (tp0 in Fig.1)
  @param[out] tSlice         Table of gradient switching times (ms) for the slice channel (ts0 in Fig.1)
  @param[out] gRead          Table of gradient amplitudes (Hz/mm) for the read channel (gr0 in Fig.1)
  @param[out] gPhase         Table of gradient amplitudes (Hz/mm) for the phase channel (gp0 in Fig.1)
  @param[out] gSlice         Table of gradient amplitudes (Hz/mm) for the slice channel (gs0 in Fig.1)
  @param[in,out] durPeriod   Duration of the period of the sequence (echo spacing,
                             ms). At input this parameter contains the value needed by the
                             method. If this value is too short for the specified resolution,
                             it is increased to a minimum legal value (see return value). It is
                             rounded up to the specified time grid.

  @return
  - @c  0  echo spacing accepted
  - @c  1  echo spacing increased
  - @c -1  error

  @todo Incomplete documentation: Fig.1 is missing

  @ingroup PVM_MRT
*/
DLL_INTERFACE int MRT_CalculateCpmgPeriod(double       slewRate,
                                          double       maxGrad,
                                          double       readGrad,
                                          double       sliceGrad,
                                          double       durPulse,
                                          double       durAcq,
                                          double       durGrid,
                                          double       spoilRead,
                                          double       spoilSlice,
                                          double       spoilAsymRead,
                                          double       spoilAsymSlice,
                                          double       resolPhase,
                                          const double *minMaxSliceScale,
                                          double       *tRead,
                                          double       *tPhase,
                                          double       *tSlice,
                                          double       *gRead,
                                          double       *gPhase,
                                          double       *gSlice,
                                          double       *durPeriod);




/**************************************************************************/
/*             gen/src/prg/pvm_toolboxes/PvMrTools/utilities.c            */
/**************************************************************************/

/** Calculates the field-of-view that can be achieved by given sweep width,
    gradient calibration constant, and gradient strength.

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
  Calculate ninimum possible FOV at full gradient strength
  @code
  double minFov = MRT_Fov(PVM_EffSWh,
                          100.0,
                          CFG_GradCalConst());
  @endcode

  @param sweepWidthHz    Spectral width of the data acquisition (Hz)
  @param grad            Gradient strength expressed as a percentage of the gradient calibration
                         constant
  @param gradCalConst    Gradient calibration constant (Hz/mm)

  @return Field-of-view (mm)

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_Fov(double sweepWidthHz,
                             double grad,
                             double gradCalConst);




/** Calculates the spectral width for an acquisition.

  @PVM_H1{Note}
  This toolbox function should be replaced by the digitizer parameter group. The handling of
  bandwidth and digital filters should than be implemented with the following set of toolboxes:
  @ref STB_InitDigPars, @ref STB_UpdateDigPars (@ref STB_SpecUpdateDigPars), and
  @ref ATB_SetDigPars

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @param effSweepWidthHz    Effective spectral width of the data acquisition. It takes
                            oversampling (anti-aliasing) into consideration (Hz).
  @param antiAliasFactor    Oversampling factor

  @return Spectral width of the data acquisition (Hz)

  @see @ref STB_InitDigPars, @ref STB_SpecUpdateDigPars, @ref STB_UpdateDigPars,
       @ref ATB_SetDigPars

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE double MRT_SWh(double effSweepWidthHz,
                             double antiAliasFactor);



DLL_INTERFACE void MRT_AcqScanHandler(void);



/** Calculates the effective spectral width for an acquisition.

  @PVM_H1{Note}
  This toolbox function should be replaced by the digitizer parameter group. The handling of
  bandwidth and digital filters should than be implemented with the following set of toolboxes:
  @ref STB_InitDigPars, @ref STB_UpdateDigPars (@ref STB_SpecUpdateDigPars), and
  @ref ATB_SetDigPars

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @param sweepWidthHz       Spectral width of the data acquisition (Hz)
  @param antiAliasFactor    Oversampling factor

  @return Effective spectral width of the data acquisition. It takes oversampling
          (anti-aliasing) into consideration (Hz).

  @see @ref STB_InitDigPars, @ref STB_SpecUpdateDigPars, @ref STB_UpdateDigPars,
       @ref ATB_SetDigPars

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE double MRT_EffSWh(double sweepWidthHz,
                                double antiAliasFactor);




/** Calculates the duration of an acquisition.

  @PVM_H1{Note}
  This toolbox function should be replaced by the digitizer parameter group. The handling of
  bandwidth and digital filters should than be implemented with the following set of toolboxes:
  @ref STB_InitDigPars, @ref STB_UpdateDigPars (@ref STB_SpecUpdateDigPars), and
  @ref ATB_SetDigPars. The exact duration of the acquisition period is available after a call
  of @ref STB_UpdateDigPars (parameter @c PVM_DigDur).

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @param effSWh      Effective spectral width of the data acquisition. It takes oversampling
                     (anti-aliasing) into consideration (Hz).
  @param readSize    Number of data points acquired in a single sweep

  @return

  @see @ref STB_InitDigPars, @ref STB_SpecUpdateDigPars, @ref STB_UpdateDigPars,
       @ref ATB_SetDigPars

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE double MRT_AcqTimeEffSWh(double effSWh,
                                       int    readSize);




/** Calculates a good phase encoding start in @em RARE.

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
  PVM_EncStart[1] = MRT_RareEncodingStart(PVM_EncStart[1], PVM_EncMatrix[1], PVM_EncMatrix[1] / nSegments);
  @endcode

  @param oldEncStart    Initial value of phase encoding start
  @param size           Matrix size in the @em RARE direction
  @param rareFactor     Rare factor

  @return Value of phase encoding start which gives minimum perturbation due to approach
          steady state, and which is possibly close to the initially specified start
          (@a oldEncStart). This value can be used to set @c ACQ_phase_encoding_start.

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_RareEncodingStart(double oldEncStart,
                                           int    size,
                                           int    rareFactor);




/** Calculates a list of frequency shifts for RF spoiling.

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
  MRT_RFSpoilFreqList(117.0, ACQ_O2_list_size, ACQ_O2_list, EncGradDur);
  @endcode

  @param      spoilfactor    Phase increment factor (deg)
  @param      spoilsize      Lists size
  @param[out] spoillist      List of the frequency values
  @param      spoildelay     Duration of the frequency shift for one step (ms)

  @see @ref MRT_RFSpoilPhaseList

  @ingroup PVM_MRT
*/
DLL_INTERFACE void MRT_RFSpoilFreqList(double spoilfactor,
                                       int    spoilsize,
                                       double *spoillist,
                                       double spoildelay);




/** Calculates a list of phase shifts for RF spoiling.

  @PVM_USEINSTEAD{MRT_RFSpoilPhaseList}

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @param spoilfactor    Phase increment factor (deg)
  @param spoilsize      Lists size
  @param spoillist      List of the frequency values

  @see @ref MRT_RFSpoilPhaseList

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE void MRT_RFSpoilPhaseList(double spoilfactor,
                                        int    spoilsize,
                                        double *spoillist);




/** Calculates a list of phase shifts for RF spoiling.

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
  int size = PVM_EncMatrix[1] + PVM_DummyScans;
  PARX_change_dims("RFPhaseList",size);
  MRT_RFSpoilPhaseList(117.0, size, RFPhaseList, Yes);
  @endcode

  @param      spoilfactor    Phase increment factor (deg)
  @param      spoilsize      Lists size
  @param[out] spoillist      List of the frequency values
  @param      absolute       YesNo option in order to calculate absolute
                             or relative phase increments

  @see @ref MRT_RFSpoilFreqList

  @ingroup PVM_MRT
*/
DLL_INTERFACE void MRT_RFSpoilPhaseList(double spoilfactor,
                                        int    spoilsize,
                                        double *spoillist,
                                        YesNo   absolute);




/** Delivers maximum field variation of a shim.

  Delivers maximum field variation of a shim characterized by a vector of sensitivities
  expressed as spherical harmonic function coefficients in units of Hz/cm^order inside a
  sphere of a given radius. Argument normconv defines the norming
  convention of spherical harmonic coefficients:

  - 0 : Coefficient of  \f$ z^{n-m}x^m \f$  equal one
  - 1 : (default) Normalization like associated Legendre
                 Polynomials. The normalization is defined by the
                 orthonormality condition of spherical functions
  - 2 : Maximum on unit sphere equal one


  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Only nonzero and positive entries of sensitivity array are considered.
  - Only nonzero and positive entries of order are considered.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  // define PVM parameters
  double parameter ShimSens[N_SHIMS][N_SHIMS];
  double parameter ShimRadius;
  ...

  // get order
  int order[N_SHIMS];
  SysInfo_ShimFuncOrder(order);

  // calculate maximum field variation
  double sens[N_SHIMS];

  for (int i = 0; i < N_SHIMS; i++)
  {
    sens[i]= MRT_MaxShimSens(ShimSens[i], order, N_SHIMS, ShimRadius);
  }
  @endcode

  @param[in] sens      Double array of length @a nshims containing a set of spherical harmonic
                       function coefficients expressed in Hz/cm^order.
                       (Only positive nonzero elements are used in calculation.)
  @param[in] order     Array of integers containing the order of the coefficients passed in
                       argument sens. (Only positive nonzero elements are used in calculation.)
  @param nshims        Defines the array length of @a sens and @a order
  @param r             Defines the radius (mm) used for the calculation
  @param nconv         Defines norming convention of spherical harmonics (see above)

  @return Maximum field variation or zero [Hz]

  @ingroup PVM_MRT
*/
DLL_INTERFACE double MRT_MaxShimSens(double *sens,
                                     int    *order,
                                     int    nshims,
                                     double r,
                                     int nconv=1);




/** Initializes or reinitializes @c PVM_SpecDimEnum according to the recipe described for
    @ref MRT_InitSpecDim.

  @PVM_PARS
  - @c PVM_SpecDimEnum

  @PVM_TBCALL
  - @ref MRT_InitSpecDim

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  MRT_SetSpectrocopyDims(1, 2);
  @endcode

  @param lowestDim     Lowest dimension to which a dimension can be set
  @param highestDim    Highest dimension to which a dimension can be set

  @return Current number of dimensions

  @see @ref PTB_SetImagingDims, @ref PTB_InitSpatDim, @ref PTB_SetSpatDim, @ref PTB_GetSpatDim,
        @ref MRT_InitSpecDim, @ref MRT_SetSpecDim, @ref MRT_GetSpecDim

  @todo @n 
        Check which of the related Set/Init functions are actually required.

  @ingroup PVM_MRT
 */
DLL_INTERFACE int MRT_SetSpectrocopyDims(int lowestDim,
    int highestDim);




/** Initializes or reinitializes @c PVM_SpecDimEnum for dimensions lying between @a lowestDim
    and @a highestDim.

  If @a lowestDim is equal to @a highestDim @c PVM_SpecDimEnum is made non-editable. If
  @c PVM_SpecDimEnum has the state <c> NO VALUE </c> it is set to the lowest applicable
  dimension, otherwise its value is confined to lie within the initialized range.

  Note that @a lowestDim and @a highestDim are constrained to lie within the range @c 0 to @c 9.

  @PVM_PARS
  - @c PVM_SpecDimEnum

  @PVM_RELCALL
  NONE

  @PVM_TBCALL
  - @ref MRT_GetSpecDim
  - @ref MRT_SetSpecDim

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  MRT_InitSpecDim(1, 2);
  @endcode

  @param lowestDim     Lowest dimension to which a dimension can be set
  @param highestDim    Highest dimension to which a dimension can be set

  @return Current number of dimensions

  @see @ref PTB_SetImagingDims, @ref PTB_InitSpatDim, @ref PTB_SetSpatDim, @ref PTB_GetSpatDim,
       @ref MRT_SetSpectrocopyDims, @ref MRT_SetSpecDim, @ref MRT_GetSpecDim

  @ingroup PVM_MRT
 */
DLL_INTERFACE int MRT_InitSpecDim(int lowestDim,
    int highestDim);




/** Sets the parameter @c PVM_SpecDimEnum to @c dimValue.

  If @c dimValue lies outside the legal range it will be set according to the recipe described
  for the function @ref UT_SetInLimits.

  @PVM_PARS
  - @c PVM_SpecDimEnum

  @PVM_RELCALL
  NONE

  @PVM_TBCALL
  - @ref MRT_GetSpecDim

  @PVM_PRECOND
  Legal dimension range must have been set with @ref MRT_InitSpecDim.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  MRT_SetSpecDim(2);
  @endcode

  @param dimensions    Number of dimensions

  @return Dimension number that was set

  @see @ref PTB_SetImagingDims, @ref PTB_InitSpatDim, @ref PTB_SetSpatDim, @ref PTB_GetSpatDim,
       @ref MRT_SetSpectrocopyDims, @ref MRT_InitSpecDim, @ref MRT_GetSpecDim

  @ingroup PVM_MRT
 */
DLL_INTERFACE int MRT_SetSpecDim(int dimensions);




/** Retrieves the current value of @c PVM_SpecDimEnum as an int.

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
  int dim = MRT_GetSpecDim();
  @endcode

  @return Number of dimensions

  @see @ref PTB_SetImagingDims, @ref PTB_InitSpatDim, @ref PTB_SetSpatDim, @ref PTB_GetSpatDim,
       @ref MRT_SetSpectrocopyDims, @ref MRT_InitSpecDim, @ref MRT_SetSpecDim

  @ingroup PVM_MRT
 */
DLL_INTERFACE int MRT_GetSpecDim(void);




/** Prints reco network.

  Prints the text description of reconstruction network
  as it is given by the parameters @c RecoStageNodes and
  @c RecoStageEdges to the standard output (Parx output).

  This function is the default relation of the parameter
  @c RecoDebug.

  @ingroup PVM_MRT_INT
 */
DLL_INTERFACE void MRT_RecoDebugRels(void);

/** Checks CS License.

  @ingroup PVM_MRT_INT
 */
DLL_INTERFACE bool MRT_RecoCheckCS(void);


/** Retrieves entry of @c PVM_ShapeHandler that belongs to RF pulse.

  This function is used to access information stored in parameter @c PVM_ShapeHandler.

  @PVM_PARS
  - @c PVM_ShapeHandler (only referenced)

  @PVM_TBCALL
  - @ref UT_GetParType

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  int shidx=-1;
  int errstate=0;
  char errmsg[1024];


  PVM_RF_SHAPE_HANDLER_TYPE *shndlr=0;
  shidx=PTB_GetShapeHandlerIndex(parn,&errstate);
  switch(errstate)
  {
    case 0:
      break;
    case 1: // no entry available
      snprintf(errmsg,1023,"parameter \"%s\" has not been initialized",parn);
      UT_ReportError(errmsg);
      break;
    case 2: // parn not found
      snprintf(errmsg,1023,"parameter \"%s\" has not been initialized",parn);
      UT_ReportError(errmsg);
      break;

    case -3: // parameter type not supported
      snprintf(errmsg,1023,"parameter \"%s\" has wrong parameter type",parn);
      UT_ReportError(errmsg);
      break;
    case -1:
      snprintf(errmsg,1023,"parameter \"%s\" does not exist",parn);
      UT_ReportError(errmsg);
      break;
  }

  shndlr = PVM_ShapeHandler +shidx;

  @endcode

  @param[in] parname name of parameter: following types are supported:
      - @c PVM_RF_PULSE (shape struct)
      - @c PV_PULSE_LIST (shape list enum)
      - @c PVM_RF_PULSE_AMP_TYPE (pulse amplitude struct)
      - double array parameter (waveform array)
  @param[out] errstate pointer to int variable (0 pointer allowed) indicating following
            error conditions:
      - @c  0 = ok
      - @c  1 = no entry available
      - @c -2 = not found
      - @c -1 = wrong parameter name
      - @c -2 = qualified parameter name
      - @c -3 = wrong parameter type

  @return -1 in case of errors, otherwise index in shape handler array

  @ingroup PVM_PTB_INT
 */
DLL_INTERFACE int MRT_GetShapeHandlerIndex(const char *const parname, int *const errstate);

/** Checks the current operation mode and tries to adapt it according to protocol demands. Until now 
    only nucleus stored in the protocol is analyzed.

  @ingroup PVM_MRT
*/
DLL_INTERFACE void MRT_CheckOperationMode(void);



/**************************************************************************/
/*         gen/src/prg/pvm_toolboxes/PvMrTools/MrtShapeAlgorithms.c       */
/**************************************************************************/
/** Calculates a conventional RF pulse shape.

  Function of type @ref SHAPE_ALG_PROC that calculates conventional sinc
  based RF pulses. Struct field @c Sharpness controls the number of sinc
  lobes.

  @PVM_H2{Default behavior}
  Based on @c Sharpness @c Type the algorithm creates the
  waveform, calculates low level attributes <c> Bwfac Sint Pint Rphac </c>
  and stores the waveform in parameter specified by @c Shape.

  @PVM_H2{Specific behavior}
  Dependent on value handle the algorithm acts as follows:

  <table>
  <tr><th> Value     </th><th>    Element edited        </th><th>   behavior                      </th></tr>
  <tr><td> 0         </td><td>    Not edited            </td><td>   default                       </td></tr>
  <tr><td> 1         </td><td>    <c>.Length        </c></td><td>   not handled                   </td></tr>
  <tr><td> 2         </td><td>    <c>.Bandwidth     </c></td><td>   not handled                   </td></tr>
  <tr><td> 3         </td><td>    <c>.Flipangle     </c></td><td>   not handled                   </td></tr>
  <tr><td> 4         </td><td>    <c>.Calculated    </c></td><td>   not applicable                </td></tr>
  <tr><td> 5         </td><td>    <c>.Sharpness     </c></td><td>   used to derive lobes          </td></tr>
  <tr><td> 7         </td><td>    <c>.Bwfac         </c></td><td>   derive appropriate shaprness  </td></tr>
  <tr><td> 8         </td><td>    <c>.Sint          </c></td><td>   ignored calculated in routine </td></tr>
  <tr><td> 9         </td><td>    <c>.Pint          </c></td><td>   ignored calculated in routine </td></tr>
  <tr><td> 10        </td><td>    <c>.Type          </c></td><td>   controlls algorithm           </td></tr>
  <tr><td> 11        </td><td>    <c>.Rphac         </c></td><td>   calculated in routine         </td></tr>
  <tr><td> 12        </td><td>    <c>.Pow           </c></td><td>   not handled                   </td></tr>
  <tr><td> 13        </td><td>    <c>.Shape         </c></td><td>   not applicable                </td></tr>
  </table>

  @PVM_PARS
  - array parameter specified by @c Shape in pulse struct pointer

  @PVM_TBCALL
  - @ref MRT_RFPulseGetIntFac

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  The shape structure (argument @a shapeStruct) has to be in a valid state
  (see error conditions below)

  @PVM_ERROR
  Errors of this function start with message "MRT_ConventionalRFShape: failure".
  They are related to the check of arguments as follows:

  -# @c NULL pointer argument @a shapeStruct ("No pointer to shape struct")
  -# @a Calculated flag in shape structure is set to @c No ("Calculated flag
     not set")
  -# Check of @a Shape struct field: this must be the name of a double array
     parameter preceeded by a "$".\n
     The following errors are possibly reported ((name) is the content of the
     @a Shape struct field):
     - check of valid parameter name ("(name): no parameter name")
     - check of valid parameter type ("(name): (type) wrong parameter type for waveform")
     - check whether parameter is an array ("(name): double array required")
     - check of qualified parameter name ("(name) : unqualified parameter expected")
     - check of parameter dimensionality ("(name) : 1 dim double array expected")
  -# Check of (optional) additional parameter list @a addParnames : a colon separated
     list of parameter names is expected ((addpn): the name that is checked):
     - check of valid parameter name ("addParList: (addpn) no parameter name")
     - check of colon separator ("addParList: (char) illegal stringlist format")

  These error conditions will cause runtime errors reported by @ref UT_ReportError.

  @param[in] shapeStruct  Pointer to a variable of type @c PVM_RF_PULSE
  @param     handle       Integer value indicating special requests (see table above)
  @param[in] addParnames  Should be set to @c NULL pointer in this function
  @param     pmax         Ignored, used in final update of struct
  @param     pcoilmax     Ignored, used in final update of struct
  @param     mpdur        Ignored, used in final update of struct
  @param     pcwmax       Ignored, used in final update of struct
  @param     pref         Ignored, used in final update of struct
  @param     setPower     Tells if the calculated pulse power should be set in shapeStruct.pow

  @return @c 1 (final update of pulse struct required based on calculated low level attributes)

  @see @ref SHAPE_ALG_PROC, @ref MRT_AfpUpRFShape, @ref MRT_AfpDownRFShape, @ref STB_UpdateRFPulse

  @todo
  - Review
  - Replace wurst pulse approach in text
  - Incomplete documentation: PVM_EXAMPLE
  - Write missing references to SysInfo functions (not yet decided).

  @ingroup PVM_MRT
*/
DLL_INTERFACE int MRT_ConventionalRFShape(PVM_RF_PULSE *const shapeStruct,
                                          int handle,
                                          const char *const addParnames,
                                          double pmax,
                                          double pcoilmax,
                                          double mpdur,
                                          double pcwmax,
                                          double pref,
                                          YesNo setPower);




/** Calculates a adiabatic full-passage (AFP) RF pulse shape with upward
    frequency sweep.

  Function of type @ref SHAPE_ALG_PROC that calculates an adiabatic pulse
  adapted to required bandwidth and power constraints of the system.
  Struct field @c Sharpness controls the duration of the pulse (sharper
  pulses are longer).

  @PVM_H2{Default behavior}
  Based on @c Sharpness and @c Type the algorithm creates the
  waveform, calculates   low level attributes <c> Bwfac Sint Pint Rphac </c>
  and stores the waveform in parameter specified by @c Shape.

  @PVM_H2{Specific behavior}
  Dependent on value handle the algorithm acts as follows:

  <table>
  <tr><th> Value     </th><th>    Element edited        </th><th>   behavior                      </th></tr>
  <tr><td> 0         </td><td>    Not edited            </td><td>   default                       </td></tr>
  <tr><td> 1         </td><td>    <c>.Length        </c></td><td>   not handled                   </td></tr>
  <tr><td> 2         </td><td>    <c>.Bandwidth     </c></td><td>   not handled                   </td></tr>
  <tr><td> 3         </td><td>    <c>.Flipangle     </c></td><td>   not handled                   </td></tr>
  <tr><td> 4         </td><td>    <c>.Calculated    </c></td><td>   not applicable                </td></tr>
  <tr><td> 5         </td><td>    <c>.Sharpness     </c></td><td>   used to derive t-bw product   </td></tr>
  <tr><td> 7         </td><td>    <c>.Bwfac         </c></td><td>   not handled                   </td></tr>
  <tr><td> 8         </td><td>    <c>.Sint          </c></td><td>   ignored calculated in routine </td></tr>
  <tr><td> 9         </td><td>    <c>.Pint          </c></td><td>   ignored calculated in routine </td></tr>
  <tr><td> 10        </td><td>    <c>.Type          </c></td><td>   controlls algorithm           </td></tr>
  <tr><td> 11        </td><td>    <c>.Rphac         </c></td><td>   calculated in routine         </td></tr>
  <tr><td> 12        </td><td>    <c>.Pow           </c></td><td>   not handled                   </td></tr>
  <tr><td> 13        </td><td>    <c>.Shape         </c></td><td>   not applicable                </td></tr>
  </table>


  @PVM_PARS
  - array parameter specified by @c Shape in pulse struct pointer

  @PVM_TBCALL
  - @ref MRT_RFPulseGetIntFac

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  The shape structure (argument @a shapeStruct) has to be in a valid state (see error conditions below)

  @PVM_ERROR
  Errors of this function start with message "MRT_ConventionalRFShape: failure".
  They are related to the check of arguments as follows:

  -# @c NULL pointer argument @a shapeStruct ("No pointer to shape struct")
  -# @a Calculated flag in shape structure is set to No ("Calculated flag not set")
  -# Check of @a Shape struct field: this must be the name of a double array parameter preceeded with a "$".\n
     The following errors are possibly reported ((name) is the content of the @a Shape struct field):
     - check of valid parameter name ("(name): no parameter name")
     - check of valid parameter type ("(name): (type) wrong parameter type for waveform")
     - check whether parameter is an array ("(name): double array required")
     - check of qualified parameter name ("(name) : unqualified parameter expected")
     - check of parameter dimensionality ("(name) : 1 dim double array expected")
  -# Check of (optional) additional parameter list @a addParnames : a colon separated
     list of parameter names is expected ((addpn): the name that is checked):
     - check of valid parameter name ("addParList: (addpn) no parameter name")
     - check of colon separator ("addParList: (char) illegal stringlist format")

  These error conditions will cause runtime errors reported by @ref UT_ReportError.

  @param[in] shapeStruct  Pointer to a variable of type PVM_RF:PULSE
  @param     handle       Integer value indicating special requests (see table above)
  @param[in] addParnames  Should be set to @c NULL pointer in this function
  @param     pmax         Ignored, used in final update of struct
  @param     pcoilmax     Ignored, used in final update of struct
  @param     mpdur        Ignored, used in final update of struct
  @param     pcwmax       Ignored, used in final update of struct
  @param     pref         Ignored, used in final update of struct
  @param     setPower     Tells if the calculated pulse power should be set in shapeStruct.pow


  @return @c 0 (final update of pulse struct checks valid pow only)

  @see @ref SHAPE_ALG_PROC, @ref MRT_ConventionalRFShape, @ref MRT_AfpDownRFShape, @ref STB_UpdateRFPulse

  @todo
  - Review
  - Replace wurst pulse approach in text
  - Incomplete documentation: PVM_EXAMPLE
  - Write missing references to SysInfo functions (not yet decided).

  @ingroup PVM_MRT
*/
DLL_INTERFACE int MRT_AfpUpRFShape(PVM_RF_PULSE *const shapeStruct,
                                   int handle,
                                   const char *const addParnames,
                                   double pmax,
                                   double pcoilmax,
                                   double mpdur,
                                   double pcwmax,
                                   double pref,
                                   YesNo setPower);




/** Calculates a adiabatic full-passage (AFP) RF pulse shape with downward frequency sweep.

@see @ref MRT_AfpUpRFShape

@ingroup PVM_MRT

 */
DLL_INTERFACE int MRT_AfpDownRFShape(PVM_RF_PULSE *const shapeStruct,
                       int handle,
                       const char *const addParnames,
                       double pmax,
                       double pcoilmax,
                       double mpdur,
                       double pcwmax,
                       double pref,
                       YesNo setPower);




/** Calculates flow-compensated readout gradient amplitudes.

  Calculates amplitude of the two dephasing lobes of the readout gradient
  so as to obtain zero integral and zero first moment at echo position.
  It is assumed that the lobes and the readout gradient pulse are adjacent
  and have identical ramps (see picture below). Gradient delay is not considered in the calculation.
  It should be taken into account in the pulse program by delaying the acquistion window.
  This function does not
  control the minimum duration of the lobes and may generate lobe amplitudes above 100%.
  Resulting amplitudes should be checked and, if necessary, the function should be called again with
  longer lobes.
  For optimal timing and lowest amplitudes, the
  lobe durations should be calculated with @ref MRT_FlowCompTimingRead.

  @image html MRT_FlowCompGradRead.png

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  Lobe durations must be higher than 2 ramps.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code

  double readFcGrad[2], lobe1, lobe2, readDur;

  encodingDuration  = 5.0;  // 5ms for both lobes

  readDur =  PVM_AcquisitionTime*PVM_EchoPosition/100 ;

  MRT_FlowCompTimingRead(PVM_RampTime, encodingDuration,
                         readDur, &lobe1, &lobe2);

  MRT_FlowCompGradRead(PVM_RampTime, lobe1, lobe2,
                       readDur,  ReadGrad, readFcGrad);

  @endcode

  @param ramp        Duration of gradient ramp.
  @param lobe1dur    Duration of first dephasing lobe (inlcuding ramps).
  @param lobe2dur    Duration of second dephasing lobe (inlcuding ramps).
  @param readDur     Duration of readout plateau from end of ramp till echo.
  @param readGrad    Amplitude of readout gradient.
  @param[out] compGrad Calculated lobe amplitudes (array of 2 elements) in the same units as readGrad.

  @ingroup PVM_MRT
*/

DLL_INTERFACE void  MRT_FlowCompGradRead(double ramp,
                                         double lobe1dur,
                                         double lobe2dur,
                                         double readDur,
                                         double readGrad,
                                         double *compGrad);




/** Calculates flow-compensated slice selection gradient amplitudes.

  Calculates the amplitudes of the two rephasing lobes of the slice selection gradient
  so as to obtain zero integral and zero first moment after the centre of the RF pulse.
  It is assumed that the selection gradient and the rephasing lobes are adjacent
  and have identical ramps (see picture below). Gradient delay is not considered in the calculation.
  It may be taken into account in the effective duration of the pulse (see code example below).
  This function does not
  control the minimum duration of the lobes and may generate lobe amplitudes above 100%.
  Resulting amplitudes should be checked and, if necessary, the function should be called again with
  longer lobes.
  For optimal timing and lowest amplitudes, the
  lobe durations should be calculated with @ref MRT_FlowCompTimingSlice.

  @image html MRT_FlowCompGradSlice.png

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  Lobe durations must be higher than 2 ramps.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code

  double sliceFcGrad[2], lobe1, lobe2, pulseDur;

  double encodingDuration  = 5.0;  // 5ms for both lobes

  pulseDur =  ExcPulse.Length*ExcPulse.RephaseFactor/100 + CFG_GradientDelayTime();

  MRT_FlowCompTimingSlice(PVM_RampTime, encodingDuration,
                         pulseDur, &lobe1, &lobe2);

  MRT_FlowCompGradSlice(PVM_RampTime, lobe1, lobe2,
                       pulseDur,  SliceGrad, sliceFcGrad);

  @endcode

  @param ramp        Duration of gradient ramp.
  @param lobe1dur    Duration of first dephasing lobe (inlcuding ramps).
  @param lobe2dur    Duration of second dephasing lobe (inlcuding ramps).
  @param pulseDur     Duration of slice selction plateau from pulse centre till ramp onset.
  @param sliceGrad    Amplitude of slice selection gradient.
  @param[out] compGrad Calculated lobe amplitudes (array of 2 elements) in the same units as sliceGrad.

  @ingroup PVM_MRT
*/
DLL_INTERFACE void  MRT_FlowCompGradSlice(double ramp,
                            double lobe1dur,
                            double lobe2dur,
                            double pulseDur,
                            double sliceGrad,
                            double *compGrad);




/** Calculates flow-compensated phase encoding gradient amplitudes.

  Calculates amplitudes of the two lobes of a bipolar phase encoding gradient pulse
  so as to achieve a given pulse integral while keeping the first moment (relative
  to the end of the pulse) zero.
  It is assumed that the lobes are adjacent
  and have identical ramps (see picture below). Gradient delay is not relevant for the calculation.
  This function does not
  control the minimum duration of the lobes and may generate lobe amplitudes above 100%.
  Resulting amplitudes should be checked and, if necessary, the function should be called again with
  longer lobes. For optimal timing and lowest amplitudes, the
  lobe durations should be calculated with @ref MRT_FlowCompTimingPhase.

  @image html MRT_FlowCompGradPhase.png

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  Lobe durations must be higher than 2 ramps.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code

  double phaseFcGrad[2], lobe1, lobe2;

  double encodingDuration  = 5.0;  // 5ms for both lobes

  MRT_FlowCompTimingPhase(PVM_RampTime, encodingDuration, &lobe1, &lobe2);

  // using gradients in units of % of max:
  phaseIntegral = 1e5/(2*PVM_GradCalConst*PVM_SpatResol[1]);

  MRT_FlowCompGradPhase(ramp, l1p, l2p, phaseIntegral, phaseFcGrad);

  // phaseFcGrad[] is now in units of %max and can be directly propagated
  // to ACQ_gradient_amplitude[]

  @endcode

  @param ramp        Duration of gradient ramp.
  @param lobe1dur    Duration of first dephasing lobe (inlcuding ramps).
  @param lobe2dur    Duration of second dephasing lobe (inlcuding ramps).
  @param integral    Integral of the pulse (both lobes) in units of lobeDur_unit x compGrad_unit.
  @param[out] compGrad Calculated lobe amplitudes (array of 2 elements).

  @ingroup PVM_MRT
*/
DLL_INTERFACE void    MRT_FlowCompGradPhase(double ramp,
                                            double lobe1dur,
                                            double lobe2dur,
                                            double integral,
                                            double *compGrad);




/** Calculates optimum timing of a flow-compensated readout gradient pulse.

  Calculates durations of the the two lobes of a flow compensated readout dephasing pulse
  that will give lowest gradient amplitudes for a given dephasing time.
  It is asumed that the lobes and the readout gradient
  puls are adjacent and have identical ramps.
  This fuction is typically called before  @ref MRT_FlowCompGradRead.

  @image html MRT_FlowCompTimingRead.png

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  Lobe durations must be higher than 2 ramps.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @see code example for @ref MRT_FlowCompGradRead


  @param ramp        Duration of gradient ramp.
  @param encDur      Duration of both lobes.
  @param readDur     Duration of readout plateau from end of ramp till echo.
  @param[out] lobe1  Pointer to the calculated amplitude of first lobe.
  @param[out] lobe2  Pointer to the calculated amplitude of second lobe.

  @ingroup PVM_MRT
*/
DLL_INTERFACE void  MRT_FlowCompTimingRead(double ramp,
                                           double encDur,
                                           double readDur,
                                           double *lobe1,
                                           double *lobe2);




/** Calculates optimum timing of a flow-compensated slice selection gradient pulse.

  Calculates durations of the the two lobes of a flow-compensated slice rephasing pulse
  that will give lowest gradient amplitudes for a given rephasing time.
  It is asumed that the slection gradient
  pulse and the lobes are adjacent and have identical ramps.
  This fuction is typically called before @ref MRT_FlowCompGradSlice.

  @image html MRT_FlowCompTimingSlice.png

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  Lobe durations must be higher than 2 ramps.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @see code example for @ref MRT_FlowCompGradSlice


  @param ramp        Duration of gradient ramp.
  @param encDur      Duration of both lobes.
  @param pulseDur    Duration of slice gradient plateau from Rf pulse centre till ramp.
  @param[out] lobe1  Pointer to the calculated amplitude of first lobe.
  @param[out] lobe2  Pointer to the calculated amplitude of second lobe.

  @ingroup PVM_MRT
*/
DLL_INTERFACE void MRT_FlowCompTimingSlice(double ramp,
                                           double encDur,
                                           double pulseDur,
                                           double *lobe1,
                                           double *lobe2);




/** Calculates optimum timing of a flow-compensated phase encoding gradient pulse.

  Calculates durations of the the two lobes of a flow-compensated phase encoding pulse
  that will give lowest gradient amplitudes for a given encoding time.
  It is asumed that the lobes are adjacent and have identical ramps.
  This fuction is typically called before @ref MRT_FlowCompGradPhase.

  @image html MRT_FlowCompTimingPhase.png

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  Lobe durations must be higher than 2 ramps.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @see code example for @ref MRT_FlowCompGradPhase


  @param ramp        Duration of gradient ramp.
  @param encDur      Duration of both lobes lobe.
  @param[out] lobe1  Pointer to the calculated amplitude of first lobe.
  @param[out] lobe2  Pointer to the calculated amplitude of second lobe.

  @ingroup PVM_MRT
*/
DLL_INTERFACE void  MRT_FlowCompTimingPhase(double ramp,
                                            double encDur,
                                            double *lobe1,
                                            double *lobe2);




/** Calculates unit vectors distributed as charged pairs on unit sphere.

  Calculates unit vectors distributed as charged pairs on unit sphere,
  i.e. distance between points on unit sphere surface is minimized.

  As result the parameter (specified by name in argument dirparn) is
  dimensioned according to the desired number of directions (argument
  ndir) and the calculated directions are stored in this parameter.



  @PVM_PARS
  parameter specifiec by name

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  parameter name belongs to 2D Nx3 double array


  @PVM_ERROR
  - wrong type of parameter (specified by name in argument dirparn)
    only 2D arrays of 3-element vectors are allowed.
  - illegal range of ndir ( >0)

  @PVM_EXAMPLE
  This code is executed as default relation of parameter
  PVM_DwCalcDirs:

  @code
  if(ParxRelsParHasValue("PVM_DwNDiffDir") && PVM_DwNDiffDir > 0)
  {
    MRT_ChargedPairDirections("PVM_DwDir",PVM_DwNDiffDir,100);
    ParxRelsParRelations("PVM_DwDir",No);
  }

  @endcode

  @param dirparn   parameter name of 2D Nx3 parameter
  @param ndir      number of directions
  @param niter (optional) number of iterations (>= 100 <=10000)

  @ingroup PVM_MRT
*/

DLL_INTERFACE int MRT_ChargedPairDirections(const char *const dirparn,
                                            const int ndir,
                                            const int niter=100);




/**************************************************************************/
/*            gen/src/prg/pvm_toolboxes/PvMrTools/MrAdjTools.c            */
/**************************************************************************/
/**
 * Add an adjustment chain to the list adjustment chains.
 * This routine can only be used inside of adjustments. The MR adjustment chain
 * defines which hardware configurations are used to define the context for
 *  an adjustment. If a hardware configuration defined here changes the
 * corresponding adjustment is repeated.
 *
 * @PVM_PARS
 * PVM_ScanInstrAdjChainDefs, AdjCurChainDefs
 *
 * @PVM_TBCALL
 * NONE
 *
 * @PVM_RELCALL
 * NONE
 *
 * @PVM_PRECOND
 * Function can only called in an adjustment method or in a method
 * that can be used as scan instruction adjustment.
 *
 *
 * @PVM_ERROR
 * NONE
 *
 * @PVM_EXAMPLE
 * This code is executed as default relation of parameter
 * PVM_DwCalcDirs:
 *
 * @code
 * MRT_AdjSetHwScope(SHIM_CHAIN,0)
 * @endcode
 *
 * @param hwScope The hardware scope.
 * @param chanNum The channel if applicable else 0.
 * @param isScanInstr Indicated if this is a scan instruction.
 * @throws PvException in case of an error.
 */
DLL_INTERFACE void MRT_AdjSetHwScope(HW_SCOPE_ENUM hwScope, int chanNum,
                                     YesNo isScanInstr = No);


/**
 * Get an adjustment chain.
 * @param hwScope The hardware scope.
 * @param chanNum The logical channel number [1...N], if applicable.
 * @return the adjustment chain definition and its context.
 * @throws PvException in case of an error.
 */
DLL_INTERFACE AdjChainContext MRT_AdjGetHwScope(const HW_SCOPE_ENUM hwScope,
                                                const int chanNum = -1);


/** Calculates shim vector (units in A).

Calculates a shim vector using a shim field matrix, an object field and
hardware specifications specified by argument.



  @PVM_PARS
  NONE

  @PVM_TBCALL
  Numerical routines:
  - PvnlLinalgSolvPinvSvdRealSV
  - PvnlLinalgPinvSvdVSUReal
  - PvnlMatrixNew
  - PvnlMatrixAttachMem
  - PvnlMatrixGenIndex
  - PvnlMatrixWriteFileAsc
  - PvnlMatrixWriteFileBin

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  correct size and structure of arguments

  @PVM_ERROR
  - error states of numerical functions
  - memory allocation failures

  @PVM_EXAMPLE

  @code

      failed=MRT_CalcShimVector(ScAmpMat,
                              ScAmpMatSD,
                              nfieldvals,
                              usedShims,
                              objFieldvals,
                              shimAmpVec,
                              maxAmp,
                              totCurrConstr,
                              sumConstr,
                              totCurr,
                              nsps,
                              ntotCurrConstr,
                              logdir);



  @endcode

  All matrices (double ** arguments) have to be in column major order

  @param ScMat       shim field matrix NFieldvals x NCoils shim fields per unit current within shim area
  @param ScMatSD     standard deviation of shim fields (NFieldvals x NCoils)
  @param NFieldvals  number of field values within shim area
  @param NCoils      number of shim coils
  @param ObjField    field of measurement object (with zero shim) size: NFieldvals
  @param ShimVals    solution shim vector (currents in A) size: NCoils
  @param Imax        maximum shim current of coils (in A) size: NCoils
  @param SumConstr   consider maximum current
  @param Csum        array containing shim power supply coils (Csum[coilindex] = SPS index), size NCoils
  @param TotCurr     total current that can be supplied by shim power supply, size Nsps
  @param Nsps        Number of shim power supplies
  @param NSumConstr  number of sum constrained shims
  @param logpath     (optional) - defunct

  @ingroup PVM_MRT_INT
*/
DLL_INTERFACE int MRT_CalcShimVector(double **ScMat,
                                     double **ScMatSD,
                                     int NFieldvals,
                                     int NCoils,
                                     double *ObjField,
                                     double *ShimVals,
                                     double *Imax,
                                     bool SumConstr,
                                     int *Csum,
                                     double *TotCurr,
                                     int Nsps,
                                     int NSumConstr,
                                     const char *const logpath=0);
/** Calculates shim vector (units in A).

Calculates a shim vector using a shim field matrix, an object field and
hardware specifications specified by argument.



  @PVM_PARS
  NONE

  @PVM_TBCALL
  Numerical routines:
  - PvnlLinalgSolvPinvSvdRealSV
  - PvnlLinalgPinvSvdVSUReal
  - PvnlMatrixNew
  - PvnlMatrixAttachMem
  - PvnlMatrixGenIndex
  - PvnlMatrixWriteFileAsc
  - PvnlMatrixWriteFileBin

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  correct size and structure of arguments

  @PVM_ERROR
  - error states of numerical functions
  - memory allocation failures

  @PVM_EXAMPLE

  @code

      failed=MRT_CalcShimVector(ScAmpMat,
                              ScAmpMatSD,
                              nfieldvals,
                              usedShims,
                              objFieldvals,
                              shimAmpVec,
                              maxAmp,
                              totCurrConstr,
                              sumConstr,
                              totCurr,
                              nsps,
                              ntotCurrConstr,
                              logdir);



  @endcode

  Storage model of all matrices has to be in column major order

  @param ScMat       shim field matrix NFieldvals x NCoils shim fields per unit current within shim area
  @param ScMatSD     standard deviation of shim fields (NFieldvals x NCoils)
  @param NFieldvals  number of field values within shim area
  @param NCoils      number of shim coils
  @param ObjField    field of measurement object (with zero shim) size: NFieldvals
  @param ShimVals    solution shim vector (currents in A) size: NCoils
  @param Imax        maximum shim current of coils (in A) size: NCoils
  @param SumConstr   consider maximum current
  @param Csum        array containing shim power supply coils (Csum[coilindex] = SPS index), size NCoils
  @param TotCurr     total current that can be supplied by shim power supply, size Nsps
  @param Nsps        Number of shim power supplies
  @param message     (optional) if !=0 additional information about result is stored in argument message

  @ingroup PVM_MRT_INT
*/
 DLL_INTERFACE int MRT_CalcShimVector(double *ScMat,
        double *ScMatSD,
        int NFieldvals,
        int NCoils,
        double *ObjField,
        double *ShimVals,
        double *Imax,
        bool SumConstr,
        int *Csum,
        double *TotCurr,
        int Nsps,
        std::string *message=0);

/**************************************************************************/
/*             gen/src/prg/pvm_toolboxes/mr/PvMrTools/NucPpm.c            */
/**************************************************************************/
/** Initialise ppm values of X-nuclei reference frequencies.

  @PVM_PARS
  - @c PVM_NucleiPpmRef

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref MRT_NucleiPpmWorkInit, @ref MRT_NucleiPpmRefSet,
       @ref MRT_NucleiPpmWorkSet, @ref MRT_NucleiPpmRefGet, @ref MRT_NucleiPpmWorkGet
  @ingroup PVM_MRT
 */
DLL_INTERFACE void MRT_NucleiPpmRefInit(void);




/** Initialise ppm values of X-nuclei working frequencies.

  @PVM_PARS
  - @c PVM_NucleiPpmWork

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref MRT_NucleiPpmRefInit, @ref MRT_NucleiPpmRefSet,
       @ref MRT_NucleiPpmWorkSet, @ref MRT_NucleiPpmRefGet, @ref MRT_NucleiPpmWorkGet

  @ingroup PVM_MRT
 */
DLL_INTERFACE void MRT_NucleiPpmWorkInit(void);




/** Set reference ppm value for a nucleus.

  Store @a ppmRef in the @c PVM_NucleiPpmRef array in the element
  corresponding to the nucleus; if it does not exist, store the
  nucleus name and @a ppmRef at the first entry whose name is "none".

  @PVM_PARS
  - @c PVM_NucleiPpmRef

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
  MRT_NucleiPpmRefSet(PVM_Nucleus1, PVM_FrqRefPpm[0]);
  @endcode

  @param[in] nucleusName Name of nucleus
  @param     ppmRef      Reference frequency [ppm]

  @return @c 0 = ok, else an error occurred (table is full or nucleus not found)

  @see @ref MRT_NucleiPpmRefInit, @ref MRT_NucleiPpmWorkInit,
       @ref MRT_NucleiPpmWorkSet, @ref MRT_NucleiPpmRefGet, @ref MRT_NucleiPpmWorkGet

  @ingroup PVM_MRT
 */
DLL_INTERFACE int MRT_NucleiPpmRefSet(const char *nucleusName,
                                      double ppmRef);




/** Set working ppm value for a nucleus

  Store @a ppmWork in the @c PVM_NucleiPpmWork array in the element
  corresponding to the nucleus; if it does not exist, store the
  nucleus name and @a ppmRef at the first entry whose name is "none".
  If all entries are used, set the last one.

  @PVM_PARS
  - @c PVM_NucleiPpmWork

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
  MRT_NucleiPpmWorkSet(PVM_Nucleus1, PVM_FrqRefPpm[0] + PVM_FrqWorkOffsetPpm[0]);
  @endcode

  @param[in] nucleusName Name of nucleus
  @param     ppmWork     Working frequency [ppm]

  @return @c 0 = ok, else an error occurred

  @see @ref MRT_NucleiPpmRefInit, @ref MRT_NucleiPpmWorkInit, @ref MRT_NucleiPpmRefSet,
       @ref MRT_NucleiPpmRefGet, @ref MRT_NucleiPpmWorkGet

  @ingroup PVM_MRT
 */
DLL_INTERFACE int MRT_NucleiPpmWorkSet(const char *nucleusName,
    double ppmWork);




/** Get reference ppm value for a nucleus.

  Find first entry corresponding to @a nucleusName in @c PVM_NucleiPpmTable
  and get reference and working ppm values from it.

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
  MRT_NucleiPpmRefGet(PVM_Nucleus1, &PVM_FrqRefPpm[0]);
  @endcode

  @param[in]  nucleusName Name of nucleus
  @param[out] ppmRef      Reference frequency [ppm]

  @see @ref MRT_NucleiPpmRefInit, @ref MRT_NucleiPpmWorkInit, @ref MRT_NucleiPpmRefSet,
       @ref MRT_NucleiPpmWorkSet, @ref MRT_NucleiPpmWorkGet

  @return @c 0 = ok, else nucleus was not found

  @ingroup PVM_MRT
 */
DLL_INTERFACE int MRT_NucleiPpmRefGet(const char *nucleusName,
    double *ppmRef);




/** Get working ppm value for a nucleus.

  Find first entry corresponding to @a nucleusName in @c PVM_NucleiPpmTable
  and get reference and working ppm values from it.

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
  MRT_NucleiPpmWorkGet(PVM_Nucleus1, &PVM_FrqWorkPpm[0]);
  @endcode

  @param[in]  nucleusName Name of nucleus
  @param[out] ppmWork     Working frequency [ppm]

  @see @ref MRT_NucleiPpmRefInit, @ref MRT_NucleiPpmWorkInit, @ref MRT_NucleiPpmRefSet,
       @ref MRT_NucleiPpmWorkSet, @ref MRT_NucleiPpmRefGet

  @return @c 0 = ok, else nucleus was not found

  @ingroup PVM_MRT
 */
DLL_INTERFACE int MRT_NucleiPpmWorkGet(const char *nucleusName,
    double *ppmWork);




/** Sets the Id of the echo loop in post processing parameters.

  CAUTION: After setting the Id the loop is not an echo loop anymore.

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
  MRT_VisuSetEchoLoopId(PV_IDL_CONSTANT(visuid_framegroup_MOVIE));
  @endcode

  @param[in] id    Id of the echo loop

  @return @c 0 if everything is ok else @c -1

@see @ref PTB_VisuSetLoopId,
     @ref MRT_VisuSetMovieLoopId, @ref PTB_VisuSetLoopName, @ref MRT_VisuSetEchoLoopName,
     @ref MRT_VisuSetMovieLoopName, @ref PTB_VisuSetLoopDependentParameter,
     @ref PTB_VisuRemoveLoopDependentParameter, @ref PTB_VisuInsertLoop

  @ingroup PVM_MRT
 */
DLL_INTERFACE int MRT_VisuSetEchoLoopId(const char *id);





/** Sets the Id of the movie loop in post processing parameters.

  CAUTION: After setting the Id the loop is not a movie loop anymore.

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
  MRT_VisuSetMovieLoopId(PV_IDL_CONSTANT(visuid_framegroup_DIFFUSION));
  @endcode

  @param[in] id    Id of the movie loop

  @return @c 0 if everything is ok else @c -1

@see @ref PTB_VisuSetLoopId, @ref MRT_VisuSetEchoLoopId,
     @ref PTB_VisuSetLoopName, @ref MRT_VisuSetEchoLoopName,
     @ref MRT_VisuSetMovieLoopName, @ref PTB_VisuSetLoopDependentParameter,
     @ref PTB_VisuRemoveLoopDependentParameter, @ref PTB_VisuInsertLoop

  @ingroup PVM_MRT
 */
DLL_INTERFACE int MRT_VisuSetMovieLoopId(const char *id);




/** Sets the name/comment of the echo loop in post processing parameters.

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
  MRT_VisuSetEchoLoopName("Inversion");
  @endcode

  @param[in] name    Name/comment of the echo loop

  @return @c 0 if everything is ok else @c -1

@see @ref PTB_VisuSetLoopId, @ref MRT_VisuSetEchoLoopId,
     @ref MRT_VisuSetMovieLoopId, @ref PTB_VisuSetLoopName,
     @ref MRT_VisuSetMovieLoopName, @ref PTB_VisuSetLoopDependentParameter,
     @ref PTB_VisuRemoveLoopDependentParameter, @ref PTB_VisuInsertLoop

  @ingroup PVM_MRT
 */
DLL_INTERFACE int MRT_VisuSetEchoLoopName(const char *name);





/** Sets the name/comment of the movie loop in post processing parameters.

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
  MRT_VisuSetMovieLoopName("Inversion");
  @endcode

  @param[in] name    Name/comment of the movie loop

  @return @c 0 if everything is ok else @c -1

@see @ref PTB_VisuSetLoopId, @ref MRT_VisuSetEchoLoopId,
     @ref MRT_VisuSetMovieLoopId, @ref PTB_VisuSetLoopName,
     @ref MRT_VisuSetEchoLoopName, @ref PTB_VisuSetLoopDependentParameter,
     @ref PTB_VisuRemoveLoopDependentParameter, @ref PTB_VisuInsertLoop

  @ingroup PVM_MRT
 */
DLL_INTERFACE int MRT_VisuSetMovieLoopName(const char *name);





/** Derive the VISU diffusion parameters for diffusion datasets.

  The VISU movie loop is divided into a a diffusion (@a ndiff elements)
  and cycle (@a nrep element) loop. The parameter VISU element comments are
  adapted for the diffusion loop. The parameter @c VisuAcqDiffusionBMatrix is
  set to the @c PVM_DwBMat diffusion b-matrix and made dependent to the
  diffusion loop.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  A diffusion method had to be acquired.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  MRT_VisuDeriveDiffusionFrameGroup(PVM_DwNDiffExp, PVM_NRepetitions);
  @endcode

  @param ndiff  Number of diffusion scans.
  @param nrep   Number of repetitions.

  @return @c -1 on error, @c 0 for success.

  @ingroup PVM_MRT
 */
DLL_INTERFACE int MRT_VisuDeriveDiffusionFrameGroup(int ndiff, int nrep);

/** Returns versions of Mapshim.

  This function provides information about different versions of @em Mapshim 
  represented as array of strings.


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

  void backbone(void)
  {
  (...)
  std::vector<std::string> mapshim_versions = MRT_MapShimVersions();

  @endcode


  @see @ref MRT_MapShimVersion

  @ingroup PVM_MRT
*/
DLL_INTERFACE std::vector<std::string> MRT_MapShimVersions(void);

/** Returns actual version of Mapshim.

  This function provides information about the actual version of @em Mapshim 
  represented as strings.


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

  void backbone(void)

  (...)
  std::string mapshim_version = MRT_MapShimVersion();

  if( mapshim_version != PVM_ShimCoeffCalVersion )
  {
     // shim calibration does not belong to actual mapshim version
  }


  @endcode


  @see @ref MRT_MapShimVersions

  @ingroup PVM_MRT
*/
DLL_INTERFACE std::string MRT_MapShimVersion(void);

/**  MR-specific actions after load of method class .

  This function is called as post-read action for pargroup MethodClass
  (i.e. the method file has been read from expno directory).
  The MR-specific but method unspecific actions are implemented there.



  @PVM_PARS
  - e.g. adaption of geometry objects to new reference position
  - reactions on deprecated parameters

  @PVM_TBCALL
  PTB_PostReadMethodGroup

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

    // calls  MRT_PostReadMethodGroup(false);

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

  @see @ref CMN_PostReadMethodGroup

  @ingroup PVM_MRT

 */
DLL_INTERFACE void MRT_PostReadMethodGroup(bool readonly);

/**  MR-specific actions before a report is created .

  This function is called each time a report is created for a  MR 
  scan with status "Completed". Dependent on value of 
  PVM_ReportHandler (the name of the xml template) specific actions 
  can be performed.
  Redirection of this parameter on method level to a local function
  should react only on specific parts and default behavior should
  be established for all other cases.
 


  @PVM_PARS
  - dependend on xml template

  @PVM_TBCALL
  NONE 

  @PVM_PRECOND
  NONE
   

  @PVM_EXAMPLE
  @code
  // react on a specific template file on method level:
  // assuming a redirection of PVM_ReportHandler to myReportRel.

  void myReportRel(void)
  {
     // assure standard actions
     MRT_ReportHandler();

     // specific actions

     std::string act_template(PVM_ReportHandler);

     if(act_templaye=="My_SpecificReportfile.xml")
     {
        // implement specific actions
     }

  }


  @endcode


 
  @ingroup PVM_MRT

 */
DLL_INTERFACE void MRT_ReportHandler(void);






/* Definitions for deprecated functions. */
#ifdef METHCOMP

/** Deprecated function name for @ref MRT_SetSpectrocopyDims. */
#define PTB_SetSpectrocopyDims(lowestDim,highestDim) MRT_SetSpectrocopyDims(lowestDim,highestDim)

/** Deprecated function name for @ref MRT_InitSpecDim. */
#define PTB_InitSpecDim(lowestDim,highestDim) MRT_InitSpecDim(lowestDim,highestDim)

/** Deprecated function name for @ref MRT_SetSpecDim. */
#define PTB_SetSpecDim(dimensions) MRT_SetSpecDim(dimensions)

/** Deprecated function name for @ref MRT_GetSpecDim. */
#define PTB_GetSpecDim() MRT_GetSpecDim()

/** Deprecated function name for @ref MRT_VisuSetEchoLoopId. */
#define PTB_VisuSetEchoLoopId(id) MRT_VisuSetEchoLoopId((id))

/** Deprecated function name for @ref MRT_VisuSetMovieLoopId. */
#define PTB_VisuSetMovieLoopId(id) MRT_VisuSetMovieLoopId((id))

/** Deprecated function name for @ref MRT_VisuSetEchoLoopName. */
#define PTB_VisuSetEchoLoopName(name) MRT_VisuSetEchoLoopName((name))

/** Deprecated function name for @ref MRT_VisuSetMovieLoopName. */
#define PTB_VisuSetMovieLoopName(name) MRT_VisuSetMovieLoopName((name))

/** Deprecated function name for @ref MRT_NucleiPpmRefInit. */
#define PTB_NucleiPpmRefInit() MRT_NucleiPpmRefInit()

/** Deprecated function name for @ref MRT_NucleiPpmRefSet. */
#define PTB_NucleiPpmRefSet(nucleusName,ppmRef) MRT_NucleiPpmRefSet(nucleusName,ppmRef)

/** Deprecated function name for @ref MRT_GetShapeHandlerIndex. */
#define UT_GetShapeHandlerIndex(parname, errstate) MRT_GetShapeHandlerIndex(parname, errstate)
#endif


#endif
#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
