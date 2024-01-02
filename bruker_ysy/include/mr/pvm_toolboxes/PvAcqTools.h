/****************************************************************
 *
 * Copyright (c) 2016 - 2018
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVACQMRTOOLS_H
#define PVACQMRTOOLS_H

#ifndef CPROTO

namespace ReadParx {
    class JobParameters;
    class JobDescription;
}

#include "mr/PvTypes/mrTypes.h"
#include <vector>

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

/* /pv60/gen/src/prg/pvm_toolboxes/PvAcqTools/nuclei.c */

/** Gets the basic frequency of the given nuclei.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_StandardFreq

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  double freq = ATB_GetTransmitterFreq(PVM_Nucleus1);
  @endcode

  @param[in] nucStr    Name of an isotope as defined in the nuclei table

  @return Frequency of the given nuclei

  @see @ref CFG_StandardFreq

  @ingroup PVM_ATB
*/
DLL_INTERFACE double ATB_GetTransmitterFreq(const char* nucStr);

/** Gets the nuclei enum value of the given nuclei string.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nucStr is a @c NULL pointer.
  - @a nucStr is a @c NULL string.

  @PVM_EXAMPLE
  @code
  PVM_Nuc1Enum = (PV_NUCLEUS_TYPE)ATB_GetNucEnum("PVM_Nuc1Enum", PVM_Nuc1);
  @endcode

  @param[in] enumPar    Name of a parameter of type @c PV_NUCLEUS_TYPE
  @param     nucStr     Name of an isotope as defined in the nuclei table

  @return Enum value of the given nuclei

  @ingroup PVM_ATB_INT
*/
DLL_INTERFACE PV_NUCLEUS_TYPE ATB_GetNucEnum(const char* enumPar,
                               char*       nucStr);

/** Writes the value of the given nucleus to the baselevel parameter @c NUC1.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nucStr is a @c NULL pointer.
  - @a nucStr is a @c NULL string.

  @PVM_EXAMPLE
  @code
  ATB_SetNuc1(PVM_Nucleus1);
  @endcode

  @param[in] nucStr    Name of an isotope as defined in the nuclei table

  @see @ref ATB_SetNuc2, @ref ATB_SetNuc3, @ref ATB_SetNuc4,
       @ref ATB_SetNuc5, @ref ATB_SetNuc6, @ref ATB_SetNuc7, @ref ATB_SetNuc8,
       @ref ATB_SetNucleus

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetNuc1(const char* nucStr);

/** Writes the value of the given nucleus to the baselevel parameter @c NUC2.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nucStr is a @c NULL pointer.
  - @a nucStr is a @c NULL string.

  @PVM_EXAMPLE
  @code
  ATB_SetNuc2(PVM_Nucleus2);
  @endcode

  @param[in] nucStr    Name of an isotope as defined in the nuclei table

  @see @ref ATB_SetNuc1, @ref ATB_SetNuc3, @ref ATB_SetNuc4,
       @ref ATB_SetNuc5, @ref ATB_SetNuc6, @ref ATB_SetNuc7, @ref ATB_SetNuc8,
       @ref ATB_SetNucleus

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetNuc2(const char* nucStr);

/** Writes the value of the given nucleus to the baselevel parameter @c NUC3.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nucStr is a @c NULL pointer.
  - @a nucStr is a @c NULL string.

  @PVM_EXAMPLE
  @code
  ATB_SetNuc3(PVM_Nucleus3);
  @endcode

  @param[in] nucStr    Name of an isotope as defined in the nuclei table

  @see @ref ATB_SetNuc1, @ref ATB_SetNuc2, @ref ATB_SetNuc4,
       @ref ATB_SetNuc5, @ref ATB_SetNuc6, @ref ATB_SetNuc7, @ref ATB_SetNuc8,
       @ref ATB_SetNucleus

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetNuc3(const char* nucStr);

/** Writes the value of the given nucleus to the baselevel parameter @c NUC4.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nucStr is a @c NULL pointer.
  - @a nucStr is a @c NULL string.

  @PVM_EXAMPLE
  @code
  ATB_SetNuc4(PVM_Nucleus4);
  @endcode

  @param[in] nucStr    Name of an isotope as defined in the nuclei table

  @see @ref ATB_SetNuc1, @ref ATB_SetNuc2, @ref ATB_SetNuc3,
       @ref ATB_SetNuc5, @ref ATB_SetNuc6, @ref ATB_SetNuc7, @ref ATB_SetNuc8,
       @ref ATB_SetNucleus

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetNuc4(const char* nucStr);

/** Writes the value of the given nucleus to the baselevel parameter @c NUC5.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nucStr is a @c NULL pointer.
  - @a nucStr is a @c NULL string.

  @PVM_EXAMPLE
  @code
  ATB_SetNuc5(PVM_Nucleus5);
  @endcode

  @param[in] nucStr    Name of an isotope as defined in the nuclei table

  @see @ref ATB_SetNuc1, @ref ATB_SetNuc2, @ref ATB_SetNuc3, @ref ATB_SetNuc4,
       @ref ATB_SetNuc6, @ref ATB_SetNuc7, @ref ATB_SetNuc8,
       @ref ATB_SetNucleus

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetNuc5(const char* nucStr);

/** Writes the value of the given nucleus to the baselevel parameter @c NUC6.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nucStr is a @c NULL pointer.
  - @a nucStr is a @c NULL string.

  @PVM_EXAMPLE
  @code
  ATB_SetNuc6(PVM_Nucleus6);
  @endcode

  @param[in] nucStr    Name of an isotope as defined in the nuclei table

  @see @ref ATB_SetNuc1, @ref ATB_SetNuc2, @ref ATB_SetNuc3, @ref ATB_SetNuc4,
       @ref ATB_SetNuc5, @ref ATB_SetNuc7, @ref ATB_SetNuc8,
       @ref ATB_SetNucleus

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetNuc6(const char* nucStr);

/** Writes the value of the given nucleus to the baselevel parameter @c NUC7.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nucStr is a @c NULL pointer.
  - @a nucStr is a @c NULL string.

  @PVM_EXAMPLE
  @code
  ATB_SetNuc7(PVM_Nucleus7);
  @endcode

  @param[in] nucStr    Name of an isotope as defined in the nuclei table

  @see @ref ATB_SetNuc1, @ref ATB_SetNuc2, @ref ATB_SetNuc3, @ref ATB_SetNuc4,
       @ref ATB_SetNuc5, @ref ATB_SetNuc6, @ref ATB_SetNuc8,
       @ref ATB_SetNucleus

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetNuc7(const char* nucStr);

/** Writes the value of the given nucleus to the baselevel parameter @c NUC8.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nucStr is a @c NULL pointer.
  - @a nucStr is a @c NULL string.

  @PVM_EXAMPLE
  @code
  ATB_SetNuc8(PVM_Nucleus8);
  @endcode

  @param[in] nucStr    Name of an isotope as defined in the nuclei table

  @see @ref ATB_SetNuc1, @ref ATB_SetNuc2, @ref ATB_SetNuc3, @ref ATB_SetNuc4,
       @ref ATB_SetNuc5, @ref ATB_SetNuc6, @ref ATB_SetNuc7,
       @ref ATB_SetNucleus

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetNuc8(const char* nucStr);

/** Writes the value of the given nucleus to the baselevel parameter @c NUCLEUS.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nucStr is a @c NULL pointer.
  - @a nucStr is a @c NULL string.

  @PVM_EXAMPLE
  @code
  ATB_SetNucleus(PVM_Nucleus1);
  @endcode

  @param[in] nucStr    Name of an isotope as defined in the nuclei table

  @see @ref ATB_SetNuc1, @ref ATB_SetNuc2, @ref ATB_SetNuc3, @ref ATB_SetNuc4,
       @ref ATB_SetNuc5, @ref ATB_SetNuc6, @ref ATB_SetNuc7, @ref ATB_SetNuc8

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetNucleus(const char* nucStr);


/** Sets parameters needed for multi-receiver acquisition.

  - Overrides some previously set parameters such as @c NUCn.
  - Must be called at the end of @c SetBaseLevel.
  - Dependend on the return value a special 4 channel adapted pulseprogram has to be loaded.
  - This is necessary for RCU based electronics.

  @PVM_PARS
  - @c ACQ_ReceiverSelect
  - @c ACQ_experiment_mode
  - @c ACQ_Routing_base_ext
  - <c>NUC2 NUC3 NUC4</c> (RCU based hardware only)

  @PVM_TBCALL
  - @ref CFG_NReceivers

  @PVM_RELCALL
  - Parameter relations of parameters listed above

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @return @c Yes in case of RCU based systems else @c No.

  @ingroup PVM_ATB
*/
DLL_INTERFACE YesNo ATB_SetMultiRec(void);

/* /pv60/gen/src/prg/pvm_toolboxes/PvAcqTools/modules.c */

/** Sets the baselevel parameters for the fat suppression preparation module.

  @PVM_PARS
  - @c PVM_FatSupPul
  - @c ACQ_RfShapes[40]
  - @c PVM_FatSupRfLength

  @PVM_TBCALL
  - @ref ATB_SetRFPulse

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_InitFatSupModule, @ref STB_UpdateFatSupModule

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetFatSupBaselevel(void);


/** Sets the baselevel parameters for the fat water separation module.

  @PVM_PARS
  - @c PVM_FwsDList1[]
  - @c PVM_FwsDList2[]

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_InitFwsModule, @ref STB_UpdateFwsModule

  @ingroup PVM_ATB

*/
DLL_INTERFACE void ATB_SetFwsBaseLevel(void);

/** Sets the reconstruction parameters for the fat water separation module.

  @PVM_PARS
  - @c PVM_FwsRecoMode
  - @c PVM_FwsKernelSize
  - @c PVM_FwsGaussBroadening
  - @c PVM_FwsImages
  - @c PVM_FwsSpatShift

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  To be called before setting the default parameters via ATB_InitUserModereco()

  @PVM_ERROR
  NONE

  @param dim       number of spatial dimensions
  @param matrix[]  matrix size [px]
  @param fov[]     field of view [mm]
  @param frqWork   working frequency [Hz]
  @param effSWh    acquisition bandwidth [Hz]

  @see @ref STB_InitFwsModule, @ref STB_UpdateFwsModule

  @ingroup PVM_ATB

*/
DLL_INTERFACE void ATB_SetFwsRecoParam(int dim, int matrix[], double fov[], double frqWork, double effSWh);




/**Adapts the reconstruction network for fat water separation.

  @PVM_PARS


  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  To be called after creation of the default network.

  @PVM_ERROR
  NONE

  @param dim       number of spatial dimensions
  @param matrix[]  matrix size [px]
  @param fov[]     field of view [mm]
  @param nslices   number of slices

  @see @ref STB_InitFwsModule, @ref STB_UpdateFwsModule

  @ingroup PVM_ATB

*/
DLL_INTERFACE void ATB_FwsRecoDerive(int dim, int matrix[], double fov[], int nslices);




//Sets the baselevel parameters for the magnetization transfer preparation module.
//Deprecated module. Replaced by SatTrans module. Following call remains to show
//a message pop-up for customer methods imported from older PVs still using it.

DLL_INTERFACE void ATB_SetMagTransBaseLevel(void);




