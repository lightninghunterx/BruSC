/*
 *******************************************************************
 *
 * $Source: /pr/CvsTree/pr/gen/h/lib/util.h,v $
 *
 * Copyright (c) 1995
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: util.h,v 1.149 2019/04/16 13:06:28 wem Exp $
 *
 *******************************************************************
*/

#ifndef LIB_UTIL_H		/* prevent multiple includes */
#define LIB_UTIL_H

#include <stdio.h>		/* for FILE */
#include <sys/types.h>		/* Chmod.o --> mode_t */
#include <stdbool.h>

/*** definitions ***/

/*********************************************************************/
/* getParfileDirForRead(), getParfileDirForWrite() property keywords */
/* must be synchronized with					     */
/* pr/gen/java/src/de/bruker/nmr/prsc/prop/parfile-dirs.prop	     */
/*********************************************************************/
#define	PP_DIRS		"PP_DIRS"	/* pulse programs	     */
#define	CPD_DIRS	"CPD_DIRS"	/* cpd programs		     */
#define	MAC_DIRS	"MAC_DIRS"	/* macros		     */
#define	MASR_DIRS	"MASR_DIRS"	/* mas rate		     */
#define	PY_DIRS		"PY_DIRS"	/* python programs	     */
#define	GP_DIRS		"GP_DIRS"	/* gradient programs	     */
#define	SHAPE_DIRS	"SHAPE_DIRS"	/* shape files		     */
#define	SP_DIRS		"SP_DIRS"	/* shape lists		     */
#define	AU_DIRS		"AU_DIRS"	/* AU programs		     */
#define	PAR_DIRS	"PAR_DIRS"	/* parameter sets	     */
#define	VD_DIRS		"VD_DIRS"	/* delays		     */
#define	VP_DIRS		"VP_DIRS"	/* pulses		     */
#define	VC_DIRS		"VC_DIRS"	/* loop counters	     */
#define	VT_DIRS		"VT_DIRS"	/* temperatures		     */
#define	VA_DIRS		"VA_DIRS"	/* amplitudes		     */
#define	F1_DIRS		"F1_DIRS"	/* frequency channel 1	     */
#define	SCL_DIRS	"SCL_DIRS"	/* solvent scaling region    */
#define	PHASE_DIRS	"PHASE_DIRS"	/* phase programs	     */
#define INTRNG_DIRS	"INTRNG_DIRS"	/* intrng files		     */
#define PEAKRNG_DIRS	"PEAKRNG_DIRS"	/* peakrng files	     */
#define BASLPNTS_DIRS	"BASLPNTS_DIRS"	/* baslpnts files	     */
#define BASE_INFO_DIRS	"BASE_INFO_DIRS"/* base_info files	     */
#define PEAKLIST_DIRS	"PEAKLIST_DIRS"	/* peaklist files	     */
#define CLEVELS_DIRS	"CLEVELS_DIRS"	/* clevels files	     */
#define REG_DIRS	"REG_DIRS"	/* region files		     */
#define INT2DRNG_DIRS	"INT2DRNG_DIRS"	/* int2drng files	     */
#define POPT_DIRS	"POPT_DIRS"	/* popt array files	     */
#define FORM_DIRS	"FORM_DIRS"	/* format files		     */
#define PROSOL_DIRS	"PROSOL_DIRS"	/* prosol files		     */

#define CHECKSUM_NO_DATA		 -99
#define CHECKSUM_NO_DATA_CHECKSUM	-100



/*** forward declarations ***/

struct curd_pars;


/*** types ***/

/*** variables ***/

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

#if defined(DLL_LINKAGE) && !defined(CPROTO)
#   ifdef util_INTERNAL
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

/* some error definitions of return types
 *       0 : no error
 *	-1 : cannot open curinst file
 *	-2 : cannot read name in curinst file
 *	-3 : argument is NULL-pointer
 *      -4 : curinst file is empty
 *      -5 : size of buffer is too small
 *      -6 : not a remote instrument
 */
enum utilErrType {utilNoErr = 0, utilNoCurinstOpen = -1, utilSpectNameNotReadable = -2,
		  utilNullPointerArg = -3, utilCurinstEmpty = -4, utilBuffSizeTooSmall = -5,
		  utilNotRemoteInstr = -6};

