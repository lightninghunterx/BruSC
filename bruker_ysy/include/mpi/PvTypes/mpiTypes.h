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
#ifndef MPI_MPI_TYPES_H
#define MPI_MPI_TYPES_H

#include "bruktyp.h"
#include "Parx/Dynenum.h"

/** macros */
#define MPI_STRING_LENGTH 4096
#define MPI_STRING_LENGTH_SHORT 1024

/** Type to define the active static field channels for selection and focussing. */
typedef enum MpiStaticFieldGenType
{
    /** The channel is for selection. */
    SELECTION,
    /** The channel is for the focus field in x direction. */
    FOCUSX,
    /** The channel is for the focus field in y direction. */
    FOCUSY, 
    /** The channel is for the focus field in z direction. */
    FOCUSZ
} MPI_STATIC_FIELD_GEN_TYPE;

/* The sequence of static MPI field. */
typedef enum MpiStaticFieldSeqType
{
    SINGLESTEP,
    MULTISTEP,
    SWEPT
} MPI_STATIC_FIELD_SEQ_TYPE;


typedef enum MpiSamplingTrajectoryType
{
    MPI_LISSAJOUS,
    MPI_RECTILINEAR,
    MPI_RADIAL,
    MPI_SPIRAL
} MPI_SAMPLING_TRAJ_TYPE;

typedef struct MpiDriveFieldType
{
    double freqDivider;
    double amplitude;
} MPI_DRIVE_FIELD_TYPE;

/* Drive field control options */

/// Leave drive field running freely
#define MPI_DF_NO_CONTROL                     0
/// Perform initial calibration of DF amplitudes
#define MPI_DF_CONTROL_CALIBRATE              1
/// Perform initial calibration of DF amplitudes and continue regulation
#define MPI_DF_CONTROL_DRIFT_COMPENSATION     3
/// Compensate channel cross talk on drive field
#define MPI_DF_CONTROL_CROSSTALK_COMPENSATION 4

typedef dynenum MPI_DF_ORIENTATION_TYPE;

/** The MPI reconstruction domain. */
typedef enum MpiDomain
{
    TIME_DOMAIN,
    FREQUENCY_DOMAIN
} MPI_DOMAIN;

/** The MPI reconstruction algorithm. */
typedef enum MpiReconstructionAlgorithm
{
    KACZMARZ,
    CGNR,
    MPI_SVD,
    UNKNOWN_ALGORITHM
} MPI_RECO_ALGO;

/** The MPI reconstruction constraint. */
typedef enum MpiReconstructionConstraints
{
    NO_CONSTRAINT,
    POSITIVE_CONSTRAINT,
    REAL_CONSTRAINT
} MPI_RECO_CONSTRAINTS;

/** The MPI reconstruction regularization type. */
typedef enum MpiRegularizationType
{
    TIKHONOV_REGULARIZATION,
    UNKNOWN_REGULARIZATION
} MPI_REGULARIZATION_TYPE;

/** MPI reconstruction attributes. */
typedef struct MpiReconstructionType
{
    /** Reconstruction domain. */
    MPI_DOMAIN domain;
    /** Number of frequency components used for reconstruction. */
    int usedFreqCompoments;
    /** The regularization used for reconstruction. */
    double regularization;
    /** The regularization type used for reconstruction. */
    MPI_REGULARIZATION_TYPE regularizationType;
    /** The reconstruction algorithm. */
    MPI_RECO_ALGO algorithm;
    /** Number or iterations, 0 is no iterative algorithm is used. */
    int numberOfIterations;
} MPI_RECO_TYPE;

/** MPI background correction modes */
typedef enum 
{
    MPI_NO_BACKGROUNDCORRECTION,   //< Do not use any background correction.
    MPI_SYSTEMFUNCTION_BACKGROUND, //< Substract system function's background from scan and system function
    MPI_BACKGROUND_THIS_SCAN,      //< Substract background taken from selected frames in this scan
    MPI_BACKGROUND_OTHER_SCAN      //< Substract background measured in a different scan
} MpiBackgroundCorrectionType;

