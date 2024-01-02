/*
 *******************************************************************
 *
 * $Source: /sc/CvsTree/sc/gen/h/acqutyp.h,v $
 *
 * Copyright (c) 1997
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 * $Id: acqutyp.h,v 1.177 2019/06/06 05:25:00 chjo Exp $
 *******************************************************************
 */

#pragma once

/****************************************************************/
/*	TYPEDEF's                                               */
/****************************************************************/
/* All non PARX conform code has to be surrounded by
 *
 * #ifndef InParxOvl
 * < your code >
 * #endif
 *
 * i.e. all except:
 * - simple enum typedefs without values
 * - enum classes (NOTE: enum classes are parsed as simple enums, hence names have to be unique!)
 * - simple struct typedefs including only 'char', 'int' and 'double' members
 */

#ifdef	__cplusplus

#ifndef InParxOvl
// flag which determines how multiple complex decoding is performed
enum  class ACQ_FN_TYPE {FN_UNKNOWN, FN_TRAD, FN_POINTS, FN_SPARSE, FN_PROJR};
enum  class ACQ_FN_MODE {UNDEFINED, QF, QSEQ, TPPI, STATES, STATES_TPPI, ECHO_ANTIECHO, PSEUDO};

class IntToFnType
{
public:

    static ACQ_FN_TYPE convert(const int val)
    {
	switch (val)
	{
	case 0: return ACQ_FN_TYPE::FN_TRAD;
	case 1: return ACQ_FN_TYPE::FN_POINTS;
	case 2: return ACQ_FN_TYPE::FN_SPARSE;
	case 3: return ACQ_FN_TYPE::FN_PROJR;
	default:return ACQ_FN_TYPE::FN_UNKNOWN;
	}
    }

    IntToFnType() = delete;

};

class IntToFnMode
{
public:

    static ACQ_FN_MODE convert(const int val)
    {
	switch (val)
	{
	case 0: return ACQ_FN_MODE::UNDEFINED;
	case 1: return ACQ_FN_MODE::QF;
	case 2: return ACQ_FN_MODE::QSEQ;
	case 3: return ACQ_FN_MODE::TPPI;
	case 4: return ACQ_FN_MODE::STATES;
	case 5: return ACQ_FN_MODE::STATES_TPPI;
	case 6: return ACQ_FN_MODE::ECHO_ANTIECHO;
	case 7: return ACQ_FN_MODE::PSEUDO;
	default:return ACQ_FN_MODE::UNDEFINED;
	}
    }

    IntToFnMode() = delete;

};


class FnModeUtil
{
public:
    /**
     * \retval true for QF and Pseudo
     *
     * Pseudo behaves the same as QF, see TOPS-1168
     */
    static bool hasLoopStructureLikeQf(ACQ_FN_MODE fnMode)
    {
	switch (fnMode)
	{
	    case ACQ_FN_MODE::QF: // fall through
	    case ACQ_FN_MODE::PSEUDO:
		return true;

	    default:
		return false;
	}
	return false;
    }

};

#endif

#endif



/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type ACQ_EXPERIMENT_MODE

  @ptypePurpose  Indicates, whether the experiment is a
                 single or multiple experiment. Multiple
                 experiments are executed parallel or interleaved.
    SingleExperiment: Only one experiment is executed.
    MultipleReceiverExperiment:
      Multiple experiments acquired by a multiple reciever system.
    ParallelExperiment:
      Single experiment with multiple receivers operating in parallel.
    MpiExperiment:
      Special experiment type for performing MPI acquisition.
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  SingleExperiment,
  MultipleReceiverExperiment,
  ParallelExperiment,
  MpiExperiment
}ACQ_EXPERIMENT_MODE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type ACQ_EXPERIMENT_TYPE

  @ptypePurpose  Describes an specific experiment,
		 e.g. in case of multi experiment acquisition.
                 @see ACQ_experiment_mode
    expObjName[1024]: Object name of the experiment
    usedPulseShapes:  Number of the used pulse shapes.
    expRepetition:    Number of subsequent scans, belonging
		      to the acutal experiment
		      (obsolete)
    expRcuNum:        Associated RCU (MultipleReceiverExperiment only)
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef struct
{
  char   expObjName[1024];
  int	 usedPulseShapes;
  int    expRepetition;
  int	 expRcuNum;
}ACQ_EXPERIMENT_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type ACQ_BF_TYPE

  @ptypePurpose   Struct containing the nucleus name and the
                  corresponding basic frequency.
    char nucleus[8]:  Nucleus name, e.g. 1H, 31P, 13C, ..
    double frequency: Corresponding basic frequency in MHz
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef struct
{
  char nucleus[8];
  double frequency;
}ACQ_BF_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SHAPED_PULSE

  @ptypePurpose  Specifies the type of a shaped rf pulse (deprecated)
     name:   Name of the pulse shape file in the
	     ../lists/wave directory
     power:  The power of the pulse in db
     offset: Frequency offset of the pulse in Hz
  @ptypeValidRange
    power:       -6, .., 150
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef struct
{
  char name[64];
  double power;
  double offset;
}SHAPED_PULSE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type ACQSHAPE

  @ptypePurpose  Specifies the type of a shaped rf pulse
     name:       Name of pulse shape file (e.g. "bp") within
                   ../lists/wave directory (e.g. "bp" or
                   ../lists/wave/user
                   ( <TSHOME>/prog/curdir/<User>/parfile-dirs.prop
                     contains "SHAPEDIRS=lists/wave;lists/wave/users") or
                 Name of corresponding PARX parameter if
                   '$' prefix is given (e.g. "$ACQ_bp")

                 In case of parallel pulses (parallel = 1),
                    individual shape files with extension .1, .., .8 (e.g. "bp.1", .., "bp.8")
                    are used instead. If the individual file does not exit, the global one
                    without extension (e.g. "bp") is used.
                 or
                    the specified PARX_parameter may be a 2 dimensional double array,
                    containing different shape values for every channel:
                    e.g. double ACQ_bp[8][n]
                         the slow index specifies pexmux channels 1,..,8 and
                         the fast index specifies shape_values 1,..,n.
     powerWatt:  The power of the pulse in Watt
     offset:     Frequency offset of the pulse in Hz
     alignment:  Position of phase 0 phase for offset
     parallel:   Flag to select parallel mode
     pex_amp_scaling:  Array to scale the corresponding pexmux channel.
                       pex_amp_scaling[n]=0.0 means, pexmux channel n+1 is not used at all.
     pex_phase_offset: Array to specify individual phase offsets for every pexmux channel.

  @ptypeValidRange
    powerWatt:       0, .., 100000.0  (maximal 100 KW)
    alignment:       0, .., 1.0
    pex_amp_scaling  0, .., 100 %
    pex_phase_offset   -180, .., 180 deg
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

#define PEXMUX_MAX 24 // Currently one pexmux supports maximal 8 channels

typedef struct
{
  char name[64];
  double powerWatt;
  double offset;
  double alignment;
  int parallel;
  double pex_amp_scaling[PEXMUX_MAX];
  double pex_phase_offset[PEXMUX_MAX];
} ACQSHAPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type PRGAIN_TYPE

  @ptypePurpose Specifies the  preamplifier gain = high or low
                Relevance for some old spectrometers only.
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  PG_high,
  PG_low
} PRGAIN_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SCAN_SIZE_TYPE

  @ptypePurpose  Specifies, how many scans are acquired in
		 1 sweep by the digitizer
    One_scan:               1 sweep = 1 scan = ACQ_size[0] points
    ACQ_phase_factor_scans: 1 sweep = ACQ_phaseFactor scans
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  One_scan,
  ACQ_phase_factor_scans
}SCAN_SIZE_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SHIM_UNIT_TYPE

  @ptypePurpose  Specifies the type of the shim unit
    TOSI:	Tomography Shim Interface using BSMS-protocol.
                AV3: Add linear Shims via DPP to corr. gradients.
    BSMS:       BSMS unit
    BSMS2:      New BSMS unit (hasELCB)
    GRASI:      GRadient And/or Shim Interface
    GTCUBE:	AV4 shims based on GtCube
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  NO_SHIM,
  TOSI,
  BSMS,
  BSMS2,
  MINISPEC,
  GRASI,
  OTHER_SHIM,
  GTCUBE
} SHIM_UNIT_TYPE;

