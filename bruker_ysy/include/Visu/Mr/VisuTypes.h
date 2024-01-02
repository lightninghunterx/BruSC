/****************************************************************
 *
 * Copyright (c) 2018
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 ****************************************************************/

#ifndef MR_VISUTYPES_H
#define MR_VISUTYPES_H

#include "Visu/Mxi/VisuTypes.h"

/** Deprecated Phase encoding directions. */
enum VISU_PHASE_ENC_DIR
{
    no_phase_dir,
    row_dir,
    col_dir,
    slice_dir,
    row_col_dir,
    row_slice_dir,
    col_slice_dir,
    row_col_slice_dir
};

/** Visu encoding indicators. */
enum VISU_GRAD_ENCODING
{
    read_enc,
    phase_enc,
    slice_enc,
    no_gradient_enc
};

/** Enumeration to describe the echo sequence type. */
enum VisuEchoSeqType
{
    /** A spin echo sequence. */
    SpinEcho,

    /** A gradient echo sequence. */
    GradientEcho,

    /** A spin and gradient echo sequence.*/
    Both
};

/** Enumeration to describe the suppression type. */
enum VisuSpectralSuppressionType
{
    /** Scan was acquired using fat suppression. */
    FatSuppression,

    /** Scan was acquired using water suppression. */
    WaterSuppression,

    /** Scan was acquired using fat and water suppression. */
    FatAndWaterSuppression,

    /** Scan was acquired without any suppression. */
    NoSuppression
};

/** Type of k-space traversal. */
enum VisuKSpaceTraversalType
{
    /** k-space traversal is rectilinear. */
    RectilinearTraversal,

    /** k-space traversal is radial. */
    RadialTraversal,

    /** k-space traversal is spiral. */
    SpiralTraversal
};

/** Encoding order of directions. */
enum VisuEncodingOrder
{
    /** Linear encoding order. */
    LinearEncoding,

    /** Centric encoding order. */
    CentricEncoding
};

/** Enumeration of flow compensation techniques. */
enum VisuFlowCompensation
{
    /** Flow compensation acceleration. */
    FlowAcceleration,
    /** Flow compensation velocity. */
    FlowVelocity,
    /** Flow compensation other. */
    FlowOther,
    /** No flow compensation. */
    FlowNone
};

/** Spoiling type for acquisition. */
enum VisuSpoilingType
{
    /** RF spoiled. */
    RFSpoiled,
    /** Gradient spoiled. */
    GradientSpoiled,
    /** RF and gradient spoiled. */
    RFAndGradientSpoiled,
    /** Not spoiled. */
    NotSpoiled
};

enum VisuTaggingDirection
{
    /** Use a grid for tagging. */
    TagGrid,
    /** Use lines for tagging. */
    TagLine
};

/** Type to describe the acquisition tagging. */
struct VisuTaggingType
{
    /** The tagging direction. */
    VisuTaggingDirection direction;
    /** Space between tagging lines in mm in both directions. */
    double distance[2];
    /** Angle from rows clockwise for both directions. */
    double angle[2];
    /** Tagging line thickness. */
    double thickness;
};

/** Type of saturation. */
enum VisuSaturationType
{
    /** Field of view saturation. */
    FovSaturation,

    /** Flow saturation parallel to slices. */
    FlowSaturation
};

/** Definition of a saturation slab. */
struct VisuSaturationAttributes
{
    /** Type of saturation. */
    VisuSaturationType satType;

    /** Thickness of saturation slab. */
    double thickness;

    /** Direction cosines of a normal vector perpendicular to the 
     * saturation plane.  */
    double orientation[3];

    /** Offset in slice direction  */
    double position[3];
};


/** K-Space filtering of MR images. */
enum VisuMrKSpaceFilterType
{
    DICOM_COSINE,
    DICOM_COSINE_SQUARED,
    DICOM_FERMI,
    DICOM_GAUSSIAN,
    DICOM_HAMMING,
    DICOM_HANNING,
    DICOM_LORENTZIAN,
    DICOM_LRNTZ_GSS_TRNSF,
    DICOM_RIESZ,
    DICOM_TUKEY,
    DICOM_NONE,
    PV_EXPONENTIAL,
    PV_SINC,
    PV_SINC_SQUARED,
    PV_TRAPEZOID,
    PV_TRAFICANTE,
    PV_TRAFICANTE_SQUARED,
    PV_SQUARE,
    PV_USER_DEF_WDW

};

#endif /* VISUTYPES_H */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



