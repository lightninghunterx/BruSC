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

#ifndef PVACQMPITOOLS_H
#define PVACQMPITOOLS_H

#ifndef CPROTO
#include "mr/PvTypes/mrTypes.h"
#include "mpi/PvTypes/mpiTypes.h"

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

/// DLL interface libParx identifiers for Windows.
#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#  ifdef PvAcqTools_INTERNAL
#    define DLL_INTERFACE __declspec(dllexport)
#  else
#    define DLL_INTERFACE __declspec(dllimport)
#  endif
#elif defined (__GNUC__) && __GNUC__ >= 4
#    define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#    define DLL_INTERFACE
#endif 



/**  Initialize acquisition parameters for MPI acquisition.
 
  This function sets the acquisition mode to MPI, initializes
  ACQ_MPI_calibration_scan to its default (no calibration experiment),
  and disables online reconstruction by default for both GOP and GSP
  pipelines.
  
  @PVM_PARS
  - @c ACQ_experiment_mode
  - @c ACQ_MPI_calibration_scan
  - @c ACQ_ScanOnlineReco
  - @c ACQ_SetupOnlineReco

  @PVM_TBCALL
  NONE
  
  @PVM_RELCALL
  - @c ACQ_experiment_mode

  @PVM_PRECOND
  - A valid MPI system configuration must be present.

  @PVM_ERROR
  - System is not capable of performing MPI experiments.

  @param mode Reserved parameter for future extensions.
  
  @ingroup PVM_ATB_MPI
 */ 
DLL_INTERFACE void ATB_MPI_InitExperiment(int mode=0);

/**  Disables/enables drive field cross talk compensation.

  The drive field channels in an MPI system may exhibit a certain
  crosstalk due to imperfect orthogonality, parallel wires, etc.
  
  As the resonance frequencies are very close to each other, this
  cross-talk may induce significant current amplitudes in neighbouring
  coils, causing distorted excitation geometries.
  
  ParaVision can detect these crosstalk and incorporate compensation
  signals in the signal generator output that cancel the crosstalk signals.
  This crosstalk compensation is enabled by default but may cost a 
  large amount of transmitter power, which can possibly even limit the 
  total available drive field amplitude. For this reason it is possible
  to deactivate the crosstalk compensation.
  
  With deactivated crosstalk compensation, ParaVision will still maintain
  stable excitation amplitudes on each channel, but crosstalk signals
  (which may drift in time) are no longer analyzed and compensated.
  
  @PVM_PARS
  - @c ACQ_MPI_DF_control_mode

  @PVM_TBCALL
  NONE
  
  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @param   disable If set to @c Yes, ParaVision will not perform a crosstalk
                   compensation, otherwise a strictly orthogonal trajectory
                   will be maintained.
                   
  @ingroup PVM_ATB_MPI
 */
DLL_INTERFACE void ATB_MPI_DisableCrossTalkCompensation(YesNo disable);

/** Define static selection field strength for the entire experiment.

  With the default MPI pulse programs, the selection field strength
  (in T/m) will be activated before the drive field starts and will 
  remain constant until the end of acquisition.
  
  While the actual control in the pulse program is based on
  ACQ_MPI_frame_list[0], the programmed value is also stored in
  ACQ_MPI_selection_field_gradient for reference and export to the viewing parameters.
  
  This call is equivalent to ATB_MPI_SetStaticSelectionAndFocusField(selectionField,0,0,0) .
  
  @PVM_PARS
  - @c ACQ_MPI_selection_field_gradient
  - @c ACQ_MPI_static_field_channels
  - @c ACQ_MPI_field_steps
  - @c ACQ_MPI_frame_list
  - @c ACQ_MPI_frame_step_time
  

  @PVM_TBCALL
  - @c CFG_MPI_get_num_static_field_channels
  - @c CFG_MPI_get_static_field_channel_index
  - @c CFG_MPI_get_max_selection_field_gradient
  - @c CFG_MPI_get_selection_field_calibration
  
  @PVM_RELCALL
  - @c ACQ_MPI_field_steps

  @PVM_PRECOND
  - A valid MPI system calibration must be present including all calibration files.
  
  @PVM_ERROR
  - Requested selection field strength outside permissible range.
  - Selection field channel not configured.

  @param selectionField Selection field strenght in T/m.
  @ingroup PVM_ATB_MPI
 */