/*--------------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SHIM_UNIT_SEL_TYPE

  @ptypePurpose  Specifies the type of the shim unit selected
    ShimUnitSel_GtCube: shim via GtCube
    ShimUnitSel_Bsms2:  shim via Bsms2
    ShimUnitSel_Other:  Shim via other shim unit
  @ptypeAuthor  MAH                                             */
/*--------------------------------------------------------------*/

typedef enum
{
  ShimUnitSel_GtCube,
  ShimUnitSel_Bsms2,
  ShimUnitSel_Other
} SHIM_UNIT_SEL_TYPE;


/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type INSTRUMENT_TYPE

  @ptypePurpose  Specifies the type of the spectrometer:
     Legacy_Instrument Old instrument, no longer supported
     Avance_II         Avance II system  (TCU/FCU/SGU/DRU/GCU)
     Avance_III        Avance III system (IPSO/SGU2/DRU)

     Avance_I will be merged with Legacy_Instrument soon.
  @ptypeAuthor  MAH/UHEI                                    */
/*------------------------------------------------------*/

typedef enum
{
  Legacy_Instrument,
  Avance_II,
  Avance_III
} INSTRUMENT_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type RF_PULSE_TYP

  @ptypePurpose  Specifies the typ of the preferred rf pulse
    Shaped_Pulse:Pulse is shaped, e.g. by a gauss-curve
		 -> small band width
    Hard_Pulse:  Pulse isn't shaped = rectangulare pulse,
		 -> great band width
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  Shaped_Pulse,
  Hard_Pulse
} RF_PULSE_TYP;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type BYTORDA_TYP

  @ptypePurpose  Specifies in which byte order
		 data is stored in the ser or fid file
    little: The most significant byte within a word is put last
    big:    The most significant byte within a word is put first
		 -> great band width
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  little,
  big
} BYTORDA_TYP;
/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type DTYPA_TYP

  @ptypePurpose  Specifies the type of acquisition data.
    Int:         32 bit Integer data
    Float:       32 bit Float data
    Double:      64 bit Float data
		 data is stored in the ser or fid file
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  Int,
  Float,
  Double
} DTYPA_TYP;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type PREEMP_HARDWARE_TYPE

  @ptypePurpose
    Specifies type of Preemphasis Gradient Unit:
      No_gradients:    No Gradient Unit is installed at all
      BGU_II:          BGU II     Gradient Unit
      GREAT_60:        GREAT 3/60 Gradient unit
      DPP:             Digital Preemphasis
      DPP_with_SW_BLanking: Digital Preemhasis and a Gradient-
                       Unit, that may be blanked by the DPP
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  No_gradients,
  BGU_II,
  GREAT_60,
  DPP,
  DPP_with_SW_Blanking,
  Other_Preemp
} PREEMP_HARDWARE_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type AQ_mod_TYPE

  @ptypePurpose
    Specifies the default acquisition mode (TopSpin acquisition parameter 'AQ_mod').
      qf:   quad off = no quadrature detection.
	    The data come from only one detector, are
	    digitized, and continously stored in memory
      qsim: simultaneous quadrature detection.
	    The signal goes through two detectors, which
	    have reference frequencies differing by 90
	    degrees. The ADC simultaneously digitizes the
	    data from the 1st and 2nd detector.
      qseq: sequential quadratus detection.
	    Like qsim, but the ADC alternatingly digitizes
	    the data from the 1st and 2nd detector.
      qdig: Like qsim, but allowing digital filtering. Called 'DQD' in TopSpin
	    (AVANCE spectrometer, equipped with DSP-filters)
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  qf,
  qsim,
  qseq,
  qdig
  // parallelQsim == 4 is obsolete, was only experimental
  // parallelDQD == 5 is obsolete, was only experimental
} AQ_mod_TYPE;

/* moved to acqumtyp.h because of conflict with string class
typedef enum
{
  _CPD,
  _BB,
  _CW,
  _HG,
  _DO,
  _HD,
  _HB,
  _PO
} DECSTAT_TYPE;
*/

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SEOUT_TYPE

  @ptypePurpose
    Specifies the SE451 receiver output used.
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  High_resolution,
  Broad_band
} SEOUT_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type DIGTYP_TYPE

  @ptypePurpose
    Specifies the type of the digitizer, to be used.
      SLOW:          12 bits, 3 us dwell time
      _16BIT:        16 bits, 4 us
      FAST:           9 bits, 0.1/0.05 us
      _BC132_12:     12 bits, 0.1 us
      _BC132_16:     16 bits, 2 us
      _FADC:         12 bits, 0.05 us
      _HADC_HRD16:   12 bits, 2.5 us  ??
      _SADC:         16 bits, 3.325 us
      _HADC_HRD16_2: 16 bits, 3.325 us
      _SADC_2:       16 bits, 3.325 us
      HRD16:         16 bitts, 2.5 us ??
      IADC:          16 bits, 0.1/0.05 us
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  SLOW,
  _16BIT,
  FAST,
  _BC132_12,
  _BC132_16,
  _FADC,
  _HADC_HRD16,
  _SADC,
  _HADC_HRD16_2,
  _SADC_2,
  HRD16,
  IADC,
  DRU,
  DigType_DRMS,
  DigType_DRMS2,
  DigType_DRMS3,
  DigType_DRMS4,
  DigType_Cube
} DIGTYP_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type DSPFIRM_TYPE

  @ptypePurpose
    Specifies, which FW (FirmWare) is used for digital
    filtering.
    The RCU of all AVANCE systems is equipped with 2 DSP's
    (Digital Signal Processors), which can perform digital
    filtering during acquisition. Before the acquisition
    ist started, the FW is load from a disk file into the DSP.

    DSP_standard      : Standard filter
    DSP_user_defined, : Filtering via DSP-FW, located in
			~/exp/stan/nmr/lists/DSPFIRM
    DSP_smooth: Filter for imaging and solids
			experiments with less components
	                -> less garbage points necessary.
    DSP_medium:         Medium filter.
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/