/** Sets the baselevel parameters for the <em> saturation transfer </em> module.

  @PVM_PARS
  <table>
  <tr><th>   Source                        </th><th>    Destination        </th></tr>
  <tr><td>@c PVM_SatTransPulseAmpl.ppow    </td><td><c> ACQ_RfShapes[43].powerWatt </c></td></tr>
  <tr><td>@c PVM_SatTransPulse.Length      </td><td> @c PVM_StP0           </td></tr>
  <tr><td>@c PVM_SatTransInterDelay        </td><td >@c PVM_StD1           </td></tr>
  <tr><td>@c PVM_SatTransSpoil.dur         </td><td> @c PVM_StD2           </td></tr>
  <tr><td>@c PVM_RiseTime                  </td><td> @c PVM_StD3           </td></tr>
  </tr>
  </table>

  @PVM_TBCALL
  - @ref ATB_SetRFPulse

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_InitSatTransModule, @ref STB_UpdateSatTransModule

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetSatTransBaseLevel(void);




/** Sets the baselevel parameters for the <em>FOV saturation</em> preparation module.

  @PVM_PARS
  - @c PVM_FovSatPul
  - @c ACQ_RfShapes[41]
  - @c PVM_FovSatRfLength

  @PVM_TBCALL
  - @ref ATB_SetRFPulse

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_InitFovSatModule, @ref STB_UpdateFovSatModule

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetFovSatBaseLevel(void);




/** Sets the baselevel parameters for the <em>Flow Saturation</em> preparation module.

  @PVM_PARS
  - @c PVM_FlowSatPul
  - @c ACQ_RfShapes[44]
  - @c PVM_SfP0

  @PVM_TBCALL
  - @ref ATB_SetRFPulse
  - @ref CFG_AmplifierEnable
  - @ref CFG_GradientRiseTime

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_InitFlowSaturationModule, @ref STB_UpdateFlowSaturationModule

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetFlowSaturationBaseLevel(void);




/** Sets the baselevel parameters for the solvent suppression preparation module.

  The pulse gains are set depending on the parameters @c PVM_WsDeriveGains
  In MethodMode the pulse gains are derived only if the flag @c PVM_WsDeriveGains
  is set to @c Yes.

  @PVM_PARS
  CHESS mode:
  <table>
  <tr><th>    Source                         </th><th>    Destination       </th></tr>
  <tr><td> @c PVM_ChPul1Ampl.ppow            </td><td><c> ACQ_RfShapes[50].powerWatt</c></td></tr>
  <tr><td> @c PVM_ChPul2Ampl.ppow            </td><td><c> ACQ_RfShapes[51].powerWatt</c></td></tr>
  <tr><td> @c PVM_ChPul2Ampl.ppow            </td><td><c> ACQ_RfShapes[52].powerWatt</c></td></tr>
  <tr><td> @c PVM_ChPul1.Length              </td><td> @c PVM_WsP0          </td></tr>
  <tr><td> @c PVM_ChPul2.Length              </td><td> @c PVM_WsP1          </td></tr>
  <tr><td> @c PVM_ChPul3.Length              </td><td> @c PVM_WsP2          </td></tr>
  <tr><td><c> PVM_ChSpoilerStrength[][3] </c></td><td><c> PVM_ChTrim[]  </c></td></tr>
  </table>
  VAPOR mode:
  <table>
  <tr><th>    Source                         </th><th>    Destination       </th></tr>
  <tr><td> @c PVM_VpPul1Ampl.ppow            </td><td><c> ACQ_RfShapes[50].powerWatt</c></td></tr>
  <tr><td> @c PVM_VpPul2Ampl.ppow            </td><td><c> ACQ_RfShapes[51].powerWatt</c></td></tr>
  <tr><td> @c PVM_VpPul1.Length              </td><td> @c PVM_WsP0          </td></tr>
  <tr><td> @c PVM_VpPul2.Length              </td><td> @c PVM_WsP1          </td></tr>
  <tr><td><c> PVM_VpSpoilerStrength[][3] </c></td><td><c> PVM_VpTrim[]  </c></td></tr>
  </tr>
  </table>

  @PVM_TBCALL

  @PVM_RELCALL

  @PVM_ERROR
  NONE

  @todo Incomplete documentation: PVM_TBCALL, PVM_PRECOND

  @see @ref STB_InitWsModule, @ref STB_UpdateWsModule

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetWsBaseLevel(void);




/** Sets the baselevel parameters for the outer volume suppression preparation module.

  The pulse gains are set depending on the parameter @c PVM_DeriveGains
  The pulse gains are derived only if the flag  @c PVM_DeriveGains is set to @c Yes.

  @PVM_PARS
  <table>
  <tr><th>    Source                        </th><th>    Destination       </th></tr>
  <tr><td> @c PVM_OvsPulse1Ampl.ppow        </td><td><c> ACQ_RfShapes[53].powerWatt</c></td></tr>
  <tr><td> @c PVM_OvsPulse1.Length          </td><td> @c PVM_OvsP0         </td></tr>
  <tr><td> @c PVM_OvsGradientStrength       </td><td> @c PVM_OvsTrim0      </td></tr>
  <tr><td><c> PVM_OvsSpoilerStrength[0] </c></td><td> @c PVM_OvsTrim1      </td></tr>
  <tr><td><c> PVM_OvsSpoilerStrength[1] </c></td><td> @c PVM_OvsTrim2      </td></tr>
  <tr><td><c> PVM_OvsSpoilerStrength[2] </c></td><td> @c PVM_OvsTrim3      </td></tr>
  <tr><td> @c PVM_RiseTime                  </td><td> @c PVM_OvsD0         </td></tr>
  <tr><td> @c PVM_OvsSpoilerDuration        </td><td> @c PVM_OvsD1         </td></tr>
  </tr>
  </table>

  @PVM_TBCALL

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_InitOvsModule, @ref STB_UpdateOvsModule

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetOvsBaseLevel(void);




/** Sets the baselevel parameters for the black blood preparation module.

  @PVM_PARS
  - @c PVM_BlBloodPul
  - @c ACQ_RfShapes[46]
  - @c PVM_BlbP0
  - @c PVM_BlbGrad

  @PVM_TBCALL
  - @ref ATB_SetRFPulse
  - @ref CFG_AmplifierEnable
  - @ref CFG_GradientRiseTime

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The module must have been updated with @ref STB_UpdateBlBloodModule.

  @PVM_ERROR
  NONE

  @see @ref STB_InitBlBloodModule, @ref STB_UpdateBlBloodModule

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetBlBloodBaseLevel(void);




/** Sets the baselevel parameters for the slice selective inversion preparation module.

  @PVM_PARS
  - @c PVM_SelIrPul
  - @c ACQ_RfShapes[42]
  - @c PVM_SelIrP0

  @PVM_TBCALL
  - @ref ATB_SetRFPulse
  - @ref CFG_GradientRiseTime
  - @ref CFG_AmplifierEnable
  - @ref GTB_OrderedFreqOffsetList

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The module must have been updated with @ref STB_UpdateSelIrModule.

  @PVM_ERROR
  NONE

  @param invSlabNumber    Number of inversion slices

  @see @ref STB_InitSelIrModule, @ref STB_UpdateSelIrModule

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetSelIrBaseLevel(int invSlabNumber);




/** Sets the baselevel parameters for the decoupling preparation module.

  @PVM_PARS
  <table>
  <tr><th>    Source                      </th><th>    Destination      </th></tr>
  <tr><td> @c PVM_DecPulseElementDuration </td><td><c> PCPD[1] </c>     </td></tr>
  <tr><td> @c PVM_DecTime                 </td><td> @c PVM_DecD0        </td></tr>
  <tr><td> @c PVM_DecDelay                </td><td> @c PVM_DecD1        </td></tr>
  <tr><td> @c CPDPRG2                     </td><td> @c PVM_DecPulseEnum </td></tr>
  </tr>
  </table>

  @PVM_TBCALL
  - @ref ATB_SetAcqO2List
  - @ref ATB_SetNuc2

  @PVM_RELCALL
  - @c O2
  - @c PCPD

  @PVM_ERROR
  NONE

  @todo Incomplete documentation: PVM_PRECOND

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetDecBaseLevel(void);




/** Sets the baselevel parameters for the nuclear overhauser enhancement preparation module.

  cpd pulses are used.

  @PVM_PARS
  The frequency and routing parameters are set for the decoupling channel.
  <table>
  <tr><th>    Source                      </th><th>    Destination </th></tr>
  <tr><td> @c PVM_NoePulseEnum            </td><td> @c CPDPRG1     </td></tr>
  <tr><td> @c PVM_NoePulseElementDuration </td><td><c> PCPD[0] </c></td></tr>
  <tr><td> @c PVM_NoePower                </td><td><c> PL[1]   </c></td></tr>
  <tr><td> @c PVM_NoeTime                 </td><td> @c PVM_NoeD0   </td></tr>
  <tr><td> @c PVM_NoeDelay                </td><td> @c PVM_NoeD1   </td></tr>
  </tr>
  </table>

  @PVM_TBCALL
  - @ref ATB_SetAcqO2List
  - @ref ATB_SetNuc2

  @PVM_RELCALL
  - @c O2
  - @c PL
  - @c PCPD

  @PVM_ERROR
  NONE

  @todo Incomplete documentation: PVM_PRECOND

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetNoeBaseLevel(void);




/** Sets the baselevel parameters for the tagging preparation module.

  @PVM_PARS
  - @c PVM_TaggingPul
  - @c ACQ_RfShapes[45]
  - @c PVM_TaggingP0

  @PVM_TBCALL
  - @ref ATB_SetRFPulse
  - @ref CFG_AmplifierEnable
  - @ref CFG_GradientRiseTime

  @PVM_RELCALL
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_InitTaggingModule, @ref STB_UpdateTaggingModule

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetTaggingBaseLevel(void);




/** Sets the baselevel parameter @c ACQ_time_points for the @em Evolution module.

  @c ACQ_time_points is an info parameter providing the time for each repetition
  and evolution cycle.

  @PVM_PARS
  - @c ACQ_time_points

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Parameters of @em Evolution module must be up-to-date.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  ATB_EvolutionSetTimePoints(PVM_NRepetitions, OneRepTime);
  @endcode

  @param nRep        number of repetitions
  @param oneRepTime  duration of one repetition [s]

  @see @ref STB_InitEvolutionModule, @ref STB_UpdateEvolutionModule

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_EvolutionSetTimePoints(int nRep, double oneRepTime);




/** Sets the baselevel parameters for the <em>CASL</em> preparation module.

  @PVM_PARS
  - @c PVM_CaslRFPul
  - @c ACQ_RfShapes[54]
  - @c PVM_CaslD0
  - @c PVM_CaslD1
  - @c PVM_CaslD2
  - @c PVM_CaslD3

  @PVM_TBCALL
  - @ref ATB_SetRFPulse
  - @ref ATB_SetNuc2

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref STB_InitCaslModule, @ref STB_UpdateCaslModule

  @ingroup PVM_ATB

 */
DLL_INTERFACE void ATB_SetCaslModuleBaseLevel(void);

/** Sets the baselevel reconstruction parameters for the <em>CASL</em> preparation module.

  @PVM_PARS

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
  int dim = PTB_GetSpatDim();
  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  int nr = PVM_NRepetitions;

  ATB_SetCaslRecoParam(dim, nSlices, nr);
  @endcode

  @param dim        Number of dimensions
  @param nSlices    Number of slices
  @param nr         Number of repetitions

  @see @ref STB_InitCaslModule, @ref STB_UpdateCaslModule,@ ref ATB_SetCaslModuleBaseLevel

  @ingroup PVM_ATB

 */
DLL_INTERFACE void ATB_SetCaslRecoParam(int dim, int nSlices, int nr);



/** Sets the baselevel parameters for the <em> Navigator </em> module.
  This function is deprecated: since PV7 the job index handling is performed within the module.

  @PVM_USEINSTEAD{ATB_SetNavBaseLevel}

  @PVM_DEACT
  20170101

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE void ATB_SetNavModuleBaseLevel(int, int, int, int);



/** Sets the baselevel parameters for the  <em> Navigator </em>  module.

  This toolbox sets the baselevel parameters for the navigator acquisition. The first argument
  requires a job parameter space opened for access. Preferably, this function should
  be used  within function SetAcquisitionParameters() where jobs parameter space is accessible.
  There, jobs parameters are assigned to the ACQ_jobs-struct.
  The navigator job receives a job title "Navigator": <em> ACQ_jobs[x].title="Navigator" </em>.
  The job number <em> x </em> is not constant but is negotiated anew with each call of backbone.
  It depends on the active- or inactive-status of other acquisitions of the protocol in use.
  It is possible to omit the last three arguments and to specifiy scan size, dummy scans and
  transaction blocks via appendLoop function calls for this job.

  @PVM_PARS
  - Members of the @c Navigator parameter group.

  @PVM_TBCALL
  - @ref CFG_GradientRiseTime
  - @ref CFG_RFPulsesMaxNumberOfPulses
  - @ref ATB_SetRFPulse

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - STB_InitNavModule(), STB_UpdateNavModule() called.
  - Job parameter space opened.

  @PVM_ERROR
  - Input parameters are not set correctly.
  - No accses to jobs parameter space.
  - Several jobs parameter spaces opened in a nested manner.

  @PVM_EXAMPLE
  @code
  void SetAcquisitionParameters(void)
  { ...
    JOBPARAMETERS(jobParameters);  //access to job parameter space
                                 //closed when leaving scope
    ...
    int nTotalScans = NA*NR;
    int nTrnsActBl=1;
    ...

    ATB_SetNavBaseLevel(jobParameters, nTrnsActBl, nTotalScans, PVM_DummyScans);
  }
  @endcode

  @param jobParameters Pointer to jobs parameter space.
  @param tpidx         Index used for the RF excitation pulse structure: ACQ_RfShapes[tpidx]
  @param transActBl    Number of scans which should be visible in the Acq display, default 1
  @param nTotal        Total number of scans, default 1
  @param nDummy        Number of dummy scans, default 0

  @return @c pointer ReadParx::JobDescription* to the job created by the call.

  @see @ref STB_InitNavModule, @ref STB_UpdateNavModule, @ref ATB_LinkJobToDriftCompModule

  @ingroup PVM_ATB
 */
DLL_INTERFACE ReadParx::JobDescription* ATB_SetNavBaseLevel( ReadParx::JobParameters* jobParameters,
                                                             const int tpidx,
                                                             const int transActBlk = 1,
                                                             const int nTotal = 1,
                                                             const int nDummies = 0
                                                           );



/** Sets the baselevel parameters for the <em> Drift Compensation </em> module.

  This toolbox sets the baselevel parameters for the drift compensation acquisition and post processing.
  The first argument requires a job parameter space opened for access. Preferably, this function should
  be used  within function SetAcquisitionParameters() where jobs parameter space is accessible.
  There jobs parameters are assigned to the ACQ_jobs-struct.
  The job is named "DriftCompensation": <em> ACQ_jobs[x].title="DriftCompensation" <em>. The job index
  <em> x <em> is not constant but is negotiated anew at each call of backbone. It depends on the
  active- or inactive-status of other acquisitions of the method. The job index is handed to parameter
  ACQ_DriftCompJob to identify the dataset to be analyzed for compensation.
  It is possible to omit the last three arguments and to specifiy scan size, dummy scans and
  transaction blocks via appendLoop function calls for this job.

  @PVM_PARS
  -    @c ACQ_DriftCompActive      -  de-/activates the drift compensation functionality in
                                      reconstruction pipeline.
  -    @c ACQ_DriftCompJob         -  acquisition job index.
  -    @c ACQ_DriftCompFilterWidth -  number of scans used for a B0 offset calculation.
  -    @c ACQ_DriftCompStep        -  number of scans skipped after a B0 offsett calculation.
  -    @c ACQ_DriftCompOffset      -  frequency offset which is transfered to the reload B0 register.
  -    @c ACQ_DriftCompDebug       -  de-/activates the generation of an output file for debugging.
  -    @c ACQ_Jobs[jobidx]...      -  jobs parameters for intrinsic navigator.
  -    @c ACQ_RfShapes[55]


  @PVM_TBCALL
  - @ref CFG_GradientRiseTime
  - @ref ATB_SetRFPulse


  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - STB_InitDriftComp(), STB_UpdateDriftComp() called.
  - Job parameter space opened.

  @PVM_ERROR
  - Input parameters are not set correctly.
  - No access to jobs parameter space.
  - Several jobs parameter spaces opened in a nested manner.

  @PVM_EXAMPLE
  @code
  void SetAcquisitionParameters(void)
  { ...
    JOBPARAMETERS(jobParameters);  //access to job parameter space
                                   //closed when leaving scope
    int spatDim = PTB_GetSpatDim();
    ...
    int nTotal = NR * NAE * NA;

    for (int i = 1; i < spatDim; i++)
      nTotal *= PVM_EncMatrix[i];
    ...

    ATB_SetDriftCompBaseLevel(jobParameters, 1, nTotal, PVM_DummyScans);
  }
  @endcode

  @param jobParameters Pointer to a jobs parameter space.
  @param transActBl	   Number of scans which should be visible in the Acq display, default 1
  @param nTotal	       Total number of scans. Depends on position in method's ppg.
  @param nDummy	       Number of dummy scans. Depends on position in method's ppg.

  @return @c ReadParx::JobDescription* - pointer to the job created by the call.

  @see @ref STB_InitDriftComp, @ref STB_UpdateDriftComp, @ref STB_UpdateDriftCompModule,

  @ingroup PVM_ATB
 */
DLL_INTERFACE ReadParx::JobDescription* ATB_SetDriftCompBaseLevel( ReadParx::JobParameters* jobParameters,
                                                                   const int transActBlk = 0,
                                                                   const int nTotal = 0,
                                                                   const int nDummies = 0
                                                                 );


/** Sets the baselevel parameters for the <em> Drift Compensation </em> module.
  This function is outdated. Since PV7 job index handling is performed within the module.
  Use ATB_SetDriftCompBaseLevel with new argument list instead.

  @PVM_USEINSTEAD{ATB_SetDriftCompBaseLevel}

  @PVM_DEACT
  20170101

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE void ATB_SetDriftCompBaseLevel(int jobidx, int tranActBl=0, int nTotal=0, int nDummy=0);



/** Links a job to  <em> Drift Compensation </em> post-processing.

  The <em> Drift Compensation </em> module can be used in two different ways: either it generates its
  data required for frequency analysis by its own (via flag PVM_DriftCompNavOnOff=On, set in
  STB_InitDriftComp) or it uses data supplied by a job tat is set up indepentently. In the latter case
  it is necessary to inform the </em> Drift Compensation </em> post-processing which job data to use.@n
  The first argument requires a job parameter space opened for access. Preferably, this function should
  be used within function SetAcquisitionParameters() where jobs parameter space is accessible.

  @PVM_PARS
  -    @c ACQ_DriftCompActive      -  de-/activates the drift compensation functionality in
                                      reconstruction pipeline.
  -    @c ACQ_DriftCompJob         -  acquisition job index.
  -    @c ACQ_DriftCompFilterWidth -  number of scans used for a B0 offset calculation.
  -    @c ACQ_DriftCompStep        -  number of scans skipped after a B0 offsett calculation.
  -    @c ACQ_DriftCompOffset      -  frequency offset which is transfered to the reload B0 register.
  -    @c ACQ_DriftCompDebug       -  de-/activates the generation of an output file for debugging.

  @PVM_TBCALL

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - STB_InitDriftComp() called with flag PVM_DriftCompNavOnOff=Off (default).
  - STB_UpdateDriftComp called with jobs repeat time.

  @PVM_ERROR
  - Input parameters are not set correctly.
  - No access to jobs parameter space.
  - Nested job parameter spaces opened.


  @PVM_EXAMPLE
  @code
  void SetAcquisitionParameters(void)
  {
    JOBPARAMETERS(jobParameters);  //access to job parameter space
                                   //closed when leaving scope
    JOBDESCRIPTION job = jobParameters->getOrCreateJob("myJob");
    job->initPars(1,128,50000);
    ...

    ATB_LinkJobToDriftCompModule( jobParameters, job );
    ...
  }
  @endcode

  @param  ReadParx::JobDescription* - pointer to a jobs parameter space.
  @param  ReadParx::JobDescription* - pointer to a job

  @see @ref STB_InitDriftComp, @ref STB_UpdateDriftComp,  @ref STB_UpdateDriftCompModule, @ref ATB_SetDriftCompBaseLevel

  @ingroup PVM_ATB
 */
DLL_INTERFACE void ATB_LinkJobToDriftCompModule(ReadParx::JobParameters* jobParameters, ReadParx::JobDescription* job);



/** Sets the baselevel parameters for the fair preparation module.

  @PVM_PARS
  - @c PVM_FairInvPul
  - @c ACQ_RfShapes[47]
  - @c PVM_FairInvRfLength
  - @c PVM_FairD1
  - @c PVM_FairD2
  - @c PVM_FairD3
  - @c PVM_FairSpoilGrad

  @PVM_TBCALL
  - @ref ATB_SetRFPulse
  - @ref CFG_AmplifierEnable
  - @ref CFG_GradientRiseTime

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The module must have been updated with @ref STB_UpdateFairModule.

  @PVM_ERROR
  NONE

  @see @ref STB_InitFairModule, @ref STB_UpdateFairModule

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetFairBaseLevel(void);




/** Initializes the pulseprogram parameters of modules.

  @c PVM parameters used in the ppg of the different preparation module:
  Delays, pulse durations loops and gradient strength are initialized to nonzero values.

  @PVM_TBCALL

  @PVM_RELCALL

  @PVM_ERROR
  NONE

  @todo Incomplete documentation: PVM_PARS, PVM_TBCALL, PVM_RELCALL, PVM_PRECOND

  @ingroup PVM_ATB_INT
*/
DLL_INTERFACE void ATB_InitPrepModuleAcqPars(void);