DLL_INTERFACE void ATB_MPI_SetStaticSelectionField(double selectionField);

/** Define static selection and focus field strengths for the entire experiment.

  With the default MPI pulse programs, the "static" field channels (selection
  and focus fields) will be ramped to the values passed to this function and will 
  remain constant until the end of acquisition.
  
  While the actual control in the pulse program is based on
  ACQ_MPI_frame_list[0], the programmed selection field value is also stored in
  ACQ_MPI_selection_field_gradient for reference and export to the viewing parameters.

  @PVM_PARS
  - @c ACQ_MPI_selection_field_gradient
  - @c ACQ_MPI_static_field_channels
  - @c ACQ_MPI_field_steps
  - @c ACQ_MPI_frame_list
  - @c ACQ_MPI_frame_step_time

  @PVM_TBCALL
  - @c CFG_MPI_get_num_static_field_channels
  - @c CFG_MPI_get_static_field_channel_index
  - @c CFG_MPI_get_max_selection_field_gradient
  - @c CFG_MPI_get_selection_field_calibration
  - @c CFG_MPI_get_max_focus_field
  - @c CFG_MPI_get_focus_field_calibration
  
  @PVM_RELCALL
  - @c ACQ_MPI_field_steps

  @PVM_PRECOND
  - A valid MPI system calibration must be present including all calibration files.

  @PVM_ERROR
  - Any field specification outside permissible range.
  - Focus field support not present on current scanner.

  @param selectionField Selection field strength in T/m
  @param focusFieldX    Focus field strength along X direction in mT.
  @param focusFieldY    Focus field strength along Y direction in mT.
  @param focusFieldZ    Focus field strength along Z direction in mT.
    
  @ingroup PVM_ATB_MPI
 */
 DLL_INTERFACE void ATB_MPI_SetStaticSelectionAndFocusField(double selectionField,
                                                           double focusFieldX,
                                                           double focusFieldY,
                                                           double focusFieldZ);

//-----------------------------------------------//
//                  WARNING                      //
//                                               //
// experimental code                             //
// do NOT use for production code                //
// for safe usage, the current limits have to be //
// derived from CONFIG-parameter                 //             
//-----------------------------------------------//     

DLL_INTERFACE void ATB_MPI_SetStaticSelectionAndFocusFieldCurrent(double currentSF, 
								  double currentFFX, 
								  double currentFFY, 
								  double currentFFZ);

//-----------------------------------------------//
//                  WARNING                      //
//                                               //
// experimental code                             //
// do NOT use for production code                //
// for safe usage, the current limits have to be //
// derived from CONFIG-parameter                 //             
//-----------------------------------------------//     

DLL_INTERFACE void ATB_MPI_SetStaticSelectionAndFocusFieldCurrentHybrid(void);

//-----------------------------------------------//
//                  WARNING                      //
//                                               //
// experimental code                             //
// do NOT use for production code                //
// for safe usage, the current limits have to be //
// derived from CONFIG-parameter                 //             
//-----------------------------------------------//     

DLL_INTERFACE int ATB_MPI_AddSelectionAndFocusFieldCurrentStep(double currentSF, 
							       double currentFFX, 
							       double currentFFY, 
                                                               double currentFFZ,
                                                               double cycleDuration);

