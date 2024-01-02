/*
 *******************************************************************
 *
 * $Source: /sc/CvsTree/sc/gen/h/lib/auliba.h,v $
 *
 * Copyright (c) 1998
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: auliba.h,v 1.45 2019/09/09 11:48:54 Marina.Maurer Exp $
 *
 *******************************************************************
 */

#ifndef lib_AULIBA_H_INCLUDED
#define lib_AULIBA_H_INCLUDED



#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif
#if defined(DLL_LINKAGE) && !defined(CPROTO)
#   ifdef auliba_INTERNAL
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

#define FILE_SELECT_MAX_LINE_LEN 0x80  /* maximun lenght of the selected filename */

/*******************************************************************************
 Some usefull parname definitions for Bsms-Functions

 int GetBsmsIntValue(const char* parname, int* value);
 int GetBsmsFloatValue(const char* parname, float* value);
 int GetBsmsBooleanValue(const char* parname, int* value);
 int PutBsmsIntValue(const char* parname, int value);
 int PutBsmsFloatValue(const char* parname, float value);
******************************************************************************/
#define BSMS_AUTO_GAIN   "AUTOGAIN"
#define BSMS_AUTO_LOCK   "AUTOLOCK"
#define BSMS_AUTO_PHASE   "AUTOPHASE"
#define BSMS_AUTO_POWER   "AUTOPOWER"
#define BSMS_AUTO_SHIM   "AUTOSHIM"
#define BSMS_DRIFT   "DRIFT"
#define BSMS_FIELD   "FIELD"
#define BSMS_LIFT   "LIFT"
#define BSMS_LOCK   "LOCK"
#define BSMS_LOCK_DC   "LOCKDC"
#define BSMS_LOCK_GAIN   "LOCKGAIN"
#define BSMS_LOCK_LOST   "LOCK_LOST"
#define BSMS_LOCK_PHASE   "LOCKPHASE"
#define BSMS_LOCK_POWER   "LOCKPOWER"
#define BSMS_LOCK_POW_STEP   "POWERSTEP"
#define BSMS_LOCK_SHIFT   "LOCKSHIFT"
#define BSMS_LOOP_FILTER   "LOOPFILTER"
#define BSMS_LOOP_GAIN   "LOOPGAIN"
#define BSMS_LOOP_TIME   "LOOPTIME"
#define BSMS_POSITION   "POSITION"
#define BSMS_READLOCK   "READLOCK"
#define BSMS_SHIM_TEMP   "SHIMTEMP"
#define BSMS_SPIN   "SPIN"
#define BSMS_SPIN_RATE   "SPINRATE"
#define BSMS_SWEEP   "SWEEP"
#define BSMS_SWEEP_AMPL   "SWEEPAMPL"
#define BSMS_SWEEP_RATE   "SWEEPRATE"
#define BSMS_X   "X"
#define BSMS_X2Y2   "(X2-Y2)"
#define BSMS_X2Y2Z   "(X2-Y2)Z"
#define BSMS_X2Y2Z2   "(X2-Y2)Z2"
#define BSMS_X2Y2Z3   "(X2-Y2)Z3"
#define BSMS_X2Y2Z4   "(X2-Y2)Z4"
#define BSMS_X2Y2Z5   "(X2-Y2)Z5"
#define BSMS_X3   "X3"
#define BSMS_X3Z   "X3Z"
#define BSMS_XY   "XY"
#define BSMS_XYZ   "XYZ"
#define BSMS_XYZ2   "XYZ2"
#define BSMS_XYZ3   "XYZ3"
#define BSMS_XYZ4   "XYZ4"
#define BSMS_XYZ5   "XYZ5"
#define BSMS_XZ   "XZ"
#define BSMS_XZ2   "XZ2"
#define BSMS_XZ3   "XZ3"
#define BSMS_XZ4   "XZ4"
#define BSMS_XZ5   "XZ5"
#define BSMS_Y   "Y"
#define BSMS_Y3   "Y3"
#define BSMS_Y3Z   "Y3Z"
#define BSMS_YZ   "YZ"
#define BSMS_YZ2   "YZ2"
#define BSMS_YZ3   "YZ3"
#define BSMS_YZ4   "YZ4"
#define BSMS_YZ5   "YZ5"
#define BSMS_Z   "Z"
#define BSMS_Z0   "Z0"
#define BSMS_Z10   "Z10"
#define BSMS_Z2   "Z2"
#define BSMS_Z3   "Z3"
#define BSMS_Z4   "Z4"
#define BSMS_Z5   "Z5"
#define BSMS_Z6   "Z6"
#define BSMS_Z7   "Z7"
#define BSMS_Z8   "Z8"
#define BSMS_Z9   "Z9"

/***********************************************************************/
/*         G L O B A L  F U N C T I O N   P R O T O T Y P E S          */

/***********************************************************************/
/* CreateNewExp.c */
/** Create a File and set debug Filepointer for debugging purposes
    \param FileName file name of debug file
 */
extern DLL_INTERFACE FILE* DebugCreateNewExperiment(const char *FileName);
/** Close debug file
 */
