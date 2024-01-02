/*
 *******************************************************************
 *
 * $Source: /sc/CvsTree/sc/gen/h/libReadConf/ReadConfC.h,v $
 *
 * Copyright (c) 2014
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 * $Id: ReadConfC.h,v 1.21.4.1 2020/03/12 15:16:58 Marina.Maurer Exp $
 *
 *******************************************************************
 */

/** C wrapper functions for the ReadConf class
 * For description of functions see ReadConf.h */

#pragma once

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

#ifdef	DLL_LINKAGE
#   ifdef libReadConf_INTERNAL
#      define DLL_INTERFACE __declspec(dllexport)
#   else
#      define DLL_INTERFACE __declspec(dllimport)
#   endif
#else
#   define DLL_INTERFACE
#endif


#ifdef	__cplusplus
extern "C"
{
#endif


struct config;		/* in <acqu/confstruc.h> */
struct conf_pars;


/** CfToPar.c */
extern DLL_INTERFACE void LibAcqCfToPar (struct config *, struct conf_pars *);

/** ParToCf.c */
extern DLL_INTERFACE void LibAcqParToCf(struct config *, const struct conf_pars *);
extern DLL_INTERFACE void LibAcqSetForIcr(void);
extern DLL_INTERFACE void LibAcqClearConfigCache(void);
extern DLL_INTERFACE int libReadconf(struct config *);
extern DLL_INTERFACE int libReadconfInstrum(struct config *, const char *);
extern DLL_INTERFACE const char* ReadconfGetErrMsg(void);

/** AcqLock.c */
extern DLL_INTERFACE int LibAcqLHpprDispTunLedNum(void);
extern DLL_INTERFACE int LibAcqLHpprDispMatLedNum(void);
extern DLL_INTERFACE int LibAcqLBsmsUsed(void);
extern DLL_INTERFACE int LibAcqLBsmsType(void);
extern DLL_INTERFACE int LibAcqLBsmsCanLock19F(void);
extern DLL_INTERFACE int LibAcqLHasBsmsAux(void);
extern DLL_INTERFACE int LibAcqLBsmsIsLockAvailable(void);
extern DLL_INTERFACE int LibAcqLBmpcUsed(void);
extern DLL_INTERFACE int LibAcqLMasType(void);
extern DLL_INTERFACE int LibAcqLVtuType(void);
extern DLL_INTERFACE int LibAcqLCrcoUsed(void);
extern DLL_INTERFACE int LibAcqLGetInstalledDigitizer(void);
extern DLL_INTERFACE int LibAcqLGetRgtype(void);
extern DLL_INTERFACE int LibAcqLGetDigtype(int);
extern DLL_INTERFACE int LibAcqLIsBirds(void);
extern DLL_INTERFACE int LibAcqLIsAvance(void);
extern DLL_INTERFACE int LibAcqLIsIPSO(void);
extern DLL_INTERFACE int LibAcqLIsBladeSystem();
extern DLL_INTERFACE int LibAcqLIsFourier();
extern DLL_INTERFACE int LibAcqLIsDataStation();
extern DLL_INTERFACE int LibAcqLIsNmrInstr();
extern DLL_INTERFACE int LibAcqLIsIcrInstr();
extern DLL_INTERFACE double LibAcqLGetConfBfreq(void);
extern DLL_INTERFACE double LibAcqLGetGradres(void);
extern DLL_INTERFACE double LibAcqLGetBFreqForNuc(const char *, const int);
extern DLL_INTERFACE int LibAcqLGetRouterType(void);
extern DLL_INTERFACE int LibAcqLGetNumRouters(void);
extern DLL_INTERFACE int LibAcqLHasBltx300(void);
extern DLL_INTERFACE int LibAcqLDefaultAmpConfigurationUsed(void);
extern DLL_INTERFACE int LibAcqLGetComdig(void);
extern DLL_INTERFACE int LibAcqLGetSyflag(void);
extern DLL_INTERFACE int LibAcqLNumReceivers(void);
extern DLL_INTERFACE int LibAcqLHasPicsConnector(void);
extern DLL_INTERFACE int LibAcqLHasMagnetTempSensor(void);
extern DLL_INTERFACE int LibAcqLGetSguType(void);
extern DLL_INTERFACE int LibAcqLPowchkActive(void);
extern DLL_INTERFACE int LibAcqLPrampTp(void);
extern DLL_INTERFACE const char* LibAcqLGetFunctionModulePtr(void);
extern DLL_INTERFACE int LibAcqLHasADM(void);
extern DLL_INTERFACE int LibAcqLNumADMCRPs(void);
extern DLL_INTERFACE int LibAcqLHasHpprPreamp(void);
extern DLL_INTERFACE int LibAcqLGetHpprCoverType(void);
extern DLL_INTERFACE int LibAcqLGetPsdVersion(int);
extern DLL_INTERFACE double LibAcqLGetMaxWobbFreq(int, int);
extern DLL_INTERFACE double LibAcqLGetMinWobbFreq(int, int);
extern DLL_INTERFACE double LibAcqLGetMaxWobbSweep(int, int);
extern DLL_INTERFACE int LibAcqHighPowerStageUsed(const int, const int*, int);
extern DLL_INTERFACE int LibAcqLVtuInstalled(void);
extern DLL_INTERFACE int LibAcqLHasDRU(void);
extern DLL_INTERFACE int LibAcqLHasDPP(void);
extern DLL_INTERFACE double LibAcqGetRgMax(unsigned int);
extern DLL_INTERFACE double LibAcqGetRgval(int, int, double);
extern DLL_INTERFACE double LibAcqGetMergedRgval(int, double);
extern DLL_INTERFACE double LibAcqGetMergedRgMin(void);
extern DLL_INTERFACE double LibAcqGetMergedRgMax(void);
extern DLL_INTERFACE unsigned int LibAcqGetMergedGainTable(double **);
extern DLL_INTERFACE int LibAcqGetRecChassisId(int);
extern DLL_INTERFACE int LibAcqGetRecSbsbAddress(int);
extern DLL_INTERFACE unsigned int LibAcqGetRoutHWCheckSum(void);
extern DLL_INTERFACE unsigned int LibAcqGetRoutHWCheckSumTS20PV(void);

/**
   get group delay of DRX; filterType corresponds to HW_FILTER_TYPE in gen/h/acqutyp.h;
   returns -1.0 in case of error, call ReadConf::ReadconfGetErrMsg() to get the error message
*/
extern DLL_INTERFACE double LibAcqGetDrxGroupDelay(const unsigned filterType);

/** ReadCf2.c */
extern DLL_INTERFACE int readconf(struct config *);

/** ReadCf2.c */
extern DLL_INTERFACE int readconfInstrum(struct config *, const char *);

/** ReadCf2.c */
extern DLL_INTERFACE int bsms_used(void);

/** ReadCf2.c */
extern DLL_INTERFACE int LibAcqGetRgtype(void);

/** ReadCf2.c */
extern DLL_INTERFACE int LibAcqGetDigtype(int);

/** ReadCf2.c */
extern DLL_INTERFACE int is_blade_system();

/** ReadCf2.c */
extern DLL_INTERFACE int is_ipso(void);

/** ReadCf2.c */
extern DLL_INTERFACE int is_avance(void);

/** ReadCf2.c */
extern DLL_INTERFACE int isDataStation(void);

/** ReadCf2.c */
extern DLL_INTERFACE double getConfBfreq(void);

/** ReadCf2.c */
extern DLL_INTERFACE int LibAcqIsAmxWithArxFormatFiles(void);

/** ReadCf2.c */
extern DLL_INTERFACE int LibAcqIsAmxWithLinearXTransmitter(void);

/** ReadCf2.c */
extern DLL_INTERFACE int has_bltx300(void);

/** ReadCf2.c */
extern DLL_INTERFACE int LibAcqGetComdig(void);

/** ReadCf2.c */
extern DLL_INTERFACE int LibAcqGetSyflag(void);

/** ReadCf2.c */
extern DLL_INTERFACE int LibAcqVtuInstalled(void);
extern DLL_INTERFACE const char* LibAcqLGetBsmsAddr(void);
extern DLL_INTERFACE const char* LibAcqLGetBgaAddr(unsigned int);
extern DLL_INTERFACE const char* LibAcqLGetBsaAddr(unsigned int);
extern DLL_INTERFACE const char* LibAcqLGetFCubeDrxDevicePath(const unsigned int);
extern DLL_INTERFACE const char* LibAcqLGetFCubeRtdDevicePath(const unsigned int);
extern DLL_INTERFACE const char* LibAcqLGetGCubeRtdDevicePath(const unsigned int);
extern DLL_INTERFACE const char* LibAcqLGetGCubeProcDevicePath(const unsigned int);
extern DLL_INTERFACE const char* LibAcqLGetGpscuTty(void);
extern DLL_INTERFACE const char* LibAcqLGetRFSuperVisorTty(void);

extern DLL_INTERFACE int LibAcqLIsCmrSpectrometer();

#ifdef __cplusplus
}
#endif



/***********************************************************************/
/*			     E N D   O F   F I L E                     */
/***********************************************************************/
