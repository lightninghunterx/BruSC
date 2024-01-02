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

#ifndef PVOVLTOOLSMR_RECO_H
#define PVOVLTOOLSMR_RECO_H

#ifndef CPROTO

#include <cstdint>
#include "mr/recotyp.h"
#include "mr/ovl_toolbox/DLLMacro.h"

/* string definitions to be used with toolbox functions */

/* strings for process definition */
/** string to identify input process */
#define RECIN                  "input"
/** string to identify compute process */
#define RECO                   "compute"
/** string to identify output process */
#define RECOUT                 "output"

/** string to identify first reconstruction pass */
#define RECOFIRSTPASS          "FIRSTPASS"
/** string to identify first nd-regridding pass */
#define RECOREGRIDNPASS        "REGRIDNPASS"
/** string to identify Fourier Transform reconstruction pass */
#define RECOFTPASS             "FTPASS"
/** string to identify Fourier Transform 1D reconstruction pass */
#define RECOFTPASS1            "FTPASS1"
/** string to identify Fourier Transform 2D reconstruction pass */
#define RECOFTPASS2            "FTPASS2"
/** string to identify Grappa reconstruction pass */
#define RECOGRAPPAPASS         "GRAPPAPASS"
/** string to identify Slice-Grappa reconstruction pass */
#define RECOSLICEGRAPPAPASS         "SLICEGRAPPAPASS"
/** string to identify Slice-Grappa reconstruction pass */
#define RECOSLICEGRAPPAREORDERPASS         "SLICEGRAPPAREORDERPASS"
/** string to identify Image Preparation reconstruction pass */
#define RECOPREPPASS           "PREPPASS"
/** string to identify Histogram reconstruction pass */
#define RECOHISTOGRAMPASS      "HISTOGRAMPASS"
/** string to identify Mapping reconstruction pass */
#define RECOMAPPASS            "MAPPASS"

/** string to identify n-th Fourier Transform reconstruction pass */
#define DEFINE_RECOFTNAMES            \
static const char* RecoFTPassName[] = \
{                                     \
  RECOFTPASS"0",                      \
  RECOFTPASS"1",                      \
  RECOFTPASS"2",                      \
  RECOFTPASS"3",                      \
  RECOFTPASS"4",                      \
  RECOFTPASS"5",                      \
  RECOFTPASS"6",                      \
  RECOFTPASS"7",                      \
  RECOFTPASS"8",                      \
  RECOFTPASS"9"                       \
};

/* strings for default filter names */

/** string to identify default source */
#define DEFAULTSOURCENAME          "Q"
/** string to identify default sink */
#define DEFAULTSINKNAME            "S"
/** string to identify default split filter */
#define DEFAULTSPLITFILTERNAME     "SP"
/** string to identify default fourier transform filter */
#define DEFAULTFTFILTERNAME        "FT"
/** string to identify default FT-shift filter */
#define DEFAULTFTSHIFTFILTERNAME   "FTS"
/** string to identify default image type filter */
#define DEFAULTIMAGETYPEFILTERNAME "G"
/** string to identify default method filter */
#define DEFAULTMETHODFILTERNAME    "PVM"
/** string to identify default mapping filter */
#define DEFAULTMAPFILTERNAME       "MAP"
/** string to identify default magnitude filter */
#define DEFAULTMAGNITUDEFILTERNAME "MAG"
/** string to identify default window multiplication filter */
#define DEFAULTWINMULTFILTERNAME   "WIN"
/** string to identify default casting filter */
#define DEFAULTCASTFILTERNAME      "CAST"
/** string to identify default combination filter */
#define DEFAULTCOMBINEFILTERNAME   "COMB"
/** string to identify default zero filling filter */
#define DEFAULTZFILLFILTERNAME     "Z"


/*==============================================================*/
/** @file
 *  @addtogroup PvOvlTools Overlay Toolbox
 *  <p>This utility library contains general functions for:</p>
 * \if PVDOXY
 *  - setting up standard reconstruction networks from
 *    within another parameter overlay
 * \endif
 *  - modifying standard reconstruction networks from
 *    within method code
 */
/*@{*/

