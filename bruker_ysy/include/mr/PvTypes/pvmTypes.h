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

#ifndef MR_PVM_TYPES_H
#define MR_PVM_TYPES_H

#include "bruktyp.h"
#include "Parx/Dynenum.h"
#include "configtyp.h"

#include "common/PvTypes/geoTypes.h"
#include "PvAdj/adjManagerTypes.h"
#include "PvAdj/adjConfigTypes.h"
#include "PvAdj/ResultParsType.h"
#include "PvMrAdj/MrAdjManagerTypes.h"
#include "mr/acqtyp.h"
#include "generated/ParImport.h"


/** Pointer type used for gradient orientation matrices.
    @ingroup PVM_DEFINE
*/
#define PtrType3x3 (double (*) [3][3])

/** Pointer type used for vectors.
    @ingroup PVM_DEFINE
*/
#define PtrType1x3 (double (*) [3])


#include "mr/PvTypes/epiTypes.h"
#include "mr/PvTypes/spiralTypes.h"
#include "mr/PvTypes/trajectoryTypes.h"
#include "mr/PvTypes/wobbleTypes.h"

/* nucleus related */

typedef dynenum STANDARD_NUCLEUS_TYPE;

/* rf power related */

/** 
    @ingroup PVM_STRUCT
*/
typedef struct
{
  double RefPow;
  char   Stat[PVSYSINFOCHANSTATE_LEN];
} REFPOW_TYPE;

/* rf pulse related */

/** List of available pre-calculated RF pulse shapes.
    Values determined from files available in directory.
    @ingroup PVM_ENUM
*/
typedef dynenum PV_PULSE_LIST;

typedef enum
{
  Excitation,
  Inversion,
  Refocussing,
  Gradient
} EXCITATION_MODE ;

typedef struct
{
  double ppow;
  double pampl;
  double patt;
} PVM_RF_PULSE_AMP_TYPE;
/** RF pulse algorithm selection type as used by @ref STB_UpdateRFPulse . 
    Dependent on the value different waveform algorithms are used to create
    the desired RF shape.
    @ingroup PVM_ENUM
*/
typedef enum    
{               
  /** Shinnar LeRoux (SLR) Waveform created by @ref MRT_ConventionalRFShape
   */            
  Conventional, 
  /** Not yet implemented, if selected calculated waveform is deactivated 
   */
  SweepUp,      
  /** Not yet implemented, if selected calculated waveform is deactivated 
   */
  SweepDown,    
  /** Adiabatic Full Passage Hyperbolic Sechant pulse with pulse sweep 
      from lower to upper frequency, created by @ref MRT_AfpUpRFShape.
   */
  AfpUp,        
  /** Not yet implemented, if selected calculated waveform is deactivated 
   */      
  AfpDown,      
  /** Not yet implemented, if selected calculated waveform is deactivated 
   */      
  AhpUp,       
  /** Not yet implemented, if selected calculated waveform is deactivated 
   */      
  AhpDown,
  /** Not yet implemented, if selected calculated waveform is deactivated 
   */      
  IahpUp,
  /** Not yet implemented, if selected calculated waveform is deactivated 
   */      
  IahpDown        
} SHAPE_ALG_TYPE;

typedef enum
{
  conventional,
  adiabatic
} PV_PULSE_TYPE;

typedef enum
{
  USER_PULSE,
  LIB_EXCITATION,
  LIB_REFOCUS,
  LIB_INVERSION,
  PVM_EXCITATION,
  PVM_REFOCUS,
  PVM_INVERSION
} PVM_PULSE_CLASSIFICATION;

typedef enum
{
  MethodMode,
  ReferenceGain,
  ManualAdjustment
} MODULE_GAIN_MODE;
  
// maybe to deprecate
typedef enum
{
  By_DeriveGains,
  By_ReferenceGain,
  Manual
} MODULE_DERIVE_GAIN;

