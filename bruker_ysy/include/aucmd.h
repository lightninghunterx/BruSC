/*
 *******************************************************************
 *
 * $Source: /sc/CvsTree/sc/gen/src/prg/makeau/aucmd.h,v $
 *
 * Copyright (c) 1995
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: aucmd.h,v 1.155.4.1 2020/02/19 09:25:03 alexander.herbig Exp $
 *
 *******************************************************************
 */
/** Prevent AU programs of known sleep() errors ***/
#define sleep	ssleep

#ifdef	HAS_WINDOWS_FEATURES
/* Perror() does not print the real error string */
#define Perror(_O_,_X_) Proc_err(_O_,"%s\n%s",_X_,strerror(errno))
#endif	/* HAS_WINDOWS_FEATURES */

/* Define dataset	*/
#define SETCURDATA \
 AUERR=setcurdat(lastparflag, curdat,disk, name, expno, procno);

#define PROCPATH(x) curd_path(0,disk,name,expno,procno,x)
#define PROCPATH2(x) curd_path(0,disk2,name2,expno2,procno2,x)
#define PROCPATH3(x) curd_path(0,disk3,name3,expno3,procno3,x)

#define ACQUPATH(x) curd_path(1,disk,name,expno,procno,x)
#define ACQUPATH2(x) curd_path(1,disk2,name2,expno2,procno2,x)
#define ACQUPATH3(x) curd_path(1,disk3,name3,expno3,procno3,x)

#define NEWPROCPATH(var,x)			\
    curd_path(0,disk,name,expno,var,x)

#define NEWACQUPATH(var,x)			\
    curd_path(1,disk,name,var,procno,x)

#define VARACQUPATH(name_x,expno_x,x)		\
    curd_path(1,disk,name_x,expno_x,procno,x)

#define VARPROCPATH(name_x,expno_x,procno_x,x)		\
    curd_path(0,disk,name_x,expno_x,procno_x,x)
#define VARPROCPATH2(name_x,expno_x,procno_x,x)		\
    curd_path(0,disk2,name_x,expno_x,procno_x,x)
#define VARPROCPATH3(name_x,expno_x,procno_x,x)		\
    curd_path(0,disk3,name_x,expno_x,procno_x,x)
/* Acquisition commands */
/************************/

#define GO	{SETCURDATA AUERR=CPR_exec("go", WAIT_TERM);}
#define ZG	{SETCURDATA AUERR=CPR_exec("zg", WAIT_TERM);}
#define RGA	{SETCURDATA AUERR=CPR_exec("rga",WAIT_TERM);}
#define II	{SETCURDATA AUERR=CPR_exec("ii", WAIT_TERM);}
#define ATMA	{SETCURDATA AUERR=CPR_exec("atma",WAIT_TERM);}
#define GO_OVERWRITE	{SETCURDATA AUERR=CPR_exec("go yes",WAIT_TERM);}
#define ZG_OVERWRITE	{SETCURDATA AUERR=CPR_exec("zg yes",WAIT_TERM);}