/* /pv60/gen/src/prg/pvm_toolboxes/PvAcqTools/epiAcq.c */

/* range checkers and relations */
DLL_INTERFACE void ATB_EpiGhostCounter(void);
DLL_INTERFACE void ATB_EpiGrappaCounter(void);
DLL_INTERFACE void ATB_EpiTrajCounter( void );

/** Sets the baselevel parameters dependent on the @c EPI_Parameters group.

  This function should be called in the baselevel setting code of methods which use
  the @em epi.mod module. It derives @c ACQP and @c PVM parameters used in the
  @em epi.mod module from the @c EPI_Parameters group. It should be called after all
  other @c ACQP parameters have been set.

  @PVM_PARS
  - @c DEOSC
  - @c ACQ_RareFactor
  - @c ACQ_PhaseFactor
  - @c ACQ_ScanShift
  - @c ACQ_scan_size
  - @c PVM_EpiD0 ... @c PVM_EpiD10
  - @c PVM_EpiVdList
  - @c EpiShape1 ... @c EpiShape7

  @PVM_TBCALL
  - @ref MRT_MakeRamp

  @PVM_RELCALL
  - @c ACQ_ScanShift
  - @c ACQ_phase_factor
  - @c ACQ_rare_factor

  @PVM_PRECOND
  - The function @ref STB_EpiUpdate must be called before this function.

  @PVM_ERROR
  NONE

  @see @ref STB_InitEpi, @ref STB_EpiUpdate,
       @ref ATB_EpiSetRecoProcess, @ref ATB_EpiSetRgAndGhostAdj, @ref ATB_EpiSetGrappaAdj

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_EpiSetBaseLevel(void);




/** Sets reconstruction process for @em EPI.

  This function should be called in the relations of @c RecoUserUpdate.

  @PVM_PARS
  - @c RecoGrappaIncludeRefLines (set to @c No)
  - @c RecoGrappaReadCenter
  - @c PVM_EpiGrappaCoefficients (resized)

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - May be called ONLY as specified above.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  ATB_EpiSetRecoProcess(PVM_EncNReceivers,
                        PVM_EncCentralStep1,
                        PVM_NEchoImages);
  @endcode

  @param nReceivers       Number of active receive channels
  @param refEcho          Number of echo (counting from @c 1) going through k-space origin
  @param nEchoImages      Number of echo images (successive acquisition as in T2_EPI/T2S_EPI)
  @param innerSegments    (optional) Number of segments measured per excitation
                          - allows for a segment loop within a slice loop
                          - default = @c 1

  @see @ref STB_InitEpi, @ref STB_EpiUpdate, @ref ATB_EpiSetBaseLevel,
       @ref ATB_EpiSetRgAndGhostAdj, @ref ATB_EpiSetGrappaAdj

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_EpiSetRecoProcess(int nReceivers,
                           int refEcho,
                           int nEchoImages,
                           int innerSegments = 1);




/* old version: supported but not documented */
DLL_INTERFACE void ATB_EpiSetRecoProcess(int nReceivers,
                           int echoAcqSize,
                           int echoFtSize,
                           int refEcho,
                           int nEchoImages,
                           int innerSegments = 1);




/** Sets RG- and ghost adjustment for @em EPI.

  This function should be called in the relations of @c PVM_AdjHandler
  when the RCVR adjustment is starting.

  @PVM_PARS
  - @c PVM_EpiAdjustmentMode (set to @c 1)
  - @c PVM_EpiEchoTimeShifting (set to @c No)
  - @c ACQ_SetupAutoName

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - May be called ONLY as specified above.

  @PVM_ERROR
  NONE

  @see @ref STB_InitEpi, @ref STB_EpiUpdate, @ref ATB_EpiSetBaseLevel,
       @ref ATB_EpiSetRecoProcess, @ref ATB_EpiSetGrappaAdj

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_EpiSetRgAndGhostAdj(void);




/** Sets GRAPPA adjustment for @em EPI.

  This function should be called in the relations of @c PVM_AdjHandler
  when the GRAPPA adjustment is starting.

  @PVM_PARS
  - @c PVM_EpiAdjustmentMode (set to @c 2)
  - @c ACQ_SetupAutoName

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - May be called ONLY as specified above.

  @PVM_ERROR
  NONE

  @see @ref STB_InitEpi, @ref STB_EpiUpdate, @ref ATB_EpiSetBaseLevel,
       @ref ATB_EpiSetRecoProcess, @ref ATB_EpiSetRgAndGhostAdj

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_EpiSetGrappaAdj(void);




/* /pv60/gen/src/prg/pvm_toolboxes/PvAcqTools/SpiralAcq.c */

/** Sets reconstruction parameters derived from the @c SpiralParameters group.

  This function should be called from the backbone of methods which use the @em Spiral module.
  It derives @c RECO parameters from the @c SpiralParameters group.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref ATB_InitUserModeReco
  - @ref ATB_SetRecoRegridN
  - @ref ATB_SetRecoRotate
  - @ref ATB_SetRecoPhaseCorr
  - @ref ATB_SetRecoTranspositionFromLoops

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Geometry parameters have been updated.
  - Encoding group has been updated.
  - @em Spiral module has been updated by @ref STB_UpdateSpiral.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  char trajFile[PATH_MAX];
  PvOvlUtilGetExpnoPath(trajFile, PATH_MAX, "traj");

  if (PVM_TrajRecoMode != Traj_Measured)
  {
    // theoretical trajectory for both reconstruction and DC
    ATB_SpiralSetRecoPars(trajFile);
  }
  else
  {
    // measured trajectory for reconstruction and theoretical trajectory for DC
    char trajDCFile[PATH_MAX];
    PvOvlUtilGetExpnoPath(trajDCFile, PATH_MAX, "trajDC");
    ATB_SpiralSetRecoPars(trajFile, trajDCFile);
  }
  @endcode

  @param[in] trajFile    Path + name of file containing trajectory data used for reconstruction
  @param[in] trajDCFile  Path + name of file containing trajectory data used for density correction
                         (@c NULL = use @a traFile)

  @see @ref STB_InitSpiral, @ref STB_UpdateSpiral, @ref STB_SpiralUpdateGeometry,
       @ref STB_SpiralCreateShapes, @ref STB_SpiralTrajectory, @ref ATB_SpiralSetupReco

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SpiralSetRecoPars(const char *trajFile,
                                         const char *trajDCFile = NULL);




/** Setup reconstruction network for the @em Spiral module.

  This function should be called for setting up the reconstruction network in methods which
  use the @em Spiral module.
  It creates a standard reconstruction network with the following extras:
  - Navigator handling
  - B0 eddy current correction

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref ATB_TrajInsertB0Correction

  @PVM_RELCALL
  - @c RecoUserUpdate

  @PVM_PRECOND
  - Encoding group has been updated.
  - @em Spiral module has been updated by @ref STB_UpdateSpiral.
  - Reconstruction parameters have been set by @ref ATB_SpiralSetRecoPars.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  int nPre    = PVM_SpiralPreSize;
  int nSpiral = PVM_SpiralSize;
  int nPost   = PVM_SpiralPostSize;
  char b0File[PATH_MAX];

  PvOvlUtilGetExpnoPath(b0File, PATH_MAX, "b0");

  // measured trajectory for reconstruction
  const int maxGradDelay = (int)(0.3 / PVM_DigDw); // [samples]

  STB_TrajCreateTrajectory(
         RecoRegridNTrajFile, b0File,
         &nPre, &nSpiral, &nPost,
         maxGradDelay, PVM_SpiralPointsPerRotation);

  // theoretical trajectory for DC
  STB_SpiralTrajectory(RecoRegridNTrajFileDC, NULL, NULL, 0, nSpiral);

  // create network
  ATB_SpiralSetupReco(nPre, nSpiral, nPost, b0File);
  @endcode

  @param     nPre    Number of samples per interleave before range used for reconstruction
                     (after navigator) (@c 0 = @c PVM_SpiralPreSize)
  @param     nSpiral Number of samples per interleave used for reconstruction
                     (@c 0 = @c PVM_SpiralSize)
  @param     nPost   Number of samples per interleave to be skipped at the end
                     (@c 0 = @c PVM_SpiralPostSize)
  @param[in] b0File  Path + name of file containing B0 correction data
                     (@c NULL = no B0 correction)

  @see @ref STB_InitSpiral, @ref STB_UpdateSpiral, @ref STB_SpiralUpdateGeometry,
       @ref STB_SpiralCreateShapes, @ref STB_SpiralTrajectory, @ref ATB_SpiralSetRecoPars

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SpiralSetupReco(int  nPre    = 0,
                                       int  nSpiral = 0,
                                       int  nPost   = 0,
                                       char *b0File = NULL);



/* /pv60/gen/src/prg/pvm_toolboxes/PvAcqTools/TrajectoryAcq.c */

/** Setup reconstruction network for trajectory measurement as an adjustment controlled by
  the @c TrajParameters group.

  This function should be called in methods using the @em Trajectory module to set up the
  reconstruction network.<br>
  The calculated trajectories will be shown in the RecoDisplay.<br>
  Note that in the obtained trajectory data the scan shift is removed.
  Hence, when using them for reconstruction the corresponding measured data
  must be handled similarly, e.g. by setting <c> ACQ_scan_shift = -1 </c> in the method.

  @PVM_PARS
  - <c>PVM_TrajK?</c>
  - <c>PVM_TrajB?</c>

  @PVM_TBCALL
  - @ref PTB_MaxArraySize

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The @c TrajParameters group must be up-to-date.

  @PVM_ERROR
  NONE

  @return @c Yes = trajectory adjustment is running and reco was set up<br>
          @c No = trajectory adjustment is not running

  @see @ref STB_InitTraj, @ref STB_UpdateTraj, @ref STB_TrajAppendAdjustment,
       @ref STB_TrajHandleAdjustmentRequests, @ref STB_TrajCreateTrajectory,
       @ref ATB_TrajInsertB0Correction, @ref ATB_TrajSetAdjustmentStatus,
       @ref ATB_TrajHandleAdjustmentResults, @ref ATB_TrajUpToDate

  @ingroup PVM_ATB
*/
DLL_INTERFACE YesNo ATB_TrajSetupReco(void);




/** Handles adjustment iterations for trajectory measurement as an adjustment controlled
  by the @c TrajParameters group.

  This function is called implicitly for methods using the @em Trajectory module during each
  trajectory adjustment iteration.

  If <c>PVM_TrajDebug = Yes</c> the following debug output in ASCII format is created in
  a subdirectory of <c>PVHOME/prog/curdir/USER/ParaVision</c>:<br>
  The files <c>trajK?</c> and <c>trajB?</c> contain the unprocessed measured trajectory
  and B0 components for the different dimensions indicated by @c x, @c y, and @c z,
  including all acquired samples. The corresponding data transferred to the RPS coordinate
  system are indicated with  @c r, @c p, and @c s.

  @PVM_PARS
  - @c PVM_TrajAdjustmentCounter
  - @c PVM_TrajUpToDate
  - Baselevel parameters
  - Reconstruction parameters

  @PVM_TBCALL
  - @ref ATB_AdjCheckRg
  - @ref UT_WriteDoubleArrayToDebugFile

  @PVM_RELCALL
  - @c PVM_AutoRgInitHandler

  @PVM_PRECOND
  - The @c TrajParameters group must be up-to-date.

  @PVM_ERROR
  NONE

  @see @ref STB_InitTraj, @ref STB_UpdateTraj, @ref STB_TrajAppendAdjustment,
       @ref STB_TrajHandleAdjustmentRequests, @ref STB_TrajCreateTrajectory,
       @ref ATB_TrajSetupReco, @ref ATB_TrajInsertB0Correction,
       @ref ATB_TrajSetAdjustmentStatus, @ref ATB_TrajHandleAdjustmentResults,
       @ref ATB_TrajUpToDate

  @ingroup PVM_ATB_INT
*/
DLL_INTERFACE void ATB_TrajAdjustmentCounterRels(void);




/** Check if a trajectory is available for the current parameter context (part of the @c TrajParameters group).

  This function checks if for all adjusted parameters values are available with the current parameter context.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @c PVM_TrajAdjContext must have been set by calling @ref STB_TrajAppendAdjustment.

  @PVM_ERROR
  NONE

  @return @c Yes = trajectory is available, else @c No.

  @see @ref STB_InitTraj, @ref STB_UpdateTraj, @ref STB_TrajAppendAdjustment,
       @ref STB_TrajHandleAdjustmentRequests, @ref STB_TrajCreateTrajectory,
       @ref ATB_TrajSetupReco, @ref ATB_TrajInsertB0Correction,
       @ref ATB_TrajSetAdjustmentStatus, @ref ATB_TrajHandleAdjustmentResults,
       @ref ATB_TrajUpToDate

  @ingroup PVM_ATB
*/
DLL_INTERFACE YesNo ATB_TrajUpToDate(void);




/** Handle adjustment results of a trajectory measurement controlled by the @c TrajParameters group.

  This function serves to transfer the status of the latest trajectory adjustment to the method.
  The relation of @c PVM_AdjResultHandler must be redirected to
  - this function for methods without further method specific adjustments
  - the local adjustment result handler for methods with other method-specific adjustments; @n
    in this case, @ref ATB_TrajHandleAdjustmentResults ist called first and other adjustments are handled afterwards; @n
    if required, the method's @c backbone may also be called.

  @PVM_PARS
  - @c PVM_TrajAdjStatus
  - @c PVM_TrajAdjStatusAdj

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The @c TrajParameters group must be up-to-date.

  @PVM_ERROR
  NONE

  @return @c Yes = trajectory adjustment result was handled, else @c No.

  @see @ref STB_InitTraj, @ref STB_UpdateTraj, @ref STB_TrajAppendAdjustment,
       @ref STB_TrajHandleAdjustmentRequests, @ref STB_TrajCreateTrajectory,
       @ref ATB_TrajSetupReco, @ref ATB_TrajInsertB0Correction,
       @ref ATB_TrajSetAdjustmentStatus, @ref ATB_TrajUpToDate

  @ingroup PVM_ATB
*/
DLL_INTERFACE YesNo ATB_TrajHandleAdjustmentResults(void);