/** RF pulse type
    @ingroup PVM_STRUCT
*/
typedef struct
{
  // Routine user input parameter
  double Length;     /**< User input only in SHAPE_FILE mode */
  double Bandwidth;  /**< */
  double Flipangle;  /**< */
  YesNo  Calculated; /**< */
  int    Sharpness;  /**< Ranging from 1-10 */
  // int    Flatness;   /**< Ranging from 1-10 */

  // Low Level Parameters:
  double Bwfac;      /**< BW @ 1ms */
  double Sint;       /**< Shape integral */
  double Pint;       /**< Power integral */
  int    Type;       /**< Replacement of classification */
                    
  // Interface to Baselevel (+Length above)
  double Rpfac;      /**< Rephase factor */
  double Pow;        /**< Amplitude */
  char   Shape[64];  /**< Wavefile or parameter name */
} PVM_RF_PULSE;

/* old RF pulse type */

typedef struct
{
  double  PulseLength;
  double  Bandwidth;
  double  Attenuation;
  double  FlipAngle;
} PVM_PULSE;

typedef struct
{
  char   Filename[64];
  double BandwidthFactor;
  double IntegralRatio;
  double RephaseFactor;
  double MinPulseLength;
  PVM_PULSE_CLASSIFICATION Classification;
  PV_PULSE_TYPE Type;
} PVM_PULSE_PROPERTIES;
/* ShapeArea --> IntegralRatio
   Group     --> Classification
*/

typedef struct
{
  double  Bandwidth;
  double  Attenuation;
  double  Rephase;
} PVM_PULSE_CORRECTIONS;

typedef struct
{
  double Length;
  double Bandwidth;
  double FlipAngle;
  double Attenuation;
  double TrimBandwidth;
  double TrimAttenuation;
  double TrimRephase;
  PVM_PULSE_CLASSIFICATION Classification;
  char   Filename[64];
  double BandwidthFactor;
  double IntegralRatio;
  double RephaseFactor;
  double MinPulseLength;
  PV_PULSE_TYPE Type; 
} PVM_RF_PULSE_TYPE;


/* voxel geometry related */

typedef enum
{
  RL_AP_HF,
  RL_HF_AP,
  AP_RL_HF,
  AP_HF_RL,
  HF_RL_AP,
  HF_AP_RL
} PV_EXCITATION_ORDER ;

// maybe to deprecate
typedef enum 
{ 
  OTHER_MethodType, 
  VSEL_MethodType, 
  CSI_MethodType, 
  ISIS_MethodType 
} PV_VOX_METHOD_TYPE;

/* encoding pargroup */

typedef enum
{
  LINEAR_ENC,
  CENTRIC_ENC
} ENC_ORDER;  

typedef enum
{
  SEG_SEQUENTIAL,
  SEG_INTERLEAVED
} SEG_MODE;

/* trigger modules */

typedef enum
{
  per_PhaseStep,
  per_Slice
} TRIGGER_MODE;
  
typedef enum 
{
   AT_START,
   PER_VOLUME,
   PER_SLICE
} SYNCHRO_ENUM;

/* MT module */

typedef enum
{
  Free_Pulse,
  DanteOffRes_Pulse,
  BinomialOnRes_Pulse,
  SelectiveOffRes_Pulse
} MAGN_TRANS_TYPE;

/* tagging module */

typedef enum
{
  Tagging_lines,
  Tagging_grid
} TAGGING_DIR;

typedef enum
{
  DANTE,
  SPAMM
} TAGGING_MODE;
  
/* decoupling module */

typedef enum
{
  Continuous_Wave,
  Composite_Pulse
} DECMODES;

typedef enum
{
  dec_bb,
  dec_garp,
  dec_garp4,
  dec_mlev16,
  dec_waltz16,
  dec_user_pulse
} DECPULSES;

/* NOE module */

typedef enum
{
  Continuous_Wave_Noe,
  Composite_Pulse_Noe
} NOEMODES;

typedef enum
{
  noe_bb,
  noe_garp,
  noe_garp4,
  noe_mlev16,
  noe_waltz16,
  noe_user_pulse
} NOEPULSES;

