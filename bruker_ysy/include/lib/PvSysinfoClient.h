/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001-2015
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVSYSINFOCLIENT_H
#define PVSYSINFOCLIENT_H



#include <stdint.h>
#include "bruktyp.h"
#include "PvSysinfoClient_state.h"
#include "mr/acqtyp.h"
#include "mr/PvTypes/mrTypes.h"
#include "PvAdj/adjManagerTypes.h"
#include "PvMrAdj/MrAdjManagerTypes.h"
#include "mr/ovl_toolbox/DLLMacro.h"

#ifndef CPROTO
#include <vector>
#include <string>
#include <set>

namespace Parx { namespace Common { class ParamInterface; } }
namespace PvRouting { class NucleusRange; class CoilReference; }

class SysInfoClient;


enum class PhaseCorrectionPolicy
{
    Coil,
    RfLine,
    Both
};


enum class SysInfo_TxRxState
{
    Logical,
    Physical
};

/**
  * @file
  * The functions declared in this file provide run-time access to scan-specific
  * system information.
  *
  * Some functions are provided in three variants, one suitable to be called from
  * within relation (including method code), the other to be used elsewhere, and
  * others can be called in overlays and elsewhere.
  * The difference is the parameter space id or Parx::Common::ParamInterface
  * which is required as first argument for the non-relation variants.
  *
  * The function variants to be used in methods are implemented in the
  * ovl_toolbox, whereas the function variants for client code are implemented
  * in libPipeline, the functions for the Parx::Comon::ParamInterface variant
  * are implemented in libPvSysInfo.
  *
  * Most functions expect an integer @a channel argument, which can be used both
  * to query logical and physical channels. Positive values for the channel
  * argument refer to logical channels, whereas negative values refer to physical
  * channels (SGU numbers).
  *
  * The functions for querying RF channel properties return @c 0 in case of success.
  * Any non-zero return value means that the requested information cannot be provided,
  * call SysInfo_getErrorMessage() for a textual error message.
  */

/** @defgroup SysInfo_overlay Sysinfo functions for relation code. */
/** @defgroup SysInfo_client  Sysinfo functions for client code.   */
/** @defgroup SysInfo_general Sysinfo functions using general Parx::Comon::ParamInterface. */


/** @ingroup SysInfo_general
    Get a string representing the hardware context.
    @param psInterface The parameter space interface.
    @param client The system information provider, if \c 0 a temporary
                  provider is used.
    @param hwScope The hardware scope for which the context is asked.
    @param chanSpec - logical channel number [1...N] for logical state.
                    - bitmask of used sguIdxs for physical state.
                    - -1 if not applicable
    @param context A buffer for the output context for the given scope
       and channel. It must be allocated by the caller with length
       \c PVSYSINFOCHANSTATE_LEN.
    @see SysInfo_TxState
    @see SysInfo_RxState
    @see SysInfo_GradState
    @see SysInfo_ShimState
    @see SysInfo_RfState
 */
DLL_INTERFACE void SysInfo_GetHwContext(
    const Parx::Common::ParamInterface & psInterface,
    const SysInfoClient * client,
    HwScopeEnum hwScope, int chanSpec, char * context);


/** @ingroup SysInfo_overlay
    Get a string representing the hardware context.
    @param hwScope The hardware scope for which the context is asked.
    @param chanSpec - logical channel number [1...N] for logical state.
                    - bitmask of used sguIdxs for physical state.
                    - -1 if not applicable
    @param context A buffer for the output context for the given scope
       and channel. It must be allocated by the caller with length
       \c PVSYSINFOCHANSTATE_LEN.
    @see SysInfo_TxState
    @see SysInfo_RxState
    @see SysInfo_GradState
    @see SysInfo_ShimState
    @see SysInfo_RfState
 */
DLL_INTERFACE void SysInfo_GetHwContext(const HwScopeEnum hwScope, 
                                        const int chanSpec, 
                                        char* context);

/**
  * @ingroup SysInfo_general
  * Delivers a code representing the RF-state of system. This code is constructed
  * from identifiers all elements of of the RF-system.
  *
  * @param psInterface The parameter space interface.
  * @param client The system information provider, if not \c 0 it is used else
  *               a temporary provider is instantiated.
  * @param key   Must point to a buffer of size PVSYSINFOCHANSTATE_LEN, into which
  *              the state description is written
  * @return      @c 0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_RfState(
    const Parx::Common::ParamInterface & psInterface,
    const SysInfoClient * client,
    char * key);

/**
  * @ingroup SysInfo_overlay
  * Delivers a slash separated list of nuclei for which the coils routed on
  * channel @a chan are adapted. The bandwidths of the amplifier and preamplifiers
  * are also taken into account.
  *
  * @param chanNum   logical channel number [1...N] for which the nuclei list should be returned
  * @param nuclei    Must point to a std::string, into which the nuclei list is copied.
  * @return       @c  0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_ChannelNuclei(const int chanNum, std::string* nuclei);
/**
  * @ingroup SysInfo_overlay
  * Delivers the of nuclei for which the coils routed on
  * channel @a chan are adapted. The bandwidths of the amplifier and preamplifiers
  * are also taken into account.
  *
  * @param chanNum   logical channel number [1...N] for which the nuclei should be returned
  * @return  the nuclei.
  */
DLL_INTERFACE const PvRouting::NucleusRange& SysInfo_ChannelNuclei(const int chanNum);


/* --- TX chain --- */

/**
  * Translate error codes (return values from SysInfo functions) into
  * a textual error message.
  * @param code The non-zero error code returned by the following functions.
  * @return     A brief string, e.g. for putting into a log file.
  */
DLL_INTERFACE const char* SysInfo_getErrorMessage(const int code);
DLL_INTERFACE std::string SysInfo_getErrorMessageStr(const int code);

/**
  * @ingroup SysInfo_overlay
  * Provide information about the number of installed signal generation units.
  * @param  ntxchan The number of installed signal generation units is returned in this
  *                 variable.
  * @return         @c 0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_ConfNSgus(size_t* nSgus);

/**
  * @ingroup SysInfo_overlay
  * Provide information about the number of transmit-capable channels.
  * @return     The number of transmit-capable channels.
  */
DLL_INTERFACE size_t SysInfo_TxNChannels();

