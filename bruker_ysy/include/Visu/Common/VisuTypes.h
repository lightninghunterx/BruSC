/****************************************************************
 *
 * Copyright (c) 2000 - 2018
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 ****************************************************************/

#ifndef COMMON_VISUTYPES_H
#define COMMON_VISUTYPES_H

#include "generated/KeyUids.h"
#include "PvUtil/TimeDefs.h"
#include "common/PvAdminRoute.h"
#include "Visu/Common/VisuDefines.h"
#include "PvAdj/ResultParsType.h"


/** Definition of the current VISU parameters. */
enum VISU_INSTANCE_TYPE
{
    /** The current VISU parameters are only valid for minimal viewing.
        They cannot be used for further processing, they do not have any
        geometry information. */
    MINIMAL_INSTANCE,

    /** The current VISU parameter are complete. */
    STANDARD_INSTANCE
};

/** Dimension types for a VISU frame. */
enum VISU_DIM_TYPE
{
    /** Spatial frame dimension. */
    spatial,

    /** Spectroscopic frame dimension. */
    spectroscopic,

    /** Temporal frame dimension. */
    temporal
};

/*
 * Typedef to describe the used reference coordinate system
 * for the specification of pose & orientation
 */
enum VISU_REFERENCE_CS
{
    other,
    model
};

/** Slice ordering in 2dseq file, only necessary for 3d spatial images. */
enum VisuDiskSliceOrderType
{
    disk_normal_slice_order,
    disk_reverse_slice_order
};

/*
 * definitions for frame groups
 */

/** Definition of a frame group */
struct VisuFrameGroupType
{
    /** Number of elements in this frame group */
    int len;
    /** Identification of frame group. */
    char groupId[VISU_MAX_NAME_LEN];
    /** User defined group comment. */
    char groupComment[VISU_MAX_NAME_LEN];
    /** Start index in dependant values description array. */
    int valsStart;
    /** Count of dependant parameters for this frame group. */
    int valsCnt;
};

/** Definition of dependant parameters for a frame group.
 *  @see VisuFrameGroupType */
struct VisuDepValsType
{
    /** Name of frame group dependant parameter. */
    char name[VISU_MAX_NAME_LEN];
    /** Dependant parameter must be an array, start index in the array. */
    int valsStart;
};

/** Definition of slice packages. */
struct VisuSlicePackType
{
    /** Frame group index where slice packages are located.
        It is -1 if all frames belong to the slice packages. */
    int fgNdx;

    /** Number of slice packages. */
    int packsCnt;
};

/** Definition of anchor and size for array slicing. */
struct VisuAnchorSizeType
{
    /** Anchor (start index in arrays). */
    int anchor;

    /** Size (size of array part). */
    int size;
};


/* DICOM PS3.16 CID 12 */
enum VisuContrastAgentEnum
{
    AIR_AGENT,
    OXYGEN_AGENT,
    WATER_AGENT,
    CARBON_DIOXIDE_AGENT,
    BARIUM_SULFAT_AGENT,
    GADOLINIUM_AGENT,
    CONTRAST_AGENT,
    RADIOPAQUE_MEDIUM_AGENT,
    NON_RADIOPAQUE_MEDIUM_AGENT,
    BUNAMIODYL_AGENT,
    CHLORODIZED_OIL_AGENT,
    DIATRIZOATE_AGENT,
    IODIPAMIDE_AGENT,
    IODIXED_OIL_AGENT,
    IODOALPHIONIC_ACID_AGENT,
    MEGLUMINE_IODIPAMIDE_AGENT,
    SODIUM_IODIPAMIDE_AGENT,
    IODAMIDE_MEGLUMINE_AGENT,
    IODOPYRACET_AGENT,
    IOPANIC_ACID_AGENT,
    IOPHENDYLATE_AGENT,
    IOPHENOXIC_ACID_AGENT,
    IPODATE_AGENT,
    PROPYLIODONE_AGENT,
    SODIUM_ACETRIZOATE_AGENT,
    IODOPHTHALEIN_AGENT,
    SODIUM_DIPROTRIZOATE_AGENT,
    SODIUM_IODOMETHAMATE_AGENT,
    MEGLUMINE_DIATRIZOATE_AGENT,
    SODIUM_DIATRIZOATE_AGENT,
    METRIZAMIDE_AGENT,
    SODIUM_TYROPANATE_AGENT,
    IONIC_IODINATED_AGENT,
    NON_IONIC_IODINATED_AGENT,
    IOHEXOL_AGENT,
    IODIXANOL_AGENT,
    GADODIAMIDE_AGENT,
    MANGAFODIPIR_TRISODIUM_AGENT,
    IOTHALAMATE_AGENT,
    IOXAGLATE_AGENT,
    METRIZOATE_AGENT,
    GADOPENTETATE_DIMEGLUMINE_AGENT,
    IOPAMIDOL_AGENT,
    IOVERSOL_AGENT,
    IOPROMIDE_AGENT,
    IOXILAN_AGENT,
    FERRUM_OXIDES_AGENT,
    USER_DEFINED_AGENT
};