/** Set status of a trajectory measurement performed as an adjustment controlled
  by the @c TrajParameters group.

  This function should be called if the trajectory result is processed or checked in
  a redirected relation of @c PVM_TrajAdjustmentCounter and the result is either
  unsatisfactory or replaced by theoretical values.

  @PVM_PARS
  - @c PVM_TrajUpToDate
  - @c PVM_TrajAdjustmentCounter
  - @c PVM_TrajAdjStatusAdj
  - @c AdjErrorComment

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The @c TrajParameters group must be up-to-date.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  ATB_TrajSetAdjustmentStatus(0, "Data is not valid");
  @endcode

  @param     status     status to be set
                        - @c 0 = failure
                        - @c 1 = repaired (result parameters were filled with theoretical values)
  @param[in] errorText  text added to the general error message
                        - only used for @a status = @c 0
                        - @c NULL = generic message (default)

  @see @ref STB_InitTraj, @ref STB_UpdateTraj, @ref STB_TrajAppendAdjustment,
       @ref STB_TrajHandleAdjustmentRequests, @ref STB_TrajCreateTrajectory,
       @ref ATB_TrajSetupReco, @ref ATB_TrajInsertB0Correction,
       @ref ATB_TrajHandleAdjustmentResults, @ref ATB_TrajUpToDate

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_TrajSetAdjustmentStatus(int status = 0,
                                               const char *errorText = NULL);




/** Inserts B0 correction into reconstruction network based on trajectory measurement
  controlled by the @c TrajParameters group.

  This function should be called in methods using the @em Trajectory module to apply
  the B0 eddy current correction.<br>
  The required filter group with prefix @em TRAJ_B0CORR will be inserted at the end of
  the @em FIRSTPASS.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The @c TrajParameters group must be up-to-date.

  @PVM_ERROR
  - Incorrect input parameters.

  @PVM_EXAMPLE
  @code
  ATB_TrajInsertB0Correction("/tmp/b0", Spiral, RecoRegridNDim);
  @endcode

  @param[in] fileName    Name of file with complex multiplication factors
                         - as created by @ref STB_TrajCreateTrajectory
                         - will be reproduced internally for every image (@c NI times)
                           and input size of 3rd dimension in case @a dimReco < @c 3
                         - will be repeated internally for each repetition (@c NR times)
  @param     nSamples    Number of used samples per interleave (excluding pre- and post-samples)
  @param     dimReco     Dimensions reconstructed from data

  @see @ref STB_InitTraj, @ref STB_UpdateTraj, @ref STB_TrajAppendAdjustment,
       @ref STB_TrajHandleAdjustmentRequests, @ref STB_TrajCreateTrajectory,
       @ref ATB_TrajSetupReco, @ref ATB_TrajSetAdjustmentStatus,
       @ref ATB_TrajHandleAdjustmentResults, @ref ATB_TrajUpToDate

  @ingroup PVM_ATB

  @todo For 3D and @c NI > 1
        - Check @c procDim of COMBINE filter.
        - Does currently not occur with SPIRAL, UTE, or UTE3D methods.
        - What about EPI?
*/
DLL_INTERFACE void ATB_TrajInsertB0Correction( const char         *fileName,
                                               const unsigned int  nSamples,
                                               unsigned int        dimReco);




/* gen/src/prg/pvm_toolboxes/mr/PvAcqTools/utilities.c */

/** Sets the @c ACQ_method parameter to a string representation of method.

  @PVM_PARS
  - @c ACQ_method

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

 *
 * @deprecated Automatically set when loading the method.

  @ingroup PVM_ATB
*/
#define ATB_SetAcqMethod()




/** Completely defines @c ACQ_trim_file, @c ACQ_n_trim, and @c ACQ_trim.

  @PVM_PARS
  - @c ACQ_n_trim

  @PVM_TBCALL
  - @ref ATB_SetAcqTrim

  @PVM_RELCALL
  - @c ACQ_n_trim

  @PVM_PRECOND
  - @a numTrims must equal the number of elements in the variable argument list.

  @PVM_ERROR
  - @a numTrims < @c 1

  @PVM_EXAMPLE
  @code
  ATB_SetAcqTrims( 5,
                   ExcSliceGrad,                   // t0
                  -ExcSliceRephGrad,               // t1
                  -ReadDephGrad,                   // t2
                   ReadGrad,                       // t3
                   ReadSpoilGrad);                 // t4
  @endcode

  @param numTrims    Number of trim values to be set
  @param ...         Comma separated list of trim values

  @see @ref ATB_SetAcqTrim

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetAcqTrims( int numTrims, ... );




/** Sets an element of the @c ACQ_trim array.

  @PVM_PARS
  - @c ACQ_trim

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Size of @c ACQ_trim must be at least @a trimIndex + 1.

  @PVM_ERROR
  - @a trimIndex < @c 0
  - @a trimIndex >= size of @c ACQ_trim

  @PVM_EXAMPLE
  @code
  ATB_SetAcqTrim(0, ExcSliceGrad, 1.0, 1.0, 1.0);
  @endcode

  @param trimIndex     Index of trim value in the @c ACQ_trim array
  @param trimValue     Trim value expressed as a percentage of the gradient calibration constant
  @param xScale        Scaling factor for the x-trim value
  @param yScale        Scaling factor for the y-trim value
  @param zScale        Scaling factor for the z-trim value

  @see @ref ATB_SetAcqTrims

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetAcqTrim(int    trimIndex,
                                  double trimValue,
                                  double xScale,
                                  double yScale,
                                  double zScale);




/** Sets the @c ACQ_dim_desc @c ACQP parameter.

  @c ACQ_dim_desc is an array of size @c ACQ_dim whose entries take the values @c Spectroscopic
  or @c Spatial. This secondary relation has 2 modes of operation dependent on the value
  of @a specIndices:
  - If @a specIndices is a @c NULL pointer: The indices zero to @a specDim-1 will be set
    to the value @c Spectroscopic with the remainder set to @c Spatial.
  - If @a specIndices is an array: The elements of @c ACQ_dim_desc
    referred to in @a specIndices will be set to @c Spectroscopic, and the remainders
    are set to @c Spatial.

  @PVM_PARS
  - @c ACQ_dim_desc

  @PVM_TBCALL
  - @ref UT_MinOfIntArray
  - @ref UT_MaxOfIntArray
  - @ref UT_UniqueIntArray

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a specIndices must be either a @c NULL pointer or an array of size @a specDim.

  @PVM_ERROR
  - @c ACQ_dim is not initialized.
  - @a specDim < @c 0
  - @a spatDim < @c 0
  - @a specDim + @a spatDim < @c 1
  - @a specDim + @a spatDim != @c ACQ_dim
  - @c ACQ_dim != size of @c ACQ_dim_desc
  - Entry %d of @a specIndices is less than @c 0.
  - Entry %d of @a specIndices is greater than @a specDim + @a spatDim.
  - Some entries @a specIndices in are not unique!

  @PVM_EXAMPLE
  @code
  ATB_SetAcqDimDesc(PTB_GetSpecDim(), PTB_GetSpatDim());
  @endcode

  @param     specDim        Number of spectral dimensions
  @param     spatDim        Number of spatial dimensions
  @param[in] specIndices    Array of size @a specDim containing the indices of @c ACQ_dim_desc
                            to be set to the value @c Spectroscopic or a @c NULL pointer.
			    The values must be unique.

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetAcqDimDesc(int specDim,
                                     int spatDim,
                                     int *specIndices = NULL);




/** Sets the @c ACQ_size @c ACQP parameter.

  This secondary relation sets those elements of @c ACQ_size whose dimension descriptors
  <c> ACQ_dim_desc[i] </c> are identical to @a dimDesc.
  In order to set both spectroscopic and spatial dimensions the function must be called twice.

  @PVM_PARS
  - @c ACQ_size

  @PVM_TBCALL
  - @ref ATB_CountDimsOfType

  @PVM_RELCALL
  - @c ACQ_size

  @PVM_PRECOND
  - @c ACQ_dim_desc must have been set.
  - @a sizes, @a antiAlias must be of size @a numDims.

  @PVM_ERROR
  - @a numDims != number of @a dimDesc entries in @c ACQ_dim_desc
  - @c ACQ_dim != size of @c ACQ_dim_desc
  - @c ACQ_dim != size of @c ACQ_size

  @PVM_EXAMPLE
  @code
  ATB_SetAcqSize(Spatial, PTB_GetSpatDim(), PVM_EncMatrix, NULL, No);
  @endcode

  @param     dimDesc        Indicates that the parsed parameters are either @c Spatial
                            or @c Spectroscopic.
  @param     numDims        Number of dimensions
  @param[in] sizes          Int array of size @a numDims containing the sizes of each dimension
  @param[in] antiAlias      Double array of size @a numDims of oversampling values for each
                            dimension in sizes. Only values greater than @c 1 will be used.
                            If a @c NULL pointer is parsed this parameter is ignored and
                            @a antiAlias are taken to be universally @c 1.0.
  @param     size0IsComplex Is set to @c Yes if @a sizes[0] is already doubled for
                            complex samples.

  @see @ref ATB_SetAcqFov

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetAcqSize(DIM_TYPE dimDesc,
                                  int numDims,
                                  int *sizes,
                                  double *antiAlias,
                                  YesNo size0IsComplex);




/** Determines how many dimensions of the given type are defined in the @c ACQ_dim_desc parameter.

  The parameter @a dimDesc can take the values:
  - @c Spatial       Counts spatial dimensions
  - @c Spectroscopic Counts spectroscopic dimensions

  This function will return @c 0 (zero) in case of
  - @c ACQ_dim is not initialized
  - @c ACQ_dim_desc has no value
  - The size of @c ACQ_dim_desc differs from the value of @c ACQ_dim.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @c ACQ_dim must have a value

  @PVM_ERROR
  - @c ACQ_dim has no value

  @PVM_EXAMPLE
  @code
  ATB_CountDimsOfType(Spectroscopic);
  @endcode

  @param dimDesc    Dimension descriptor to be counted

  @return Number of dimensions

  @ingroup PVM_ATB
*/
DLL_INTERFACE int ATB_CountDimsOfType(DIM_TYPE dimDesc);




/** Determines how many dimensions are defined as @c Spatial in the @c ACQ_dim_desc parameter.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref ATB_CountDimsOfType

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @return Number of dimensions

  @see @ref ATB_CountDimsOfType, @ref ATB_SpecDim

  @ingroup PVM_ATB
*/
DLL_INTERFACE int ATB_SpatDim(void);




/** Determines how many dimensions are defined as @c Spectroscopic in the @c ACQ_dim_desc
  parameter.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref ATB_CountDimsOfType

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @return Number of dimensions

  @see @ref ATB_CountDimsOfType, @ref ATB_SpatDim

  @ingroup PVM_ATB
*/
DLL_INTERFACE int ATB_SpecDim(void);