/*** function prototypes ***/

/* cpr_exch_file.c */
extern DLL_INTERFACE int CprExchangeFileEnabled(void);
extern DLL_INTERFACE void createCPR_exchange_file(int *, char **);
extern DLL_INTERFACE void closeCPR_exchange_file(void);
extern DLL_INTERFACE void appendCPR_exchange_message(const char *);
extern DLL_INTERFACE void gui_call_free_result(void);
extern DLL_INTERFACE const char *getCPR_exchange_file_args(const char **);
extern DLL_INTERFACE const char *getCPR_exchange_message(const char *);
extern DLL_INTERFACE const char* gui_call_with_result(const char*, int);

/* debug.c */

/** Debug File Handling

     DebugStart() opens an ASCII file for debugging and sets the global file pointer 'DebugFile'
     which can be used with fprintf(). DebugStop() closes the file.

     Example:
     \begin{verbatim}
     #include <stdio.h>
     #include <lib/util.h>

     printf("Debug version of go4 module, messages appear at \"%s\"\n", DebugStart("go4",1));
     ...
     fprintf(DebugFile,"debug: any message\n");
     ...
     DebugStop();
     \end{verbatim}

     \since 1994-03-16
     \author Andreas Huennebeck, Bruker BioSpin, D-76287 Rheinstetten, Germany.
 */
/*@{*/
    /** global file pointer for debugging */
    extern DLL_INTERFACE FILE *DebugFile;

    /** open file for debugging puposes.

     DebugStart() opens an ASCII file for debugging and sets the global file pointer 'DebugFile'
     which can be used for calling fprintf(). After opening the file a message is printed
     stating the name of the module or program derived from name and the actual time.
     The name of the debug file is read from the environment variable UXNMR_DEBFILE. If
     this variable does not exist, stdout is used instead.

     \param name	name of program or module
     \param module_flag	=0: standalone program, =1: uxnmr module
     \return		a pointer to a string which states where the debug messages will appear.

     \see DebugStop, DebugFile
     \since 1994-03-16
     \author Andreas Huennebeck, Bruker BioSpin, D-76287 Rheinstetten, Germany.
     */
    extern DLL_INTERFACE const char *DebugStart (const char* name, int module_flag);

    /** close debugging file.

     DebugStop() prints the name of the module or program and the actual time,
     then closes the file specified in 'DebugFile'.

     \see DebugStart, DebugFile
     \since 1994-03-16
     \author Andreas Huennebeck, Bruker BioSpin, D-76287 Rheinstetten, Germany.
     */
    extern DLL_INTERFACE void DebugStop (void);
/*@}*/


/* durtoasc.c */
extern DLL_INTERFACE int DurationToAscii(char*, double);

/* edit_file.c */
extern DLL_INTERFACE int edit_file (const char *);
extern DLL_INTERFACE int edit_file_nowait (const char *);

/* getcurdir.c */
extern DLL_INTERFACE const char *getcurdir (char *, const char *);

/* getdir.c */

/** get directory list

 getdir() must be called as getdir(part,&dir,matchstring) where dir is defined as char **dir.
 Needed space is malloc()'ed in getdir() and must be freed by calling freedir().

 The matching string of the getdir() function may include an option separated by / (slash).
 "/all" returns all entries (include .* files)

 Whenever one of the next options is selected the file status info is added to the filename:
 \begin{itemize}
 \item "/dir": return only subdirectory names
 \item "/files": return only ordinary file names
 \item "/stat": add status-info to returned array
 \end{itemize}
*/
extern DLL_INTERFACE int getdir (const char *part, char ***dir, const char *matchstring);

/** freedir is the corresponding routine to getdir() to free the malloc() space. */
extern DLL_INTERFACE void freedir (char **);


/** Instrument Name Handling */
/*@{*/

    /* in getinstr.c: */

    /** GetInstrName(): fill 'name_ptr' with instrument name
	If it is a remote instrument, the name "remote_spect" is returned.
       \param name_ptr	char array to be filled with instrument name
       \param name_size	size of char array 'name_ptr'
       \return		noErr if OK
       \return		errorcode (!= noErr) if failed - call getinstr_err() for error message
    */
