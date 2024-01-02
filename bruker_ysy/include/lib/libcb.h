 /*
 *******************************************************************
 *
 * $Source: /bscl/CvsTree/bscl/gen/h/lib/libcb.h,v $
 *
 * Copyright (c) 1995
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 * $Id: libcb.h,v 1.26 2017/10/25 08:28:28 wem Exp $
 *
 *******************************************************************
 *	Definitions for the Bruker C Library
 *
 *	Library:	.../bscl/gen/lib/libcb.a
 *	Source:		.../bscl/gen/src/lib/libcb/
 *	Documentation:	.../bscl/gen/docu/lib/libcb/
 *******************************************************************
 */

/*** necessary includes ***/

#include <stdio.h>		/* to know 'FILE', and to prevent include
				 * of stdio.h after libcb.h  */
#include <stdarg.h>		/* to know 'va_list' */
#include <values.h>		/* to know HIBITI/MININT */
#include <sys/types.h>		/* to know 'size_t' */

/******************************************************************/
/******************************************************************/
/*****	#define :  all identifiers should start with "CB_"    *****/
/******************************************************************/
/******************************************************************/

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif
#ifdef	DLL_LINKAGE
#   ifdef libcb_INTERNAL
#	define DLL_INTERFACE __declspec(dllexport)
#   else
#	define DLL_INTERFACE __declspec(dllimport)
#   endif
#else
#   define DLL_INTERFACE
#endif

#ifndef HIBITI
#    define HIBITI MININT	/* HIBITI (not on Linux) is the   */
#endif				/* same as MININT (only on Linux) */

/******************************************************************/
/*	R E T U R N   V A L U E S				  */
/******************************************************************/

/******************************************************************/
/*	E R R O R   C O D E S					  */
/******************************************************************/

				   /* error codes for CrY_open () */
#define	DB_CrY_NO_PERMISSION	-1
#define	DB_CrY_FILE_NOT_FOUND	-2
#define	DB_CrY_NO_SPACE		-3
#define	DB_CrY_NO_MEMORY	-4
#define	DB_CrY_CRYPT_FAILED	-5

/******************************************************************/
/*	E R R O R   M E S S A G E   M A C R O S			  */
/******************************************************************/

/******************************************************************/
/*	E R R O R   M E S S A G E   B U F F E R   S I Z E S	  */
/******************************************************************/

#define CrY_ERRSTR_SIZE		200	/* size for CrY_errstr	  */

/******************************************************************/
/*	F U R T H E R   T O K E N S				  */
/******************************************************************/

#define	CB_TIMESZ	24	/* time text buffer size	  */

/******************************************************************/
/*	E X T E R N   E R R O R   M E S S A G E   A R R A Y S	  */
/******************************************************************/

/******************************************************************/
/*	T Y P E D E F S						  */
/******************************************************************/

/******************************************************************/
/*	O T H E R   D E F I N I T I O N S			  */
/******************************************************************/

#if defined(HAS_WINDOWS_FEATURES) || defined(WIN_NT)
/* on Windows NT snprintf() and vsnprintf() exist with underscore */
/* This is safe here as we include stdio.h first (see above).     */
#ifdef	_MSC_VER
#if	_MSC_VER < 1900
#	define snprintf _snprintf
#	define vsnprintf _vsnprintf
#endif	/* _MSC_VER < 1900	*/
#endif	/* _MSC_VER		*/
#endif

/******************************************************************/
/*	E X T E R N A L   V A R I A B L E S			  */
/******************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

DLL_INTERFACE extern const char*	smalloc_err;
DLL_INTERFACE extern const char*	sfree_err;

/******************************************************************/
/*	E X T E R N A L   F U N C T I O N   P R O T O T Y P E S	  */
/******************************************************************/

/*** new functions ***/

