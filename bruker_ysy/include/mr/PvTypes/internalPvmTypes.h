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

#ifndef MR_INT_PVM_TYPES_H
#define MR_INT_PVM_TYPES_H

#define NUC_PPMTABLE_SIZE       20

#include "mr/PvTypes/spiralTypesInternal.h"

typedef struct
{
  char   Nuc[8];
  double RefAtt;
  char   Stat[PVSYSINFOCHANSTATE_LEN];
} REFG_TYPE;

/** Parameter type for receiver gain adjustment used for 
    internal brackening
*/
typedef struct
{
  double digf;
  double RGval;
} RG_LIMITS;

/** Section usage for Pipeline Filter */
typedef enum
{
  SEC_DATA,         /**< Section contains data */
  SEC_NAVIGATOR,    /**< Section contains a navigator */
  SEC_GARBAGE,      /**< Section contains garbage */
  SEC_BEGINLOOP,    /**< Loop begin marker (field nrep gives loop runs,
		         other fields of this section are irrelevant) */
  SEC_ENDLOOP       /**< Loop end marker (other fileds are irrelevant) */
} PF_SECTION_USAGE;

/** Section for Pipeline Filter */
typedef struct
{
  PF_SECTION_USAGE usage;    /**< what a section is */
  int           size;        /**< size of section in cplx words */
  int           nrep;        /* for navig, data, garbage: 
			        how many identical data sections
                                are described by this section
			        for SEC_ENDLOOP: n.of loop repetitions */
  double        recGain;     /**< RG used for this section */
  double        sweepWidth;  /**< SW_h used for this section */
  double        delayToNext; /**< delay between this section and the next, ms */
  double        echoTime;    /**< gradient echo time for this section, ms */     
} PF_SECTION;

typedef enum
{
  No_Conflict,
  Conflict
} OVS_ConflictType;

typedef enum
{
  RPS_KS,
  PAT_KS,
  LAB_KS
} GRAD_KSTYP;

typedef struct
{
  double st;
  double ru;
  double rd;
  double dur;
  double pl;
  GRAD_KSTYP ks;
  double vec[3];
} SGT_TYPE;

/* old rf pulse structure related types */

typedef struct 
{
  double		bandWidthFactor;
  double		attenuationCorr;
  double		rephaseFactor;
  PV_PULSE_TYPE		type;
} PV_PULSE_PROPERTIES;

typedef struct 
{
  char			filename[16];
  double		length;
  double		bandWidth;
  double		attenuation;
  double                flipAngle;
  PV_PULSE_PROPERTIES	property;
} PV_PULSE;             


typedef enum
{
  _1H,
  _2H,
  _3He,
  _7Li,
  _13C,
  _15N,
  _17O,
  _19F,
  _23Na,
  _31P,
  _OtherNucleus
} PV_NUCLEUS_TYPE;

typedef enum
{
  FixedLimit,
  FixedLimitSingleOblique,
  FixedLimitDoubleOblique,
  OrientDependent
} GRAD_LIMIT_MODE ;

// old cuboid handling
typedef struct
{
  double pos[3];
  double siz[3];
  double ori[3][3];
} ADJ_CUBOID_TYPE;

// nuclei frequency handling
typedef struct
{
  char nucleus[8];
  double ppm;
} NUC_PPM_TYPE;

/* internal RF pulse handling */

/*---------------------------------------------------------------
 * supported types: PSn: PVM_RF_PULSE
 *                  PEn: PV_PULSE_LIST
 *                  PWn: double array
 *                  PAn: PVM_RF_PULSE_AMP_TYPE
 *--------------------------------------------------------------*/

/** RF shape handler type */
typedef struct
{
  char  PSn[128];  /**< name of pulse struct */
  char  PEn[128];  /**< name of pulse enum */
  char  PWn[128];  /**< name of pulse waveform */
  char  PAn[128];  /**< name of pulse amplitude parameter */
  YesNo CalcAmpl;  /**<  */
  int   Fchan;     /**<  */
  double Pmax;     /**< max peak power */
  int type;        /**< type of RF pulse */
  
  /** types of parameters (0 not available)
      - Types[0]: shape struct parameter 
        - 0 = not specified
        - 1 = PVM_RF_PULSE
        - 2 = PVM_RF_PULSE_TYPE
      - Types[1]: shape list parameter
        - 0 = not specified
        - 1 = PV_PULSE_LIST
      - Types[2]: shape ampl parameter type
        - 0 = not specified
        - 1 = PVM_RF_PULSE_AMP_TYPE
      - Types[3]: shape waveform parameter
       - 0 = not specified
       - 1 = 1D double array
  */
  int   Types[4];  
} PVM_RF_SHAPE_HANDLER_TYPE;


#endif