/** @ingroup SysInfo_general
  * Provides a key representing the state of the transmit chain routed on the
  * channel @a chan. This key is constructed from identifiers of all elements
  * of the chain (fcu, sgu, amp, tx-coil), as well as the date of the cortab file.
  * On the method level the return value may be used eg. to describe hardware
  * scope of adjustment results.
  *
  * @param psInterface The parameter space interface.
  * @param client System information client, if \c 0 a client is instantiated.
  * @param chanSpec  logical channel number [1...N] for logical state
  *                  bitmask of used sguIdxs for physical state
  * @param state     logical or physical state
  * @param key       Must point to a buffer of size PVSYSINFOCHANSTATE_LEN, into which
  *                  the state description is written
  * @return     @c 0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_TxState(
    const Parx::Common::ParamInterface & psInterface,
    const SysInfoClient * client,
    const int chanSpec, const SysInfo_TxRxState& state, char * key);

/**
  * @ingroup SysInfo_overlay
  * Provides a key representing the state of the transmit chain routed on the
  * channel @a chan. This key is constructed from identifiers of all elements
  * of the chain (fcu, sgu, amp, tx-coil), as well as the date of the cortab file.
  * On the method level the return value may be used eg. to describe hardware
  * scope of adjustment results.
  *
  * @param chanNum  logical channel number [1...N]
  * @param state    logical or physical state of logical channel chanNum
  * @param key   Must point to a buffer of size PVSYSINFOCHANSTATE_LEN, into which
  *              the state description is written
  * @return     @c 0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_TxState(const int chanNum,
                                  const SysInfo_TxRxState& state,
                                  char * key);

/**
  * @ingroup SysInfo_overlay
  * Returns the number of transmit coil elements as currently routed to channel
  * @a chan. Quadrature coil element pairs with a splitter are reported as a single
  * coil.
  * @param chan     logical channel number [1...N] for which the transmit coil elements should be counted.
  * @param ntxcoils The coil element count is placed in this variable
  * @return @c 0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_TxNCoilElements(const int chanNum, size_t* ntxcoils);

/**
 *  @ingroup SysInfo_overlay
 *  Returns a table indicating which transmit coil elements are currently enabled on channel
 *  @a chanNum.
  * @param chanNum        logical channel number [1...N] for which the transmit coil element selection
 *                        table should be returned.
 *  @param activeElements The Enabled/Disabled state for all routed elements is placed in this
 *                        variable.
 *  @return               @c 0 if everything is OK.
 */
DLL_INTERFACE int SysInfo_TxActiveCoilElements(const int chanNum,
                                               std::vector<bool>* activeElements);

/**
 *  @ingroup SysInfo_overlay
 *  Returns the index of the signal generation unit connected to a coil element on channel
 *  @a chanNum.
 *  @param chanNum      logical channel number [1...N]
 *  @param coilElemIdx  coil element index [0...N-1]
 *  @param txSguIdx     index [0...N-1] of the signal generation unit connected
 *                      to the requested element.
 *  @return             @c 0 if everything is OK.
 */
DLL_INTERFACE int SysInfo_TxSguIdx(const int chanNum,
                                   const int coilElemIdx,
                                   int* txSguIdx);

/**
 *  @ingroup SysInfo_overlay
 *  Returns the indeces of signal generation units routed on channel @a chanNum as bitmask.
 *  @param chanNum        logical channel number [1...N]
 *  @param txSgusBitmask  bitmask of used txSgu indeces
 *  @return               @c 0 if everything is OK.
 */
DLL_INTERFACE int SysInfo_TxSgusBitmask(const int chanNum);

/**
 *  @ingroup SysInfo_overlay
 *  Returns the numbers of signal generation units routed on channel @a chanNum.
 *  @param chanNum        logical channel number [1...N]
 *  @param txSguNums      numbers [1...N] of the signal generation units.
 *  @return               @c 0 if everything is OK.
 */
DLL_INTERFACE int SysInfo_TxSguNums(const int chanNum,
                                    std::set<int>* txSguNums);

/**
 *  @ingroup SysInfo_overlay
  * @ingroup SysInfo_overlay
  * Delivers a power level (in Watt) which is recommended for starting a
  * reference gain adjustment on channel @a chan.
  *
  * A power level of 0 Watt indicates that no information is available.
  *
  * @param chanNum  logical channel number [1...N]
  * @param power    The power value is returned in this variable
  * @return         @c 0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_TxRefPower(const int chanNum, double* power);

/**
  * @ingroup SysInfo_overlay
  * Delivers the maximum peak power (in Watt) of the transmitter routed
  * on channel @a chan at the input of the coil that is obtained without
  * attenuation (TPQQ.power = -6dB).
  * If @a nucleus is specified, the cortab files are used to determine a
  * calibrated power level, otherwise the nominal power specification
  * of the transmitter is used.
  * A power level of 0 Watt is returned if the transmitter's peak power
  * is unknown.

  * @param chanNum  logical channel number [1...N]
  * @param nucleus the nucleus to be used on this channel (may be null pointer)
  * @return the power value
  */
DLL_INTERFACE double SysInfo_TxMaxPower(const int chanNum, const char* nucleus = nullptr);

/**
  * @ingroup SysInfo_overlay
  * Delivers the maximum peak power (in Watt) that can be transmitted on
  * channel chan. Takes into account the currently routed amplifier and  RF coil.
  * (min of max transmitter power and max allowed power of the transmit coil).
  *
  * A power limit of 0 Watt is returned if either the amplifiers peak power
  * is unknown or the routed coil does not specify power limits.
  *
  * @param chanNum  logical channel number [1...N]
  * @return the power value
  */
DLL_INTERFACE double SysInfo_TxMaxAllowedPeakPower(const int chanNum);

/**
  * @ingroup SysInfo_overlay
  * Delivers maximum CW power (in Watt) that can be transmitted on channel chan.
  * Takes into account the currently routed amplifier and  RF coil. (min CW power
  * of amplifier and coil).
  *
  * A power limit of 0 Watt is returned if either the amplifiers cw power
  * is unknown or the routed coil does not specify power limits.
  *
  * @param chanNum  logical channel number [1...N]
  * @param power The power value is returned in this variable
  * @return      @c 0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_TxMaxAllowedCWPower(const int chanNum, double* power);

/**
  * @ingroup SysInfo_overlay
  * Delivers maximum RF peak power in watt for the RF coil routed on
  * channel chan.
  *
  * A power of 0W is returned if the routed coil does not provide power
  * limits
  *
  * @param chanNum  logical channel number [1...N]
  * @param power The max power is returned in this variable
  * @return     @c 0 if everything is OK.
  *
  */
DLL_INTERFACE int SysInfo_TxCoilMaxPower(const int chanNum, double* power);

/**
  * @ingroup SysInfo_overlay
  * Delivers maximum RF (block)pulse duration in ms at maximum RF power
  * (as delivered by SysInfoTxCoilMaxPower)
  *
  * A duration of 0ms is returned if the routed coil does not power
  * limits or max duration
  *
  * @param chanNum  logical channel number [1...N]
  * @param length The duration is returned in this variable
  * @return @c 0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_TxCoilMaxPowPulselength(const int chanNum, double* length);

/**
  * @ingroup SysInfo_overlay
  * Delivers a slash separated list of nuclei for which the transmit coil routed on
  * channel @a chan is adapted. The bandwidths of the amplifier and preamplifier
  * are also taken into account.
  *
  * @param chanNum  logical channel number [1...N]
  * @param nuclei Must point to a buffer of size PVSYSINFOCHANSTATE_LEN, into which
  *               the nuclei list is copied.
  * @return       @c 0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_TxCoilNuclei(const int chanNum, char* nuclei);

/**
  * @ingroup SysInfo_overlay
  * Return the transmit coil routed on channel @chanNum
  *
  * @param chanNum  logical channel number [1...N].
  * @return         a pointer to the coil, or nullptr.
  * @throw Error   in case of any problem.
  */
DLL_INTERFACE const PvRouting::CoilReference* SysInfo_TxCoil(const int chanNum);

/**
  * @ingroup SysInfo_overlay
  * Delivers the scaling factors (multiplicative) that have to be applied to
  * the transmit amplitudes on channel @a chan to equalize them.
  *
  * For non-array coils, the delivered value is always 1.0 .
  *
  * @param chanNum  logical channel number [1...N] for which the scalings should be returned
  * @param scalings The scaling factors are returned in this variable.
  * @return         @c  0 if everything is OK.

  */
DLL_INTERFACE int SysInfo_TxScalings(const int chanNum,
                                     std::vector<double>* scalings);