/** Sets the @c ACQ_fov @c ACQP parameter.

  This secondary relation sets those elements of @c ACQ_fov whose dimension descriptors
  <c> ACQ_dim_desc[i] </c> are identical to @a dimDesc.

  @PVM_PARS
  - @c ACQ_fov

  @PVM_TBCALL
  - @ref ATB_CountDimsOfType

  @PVM_RELCALL
  - @c ACQ_fov

  @PVM_PRECOND
  - @c ACQ_dim must have been set and its relations called.
  - @c ACQ_dim_desc must have been set.
  - @a fov and  @a antiAlias must be of size @a numDims.

  @PVM_ERROR
  - @c ACQ_dim != size of @c ACQ_fov
  - @c ACQ_dim != size of @c ACQ_dim_desc
  - @a numDims != number of @a dimDesc entries in @c ACQ_dim_desc

  @PVM_EXAMPLE
  @code
  ATB_SetAcqFov(Spatial, PTB_GetSpatDim(), PVM_Fov, PVM_AntiAlias);
  @endcode

  @param     dimDesc   Dimension descriptor to be considered
  @param     numDims   Number of element to be set
  @param[in] fov       Double array of size @a numDims of field-of-view (mm)
  @param[in] antiAlias Double array of size @a numDims of anti-alias values to be
                       accounted for in setting @c ACQ_fov

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetAcqFov(DIM_TYPE dimDesc,
                                 int      numDims,
                                 double   *fov,
                                 double   *antiAlias);




/** Sets the @c ACQ_grad_matrix @c ACQP parameters on the basis of the orientation matrices
  defined for the slice packs and the slice order.

  @PVM_PARS
  - @c ACQ_grad_matrix_size
  - @c ACQ_grad_matrix

  @PVM_TBCALL
  - @ref GTB_NumberOfSlices
  - @ref UT_UniqueIntArray
  - @ref UT_CopyOriMatrix

  @PVM_RELCALL
  - @c ACQ_grad_matrix_size

  @PVM_PRECOND
  - The size of @a sPacksNSlices is @a nPacks.
  - @a gradMatrix provides @a nPacks gradient matrices.
  - The size of @a sliceOrder must be the total number of slices in @a sPacksNSlices.

  @PVM_ERROR
  - @a nPacks < @c 1
  - No slices defined in @a sPacksNSlices
  - @a gradMatrix is a @c NULL pointer.
  - @a gradMatrix[0] is a @c NULL pointer.
  - @a sliceOrder is a @c NULL pointer.
  - @a sliceOrder contains non-unique elements.

  @PVM_EXAMPLE
  @code
  ATB_SetAcqGradMatrix(PVM_NSPacks,
                       PVM_SPackArrNSlices,
                       PtrType3x3 PVM_SPackArrGradOrient[0],
                       PVM_ObjOrderList);
  @endcode

  @param     nPacks        Number of slice packs
  @param[in] sPacksNSlices Number of slices per pack
  @param[in] gradMatrix    Gradient matrices for each pack
  @param[in] sliceOrder    Slice order of the slices in all packs

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetAcqGradMatrix(int      nPacks,
                                        int     *sPacksNSlices,
                                        double (*gradMatrix)[3][3],
                                        int     *sliceOrder);

/** Sets the @c ACQ_grad_matrix @c ACQP parameters on the basis of geometry objects.



  @PVM_PARS
  - @c if specified parameter with name @a gmname or @a ACQ_GradientMatrix


  @PVM_TBCALL
  - @ref UT_GetParType
  - PARX parameter utility functions

  @PVM_RELCALL
  - default relation of grad matrix parameter @c gmname

  @PVM_PRECOND
  - The geometry object has to be set correctly before


  @PVM_ERROR
  - @a gobj must be the name of a scalar parameter of type GEO_OBJECT
  - @a if the acquisition order must be considered it has to be defined
       by parameter gobj (see also @ref PTB_InsertGeoObj parameter
       @a geo_order)
  - @a gobj must be set up correctly (see also @ref PTB_InsertGeoObj )
  - Field .par of gobj must be a cuboid array


  @PVM_EXAMPLE
  @code

  // transfers geometry orientation of slices (MRI) to gradient matrix
  // ACQ_grad_matrix
  // ACQ_grad_matrix transforms from PV patient coordinate system into
  // acquisition system
  ATB_SetAcqGradMatrix("PVM_SliceGeoObj","ACQ_grad_matrix",Yes);

  // this call sets ACQ_GradientMatrix
  ATB_SetAcqGradMatrix("PVM_SliceGeoObj");

  // this call set ACQ_GradientMatrix to unit matrix
  ATB_SetAcqGradMatrix(0);

  // this call sets ACQ_grad_matrix to unit matrix
  ATB_SetAcqGradMatrix(0,"ACQ_grad_matrix",No);

  @endcode

  @param      gobj        Name of geometry object parameter if 0 a unit
                          matrix (1 element) is set up
  @param[out] gmname      Optional name of gradient matrix array (default
                          ACQ_GradientMatrix) that is to set
  @param[in]  objmat      Gradient matrix transforms from PV Pat KS
                          into acquisition system (objmat=Yes), otherwise
                          from modality (gradient) into acquisiton
                          coordinate system

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetAcqGradMatrix(const char *const gobj=0,
                                        const char *const gmname=0,
                                        YesNo objmat=No);





/** Sets the @c NSLICES @c ACQP parameter.

  @PVM_PARS
  - @c NSLICES

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c NSLICES

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nSlices < @c 1

  @PVM_EXAMPLE
  @code
  ATB_SetNSlices(GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices));
  @endcode

  @param nSlices    Number of slices

  @see @ref ATB_SetNR, @ref ATB_SetNI, @ref ATB_SetNA, @ref ATB_SetNAE

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetNSlices(int nSlices);




/** Sets the @c NR @c ACQP parameter.

  @PVM_PARS
  - @c NR

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c NR

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nR < @c 1

  @PVM_EXAMPLE
  @code
  ATB_SetNR(PVM_NRepetitions * PVM_EvolutionCycles);
  @endcode

  @param nR    Number of repetitions

  @see @ref ATB_SetNSlices, @ref ATB_SetNI, @ref ATB_SetNA, @ref ATB_SetNAE

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetNR(int nR);




/** Sets the @c NI @c ACQP parameter.

  @PVM_PARS
  - @c NI

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c NI

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nI < @c 1

  @PVM_EXAMPLE
  @code
  ATB_SetNI(GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices) * PVM_NEchoImages *  PVM_NMovieFrames);
  @endcode

  @param nI    Number of images

  @see @ref ATB_SetNSlices, @ref ATB_SetNR, @ref ATB_SetNA, @ref ATB_SetNAE

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetNI(int nI);




/** Sets the @c NA @c ACQP parameter.

  @PVM_PARS
  - @c NA

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c NA

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nA < @c 1

  @PVM_EXAMPLE
  @code
  if (PVM_MotionSupOnOff == Off)
  {
    ATB_SetNA(PVM_NAverages);
    ATB_SetNAE(1);
  }
  else
  {
    ATB_SetNAE(PVM_NAverages);
    ATB_SetNA(1);
  }
  @endcode

  @param nA    Number of averages

  @see @ref ATB_SetNSlices, @ref ATB_SetNR, @ref ATB_SetNI, @ref ATB_SetNAE

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetNA(int nA);




/** Sets the @c NAE @c ACQP parameter.

  @PVM_PARS
  - @c NAE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c NAE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a nAE < @c 1

  @PVM_EXAMPLE
  @code
  if (PVM_MotionSupOnOff == Off)
  {
    ATB_SetNA(PVM_NAverages);
    ATB_SetNAE(1);
  }
  else
  {
    ATB_SetNAE(PVM_NAverages);
    ATB_SetNA(1);
  }
  @endcode

  @param nAE    Number of averaged experiments

  @see @ref ATB_SetNSlices, @ref ATB_SetNR, @ref ATB_SetNI, @ref ATB_SetNA

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetNAE(int nAE);




/** Sets the @c ACQ_phase_factor @c ACQP parameter.

  @PVM_PARS
  - @c ACQ_phase_factor

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c ACQ_phase_factor

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - @a acqPhaseFactor < @c 1

  @PVM_EXAMPLE
  @code
  ATB_SetAcqPhaseFactor(PVM_RareFactor);
  @endcode

  @param acqPhaseFactor    Integer containing the phase factor

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetAcqPhaseFactor(int acqPhaseFactor);



/** Sets the @c ACQ_scan_size parameter.

  @c ACQ_scan_size takes one of the following values:
  - @c One_scan = Entire @c ACQ_size is collected in a single sweep,
                        e.g. single-shot @em EPI.
  - @c ACQ_phase_factor_scans

  @PVM_PARS
  - @c ACQ_scan_size

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
  ATB_SetAcqScanSize(One_scan);
  @endcode

  @param acqScanSize    Numeration of type @c SCAN_SIZE_TYPE

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetAcqScanSize(SCAN_SIZE_TYPE acqScanSize);




/** Sets the @c ACQ_exp_type parameter.

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE void ATB_SetAcqExpType(EXPERIMENT_TYPE acqExpType);




/** Sets the @c ACQ_01_list_size and @c ACQ_01_list parameters.

  @c ACQ_O1_list_size is set to @a nSlices.

  @PVM_PARS
  - @c ACQ_O1_list_size
  - @c @c ACQ_01_list

  @PVM_TBCALL
  - @ref GTB_OrderedFreqOffsetList

  @PVM_RELCALL
  - @c ACQ_O1_list_size

  @PVM_PRECOND
  - @a sliceOrder and @a freqOffsetList must be sized according to @a nSlices.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  ATB_SetAcqO1List(GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices),
                   PVM_ObjOrderList,
                   PVM_SliceOffsetHz);
  @endcode

  @param     nSlices        Number of slices in all packages
  @param[in] sliceOrder     Order of slices in all packages
  @param[in] freqOffsetList List of frequency offsets for all slices

  @see @ref ATB_SetAcqO1BList, @ref ATB_SetAcqO2List, @ref ATB_SetAcqO3List

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetAcqO1List(int    nSlices,
                                    int    *sliceOrder,
                                    double *freqOffsetList);




/** Sets the @c ACQ_01B_list_size and @c ACQ_01B_list parameters.

  @c ACQ_O1B_list_size is set to @a nSlices.

  @PVM_PARS
  - @c ACQ_O1B_list_size
  - @c @c ACQ_01B_list

  @PVM_TBCALL
  - @ref GTB_OrderedFreqOffsetList

  @PVM_RELCALL
  - @c ACQ_O1B_list_size

  @PVM_PRECOND
  - @a sliceOrder and @a freqOffsetList must be sized according to @a nSlices.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  ATB_SetAcqO1BList(GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices),
                    PVM_ObjOrderList,
                    PVM_ReadOffsetHz);
  @endcode

  @param     nSlices        Number of slices in all packages
  @param[in] sliceOrder     Order of slices in all packages
  @param[in] freqOffsetList List of frequency offsets for all slices

  @see @ref ATB_SetAcqO1List, @ref ATB_SetAcqO2List, @ref ATB_SetAcqO3List

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetAcqO1BList(int     nSlices,
                                     int    *sliceOrder,
                                     double *freqOffsetList);




/** Sets the @c ACQ_02_list_size and @c ACQ_02_list parameters.

  @c ACQ_O2_list_size is set to @a nSlices.

  @PVM_PARS
  - @c ACQ_O2_list_size
  - @c @c ACQ_02_list

  @PVM_TBCALL
  - @ref GTB_OrderedFreqOffsetList

  @PVM_RELCALL
  - @c ACQ_O2_list_size

  @PVM_PRECOND
  - @a sliceOrder and @a freqOffsetList must be sized according to @a nSlices.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  ATB_SetAcqO2List(GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices),
                   PVM_ObjOrderList,
                   PVM_SliceOffsetHz);
  @endcode

  @param     nSlices        Number of slices in all packages
  @param[in] sliceOrder     Order of slices in all packages
  @param[in] freqOffsetList List of frequency offsets for all slices

  @see @ref ATB_SetAcqO1List, @ref ATB_SetAcqO1BList, @ref ATB_SetAcqO3List

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetAcqO2List(int    nSlices,
                                    int    *sliceOrder,
                                    double *freqOffsetList);




/** Sets the @c ACQ_03_list_size and @c ACQ_03_list parameters.

  @c ACQ_O3_list_size is set to @a nSlices.

  @PVM_PARS
  - @c ACQ_O3_list_size
  - @c @c ACQ_03_list

  @PVM_TBCALL
  - @ref GTB_OrderedFreqOffsetList

  @PVM_RELCALL
  - @c ACQ_O3_list_size

  @PVM_PRECOND
  - @a sliceOrder and @a freqOffsetList must be sized according to @a nSlices.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  ATB_SetAcqO3List(GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices),
                   PVM_ObjOrderList,
                   PVM_SliceOffsetHz);
  @endcode

  @param     nSlices        Number of slices in all packages
  @param[in] sliceOrder     Order of slices in all packages
  @param[in] freqOffsetList List of frequency offsets for all slices

  @see @ref ATB_SetAcqO1List, @ref ATB_SetAcqO1BList, @ref ATB_SetAcqO2List

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetAcqO3List(int     nSlices,
                                    int    *sliceOrder,
                                    double *freqOffsetList);




/** Sets the @c ACQ_obj_order parameter.

  @PVM_PARS
  - @c ACQ_obj_order

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @a sliceOrder must be sized according to @a nSlices.

  @PVM_ERROR
  - @a nSlices < @c 1
  - @a innerLoop < @c 1
  - @a outerLoop < @c 1
  - @c ACQ_obj_order too small
  - @a sliceOrder is @c NULL pointer

  @PVM_EXAMPLE
  @code
  ATB_SetAcqObjOrder(GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices), PVM_ObjOrderList, PVM_NEchoImages, 1);
  @endcode

  @param     nSlices       Total number of slices
  @param[in] sliceOrder    int array containing slice ordering
  @param     innerLoop     Number of images acquired inside the slices loop
  @param     outerLoop     Number of images acquired outside of slices loop

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetAcqObjOrder(int nSlices,
                                      int *sliceOrder,
                                      int innerLoop,
                                      int outerLoop);




/** Sets the @c ACQ_slice_angle parameter.

  @PVM_PARS
  - @c ACQ_slice_angle

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c ACQ_slice_angle

  @PVM_PRECOND
  - @a acqGradMatrix must provide @a nOrient matrices.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  ATB_SetAcqSliceAngle(PtrType3x3 PVM_SPackArrGradOrient[0],
                       PVM_NSPacks);
  @endcode

  @param[in] acqGradMatrix  List of gradient orientation matrices
  @param     nOrient        Number of matrices

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetAcqSliceAngle(double (* acqGradMatrix)[3][3],
                                        int    nOrient);




/** Sets the baselevel parameters slice separation mode and slice separation values.

  @PVM_PARS
  - @c ACQ_slice_sepn_mode
  - @c ACQ_slice_sepn

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The size of @a sliceDistance must be @a nPacks.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  ATB_SetAcqSliceSepn(PVM_SPackArrSliceDistance,
                      PVM_NSPacks);
  @endcode

  @param[in] sliceDistance    Slice distance in slice packages (mm)
  @param     nPacks           Number of slice packages

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetAcqSliceSepn(double *sliceDistance,
                                       int    nPacks);


/** Sets the reco rotate for one dimension.

  @PVM_PARS
  - @c RECO_rotate

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c RECO_rotate

  @PVM_PRECOND
  - The size of @a offset must be @a nSlices.
  - The dimensions of @c RECO_rotate must be at least [@a orientNdx + 1][@a nSlices * @a nEchoImages].

  @PVM_EXAMPLE
  @code
  ATB_SetRecoRotate(
    NULL,
    PVM_Fov[0] * PVM_AntiAlias[0],
    NSLICES,
    PVM_NEchoImages,
    0);

  if (PTB_GetSpatDim() > 1)
  {
    ATB_SetRecoRotate(
      PVM_EffPhase1Offset,
      PVM_Fov[1] * PVM_AntiAlias[1],
      NSLICES,
      PVM_NEchoImages,
      1);
  }
  @endcode

  @PVM_ERROR
  NONE

  @param[in] offset     Offset in the dimension
                        - array: one for each slice
                        - @c NULL: all offsets should be zero for @a orientNdx
  @param fov            FOV in the dimension (effective FOV: includes anti-alias factor)
  @param nSlices        Number of slice
  @param nEchoImages    Number of echoes
  @param orientNdx      Dimension index

  @see @ref ATB_InitUserModeReco, @ref ATB_SetRecoTranspositionFromLoops, @ref ATB_SetRecoPhaseCorr

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetRecoRotate(double *offset,
                                     double fov,
                                     int    nSlices,
                                     int    nEchoImages,
                                     int    orientNdx);




/** Sets the reco rotate for one dimension.

  @PVM_PARS
  - @c RECO_rotate

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c RECO_rotate

  @PVM_PRECOND
  - The size of @a offset must be @a nSlices.
  - The dimensions of @c RECO_rotate must be at least [@a orientNdx + 1][@a nSlices * @a nEchoImages].

  @PVM_EXAMPLE
  @code
  ATB_SetRecoRotate(
    NULL,
    PVM_Fov[0] * PVM_AntiAlias[0],
    NSLICES,
    PVM_NEchoImages,
    1,
    0);

  if (PTB_GetSpatDim() > 1)
  {
    ATB_SetRecoRotate(
      PVM_EffPhase1Offset,
      PVM_Fov[1] * PVM_AntiAlias[1],
      NSLICES,
      PVM_NEchoImages,
      1,
      1);
  }
  @endcode

  @PVM_ERROR
  NONE

  @param[in] offset     Offset in the dimension
                        - array: one for each slice
                        - @c NULL: all offsets should be zero for @a orientNdx
  @param fov            FOV in the dimension (effective FOV: includes anti-alias factor)
  @param nSlices        Number of slice
  @param nInnerLoops    Number of loopings within the slice loop, typically number of echoes
  @param nOuterLoops    Number of loopings outside the slice loop
  @param orientNdx      Dimension index

  @see @ref ATB_InitUserModeReco, @ref ATB_SetRecoTranspositionFromLoops, @ref ATB_SetRecoPhaseCorr

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetRecoRotate(double *offset,
                                     double fov,
                                     int    nSlices,
                                     int    nInnerLoops,
                                     int    nOuterLoops,
                                     int    orientNdx);




/** Set @c RECO_offset

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE void ATB_SetRecoOffset(int    *recoFtSize,
                                     double *antiAlias,
                                     int     ni,
                                     int     dim);




/** Sets the reco transposition parameters.

  @PVM_USEINSTEAD{ATB_SetRecoTranspositionFromLoops}

  This function does not work with movies acquired with the @c NI loop.

  The current version makes the following call:
  @code
  ATB_SetRecoTranspositionFromLoops(acqMatrix, nSlices, nEchoImages,
				    nSlices*nEchoImages, objOrder);
  @endcode

  @param acqMatrix      Gradient matrix
  @param nPacks         Number of slice packs
  @param nSlices        Total number of slices
  @param nEchoImages    Number of echo images
  @param objOrder       Object order list

  @see @ref ATB_SetRecoTranspositionFromLoops

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE void ATB_SetRecoTransposition(double (*acqMatrix )[3][3],
                                            int    nPacks,
                                            int    nSlices,
                                            int    nEchoImages,
                                            int   *objOrder);




/** Sets the reco transposition parameters.

  @PVM_USEINSTEAD{ATB_SetRecoTranspositionFromLoops}
  This function is replaced by a version with a different interface

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  Any call of this function reports an error, use new version instead


  @param[in] acqMatrix    Gradient rotation matrix; must be allocated for @a nSlices 3x3 elements
  @param     nSlices      Total number of slices
  @param     nInnerLoops  Number of loopings within the slice loop, typically n
  @param     nObjects     Total number of objects (NI).
  @param[in] objOrder     Object order list; must be allocated for @a nObjects elements

  @see @ref ATB_InitUserModeReco, @ref ATB_SetRecoRotate, @ref ATB_SetRecoPhaseCorr


  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE void ATB_SetRecoTranspositionFromLoops(double (*acqMatrix )[3][3],
                                                     int    nSlices,
                                                     int    nInnerLoops,
                                                     int    nObjects,
                                                     int    *objOrder);

/** Sets the reco transposition parameters.

  This function redimensions and calculates the values of the @c RECO_transposition array
  depending on the image orientation. It is compatible with any arrangement of the slice
  loop within the @c NI loop.

  @PVM_PARS
  - @c RECO_Transposition

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c RECO_Transposition

  @PVM_PRECOND
  - Geometry parameter reffered by name (argument @a gcnam  has
    to be updated correctly before
  - objOrder must be a 1-dimensional array (dimension @a nObjects containing a unique
    permutation of 0,1,..., @a nObjects -1

  @PVM_ERROR
  - Illegal values of geometry object parameter @a gcnam
  - Illegal order list @a objOrder

  @PVM_EXAMPLE
  @code
  ATB_SetRecoTranspositionFromLoops(
    "PVM_SliceGeo",
    NSLICES,
    1,
    NI,
    ACQ_obj_order);
  @endcode

  @param[in] gcnam        name of the geometry interface parameter of type GEO_CUBOID_PACK
  @param[in] spatdim      number of spatial dimensions (must be in [1,3]
  @param     nInnerLoops  Number of loopings within the slice loop, typically number of echo images
  @param     nObjects     Total number of objects (NI).
  @see @ref ATB_InitUserModeReco, @ref ATB_SetRecoRotate, @ref ATB_SetRecoPhaseCorr

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetRecoTranspositionFromLoops(const char *const gcnam,
                                                     int spatdim,
                                                     int nInnerLoops,
                                                     int nObjects);



/** Sets the reco phase correction for one dimension.

  Prepares a linear phase correction consisting of zeroth and first order. @n
  It is usually applied to correct the image phase for the central echo position of the
  time domain data.

  If the function detects that a homodyne Half-FT is used in dimension @c dim or
  above (RecoHalfFT == Yes), then the parameters @c echoPos and @c offset are
  ignored and the phase correction is set to @c NO_PC, because the Half-FT
  reconstruction already includes a phase correction.

  @PVM_PARS
  - @c RECO_pc_mode
  - @c RECO_pc_lin

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c RECO_pc_mode

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  for (int i = 0; i < PTB_GetSpatDim(); i++)
  {
    ATB_SetRecoPhaseCorr(50.0, 0.0, i);
  }
  @endcode

  @param echoPos    Echo position [%], usually @c 50.0
  @param offset     Zero-order offset correction [deg], usually @c 0
  @param dim        Dimension index

  @see @ref ATB_InitUserModeReco, @ref ATB_SetRecoRotate, @ref ATB_SetRecoTranspositionFromLoops

  @ingroup PVM_ATB

*/
DLL_INTERFACE void ATB_SetRecoPhaseCorr(double echoPos,
                                        double offset,
                                        int    dim);




