/*
 *******************************************************************
 *
 * $Source: /sc/CvsTree/sc/gen/h/lib/libacq.h,v $
 *
 * Copyright (c) 1998
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 * $Id: libacq.h,v 1.131 2019/08/02 15:05:06 ah Exp $
 *
 *******************************************************************
 */
/*:=INFO=:**************************************************************
 *
 * Header : libacq.h
 *
 * Description : interface to library libacq.a
 *
 *::=info=:*************************************************************/

#ifndef LIB_ACQ_H			/* prevent multiple includes */
#define LIB_ACQ_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif
#if defined(DLL_LINKAGE) && !defined(CPROTO)
#   ifdef libacq_INTERNAL
#      define DLL_INTERFACE __declspec(dllexport)
#   else
#      define DLL_INTERFACE __declspec(dllimport)
#   endif
#else
#   define DLL_INTERFACE
#endif

/*** necessary includes ***/

/*** definitions ***/
#define LIBACQ_MAXLIST 0x60			   /* must be a multiple of 6 */


/*** types and enums ***/

#include <stddef.h>
#include <lib/LibAcqDef.h>


/*** forward declarations (avoid unnecessary includes) ***/

struct ConfHptxInfo;	/* in <acqu/confstruc.h> */
struct ConfAcbTcamInfo;	/* in <acqu/confstruc.h> */
struct all_pars;
struct spec_pars;
struct scon_pars;
struct probe_pars;
struct gspar;
struct memload;
struct processControlBlock;	/* in <lib960/I960CmSystemTables.h> */
struct intrTable;		/* in <lib960/I960CmSystemTables.h> */
struct controlTable;		/* in <lib960/I960CmSystemTables.h> */
struct talkie;			/* in <lib960/I960CmSystemTables.h> */

/*** variables ***/

	/* for error handling of load_to_mem() */
extern DLL_INTERFACE char load_to_mem_err [];
extern DLL_INTERFACE int load_to_mem_errno;

/*** function prototypes ***/
extern DLL_INTERFACE void AcqInterfaceInit (void);

/* /usr/people/ge/dvl/src/lib/libacq/CreateI960InitFile.c */
extern DLL_INTERFACE void
AcqCIIFHandleOptions
(
    const char *cpBoardName,
    const char *cpProgName,
    const char *cpFirmware,
    const char *cpInitFile,
    struct intrTable *spIntrTable,
    int argc,
    const char *argv[]
);

extern DLL_INTERFACE void AcqCIIFFillIntrTable
(
    const char *cpBoardName,
    const char *cpProgName,
    const char *cpFirmware,
    const char *cpInitFile,
    const char *cpIntrAddress,
    const char *cpIntrVector,
    unsigned int uiIntrVector,
    struct intrTable *spIntrTable
);

extern DLL_INTERFACE void AcqCIIFWriteAllTablesToFile
(
    const char *cpBoardName,
    const char *cpProgName,
    const char *cpFirmware,
    const char *cpInitFile,
    struct processControlBlock *spPRCB,
    struct intrTable *spIntrTable,
    struct controlTable *spControlTable,
    struct talkie *spTalkie
);

/* /usr/people/ge/dvl/src/lib/libacq/editnucl.c */
extern DLL_INTERFACE int editnucl(const char*, double, int);

/* /usr/people/ge/dvl/src/lib/libacq/error.c */
extern DLL_INTERFACE const char *LibAcqGetErrMsg(void);

/* /usr/people/ge/dvl/src/lib/libacq/GetRoutOut.c */
extern DLL_INTERFACE int LibAcqGetRouterOutput(const struct
					       ConfAcbTcamInfo*, int);

/* /usr/people/ge/dvl/src/lib/libacq/GetTcamIdx.c */
extern DLL_INTERFACE int LibAcqGetTcamIdx(const struct ConfAcbTcamInfo*,
					  const struct ConfHptxInfo*,
					  int, const int*);

/* /usr/people/ge/dvl/src/lib/libacq/gcu2.c */
extern DLL_INTERFACE const char *AcqGcu2GetYBusCmdName(int);

/* /usr/people/ge/dvl/src/lib/libacq/gcu3.c */
extern DLL_INTERFACE const char *AcqGcu3GetFBusCmdName(int);

/* /usr/people/ge/dvl/src/lib/libacq/getprobehead.c */
/* returns both upper and lower temperature limit
 * (as integer values with unit 'Celsius') of
 * the currently installed probe;
 * returns 0 if everything is OK and -1 in case of error;
 * the respective error text can be retrieved using
 * getProbeheadInfoError();
 */
extern DLL_INTERFACE int LibAcqGetProbeTempRange(int* maxTempC,
						 int* minTempC);

/* /usr/people/ge/dvl/src/lib/libacq/getprobehead.c */
/* returns both upper and lower temperature limit
 * (as integer values with unit 'Celsius') of the cooling
 * air stream for an ET probe;
 * returns 0 if everything is OK and -1 in case of error;
 * the respective error text can be retrieved using
 * getProbeheadInfoError();
 */
extern DLL_INTERFACE int LibAcqGetCoolingAirTempRange(int* maxTempC,
						      int* minTempC);

/* /usr/people/ge/dvl/src/lib/libacq/getprobehead.c */
/* returns upper temperature limit
 * (as integer value with unit 'Celsius') of
 * the currently installed probe;
 * returns 0 if everything is OK and -1 in case of error;
 * if no other value is specified by the probe the default value '-274' is returned;
 * the respective error text can be retrieved using
 * getProbeheadInfoError();
 */
extern DLL_INTERFACE int LibAcqGetProbeHeaterLimit(int* maxTempC);