/**
  * @ingroup SysInfo_overlay
  * Delivers the phase corrections (additive) that have to be applied to
  * the signals for the coils on channel @a chan to equalize them.
  *
  * For non-array coils, the delivered value is always 0.0.
  *
  * @param chanNum  logical channel number [1...N] for which the scalings should be returned
  * @param phaseShifts The phase shifts (in degree) are returned in this variable.
  * @return            @c  0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_TxPhaseOffsets(const int chanNum,
                                         std::vector<double>* phaseShifts);



/* --- Receive Chain: --- */

/**
  * @ingroup SysInfo_general
  * Delivers a code representing the state of the receiver chain routed
  * on channel chan. This code is constructed from identifiers all elements
  * of the chain (sgu, preamp, rx-coil).
  *
  * @param psInterface  The parameter space interface.
  * @param client       The system information provider, if not \c 0 it is used else
  *                     a temporary provider is instantiated.
  * @param chanSpec     logical channel number [1...N] for logical state
  *                     bitmask of used sguIdxs for physical state
  * @param state        logical or physical state
  * @param key          Must point to a buffer of size PVSYSINFOCHANSTATE_LEN, into which
  *                     the state description is written
  * @return      @c 0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_RxState(
    const Parx::Common::ParamInterface & psInterface,
    const SysInfoClient * client,
    const int chanSpec, const SysInfo_TxRxState& state, char * key);

/**
  * @ingroup SysInfo_overlay
  * Delivers a code representing the state of the receiver chain routed
  * on channel chan. This code is constructed from identifiers all elements
  * of the chain (sgu, preamp, rx-coil).
  *
  * @param chanSpec     logical channel number [1...N] for logical state
  *                     bitmask of used sguIdxs for physical state
  * @param state        logical or physical state
  * @param key          Must point to a buffer of size PVSYSINFOCHANSTATE_LEN, into which
  *                     the state description is written
  * @return      @c 0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_RxState(const int chanSpec,
                                  const SysInfo_TxRxState& state,
                                  char * key);

/**
  * @ingroup SysInfo_overlay
  * Provide information about the number of receive-capable channels.
  * @return     The number of receive-capable channels.
  */
DLL_INTERFACE size_t SysInfo_RxNChannels();

/**
  * @ingroup SysInfo_overlay
  * Returns the number of receive coil elements as currently routed to channel
  * @a chan. Quadrature coil element pairs with an combiner are reported as a single
  * coil.
  * @param chanNum  logical channel number [1...N] for which the scalings should be returned
  * @param nrxelements The coil element count is placed in this variable
  * @return @c 0 if everything is OK.
  */
DLL_INTERFACE int  SysInfo_RxNCoilElements(const int chanNum, int* nrxelements);

/**
 *  @ingroup SysInfo_overlay
 *  Returns a table indicating which transmit coil elements are currently enabled on channel
 *  @a chanNum.
  * @param chanNum        logical channel number [1...N] for which the transmit coil element selection
 *                        table should be returned.
 *  @param activeElements The Enabled/Disabled state for all routed elements is placed in this
 *                        variable.
 *  @return               @c 0 if everything is OK.
 */
DLL_INTERFACE int SysInfo_RxActiveCoilElements(const int chanNum,
                                               std::vector<bool>* activeElements);

/**
 *  @ingroup SysInfo_overlay
 *  Returns the index of the signal generation unit connected to a coil element on channel
 *  @a chanNum.
 *  @param chanNum      logical channel number [1...N]
 *  @param coilElemIdx  coil element index [0...N-1]
 *  @param txSguIdx     index [0...N-1] of the signal generation unit connected
 *                      to the requested element.
 *  @return             @c 0 if everything is OK.
 */
DLL_INTERFACE int SysInfo_RxSguIdx(const int chanNum,
                                   const int coilElemIdx,
                                   int* rxSguIdx);

/**
 *  @ingroup SysInfo_overlay
 *  Returns the indeces of signal generation units routed on channel @a chanNum as bitmask.
 *  @param chanNum        logical channel number [1...N]
 *  @return the bitmask of used rxSgu indeces
 */
DLL_INTERFACE int SysInfo_RxSgusBitmask(const int chanNum);

/**
 *  @ingroup SysInfo_overlay
 *  Returns the numbers of signal generation units routed on channel @a chanNum.
 *  @param chanNum        logical channel number [1...N]
 *  @param txSguNums      
 *  @return the numbers [1...N] of the signal generation units.
 */
DLL_INTERFACE std::set<int> SysInfo_RxSguNums(const int chanNum);

/**
 *  @ingroup SysInfo_overlay
 *  Returns the numbers of receivers routed on channel @a chanNum.
 *  @param chanNum      logical channel number [1...N]
 *  @return the numbers [1...N] of the receivers.
 */
DLL_INTERFACE std::set<int> SysInfo_RecNums(const int chanNum);

/**
 *  @ingroup SysInfo_overlay
 *  Returns the numbers of receive preamplifiers routed on channel @a chanNum.
 *  @param chanNum      logical channel number [1...N]
 *  @param rxPreampNums [1...N] of the preamplifiers.
 *  @return               @c 0 if everything is OK.
 */
DLL_INTERFACE int SysInfo_RxPreampNums(const int chanNum,
                                       std::set<int>* rxPreampNums);

/**
 *  @ingroup SysInfo_overlay
 *  Returns the number of the receiver connected to a receive preamplifier.
 *  @param rxPreampNum       preamplifier number [1...N]
 *  @param recNum            number [1...N] of the receiver.
 *  @return               @c 0 if everything is OK.
 */
DLL_INTERFACE int SysInfo_RecNumForRxPreamp(const int rxPreampNum,
                                            int* recNum);

/**
 *  @ingroup SysInfo_overlay
 *  Returns a table indicating which receive coil elements are currently enabled on channel
 *  @a chan.
 *  @param chan           Channel specification for which the receive coil element selection
 *                        table should be returned.
 *  @param activeElements The Enabled/Disabled state for all routed elements is placed in this
 *                        variable. It must point to a buffer with sufficient elements to hold
 *                        the activation state for the number of coil elements returned
 *                        by SysInfo_RxNCoilElements().
 *  @return               @c 0 if everything is OK.
 */
DLL_INTERFACE int SysInfo_RxActiveCoilElements(int chan, YesNo * activeElements);

/**
  * @ingroup SysInfo_overlay
  * Delivers a slash separated list of nuclei for which the receive coil(s) routed on
  * this channel are adapted. The bandwidth of the preamplifier is also taken into account.
  *
  * @param chan   channel specification for which the nuclei list should be returned
  * @param nuclei Must point to a buffer of size PVSYSINFOCHANSTATE_LEN, into which
  *               the nuclei list is copied.
  * @return       @c  0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_RxCoilNuclei(int chan, char * nuclei);

/**
  * @ingroup SysInfo_overlay
  * Return the receive coil routed on channel @chanNum
  *
  * @param chanNum  logical channel number [1...N].
  * @return         a pointer to the coil, or nullptr.
  * @throw Error   in case of any problem.
  */
DLL_INTERFACE const PvRouting::CoilReference* SysInfo_RxCoil(const int chanNum);

