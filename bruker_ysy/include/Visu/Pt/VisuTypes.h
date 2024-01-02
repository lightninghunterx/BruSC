/****************************************************************
 *
 * Copyright (c) 2000 - 2018
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PT_VISUTYPES_H
#define PT_VISUTYPES_H

#include "Visu/Common/VisuTypes.h"

/** 
 * PET Acquisition Start Condition type.
 * It must have similar values to the DICOM attribute 
 * "Acquisition Start Condition" (0018,0073).
 */
enum VisuPtAcqStartConditionType
{
    /** Preset count density (counts/sec) was reached. */
    VPT_START_DENS,

    /** Preset relative count density difference (change in counts/sec) was reached.*/
    VPT_START_RDD,

    /** Acquisition was started manually. */
    VPT_START_MANU,

    /** Acquisition was started automatically when ready. */
    VPT_START_AUTO,

    /** Preset number of cardiac triggers was reached. */
    VPT_START_CARD_TRIG,

    /** Preset number of respiratory triggers was reached. */
    VPT_START_RESP_TRIG
};

/** 
 * PET Acquisition Stop Condition type.
 * It must have similar values to the DICOM attribute 
 * "Acquisition Termination Condition" (0018,0071).
 */
enum VisuPtAcqStopConditionType
{
    /** Preset counts was reached. */
    VPT_STOP_CNTS,

    /** Preset count density (counts/sec) was reached. */
    VPT_STOP_DENS,

    /** Preset relative count density difference (change in counts/sec) was reached.*/
    VPT_STOP_RDD,

    /** Acquisition was terminated manually. */
    VPT_STOP_MANU,
    /** Data overflow occurred. */
    VPT_STOP_OVFL,

    /** Preset time limit was reached. */
    VPT_STOP_TIME,
    /** Preset number of cardiac triggers was reached. */
    VPT_STOP_CARD_TRIG,
    /** Preset number of respiratory triggers was reached. */
    VPT_STOP_RESP_TRIG
};

/** The PET energy window range for acquiring images. */
struct VisuPtAcqEnergyWindowRangeType
{
    /** The lower limit of the energy window in keV. */
    double lowerLimit;
    /** The upper limit of the energy window in keV. */
    double upperLimit;
};

/** The type of the detector motion. */
enum VisuPtAcqDetectorMotionTypeEnum
{
    /// No motion.
    VPT_STATIONARY,
    /// Interrupted motion, acquire only while detectors are stationary.
    VPT_STEP_AND_SHOOT,
    /// Gantry motion and acquisition are simultaneous and continuous.
    VPT_CONTINIOUS,
    /// Wobble motion.
    VPT_WOBBLE,
    /// Clamshell motion.
    VPT_CLAMSHELL,
    /// Unknown detector motion type.
    VPT_UNKNOWN_MOTION_TYPE
};

/** The detector geometry type. */
enum VisuPtAcqDetectorGeometryType
{
    /// Detector is a cylindric ring.
    VPT_CYLINDRICAL_RING,
    /// Detector a partial cylindric ring.
    VPT_CYL_RING_PARTIAL,
    /// Detector is multiple planar.
    VPT_MULTIPLE_PLANAR,
    /// Detector is partial multiple planar.
    VPT_MUL_PLANAR_PARTIAL,
    /// Unknown detector geometry type.
    VPT_UNKNOWN_GEOMETRY
};

/** The collimator type. */
enum VisuPtCollimatorType
{
    /** Ring collimator. */
    VPT_COLLIMATOR_RING,
    /** No collimator. */
    VPT_COLLIMATOR_NONE,
    /** Collimator usage is unknown.*/
    VPT_COLLIMATOR_UNKNOWN
};

/**
 * Detector information during acquisition.
 */
struct VisuPtAcqDetectorInfoType
{
    /**
     * The detector motion type, may not be \c null.
     */
    VisuPtAcqDetectorMotionTypeEnum motionType;
    /**
     * The detector geometry type, may not be \c null.
     */
    VisuPtAcqDetectorGeometryType geometryType;
    /**
     * The distance of opposing detectors in mm.
     */
    double distance;
    /**
     * The size of the detector in mm.
     */
    double size;
    /**
     * Indicates type of collimator.
     */
    VisuPtCollimatorType collimatorType;
    /**
     * The coincidence timing window in nanoseconds.
     * The maximum time difference between two single events in two opposing 
     * detectors that will be accepted as coincidence event.
     */
    double coincidenceWindowWidth;
};

