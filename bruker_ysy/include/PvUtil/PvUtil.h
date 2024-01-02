/****************************************************************
 *
 * Copyright (c) 2000 - 2019
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 ****************************************************************/

#ifndef PVUTIL_PVUTIL_H
#define PVUTIL_PVUTIL_H

#ifndef CPROTO

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctime>
#include <cstdint>
#include <string>
#include <vector>
#include "modality.h"
#include "PvUtil/TimeDefs.h"
#include "PvUtil/DataPathType.h"
#include "PvUtil/DLLMacro.h"

/** 
 * @file
 * @addtogroup PvUtil
 * @{
 */

/* gen/src/lib/libPvUtil/Copy.c */

/** Copy a directory with all contents and subdirectories.
    The source directory must exist. If it is a symbolic
    link the link will be followed as all symbolic
    links which are subsequently encountered. 
    The content and all subdirectories of the source directory 
    will be copied to the destination directory. If the 
    destination directory already exists and if it
    is a directory or a symbolic link to a directory, it
    will be removed before performing the copy operation.
    If the parent directory of the destination directory doesn't
    yet exist, the copy will fail.
    @param dest Path of destination directory.
    @param src  Path of source directory.
    @throws PvException On error this function throws a C-exception.
 */
DLL_INTERFACE void CopyDirectory(const char * dest, const char * src);

/** Copy a directory with all contents and subdirectories.
    This function does the same as CopyDirectory(). The only difference
    it the error handling. It does not throw exceptions.
    @param dest Destination directory.
    @param src  Source directory.
    @return Yes if copy was successful else No.
 */
DLL_INTERFACE bool CopyDirectoryNE(const char * dest, const char * src);

/** Copy a file and fill data 
    A source file is copied to a destination file. If both files are the
    same (same @c inode) the files are not copies. The destination file must 
    not be a directory. If the destination file exists it is overwritten.
    @param dest Path of destination file.
    @param src  Path of source file.
    @param blocksize size of single block to be filled to 1K multiples: 
        size of src must be a multiple, otherwise an exception is raised. 
        As well if the size of a single block is larger than the internal data
        buffer, which currently holds 256K.
    @throws PvException On error (\a dest is a directory, \a dest is not 
        writable, etc.).
 */

DLL_INTERFACE void PvUtilCopyFileToKData(const char*  dest, const char* src, const unsigned int blocksize);
/** Copy a file.
    A source file is copied to a destination file. If both files are the
    same (same @c inode) the files are not copies. The destination file must 
    not be a directory. If the destination file exists it is overwritten.
    @param dest Path of destination file.
    @param src  Path of source file.
    @throws PvException On error (\a dest is a directory, \a dest is not 
        writable, etc.).
 */
DLL_INTERFACE void PvUtilCopyFile(const char * dest, const char * src);

/** Copy a file.
    This function does the same as PvUtilCopyFile(). The only difference
    it the error handling. It does not throw exceptions.
    @param dest Path of destination file.
    @param src  Path of source file.
    @return Yes if copy was successful else No.
 */
DLL_INTERFACE bool PvUtilCopyFileNE(const char * dest, const char * src);

/* gen/src/lib/libPvUtil/Fpe.c */

/** Enable divide by zero floating point exception. 
    The system will signal an SIGFPE in case of a division by zero. 
    If floating point exceptions are not switched on this 
    operation may succeed and the result value may be  
    <code>INF</code>, <code>-INF</code>, etc. On Windows systems this 
    routine is a no-op.
 */
DLL_INTERFACE void PvUtilEnableDivideByZeroFpe();

/** Disable invalid operations floating point exceptions.
    @see PvUtilEnableDivideByZeroFpe.
 */
DLL_INTERFACE void PvUtilDisableDivideByZeroFpe();

/** Enable invalid operation floating point exceptions.
    @see PvUtilDisableInvalidOperationFpe.
 */
DLL_INTERFACE void PvUtilEnableInvalidOperationFpe();

/** Disable invalid operation floating point exceptions.
    @see PvUtilEnableInvalidOperationFpe.
 */
DLL_INTERFACE void PvUtilDisableInvalidOperationFpe();

/** Enable the underflow and overflow floating point exception. 
    CAUTIOUS: Underflow and overflow exceptions do also appear if
    a value is outside of the double range. But this may be valid since
    most machines are able to work with such values.
 */
DLL_INTERFACE void PvUtilEnableUnderOverflowFpe();


/** Disable the underflow and overflow floating point exception. */
DLL_INTERFACE void PvUtilDisableUnderOverflowFpe();


/* gen/src/lib/libPvUtil/System.c */

/** 
 * Returns current OS system login user. 
 * Cannot be used outside of main scope.
 * @return The current login user name.
 */
DLL_INTERFACE const char *PvGetUser();

/** 
 * Returns <code>true</code> if current user is NMR-superuser. 
 * Cannot be used outside or main scope.
 * @return Indicates if the current user is the NMR Superuser.
 */
DLL_INTERFACE bool PvIsNmrSuperUser();

/** 
 * Returns file / directory \c umask. 
 * Cannot be used outside of main scope.
 * @return The current umask value.
 */
DLL_INTERFACE mode_t PvGetUmask();

/** 
 * Returns the current process path name. 
 * Cannot be used outside of main scope.
 * @return The current process name.
 */
DLL_INTERFACE const char * PvGetProcessName();

/// Returns the current process id.
DLL_INTERFACE int PvGetProcessId();

/// Return the name of the current computer.
DLL_INTERFACE const char * PvGetComputerName();


/* gen/src/lib/libPvUtil/DirUtil.c */

/** Read entries from a directory and process them.
    This function reads all entries of a given directory.
    For each directory entry found it does:
    -# The test function (if it is provided) is called.
       If it returns false the next directory entry will be read.
    -# If no test function was provided or if it returns
       true and if a processing function is provided the 
       processing function will be called. Then the
       next directory entry will be read.
    
    The test and processing function will be called for the 
    directory entries with a parameter containing \a dir_path/\<entry\>.
    The test and processing function can report errors using 
    \c PvException. This function is not C++ exception safe.
    
    @param dir_path Directory to read entries from.
    @param test_fcn Test function for directory entries.
    @param proc_fcn Processing function for directory entries.
    @throws PvExceptiom On error a C-exception is thrown.
        
 */
DLL_INTERFACE void ReadDirEntries(const char * dir_path,
    bool (* test_fcn)(const char *),
    void (* proc_fcn)(const char *));


/** Read entries from a directory and process them.
    This function does the same as ReadDirEntries().
    The difference is that the caller can supply data in an opaque pointer 
    which will be given unchanged as second parameter to the test and 
    processing function.
    @param dir_path    Directory to read entries from.
    @param test_fcn    Test function for directory entries.
    @param proc_fcn    Processing function for directory entries.
    @param client_data Caller supplied data pointer for test and 
                       processing function.
    @throws PvException On error a C-exception is thrown.
 */
DLL_INTERFACE void ReadDirEntriesForClient(
    const char * dir_path,
    bool (* test_fcn)(const char *, void *),
    void (* proc_fcn)(const char *, void *),
    void * client_data);

/** Read entries from a directory and process them.
    This function does the same as ReadDirEntries().
    The difference is that the test and processing function
    will be called with two parameters: the first parameter is
    \a dir_path the second parameter is the name of the directory
    entry (with out directory names).
    @param dir_path Directory to read entries from.
    @param test_fcn Test function for directory entries.
    @param proc_fcn Processing function for directory entries.
    @throws PvException On error a C-exception is thrown.
 */
DLL_INTERFACE void ReadDirNames(
    const char * dir_path,
    bool (* test_fcn)(const char *, const char *),
    void (* proc_fcn)(const char *, const char *));

/** Read entries from a directory and process them.
    This function does the same as ReadDirNames().
    The difference is that the caller can supply data in an opaque pointer 
    which will be given unchanged as third parameter to the test and 
    processing function.
    @param dir_path    Directory to read entries from.
    @param test_fcn    Test function for directory entries.
    @param proc_fcn    Processing function for directory entries.
    @param client_data Caller supplied data pointer for test and 
                       processing function.
    @throws PvException On error a C-exception is thrown.

 */
DLL_INTERFACE void ReadDirNamesForClient(
    const char * dir_path,
    bool (* test_fcn)(const char *, const char *, void *),
    void (* proc_fcn)(const char *, const char *, void *),
    void * client_data);


/** Read entries from a directory, sort and process them.
    The selection and processing functions are
    called with two parameters: the first parameter is
    a directory path the second parameter is the name of the directory
    entry (with out directory names).
    @param dir_path Directory to read entries from.
    @param test_fcn Selection function for directory entries.
    @param sort_fcn Sort function as described in ScanDirEntries().
    @param proc_fcn Processing function for directory entries.
    @return The number of entries that were processed, 0 on error.
    @throws PvException On error a C-exception is thrown.
 */
DLL_INTERFACE int ScanDirNames(
    const char * dir_path,
    bool (* test_fcn)(const char *, const char *),
    int (* sort_fcn)(const void *, const void *),
    void (* proc_fcn)(const char *, const char *));

/** Read entries from a directory, sort and process them.
    This function does the same as ScanDirNames().
    The difference is that the caller can supply data in an opaque pointer 
    which will be given unchanged as third parameter to the selection and 
    processing function.
    @param dir_path    Directory to read entries from.
    @param test_fcn    Selection function for directory entries.
    @param sort_fcn    Sort function as described in ScanDirEntries().
    @param proc_fcn    Processing function for directory entries.
    @param client_data Caller supplied data pointer for test and 
                       processing function.
    @return The number of entries processed, 0 on error.
    @throws PvException On error a C-exception is thrown.
 */

DLL_INTERFACE int ScanDirNamesForClient(
    const char * dir_path,
    bool (* test_fcn)(const char *, const char *, void *),
    int (* sort_fcn)(const void *, const void *),
    void (* proc_fcn)(const char *, const char *, void *),
    void * client_data);

/** Alpha numerical case insensitive sorting function.
    This sorting function can be used in all ScanDir... functions.
    @param name1 first string to compare
    @param name2 second string to compare.
    @return 0 if strings are identical, 1 if name1 is
      lexically greater than name2, -1 if name1 is 
      lexically less than name2. All comparisons
      are performed case insensitive.
 */
DLL_INTERFACE int DirEntryAlphNumSort(const void * name1,
    const void * name2);

/* gen/src/lib/libPvUtil/FileToKey.c */

/** Create a key from an existing path name. 
    It works by obtaining the file status first, then adding 
    the dev id and the inode number	together. The <code>projId</code> is  
    xor'd into the top 8	bits. This key is stored in a directory and the
    inode number is returned for the key file in the directory.
    @param path Path for which a key should be returned.
    @param projId Project identifier, can be used to return serveral 
      different keys for different purposes from the same path.
    @return A unique key for the \c path and \c projId.
    @see PvCleanKeyDir
    
 */
DLL_INTERFACE unsigned PvFileToKey(const char * path, unsigned char projId);

/** Clean directory of stored keys created by <code>PvFileToKey</code>.
    @see PvFileToKey
 */
DLL_INTERFACE void PvCleanKeyDir(void);

/* gen/src/lib/libPvUtil/FormatDataPath.c */