typedef enum
{
  DSP_standard,
  DSP_bypass3,
  DSP_smooth,
  DSP_medium,
  DSP_rectangle
} DSPFIRM_TYPE;


/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type DIGMOD_TYPE

  @ptypePurpose
    Specifies Specifies the default digitizer mode.
      analog_mode:           Digital Filtering is disabled
      digital_mode:          Digital Filtering is enabled
      baseopt_solids:	     Digital filtering with baseline optimisation for solids
      baseopt:		     Digital filtering with baseline optimisation
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
    obsolete_analog,
    digital_mode,
    obsolete_baseopt_solids,
    baseopt_mode
} DIGMOD_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type DQDMODE_TYPE

  @ptypePurpose
    Specifies the frequency shift applied in Digital
    Quadrature Detection mode
      DQDMODE_add: Positive shift
      DQDMODE_sub: Negative shift
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  DQDMODE_add,
  DQDMODE_sub
} DQDMODE_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type CONFIG_STANDARD_DIGITIZER_TYPE

  @ptypePurpose
    Specifies the type of the standard digitizer.
    Relevance for digital (AVANCE) spectrometers only.
    All digitizer are 16 bit wide.
      No_standard_digitizer: No standard digitizer installed
      HADC_bbbbbb:           Highresolution Analog Digital Converter,
			     where bbbbbb = bandwidth of digitizer
      SADC_bbbbbb:           Slow ADC
      IADC_bbbbbb:           ADC, designed for Imaging
      xxxx_bbbbbb_2:         Digitizer, using other analog filters
      HRD16_400kHz:          Old 16 bit digitizer
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  No_standard_digitizer,
  HADC_400kHz,
  SADC_300kHz,
  HADC_400kHz_2,
  SADC_300kHz_2,
  HRD16_400kHz,
  HADC_2MHz,
  IADC_2MHz,
  IADC_4MHz,
  RXAD_20MHz
} CONFIG_STANDARD_DIGITIZER_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type CONFIG_MASTER_CLOCK_TYPE

  @ptypePurpose
    Specifies the Process Controller master clock.
    Relevance only for MSLX spectrometers.
      Clock_is_40_MHz: 40 MHz master clock
      Clock_is_20_MHz: 20 MHz master clock
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{ Clock_is_40_MHz,
  Clock_is_20_MHz
} CONFIG_MASTER_CLOCK_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type DIGITIZER_TYPE

  @ptypePurpose
    Specifies the type of the digitizer within ParaVision
      SLOW_12BIT:    12 bits, 3 us dwell time??
      SLOW_16BIT:    16 bits, 4 us        ??
      FAST_9BIT:      9 bits, 0.1/0.05 us ??
      FAST_12BIT:    12 bits, 0.1 us      ??
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  SLOW_12BIT,
  SLOW_16BIT,
  FAST_9BIT,
  FAST_12BIT
} DIGITIZER_TYPE;

typedef enum
{
  FADC_BC133,
  HADC_16,
  SADC_16
} AVANCE_DIGITIZER_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type INT_DIG_TYPE

  @ptypePurpose
    Specifies the type of the internal digitizer.
    Relevance only for non digital (non Avance) spectrometers.
    _SLOW_12BIT:  12 bit digitizer, bandwidth 125 KHz
    _BC132_12BIT: 12 bit digitizer, bandwidth   5 MHz
    _NO_INT_DIG:  No internal digitizer installed.
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  _SLOW_12BIT,
  _BC132_12BIT,
  _NO_INT_DIG
} INT_DIG_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type EXT_DIG_TYPE

  @ptypePurpose
    Specifies the type of the external digitizer.
    These digitizers work only in conjunction with
    the _SLOW_12BIT internal digitizer ( @see INT_DIG_TYPE ).
    Relevance only for non digital (non Avance) spectrometers.
    _NO_EXT_DIG: No internal digitizer installed.
    _SLOW_16BIT: 16 bit digitizer,  bandwidth 50 KHz
    _FAST_9BIT:   9 bit digttizer,  bandwidht 10 MHz (BC131)
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  NO_EXT_DIG,
  _SLOW_16BIT,
  _FAST_9BIT
} EXT_DIG_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type EXPERIMENT_TYPE

  @ptypePurpose  Describes the experiment type
    Imaging:                Imaging experiment
    Spectroscopy:           Spectroscopy experiment
    User_defined_experiment:Other experiment type
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  Imaging,
  Spectroscopy,
  User_defined_experiment
} EXPERIMENT_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type DIM_TYPE

  @ptypePurpose  Describes each dimension of the experiment
    Spatial:
      Gop and Gsauto will perform DS dummy Muliplex-steps.
      If this is the 1st dimension, echoes are shown by default.
    Spectroscopic:
      Gop and Gsauto will perform DS dummy Scans.
      If this is the 1st dimension, FID's are shown by default.
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  Spatial,
  Spectroscopic
} DIM_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type PAPS_TYPE

  @ptypePurpose  Specifies the phase cycling mode to control
    CP: Add consecutive scans of an fid.
    AP: Adds two scans and subtracts the next two.
    QP: More complicated procedure
    AV: ??
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  CP,
  AP,
  QP,
  AV
} PAPS_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type PARMODE_TYPE

  @ptypePurpose  Specifies the type of data acquired.
		 This is relevant for XWINNMR-display.
    _1D: Data of a 1D experiment
    _2D: Data of a 2D experiment
    _3D: Data of a 3d experiment
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  _1D,
  _2D,
  _3D
} PARMODE_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type LOHI_TYPE

  @ptypePurpose  Specifies decoupler power level (MSL only)
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  LO,
  HI
} LOHI_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type PREEMP_RANGE_TYPE

  @ptypePurpose
    The BGU II Preemphasis Unit adds 3 exponential preem-
    phasis decays to the gradient input voltage.
    PREEMP_RANGE_TYPE specifies the maximum time base of
    one exponential decay:
       _2ms:    Maximum time base =    2 ms
       _20ms:   Maximum time base =   20 ms
       _200ms:  Maximum time base =  200 ms
       _2000ms: Maximum time base = 2000 ms
       _DC:	No preemphasis
    For BGU-II and GREAT-60 the actual time bases being 0.2, 2, 20, 200 ms
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  _2ms,
  _20ms,
  _200ms,
  _2000ms,
  _DC
} PREEMP_RANGE_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type PREEMP_STRUCT

  @ptypePurpose
    The BGU II Preemphasis Unit adds 3 exponential decays
    to the gradient input voltage.  This struct defines one
    decay for the preemphasis output and the B0 output:
      pe_gain:
        Amplitude of the exponential preemphasis decay
        in per cent of the gradient input voltage.
      pe_time_constant:
        Time base of the exp. preemphasis decay in
        per cent of pe_range.
      pe_range:
        Maximum time base of the exp. preemphasis decay
      B0_gain:
        Amplitude of the exponential B0 decay in
        per cent of the gradient input voltage.
      B0_time_constant:
        Time base of the B0 preemphasis decay in
        per cent of B0_range.
      B0_range:
        Maximum time base of the exponential B0-decay
  @ptypeValidRange
       active_shield_gradients: -100 <= pe_gain <= 100
       else                        0 <= pe_gain <= 100
       0    <= pe_time_constant <= 100.
       -100 <= B0_gain          <= 100
       0    <= B0_time_constant <= 100.
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef struct
{
  double pe_gain;
  double pe_time_constant;
  PREEMP_RANGE_TYPE pe_range;
  double B0_gain;
  double B0_time_constant;
  PREEMP_RANGE_TYPE B0_range;
} PREEMP_STRUCT;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type CROSS_PREEMP_STRUCT

  @ptypePurpose
    The BGU II Preemphasis Unit adds 3 exponential decays
    to the gradient input voltage.  This struct defines one
    decay for the cross-preemphasis output:
      cross_pe_gain:
        Amplitude of the exponential cross-preemphasis decay
        in per cent of the gradient input voltage.
      cross_pe_tc:
        Time base of the exp. cross-preemphasis decay in
        per cent of cross_pe_range.
      cross_pe_range:
        Maximum time base of the exp. cross-preemphasis decay
  @ptypeValidRange  0 <= (pe_gain or cross_pe_tc) <= 100
  @ptypeAuthor      MAH                                 */