/** Activate drive field channels and define their amplitudes.

  This function is used to define which DF channels should be operated and to set their
  amplitudes.
  
  @PVM_PARS
  - @c ACQ_MPI_drive_field_strength
  - @c ACQ_MPI_DF_field_matrix
  - @c ACQ_MPI_channel_select
  - @c ACQ_MPI_div
  - @c ACQ_MPI_drive_field_cycle
  - @c ACQ_MPI_
  
  @PVM_TBCALL
  - @c CFG_MPI_get_num_drive_field_channels
  - @c CFG_MPI_channel_frequency_divisor
  - @c CFG_MPI_get_max_drive_field_amplitude
  
  @PVM_RELCALL
  - @c ACQ_MPI_channel_select
  - @c ACQ_MPI_drive_field_strength
  - @c ACQ_MPI_DF_field_matrix

  @PVM_PRECOND
  - A valid MPI system calibration must be present including all calibration files.

  @PVM_ERROR
  - An invalid dimensionality is specified.
  - A null pointer is passed for any parameter.
  - An invalid channel is specified in @a activeChannels
  - An amplitude outside range is specified in @a amplitudes.

  @param df_dimension    The dimensionality of the excitation (number of channels to be operated)
  @param activeChannels  Pointer to @a df_dimension integer values listing the indices of the active channels
  @param amplitudes      Pointer to @a df_dimension double values listing the amplitudes to be realized on the active channels.
  
  @ingroup PVM_ATB_MPI
 */
DLL_INTERFACE void ATB_MPI_SetDriveFieldParameters(int df_dimension,
                                                   const int * activeChannels,
                                                   const double * amplitudes );

/**  Set acquisition parameters for MPI acquisition.

  This function is used to set the basic acquisition parameters such as detection bandwidth,
  number of averages and number of repetitions.
  
  As an additional parameter the cycle length is required. This can typically be taken from
  the parameter PVM_MPI_DriveFieldCycle.
  
  @PVM_PARS
  - @c ACQ_jobs_size
  - @c ACQ_jobs
  - @c NA
  - @c NR
  - @c SW_h

  @PVM_TBCALL

  @PVM_RELCALL
  - @c ACQ_jobs_size
  - @c ACQ_jobs

  @PVM_PRECOND
  - A valid MPI system calibration must be present including all calibration files.

  @PVM_ERROR
  - None

  @param cycleLength Duration of a single lissajous cycle in ms
  @param bandwidth   Acquisition bandwidth in MHz
  @param averages    Number of averages
  @param repetiions  Number of repetitions
  
  @ingroup PVM_ATB_MPI
 */ 
DLL_INTERFACE void ATB_MPI_SetAcquisitionParameters(double cycleLength, double bandwidth, int averages, int repetitions);

/**  Set the receivers to be used for MPI acquisition and select gain setting.

  This function is used to define which receivers are to be used for acquisition and at
  which gain setting they are supposed to operate.
  
  @PVM_PARS
  - @c ACQ_ReceiverSelect
  - @c ACQ_MPI_RG

  @PVM_TBCALL
  - @c CFG_MPI_get_num_receivers
  - @c CFG_MPI_get_num_drive_field_channels
  
  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - A valid MPI system calibration must be present including all calibration files.

  @PVM_ERROR
  - An illegal value for the number of receivers is specificed.
  - A null pointer is passed for active receivers.
  - 
  @param numRec          Number of entries in activeReceivers
  @param activeReceivers This parameter must point to a YesNo array with @a numRec entries. 
                         A @c Yes entry means that the indicated receiver is activated.
  @param gain            The gain level for the amplification stage in the MPI receiver. This should normally
                         be left at the lowest allowed value.
                         
  @ingroup PVM_ATB_MPI
 */ 
DLL_INTERFACE void ATB_MPI_SetReceiverParameters(int numRec,const YesNo * activeReceivers, FtmsGainType gain);

/**  Indicate whether spectral and/or time data displays should be active during acquisition.

  This function is used to enable/disable the display of per channel spectra and/or time data.
  
  @PVM_PARS
  - @c ACQ_MPI_display_channels
  - @c ACQ_MPI_display_frequency_data
  - @c ACQ_MPI_display_time_data
  
  @PVM_TBCALL
  - @c CFG_MPI_get_num_receivers
  
  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - A valid MPI system calibration must be present including all calibration files.

  @PVM_ERROR
  

  @param showSpectra   Set to @c Yes if spectra should be shown during acquisition
  @param showTimeData  Set to @c Yes if time data should be shown during acquisition
  @ingroup PVM_ATB_MPI
 */ 