/** Sets baselevel parameter @c PULPROG.

  This function sets the name of the pulseprogram (@c PULPROG).
  In case of pulseprogram names that are too long, the string will be terminated to the
  maximum length of the pulseprogram name and a message appears in the ParaVision shell
  and it returns @c 0. In all other cases @c 1 is returned.

  @PVM_PARS
  - @c PULPROG

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c PULPROG

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  ATB_SetPulprog("FLASH.ppg");
  @endcode

  @param ppgname     Pulseprogram name

  @return @c 0 if @a ppgname is too long else @c 1

  @ingroup PVM_ATB
*/
DLL_INTERFACE int ATB_SetPulprog(const char *const ppgname);




/** Initializes parameter @c ACQ_echo_descr.

  This function sets the string array parameter @c ACQ_echo_descr to a 1-element string array
  with an empty string.

  This parameter is used for the image display to provide optional description of echo images.

  @PVM_H1{Note}
  In case a special image description should be provided the parameter @c ACQ_echo_descr has to
  be redimensioned to the desired number of additional repetitions. (Take care of the second
  dimension of the string array, it must be 20!, see code below.) This number must be stored
  in parameter @c ACQ_n_echo_images and the desired descriptions should be stored in the string
  elements of @c ACQ_echo_descr. Please consider the maximum length of 20 characters for a string
  element of @c ACQ_echo_descr. (The usage of @c snprintf is recommended, see manual page of
  this function for further information).

  @PVM_PARS
  - @c ACQ_echo_descr
  - @c ACQ_n_echo_images

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - Default relation of parameter @c PULPROG

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  int yourNumberOfAddReps=1;

  (...)
  // some code to set the additional repetition loop extension
  (...)

  ACQ_n_echo_images = yourNumberOfAddReps;
  PARX_change_dims("ACQ_echo_descr",yourNumberOfAddReps,20);
  for(int i=0;i<yourNumberOfAddReps;i++)
  {
    snprintf(ACQ_echo_descr[i],20,"Add. Echo. %d",i+1);
  }
  @endcode

  @see @ref ATB_ResetMovieDescr

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_ResetEchoDescr(void);




/** Initializes parameter @c ACQ_movie_descr.

  This function sets the string array parameter @c ACQ_movie_descr to a 1-element string array
  with an empty string.

  This parameter is used for the image display to provide optional description of an extension
  of the repetition loop.

  @PVM_H1{Note}
  In case a special image description should be provided the parameter @c ACQ_echo_descr has to
  be redimensioned to the desired number of additional repetitions. (Take care of the second
  dimension of the string array, it must be 20!, see code below.) This number must be stored
  in parameter @c ACQ_n_echo_images and the desired descriptions should be stored in the string
  elements of @c ACQ_echo_descr. Please consider the maximum length of 20 characters for a string
  element of @c ACQ_echo_descr. (The usage of @c snprintf is recommended, see manual page of
  this function for further information).

  @PVM_PARS
  - @c ACQ_movie_descr
  - @c ACQ_n_movie_frames

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - Default relation of parameter @c PULPROG

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  int yourNumberOfAddReps=1;

  (...)
  // some code to set the additional repetition loop extension
  (...)

  ACQ_n_movie_frames = yourNumberOfAddReps;
  PARX_change_dims("ACQ_movie_descr",yourNumberOfAddReps,20);
  for(int i=0;i<yourNumberOfAddReps;i++)
  {
    snprintf(ACQ_movie_descr[i],20,"Add. Rep. %d",i+1);
  }
  @endcode

  @see @ref ATB_ResetEchoDescr

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_ResetMovieDescr(void);



//DLL_INTERFACE void ATB_SetJob0FromLegacyPars(void);

/* /pv60/gen/src/prg/pvm_toolboxes/PvAcqTools/TuneShim.c */

/** Set shim step width for shimming with tune algorithm.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - The size of @a subs must be @a nspr.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  int order[N_SHIMS];
  SysInfo_ShimFuncOrder(order);

  double sens[N_SHIMS];
  for (int i = 0; i < N_SHIMS; i++)
  {
    sens[i] = MRT_MaxShimSens(PVM_TuneShimSens[i], order, N_SHIMS, PVM_TuneShimRadius);
  }

  ATB_TuneShimSetStepWidth(sens,
                           PVM_TuneShimSet,
                           PVM_TuneShimStepWidth[PVM_TuneShimAlgCnt[0]],
                           PVM_TuneShimNShimRep);
  @endcode

  @param[in]  sensitivity  Maximum shim sensitivity [Hz]
  @param[out] subs         Subset of shims to be handled
                           (the elements @c step and @c stepHz will be modified)
  @param      swhz         Desired step width [Hz]
  @param      nspr         Number of shims in subset

  @return Minimum step size set [Hz] (may be smaller than @a swhz due to limited range)

  @ingroup PVM_ATB_INT
*/
DLL_INTERFACE double ATB_TuneShimSetStepWidth(const double *sensitivity,
                                              TUNE_SHIM_TYPE *subs,
                                              const double swhz,
                                              int nspr);




/* /pv60/gen/src/prg/pvm_toolboxes/PvAcqTools/RecoParSet.c */

/** Initializes a default reconstruction.

  Sets the baselevel reconstruction parameters (@c RECO) to a default as defined by the baselevel
  acquisition parameters (@c ACQP).

  @PVM_DEACT
  20100101

  @PVM_PARS
  - Dependent on @c ACQP parameter settings

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - Dependent on @c ACQP parameter settings

  @PVM_PRECOND
  - This routine should be called after all @c ACQP parameters have their final values.

  @PVM_ERROR
  NONE

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE void ATB_InitDefaultReco(void);




/** Sets parameters for the user mode reconstruction.

  @PVM_USEINSTEAD{ATB_InitUserModeReco}

  Parameters are set concerning the following reconstruction tasks:
  - Sorting
  - Parallel imaging (GRAPPA)

  @PVM_DEACT
  20100101

  @PVM_PARS
  - @c RecoSortDim
  - @c RecoSortSize
  - @c RecoSortRange
  - @c RecoSortSegment
  - @c RecoSortMaps
  - @c RecoGrappaAccelFactor
  - @c RecoGrappaReadCenter
  - @c RecoGrappaPhaseCenter
  - @c RecoGrappaNumRefPhase
  - @c RecoGrappaTruncThresh
  - @c RecoGrappaNumRefRead
  - @c RecoGrappaKernelRead
  - @c RecoGrappaKernelPhase
  - @c RecoGrappaIncludeRefLines

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c RecoSortDim
  - @c RecoGrappaAccelFactor

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @param dim                Number of dimensions
  @param acqSize            Acquisition size (@a dim elements; 1st @a dim: no doubling,
                            only used for GRAPPA)
  @param ftSize             Fourier transform size (@a dim elements, not smaller than @a acqSize)
  @param encSteps2D         Encoding steps in 2nd dimension. Must contain @a acqSize[1]
                            elements ranging from @a -ftSize/2 to @a ftSize/2.
                            May also be @c NULL, meaning linearly arranged steps
  @param encSteps3D         Encoding steps in 3rd @a dim (ignored in 2D methods)
  @param numActReceivers    Number of active receivers (GRAPPA only)
  @param acceleration       Parallel MRI acceleration factor in 2nd @a dim (GRAPPA only)
  @param refLines           Number of reference lines for GRAPPA in 2nd @a dim (@c 0 = no GRAPPA)
  @param nImages            Number of images
  @param objOrder           Acquisition object order (@a nImages elements, @c NULL = @c linear)
  @param phaseFactor        Number of 2D steps in the innermost loop (@c ACQ_phase_factor)
  @param echoPosition       Echo position in readout window [%]
                            - 50 = centre

  @deprecated @ingroup PVM_DEPRECATED
*/
DLL_INTERFACE void ATB_InitUserModeReco(int        dim,
                                        const int *acqSize,
                                        const int *ftSize,
                                        const int *encSteps2D,
                                        const int *encSteps3D,
                                        int        numActReceivers,
                                        int        acceleration,
                                        int        refLines,
                                        int        nImages,
                                        int       *objOrder,
                                        int        phaseFactor,
                                        double     echoPosition);




/** Sets parameters for the user mode reconstruction.

  Parameters are set concerning the following reconstruction tasks:
  - Activation of user mode reconstruction
  - Sizes
  - Sorting
  - Half Fourier
  - Channel scaling
  - Parallel imaging (GRAPPA)

  @PVM_H2{Dimensions}
  - The first dimension (0) is the direct ("read") dimension.
  - All following dimensions (1, 2, ...) are indirect dimensions.
  - Sorting is handled for up to three indirect dimensions
   (based on the order specified in parameters @a encSteps?).

  @PVM_PARS
  - @c RecoAutoDerive
  - @c RECO_inp_size
  - @c RECO_ft_size
  - @c RECO_size
  - @c RECO_offset
  - @c RecoSortDim
  - @c RecoSortSize
  - @c RecoSortRange
  - @c RecoSortSegment
  - @c RecoSortMaps
  - @c RecoScaleChan
  - @c RecoPhaseChan
  - @c RecoHalfFT
  - @c RecoHalfFTPos
  - @c RecoGrappaAccelFactor
  - @c RecoGrappaReadCenter
  - @c RecoGrappaPhaseCenter
  - @c RecoGrappaNumRefPhase
  - @c RecoGrappaTruncThresh
  - @c RecoGrappaNumRefRead
  - @c RecoGrappaKernelRead
  - @c RecoGrappaKernelPhase
  - @c RecoGrappaIncludeRefLines

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c RecoResetPars
  - @c RECO_inp_size
  - @c RECO_ft_size
  - @c RECO_size
  - @c RecoSortDim
  - @c RecoHalfFT
  - @c RecoGrappaAccelFactor

  @PVM_PRECOND
  - Acquisition baselevel parameters must have been set.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  Standard Cartesian case
  @code
  int dim = PTB_GetSpatDim();

  ATB_InitUserModeReco(
    dim,
    0,
    PVM_EncMatrix,
    PVM_Matrix,
    PVM_AntiAlias,
    PVM_EncPftOverscans,
    NI,
    ACQ_obj_order,
    ACQ_phase_factor,
    PVM_EncSteps1,
    PVM_EncSteps2,
    NULL,
    PVM_EncNReceivers,
    PVM_EncChanScaling,
    dim > 1 ? PVM_EncPpiRefLines[1]:0,
    dim > 1 ? PVM_EncPpi[1]:0);
  @endcode

  @param dim              Number of dimensions
  @param dimNonCart       Number of non-Cartesian dimensions
                          - Disables zero-filling before regridding
  @param[in] encSize      Encoding size
                          - @a dim elements
                          - @c NULL = @c derive from acquisition parameters
  @param[in] imgSize      Image size
                          - @a dim elements
                          - @c NULL = @c derive
  @param[in] antiAlias    Anti-aliasing factors
                          - @a dim elements
                          - @c NULL = @c 1
  @param[in] pftOverscans Partial Fourier overscans
                          - @a dim elements
                          - @c NULL = @a encSize[dim] / 2
  @param nImages          Number of output images
  @param objOrder         Acquisition object order
                          - @a nImages elements
                          - @c NULL = linear
  @param phaseFactor      Number of 2D steps in the innermost loop
  @param[in] encSteps1    Encoding steps of dimension 1
                          - Only for @a dim > 1.
                          - @a encSize[1] elements ranging from @a -ftSize[1]/2 to @a ftSize[1]/2.
                          - @c NULL = linear order
  @param[in] encSteps2    Encoding steps of dimension 2
                          - Only for @a dim > 2.
                          - @a encSize[2] elements ranging from @a -ftSize[2]/2 to @a ftSize[2]/2.
                          - @c NULL = linear order
  @param[in] encSteps3    Encoding steps of dimension 3
                          - Only for @a dim > 3.
                          - @a encSize[3] elements ranging from @a -ftSize[3]/2 to @a ftSize[3]/2.
                          - @c NULL = linear order
  @param numActReceivers  Number of active receivers
  @param[in] chanScaling  Scaling factors for active receivers
                          - @c NULL = @c 1
  @param refLines         Number of reference lines for GRAPPA in 2nd dimension
                          - @c 0 = no GRAPPA
  @param acceleration     Parallel MRI acceleration factor in 2nd dimension
                          - GRAPPA only

  @param sortImages       (optional) Number of input images
                          - required if nImages describes not the dimensionality of objOrder
                            (number of input images is different from number of output images)

  @param acqSize          (optional) Acquisition size
                          - required if acquisition size differes from encoding size, eg for non-cartesian imaging
                          - @a dim elements

  @see @ref ATB_SetRecoRegridN, @ref ATB_EpiSetRecoPars, @ref ATB_SetRecoRotate,
       @ref ATB_SetRecoTranspositionFromLoops, @ref ATB_SetRecoPhaseCorr

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_InitUserModeReco(int           dim,
                                        int           dimNonCart,
                                        const int    *encSize,
                                        const int    *imgSize,
                                        const double *antiAlias,
                                        const int    *pftOverscans,
                                        int           nImages,
                                        const int    *objOrder,
                                        int           phaseFactor,
                                        const int    *encSteps1,
                                        const int    *encSteps2,
                                        const int    *encSteps3,
                                        int           numActReceivers,
                                        const double *chanScaling,
                                        int           refLines,
                                        int           acceleration,
                                        int           sortImages=0,
                                        const int     *acqSize = NULL);




/** Sets parameters for EPI reconstruction.

  Function for setting the recontruction parameters for EPI
  (calls @ref ATB_InitUserModeReco).
  It can also handle spectroscopic EPI (EPSI). In that case the optional
  specSize parameter should be used to provide the spectroscopic
  acquisition size (reconstruction size will be set the same).
  In both cases PVM_EncMatrix and PVM_Matrix are used to get the acquisition
  and reconstruction sizes in the spatial dimensions.

  @PVM_PARS
  - @c RecoSortMaps
  - @c RecoSortSize
  - @c RecoSortRange
  - @c RecoSortDim
  - @c RecoReverseSegment

  @PVM_TBCALL
  - @ref ATB_InitUserModeReco
  - @ref ATB_SetRecoRotate
  - @ref ATB_SetRecoPhaseCorr
  - @ref ATB_SetRecoTransposition

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Acquisition baselevel parameters must have been set.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  ATB_EpiSetRecoPars(
    acceleration,
    1,
    1);
  @endcode

  @param acceleration    Desired parallel acceleration
  @param tDim            Number of images in time domain, e.g. number of echoes
  @param ISegments       Parameter for future extention (must be @c 1 currently)
  @param specSize        Optional parameter providing the spectral acquisition
                         size in EPSI mode.

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_EpiSetRecoPars(int acceleration,
                                      int tDim,
                                      int ISegments,
                                      int specSize=0);




/** Sets parameters for the 2D/3D regridding reconstruction (RegridN).

  The RegridN reconstruction is part of the standard reconstruction network in the user mode
  and provides regridding of 2D or 3D data (e.g. radial or spiral).

  The created network includes (in addition to the standard network):
  - Reading the trajectory from file (required for every interleave, slice, and repetition)
  - Density correction
  - Regridding
  - Apodisation correction
  - Cutting a circular/spherical FOV for radial and spiral methods

  @PVM_PARS
  - @c RecoRegridN
  - @c RecoRegridNPoints
  - @c RecoRegridNInterleaves
  - @c RecoRegridNTrajType
  - @c RecoRegridNTrajFile
  - @c RecoRegridNDensCorr
  - @c RecoRegridNTrajFileDC

  @PVM_TBCALL
  PTB_GetSpatDim

  @PVM_RELCALL
  - @c RecoRegridN

  @PVM_PRECOND
  - The user mode should have been activated.
  - @c RECO_size must have been set.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  UTE3D
  @code
  char trajFile[PATH_MAX];
  PvOvlUtilGetExpnoPath(trajFile, PATH_MAX, "traj");
  const int readP = PVM_EncMatrix[0]/2 + RampPoints;
  ATB_SetRecoRegridN(
    readP,
    NPro,
    RecoRegridNTrajRadial,
    trajFile,
    RecoRegridNDCPre,
    NULL);
  @endcode

  @param nPoints         Number of sample points per interleave
  @param nInterleaves    Number of interleaves
  @param trajType        Type of trajectory
                         - @c RecoRegridNTrajRadial (requires license PVMUTE)
                         - @c RecoRegridNTrajSpiral (requires license PVMSPIRAL)
                         - @c RecoRegridNTrajAny (requires license PVMUTE or PVMSPIRAL;
                         no density pre-correction)
  @param[in] trajFile    Full name of trajectory file
  @param densCorr        Type of density correction
                         - @c RecoRegridNDCNone
                         - @c RecoRegridNDCPost
                         - @c RecoRegridNDCPre
                         - @c RecoRegridNDCPrePost
  @param[in] trajFileDC  Full name of trajectory file used for density correction
                         - Only required for density pre-correction
                         - @c NULL = @c use @a trajFile instead
  @param[in] recoRegridNDim Specifies dimension for reconstruction. Default value 0 uses
                         the dimension of acquisition. This argument was introduced
                         for the reconstruction of spiral trajectories acquired in 3D-mode.

  @see @ref ATB_SetRecoRegridNOff

  @ingroup PVM_ATB
*/
DLL_INTERFACE void  ATB_SetRecoRegridN( int                   nPoints,
                                        int                   nInterleaves,
                                        RecoRegridNTrajTypes  trajType,
                                        const char           *trajFile,
                                        RecoRegridNDCTypes    densCorr,
                                        const char           *trajFileDC,
                                        const int             recoRegridNDim = 0
                                      );