/**
  * @ingroup SysInfo_overlay
  * Delivers the scaling factors (multiplicative) that have to be applied to
  * (parallel-)data on channel @a chanNum to equalize them.
  *
  * @param chanNum  logical channel number [1...N] for which the scalings should be returned
  * @param scalings Return M scaling factors, where M is the number of
  *                 parallel data streams on channel @a chan.
  *                 (The scaling factors are sorted according to their originating receiver numbers.)
  * @return         @c  0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_RxDataScalings(const int chanNum,
        std::vector<double>* scalings);

/**
  * @ingroup SysInfo_overlay
  * Delivers the number of parallel data streams on a channel @a chanNum.
  *
  * @param chanNum  logical channel number [1...N] for which the count should be returned
  * @param cnt      Return the number of data streams
  * @return         @c  0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_RxDataCount(const int chanNum, size_t* cnt);

/**
  * @ingroup SysInfo_overlay
  * Delivers the phase corrections (additive) that have to be applied to
  * the signals for the coils on channel @a chan to equalize them.
  *
  * For non-array coils, the delivered value is always 0.0.
  *
  * @param chanNum  logical channel number [1...N] for which the scalings should be returned
  * @param phaseShifts The phase shifts (in degree) are returned in this variable.
  * @return            @c  0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_RxDataPhaseOffsets(const int chanNum, PhaseCorrectionPolicy policy,
        std::vector<double>* phaseShifts);

/**
  * @ingroup SysInfo_overlay
  * Delivers the nearest RG value on channel @a chanNum, depending on the search direction.
  *
  * @param chanNum     logical channel number [1...N] for which the value should be returned
  * @param desiredGain Return the nearest possible gain value.
  * @param searchDir   specify the search policy if the input value is not exactly valid:
  *                      1: get next higher value
  *                      0: get arithmetically closest value
  *                     -1: get next lower value
  * @return         @c  0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_NearestRecGainValue(const int chanNum,
        double* desiredGain, const int searchDir = 0);

/**
  * @ingroup SysInfo_overlay
  * Delivers the maximum RG value on channel @a chanNum.
  *
  * @param chanNum     logical channel number [1...N] for which the value should be returned
  * @param gain        Return the maximum possible gain value.
  * @return         @c  0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_MaxRecGainValue(const int chanNum, double* gain);

/**
  * @ingroup SysInfo_overlay
  * Delivers the minimum RG value on channel @a chanNum.
  *
  * @param chanNum     logical channel number [1...N] for which the value should be returned
  * @param gain        Return the maximum possible gain value.
  * @return         @c  0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_MinRecGainValue(const int chanNum, double* gain);

/**
  * @ingroup SysInfo_overlay
  * Returns the table of RG values for channel @a chanNum.
  *
  * @param chanNum     logical channel number [1...N] for which the table should be returned
  * @param gainTab     Return the gain table
  * @return         @c  0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_RecGainTab(const int chanNum,
        std::vector<double>* gainTab);

/**
 * Get the minimum value of the pre-scan-delay (a.k.a 'DE')
 * @param minScanDelay  The placeholder for the minimum pre-dcan-delay Value (aka 'DE')
 * @return  The minimum 'DE' value
 */
/**
 *  Get the current value of the pre-scan-delay, i.e waiting time between TX and RX,
 *  considering preamplifier TX/RX switching and the coils ringing delay for example.
 *  @param chanNum     logical channel number [1...N] for which the value should be returned.
 *                     If left empty, the delay over all channels is returned.
 *  @return  The minimum delay in [us]
 */
DLL_INTERFACE double SysInfo_MinPreScanDelay(const int chanNum = 0);

/* --- Gradient Chain --- */

/**
  * @ingroup SysInfo_general
  * Provides a key representing the state of the gradient chain.
  * On the method level the return value may be used eg. to describe hardware
  * scope of adjustment results.
  *
  * @param psInterface The parameter space interface.
  * @param client The system information provider, if not \c 0 it is used else
  *               a temporary provider is instantiated.
  * @param key   Must point to a buffer of size PVSYSINFOCHANSTATE_LEN, into which
  *              the state description is written
  * @return      @c  0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_GradState(
    const Parx::Common::ParamInterface & psInterface,
    const SysInfoClient * client, char * key);

/**
  * @ingroup SysInfo_overlay
  * Provides a key representing the state of the gradient chain.
  * On the method level the return value may be used eg. to describe hardware
  * scope of adjustment results.
  *
  * @param key   Must point to a buffer of size PVSYSINFOCHANSTATE_LEN, into which
  *              the state description is written
  * @return      @c  0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_GradState(char * key);

/**
  * @ingroup SysInfo_overlay
  * Provides a code representing the gradient status.
  * On the method level the return value may be used eg. to load the preemphasis file.
  *
  * @param status Must point to a buffer of size PVSYSINFOCHANSTATE_LEN, into which
  *               the status description is written
  * @return       @c  0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_GradStatus(char * status);

/**
  * @ingroup SysInfo_overlay
  * Get if the current MR instrument is a data station.
  *
  * @param isDatastation   'true' if the instrument is a data station, 'false'
  *                         otherwise.
  *
  * @return      0 in case of success
  *               1 in case, no information is available
  *              -1 in case of an errror condition
  */
DLL_INTERFACE int SysInfo_IsDatastation(bool *isDatastation);

/**
  * @ingroup SysInfo_overlay
  * Get the Instrument Configuration Directory.
  *
  * @param path   The instrument path
  *
  * @return      0 in case of success
  *               1 in case, no information is available
  *              -1 in case of an errror condition
  */
DLL_INTERFACE int SysInfo_InstrPath(std::string& path);

/**
  * @ingroup SysInfo_overlay
  * Delivers max current in ampere, that can be provided by
  * Gradient Power Supply of gradient channel X, Y or Z.
  *
  * @param chan	    Gradient channel @c "X", @c "Y" or @c "Z"
  * @param current  Maximum current [A]
  *
  * @return       0 in case of success
  *               1 in case, no information is available
  *              -1 in case of an errror condition
  */
DLL_INTERFACE int SysInfo_GPSCurrentMax(const char *chan, double *current);

/**
  * @ingroup SysInfo_overlay
  * Delivers max voltage in volt, that can be provided by
  * Gradient Power Supply of gradient channel X, Y or Z.
  *
  * @param chan	    Gradient channel @c "X", @c "Y" or @c "Z"
  * @param voltage  Maximum voltage [V]
  *
  * @return       0 in case of success
  *               1 in case, no information is available
  *              -1 in case of an errror condition
  */
DLL_INTERFACE int SysInfo_GPSVoltageMax(const char *chan, double *voltage);

/**
 *  @ingroup SysInfo_overlay
 *  Delivers the maximum clock for gradient power supplies.
 *  If the result is 0.0, the GPS are analog.
 *  @return     maximum clock off all installed gradient power supplies   
 **/
DLL_INTERFACE double SysInfo_GPSClockMax(void);
/**
  @ingroup SysInfo_overlay
  Maximum current to be applied on a gradient coil.
 
  @param id         name of gradient coil, e.g. @c "X", @c "Y", or @c "Z"
  @param maxCurrent maximum current [A]
  @return           @c 0 if everything is OK.
*/
DLL_INTERFACE int SysInfo_GradMaxCurrent(const char *id, double *maxCurrent);

/**
  @ingroup SysInfo_overlay
  Maximum power to be applied on a gradient coil.

  @param id       name of gradient coil, e.g. @c "X", @c "Y", or @c "Z"
  @param maxPower maximum power [W]
  @return         @c 0 if everything is OK.
*/
DLL_INTERFACE int SysInfo_GradMaxPower(const char *id, double *maxPower);

