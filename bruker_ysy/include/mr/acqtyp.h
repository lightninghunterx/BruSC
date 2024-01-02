/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1995-2005
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef UXNMRPARX_ACQUMTYP_H
#define UXNMRPARX_ACQUMTYP_H

#include "subject/subjtyp.h"
#include "common/acqtyp.h"
#include "acqutyp.h"
#include "bruktyp.h"
#include "Parx/Dynenum.h"

#define TEMPSENS_NAME_LEN 20
#define SHIMSTATUS_NAME_LEN 20
#define SHIMFUNCTION_NAME_LEN 20

#define GRADSTATUS_NAME_LEN 20
#define GRADCOIL_COIL_LEN  8

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
} SORIENT_TYPE;

/* _BB creates conflict with NT string header */

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

/* plus creates conflict with STL libraries for SGI */
typedef enum
{
  normal,
  plus
} HPPRGN_TYPE;

typedef enum
{
  no,
  yes
} PSTRUCT_BOOLEAN;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type DIR_ENUM_LIST_TYPE

  @ptypePurpose   This is the type of dynamic enum parameters,
                  that specify entries of a given directory.
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef dynenum DIR_ENUM_LIST_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type GRADSTATUS_DESCRIPTION_TYPE

  @ptypePurpose   This is the main structure, containing
                  Bis Information of a gradient tube

                  <BIS-Group>.<Value> (@see BIS-Reference Manual)
    id:           GradSystem.CoilCode
    name:	  GradSystem.DefaultStatus
    manufacturer: Production.Location
    partNo:	  Production.ProductId
    serialNo:	  Production.SerialNo
    bisDate:      Bis.BisDate
    isocenterOffsetZ: GradSystem.IsocenterOffsetZ
    maxPower:     GradSystem.MaxTotalPower
    avgTime:      GradSystem.AveragePowerDelay
    nCoils:       GradCoil: number of entries
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/
typedef struct
{
  int             id;
  char            name[GRADSTATUS_NAME_LEN];
  char            manufacturer[MANUFACTURER_LEN];
  char            partNo[PART_NUM_LEN];
  char            serialNo[SERIAL_NUM_LEN];
  int             bisDate;
  double          isocenterOffsetZ;
  double          maxPower;
  double          avgTime;

  int             nCoils;
} GRADSTATUS_DESCRIPTION_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type PVSHIM_DATA_TYPE

  @ptypePurpose    Enum, that specifices the shim data type
    PVSHIM_function_values: Logical shim values,
                            that means the shim values
                            - may be linear combinations
                              of physical shims and
                            - are scaled up from
                              the maximal allowed shim value
                              to ShimMaxVal
    PVSHIM_raw_values:      Raw shim values as delivered to
                            the TOSI- or GRASI (DAC input)
  @ptypeAuthor     MAH                                 */
/*------------------------------------------------------*/

typedef enum
{
  PVSHIM_function_values,
  PVSHIM_raw_values        // If nothing is set
} PVSHIM_DATA_TYPE;


