/****************************************************************
 *
 * Copyright (c) 1995-2019
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 ****************************************************************/

#ifndef INCLUDE_FROM_PARX_H
#error This file cannot be included directly. It must be included indirectly by Parx/Parx.h. If you include it directly you may use functions in the wrong context.
#endif

/*--------------------------------------------------------------*
 * prevent multiple includes
 *--------------------------------------------------------------*/
#ifndef PARX_LIBPARX_H
#define PARX_LIBPARX_H

#ifdef InParxOvl
#error This file cannot be included into overlays, methods, and PARX toolboxes.
#endif


#ifndef CPROTO

#include <string>
#include <vector>
#include <unordered_set>
#include "PvUtil/DataPathType.h"
#include "Parx/Utils/ParamFiles.hh"
#include "modality.h"

/** 
 *  @ingroup PvParx
 *  @defgroup Functions PARX client interface functions (libParx).
 *  This utility library contains functions that cannot be used in 
 *     overlays, overlay extension, and toolboxes. They can only be used
 *     in normal C and C++ code.
 *  
 *  All functions are implemented in libParxClient. 
 *  @{
 */

#ifdef DLL_INTERFACE
#undef DLL_INTERFACE
#endif

/// DLL interface libParx identifiers for Windows.
#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#if defined (libParxClientBase_INTERNAL)
#define DLL_INTERFACE __declspec(dllexport)
#else
#define DLL_INTERFACE __declspec(dllimport)
#endif
#elif defined (__GNUC__) && __GNUC__ >= 4
#define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#define DLL_INTERFACE
#endif 

/** End of parameter list indicator.
 *  @see @ref PARX_set_all_values
 *  @see @ref PARX_get_all_values
 */
#define PAR_END ((char *)0)

namespace Parx
{
    namespace Utils
    {
        /** Forward declaration of a Parx string array representation. */
        class ParxStringArray;
    }
}


/* gen/src/lib/Parx/libParx/Clear.c */


/** Clear a parameter group.
    All parameter members of the group are cleared, i.e. the value of
    all parameters is set to no value. Array dimensions and flags 
    are not changed.
    @param[in] psid The parameter space identifier.
    @param[in] group The group to be cleared.
    @throws PvException in case of an error.
    @see @ref ParxClearPar
 */
DLL_INTERFACE void ParxClearGroupPars(int psid, const char * group);

/** Clear a simple parameter.
    The parameter value is cleared, i.e. it is set to no value. Array
    dimensions and flags are not changed.
    @param[in] psid The parameter space identifier.
    @param[in] parname The parameter to be cleared.
    @throws PvException in case of an error.
    @remark Implemented in libParx and libParxClient.
 */
DLL_INTERFACE void ParxClearPar(int psid, const char * parname);

/* gen/src/lib/Parx/libParx/Clone.c */

/** Clone a parameter space.
    Clone the content of the source parameter space into the
    destination parameter space. All overlays, parameters, groups, etc 
    present in the source parameter space are copied into the destination
    parameter space.
    @param[in] destId Destination parameter space id.
    @param[in] srcId Source parameter space id.
    @throws PvException on error.. 
 */
DLL_INTERFACE void ParxClonePs(int destId, int srcId);

/** Copy a parameter group to another parameter space.
    It copies the given parameter group to another parameter space. The
    corresponding overlay of the source parameter space must be pushed into
    the destination parameter space before calling this routine. Parameter
    relations and / or main routines are not called.
    @param[in] destId Destination parameter space id.
    @param[in] srcId Source parameter space id.
    @param[in] group Group to be copied.
    @throws PvException on error. 
 */
DLL_INTERFACE void ParxCloneParGroup(int destId, int srcId, const char * group);

/** Copy a parameter to another parameter space.
    It copies the given parameter to another parameter space. The 
    corresponding overlay of the source parameter space must be pushed into
    the destination parameter space before calling this routine. 
    @param[in] destId Destination parameter space id.
    @param[in] srcId Source parameter space id.
    @param[in] parname Parameter to be copied.
    @param[in] callMode Indicates if and which the parameter relations should 
      be called.
    @throws PvException on error. 
 */
DLL_INTERFACE void ParxClonePar(int destId, int srcId, const char * parname,
    ParxCallMode callMode);