/**
  @ingroup SysInfo_overlay
  Resistance of a gradient coil.
  @param id         name of gradient coil, e.g. @c "X", @c "Y", or @c "Z"
  @param resistance resistance [Ohm]
  @return           @c 0 if everything is OK.
*/
DLL_INTERFACE int SysInfo_GradResistance(const char *id, double *resistance);

/**
  @ingroup SysInfo_overlay
  Offset of Z gradient coil with respect to patient coordinate system.

  @param gradOffset gradient offset [mm]
  @return           @c 0 if everything is OK.
*/
DLL_INTERFACE int SysInfo_GradOffset(double *gradOffset);

/**
  @ingroup SysInfo_overlay
  Default field-of-view for current gradient system.

  @param fov default FOV [mm]
  @return    @c 0 if everything is OK.
*/
DLL_INTERFACE int SysInfo_DefaultFov(double *fov);

#if 0
/* Gradient and shims not yet supported  */
/** SysInfo_GrMaxDCCurrent

Delivers max DC current in ampers for gradient channel (0=x,1=y,2=z).
Value is dependent on the max. duty cycle of gradient power supply,
and gradient coil.*/
int SysInfo_GrMaxDCCurrent(SysInfoHandleT, int chan, double *current);

/** SysInfo_GrMaxPeakCurrent
Delivers max current in ampers at 100% gradient amplitude (trim=100%,
acq_scaling=1, preemp_scaling=1) for gradient channel chan. */
int SysInfo_GrMaxPeakCurrent(SysInfoHandleT, int chan, double *current);

/** SysInfo_GrMaxPeakCurrent
   Delivers the bandwidth of the gradient amplifier on channel chan.*/
int SysInfo_GrMaxPeakCurrent(SysInfoHandleT, int chan, double *band);
#endif

/* --- Shim Chain --- */

/**
  * @ingroup SysInfo_general
  * Provides a key representing the state of the shim chain.
  * The key is constructed from identifies of all elements of the shim chain
  * (shim coil, gradient coil, power supply, gradient power supply, GRASI, TOSI ...).
  * The state is intended to be used to describe the hardware scope of shim
  * calibration information. In case the components of the shim chain are
  * not detected, the key is set to an empty string.
  *
  * @param psInterface  Parameter space interface of the current scan.
  * @param client The system information provider, if not \c 0 it is used else
  *               a temporary provider is instantiated.
  * @param key   Must point to a string buffer of size PVSYSINFOCHANSTATE_LEN,
  *              into which the state description is written
  * @return      != 0 if ShimStatus cannot be retrieved
  */
DLL_INTERFACE int SysInfo_ShimState(
    const Parx::Common::ParamInterface & psInterface,
    const SysInfoClient * client, char * key);

/**
  * @ingroup SysInfo_overlay
  * Provides a key representing the state of the shim chain.
  * The key is constructed from identifies of all elements of the shim chain
  * (shim coil, gradient coil, powersupply, gradient powersupply, GRASI, TOSI ...).
  * The state is intended to be used to describe the hardware scope of shim
  * calibration information. In case the components of the shim chain are
  * not detected, the key is set to an empty string.
  *
  * @param key   Must point to a string buffer of size PVSYSINFOCHANSTATE_LEN,
  *              into which the state description is written
  * @return      != 0 if ShimStatus cannot be retrieved
  */
DLL_INTERFACE int SysInfo_ShimState(char * key);

/**
  * @ingroup SysInfo_overlay
  * Provides a code representing the shim status.
  * On the method level the return value may be used eg. to load the preemphasis file.
  *
  * @param status Must point to a buffer of size PVSYSINFOCHANSTATE_LEN, into which
  *               the status description is written
  * @return       @c  0 if everything is OK.
  */
DLL_INTERFACE int SysInfo_ShimStatus(char * status);

/**
  * @ingroup SysInfo_overlay
  * Delivers, how many shims are actually available
  *
  * @return       number of actual shims available
  *
  * In case of an error PvStdException() is thrown.
  */
DLL_INTERFACE int SysInfo_NShims(void);

/**
  * @ingroup SysInfo_overlay
  * Delivers, how many harmonic shim functions are actually available
  *
  * @return       number of harmonic shims shims actuallly available
  *
  * In case of an error PvStdException() is thrown.
  */
DLL_INTERFACE int SysInfo_NShimFunctions(void);

/**
  * @ingroup SysInfo_overlay
  * Delivers, information if dynamic shimming is possible.
  * (e.g. BSMS does not support dynamic shimming)
  *
  * In case of an error PvStdException() is thrown.
  * @return Yes (No) if dynamic shimming is (not) possible.
  */
DLL_INTERFACE YesNo SysInfo_DynamicShimming(void);

/**
  * @ingroup SysInfo_overlay
  * Delivers rise time (unit ms) of dynamic shim switching event.
  * The risetime is the minimum duration of a shim ramp for a 100%
  * step that can be applied on all shim channels in a dynamic shimming
  * experiment.
  *
  * In case of a error PvStdException() is thrown.
  * @return rise time (unit ms), 0.0 in case dynamic shimming is not possible.
  */
DLL_INTERFACE double SysInfo_ShimRiseTime(void);

/**
  * @ingroup SysInfo_overlay
  * For all actual shim channels the function
  *   delivers maximal Current of Shim Power Supply,
  *   corresponding to CONFIG_shim_max_val/uxnmr.par:ShimMaxVal.
  *
  * @param imax  Must reference to a vector of double.
  *              The vector is resized to the # of available shims
  *              (imax.size() equals SysInfo_NShims()) and filled accordingly
  *
  * In case of an error PvStdException() is thrown.
  */
DLL_INTERFACE void SysInfo_MaxCurrentSPS(std::vector<double> &imax);

/**
  * @ingroup SysInfo_overlay
  * For all actual shim channels the function
  *   delivers maximal Current the coil can take
  *
  * @param imax  Must reference to a vector of double.
  *              The vector is resized to the # of available shims
  *              (imax.size() equals SysInfo_NShims()) and filled accordingly
  *
  * In case of an error PvStdException() is thrown.
  */
DLL_INTERFACE void SysInfo_MaxShimCurrent(std::vector<double> &imax);


/**
  * @ingroup SysInfo_overlay
  * Indicates if shim vector controls offset frequency on frequency channel.
  * If available, the 1st entry of a shim vector controlls the B0
  * component as additive offset to the transmit/receive frequency
  * channel. If set to false entries of shim vectors control solidly
  * dependent field variations only.
  *
  * @return true (false) if B0 handling is (not) available.
  *
  * In case of an error PvStdException() is thrown.
  */
DLL_INTERFACE bool SysInfo_ShimB0(void);

/**
  * @ingroup SysInfo_overlay
  * Indicates if the shims are controlled by a BSMS
  *
  * @return true if shim controlled by BSMS.
  *
  * In case of an error PvStdException() is thrown.
  */
DLL_INTERFACE bool SysInfo_ShimUnitBSMS(void);


/**
  * @ingroup SysInfo_overlay
  *   Delivers spherical harmonic function coefficients for the specifid shim.
  *
  * @param id     index within the array, containing actual shims
  *               range 0,.., navsh -1
  * @param sens  Reference to a vector, containing the sensitivity in
  *               Hz/cm**order/A and the corresponding spherical harmonic function
  * @param nconv  Pointer to normalization convention:
  *               Set to @c 1: normalization like associated legendre polynomials
  *                            (e.g. BioSpec shims)
  *               Set to @c 2: maximum on unit sphere equal one ( most microimaging coils).
  *
  * In case of an error PvStdException() is thrown.
  */