extern DLL_INTERFACE enum utilErrType GetInstrName(char *name_ptr, unsigned int name_size);

    /** GetInstrPathTo(): fill 'path_ptr' with path to 'name'
       \param name	file name
       \param path_ptr	char array to be filled with path
       \param path_size	size of char array 'path_ptr'
       \return		noErr if OK
       \return		errorcode (!= noErr) if failed - call getinstr_err() for error message
    */
extern DLL_INTERFACE enum utilErrType GetInstrPathTo(const char *name, char *path_ptr,
							 unsigned int path_size);

/** GetAbsSpectDirectory(): fill spectdir with absolute path to conf/instr/<spect|remote_spect>> directory
   \param spectdir	e.g. "/opt/topspin/conf/instr/remote_spect"
   \param spectdir_size	size of char array 'spectdir'
   \return		noErr if OK
   \return		errorcode (!= noErr) if failed - call getinstr_err() for error message
*/
extern DLL_INTERFACE enum utilErrType GetAbsSpectDirectory(char *spectdir, unsigned int spectdir_size);

    /** forceLocalInstrum(): use only local instrument information
     *
     *  use only local instrument information by forcing GetRemoteSpectr() to
     *  return utilNotRemoteInstr in all cases. Setting this flag is not thread-safe.
       \param val
    */
extern DLL_INTERFACE void forceLocalInstrum(bool val);

    /** Global Function: GetRemoteSpectr
	\param spectrPtr  - char array to be filled with spectrometer name
			    '\0' if local
	\param spectrSize - length of array
       \return		noErr if OK
       \return		errorcode (!= noErr) if failed - call getinstr_err() for error message
    */
extern DLL_INTERFACE enum utilErrType GetRemoteSpectr(char* spectrPtr, unsigned int spectrSize);


    /** return error message created by GetInstr...() functions in case of error.
	\param errorcode	the error code as returned by GetInstr...()
	\return			pointer to character array containingthe error message
    */
extern DLL_INTERFACE const char *getinstr_err(int errorcode);



/* getParamDirs.c */
extern DLL_INTERFACE int getParfileDirForRead(const char *, const char *, char *);
extern DLL_INTERFACE int getParfileDirForWrite(const char *, const char *, char *);
extern DLL_INTERFACE int getParfileDirForWriteExcludeSuffix(const char *, const char *, char *, const char*);
extern DLL_INTERFACE int checkParfileDirReadability(const char * const *);
extern DLL_INTERFACE void setParamDirsParfilePath(char *);
extern DLL_INTERFACE const char * const * getParamDirs(const char *);
extern DLL_INTERFACE const char * const * getParamKeys(void);
extern DLL_INTERFACE const char * getParamDirsErrMsg(const char *);
extern DLL_INTERFACE const char * getParamKeysErrMsg(void);
extern DLL_INTERFACE const char * getParamDirsParfileName(void);

/* getstan.c */
extern DLL_INTERFACE char *getstan (char *, const char *);

/* gmatch.c */
extern DLL_INTERFACE int gmatch (const char *, const char *);

/* hypercomplex.c */
extern DLL_INTERFACE int isHypercomplexFnmode(int);

/* intpol.c */
extern DLL_INTERFACE double IntpolLin (double, double, double, double, double);

/* mkudir.c */

/** create a complete directory chain accessible by Nmr user and Nmr superuser

 Mkudir() works like mkudir(util). The difference is that on Windows the permissions
 of the new directory are set such that it is accessible by Nmr user and Nmr superuser.

 In C++ code you should consider calling UtilCreateDirectoryChain(util) instead.
 */
extern DLL_INTERFACE int Mkudir (const char *);

/** create a complete directory chain (obsolete)

 mkudir() works almost like {\tt mkdir -p} on Unix systems. The difference is that
 the last component in {\bf filepath} is handled as file which means that it is {\bf not|
 created as directory.

 In plain C library code you should consider calling mkudir_r(util) instead. In C++ code
 you should consider calling UtilCreateDirectoryChain(util) instead.

 \param filepath the full path to the file whose directory shall be created.name of the new directory
 \return 0 on success
 \return -1 on error, but prints the error message via Proc_err(uni) first.
 */