/* gen/src/lib/Parx/libParx/Elem.c */

/** Fill a string representation of the parameter (element) value in a buffer.
    This routine creates a string representation of the parameter (element) 
    value, allocates a buffer and stores it into the buffer 
    (<code>stringBuf</code>). The buffer must be freed using 
    <code>PARX_free_encoded_val</code>.
    @param[in] psid Parameter space identifier.
    @param[in] par_name Parameter (element) name.
    @param[in] show_units if <code>Yes</code> the string representation of the
                      units is appended to the parameter value.
    @param[out] stringBuf Pointer to the value buffer that is allocated and set
                     in this function. if <code>*stringBuf != 0</code> a 
                     @ref PARX_free_encoded_val is called on this variable.
    @param[out] len Length of the allocated value buffer.
    @throws PvException on error.
    @see @ref PARX_free_encoded_val
    @remark Implemented in libParx and libParxClient.  The parameters are 
    all coded using a standard format.
 */
DLL_INTERFACE void PARX_malloc_encode_val(int psid, const char * par_name,
    bool show_units, char ** stringBuf, int * len);

/** Free a string buffer allocated by @ref PARX_malloc_encode_val.
    @param stringBuf Pointer to a string buffer allocated by 
                     @ref PARX_malloc_encode_val. The buffer is freed
                     and set to 0.
    @see @ref PARX_malloc_encode_val
    @remark Implemented in libParx and libParxClient.
 */
DLL_INTERFACE void PARX_free_encoded_val(char ** stringBuf);

/** Set a value of a parameter (element) from a string.
    @param psid Parameter space identifier.
    @param parname Parameter (element) name..
    @param parval Parameter value in string format to be set.
    @throws PvException if parameter (element) cannot be set from the given 
 *      string.
    @see @ref PARX_malloc_encode_val
 */
DLL_INTERFACE void PARX_decode_value(int psid, const char * parname,
    const char * parval);


/** Get element type indicator.
    For simple types and arrays of simple types only the type id is returned.
    For structs, enums and struct / enum arrays also the type name is returned.
    @param[in] psid The parameter space id.
    @param[in] name Parameter element name.
    @param[out] ti The type indicator.
    @throws PvException in case of an error.
    @see PARX_TYPE_CHAR PARX_TYPE_INT PARX_TYPE_INT64 PARX_TYPE_DOUBLE 
    @see PARX_TYPE_VOID PARX_TYPE_TIME_T PARX_TYPE_DYNENUM PARX_TYPE_ENUM 
    @see PARX_TYPE_STRUCT PARX_ARRAY_TYPE
 */


DLL_INTERFACE void ParxGetElementType(int psid, const char * name,
    ParxElementType & ti);

/* gen/src/lib/Parx/libParx/Fileio.c */

/** Write group parameter values a file.
    The parameters are written in the ParaVision JCAMP format. 
    All parameters that are member of the given group, have
    values and are not hidden in file will be written. If the file
    already exists it will be overwritten. If \c group is a 
    <code>*</code> all parameters of the current parameter
    space are saved.
    @param[in] psid The parameter space id.
    @param[in] group The parameter group name,
    @param[in] path The destination file name.
    @param[in] mode ParaVision write mode for JCAMP creation.
    @throws PvException in case of an error.
    @see @ref ParxWriteList
    @see @ref ParxWriteModeEnum

    
 */
DLL_INTERFACE void ParxWriteGroup(int psid, const char * group,
    const char * path, Parx::Utils::WriteMode mode);

/** Write a list of parameter values to a file.
    The parameters are written in the ParaVision JCAMP format.
    If the list of parameters contain a group all parameters of the group 
    are written to file.
    @param[in] psid The parameter space id.
    @param[in] names The list of parameter names to be written.
    @param[in] path The destination file name.
    @param[in] mode ParaVision write mode for JCAMP creation.
    @throws PvException in case of error.
    @see @ref ParxWriteGroup
    @see @ref ParxWriteModeEnum
 */
DLL_INTERFACE void ParxWriteList(int psid,
    const std::vector<std::string> & names,
    const char * path, Parx::Utils::WriteMode mode);