DLL_INTERFACE void SysInfo_FieldSensitivity(int id,
   std::vector<SHIM_SENSITIVITY_TYPE> &sens, int *const nconv = 0);

/**
  * @ingroup SysInfo_client
  * Indicates which shim function is available
  * (marked as nonzero elements in array sfkey)
  *
  * @param psid   Parameter space id of the current scan
  * @param sfkey  Must point to an array of length N_SHIMS
  *
  * @return       0 in case of success
  *               1 in case, no information about shim functions is available
  *              -1 in case of an errror condition
  */
DLL_INTERFACE int SysInfo_ShimFunctions(int psid, int *sfkey); // DEPRECATED

/**
  * @ingroup SysInfo_overlay
  * Indicates which shim function is available
  * (marked as nonzero elements in array sfkey)
  *
  * @param sfkey  Must point to an array of length N_SHIMS
  *
  * @return       0 in case of success
  *               1 in case, no information about shim functions is available
  *              -1 in case of an errror condition
  */
DLL_INTERFACE int SysInfo_ShimFunctions(int *sfkey); // DEPRECATED

/**
  * @ingroup SysInfo_client
  * Indicates which shim coils is connected to the  shim power supply
  * (marked as nonzero elements in array srkey)
  *
  * @param psid   Parameter space id of the current scan
  * @param srkey  Must point to an array of length N_SHIMS
  *
  * @return       0 in case of success
  *               1 in case, no information about shim functions is available
  *              -1 in case of an errror condition
  */
DLL_INTERFACE int SysInfo_RawShims(int psid, int *srkey); // DEPRECATED

/**
  * @ingroup SysInfo_overlay
  * Indicates which shim coils is connected to the  shim power supply
  * (marked as nonzero elements in array srkey)
  *
  * @param srkey  Must point to an array of length N_SHIMS
  *
  * @return       0 in case of success
  *               1 in case, no information about shim functions is available
  *              -1 in caes fo an error
  */
DLL_INTERFACE int SysInfo_RawShims(int *srkey); // DEPRECATED

/**
  * @ingroup SysInfo_client
  * Delivers the max int value of shim function vector components.
  *
  * @param psid   Parameter space id of the current scan
  * @param sfmax  Points to a scalar integer value
  *
  * @return       0 in case of success
  *               1 in case of information is available
  */
DLL_INTERFACE int SysInfo_ShimFuncMaxVal(int psid, int *sfmax); // DEPRECATED

/**
  * @ingroup SysInfo_overlay
  * Delivers the max int value of shim function vector components.
  *
  * @param sfmax  Points to a scalar integer value
  *
  * @return       0 in case of success
  *               1 in case of information is available
  */
DLL_INTERFACE int SysInfo_ShimFuncMaxVal(int *sfmax); // DEPRECATED

/**
  * @ingroup SysInfo_client
  * Delivers integer values of raw shim vector components.
  * corresponding to max allowed current (independent of power
  * or total current constraints) on corresponding shim coils.
  * A value of 0 is set for a shim channel, that is not connected
  * to a coil.
  *
  * @param psid   Parameter space id of the current scan
  * @param srmax  Must point to an int array of size N_SHIMS
  *
  * @return       0 in case of success
  *               1 in case of information is available
  *              -1 in case of an error condition
  */
DLL_INTERFACE int SysInfo_ShimRawMaxVal(int psid, int *srmax);  // DEPRECATED

/**
  * @ingroup SysInfo_overlay
  * Delivers integer values of raw shim vector components.
  * corresponding to max allowed current (independent of popwer
  * or total current constraints) on corresponding shim coils.
  * A value of 0 is set for a shim channel, that is not connected
  * to a coil.
  *
  * @param srmax  Must point to an int array of size N_SHIMS
  *
  * @return       0 in case of success
  *               1 in case of no information is available
  *              -1 in case of an error condition
  */
DLL_INTERFACE int SysInfo_ShimRawMaxVal(int *srmax); // DEPRECATED

/**
  * @ingroup SysInfo_client
  * Converts the SHIM_PAR enum state into the related shim ID
  *
  * @param par    variable of type SHIM_PAR
  * @param id     Points to an int variable that specifies the shim ID
  *
  * @return       0 in case of success
  *              -1 in case of illegal values of par
  */
DLL_INTERFACE int SysInfo_ShimParToId(SHIM_PAR par, int *const id); // DEPRECATED

/**
  * @ingroup SysInfo_client
  * Converts the shim ID into the related SHIM_PAR enum state
  *
  * @param par    Points to a variable of type SHIM_PAR
  * @param id     int variable that specifies the shim ID
  *
  * @return       0 in case of success
  *              -1 in case of illegal values of par
  */
DLL_INTERFACE int SysInfo_ShimIdToPar(SHIM_PAR *const par, int id); // DEPRECATED

/**
  * @ingroup SysInfo_client
  * Returns the SHIM_PAR enum of most signicant shim function of the shim,
  * specifed by ID
  *
  * ID must be in interval [0,number actual avaialbe shims -1].
  *
  * @param id     int variable that specifies the shim ID
  *
  * @return       shim function as SHIM_PAR enum
  *
  */
DLL_INTERFACE SHIM_PAR SysInfo_ShimIdToPar(int id);
DLL_INTERFACE SHIM_PAR SysInfo_ShimIdToPar(int psid, int id);


/**
  * @ingroup SysInfo_client
  * Returns the name of most significant shim function of the shim,
  * specified by ID
  *
  * ID must be in interval [0,number actual available shims -1].
  *
  * @param psid   Parameter space id of the current scan
  * @param id     int variable that specifies the shim ID
  *
  * @return       pointer to const string containing unique shim name
  *
  */
DLL_INTERFACE const char *SysInfo_ShimIdToName(int psid, int id);

/**
  * @ingroup SysInfo_client
  * Returns the name of most signicant shim funcion of the shim,
  * specifed by ID
  *
  * ID must be in interval [0,number actual avaialbe shims -1].
  *
  * @param id     int variable that specifies the shim ID
  *
  * @return       pointer to const string containing unique shim name
  *
  */
DLL_INTERFACE const char *SysInfo_ShimIdToName(int id);

/**
  * @ingroup SysInfo_client
  * Converts the shim function (argument func into a unique shim name
  * specified as string.
  *
  * @param func   SHIM_PAR variable that specifies the shim funcion
  *
  * @return       pointer to const string containing unique shim name
  *
  */
DLL_INTERFACE const char *SysInfo_ShimFuncToName(SHIM_PAR func);

/**
  * @ingroup SysInfo_client
  * Converts a (legal) unique shim name into a shim ID. Illegal
  * names result in a nonzero return value, argument id remains
  * unchanged in this case.
  *
  * @param name   unique shim name
  * @param id     int variable that specifies the shim ID
  *
  * @return       0 in case of success
  *              -1 in case of illegal shim name
  *               1 in case of internal error
  */
DLL_INTERFACE int SysInfo_ShimNameToId(const char *const name, int *const id);

/**
  * @ingroup SysInfo_client
  * Provides the scaling factors in units of Ampere/(DAC value]
  * that are used to convert raw shim value int currents. A value
  * of 0 is set for a shim ID that is not connected to the shim
  * power supply.
  *
  * @param psid   Parameter space id of the current scan
  * @param lambda Must reference to a vector of double.
  *               The vector is resized to the # of available shims
  *
  * @return       0 in case of success
  *               1 no information is available
  *              -1 in case of an error condition
  */