/** find out whether current probe is a cryo probe
    \return 1 if yes
    \return 0 if no
    \return -1 in case of error. Error text can be retrieved using getProbeheadInfoError().
 */
extern DLL_INTERFACE int LibAcqIsCryoProbe(void);

/** find out whether current probe is a cryo probe
    \return 1 if yes
    \return 0 if no
    \return -1 in case of error. Error text can be retrieved using getProbeheadInfoError().
 */
extern DLL_INTERFACE int LibAcqCryoProbeInstalled(void);

extern DLL_INTERFACE int LibAcqIsMasProbe(void);
extern DLL_INTERFACE int LibAcqAtmaProbeInstalled(void);

/** find out whether probe is a quadrature probe usable for parallel acquisition

    This means that a certain nucleus may be observed on two different coils simultaneously.
    \return 1 if a quadrature probe is installed
    \return 0 when quadrature probe is not installed
    \return -1 in case of error; call getProbeheadInfoError() to get the error message
 */
extern DLL_INTERFACE int LibAcqQuadratureProbeInstalled(void);

extern DLL_INTERFACE int LibAcqProbeXYZ(void);

/*
 get first nucleus of coil coilNo [1...];
 in case of error -1 is returned;
 in case no nucleus can be found -1 is returned;
 in case of success 0 is returned;
 error message could be retrieved using getProbeheadInfoError();
 the returning nucleus may also be 'BB';
 */
extern DLL_INTERFACE int LibAcqGetFirstNucleusOfCoil(const unsigned int coilNo,
						     char* nuc,
						     const unsigned int nucLen);


/*
returns whether coil coilNo[1...] is a BB coil;
the method returns '1' if this is the case, otherwise '0' is returned;
in case of error '-1' is returned;
error message could be retrieved using getProbeheadInfoError();
*/
extern DLL_INTERFACE int LibAcqIsProbeBBCoil(const unsigned int coilNo);

/*
returns whether coil coilNo[1...] is a QNP coil;
the method returns '1' if this is the case, otherwise '0' is returned;
in case of error '-1' is returned;
error message could be retrieved using getProbeheadInfoError();
*/
extern DLL_INTERFACE int LibAcqIsProbeSWITCHCoil(const unsigned int coilNo);


/*
get coil number [1...] for nucleus nucleus; return -1 in case of error;
nucleus could also be 'BB';
 error message could be retrieved using getProbeheadInfoError();
 */
extern DLL_INTERFACE int LibAcqGetCoilForNuc(const char* nucleus);
extern DLL_INTERFACE int getProbeheadInfo(int *, char *);
extern DLL_INTERFACE int getCurrentProbeId(char* curProbeId, size_t bufferSize);
extern DLL_INTERFACE int LibAcqGetInstalledProbeName(char *);
extern DLL_INTERFACE int getProbeheadPicsInfo(char *);
extern DLL_INTERFACE const char *getProbeheadInfoError(void);

extern DLL_INTERFACE int LibAcqGetNumCoils(void);

/*
  returns all nuclei separated by an empty space that are part of the
  BB group of the PICS; if this group does not exist an empty string
  is returned; in case of error -1 is returned, else 0 is returned;
  error message could be retrieved using getProbeheadInfoError();
  the returning nucleus may also be 'BB';
*/
extern DLL_INTERFACE int LibAcqGetBBNucRange(char*, unsigned int maxLen);
/*
 * find out whether nucleus testNuc is part of the nucleus range nucRange;
 * nucRange may contain tokens like
 * '{}', '/', '-' etc, e.g. '13C-15N/3H-19F' or '31P-15N/{13C-2H}';
 * this method returns '1' if testNuc is part of nucRange, '0' if this is not
 * the case, '-1' in case of error. The respective error text can be retrieved using
 * getProbeheadInfoError();
*/
extern DLL_INTERFACE int LibAcqNucIsInRange(const char* nucRange, const char* testNuc);

/*
 * return all nuclei residing in nucRange;
 * nucRange may contain tokens like
 * '{}', '/', '-' etc, e.g. '13C-15N/3H-19F' or '31P-15N/{13C-2H}';
 * the returning character array contains all nuclei separated with white space,
 * e.g. '13C 31P 29Si'
 * this method returns '0' in case of success and '-1' in case of error;
 * the respective error text can be retrieved using
 * getProbeheadInfoError();*/
extern DLL_INTERFACE int LibAcqGetNucleiForToken(const char* nucRange,
						 char*,
						 const unsigned int maxLen);

/* getpulse.c */
extern DLL_INTERFACE int getProsolPulse(const char *, const char *,
					double *, double *, double *);
extern DLL_INTERFACE const char *getProsolPulseError(int);

/* loadcoprog.c */
extern DLL_INTERFACE void memload_init(struct memload *);
extern DLL_INTERFACE void memload_free(struct memload *);
extern DLL_INTERFACE int load_to_mem(const char *, int,
				     struct memload *);

/* setroutwd.c */
extern DLL_INTERFACE void SetRouterCtrlWords(int *, int *, int *,
					     const char *, const char *,
					     const char *,
					     int, int, int,
					     int, int, int,
					     double, double, double);


/* acb_tcam.c */
extern DLL_INTERFACE int acb_tcam_get_amp_input(unsigned char);

/* SwiBox.c */
extern DLL_INTERFACE AcqSwiBox LibAcqGetSwiBoxType(const struct ConfAcbTcamInfo*);
extern DLL_INTERFACE int LibAcqGetSwiBoxCode(AcqSwiBox);

#ifdef __cplusplus
}
#endif
#endif /* LIB_ACQ_H */
/***********************************************************************/
/*			     E N D   O F   F I L E                     */
/***********************************************************************/