extern DLL_INTERFACE int mkudir (const char *filepath);

/** create a complete directory chain

 mkudir_r() works almost like {\tt mkdir -p} on Unix systems. The difference is that
 the last component in {\bf filepath} is handled as file which means that it is {\bf not|
 created as directory.

 In C++ code you should consider calling UtilCreateDirectoryChain(util) instead.

 \param filepath the full path to the file whose directory shall be created.name of the new directory
 \param errMsgBuf buffer for error messages
 \param errMsgBufSize the size of {\bf errMsgBuf}
 \return 0 on success
 \return -1 on error, store the error message in {\bf errMsgBuf}.
 */
extern DLL_INTERFACE int mkudir_r (const char* filepath, char errMsgBuf[], size_t errMsgBufSize);

/* printcap.c */
extern DLL_INTERFACE int GetPrintcapEntries(char ***);

/* showfile.c */
extern DLL_INTERFACE int showfile (const char *);

/* xwinversion.c */
extern DLL_INTERFACE const char *GetTsVersionText (void);
extern DLL_INTERFACE const char* GetTsVersionProduct (void);
extern DLL_INTERFACE const char *GetTsVersionLong (void);
extern DLL_INTERFACE const char *GetTsVersionShort (void);
extern DLL_INTERFACE const char *GetTsVersionDot (void);
extern DLL_INTERFACE const char *GetTsVersionNopl (void);
extern DLL_INTERFACE const char *GetTsVersionNAME (void);
extern DLL_INTERFACE const char *GetTsVersionName (void);
extern DLL_INTERFACE int GetTsVersionMajor (void);
extern DLL_INTERFACE int GetTsVersionMinor (void);
extern DLL_INTERFACE int GetTsVersionBeta (void);
extern DLL_INTERFACE int GetTsVersionPl (void);
extern DLL_INTERFACE int GetTsVersionIsRel (void);
extern DLL_INTERFACE int GetTsVersionIsBeta (void);
extern DLL_INTERFACE int GetTsVersionIsAlpha (void);
extern DLL_INTERFACE int GetTsFeatureIsProcessingEdition (void);
extern DLL_INTERFACE int GetTsFeatures (
    char* buf,	/* out: features are copied here, separated by '|' */
    size_t len	/* in: size of buf */
    );
extern DLL_INTERFACE int GetTsVersionInfo (
    char* buf,	/* out: features are copied here, separated by '|' */
    size_t len	/* in: size of buf */
    );
extern DLL_INTERFACE int getxwinvers (char *);

/* AbsolutePath.c */
extern DLL_INTERFACE int isAbsolutePath(const char*);

/* CheckSum.c */
extern DLL_INTERFACE int CheckSumBuffer(const void*, char*, int, int, int,
					const int*, const int*);
extern DLL_INTERFACE int CheckSumBufferRow(char*, const void*, int, int, int, const int*);
extern DLL_INTERFACE void CheckSumBufferEnd(char*, char*, int, int, int, const int*);
extern DLL_INTERFACE int CheckSumFile(const char*, const char*, char*, int*, int,
				      int, int, int, int, const int*, const int*);
extern DLL_INTERFACE int CheckSumFileMax(const char*, const char*, char*, int*,
					 int, int, int, int, int, const int*,
					 const int*, double*, double*);
extern DLL_INTERFACE int CheckSumConfigFile(const char*, char*, int);
extern DLL_INTERFACE int CheckSumInvalid(int);
extern DLL_INTERFACE const char* CheckSumError(int);

/* Chmod.c */
#ifdef	HAS_WINDOWS_FEATURES
extern DLL_INTERFACE int Chmod (const char *, unsigned long);
extern DLL_INTERFACE int Chown (const char *, unsigned long, unsigned long);
extern DLL_INTERFACE int Mkdir (const char *, unsigned long);
#else
extern DLL_INTERFACE int Chmod (const char *, mode_t);
extern DLL_INTERFACE int Chown (const char *, uid_t, gid_t);
extern DLL_INTERFACE int Mkdir (const char *, mode_t);
#endif