#ifdef __cplusplus
/** Helper Class to encapsulate access to shared memories
 *  needed to use RecoMethodFilter.
 */

class DLL_INTERFACE RecoGlobalMemory {
 public:
    /** constructor to attach to shared memory
     */
    RecoGlobalMemory(int id);
    /** destructor detaches from shared memory
     */
    ~RecoGlobalMemory();
    /** get address shared memory is mapped to
     *  @return pointer to memory address where shared memory
     *          was mapped. Returns 0 when attaching failed
     */
    void* getAddress();
 private:
    /** No copy construction. */
    RecoGlobalMemory(const RecoGlobalMemory &rhs) = delete;

    /** No assignment. */
    RecoGlobalMemory &operator=(const RecoGlobalMemory &rhs) = delete;

    /// internal storage for memory address
    void* ptr_;
};
#endif

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* gen/src/prg/ovl_toolbox/RecoTools.c */

/*==============================================================*/
/** return the number of currently selected receivers for data acquisition per
 *  job or logical channel
 *
 * @return the number of currently active receivers
 */
/*==============================================================*/
DLL_INTERFACE int RecoNrActiveReceivers(); // LEGACY FUNCTION -> calls RecoNrActiveReceiversForJob(0)
DLL_INTERFACE int RecoNrActiveReceiversForJob(const int jobIdx);
DLL_INTERFACE int RecoNrActiveReceiversForChan(const int chanNum);

/*==============================================================*/
/** utiltity function to connect stages in network
 *
 * Add a connector between given stages.
 *
 * Named connectors may be specified and will be correctly arranged
 * with a possible thread index.
 *
 * @param passId    text ID to identify the pass of the network
 * @param nrThread  index of the thread for automatic name extension,
 *                  the value -1 will deactivate name completion
 * @param inputId   text ID of the stage where connection starts
 *                  including possibly named connector
 * @param outputId  text ID of the stage where connection ends
 *                  including possibly named connector
 */
/*==============================================================*/
DLL_INTERFACE void RecoComputeConnectStages (const char *passId,
                                             const int nrThread,
                                             const char *inputId,
                                             const char *outputId);

/*==============================================================*/
/** utiltity function to remove connection between stages in network
 *
 * Remove the connector between given stages.
 *
 * Named connectors may be specified and will be correctly arranged
 * with a possible thread index.
 *
 * @param passId    text ID to identify the pass of the network
 * @param nrThread  index of the thread for automatic name extension,
 *                  the value -1 will deactivate name completion
 * @param inputId   text ID of the stage where connection starts
 *                  including possibly named connector
 * @param outputId  text ID of the stage where connection ends
 *                  including possibly named connector
 */
/*==============================================================*/
DLL_INTERFACE void RecoComputeDisconnectStages (const char *passId,
                                                const int nrThread,
                                                const char *inputId,
                                                const char *outputId);

/*==============================================================*/
/** utiltity function to add a stage to a network
 *
 * Add a reco stage to the given pass in the given instance.
 * Do not make any connections to other stages.
 * As the pass is defined by its numerical id, it is assumed
 * that the corresponding pass has been constructed before.
 *
 * The complete stage definition consisting of type, id and
 * description must fit into the RecoStageNodes.node character
 * string (RECOSTAGENODESIZE defined in RecoTyp.h)
 *
 * @param passId    text ID to identify the pass of the network
 * @param nrThread  index of the thread for automatic name
 *                  extension, the value -1 will deactivate
 *                  name completion
 * @param stageType text string defining the filter class
 *                  it is not checked, whether this string
 *                  defines an existing stage. When an unknown
 *                  type is used this will lead to runtime
 *                  errors when reco is started.
 * @param stageId   text ID of the filter - will not be tested
 *                  whether it is unique here, double entries
 *                  will lead to error messages when reco
 *                  is started
 * @param stageArguments argument string for the stage to be
 *                  created.
 */
/*==============================================================*/
DLL_INTERFACE void  RecoComputeAddStage (const char *passId,
                                         const int  nrThread,
                                         const char *stageType,
                                         const char *stageId,
                                         const char *stageArguments);