DLL_INTERFACE int SysInfo_DacScale(int psid, std::vector<double> &lambda);

/**
  * @ingroup SysInfo_overlay
  * Provides the scaling factors in units of Ampere/(DAC value]
  * that are used to convert raw shim value int currents. A value
  * of 0 is set for a shim ID that is not connected to the shim
  * power supply.
  *
  * @param lambda Must reference to a vector of double.
  *               The vector is resized to the # of available shims
  *
  * @return       0 in case of success
  *               1 no information is available
  *              -1 in case of an error condition
  */
DLL_INTERFACE int SysInfo_DacScale(std::vector<double> &lambda);

/**
  * @ingroup SysInfo_client
  * Delivers the order of spherical harmonic functions
  * specified by offset in array  sford
  *
  * @param sford  Must reference to a vector of int
  *               The vector is resized to the # of available shims
  *
  * @return       0 in case of success
  */
DLL_INTERFACE int SysInfo_ShimOrder(std::vector<int> &sford);

/**
 * @ingroup SysInfo_client
 * Delivers the order of a spherical harmonic functions
 *         *
 * @param func  spherical harmonic function
 * @return      returns order of harmonic function
 *                      -1 in case of erraneous shim function
 */

DLL_INTERFACE int SysInfo_ShimFuncOrder(SHIM_PAR func);

/**
  * @ingroup SysInfo_client
  * Delivers spherical harmonic coefficients of shim functions
  * in units of Hz/cm^^N for the max allowed current through
  * coil specified by id. The spherical harmonic function that
  * corresponds to the coefficients is specified by the offset
  * inside argument srfm.
  *
  * @param psid   Parameter space id of the current scan
  * @param id     id of the shim coil
  * @param srfm   Must point to a double array of size N_SHIMS
  *
  * @return       0 in case of success
  *               1 no information is available
  *              -1 in case of an error condition
  */
 DLL_INTERFACE int SysInfo_ShimMaxRawField(int psid, // DEPRECATED
                                    const int id, double *srfm);

/**
  * @ingroup SysInfo_overlay
  * Delivers spherical harmonic coefficients of shim functions
  * in units of Hz/cm^^N for the max allowed current through
  * coil specified by id. The spherical harmonic function that
  * corresponds to the coefficients is specified by the offset
  * inside argument srfm.
  *
  * @param id     id of the shim coil
  * @param srfm   Must point to a double array of size N_SHIMS
  *
  * @return       0 in case of success
  *               1 no information is available
  *              -1 in case of an error condition
  */
DLL_INTERFACE int SysInfo_ShimMaxRawField(const int id, double *srfm); // DEPRECATED

/**
  * @ingroup SysInfo_client
  * Delivers spherical harmonic coefficients of shim functions
  * in units of Hz/cm^^N for the max value of shim function
  * specified by id. The spherical harmonic function that
  * corresponds to the coefficients is specified by the offset
  * inside argument sfm.
  *
  * Note: In case of coupled coil pairs is not connected to the
  *       SPS mor than one component is not 0.0.
  *
  * @param psid   Parameter space id of the current scan
  * @param id     id of the shim function
  * @param sfm    Must point to a double array of size N_SHIMS
  *
  * @return       0 in case of success
  *               1 no information is available
  *              -1 in case of an error condition
  */
 DLL_INTERFACE int SysInfo_ShimMaxFuncField(int psid,         // DEPRECATED
                                     const int id, double *sfm);

/**
  * @ingroup SysInfo_overlay
  * Delivers sperical harmonic coefficients of shim functions
  * in units of Hz/cm^^N for the max value of shim function
  * specified by id. The spherical harmonic function that
  * corresponds to the coefficients is specified by the offset
  * inside argument sfm.
  *
  * Note: In case of coupled coil pairs is not connectd to the
  *       SPS mor than one component is not 0.0.
  *
  * @param id     id of the shim function
  * @param sfm    Must point to a double array of size N_SHIMS
  *
  * @return       0 in case of success
  *               1 no information is available
  *              -1 in case of an error condition
  */
DLL_INTERFACE int SysInfo_ShimMaxFuncField(const int id, double *sfm); //DEPRECATED

/**
  * @ingroup SysInfo_overlay
  * Specifies for which power supply a coil has to be considered for
  * the calculation of the max. allowed total power.
  * 0 - Coil must not be considered
  * 1 - Coil must be considered for 1st Powersupply
  * 2 - Coil must be considered for 2nd Powersupply
  * ..
  *
  * Note: The B0 coil is not considered
  *
  * @param pcnst  Must reference to a vector of int.
  *               The vector is resized to the # of available shims
  *
  * @return       0 in case of success
  *               1 no information is available
  *              -1 in case of an error condition
  */
DLL_INTERFACE int SysInfo_ShimPowConstrained(std::vector<int> &pcnst);


/**
  * @ingroup SysInfo_overlay
  * Specifies max allowed total power (in W) that can be
  * delivered by SPS and tolerated by the shim coils.
  *
  * Note: The value is reduced by the max B0 coil power.
  *
  * @param maxp   Points to a scalar double value
  *
  * @return       0 in case of success
  *               1 no information is available
  *              -1 in case of an error condition
  */
DLL_INTERFACE int SysInfo_ShimMaxPow(double *const maxp);

/**
  * @ingroup SysInfo_overlay
  * Specifies for which power supply a coil has to be considered
  * for total current constraints.
  * Entries of arg @a ccnst have the following values:
  * 0 - Coil must not be considered
  * 1 - Coil must be considered for 1st Powersupply
  * 2 - Coil must be considered for 2nd Powersupply
  * ..
  *
  *
  * @param ccnst  Must reference to a vector of int.
  *               The vector is resized to the # of available shims
  *
  * @return       0 in case of success
  *               1 no information is available
  *              -1 in case of an error condition
  */
DLL_INTERFACE int SysInfo_ShimCurrConstrained(std::vector<int> &ccnst);


/**
  * @ingroup SysInfo_overlay
  * Delivers the max total current that can be delivered by the
  * specified shim power supply
  *
  * Note: The value is reduced by the max B0 coil current.
  *
  * @param imsp   Points to a scalar double value
  * @param spidx  1, .., NSPS specifies the Shim Powersupply
  *
  * @return       0 in case of success
  *               1 no information is available
  *              -1 in case of an error condition
  */
DLL_INTERFACE int SysInfo_ShimMaxSumCurrent(double *const imsp, int spidx=1);

/**
  * @ingroup SysInfo_overlay
  * Delivers the resistance (in Ohm) of the coils.
  * Coil ID that are not connected to the SPS are set to 0.0.
  *
  * @param r      Must reference to a vector of double.
  *               The vector is resized to the # of available shims
  *               (imax.size() equals SysInfo_NShims()) and filled accordingly
  *
  * @return       0 in case of success
  *               1 no information is available
  *              -1 in case of an error condition
  */
DLL_INTERFACE int SysInfo_ShimResistance(std::vector<double> &r);

/**
  * @ingroup SysInfo_overlay
  * Delivers unique identifier of shim coupling matrix for BSMS (BOSS) systems.
  * For TOSI based system a unique identifier is used that does not
  * interfere with identifiers of BOSS systems.
  *
  * Return: 0 in case of success else errorcode (stat). Error states
  * represented as string may be retrieved
  *
  * @param shid   Must point to a buffer of size PVSYSINFOSHIMMATRIXID_LEN
  *
  * @return       0 in case of success
  *              !0 errorcode of error condition
  */