/** Function to create (parts) of a dataset path (new dataset path form).
    This function can create a new dataset path of the form
    PREFIX/NAME/EXPNO/pdata/PROCNO.
    @param pathname Resulting dataset path.
    @param pathnameLen Maximum length of the dataset path.
    @param prefix Study directory prefix or disk unit (depend of 
       setting of user and type).
    @param name Name (study) directory. If 0 and <code>expno</code>, 
       <code>procno</code> are 0 only a prefix or type path is created.
    @param expno Experiment number. If 0 and <code>procno</code> is 0 only a 
        name (study) path is created.
    @param procno Processing number. If 0 and <code>procno</code>  only an 
        experiment path is created.
    @param suffix Suffix for dataset directory (e.g. file names, etc.).
    @return  The length of the constructed path.
    @retval -1 if \c pathname is \c 0.
    @retval -2 if the input parameters (\c prefix, \c name, \c expno, \c procno)
        are incorrect.
    @retval -3 If the length \c pathnameLen is not sufficient to hold
        the constructed path.
 */
DLL_INTERFACE int FormatDataPathname(
    char * pathname, unsigned int pathnameLen,
    const char * prefix,
    const char * name, int expno, int procno,
    const char * suffix);

/** Function to create (parts) of a dataset path.
    This function can create the following new dataset paths
    PREFIX/NAME/EXPNO/pdata/PROCNO and the depricated dataset paths.
    DU/data/USER/TYPE/NAME/EXPNO/pdata/PROCNO. The new dataset path will be 
    constructed if <code>user</code> and <code>type</code> are 0.
    @param pathname Resulting dataset path.
    @param pathnameLen Maximum length of the dataset path.
    @param prefix Study directory prefix, has higher priority that du.
    @param du Disk Unit. It is only valid if <code>prefix</code> is 0 
              or the empty string.
    @param user User name. It is only valid if <code>prefix</code> is 0 
                or the empty string and <code>du</code> is not 0 and not
                the empty string. If 0 and <code>type</code>, 
                <code>name</code>, <code>expno</code>, 
                and <code>procno</code> are 0 only a disk unit path is created.
    @param type Type.  It is only valid if <code>prefix</code> is 0 
                or the empty string and <code>du</code>, <code>user</code> 
                is not 0 and not the empty string. If 0 and <code>name</code>, 
                <code>expno</code>, and <code>procno</code> are 0 only a 
                disk unit or user path  is created.
    @param name Name (study) directory. If 0 and <code>expno</code>, 
       <code>procno</code> are 0 only a prefix or type path is created.
    @param expno Experiment number. If 0 and <code>procno</code> is 0 only a 
        name (study) path is created.
    @param procno Processing number. If 0 and <code>procno</code>  only an 
        experiment path is created.
    @param suffix Suffix for dataset directory (e.g. file names, etc.).
    @return  The length of the constructed path.
    @retval -1 if \c pathname is \c 0.
    @retval -2 if the input parameters (\a prefix, \a name, \a expno, 
       \a procno, \a du, \a user, \a type)  are incorrect.
    @retval -3 If the length \a pathnameLen is not sufficient to hold
        the constructed path.
 */
DLL_INTERFACE int FormatDataPathname(
    char * pathname, unsigned int pathnameLen,
    const char * prefix,
    const char * du, const char * user, const char * type,
    const char * name, int expno, int procno,
    const char * suffix);

/* gen/src/lib/libPvUtil/GetCc.c */

/** Get command and options for invoking the C compiler.
    This function creates a tight coupling between the compiler
    and its options.

    The compiler command and options will be determined with 
    three different strategy . The last of these strategies that 
    succeeds wins. In order, the strategies are:
    -# use the default compiler and options hardwired in the source
       code.
    -# check for an installed GNU C compiler in $XWINNMRHOME/gnu/bin.
       Use it if it is set.
    -# check the given environment variable for compiler and option.
       Use the values of the environment variables if they are set.
    
    @param envCmdName  Name of environment variable for compiler command.
    @param envOptsName Name of environment variable for compiler options.
    @param debug       Yes if compiled with debug information or optimized.
    @param cmd         Pointer to static output string returning compiler 
                       command.
    @param opts        Pointer to static output string returning compiler
                       options.
 */
DLL_INTERFACE void PvGetCcCmdAndOpts(const char * envCmdName,
    const char * envOptsName,
    bool debug,
    const char ** cmd,
    const char ** opts);

/** Get command and options for invoking the C preprocessor.
    This function creates a tight coupling between the compiler
    and its options.

    The compiler command and options will be determined with 
    three different strategy . The last of these strategies that 
    succeeds wins. In order, the strategies are:
    -# use the default compiler and options hardwired in the source
       code.
    -# check for an installed GNU C compiler in $XWINNMRHOME/gnu/bin.
       Use it if it is set.
    -# check the given environment variable for compiler and option.
       Use the values of the environment variables if they are set.
    
    @param envCmdName  Name of environment variable for compiler command.
    @param envOptsName Name of environment variable for compiler options.
    @param debug       Yes if compiled with debug information or optimized.
    @param cmd         Pointer to static output string returning compiler
                       command.
    @param opts        Pointer to static output string returning compiler
                       options.
 */
DLL_INTERFACE void PvGetCppCmdAndOpts(const char * envCmdName,
    const char * envOptsName,
    bool debug,
    const char ** cmd,
    const char ** opts);

/** Get command and option for invoking the linker.
    The linker command and options returned are chosen to create a 
    shared or dynamic library. This call provides a strong coupling
    between the linker command and the options.
    
    The linker command and options will be determined with 
    three different strategy . The last of these strategies that 
    succeeds wins. In order, the strategies are:
    -# use the default linker and options hardwired in the source
       code.
    -# check for an installed GNU C compiler in $XWINNMRHOME/gnu/bin.
       Use it for linking if it is set.
    -# check the given environment variable for linker and option.
       Use the values of the environment variables if they are set.

    @param envCmdName  Name of environment variable for linker command.
    @param envOptsName Name of environment variable for linker options.
    @param debug       Yes if compiled with debug information or optimized.
    @param cmd         Pointer to static output string returning linker command.
    @param opts        Pointer to static output string returning linker options.
 */
DLL_INTERFACE void PvGetShLdCmdAndOpts(const char * envCmdName,
    const char * envOptsName,
    bool debug,
    const char ** cmd,
    const char ** opts);
/* gen/src/lib/libPvUtil/GuiDate.c */

/** Tests given string against defined date formats.
 *  The dateString is tested against the following formats:
 *  - <code> [d]d.[m]m.yyyy </code>
 *  - <code> [d]d[.- ]MON[.- ]yyyy </code>
 *  - <code> [m]m/[d]d/yyyy </code>
 *  - <code> yyyymmdd </code>
 *  - <code> yyyy.[m]m.[d]d </code>
 *
 *  where d means a day digit, m a month digit, y a year digit. 
 *  MON is an abbreviation of the month (the first three characters of
 *  the English month name).
 *
 *  @param dateString String to test against date format.
 *  @param isUnixTime Must be Yes if date is after January, 1st 1970.
 *  @param day        Return value day of month if dateString is valid.
 *  @param month      Return value month of year if dateString is valid.
 *  @param year       Return value year of dateString if valid.
 *  @returns Yes if dateString was a valid against the defined date
 *               formats.
 *
 */
DLL_INTERFACE bool DATE_TestConvert(const char * dateString,
    bool isUnixTime,
    int * day, int * month, int * year);

/** Creates date string in the form <code>dd.MON.yyyy</code>.
 *  This function create a string representation of the given
 *  day, month, and year. For description of the string format 
 *  see <code>DATE_TestConvert</code>.
 *  \see DATE_TestConvert. 
 *  @param dateString Will be filled with string describing date.
 *  @param day        Day of month.
 *  @param month      Month of year.
 *  @param year       Year.
 */
DLL_INTERFACE void DATE_Normalize(char * dateString, int day,
    int month, int year);



/** Convert given milli seconds into a string representation.
 *  The returned <code>timeStr</code> has the format 
 *  <code> "hour"h"minute"m"sec"s"msec"ms"</code>.
 *  @param timeStr  String representation of given milli seconds. The string
 *                  must be allocated with a minimum of 16 characters.
 *  @param timeMsec Number of milli seconds.
 */
DLL_INTERFACE void PvUtilConvertMsecToStr(char * timeStr, double timeMsec);

/** Convert time string representation into @c msec.
 *  The parameter  <code>timeStr</code> must be in format 
 *  <code> "hour"h"minute"m"sec"s"msec"ms"</code>.
 *  @param timeStr  String representation of given milli seconds. 
 *  @param standard Standard representation as created by 
 *           <code>PvUtilConvertMsecToStr</code>,
 *  @return time in @c msec. On error -1.
 */
DLL_INTERFACE double PvUtilConvertTimeStrToMsec(const char * timeStr,
    bool standard);

/* gen/src/lib/libPvUtil/@param01Time.c */

/** Type for times since January 1st, 1970, 00:00:00 in msec. */
typedef int64_t timemsec_t;

/** Get the current time.
    @param ts A time structure that is filled. May be 0.
    @return The current time in milliseconds since 00:00:00 on January 1, 1970.
 */
DLL_INTERFACE timemsec_t PvUtilCurrentTime(struct PvTimeStruct * ts);

/** Get current time zone in minutes for given time.
    @param tsec Time in second from 1st January 1970.
    @return time zone offset in minutes.
 */
DLL_INTERFACE int PvUtilGetCurrentTimezone(time_t tsec);

/** Format a time structure element into a string representation.
    @param timeStr Buffer for string representation.
    @param len Maximum length of buffer.
    @param format Format string as defined in \c strftime(3).
    @param ts Pointer to time structure.
    @param localTime If \c true format creates the time string in 
           local time zone for this date else in the time zone defined by 
           <code>ts->tzoffset</code>. CAUTION: the format 
      option %z and %Z create incorrect time zone strings if \a localTime is
      set to \c false.
    @return 0 on error else number of characters in string.
 */
DLL_INTERFACE size_t PvUtilFormatTimeString(
    char * timeStr, size_t len, const char * format,
    const struct PvTimeStruct * ts, bool localTime);

/** Format a time structure element into a string representation.
    Add the number of milli seconds with a . to the %S format string.
    @param timeStr Buffer for string representation.
    @param len Maximum length of buffer.
    @param format Format string as defined in \c strftime(3).
    @param ts Pointer to time structure.
    @param localTime If \c true format creates the time string in local time 
    zone for this date else in the time zone defined by 
    <code>ts->tzoffset</code>. CAUTION: the format option %z and %Z create 
    incorrect time zone strings if \a localTime is set to \c false.
    @return 0 on error else number of characters in string.
 */
DLL_INTERFACE size_t PvUtilFormatTimeStringMsec(
    char * timeStr, size_t len, const char * format,
    const struct PvTimeStruct * ts, bool localTime);


/** Convert a time structure into a Iso8601 time string.
    The format of the Iso8601 time array is
    YYY-MM-DDTHH:MM:SS,MSEC+-HHMM.
    @param timeStr Time String to be filled.
    @param len The maximum length of the time string.
    @param ts A valid time structure.
    @param localtime If \c true create time string using local time zone,
      if \c false create time string using time zone stored in <code>ts->tzoffset</code>.
    @return 0 on error of number of characters set.
 */