/*------------------------------------------------------*/

typedef struct
{
  double cross_pe_gain;
  double cross_pe_tc;
  PREEMP_RANGE_TYPE cross_pe_range;
} CROSS_PREEMP_STRUCT;

#include <gradient_ramp_typ.h>

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SL_SEPN_MODE

  @ptypePurpose @see ACQ_slice_sepn_mode
      It describes the slice separation mode of a
      multi slice experiment.
    Contiguous:   No gap between the acquired slices.
    Equidistant:  There may be a constant gap between the slices
    Var_Parallel: The gap may be different. But all slices
		  have the same orientation
    Var_Angle:	  The orientation of every slice may be
		  different (multi oblique slices).
    Packages:     There may be different packages, but the
		  the slices of each package are equidistant
  @ptypeAuthor      MAH                                 */
/*------------------------------------------------------*/

typedef enum
{
  Contiguous,
  Equidistant,
  Var_Parallel,
  Var_Angle,
  Packages
} SL_SEPN_MODE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SL_ORIENT_TYPE

  @ptypePurpose	@see ACQ_slice_orient
      It indicates the slice orientation of the acquired
      images and the direction of the read gradient.
      (From the view of a patient)
      Note: The gradient compiler uses ACQ_grad_matrix.
    Transverse_Left_Right:
    Transverse_Posterior_Anterior:
    Sagittal_Posterior_Anterior:
    Sagittal_Head_Foot:
    Coronal_Left_Right:
    Coronal_Head_Foot:
      Tansversal, saggital or coronal are acquired.
      The direction of the read gradient is left to right,
      posterior to anterior or head to foot.
    Trans_Cor_oblique:
    Trans_Cor_Left_Right:
    Trans_Sag_oblique:
    Trans_Sag_Posterior_Anterior:
    Cor_Sag_oblique:
    Cor_Sag_Head_Foot:
      Single oblique slice are aquired.
      The direction of the read gradient is the oblique
      direction or the non-oblique direction of the image.
    Double_Oblique_In_Plane:
    Double_Oblique_Perpendicular:
      The slice orientation has been adusted by using a
      reference image.
      The slice is perpendicular to the reference.
      The direction of the read gradient lies in the plane
      of the reference image or is perpendiculare to it.
    Arbitrary_Oblique:
      The slice orientation is arbtirary oblique, as
      defined by the ACQ_grad_matrix.
  @ptypeAuthor      MAH                                 */
/*------------------------------------------------------*/

typedef enum
{
  Transverse_Left_Right,
  Transverse_Posterior_Anterior,
  Sagittal_Posterior_Anterior,
  Sagittal_Head_Foot,
  Coronal_Left_Right,
  Coronal_Head_Foot,
  Trans_Cor_oblique,
  Trans_Cor_Left_Right,
  Trans_Sag_oblique,
  Trans_Sag_Posterior_Anterior,
  Cor_Sag_oblique,
  Cor_Sag_Head_Foot,
  Double_Oblique_In_Plane,
  Double_Oblique_Perpendicular,
  Arbitrary_Oblique
} SL_ORIENT_TYPE;

/* MAH 990309: CY caused conflict with RPC of Windows NT.
   ORIENT_TYPE should be defined locally for uxnmr_pars:acqum

typedef enum
{
  TX,
  TY,
  TC,
  TS,
  SX,
  SY,
  SZ,
  CX,
  CY,
  CZ,
  CS,
  TSX,
  TCX,
  TSY,
  TCY,
  CSZ,
  FR
} SORIENT_TYPE; */

typedef enum
{
  Left_Right,
  Head_Foot,
  Posterior_Anterior
} READ_DIRECTION;

typedef enum
{
  X_dir,
  Y_dir,
  Z_dir
} GRAD_DIRECTION;