/** Read group parameter values from a file.
    Read values and visibility information for parameters in a group from 
    a file in JCAMP format. Array parameters will be re-dimensioned to 
    match the dimensions given in the file. Parameters which are present in the 
    file but are not members of the specified group are skipped. 
    Parameters which are members of the group but 
    are not present in the file are not affected directly.
    Parameters which are hidden for file output but are
    present in the file will be read and this flag will be
    cleared. After the file has been read and the parameter
    \c callRels is \c Yes, the main	routine of the overlay in 
    which the group is defined will be called with the arguments
    <code>main( 2, "read", class );</code>
    As a special case a group name of <code>*</code> is allowed and
    will write all parameters that are present in the 
    parameter space.
    @param[in] psid The parameter space id.
    @param[in] group The parameter group.
    @param[in] path The file path from which parameters are read.
    @param[in] forceReadOnly Indicates if group post-read function is
 *      called with read-only option.
    @return Number of parameters read.
 * @throws PvException in case of error.
    @see @ref ParxReadList
 */
DLL_INTERFACE int ParxReadGroup(int psid, const char * group,
    const char * path, bool forceReadOnly);

/** Read parameter values from a file.
    Read values and visibility information for given parameters from 
    a file in JCAMP format. Group names in the list are expanded and the 
    parameters contained in the group are read. Array parameters will 
    be re-dimensioned to match the dimensions given in the file. 
    Parameters which are present in the file but are not members of the 
    list are skipped silently. Only those parameter are affected that 
    are in the list and in the file.
    Parameters which are hidden for file output but are
    present in the file will be read and this flag will be
    cleared. After the file has been read and the 
    \c callRels is \c Yes the relations of the parameters are called.
    As a special case a group name of <code>*</code> is allowed and
    will write all parameters of the parameter space to the file.
    @param[in] psid The parameter space id.
    @param[in] names The parameters and groups to be read.
    @param[in] path The file path from which parameters are read.
    @param[in] callRels If \c Yes call relations of parameters in \c names
       and call read main routines of groups in \c names. Main routines and
       relations are not called for parameter or groups that are not mentioned
       directly in main (i.e. they are not called for parameters or group that
       are contained in the group of \c names).
    @return number of parameters read.
 * @throws PvException in case of error.
   @see @ref ParxReadList
 */
DLL_INTERFACE int ParxReadList(int psid, const std::vector<std::string> & names,
    const char * path, bool callRels);

/* gen/src/lib/Parx/libParx/Methods.c */

/** Get the stored method attachment.
    @param psid The parameter space id which holds the method.
    @param id The identification of the attachment.
    @return The method attachment of identifier \a id a string. The ownership
 *     is transferred to the caller. The string must be deleted using \c delete[].
 *  @throws PvException if an attachment with the given identifier exists
 *     but it is not possible to create or fill \a buf.
 */
DLL_INTERFACE const char * ParxGetMethodAttachment(int psid, const char * id);

/* /home/anba/devel/HEAD/gen/src/lib/Parx/libParx/Nmrsu.c */
/**
 * Write group parameter values to a file as NMR super user.
 * The parameters are written in the ParaVision JCAMP format. 
 * All parameters that are member of the given group, have
 * values and are not hidden in file will be written. If the file
 * already exists it will be overwritten. If \c group is a 
 * <code>*</code> all parameters of the current parameter
 * space are saved.
 * The file is owned by the NMR super user.
 * @param[in] psid The parameter space id.
 * @param[in] group The parameter group name.
 * @param[in] path The destination file name.
 * @param[in] mode ParaVision write mode for JCAMP creation.
 * @throws PvException on error.
 * @see @ref ParxWriteGroup
 * @see @ref ParxWriteModeEnum 
 */
DLL_INTERFACE void ParxWriteGroupAsNMRSU(int psid, const char * group,
    const char * path, Parx::Utils::WriteMode mode);

/* gen/src/lib/Parx/libParx/Open.c */

/** Create / open a shared parameter space. 
    This function opens a parameter space. It does not push any overlays
    into the parameter space. If the parameter space was already opened
    by another process or part of the program the routine attaches to the
    open parameter space. It does not change already opened parameter 
    spaces.
    @param psKey A parameter space key identifying the parameter space.
 * @param modality Modality for which this parameter space is opened. If
 *      unknown give \c NO_Modality.
    @param created An output parameter indicating if the parameter space
                   was already be opened.
    @return The id of the opened shared parameter space.
 * @throws PvException in case of an error.

 */