DLL_INTERFACE size_t PvUtilIso8601TimeString(char * timeStr, size_t len,
    const struct PvTimeStruct * ts,
    bool localtime);


/** Parse an Iso8601 time string.
 *  @param ts The time structure to be filled.
 *  @param timeStr The time string.
 *  @param dateTimeSep Separator character between date and time.
 *  @param timeMsecSep Separator character between time and <code>msec</code>.
 *  @return first character after which parsing ends. If parsing
 *  was completely not possible return 0.
 */
DLL_INTERFACE const char * PvUtilParseIso8601TimeString(
    struct PvTimeStruct * ts, const char * timeStr,
    const char * dateTimeSep = "T", const char * timeMsecSep = ",");


/** Convert the time zone offset in <code>msec</code> to a HHMM time zone integer value.
 *  For example for <code>tzoffset=4400</code> return 130 for 
 *  1 hour and 30 seconds.
 *  @param[in] tzoffset The time zone offset in seconds.
 *  @return the time zone offset integer <code>HHMM</code>.
 */

DLL_INTERFACE int PvUtilGetTimeZoneHHMM(int tzoffset);

/* gen/src/lib/libPvUtil/IsoUidGen.c */

/** Create a unique ISO instance UID.
 *  This function creates a unique UID and fills it into <code>uid</code>.
 *  @param uid Filled with UID, Must be an allocated string of 64 
 *             characters excluding '\\0'.
 *  @return Unique ISO instance UID.
 */
DLL_INTERFACE char * PvUtilCreateISOInstanceUID(char * uid);

/** Create a special product UID which is unique in BRUKER.
 *  @param uid          Filled with UID, Must be an allocated string of 64 
 *                      characters excluding '\\0'.
 *  @param product      Product number.
 *  @param subProduct   Sub product number.
 *  @param majorVersion Major version number of product.
 *  @param minorVersion Minor version number of product.
 *  @param patchNumber  Number of path of product.
 *  @param date_string  String with product date.
 *  @return A pointer to \a uid or \c 0 if no product unique identfier
 *      was created.
 */
DLL_INTERFACE char * PvUtilCreateISOProductUID(char * uid,
    unsigned int product,
    unsigned int subProduct,
    unsigned int majorVersion,
    unsigned int minorVersion,
    unsigned int patchNumber,
    const char * date_string);

/** Create a special UID for datasets where the PROCNO is not writable.
 *  @param uid          Filled with UID, Must be an allocated string of 64 
 *                      characters excluding '\\0'.
 *  @param study        study name.
 *  @param expno        experiment number.
 *  @param procno       processing number.
 *  @param refpath      Path for this dataset.
 *  @return A pointer to the filled \a uid or \c 0 if not dataset unique
 *     identifier was created.
 */
DLL_INTERFACE char * PvUtilCreateDsetUid(char * uid,
    const char * study,
    int expno,
    int procno,
    const char * refpath);

/** Tests if given string is ISO UID.
 * @param uid String to test against UID syntax.
 * @param isBrukerUid Additional test if it is a Bruker UID.
 * @param endChar end character of ISO UID, can be used if ISO UID
 *                is only a prefix.
 * @return Yes if string is conform to ISO UID syntax else No.
 */
DLL_INTERFACE bool PvUtilTestIsoUid(const char * uid, bool isBrukerUid,
    char endChar);

/* gen/src/lib/libPvUtil/MakeDir.c */

/** 
 * Creates all missing directories for a give file path.
 * For a given file path all missing directories are created with
 * the given access permissions. 
 * This function does shows errors only in the return code and 
 * sets the permission of all created directories.
 *
 * CAUTION: <code>filepath</code> is interpreted a filename even if it
 *   is a directory name. That means only the directories 
 *   <code>dirname(filename)</code>  are created.
 * 
 * @param filepath Path to a file.
 * @param mode     Permissions of all created directories.
 * @return \c true if creation was successful, \c false if directory exists.
 * @throws PvException if creation was not possible and directory
 *    did not exist.
 */
DLL_INTERFACE bool PvUtilCreateFilePath(const char * filepath,
    mode_t mode);


/** 
 * Creates a all missing directories of the directory path.
 * This function does shows errors only in the return code and 
 * sets the permission of all created directories.
 *
 * @param dirpath Path to a directory that will be created.
 * @param mode     Permissions of all created directories.
 * @return \c true if creation was successful, \c false if directory exists.
 * @throws PvException if creation was not possible and directory
 *    did not exist.
 */
DLL_INTERFACE bool PvUtilCreateDirPath(const char * dirpath,
    mode_t mode);

/**
 * Create a temporary directory in the given parent path.
 * 
 * @param parent The directory in which the temporary directory is created.
 *     If \c 0 the temporary directory of the operating system is used.
 * @param prefix The prefix of the temporary directory name (not \c 0).
 * @param dir Buffer to hold the created directory name. 
 *     Must be minimum of PATH_MAX characters long.
 * @return 0 if creation was possible, -1 if temporary directory cannot be 
 *    created. 
 */
DLL_INTERFACE int PvUtilCreateTemporaryDirectory(const char * parent,
    const char * prefix, char * dir);

/** 
 * Creates all missing directories for a give file path.
 * For a given file path all missing directories are created with
 * the users <code>umask</code> for  access permissions. 
 * This function does shows errors only in the return code. 
 *
 * CAUTION: <code>filePath</code> is interpreted as filename even if it
 *   is a directory name. That means only the directories 
 *   <code>dirname(filename)</code> are created.
 * 
 * @param filePath Path to a file.
 * @return \c true if creation was successful, \c false if directory exists.
 * @throws PvException if creation was not possible and directory
 *    did not exist.
 * \see PvUtilCreateFilePath
 */
DLL_INTERFACE bool PvUtilCreateUmaskFilePath(const char * filePath);

/** 
 * Creates all missing directories for a give directory path.
 * For a given directory all missing directories are created with
 * the users <code>umask</code> for  access permissions. 
 * This function does shows errors only in the return code. 
 *
 * @param dirPath Directory to be created.
 * @return \c true if creation was successful, \c false if directory exists.
 * @throws PvException if creation was not possible and directory
 *    did not exist.
 * \see PvUtilCreateDirPath
 */
DLL_INTERFACE bool PvUtilCreateUmaskDirPath(const char * dirPath);

/**
 * Change access permissions of a file or directory.
 * @param path The file or directory.
 * @param mode The \c chmod access permissions.
 * @return 0 if permission change is successful else -1.
 */
DLL_INTERFACE int PvUtilChangePathPermission(const char * path,
    mode_t mode);

/* gen/src/lib/libDatPath/MkDatPath.c */

/** 
 * Create a data set path directory.
 * The path is created using the umask of the current user.
 * @param path new data set path name.
 * @return \c true if data set path is created, \c false if data set path
 *   already exists.
 * @throws PvException if the path has not the form of a dataset path (see 
 * <code>ParseDataPathname</code>, if is not possible to create the path.
 */
DLL_INTERFACE bool PvUtilCreateDatasetPath(const char * path);

/** 
 * Create a data set path.
 * The path is created using the umask of the current user from the 
 * concatenation of the path component names.
 * If the data set path already exists nothing happens.
 * @param datpath The dataset path root directory.
 * @param name The study directory component.
 * @param expno The experiment number.
 * @param procno The processing number.
 * @return \c true if data set path is created, \c false if data set path
 *   already exists.
 * @throws PvException if the arguments are incorrect or the path cannot
 * be created.
 */
DLL_INTERFACE bool PvUtilCreateDatasetPath(const char * datpath, const char * name,
    int expno, int procno);

/**
 * Search the next non-existing processing directory and create it.
 * Beginning from a start processing  number this routine searches for a 
 * non-existing processing directory in the current experiment path. If a 
 * non-existing processing directory is found it is created. 
 * This routine is multi-processing safe.
 * @param dstProcno Created dataset path buffer.
 * @param maxLen Maximum length of \a dstProcno buffer.
 * @param firstProcnoPath The fist processing path to be tested. The 
 *    corresponding experiment path must exist. If the processing path does not 
 *    exist it is created and returned in \a dstProcno. If it exists the next 
 *    greater processing number is used to create a new path.
 * @throws PvException if it is not able to create a new processing path.
 */
DLL_INTERFACE void PvUtilCreateNewProcnoPath(char * dstProcno, int maxLen, 
    const char * firstProcnoPath);

/**
 * Search the next non-existing processing directory and create it.
 * Beginning from a start processing  number this routine searches for a 
 * non-existing processing directory in the current experiment path. If a 
 * non-existing processing directory is found it is created. 
 * This routine is multi-processing safe.
 * @param dstProcno Created PROCNO path buffer.
 * @param maxLen Maximum length of \a dstProcno buffer.
 * @param datpath The dataset path root directory.
 * @param name The study directory component.
 * @param expno The experiment number. The experiment path consisting of 
 *    \a datpath, \a name, and \a expno must exist.
 * @param firstProcno The fist processing number to be tested in 
 *    the experiment path. If the processing path does not exist it
 *    is created. If it exists the next greater processing number is used to 
 *    create a new path. This is done in a loop until a new processing path
 *    is created.
 * @throws PvException if it is not able to create a new processing path.
 */
DLL_INTERFACE void PvUtilCreateNewProcnoPath(char * dstProcno, int maxLen, 
    const char * datpath, const char * name, int expno, int firstProcno);

/* gen/src/lib/libPvUtil/Modality.cc */

/**
 * Get the string representation for the given modality type.
 * @param modalityType The modality type for which the string representation
 *                     should be returned.
 * @return The string representation for the given modality type
 *         (never <code>NULL</code>).
 * @throws PvException if modality is unknown.
 */
DLL_INTERFACE const char* GetModalityTypeString(
    PvModalityType modalityType);

/**
 * Get the display string representation for the given modality type.
 * @param modalityType The modality type for which the string representation
 *                     should be returned.
 * @return The display string representation for the given modality type
 *         (never <code>NULL</code>).
 * @throws PvException if modality is unknown.
 */
DLL_INTERFACE const char* GetModalityTypeDisplayString(
    PvModalityType modalityType);

/**
 * Get the given modality type form the modality display string.
 * @param modalityTypeDisplayStr The modality type display string.
 * @return The modality type form the display string.
 * @throws PvException if modalityTypeDisplayStr is 0, empty, or is not 
 * a modality name..
 */
DLL_INTERFACE PvModalityType GetModalityTypeFromDisplayString(
    const char * modalityTypeDisplayStr);


/**
 * Get the modality type for a given string representation.
 * @param modalityTypeStr The type string for which the modality type
 *      is returned
 * @return The modality type from the string.
 * @throws PvException if the string is not the representation of a
 *  modality type.
 */
DLL_INTERFACE PvModalityType GetModalityTypeFromString(
    const char * modalityTypeStr);

/**
 * Get the modality type enumeration name string.
 * @param modalityType The modality type.
 * @return The modality enumeration string e.g. for \c modalityType 
 * \c MR_Modality the string \c "MR_Modality" is returned.
 * @throws PvException if modality is unknonwn.
 */
DLL_INTERFACE const char * GetModalityTypeEnumName(
    PvModalityType modalityType);

/* gen/src/lib/libPvUtil/ParseDataPath.c */