/** MPI data type. */
typedef enum MpiDatasetType
{
    /** The VISU parameter set describes the system function. */
    SYSTEM_FUNCTION,
    /** The VISU parameter set describes the empty measurement. */
    EMPTY_MEASURMENT,
    /** The VISU parameter set describing raw data. */
    MPI_DATASET
} MPI_DATA_TYPE;

typedef struct 
{
    /* coordinate of the senter of the sample, relative to the center
     * of the tool mounting plate */
    double sampleCenter[3];
    /* Two edges defining a cuboid around the sample, which is used for collision checks.
     * edgeToolMount is the coordinate of an edge, which lies on the mounting plate
     * => [0] must equal 0*/
    double edgeToolMount[3];
    /* edgeSampleEnd is the coordinate of an edge, which is at the sample end of the sample holder
     * [0] must be outside sampleCenter[0]*/
    double edgeSampleEnd[3];
} MPI_SAMPLE_GEOMETRY;


/** MPI thresholding operator used for denoising of the MPI system matrix */
typedef enum MpiThresholdingOperator
{
    /** hard thresholding operator*/
    MPI_DSM_HARD,
    /** soft thresholding operator*/
    MPI_DSM_SOFT
} MPI_THRESHOLDING_OPERATOR;

/** MPI transformation type used for denoising of the MPI system matrix (DSM) */
typedef enum MpiDenoisingTrafoType
{
    MPI_DSM_FOURIER,
    MPI_DSM_COSINE
} MPI_DENOISING_TRAFOTYPE;

/** MPI transforamtion type used for sparse reconstruction (SR) of the particle distribution*/
typedef enum MpiSparseRecoTrafoType
{
    MPI_SR_FOURIER,
    MPI_SR_COSINE
} MPI_SPARSERECO_TRAFOTYPE;

/** Reconstruction Info for the selected system function */
typedef enum MpiRecoInfoSystemFunction
{
    MPI_NO_RECONSTRUCTION_POSSIBLE,
    MPI_MISSMATCH,
    MPI_MATCH
} MPI_RECOINFOSYSTEMFUNCTION;

/** Averaging type of object measurement data */
typedef enum MpiAverageType
{
    MPI_NO_AVERAGE,
    MPI_BLOCK_AVERAGE,
    MPI_MOVING_AVERAGE
} MPI_AVERAGETYPE;

/** MPI system function attributes. */
typedef struct MpiSystemFunctionType
{
    /** id equivalent to time */
    int id;
    /** dimension */
    int dim;
    /** Selection Field */
    double selectionField;
    /** Drive Field. */
    double driveField[3];
    /** Focus Field */
    double focusField[3];
    /** fov */
    double fov[3];
    /** fov center */
    double fovCenter[3];
    /** image sizes */
    int sizes[3];
    /** particle */
    char tracer[1024];
    /** batch */
    char batch[1024];
    /** bandwidth */
    double bandwidth;
    /** active receivers */
    YesNo activeReceivers[3];
    /** orientation */
    int orientation;
    /** drive field control mode */
    int driveFieldControlMode;
} MPI_SYSTEMFUNCTION_TYPE;


/** dynamic enum of all matching system functions */
typedef dynenum MatchingSystemFunction;

/** Field of View Info  */
typedef enum MpiFovInfo
{
    MPI_FOV_EQUAL_DFFOV,
    MPI_FOV_SMALLER_DFFOV,
    MPI_FOV_GREATER_DFFOV
} MPI_FIELDOFVIEWINFO;

/** Datatype of systemfunction and particle measurement */
typedef enum DataWordType
{
    MPI_SINGLE,
    MPI_DOUBLE
} DATAWORDTYPE; 

/* MPI Gain settings */
typedef enum
{
    Gain9,
    Gain12,
    Gain15,
    Gain18,
    Gain21,
    Gain24,
    Gain27,
    Gain30
} FtmsGainType;

/** MPI windowing function for spectral cleaning */
typedef enum MpiWindowingFunctionSC
{
    MPI_WSC_HANNING,
    MPI_WSC_NUTALL3B,
    MPI_WSC_COS4,
    MPI_WSC_FT1D05,
    MPI_WSC_RECTANGULAR
} MPI_WINDOWING_FUNCTION_SC;

#endif
