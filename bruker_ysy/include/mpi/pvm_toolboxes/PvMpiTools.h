/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1995-2015
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVMPITOOLS_H
#define PVMPITOOLS_H

#include "mpi/PvTypes/mpiTypes.h"
#ifndef CPROTO

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

/// DLL interface libParx identifiers for Windows.
#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#  ifdef PvMpiTools_INTERNAL
#    define DLL_INTERFACE __declspec(dllexport)
#  else
#    define DLL_INTERFACE __declspec(dllimport)
#  endif
#elif defined (__GNUC__) && __GNUC__ >= 4
#    define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#    define DLL_INTERFACE
#endif 



//----------------------------------------------------//
//------------------- init groups --------------------//
//----------------------------------------------------//

/** Initialize the basic PVM/MPI parameters.
 
    A call to this function should be the first statement in the method initialization
    of every MPI method.
    
  @PVM_PARS
  PVM_MPI_Experiment
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
    - The system is not capable of executing MPI experiments.
    - Parameter PVM_MPI_Experiment does not exist.

  @PVM_EXAMPLE 
  @code

  MPT_InitMpiExperient();

  @endcode
  
  @return
  NONE
  
  @see
  NONE

  @ingroup PVM_MPT
*/
DLL_INTERFACE void MPT_InitMpiExperiment(void);

/** Initialize drive field parameter group.
 
    Depending on the dimensionality of the FOV covered by the drive field,
    different selections for the orientation are possible. This function
    initializes the selection parameter PVM_MPI_DF_Orientation to the appropriate options.
   
  @PVM_PARS
  PVM_MPI_DF_Orientation
  
  @PVM_TBCALL
  MPT_InitDriveFieldOrientations

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  MPT_InitMpiExperient() has been called.

  @PVM_ERROR
    - an illegal dimension was passed.

  @PVM_EXAMPLE 
  @code

  MPT_InitDriveField(3);

  @endcode
  
  @param dimension
  
  @return
  NONE
  
  @see
  NONE
  
  @ingroup PVM_MPT
*/

DLL_INTERFACE void MPT_InitDriveField(int dimension);

/** Initialize the quasi-static field channels of the MPI system.
 
    This includes the selection field and the focus field channels (if present).
   
  @PVM_PARS
  PVM_MPI_selection_field_gradient
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR

  @PVM_EXAMPLE 
  @code

  MPT_InitStaticFields();

  @endcode
  
  @param dimension
  
  @return
  NONE
  
  @see
  NONE
  
  @ingroup PVM_MPT
 */
DLL_INTERFACE void MPT_InitStaticFields(void);


/** Initialize the type of data representation
   
  @PVM_PARS
  PVM_MPI_Wordtype
  PVM_MPI_Wordsize
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR

  @PVM_EXAMPLE 
  @code

  MPT_InitDataRepresentation();

  @endcode
  
  @param 
  
  @return
  NONE
  
  @see
  NONE
  
  @ingroup PVM_MPT
 */
DLL_INTERFACE void MPT_InitDataRepresentation(void);

DLL_INTERFACE void MPT_InitReceivers(void);

DLL_INTERFACE void MPT_InitBandwidth(void);

/** Initalize spectral cleaning

  @PVM_PARS
  PVM_MPI_SpectralCleaning
  PVM_MPI_NrWindowedRepetitions
  PVM_MPI_WindowingFunction
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code


  MPT_InitSpecralCleaning();

  @endcode
  
  @param 
  NONE

  @return 
  NONE
  
  @see 
  NONE
  
  @ingroup PVM_MPI
*/
DLL_INTERFACE void MPT_InitSpectralCleaning(void);

DLL_INTERFACE void MPT_InitBackgroundMeasurementCalibration(void);

DLL_INTERFACE void MPT_InitSNRCalculation(void);

DLL_INTERFACE void MPT_InitImageMatrix(void);

DLL_INTERFACE void MPT_UpdateSNRCalculation(YesNo activateSNRWithinDFFov,
                              int dim,
                              double* fov,
                              double* df_fov,
                              int* sizes);