/** Parse a data path name into separate component items.
    A new style data path name has the following components:
    <code>DATPATH/NAME/EXPNO/pdata/PROCNO</code>.
    The pathname components written in lower-case must appear exactly as 
    shown. The other components are defined as:
    - DATPATH is the prefix path preceding the study directory.
    - NAME is the name of the study directory.
    - EXPNO is the experiment number directory.
    - PROCNO is the processed image number directory.
    This function can also parse parts of a data path. The parsing begins at 
    the end of the path and, therefore, <code>DATPATH</code> is always be the
    largest possible path.
    <p>Examples:</p>
    <pre> ParseDataPathname(path, IsProcnoPath, 0,0,0,0,0,0);</pre>
    <p>This call specifies that <code>path</code> is expected to be a processing 
    path name. The caller is not interested into the individual components. It
    can be used for checking the syntax of a data path name.</p>
    <pre> ParseDataPathname(path, IsProcnoPath, datpath, datpathLen, 
            name, nameLen, &expno, &procno); </pre>
    <p> This call demands all new style components of a dataset path name.
        They will be returned in the buffers if the buffers are long enough.</p>
    <p>The function will search for the largest data set path even if the <code>
    pathType</code> denotes a shorter data set path. For example, if 
    <code>IsNamePath</code> is specified but a processing path is found it
    returns <code>ProcnoPathFound</code> and sets all components of a 
    processing path (if they are requested).</p>

    @see DataPathResultCode
    @param path The path to parse for a data path name.
    @param pathType The type of the data path name to parse.
    @param datpath The prefix component of the data path set by this function. 
       If the pointer is 0 the prefix component is not requested and not set.
    @param datpathLen The maximum length allocated for <code>datapath</code>.
    @param name The study component of the data path set by this function.
       If the pointer is 0 the study component is not requested and not set.
    @param nameLen The maximum length allocated for <code>name</code>.
    @param expno The experiment number component  of the data path set by this
       function. If the pointer is 0 the experiment number component is not 
       requested and not set.
    @param procno The processed image number component set by this function.
       If the pointer is 0 the processed image number component is not
       requested and not set.
    @return The result code. Possible result codes are defined in the 
       enumeration <code>DataPathResultCode</code>. If the function was 
       able to parse <code>path</code>
       into the given <code>pathType</code> it returns an indicator of the
       biggest type that was able to be parsed. On error the code is negative.
    
 */
DLL_INTERFACE DataPathResultCode ParseDataPathname(
    const char * path, DataPathType pathType,
    char * datpath, unsigned int datpathLen,
    char * name, unsigned int nameLen,
    int * expno, int * procno);

/** Parse a data path name into separate component items.
    Same as ParseDataPathname but errors are reported as exceptions.

    @see ParseDataPathname
    @param path The path to parse for a data path name.
    @param pathType The type of the data path name to parse.
    @param datpath The prefix component of the data path set by this function. 
       If the pointer is 0 the prefix component is not requested and not set.
    @param datpathLen The maximum length allocated for <code>datapath</code>.
    @param name The study component of the data path set by this function.
       If the pointer is 0 the study component is not requested and not set.
    @param nameLen The maximum length allocated for <code>name</code>.
    @param expno The experiment number component  of the data path set by this
       function. If the pointer is 0 the experiment number component is not 
       requested and not set.
    @param procno The processed image number component set by this function.
       If the pointer is 0 the processed image number component is not
       requested and not set.
    @return The result code. If the function was able to parse <code>path</code>
       into the given <code>pathType</code> it returns an indicator of the
       biggest type that was able to be parsed. 
    @exception PvException in case of an error.
    
 */
DLL_INTERFACE DataPathResultCode ParseDataPathnameE(
    const char * path, DataPathType pathType,
    char * datpath, unsigned int datpathLen,
    char * name, unsigned int nameLen,
    int * expno, int * procno);

/** Parse a data path name into separate component items.
    This function can parse new style and old style data path names.
    A new style data path name has the following components:
    <code>DATPATH/NAME/EXPNO/pdata/PROCNO</code>.
    An old style data path name has the following components:
    <code>DU/data/USER/TYPE/NAME/EXPNO/pdata/PROCNO</code>
    Every old style data path name is also a new style data path name whereas
    <code>DATPATH = DU/data/USER/TYPE</code>.

    The pathname components written in lower-case must appear exactly as 
    shown. The other components are defined as:
    - DATPATH is the prefix path preceding the study directory.
    - DU is the name of the disk unit.
    - USER is the name of the user directory.
    - TYPE is the name of the type directory (<code>nmr</code>).
    - NAME is the name of the study directory.
    - EXPNO is the experiment number directory.
    - PROCNO is the processed image number directory.
    This function can also parse parts of a data path. The parsing begins at 
    the end of the path and, therefore, <code>DATPATH</code> and/or 
    <code>DU</code> is always be the largest possible path.

    @param path The path to parse for a data path name.
    @param pathType The type of the data path name to parse.
    @param datpath The prefix component of the data path set by this function. 
       If the ponter is 0 the prefix component is not requested and not set.
    @param datpathLen The maximum length allocated for <code>datapath</code>.
    @param du The disk unit component of the data path set by this function.
       If the pointer is not 0 <code>path</code> must be an old style data
       path name. If the pointer is 0 it is not set and <code>path</code>
       can be an old or new style data path name.
    @param duLen The maximum length allocated for <code>du</code>.
    @param user The user component of the data path set by this function.
       If the pointer is not 0 <code>path</code> must be an old style data
       path name. If the pointer is 0 it is not set and <code>path</code>
       can be an old or new style data path name.
    @param userLen The maximum length allocated for <code>user</code>.
    @param type The type component of the data path set by this function.
       If the pointer is not 0 <code>path</code> must be an old style data
       path name. If the pointer is 0 it is not set and <code>path</code>
       can be an old or new style data path name.
    @param typeLen The maximum length allocated for <code>type</code>.
    @param name The study component of the data path set by this function.
       If the pointer is 0 the study component is not requested and not set.
    @param nameLen The maximum length allocated for <code>name</code>.
    @param expno The experiment number component  of the data path set by this
       function. If the pointer is 0 the experiment number component is not 
       requested and not set.
    @param procno The processed image number component set by this function.
       If the pointer is 0 the processed image number component is not
       requested and not set.
    @return The result code. If the function was able to parse <code>path</code>
       into the given <code>pathType</code> it returns an indicator of the
       biggest type that was able to be parsed. On error the code is negative.
    
 */
DLL_INTERFACE DataPathResultCode ParseDataPathname(
    const char * path, DataPathType pathType,
    char * datpath, unsigned int datpathLen,
    char * du, unsigned int duLen,
    char * user, unsigned int userLen,
    char * type, unsigned int typeLen,
    char * name, unsigned int nameLen,
    int * expno, int * procno);

/** Parse a data path name into separate component items.
    Same as ParseDataPathname but errors are reported as exceptions.

    @see ParseDataPathname
    @param path The path to parse for a data path name.
    @param pathType The type of the data path name to parse.
    @param datpath The prefix component of the data path set by this function. 
       If the pointer is 0 the prefix component is not requested and not set.
    @param datpathLen The maximum length allocated for <code>datapath</code>.
    @param du The disk unit component of the data path set by this function.
       If the pointer is not 0 <code>path</code> must be an old style data
       path name. If the pointer is 0 it is not set and <code>path</code>
       can be an old or new style data path name.
    @param duLen The maximum length allocated for <code>du</code>.
    @param user The user component of the data path set by this function.
       If the pointer is not 0 <code>path</code> must be an old style data
       path name. If the pointer is 0 it is not set and <code>path</code>
       can be an old or new style data path name.
    @param userLen The maximum length allocated for <code>user</code>.
    @param type The type component of the data path set by this function.
       If the pointer is not 0 <code>path</code> must be an old style data
       path name. If the pointer is 0 it is not set and <code>path</code>
       can be an old or new style data path name.
    @param typeLen The maximum length allocated for <code>type</code>.
    @param name The study component of the data path set by this function.
       If the pointer is 0 the study component is not requested and not set.
    @param nameLen The maximum length allocated for <code>name</code>.
    @param expno The experiment number component  of the data path set by this
       function. If the pointer is 0 the experiment number component is not 
       requested and not set.
    @param procno The processed image number component set by this function.
       If the pointer is 0 the processed image number component is not
       requested and not set.
    @return The result code. If the function was able to parse <code>path</code>
       into the given <code>pathType</code> it returns an indicator of the
       biggest type that was able to be parsed. 
    @exception PvException in case of an error.
    
 */
DLL_INTERFACE DataPathResultCode ParseDataPathnameE(
    const char * path, DataPathType pathType,
    char * datpath, unsigned int datpathLen,
    char * du, unsigned int duLen,
    char * user, unsigned int userLen,
    char * type, unsigned int typeLen,
    char * name, unsigned int nameLen,
    int * expno, int * procno);

/** Parse a frame path name into separate component items.
    A new style frame path name has the following components:
    <code>DATPATH/NAME/EXPNO/pdata/PROCNO/FRAMENO</code>.
    An old style data path name has the following components:
    <code>DU/data/USER/TYPE/NAME/EXPNO/pdata/PROCNO</code>
    Every old style data path name is also a new style data path name whereas
    <code>DATPATH = DU/data/USER/TYPE</code>.
    The pathname components written in lower-case must appear exactly as 
    shown. The other components are defined as:
    - DATPATH is the prefix path preceding the study directory.
    - NAME is the name of the study directory.
    - EXPNO is the experiment number directory.
    - PROCNO is the processed image number directory.
    - FRAMENO indicates an XTIP frame number and is not a real directory.
    This function is identical to <code>ParseDataPathname</code> but can also
    retrieve a frame number.

    @see ParseDataPathname
    @param path The path to parse for a data path name.
    @param pathType The type of the data path name to parse.
    @param datpath The prefix component of the data path set by this function. 
       If the pointer is 0 the prefix component is not requested and not set.
    @param datpathLen The maximum length allocated for <code>datapath</code>.
    @param name The study component of the data path set by this function.
       If the pointer is 0 the study component is not requested and not set.
    @param nameLen The maximum length allocated for <code>name</code>.
    @param expno The experiment number component  of the data path set by this
       function. If the pointer is 0 the experiment number component is not 
       requested and not set.
    @param procno The processed image number component set by this function.
       If the pointer is 0 the processed image number component is not
       requested and not set.
    @param frameNo The frame number component set by this function.  
       If the pointer is 0 the frame number component is not requested and 
       not set.
    @return The result code. If the function was able to parse <code>path</code>
       into the given <code>pathType</code> it returns an indicator of the
       biggest type that was able to be parsed. On error the code is negative.
    
 */
DLL_INTERFACE DataPathResultCode ParseFramePathname(
    const char * path, DataPathType pathType,
    char * datpath, unsigned int datpathLen,
    char * name, unsigned int nameLen,
    int * expno, int * procno, int * frameNo);