/* evolution module */

typedef enum
{
  Constant_Delay,
  Variable_Delay
} EVOL_MODE;
   
/* water suppression module */

typedef enum
{
  NO_SUPPRESSION,
  CHESS,
  VAPOR
} WS_MODE_TYPE;

/* flow saturation module */

typedef enum
{
  up,
  down
} IN_FLOW_POS_TYPE;

/* diffusion module */

typedef dynenum DW_PREP_MODE;

typedef dynenum DW_DGS_TYPE;

typedef dynenum DW_MEASMODE_TYPE;

/* adjustment related */

typedef enum
{
  AutoAdj,
  UserAdj
} ADJ_MODE;

/* shim adjustment */

/** 
    @ingroup PVM_STRUCT
*/
typedef struct
{
  SHIM_PAR name;
  double   range;
  double   step;
  int      iter;
  int      stab;
  double   stepHz;
} TUNE_SHIM_TYPE;

typedef enum
{
  Shim_1st_order,     
  Shim_1st_order_z2,  
  Shim_1st_2nd_Order, 
  Shim_all     
} SHIM_SUBSETS_TYPE;

/* method datastructures */


/** Gradient spoiler properties.
    @see @ref MRT_InitSpoiler
    @ingroup PVM_STRUCT
*/
typedef struct
{
  YesNo automatic;  /**< Automatic parameter setting */
  double spoil;     /**< Spoiling efficiency [cycles/pixel] */
  double dur;       /**< Duration [ms] */
  double ampl;      /**< Amplitude [%] */
} PVM_SPOILER_TYPE;

typedef enum
{
  SignalType_Fid,
  SignalType_Echo
} PVM_SIGNAL_TYPE;

/* CASL module */

typedef enum
{
  ASL_Coil,
  Resonator 
} CASL_COILTYPE;

typedef enum
{
  CASL_Automatic,
  CASL_Manually 
} CASL_B1MODE;

typedef enum
{
  Parallel,
  Arbitrary
} CASL_LABELPOS;

typedef enum
{
  Label_Control,
  Label,
  Control
}CASL_LABELTYPE;

typedef enum
{
  Interleaved,
  Dynamic
}CASL_ACQORDER;

typedef enum
{
  Default_value,
  Adjustment,
  Manual_value        
}CASL_CONTROLFREQ;

typedef enum
{
  CASL_DEFAULT,       
  CASL_CBFr,   
  CASL_CBF   
}CASL_RECOMODE;

/* navigator module */

typedef enum
{
  Nav_arbitrary,
  Nav_parallel,
  Nav_in_slice
} NavPosMode;

/* Fair module */

typedef enum
{
  SELECTIVE,
  NONSELECTIVE, 
  INTERLEAVED, /* selective and non sel. TIR-series are interleaved 
                  when Nrep > 1 */
  INTERLEAVED2 /* selective and non-sel. inversions are interleaved
                  within each TIR series (also with Nrep=1) */
} FAIR_INV_TYPE;

typedef enum
{
  LINEAR_TIR,
  GEOMETRIC_TIR,
  USER_TIR
} FAIR_TIR_TYPE;

typedef enum
{
  CONST_REP,
  CONST_RECOV
} FAIR_REP_TYPE;



/** Module Saturation Transfer: (physical) unit selector for rf frequency 
    offsets.
    @ingroup PVM_ENUM
*/
typedef enum
  {
    unit_hz,
    unit_ppm
  } SAT_TRANS_FREQ_UNIT;


/** Selector for saturation transfer type
    @ingroup PVM_ENUM
*/
typedef enum
  {
    MTC,
    CEST
  } SAT_TRANS_TYPE;

/** Module Saturation Transfer: lists of offset frequencies can be
    set up in two different ways:
    @ingroup PVM_ENUM
*/
typedef enum
{
  by_value,  /**< add a single value to the list  */
  by_range   /**< add a band of values by specifying start, end, and increment value */
} SAT_TRANS_FREQ_INPUT;