DLL_INTERFACE void MPT_InitTracer(const char * contrastAgent,
                    const char * batch,
                    double volume,
                    double concentration,
                    double particleDiameter=0.0,
                    double saturationMagnetization=0.0);

DLL_INTERFACE void MPT_InitRecoSetting(void);

/** Initalize the MPI algorithm parameter group

  @PVM_PARS
  PVM_MPI_ReconstructionAlgorithm
  PVM_MPI_EnforceReal
  PVM_MPI_EnforcePositive
  PVM_MPI_NrIterations
  PVM_MPI_RegularizationParameter
  PVM_MPI_MatrixRowWeighting
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code


  MPT_InitAlgorithm();

  @endcode
  
  @param 
  NONE

  @return 
  NONE
  
  @see 
  NONE
  
  @ingroup PVM_MPI
*/
DLL_INTERFACE void MPT_InitAlgorithm();



/** Initalize the MPI system matrix denoising parameter group

  @PVM_PARS
  PVM_MPI_ActivateSystemMatrixDenoising
  PVM_MPI_SystemMatrixDenoisingTrafoType
  PVM_MPI_SystemMatrixDenoisingThresholdType
  PVM_MPI_SystemMatrixDenoisingWeight
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  // initalize the default MPI sytem matrix denoising parameter

  MPT_InitDenoising();

  @endcode
  
  @param NONE

  @return 
  NONE
  
  @see 
  NONE
  
  @ingroup PVM_MPI
*/
DLL_INTERFACE void MPT_InitDenoising(void);



/** Initalize the MPI sparse reconstruction parameter group

  @PVM_PARS
  PVM_MPI_ActivateSparseReconstruction
  PVM_MPI_SparseRecoTrafoType
  PVM_MPI_PercentageBasisCoefficients
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  // initalize the default MPI sparse reconstruction parameter

  MPT_InitSparseReco();

  @endcode
  
  @param NONE

  @return 
  NONE
  
  @see 
  NONE
  
  @ingroup PVM_MPI
*/
DLL_INTERFACE void MPT_InitSparseReco(void);



/** Initalize the MPI frequency component selection parameter group

  @PVM_PARS
  PVM_MPI_SNRThreshold
  PVM_MPI_ActivateMixedFrequenciesSelection
  PVM_MPI_MaximumMixingOrder
  PVM_MPI_BorderFrequencyX
  PVM_MPI_BorderFrequencyY
  PVM_MPI_BorderFrequencyZ
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  // initalize the MPI frequency component selection parameter with an aquisition bandwidth of 0.625MHz

  MPT_InitFreqCompSelection(0.625);

  @endcode
  
  @param bandwidth bandwidth of the acquisition

  @return 
  NONE
  
  @see 
  NONE
  
  @ingroup PVM_MPI
*/
DLL_INTERFACE void MPT_InitFreqCompSelection(double bandwidth);


/** Initalize the MPI repetition selection parameter group

  @PVM_PARS
  PVM_MPI_SelectedRepetitions
  PVM_MPI_MovingAverage
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  // initalize the MPI repetition selection parameter 

  MPT_InitRepetitionSelection(1);

  @endcode
  
  @param nrRepetition maximal number of measured repetition

  @return 
  NONE
  
  @see 
  NONE
  
  @ingroup PVM_MPI
*/
DLL_INTERFACE void MPT_InitRepetitionSelection(int nrRepetition);


/** Initalize the MPI system function parameters

  @PVM_PARS
  PVM_MPI_IsSystemFunctionSelected
  PVM_MPI_RecoInfoSystemFunction
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  // initalize the MPI system function parameters 

  MPT_InitInitSystemFunction();

  @endcode
  
  @param NONE 

  @return 
  NONE
  
  @see 
  NONE
  
  @ingroup PVM_MPI
*/
DLL_INTERFACE void MPT_InitSystemFunction(void);