/** Parse a frame path name into separate component items.
    Same as ParseFramePathname but errors are reported as exceptions.

    @see ParseFramePathname
    @param path The path to parse for a data path name.
    @param pathType The type of the data path name to parse.
    @param datpath The prefix component of the data path set by this function. 
       If the pointer is 0 the prefix component is not requested and not set.
    @param datpathLen The maximum length allocated for <code>datapath</code>.
    @param name The study component of the data path set by this function.
       If the pointer is 0 the study component is not requested and not set.
    @param nameLen The maximum length allocated for <code>name</code>.
    @param expno The experiment number component  of the data path set by this
       function. If the pointer is 0 the experiment number component is not 
       requested and not set.
    @param procno The processed image number component set by this function.
       If the pointer is 0 the processed image number component is not
       requested and not set.
    @param frameNo The frame number component set by this function.  
       If the pointer is 0 the frame number component is not requested and 
       not set.
    @return The result code. If the function was able to parse <code>path</code>
       into the given <code>pathType</code> it returns an indicator of the
       biggest type that was able to be parsed. 
    @exception PvException in case of an error.
    
 */
DLL_INTERFACE DataPathResultCode ParseFramePathnameE(
    const char * path, DataPathType pathType,
    char * datpath, unsigned int datpathLen,
    char * name, unsigned int nameLen,
    int * expno, int * procno, int * frameNo);

/** Parse a frame path name into separate component items.
    A new style frame path name has the following components:
    <code>DATPATH/NAME/EXPNO/pdata/PROCNO/FRAMENO</code>.
    The pathname components written in lower-case must appear exactly as 
    shown. The other components are defined as:
    - DATPATH is the prefix path preceding the study directory.
    - DU is the name of the disk unit.
    - USER is the name of the user directory.
    - TYPE is the name of the type directory (<code>nmr</code>).
    - NAME is the name of the study directory.
    - EXPNO is the experiment number directory.
    - PROCNO is the processed image number directory.
    - FRAMENO indicates an XTIP frame number and is not a real directory.
    This function is identical to <code>ParseDataPathname</code> but can also
    retrieve a frame number.

    @see ParseDataPathname
    @param path The path to parse for a data path name.
    @param pathType The type of the data path name to parse.
    @param datpath The prefix component of the data path set by this function. 
       If the pointer is 0 the prefix component is not requested and not set.
    @param datpathLen The maximum length allocated for <code>datapath</code>.
    @param du The disk unit component of the data path set by this function.
       If the pointer is not 0 <code>path</code> must be an old style data
       path name. If the pointer is 0 it is not set and <code>path</code>
       can be an old or new style data path name.
    @param duLen The maximum length allocated for <code>du</code>.
    @param user The user component of the data path set by this function.
       If the pointer is not 0 <code>path</code> must be an old style data
       path name. If the pointer is 0 it is not set and <code>path</code>
       can be an old or new style data path name.
    @param userLen The maximum length allocated for <code>user</code>.
    @param type The type component of the data path set by this function.
       If the pointer is not 0 <code>path</code> must be an old style data
       path name. If the pointer is 0 it is not set and <code>path</code>
       can be an old or new style data path name.
    @param typeLen The maximum length allocated for <code>type</code>.
    @param name The study component of the data path set by this function.
       If the pointer is 0 the study component is not requested and not set.
    @param nameLen The maximum length allocated for <code>name</code>.
    @param expno The experiment number component  of the data path set by this
       function. If the pointer is 0 the experiment number component is not 
       requested and not set.
    @param procno The processed image number component set by this function.
       If the pointer is 0 the processed image number component is not
       requested and not set.
    @param frameNo The frame number component set by this function.  
       If the pointer is 0 the frame number component is not requested and 
       not set.
    @return The result code. If the function was able to parse <code>path</code>
       into the given <code>pathType</code> it returns an indicator of the
       biggest type that was able to be parsed. On error the code is negative.
    
 */
DLL_INTERFACE DataPathResultCode ParseFramePathname(
    const char * path, DataPathType pathType,
    char * datpath, unsigned int datpathLen,
    char * du, unsigned int duLen,
    char * user, unsigned int userLen,
    char * type, unsigned int typeLen,
    char * name, unsigned int nameLen,
    int * expno, int * procno, int * frameNo);

/** Parse a frame path name into separate component items.
    Same as ParseFramePathname but errors are reported as exceptions.

    @see ParseFramePathname
    @param path The path to parse for a data path name.
    @param pathType The type of the data path name to parse.
    @param datpath The prefix component of the data path set by this function. 
       If the pointer is 0 the prefix component is not requested and not set.
    @param datpathLen The maximum length allocated for <code>datapath</code>.
    @param du The disk unit component of the data path set by this function.
       If the pointer is not 0 <code>path</code> must be an old style data
       path name. If the pointer is 0 it is not set and <code>path</code>
       can be an old or new style data path name.
    @param duLen The maximum length allocated for <code>du</code>.
    @param user The user component of the data path set by this function.
       If the pointer is not 0 <code>path</code> must be an old style data
       path name. If the pointer is 0 it is not set and <code>path</code>
       can be an old or new style data path name.
    @param userLen The maximum length allocated for <code>user</code>.
    @param type The type component of the data path set by this function.
       If the pointer is not 0 <code>path</code> must be an old style data
       path name. If the pointer is 0 it is not set and <code>path</code>
       can be an old or new style data path name.
    @param typeLen The maximum length allocated for <code>type</code>.
    @param name The study component of the data path set by this function.
       If the pointer is 0 the study component is not requested and not set.
    @param nameLen The maximum length allocated for <code>name</code>.
    @param expno The experiment number component  of the data path set by this
       function. If the pointer is 0 the experiment number component is not 
       requested and not set.
    @param procno The processed image number component set by this function.
       If the pointer is 0 the processed image number component is not
       requested and not set.
    @param frameNo The frame number component set by this function.  
       If the pointer is 0 the frame number component is not requested and 
       not set.
    @return The result code. If the function was able to parse <code>path</code>
       into the given <code>pathType</code> it returns an indicator of the
       biggest type that was able to be parsed. 
    @exception PvException in case of an error.
    
 */
DLL_INTERFACE DataPathResultCode ParseFramePathnameE(
    const char * path, DataPathType pathType,
    char * datpath, unsigned int datpathLen,
    char * du, unsigned int duLen,
    char * user, unsigned int userLen,
    char * type, unsigned int typeLen,
    char * name, unsigned int nameLen,
    int * expno, int * procno, int * frameNo);



/* gen/src/lib/libPvUtil/PathIs.c */

/**
 * Indicates if a path is an absolute or relation path.
 * @param path The path name.
 * @return \c true if \a path is an absolute path, 
 * \c false if it is a relative path.
 */
DLL_INTERFACE bool PvPathIsAbsolute(const char * path);

/** Test if the given path is a subdirectory.
 *  This function will test the path that it receives to see if its base name
 *  refers to a subdirectory of the directory name. 
 * @param path Path name to test .
 * @return Yes if <code>path</code> is a subdirectory else No.
 */
DLL_INTERFACE bool PathIsSubdir(const char * path);


/** Test if the given path is a subdirectory.
 *  This function does the same as PathIsSubdir. It is mainly used
 *  as interface function for <code>ReadDirEntriesForClient</code>.
 * \see PathIsSubdir
 * \see ReadDirEntriesForClient
 * @param path       Path name to test.
 * @param clientdata Not used.
 * @return Yes if <code>path</code> is a subdirectory else No.
 */
DLL_INTERFACE bool PathIsSubdirForClient(const char * path, void * clientdata);


/** Test if the given path is a subdirectory.
 *  This function does the same as PathIsSubdir. The path
 *  to test is dirName/baseName.
 * \see PathIsSubdir
 * @param dirName  Name parent directory.
 * @param baseName Name of directory to test (without path).
 * @return Yes if <code>dirName/baseName</code> is a subdirectory else No.
 */
DLL_INTERFACE bool PathNameIsSubdir(const char * dirName,
    const char * baseName);

/** Test if the given path is a subdirectory.
 *  This function does the same as PathNameIsSubdir. It is mainly used
 *  as interface function for <code>ReadDirNamesForClient</code>.
 * \see PathNameIsSubdir
 * \see ReadDirNamesForClient
 * @param dirName    Name parent directory.
 * @param baseName   Name of directory to test (without path).
 * @param clientdata Not used.
 * @return Yes if <code>dirName/baseName</code> is a subdirectory else No.
 */
DLL_INTERFACE bool PathNameIsSubdirForClient(const char * dirName,
    const char * baseName,
    void * clientdata);
/** Test if the given path is a subdirectory.
 *  This function does the same as PathIsSubdir except it will not
 *  follow symbolic links.
 * \see PathIsSubdir
 * @param path Path name to test.
 * @return Yes if <code>path</code> is a subdirectory else No.
 */
DLL_INTERFACE bool PathIsSubdirNF(const char * path);

/** Test if the given path is a subdirectory.
 *  This function does the same as PathIsSubdirForClient except it will not
 *  follow symbolic links.
 * \see PathIsSubdirForClient
 * @param path       Path name to test.
 * @param clientdata Not used.
 * @return Yes if <code>path</code> is a subdirectory else No.
 */
DLL_INTERFACE bool PathIsSubdirNFForClient(const char * path,
    void * clientdata);
/** Test if the given path is a subdirectory.
 *  This function does the same as PathIsSubdirNF. The path
 *  to test is dirName/baseName.
 * \see PathIsSubdirNF
 * @param dirName  Name parent directory.
 * @param baseName Name of directory to test (without path).
 * @return Yes if <code>dirName/baseName</code> is a subdirectory else No.
 */
DLL_INTERFACE bool PathNameIsSubdirNF(const char * dirName,
    const char * baseName);

/** Test if the given path is a subdirectory.
 *  This function does the same as PathNameIsSubdirNF. It is mainly used
 *  as interface function for <code>ReadDirNamesForClient</code>.
 * \see PathNameIsSubdirNF
 * \see ReadDirNamesForClient
 * @param dirName    Name parent directory.
 * @param baseName   Name of directory to test (without path).
 * @param clientdata Not used.
 * @return Yes if <code>dirName/baseName</code> is a subdirectory else No.
 */
DLL_INTERFACE bool PathNameIsSubdirNFForClient(const char * dirName,
    const char * baseName,
    void * clientdata);
/** Test if the given path is a directory.
 *  This function will test the path that it receives
 *  to see if it is a directory.
 * @param path Path name to test .
 * @return Yes if <code>path</code> is a subdirectory else No.
 */
DLL_INTERFACE bool PathIsDir(const char * path);

/** Test if the given path is a directory.
 *  This function does the same as PathIsDir. It is mainly used
 *  as interface function for <code>ReadDirEntriesForClient</code>.
 * \see PathIsDir.
 * \see ReadDirEntriesForClient
 * @param path       Path name to test.
 * @param clientdata Not used.
 * @return Yes if <code>path</code> is a directory else No.
 */
DLL_INTERFACE bool PathIsDirForClient(const char * path, void * clientdata);

/** Test if the given path is a directory.
 *  This function does the same as PathIsDir. The path
 *  to test is dirName/baseName.
 * \see PathIsDir.
 * @param dirName  Name parent path.
 * @param baseName Name of path to test.
 * @return Yes if <code>dirName/baseName</code> is a directory else No.
 */
DLL_INTERFACE bool PathNameIsDir(const char * dirName, const char * baseName);

/** Test if the given path is a directory.
 *  This function does the same as PathNameIsDir. It is mainly used
 *  as interface function for <code>ReadDirNamesForClient</code>.
 * \see PathNameDir
 * \see ReadDirNamesForClient
 * @param dirName    Name parent path.
 * @param baseName   Name of path to test.
 * @param clientdata Not used.
 * @return Yes if <code>dirName/baseName</code> is a directory else No.
 */