typedef enum
{
  User_Defined_Mode,
  Constant
} ECHO_SCAN_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type FREQ_CALCULATION_MODE

  @ptypePurpose  Describes how to calculate the frequency
    BF_plus_Offset:
      Frequency = Basic frequency + Offset frequency On (n=1,..,3)
    BF_plus_Offset_list:
      Frequency = Basic frequency + ACQ_On_list[m}
      n=1,..,3; m=0,..,ACQ_On_list_size-1;
    FreqLIST:
      The parameter FQnLIST (FnLIST for ABX/MSLX) contains
      the name of file containing the frequency list absolute
      values. The file is located in ../lists/f1.
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  BF_plus_Offset,
  BF_plus_Offset_list,
  FreqLIST
} FREQ_CALCULATION_MODE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type PHASE_ENC_TYPE

  @ptypePurpose  Describes the phase encoding of the 1st,
       2nd or 3rd dimenension in case of gradient function
       r1d, r2d or r3d.
    User_Defined_Encoding:
      The chronological order of the phase encoding is
      described by the parameters ACQ_spatial_phase_n
      [n=0,..,3 corresponding to gradient ramps r1d,..,r3d)
    Read:
      The value of the read gradient doesn't change.
      If the data are aquired in the presence of a read
      gradient, the phase encoding in this dimension is done
      via the variable time, the gradient is applied for
      each acquired point.
    Linear:
      The phase encoding starts at the most negative value.
      Subsequent phase encoding will be incremented until the
      most positive value is reached. The number of phase
      encoding steps is defined by the size of the correspon-
      ding dimension. The phase endcoding start can be
      modified by @see ACQ_phase_enc_start
    Centred:
      The phase encoding is centered. That means, the phase
      encoding starts at the position, defined by
      @ACQ_phase_enc_start . Subsequent phase encodings will
      get alternatively the next values above and below this
      starting point. This mode is very interesting for
      monitoring longtime imaging experiments with
      ACQ_phase_enc_start set to zero. At any time, the
      acquired NSCANs are contiguous concerning the k-space
      and the experiment starts with high signal NSCANs
      (no phase gradient). These NSCANs can be taken to
      reconstruct images before the experiment is finished.
      The resolutin becomes certainly better, the more NSCANs
      are acquired. But if the operator detects a bad position,
      the experiment may be stopped.
    Rare:
      Selects a phase encoding for a RARE-experiment. In a
      RARE-experiment the echoes of a multi echo track are
      phase encoded. The number of echoes within a track are
      specified by the parameter @see ACQ_phase_factor . To
      minimize artifacts due to the decreasing amplitudes of
      the echoes during a multiecho track (due to T2 and
      diffusion), the phase encoding imcrement is determined
      by ACQ_size[1]/ACQ_rare_factor. The k-space is divided
      into ACQ_phase_factor segments whereby each segment has
      the same quantity of T2 and diffusion effects.
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  User_Defined_Encoding,
  Read,
  Linear,
  Centred,
  Rare
} PHASE_ENC_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type ACQ_WORDSIZE

  @ptypePurpose  Describes the word size of the raw data
    _32_BIT:	The raw data are stored as 32 bit integer
    _16_BIT:	The raw data are stored as 16 bit integer
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  _32_BIT,
  _16_BIT
}ACQ_WORDSIZE;

typedef struct
{
  double	elapsed;
  double	remaining;
  double	est_error;
} GO_TIME_EST;

typedef enum
{
  Single_experiment,
  Infinite_repetition,
  Infinite_steady_state
}PIPE_MODE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type ACQ_SCAN_TYPE

  @ptypePurpose  Specifies the type of the current experiment
    No_Experiment:     No epxeriment is specified.
    Scan_Experiment:   Scan  mode -> acquire and store data
    Setup_Experiment:  Setup mode -> e.g. to adjust parameters
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{ No_Experiment,
  Scan_Experiment,
  Setup_Experiment
}ACQ_SCAN_TYPE;

typedef	enum
{
  Each_PE_Step,
  Each_Accum,
  Each_Scan,
  No_display
} GO_UPDATE;

typedef	enum
{
  Each_experiment,
  On_request
} GS_UPDATE;

typedef enum
{
  Exit_gsauto,
  Repeat_gsauto,
  Start_gop
} GS_CONTINUE;

typedef enum
{
  Spectrometer_Parameters,
  Gradients,
  Preemphasis,
  Shim,
  Freeze_Parameters
} GSTYP;

typedef enum
{
  No_restart,
  Recompile,
  Restart
} GS_RESTART;

typedef enum
{
    Standard_KBlock_Format,
    continuous
} GO_BLOCKTYPE;

typedef enum
{
    GO_32BIT_SGN_INT,
    GO_16BIT_SGN_INT,
    GO_32BIT_FLOAT
} GO_DATATYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SHIM_PAR

  @ptypePurpose    Enum, describing a shim channel.
  @ptypeAuthor     MAH                                 */
/*------------------------------------------------------*/

typedef enum
{
  Shim_Z0,
  Shim_Z,
  Shim_Z2,
  Shim_Z3,
  Shim_Z4,
  Shim_X,
  Shim_Y,
  Shim_XZ,
  Shim_YZ,
  Shim_2XY,
  Shim_X2_Y2,
  Shim_Z2X,
  Shim_Z2Y,
  Shim_Z5,
  Shim_Z_2XY,
  Shim_Z_X2_Y2,
  Shim_X3,
  Shim_Y3,
// Append 21 additionals shim, supported by BSMS
  Shim_X2Y2Z2,
  Shim_XZ4,
  Shim_XZ3,
  Shim_Z6,
  Shim_YZ4,
  Shim_YZ3,
  Shim_XYZ2,
  Shim_X3Z,
  Shim_Y3Z,
  Shim_Z7,
  Shim_Z8,
  Shim_Z9,
  Shim_Z10,
  Shim_XZ5,
  Shim_YZ5,
  Shim_X2Y2Z3,
  Shim_X2Y2Z4,
  Shim_X2Y2Z5,
  Shim_XYZ3,
  Shim_XYZ4,
  Shim_XYZ5,
  Shim_39,
  Shim_40,
  Shim_41,
  Shim_42,
  Shim_43,
  Shim_44,
  Shim_45,
  Shim_46,
  Shim_47,
  Shim_48,
  Shim_49,
  Shim_50,
  Shim_51,
  Shim_52,
  Shim_53,
  Shim_54,
  Shim_55,
  Shim_56,
  Shim_57,
  Shim_58,
  Shim_59,
  Shim_60,
  Shim_61,
  Shim_62,
  Shim_63,
  Shim_Invalid
}SHIM_PAR;

#define Shim_Lacking Shim_Invalid  // BSMS supports only 39 shims