/** The primary source of counts. */
enum VisuPtAcqCountsSourceType
{
    /** Primary source is emission. */
    VPT_EMISSION,
    /** Primary source is transmission. */
    VPT_TRANSMISSION
};

enum VisuPtRecoRandomsCorrectionMethodType
{
    /** No randoms correction used.*/
    VPT_RANDOMS_NONE,
    /** Delayed event subtraction correction method. */
    VPT_RANDOMS_DLYD,
    /** Single estimation correction method. */
    VPT_RANDOMS_SING,
    /** Processed delays correction method. */
    VPT_RANDOMS_PDDL,
    /** Unknown correction method. */
    VPT_RANDOMS_UNKNOWN
};

/** PET instrument geometry and data collection information. */
struct VisuPtAcqInstrumentInfoType
{
    /** Distance in mm from top of patient table to data collection center. */
    double tableHeight;

    /** 
     * Relative longitudinal position of acquisition location in mm from 
     * the modality reference point. 
     */
    double tablePosition;

    /** 
     * Nominal angle of tilt in degree of the scanning gantry.
     * Zero degrees means the gantry is not tilted, negative degrees are 
     * when the top of the gantry is tilted away from where the table 
     * enters the gantry.
     */
    double gantryDetectorTilt;

    /**
     * Nominal angle of slew in degrees of the gantry. Zero degrees means 
     * the gantry is no slewed. Positive slew is moving the gantry on the 
     * patient's left toward the patient's superior, when the patient 
     * is supine.
     */
    double gantryDetectorSlew;

    /**
     * The diameter in mm of the region over which data were collected.
     */
    double dataCollectionDiameter;

    /**
     * The coordinates in the subject coordinate system in mm 
     * of the center of the region in which data were collected.
     */
    double dataCollectionCenter[3];

    /**
     * The coordinates in the subject coordinate system of the 
     * reconstruction center target point as used for reconstruction in mm.
     */
    double reconstructionTargetCenter[3];
};

/** The PET reconstruction type. */
enum VisuPtReconstructionType
{
    /** 2D reconstruction. */
    VPR_2D,
    /** 3D reconstruction. */
    VPR_3D,
    /** 3D rebinned reconstruction. */
    VPR_3D_REBINNED,
    /** Unknown reconstruction type. */
    VPR_UNKNOWN
};

enum VisuPtRecoAlgorithmType
{
    /** Filter back projection algorithms. */
    VPRA_FILTER_BACK_PROJ,
    /** Projection algorithm. */
    VPRA_PROJECTION,
    /** RAMLA algorithm. */
    VPRA_RAMLA,
    /** MLEN algorithm. */
    VPRA_MLEM,
    /** Unknown algorithm. */
    VPRA_UNKNOWN
};

/** The PET reconstruction structure. */
struct VisuPtReconstructionStruct
{
    /** The reconstruction type. */
    VisuPtReconstructionType type;
    /** The reconstruction algorithm. */
    VisuPtRecoAlgorithmType algo;
    /** The number of iterations. */
    int numIterations;
    /** The number of reconstruction subsets. */
    int numSubsets;
    /** The reconstruction field of view in x and y direction in mm. */
    double fov[2];
    /** Name of the reconstruction preset, may be empty. */
    char presetName[VISU_MAX_NAME_LEN];
};

/** 
 * PET Radionuclide.
 * From DICOM PS 3.16 CID 4020.
 */