extern DLL_INTERFACE void DebugCreateNewExperimentClose(void);
/** Get error message of 'CreateNewExperiment'
    \return pointer to error message
 */
extern DLL_INTERFACE const char *CreateNewExperimentErrorMsg(void);
/** Create a new experiment. Creates a new data set, reads in parameters, sets the solvent
    and solvent dependent parameters.
    \param SolventName the SOLVENT parameter
    \param Experiment the name of the standard parameter set
    \param Partition DU or DATASET
    \param UserName (obsolete)
    \param DatasetName NAME parameter   of the new dataset
    \param Expno EXPNO parameter of the new dataset
    \param Procnp PROCNO parameter of the new dataset
 */
extern DLL_INTERFACE int CreateNewExperiment(const char *SolventName,
					     const char *Experiment,
					     const char *Partition,
					     const char *UserName,
					     const char *DatasetName,
					     int Expno, int Procno);
/* curdpath.c */
/** return path of dataset
    \param acquflag 0 = processed data directory, 1 = raw data directory
    \datpath DATPATH of dataset
    \name NAME of dataset
    \expno EXPNO of dataset
    \procno PROCNO of dataset
    \file optional filename to be appended to the directory
    \return path of dataset
 */
extern DLL_INTERFACE const char *curd_path(int acquflag,
					   const char *datpath,
					   const char *name,
					   int expno, int procno, const char *file);
/* execau.c */
/** compile AU program
    \param lname name of the AU program
    \param silent 0 = show message about progress in Proc_err, 1= only status message
    \return 0 if compilation ok
    \return 1 if compilation errors
 */
extern DLL_INTERFACE int compile(char *lname, int silent);
/** execute AU program
    \param autype 0=xaua, 1=xaup, 2=xau
    \param exectype arguments for CPR_exec() (NO_WAIT etc.)
    \param name the name of the AU program
    \param curdata the current data path
    \param cmd more arguments if required
 */
extern DLL_INTERFACE int execau(int, int, const char *, const char *, const char *);
/* getcurdat.c */
/** get error message of 'getcurdat'
    \return the text
 */
extern DLL_INTERFACE int getcurdat(int set_no, const char *curdat, char *datpath, char *name,
				   int *expno, int *procno);
/* setcurdat.c */
/** set curdat to new dataset
    \param set_no 1 = curdat, 2 = curdat2, 3 = curdat3
    \param curdat the name of the curdat file (= first argument in AU program execution)
    \param datpath new DATPATH
    \param name new NAME
    \param expno new EXPNO
    \param procno new PROCNO
 */
extern DLL_INTERFACE int setcurdat(int set_no, const char *curdat, const char *datpath, const char *name,
				   int expno, int procno);
/* getparamname.c */
/** get parameter unit
   \param par name of the parameter
   \return unit as string
 */
extern DLL_INTERFACE const char *getparamunit(const char* par);
/* fetchstorm.c */
/** get or store acqum parameters
    \param curdat the name of the curdat file (= first argument in AU program execution)
    \param modus 0 = store, 1 = fetch
    \param arglist parameter(s)
 */
extern DLL_INTERFACE int fetchstorm(const char *curdat, int modus, const char *arglist, ...);
/* fetchstord3.c */
/** get or store d3 (image) parameters
    \param curdat the name of the curdat file (= first argument in AU program execution)
    \param modus 0 = store, 1 = fetch
    \param arglist parameter(s)
 */
extern DLL_INTERFACE int fetchstord3(const char *curdat, int modus, const char *arglist, ...);
/* mod_stopar.c */
/** fetch or store parameter
    \param curdat the name of the curdat file (= first argument in AU program execution)
    \param use_curdat curdat is the curdat file (1) or an absolute path (0)
    \param modus 0 = store, 1 = fetch
    \param dimarg the dimension of the parameter (f1 - f8)
    \param arglist parameter(s)
 */
extern DLL_INTERFACE int fetchstor(const char *curdat, int use_curdat, int modus, const char *dimarg,
				   const char *arglist, ...);
/* FileSelect.c */
/** calls GUI to select list of files or directory(s)
    \param cpSourceDir
    \param flist list of files
    \param ipLinesMax
    \param modeDirectoryOnly  1=select dir, 0=select files
    \return	-1	error
    \return     -1  && *ipLinesMax == 0 or
                -1  && *ipLinesMax == *ipLinesMax+1
                    error: Not enough space for all list elements
    \return     -1  && (0 < *ipLinesMax < *ipLinesMax+1)
                 error: List not completed after 30 seconds
    \return      0  && *ipLinesMax == 0 ok: Nothing selected, list is empty
     \return       0  && *ipLinesMax != 0
              ok: *ipLinesMax == number of selected files
 */
extern DLL_INTERFACE int FileSelect(char *cpSourceDir, char (*flist)[0x80], int *ipLinesMax,
				    int modeDirectoryOnly);