/* DICOM PS3.16 CID 13. */
enum VisuContrastIngredientEnum
{
    IODINE_INGREDIENT,
    GADOLINIUM_INGREDIENT,
    CARBON_DIOXIDE_INGREDIENT,
    BARIUM_INGREDIENT,
    XENON_INGREDIENT,
    AIR_INGREDIENT,
    OXYGEN_INGREDIENT,
    WATER_INGREDIENT,
    IRON_INGREDIENT,
    USER_DEFINED_INGREDIENT
};

/** Contrast agent definition. */
struct VisuContrastAgentType
{
    /** The contrast agent. */
    VisuContrastAgentEnum agent;

    /** The contrast / bolus administration route. */
    PvAdminRouteEnum adminRoute;

    /** Total volume in ml of diluted contrast agent. */
    double volume;

    /** Milligrams of active ingredient per milliliter of agent. */
    double concentration;

    /** Start of administration profiles for this agent. */
    int adminProfileStart;

    /** Number of administration profiles for this agent. */
    int adminProfileCnt;

    /** Start of ingredients for this agent. */
    int ingredientsStart;

    /** Number of ingredients for this agent. */
    int ingredientsCnt;
    
    /* Later additions must be at the end of the type. */
        
    /**
     * The contrast agent name.
     * Must be set if \a agent is \c USER_DEFINED_AGENT.
     */
    char agentName[VISU_MAX_NAME_LEN];
    
    /** 
     * The contrast / bolus administration route name.
     * Must be set if \a adminRoute is \c UNKNOWN_ROUTE. 
     */
    char adminRouteName[VISU_MAX_NAME_LEN];
};

/** Contrast agent profile. */
struct VisuContrastAgentAdminProfileType
{
    /** Volume of the diluted contrast agent. */
    double volume;
    /** Start of administration profile. */
    pvtime_t start;
    /** Stop of administration profile. */
    pvtime_t stop;
    /** The flow rate. */
    double flow_rate;
    /** The flow duration. */
    double flow_duration;
};

/** The VISU reference dataset definition. */
struct VisuReferenceDataset
{
    /** The unique identifier of the reference dataset. */
    char uid[de_bruker_mri_generated_uid_CStrIsoUidLen];
    /** The path of the reference dataset. */
    AdjProcnoResultType datasetPath;
};

/** Word type for raw data. */
enum VisuRawDataWordtype
{
    /** Raw data is stored as 32 bit floating point values. */
    FloatRawData,
    /** Raw data is stored as 64 bit floating point values. */
    DoubleRawData,
    /** Raw data is stored as 16 bit signed integer values. */
    Signed16BitRawData,
    /** Raw data is stored as 16 bit unsigned integer values. */
    Unsigned16BitRawData,
    /** Raw data is stored as 32 bit signed integer values. */
    Signed32BitRawData,
    /** Raw data is stored as 32 bit unsigned integer values. */
    Unsigned32BitRawData
};

enum VisuVoiLutFcnType
{
    /** 
     * Linear function.
     * Width and height are applied according to the following pseudo-code, 
     * where x is the input value, y is an output value with a range from 
     * y_min to y_max , c is Window Center  and w is Window Width:
     * if (x <= c - 0.5 - (w-1) /2), then y = y_min
     * else if (x > c - 0.5 + (w-1) /2), then y = y_max
     * else y = ((x - (c - 0.5)) / (w-1) + 0.5) * (y_max - y_min ) + y_min
     */
    VOILUT_LINEAR,