#define N_SHIMS 64
#define	N_GS_TYPES 4

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type VALID_SHIM_PS

  @ptypePurpose  Describes the type of the shim power supply
    User_SPS:    User defined Shim Power Supply
    BSPS_12802:  Bruker Shim Power Supply,  12 channels, 80V, 2A
    BSPS_66010:  Bruker Shim Power Supply,   6 channels, 60V, 10A
    BSPS_8155:   Bruker Shim Power Supply,   8 channels, 15V, 5 A
    RRI_MXH6:    Resonance Research Instruments,  6 channels, 100V, 10A
    RRI_MXH12:   Resonance Research Instruments, 12 channels, 100V, 10A
    BSPS_12105:  Bruker Shim Power Supply,  12 channels, 10V, 5A
    BSMS_SPS:    BSMS Shim Power Supply
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  No_SPS,
  User_SPS,
  BSPS_12802,
  BSPS_66010,
  RRI_MXH6,
  RRI_MXH12,
  BSPS_8155,
  BSPS_12105,
  BSMS_SPS
} VALID_SHIM_PS;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type GRADIENT_AMPLIFIER_CONTROLLER_TYPE
 *
 * @ptypePurpose  Describes the type of the gradient amplifier controller to be used
 *   GAB_SYSTEM:          Use GAB board of BSMS
 *   UIMAGE_UIMAGE:       Use u-imaging gradeint controller (GREAT)
 *   MRI_SYSTEM:          Use MRI gradient controller
 *   NO_SYSTEM
 *   GAB_B0_SYSTEM:       Use GAB and B0
 *   GAB_GPROC_SYSTEM:    Use GAB and GPROC
 *   GAB_B0_GPROC_SYSTEM: Use GAB, B0 and GRPOC
 * @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  GAB_SYSTEM,
  UIMAGE_SYSTEM,
  MRI_SYSTEM,
  NO_SYSTEM,
  GAB_B0_SYSTEM,
  GAB_GPROC_SYSTEM, 
  GAB_B0_GPROC_SYSTEM
} GRADIENT_AMPLIFIER_CONTROLLER_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type VALID_GRADIENT_PS

  @ptypePurpose  Describes the type of the gradient power supply
    User_GPS:    User defined Gradient Power Supply
    Copley_231:  100 A Copley Power Supply
    Copley_265:  200 A Copley Power Supply
    Copley_700:  700 A Copley Power Supply
    Siemens_GPA: 1000 A Siemens Power Supply
    Great40:     Gradient Unit with 40 A Power Supply
    Great60:     Gradient Unit with 60 A Power Supply
    IECO_300A_500V: Ieco GPA-400-750, nominal 300 A and 500 V
    IECO_200A_300V: Ieco GPA-400-350, nominal 200 A and 300 V
    Copley_232HC: Copley 232HC (ICON) nominal  60 A and 150 V
    IECO_150A_300V: Ieco GPA-150-350, nominal 150 A and 300 V
    IECO_300A_300V: Ieco GPA-400-350, nominal 300 A and 300 V
    Copley_266:  250A Copley Power Supply
    Techron:     Techron Power Supply
    IECO_XPA:    IECO XPA 175/150-350
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  No_GPS,
  User_GPS,
  Copley_231,
  Copley_265,
  Copley_700,
  Siemens_GPA,
  Great40,
  Great60,
  IECO_300A_500V,
  IECO_200A_300V,
  Copley_232HC,
  IECO_150A_300V,
  IECO_300A_300V,
  Copley_266,
  Techron,
  IECO_XPA
} VALID_GRADIENT_PS;

typedef enum
{
  No_Estimation,
  Init_Estimation,
  Next_Values,
  Estimate_Gain,
  Gain_Estimated,
  Estimation_Done
} GS_ESTIMATION;

typedef struct
{
  SHIM_PAR	Shim_Name;
  int		Shim_Value;
  GS_ESTIMATION	Current_State;
  int		Estimation_Index;
  int		N_Estimations;
  double	Min_Gain;
  double	Max_Gain;
  double	Max_Gain_Increment;
  double	Max_Area;
  double	Area_Noise;
  double	Current_Gain;
  double	Current_Area;
  double	Estimated_Gain;
} GS_EST_GAUSS;

typedef enum
{
  No_Shim_Control,
  Load_Shim_Set1,
  Load_Shim_Offset2
} GS_SHIM_CONTROL;

typedef enum
{
  Before_calling_acq_processor,
  Before_PIPE_filter,
  Entered_acq_init,
  Before_set_aqpar,
  Before_set_interface,
  Before_set_parameter,
  Before_set_dmapointer,
  Before_handle_new_ni,
  Before_bload_pulser,
  Before_set_intrpt_service,
  Before_starting_SPC,
  Entered_acq_proc,
  Stop_requested,
  Set_interface_requested,
  Before_wait_to_output,
  Before_lseek,
  Before_reading_data,
  Before_signal_output,
  Before_last_signal_output,
  Entered_acq_wrapup
} ACQ_DEBUG;

typedef enum
{
  No_debugging,
  Dump_compiled_lists,
  Dump_parameters,
  View_dump,
  Set_parameters
} ACQ_DEBUG_COMMAND;

typedef enum
{
  Pipe_debug,
  Sample_size,
  N_scans,
  N_averages,
  Dummy_scans,
  Dummy_flag,
  N_total,
  A3000_fids,
  Acq_blocks,
  Cur_n_scans,
  Cur_n_sweeps,
  Dma_ptr,
  In_cnt,
  Out_cnt,
  Dummy_cnt,
  Dwellt,
  Dig_cw,
  Fast_dig_cw,
  Stadc_seen,
  Stop_request,
  Pipe_mode,
  Set_interface,
  Dummy_mode,
  N_experiments
} SET_ACQ_DEBUG;

typedef enum
{
  No_info,
  Of_raw_data,
  Of_reco_data
} GS_INFO;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SHAPED_PULSES

  @ptypePurpose Specifies, how to edit shaped pulses within
		XWINNMR.
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  SP_edit,
  SP_yes
} SHAPED_PULSES;

typedef enum
{
  LPno,
  LPfr,
  LPfc,
  LPbr,
  LPbc
} ME_mod_TYPE;

typedef struct
{
  int	Position;
  int	Max_U;
  int	Max_V;
  int	Left_U;
  int	Left_V;
  int	Right_U;
  int	Right_V;
} GS_INFO_MAX_POINT;

typedef struct
{
  ACQ_DEBUG Debug;
  int Pipe_debug[10];
  int Stop_request;
  int In_cnt;
  int Out_cnt;
  int N_experiments;
  int N_level1_interrupts;
  int N_level2_interrupts;
  int Dummy_mode;
  int Dummy_scans;
  int Dummy_flag;
  int Dummy_cnt;
  int Cur_n_scans;
  int Cur_n_sweeps;
  int Dma_ptr;
  int Set_interface;
  int Dwellt;
  int Dig_cw;
  int Fast_dig_cw;
  int Stadc_seen;
  int SPC_trigger_flag;
  int ADC_trigger_flag;
  int Gradient_program;
  int Pipe_mode;
  int Sample_size;
  int N_scans;
  int N_averages;
  int N_total;
  int A3000_fids;
  int Acq_blocks;
  int A3001_Size;
  int Instrument_type;
} ACQ_COPROC_PARM;