DLL_INTERFACE bool PathNameIsDirForClient(const char * dirName,
    const char * baseName,
    void * clientdata);

/** Test if the given path is a directory.
 *  This function does the same as PathIsDir except it will not
 *  follow symbolic links.
 * \see PathIsDir
 * @param path Path name to test.
 * @return Yes if <code>path</code> is a directory else No.
 */
DLL_INTERFACE bool PathIsDirNF(const char * path);

/** Test if the given path is a directory.
 *  This function does the same as PathIsDirForClient except it will not
 *  follow symbolic links.
 * \see PathIsDirForClient
 * @param path       Path name to test.
 * @param clientdata Not used.
 * @return Yes if <code>path</code> is a directory else No.
 */
DLL_INTERFACE bool PathIsDirNFForClient(const char * path, void * clientdata);

/** Test if the given path is a directory.
 *  This function does the same as PathIsDirNF. The path
 *  to test is dirName/baseName.
 * \see PathIsDirNF
 * @param dirName  Name parent path. 
 * @param baseName Name of path to test.
 * @return Yes if <code>dirName/baseName</code> is a directory else No.
 */
DLL_INTERFACE bool PathNameIsDirNF(const char * dirName,
    const char * baseName);
/** Test if the given path is a directory.
 *  This function does the same as PathNameIsDirNF. It is mainly used
 *  as interface function for <code>ReadDirNamesForClient</code>.
 * \see PathNameIsDirNF
 * \see ReadDirNamesForClient
 * @param dirName    Name parent path.
 * @param baseName   Name of path to test.
 * @param clientdata Not used.
 * @return Yes if <code>dirName/baseName</code> is a directory else No.
 */
DLL_INTERFACE bool PathNameIsDirNFForClient(const char * dirName,
    const char * baseName,
    void * clientdata);

/** Test if given paths base name is a positive integer number and a subdirectory.
 *  This function tests if the given path is a subdirectory and the base name
 *  of this path must be only digits.
 *  @param path Path to test.
 *  @return Yes if <code>path</code> fulfills the condition else No.
 */
DLL_INTERFACE bool PathIsNumSubdir(const char * path);

/** Test if given paths base name is a positive integer number and a subdirectory.
 *  This function does the same as @ref PathIsNumSubdir. It is mainly used
 *  as interface function for @ref ReadDirEntriesForClient
 *  \see @ref PathIsNumSubdir
 *  \see @ref ReadDirEntriesForClient
 *  @param path       Path name to test.
 *  @param clientdata Not used.
 *  @return Yes if <code>path</code> fulfills the condition else No.
 */
DLL_INTERFACE bool PathIsNumSubdirForClient(const char * path,
    void * clientdata);

/** Test if given paths base name is a positive integer number and a subdirectory.
 *  This function does the same as @ref PathIsNumSubdir. The path
 *  to test is dirName/baseName.
 * \see @ref PathIsNumSubdir
 * @param dirName  Name parent path.
 * @param baseName Name of path.
 * @return Yes if \a dirName / \a baseName fulfills the condition else No.
 */
DLL_INTERFACE bool PathNameIsNumSubdir(const char * dirName,
    const char * baseName);


/** Test if given paths base name is a positive integer number and a subdirectory.
 *  This function does the same as @ref PathNameIsNumSubdir.  It is mainly used
 *  as interface function for @ref ReadDirNamesForClient.
 * \see @ref PathNameIsNumSubdir
 * \see @ref ReadDirNamesForClient
 * @param dirName    Name parent path.
 * @param baseName   Name of path.
 * @param clientdata Not used.
 * @return Yes if <code>dirName/baseName</code> fulfills the condition else No.
 */
DLL_INTERFACE bool PathNameIsNumSubdirForClient(const char * dirName,
    const char *baseName,
    void * clientdata);

/** Test if given paths base name is a positive integer number and a subdirectory.
 *  This function does the same as @ref PathIsNumSubdir except that it does not
 *  follow links.
 *  \see @ref PathIsSubdirNF
 *  @param path Path name to test.
 * @return Yes if \a path fulfills the condition else No.
 */
DLL_INTERFACE bool PathIsNumSubdirNF(const char * path);

/** Test if given paths base name is a positive integer number and a subdirectory.
 *  This function does the same as @ref PathIsNumSubdirForClient except that 
 *  it does not follow links. It is mainly used
 *  as interface function for @ref ReadDirEntriesForClient.
 *  \see @ref PathIsNumSubdirForClient
 *  \see @ref ReadDirEntriesForClient
 *  @param path       Path name to test.
 *  @param clientdata Not used.
 * @return Yes if <code>path</code> fulfills the condition else No.
 */
DLL_INTERFACE bool PathIsNumSubdirNFForClient(const char * path,
    void * clientdata);
/** Test if given base name is a positive integer number and a subdirectory.
 *  This function does the same as @ref PathIsNumSubdirNF. The path
 *  to test is dirName/baseName.
 * \see @ref PathIsNumSubdirNF
 * @param dirName  Name parent directory.
 * @param baseName Name of directory to test (without path).
 * @return Yes if <code>dirName/baseName</code> fulfills the condition else No.
 */
DLL_INTERFACE bool PathNameIsNumSubdirNF(const char * dirName,
    const char * baseName);

/** Test if given base name is a positive integer number and a subdirectory.
 *  This function does the same as @ref PathIsNumSubdirNF. It is mainly used
 *  as interface function for @ref ReadDirNamesForClient.
 * \see @ref PathIsNumSubdirNF
 * \see @ref ReadDirNamesForClient
 * @param dirName    Name parent directory.
 * @param baseName   Name item to test.
 * @param clientdata Not used.
 * @return Yes if <code>dirName/baseName</code> fulfills the condition else No.
 */
DLL_INTERFACE bool PathNameIsNumSubdirNFForClient(const char * dirName,
    const char * baseName,
    void * clientdata);

/** Test if given path is a regular file.
 *  This function tests if the path is the name of a regular file. The function
 *  follows symbolic links.
 * @param path Name parent directory.
 * @return Yes if <code>path</code> is a regular file else No.
 */
DLL_INTERFACE bool PathIsRegFile(const char * path);

/** Test if given path is a regular file.
 *  This function tests if the path is the name of a regular file. The function
 *  follows symbolic links. It is mainly used
 *  as interface function for <code>ReadDirEntriesForClient</code>.
 * \see PathIsRegFile.
 * \see ReadDirEntriesForClient.
 * @param path       Name parent directory.
 * @param clientdata Not used.
 * @return Yes if <code>path</code> is a regular file else No.
 */
DLL_INTERFACE bool PathIsRegFileForClient(const char * path,
    void * clientdata);

/** Test if given path dirName/baseName is a regular file.
 *  This function tests if the <code>dirName/baseName</code> is the name 
 *  of a regular file. The function follows symbolic links.
 * \see PathIsRegFile
 * @param dirName  Name parent directory.
 * @param baseName Name item to test.
 * @return Yes if <code>dirName/baseName</code> is a regular file else No.
 */
DLL_INTERFACE bool PathNameIsRegFile(const char * dirName,
    const char * baseName);

/** Test if given path dirName/baseName is a regular file.
 *  This function tests if the <code>dirName/baseName</code> is the name 
 *  of a regular file. The function follows symbolic links. It is mainly used
 *  as interface function for <code>ReadDirNamesForClient</code>.
 * \see PathIsRegFileForClient
 * \see ReadDirNamesForClient
 * @param dirName    Name parent directory.
 * @param baseName   Name item to test.
 * @param clientdata Not used.
 * @return Yes if <code>dirName/baseName</code> is a regular file else No.
 */
DLL_INTERFACE bool PathNameIsRegFileForClient(const char * dirName,
    const char * baseName,
    void * clientdata);

/** Test if given path is a regular file.
 *  This function tests if the path is the name of a regular file. The function
 *  does not follow symbolic links.
 * \see PathIsRegFile.
 * @param path Name parent directory.
 * @return Yes if <code>path</code> is a regular file else No.
 */
DLL_INTERFACE bool PathIsRegFileNF(const char * path);

/** Test if given path is a regular file.
 *  This function tests if the path is the name of a regular file. The function
 *  does not follow symbolic links. It is mainly used
 *  as interface function for <code>ReadDirEntriesForClient</code>.
 * \see PathIsRegFileNF.
 * \see ReadDirEntriesForClient.
 * @param path       Name parent directory.
 * @param clientdata Not used.
 * @return Yes if <code>path</code> is a regular file else No.
 */
DLL_INTERFACE bool PathIsRegFileNFForClient(const char *path,
    void * clientdata);

/** Test if given path dirName/baseName is a regular file.
 *  This function tests if the <code>dirName/baseName</code> is the name 
 *  of a regular file. The function does not follow symbolic links.
 * \see PathIsRegFileNF
 * @param dirName  Name parent directory.
 * @param baseName Name item to test.
 * @return Yes if <code>dirName/baseName</code> is a regular file else No.
 */
DLL_INTERFACE bool PathNameIsRegFileNF(const char * dirName,
    const char * baseName);
/** Test if given path dirName/baseName is a regular file.
 *  This function tests if the <code>dirName/baseName</code> is the name 
 *  of a regular file. The function does not follow symbolic links. 
 *  It is mainly used as interface function for 
 *  <code>ReadDirNamesForClient</code>.
 * \see PathNameIsRegFileNF.
 * \see PathIsRegFileForClient.
 * \see ReadDirNamesForClient.
 * @param dirName    Name parent directory.
 * @param baseName   Name item to test.
 * @param clientdata Not used.
 * @return Yes if <code>dirName/baseName</code> is a regular file else No.
 */
DLL_INTERFACE bool PathNameIsRegFileNFForClient(const char * dirName,
    const char * baseName,
    void * clientdata);

/** Test if given path is a symbolic link.
 *  This function tests if the <code>path</code> is the name 
 *  of a symbolic link. 
 * @param path Path to test.
 * @return Yes if <code>path</code> is a symbolic link else No.
 */
DLL_INTERFACE bool PathIsSymLink(const char * path);

/** Test if given path is a symbolic link.
 *  This function tests if the path is the name of a symbolic link. It is 
 *  mainly used  as interface function for <code>ReadDirEntriesForClient</code>.
 * \see PathIsSymLink
 * \see ReadDirEntriesForClient.
 * @param path       Path name to test. 
 * @param clientdata Not used.
 * @return Yes if <code>path</code> is a symbolic link else No.
 */
DLL_INTERFACE bool PathIsSymLinkForClient(const char * path,
    void * clientdata);

/** Test if given path dirName/baseName is a symbolic link.
 *  This function tests if the <code>dirName/baseName</code> is the name 
 *  of a symbolic link. 
 * \see PathIsSymLink
 * @param dirName  Parent directory name.
 * @param baseName Item name to test.
 * @return Yes if <code>dirName/baseName</code> is a symbolic link else No.
 */
DLL_INTERFACE bool PathNameIsSymLink(const char * dirName,
    const char * baseName);

/** Test if given path dirName/baseName is a symbolic link.
 *  This function tests if the <code>dirName/baseName</code> is the name 
 *  of a symbolic link. 
 * \see PathIsSymLink
 * \see ReadDirNamesForClient.
 * @param dirName    Parent directory name.
 * @param baseName   Item name to test.
 * @param clientdata Not used.
 * @return Yes if <code>dirName/baseName</code> is a symbolic link else No.
 */