/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SHIMSTATUS_DESCRIPTION_TYPE

  @ptypePurpose   This is the  main structure, containing
                  Bis Information of the shim system

                  <BIS-Group>.<Value> (@see BIS-Reference Manual)
    id:           ShimSystem.CoilCode
    name:         ShimSystem.DefaultStatus
    manufacturer: Production.Location
    partNo:       Production.ProductId
    serialNo:     Production.SerialNo
    bisDate:      Bis.BisDate
    maxPower:     ShimSystem.MaxTotalPower
    avgTime:      ShimSystem.AvgPowerDelay
    nCoils:       ShimSystem.NumCoils
    nFunc:        ShimFunction: Number of entries
    nMatrixEntries:ShimMatrix:  Number of entries
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef struct
{
  int             id;
  char            name[SHIMSTATUS_NAME_LEN];
  char            manufacturer[MANUFACTURER_LEN];
  char            partNo[PART_NUM_LEN];
  char            serialNo[SERIAL_NUM_LEN];
  int             bisDate;
  double          maxPower;
  double          avgTime;
  int             nCoils;
  int             nFunc;
  int             nMatrixEntries;
} SHIMSTATUS_DESCRIPTION_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SHIM_TYPE

  @ptypePurpose   This structure contains Bis Information
                  of a shim coil

                  <BIS-Group>.<Value> (@see BIS-Reference Manual)
    id:           ShimCoil.ID
    resistance:   ShimCoil.Resistance
    maxVoltage:   ShimCoil.MaxVoltage
    maxCurrent:   ShimCoil.MaxCurrent
    maxPower:     ShimCoil.MaxPower
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef struct
{
  int    id;
  double resistance;
  double maxVoltage;
  double maxCurrent;
  double maxPower;
} SHIMCOIL_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SHIMFUNCTION_TYPE

  @ptypePurpose   This structure contains Bis Information of
                  an analytical shim function

                  <BIS-Group>.<Value> (@see BIS-Reference Manual)
    id:           ShimFunction.ID
    name:         ShimFunction.Function
    order:        ShimFunction.Order
    sens:         ShimFunction.Sensitivity
    degree:       ShimFunction.Degree

    Note: In the SFSH group the meaning of Order and Degree is reversed 
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef struct
{
  int                 id;
  char                name[SHIMFUNCTION_NAME_LEN];
  int                 order;
  double              sens;
  int                 degree;
} SHIMFUNCTION_TYPE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SHIM_HARMONIC_ENTRY_TYPE

  @ptypePurpose   This structure contains the Bis Information of
                  of a harmonic component of a given coil 
                  (inverse BossMatrix * ShimFunction.Sensitvity)

                  <BIS-Group>.<Value> (@see BIS-Reference Manual)
    id:           SFSH.ID
    degree        SFSH.Degree
    order:        SFSH.Order
    sens:         SFSH.Sensitivity

    Note: In the ShimFunction group the meaning of Order and Degree is reversed 
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef struct
{
  int                 id;
  int                 degree;
  int                 order;
  double              sens;
} SHIM_HARMONIC_ENTRY_TYPE;

#define SHIMMATRIX_EPS 0.001   // Minimal shim matrix entry
#define SHIMCOILRES_MIN 0.1    // Minmale resistance of shim coil:
#define SPSCURRMAX_MIN 0.01    // Minimal CONFIG_SPS_CurrentMax
#define SPSVOLTMAX_MIN 0.01    // Minimal CONFIG_SPS_VoltageMax
#define SHIMSENS_MIN   0.0001  // Minimal Shim Sensitivity

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type SHIMMATRIX_ENTRY_TYPE

  @ptypePurpose   The shim matrix contains the mapping of
                  analytical shim functions into shim coils.
                  This structure, contains Bis Information
                  of a shim matrix entry.

                  <BIS-Group>.<Value> (@see BIS-Reference Manual)
    fctId:        ShimMatrix.FctID
    coildId:      ShimMatrix.CoilID
    factor:       ShimMatrix.Factor
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef struct
{
  int                 fctId;
  int                 coilId;
  double              factor;
} SHIMMATRIX_ENTRY_TYPE;

typedef struct
{
    int dim;            /* Dimension of submatrix: 1, 2	*/
    int fidx[2];        /* TOSI Function index		*/
    int cidx[2];        /* TOSI Coil index	        */
    double m[2][2];
    double mi[2][2];
} SHIMSUBMATRIX_TYPE;
/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of SHIM_SENSITIVITY_TYPE
 *
 * @ptypePurpose    Struct containing sensitivity of the
 *                  corresponding spherical harmonic coefficient
 *   shim           spherical harmonic shim function
 *   sens           sensitivity in Hz/cm**order/A
 * @ptypeAuthor     MAH                                 */
/*------------------------------------------------------*/

typedef struct
{
  SHIM_PAR shim;
  double   sens;
} SHIM_SENSITIVITY_TYPE;

typedef dynenum COIL_CONFIG_TABLE;
typedef dynenum COIL_OPERATION_MODE_TABLE;

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type COIL_OPERATION_MODE_COIL_NAMES

  @ptypePurpose   Dynamic enum parameter to hold speaking names of the used
                  coil within an operation mode.
  @ptypeAuthor  THOR                                     */