enum VisuRadionuclide
{
    RN_11_CARBON,
    RN_13_NITROGEN,
    RN_14_OXYGEN,
    RN_15_OXYGEN,
    RN_18_FLUORINE,
    RN_22_SODIUM,
    RN_38_POTASSIUM,
    RN_43_SCANDIUM,
    RN_44_SCANDIUM,
    RN_45_TITANIUM,
    RN_51_MANGANESE,
    RN_52_IRON,
    RN_52_MANGANESE,
    RN_52M_MANGANESE,
    RN_60_COPPER,
    RN_61_COPPER,
    RN_62_COPPER,
    RN_62_ZINC,
    RN_64_COPPER,
    RN_66_GALLIUM,
    RN_68_GALLIUM,
    RN_68_GERMANIUM,
    RN_70_ARSENIC,
    RN_72_ARSENIC,
    RN_73_SELENIUM,
    RN_75_BROMINE,
    RN_76_BROMINE,
    RN_77_BROMINE,
    RN_82_RUBIDIUM,
    RN_86_YTTRIUM,
    RN_89_ZIRCONIUM,
    RN_90_NIOBIUM,
    RN_90_YTTRIUM,
    RN_94M_TECHNETIUM,
    RN_124_IODINE,
    RN_152_TERBIUM,
    RN_USER_DEFINED
};

/** 
 * PET Radiopharmaceutical.
 * From DICOM PS3.16 CID 4021
 */