DLL_INTERFACE bool PathNameIsSymLinkForClient(const char * dirName,
    const char *baseName,
    void * clientdata);

/** Determines if two paths point to the same directory or file.
 *  The real paths are compared, i.e. not symbolic links.
 *  @param path1  First path to compare
 *  @param path2  Second path to compare
 *  @return  Yes if the paths are different.
 *  @throws PvException. The system cannot determine if the paths
 *               are are different.
 */
DLL_INTERFACE bool PathsDiffer(const char * path1,
    const char * path2);
/* gen/src/lib/libPvUtil/ProgramDefs.c */

/** Return program identifier.
 *  Each program can have a unique identifier that must be set
 *  with <code>TeSetProgramId</code>.
 *  \see TeSetProgramId
 *  @return Program name.
 */
DLL_INTERFACE const char *TeGetProgramId(void);

/** Test if program identifier is set.
 *  \see TeSetProgramId
 *  @return Yes if program identifier is set else No.
 */
DLL_INTERFACE bool TeIsProgramId(void);

/** Set program identifier.
 *  Each program can have a unique identifier that is set in this
 *  function.
 *  \see TeGetProgramId
 *  @param id New program identifier.
 */
DLL_INTERFACE void TeSetProgramId(const char * id);

/* gen/src/lib/libPvUtil/PvInstrument.c */

/**
 * Get the current instrument from the conf/instr/curinst file.
 * @return The content of the first line of the conf/instr/curinst file.
 * @throws PvException if there is not a configuration available 
 * (conf/instr/curinst is missing).
 */
DLL_INTERFACE std::string PvUtilGetInstrument();

/**
 * Get the current local instrument name.
 * @return The name of the local configuration.
 * @throws PvException if no configuration is configured.
 */
DLL_INTERFACE std::string PvUtilGetLocalInstrument();

/**
 * Make the path for the common configuration of the current instrument.
 * The path may not exist and is not created.
 * @param path The path buffer to be filled.
 * @param maxLen The maximum length of the path buffer.
 * @param suffix The suffix that is added to the directory (configdir/suffix).
 * @return The number of characters that are needed to create this path.
 * @throws PvException if there is not a configuration available 
 * (conf/instr/curinst is missing).
 */
DLL_INTERFACE int PvPathMkCommonInstrumentPath(char * path, unsigned int maxLen,
    const char * suffix);

/**
 * Make the path for the modality configuration of the current instrument.
 * The path may not exist and is not created.
 * @param path The path buffer to be filled.
 * @param maxLen The maximum length of the path buffer.
 * @param modality The modality of the instrument.
 * @param suffix The suffix that is added to the directory (configdir/suffix).
 * @return The number of characters that are needed to create this path.
 * @throws PvException if there is not a configuration available 
 * (conf/instr/curinst is missing).
 */
DLL_INTERFACE int PvPathMkLocalInstrumentPath(char * path, unsigned int maxLen,
    PvModalityType modality, const char * suffix);

/**
 * Make the path for the user common configuration of the current instrument.
 * The path may not exist and is not created.
 * @param path The path buffer to be filled.
 * @param maxLen The maximum length of the path buffer.
 * @param suffix The suffix that is added to the directory (configdir/suffix).
 * @return The number of characters that are needed to create this path.
 * @throws PvException if there is not a configuration available 
 * (conf/instr/curinst is missing).
 */
DLL_INTERFACE int PvPathMkUserCommonInstrumentPath(char * path,
    unsigned int maxLen, const char * suffix);

/**
 * Make the path for the modality user configuration of the current instrument.
 * The path may not exist and is not created.
 * @param path The path buffer to be filled.
 * @param maxLen The maximum length of the path buffer.
 * @param modality The modality of the instrument.
 * @param suffix The suffix that is added to the directory (configdir/suffix).
 * @return The number of characters that are needed to create this path.
 * @throws PvException if there is not a configuration available 
 * (conf/instr/curinst is missing).
 */
DLL_INTERFACE int PvPathMkUserInstrumentPath(char * path, unsigned int maxLen,
    PvModalityType modality, const char * suffix);

/* gen/src/lib/libPvUtil/PvPath.c */

/** Get the parent path of the given path.
    @param path The (absolute or relative) path whose parent should be returned.
    @return The (absolute or relative) parent path or an empty string,
            if the path does not have a parent.
 */
DLL_INTERFACE std::string PvPathGetParent(const std::string& childPath);

/**
 * Set the ParaVision installation directory.
 * If the environment variable XWINNMRHONE is set this call is not 
 * necessary. It must be called as early as possible.
 * 
 * CAUTION: This call does not change the topspin functions of libPath 
 * library. They always use the environment variable XWINNMRHOME.
 * @param installDirectory The ParaVision installation directory.
 * @throws PvException if \a installDirectory is \c nullptr, empty or too long.
 */
DLL_INTERFACE void PvPathSetInstDir(const char * installDirectory);

/** Create string for the current ParaVision installation directory.
    @param path Buffer to fill in the current installation directory.
    @param maxLen Maximum length of path. It is guaranteed that not more than 
       this length characters are filled into the string.
    @param suffix If suffix is not 0 the \c path is created as
    PVINSTDIR/suffix.
    @return length of created string. If is the complete length, even if
      \a path does not contain enough space.
 * @throws PvException if installation directory is not set or XWINNMRHOME 
 * environment variable is is not set.
 */
DLL_INTERFACE int PvPathMkInst(char * path, unsigned int maxLen,
    const char * suffix);

/** Create string for the current ParaVision genX path.
    For linux systems \c PVINSTDIR/gen2, for Windows
    \c PVINSTDIR/gen8 ist created.
    @param path Buffer for created string.
    @param maxLen Maximum length of path. It is guaranteed that not more than 
       this length characters are filled into the string.
    @param suffix If \c suffix if not 0 the \c path is filled as
       \c PVINSTDIR/GENX/suffix .
    @return length of created string. If is the complete length, even if
      \a path does not contain enough space.
 * @throws PvException if installation directory is not set or XWINNMRHOME 
 * environment variable is is not set.
 */
DLL_INTERFACE int PvPathMkGenX(char * path, unsigned int maxLen,
    const char * suffix);

/** Create string for the current ParaVision \c prog directory.
    @param path Buffer for the created string.
    @param maxLen Maximum length of path. It is guaranteed that not more than 
       this length characters are filled into the string.
    @param suffix If \c suffix if not 0 the \c path is filled as
    \c PVINSTDIR/prog/suffix .
    @return length of created string. If is the complete length, even if
      \a path does not contain enough space.
 * @throws PvException if installation directory is not set or XWINNMRHOME 
 * environment variable is is not set.
 */
DLL_INTERFACE int PvPathMkProg(char * path, unsigned int maxLen,
    const char * suffix);

/** Create string for the ParaVision \c conf directory.
    @param path Buffer for the created string.
    @param maxLen Maximum length of path. It is guaranteed that not more than 
       this length characters are filled into the string.
    @param suffix If \c suffix if not 0 the \c path is filled as
    \c PVINSTDIR/conf/suffix .
    @return length of created string. If is the complete length, even if
      \a path does not contain enough space.
 * @throws PvException if installation directory is not set or XWINNMRHOME 
 * environment variable is is not set.
 */
DLL_INTERFACE int PvPathMkConf(char * path, unsigned int maxLen,
    const char * suffix);

/** Create string for the ParaVision \c exp/stan/nmr directory.
    @param path Buffer for the created string.
    @param maxLen Maximum length of path. It is guaranteed that not more than 
       this length characters are filled into the string.
    @param suffix If \c suffix if not 0 the \c path is filled as
    \c PVINSTDIR/exp/stan/nmr/suffix .
    @return length of created string. If is the complete length, even if
      \a path does not contain enough space.
 * @throws PvException if installation directory is not set or XWINNMRHOME 
 * environment variable is is not set.
 */
DLL_INTERFACE int PvPathMkExpStanNmr(char * path, unsigned int maxLen,
    const char * suffix);

/** Create string for the ParaVision \c prog/bin directory.
    @param path Buffer for the created string.
    @param maxLen Maximum length of path. It is guaranteed that not more than 
       this length characters are filled into the string.
    @param suffix If \c suffix if not 0 the \c path is filled as
    \c PVINSTDIR/prog/bin/suffix .
    @return length of created string. If is the complete length, even if
      \a path does not contain enough space.
 * @throws PvException if installation directory is not set or XWINNMRHOME 
 * environment variable is is not set.
    @see PvPathMkProg
 */
DLL_INTERFACE int PvPathMkProgBin(char * path, unsigned int maxLen,
    const char * suffix);


/** Create string for the ParaVision \c exp directory.
    @param path Buffer for the created string.
    @param maxLen Maximum length of path. It is guaranteed that not more than 
       this length characters are filled into the string.
    @param suffix If \c suffix if not 0 the \c path is filled as
    \c PVINSTDIR/exp/suffix .
    @return length of created string. If is the complete length, even if
      \a path does not contain enough space.
 * @throws PvException if installation directory is not set or XWINNMRHOME 
 * environment variable is is not set.
 */
DLL_INTERFACE int PvPathMkExp(char * path, unsigned int maxLen,
    const char * suffix);

/** Create string for the standard ParaVision plugin directories.
    @param path     The buffer for the created string.
    @param maxLen   The maximum length of path. It is guaranteed that not more
                    than this length characters are filled into the string.
    @param type     The plugin type (must not be \c NULL or empty).
    @param plugin   The optional name of a specific plugin. If \a plugin is
                    non-NULL the \a path is filled as
                    \c PVINSTDIR/prog/\<plugins\>/\<type\>/\<plugin\> .
    @return The length of the created string (may be larger than \a maxLen if
            the entire path does not fit into the given space) or -1 on error.
 * @throws PvException if installation directory is not set or XWINNMRHOME 
 * environment variable is is not set.
    @see PvPathMkProg
 */
DLL_INTERFACE int PvPathMkPlugins(char* path, unsigned int maxLen,
    const char* type, const char* plugin);

/** Create string for the ParaVision \c prog/parx directory.
    @param path Buffer for the created string.
    @param maxLen Maximum length of path. It is guaranteed that not more than 
       this length characters are filled into the string.
    @param suffix If \c suffix if not 0 the \c path is filled as
    \c PVINSTDIR/prog/parx/suffix .
    @return length of created string. If is the complete length, even if
      \a path does not contain enough space.
 * @throws PvException if installation directory is not set or XWINNMRHOME 
 * environment variable is is not set.
    @see PvPathMkProg
 */
DLL_INTERFACE int PvPathMkParx(char * path, unsigned int maxLen,
    const char * suffix);

/** Create a string  PVINSTDIR/prog/curdir/USER.
    @param path Buffer to fill in the created file / directory name.
    @param maxLen Maximum length of path. It is guaranteed that not more than 
       this length characters are filled into the string.
    @param suffix If \c suffix if not 0 the \c path is filled as
    \c  PVINSTDIR/prog/curdir/USER/suffix.
    @return length of created string. If is the complete length, even if
      \a path does not contain enough space.
 * @throws PvException if installation directory is not set or XWINNMRHOME 
 * environment variable is is not set. It is also thrown if the directory does
 * not exist and cannot be created.
 */