DLL_INTERFACE int PARX_open(const char * psKey,
    PvModalityType modality, bool * created);

/** 
 * Create a local parameter space.
 * @param modality The modality for which the parameter space is opened.
 * @return The id of the local parameter space.
 * @throws PvException in case of an error.
 */
DLL_INTERFACE int ParxCreateLocalPs(PvModalityType modality);

/** Close a parameter space.
    This routine closes shared and local parameter spaces.
    @param psid The id of the parameter space to be closed.
    @throws PvException in case of an error.
 */
DLL_INTERFACE void PARX_close(int psid);

/* gen/src/lib/Parx/libParx/Overlay.c */

/** Push an overlay to a parameter space.
    @param psid The parameter space.
    @param ovlname Name of path of overlay. If the @a ovlname is an absolute
    path the path is used (without trailing .so). If it is not an absolute path
    the overlay name is searched in the overlay search list. If the name starts
    with a hash the hash is removed and the overlay is initialized readonly.
    @throws PvException in case of error.
 */
DLL_INTERFACE void PARX_pushc_ovlay(int psid, const char * ovlname);

/* gen/src/lib/Parx/libParx/ParGet.c */

/** Get size of a (qualified) parameter value in bytes.
    This value can be used to allocate a buffer to hold the value.
    @param psid The parameter space.
    @param name The (qualified) parameter name.
    @return Size in bytes of the qualified parameter. 
    @throws PvException in case of an error.
 */
DLL_INTERFACE int PARX_get_size(int psid, const char * name);

/**
 * Get the dimensions of a given parameter.
 * @param psid The parameter space.
 * @param name The parameter name.
 * @param dims Output of dimensions.
 * @throws PvException in case of an error.
 */
DLL_INTERFACE void ParxGetDims(int psid, const char * name,
    std::vector<int> & dims);

/** Get number of elements in an array dimension of a (qualified) parameter.
    @param psid The parameter space.
    @param name The (qualified) parameter name.
    @param nthDim Dimension number beginning by 1.
    @return Number of element in the given dimension.
 * @throws PvException if not an array or \a nthDim dimension does not exist.
 */
DLL_INTERFACE int PARX_get_nth_dim(int psid, const char * name, int nthDim);

/** Get values from a list of parameters in a parameter space.
    @see PARX_set_all_values
    @param psid Parameter space.
    @param ...  A list of pairs consisting of the parameter name
       and a pointer to memory that is able to hold the requested value.
       The memory is filled with the value. The called is responsible
       to allocated the memory. The list is terminated with a \c PAR_END entry.
       Do not use \c NULL_PTR, \c 0, \c NULL.
       For example ,
       <pre>
          int a;
          char b[10];
          PARX_get_all_values(psid, "A", &a, "B", b, PAR_END);
       </pre>
       fills the integer value of parameter <code>A</code> into the variable 
       <code>a</code> and the string value of parameter <code>B</code> 
       into variable <code>b</code>. The last parameter of this call
       must be PAR_END. It should not be \c NULL_PTR, \c 0, \c NULL.

    @return \c true if all parameter have a value, \c false if one
 *     has not a value.
 * @throws PvException in case of an error. 
 */

DLL_INTERFACE bool PARX_get_all_values(int psid, ...)
#ifdef __GNUC__
#if __GNUC__ >= 4
__attribute__ ((sentinel))
#endif
#endif
;

/** Get values from parameters in a parameter space.
    @see @ref PARX_get_all_values
    @param psid Parameter space.
    @param parList An array of parameter structures (\c ParxParamGetStruct). 
       If the \c name element is set on input information about the 
       parameter with the (qualified) name is retrieved. If \c allocateBuf is
       \c Yes the buffer is allocated. If allocateBuf is \c No and the \c buf
       element is not 0 the value of the \c name parameter
       is copied into the buffer. The elements \c hasValue and \c isHidden 
       are filled.
       If the \c name element is 0 the element is not used in retrieving
       parameter information.
    @param cnt Number of element in the \c parList array.
    @throws PvException in case of an error.
    @see @ref ParxParamGetStruct
    @see @ref Parx::Utils::freeParxParamGetStructBuffers
 */