/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SHIM_CHANNEL

  @ptypePurpose    Struct, containing all shim values of
		   all shim channels.
  @ptypeValidRange:-CONFIG_shim_max_val, .., CONFIG_shim_max_val
  @ptypeAuthor     MAH                                 */
/*------------------------------------------------------*/

typedef struct
{ int	Z0;
  int	Z1;
  int	Z2;
  int	Z3;
  int	Z4;
  int	X;
  int	Y;
  int	XZ;
  int	YZ;
  int	XY;
  int	X2_Y2;
  int	XZ2;
  int	YZ2;
  int	Z5;
  int	ZXY;
  int	ZX2_Y2;
  int	X3;
  int	Y3;
//  Addtional 21 shim, supported by BSMS
  int   X2Y2Z2;
  int   XZ4;
  int   XZ3;
  int   Z6;
  int   YZ4;
  int   YZ3;
  int   XYZ2;
  int   X3Z;
  int   Y3Z;
  int   Z7;
  int   Z8;
  int   Z9;
  int   Z10;
  int   XZ5;
  int   YZ5;
  int   X2Y2Z3;
  int   X2Y2Z4;
  int   X2Y2Z5;
  int   XYZ3;
  int   XYZ4;
  int   XYZ5;
} SHIM_CHANNEL;

typedef enum
{
  Absolute,
  Slice_Thickness_Rel
} SLICE_SEPN_EQ_MODE;

typedef enum
  {
  No_Sat,
  Fov_Sat,
  Inflow_Sat,
  Fov_and_Inflow_Sat
} SAT_MODE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type IMPEDANCE_LOAD

  @ptypePurpose   For the GREAT-60 preemphasis unit:
		  Specifies the Amplifier Impedance load.
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum			/* for the GREAT preemphasis units */
{
  High_Impedance,
  Low_Impedance
} IMPEDANCE_LOAD;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type PREEMP_GAIN

  @ptypePurpose   For the GREAT-60 preemphasis unit:
		  Specifies the Amplifier Gain in Ampere.
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum			/* for the GREAT-60 preemphasis unit */
{
  _60A,
  _50A,
  _40A,
  _30A,
  _20A,
  _10A
} PREEMP_GAIN;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type DC_ALG_TYPE

  @ptypePurpose Specifies the algorithm used for dutycycle
                supervision (e.g. GRASI2 gradient unit)
     IndepPowSv:  Power Supervision, assuming independant channels
     DepPowSv:    Power Supervision, assuming mutual channel influence
     IndepSPowSv: Short Power Supervision via a temperature based model
     DepSPowSv:   Short Power Supervision, assuming mutual channel influence
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  IndepPowSv,
  DepPowSv,
  IndepSPowSv,
  DepSPowSv
} DC_ALG_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type PREEMP_AMPLIFIER

  @ptypePurpose   For the GREAT-60 preemphasis unit:
		  Specifies the type of the gradient amplifier connected.
  @ptypeAuthor  SGE                                     */
/*------------------------------------------------------*/

typedef enum			/* for the GREAT-60 preemphasis unit */
{
  PREEMP_AMPLIFIER_NONE,
  PREEMP_AMPLIFIER_60A,
  PREEMP_AMPLIFIER_40A,
  PREEMP_AMPLIFIER_H0B0,
  PREEMP_AMPLIFIER_B0
} PREEMP_AMPLIFIER;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type PIN_DRIVER_MODE

  @ptypePurpose   Enum type for pin driver modes
                  Coil detuning lines can be regulated to
                  constant voltage or constant current
  @ptypeAuthor    uhei                                  */
/*------------------------------------------------------*/
typedef enum
{
  voltageMode,
  currentMode
} PIN_DRIVER_MODE;

#define MANUFACTURER_LEN         16 /* Maximum length of manufacturer name       */
#define PART_NUM_LEN             64 /* Maximum length of part number             */
#define SERIAL_NUM_LEN           16 /* Maximum length of serial number           */

#define COIL_CONFIG_FILENAME_LEN 512

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SPECIAL_ROUTING

  @ptypePurpose   Enforce special routings for pv service methods
  @ptypeAuthor    uhei                                  */
/*------------------------------------------------------*/
typedef enum
{
  SpecialRouting_ReceiveOnlyChan1,
  SpecialRouting_ReceiveOnlyChan2,
  SpecialRouting_ReceiveOnlyChan3,
  SpecialRouting_ReceiveOnlyChan4,
  SpecialRouting_ReceiveOnlyAllRecs,
  SpecialRouting_Sgu1ToRec2,
  SpecialRouting_Sgu2ToRec1,
  SpecialRouting_Sgu1ToRec3,
  SpecialRouting_Sgu1ToRec4,
  SpecialRouting_ObserveTransmitter1hp,
  SpecialRouting_ObserveTransmitter1lp,
  SpecialRouting_ObserveTransmitter2hp,
  SpecialRouting_ObserveTransmitter2lp,
  SpecialRouting_ObserveTransmitter3hp,
  SpecialRouting_ObserveTransmitter3lp,
  SpecialRouting_ObserveTransmitter4hp,
  SpecialRouting_ObserveTransmitter4lp,
  SpecialRouting_AdmTest
} SPECIAL_ROUTING;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type PIPELINE_STATUS

  @ptypePurpose   The various states of the PV pipeline.
                  Set by the pipe master, evaluated by acq_control
  @ptypeAuthor    uhei                                  */
/*------------------------------------------------------*/

typedef enum
{
  NoPipe,
  PreInit,
  Init,
  Proc,
  Wrapup,
  PostWrapup
} PIPELINE_STATUS;

#define ACQ_MAX_JOBS     15
#define ACQ_JOB_TITLE_LEN 64

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type JOB_DESCRIPTION_TYPE

  @ptypePurpose specifies jobs for interleaved acquisition
  @ptypeAuthor  PFRE                                    */
/*------------------------------------------------------*/
typedef struct
{
    int scanSize;              /* TD-size for job 0 for inactive job*/
    int transactionBlocks;     /* blocks for GS mode */
    int dummyScans;            /* dummy scans to be performed for job */
    int nTotalScans;           /* number of scans for GOP experiment */
    double receiverGain;       /* RG to be used for job */
    double swh;                /* sweep width in Hertz */
    int nStoredScans;          /* number of stored scans for GOP experiment */
    int chanNum;               /* channel number this job applies to */
    char title[ACQ_JOB_TITLE_LEN];    /* symbolic name for job */
} JOB_DESCRIPTION_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type GRADSTATUS_ENUM_SHIELD_TYPE

  @ptypePurpose   This is the type of dynamic enum parameters,
                  that specify the type of weither a coil is
		  shielded (S) or unshielded (U)
  @ptypeAuthor  ROE                                     */