DLL_INTERFACE void ATB_MPI_ActivateDisplays( YesNo showSpectra, YesNo showTimeData );



/** Add a new focus field step to field sequence.

  Focus field vectors added by this function are added to the list that will be executed out by a pulse
  program that supports field switching. The field vectors are executed in the order in which they
  were added by this function. The list repeats automatically, including the intial set of
  values defined by ATB_MPI_SetStaticSelectionAndFocusField().
  
  The selection field is not changed with respect to the previous state.
  
  @PVM_PARS
  - @c ACQ_MPI_selection_field_gradient
  - @c ACQ_MPI_static_field_channels
  - @c ACQ_MPI_field_steps
  - @c ACQ_MPI_frame_list
  - @c ACQ_MPI_frame_step_time

  @PVM_TBCALL
  - @c CFG_MPI_get_num_static_field_channels
  - @c CFG_MPI_get_static_field_channel_index
  - @c CFG_MPI_get_max_selection_field_gradient
  - @c CFG_MPI_get_selection_field_calibration
  - @c CFG_MPI_get_max_focus_field
  - @c CFG_MPI_get_focus_field_calibration
  
  @PVM_RELCALL
  - @c ACQ_MPI_field_steps

  @PVM_PRECOND
  - A valid MPI system calibration must be present including all calibration files.
  - An initial set of field values must have been set using ATB_MPI_SetStaticSelectionField() (no focus fields) or
    ATB_MPI_SetStaticSelectionAndFocusField().
  - The drive field parameters must have been set using ATB_MPI_SetDriveFieldParameters() to define
    the drive field cycle duration (parameter ACQ_MPI_drive_field_cycle).

  @PVM_ERROR
  - Any field specification outside permissible range.
  - Focus field support not present on current scanner.
  - Initial values not set.

  @param focusFieldX    Focus field strength along X direction in mT.
  @param focusFieldY    Focus field strength along Y direction in mT.
  @param focusFieldZ    Focus field strength along Z direction in mT.

  @return Number of drive field cycles required until the new field configuration has stabilized.
  
  @note The field sequence has to be rebuilt when the drive field cycle length changes (change of dimensionality or selection of active channels via
        ATB_MPI_SetDriveFieldParameters() ).
        To rebuilt the sequence, restart with ATB_MPI_SetStaticSelectionField() or ATB_MPI_SetStaticSelectionAndFocusField() and use the Add functions
        to define the list.
        
  @ingroup PVM_ATB_MPI
 */ 
DLL_INTERFACE int ATB_MPI_AddFocusFieldStep(double focusFieldX,
                                            double focusFieldY,
                                            double focusFieldZ);