/** Disables the 2D/3D regridding reconstruction (RegridN).

  May be useful after activating the RegridN reconstruction with @ref ATB_SetRecoRegridN.

  @PVM_PARS
  - @c RecoRegridN

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - @c RecoRegridN

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @see @ref ATB_SetRecoRegridN

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetRecoRegridNOff(void);




/* /pv60/gen/src/prg/pvm_toolboxes/PvAcqTools/diffAcq.c */

/** Sets parameters used in the diffusion preparation pulseprograms included by methods using
  the @em DTI module.

  Dependent on the actual diffusion preparation mode (@c PVM_DiffPrepMode)

  - The frequency offset list for the slice selective refocussing is dimensioned and calculated
    (@c PVM_DwFlist0).
  - The duration, shape, and attenuation of the used RF-pulse(s) are set.
  - The delays defined in pulseprogram part @c DwiHead.mod are set to their final values.

  @PVM_PARS
  Parameters used in the @em DTI preparation as delays, pulse durations or frequency lists. @n
    - @c PVM_DwD0 - @c PVM_DwD19
    - @c PVM_DwP0 - @c PVM_DwP1
    - @c PVM_DwFlist0
    - @c PVM_DwExcPulse1 (ACQ_RfShapes[48])
    - @c PVM_DwRfcPulse1 (ACQ_RfShapes[49])

  @PVM_TBCALL
   - @ref CFG_GradientRiseTime
   - @ref ATB_SetRFPulse

  @PVM_PRECOND
  - Parameters of the diffusion group must have their final values.
    (@ref STB_UpdateDiffusionPreparation must have been called before.)
  - Parameters used as arguments (see list below) must have their final values.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  In this example @c ACQ_O1_list is used as frequeny list for the slice excitation, parameter
  @c ExcSliceGrad (local scalar double method parameter) is used as excitation slice gradient
  amplitude (% of max grad. power). The total number of slices is calculated using
  @c GTB_NumberOfSlices and the diffusion gradients should be switched during the setup mode.

  @code
  ATB_DwAcq(ACQ_O1_list,
    	    GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices ),
            ExcSliceGrad,Yes);
  @endcode

  @param[in] flist     Double array containing the offset frequencies used for slice excitation
                       (dimension must be @a flsize see below)
  @param flsize        Scalar integer containing the number of elements in array @a flist
  @param sgrad         Scalar double containing the final amplitude of the excitation slice
                       gradient (expressed in % of max grad. power)
  @param iGradSetup    Scalar @c YesNo argument. If set to @c Yes the diffusion gradients will be
                       stepped in the diffusion loop during the setup period. In case the @c NR
                       loop is used as diffusion loop, this parameter should be set to @c No.

  @see @ref STB_InitDiffusionPreparation, @ref STB_UpdateDiffusionPreparation

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_DwAcq(double       *flist,
                             int           flsize,
                             const double  sgrad,
                             YesNo         iGradSetup);




/* /pv60/gen/src/prg/pvm_toolboxes/PvAcqTools/AdjVals.c */

/** Sets a special reference attenuation for nucleus of interest.

  The special reference attenuation (1 ms 90 deg pulse) should be set by an appropriate adjustment
  routine. If the desired nucleus has been adjusted before for the same nucleus and the same status,
  the corresponding entry of @c PVM_RefAtt will be modified. Otherwise a new entry of this array
  will be generated.

  @PVM_PARS
  - @c PVM_RefAtt

  @PVM_TBCALL
   - @c CFG_MinRFAttenuation
   - @c CFG_RFPulseHighestAttenuation
   - @c CFG_IsNucleusKnown

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  Sets a global reference attenuation for protons

  @code
   double refAtt;

  // adjustment of reference attenuation ...
  ATB_SetRefAtt("1H",refAtt,"global");
  @endcode

  @param[in] nuc  String containing the nucleus of interest
  @param att      Double value containing the reference attenuation. Attenuation only if @c Yes is
                  returned.
  @param[in] stat Containing information about special requirements for the reference attenuation

  @return
  - @c 2 = @a att is not in legal range (as defined by @ref CFG_MinRFAttenuation
    and @ref CFG_RFPulseHighestAttenuation)
  - @c -1 = the nucleus as specified in @a nuc is not known (tested by @ref CFG_IsNucleusKnown)
  - @c 0 = successfull modification

  @ingroup PVM_ATB
*/
DLL_INTERFACE int   ATB_SetRefAtt(const char *const nuc,
                                  const double      att,
                                  const char *const stat);

/* /pv60/gen/src/prg/pvm_toolboxes/PvAcqTools/AtbAdjTools.c */

/** Sets @c GS parameters for standard receiver gain adjustment.

  This routine sets the minimum @c GS parameters necessary to run a receiver gain adjustment
  based on digitizer filling factor.

  @PVM_PARS
  - @c ACQ_SetupAutoName
  - @c ACQ_Setupdim
  - @c ACQ_SetupType
  - @c ACQ_SetupShowDigFilling

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - Default relations of changed @c GS parameter

  @PVM_PRECOND
  - @a AutoCounterName must be a valid parameter name of an @c int parameter used as
  autocounter of the adjustment algorithm.

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  ATB_RgAdjSetGsPars("PVM_AutoRgCounter");
  @endcode

  @param AutoCounterName    Parameter name as described above

  @return Non-zero return value indicates an illegal parameter name given as argument.

  @ingroup PVM_ATB
*/
DLL_INTERFACE int ATB_RgAdjSetGsPars(const char *const AutoCounterName);




/** Receiver gain adjustment algorithm.

  This routine implements a receiver gain adjustment algorithm. If the digitizer filling
  (provided by argument @a digfill) is outside the desired limits (provided by arguments
  @a lowerlim and @a upperlim), the algorithm scales the receiver gain (provided by pointer
  argument @a recgain) to reach a target digitizer filling (@a upperlim - @c 10\% or, if
  higher @a lowerlim). The digitizer filling during an auto adjustment should be provided
  by routine @ref ATB_GetDigFill.

  The algorithm is using information of previous scans (receiver gain values for the lowest
  and highest digitizer filling during the adjustment) to achieve a faster convergence. These
  limits are initialized to system defaults if argument @a digfill is set to a negative value
  and pointer argument @a recgain is set to a system specific starting value. Passing a
  @c NULL pointer for @a recgain is only allowed if the algorithm is forced to initilize
  (argument @a digfill < @c 0.0). For all other cases a @c NULL pointer argument @a recgain
  forces an error condition.

  If the signal condition during an adjustment differs significantly from previous calls of
  the routine (e.g. if the receiver adjustment is a part of an other adjustment) information
  about previous scans may be cleared by setting argument @a reset to @c Yes. The @a digfill
  information for the current receiver gain is still used in this case, the brackening is
  based however by system limits of the receiver gain.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref CFG_RgValue
  - @ref CFG_MaxRg
  - @ref CFG_MinRg
  - @ref CFG_RgInitVal

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - Initialization of the routine before the first acquisition
  - Digitizer filling factor @a digfill belongs to the receiver gain value
  - Proper limits of the desired digitizer filling factor. If the rules below are broken
    @c -3 is returned without changing any argument.
    - Upper limit (@a upperlim) in the range of @c 5.1\% to @c 99\%
    - Lower limit (@a lowerlim) in the range of @c 0.0\% to @a upperlim - @c 5\%

  @PVM_ERROR
  Negative return values indicate the following error conditions:
  - Stop Conditions
    - @c -6 = Illegal limits for digitizer filling or @c NULL pointer for argument
                    @a recgain
    - @c -3 = Unstable signal
    - @c -2 = Signal overflow: digitizer overflow at lowest possible receiver gain
    - @c -1 = Weak signal: digitizer filling below lower limit at highest possible
                    receiver gain
    - @c 0  = Digitizer filling is within the specified range (stop condition).
  - Continue Adjustment
    - @c 1 =  An additional iteration is required, continue adustment.

  @PVM_EXAMPLE
  @code
  ATB_AdjRgAlg(ATB_GetDigFill(), &RG, 5.0, 90.0, No);
  @endcode

  @param digfill       Digitizer filling factor (in % as provided by @ref ATB_GetDigFill)
  @param recgain       Pointer to the receiver gain
  @param lowerLimit    Desired lower limit of the receiver gain (in %)
  @param upperLimit    Desired upper limit of the receiver gain (in %)
  @param reset         If set to @c Yes: Clear @a digfill information of previous scans.

  @return  Stop conditions (@c -6, @c -2, @c -1, @c 0), continue condition (@c 1)

  @see @ref ATB_AdjCheckRg, @ref ATB_GetDigFill

  @ingroup PVM_ATB
*/

DLL_INTERFACE int ATB_AdjRgAlg(double digfill,
                               double *recgain,
                               double lowerLimit,
                               double upperLimit,
                               YesNo  reset);




/** Check receiver gain during adjustment.

  This function is to be called from the relations of an adjustment counter
  to check and adjust the receiver gain.

  In case of failure, the reason is provided in @c AdjErrorComment which will
  be displayed after aborting the adjustment.

  @PVM_PARS
  - @c RG
  - @c AdjErrorComment

  @PVM_RELCALL
  - @c RG

  @PVM_TBCALL
  - @ref ATB_AdjRgAlg
  - @ref ATB_GetDigFill

  @PVM_PRECOND
  - @c RG is assumed to be the receiver gain parameter

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  switch (ATB_AdjustmentCheckRg(5.0, 80.0))
  {
    case -1:
      // failure
      PVM_ArrayPhaseCounter = -1;
      return;
    case 1:
      // repeat after adapting RG
      return;
    default:
      // ok
      break;
  }
  @endcode

  @param lowerRGLimit (optional) Desired lower limit of the receiver gain [%] (default = 10.0)
  @param upperRGLimit (optional) Desired upper limit of the receiver gain [%] (default = 70.0)

  @return
  - @c  0 = success (RG value within range)
  - @c  1 = new RG value was set, repeat adjustment now
  - @c -1 = failure

  @see ATB_AdjRgAlg

  @ingroup PVM_ATB
*/
DLL_INTERFACE int ATB_AdjCheckRg(double lowerRGLimit = 10.0, double upperRGLimit = 70.0);




/** Provides digitizer filling factor for jobs.

  This routine is intended to be used during an auto adjustment procedure and delivers the
  digitizer filling factor in units of % for job specified by argument @c jid.
  The return value is controlled by
  @c ACQ_SetupDigFilling and may be above  100\% under overflow conditions.
  Outside adjustment procedures return value is undefined and may cause error conditions.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  - Default relations of changed @c GS parameter

  @PVM_PRECOND
  - @c ACQ_SetupShowDigFilling must be set to @a Yes.

  @PVM_ERROR
  - job index @c jid must be >=0 and less than dimension of ACQ_jobs
  - negative value of digitizer filling

  @param  jid index of job (must lie within actual dimension fo ACQ_jobs
  @return Digitizer filling in %

  @ingroup PVM_ATB
*/
DLL_INTERFACE double ATB_GetDigFill(int jid = 0);




/** Receiver gain autocounter default relation.

  This routine is used as default relation of parameter @c PVM_AutoRgCounter. It is not intended
  to be called inside the standard method code. To implement an alternative adjustment algorithm,
  the relation of @c PVM_AutoRgCounter may be redirected to a local function. Toolbox
  @ref ATB_AdjRgAlg is used for the adjustment algorithm, signal underflow is not treated as
  error condition.

  @PVM_PARS
  - @c PVM_AutoRgCounter
  - @c RG

  @PVM_TBCALL
  - @ref ATB_GetDigFill
  - @ref ATB_AdjRgAlg

  @PVM_RELCALL
  - @c PVM_AutoRgInitHandler (during initialization phase)
  - @c RG

  @PVM_PRECOND
  - Call of @ref ATB_RgAdjSetGsPars

  @PVM_ERROR
  NONE
  @PVM_H1{Possible adjustment error conditions}
  - Signal intensity too high: Digitizer overflow at lowest possible receiver gain
  - Illegal limits for digfilling passed

  @see @ref ATB_AdjRgAlg, @ref ATB_GetDigFill

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_AutoRgCounter(void);




/** Initializes default @c RG adjustment.

  Default relation of parameter @c PVM_AutoRgInitHandler. This routine is called during the
  initialization phase of the standard @c RG adjustment algorithm.

  @PVM_PARS
  - @c RVM_AutoRgCounter set to @c 1
  - @c NA, @c NAE, @c NR set to @c 1
  - @c ACQ_DS_enabled set to @c No
  - @c RG set to @c PVM_AutoRgInitVal if this parameter has a value.

  @PVM_TBCALL
  - @ref ATB_AdjRgAlg

  @PVM_RELCALL
  - @c NA
  - @c NAE
  - @c ACQ_DS_enabled
  - @c RG

  @PVM_PRECOND
  - Call of @ref ATB_RgAdjSetGsPars

  @PVM_ERROR
  NONE
  @PVM_H1{Possible adjustment error conditions}
  NONE

  @see @ref ATB_AdjRgAlg, @ref ATB_AutoRgCounter

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_AutoRgInit(void);




/** Initializes the acquisition of a reference scan in spectroscopy.

  This routine is intended to be used to prepare the @c GS parameters in order to acquire a
  reference scan in spectroscopy sequences. It is bound to the parameter group @c ReferenceScan.
  It should be called in the initialization phase of the reference scan adjustment. The correct
  functionality of this adjustments depends on the correct setting of @c GS parameters done
  by this routine.

  @PVM_PARS
  - @c ACQ_SetupType
  - @c PVM_RefScan (array parameter holding the reference scan)
  - @c RG

  @PVM_TBCALL
  - @ref ATB_AdjRgAlg
  - @ref CFG_RgInitVal

  @PVM_RELCALL
  - @c ACQ_SetupType
  - @c RG

  @PVM_PRECOND
  - <c> ACQ_size[0] </c> must have its final value.

  @PVM_ERROR
  NONE

  @see @ref ATB_RefScanCounter

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_InitRefScanAQ(void);




/** Adjustment algorithm for reference scan acquisition.

  This routine is used as default relation of parameter @c PVM_RefScanCounter. This parameter
  controls the adjustment procedure of the spectroscopic reference scan acquisition. The adjusted
  @c RG is stored in parameter @c PVM_RefScanRG and should be visible in the @c ReferenceScan
  parameter subclass after completion. In addition parameter @c PVM_RefScan contains the
  reference scan in time domain represented as integer array with alternating real and imaginary
  part. This parameter is not visible in the method editor but it is stored in the method file in
  the expno directory. The array is also written in binary format into a file fid.refscan
  (int array of size <c>ACQ_size[0]</c>, alternating real and imaginary part).

  @PVM_PARS
  - @c PVM_RefScanCounter
  - @c RG
  - @c PVM_RefScanRG
  - @c PVM_RefScan
  - @c AdjParameterListSize
  - @c AdjParameterList

  @PVM_TBCALL
  - @ref ATB_InitRefScanAQ
  - @ref ATB_AdjRgAlg

  @PVM_RELCALL
  - @c AdjParameterListSize

  @PVM_PRECOND
  - See @ref ATB_InitRefScanAQ

  @PVM_ERROR
  NONE

  @see @ref ATB_InitRefScanAQ

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_RefScanCounter(void);




/* /pv60/gen/src/prg/pvm_toolboxes/PvAcqTools/TuneShim.c */