DLL_INTERFACE void ParxGetNamedParameters(int psid,
    ParxParamGetStruct * parList,
    unsigned int cnt);


/** Get value of an integer parameter in a parameter space.
    @param psid Parameter space.
    @param name The (qualified) name of the parameter.
    @param indices Array indices, may be 0. If the \c name is
    qualified or the parameter is not an array the indices array is not used. 
    The indices array must have number of parameter dimensions elements. Each
    index if used as an dimension index. The indices number begin at \c 0.
    @return The integer value of the given parameter.
    @exception PvException If (indexed and qualified) parameter is not of type 
    integer, parameter does not exist, has no value, etc.
 */
DLL_INTERFACE int PARX_get_ivalue(int psid, const char * name,
    const unsigned * indices);

/** Get value of a double parameter in a parameter space.
    @param psid Parameter space.
    @param name The (qualified) name of the parameter.
    @param indices Array indices, may be 0. If the \c name is
    qualified or the parameter is not an array the indices array is not used. 
    The indices array must have number of parameter dimensions elements. Each
    index if used as an dimension index. The indices number begin at \c 0.
    @return The double value of the given (indexed and qualified) parameter.
    @exception PvException If (indexed and qualified) parameter is not of type 
    double, parameter does not exist, has no value, etc.
 */
DLL_INTERFACE double PARX_get_dvalue(int psid, const char * name,
    const unsigned * indices);
/** Get value of an enumeration parameter (element) in a parameter space as integer.
    @param psid Parameter space.
    @param name The (qualified) name of the parameter.
    @param indices Array indices, may be 0. If the \c name is
    qualified or the parameter is not an array the indices array is not used. 
    The indices array must have number of parameter dimensions elements. Each
    index is used as an dimension index. The indices number begin at \c 0.
    @return The enumeration value as integer of the given (indexed)
    parameter (element).
    @exception PvException If parameter (element) is not an enumeration,
    the parameter does not exist, or has no value, etc.
 */
DLL_INTERFACE int PARX_get_evalue(int psid, const char * name,
    const unsigned * indices);

/** Get enumeration parameter value set as name list.
    @param psid Parameter space.
    @param name The (qualified) enumeration parameter name.
    @param enumValues A vector of enumeration value names to be filled by this function.
 * @throws PvException on error.
 */
DLL_INTERFACE void ParxGetEnumValueSet(
    int psid, const char * name, std::vector<std::string> & enumValues);

/** Get number of enumeration values of a parameters.
    @param psid Parameter space.
    @param name The (qualified) enumeration parameter name.
    @return The number of enumeration values for the given parameter.
 * @throws PvException on error, e.g. parameter is not of type enumeration.
 */
DLL_INTERFACE int ParxGetEnumValueCnt(int psid, const char * name);

/** Indicates if parameter has a value. 
    @param psid Parameter space.
    @param name Parameter name.
    @return \c true if parameter has a value.
    @throws PvStdException if parameter \a name is not a parameter.
 */
DLL_INTERFACE bool PARX_has_value(int psid, const char * name);

/** Indicates if parameter exists.
    @param psid Parameter space.
    @param name Parameter name.
    @return \c true if parameter exists.
 */
DLL_INTERFACE bool ParxParamExists(int psid, const char * name);

/** Get a parx string array parameter value.
    @param psid The parameter space.
    @param name The parameter name.
    @return A ParxString array if parameter is a string array and has a value
 *  else 0. The  string array must be deleted using delete by the caller.
 * @throws PvException in case of an error.
    @see @ref Parx::Utils::ParxStringArray
 */
DLL_INTERFACE Parx::Utils::ParxStringArray *
ParxGetStringArray(int psid, const char * name);

/* gen/src/lib/Parx/libParx/ParID.c */