/** Reconstruction mode for Fat Water Separation (FWS)
    @ingroup PVM_ENUM
 */
typedef enum
{
  FWS_DEFAULT,       /**< default reconstruction without FWS (displayname = Default)*/
  FWS_SEPARATION,    /**< separation of fat and water into two images (displayname = Fat Water Separation)*/
  FWS_COMBINATION    /**< separation and subsequent combination of fat and water into one image (displayname = Fat Water Combination)*/
}FWS_RECO_MODE;


/** States of method specific shim requirements
    @ingroup PVM_ENUM
*/
typedef enum
{
    Current_Shim, /**< method accepts shim that is currently loaded  */
    Study_Shim, /**< method requires global shim adjusted in study  */
    Map_Shim   /**< method requires map shim within specified volume */
}REQ_SHIM;

/** Shape of volume in geometry editor
    @ingroup PVM_ENUM

 */
typedef enum
  {
    Cuboid, /**< volume displayed as cuboid   */
    Cylinder, /**< volume displayed as cylinder aligned along slice direction */
    Ellipsoid /** < volume displayed as ellipsoid */
  }GEOED_VOLUME_SHAPE;


/** List of startup shims for currently active hardware
    @ingroup PVM_ENUM
 */
typedef dynenum STARTUP_SHIM_LIST;


/** Choice of startup shim condition
    @ingroup PVM_ENUM
 */
typedef enum
{
  TuneupShim,  /** Acceptance startup shim condition */
  StartupShim, /** Startup shim condition adjusted and selected by user */
  CurrentShim  /** No change of active shim condition */
} STARTUP_SHIM_CONDITION; 



/* hardware related */

typedef dynenum DIGFILT_TYPE;

typedef dynenum PREEMPHASIS_TYPE;



/* other */

typedef enum
{
  Logical_Read_Grad,
  Logical_Phase_Grad,
  Logical_Slice_Grad
} PVM_LOGICAL_GRAD_TYPE;


/*** DEPRECATED ***/

typedef enum 
{
  Matrix, 
  Fov,
  Resolution,
  Fov_and_Resolution ,
  Non_Isotropic 
} ISOTROPIC_TYPE;

typedef enum
{
    FirstOrder,
    SecondOrder,
    ThirdOrder,
    AllShims
}MAP_SHIM_USAGE;


typedef struct
{
  char title[24];
  double dvalue;
} PVM_RG_TYPE;

typedef dynenum PV_RG_TITLE;


/** ArrayPhase adjustment mode.
    Determines when phases are calibrated.
*/
typedef enum
{
  ArrayPhase_Adj_Auto,          /**< Automatic (if required) */
  ArrayPhase_Adj_Force,         /**< Force (perform always) */
  ArrayPhase_Adj_Skip           /**< Skip (do not perform) */
} ARRAYPHASE_MODE;

/** Statistics of Fieldmap B0 shim calculation.
 * Provides details of shims calculated from B0 fieldmaps.
 * @ingroup PVM_STRUCT
 */
typedef struct
{
    YesNo  succeeded;       /** Information about success of calculation */
    char   identifier[256]; /** Information about calculated shim */
    double volume;          /** Volume of map area / ul  */
    int    nfieldvals;      /** Number of field values */
    double mean;            /** Mean field value */
    double sd;              /** Standard deviation of field value */
    double min;             /** Minimum field value */
    double max;             /** Maximum field value */
    double adev;            /** Absolute deviation of field value */
    int    nbins;           /** Number of histogram bins  */
    int    ol;              /** Number of lower outliers  */
    int    oh;              /** Number of higher outliers */
}MAPSHIM_SHIM_STATISTICS;

/** Shim properties. 
 * Provides properties of shims.
 * @ingroup PVM_STRUCT
 */
typedef struct
{
    char name[256];     /** name of shim coil*/
    double maxcurrent;  /** max current */
    int spsidx;         /** attached shim power supply */ 
}MAPSHIM_SHIM_PROPERTIES;






#endif