enum VisuRadiopharmaceutical
{
    RP_28H1_89_ZR,
    RP_2FA_F_18_,
    RP_7D12_89_ZR,
    RP_7E11_89_ZR,
    RP_ACETATE_C_11_,
    RP_AGN_150998_89_ZR,
    RP_AMMONIA_N_13_,
    RP_ANTI_B220_89_ZR,
    RP_ATSM_CU_60_,
    RP_ATSM_CU_61_,
    RP_ATSM_CU_62_,
    RP_ATSM_CU_64_,
    RP_BENRALIZUMAB_89_ZR,
    RP_BEVACIZUMAB_89_ZR,
    RP_BLINATUMOMAB_89_ZR,
    RP_BRENTUXIMAB_89_ZR,
    RP_BUTANOL_O_15_,
    RP_CARBON_DIOXIDE_O_15_,
    RP_CARBON_MONOXIDE_C_11_,
    RP_CARBON_MONOXIDE_O_15_,
    RP_CARFENTANIL_C_11_,
    RP_CETUXIMAB_89_ZR,
    RP_CG250_F_AB_2_89_ZR,
    RP_CHOLINE_C_11_,
    RP_CLR1404_I_124_,
    RP_CLR1404_I_131_,
    RP_CMAB_U36_89_ZR,
    RP_CU36_89_ZR,
    RP_DF_FK_2_89_ZR,
    RP_DF_FK_2_3PEG_4_89_ZR,
    RP_DF_CD45_89_ZR,
    RP_DF_FK_89_ZR,
    RP_DF_FK_PEG_3_89_ZR,
    RP_DN30_89_ZR,
    RP_E4G10_89_ZR,
    RP_ECROMEXIMAB_89_ZR,
    RP_EDOTREOTIDE_GA_68_,
    RP_EDTA_GA_68_,
    RP_FALLYPRIDE_C_11_,
    RP_FALLYPRIDE_F_18_,
    RP_FLB_457_C_11_,
    RP_FLORBETABEN_F_18_,
    RP_FLORBETAPIR_F_18_,
    RP_FLUBATINE_F_18_,
    RP_FLUCICLATIDE_F_18_,
    RP_FLUCICLOVINE_F_18_,
    RP_FLUMAZENIL_C_11_,
    RP_FLUMAZENIL_F_18_,
    RP_FLUORETHYLTYROSIN_F_18_,
    RP_FLUOROBENZOTHIAZOLE_F_18_,
    RP_FLUOROCHOLINE_F_18_,
    RP_FLUORODEOXYGLUCOSE_F_18_, /* FDG */
    RP_FLUOROESTRADIOL_FES_F_18_,
    RP_FLUOROETANIDAZOLE_F_18_,
    RP_FLUORO_L_DOPA_F_18_,
    RP_FLUOROMETHANE_F_18_,
    RP_FLUOROMISONIDAZOLE_F_18_,
    RP_FLUOROPROPYL_DIHYDROTETRABENAZINE_DTBZ_F_18_,
    RP_FLUOROTRIOPRIDE_F_18_,
    RP_FLUOROURACIL_F_18_,
    RP_FLUTEMETAMOL_F_18_,
    RP_FRESOLIMUMAB_89_ZR,
    RP_GA201_89_ZR,
    RP_GERMANIUM_GE_68_,
    RP_GLEMBATUMUMAB_VEDOTIN_89_ZR,
    RP_GLUTAMATE_N_13_,
    RP_GLUTAMINE_C_11_,
    RP_GLUTAMINE_C_14_,
    RP_GLUTAMINE_F_18_,
    RP_ISO_1_F_18_,
    RP_J591_89_ZR,
    RP_MARGETUXIMAB_89_ZR,
    RP_MEDI_551_89_ZR,
    RP_MESPIPERONE_C_11_,
    RP_METHIONINE_C_11_,
    RP_MOGAMULIZUMAB_89_ZR,
    RP_MONOCLONAL_ANTIBODY_MAB_64_CU,
    RP_MONOCLONAL_ANTIBODY_MAB_89_ZR,
    RP_MONOCLONAL_ANTIBODY_I_124_,
    RP_NANOCOLLOIDAL_ALBUMIN_89_ZR,
    RP_NIFENE_F_18_,
    RP_OBINITUZIMAB_89_ZR,
    RP_OCARATUZUMAB_89_ZR,
    RP_OXYGEN_O_15_,
    RP_OXYGEN_WATER_O_15_,
    RP_PALMITATE_C_11_,
    RP_PANITUMUMAB_89_ZR,
    RP_PEGDINETANIB_89_ZR,
    RP_PINATUZUMAB_VEDOTIN_89_ZR,
    RP_PITTSBURGH_COMPOUND_B_C_11_,
    RP_POLATUZUMAB_VEDOTIN_89_ZR,
    RP_PTSM_CU_62_,
    RP_R1507_89_ZR,
    RP_RACLOPRIDE_C_11_,
    RP_RANIBIZUMAB_89_ZR,
    RP_RITUXIMAB_89_ZR,
    RP_RO5323441_89_ZR,
    RP_RO542908_89_ZR,
    RP_ROLEDUMAB_89_ZR,
    RP_RUBIDIUM_CHLORIDE_RB_82_,
    RP_SAR3419_89_ZR,
    RP_SODIUM_FLUORIDE_F_18_,
    RP_SODIUM_IODIDE_I_124_,
    RP_SODIUM_NA_22_,
    RP_SPIPERONE_F_18,
    RP_T807_F_18_,
    RP_THYMIDINE_FLT_F_18_,
    RP_TRASTUZUMAB_89_ZR,
    RP_TRC105_89_ZR,
    RP_TYROSINE_3_OCTREOTATE_GA_68_,
    RP_UBLITUXIMAB_89_ZR,
    RP_XMAB5574_89_ZR,
    RP_USER_DEFINED
};

/** PET Compound type. */
struct VisuPtCompoundType
{
    /** Compound Radiopharmaceutical. */
    VisuRadiopharmaceutical radiopharmaceutical;
    
    /** 
     * Compound name. 
     * May be empty, must have a value if \c radiopharmaceutical is 
     * \c RD_USER_DEFINED.
     */
    char compoundName[VISU_MAX_NAME_LEN];
    
    /// Isotope enumeration.
    VisuRadionuclide radionucloide;
    
    /** 
     * Isotope name. 
     * May be empty, must have a value if \c radionucloide is 
     * \c RN_USER_UNDEFINED.
     */
    char isotopeName[VISU_MAX_NAME_LEN];
    /// Isotope branching fraction.
    double branchingFraction;
    /// Isotope half life in s.
    double halfLife;

    /** The administration route. */
    PvAdminRouteEnum adminRoute;
    
    /** 
     * The administration route name.
     * May be empty, must have a value if \c adminRoute \c UNKNOWN_ROUTE. 
     */
    char adminRouteName[VISU_MAX_NAME_LEN];
    
    /** The administration time. */
    pvtime_t adminTime;
    /** The dose at the administration time in MBq. */
    double adminDose;
};


#endif /* VISUTYPES_H */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



