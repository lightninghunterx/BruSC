/*
***********************************************************************
 *
 * $Source: /bscl/CvsTree/bscl/gen/h/Path/Util.h,v $
 *
 * Copyright (c) 1997
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 * $Id: Util.h,v 1.13 2014/08/19 15:04:51 wem Exp $
 *
 ***********************************************************************
 */

#ifndef PATH_UTIL_H			/* prevent multiple includes */
#define PATH_UTIL_H

/*:=INFO=:**************************************************************
 *
 * Library : libPath.a
 * Header  : Path/Util.h
 *
 * Description : Utility functions
 *
 * Following functions and variables are exported :
 * - PathFixForUnix(): replace '\' with '/' in character string
 * - PathFixForWindosw(): replace '/' with '\' in character string
 * - PathIsAbsolute(): returns 0 if path is relative, != 0 if absolute
 *
 *::=info=:*************************************************************/

/*** necessary includes ***/

/*** definitions and types ***/

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif
#if defined(DLL_LINKAGE) && !defined(CPROTO)
#   ifdef libPath_INTERNAL
#      define DLL_INTERFACE __declspec(dllexport)
#   else
#      define DLL_INTERFACE __declspec(dllimport)
#   endif
#else
#   define DLL_INTERFACE
#endif

/*** variables ***/

/*** function prototypes ***/

#ifdef __cplusplus
extern "C" {
#endif

/** replaces '\' with '/' in character string.

 {\tt PathFixForUnix()} replaces all '\' characters in {\tt path}
 by '/' characters, so making the path compatible for unix or
 NutCracker.
 @author \URL[Andreas Huennebeck]{mailto:ah@bruker.de}
 */
DLL_INTERFACE void PathFixForUnix (char *path);

/** replaces '/' with '\' in character string.

 {\tt PathFixForWindows()} replaces all '/' characters in {\tt path}
 by '\' characters, so making the path compatible for unix or
 NutCracker.
 @author \URL[Andreas Huennebeck]{mailto:ah@bruker.de}
 */
DLL_INTERFACE void PathFixForWindows (char *path);

/** removes mask characters ('"') in character string.
 {\tt PathDelMaskChars()} removes all '"' characters in {\tt path}.
 This is necessary on Windows where the mask characters are stored in
 environment strings. This happens if an environment string is set in
 the command prompt and the string completion is used.
 @author \URL[Andreas Huennebeck]{mailto:ah@bruker.de}
 */
DLL_INTERFACE void PathDelMaskChars(char* path);

/** returns 0 if relative path, != 0 if absolute path.

 {\tt PathIsAbsolute()} returns {\tt 0} if {\tt path} is a relative path,
 and {\tt != 0} if {\tt path} is an absolute path.
 @author \URL[Andreas Huennebeck]{mailto:ah@bruker.de}
 */
DLL_INTERFACE int PathIsAbsolute (const char* path);

/** read key from ini file.

   {\tt PathReadIniFileEntry} reads an entry of a ini file. It
   returns the size of the entry if the entry is found, and a {\tt 0}
   otherwise. It also returns quietly a {\tt 0} in case of errors:
   \begin{itemize}
   \item the file does not exist
   \item the file cannot be opened
   \item bad arguments
   \end{itemize}
   Please note that leading and following white spaces of entries are removed.
   @param key		the key
   @param item		the item of the key
   @param defaultstring	the default value if neither key nor item are found
   @param entry		to store the entry
   @param entrySize	size of the entry
   @param filename	the path of the ini file
   @author \URL[Andreas Huennebeck]{mailto:ah@bruker.de}
 */
DLL_INTERFACE int PathReadIniFileEntry
(
    const char *key,
    const char *item,
    const char *defaultstring,
    char *entry,
    int entrySize,
    const char *filename
);

/** read key from Windows Registry.

   {\tt PathReadWindowsRegistry} reads an entry from the Windows Registry.
   It returns the size of the entry if the entry is found, and a {\tt 0}
   otherwise. It also returns quietly a {\tt 0} in case of errors:
   \begin{itemize}
   \item the registry does not exist
   \item bad arguments
   \end{itemize}
   Please note that leading and following white spaces of entries are
   {\bf not} removed.
   @param regpath	the path into the registry
   @param item		the item of the key
   @param defaultstring	the default value if neither key nor item are found
   @param entry		to store the entry
   @param entrySize	size of the entry
   @author \URL[Andreas Huennebeck]{mailto:ah@bruker.de}
 */
DLL_INTERFACE int PathReadWindowsRegistry
(
    const char *regpath,
    const char *item,
    const char *defaultstring,
    char *entry,
    int entrySize
);

#ifdef __cplusplus
}
#endif

#endif /* PATH_UTIL_H */
/***********************************************************************/
/*			     E N D   O F   F I L E                     */
/***********************************************************************/