DLL_INTERFACE extern int angle(int);
DLL_INTERFACE extern int dircmp(const char *, const char *);
DLL_INTERFACE extern char *dircmp_err(int);
DLL_INTERFACE extern int dircp(const char *, const char *);
DLL_INTERFACE extern const char *dircp_err(int);
DLL_INTERFACE extern char *fgetsNoCr(char *, int, FILE *);
DLL_INTERFACE extern float fldiff(float, float);
DLL_INTERFACE extern char getcopt(const char *);
DLL_INTERFACE extern int iceil(float);
DLL_INTERFACE extern int iceild(double);
DLL_INTERFACE extern int ifloor(float);
DLL_INTERFACE extern int ifloord(double);
DLL_INTERFACE extern int iround(double);
DLL_INTERFACE extern int isfloat(float *);
DLL_INTERFACE extern int isflone(float);
DLL_INTERFACE extern int isodigit(int);
DLL_INTERFACE extern int ldfile(const char *);
DLL_INTERFACE extern char *ldfile_err(int, const char *);
DLL_INTERFACE extern int local_endian(void);
DLL_INTERFACE extern void local_swap(void *, const size_t, const size_t, const int);
DLL_INTERFACE extern void local_swap2(register void *, const int, const int);
DLL_INTERFACE extern void local_swap4(register void *, const int, const int);
DLL_INTERFACE extern void local_swap8(register void *, const int, const int);
DLL_INTERFACE extern void matmul(int, int, int, const double *, const double *, double *);
DLL_INTERFACE extern char *memocpy(char *, char *, int);
DLL_INTERFACE extern int mv(const char *, const char *);
DLL_INTERFACE extern char *mv_err(int);
DLL_INTERFACE extern int now(char *);
DLL_INTERFACE extern double parsnum(char *, char **);
DLL_INTERFACE extern int poll_iflag(volatile const int *const , const int, const int);
DLL_INTERFACE extern int rddb532(const char *);
DLL_INTERFACE extern short rddb532sh(const char *);
DLL_INTERFACE extern unsigned short rddb532ush(const char *);
DLL_INTERFACE extern char *rdfloat(char *, float *);
DLL_INTERFACE extern int rdint(char *);
DLL_INTERFACE extern int proc_active(const char *);
DLL_INTERFACE extern const char *proc_active_errmsg(void);
DLL_INTERFACE extern int sfree(char **);
DLL_INTERFACE extern int smalloc(char **, unsigned int);
DLL_INTERFACE extern unsigned int ssleep(unsigned int);
DLL_INTERFACE extern char *Str2FormatStr(const char *, char *, unsigned int);
DLL_INTERFACE extern char *strdfws(char *);
DLL_INTERFACE extern char *strdlfws(char *);
DLL_INTERFACE extern char *strdlws(char *);
DLL_INTERFACE extern int stricmpi_(const char *, const char *);
DLL_INTERFACE extern int strnicmpi_(const char *, const char *, size_t);
DLL_INTERFACE extern int swap_int(const int, const int);
DLL_INTERFACE extern short swap_short(const int, const int);
DLL_INTERFACE extern int SystemNat(const char *);
DLL_INTERFACE extern int xdigval(int);

/*** replacements of missing functions ***/

#if defined(HAS_NO_ROUND)
DLL_INTERFACE extern double round(double);
#endif
#if defined(HAS_NO_SNPRINTF) || defined(RISCOS_4_SYSV) || defined(IRIX_5)
DLL_INTERFACE extern int snprintf(char *s, size_t n, const char *format, ...);
#endif
#if defined(HAS_NO_VSNPRINTF) || defined(RISCOS_4_SYSV) || defined(IRIX_5)
#include <stdarg.h> /* to know va_list */
DLL_INTERFACE extern int vsnprintf(char*, size_t, const char*, va_list);
#endif
#if defined(HAS_NO_STRCASECMP) || defined(RISCOS_4_SYSV) || defined(ASX32_3_OS) || defined(WIN_NT)
DLL_INTERFACE extern int strcasecmp(const char *s1, const char *s2);
DLL_INTERFACE extern int strncasecmp(const char *s1, const char *s2, size_t n);
#endif

#if defined(HAS_NO_ANSI_C_LIBRARY) || defined(RISCOS_4_SYSV) || defined(ASX32_3_OS)
DLL_INTERFACE extern char *strerror (int errnum);
DLL_INTERFACE extern char *strstr(const char* str, const char* sub);
DLL_INTERFACE extern int mkfifo(const char *path, mode_t mode);
struct tm;			/* forward declaration for mktime() */
DLL_INTERFACE extern time_t mktime (struct tm *timeptr);
#endif
#if !defined(HAS_STRLWR) && !defined(WIN_NT)
DLL_INTERFACE extern char *strlwr(char *s);
#endif
#if !defined(HAS_STRUPR) && !defined(WIN_NT)
DLL_INTERFACE extern char *strupr(char *s);
#endif


/*** replacements of broken functions ***/

#if defined(HAS_BROKEN_SYSTEM) || defined(RISCOS_4_SYSV)
DLL_INTERFACE extern int system(const char* s);
#endif
#if defined(HAS_BROKEN_POPEN) || defined(RISCOS_4_SYSV)
DLL_INTERFACE extern FILE* popen(const char* cmd, const char* mode);
#endif

/** return string containing system error message related to specified system error code 'systemErrnoe'.

   This is a replacement for the thread safe strerror_r() function, which unfortunately exists in
   different interface versions.

   Our version always fills the specified buffer and returns a pointer to this buffer, and
   it always contains a meaningful error message, even if strerror_r() itsself fails.

   Attention: if the platform has no strerror_r() function then strerror() is used which is not thread safe!

   \param systemErrno		the system error code 'errno'
   \param messageBuffer		the buffer used to store the error message
   \param messageBufferSize	the size of this buffer
   \return	a pointer to the begin of 'buf'
 */
DLL_INTERFACE extern char* Strerror_r(int systemErrno, char* messageBuffer, size_t messageBufferSize);

#ifdef __cplusplus
}
#endif
/******************************************************************/
/*			E N D   O F   F I L E                     */
/******************************************************************/