/*==============================================================*/
/** Utiltity function to remove a stage from a network
 *
 * Remove the stage description for specified network
 * if it exists. The return value indicates, whether a stage
 * was removed. All connections beginning in the description
 * or leading to it will be removed as well. It is not checked
 * whether any nodes remain in the given network. The method
 * does not work properly, when named connectors are used or
 * when more than one input or output is attached. @par
 *  remove(X) @par
 *  A->X->B  =>  A->B @par
 *
 * @param passId    text ID to identify the pass of the network
 * @param nrThread  index of the thread for automatic name
 *                  extension, the value -1 will deactivate
 *                  name completion
 * @param stageId   text ID of the stage to be removed
 *                  if the stage does not exist No will be
 *                  returned.
 * @return          returns Yes, when a stage was found and
 *                  removed,  No when no stage was found.
 */
/*==============================================================*/
DLL_INTERFACE YesNo RecoComputeRemoveStage (const char *passId,
					    const int nrThread,
					    const char *stageId);

/*==============================================================*/
/** utility function to insert stage into network
 *
 * Insert a stage in a given network in a given thread in front of
 * a specific stage. Multiple inputs and outputs are not supported.
 *
 *    insert(..,B,X): @par
 *    A ->B     =>  A->X->B @par
 *
 * Named connectors may be specified and will be correctly arranged
 * with a possible thread index.
 *
 * @param passId   index of the pass
 * @param position id of the stage in front of which the
 *      new stage is to be inserted
 * @param nrThread  index of the thread used for name
 *      completion, the value -1 will deactivate
 *                  name completion
 * @param stageType text string defining the filter class
 *      it is not checked, whether this string defines
 *      an existing stage. When an unknown type is used
 *      this will lead to runtime errors when acquisition is
 *      started.
 * @param stageId textID of the new Stage - will not be tested
 *      whether it is unique here, double entries
 *      will lead to error messages when reco is started
 * @param stageArguments parameter string for the
 *      inserted stage.
 */
/*==============================================================*/
DLL_INTERFACE void RecoComputeInsertStage (const char * passId,
                                           const int  nrThread,
                                           const char *position,
                                           const char *stageType,
                                           const char *stageId,
                                           const char *stageArguments);

/*==============================================================*/
/** utility function to append stage into network
 *
 * Append a stage in a given network in a given thread behind
 * a specific stage. Multiple inputs and outputs are not supported.
 *
 *    append(..,A,X): @par
 *    A->B  =>  A->X->B @par
 *
 * Named connectors may be specified and will be correctly arranged
 * with a possible thread index.
 *
 * @param passId index of the pass
 * @param nrThread  index of the thread used for name
 *      completion, use -1 to avoid name completion
 * @param position id of the stage behind which the
 *      new stage is to be inserted
 * @param stageType text string defining the filter class.
 *      It is not checked, whether this string defines
 *      an existing stage. If an unknown type is used
 *      this will lead to runtime errors when acquisition is
 *      started.
 * @param stageId textID of the new Stage. It is not checked
 *      if it is unique. Multiple entries will lead to error
 *      messages when reco is started.
 * @param stageArguments parameter string for the
 *      appended stage.
 */
/*==============================================================*/
DLL_INTERFACE void RecoComputeAppendStage (const char *passId,
                                           const int  nrThread,
                                           const char *position,
                                           const char *stageType,
                                           const char *stageId,
                                           const char *stageArguments);

/*==============================================================*/
/** utility function to to append a filter for reproducing data
 *
 * This convenience function is used to insert a filter
 * at the given position that reproduces the input data @a nRep times.
 *
 * The functionality is accomplished by combining a @em TEE and a
 * @em COMBINE filter.
 *
 * A typical application is the reproduction of a trajectory provided
 * for a single slice that is used for every slice in a multiple-slice
 * data set.
 *
 * @param passId     index of the pass
 * @param nrThread   index of the thread for automatic name extension,
 * @param position   id of the stage after which the new stage is to be inserted,
 *                   the value -1 will deactivate name completion
 * @param stageId    textID of the new stage
 * @param nRep       number of reproductions (current maximum 256)
 * @param procDim    new processing dimension
 */