/** Initalize the parameters which are necessary for the MPI system function selection

  @PVM_PARS
  PVM_MPI_SFS_DriveFieldAmplitudeX
  PVM_MPI_SFS_DriveFieldAmplitudeY
  PVM_MPI_SFS_DriveFieldAmplitudeY
  PVM_MPI_SFS_DriveFieldAmplitudeZ
  PVM_MPI_SFS_SFGradient
  PVM_MPI_SFS_FocusFieldX
  PVM_MPI_SFS_FocusFieldY
  PVM_MPI_SFS_FocusFieldZ
  PVM_MPI_SFS_FieldOfViewX
  PVM_MPI_SFS_FieldOfViewY
  PVM_MPI_SFS_FieldOfViewZ
  PVM_MPI_SFS_FOVCenterX
  PVM_MPI_SFS_FOVCenterY
  PVM_MPI_SFS_FOVCenterZ
  PVM_MPI_SFS_ImageSizeX
  PVM_MPI_SFS_ImageSizeY
  PVM_MPI_SFS_ImageSizeZ
  PVM_MPI_SFS_Tracer
  PVM_MPI_SFS_TracerBatch
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  // initalize the parameters which are necessary for the MPI system function selection 

  MPT_InitInitSystemFunctionSelection();

  @endcode
  
  @param NONE 

  @return 
  NONE
  
  @see 
  NONE
  
  @ingroup PVM_MPI
*/
DLL_INTERFACE void MPT_InitSystemFunctionSelection(void);

DLL_INTERFACE void MPT_InitDisplayedSystemFunctionComponents(double bandwidth);

DLL_INTERFACE void MPT_InitBackgroundCorrection(void);

//----------------------------------------------------//
//------------------- update groups ------------------//
//----------------------------------------------------//

/** Update drive field parameter group.
 
    Depending on the dimensionality of the FOV covered by the drive field,
    different selections for the orientation are possible. This function
    initializes the selection parameter PVM_MPI_DF_Orientation to the appropriate options.
   
  @PVM_PARS
  PVM_MPI_DF_Orientation
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  MPT_UpdateDriveField();

  @endcode
  
  @return
  NONE
  
  @see
  NONE
  
  @ingroup PVM_MPT
 */
DLL_INTERFACE void MPT_UpdateDriveField(void);

/** Update receiver bandwidth.
 
    The currently selected drive field channels determine the permissible acquisition bandwidths.
    This method will shift the currently selected bandwidth to a supported value.
   
  @PVM_PARS
  PVM_MPI_bandwidth
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  Drive Field parameter group has been updated.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  MPT_UpdateBandwidth();

  @endcode
  
  @return
  NONE
  
  @see
  NONE
  
  @ingroup PVM_MPT
 */
DLL_INTERFACE void MPT_UpdateBandwidth(void);

/** Update spectral cleaning parameter
   
  @PVM_PARS
  PVM_MPI_SpectralCleaning
  PVM_MPI_WindowingFunction
  PVM_MPI_NrWindowedRepetitions
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  MPT_UpdateSpectralCleaning();

  @endcode
  
  @return
  NONE
  
  @see
  NONE
  
  @ingroup PVM_MPT
 */
DLL_INTERFACE void MPT_UpdateSpectralCleaning(void);

DLL_INTERFACE void MPT_UpdateReceivers(void);

DLL_INTERFACE void MPT_UpdateSpatialProperties(void);

/** Update the MPI algorithm parameter group

  @PVM_PARS
  PVM_MPI_ReconstructionAlgorithm
  PVM_MPI_ReconstructionAlgorithmInt
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  // update to the Kaczmarz Algorithm

  MPT_UpdateAlgorithm(KACZMARZ);

  @endcode

  @return 
  NONE
  
  @see 
  NONE
  
  @ingroup PVM_MPI
*/
DLL_INTERFACE void MPT_UpdateAlgorithm(MpiReconstructionAlgorithm);


/** Update the MPI system matrix denoising parameter group, check with the help of sizes if denoising can be used

  @PVM_PARS
  PVM_MPI_ActivateSystemMatrixDenoising
  PVM_MPI_SystemMatrixDenoisingTrafoType
  PVM_MPI_SystemMatrixDenoisingThresholdType
  PVM_MPI_SystemMatrixDenoisingWeight
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  // Update the default MPI sytem matrix denoising parameter

  MPT_InitDenoising();

  @endcode
  
  @param dim dimension
  @param sizes image sizes
  
  @return 
  NONE
  
  @see 
  NONE
  
  @ingroup PVM_MPI
*/
DLL_INTERFACE void MPT_UpdateDenoising(int dim, int* sizes);


