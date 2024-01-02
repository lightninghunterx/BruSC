/*
 *******************************************************************
 *
 * $Source: /bscl/CvsTree/bscl/gen/h/Path/DataC.h,v $
 *
 * Copyright (c) 2002
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 * $Id: DataC.h,v 1.7 2007/11/30 09:11:53 ah Exp $
 *
 *******************************************************************
 */

#ifndef PATH_DATA_C_H
#define PATH_DATA_C_H

/*:=INFO=:**************************************************************
 *
 * Header : Path/DataC.h
 *
 * Description: Declaration of data path functions (in C)
 *
 *::=info=:*************************************************************/

/*** necessary includes ***/

#include <Path/DllMacro.h>

/*** forward declarations ***/

/*** definitions ***/

/*** types and enums ***/

/** error codes.

    These are the error codes which are returned from the C-functions
    and also from the PathData::init() member function. All error codes
    are negative values.
 */
enum PathDataErrCode 
{
    /** no error. */
    PathDataErrCodeOK = 0,

    /** argument is NULL pointer */
    PathDataErrCodeNullPtrArg = -1,

    /** bad argument */
    PathDataErrCodeBadArg = -2,

    /** argument is out of range */
    PathDataErrCodeOutOfRange = -3,

    /** error in sprintf */
    PathDataErrCodeSprintf = -4,

    /** buffer is too small */
    PathDataErrCodeBufTooSmall = -5,

    /** DU is empty. */
    PathDataErrCodeDuEmpty = -6,

    /** USER is empty. */
    PathDataErrCodeUserEmpty = -7,

    /** TYPE is empty. */
    PathDataErrCodeTypeEmpty = -8,

    /** NAME is empty. */
    PathDataErrCodeNameEmpty = -9,

    /** EXPNO is out of range. */
    PathDataErrCodeExpnoOutOfRange = -10,

    /** PROCNO is out of range. */
    PathDataErrCodeProcnoOutOfRange = -11,

    /** not yet implemented */
    PathDataErrCodeNotYetImplemented = -19,

    /** other unspecified error */
    PathDataErrCodeOther = -20
};

/*** variables ***/

/*** function prototypes ***/