/* Eurotherm Temperature unit commands	*/
/* XWINNMR Parameter TE,TE2 and TG	*/
/****************************************/
/* Standard Unit			*/
#define TESET  { SETCURDATA AUERR=CPR_exec("teset", WAIT_TERM);}
#define TEGET  { SETCURDATA AUERR=CPR_exec("teget", WAIT_TERM);}
#define TEREADY(seconds,accuracy)  \
	{sprintf(Hilfs_string, "%s %d %.1f", "teready", seconds, accuracy); \
	SETCURDATA \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define TEPAR(val)  \
	{sprintf(Hilfs_string, "%s %s", "tepar", val);	\
	SETCURDATA \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
/****************************************/
/* Second Unit for BEST			*/
#define TE2SET  { SETCURDATA AUERR=CPR_exec("te2set", WAIT_TERM);}
#define TE2GET  { SETCURDATA AUERR=CPR_exec("te2get", WAIT_TERM);}
#define TE2READY(seconds,accuracy)  \
	{sprintf(Hilfs_string, "%s %d %.1f", "te2ready", seconds, accuracy); \
	SETCURDATA \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
/****************************************/
/* Third Unit for Probe Capillary			*/
#define TE3SET  { SETCURDATA AUERR=CPR_exec("te3set", WAIT_TERM);}
#define TE3GET  { SETCURDATA AUERR=CPR_exec("te3get", WAIT_TERM);}
#define TE3READY(seconds,accuracy)  \
	{sprintf(Hilfs_string, "%s %d %.1f", "te3ready", seconds, accuracy); \
	SETCURDATA \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
/****************************************/
/* additional BSVT command interface    */
#define VTU_GET_PARAM(parname,value) \
    {value = vtc_get_parameter(parname);\
	if (value == NULL) AUERR=-1;}
#define VTU_PUSH_PARAM(parname,value) {AUERR = vtc_push_parameter(parname,value);}
/* First logical VTC channel			*/
#define TE1SET  { SETCURDATA AUERR=CPR_exec("te1set", WAIT_TERM);}
#define TE1GET  { SETCURDATA AUERR=CPR_exec("te1get", WAIT_TERM);}
#define TE1READY(seconds,accuracy)  \
	{sprintf(Hilfs_string, "%s %d %.1f", "te1ready", seconds, accuracy); \
	SETCURDATA \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
/* Forth logical VTC channel			*/
#define TE4SET  { SETCURDATA AUERR=CPR_exec("te4set", WAIT_TERM);}
#define TE4GET  { SETCURDATA AUERR=CPR_exec("te4get", WAIT_TERM);}
#define TE4READY(seconds,accuracy)  \
	{sprintf(Hilfs_string, "%s %d %.1f", "te4ready", seconds, accuracy); \
	SETCURDATA \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define VTU_HEATER(mode)				\
	{sprintf(Hilfs_string, "%s %s", "heater", mode);\
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define VTU_HEATER_STATUS(status) \
	{ AUERR = vtc_gasflow_status(&target,&measured);}
#define VTU_CHILLER(mode)				\
	{sprintf(Hilfs_string, "%s %s", "chiller", mode);\
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define VTU_GASFLOW(value)				\
	{sprintf(Hilfs_string, "%s %f", "vtugasflow", value); \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define VTU_GASFLOW_STATUS(target,measured)		\
	{ AUERR = vtc_gasflow_status(&target,&measured);}
#define VTU_SELFTUNE {AUERR=CPR_exec("vtuselftune",WAIT_TERM);}
#define VTU_SELFTUNE_SAVE(filename) \
	{sprintf(Hilfs_string, "%s \"%s\"", "vtuselftune", filename); \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define VTU_SELFTUNE_LOAD(filename) \
	{sprintf(Hilfs_string, "%s \"%s\"", "vtuselftune load", filename); \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define VTU_LOG {AUERR=CPR_exec("vtulog",WAIT_START);}
#define VTU_LOG_DATA(seconds) \
	{sprintf(Hilfs_string, "%s %d %s", "vtulog -delta", seconds, "log2data"); \
	SETCURDATA \
	AUERR=CPR_exec(Hilfs_string,WAIT_START);}
#define VTU_LOG_STOP {AUERR=CPR_exec("vtulog stop",WAIT_TERM);}

#define VTU_GET_SELFTUNE_SETTINGS(name,channel,comment)	\
	{sprintf(Hilfs_string, "%s %s %s %d %s",	\
		 "sendgui", "getselftunesettings", name, channel, comment); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define VTU_SET_SELFTUNE_SETTINGS(name,channel)	\
	{sprintf(Hilfs_string, "%s %s %s %d",	\
		 "sendgui", "setselftunesettings", name, channel); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define VTU_SET_CORRECTION(name,slope,bias)	\
	{sprintf(Hilfs_string,"sendgui setcorrection Correction_Name=%s\nCorrection_Slope=%g\nCorrection_Bias=%g",name,slope,bias); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define VTU_SET_CORRECTION_WITH_PROBE(name,slope,bias)				\
	{char phead_string[PATH_MAX];						\
	if (getProbeName(phead_string, sizeof(phead_string)) < 0)		\
	    strcpy(phead_string, "unknown");					\
	sprintf(Hilfs_string,"sendgui setcorrection Correction_Name=%s\n"	\
		      "Correction_Slope=%g\nCorrection_Bias=%g\n"		\
		      "Correction_Probe=%s",name,slope,bias,phead_string);	\
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define VTU_SET_CORRECTION_WITH_PROBE_AND_COMMENT(name,slope,bias,comment)	\
	{char phead_string[PATH_MAX];						\
	if (getProbeName(phead_string, sizeof(phead_string)) < 0)		\
	    strcpy(phead_string, "unknown");					\
	sprintf(Hilfs_string,"sendgui setcorrection Correction_Name=%s\n"	\
		      "Correction_Slope=%g\nCorrection_Bias=%g\n"		\
		      "Correction_Probe=%s\nCorrection_Comment=%s",		\
		      name,slope,bias,phead_string,comment);			\
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define VTU_ENABLE_CORRECTION(name) \
	{sprintf(Hilfs_string,\
	"%s %s %s", "sendgui", "enablecorrection", name); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define VTU_DISABLE_CORRECTION \
	{SETCURDATA AUERR=CPR_exec("sendgui disablecorrection",WAIT_TERM);}

/****************************************************/
/* Additional Unit for Gradient temperature control */
#define TEGSET  { SETCURDATA AUERR=CPR_exec("tegset", WAIT_TERM);}
#define TEGGET  { SETCURDATA AUERR=CPR_exec("tegget", WAIT_TERM);}
#define TEGREADY(seconds,accuracy)  \
	{sprintf(Hilfs_string, "%s %d %.1f", "tegready", seconds, accuracy); \
	SETCURDATA \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
/******************************************/
/* vtlist handling only for teset command */
#define VT \
	{sprintf(Hilfs_string, "%s %f", "teset", vtlist[xvt]);	\
	SETCURDATA \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define RVTLIST \
	{char vtlistname[16];\
	FETCHPAR("VTLIST",vtlistname);\
	sprintf(longpath, "%s/%s", getstan(NULL,"lists/vt"),vtlistname);\
	if (textfilepointer=fopen(longpath, "r"))\
	{xvt=0;\
	while (fscanf(textfilepointer, "%f", &vtlist[xvt]) != EOF )\
		xvt = (++xvt % (sizeof(vtlist)/sizeof(float)) ) ;\
	fclose(textfilepointer);}\
	xvt = 0;}
#define IVTLIST \
	{xvt = (++xvt % (sizeof(vtlist)/sizeof(float)));\
	if (vtlist[xvt] == 0.0) xvt = 0; \
	}
#define DVTLIST \
	{xvt = (xvt>0)? xvt-- : (sizeof(vtlist)/sizeof(float)-1);\
	if (vtlist[xvt] == 0.0) xvt = 0; \
	}

/****************************************/

/* Sample Changer  BSMS Unit commands */
/**************************************/

#define AUTOGAIN  {AUERR=CPR_exec( "autogain",WAIT_TERM);}
#define AUTOPHASE  {AUERR=CPR_exec( "autophase",WAIT_TERM);}
#define AUTOSHIM_ON  {AUERR=CPR_exec( "autoshim on",WAIT_TERM);}
#define AUTOSHIM_OFF {AUERR=CPR_exec( "autoshim off",WAIT_TERM);}
#define EJ	AUERR=CPR_exec("ej",WAIT_TERM);
#define IJ	AUERR=CPR_exec("ij",WAIT_TERM);
#define INSERT_REFERENCE	AUERR=CPR_exec("insertref",WAIT_TERM);
#define LO(val) \
	{sprintf(Hilfs_string, "%s %f", "lo", val);		\
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define LG  {AUERR=CPR_exec( "lg auto",WAIT_TERM);}
#define LOCK	{SETCURDATA AUERR=CPR_exec( "lock -acqu",WAIT_TERM);}
#define LOCK_ON	{SETCURDATA AUERR=CPR_exec( "lock on",WAIT_TERM);}
#define LOCK_OFF {SETCURDATA AUERR=CPR_exec( "lock off",WAIT_TERM);}
#define LOCK_AND_SWEEP_OFF {SETCURDATA AUERR=CPR_exec( "lock +sweepoff",WAIT_TERM);}
#define LOCK_ON_AFTER_HOLD {SETCURDATA AUERR=CPR_exec( "lock +afterholdon",WAIT_TERM);}
#define SWEEP_ON {SETCURDATA AUERR=CPR_exec( "sweep on",WAIT_TERM);}
#define SWEEP_OFF {SETCURDATA AUERR=CPR_exec( "sweep off",WAIT_TERM);}
#define LOPO	{SETCURDATA AUERR=CPR_exec( "lopo -acqu",WAIT_TERM);}
#define ROT	{SETCURDATA AUERR=CPR_exec( "ro acqu",WAIT_TERM);}
#define ROTOFF	{SETCURDATA AUERR=CPR_exec( "ro off wait",WAIT_TERM);}
#define RSH(shim) \
	{sprintf(Hilfs_string, "%s %s", "rsh", shim); \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define WSH(shim) \
	{sprintf(Hilfs_string, "%s %s %s", "wsh", shim, "y"); \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define SETSH(shim,val) \
	{sprintf(Hilfs_string, "%s %s %d", "setsh", shim, val); \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define TUNESX AUERR=CPR_exec("tune .sx",WAIT_TERM);
#define TUNE(file) \
	{sprintf(Hilfs_string, "%s %s", "tune", file); \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define LTIME(val) \
	{sprintf(Hilfs_string, "%s %f", "ltime", val); \
	SETCURDATA \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define LGAIN(val) \
	{sprintf(Hilfs_string, "%s %f", "lgain", val); \
	SETCURDATA \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define LFILTER(val) \
	{sprintf(Hilfs_string, "%s %d", "lfilter", val); \
	SETCURDATA \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define WAIT_CHECKLOCKSHIFT	AUERR=CPR_exec("shimcntl",WAIT_TERM);
#define GETPROSOL					\
	{						\
	    const char* errtxt;				\
	    SETCURDATA;					\
	    errtxt = gui_call_with_result("getprosol", 0);\
	    if ((errtxt != 0) && (errtxt[0] != '\0'))	\
	    {						\
		AUERR = -1;				\
		Proc_err(DEF_ERR_OPT, "%s", errtxt);	\
	    }						\
	    else AUERR = 0;				\
	}

#define CALL_PROBESERVER(Hilfs_string, result)		\
	{						\
	    GUICALL_WITH_ERRORHANDLING(Hilfs_string, result); \
	}

#define GUICALL_WITH_ERRORHANDLING(Hilfs_string, result)\
	{						\
	    SETCURDATA;					\
	    result = gui_call_with_result(Hilfs_string, 0);\
	    if (strncmp(result, "Error", 5) == 0)	\
	    {						\
		AUERR = -1;				\
		Proc_err(DEF_ERR_OPT, " %s", result);	\
	    }						\
	    else AUERR = 0;				\
	}

#define FLASHPICS(pics_file_content)			\
	{						\
	    const char* result_gui;			\
	    sprintf(Hilfs_string, "%s %s %s",		\
			  "probe_client pushCmdParams",	\
			  "flashPics", pics_file_content);\
	    CALL_PROBESERVER(Hilfs_string, result_gui);	\
	}

#define GET_SAMPLE_DIAMETER(samplediameter)		\
	{						\
	    const char* result_gui;			\
	    sprintf(Hilfs_string, "%s %s",		\
			  "probe_client pushCmdParams",	\
			  "getSampleDiameter");		\
	    CALL_PROBESERVER(Hilfs_string, result_gui);	\
	    samplediameter = atof(result_gui);		\
	}

#define Get_INSTALLED_PROBE(singleProbeInfo, numberOfElements)	\
	{							\
	    const char* result_gui;				\
	    sprintf(Hilfs_string,"probe_client getParams getInstalledProbe");	\
	    CALL_PROBESERVER(Hilfs_string, result_gui);		\
	    char* result = strdup(result_gui);			\
	    char* pch = strtok (result_gui,",");		\
	    numberOfElements = 0;				\
	    while (pch != NULL)					\
	    {							\
		singleProbeInfo[numberOfElements] = pch;	\
		pch = strtok (NULL, ",");			\
		numberOfElements++;				\
	    }							\
	}


#define CALL_HCONFSERVER(Hilfs_string, result)			\
	{							\
	    GUICALL_WITH_ERRORHANDLING(Hilfs_string, result);	\
	}


/* MAS Unit commands			*/
/****************************************/
#define MASI  { AUERR=CPR_exec("masi",WAIT_TERM);}
#define MASE  { AUERR=CPR_exec("mase",WAIT_TERM);}
#define MASR  { SETCURDATA AUERR=CPR_exec("masr acqu",WAIT_TERM);}
#define MASRGET  { SETCURDATA AUERR=CPR_exec("masr get",WAIT_TERM);}
#define MASH  { AUERR=CPR_exec("mash wait",WAIT_TERM);}
#define MASG(retry) \
	{sprintf(Hilfs_string, "%s %d", "masg wait", retry); \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

/* MAS2 Unit commands			*/
/****************************************/
#define MAS_INSERT  { AUERR=CPR_exec("masinsert",WAIT_TERM);}
#define MAS_EJECT  { AUERR=CPR_exec("maseject",WAIT_TERM);}
#define MASRATE_SET  { SETCURDATA AUERR=CPR_exec("masrate acqu",WAIT_TERM);}
#define MASRATE_GET  { SETCURDATA AUERR=CPR_exec("masrate get",WAIT_TERM);}
#define MAS_HALT  { AUERR=CPR_exec("mashalt wait",WAIT_TERM);}
#define MAS_GO	{ AUERR=CPR_exec("masgo wait",WAIT_TERM);}
#define MAS_STATUS_LOG { AUERR=CPR_exec("masstatus log2data",WAIT_TERM);}
#define MAS_STATUS_LOG_STOP { AUERR=CPR_exec("masstatus stop",WAIT_TERM);}

/* MAS3 Unit commands for manual mode */
/**************************************/
#define MMAS_ABORT_ROTATION(error)\
{\
	error = gui_call_with_result("mmasabortrotation", 0);\
}

#define MMAS_AUTOROTATION_MODE(error)\
{\
	error = gui_call_with_result("mmasautorotationmode", 0);\
}

#define MMAS_AUTO_STOP_ON_COMMUNICATION_TIMEOUT(value, result)\
{\
	if(strlen(value) == 0)\
	{\
		result = gui_call_with_result("mmasautostoponcommunicationtimeout", 0); \
	}\
	else\
	{\
		sprintf(Hilfs_string, "%s %s", "mmasautostoponcommunicationtimeout", value);	\
		result = gui_call_with_result(Hilfs_string, 0);\
	}\
}

#define MMAS_AUTO_STOP_ON_ERRORS(value, result)\
{\
	if(strlen(value) == 0)\
	{\
		result = gui_call_with_result("mmasautostoponerrors", 0); \
	}\
	else\
	{\
		sprintf(Hilfs_string, "%s %s", "mmasautostoponerrors", value); \
		result = gui_call_with_result(Hilfs_string, 0);\
	}\
}

#define MMAS_BEARING_PRESSURE(pressure)\
{\
	pressure = gui_call_with_result("mmasbearingpressure", 0); \
}

#define MMAS_BEARING_SENSE_PRESSURE(pressure)\
{\
	pressure = gui_call_with_result("mmasbearingsensepressure", 0); \
}

#define MMAS_CAN_CHANGE_TARGET_SPINRATE(value, result)\
{\
	if(strlen(value) == 0)\
	{\
		result = gui_call_with_result("mmascanchangetargetspinrate", 0); \
	}\
	else\
	{\
		sprintf(Hilfs_string, "%s %s", "mmascanchangetargetspinrate", value); \
		result = gui_call_with_result(Hilfs_string, 0);\
	}\
}

#define MMAS_CAN_ENABLE_REGULATION_MODE(value)\
{\
	value = gui_call_with_result("mmascanenableregulationmode", 0); \
}

#define MMAS_CAN_SET_PRESSURE(value)\
{\
	value = gui_call_with_result("mmascansetpressure", 0); \
}

#define MMAS_CAN_STOP_ROTATION(value, result)\
{\
	if(strlen(value) == 0)\
	{\
		result = gui_call_with_result("mmascanstoprotation", 0); \
	}\
	else\
	{\
		sprintf(Hilfs_string, "%s %s", "mmascanstoprotation", value); \
		result = gui_call_with_result(Hilfs_string, 0);\
	}\
}

#define MMAS_COMMUNICATION_TIMEOUT(timeout, result)\
{\
	if(strlen(timeout) == 0)\
	{\
		result = gui_call_with_result("mmascommunicationtimeout", 0); \
	}\
	else\
	{\
		sprintf(Hilfs_string, "%s %s", "mmascommunicationtimeout", timeout); \
		result = gui_call_with_result(Hilfs_string, 0);\
	}\
}

#define MMAS_DEFAULT_ROTATION_MODE(error)\
{\
	error = gui_call_with_result("mmasdefaultrotationmode", 0); \
}

#define MMAS_DEMANDED_BEARING_PRESSURE(value, result)\
{\
	if(strlen(value) == 0)\
	{\
		result = gui_call_with_result("mmasdemandedbearingpressure", 0); \
	}\
	else\
	{\
		sprintf(Hilfs_string, "%s %s", "mmasdemandedbearingpressure", value); \
		result = gui_call_with_result(Hilfs_string, 0);\
	}\
}

#define MMAS_DEMANDED_DRIVE_PRESSURE(value, result)\
{\
	if(strlen(value) == 0)\
	{\
		result = gui_call_with_result("mmasdemandeddrivepressure", 0); \
	}\
	else\
	{\
		sprintf(Hilfs_string, "%s %s", "mmasdemandeddrivepressure", value); \
		result = gui_call_with_result(Hilfs_string, 0);\
	}\
}

#define MMAS_DEMANDED_SPINRATE(value, result)\
{\
	if(strlen(value) == 0)\
	{\
		result = gui_call_with_result("mmasdemandedspinrate", 0); \
	}\
	else\
	{\
		sprintf(Hilfs_string, "%s %s", "mmasdemandedspinrate", value); \
		result = gui_call_with_result(Hilfs_string, 0);\
	}\
}

#define MMAS_DRIVE_PRESSURE(value)\
{\
	value = gui_call_with_result("mmasdrivepressure", 0); \
}

#define MMAS_ERROR_MESSAGES(value, result)\
{\
	sprintf(Hilfs_string, "%s %s", "mmaserrormessages", value); \
	result = gui_call_with_result(Hilfs_string, 0);\
}

#define MMAS_FINISHED_STOP_ROTATION(result)\
{\
	result = gui_call_with_result("mmasfinishedstoprotation", 0);\
}

#define MMAS_IS_ROTATION_OFF(value)\
{\
	value = gui_call_with_result("mmasisrotationoff", 0); \
}

#define MMAS_IS_ROTATION_ON(value)\
{\
	value = gui_call_with_result("mmasisrotationon", 0); \
}

#define MMAS_MAIN_PRESSURE(pressure)\
{\
	pressure = gui_call_with_result("mmasmainpressure", 0); \
}

#define MMAS_MANUAL_ROTATION_MODE(error)\
{\
	error = gui_call_with_result("mmasmanualrotationmode", 0); \
}
#define MMAS_REGULATION_TARGET_ROTATION_FREQUENCY(value)\
{\
	value = gui_call_with_result("mmasregulationtargetrotationfrequency", 0); \
}

#define MMAS_ROTATION_ERROR_CODES(value)\
{\
	value = gui_call_with_result("mmasrotationerrorcodes", 0); \
}

#define MMAS_ROTATION_MODE(value, result)\
{\
	if(strlen(value) == 0)\
	{\
		result = gui_call_with_result("mmasrotationmode", 0); \
	}\
	else\
	{\
		sprintf(Hilfs_string, "%s %s", "mmasrotationmode", value); \
		result = gui_call_with_result(Hilfs_string, 0);\
	}\
}

#define MMAS_ROTATION_STATE(result)\
{\
	result = gui_call_with_result("mmasrotationstate", 0); \
}

#define MMAS_SHOULD_STOP_ROTATION(result)\
{\
	result = gui_call_with_result("mmasshouldstoprotation", 0); \
}

#define MMAS_SPINLOCK_TOLERANCE(value, result)\
{\
	if(strlen(value) == 0)\
	{\
		result = gui_call_with_result("mmasspinlocktolerance", 0); \
	}\
	else\
	{\
		sprintf(Hilfs_string, "%s %s", "mmasspinlocktolerance", value); \
		result = gui_call_with_result(Hilfs_string, 0);\
	}\
}

#define MMAS_SPINRATE(result)\
{\
	result = gui_call_with_result("mmasspinrate", 0); \
}

#define MMAS_SPINRATE_LEVEL(result)\
{\
	result = gui_call_with_result("mmasspinratelevel", 0); \
}

#define MMAS_START_ROTATION(error)\
{\
	error = gui_call_with_result("mmasstartrotation", 0);\
}

#define MMAS_STOP_ROTATION(error)\
{\
	error = gui_call_with_result("mmasstoprotation", 0);\
}

#define MMAS_TARGET_BEARING_PRESSURE_RAMP(value, result)\
{\
	if(strlen(value) == 0)\
	{\
		result = gui_call_with_result("mmastargetbearingpressureramp", 0); \
	}\
	else\
	{\
		sprintf(Hilfs_string, "%s %s", "mmastargetbearingpressureramp", value); \
		result = gui_call_with_result(Hilfs_string, 0);\
	}\
}

#define MMAS_TARGET_DRIVE_PRESSURE_RAMP(value, result)\
{\
	if(strlen(value) == 0)\
	{\
		result = gui_call_with_result("mmastargetdrivepressureramp", 0); \
	}\
	else\
	{\
		sprintf(Hilfs_string, "%s %s", "mmastargetdrivepressureramp", value); \
		result = gui_call_with_result(Hilfs_string, 0);\
	}\
}

/* read/write commands for lists */
/*********************************/
#define RMISC(list_type,file_name) \
	{sprintf(Hilfs_string, "%s %s %s", "rmisc", list_type, file_name); \
	SETCURDATA \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define WMISC(list_type,file_name) \
	{sprintf(Hilfs_string, "%s %s %s %s", "wmisc", list_type, file_name, "y"); \
	SETCURDATA \
	AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}


/* getlim commands	  */
/**************************/
#define GETLIM		{SETCURDATA AUERR=CPR_exec("getlim1d",WAIT_TERM);}
#define GETLCOSY	{SETCURDATA AUERR=CPR_exec("getlcosy",WAIT_TERM);}
#define GETLXHCO	{SETCURDATA AUERR=CPR_exec("getlxhco",WAIT_TERM);}
#define GETLJRES	{SETCURDATA AUERR=CPR_exec("getljres",WAIT_TERM);}
#define GETLINV		{SETCURDATA AUERR=CPR_exec("getlinv",WAIT_TERM);}

#define GETLIM_PP	{SETCURDATA AUERR=CPR_exec("getlim1dpp",WAIT_TERM);}
#define GETLCOSY_PP	{SETCURDATA AUERR=CPR_exec("getlcosypp",WAIT_TERM);}
#define GETLXHCO_PP	{SETCURDATA AUERR=CPR_exec("getlxhcopp",WAIT_TERM);}
#define GETLJRES_PP	{SETCURDATA AUERR=CPR_exec("getljrespp",WAIT_TERM);}
#define GETLINV_PP	{SETCURDATA AUERR=CPR_exec("getlinvpp",WAIT_TERM);}
#define SINO		{SETCURDATA AUERR=CPR_exec("sino noprint",WAIT_TERM);}

/* 1D processing commands */
/**************************/

#define ABS	{SETCURDATA AUERR=CPR_exec( "abs",WAIT_TERM);}
#define ABSD	{SETCURDATA AUERR=CPR_exec( "absd",WAIT_TERM);}
#define ABSF	{SETCURDATA AUERR=CPR_exec( "absf",WAIT_TERM);}
#define ABSN	{SETCURDATA AUERR=CPR_exec( "abs n",WAIT_TERM);}
#define APBK	{SETCURDATA AUERR=CPR_exec( "sendgui apbk",WAIT_TERM);}
#define APK	{SETCURDATA AUERR=CPR_exec( "apk",WAIT_TERM);}
#define APK0	{SETCURDATA AUERR=CPR_exec( "apk0",WAIT_TERM);}
#define APK0F	{SETCURDATA AUERR=CPR_exec( "apk0f",WAIT_TERM);}
#define APK1	{SETCURDATA AUERR=CPR_exec( "apk1",WAIT_TERM);}
#define APKF	{SETCURDATA AUERR=CPR_exec( "apkf",WAIT_TERM);}
#define APKS	{SETCURDATA AUERR=CPR_exec( "apks",WAIT_TERM);}
#define BC	{SETCURDATA AUERR=CPR_exec( "bc same",WAIT_TERM);}
#define BCM	{SETCURDATA AUERR=CPR_exec( "bcm",WAIT_TERM);}
#define EF	{SETCURDATA AUERR=CPR_exec( "ef same",WAIT_TERM);}
#define EFP	{SETCURDATA AUERR=CPR_exec( "efp same",WAIT_TERM);}
#define EM	{SETCURDATA AUERR=CPR_exec( "em same",WAIT_TERM);}
#define FMC	{SETCURDATA AUERR=CPR_exec( "fmc same",WAIT_TERM);}
#define FT	{SETCURDATA AUERR=CPR_exec( "ft same",WAIT_TERM);}
#define FP	{SETCURDATA AUERR=CPR_exec( "fp same",WAIT_TERM);}
#define GF	{SETCURDATA AUERR=CPR_exec( "gf same",WAIT_TERM);}
#define GFP	{SETCURDATA AUERR=CPR_exec( "gfp same",WAIT_TERM);}
#define GM	{SETCURDATA AUERR=CPR_exec( "gm same",WAIT_TERM);}
#define HT	{SETCURDATA AUERR=CPR_exec( "ht",WAIT_TERM);}
#define IFT	{SETCURDATA AUERR=CPR_exec( "ift",WAIT_TERM);}
#define MC	{SETCURDATA AUERR=CPR_exec( "mc",WAIT_TERM);}
#define PK	{SETCURDATA AUERR=CPR_exec( "pk",WAIT_TERM);}
#define PS	{SETCURDATA AUERR=CPR_exec( "ps",WAIT_TERM);}
#define QSIN	{SETCURDATA AUERR=CPR_exec( "qsin same",WAIT_TERM);}
#define SAB	{SETCURDATA AUERR=CPR_exec( "sab",WAIT_TERM);}
#define SINM	{SETCURDATA AUERR=CPR_exec( "sinm same",WAIT_TERM);}
#define SREF	{SETCURDATA AUERR=CPR_exec( "sref",WAIT_TERM);}
#define TM	{SETCURDATA AUERR=CPR_exec( "tm same",WAIT_TERM);}
#define TRF	{SETCURDATA AUERR=CPR_exec( "trf same",WAIT_TERM);}
#define TRFP	{SETCURDATA AUERR=CPR_exec( "trfp",WAIT_TERM);}
#define UWM	{SETCURDATA AUERR=CPR_exec( "uwm same",WAIT_TERM);}
#define GENFID(num) \
	{sprintf(Hilfs_string, "%s %d %s", "genfid", num, "y n"); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define CONVDTA(num) \
	{sprintf(Hilfs_string, "%s %d %s", "convdta", num, "y n"); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}


/* Deconvolution commands */
/**************************/

#define LDCON	{SETCURDATA AUERR=CPR_exec("ldcon",WAIT_TERM);}
#define GDCON	{SETCURDATA AUERR=CPR_exec("gdcon",WAIT_TERM);}
#define MDCON	{SETCURDATA AUERR=CPR_exec("mdcon",WAIT_TERM);}


/* Algebra commands */
/********************/

#define ADD	{SETCURDATA AUERR=CPR_exec( "add",WAIT_TERM);}
#define ADDFID	{SETCURDATA AUERR=CPR_exec( "addfid y",WAIT_TERM);}
#define ADDC	{SETCURDATA AUERR=CPR_exec( "addc",WAIT_TERM);}
#define AND	{SETCURDATA AUERR=CPR_exec( "and",WAIT_TERM);}
#define DIV	{SETCURDATA AUERR=CPR_exec( "div",WAIT_TERM);}
#define DT	{SETCURDATA AUERR=CPR_exec( "dt",WAIT_TERM);}
#define FILT	{SETCURDATA AUERR=CPR_exec( "filt",WAIT_TERM);}
#define LS	{SETCURDATA AUERR=CPR_exec( "ls",WAIT_TERM);}
#define MUL	{SETCURDATA AUERR=CPR_exec( "mul",WAIT_TERM);}
#define MULC	{SETCURDATA AUERR=CPR_exec( "mulc",WAIT_TERM);}
#define NM	{SETCURDATA AUERR=CPR_exec( "nm",WAIT_TERM);}
#define RS	{SETCURDATA AUERR=CPR_exec( "rs",WAIT_TERM);}
#define RV	{SETCURDATA AUERR=CPR_exec( "rv",WAIT_TERM);}
#define ZF	{SETCURDATA AUERR=CPR_exec( "zf",WAIT_TERM);}
#define ZP	{SETCURDATA AUERR=CPR_exec( "zp",WAIT_TERM);}


/* T1 processing commands */
/**************************/

#define CT1		{SETCURDATA AUERR=CPR_exec( "ct1",WAIT_TERM);}
#define CT2		{SETCURDATA AUERR=CPR_exec( "ct2",WAIT_TERM);}
#define DAT1		{SETCURDATA AUERR=CPR_exec( "dat1",WAIT_TERM);}
#define DAT2		{SETCURDATA AUERR=CPR_exec( "dat2",WAIT_TERM);}
#define PD		{SETCURDATA AUERR=CPR_exec( "pd",WAIT_TERM);}
#define PD0		{SETCURDATA AUERR=CPR_exec( "pd0",WAIT_TERM);}
#define PF		{SETCURDATA AUERR=CPR_exec( "pf",WAIT_TERM);}
#define PFT2		{SETCURDATA AUERR=CPR_exec( "pft2",WAIT_TERM);}
#define SIMFIT		{SETCURDATA AUERR=CPR_exec( "simfit",WAIT_TERM);}
#define SIMFITALL	{SETCURDATA AUERR=CPR_exec( "simfit all",WAIT_TERM);}
#define SIMFITASCALL	{SETCURDATA AUERR=CPR_exec( "simfit asc all",WAIT_TERM);}


/* 2D processing commands */
/**************************/

#define ABS1	{SETCURDATA AUERR=CPR_exec( "abs1",WAIT_TERM);}
#define ABS2	{SETCURDATA AUERR=CPR_exec( "abs2",WAIT_TERM);}
#define ABSD1	{SETCURDATA AUERR=CPR_exec( "absd1",WAIT_TERM);}
#define ABSD2	{SETCURDATA AUERR=CPR_exec( "absd2",WAIT_TERM);}
#define ABSOT1	{SETCURDATA AUERR=CPR_exec( "absot1",WAIT_TERM);}
#define ABSOT2	{SETCURDATA AUERR=CPR_exec( "absot2",WAIT_TERM);}
#define ABST1	{SETCURDATA AUERR=CPR_exec( "abst1",WAIT_TERM);}
#define ABST2	{SETCURDATA AUERR=CPR_exec( "abst2",WAIT_TERM);}
#define ADD2D	{SETCURDATA AUERR=CPR_exec( "add2d",WAIT_TERM);}
#define ADDSER	{SETCURDATA AUERR=CPR_exec( "addser",WAIT_TERM);}
#define BCM1	{SETCURDATA AUERR=CPR_exec( "bcm1",WAIT_TERM);}
#define BCM2	{SETCURDATA AUERR=CPR_exec( "bcm2",WAIT_TERM);}
#define LEVCALC	{SETCURDATA AUERR=CPR_exec( "levcalc",WAIT_TERM);}
#define MUL2D	{SETCURDATA AUERR=CPR_exec( "mul2d",WAIT_TERM);}
#define PTILT	{SETCURDATA AUERR=CPR_exec( "ptilt",WAIT_TERM);}
#define PTILT1	{SETCURDATA AUERR=CPR_exec( "ptilt1",WAIT_TERM);}
#define REV1	{SETCURDATA AUERR=CPR_exec( "rev1",WAIT_TERM);}
#define REV2	{SETCURDATA AUERR=CPR_exec( "rev2",WAIT_TERM);}
#define SUB1	{SETCURDATA AUERR=CPR_exec( "sub1",WAIT_TERM);}
#define SUB2	{SETCURDATA AUERR=CPR_exec( "sub2",WAIT_TERM);}
#define SUB1D1	{SETCURDATA AUERR=CPR_exec( "sub1d1",WAIT_TERM);}
#define SUB1D2	{SETCURDATA AUERR=CPR_exec( "sub1d2",WAIT_TERM);}
#define SYM	{SETCURDATA AUERR=CPR_exec( "sym",WAIT_TERM);}
#define SYMA	{SETCURDATA AUERR=CPR_exec( "syma",WAIT_TERM);}
#define SYMJ	{SETCURDATA AUERR=CPR_exec( "symj",WAIT_TERM);}
#define TILT	{SETCURDATA AUERR=CPR_exec( "tilt",WAIT_TERM);}
#define XF1	{SETCURDATA AUERR=CPR_exec( "xf1",WAIT_TERM);}
#define XF1P	{SETCURDATA AUERR=CPR_exec( "xf1p",WAIT_TERM);}
#define XF2	{SETCURDATA AUERR=CPR_exec( "xf2 same",WAIT_TERM);}
#define XF2P	{SETCURDATA AUERR=CPR_exec( "xf2p",WAIT_TERM);}
#define XFB	{SETCURDATA AUERR=CPR_exec( "xfb same",WAIT_TERM);}
#define XFBP	{SETCURDATA AUERR=CPR_exec( "xfbp",WAIT_TERM);}
#define XF1M	{SETCURDATA AUERR=CPR_exec( "xf1m",WAIT_TERM);}
#define XF2M	{SETCURDATA AUERR=CPR_exec( "xf2m",WAIT_TERM);}
#define XFBM	{SETCURDATA AUERR=CPR_exec( "xfbm",WAIT_TERM);}
#define XF1PS	{SETCURDATA AUERR=CPR_exec( "xf1ps",WAIT_TERM);}
#define XF2PS	{SETCURDATA AUERR=CPR_exec( "xf2ps",WAIT_TERM);}
#define XFBPS	{SETCURDATA AUERR=CPR_exec( "xfbps",WAIT_TERM);}
#define XHT1	{SETCURDATA AUERR=CPR_exec( "xht1",WAIT_TERM);}
#define XHT2	{SETCURDATA AUERR=CPR_exec( "xht2",WAIT_TERM);}
#define XIF1	{SETCURDATA AUERR=CPR_exec( "xif1",WAIT_TERM);}
#define XIF2	{SETCURDATA AUERR=CPR_exec( "xif2",WAIT_TERM);}
#define XTRF	{SETCURDATA AUERR=CPR_exec( "xtrf same",WAIT_TERM);}
#define XTRF2	{SETCURDATA AUERR=CPR_exec( "xtrf2 same",WAIT_TERM);}
#define XTRFP	{SETCURDATA AUERR=CPR_exec( "xtrfp",WAIT_TERM);}
#define XTRFP1	{SETCURDATA AUERR=CPR_exec( "xtrfp1",WAIT_TERM);}
#define XTRFP2	{SETCURDATA AUERR=CPR_exec( "xtrfp2",WAIT_TERM);}
#define ZERT1	{SETCURDATA AUERR=CPR_exec( "zert1",WAIT_TERM);}
#define ZERT2	{SETCURDATA AUERR=CPR_exec( "zert2",WAIT_TERM);}
#define GENSER(num) \
	{sprintf(Hilfs_string, "%s %d %s", "genser", num, "y n"); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}


/* 3d processing commands    */
/*****************************/
#define FT3D	{SETCURDATA AUERR=CPR_exec( "ft3d n",WAIT_TERM);}
#define TF3(sav_im) \
  { sprintf(Hilfs_string, "%s %s", "tf3", sav_im);	\
    SETCURDATA AUERR=CPR_exec(Hilfs_string, WAIT_TERM); }
#define TF2(sav_im) \
  { sprintf(Hilfs_string, "%s %s", "tf2", sav_im);	\
    SETCURDATA AUERR=CPR_exec(Hilfs_string, WAIT_TERM); }
#define TF1(sav_im) \
  { sprintf(Hilfs_string, "%s %s", "tf1", sav_im);	\
    SETCURDATA AUERR=CPR_exec(Hilfs_string, WAIT_TERM); }
#define TF3P(sav_im) \
  { sprintf(Hilfs_string, "%s %s", "tf3p", sav_im);	\
    SETCURDATA AUERR=CPR_exec(Hilfs_string, WAIT_TERM); }
#define TF2P(sav_im) \
  { sprintf(Hilfs_string, "%s %s", "tf2p", sav_im);	\
    SETCURDATA AUERR=CPR_exec(Hilfs_string, WAIT_TERM); }
#define TF1P(sav_im) \
  { sprintf(Hilfs_string, "%s %s", "tf1p", sav_im);	\
    SETCURDATA AUERR=CPR_exec(Hilfs_string, WAIT_TERM); }
#define TABS3	{SETCURDATA AUERR=CPR_exec("tabs3", WAIT_TERM);}
#define TABS2	{SETCURDATA AUERR=CPR_exec("tabs2", WAIT_TERM);}
#define TABS1	{SETCURDATA AUERR=CPR_exec("tabs1", WAIT_TERM);}
#define R12(slice_nr, procno) \
    { sprintf(Hilfs_string, "%s %d %d %s", "r12", slice_nr, procno, "n"); \
    SETCURDATA \
    AUERR=CPR_exec(Hilfs_string, WAIT_TERM); }
#define R13(slice_nr, procno) \
    { sprintf(Hilfs_string, "%s %d %d %s", "r13", slice_nr, procno, "n"); \
    SETCURDATA \
    AUERR=CPR_exec(Hilfs_string, WAIT_TERM); }
#define R23(slice_nr, procno) \
    { sprintf(Hilfs_string, "%s %d %d %s", "r23", slice_nr, procno, "n"); \
    SETCURDATA \
    AUERR=CPR_exec(Hilfs_string, WAIT_TERM); }

/* read/write rows, columns, projections */
/*****************************************/

#define RSC(num, pno) \
    {if (pno == (-1)) sprintf(Hilfs_string, "%s %d %s", "rsc", num, "n"); \
     else sprintf(Hilfs_string, "%s %d %d %s", "rsc", num, pno, "n"); \
     SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define RSR(num, pno) \
    {if (pno == (-1)) sprintf(Hilfs_string, "%s %d %s", "rsr", num, "n"); \
     else sprintf(Hilfs_string, "%s %d %d %s", "rsr", num, pno, "n"); \
     SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define RSER(num, eno, pno) \
    {if (eno == (-1)) sprintf(Hilfs_string, "%s %d %s", "rser", num, "n"); \
     else sprintf(Hilfs_string, "%s %d %d %d %s", "rser", num, eno, pno, "n"); \
     SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define RSER2D(direction, num, eno) \
    {sprintf(Hilfs_string, "%s s%d %d %d %s", "rser2d", direction, num, eno, "y n"); \
     SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define F1SUM(n1,n2,pno) \
	{if (pno == (-1)) \
	  sprintf(Hilfs_string,"f1sum %d %d n", n1,n2); \
	else \
	  sprintf(Hilfs_string,"f1sum %d %d %d n", n1,n2,pno); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define F2SUM(n1,n2,pno) \
	{if (pno == (-1)) \
	  sprintf(Hilfs_string,"f2sum %d %d n", n1,n2); \
	else \
	  sprintf(Hilfs_string,"f2sum %d %d %d n", n1,n2,pno); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define F1DISCO(n1,n2,r1,pno) \
	{if (pno == (-1)) \
	  sprintf(Hilfs_string,"f1disco %d %d %d n", n1,n2,r1); \
	else \
	  sprintf(Hilfs_string,"f1disco %d %d %d %d n", n1,n2,r1,pno); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define F2DISCO(n1,n2,r1,pno) \
	{if (pno == (-1)) \
	  sprintf(Hilfs_string,"f2disco %d %d %d n", n1,n2,r1); \
	else \
	  sprintf(Hilfs_string,"f2disco %d %d %d %d n", n1,n2,r1,pno); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define F1PROJN(n1,n2,pno) \
	{if (pno == (-1)) \
	  sprintf(Hilfs_string,"f1projn %d %d n", n1,n2); \
	else \
	  sprintf(Hilfs_string,"f1projn %d %d %d n", n1,n2,pno); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define F1PROJP(n1,n2,pno) \
	{if (pno == (-1)) \
	  sprintf(Hilfs_string,"f1projp %d %d n", n1,n2); \
	else \
	  sprintf(Hilfs_string,"f1projp %d %d %d n", n1,n2,pno); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define F2PROJN(n1,n2,pno) \
	{if (pno == (-1)) \
	  sprintf(Hilfs_string,"f2projn %d %d n", n1,n2); \
	else \
	  sprintf(Hilfs_string,"f2projn %d %d %d n", n1,n2,pno); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define F2PROJP(n1,n2,pno) \
	{if (pno == (-1)) \
	  sprintf(Hilfs_string,"f2projp %d %d n", n1,n2); \
	else \
	  sprintf(Hilfs_string,"f2projp %d %d %d n", n1,n2,pno); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define RHNP(pno) \
	{if(pno == (-1)) sprintf(Hilfs_string,"rhnp n"); \
	else sprintf(Hilfs_string,"rhnp %d n", pno); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define RHPP(pno) \
	{if (pno == (-1)) sprintf(Hilfs_string,"rhpp n"); \
	else sprintf(Hilfs_string,"rhpp %d n", pno); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define RVNP(pno) \
	{if (pno == (-1)) sprintf(Hilfs_string,"rvnp n"); \
	else sprintf(Hilfs_string,"rvnp %d n", pno); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define RVPP(pno) \
	{if (pno == (-1)) sprintf(Hilfs_string,"rvpp n"); \
	else sprintf(Hilfs_string,"rvpp %d n", pno); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define WSC(num,pno,expno1,name1,user1,disk1) \
	{sprintf(Hilfs_string, "%s %d %d %d \"%s\" \"%s\" %s", \
		 "wsc", num, pno, expno1, name1, disk1, "n");  \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define WSR(num,pno,expno1,name1,user1,disk1) \
	{sprintf(Hilfs_string, "%s %d %d %d \"%s\" \"%s\" %s", \
		 "wsr", num, pno, expno1, name1, disk1, "n");  \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define WSER(fidnum,name1,expno1,procno1,disk1,user1) \
	{sprintf(Hilfs_string, "%s %d %d %d \"%s\" \"%s\" %s", \
		 "wser", fidnum, expno1, procno1, name1, disk1, "n"); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define WSERP(fidnum,name1,expno1,procno1,disk1,user1) \
	{sprintf(Hilfs_string, "%s %d %d %d \"%s\" \"%s\" %s", \
		 "wserp", fidnum, expno1, procno1, name1, disk1, "n"); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}


/* Audit trail data checksum commands */
/**************************************/

#define GDCHECK		{SETCURDATA AUERR=CPR_exec("gdcheck", WAIT_TERM);}
#define GDCHECK_RAW	{SETCURDATA AUERR=CPR_exec("gdcheck raw", WAIT_TERM);}

#define AUDITCOMMENTP(co)					\
{								\
  FILE* aufp;							\
  const char* proc_path = PROCPATH("auditc.txt");		\
  aufp = fopen(proc_path, "wt");				\
  if (aufp == 0)						\
  {								\
    Proc_err(DEF_ERR_OPT, "%s %s", "Cannot open", proc_path);	\
    ABORT							\
  }								\
  if (fprintf(aufp, "%s", co) < 0)				\
  {								\
    Proc_err(DEF_ERR_OPT, "%s %s", "Cannot write", proc_path);	\
    ABORT							\
  }								\
  fclose(aufp);							\
  AUERR=CPR_exec("gdcheck comment", WAIT_TERM);			\
}

#define AUDITCOMMENTA(co)					\
{								\
  FILE* aufp;							\
  const char* acqu_path = ACQUPATH("auditc.txt");		\
  aufp = fopen(acqu_path, "wt");				\
  if (aufp == 0)						\
  {								\
    Proc_err(DEF_ERR_OPT, "%s %s", "Cannot open", acqu_path);	\
    ABORT							\
  }								\
  if (fprintf(aufp, "%s", co) < 0)				\
  {								\
    Proc_err(DEF_ERR_OPT, "%s %s", "Cannot write", acqu_path);	\
    ABORT							\
  }								\
  fclose(aufp);							\
  AUERR=CPR_exec("gdcheck raw comment", WAIT_TERM);		\
}

/* Peak picking and output commands */
/************************************/

#define PP	{SETCURDATA AUERR=CPR_exec( "pp",WAIT_TERM);}
#define PPH	{SETCURDATA AUERR=CPR_exec( "pph",WAIT_TERM);}
#define PPJ	{SETCURDATA AUERR=CPR_exec( "ppj",WAIT_TERM);}
#define PPP	{SETCURDATA AUERR=CPR_exec( "ppp",WAIT_TERM);}
#define PP2D	{SETCURDATA AUERR=CPR_exec( "sendgui pp nodia",WAIT_TERM);}
#define LI	{SETCURDATA AUERR=CPR_exec( "li",WAIT_TERM);}
#define LIPP	{SETCURDATA AUERR=CPR_exec( "lipp",WAIT_TERM);}
#define LIPPF	{SETCURDATA AUERR=CPR_exec( "lippf",WAIT_TERM);}

#define NMRQUANT(x) \
	{sprintf(Hilfs_string, "%s %s", "nmrquant", x);	\
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define SIGREG	{SETCURDATA AUERR=CPR_exec( "sendgui sigreg",WAIT_TERM);}


/* Plot commands */
/*****************/

#undef  PLOT	/* since it was already defined in pstruc.h */
#define PLOT	{SETCURDATA AUERR=CPR_exec("plot",WAIT_TERM);}

#define AUTOPLOT \
    if (search_argc_argv_arg(i_argc,i_argv,"noplot") == 0) {\
	SETCURDATA AUERR=CPR_exec("autoplot",WAIT_TERM);} \
    else {SETCURDATA AUERR=CPR_exec("xwp_lp",WAIT_TERM);}

#define XWP_LP	{SETCURDATA AUERR=CPR_exec("xwp_lp",WAIT_TERM);}
#define XWP_PP	{SETCURDATA AUERR=CPR_exec("pp",WAIT_TERM);}
#define DECLARE_PORTFOLIO

#define CREATE_PORTFOLIO(v1)			\
    strcpy(pf_name, v1);			\
    pf_fd = fopen(pf_name, "wt+");		\
    if(!pf_fd)					\
    {						\
	Proc_err(ERROPT_AK_NO,			\
"Error: Could not create portfolio file:\n"	\
"%s\nAUTOPLOT_WITH_PORTFOLIO plots will\n"	\
"probably not come out correctly.",pf_name);	\
    }						\
    else					\
    {						\
	fprintf(pf_fd, "#!Portfolio 0.6\n");	\
	fprintf(pf_fd, "1 /dummy\n");		\
    }

#define ADD_TO_PORTFOLIO(v1,v2,v3,v4,v5)	\
    if (pf_fd)					\
    {						\
	fprintf(pf_fd, "\"%s\" \"%s\" %d %d\n", v1, v3, v4, v5); \
    }

#define ADD_CURDAT_TO_PORTFOLIO			\
    if (pf_fd)					\
    {						\
	fprintf(pf_fd, "\"%s\" \"%s\" %d %d\n",	\
		disk, name, expno, procno);	\
    }

#define CLOSE_PORTFOLIO	\
    if (pf_fd)		\
    fclose(pf_fd);

#define XWINPLOT_WITH_PORTFOLIO				\
    sprintf(pf_cmdLine, "plot -p \"%s\"", pf_name);	\
    AUERR=CPR_exec(pf_cmdLine, WAIT_TERM);

#define AUTOPLOT_WITH_PORTFOLIO					\
    if (search_argc_argv_arg(i_argc, i_argv, "noplot") == 0) {	\
    FETCHPAR("LAYOUT", pf_layoutFile);				\
    sprintf(pf_cmdLine, "autoplot -p \"%s\" \"%s\"",		\
    pf_name, pf_layoutFile);					\
    AUERR=CPR_exec(pf_cmdLine, WAIT_TERM);}			\
    else {SETCURDATA AUERR=CPR_exec("xwp_lp", WAIT_TERM);}

#define AUTOPLOT_WITH_PORTFOLIO_TO_FILE(v1)			\
    FETCHPAR("LAYOUT", pf_layoutFile);				\
    sprintf(pf_cmdLine, "autoplot -e \"%s\" -p \"%s\" \"%s\"",	\
    v1, pf_name, pf_layoutFile);				\
    AUERR=CPR_exec(pf_cmdLine, WAIT_TERM);

#define AUTOPLOT_TO_FILE(v1)					\
    FETCHPAR("LAYOUT", pf_layoutFile);				\
    sprintf(pf_cmdLine, "autoplot -e \"%s\" \"%s\"",		\
    v1, pf_layoutFile);						\
    AUERR=CPR_exec(pf_cmdLine, WAIT_TERM);

#define BIRTREPORT(dp, xp, rp)					\
    {sprintf(Hilfs_string,					\
	     "%s %s %s=\"%s\" %s=\"%s\" %s=\"%s\"",		\
	     "sendgui", "runbirt", "designPath", dp,		\
	     "xmlPath", xp, "reportPath", rp);			\
    AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define BIRTREPORT_AND_CHOOSE_OPEN_REPORT(dp, xp, rp, sr)	\
    {sprintf(Hilfs_string,					\
	     "%s %s %s=\"%s\" %s=\"%s\" %s=\"%s\" %s=\"%s\"",	\
	     "sendgui", "runbirt", "designPath", dp,		\
	     "xmlPath", xp, "reportPath", rp, "showReport", sr);\
    AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

/* Data set creation commands */
/******************************/

#define WRA(eno) \
	{sprintf(Hilfs_string, "%s %d %s", "wraau", eno, "y");	\
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define WRP(pno) \
	{sprintf(Hilfs_string, "%s %d %s", "wrpau", pno, "y");	\
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define WRPA(name1,expno1,procno1,disk1,user1) \
	{sprintf(Hilfs_string, "%s \"%s\" %d %d \"%s\" %s",	\
		 "wrpa", name1, expno1, procno1, disk1, "y");	\
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define WRAPARAM(eno) \
	{sprintf(Hilfs_string, "%s %d %s", "wraparamau", eno, "y"); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define WRPPARAM(pno) \
	{sprintf(Hilfs_string, "%s %d %s", "wrpparamau", pno, "y"); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}


/* Data set selection commands */
/*******************************/

#define USELASTPARS lastparflag=0; /*setcurdat copies pars in any case*/
#define USECURPARS  lastparflag=1; /* only if not yet existing */

#define DU(x)		strcpy(disk, x);
/*
 #define OWNER(x)	strcpy(user, x);
*/
#define SETUSER(x)	strcpy(user, x);
#define RE(x)		strcpy(name, x);
#define REXPNO(x)	expno=x;
#define IEXPNO		expno++;
#define DEXPNO		expno--;
#define RPROCNO(x)	procno=x;
#define IPROCNO		procno++;
#define DPROCNO		procno--;

#define DATASET(name1,expno1,procno1,disk1,user1)		\
   { strcpy(name,name1); strcpy(disk,disk1);			\
   expno=expno1; procno=procno1;				\
   SETCURDATA }

#define DATASET2(name,expno,procno,disk,user)			\
   { SETCURDATA							\
   AUERR=setcurdat(2, curdat, disk, name, expno, procno); }

#define DATASET3(name,expno,procno,disk,user)			\
   { SETCURDATA							\
   AUERR=setcurdat(3, curdat, disk, name, expno, procno); }

#define GETCURDATA \
    AUERR=getcurdat(1, curdat,disk, name, &expno, &procno);

#define GETCURDATA2 \
   AUERR=getcurdat(2, curdat, disk2, name2, &expno2, &procno2);

#define GETCURDATA3 \
   AUERR=getcurdat(3, curdat, disk3, name3, &expno3, &procno3);

#define REFRESHDATA Show_meta(SM_RAW | SM_RAWP | SM_PROC | SM_PROCP);

/* Control statements */
/**********************/

#define TIMES(n) \
  for(loopcount1=0; loopcount1<n; loopcount1++) {

#define TIMES2(n) \
  for(loopcount2=0; loopcount2<n; loopcount2++) {

#define TIMES3(n) \
  for(loopcount3=0; loopcount3<n; loopcount3++) {

#define TIMESLIST \
 for(listcount1=0; listcount1<loopcountlist[xloopcount]; listcount1++) {

#define TIMESINFINITE	\
 for(loopcountinf=0; loopcountinf<10000000; loopcountinf++) {

#define END }


/* Fetching and storing parameters */
/***********************************/

/* normal 1D parameters */
#define FETCHPAR(par, valuepntr) \
   {SETCURDATA AUERR=fetchstor(curdat, 1, 1, "1", par, valuepntr);}
#define STOREPAR(par, value) \
   {SETCURDATA AUERR=fetchstor(curdat, 1, 0, "1", par, value);}


/* status parameters */
#define FETCHPARS(par, valuepntr) \
   {SETCURDATA AUERR=fetchstor(curdat, 1, 1, "1s", par, valuepntr);}
#define STOREPARS(par, value) \
   {SETCURDATA AUERR=fetchstor(curdat, 1, 0, "1s", par, value);}


/* parameters in higher dimensions, dim is the dimension (F1: dim = 1 etc.)  */
#define FETCHPARN(dim, par, valuepntr) \
   {sprintf(Hilfs_string, "f%d", dim); \
    SETCURDATA AUERR=fetchstor(curdat, 1, 1, Hilfs_string, par, valuepntr);}
#define STOREPARN(dim, par, value) \
   {sprintf(Hilfs_string, "f%d", dim); \
    SETCURDATA AUERR=fetchstor(curdat, 1, 0, Hilfs_string, par, value);}


/* status parameters in higher dimensions */
#define FETCHPARNS(dim, par, valuepntr) \
   {sprintf(Hilfs_string, "f%ds", dim); \
    SETCURDATA AUERR=fetchstor(curdat, 1, 1, Hilfs_string, par, valuepntr);}
#define STOREPARNS(dim, par, value) \
   {sprintf(Hilfs_string, "f%ds", dim); \
    SETCURDATA AUERR=fetchstor(curdat, 1, 0, Hilfs_string, par, value);}

/* obsolete macros, replace by the macros above: */
/*************************************************/
#define FETCHPAR1(par, valuepntr) \
   {SETCURDATA AUERR=fetchstor(curdat, 1, 1, "2", par, valuepntr);}
#define FETCHPAR1S(par, valuepntr) \
   {SETCURDATA AUERR=fetchstor(curdat, 1, 1, "2s", par, valuepntr);}
#define FETCHPAR3(par, valuepntr) \
   {SETCURDATA AUERR=fetchstor(curdat, 1, 1, "3", par, valuepntr);}
#define FETCHPAR3S(par, valuepntr) \
   {SETCURDATA AUERR=fetchstor(curdat, 1, 1, "3s", par, valuepntr);}
#define FETCHPARM(par, valuepntr)  \
   {SETCURDATA AUERR=fetchstorm(curdat, 1, par, valuepntr);}
#define STOREPAR1(par, value) \
   {SETCURDATA AUERR=fetchstor(curdat, 1, 0, "2", par, value);}
#define STOREPAR1S(par, value) \
   {SETCURDATA AUERR=fetchstor(curdat, 1, 0, "2s", par, value);}
#define STOREPAR3(par, value) \
   {SETCURDATA AUERR=fetchstor(curdat, 1, 0, "3", par, value);}
#define STOREPAR3S(par, value) \
   {SETCURDATA AUERR=fetchstor(curdat, 1, 0, "3s", par, value);}
#define STOREPARM(par, value)  \
   {SETCURDATA AUERR=fetchstorm(curdat, 0, par, value);}

/* T1 parameters */
#define FETCHT1PAR(par, valuepntr)  \
   {SETCURDATA AUERR=fetchstort1(curdat, 1, par, valuepntr);}
#define STORET1PAR(par, value)  \
   {SETCURDATA AUERR=fetchstort1(curdat, 0, par, value);}

/* dosy parameters */
#define FETCHDOSYPAR(par, valuepntr)  \
   {SETCURDATA AUERR=fetchstordosy(curdat, 1, par, valuepntr);}
#define STOREDOSYPAR(par, value)  \
   {SETCURDATA AUERR=fetchstordosy(curdat, 0, par, value);}

/* mem parameters */
#define STOREMEMPAR(par, valuepntr) \
   {SETCURDATA AUERR=fetchstormem(curdat,1,0,"1",par,valuepntr);}
#define FETCHMEMPAR(par, valuepntr) \
   {SETCURDATA AUERR=fetchstormem(curdat,1,1,"1",par,valuepntr);}

/* eretic parameters */
#define STOREERETICPAR(par, valuepntr) \
   {SETCURDATA AUERR=fetchstorcommon(curdat, 1, 0, par, valuepntr);}
#define FETCHERETICPAR(par, valuepntr) \
   {SETCURDATA AUERR=fetchstorcommon(curdat, 1, 1, par, valuepntr);}

/* prodecomp_reg parameters */
#define STOREPRODECOMP_REGPAR(par, valuepntr) \
   {SETCURDATA AUERR=fetchstorcommon(curdat, 3, 0, par, valuepntr);}
#define FETCHPRODECOMP_REGPAR(par, valuepntr) \
   {SETCURDATA AUERR=fetchstorcommon(curdat, 3, 1, par, valuepntr);}

/* used_from parameters */
#define STOREUSED_FROMPAR(par, valuepntr) \
   {SETCURDATA AUERR=fetchstorcommon(curdat, 2, 0, par, valuepntr);}
#define FETCHUSED_FROMPAR(par, valuepntr) \
   {SETCURDATA AUERR=fetchstorcommon(curdat, 2, 1, par, valuepntr);}


/* Parameter set handling commands */
/***********************************/

#define WPAR(name, type) \
	{strcpy(Hilfs_string, "wparau ");	\
	strcat(Hilfs_string, name);		\
	strcat(Hilfs_string, " ");		\
	strcat(Hilfs_string, type);		\
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define RPAR(name, type) \
	{strcpy(Hilfs_string, "rparau ");	\
	strcat(Hilfs_string, name);		\
	strcat(Hilfs_string, " ");		\
	strcat(Hilfs_string, type);		\
	strcat(Hilfs_string, " remove=yes");	\
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}

#define DELPAR(name) \
	{strcpy(Hilfs_string, "delpar ");	\
	strcat(Hilfs_string, name);		\
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}


/* Commands to return from an AU program */
/*****************************************/

#define  QUIT	return(AUERR);}	/* last command of an au program*/
#define  QUITMSG(x)	\
		{ Proc_err(0,"%s", x);		\
		return(AUERR);}} /* last command of an au program*/
#define  ABORT	return(-1);	/* error return			*/
#define  STOP   return(AUERR);	/* normal return		*/
#define  STOPMSG(x)	\
		{ Proc_err(0,"%s", x);		\
		return(AUERR);}
#define  ERRORABORT	\
		if (AUERR<0) return(AUERR);

#define  GETCURDATA23_ERROR_ABORT	\
		if (AUERR) { Proc_err(ERROR_OPT,"%s",getcurdatLastErrorMsg());return(AUERR);}

/* Commands to execute an AU program */
/*************************************/

#define XAUA	  {SETCURDATA AUERR=execau(0,WAIT_TERM,"",curdat,"");}
#define XAUP	  {SETCURDATA AUERR=execau(1,WAIT_START,"",curdat,"");}
#define XAUPW	  {SETCURDATA AUERR=execau(1,WAIT_TERM,"",curdat,"");}
#define XAU(name,arg) {SETCURDATA AUERR=execau(2,WAIT_TERM,name,curdat,arg);}
#define XCMD(command) {SETCURDATA AUERR=CPR_exec(command,WAIT_TERM);}
#define XMAC(name) \
	{sprintf(Hilfs_string, "%s %s %s", "sendgui clientid=local", "xmac", name); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}
#define XPY(name) \
	{sprintf(Hilfs_string, "%s %s %s", "sendgui clientid=local", "xpy", name); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_TERM);}


/* Conversion of a A2000/3000 file to X32 */
/******************************************/

#define CONV(instrname,filename) \
	{sprintf(Hilfs_string, "%s %s %s", "conv", instrname, filename); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_OBJ);}

#define CONVCP(instrname,filename,targetdir) \
	{sprintf(Hilfs_string, "%s %s %s %s", \
		 "conv", instrname, filename, targetdir); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_OBJ);}

#define CONVSWP(instrname,filename) \
	{sprintf(Hilfs_string, "%s %s %s", "convswp", instrname, filename); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_OBJ);}


/* Conversion of data sets from other vendors */
/**********************************************/

#define JCONV(jname,uxname,uxexp,uxdisk,uxuser) \
	{sprintf(Hilfs_string,"jconv %s \"%s\" %d \"%s\" \"%s\"",\
			jname,uxname,uxexp,uxdisk,uxuser);\
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_OBJ);}

#define VCONV(vname,uxname,uxexp,uxdisk,uxuser) \
	{sprintf(Hilfs_string,"vconv %s \"%s\" %d \"%s\" \"%s\"",\
			vname,uxname,uxexp,uxdisk,uxuser);\
	SETCURDATA AUERR=CPR_exec(Hilfs_string,WAIT_OBJ);}


/* JCAMP-DX commands tojdx and fromjdx */
/***************************************/

#define TOJDX(outname, outtype, compmode, title, origin, owner)	\
	{sprintf(Hilfs_string, "%s \"%s\" %d %d \"%s\" \"%s\" \"%s\"", \
		 "tojdx", outname, outtype, compmode, title, origin, owner); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string, WAIT_TERM);}

#define TOJDX5(outname, outtype, compmode, title, origin, owner) \
	{sprintf(Hilfs_string, "%s \"%s\" %d %d \"%s\" \"%s\" \"%s\"", \
		 "tojdx5", outname, outtype, compmode, title, origin, owner); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string, WAIT_TERM);}

#define FROMJDX(inputname)				\
	{sprintf(Hilfs_string, "%s \"%s\" %s", "fromjdx", inputname, "y n"); \
	SETCURDATA AUERR=CPR_exec(Hilfs_string, WAIT_TERM);}

/* Data set lists */
/******************/

#define IFEODATASETLIST if(namelist[xdataset+1][0] == '\0')
#define IFEOPARSETLIST if(parsetlist[xparset+1][0] == '\0')
#define IFEOPULPROGLIST if(pulproglist[xpulprog+1][0] == '\0')
#define IFEOLOOPCOUNTLIST if(loopcountlist[xloopcount+1] == 0)

#define SETDATASET	\
	{strcpy(name, namelist[xdataset]);	\
	strcpy(disk, dulist[xdataset]);		\
	strcpy(user, userlist[xdataset]);	\
	expno=expnolist[xdataset];		\
	procno=procnolist[xdataset];	}

#define IDATASETLIST	\
	{xdataset++;	\
	if(namelist[xdataset][0] == '\0') xdataset=0; /* end of list */ \
	SETDATASET}

#define DDATASETLIST	\
	{xdataset--;	\
	if(xdataset < 0) xdataset=0; \
	SETDATASET}

#define RDATASETLIST(x)	\
	{xdataset=x;	\
	if(namelist[xdataset][0] == '\0') xdataset=0; /* end of list */ \
	if(xdataset < 0) xdataset=0; \
	SETDATASET}


/* Pulse program lists */
/***********************/

#define SETPULPROG STOREPAR("PULPROG", pulproglist[xpulprog]);

#define IPULPROGLIST	\
	{xpulprog++;	\
	if(pulproglist[xpulprog][0] == '\0') xpulprog=0; \
	SETPULPROG}

#define DPULPROGLIST	\
	{xpulprog--;	\
	if(xpulprog < 0) xpulprog=0; \
	SETPULPROG}

#define RPULPROGLIST(x)	\
	{xpulprog=x;	\
	if(pulproglist[xpulprog][0] == '\0') xpulprog=0; \
	if(xpulprog < 0) xpulprog=0; \
	SETPULPROG}


/* Loop counter lists */
/**********************/

#define ILOOPCOUNTLIST	\
	{xloopcount++;	\
	if(loopcountlist[xloopcount] == 0) xloopcount=0; }

#define DLOOPCOUNTLIST	\
	{xloopcount--;	\
	if(xloopcount < 0) xloopcount=0; }

#define RLOOPCOUNTLIST(x)	\
	{xloopcount=x;	\
	if(loopcountlist[xloopcount] == 0) xloopcount=0; \
	if(xloopcount < 0) xloopcount=0; }


/* Delete commands */
/**************************/

#define DELETEPROCDATA(name1,expno1,procno1,disk1,user1)	\
	AUERR = RemoveData(curdat, 0, disk1, name1, expno1, procno1, 1);

#define DELETEIMAGINARYDATA(name1,expno1,procno1,disk1,user1)	\
	AUERR = RemoveData(curdat, 0, disk1, name1, expno1, procno1, 2);

#define DELETERAWDATA(name1,expno1,disk1,user1)	\
	AUERR = RemoveData(curdat, 0, disk1, name1, expno1, -1, 3);

#define DELETEPROCNO(name1,expno1,procno1,disk1,user1)	\
	AUERR = RemoveData(curdat, "DELETEPROCNO", disk1, name1, expno1, procno1, 0);

#define DELETEEXPNO(name1,expno1,disk1,user1)	\
	AUERR = RemoveData(curdat, "DELETEEXPNO", disk1, name1, expno1, -1, 0);

#define DELETENAME(name1,disk1,user1)	\
	AUERR = RemoveData(curdat, "DELETENAME", disk1, name1, -1, -1, 0);

/* Miscellaneous commands */
/**************************/

#define GETSTRING(prompt,var)			\
	{ const char* answer_from_Gets = Gets(prompt,var); \
	  AUERR = NORM_TERM;				\
	  if (answer_from_Gets == 0) Exit_unimar(-1);	\
	  else  strcpy(var,answer_from_Gets); }

#define GETINT(prompt,var)			\
	{ const char* answer_from_Gets;		\
	  sprintf(Hilfs_string,"%d",var);	\
	  answer_from_Gets = Gets(prompt,Hilfs_string); \
	  AUERR = NORM_TERM;				\
	  if (answer_from_Gets == 0) Exit_unimar(-1);	\
	  else	var=atoi(answer_from_Gets); }

#define GETFLOAT(prompt,var)			\
	{ const char* answer_from_Gets;		\
	  sprintf(Hilfs_string,"%G",var);	\
	  answer_from_Gets = Gets(prompt,Hilfs_string); \
	  AUERR = NORM_TERM;				\
	  if (answer_from_Gets == 0) Exit_unimar(-1);	\
	  else	var=(float)atof(answer_from_Gets); }

#define GETDOUBLE(prompt,var)			\
	{ const char* answer_from_Gets;		\
	  sprintf(Hilfs_string,"%18.10G",var);	\
	  answer_from_Gets = Gets(prompt,Hilfs_string); \
	  AUERR = NORM_TERM;				\
	  if (answer_from_Gets == 0) Exit_unimar(-1);	\
	  else	var=atof(answer_from_Gets); }

#define VIEWDATA \
	{SETCURDATA Change_object();}	/* show curdat on display */

#define	VIEWDATA_SAMEWIN \
	{ SETCURDATA						\
	  sprintf(Hilfs_string, "%s %s %s",			\
		  "sendgui", "_replace_data",			\
		  curd_path(0,disk,name,expno,procno,0));	\
	  CPR_exec(Hilfs_string, WAIT_TERM); }

#define CLOSE_DATASETWIN \
	{ SETCURDATA						\
	  sprintf(Hilfs_string, "%s %s %s",			\
		  "sendgui", "closeall",			\
		  curd_path(0,disk,name,expno,procno,0));	\
	  CPR_exec(Hilfs_string, WAIT_TERM); }

#define MKCURDATLIST(filename,argument) \
	{SETCURDATA mk_list_entry(curdat,filename,argument);}

#define TRUNCATE_FILE(filename)					\
	{int fd;						\
	 if ((fd = open(filename,O_RDWR | O_TRUNC)) != -1)	\
	    close(fd);	}

#define WAIT_UNTIL(hour,minute,day,month)		\
	AUERR=waituntil(hour,minute,day,month);

#define JAVACALL(x) gui_call_with_result(x, 0);
#define JAVAFREE    gui_call_free_result();