/** Update the MPI sparse reconstruction parameter group and compute the number of used basis coefficient

  @PVM_PARS
  PVM_MPI_ActivateSparseReconstruction
  PVM_MPI_SparseRecoTrafoType
  PVM_MPI_PercentageBasisCoefficients
  PVM_MPI_NrBasisCoefficients
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  // update the default MPI sparse reconstruction parameter

  MPT_UpdateSparseReco(40);

  @endcode
  
  @param positions number of spatial positions

  @return 
  NONE
  
  @see 
  NONE
  
  @ingroup PVM_MPI
*/
DLL_INTERFACE void MPT_UpdateSparseReco(int nrPositions);


/** Update the MPI frequency component selection parameter group

  @PVM_PARS
  PVM_MPI_SNRThreshold
  PVM_MPI_ActivateMixedFrequenciesSelection
  PVM_MPI_MaximumMixingOrder
  PVM_MPI_BorderFrequencyX
  PVM_MPI_BorderFrequencyY
  PVM_MPI_BorderFrequencyZ
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  // update the MPI frequency component selection parameter in tge BrukerMPI method
  // SNRList has to be loaded

  MPT_UpdateFreqCompSelection(PVM_MPI_bandwidth,1/MPI_drive_field_cycle,PVM_MPI_NrFrequencyComponents,MPI_active_receivers,RecoDim,MPI_BasisFrequencyCoefficient)

  @endcode
  
  @param bandwidth bandwidth of the acquisition
  @param samplingFrequency the sampling frequency of the acquisition
  @param nrFreqComp number of frequency components
  @param activeReceiver array of active receiver
  @param dim dimension of experiment
  @param basicFreqCoeff frequency coefficient of the basis frequencies

  @return 
  NONE
  
  @see 
  NONE
  
  @ingroup PVM_MPI
*/
DLL_INTERFACE void MPT_UpdateFreqCompSelection(double bandwidth, 
                                 double samplingFrequency, 
                                 int nrFreqComp, 
                                 YesNo* activeReceiver, 
				 int dim, 
				 int* activeChannels);


/** Update the MPI repetition selection parameter group

  @PVM_PARS
  PVM_MPI_SelectedRepetitions
  PVM_MPI_MovingAverage
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  // initalize the MPI repetition selection parameter 

  MPT_InitRepetitionSelection(1);

  @endcode
  
  @param nrRepetition number of measured repetition
  @param nrAverages number of averages of measured data

  @return 
  NONE
  
  @see 
  NONE
  
  @ingroup PVM_MPI
*/
DLL_INTERFACE void MPT_UpdateRepetitionSelection(int nrRepetition, int nrAverages);


/** Update the parameters which are used for MPI system function selection

  @PVM_PARS
  PVM_MPI_SFS_Dim
  PVM_MPI_SFS_Bandwidth
  PVM_MPI_SFS_ReceiverX
  PVM_MPI_SFS_ReceiverY
  PVM_MPI_SFS_ReceiverZ
  PVM_MPI_SFS_DFControlMode
  PVM_MPI_SFS_DriveFieldAmplitudeX
  PVM_MPI_SFS_DriveFieldAmplitudeY
  PVM_MPI_SFS_DriveFieldAmplitudeY
  PVM_MPI_SFS_DriveFieldAmplitudeZ
  PVM_MPI_SFS_SFGradient
  PVM_MPI_SFS_FocusFieldX
  PVM_MPI_SFS_FocusFieldY
  PVM_MPI_SFS_FocusFieldZ
  PVM_MPI_SFS_FieldOfViewX
  PVM_MPI_SFS_FieldOfViewY
  PVM_MPI_SFS_FieldOfViewZ
  PVM_MPI_SFS_FOVCenterX
  PVM_MPI_SFS_FOVCenterY
  PVM_MPI_SFS_FOVCenterZ
  PVM_MPI_SFS_ImageSizeX
  PVM_MPI_SFS_ImageSizeY
  PVM_MPI_SFS_ImageSizeZ
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  // update the parameters of the MPI systefunction selection 

  MPT_UpdateSystemFunctionSelection(PTB_GetSpatDim,PVM_MPI_bandwidth,PVM_MPI_ActiveReceivers,
				    ACQ_MPI_DF_control_mode,PVM_MPI_DF_Orientation);

  @endcode
  
  @param dim dimension
  @param bandwidth bandwidth 
  @param activeReceivers active receivers
  @param DFControlMode integer which describes the drive field control mode
  @param orientation orientation of experiment

  @return 
  NONE
  
  @see 
  NONE
  
  @ingroup PVM_MPI
*/
DLL_INTERFACE void MPT_UpdateSystemFunctionSelection(int dim, double bandwidth, YesNo activeReceivers[3], int DFControlMode, int orientation);