/** Obtain key for a dataset parameter space.
    @param[out] psKey Filled by this function with a valid parameter space key.
    The variable must be allocated with at least
    <code>de_bruker_mri_generated_uid_CStrMaxPsKeyLen</code>
    characters.
 *  @param[out] modality The modality of the dataset.
    @param[in] procnoPath Dataset path.
    @param[in] suffix <code>PS_ACQ_SUFFIX</code> or <code>PS_WORK_SUFFIX</code>.
    @throws PvException if it is not possible to create a parameter space 
    key.
 */
DLL_INTERFACE void ParxPathToPsIdent(char * psKey,
    PvModalityType & modality, const char * procnoPath, int suffix);

/** 
 * Obtain key for a dataset parameter space.
 * @param[out] psKey Filled by this function with a valid parameter space key.
 * The variable must be allocated with at least
 * <code>de_bruker_mri_generated_uid_CStrMaxPsKeyLen</code> characters.
 * @param[out] modality The modality of the dataset.
 * @param[in] prefix Dataset path prefix path component.
 * @param[in] study Dataset path study pathname component.
 * @param[in] expno Dataset path study experiment number component.
 * @param[in] procno Dataset path study processing number component.
 * @param[in] suffix <code>PS_ACQ_SUFFIX</code> or <code>PS_WORK_SUFFIX</code>.
 * @throws PvException if it is not possible to create a parameter space key.
 */
DLL_INTERFACE void ParxPathToPsIdent(char * psKey, PvModalityType & modality,
    const char * prefix, const char * study, int expno, int procno,
    int suffix);

/** Change the dataset key on disk.
    This routine changes the dataset key on disk. It does not affect other
    open parameter spaces using the old dataset key. It creates a new unique 
    dataset key and stored it in the parameters DATASET_KEY, 
    RECO_base_image_uid, VisuUid (only if they have values in the current
    instance of the dataset).
    @param procnoPath path of the dataset in which keys are changed.
    @throws PvStdException on error.
 */
DLL_INTERFACE void ParxChangeDatasetId(const char * procnoPath);

/**
 * Get the dataset path for a dataset parameter space.
 * @param psid The parameter space identifier.
 * @param dsetPath The dataset path buffer to be filled.
 * @param maxLen The maximum length of the dataset path buffer.
 * @param dsetPathType The type of the returned dataset.
 * @param suffix The suffix in the requested dataset path.
 * @throws PvException on error.
 */
DLL_INTERFACE void ParxGetDatasetPath(int psid, char * dsetPath,
    unsigned int maxLen, DataPathType dsetPathType, const char * suffix);

/**
 * Get the dataset path components for a dataset parameter space.
 * @param psid The parameter space identifier.
 * @param prefix The dataset path buffer for the prefix directory. 
 * If \a prefix is \c 0 the prefix is not returned.
 * @param prefixLen The maximum length of the \a prefix buffer.
 * @param study The dataset study name buffer. If \a study is \c 0 the
 * study name is not returned.
 * @param studyLen The maximum length of the \a study buffer.
 * @param expno Pointer to the dataset experiment number. If \a expno is \c 0
 * the experiment number is not returned.
 * @param procno Pointer to the dataset processing number. If \a procno is \c 0
 * the processing number if not returned.
 * @throws PvException on error.
 */
DLL_INTERFACE void ParxGetDatasetPathComponents(int psid,
    char * prefix, unsigned int prefixLen,
    char * study, unsigned int studyLen,
    int * expno, int * procno);

/* gen/src/lib/Parx/libParx/ParSet.c */

/**
 * Set the dataset path into parameter space.
 * @param psid The parameter space.
 * @param procnoPath The processing path for this parameter space.
 * @throws PvException on error.
 */
DLL_INTERFACE void ParxSetDatasetPath(int psid, const char * procnoPath);

/** 
 * Set values of a list of parameters in a parameter space.
 * Set parameter values and perform relations after all parameter are set
 * in order of the parameters in the array \c parValList.
 * @param psid Parameter space.
 * @param parValList Array of (qualified) parameter names and new values.
 * If a name or a new value (\c buf) is \c nullptr the entry is skipped.
 * @throws PvException in case of an error.
 */
DLL_INTERFACE void ParxSetValues(int psid, 
    const std::vector<ParxParamSetStruct> & parValList);