#ifdef __cplusplus
extern "C" {
#endif

/** return path to acquisition directory of data set.

 {\tt PathDataGetAcquDir()} builds the path to the acquisition
 directory of the desired data set. The path is built using two different
 methods depending on {\tt datpath}:

 \begin{description}
 \item[datpath is empty or NULL] the path is created as 
       {\em {\tt du}/data/{\tt user}/nmr/{\tt name}/{\tt expno}}.
 \item[datpath is not empty] the path is created as 
       {\em {\tt datpath}/{\tt name}/{\tt expno}}.
 \end{description}

 Please include either {\em Path/DataC.h} or {\em lib/libPath.h}.

 \param path     output: pointer to buffer where the path is built
 \param pathSize input: size of buffer for path (minimum is PATH_MAX)
 \param datpath  input: pointer to character string containing the data path (DATPATH)
 \param du       input: pointer to character string containing the disk unit (DU)
 \param user     input: pointer to character string containing the user (USER)
 \param type     input: pointer to character string containing the type (TYPE)
 \param name     input: pointer to character string containing the data set name (NAME)
 \param expno    input: the experiment number (EXPNO)
 \return         number of characters written into {\tt path}.
 \return         a negative value equal to an error code of type
                 {\tt PathDataErrCode}, in case of error. Use {\tt PathDataGetErrMsg()}
		 to retrieve the corresponding error message.
 \see: PathDataErrCode, PathDataGetErrMsg
 */
DLL_INTERFACE int PathDataGetAcquDir(char path[], unsigned int pathSize,
				     const char* datpath, const char* du, const char* user,
				     const char* type, const char* name, int expno);


/** return path to processing directory of data set.

 {\tt PathDataGetProcDir()} builds the path to the processing
 directory of the desired data set. The path is built using two different
 methods depending on {\tt datpath}:

 \begin{description}
 \item[datpath is empty or NULL] the path is created as 
       {\em {\tt du}/data/{\tt user}/nmr/{\tt name}/{\tt expno}/pdata/{\tt procno}}.
 \item[datpath is not empty] the path is created as 
       {\em {\tt datpath}/{\tt name}/{\tt expno}/pdata/{\tt procno}}.
 \end{description}

 Please include either {\em Path/DataC.h} or {\em lib/libPath.h}.

 \param path     output: pointer to buffer where the path is built
 \param pathSize input: size of buffer for path (minimum is PATH_MAX)
 \param datpath  input: pointer to character string containing the data path (DATPATH)
 \param du       input: pointer to character string containing the disk unit (DU)
 \param user     input: pointer to character string containing the user (USER)
 \param type     input: pointer to character string containing the type (TYPE)
 \param name     input: pointer to character string containing the data set name (NAME)
 \param expno    input: the experiment number (EXPNO)
 \param procno   input: the processing number (PROCNO)
 \return         number of characters written into {\tt path}.
 \return         a negative value equal to an error code of type
                 {\tt PathDataErrCode}, in case of error. Use {\tt PathDataGetErrMsg()}
		 to retrieve the corresponding error message.
 \see: PathDataErrCode, PathDataGetErrMsg
 */
DLL_INTERFACE int PathDataGetProcDir(char path[], unsigned int pathSize,
				     const char* datpath, const char* du, const char* user,
				     const char* type, const char* name, int expno, int procno);


/** cut path into separate items.

 {\tt PathDataGetItems()} cuts a path to a data set directory into
 the sparate items DATPATH, DU, USER, TYPE, NAME, EXPNO and PROCNO. It handles paths
 in old and new style:

 \begin{description}
 \item[old style path:] {\em {\tt du}/data/{\tt user}/{\tt type}/{\tt name}/{\tt expno}/pdata/{\tt procno}}
                        (without DATPATH)
 \item[new style path:] {\em {\tt datpath}/{\tt name}/{\tt expno}/pdata/{\tt procno}} (with DATPATH)
 \end{description}

 The current implementation handles 'path' as an old style path if the components 
 {\em type}, {\em user} and {\em data} are found. In all other cases the new
 style is handled which means that 'datpath' is set and 'du', 'user' and
'type' are left empty.
   
 {\tt PathDataGetItems()} stores the items in the appropriate character
 buffers if the pointer to the corresponding buffer is not NULL and if the
 size of this buffer is big enough. If the path contains just an acquisition
 directory (that is a path without {\em pdata/}{\tt procno}) then an error
 is returned only if the {\tt procno} pointer is not NULL. 

 In case of an error an error code is 
 returned, and the corresponding error message can be retrieved using
 {\tt PathDataGetErrMsg()}.

 Please include either {\em Path/DataC.h} or {\em lib/libPath.h}.

 \see PathDataGetErrMsg
 \param path     pointer to buffer containing the path
 \param datpath  pointer to character buffer to store data path (DATPATH)
 \param datpath  size of character buffer for DATPATH
 \param du       pointer to character buffer to store disk unit (DU)
 \param duSize   size of character buffer for DU
 \param user     pointer to character buffer containing the user (USER)
 \param userSize size of character buffer for USER
 \param type     pointer to character buffer containing the type (TYPE)
 \param typeSize size of character buffer for TYPE
 \param name     pointer to character buffer containing the data set name (NAME)
 \param nameSize size of character buffer for NAME
 \param expno    pointer to integer to store the experiment number (EXPNO)
 \param procno   pointer to integer to store the processing number (PROCNO)
 \return         0 in case of success, error code in case of failure.
 */
DLL_INTERFACE enum PathDataErrCode PathDataGetItems(
    const char* path, 
    char* datpath, unsigned int datpathSize,
    char* du, unsigned int duSize,
    char *user, unsigned int userSize, 
    char *type, unsigned int typeSize, 
    char* name, unsigned int nameSize,
    int* expno,
    int* procno);


/** return error message.

 {\tt PathDataGetErrMsg()} returns the error message which corresponds
 to the specified {\tt error} code.

 Please include either {\em Path/DataC.h} or {\em lib/libPath.h}.

 \param error    error code
 \return         pointer to character buffer containing the error message
 */
DLL_INTERFACE const char* PathDataGetErrMsg(int error);

#ifdef __cplusplus
}
#endif

#endif /* PATH_DATA_C_H */
/***********************************************************************/
/*			     E N D   O F   F I L E                     */
/***********************************************************************/