DLL_INTERFACE void MPT_UpdateDisplayedSystemFunctionComponents(double sizeSystemMatrix, 
                                                 double availableMainMemory,
                                                 double bandwidth, 
                                                 double samplingFrequency, 
						 int nrFreqComp, 
                                                 YesNo* activeReceiver);


DLL_INTERFACE void MPT_UpdateImageMatrix();

//-------------------------------------------------------------//
//--------------------- parameter relations -------------------//
//-------------------------------------------------------------//

/* Geometry */
DLL_INTERFACE void MPT_DF_Orientation(void);
DLL_INTERFACE void MPT_drive_field_strength(void);
DLL_INTERFACE void MPT_selection_field_gradient(void);
DLL_INTERFACE void MPT_FovCenter(void);

/* Acquisition */
DLL_INTERFACE void MPT_ActiveReceivers(void);
DLL_INTERFACE void MPT_bandwidth(void);
DLL_INTERFACE void MPT_RG(void);
DLL_INTERFACE void MPT_SpectralCleaning(void);
DLL_INTERFACE void MPT_NrWindowedRepetitions(void);
DLL_INTERFACE void MPT_WindowingFunction(void);

/* Reconstruction */
DLL_INTERFACE void MPT_ActivateSystemMatrixDenoising(void);
DLL_INTERFACE void MPT_SystemMatrixDenoisingTrafoType(void);
DLL_INTERFACE void MPT_SystemMatrixDenoisingThresholdType(void);
DLL_INTERFACE void MPT_SystemMatrixDenoisingWeight(void);
DLL_INTERFACE void MPT_ActivateSparseReconstruction(void);
DLL_INTERFACE void MPT_SparseRecoTrafoType(void);
DLL_INTERFACE void MPT_PercentageBasisCoefficients(void);
DLL_INTERFACE void MPT_NrBasisCoefficients(void);
DLL_INTERFACE void MPT_SNRThreshold(void);
DLL_INTERFACE void MPT_ActivateMixedFrequenciesSelection(void);
DLL_INTERFACE void MPT_MaximumMixingOrder(void);
DLL_INTERFACE void MPT_BorderFrequencyX(void);
DLL_INTERFACE void MPT_BorderFrequencyY(void);
DLL_INTERFACE void MPT_BorderFrequencyZ(void);
DLL_INTERFACE void MPT_AverageType(void);
DLL_INTERFACE void MPT_NrAveragedRepetitions(void);
DLL_INTERFACE void MPT_SelectedRepetitions(void);
DLL_INTERFACE void MPT_EnforceReal(void);
DLL_INTERFACE void MPT_EnforcePositive(void);
DLL_INTERFACE void MPT_BackgroundFrames(void);
DLL_INTERFACE void MPT_BackgroundScan(void);
DLL_INTERFACE void MPT_BackgroundCorrectionMode(void);
DLL_INTERFACE void MPT_BackgroundCorrectionParameterVisibility(void);
DLL_INTERFACE void MPT_UpdateBackgroundCorrection(int nrepetitions, const char * pathSystemFunction);