/** Set dimensions element counts of an array parameter.
    After changing the dimensions the parameter relations are called.
    @param psid Parameter space.
    @param name The (qualified) parameter name.
    @param ... Array element counts for every array dimension of
    the (qualified) parameter.
    @remark Implemented in libParx and libParxClient.
    @throws PvException in case of an error.
 */
DLL_INTERFACE void PARX_set_dims(int psid, const char * name, ...);

/** 
 * Set edit flag for a list of parameters.
 * @param psid Parameter space id.
 * 
 * @param list List of parameter and group names. If a group name 
 *          is given in the list all parameters of the group change 
 *          the flag.
 * @param editFlag Indicates if parameter can be edited in editor.
 * @throws PvException in case of error.
 */
DLL_INTERFACE void ParxSetParEditFlag(int psid,
    const std::vector<std::string> & list, bool editFlag);

/** 
 * Set file visible flag for a list of parameters.
 * @param psid Parameter space id.
 * @param list List of parameter and group names. If a group name 
 *          is given in the list all parameters of the group change 
 *          the flag.
 * @param flag Indicates if parameter is saved into a file.
 * @throws PvException in case of an error.
 */
DLL_INTERFACE void ParxSetParVisibleInFileFlag(int psid,
    const std::vector<std::string> & list, bool editFlag);

/** 
 * Set editor visible flag for a list of parameters.
 * @param psid Parameter space id.
 * @param list List of parameter and group names. If a group name 
 *          is given in the list all parameters of the group change 
 *          the flag.
 * @param visFlag Indicates if parameter is visible in an editor.
 * @throws PvException in case of an error.
 */
DLL_INTERFACE void ParxSetParVisibleInEditorFlag(int psid,
    const std::vector<std::string> & list, bool visFlag);

/** 
 * Set edit flag for a list of groups.
 * The parameters in the groups are also affected. Their group edit flag 
 * is set. 
 * If the group is not editable all parameters contained in the group are 
 * also not editable independent of their own edit flag. 
 * @param psid Parameter space id.
 * @param list List of group names. 
 * @param editFlag Indicates if group (parameters) can be edited in editor.
 * @throws PvException in case of an error.
 */
DLL_INTERFACE void ParxSetGroupEditFlag(int psid,
    const std::vector<std::string> & list, bool editFlag);

/** 
 * Set editor visibility for a list of groups.
 * The parameters in the groups are also affected. Their group visible flag 
 * in editor is set. 
 * If the group is not visible in editor all parameters contained 
 * in the group are also not visible in editor independent of their 
 * own editor visible flag. 
 * @param psid Parameter space id.
 * @param list List of group names. 
 * @param visFlag Indicates if group (parameters) are visible in editor.
 * @throws PvException in case of an error.
 */
DLL_INTERFACE void ParxSetGroupVisibleInEditorFlag(int psid,
    const std::vector<std::string> & list, bool visFlag);

/* gen/src/lib/Parx/libParx/Uxnmr.c */

/** Get the parameter space key for a parameter space.
    @param psid The parameter space.
    @return The parameter space key.
 * @throws PvException if psid is not a parameter space.
 */
DLL_INTERFACE const char * ParxGetPsKey(int psid);

/**
 * Get the parameter space modality/.
 * @param psid The parameter space identification.
 * @return  The parameter space modality.
 * @throws PvException if \a psid is not a parameter space.
 */
DLL_INTERFACE PvModalityType ParxGetPsModalityType(int psid);

/** 
 * Get a dataset path from a parameter file containing the CURD parameter group.
 * @param curdFilename The parameter file containing the CURD parameter group. 
 * @return The dataset path from the CURD file.
 * @throws PvException on error, e.g. if the file was not a CURD parameter file.
 */
DLL_INTERFACE std::string ParxGetPathFromCurdFile(
    const char * curdFilename);

/** Open a parameter space for with dataset parameters.
    @param procnoPath Dataset directory.
    @param creator Creator name (e.g. name of process), may be NULL.
    @param acqRels Indicates if acquisition main routines are performed when
       creating and reading dataset parameters.
    @param recoRels Indicates if reconstruction and post processing 
       main routines are performed when reating and reading dataset parameters. 
    @param openForPrototyping Indicates if dataset should be opened for
                   prototyping.
    @param suffix Defines type of parameter space (for editing or acquisition).
    @return A valid parameter identifier.
 * @throws PvException in case of an error.

 */
