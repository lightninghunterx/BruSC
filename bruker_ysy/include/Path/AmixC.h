/*
 *******************************************************************
 *
 * $Source: /bscl/CvsTree/bscl/gen/h/Path/AmixC.h,v $
 *
 * Copyright (c) 1996
 * BRUKER ANALYTIK GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 * $Id: AmixC.h,v 1.4 2018/07/17 15:38:35 ah Exp $
 *
 *******************************************************************
 */

#ifndef PATH_AMIX_C_H
#define PATH_AMIX_C_H

#include <Path/DllMacro.h>

#ifdef __cplusplus
extern "C" {
#endif

/** C wrapper to PathAmix::getProgPath().

 {\tt PathAmixProg()} is the C wrapper function to 
 {\tt PathAmix::getProgPath()}. 

 It returns the path to the AMIX program directory.

 {\tt PathAmix::getProgPath()} calls {\tt PathPlatform::getAmixProg()}
 to do its job.

 Please include either {\em Path/AmixC.h} or {\em lib/libPath.h}.

 @author \URL[Andreas H�nnebeck]{mailto:ah@bruker.de}
 @see PathAmix, PathPlatform
 */
DLL_INTERFACE const char* PathAmixProg (void);

/** C wrapper to PathAmix::getProgName().

 {\tt PathAmixName()} is the C wrapper function to 
 {\tt PathAmix::getProgName()}. 

 It returns the name of the AMIX program.

 {\tt PathAmix::getProgName()} calls {\tt PathPlatform::getAmixName()}
 to do its job.

 Please include either {\em Path/AmixC.h} or {\em lib/libPath.h}.

 @author \URL[Andreas H�nnebeck]{mailto:ah@bruker.de}
 @see PathAmix, PathPlatform
 */
DLL_INTERFACE const char* PathAmixName(void);

/** C wrapper to PathAmix::getWinName().

 {\tt PathAmixWinName()} is the C wrapper function to 
 {\tt PathAmix::getWinName()}. 

 It returns the windows name of the AMIX program.

 {\tt PathAmix::getWinName()} calls {\tt PathPlatform::getAmixWin()}
 to do its job.

 Please include either {\em Path/AmixC.h} or {\em lib/libPath.h}.

 @author \URL[Andreas H�nnebeck]{mailto:ah@bruker.de}
 @see PathAmix, PathPlatform
 */
DLL_INTERFACE const char* PathAmixWinName(void);


#ifdef __cplusplus
}
#endif

#endif /* PATH_X_WIN_NMR_C_H */