DLL_INTERFACE int PvPathMkCurdir(char * path, unsigned int maxLen,
    const char * suffix);

/** Create a string  PVINSTDIR/prog/curdir/USER/ParaVision.
    @param path Buffer to fill in the created file / directory name.
    @param maxLen Maximum length of path. It is guaranteed that not more than 
       this length characters are filled into the string.
    @param suffix If \c suffix if not 0 the \c path is filled as
    \c  PVINSTDIR/prog/curdir/USER/ParaVision/suffix.
    @return length of created string. If is the complete length, even if
      \a path does not contain enough space.
 * @throws PvException if installation directory is not set or XWINNMRHOME 
 * environment variable is is not set. It is also thrown if the directory does
 * not exist and cannot be created.
    @see PvPathMkCurdir
 */
DLL_INTERFACE int PvPathMkCurdirPv(char * path, unsigned int maxLen,
    const char * suffix);

/**
 * Convert path to a common format with directory separator  '/'.
 * @param path The path to be converted inline.
 */
DLL_INTERFACE void PvPathConvertToCommon(char * path);

/**
 * Convert path to use OS specific directory separator.
 * @param path The path to be converted inline.
 */
DLL_INTERFACE void PvPathConvertToOs(char *path);

/* gen/src/lib/libPvUtil/Remove.c */

/** Remove recursively content of a directory,
 *  This procedure will recursively traverse a directory tree starting 
 *  at the given <code>dir_path</code> and remove all regular files 
 *  and subdirectories. The starting directory will be left standing, 
 *  but empty. If <code>dir_path</code> is a symbolic link to a directory,
 *  the link will be traversed to clean the designated directory.
 *  @param dir_path Starting directory.
 *  @throws PvException. It is not possible to clean the directory
 *     completely. CAUTION: In this case part of the directory may
 *     already be cleaned.
 */
DLL_INTERFACE void CleanDirectory(const char * dir_path);

/** Remove recursively content of a directory,
 *  This procedure does the same as <code>CleanDirectory</code> but does
 *  not throw an exception.
 *  \see CleanDirectory
 *  @param dir_path Starting directory.
 *  @return Yes if cleaning was possible, No else. CAUTION: In the case 
 *     of No as return value a part of the directory may already be cleaned.
 */

DLL_INTERFACE bool CleanDirectoryNE(const char * dir_path);

/** Remove directory including content.
 *  This procedure will recursively traverse a directory tree starting at 
 *  <code>dir_path</code> and will remove all regular files and 
 *  subdirectories. It will end by removing the starting directory 
 *  <code>dir_path</code>. If the given path is a symbolic link to a directory,
 *  the link itself will be removed, but nothing else will be disturbed.
 *  @param dir_path Starting directory to remove.
 *  @throws PvException. It is not possible to remove the directory
 *     completely. CAUTION: In this case part of the directory may
 *     already be deleted.
 */
DLL_INTERFACE void PvUtilRemoveDirectory(const char * dir_path);

/**
 * Remove the given (empty) directory.
 * @throws PvException on error (not a directory, not empty ...).
 */
DLL_INTERFACE void PvUtilRemoveEmptyDirectory(const char * dir_path);

/** Remove directory including content.
 *  This procedure does the same as <code>PvUtilRemoveDirectory</code> but does
 *  not throw an exception.
 *  \see PvUtilRemoveDirectory
 *  @param dir_path Starting directory to remove.
 *  @return Yes if deletion was possible, No else. CAUTION: In the case of No
 *  parts of the directory may already be deleted.
 */
DLL_INTERFACE bool PvUtilRemoveDirectoryNE(const char * dir_path);

/** Removes the given file path.
 *  @param file_path Path of file to remove.
 *  @throws PvException. If \c file_path is not a valid file or
 *    link or deletion is not possible an exception is raised.
 */
DLL_INTERFACE void RemoveFile(const char * file_path);

/** Removes the given file path.
 *  @see RemoveFile
 *  @param file_path Path of file to remove.
 *  @return Yes if deletion was possible and successful else No..
 */
DLL_INTERFACE bool RemoveFileNE(const char * file_path);

/* gen/src/lib/libPvUtil/ReportExc.c */

class PvException;

/** 
 * Set exception reporter.
 * C Exceptions can be reported using @ref ReportException.
 * A reporter can be registered with this function. If a reporter function
 * is not set the exception is reported to standard error.
 * \see ReportErrMsg
 *  @param new_exc_handler  New exception reporter.
 */
DLL_INTERFACE void SetReportExcHndls(
    void (*new_exc_handler)(const PvException &));

/** 
 * Switch on/off exception error reporting.
 *  \see ReportException
 *  @param enable If \c true  exceptions will be reported.
 *  @return old enabling status of exception error reporting.
 */
DLL_INTERFACE bool SetReportExcEnabled(bool enable);



/** Report a C-Exceptions.
 *  This function reports a C-Exception if the exception error reporting 
 *  is switched on. For reporting the actual message <code>ReportErrMsg</code>
 *  is used.
 *  \see SetReportExcEnabled.
 *  @param ex A ParaVision exception.
 */

DLL_INTERFACE void ReportException(const PvException & ex);

/** 
 * Report an error message.
 * The error message is converted into an exception and then the exception
 * is reported with @ref ReportException.
 * @param msg Error message.
 * @see ReportException.
 */
DLL_INTERFACE void ReportErrMsg(const char * msg);

/* gen/src/lib/libPvUtil/Split.cc */

/**
 * Split the a string into a vector of strings by a separator.
 * @param result The result vector.
 * @param str The string to be split.
 * @param seps The separators (e.g. " \n\t" for whitespace).
 * @param emptyElems Indicates if the list should have empty elements.
 * @param trim Indicates if whitespace should be removed from the string 
 * start and end.
 * @throws PvException if separator is empty string. 
 */
DLL_INTERFACE void PvUtilSplitString(std::vector<std::string> & result, 
    const std::string & str, const std::string & seps, 
    bool emptyElems, bool trim);

/**
 * Split a path into a vector of path elements.
 * @param pathElements The vector of path elements to be filled.
 * @param path The path to be split.
 */
DLL_INTERFACE void PvUtilSplitPath(std::vector<std::string> & pathElements,
    const std::string & path);


/* gen/src/lib/libPvUtil/StoreTime.c */


/** Convert normalized time string into binary time.
    @param ts Pointer to time structure to be set.
    @param normTime  Normalized time in the Iso8601 or  
    <code>hh:mm:ss dd Mon yyyy</code> format. 
    Some older formats from Pv1 and 2 are also allowed.
    @throws PvException on error.
 */
DLL_INTERFACE void TimeConvertAsciiToBin(PvTimeStruct * ts,
    const char * normTime);

/**
 * Set correct time zone by comparing a local time with an absolute time.
 * @param absTime The absolute time.
 * @param localTime the local time.
 * @return a pvtime_t element with a corrected time zone.
 */
DLL_INTERFACE const PvTimeStruct PvUtilDerivePvTimeZone(const PvTimeStruct & absTime,
    const PvTimeStruct & localTime);



/* gen/src/lib/libPvUtil/TextFile.c */

/** Read a text file from disk into a memory buffer.
    This procedure will read a text file from disk into
    a dynamically allocated buffer and will return the
    buffer to the caller containing an image of the text file.
    If the file doesn't exist this is not treated as an
    error, instead a NULL pointer is returned for the
    buffer pointer and a buffer size of zero is returned.
    The carriage returns will be removed - especially against Windoof.
    The allocated buffer must be freed by the caller.
    @param path The path where the file is located.
    @param buf_ptr_ptr A pointer to a buffer which will be allocated by 
    this function.
    @param buf_size_ptr A pointer to an int which stores the length of the 
    filled buffer.
    @return The buffer of 0 on error.
 */
DLL_INTERFACE void *ReadTextFileImage(const char * path,
    void ** buf_ptr_ptr,
    int * buf_size_ptr);

/* gen/src/lib/libPvUtil/transpUtil.c */

/** 
 * Create a dimension order from a transposition.
    An array is create of dimension \c dim containing the indexes in natural
    order. The transposeNdx defined which elements are swapped. The array
    is returned. 
    @param transposeNdx The transpose index.
    @param dim The array dimension.
    @return Dimension order array.
 */
DLL_INTERFACE std::vector<int> PvUtilCreateDimOrder(int transposeNdx, int dim);


/* gen/src/lib/libPvUtil/TstDatPaths.c */

/** Test for raw data file.
    The experiment path \c expnoPath is tested if a raw data file exists.
    If this is the case the path to the raw data is filled into \c dataPath 
    if it is not 0.
    @param dataPath Buffer to be filled with the raw data file path (if not \c 0).
    @param maxLen Maximum allocated size of \a dataPath.
    @param expnoPath Experiment path to be searched for raw data file.
    @return 0 if raw data does not exist or \c expnoPath is not an 
    experiment path else 1.
    @deprecated
 */
DLL_INTERFACE bool TstForRawData(char * dataPath, unsigned int maxLen,
    const char * expnoPath);

/**
 * Test if a raw data file exists.
 * 
 * @param expnoPath An experiment or processing path.
 * @return Indication if a raw data file exists.
 */
DLL_INTERFACE bool TstForRawData(const char * expnoPath);

/** Make an experiment path free.
    An experiment path if freed if it is not free already. This procedure will
    -# delete raw data files,
    -# delete extra processing directories except the first one 
           (if \a removeOtherProcnos is \c Yes),
    -# delete image files,
    -# delete image description files (\c d3proc, \c proc, \c visu_pars).
    -# change permissions of experiment parameter files to writable (\c method,
       \c acqp, ...).
    No warning is given to the user nor is any confirmation requested here.
    @param expnoPath Existing experiment path to be freed.
    @param removeOtherProcnos Indication if other PROCNOs should be removed.
 */
DLL_INTERFACE void MakeExpnoFree(const char * expnoPath, bool removeOtherProcnos);

/** Test if a processing path contains image files.
    @param procnoPath A processing or experiment path. If it is an experiment
    path the processing 1 will be tested.
    @return \c true if image files exist, \c false if not or \c procnoPath is invalid.
 */
DLL_INTERFACE bool TstForImageFile(const char * procnoPath);

/** Test if the processing directory is free.
    A processing directory is considered free if it does not contain any
    image files.
    @see TstIfExpnoFree
    @param procnoPath The processing directory.
    @return \c true if processing directory is free, \c false if it is not free or 
        \c procnoPath is invalid.
 */
DLL_INTERFACE bool TstIfProcnoFree(const char * procnoPath);

/** Make a processing directory free.
    This procedure will delete
    -# all image files
    -# the image description files (\c d3proc, \c proc, \c visu_pars).
    No warning is given to the user, nor is any confirmation
    requested here.
    @param procnoPath The processing path.
 */
DLL_INTERFACE void MakeProcnoFree(const char * procnoPath);

/**
 * Lock or unlock data file so that it is non-writable or writable.
 * @param parfile The path to the data file.
 * @param lock Indication if parameter file is locked.
 * @return 0 if (un)locking was successful, -1 on error, -2 if file does not
 * exist or was already locked / unlocked.
 */
DLL_INTERFACE int PvPathLockDataFile(const char * parfile, bool lock);


/** @} */

#endif /* ! CPROTO */

#endif /* PVUTIL_PVUTIL_H */


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/