/* getAmpPowerMax.c */
/** get the amplifier power of the amplifier used in the logical channel chan
    \param chan logical channel
    \param curdat path to current directory
    \return power in Watt
    \return 0.0 in case of error
*/
extern DLL_INTERFACE double getAmpPowerMax(int chan, const char* curdat);

/* BsmsCmdInterFace.c */
/** wrapper functions for "bsmscmd <PARAMETR> [VALUE]"
*/
extern DLL_INTERFACE int GetBsmsIntValue(const char* parname, int* value);
extern DLL_INTERFACE int GetBsmsFloatValue(const char* parname, float* value);
extern DLL_INTERFACE int GetBsmsDoubleValue(const char* parname, double* value);
extern DLL_INTERFACE int GetBsmsBooleanValue(const char* parname, int* value);
extern DLL_INTERFACE int GetBsmsFloatValueUnformatted(const char* parname, float* value);
extern DLL_INTERFACE int GetBsmsDoubleValueUnformatted(const char* parname, double* value);

extern DLL_INTERFACE int PutBsmsIntValue(const char* parname, int value);
extern DLL_INTERFACE int PutBsmsFloatValue(const char* parname, float value);
extern DLL_INTERFACE int GetBsmsLockPower(double *lpow);
extern DLL_INTERFACE int SetBsmsLockPower(double lpow);

/** wrapper functions for "bsmsauxcmd <PARAMETR> [VALUE]"
*/
extern DLL_INTERFACE int GetBsmsAuxIntValue(const char* parname, int* value);
extern DLL_INTERFACE int GetBsmsAuxFloatValue(const char* parname, float* value);
extern DLL_INTERFACE int GetBsmsAuxDoubleValue(const char* parname, double* value);
extern DLL_INTERFACE int GetBsmsAuxBooleanValue(const char* parname, int* value);
extern DLL_INTERFACE int PutBsmsAuxIntValue(const char* parname, int value);
extern DLL_INTERFACE int PutBsmsAuxFloatValue(const char* parname, float value);

/* vtucmd.c */
/** get parameters of the BSVT (BSMS/2 with ELCB)
    \param parname paramter name know by ELCB
    \return parameter value
    \return NULL in case of error
*/
extern DLL_INTERFACE const char* vtc_get_parameter(const char* parname);

/** push parameters of the BSVT (BSMS/2 with ELCB)
    \param parname paramter name know by ELCB
    \param value
    \return	-1	error
    \return 0 ok:
*/
extern DLL_INTERFACE int vtc_push_parameter(const char* parname,const char* value);

/** get gasflow status from BSVT (BSMS/2 with ELCB)
    \param pointer to float variable target flow
    \param pointer to float variable measured flow
    \return	-1	error
    \return 0 ok:
*/
extern DLL_INTERFACE int vtc_gasflow_status(float* target,float* measured);

/** Polynom Fit
    \param degree of polynom
    \param array og abcissa
    \param array of ordinates
    \param number of data points

    \param result of coefficeient
    \param error of result

    \return	-1	error
    \return 0 ok:
*/
extern DLL_INTERFACE int polynomial_fit(const int degree,
					const double p2FitAbsc[],
					const double p2FitData[],
					const int p2FitPts,
					double result[]);

extern DLL_INTERFACE int lmcurve_fit(const int m_dat,const double *t, const double *y,
				     double (*f)( double t, const double *par ),
				     const int n_par,double *par);

extern DLL_INTERFACE int create_dummy_fid(const char* pathfid,int td);

/* Levenberg-Marquardt minimization. */
extern DLL_INTERFACE int lm_minimization(int n_par, double *par, int m_dat, const void *data,
            void (*evaluate) (const double *par, int m_dat, const void *data,
                              double *fvec, int *userbreak) );

extern DLL_INTERFACE void set_lmmin_logstream(FILE *fp, int v);
extern DLL_INTERFACE void set_lmmin_verbosity(int v);
extern DLL_INTERFACE int number_of_iterations();
extern DLL_INTERFACE double  normOfTheResidueVector();

extern DLL_INTERFACE int lmmin_outcome();
extern DLL_INTERFACE const char* lmmin_shortmsg();
extern DLL_INTERFACE const char* lmmin_infomsg();
extern DLL_INTERFACE const char* lmmin_error();

/*
 * get probe information: probe ID, probe Name
 *
 */
extern DLL_INTERFACE int getProbeId(char* probeId, size_t size);
extern DLL_INTERFACE int getProbeName(char* probeName, size_t size);
extern DLL_INTERFACE int isSolidProbe();
extern DLL_INTERFACE int isAtmaProbe();

extern DLL_INTERFACE int getPreempDefaultFileForProbe(const char* probeId, const char* targetPath,
						    char* errorBuffer, const int bufferLength);

extern DLL_INTERFACE int writeSconToEpu(	char* parameters[]
						, char* values[]
						, const unsigned int numParameters
						, char* errorBuffer
						, const unsigned int bufferLength );


extern DLL_INTERFACE int test_aip(void);
#ifdef __cplusplus
}
#endif
#endif  /* lib_AULIBA_H_INCLUDED  */