/// @cond PVDOXY
// Omit these toolbox functions in user docu

/*==============================================================*/
DLL_INTERFACE void RecoComputeAppendReproduceFilter(
  const char         *passId,
  const int          nrThread,
  const char         *position,
  const char         *stageId,
  const unsigned int nRep,
  unsigned int       procDim);

/*==============================================================*/
/** insert description of complete input process
 *
 * Derive the complete pipeline structure for the input
 * filter process in the reconstruction pipeline.
 *
 * The default stage description of an input process is
 * derived in this function. This needs to be provided only
 * during offline reconstruction or when (in future) disk-reco
 * is used.
 */
/*==============================================================*/
DLL_INTERFACE void RecoDeriveInputProcess (void);

/*==============================================================*/
/** insert description of complete compute process
 *
 * Derive the complete pipeline structure for the compute
 * filter process in the reconstruction pipeline.
 *
 * The default stage description of the compute process is
 * derived in this function. The passes used for computation
 * typically are:
 * - @b FIRSTPASS to provide manipulations on scans (FT filter ...)
 *   and sort data
 * - @b FTPASS to perform Fourier transformation in higher spatial
 *   directions
 * - @b PREPPASS to transform data into the desired final format
 * - @b MAPPASS  to map data into the given range
 *
 * With Grappa or 2D/3D Regridding the above order of passes are modified.
 *
 * The functionality may be overwritten in method code. Separate
 * functions are provided to create the different passes.
 *
 * @param grappaMode Grappa processing mode
 *                   - GrappaProcStandard: coefficients are determined and applied directly
 *                   - GrappaProcCalibrate: coefficients are calibrated and stored (e.g. EPI Grappa adjustment)
 *                   - GrappaProcApply: coefficients are restored and applied to the data (e.g. EPI scan)
 * @param epiCombine Yes = specific behaviour for EPI double sampling
 */
/*==============================================================*/
DLL_INTERFACE void RecoDeriveComputeProcess (
    RECO_GRAPPA_PROC_MODE grappaMode = GrappaProcStandard,
    YesNo epiCombine = No);

/*==============================================================*/
/** insert description of a dummy compute process
 */
/*==============================================================*/
DLL_INTERFACE void RecoDeriveDummyComputeProcess();

/*==============================================================*/
/** insert description of complete compute process for MPI reconstruction
 */
/*==============================================================*/
DLL_INTERFACE void RecoDeriveMPIComputeProcess(unsigned int basisTrafo, bool SNRIsPrecomputed, bool basisTrafoIsPrecomputed, bool SelectMixedFrequencies);

/*==============================================================*/
/** insert description of complete compute process for MPI calibration
 */
/*==============================================================*/
DLL_INTERFACE void RecoDeriveMPICalibrationComputeProcess(unsigned int basisTrafo);

/*==============================================================*/
/** insert description of complete compute process for compressed sensing reconstruction
 */
/*==============================================================*/
DLL_INTERFACE void RecoDeriveCSComputeProcess();

/*==============================================================*/
/** insert description of complete compute process for backprojection
 */
/*==============================================================*/
DLL_INTERFACE void RecoDeriveBackproComputeProcess (void);

/*==============================================================*/
/** insert description of complete output process
 *
 * The default stage description of an output process is
 * derived in this function. It includes
 * - reading data from a interprocess pipeline queue (the used
 *   blocksize depends on the data format and should be sufficient
 *   to hold at least a 2D frame or a complete phase encoding step
 *   in 1D mode
 * - sending data to a spy display process
 * - storing data into a file
 * - writing parameter files during wrapup @par
 *
 * The function must (currently) be called after setting up the
 * compute process because the pass structure depends on the
 * passes defined before.
 *
 */
/*==============================================================*/
DLL_INTERFACE void RecoDeriveOutputProcess (void);