/* range checkers and relations */
DLL_INTERFACE void ATB_TuneShimQF(void);

DLL_INTERFACE void ATB_TuneShimInitAdjHandler(void);
DLL_INTERFACE void ATB_TuneShimCounter(void);
DLL_INTERFACE void ATB_TuneShimFinishAdjHandler(void);




/** Sets RF pulse baselevel parameters.

  This function is used to transfer the state of PVM rf pulses
  of type @c PVM_RF_PULSE to parameter @c ACQ_RfShapes. Optionally
  the pulse duration is transferred into a scalar double parameter
  (usually one element of the @c P array).

  @PVM_PARS
  - entry of @c ACQ_RfShapes as specified by argument @a acqRfShapeParn
  - (optional) double parameter that is used to control the pulse
    duration on pulseprogram level

  @PVM_TBCALL
  - @ref UT_GetParType
  - SysInfo_TxNCoilElements
  - SysInfo_TxActiveCoilElements
  - SysInfo_getErrorMessage
  - @ref CFG_RFPulseMinDuration

  @PVM_RELCALL
  - relation of parameter specified by argument @a acqPulseDurParn
  - relation of parameter specified by argument @a acqRfShapeParn

  @PVM_PRECOND
  - @ref STB_UpdateNuclei has been called before
  - @c PVM_RF_PULSE parameter (specified by argument @a rfpulseParn)
    initialized by @ref STB_InitRFPulse and updated by @ref STB_UpdateRFPulse

  @PVM_ERROR

  - empty string, for mandatory arguments @a rfpulseParn, @a acqRfShapeParn
  - no valid parameter names for all arguments
  - wrong type of parameter specified by argument @a rfpulseParn :
    only parameter of type @c PVM_RF_PULSE are supported,
    array parameters are not allowed.
  - wrong type of parameter specified by argument @a acqRfShapeParn :
    only parameter of type @c ACQSHAPE are allowed (elements of @c ACQ_RfShapes).
  - wrong type of parameter specified by argument @a acqPulseDurParn (if
    specified): only double parameters are allowed.
  - no initialization and proper update of parameter specified by @a rfpulseParn
  - error states of following functions
    - SysInfo_TxNCoilElements
    - SysInfo_TxActiveCoilElements

  @PVM_EXAMPLE
  In the following example @c ExcPulse1 is the name of a parameter of type
  @c PVM_RF_PULSE, @c ACQ_RfShapes[0] is set the pulse duration is stored
  in baselevel parameter P[0]:

  @code
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");
  @endcode

  Example to be used to update only the shape struct. This
  is used to handle manually adjusted RF pulse amplitude(s)
  during setup pipeline acquisitions:

  @code
  void HandleRFPulseAmplitude(void)
  {
    DB_MSG(("-->HandleRFPulseAmplitude"));

    STB_UpdateRFShapeAmplitude("ExcPulse1Ampl",No);
    ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]");

    DB_MSG(("<--HandleRFPulseAmplitude"));
  }
  @endcode

  @param rfpulseParn      name of a scalar parameter of type @c PVM_RF_PULSE
  @param acqRfShapeParn   name of one element of parameter @c ACQ_RfShapes
  @param acqPulseDurParn  (optional) name of a double parameter that is used to
                          control the pulse duration on pulseprogram level

  @return 0 in case of success

  @ingroup PVM_ATB
*/
DLL_INTERFACE int ATB_SetRFPulse(const char *const rfpulseParn,
                                 const char *const acqRfShapeParn,
                                 const char *const acqPulseDurParn=0);




/* /pv60/gen/src/prg/pvm_toolboxes/PvAcqTools/ArrayPhaseAcq.c */

/** Handles adjustment iterations for <em>Array Phase</em> adjustment.

  This function is called implicitly for methods using the <em>Array Phase</em>
  adjustment during each iteration.

  After adjustment of RG, signals on all channels are averaged
  until SNR threshold is reached, then relative phase corrections
  are calculated to equalise channels. If SNR is not reached in
  maximum number if averages, corrections are set to zero.

  @PVM_PARS
  - @c PVM_ArrayPhaseCounter
  - @c PVM_ArrayPhaseNSamp
  - @c PVM_ArrayPhaseSampOffset
  - @c GS_info_offset
  - @c PVM_ArrayPhaseRe
  - @c PVM_ArrayPhaseIm
  - @c PVM_ArrayPhaseZ2
  - @c PVM_ArrayPhase

  @PVM_TBCALL
  - @ref ATB_AdjCheckRg

  @PVM_RELCALL

  @PVM_PRECOND
  - @ref STB_ArrayPhaseAppendAdjustment must have been called.

  @PVM_ERROR
  NONE

  @see @ref STB_ArrayPhaseAppendAdjustment, @ref STB_ArrayPhaseHandleAdjustmentRequests, @ref ATB_ArrayPhaseSetRecoPhase

  @ingroup PVM_ATB_INT
*/
DLL_INTERFACE void ATB_ArrayPhaseCounterRels(void);




/** Transfers result of <em>Array Phase</em> adjustment to reco parameter.

  This function is to be called in @c SetRecoParam() of a method using the
  <em>Array Phase</em> adjustment.

  In case the values are not available, zero is used instead.

  @PVM_PARS
  - @c RecoPhaseChan

  @PVM_TBCALL
  - @c UT_InitDoubleArray

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - @ref STB_ArrayPhaseAppendAdjustment must have been called.

  @PVM_ERROR
  NONE

  @see @ref STB_ArrayPhaseAppendAdjustment, @ref STB_ArrayPhaseHandleAdjustmentRequests

  @ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_ArrayPhaseSetRecoPhase(void);




/** Delivers the actual state of shims. (deprecated).

  @PVM_USEINSTEAD{ATB_GetActualShim}

  @deprecated @ingroup PVM_DEPRECATED
*/

DLL_INTERFACE void ATB_GetActualShimsets(const char *const shimsetpar,
                                         const char *const actshimpar,
                                         PVSHIM_DATA_TYPE  dattyp,
                                         int *const        nshimsets=0);
/** Delivers the actual state of shims.

    It stores the shims that are currently active inside the shim hardware
    into a vector (unit %of max), entries according to the shim coil
    identifier. This function may be used only in adjustment relations
    (see preconditions below).

  @PVM_PARS
  NONE

  @PVM_TBCALL

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - call inside relations of
    - PVM_AdjHandler
    - PVM_AdjResultHandler
    - PVM_GopAdjResultHandler
    - relation of auto counter during GS_auto pipeline

  @PVM_ERROR
  - Call outside relations mentioned above

  @param sv vector that is dimensioned to the number of available shims
            entries set in % of max shim current for the coil identified
            by offset.
  @ingroup PVM_ATB

 */
DLL_INTERFACE void ATB_GetActualShim(std::vector<double> &sv);

/** Delivers the actual state of shims.

    It stores the shims that are currently active inside the shim hardware
    into a double parameter (unit %of max), entries according to the shim coil
    identifier. This function may be used only in adjustment relations
    (see preconditions below).


  @PVM_PARS
  NONE

  @PVM_TBCALL


  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - call inside relations of
    - PVM_AdjHandler
    - PVM_AdjResultHandler
    - PVM_GopAdjResultHandler
    - relation of auto counter during GS_auto pipeline


  @PVM_ERROR
  - Call outside relations mentioned above
  - illegal parameter type (no double array)
  @code
  // assuming double array parameter MyShim

  ATB_GetActualShim("MyShim");
  // myshim has now nshims (number of shims in system) entries
  // with values in [-100.0, 100.0]

  @endcode



  @param shimparn name of double array parameter that is dimensioned to
                  the number of available shims entries set in % of max
                  shim current for the coil identified by offset.
  @ingroup PVM_ATB

 */
DLL_INTERFACE void ATB_GetActualShim(const char *const shimparn);





/** Loads shim vector to shim unit.

    Loads shims from shim vector (entries % of max) to the shim hardware
    Entries control currents of the corresponding coils (according to the
    shim coil identifies). They are rounded according to the precision of
    the related hardware and stored back to the shim vector. If optional
    argument @a simulate is set to @c true
    only the rounding is done without changing shims currents on hardware
    level.

    This function may be used only in adjustment relations
    (see preconditions below).


  @PVM_PARS
  NONE

  @PVM_TBCALL

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - call inside relations of
    - PVM_AdjHandler
    - PVM_AdjResultHandler
    - PVM_GopAdjResultHandler
    - relation of auto counter during GS_auto pipeline

  @PVM_ERROR
  - Call outside relations mentioned above
  - illegal values of vector

  @PVM_EXAMPLE
  @code
  std::vector<double> shimvec;
  // -- retrive shim vector somehow: here set 2nd entry to 2.5%
  //
  int nshims=SysInfo_NShims();
  shimvec.assign(nshims,0.0);
  if(nshims > 1)
     shimvec[1]=2.5;
  ATB_LoadShim(shimvec);

  @endcode


  @param sv vector that is dimensioned to the number of available shims
            entries set in % of max shim current for the coil identified
            by offset.
  @param simulate (optional) if set to true the values are not loaded to hardware
  @ingroup PVM_ATB

 */
DLL_INTERFACE void ATB_LoadShim(std::vector<double> &sv, bool simulate=false);

/** Loads shim from parameter to shim unit.

    Loads shims from double array parameter (entries % of max)
    specified by name to the shim hardware.
    Entries control currents of the corresponding coils (according to the
    shim coil identifies). They are rounded to the precision of
    the related hardware and stored back to the parameter. If optional
    argument @a simulate is set to @c true only the rounding is done
    without changing shim currents on hardware level.

    This function may be used only in adjustment relations
    (see preconditions below).

  @PVM_PARS
  NONE

  @PVM_TBCALL
  - @ref ATB_LoadShim


  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  - call inside relations of
    - PVM_AdjHandler
    - PVM_AdjResultHandler
    - PVM_GopAdjResultHandler
    - relation of auto counter during GS_auto pipeline

  @PVM_ERROR
  - Call outside relations mentioned above
  - illegal values (entries not in [-100,100])
  - illegal array size
  - illegal parameter type (double array expected)

  @PVM_EXAMPLE
  @code
  // assuming parameter MyShimpar is a double array
  int nshims=SysInfo_NShims();
  PARX_change_dims("MyShimPar",nshims);
  memset(MyShimPar,0,nshims*sizeof(double));
  // set 2nd entry of shim to 2.5%
  if(nshims >1)
    MyShimPar[1]=2.5;

  // load this shim to hardware
  ATB_LoadShim("MyShimPar");



  @endcode


  @param shimparn name of double array parameter  that is dimensioned
                  to the number of available shims entries set in % of
                  max shim current for the coil identified
                  by offset.
  @param simulate (optional) if set to true the values are not loaded to hardware
  @ingroup PVM_ATB

 */
DLL_INTERFACE void ATB_LoadShim(const char *const shimparn, bool simulate=false);


/**
Updates the receiver gain of a selected job.

This general update routine assures information about the actual
values of ACQ_jobs receiverGain fields and provides access to the
signal amplification during setup experiments.
Depending on the value of the enum parameter @c PVM_RgJob,
the value (as shown in parameter @c PVM_RgValue) is transferred
to the corresponding baselevel parameter and used also as starting
value of a receiver gain adjustment for the selected job.
@c PVM_RgJob is used to select the job according to its title.

@PVM_PARS
- PVM_RgJob
- PVM_RgValue
Only if changed:
- PVM_AutoRg
- receiverGain fields of the corresponding @c ACQ_jobs entry.

@PVM_TBCALL


@PVM_RELCALL
ACQ_jobs[jndx].receiverGain


@PVM_PRECOND
NONE

@PVM_ERROR
NONE

@throws PvStdException in case of illegal state of ACQ_jobs

@ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_RgHandler();

DLL_INTERFACE void ATB_RgGroupHandler(bool readonly);

DLL_INTERFACE double ATB_GetAutoRgValue(const char *const title);
DLL_INTERFACE void ATB_SetAutoRg(const char *const title, double rgval);


/**
Sets up PVM_AutoRg

PVM_AutoRg is a container parameter that keeps the receiver gain values
of all ACQ_jobs that were activated once - even those which are
currently de-activated again and are not in ACQ_jobs anymore.
@c ATB_SetReceiverGains transfers the receiver gains from @c PVM_AutoRg
to @c ACQ_jobs. Elements of the @c PVM_AutoRg and @c ACQ_jobs arrays
belonging together are identified by their titles. Hence, it is advised
to use unique job titles when setting up jobs.
Whenever a new ACQ_job is created within the method (e.g. by
switching on a navigator) the size of PVM_AutoRg is increased.

@PVM_PARS
- PVM_AutoRg
- receiverGain fields of the corresponding @c ACQ_jobs array element.

@PVM_TBCALL
ATB_RgHandler

@PVM_RELCALL
ACQ_jobs[jndx].receiverGain


@PVM_PRECOND
ACQ_jobs are set up. In many methods the jobs are set up in function
@c SetAcquisitionParameters. Call @c ATB_SetReceiverGains after this
function.

@PVM_ERROR
NONE

@see @ref ATB_RgHandler

@ingroup PVM_ATB
*/
DLL_INTERFACE void ATB_SetReceiverGains(void);


/**
Inserts a reco filter stage that writes reco data to a PARX parmeter

Inserts the reco filter stages RecoTeeFilter, RecoCastFilter, and RecoParamterSink after a specified existing stage.
The RecoParameterSink writes the data points into the PARX parameter PVM_RecoDataPoints.
The dimension of PVM_RecoDataPoints is set to a given number of data points first.

@PVM_PARS
- PVM_RecoDataPoints

@PVM_PRECOND
Default reconstruction network needs to be set up before this function is called.

@PVM_ERROR
NONE

@ingroup PVM_ATB

  @param recoDataPointsRequired YesNo parameter that specifies if the filter stages are inserted
  @param npoints Required umber of (real valued) data points in PVM_RecoDataPoints. Must match the corresponding reco buffer.
  @param hook String parameter that specifies the reco stage after that the data points are extracted.

  @PVM_EXAMPLE
  @code
  ATB_GetRecoDataPoints(PVM_RecoDataPointsRequired,  PVM_EncNReceivers*RECO_inp_size[0]*2, "Q");
  @endcode


*/
DLL_INTERFACE void ATB_GetRecoDataPoints(YesNo recoDataPointsRequired, int npoints, const char *const hook);


/**
Translate a set of parallel lists defined "per coil element" into lists defined "per physical channel".
The translated lists are stored in a given PARX parameter, which can be used as PPG definition.

@PVM_ERROR
  - Empty parameter @a listPerElemIdx
  - Lists in @a listPerElemIdx have inconsistent size

@ingroup PVM_ATB

  @param chanNum        The logical channel number for which the lists are defined
  @param listPerElemIdx Parameter holding lists for each coil element index [0 ... numElemsPerChan]
  @param ppgParName     PARX parameter which is dimensioned and filled with the lists for each physical channel index

  @PVM_EXAMPLE
  @code
  ATB_FillParallelList(1, {{elem0_val0, elem0_val1, ...}, {elem1_val0, elem1_val1, ...}, ...}, "TwoDimDoublePar");
  @endcode
*/
DLL_INTERFACE void ATB_SetParallelList(const int chanNum,
                                       const std::vector<std::vector<double>>& listPerElemIdx,
                                       const std::string& parName);

/**
Translate a single list into lists defined "per physical channel", i.e. set the same list for
all used physical channels.
The translated lists are stored in a given PARX parameter, which can be used as PPG definition.

@PVM_ERROR
  - Empty parameter @a singleListForAllElems

@ingroup PVM_ATB

  @param chanNum               The logical channel number for which the lists are defined
  @param singleListForAllElems Parameter holding the list
  @param ppgParName            PARX parameter which is dimensioned and filled with the lists for each physical channel index

  @PVM_EXAMPLE
  @code
  ATB_FillParallelList(1, {val0, val1, ...}, "TwoDimDoublePar");
  @endcode
*/
DLL_INTERFACE void ATB_SetParallelList(const int chanNum,
                                       const std::vector<double>& singleListForAllElems,
                                       const std::string& parName);



#endif
#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