/* System matrix selection */
DLL_INTERFACE void MPT_SFS_Import(void);
DLL_INTERFACE void MPT_SFS_Reset(void);
DLL_INTERFACE void MPT_SFS_Select(void);
DLL_INTERFACE void MPT_SFS_SetupScanAndReco(void);
DLL_INTERFACE void MPT_SFS_Export(void);
DLL_INTERFACE void MPT_SFS_DriveFieldAmplitudeX(void);
DLL_INTERFACE void MPT_SFS_DriveFieldAmplitudeY(void);
DLL_INTERFACE void MPT_SFS_DriveFieldAmplitudeZ(void);
DLL_INTERFACE void MPT_SFS_SFGradient(void);
DLL_INTERFACE void MPT_SFS_FocusFieldX(void);
DLL_INTERFACE void MPT_SFS_FocusFieldY(void);
DLL_INTERFACE void MPT_SFS_FocusFieldZ(void);
DLL_INTERFACE void MPT_SFS_FOVCenterX(void);
DLL_INTERFACE void MPT_SFS_FOVCenterY(void);
DLL_INTERFACE void MPT_SFS_FOVCenterZ(void);
DLL_INTERFACE void MPT_SFS_FieldOfViewX(void);
DLL_INTERFACE void MPT_SFS_FieldOfViewY(void);
DLL_INTERFACE void MPT_SFS_FieldOfViewZ(void);
DLL_INTERFACE void MPT_SFS_ImageSizeX(void);
DLL_INTERFACE void MPT_SFS_ImageSizeY(void);
DLL_INTERFACE void MPT_SFS_ImageSizeZ(void);
DLL_INTERFACE void MPT_SFS_Tracer(void);
DLL_INTERFACE void MPT_SFS_TracerBatch(void);


// background measurement calibration
DLL_INTERFACE void MPT_BackgroundMeasurementCalibrationIncrement(void);
DLL_INTERFACE void MPT_NrBackgroundMeasurementCalibrationScans(void);


// tracer
DLL_INTERFACE void MPT_Tracer(void);
DLL_INTERFACE void MPT_TracerBatch(void);
DLL_INTERFACE void MPT_TracerVolume(void);
DLL_INTERFACE void MPT_TracerConcentration(void);
DLL_INTERFACE void MPT_TracerDiameter(void);
DLL_INTERFACE void MPT_SaturationMagnetization(void);


// systemfunction calibration
DLL_INTERFACE void MPT_BorderDisplayedFrequenciesX(void);
DLL_INTERFACE void MPT_BorderDisplayedFrequenciesY(void);
DLL_INTERFACE void MPT_BorderDisplayedFrequenciesZ(void);



//-----------------------------------------------------------//
//--------------------- utility functions -------------------//
//-----------------------------------------------------------//

/** Check if the current parameter space contains an MPI experiment.
  
    This function returns @c Yes if MPT_InitMpiExperient has been
    called for this parameter space.
    
  @PVM_PARS
  PVM_MPI_Experiment
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  if ( No == MPT_IsMpiExperiment() )
      UT_ReportError("This is not an MPI experiment.");
  @endcode
  
  @return
  @c Yes if called within a parameter space initialized for MPI.
  
  @see
  NONE

  @ingroup PVM_MPT
*/

DLL_INTERFACE YesNo MPT_IsMpiExperiment(void);

/** Return the index of a channel base frequency in the FFT transform of a single MPI lissajous cycle.
    The index @c 0 always refers to the frequency 0 Hz (DC).

  @PVM_PARS
  PVM_MPI_channel_select
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
    - No or illegal system information about the MPI channel configuration availabe.
    - No or illegal information about the active channels available.
    - chan argument out of range

  @PVM_EXAMPLE 
  @code

  // get index of Z channel excitation frequency in FFT transform

  int zIndex = MPT_BaseFrequencyIndex(2);

  @endcode
  
  @param chan Index of frequency channel [0..2], corresponding to X,Y,Z.

  @return Index of channel frequency in FFT transform of a single MPI lissajous cycle.
  
  @see CFG_MPI_base_frequency CFG_MPI_channel_frequency_divisors
  
  @ingroup PVM_MPT
*/
DLL_INTERFACE int MPT_BaseFrequencyIndex(int chan);