/* DebugUtil.c */
extern DLL_INTERFACE FILE *LibutilSetDebug (const char *);
extern DLL_INTERFACE FILE *LibutilSetDebugPntr (FILE *);
extern DLL_INTERFACE void LibutilSetDebuglevel (int);

/* ExecProgram */
extern DLL_INTERFACE int ExecProgram(const char * const *, int);
extern DLL_INTERFACE int ExecProgramTimeout(const char* const *, int, int);

/* GetSystemPrinter.c */
extern DLL_INTERFACE int GetSystemPrinterList(char ***);
extern DLL_INTERFACE const char* GetSystemDefaultPrinter(void);
extern DLL_INTERFACE const char* GetMediaSize(const char*);
extern DLL_INTERFACE const char **GetCupsPPDOptions(const char*);

/* PrintCommand.c */
extern DLL_INTERFACE void SetTextProperties(double);
extern DLL_INTERFACE void SetCenterTitle(const char *);
extern DLL_INTERFACE int PrintTextFile(const char *, const char *);

/* RemoveData.c	*/
extern DLL_INTERFACE int RemoveData(const char*, const char*,
				    const char*, const char*, int, int, int);
/* RemoveTree.c	*/
#ifndef	HAS_WINDOWS_FEATURES
extern DLL_INTERFACE void RemoveError(const char *);
#endif	/* ! HAS_WINDOWS_FEATURES */
extern DLL_INTERFACE int RemoveEmptyTree(const char *, int);
extern DLL_INTERFACE int RemoveTree(const char *, int);

/* remove files in directory but keep directory itself,
 * as well as all sub-directories including their files;
 * second parameter is verbose flag*/
extern DLL_INTERFACE int RemoveEntries(const char *, int);
extern DLL_INTERFACE int RemovePlainFiles(const char *, int);

/* SearchCmdArg.c */
extern DLL_INTERFACE int search_cmd_arg(const char *, const char *);
extern DLL_INTERFACE int search_argc_argv_arg(int, char **, const char *);


/* TsPath.c */

/** Data Path Handling */
/*@{*/

    /** get the paths to the current dataset as specified in 'cp'
	\param cp		curdat parameters
	\param idx		type of parameter set: 2=curdat2, 3=curdat3, otherwise=curdat
	\param acqu		pointer to buffer which gets the dataset up to expno
				(set to NULL if not needed)
	\param acqusize	size of 'acqu' buffer
	\param proc		pointer to buffer which gets the dataset up to procno
				(set to NULL if not needed)
	\param procsize	size of 'proc' buffer
	\return 0		in case of success
	\return -1		in case of failure - call TsPathGetErrMsg() to get the error message

	\see TsPathGetErrMsg
	\since 2007-11-21
	\author Andreas Huennebeck, Bruker BioSpin, D-76287 Rheinstetten, Germany.
    */
extern DLL_INTERFACE int TsPathGetCurdatDirs(const struct curd_pars*, int, char*,
					     unsigned int, char*, unsigned int);

    /** return error message created by TsPathGetCurdatDirs() in case of error.
	\return pointer to character buffer containing the error message
	\return pointer to empty string if no error has occurred.

	\see TsPathGetCurdatDirs
	\since 2007-11-21
	\author Andreas Huennebeck, Bruker BioSpin, D-76287 Rheinstetten, Germany.
     */
extern DLL_INTERFACE const char* TsPathGetErrMsg(void);
/*@}*/

/* EncryptAdm.c */
extern DLL_INTERFACE int EncryptNMRAdminPwd(const char*, int*, size_t);

/* ValidateAdm.c */
extern DLL_INTERFACE int ValidateNMRAdminPwd(const char*);

/* ValidatePwd.c */
extern DLL_INTERFACE int ValidatePassword(const char *, const char *);

#ifdef	HAS_WINDOWS_FEATURES
/* RunningProcs.c */
int GetRunningProcesses(const char*);
/* win32srv.c */
int StartSrv(const int, char ***const,
	 const char *const,
	 const char *const,
	 const char *const,
	 const char *const,
	 const char *const);
#endif	/* HAS_WINDOWS_FEATURES */

#ifdef __cplusplus
}
#endif

#endif /* LIB_UTIL_H */
/**********************************************************************/
/*			     E N D   O F   F I L E                    */
/**********************************************************************/
