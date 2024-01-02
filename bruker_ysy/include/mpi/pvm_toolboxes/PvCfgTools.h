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

#ifndef PVCFGMPITOOLS_H
#define PVCFGMPITOOLS_H

#ifndef CPROTO

#include "common/pvm_toolboxes/PvCfgTools.h"
#include "mpi/PvTypes/mpiTypes.h"

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

/// DLL interface libParx identifiers for Windows.
#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#  ifdef PvCfgMpiTools_INTERNAL
#    define DLL_INTERFACE __declspec(dllexport)
#  else
#    define DLL_INTERFACE __declspec(dllimport)
#  endif
#elif defined (__GNUC__) && __GNUC__ >= 4
#    define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#    define DLL_INTERFACE
#endif 


/* gen/src/prg/pvm_toolboxes/mpi/PvCfgMpiTools/mpi.c */

/** Returns the virtual base frequency from which the MPI channel 
    frequencies are derived.

    The MPI channel frequencies are derived from this base freuqency 
    by dividing by channel-specific coeefficients which are available
    by CFG_MPI_channel_frequency_divisor() or CFG_MPI_channel_frequency_divisors().


  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
    - No or illegal system information about the MPI base frequency availabe.

  @PVM_EXAMPLE
  @code

  // get MPI channel frequency for Y channel

  double yFrequency = CFG_MPI_base_frequency / CFG_MPI_channel_frequency_divisor(1);
  @endcode

  @return Virtual base frequency for MPI channels.

  @see CFG_MPI_channel_frequency_divisor CFG_MPI_channel_frequency_divisors

  @ingroup PVM_CFG_MPI
*/
DLL_INTERFACE double CFG_MPI_base_frequency(void);




/** Returns the channel divisor for computing the MPI channel frequencies.

    The MPI channel frequencies are derived from the virtual MPI base frequency by dividing by this divisor.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
    - No or illegal system information about the MPI channel divisors availabe.
    - Illegal index channel index provided.

  @PVM_EXAMPLE
  @code

  // get MPI channel frequency for Y channel

  double yFrequency = CFG_MPI_base_frequency / CFG_MPI_channel_frequency_divisor(1);
  @endcode

  @param chan MPI channel index [0...2]

  @return Channel divisor.

  @see CFG_MPI_base_frequency CFG_MPI_get_num_drive_field_channels CFG_MPI_channel_frequency_divisors

  @ingroup PVM_CFG_MPI
*/
DLL_INTERFACE int CFG_MPI_channel_frequency_divisor(int chan);





/** Returns the channel divisors for computing the MPI channel frequencies.

    The MPI channel frequencies are derived from the virtual MPI base frequency by dividing by these divisors.

    @a div must point to an integer array with at least 3 entries (more channels in theory possible, but currently
    not supported).

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
    - No or illegal system information about the MPI channel divisors availabe.
    - A null pointer is passed.

  @PVM_EXAMPLE
  @code

  // get MPI channel frequency divisors

  int div[3];
  if ( CFG_MPI_get_num_drive_field_channels()> 3 )
      UT_ReportError("More than 3 DF channels are not supported!");
  CFG_MPI_channel_frequency_divisors(div);
  @endcode

  @param div pointer to an array with at least three integer entries.

  @return Channel divisors.

  @see CFG_MPI_get_num_drive_field_channels CFG_MPI_base_frequency CFG_MPI_channel_frequency_divisors

  @ingroup PVM_CFG_MPI
*/
DLL_INTERFACE void CFG_MPI_channel_frequency_divisors(int * div);





/** Returns the number of installed drive field channels.

  @a div must point to an integer array with at least three entries.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
    - No or illegal system information about the MPI channels available

  @PVM_EXAMPLE
  @code

  // get MPI channel frequency divisors

  int channels = CFG_MPI_get_num_drive_field_channels();
  @endcode

  @return Number of drive field channels.

  @see CFG_MPI_channel_frequency_divisors

  @ingroup PVM_CFG_MPI
*/
DLL_INTERFACE int CFG_MPI_get_num_drive_field_channels(void);





/** Get maximum allowed gradient strength for MPI selection field.

  @PVM_PARS
  NONE

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
  // set maximum selection field strength
  ParxRelsParSetDoubleMaximum("Selection_Field",CFG_MPI_get_max_selection_field_gradient);
  @endcode

  @return Maximum gradient strength in T/m

  @ingroup PVM_CFG_MPI
*/
DLL_INTERFACE double CFG_MPI_get_max_selection_field_gradient(void);
DLL_INTERFACE double CFG_MPI_get_selection_field_calibration(void);
DLL_INTERFACE double CFG_MPI_get_max_focus_field(int);
DLL_INTERFACE double CFG_MPI_get_focus_field_calibration(int);





/** Get maximum supported MPI acquisition bandwidth in MHz

  @PVM_PARS
  NONE

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
  // set maximum selection field strength
  ParxRelsParSetDoubleMaximum("BandWidth",CFG_MPI_get_max_bandwidth);
  @endcode

  @return Maximum acquisition bandwidth in MHz

  @ingroup PVM_CFG_MPI
*/
DLL_INTERFACE double CFG_MPI_get_max_bandwidth(void);





/** Get number of MPI receive channels.
  
    MPI systems have one receiver per channel for monitoring and acquisition control.
    Any additional receivers are used for signal detection, and their number is reported by this function. 

  @PVM_PARS
  NONE

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
  int numReceivers = CFG_MPI_get_num_receivers();
  @endcode

  @return Number of receive channels.

  @ingroup PVM_CFG_MPI
*/
DLL_INTERFACE int CFG_MPI_get_num_receivers(void);





/** Returns the maximum permissible drive field amplitude for a given channel.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
    - No or illegal system information about the MPI drive field amplitudes.

  @PVM_EXAMPLE
  @code

  // get MPI channel frequency divisors

  double max_df[3];
  if ( CFG_MPI_get_num_drive_field_channels()> 3 )
      UT_ReportError("More than 3 DF channels are not supported!");
  for (int i=0; i<CFG_MPI_get_num_drive_field_channels; ++i)
      max_df[i] = CFG_MPI_get_max_drive_field_amplitude(i);
  @endcode

  @param direction MPI DF channel index (typically 0=x, 1=y, 2=z)
  @return Number of drive field channels.

  @see CFG_MPI_channel_frequency_divisors

  @ingroup PVM_CFG_MPI
*/
DLL_INTERFACE double CFG_MPI_get_max_drive_field_amplitude(int direction);
DLL_INTERFACE double CFG_MPI_get_max_fov(int direction);
DLL_INTERFACE int CFG_MPI_get_num_static_field_channels(void);
DLL_INTERFACE int CFG_MPI_get_static_field_channel_index(int);
DLL_INTERFACE double CFG_MPI_get_bore_diameter(void);
DLL_INTERFACE double CFG_MPI_get_bore_length(void);
DLL_INTERFACE MPI_SAMPLE_GEOMETRY CFG_MPI_get_sample_geometry(void);
DLL_INTERFACE double CFG_MPI_get_static_field_settle_time(int channel, double startVal, double endVal);
DLL_INTERFACE double CFG_MPI_get_minimum_precalc_time(void);
#endif
#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/

