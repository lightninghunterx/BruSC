/*
 *******************************************************************
 *
 * $Source: /sc/CvsTree/sc/gen/h/libHConfClient/HConfInterfaceC.h,v $
 *
 * Copyright (c) 2016
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 * $Id: HConfInterfaceC.h,v 1.10 2019/04/11 13:52:18 alexander.herbig Exp $
 *
 *******************************************************************
 */

#ifndef HCONF_IFC_C_H			       /* prevent multiple includes */
#define HCONF_IFC_C_H


#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

#ifdef	DLL_LINKAGE
#   ifdef libHConfClient_INTERNAL
#      define DLL_INTERFACE __declspec(dllexport)
#   else
#      define DLL_INTERFACE __declspec(dllimport)
#   endif
#else
#   define DLL_INTERFACE
#endif


#ifdef __cplusplus
extern "C" {
#endif

/*get the spectros version via Corba
 * cStringBuffer has to be provided / allocated by caller, contains the version string if successful
 * bufferLength is the size of cStringBuffer
 * returns 0 in case of success,
 * returns -1 if cStringBuffer==null or if its length is zero
 * returns -2 if cStringBuffer is to small
 * returns -3 if a communication error (HConfServer) occurred
 */
extern DLL_INTERFACE int getSpectrosVersion(char* cStringBuffer, const size_t bufferLength);


/*get corbaloc of BSMS proxy server via Corba
 * cStringBuffer has to be provided / allocated by caller, contains the corbaloc string if successful
 * bufferLength is the size of cStringBuffer
 * returns 0 in case of success,
 * returns -1 if cStringBuffer==null or if its length is zero
 * returns -2 if cStringBuffer is to small
 * returns -3 if a communication error (HConfServer) occurred
 */
extern DLL_INTERFACE int getCorbalocBsmsProxyServer(char* cStringBuffer, const size_t bufferLength);


/* copy Helium logfile from EPU to workstation.
 *
 * returns 0 in case of success,
 * returns -1 in case of an error.
 */
extern DLL_INTERFACE int copyHeliumLogfileFromEPUToWorkstation();


/* delete file on EPU
 *
 *
 * subDirPath:  e.g. /opt/topspin/prog/curdir/root
 *              To use TSHOME_EPU and SPECT_EPU omit the first '/'.
 *
 * filename: e.g. test.log
 *
 * dirToken: NONE = 0: default value
 *           TSHOME_EPU = 1: relative to topspin home folder on the EPU, e.g. /opt/topspin
 *           SPECT_EPU = 2: relative to topspin spect folder on the EPU, e.g. /opt/topspin/conf/instr/spect
 *
 * !!!To use TSHOME_EPU and SPECT_EPU the input parameter 'subDirPath' is not allowed to start with "/" !!!
 *
 * returns 0 in case of success,
 * returns -1 in case of an error.
 */

enum dirToken {NONE, TSHOME_EPU, SPECT_EPU, SCON2_EPU};

extern DLL_INTERFACE int deleteFileOnEPU(const char* subDirPath, const char* filename, const enum dirToken);


/*
 * delete folder on EPU
 *
 * subDirPath:  e.g. /opt/topspin/prog/curdir/root
 *             To use TSHOME_EPU and SPECT_EPU omit the first '/'.
 *
 * dirname: folder to delete e.g. testFolder
 *
 ** dirToken: NONE: default value
 *           TSHOME_EPU: relative to topspin home folder on the EPU, e.g. /opt/topspin
 *           SPECT_EPU: relative to topspin spect folder on the EPU, e.g. /opt/topspin/conf/instr/spect
 *
 ** !!!To use TSHOME_EPU and SPECT_EPU the input parameter 'subDirPath' is not allowed to start with "/" !!!
 **
 * returns 0 in case of success,
 * returns -1 in case of an error.
 */

extern DLL_INTERFACE int deleteFolderOnEPU(const char* dirPath, const char* dirname, const enum dirToken);


/*
 * create folder on EPU
 *
 * subDirPath:  e.g. /opt/topspin/prog/curdir/root
 *             To use TSHOME_EPU and SPECT_EPU omit the first '/'.
 *
 * dirname: folder to create e.g. testFolder
 *
 ** dirToken: NONE: default value
 *           TSHOME_EPU: relative to topspin home folder on the EPU, e.g. /opt/topspin
 *           SPECT_EPU: relative to topspin spect folder on the EPU, e.g. /opt/topspin/conf/instr/spect
 *
 ** !!!To use TSHOME_EPU and SPECT_EPU the input parameter 'subDirPath' is not allowed to start with "/" !!!
 **
 * returns 0 in case of success,
 * returns -1 in case of an error.
 */

extern DLL_INTERFACE int createFolderOnEPU(const char* subDirPath, const char* dirname, const enum dirToken token);


#ifdef __cplusplus
}
#endif
#endif