/*==============================================================*/
/** add a default mapping pass for the compute process instance
 *
 * The mapping pass acts on a given !single! buffer and
 * will write data to the output queue. As a precondition for most
 * apping functions, the RECO_map_min and RECO_map_max parameters
 * must have been assigned a value, when this pass is run, because they
 * will provide information about the data which will be used later
 * on for mapping.
 *
 *  mapping comprises the following steps
 *  - read data from buffer
 *  - map data according to @b RECO_map_mode
 *  - cast data according to @b RECO_wordtype
 *  - write data to queue to output process (as 2D frame/pe-step)
 *
 *  There should hardly be any need to change the behaviour of
 *  this pass as mapping is always needed and RECO_map parameters
 *  are written by the Map filter. This is in fact essential
 *  for the correct display in image processing tools and setting
 *  of the VISU parameters.
 *  As precondition, the @b RECO_map_min and @b RECO_map_max
 *  parameters must have been set per image object.
 *  How is real/complex/coil data handled : do they define
 *  different image objects for mapping? They probably should not!
 *
 *  @image html RecoMapPass.png
 *
 *  @param passId text string to define name of pass for later
 *                reference
 *  @param bufferId text string to define the data buffer
 *                for data input
 *  @return NULL, output currently always redirected to queue
 */
/*==============================================================*/
DLL_INTERFACE const char *RecoComputeAddMapPass (const char *passId,
						 const char *bufferId);

/** add a histogram pass for the compute process instance
 *
 * histogram pass consist of
 * - determination of the lower and upper quantils per repetition
 *
 *  @param passId text string to define name of pass for later
 *                reference
 *  @param bufferId text string to define the data buffer
 *                for data input
 *  @return NULL, output currently always redirected to queue
 */
/*==============================================================*/
DLL_INTERFACE const char *RecoComputeAddHistogramPass (const char *passId,
        const char *bufferId);

/**
 * add a dummy pass for the compute process instance
 */
/*==============================================================*/
DLL_INTERFACE void RecoComputeAddDummyPass(const char * passId);

/**
 * add a default MPI pass for the compute process instance
 * @param SNRIsPrecomputed boolean if the SNR has been precomputed
 * @return name of output buffer - will be bufferId
 */
/*==============================================================*/
DLL_INTERFACE const char* RecoComputeAddMPIPass(bool SNRIsPrecomputed, bool SelectMixedFrequencies);
/**
 * add a default MPI calibration pass for the compute process instance
 * @param basisTrafo basis transformation to be applied if the
 *        calibration positions are sparse
 * @return name of output buffer - will be bufferId
 */
/*==============================================================*/
DLL_INTERFACE const char* RecoComputeAddMPICalibrationPass(unsigned int basisTrafo);
/**
 * add a sparse MPI pass for the compute process instance
 * @param basisTrafo basis transformation to be applied
 * @param SNRIsPrecomputed boolean if the SNR has been precomputed
 * @param basisTrafoIsPrecomputed boolean if the basis transformation has been
 *        precomputed
 * @return name of output buffer - will be bufferId
 */
/*==============================================================*/
DLL_INTERFACE const char*  RecoComputeAddSparseMPIPass(unsigned int basisTrafo,
                       bool SNRIsPrecomputed, bool basisTrafoIsPrecomputed, bool SelectMixedFrequencies);

/**
 * add compressed sensing pass for the compute process instance
 * @return name of output buffer - will be bufferId
 */
/*==============================================================*/
DLL_INTERFACE const char* RecoComputeAddCSPass(const char* passId, 
				     const char* bufferId, 
				     int nrReceivers,
				     int full_dim);
/*==============================================================*/
/** add a default image preparation pass for the compute process
 *  instance
 *
 *  create default image preparation pass
 *
 *  image preparation consists of the following parts
 *  - linear phase correction
 *  - cutoff of an image window
 *  - regridding corrections
 *  - scaling of parallel channels
 *  - combining parallel channels
 *  - creation of the correct image typ (magnitude ...)
 *  - image transposition
 *  - determination of the image minima/maxima per frame
 *
 *  The composition of the corresponding stages is done according
 *  to the corresponding \c RECO parameters
 *
 *  @image html RecoPrepPass.png "preparation pass (incomplete)"
 *
 *  @param passId text string to define name of pass for later reference
 *  @param bufferId text string to define the data buffer for data input
 *  @param nrReceivers number of input channels for pass
 *  @param epiCombine Yes = increment number of processing dimensions (required for EPI double sampling)
 *
 *  @return  name of output buffer - will be bufferId
 *
 *  @see RecoComputeAddRegridNPass
 */
