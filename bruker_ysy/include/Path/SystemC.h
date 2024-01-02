/*
 *******************************************************************
 *
 * $Source: /bscl/CvsTree/bscl/gen/h/Path/SystemC.h,v $
 *
 * Copyright (c) 1995
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: SystemC.h,v 1.21 2020/01/16 09:13:42 christoph.kessler Exp $
 *
 *******************************************************************
 */

#ifndef PATH_SYSTEM_C_H
#define PATH_SYSTEM_C_H

#include <Path/DllMacro.h>	/* always the last include! */

#ifdef __cplusplus
extern "C" {
#endif


/** C wrapper to PathSystem::getCentralAuditLogFilePath().

 {\tt PathXWinNMRHints()} is the C wrapper function to
 {\tt PathXWinNMR::centralAuditLogFilePath()}.

 Please include either {\em Path/SystemC.h} or {\em lib/libPath.h}.
 @see PathSystem
 */
DLL_INTERFACE const char* PathCentralAuditLogFile (void);


/** C wrapper to PathSystem::getSpectStatusProxyLogFilePath().

 Please include either {\em Path/SystemC.h} or {\em lib/libPath.h}.
 @see PathSystem
 */
DLL_INTERFACE const char* PathSpectStatusProxyLogFilePath (void);

/** C wrapper to PathSystem::getSpectStatusProxyLogFileDir().

 Please include either {\em Path/SystemC.h} or {\em lib/libPath.h}.
 @see PathSystem
 */
DLL_INTERFACE const char* PathSpectStatusProxyLogFileDir (void);


/** C wrapper to PathSystem::getPdfViewerPath().

 {\tt PathSystemPdfViewer()} is the C wrapper function to
 {\tt PathSystem::getPdfviewerPath()}.

 It returns the complete path to a PDF viewer,
 e.g. {\em "/usr/local/bin/acroread"}. If no PDF viewer is installed,
 a {\tt NULL} pointer is returned.

 Please include either {\em Path/SystemC.h} or {\em lib/libPath.h}.
 @see PathSystem
 */
DLL_INTERFACE const char* PathSystemPdfViewer (void);

/** C wrapper to PathSystem::getDynamicsCenterInstPath().

 {\tt PathDynamicsCenter()} is the C wrapper function to
 {\tt PathSystem::getDynamicsCenterInstPath()}.

 It returns the complete path to Dynamics Center installation directory,
 e.g. {\em "/opt/Bruker/DynamicsCenter"}. If not installed,
 a {\tt NULL} pointer is returned.

 Please include either {\em Path/SystemC.h} or {\em lib/libPath.h}.
 @see PathSystem
 */
DLL_INTERFACE const char* PathDynamicsCenter (void);

/** C wrapper to PathSystem::getHomePath().

 {\tt PathSystemHome()} is the C wrapper function to
 {\tt PathSystem::getHomePath()}.

 It returns the complete path to the home directory
 of the current user, e.g. {\em "/usr/people/guest"}. If the home
 directory cannot be determined, an empty string is returned ({\em ""}).

 Please include either {\em Path/SystemC.h} or {\em lib/libPath.h}.
 @see PathSystem, PathPlatform
 */
DLL_INTERFACE const char* PathSystemHome (void);

/** C wrapper to PathSystem::getTempDir().

 {\tt PathSystemTemp()} is the C wrapper function to
 {\tt PathSystem::getTempDir()}.

 It returns the complete path to the temporary directory
 of the operating system, meaning
 \begin{itemize}
 \item on Unix system: {\em "/tmp"}
 \item on Windows-NT: the path specified by the environment variable
       {\tt TEMP}. If {\tt TEMP} is not set then {\tt PathSystemTemp()}
       returns {\em "C:/bruker/tmp"}.
 \end{itemize}

 Please include either {\em Path/SystemC.h} or {\em lib/libPath.h}.
 @see PathSystem
 */
DLL_INTERFACE const char* PathSystemTemp (void);

/** C wrapper to PathSystem::getInstType().

 {\tt PathSystemInstType()} is the C wrapper function to
 {\tt PathSystem::getInstType()}.

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

 Please include either {\em Path/SystemC.h} or {\em lib/libPath.h}.
 @see PathSystem, PathPlatform
 */
DLL_INTERFACE enum PathInstType PathSystemInstType(void);

#ifdef __cplusplus
}
#endif

#endif	/* PATH_SYSTEM_C_H */