    /**
     * Sigmoid function.
     * OUT = Output_range / ( 1 + exp(-4 * (IN-WC)/ww))
     * IN is the input value of the LUT.
     * WC is the Window Center
     * WW is the Window Width
     * Output_range is the maximum possible output value 
     */
    VOILUT_SIGMOID,

    /**
     * Linear exact function.
     * x is the input value, y is an output value with a range from 
     * y_min to y_max , c is Window Center  and w is Window Width :
     * if (x <= c - w/2), then y = y_min
     * else if (x > c + w/2), then y = y_max
     * else y = (x - c) / w * (y_max - y_min ) + y_min
     */
    VOILUT_LINEAR_EXACT,

    /** Unknown function. */
    VOILUT_UNKNOWN
};

/** Definition of a VOI LUT Type. */
struct VisuVoiLutType
{
    /** Window width for display on  scaled value. */
    double width;
    /** Window center for display on scaled value. */
    double center;
    /** Function to calculate display values. */
    VisuVoiLutFcnType type;
    /** Free-form description of VOI LUT. */
    char description[VISU_MAX_NAME_LEN];
};

/**
 * Definition the frame acquisition time.
 */
struct VisuFrameAcqTime
{
    /** Start time of the frame acquisition. */
    pvtime_t start;
    /** Duration of the frame acquisition in milliseconds. */
    double duration;
};

/** Cardiac synchronization technique. */
enum VisuCardiacSynchType
{
    /** No Cardiac synchronization is used. */
    CARDIAC_NONE,
    /** 
     * No gating applied, 
     * total time for acquisition is shorter than cardiac cycle.
     */
    CARDIAC_REALTIME,
    /** Thresholds for gating windows are defined before acquisition.*/
    CARDIAC_PROSPECTIVE,
    /** Thresholds for gating window are defined after acquisition. */
    CARDIAC_RETROSPECTIVE,
    /** Constant RR interval, thresholds are nit required. */
    CARDIAC_PACED
};

/** Cardiac signal source. */
enum VisuCardiacSignalSourceType
{
    /** Signal source is an electrocardiogram. */
    CARDIAC_ECG,
    /** Signal source is a vector cardiogram. */
    CARDIAC_VCG,
    /** Signal source is a peripheral pulse. */
    CARDIAC_PP,
    /** Signal source is magnetic resonances navigator. */
    CARDIAC_MR
};

/** Respiratory synchronization technique. */
enum VisuRespSynchType
{
    /** No respiratory synchronization used. */
    RESP_NONE,
    /** Synchronization by breath hold. */
    RESP_BREATH_HOLD,
    /** Image acquisition is shorter than respiratory cycle. */
    RESP_REALTIME,
    /** Prospective gating. */
    RESP_GATING,
    /** Prospective through-plane of in-plane motion tracking. */
    RESP_TRACKING,
    /** Prospective phase ordering. */
    RESP_PHASE_ORDERING,
    /** Prospective real time averaging, diminishing variance, etc.*/
    RESP_PHASE_RESCANNING,
    /** Retrospective gating. */
    RESP_RETROSPECTIVE,
    /** Retrospective image correction. */
    RESP_CORRECTION
};

/** Respiratory signal source. */
enum VisuRespSignalSourceType
{
    /** No Respiratory source.*/
    RESP_NO_SOURCE,
    /** Device detecting / measuring expansion of chest. */
    RESP_BELT,
    /** Nasal probe device. */
    RESP_NASAL_PROBE,
    /** CO2 sensor. */
    RESP_CO2_SENSOR,
    /** MR navigator and organ edge detection. */
    RESP_NAVIGATOR,
    /** Phase of center k-space line. */
    RESP_MR_PHASE,
    /** Baseline demodulation of ECG. */
    RESP_ECG
};

/** Trigger time structure. */
struct VisuTriggerTimeType
{
    /** Nominal trigger time. */
    double nominal;
    /** actual trigger time. */
    double actual;
};


#endif /* VISUTYPES_H */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