/** set the dynenum PVM_MPI_SFS_MatchingSystemFunction

  @PVM_PARS
  PVM_MPI_SFS_MatchingSystemFunction
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  // set the dynenum PVM_MPI_SFS_MatchingSystemFunction

  MPT_SetMatchingSystemFunction();

  @endcode
  
  @param 

  @return 
  
  @see 
  
  @ingroup PVM_MPT
*/
DLL_INTERFACE void MPT_SetMatchingSystemFunction(void);


/** check if the selected system function fits to the experiment

  @PVM_PARS
  PVM_MPI_IsSystemFunctionSelected
  PVM_MPI_SelectedSystemFunctionContext
  PVM_MPI_RecoInfoSystemFunction
  PVM_SpatDimEnum
  PVM_MPI_selection_field_gradient
  PVM_MPI_Tracer
  PVM_MPI_TracerBatch
  PVM_MPI_bandwidth
  PVM_MPI_DF_Orientation
  PVM_Fov
  PVM_Matrix
  PVM_MPI_drive_field_strength
  PVM_MPI_ActiveReceivers
  PVM_MPI_FovCenter
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  // check if the selected system function fits to the experiment

  MPT_CheckSystemFunctionConsistence();

  @endcode
  
  @param 

  @return 
  
  @see 
  
  @ingroup PVM_MPT
*/
DLL_INTERFACE void MPT_CheckSystemFunctionConsistence(void);


/** derive the necessary MPI RECO parameters

  @PVM_PARS
  RecoAutoDerive
  RecoDim
  RecoObjectsPerRepetition
  RECO_size
  RECO_proportional_size
  RECO_fov
  RECO_ft_size
  RECO_inp_size
  RECO_offset
  RecoNumRepetitions
  
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE 
  @code

  // derive the MPI RECO parameters

  MPT_DeriveRecoParameters(PTB_GetSpatDim(),PVM_Matrix,PVM_Fov,1,PVM_MPI_NrReconstructedRepetitions,MPI_online_reco);

  @endcode
  
  @param 

  @return 
  
  @see 
  
  @ingroup PVM_MPT
*/
DLL_INTERFACE void MPT_DeriveRecoParameters( const int dim,
                               const int* sizes,
                               const double* fov,
                               const int objectsPerRepetition,
			       const int nrReconstrutedRepetitions,
                               const YesNo onlineReco
);

DLL_INTERFACE void MPT_CheckBackgroundMeasurementCalibrationIncrement(int nrSystemMatrixCalibScans);

DLL_INTERFACE void MPT_SetRegularCalibrationScanPositions(int nrCalibrationScans, 
                                            double* robotRefPositionBackground,
                                            int nrSystemMatrixCalibScans,
                                            int backgroundMeasurementCalibrationIncrement,
					    int backgroundMeasurementCalibrationScans,
					    int backgroundMeasurementCalibrationAdditionalScans,
					    int dim,
					    int* sizes,
                                            double* fov,
                                            double* fovCenter);

DLL_INTERFACE void MPT_SetRandomCalibrationScanPositions(int nrCalibrationScans, 
                                           int dim,
                                           int* sizes,
                                           double* fov,
                                           double* fovCenter);

DLL_INTERFACE YesNo MPT_CanEstimateOverScan(void);
DLL_INTERFACE double MPT_EstimatedOverScan(double selection_field_gradient);

DLL_INTERFACE void MPT_FillSimulationData( const int rec, int * data, const int numPoints, const int maxVal);

DLL_INTERFACE void MPT_SetSystemFunctionTransposition(int nrCalibrationScans, 
                                        int nrFreqComp, 
                                        int nrReceiver);

/**
 * Create the dynamic enum value range for PVM_MPI_DFOrientation.
 * 
 * Depending on the dimension, this can be {"x", "y", "z"}, {"xy", "xz", "yz"}
 * or {"xyz"}.
 * 
 * @param dim experiment dimension
 */
DLL_INTERFACE void MPT_init_drive_field_orientations(int dim);

DLL_INTERFACE void MPT_DoReferenceDrive(void);

#endif
#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