/*==============================================================*/
DLL_INTERFACE const char *RecoComputeAddPrepPass (const char *passId,
                                                  const char *bufferId,
                                                  int nrReceivers,
                                                  YesNo epiCombine = No);

/*==============================================================*/
/** add a default first pass for the compute process
 *  instance
 *
 *  First pass includes operation on 1D level and sorting
 *  data in the end. The detailed operation depends on
 *  reco parameters:
 *  - read data from queue
 *  - split parallel data
 *  - cast to double
 *  - B0 off-resonance demodulation
 *  - 1D regridding
 *  - reverse data direction
 *  - zero-filling
 *  - window multiplication
 *  - fourier transformation
 *  - spatial shift (rotation)
 *  - sorting of data according to @b RecoSort parameters
 *  - possibly reduce data dimension in case of 3D regridding
 *
 *  @param passId text string to define name of pass for later reference
 *  @param bufferId text string to define the data buffer for data input
 *  @param nrReceivers number of input channels for pass
 *  @param addFT Yes = apply FT
 *
 * @todo Control the fillingPosition of zero-filling (also for sorting).
 */
/*==============================================================*/
DLL_INTERFACE const char *RecoComputeAddFirstPass (const char *passId,
                                                   const char *bufferId,
                                                   int nrReceivers,
                                                   YesNo addFT);

/*==============================================================*/
/** add a default FT pass for a given spatial direction
 *
 *  FT pass includes the following operations - for multiple
 *  receiver applications. For each receiver a completely
 *  parallel thread will be created.
 *  - read data from buffer
 *  - window multiplication
 *  - fourier transformation (possible half-FT)
 *  - spatial shift (rotation)
 *  - store data to buffer
 *
 *  \image html RecoFTPass.png "possible filters in FT pass (not up-to-date)"
 *
 *  @param passId text string to define name of pass for later reference
 *  @param bufferId text string to define the data buffer for data input
 *  @param direction spatial direction for Fourier transformation
 *  @param exponent  exponent for Fourier operator: +1 or -1
 *  @param nrReceivers number of input channels for pass
 *
 *  @return  name of output buffer - will be bufferId
 */
/*==============================================================*/
DLL_INTERFACE const char *RecoComputeAddFTPass (const char *passId,
						const char *bufferId,
						int direction,
						int exponent,
						int nrReceivers);

/*==============================================================*/
/** add a default empty pass
 *
 *  The pass consists only of a source and a sink and includes
 *  the following operations for multiple receiver applications.
 *  For each receiver a completely parallel thread will be created.
 *  - read data from buffer
 *  - store data to buffer
 *
 *  @param passId text string to define name of pass for later
 *                reference
 *  @param bufferId text string to define the data buffer
 *                for data input
 *  @param procDim processing dimension of data (if set to 0,
 *                it will be derived from the data)
 *  @param nrReceivers number of input channels for pass
 *  @param nrOut (optional) number of output channels (0 = same as @em nrReceivers = default)
 *  @return  name of output buffer - will be bufferId
 */
/*==============================================================*/
DLL_INTERFACE const char *RecoComputeAddEmptyPass (const char *passId,
                                                   const char *bufferId,
                                                   int procDim,
                                                   int nrReceivers,
                                                   int nrOut = 0);

/*==============================================================*/
/** add a default backprojection pass for the compute process
 *  instance
 *
 *  @image html RecoBackproPass.png "backprojection pass"
 *
 *  @param passId text string to define name of pass for later
 *                reference
 *  @param bufferId text string to define the data buffer
 *                for data input
 *  @param nrReceivers number of input channels for pass
 *  @return  name of output buffer - will be bufferId
 */
/*==============================================================*/
DLL_INTERFACE const char *RecoComputeAddBackproPass(const char *passId,
						    const char *bufferId,
						    int nrReceivers);