DLL_INTERFACE int SysInfo_ShimMatrixId(char *shid); // DEPRECATED

/**
  * @ingroup SysInfo_overlay
  * Delivers the number of installed shim power supplies
  *
  * @param nsps   Points to a scalar int value
  * @return       0 in case of success
  *              -1 in case of an error condition
  */
DLL_INTERFACE int SysInfo_ShimNSPS(int *const nsps);

/**
  * @ingroup SysInfo_overlay
  * Converts SHIM_PAR enum state to solid harmonic function represented
  * by degree n and order m. For states that cannot be represented by solid
  * harmonic function an exception is thrown.
  * Counterpart of SysInfo_ShimSolidHarmToShimPar
  * @see SysInfo_ShimSolidHarmToShimPar
  * @param sp  SHIM_PAR state
  * @param n   degree of solid harmonic function
  * @param m   order of solid harmonic function
  * @param name (optional) string representation of SHIM_PAR state sp
  */
DLL_INTERFACE void SysInfo_ShimParToSolidHarm(const SHIM_PAR sp,
                                              int &n, int &m,
                                              std::string *name=0);


/**
  * @ingroup SysInfo_overlay
  * Converts solid harmonid function represented by
  * by degree n and order m int a SHIM_PAR enum state.
  * For illegal degree n or order m or in case the
  * specified combination cannot be converted an exception
  * is thrown.
  * Counterpart of SysInfo_ShimParToSolidHarm
  * @see SysInfo_ShimParToSolidHarm
  * @param n   degree of solid harmonic function
  * @param m   order of solid harmonic function
  * @param sp  SHIM_PAR state
  * @return  @see LegendreP::Findex (n,m)
 */
DLL_INTERFACE int SysInfo_ShimSolidHarmToShimPar(const int n,
                                                 const int m,
                                                 SHIM_PAR &sp);

/**
  * @ingroup SysInfo_overlay
  * Field characterization of shim coil by solid harmonic coefficients.
  * The solid harmonic coefficients are provided in units of
  * @f$ Hz/cm^n/A @f$,
  * where id is the coil identifier @f$ id \in [0,MID-1],
  * MID=@f$ @ref SysInfo_NShims .
  * and coefficients belong to degree @f$ n @f$ and order @f$ m @f$
  * as defined by vector cnm .
  * The normalization convention of coefficients is associated to
  * @ref PVNL_LEGENDRE_NCONV with
  * @f$ 0 \equiv @f$ NcRomeoHoult @f$ 1 \equiv @f$ NcMaxLegendre
  * @f$ 2 \equiv @f$ NcMaxUnitSphere
  *
  * @see LegendreP::Field @see LegendreP::NormConst
  * @param id coil identifier
  * @param cnm vector containing the solid harmonic function expansion of
  *            field generated by coil
  * @param nconv desired normalization convention of solid
  *              harmonic coefficients
  * @return true if solid harmonic coefficients available as BIS information 
  */
DLL_INTERFACE bool SysInfo_ShimSolidHarmField(int id,
                                              std::vector<SOLID_HARMONIC_COEFF> &cnm,
                                              int *const nconv);


/**
  * @ingroup SysInfo_overlay
  * Identifiers for shim channels.
  * Provides identifiers for shim channels.
  * @param sid vector strings (resized to number of available shims)
  */
DLL_INTERFACE void SysInfo_ShimIdentifiers(std::vector<std::string> &sid);

/**
  * @ingroup SysInfo_overlay
  * Identifies available shim functions.
  * Shim functions are identified by their index @see LegendreP::Findex
  * @param sfid vector of shim function indices (size is number of
  *        available shim functions)
  * @param maxdeg maximum degree of shim function
  */
DLL_INTERFACE void SysInfo_ShimFunctions(std::vector<int> &sfid,
                                         unsigned int &maxdeg);


/**
  * @ingroup SysInfo_overlay
  * Retrieves GRADCC of current preemphasis calibration.
  * @return GRADCC
  */
DLL_INTERFACE double SysInfo_GRADCC(void);


/**
  * @ingroup SysInfo_overlay
  * Retrieves B0CC of current preemphasis calibration.
  * @return B0CC
  */
DLL_INTERFACE double SysInfo_B0CC(void);


/**
  * @ingroup SysInfo_overlay
  * Retrieves COILDEL of current preemphasis calibration.
  * @return COILDEL
  */
DLL_INTERFACE double SysInfo_COILDEL(void);


/**
  * @ingroup SysInfo_overlay
  * Retrieves DPPFAC of current preemphasis calibration.
  * @return DPPFAC
  */
DLL_INTERFACE double SysInfo_DPPFAC(void);


/**
  * @ingroup SysInfo_overlay
  * Retrieves RAMPTM of current preemphasis calibration.
  * @return RAMPTM
  */
DLL_INTERFACE double SysInfo_RAMPTM(void);


/**
  * @ingroup SysInfo_overlay
  * Retrieves RISETM of current preemphasis calibration.
  * @return RISETM
  */
DLL_INTERFACE double SysInfo_RISETM(void);


/**
  * @ingroup SysInfo_overlay
  * Retrieves the gradient resolution in us.
  * @return GRADRES
  */
DLL_INTERFACE double SysInfo_GRADRES(void);


/**
  * @ingroup SysInfo_overlay
  * Retrieves the scaling factors of current gradient system.
  * @return scaling factors
  */
DLL_INTERFACE std::vector<double> SysInfo_ScalingFactor(void);


/**
  * @ingroup SysInfo_overlay
  * Retrieves the actual current values of current gradient system.
  * @return actual current
  */
DLL_INTERFACE std::vector<double> SysInfo_ActualCurrent(void);


/**
  * @ingroup SysInfo_overlay
  * Retrieves the solid harmonic coefficients of the gradient coil
  * stored in CONFIG_SCAN_gradient_system for a given axis
  * If the coil is unknown, the returned vector is empty.
  * If a description in the BIS structure exists, it is used,
  * for a number of known gradient systems, default settings are provided.
  * @param coil_axis_str Text String to identify coil (X,Y,Z)
  * @return vector with solid harmonics coefficients
  */
DLL_INTERFACE std::vector<SOLID_HARMONIC_COEFF> SysInfo_GradientSolidHarmonicCoefficients(
  const std::string& coil_axis_str
);

/**
  * @ingroup SysInfo_overlay
  * Retrieves the solid harmonic coefficients of the gradient coil
  * stored in CONFIG_SCAN_shim_system for a given coil.
  * If the coil is unknown, the returned vector is empty.
  * @param coil id of coil in bis structure
  * @return vector with solid harmonics coefficients
  */
DLL_INTERFACE std::vector<SOLID_HARMONIC_COEFF> SysInfo_ShimSolidHarmonicCoefficients(
  const int coil
);

template<typename FunT>
void SysInfo_ReportOnError(const int state, FunT errorFun, const std::string& msgPrefix)
{
    if (state != 0)
        errorFun(msgPrefix + "SysInfo: " + SysInfo_getErrorMessageStr(state));
}


// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// Helper function to report an error
#define SYSINFO_ThrowOnError(state) SysInfo_ReportOnError(state,\
        [](const std::string& err){ throw PvStdException(err.c_str());}, \
        std::string(__FUNCTION__) + "(): ")


#endif /* CPROTO */

#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/