/*------------------------------------------------------*/

typedef enum
{
  shielded,
  unshielded
} GRADCOIL_ENUM_SHIELD_TYPE;

#define GRADCOIL_NAME_LEN  64

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type GRADCOIL_TYPE

  @ptypePurpose   This structure contains Bis Information
                  of a gradient coil

                  <BIS-Group>.<Value> (@see BIS-Reference Manual)
    id:           GradCoil.ID
    shield:       GradCoil.Shield
    sensitivity:  GradCoil.Sensitivity
    resistance:   GradCoil.Resistance
    thermcond:    GradCoil.ThermCond
    maxVoltage:   GradCoil.MaxVoltage
    maxCurrent:   GradCoil.MaxCurrent
    maxPower:     GradCoil.MaxPower
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/
typedef struct
{
  char   id[GRADCOIL_NAME_LEN];
  GRADCOIL_ENUM_SHIELD_TYPE shield;
  double sensitivity ;
  double resistance;
  double thermcond;
  double maxVoltage;
  double maxCurrent;
  double maxPower;
} GRADCOIL_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type WBELEMENT_DESC_TYPE

  @ptypePurpose   This structure describes a single coil element
                  that can be wobbled.
  @ptypeAuthor  THOR                                    */
/*------------------------------------------------------*/
typedef struct
{
    char shortDesc[21];
    char longDesc[65];
    int coilIndex;
    int elementIndex;
    int admCrpTmChannel;
    int admTmChannel;
    double tuneVal;
    double matchVal;
    int groupIndex;
} WBELEMENT_DESC_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type WBELEMENT_IDX_TYPE

  @ptypePurpose   This structure describes the mapping between a single coil
                  element that is selected for wobble and its used job.
  @ptypeAuthor  THOR                                    */
/*------------------------------------------------------*/
typedef struct
{
    int tabIdx;
    int jobIdx;
} WBELEMENT_IDX_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type HW_FILTER_TYPE

  @ptypePurpose Specifies the digital HardWare filter, e.g of an AV4 DRX.
    USAGE for AV4 DRX:
     For filtering on the DRX only (no software filtering/decimation needed), the
     three filter types 'Sharp', 'Medium' and 'Smooth' are offered.
     Parts of the DRX's filter chain can be skipped by choosing a bypass option.
     However, further software filtering and decimation becomes mandatory in this
     case.
     Bypassing (parts of) the filter chain reduces the hardware group delay,
     which might be crucial for fast acquisition.

     Sharp    : Sharp filter with relatively large group delay.
     Medium   : Medium filter with relatively average group delay.
     Smooth   : Smooth filter with relatively small group delay.
     Bypass3  : Partly bypass the FIR filter chain. Software decimation >=2 is
                mandatory.
     Bypass2  : Partly bypass the FIR filter chain. Software decimation >=4 is
                mandatory.
     Bypass1  : Partly bypass the FIR filter chain. Software decimation >=8 is
                mandatory.
     BypassFir: Bypass the FIR filter chain completely. Setting for shortest
                hardware group delay. Software decimation >=16 is mandatory.
                Standard option for BaseOpt.
  @ptypeAuthor  THOR                                     */
/*------------------------------------------------------*/
typedef enum
{
    HwFilter_Unknown,
    HwFilter_Sharp,
    HwFilter_Medium,
    HwFilter_Smooth,
    HwFilter_Bypass3,
    HwFilter_Bypass2,
    HwFilter_Bypass1,
    HwFilter_BypassFir
} HW_FILTER_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SW_FILTER_TYPE

  @ptypePurpose Specifies the digital SoftWare filter, e.g. on an AV4 EPU.

   None: No digital filtering. No decimation.

   LowPassSharp: Sharp low pass filter. Large group delay.
                 Cut off frequency is automatically set according to a given
                 software decimation.

   LowPassSmooth: Smooth low pass filter. Small group delay.
                  Cut off frequency is automatically set according to a given
                  software decimation.

   LowPassSmooth: Smooth low pass filter. Small group delay.
                  Cut off frequency is automatically set according to a given
                  software decimation.

   SinglePointAveraging: Aquire single-point scans and average over N points.
                         N is defined by the software decimation.

  @ptypeAuthor  THOR                                     */
/*------------------------------------------------------*/
typedef enum
{
    SwFilter_Unknown,
    SwFilter_None,
    SwFilter_LowPassSharp_Decim,
    SwFilter_LowPassSmooth_Decim,
    SwFilter_SinglePointAveraging
} SW_FILTER_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SW_DATA_EXTENSION

  @ptypePurpose Specifies how data is extended outside its border regions during
                filtering.

    Zero     : Signal is assumed to be zero outside its border regions.
               Fastest, because points need not to be calculated.
    Cyclic   : Signal is assumed to be a circular repetition of itself.
               Same as doing an FFT.
    Symmetric: Signal is assumed to be a symmetric repetition of itself.
  @ptypeAuthor  THOR                                     */
/*------------------------------------------------------*/
typedef enum
{
    SwDataExt_Unknown,
    SwDataExt_Zero,
    SwDataExt_Cyclic,
    SwDataExt_Symmetric
} SW_DATA_EXTENSION;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type RX_FILTER_SETTING

  @ptypePurpose Specifies a receivers digital filter setting

  @ptypeAuthor  THOR                                     */
/*------------------------------------------------------*/
typedef struct
{
    HW_FILTER_TYPE hwFilterType;
    SW_FILTER_TYPE swFilterType;
    int hwDecim;
    int swDecim;
    int hwScanShift;
    int swScanShift;
    SW_DATA_EXTENSION swDataExtension;
    int backPrediction;
} RX_FILTER_SETTING;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type RX_FILTER_INFO

  @ptypePurpose Information derived from a receivers digital filter setting

  @ptypeAuthor  THOR                                     */
/*------------------------------------------------------*/
typedef struct
{
    double groupDelay;
    double compensatedGrpDelay;
    double waitTime;
    double dynamicRange;
    double dataRange;
} RX_FILTER_INFO;


/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type ADMCRP_CHN_SETTING

  @ptypePurpose Setting of an ADM-CRP channel

  @ptypeAuthor  THOR                                     */
/*------------------------------------------------------*/
typedef struct
{
    int isUsed;
    int isWobbled;
    int logicIsActiveHigh;
    int blkTrMask;
} ADMCRP_CHN_SETTING;


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