/*==============================================================*/
/** add a default GRAPPA pass for the compute process instance
 *
 *  The GRAPPA pass comprises different stages for the GRAPPA
 *  parallel reconstruction algorithm.
 *  The composition of the corresponding stages is done according
 *  to the corresponding \c RECO parameters. A grappa pass
 *  consists of the following stages:
 *  - Extract reference lines
 *  - Calculate Calibration information
 *  - Perform GRAPPA algorithm
 *
 *  @image html RecoGrappaPass.png "typical GRAPPA pass"
 *
 *  @param passId text string to define name of pass for later
 *                reference
 *  @param bufferId text string to define the data buffer
 *                for data input
 *  @param nrReceivers number of input channels for pass
 *  @return  name of output buffer - will be bufferId
 */
/*==============================================================*/
DLL_INTERFACE const char *RecoComputeAddGrappaPass(const char *passId,
						   const char *bufferId,
						   int nrReceivers);

/*==============================================================*/
/** add a GRAPPA calibration pass for the compute process instance
 *
 *  The Grappa coefficients are calibrated from the data and stored.
 *
 *  Used parameters:
 *  - @c PVM_EpiGrappaCoefficients
 *
 *  @param passId text string to define name of pass for later
 *                reference
 *  @param bufferId text string to define the data buffer
 *                for data input
 *  @param nrReceivers number of input channels for pass
 *  @return  name of output buffer - will be bufferId
 */
/*==============================================================*/
DLL_INTERFACE const char *RecoComputeAddGrappaCalibPass(const char *passId,
							const char *bufferId,
							int nrReceivers);

/*==============================================================*/
/** Calculate the dimensions of the GRAPPA coefficients
 *
 *  @param grappa_result  provides the sizes for the different dimensions on return
 *  @param nrReceivers  number of receiver channels
 *  @param epiCombine  Yes = specific behaviour for EPI double sampling
 */
/*==============================================================*/
DLL_INTERFACE void RecoGrappaSize(int grappa_result[4],
                                  int nrReceivers,
                                  YesNo epiCombine);

/*==============================================================*/
/** Calculate the dimensions of the Slice-GRAPPA coefficients
 *
 *  @param slice_grappa_result  provides the sizes for the different dimensions on return
 *  @param nrReceivers  number of receiver channels
 *  @param epiCombine  Yes = specific behaviour for EPI double sampling
 */
/*==============================================================*/
DLL_INTERFACE void RecoSliceGrappaSize(int slice_grappa_result[3],
                                  int nrReceivers,
                                  YesNo epiCombine);
/*==============================================================*/
/** add a GRAPPA application pass for the compute process instance
 *
 *  The previously stored Grappa coefficients are applied to the data.
 *
 *  Used parameters:
 *  - @c PVM_EpiGrappaCoefficients
 *
 *  @param passId text string to define name of pass for later reference
 *  @param bufferId text string to define the data buffer for data input
 *  @param nrReceivers number of input channels for pass
 *  @param epiCombine Yes = specific behaviour for EPI double sampling
 *
 *  @return  name of output buffer - will be bufferId
 */
/*==============================================================*/
DLL_INTERFACE const char *RecoComputeAddGrappaApplyPass(const char *passId,
                                                        const char *bufferId,
                                                        int nrReceivers,
                                                        YesNo epiCombine = No);

/*==============================================================*/
/** add a Slice-GRAPPA application pass for the compute process instance
 *
 *  The previously stored Slice-Grappa coefficients are applied to the data.
 *
 *  Used parameters:
 *  - @c PVM_EpiSliceGrappaCoefficients
 *
 *  @param passId text string to define name of pass for later reference
 *  @param bufferId text string to define the data buffer for data input
 *  @param nrReceivers number of input channels for pass
 *  @param epiCombine Yes = specific behaviour for EPI double sampling
 *
 *  @return  name of output buffer - will be bufferId
 */
/*==============================================================*/
DLL_INTERFACE const char *RecoComputeAddSliceGrappaApplyPass(const char *passId,
                                                        const char *bufferId,
                                                        int nrReceivers,
                                                        YesNo epiCombine = No);