DLL_INTERFACE int ParxOpenDatasetPs(const char * procnoPath,
    const char * creator,
    bool acqRels,
    bool recoRels,
    bool openForPrototyping,
    int suffix);

/** Clone a parameter space content with a different key.
    If the new key identifies a non existing parameter space the 
    parameter space is created and the content of the source parameter
    space is copies to the new parameter space. If the new key identifies an
    existing parameter space a new reference is returned for this new
    parameter space.
    @param psid Source parameter space id.
    @param creator  Creator name (e.g. name of process), may be NULL.
    @param suffix Defines type of new parameter space 
         (for editing or acquisition).
 * @return Parameter space identifier of the cloned parameter space.
 * @throws PvException on error.
 */
DLL_INTERFACE int ParxCloneDatasetPs(int psid, const char * creator,
    int suffix);

/** Initialize a parameter space with dataset parameters stored on disk.
    @param psid Parameter space to initialize.
    @param path PROCNO path. If \c 0 the parameter is initialized as dataset
                 parameter space but no dataset is loaded.
    @param reset Indicates if the parameter space should be reseted before
                 loading the parameter files.
    @param expnoOnly Indicates if only the subject, study, and experiment 
                 parameter files are read.
    @param acqRels Indicates if the subject, study, experiment read
                 main routines are performed after reading parameter files.
    @param recoRels Indicates if the scan series read main routine 
                 are performed after reading the parameter files. 
    @param openForPrototyping Indicates if dataset should be opened for
                   prototyping.
    @throws PvException on error.
 */
DLL_INTERFACE void ParxInitPsFromDatapath(int psid, const char * path,
    bool reset, bool expnoOnly, bool acqRels, bool recoRels,
    bool openForPrototyping);

/** Initialize a parameter space with dataset parameters stored on disk.
    @param psid Parameter space to initialize.
    @param path PROCNO path. If \c 0 the parameter is initialized as dataset
                 parameter space but no dataset is loaded.
    @param reset Indicates if the parameter space should be reseted before
                 loading the parameter files.
    @param expnoOnly Indicates if only the subject, study, and experiment 
                 parameter files are read.
    @param acqRels Indicates if the subject, study, experiment read
                 main routines are performed after reading parameter files.
    @param recoRels Indicates if the scan series read main routine 
                 are performed after reading the parameter files. 
    @param openForPrototyping Indicates if dataset should be opened for
                   prototyping.
    @param allowAcqVisu If set to \c Yes it allow the loading of a \c visu_pars
                file located in the EXPNO directory if no \c visu_pars file in
                the PROCNO directory exists. This value is only used if
                \a recoRels is \c Yes.
    @throws PvException on error.
 */
DLL_INTERFACE void ParxInitPsFromDatapath(int psid, const char * path,
    bool reset, bool expnoOnly, bool acqRels, bool recoRels,
    bool openForPrototyping, bool allowAcqVisu);



/** 
    Save dataset parameters.
    @param psid The parameter space id.
    @param procnoPath The dataset path where the parameters are saved.
      If \a procnoPath is 0 or an empty string the parameter space path is used.
    @param fileIdSet The set of dataset files that should be stored.
    @throws PvException on error.
 */
DLL_INTERFACE void ParxSaveDataset(int psid, const char * procnoPath,
    const Parx::Utils::DsetGroupIdSetType & fileIdSet);

/**
 *  Get the number of open parameter space.
 * @return number of open parameter space.
 * throws PvException in case of an error.
 */
DLL_INTERFACE int ParxGetOpenPsCnt();


/**
 * Set the warning level for a given type of status messages.
 * 
 * This routine is mainly used for some tests where the status warning messages 
 * should be reported as information messages.
 * @param psid The parameter space for which the messages are set.
 * @param key The message type key.
 * @param level The new status message warning level.
 * @return The old status message warning level.
 */
DLL_INTERFACE ParxStatusMsgLevel ParxSetStatusMessageLevel(int psid,
    ParxStatusMsgKey key, ParxStatusMsgLevel level);

/*@}*/

#endif /* CPROTO */

#endif /* ifndef PARX_LIBPARX_H_ */
