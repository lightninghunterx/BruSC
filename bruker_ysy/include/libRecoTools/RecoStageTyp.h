/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2000-2004
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef RECOSTAGETYPES_H
#define RECOSTAGETYPES_H

/******************************************************************/
/*  the type definitions made here can be used in Overlays for    */
/*  parameter definition as well as in the definition of          */
/*  a CPR-module for generating a pipeline-filter.                */
/******************************************************************/

#define RECOSTAGEINSTSIZE 16
#define RECOSTAGENODESIZE 4096
#define RECOSTAGEEDGESIZE 64
#define RECOSTAGEPASSSIZE 64

/* strings for enums */
#define RECOSTAGE_FLOAT      "FLOAT"
#define RECOSTAGE_SIGNED     "SIGNED"
#define RECOSTAGE_UNSIGNED   "UNSIGNED"
#define RECOSTAGE_REAL       "REAL"
#define RECOSTAGE_COMPLEX    "COMPLEX"

/* string definition for stage type identification */

#define RECOBACKPROFILTER        "RecoBackproFilter"
#define RECOBCFILTER             "RecoBCFilter"
#define RECOBUFFERSINK           "RecoBufferSink"
#define RECOBUFFERSOURCE         "RecoBufferSource"
#define RECOCASTFILTER           "RecoCastFilter"
#define RECOCOMBINEFILTER        "RecoCombineFilter"
#define RECOCOMPLEXPHASEFILTER   "RecoPhaseFilter"
#define RECOCONJQNEGFILTER       "RecoConjQnegFilter"
#define RECOCUTOFFFILTER         "RecoCutoffFilter"
#define RECODTIFILTER            "RecoDTIFilter"
#define RECOFILESINK             "RecoFileSink"
#define RECOFILESOURCE           "RecoFileSource"
#define RECOFTFILTER             "RecoFTFilter"
#define RECOFTSHIFTFILTER        "RecoFTShiftFilter"
#define RECOGAUSSWINMULTFILTER   "RecoGaussWinMultFilter"
#define RECOHISTOGRAMFILTER      "RecoHistogramFilter"
#define RECOIMAGEDISPLAYFILTER   "RecoImageDisplayFilter"
#define RECOMAGNITUDEFILTER      "RecoMagnitudeFilter"
#define RECOMAPFILTER            "RecoMapFilter"
#define RECOMETHODFILTER         "RecoMethodFilter"
#define RECOMINMAXFILTER         "RecoMinMaxFilter"
#define RECOMULTFILTER           "RecoMultFilter"
#define RECOPHASCORRFILTER       "RecoPhasCorrFilter"
#define RECOPROFILEDISPLAYFILTER "RecoProfileDisplayFilter"
#define RECOQUEUESINK            "RecoQueueSink"
#define RECOQUEUESOURCE          "RecoQueueSource"
#define RECOSHAREDQUEUESINK      "RecoSharedQueueSink"
#define RECOSHAREDQUEUESOURCE    "RecoSharedQueueSource"
#define RECOREGRIDFILTER         "RecoRegridFilter"
#define RECOREIMFILTER           "RecoReImFilter"
#define RECOREVERSEFILTER        "RecoReverseFilter"
#define RECOSCALEFILTER          "RecoScaleFilter"
#define RECOSORTFILTER           "RecoSortFilter"
#define RECOSPLITFILTER          "RecoSplitFilter"
#define RECOSUMOFSQUARESFILTER   "RecoSumOfSquaresFilter"
#define RECOTEEFILTER            "RecoTeeFilter"
#define RECOTRANSPOSEFILTER      "RecoTransposeFilter"
#define RECOZFILLFILTER          "RecoZfillFilter"
#define RECOGRADCORRFILTER       "RecoGradCorrFilter"


/** The RecoStageNodeT is the structure describing internally the
 *  node description
 */
typedef struct {
    /** text description of the instance */
    char instance[RECOSTAGEINSTSIZE];
    /** index of pass */
    int pass;
    /** text description of node */
    char node[RECOSTAGENODESIZE];
} RecoStageNodeT;
/** The RecoStageEdgeT is the structure definition for internal
 *  description of edges connecting node in text form.
 *  Each description is assigned to a unique combination of pass
 *  and instance
 */
typedef struct {
    /** textual id of instance, e.g. reco, merge */
    char instance[RECOSTAGEINSTSIZE];
    /** ndx of pass */
    int pass;
    /** text description of connectors */
    char edge[RECOSTAGEEDGESIZE];
} RecoStageEdgeT;
/**
 * The RecoStagePassT is the structure describing internally the
 * mapping between the passId and the pass index
 */
typedef struct {
    /** textual id of pass */
    char passId[RECOSTAGEPASSSIZE];
    /** ndx of pass */
    int pass;
} RecoStagePassT;

#endif /* RECOSTAGETYPES_H */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