/*==============================================================*/
/** add a Slice-GRAPPA reorder pass for the compute process instance
 *
 *  The previously reconstructed Slice-Grappa slices are re-ordered.
 *
 *  Used parameters:
 *  - @c 
 *
 *  @param passId text string to define name of pass for later reference
 *  @param bufferId text string to define the data buffer for data input
 *  @param nrReceivers number of input channels for pass
 *
 *  @return  name of output buffer - will be bufferId
 */
/*==============================================================*/
DLL_INTERFACE const char *RecoComputeAddSliceGrappaReorderPass(const char *passId,
                                                        const char *bufferId,
                                                        int nrReceivers);
/*==============================================================*/
/** add a Slice-GRAPPA calibration pass for the compute process instance
 *
 *  The Slice-Grappa coefficients are calibrated from the data and stored.
 *
 *  Used parameters:
 *  - @c PVM_EpiSliceGrappaCoefficients
 *
 *  @param passId text string to define name of pass for later
 *                reference
 *  @param bufferId text string to define the data buffer
 *                for data input
 *  @param nrReceivers number of input channels for pass
 *  @return  name of output buffer - will be bufferId
 */
/*==============================================================*/
DLL_INTERFACE const char *RecoComputeAddSliceGrappaCalibPass(const char *passId,
							const char *bufferId,
							int nrReceivers);

/*==============================================================*/
/** add pass for 2D/3D regridding
 *
 *  A pass is created containing the first part of 2D or 3D regridding.
 *  It is assumed that the maximum data dimension is 3.
 *  The created filter combination includes:
 *  - reading the trajectory from file (the read trajectory will be repeated internally
 *    for every image, possibly phase encoding step in 3rd dimension, and repetition)
 *  - density correction
 *  - regridding
 *
 *  The second part is implemeted in the preparation pass,
 *  including:
 *  - undo oversampling
 *  - apodisation correction
 *  - cutting a circular/spherical FOV for radial and spiral methods
 *
 *  A license check for PVMUTE or PVMSPIRAL is performed.
 *
 *  The following PVM parameters determine the behaviour of the
 *  regridding procedure:
 *  - method baselevel parameters \c NI and \c NR
 *  - method parameter \c PVM_Matrix
 *  - reco parameters \c RECO_inp_size and \c RECO_ft_size
 *  - members of the \c RecoRegridNGroup
 *
 *  @param passId         index of the current pass
 *  @param bufferId       text string to define the buffer for data input
 *  @param nrReceivers    number of input channels for pass
 *
 *  The first part up to the regrid filter depends on the value of \c RecoRegridNDensCorr :
 *  \n\n
 *  @image html RegridFilterOrder1a.png
 *  \n
 *  @image html RegridFilterOrder1b.png
 *  \n
 *  @image html RegridFilterOrder1c.png
 *  \n
 *  The second part (as implemented in the preparation pass) is identical for all cases:
 *  \n\n
 *  @image html RegridFilterOrder2.png
 *  \n
 *  @see RecoRegridNFilter for details about reconstruction\n
 *
 *  @todo Implement
 *        - use same trajectory for all receivers
 *        - check proDim of @em COMBINE filter for 3D and @c NI > 1
 */
DLL_INTERFACE const char* RecoComputeAddRegridNPass(const char* passId,
                                                    const char* bufferId,
                                                    int         nrReceivers);

/*==============================================================*/
/** add passes for 2D/3D regridding
 *
 *  Add passes required for 2D or 3D regridding:
 *  - RegridN pass
 *  - Zero-filling
 *  - Fourier transforms
 *
 *  As id of the regridding pass RECOREGRIDNPASS is used.
 *
 *  @see RecoComputeAddRegridNPass
 */
DLL_INTERFACE const char* RecoComputeAddRegridNPasses(const char* bufferId,
                                                      int         nrReceivers);

/*==============================================================*/
/** prepare creation of compute process
 *
 *  This function must be called before any passes are created.
 *
 */
DLL_INTERFACE void RecoInitComputeProcess(void);

/// @endcond -- Omit above toolbox functions in user docu

#ifdef __cplusplus
}
#endif

/*@}*/

#endif

#endif


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