/** Add a new selection field step to field sequence.

  A selection field step added by this function is added to the list that will be executed out by a pulse
  program that supports field switching. The field vectors are executed in the order in which they
  were added by this function. The list repeats automatically, including the intial set of
  values defined by ATB_MPI_SetStaticSelectionAndFocusField().
  
  Focus field values present in the previous field steps will be copied, so during sequence execution no focus
  field change will occur.
  
  @PVM_PARS
  - @c ACQ_MPI_selection_field_gradient
  - @c ACQ_MPI_static_field_channels
  - @c ACQ_MPI_field_steps
  - @c ACQ_MPI_frame_list
  - @c ACQ_MPI_frame_step_time

  @PVM_TBCALL
  - @c CFG_MPI_get_num_static_field_channels
  - @c CFG_MPI_get_static_field_channel_index
  - @c CFG_MPI_get_max_selection_field_gradient
  - @c CFG_MPI_get_selection_field_calibration
  - @c CFG_MPI_get_max_focus_field
  - @c CFG_MPI_get_focus_field_calibration
  
  @PVM_RELCALL
  - @c ACQ_MPI_field_steps

  @PVM_PRECOND
  - A valid MPI system calibration must be present including all calibration files.
  - An initial set of field values must have been set using ATB_MPI_SetStaticSelectionField() (no focus fields) or
    ATB_MPI_SetStaticSelectionAndFocusField().
  - The drive field parameters must have been set using ATB_MPI_SetDriveFieldParameters() to define
    the drive field cycle duration (parameter ACQ_MPI_drive_field_cycle).

  @PVM_ERROR
  - Any field specification outside permissible range.
  - Initial values not set.

  @param selectionField Selection field strength in T/m.

  @return Number of drive field cycles required until the new field configuration has stabilized.
  
  @note The field sequence has to be rebuilt when the drive field cycle length changes (change of dimensionality or selection of active channels via
        ATB_MPI_SetDriveFieldParameters() ).
        To rebuilt the sequence, restart with ATB_MPI_SetStaticSelectionField() or ATB_MPI_SetStaticSelectionAndFocusField() and use the Add functions
        to define the list.
          @ingroup PVM_ATB_MPI
 */ 
 DLL_INTERFACE int ATB_MPI_AddSelectionFieldStep(double selectionField);

/** Add a new field step to field sequence.

  Focus field vectors added by this function are added to the list that will be executed out by a pulse
  program that supports field switching. The field vectors are executed in the order in which they
  were added by this function. The list repeats automatically, including the intial set of
  values defined by ATB_MPI_SetStaticSelectionAndFocusField().
  
  @PVM_PARS
  - @c ACQ_MPI_selection_field_gradient
  - @c ACQ_MPI_static_field_channels
  - @c ACQ_MPI_field_steps
  - @c ACQ_MPI_frame_list
  - @c ACQ_MPI_frame_step_time

  @PVM_TBCALL
  - @c CFG_MPI_get_num_static_field_channels
  - @c CFG_MPI_get_static_field_channel_index
  - @c CFG_MPI_get_max_selection_field_gradient
  - @c CFG_MPI_get_selection_field_calibration
  - @c CFG_MPI_get_max_focus_field
  - @c CFG_MPI_get_focus_field_calibration
  
  @PVM_RELCALL
  - @c ACQ_MPI_field_steps

  @PVM_PRECOND
  - A valid MPI system calibration must be present including all calibration files.
  - An initial set of field values must have been set using ATB_MPI_SetStaticSelectionField() (no focus fields) or
    ATB_MPI_SetStaticSelectionAndFocusField().
  - The drive field parameters must have been set using ATB_MPI_SetDriveFieldParameters() to define
    the drive field cycle duration (parameter ACQ_MPI_drive_field_cycle).

  @PVM_ERROR
  - Any field specification outside permissible range.
  - Focus field support not present on current scanner.
  - Initial values not set.

  @param selectionField Selection field strength in T/m
  @param focusFieldX    Focus field strength along X direction in mT.
  @param focusFieldY    Focus field strength along Y direction in mT.
  @param focusFieldZ    Focus field strength along Z direction in mT.

  @return Number of drive field cycles required until the new field configuration has stabilized.
  
  @note The field sequence has to be rebuilt when the drive field cycle length changes (change of dimensionality or selection of active channels via
        ATB_MPI_SetDriveFieldParameters() ).
        To rebuilt the sequence, restart with ATB_MPI_SetStaticSelectionField() or ATB_MPI_SetStaticSelectionAndFocusField() and use the Add functions
        to define the list.
        
  @ingroup PVM_ATB_MPI
 */ 
DLL_INTERFACE int ATB_MPI_AddSelectionAndFocusFieldStep(double selectionField,
                                                        double focusFieldX,
                                                        double focusFieldY,
                                                        double focusFieldZ);

DLL_INTERFACE void ATB_MPI_SetSelectionAndFocusFieldList(int numSteps, double * values);

DLL_INTERFACE void ATB_MPI_SetFocusFieldList(int numSteps, double * values);

#endif
#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