/*------------------------------------------------------*/
typedef dynenum COIL_OPERATION_MODE_COIL_NAMES;


/* Maximum length of a MRI PICS/HWIDS bis */
#define COIL_BIS_LEN SUBJECT_MR_COIL_BIS_LEN

/* Maximum length of a gradient/shim status string */
#define STATUS_STRING_LEN 128

/* Maximum array size of routing parameters, used for array traversals
   Currently SWIBOX is the largest array */
#define ROUT_ARRAY_MAX 20

/* Array size for SPECPAR parameters */
#define SPECPAR_SIZE 16

// definitions for parameters for MXI pipeline

typedef enum
{
    STORE_discard,
    STORE_raw,
    STORE_processed
} SCAN_STORE_DATA_MODE_TYPE;

typedef enum
{
    STORE_32bit_signed,
    STORE_64bit_float
} SCAN_STORAGE_DATA_TYPE;

typedef enum
{
    DISPLAY_off,
    DISPLAY_each_scan,
    DISPLAY_each_accumulation,
    DISPLAY_each_pe_step

} SCAN_DISPLAY_MODE_TYPE;

typedef enum
{
    PIPELINE_processed,
    PIPELINE_raw
} SCAN_PIPELINE_MODE_TYPE;

typedef enum
{
    ACCUM_none,
    ACCUM_add,
    ACCUM_average
} SCAN_ACCUM_MODE_TYPE;

typedef enum
{
    NORMALIZE_none,
    NORMALIZE_divide_by_4rg
} SCAN_NORMALIZE_TYPE;

typedef enum
{
    LOG_none,
    LOG_file,
    LOG_relation,
    LOG_file_and_relation
} SCAN_LOG_TIMESTAMP_TYPE;

typedef struct 
{
    SCAN_STORE_DATA_MODE_TYPE storeDataMode;
    SCAN_STORAGE_DATA_TYPE    storageDataType;
    SCAN_DISPLAY_MODE_TYPE    displayMode;
    SCAN_LOG_TIMESTAMP_TYPE   logTimeStamp;
    SCAN_ACCUM_MODE_TYPE      accumMode;
    int                       innerAccumNWeights;
    int                       innerAccumLoops;
    int                       outerAccumNWeights;
    int                       outerAccumLoops;
    int                       nStoredScans;
    int                       nAccumScans;
    SCAN_NORMALIZE_TYPE	      normalizeMode;
    SCAN_PIPELINE_MODE_TYPE   pipelineMode;
    int                       storeScanTags;
} SCAN_PIPE_JOB_SETTINGS_TYPE;

/// maximum number of possible scan tags
#define MAX_N_SCAN_TAGS 4
// maximum number of logical channels
#define MAX_N_LOG_CHAN 8


typedef struct
{
    SCAN_DISPLAY_MODE_TYPE    displayMode;
    int                       nAccumScans;
} SETUP_PIPE_JOB_SETTINGS_TYPE;


typedef struct
{
    int dbLev_AcqControl;
    int dbLev_CubeServer;
    YesNo enableSequencerLog;
} ACQ_DEBUG_OPTIONS_TYPE;


typedef struct
{
    YesNo compensateGrpDelay; // enable group delay compensation
    YesNo forceShortFilter;   // use a short filter kernel for less ripple
} RX_FILTER_DEFAULT_OPTIONS;

// compensate the group delay of all filters
#define RX_FILTER_DEFAULT_OPT_CMP_GROUP_DELAY 1
// use a smoother filter to save computing time
#define RX_FILTER_DEFAULT_OPT_FORCE_SHORT_SW_FILTER 2

typedef struct
{
    double  value; 
    int     jobIdx; 
} ACQ_TIMESTAMP_TYPE;


typedef struct
{
    ACQ_TIMESTAMP_TYPE timestamp; 
    int idx; 
} ACQ_SCAN_TIMESTAMP_TYPE;


typedef struct
{
    int timeout;
    YesNo enabledByPulProg;
    YesNo abortAfterTimeout;
} ACQ_TRIGGER_TIMEOUT_TYPE;



#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
