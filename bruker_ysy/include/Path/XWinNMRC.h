/*
 *******************************************************************
 *
 * $Source: /bscl/CvsTree/bscl/gen/h/Path/XWinNMRC.h,v $
 *
 * Copyright (c) 1996
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 * $Id: XWinNMRC.h,v 1.27 2019/05/15 14:58:11 ah Exp $
 *
 *******************************************************************
 */

#ifndef PATH_X_WIN_NMR_C_H
#define PATH_X_WIN_NMR_C_H

#define LOCAL_PATHPLATFORM_USAGE_ALLOWED
#include <Path/Platform.h>
#undef LOCAL_PATHPLATFORM_USAGE_ALLOWED
#include <Path/DllMacro.h>	/* always the last include! */

#ifdef __cplusplus
extern "C" {
#endif

/** C wrapper to PathXWinNMR::getInstPath().

 {\tt PathXWinNMRInst()} is the C wrapper function to
 {\tt PathXWinNMR::getInstPath()}.

 It returns the path to the XWIN-NMR installation
 directory, as it is set with the environment variable
 {\tt XWINNMRHOME}, e.g. {\em "/u"}. If {\tt XWINNMRHOME} is not
 set then {\em "/undefined"} is returned.

 {\tt PathXWinNMR::getInstPath()} calls {\tt PathPlatform::getXwinHome()}
 to do its job.

 Please include either {\em Path/XWinNMRC.h} or {\em lib/libPath.h}.
 @see PathXWinNMR, PathPlatform
 */
DLL_INTERFACE const char* PathXWinNMRInst (void);


/** C wrapper to PathXWinNMR::getProgPath().

 {\tt PathXWinNMRProg()} is the C wrapper function to
 {\tt PathXWinNMR::getProgPath()}.

 It returns the path to the XWIN-NMR program
 directory (by default depending on the environment variable
 {\tt XWINNMRHOME}), e.g. {\em "/u/prog"}.

 This default setting can be changed by setting the environment
 variable {\tt XWINNMRPROG}. If neither {\tt XWINNMRHOME} nor
 {\tt XWINNMRPROG} are set then {\em "/undefined/prog"} is returned.

 {\tt PathXWinNMR::getProgPath()} calls {\tt PathPlatform::getXwinProg()}
 to do its job.

 Please include either {\em Path/XWinNMRC.h} or {\em lib/libPath.h}.
 @see PathXWinNMR, PathPlatform
 */
DLL_INTERFACE const char* PathXWinNMRProg (void);


/** C wrapper to PathXWinNMR::getAppPath().

 {\tt PathXWinNMRApp()} is the C wrapper function to
 {\tt PathXWinNMR::getAppPath()}.

 It returns the path to the XWIN-NMR application
 defaults directory (depending on the environment variable
 {\tt XWINNMRHOME}), e.g. {\em "/u/prog/app-defaults"}.

 If {\tt XWINNMRHOME} is not set then
 {\em "/undefined/prog/app-defaults"} is returned.

 Please include either {\em Path/XWinNMRC.h} or {\em lib/libPath.h}.
 @see PathXWinNMR
 */
DLL_INTERFACE const char* PathXWinNMRApp (void);


/** C wrapper to PathXWinNMR::getHintsPath().

 {\tt PathXWinNMRHints()} is the C wrapper function to
 {\tt PathXWinNMR::getHintsPath()}.

 It returns the path to the XWIN-NMR hints
 directory (depending on the environment variable
 {\tt XWINNMRHOME}), e.g. {\em "/u/prog/hints"}.

 If {\tt XWINNMRHOME} is not set then
 {\em "/undefined/prog/hints"} is returned.

 Please include either {\em Path/XWinNMRC.h} or {\em lib/libPath.h}.
 @see PathXWinNMR
 */
DLL_INTERFACE const char* PathXWinNMRHints (void);


/** C wrapper to PathXWinNMR::getExpPath().

 {\tt PathXWinNMRExp()} is the C wrapper function to
 {\tt PathXWinNMR::getExpPath()}.

 It returns the path to the XWIN-NMR experiment
 directory (by default depending on the environment variable
 {\tt XWINNMRHOME}), e.g. {\em "/u/exp"}.

 This default setting can be changed by setting the environment
 variable {\tt XWINNMREXP}. If neither {\tt XWINNMRHOME} nor
 {\tt XWINNMREXP} are set then {\em "/undefined/exp"} is returned.

 Please include either {\em Path/XWinNMRC.h} or {\em lib/libPath.h}.
 @see PathXWinNMR
 */
DLL_INTERFACE const char* PathXWinNMRExp (void);


/** C wrapper to PathXWinNMR::getExpStanPath().

 {\tt PathXWinNMRExpStan()} is the C wrapper function to
 {\tt PathXWinNMR::getExpStanPath()}.

 It returns the path to the XWIN-NMR experiment
 standard directory (by default depending on the environment variable
 {\tt XWINNMRHOME}), e.g. {\em "/u/exp/stan"}.

 This default setting can be changed by setting the environment
 variable {\tt UXNMR_STAN}. If neither {\tt XWINNMRHOME} nor
 {\tt UXNMR_STAN} are set then {\em "/undefined/exp/stan"} is returned.

 Please include either {\em Path/XWinNMRC.h} or {\em lib/libPath.h}.
 @see PathXWinNMR
 */
DLL_INTERFACE const char* PathXWinNMRExpStan (void);


/** C wrapper to PathXWinNMR::getConfPath().

 {\tt PathXWinNMRConf()} is the C wrapper function to
 {\tt PathXWinNMR::getConfPath()}.

 It returns the path to the XWIN-NMR configuration
 directory (by default depending on the environment variable
 {\tt XWINNMRHOME}), e.g. {\em "/u/conf"}.

 This default setting can be changed by setting the environment
 variable {\tt XWINNMRCONF}. If neither {\tt XWINNMRHOME} nor
 {\tt XWINNMRCONF} are set then {\em "/undefined/conf"} is returned.

 Please include either {\em Path/XWinNMRC.h} or {\em lib/libPath.h}.
 @see PathXWinNMR
 */
DLL_INTERFACE const char* PathXWinNMRConf (void);


/** C wrapper to PathXWinNMR::getPlotPath().

 {\tt PathXWinNMRPlot()} is the C wrapper function to
 {\tt PathXWinNMR::getPlotPath()}.

 It returns the path to the XWIN-NMR plot
 directory (by default depending on the environment variable
 {\tt XWINNMRHOME}), e.g. {\em "/u/plot"}.

 This default setting can be changed by setting the environment
 variable {\tt XWINNMRPLOT}. If neither {\tt XWINNMRHOME} nor
 {\tt XWINNMRPLOT} are set then {\em "/undefined/plot"} is returned.

 Please include either {\em Path/XWinNMRC.h} or {\em lib/libPath.h}.
 @see PathXWinNMR
 */
DLL_INTERFACE const char* PathXWinNMRPlot (void);


/** C wrapper to PathXWinNMR::getLayoutPath().

 {\tt PathXWinNMRLayout()} is the C wrapper function to
 {\tt PathXWinNMR::getLayoutPath()}.

 It returns the path to the XWIN-NMR plot layout
 directory (by default depending on the environment variable
 {\tt XWINNMRHOME}), e.g. {\em "/u/plot/layouts"}.

 This default setting can be changed by setting the environment
 variable {\tt XWINNMRLAYOUT}. If neither {\tt XWINNMRHOME} nor
 {\tt XWINNMRLAYOUT} are set then {\em "/undefined/plot/layouts"}
 is returned.

 Please include either {\em Path/XWinNMRC.h} or {\em lib/libPath.h}.
 @see PathXWinNMR
 */
DLL_INTERFACE const char* PathXWinNMRLayout (void);


/** C wrapper to PathXWinNMR::getPrintPath().

 {\tt PathXWinNMRPrint()} is the C wrapper function to
 {\tt PathXWinNMR::getPrintPath()}.

 It returns the path to the XWIN-NMR print
 directory (by default depending on the environment variable
 {\tt XWINNMRHOME}), e.g. {\em "/u/print"}.

 This default setting can be changed by setting the environment
 variable {\tt XWINNMRPRINT}. If neither {\tt XWINNMRHOME} nor
 {\tt XWINNMRPRINT} are set then {\em "/undefined/print"} is returned.

 Please include either {\em Path/XWinNMRC.h} or {\em lib/libPath.h}.
 @see PathXWinNMR
 */
DLL_INTERFACE const char* PathXWinNMRPrint (void);


/** C wrapper to PathXWinNMR::getDotXWinNMRPath().

 {\tt PathXWinNMRDotXWinNMR()} is the C wrapper function to
 {\tt PathXWinNMR::getDotXWinNMRPath()}.

 It returns the path to the file
 {\em .xwin-nmr-<host>} in the home directory of the current user,
 e.g. {\em "/usr/people/guest/.xwinnmr-dmx300"}.

 If anything fails, then {\tt getDotXWinNMRPath()} returns
 {\em ".xwinnmr"}.

 {\tt PathXWinNMR::getDotXWinNMRPath()} calls
 {\tt PathPlatform::getUserHome()} to do its job.

 Please include either {\em Path/XWinNMRC.h} or {\em lib/libPath.h}.
 @see PathXWinNMR, PathPlatform
 */
DLL_INTERFACE const char* PathXWinNMRDotXWinNMR (void);

/**
 * get path to blade data directory
 */
DLL_INTERFACE const char* PathXWinNMRBladeDataPath(void);


/** C wrapper to PathXWinNMR::getCurDirPath().

 {\tt PathXWinNMRCurDir()} is the C wrapper function to
 {\tt PathXWinNMR::getCurDirPath()}.

 It returns a part of the path to the
 XWIN-NMR curdir directory of the current user,
 e.g. {\em "curdir/guest"}. If anything fails, then
 {\tt PathXWinNMRCurDir()} returns an empty string ({\em ""}).

 Please include either {\em Path/XWinNMRC.h} or {\em lib/libPath.h}.

 To get the full path you must concat {\tt PathXWinNMRProg()} and
 {\tt PathXWinNMRCurDir()}, see the following example:
 \begin{verbatim}
 char *curdir = malloc(strlen(PathXWinNMRProg()) +
                       strlen(PathXWinNMRCurDir() + 2);
 sprintf(curdir, "%s/%s", PathXWinNMRProg(),
                          PathXWinNMRCurDir());
 \end{verbatim}

 {\tt PathXWinNMR::getCurDirPath()} calls {\tt PathPlatform::getUserName()}
 to do its job.
 @see PathXWinNMR, PathPlatform
 */
DLL_INTERFACE const char* PathXWinNMRCurDir (void);


/** C wrapper to PathXWinNMR::getRemLockPath().

 {\tt PathXWinNMRRemLock()} is the C wrapper function to
 {\tt PathXWinNMR::getRemLockPath()}.

 It returns the complete path to the remote lock
 directory (located within the diskless distribution for the CCU unix
 operating system), meaning
 \begin{itemize}
 \item on {\bf Unix}: {\em "/usr/diskless/clients/spect/lock"}
 \item on {\bf Windows-NT}: the path as specified by Windows-NT registry
       entry, completed by {\em "/clients/spect/lock"}. If there is no
       entry in the registry then {\tt PathXWinNMRRemLock()} returns
       {\em "C:/bruker/diskless/clients/spect/lock"}.
 \end{itemize}

 {\tt PathXWinNMR::getRemLockPath()} calls
 {\tt PathPlatform::getDisklessLock()} to do its job.

 Please include either {\em Path/XWinNMRC.h} or {\em lib/libPath.h}.
 @see PathXWinNMR, PathPlatform
 */
DLL_INTERFACE const char* PathXWinNMRRemLock (void);


/** C wrapper to PathXWinNMR::getInstType().

 {\tt PathXWinNMRInstType()} is the C wrapper function to
 {\tt PathXWinNMR::getInstType()}.

 It returns the installation/program type that the Path library has
 been initialized to.  Note that it is
 initialized in XWinNMR.c and/or System.c, but it is based on compiler
 flags, so it is not really controlled at run time.

 The 'insttype' can then be used to control the format of the various
 paths used in the Path library.
 \begin{itemize}
 \item PathInstTypeNmr: for NMR using Xwin-NMR or TopSpin
 \item PathInstTypeFtms: for FTMS using ApexControl
 \end{itemize}

 Please include either {\em Path/XWinNMRC.h} or {\em lib/libPath.h}.
 @see PathXWinNMR, PathPlatform
 */
DLL_INTERFACE enum PathInstType PathXWinNMRInstType(void);


#ifdef __cplusplus
}
#endif

#endif /* PATH_X_WIN_NMR_C_H */
