/*
 *******************************************************************
 *
 * $Source: /pr/CvsTree/pr/gen/h/lib/par.h,v $
 *
 * Copyright (c) 1995
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: par.h,v 1.73.2.1 2020/03/31 11:52:30 tilman.sauerbeck Exp $
 *
 *******************************************************************
*/

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

#if defined(DLL_LINKAGE) && !defined(CPROTO)
#   ifdef par_INTERNAL
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

struct pnamtyp;

extern DLL_INTERFACE int getpar(const char *, const char *, void *);
extern DLL_INTERFACE int putpar(const char *, const char *, const void *);
extern DLL_INTERFACE int writebuf(const char *, const char *, size_t);
extern DLL_INTERFACE int delpar(const char *);
extern DLL_INTERFACE int getpar_nowait(const char *, const char *, void *);
extern DLL_INTERFACE int putpar_nowait(const char *, const char *, const void *);
extern DLL_INTERFACE int jcwrite_class(FILE *, const char *, const void *);
extern DLL_INTERFACE int jcwrite_string(FILE *, const char *, const void *);
extern DLL_INTERFACE struct pnamtyp* parGet2nd(struct pnamtyp*);
extern DLL_INTERFACE const struct pnamtyp* parConstGet2nd(const struct pnamtyp*);
extern DLL_INTERFACE const struct pnamtyp* parConstGetEnd2nd(const struct pnamtyp*);
extern DLL_INTERFACE const char* par_err(int);
extern DLL_INTERFACE const char* GetHost(void);
extern DLL_INTERFACE const char* GetUser(void);
extern DLL_INTERFACE const char* GetDomainAndUser(void);
extern DLL_INTERFACE void StddDu(char*);
extern DLL_INTERFACE size_t StrfLocalTimeMillisec(char*, size_t, const char*)
#ifdef	__GNUC__
			    __attribute__ ((format (strftime, 3, 0)))
#endif
									;
extern DLL_INTERFACE size_t Iso8601XmlDateTime(char*, size_t);

extern DLL_INTERFACE int AuditCreate(const char*, const char*);

/**
 * Identical to AuditCreate(), except that the system user name as well as
 * the hostname are not determined automatically, but set to @p system_user
 * and @p hostname respectively.
 */
extern DLL_INTERFACE int AuditCreateWithUserAtHost(const char*, const char*, const char*, const char*);
extern DLL_INTERFACE int AuditAppend(const char*, const char*);

/**
 * Identical to AuditAppend(), except that the system user name as well as
 * the hostname are not determined automatically, but set to @p system_user
 * and @p hostname respectively.
 */
extern DLL_INTERFACE int AuditAppendWithUserAtHost(const char*, const char*, const char*, const char*);
extern DLL_INTERFACE int AuditAppendComp(const char*, const char*, const char*);
extern DLL_INTERFACE int AuditAppendCheck(const char*, const char*, const char*, int);
extern DLL_INTERFACE int AuditVerify(const char*);
extern DLL_INTERFACE int AuditCopy(const char*, const char*, const char*);
extern DLL_INTERFACE int AuditCopyComp(const char*, const char*, const char*,
				       const char*);
extern DLL_INTERFACE int AuditCopyCheck(const char*, const char*, const char*,
					const char*, int);
extern DLL_INTERFACE int AuditGetEntry(const char*, char*, int*, int);
extern DLL_INTERFACE int AuditGetFullEntry(const char*, char*, int*, int);
extern DLL_INTERFACE int AuditGetLastEntry(const char*, char*, int*);
extern DLL_INTERFACE int AuditGetLastFullEntry(const char*, char*, int*);
extern DLL_INTERFACE int AuditGetLastEntryTimeStamp(const char*, char*);
extern DLL_INTERFACE const char* AuditHashType(void);
extern DLL_INTERFACE const char* AuditVerifyError(int);

extern DLL_INTERFACE char* AuditOpenHash(void);
extern DLL_INTERFACE int   AuditPrintHash(char*, char*, int);
extern DLL_INTERFACE void  AuditWriteCharHash(char*, int);
extern DLL_INTERFACE void  AuditWriteHash(char*, const void*, size_t, int, size_t);
extern DLL_INTERFACE void  AuditCopyHash(const char*, char*);
extern DLL_INTERFACE void  AuditCloseHash(char*);
extern DLL_INTERFACE void  UpdateTopspinUser(void);
#ifdef __cplusplus
}
#endif

/* all pstruc_dim8 parameters */

#define PAR_ACQU_O "ACQT0/BF1/FnILOOP/FnMODE/NUC1/NusJSP/NusT2/NusTD/O1/ProjAngle/SFO1/SW/SW_h/TD"

/* AuditCopyCheck() non error return values */

#define PAR_ACC_NO_CHECKSUM	  1
#define PAR_ACC_INVALID_CHECKSUM  2
#define PAR_ACC_INVALID_AUDITFILE 3

/* getpar()/putpar() error return values */

#define PAR_NO_FILE		-1
#define PAR_CREATE_ERROR	-2
#define PAR_NO_MEM		-3
#define PAR_READ_ERROR		-4
#define PAR_WRITE_ERROR		-5
#define PAR_P_NOT_IN_FILE	-6
#define PAR_P_NOT_IN_STRUC	-7
#define PAR_FNAME_ERROR		-8
#define PAR_OPEN_ERROR		-9
#define PAR_NO_CLASS	       -10
#define PAR_NO_BINARY	       -11
#define PAR_INVALID_STRUC      -12
#define PAR_FILE_BUSY	       -13
#define PAR_NO_LOCK	       -14
#define PAR_UNKNOWN_FORMAT     -15
#define PAR_NO_PERM	       -16
#define PAR_WRONG_BYTEORDER    -17
#define PAR_ZERO_SIZE	       -18
#define PAR_FOPEN_ERROR        -19
#define PAR_FILE_TOO_MANY_P    -20
#define PAR_UNALIGNED_STRUC    -21
#define PAR_FCLOSE_ERROR       -22
#define PAR_OVERFLOW	       -23
#define PAR_LOCKED	       -24
#define PAR_LOCK_INTERRUPTED   -25
#define PAR_READONLY	       -29
