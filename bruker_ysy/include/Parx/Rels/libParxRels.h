/****************************************************************
 *
 * Copyright (c) 1995-2019
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef INCLUDE_FROM_PARX_H
#error This file cannot be included directly. It must be included indirectly by Parx/Parx.h. If you include it directly you may use functions in the wrong context.
#endif

/*--------------------------------------------------------------*
 * prevent multiple includes
 *--------------------------------------------------------------*/
#ifndef PARX_LIBPARXRELS_H
#define PARX_LIBPARXRELS_H


#ifndef InParxOvl
#error This file can only be included in overlays, methods, or PARX toolboxes.
#endif


#ifndef CPROTO

#include <string>
#include <vector>
#include <unordered_map>
#include "bruktyp.h"
#include "Parx/Utils/ParamFiles.hh"
#include "generated/DataPath.hh"

namespace Parx
{
    class PsAttribute;
    class ParxLicenseProvider;
    struct ParxDatasetChangeStatus;
    namespace Relation
    {
        class Context;
    }
}

namespace PvDatabase
{
    class DbValue;
}

/**
 * @file
 */

/**
 *  @ingroup PvParx
 *  @defgroup ParxRelation PVM Parameter Utilities
 *  Functions for handling PVM parameters from methods
 *
 *  @internal
 *  ParxRelation Parx relation utilities (libParxRels): @n
 *  This utility library contains functions that can be used in
 *     overlays, overlay extension, and toolboxes but \b not in
 *     \em normal libraries used outside of overlays, overlay extensions,
 *     or toolboxes.
 */
/*@{*/


#ifdef DLL_INTERFACE
#undef DLL_INTERFACE
#endif

/// DLL interface libParxRels identifiers for Windows.
#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#if defined (libParxRels_INTERNAL)
#define DLL_INTERFACE __declspec(dllexport)
#else
#define DLL_INTERFACE __declspec(dllimport)
#endif
#elif defined(__GNUC__) && __GNUC__ >= 4
#define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#define DLL_INTERFACE
#endif


/* gen/src/lib/Parx/libParxRels/Attributes.c */

/** Return minimum value of an integer parameter / element.
    @param[in] parname Parameter (element) name.
    @return Current minimum value of the parameter (element).
    @exception PvException if the parameter (element) is not of type \c int.
    @see ParxRelsParSetIntMinimum
 */
DLL_INTERFACE int ParxRelsParGetIntMinimum(const char * parname);

/** Set minimum value of an integer parameter (element).
    @param[in] parname Parameter (element) name.
    @param[in] minimum New minimum value of the parameter (element).
    @exception PvException if the parameter (element) is not of type \c int.
    @see ParxRelsParGetIntMinimum
 */
DLL_INTERFACE void ParxRelsParSetIntMinimum(const char * parname, int minimum);

/** Return maximum value of an integer parameter (element).
    @param[in] parname Parameter (element) name.
    @return current Maximum value of the parameter (element).
    @exception PvException if the parameter (element) is not of type \c int.
    @see ParxRelsParSetIntMaximum
 */
DLL_INTERFACE int ParxRelsParGetIntMaximum(const char * parname);

/** Set maximum value of an integer parameter (element).
    @param[in] parname Parameter (element) name.
    @param[in] maximum New maximum value of the parameter (element).
    @exception PvException if the parameter (element) is not of type \c int.
    @see ParxRelsParGetIntMaximum
 */
DLL_INTERFACE void ParxRelsParSetIntMaximum(const char * parname, int maximum);

/** Return minimum value of a 64 bit integer parameter (element).
    @param[in] parname Parameter (element) name.
    @return Current minimum value of the parameter (element).
    @exception PvException if the parameter (element) is not of type \c int64_t.
    @see ParxRelsParSetInt64Minimum
 */
DLL_INTERFACE int64_t ParxRelsParGetInt64Minimum(const char * parname);

/** Set minimum value of a 64 bit integer parameter (element).
    @param[in] parname Parameter (element) name.
    @param[in] minimum New minimum value of the parameter (element).
    @exception PvException if the parameter (element) is not of type \c int64_t.
    @see ParxRelsParGetInt64Minimum
 */
DLL_INTERFACE void ParxRelsParSetInt64Minimum(const char * parname,
    int64_t minimum);

/** Return maximum value of a 64 bit integer parameter (element).
    @param[in] parname Parameter (element) name.
    @return Current maximum value of the parameter (element).
    @exception PvException if the parameter (element) is not of type \c int64_t.
    @see ParxRelsParSetInt64Maximum
 */
DLL_INTERFACE int64_t ParxRelsParGetInt64Maximum(const char * parname);

/** Set maximum value of an 64 bit integer parameter (element).
    @param[in] parname Parameter (element) name.
    @param[in] maximum New maximum value of the parameter (element).
    @exception PvException if the parameter (element) is not of type \c int64_t.
    @see ParxRelsParGetInt64Maximum
 */
DLL_INTERFACE void ParxRelsParSetInt64Maximum(const char * parname,
    int64_t maximum);

/** Return minimum value of a double parameter (element).
    @param[in] parname Parameter (element) name.
    @return Current minimum value of the parameter (element).
    @exception PvException if the parameter (element) is not of type \c double.
    @see ParxRelsParSetDoubleMinimum
 */
DLL_INTERFACE double ParxRelsParGetDoubleMinimum(const char * parname);

/** Set minimum value of a double parameter (element).
    @param[in] parname Parameter (elemenet) name.
    @param[in] minimum New minimum value of the parameter (element).
    @exception PvException if the parameter (element) is not of type \c double.
    @see ParxRelsParGetDoubleMinimum
 */
DLL_INTERFACE void ParxRelsParSetDoubleMinimum(const char * parname,
    double minimum);

/** Return maximum value of a double parameter (element).
    @param[in] parname Parameter (element) name.
    @return Current maximum of the parameter (element).
    @exception PvException if the parameter (element) is not of type \c double.
    @see ParxRelsParSetDoubleMaximum
 */
DLL_INTERFACE double ParxRelsParGetDoubleMaximum(const char * parname);

/** Set maximum value of a double parameter (element).
    @param[in] parname Parameter (element) name.
    @param[in] maximum New maximum value of the parameter (element).
    @exception PvException if the parameter (element) is not of type \c double.
    @see ParxRelsParGetDoubleMaximum
 */
DLL_INTERFACE void ParxRelsParSetDoubleMaximum(const char * parname,
    double maximum);

/** Add a parameter space attribute.
    A parameter space attribute is automatically removed (and deleted)
    if the parameter space is closed.
    @param[in] psAttr The new parameter space attribute.
    @exception PvException if the parameter attribute does already exist.
 */
DLL_INTERFACE void ParxRelsAddPsAttribute(Parx::PsAttribute * psAttr);

/** Remove a parameter space attribute.
 *  @param[in] name The parameter attribute name.
 */

DLL_INTERFACE void ParxRelsRemovePsAttribute(const char * name);

/** Get a registered parameter space attribute.
    @param[in] name The parameter attribute name.
    @return A parameter space attribute object if it is registered using
       \a name or 0 if it is not registered yet.
 */
DLL_INTERFACE Parx::PsAttribute * ParxRelsGetPsAttribute(const char * name);


/* get/src/lib/Parx/libParxRels/Database.c */

/**
 * Get a database attribute and set its value into the given
 * (qualified) parameter.
 *
 * The parameter is set if
 * - a database table is connected to the current parameter space.
 * - the attribute in the category exist, has a value and is of the same
 *   type as the PARX parameter.
 *
 * If a database table is not connected to the parameter space or the attribute
 * is a \c null value this function does not change the parameter and returns.
 *
 * @param paramName The PARX parameter name.
 * @param dbAttrName The name of the database attribute.
 * @return \c true if PARX parameter value is set and \c false if PARX parameter
 * value remain.
 * @throws PvException if the attributes \a dbAttr does not exist in the dataset database tables, the types are not
 * identical, or the PARX parameter cannot be set.
 */
DLL_INTERFACE bool ParxRelsParSetFromDatabase(const char * paramName,
    const char * dbAttrName);

/**
 * Get a database attribute and set its value into the given enumeration
 * (qualified) parameter.
 *
 * The parameter is set if
 * - a database table is connected to the current parameter space.
 * - the attribute has a value and is of the type string.
 * - the database value string is as key in the database enumeration value to
 *   PARX enumeration value mapper.
 *
 * @param paramName The PARX parameter name.
 * @param dbAttrName The name of the database attribute.
 * @param dbToParxEnumMap Map from all database enumeration string to
 *   PARX enumeration strings.
 * @return \c true if PARX parameter value is set and \c false if PARX parameter
 * value remain.
 * @throws PvException if the attributes \a dbAttr does not exist in the
 *   dataset database tables, the type in the
 *   database is not string, the PARX parameter is not an enumeration,
 *   the database value string is not in the \a dbToParxEnumMap,
 *   or the PARX parameter cannot be set.
 * @see ParxRelsParSetFromDatabase
 */
DLL_INTERFACE bool ParxRelsParEnumSetFromDatabase(const char * paramName,
    const char * dbAttrName,
    const std::unordered_map<std::string, std::string> & dbToParxEnumMap);

/**
 * Get a database value for the current dataset.
 * @param dbAttrName The table attribute name.
 * @return A pointer to a database value. The ownership is given to the caller.
 *    The memory is created using \c new. Returns 0 if the
 * @throws PvException if it is not possible get the database value.
 */
DLL_INTERFACE const PvDatabase::DbValue * ParxRelsGetDatabaseValue(
    const char * dbAttrName);

/* gen/src/lib/Parx/libParxRels/GroupFlags.c */

/** Set parameter group to non editable in an editor.
    Setting the group to non-editable affects the edit status of
    all parameters contained in the group. If the group is non-editable the
    parameters contained in the group are non-editable independent of their
    own edit status flag.
    @param[in] list list of group names.
       If the <code>list</code> contains
       a <code>*</code> the edit status of all groups of the
       current overlay is cleared. The group names are separated by
       - spaces
       - commas
       - tabulators
       - slashes.
    @exception PvException if it was not possible to change the
      edit status flags or one of the groups  in <code>list</code>
     does not exist.
    @see ParxRelsMakeGroupNonEditable(const std::vector<std::string> &)
 *  @deprecated since PV 360 V2.1
 */
DLL_INTERFACE void ParxRelsMakeGroupNonEditable(const char * list);

/** Set parameter group to non editable in an editor.
    Setting the group to non-editable affects the edit status of
    all parameters contained in the group. If the group is non-editable the
    parameters contained in the group are non-editable independent of their
    own edit status flag.
    @param list list of group names.
    @exception PvException if it was not possible to change the
      edit status flags or one of the groups  in <code>list</code>
     does not exist.
    @see @ref ParxRelsMakeGroupEditable
    @see @ref ParxRelsGroupIsEditable
 */
DLL_INTERFACE void ParxRelsMakeGroupNonEditable(
    const std::vector<std::string> & list);

/** Set parameter group edit status in an editor.
    Setting the group to editable affects the edit status
    of all parameters contained in the group.
    If the group is editable the parameters contained in the group are
    only editable if their own edit status flag is also set.
    @param[in] list list of group names. If the <code>list</code> contains
       a <code>*</code> the edit status of all groups of the
       current overlay is set. The group names are separated by
       - spaces
       - commas
       - tabulators
       - slashes.
    @exception PvException if it was not possible to change the
      edit status flags or one of the groups in <code>list</code>
     does not exist.
    @see ParxRelsMakeGroupEditable(const std::vector<std::string> &)
 *  @deprecated since PV 360 V2.1
 */
DLL_INTERFACE void ParxRelsMakeGroupEditable(const char * list);

/** Set parameter group edit status in an editor.
    Setting the group to editable affects the edit status
    of all parameters contained in the group.
    If the group is editable the parameters contained in the group are
    only editable if their own edit status flag is also set.
    @param list list of group names. 
    @exception PvException if it was not possible to change the
      edit status flags or one of the groups in <code>list</code>
     does not exist.
    @see @ref ParxRelsMakeGroupNonEditable
    @see @ref ParxRelsGroupIsEditable
 */
DLL_INTERFACE void ParxRelsMakeGroupEditable(const std::vector<std::string> & list);

/** Test if a parameter group is editable in an editor.
    @param[in] groupName The group name.
    @retval false if not editable
    @retval true if editable.
    @exception PvException in \a groupName is not a group.
    @see ParxRelsMakeGroupEditable
    @see ParxRelsMakeGroupNonEditable
 */
DLL_INTERFACE bool ParxRelsGroupIsEditable(const char * groupName);

/** Clear visibility in editor for a parameter group.
    Clearing the group visibility affects the visibility of all parameters.
    If the group is invisible the parameters contained in the group are
    also invisible independent of their own visibility flag.
    @param[in] list list of group names. If the <code>list</code> contains
       a <code>*</code> the visibility of all groups of the
       current overlay is removed. The group names are separated by
       - spaces
       - commas
       - tabulators
       - slashes.
    @exception PvException if it was not possible to change the
      visibility flags or one of the groups in <code>list</code>
     does not exist.
    @see ParxRelsHideGroupInEditor(const std::vector<std::string> &)
 *  @deprecated since PV 360 V2.1
 */
DLL_INTERFACE void ParxRelsHideGroupInEditor(const char * list);

/** Clear visibility in editor for a parameter group.
    Clearing the group visibility affects the visibility of all parameters.
    If the group is invisible the parameters contained in the group are
    also invisible independent of their own visibility flag.
    @param[in] list list of group names. If the <code>list</code> contains
       a <code>*</code> the visibility of all groups of the
       current overlay is removed. The group names are separated by
       - spaces
       - commas
       - tabulators
       - slashes.
    @exception PvException if it was not possible to change the
      visibility flags or one of the groups in <code>list</code>
     does not exist.
    @see @ref ParxRelsShowGroupInEditor
    @see @ref ParxRelsGroupIsVisibleInEditor
 */
DLL_INTERFACE void ParxRelsHideGroupInEditor(const std::vector<std::string> & list);

/** Set visibility in editor for a parameter group.
    Setting the group visibility affects the visibility of all parameters.
    If the group is invisible the parameters contained in the group are
    also invisible independent of their own visibility flag.
    @param[in] list list of group names. If the <code>list</code> contains
       a <code>*</code> the visibility of all groups  of the
       current overlay is set. The group names are separated by
       - spaces
       - commas
       - tabulators
       - slashes.
    @exception PvException if it was not possible to change the
      visibility flags or one of the groups in <code>list</code>
     does not exist.
    @see ParxRelsShowGroupInEditor(const std::vector<std::string> &)
 *  @deprecated since PV 360 V2.1
 */
DLL_INTERFACE void ParxRelsShowGroupInEditor(const char * list);

/** Set visibility in editor for a parameter group.
    Setting the group visibility affects the visibility of all parameters.
    If the group is invisible the parameters contained in the group are
    also invisible independent of their own visibility flag.
    @param list list of group names. 
    @exception PvException if it was not possible to change the
      visibility flags or one of the groups in <code>list</code>
     does not exist.
    @see @ref ParxRelsHideGroupInEditor
    @see @ref ParxRelsGroupIsVisibleInEditor
 */
DLL_INTERFACE void ParxRelsShowGroupInEditor(const std::vector<std::string> & list);

/** Test if a group is visible in an editor.
    @param[in] groupName The group name.
    @retval 0 if not visible in editor
    @retval 1 if visible in editor.
    @exception PvException in \a groupName is not a group.
 */
DLL_INTERFACE bool ParxRelsGroupIsVisibleInEditor(const char * groupName);

/**
 * Indicates if a group is marked as read only.
 * Read-only means that no parameter should be derived from the current
   configuration, only from stored values.
 * @param groupName Group to test read-only flag.
 * @retval true if the group is marked for read-only access.
 * @retval false if the group is marked for changeable.
 */
DLL_INTERFACE bool ParxRelsGroupIsReadOnly(const char * groupName);

/**
 * Indicates if a method is marked as read-only.
 * It is implemented as @ref ParxRelsGroupIsReadOnly for the group
 * \c MethodClass.
 */
#define ParxRelsMethodIsReadOnly() ParxRelsGroupIsReadOnly(::de::bruker::mri::generated::paramgroups::MethodGroup)


/* gen/src/lib/Parx/libParxRels/CopyPar.c */

/** Copy the parameter state flags.
    This routine copies the parameter state flags (editable, visible)
    to another parameter. Values and attributes are not copied.
    The parameters do not need to have the same type.
    @param[in] dst Destination parameter (element) name of the copy operation.
    @param[in] src Source parameter (element) name of the copy operation.
    @exception PvException if the copy is not possible.
 */
DLL_INTERFACE void ParxRelsCopyParState(const char * dst, const char * src);

/**
 * Copy the value of a source to a destination parameter (element).
 * The value and the minimum / maximum attributes are copied. In case of an
 * array parameter the dimension of the destination is changed to the dimension
 * of the source. In case of a dynamic enumeration the
   value set is also copied.
 * @param[in] dst Destination parameter (element) name of the copy operation.
 * @param[in] src Source parameter (element) name of the copy operation.
 * @throws PvException if the copy is not possible, e.g. the source
 *     and destination parameter have different types.
 * @remark This function invalidates all pointers to the destination parameter
 *     \a dst.
 */
DLL_INTERFACE void ParxRelsCopyParValue(const char * dst, const char * src);

/** Copy a source parameter (element) into a destination parameter element.
    This routine copies the value, state flags and attributes (e.g. visibility,
    minimum, maximum, etc.). In case of an array the destination parameter
    will be re-dimensioned if necessary. In case of a dynamic enumeration the
    value set is also copied. If a parameter element is copied the element
    and attribute as well as the parameter state are copied.
    @param[in] dst Destination parameter (element) name of the copy operation.
    @param[in] src Source parameter (element) name of the copy operation.
    @throws PvException if the copy is not possible, e.g. the source
      and destination parameter have different types.
    @remark This function invalidates all pointers to the destination parameter
      \a dst.
 */
DLL_INTERFACE void ParxRelsCopyPar(const char * dst, const char * src);

/** Copy the value set of dynamic enumeration values.
    This procedure expects two dynamic enumeration parameter
    names as input. It will copy the dynamic enumeration value set
    of the src parameter to the destination parameter.
    @param[in] dst Destination dynamic enumeration parameter name.
    @param[in] src Source dynamic enumeration parameter name.
    @exception PvException if the source or destination are not
      are not dynamic enumeration parameters of the same type.
 */
DLL_INTERFACE void ParxRelsCopyDynEnumValueSet(const char * dst,
    const char * src);

/* gen/src/lib/Parx/libParxRels/Dataset.c */

/// Get the current parameter space key,
DLL_INTERFACE const std::string & ParxRelsGetPsKey();

/// Get the modality of the current parameter space.
DLL_INTERFACE PvModalityType ParxRelsGetPsModality();

/** 
 * Get the dataset change status for a dataset group in the parameter space,
 * @param group The name of the group.
    @return \c true if the group is changeable else \c false. Returns also \c false
 * if the group is not known. 
    @throw PvException in case of an error.
 */
DLL_INTERFACE bool ParxRelsGetDatasetGroupChangeStatus(const std::string & group);


/* gen/src/lib/Parx/libParxRels/FileIo.c */

/** Write all parameters of a group to a file.
    Write all parameter with the set flag "store in file" of the given group
    to the parameter file.
    @param[in] group Parameter group name.
    @param[in] file Parameter file name.
    @param[in] mode Parameter write mode.
    @exception PvException if it is not possible to write the
      group to the file.
 */
DLL_INTERFACE void ParxRelsWriteGroup(const char * group,
    const char * file, Parx::Utils::WriteMode mode);

/** Read a parameter group from a file.
    All parameters in the group (and sub groups) are read from the file. If a
    parameter is not stored in the file its value does not change. Parameters
    stored in the file but not member of the group are not read into
    the parameter space. Relations and group post-read functions are not called.
    @param[in] group Parameter group name.
    @param[in] file Parameter file name to read from.
    @param[in] printErrors Indicates if error messages are printed to standard
       error.
    @return The number of parameters read, may be \c if no parameters were read.
    @exception PvException if it is not possible to read the group parameters.
 */
DLL_INTERFACE int ParxRelsReadGroup(const char * group, const char * file,
    bool printErrors);


/** Read a list of parameters or parameter groups from a file.
    All parameters in the list are read from the file. If a parameter is not
    stored in the file its value does not change. Parameters stored in the file
    but not requested in the list are ignored. Relations and overlay main
    routines are not called.
    @param[in] names List of parameters or groups names.
    @param[in] path File name where parameters or groups are read from.
    @param[in] printErrors Indicates if error messages are printed to standard
       error.
    @return The number of parameters read, may be \c 0 if no parameters 
 * were read.
    @exception PvException if it is not possible to read the parameters.
 */
DLL_INTERFACE int ParxRelsReadParList(const std::vector<std::string> & names,
    const char * path, bool printErrors);

/** Read a list of parameters or parameter groups from a file.
    All parameters in the list are read from the file. If a parameter is not
    stored in the file its value does not change. Parameters stored in the file
    but not requested in the list are ignored. Relations and overlay main
    routines are not called.
    @param[in] cnt Number of parameters or groups to read from file.
    @param[in] names List of parameters or groups names
        with <code>cnt</code> entries.
    @param[in] path File name where parameters or groups are read from.
    @param[in] printErrors Indicates if error messages are printed to standard
       error.
    @retval 0 if reading was ok
    @retval -1 if <code>file</code> does not exist.
    @exception PvException if it is not possible to read the parameters.
 */
DLL_INTERFACE int ParxRelsReadParList(int cnt, const char * const * names,
    const char * path, bool printErrors);


/** Write a list of parameters or groups to a file.
    Write all parameter with the set flag "store in file" of the given list
    to the parameter file.
    @param[in] names List of parameters or groups names.
    @param[in] path File name where parameters or groups are written to.
 *  @param[in] mode The mode how to write the parameter files.
    @exception PvException if it is not possible to read the parameters.
 */
DLL_INTERFACE void ParxRelsWriteParList(const std::vector<std::string> & names,
    const char * path, Parx::Utils::WriteMode mode);

/** Write a list of parameters or groups to a file.
    Write all parameter with the set flag "store in file" of the given list
    to the parameter file.
    @param[in] cnt Number of parameters or groups to write.
    @param[in] names List of parameters or groups names
        with <code>cnt</code> entries.
    @param[in] path File name where parameters or groups are written to.
 *  @param[in] mode The mode how to write the parameter files.
    @exception PvException if it is not possible to read the parameters.
 */
DLL_INTERFACE void ParxRelsWriteParList(int cnt, const char * const * names,
    const char * path, Parx::Utils::WriteMode mode);



/* gen/src/lib/Parx/libParxRels/ParArrDims.c */

/** Return number of elements in a given dimension of a parameter array.
    @param[in] parname Parameter name of an array.
    @param[in] ndx Dimension index of the array, beginning with 1.
    @return Number of elements in dimension <code>dim</code>.
    @exception PvException if <code>parname</code> is not an array
      or if <code>dim</code> is not one of the dimensions of this array.
 */
DLL_INTERFACE unsigned int PARX_get_dim(const char * parname,
    unsigned short ndx);

/** Return minimum number of elements in a given dimension of a parameter array.
    @param[in] parname Parameter name of an array.
    @param[in] ndx Dimension index of the array, beginning with 1.
    @return Minimum number of elements in dimension <code>ndx</code>.
    @exception PvException if <code>parname</code> is not an array
      or if <code>dim</code> is not one of the dimensions of this array.
 */
DLL_INTERFACE unsigned int ParxRelsGetMinDim(const char * parname,
    unsigned short ndx);


/** Return maximum number of elements in a given dimension of a parameter array.
    @param[in] parname Parameter name of an array.
    @param[in] ndx Dimension index of the array, beginning with 1.
    @return Maximum number of elements in dimension <code>ndx</code>.
    @exception PvException if <code>parname</code> is not an array
      or if <code>dim</code> is not one of the dimensions of this array.
 */
DLL_INTERFACE unsigned int ParxRelsGetMaxDim(const char * parname,
    unsigned short ndx);

/** Change the element count of dimensions of an array parameter.
    The element counts of all defined dimensions must be specified.
    You can only change the dimensions that are defined as dynamic in
    the parameter definition. The values for the other dimensions must
    match those given in the parameter definition.
    Example:
    - Parameter definition: <code>int parameter a[][128]</code>.
    - Correct usage: <code>PARX_change_dims("a", 10, 128)</code>.
    - Crash: <code>PARX_change_dims("a", 10)</code>. The count of element
        for the second dimension is missing.
    - Invalid: <code>PARX_change_dims("a", 10, 10)</code>. The second
        is fixed.

    @param[in] parname Array parameter name.
    @param[in] ... Number of elements for the array dimensions defined in the
      parameter definition.
    @exception PvException if it was not possible to set the dimension
     element counts or the parameter is not an array.
    @remark
    -# Because of the varargs interface an incorrect number of
    element counts can lead to a crash of the ParaVision program.
    -# This function invalidates all pointers to the parameter.
    .
 * @deprecated since PV 360 V2.1
 * @see ParxRelsChangeDims
 */
DLL_INTERFACE void PARX_change_dims(const char * parname, ...);


/** Change the number of elements of the dimensions of an array parameter.
    You can only change dimensions that are defined as dynamic in
    the parameter definition. The values for the other dimensions must
    match those given in the parameter definition. At the end of the array 
 *  dimensions that should not or cannot be changed can be left out.
    Example:
    - Parameter definition: <code>int parameter a[][128]</code>.
    - Correct usage: <code>ParxRelsChangeDims("a", { 10, 128 })</code>.
 *  - Correct usage: <code>ParxRelsChangeDims("a", { 10 })</code>, same as above.
    - Invalid: <code>ParxRelsChangeDims("a", { 10, 10 })</code>. The second dimension
        is fixed and different.
 *  - Invalid: <code> ParxRelsChangeDims("a", { 10, 128, 15 })</code>. Too many dimensions.
 *  - Parameter definition: <code>int parameter b[128][]</code>.
    - Correct usage: <code>ParxRelsChangeDims("b", { 128, 10 })</code>.
 *  - Invalid: <code>ParxRelsChangeDims("a", { 10 })</code>, first dimension must be
 *  - give since second is dynamic.
    - Invalid: <code>PARX_change_dims("a", { 10, 10 })</code>. The second first
        is fixed.

    @param parname Array parameter name.
    @param[in] Vector of dimension element lengths.
    @exception PvException if it was not possible to set the dimension
     element counts or the parameter is not an array.
    @remark This function invalidates all pointers to the parameter.
    
 
 */
DLL_INTERFACE void ParxRelsChangeDims(const char * parname, const std::vector<int> & dims);

/** Change the minimum number of elements in a parameter dimension.
    @see PARX_change_dims
    @see ParxRelsChangeMaxDims
    @param[in] parname Array parameter name.
    @param[in] ... Minimum number of elements for the array dimensions.
     @exception PvException
      - if in one dimension the new minimum number of array elements
      is greater than the current number.
      - if the parameter is not an array.
      .
 */
DLL_INTERFACE void ParxRelsChangeMinDims(const char * parname, ...);

/** Change the maximum number of elements in a parameter dimension.
    @param[in] parname Array parameter name.
    @param[in] ... Maximum number of elements for the array dimensions.
    @exception PvException
      - if in one dimension the new maximum number of array elements
      is lower than the current number.
      - if the parameter is not an array.
      .
    @see PARX_change_dims
    @see ParxRelsChangeMinDims
 */
DLL_INTERFACE void ParxRelsChangeMaxDims(const char * parname, ...);

/** Change element count for one dimension of an array parameter.
    @see PARX_change_dims
    @param[in] parname Array parameter name.
    @param[in] ndx Dimension index in array beginning with 1.
    @param[in] len Number of elements for the dimension <code>ndx</code>.
    @exception PvException
      - if the dimension does not exist,
      - if the parameter is not an array.
      - if the number of dimension elements is changed for a static fixed
        dimension.
      - if the new dimension element count is not in the range defined by
        the minimum and maximum dimensions.
      .
    @remark This function invalidates all pointer to the parameter.
 */
DLL_INTERFACE void ParxRelsChangeNthDim(const char * parname,
    int ndx, int len);

/** Change minimum element count for one dimension of an array parameter.
    @see ParxRelsChangeMinDims
    @see ParxRelsChangeNthMaxDim
    @param[in] parname Array parameter name.
    @param[in] ndx Dimension index in array beginning with 1.
    @param[in] len Minimum number of elements for the dimension \a ndx.
    @exception PvException
      - if the dimension does not exist,
      - if the parameter is not an array.
      - if the \a ndx dimension is static and the minimum number of elements is
          changed.
      - if the new minimum dimension element count is greater than the current
          dimension element count.
      .
 */
DLL_INTERFACE void ParxRelsChangeNthMinDim(const char * parname,
    int ndx, int len);

/** Change maximum element count for one dimension of an array parameter.
    If the current given parameter array dimension is outside of the new
    maximum it is adapted.
    @see ParxRelsChangeNthMinDim
    @see ParxRelsChangeMaxDims
    @param[in] parname Array parameter name.
    @param[in] ndx Dimension index in array beginning with 1.
    @param[in] len Maximum number of elements for the dimension \a ndx.
    @exception PvException
      - if the dimension does not exist,
      - if the parameter is not an array.
      - if the \a ndx dimension is static and the maximum number of elements is
          changed.
      - if the new minimum dimension element count is lower than the current
          dimension element count.
      .
 */
DLL_INTERFACE void ParxRelsChangeNthMaxDim(const char * parname,
    int ndx, int len);


/** Get the dimension count of a parameter (element).
    @param parElem Name of parameter (element).
    @return The number of dimensions, for a scalar parameter 0 is
         returned.
    @throws PvException if \a parElem is not a valid parameter element.
 */
DLL_INTERFACE int ParxRelsGetDimCount(const char * parElem);

/* gen/src/lib/Parx/libParxRels/ParFlags.c */


/** Set parameter status for editor visibility and file storage.
    @param parlist List of parameter and group names.
 *         If a group name is given in the list all parameters of the
           group change the state.
    @param status The following  states are possible:
    - 0 if parameter is not hidden (\c NOT_HIDDEN).
    - 1 if parameter is not visible in editor (\c HIDE_FOR_EDIT).
    - 2 if parameter is hidden in file (\c HIDE_IN_FILE).
    - 3 if parameter is hidden in file and not visible in editor
        (\c HIDE_ALWAYS).
    .
    @exception PvException if a string in \a parlist is not a parameter or group.
    @see NOT_HIDDEN
    @see HIDE_FOR_EDIT
    @see HIDE_IN_FILE
    @see HIDE_ALWAYS
    @see ParxRelsParIsHidden
    @see ParxRelsHideInEditor
    @see ParxRelsHideInFile
 */
DLL_INTERFACE void PARX_hide_pars(int status, const std::vector<std::string> & parlist);

/** Change state of parameters to "non editable in editor".
    @param[in] pgList List of parameter and group names. The parameter and group
           names are separated by spaces, tabulators, commas, and slash.
           If a group name is given in the list all parameters of the
           group change the state to "non editable in editor".
    @exception PvException if a parameter or group does not exist.
    @see ParxRelsMakeNonEditable(const std::vector<std::string> &)
 *  @deprecated since PV 360 V2.1
 */
DLL_INTERFACE void ParxRelsMakeNonEditable(const char * pgList);

/** Change state of parameters to "non editable in editor".
    @param pgList List of parameter and group names. If a group name is given 
 *         in the list all parameters of the group change the state to 
 *         "non editable in editor".
    @exception PvException if a parameter or group does not exist.
    @see ParxRelsMakeEditable
    @see ParxRelsParIsEditable
 */
DLL_INTERFACE void ParxRelsMakeNonEditable(const std::vector<std::string> & pgList);

/** Change state of parameters to "non visible in editor".
    @param[in] pgList List of parameter and group names. The parameter and group
           names are separated by spaces, tabulators, commas, and slash.
           If a group name is given in the list all parameters of the
           group change the state to "non visible in editor".
    @exception PvException if a parameter or group does not exist.
    @see ParxRelsHideInEditor(const std::vector<std::string> &)
 *  @deprecated since PV 360 V2.1
 */
DLL_INTERFACE void ParxRelsHideInEditor(const char * pgList);

/** Change state of parameters to "non visible in editor".
    @param[in] pgList List of parameter and group names. 
           If a group name is given in the list all parameters of the
           group change the state to "non visible in editor".
    @exception PvException if a parameter or group does not exist.
    @see PARX_hide_pars
    @see ParxRelsShowInEditor
 */
DLL_INTERFACE void ParxRelsHideInEditor(const std::vector<std::string> & pgList);

/** Change state of parameters to "not store in parameter file".
    @param[in] pgList List of parameter and group names. The parameter and group
           names are separated by spaces, tabulators, commas, and slash.
           If a group name is given in the list all parameters of the
           group change the state to "do not store in parameter file".
    @exception PvException if a parameter or group does not exist.
 *  @see ParxRelsHideInFile(const std::vector<std::string> &)
 *  @deprecated since PV 360 V2.1
 */
DLL_INTERFACE void ParxRelsHideInFile(const char * pgList);

/** Change state of parameters to "not store in parameter file".
    @param pgList List of parameter and group names. 
 *         If a group name is given in the list all parameters of the
           group change the state to "do not store in parameter file".
    @exception PvException if a parameter or group does not exist.
    @see PARX_hide_pars
    @see ParxRelsShowInFile
 */
DLL_INTERFACE void ParxRelsHideInFile(const std::vector<std::string> & pgList);

/** Change state of parameters to "editable in editor".
    @param[in] pgList List of parameter and group names. The parameter and group
           names are separated by spaces, tabulators, commas, and slash.
           If a group name is given in the list all parameters of the
           group change the state to "editable in editor".
    @exception PvException if a parameter or group does not exist.
 *  @see ParxRelsMakeEditable(const std::vector<std::string> &)
 *  @deprecated since PV 360 V2.1
 */
DLL_INTERFACE void ParxRelsMakeEditable(const char * pgList);

/** Change state of parameters to "editable in editor".
    @param[in] pgList List of parameter and group names. 
           If a group name is given in the list all parameters of the
           group change the state to "editable in editor".
    @exception PvException if a parameter or group does not exist.
    @see ParxRelsMakeNonEditable
    @see ParxRelsParIsEditable
 */
DLL_INTERFACE void ParxRelsMakeEditable(const std::vector<std::string> & pgList);

/** Change state of parameters to "visible in editor".
    @param[in] pgList List of parameter and group names. The parameter and group
           names are separated by spaces, tabulators, commas, and slash.
           If a group name is given in the list all parameters of the
           group change the state to "visible in editor".
    @exception PvException if a parameter or group does not exist.
 *  @see ParxRelsShowInEditor(const std::vector<std::string> &)
 *  @deprecated since PV 360 V2.1
 */
DLL_INTERFACE void ParxRelsShowInEditor(const char * pgList);

/** Change state of parameters to "visible in editor".
    @param pgList List of parameter and group names. 
           If a group name is given in the list all parameters of the
           group change the state to "visible in editor".
    @exception PvException if a parameter or group does not exist.
    @see PARX_hide_pars
    @see ParxRelsHideInEditor
 */
DLL_INTERFACE void ParxRelsShowInEditor(const std::vector<std::string> & pgList);

/** Change state of parameters to "store in parameter file".
    @param[in] pgList List of parameter and group names. The parameter and group
           names are separated by spaces, tabulators, commas, and slash.
           If a group name is given in the list all parameters of the
           group change the state to "store in parameter file".
    @throws PvException if a parameter or group does not exist.
 *  @see ParxRelsShowInFile(const std::vector<std::string> &)
 *  @deprecated since PV 360 V2.1
 */
DLL_INTERFACE void ParxRelsShowInFile(const char * pgList);

/** Change state of parameters to "store in parameter file".
    @param pgList List of parameter and group names. 
           If a group name is given in the list all parameters of the
           group change the state to "store in parameter file".
    @throws PvException if a parameter or group does not exist.
    @see PARX_hide_pars
    @see ParxRelsHideInFile
 */
DLL_INTERFACE void ParxRelsShowInFile(const std::vector<std::string> & pgList);


/**
 * Set the change flag of parameters.
 * The change flag is used to indicate whether a parameter has changed its
 * value and is used to indicate if parameters need to be stored.
 * @param pgList  List of parameter and group names.
 *   If a group name is given in the list the changed
 *   flag of all parameters is cleared.
 * @throws PvException if a parameter or group does not exist.
 */
DLL_INTERFACE void ParxRelsSetChangeFlag(const std::vector<std::string> & pgList);


/**
 * Clear the change flag of parameters.
 * The change flag is used to indicate whether a parameter has changed its
 * value. For special purposes is may be sensible.
 * @param pgList List of parameter and group names.
 *   If a group name is given in the list the changed
 *   flag of all parameters is cleared.
 * @throws PvException if a parameter or group does not exist.
 */
DLL_INTERFACE void ParxRelsClearChangeFlag(const std::vector<std::string> & pgList);


/** Test if the  parameter (element) is an array.
    @param[in] qPar  Parameter (element) name.
    @retval true if  parameter (element) is an array 
    @retval false if parameter (element) is not an array.
    @exception PvException if given name is not a parameter.
 */
DLL_INTERFACE bool ParxRelsParIsArray(const char * qPar);

/** Get type information for a parameter element.
    @param[in] elemName Parameter element name.
    @param[out] ti The type information for the given parameter element.
    @throws PvException in case of an error.
 */
DLL_INTERFACE void ParxRelsParGetType(const char * elemName,
    ParxTypeIndicator * ti);
/* gen/src/lib/Parx/libParxRels/ParGetSet.c */

/** Get value of a parameter (element).
    @param[in] parname The parameter (element) name.
    @param[out] value Memory to store the parameter (element) value.
    The memory must be allocated by the caller and it must have enough space
    to hold the parameter (element)  value.
    @retval true if parameter has a value. In this case the parameter value is
       copied to \a value.
    @retval false if parameter has no value. In this case \a value is not changed.
    @exception PvException if \a parname is not parameter.
 */
DLL_INTERFACE bool ParxRelsParGetValue(const char * parname, void * value);

/** Check parameter (element) against the defined conditions.
    The conditions are defined in the parameter (element) attributes. They are
    - minimum and maximum values for \c int, \c int_64_t, \c double values.
    - enumeration number value inside of the enumeration range.
    @param[in] parname The parameter element name.
    @retval true if conditions are met.
    @retval false if conditions are not met.
    @exception PvException if \a parname is not parameter.
 */
DLL_INTERFACE bool ParxRelsParCheckValue(const char * parname);

/** Get int value of a parameter (element).
    @param[in] parname The integer parameter (element) name.
    @param[out] value Memory to store the int value.
    @retval Yes if parameter has a value. In this case the integer value
      is copied into \a value.
    @retval false if the parameter has no value. In this case \a value
      is not changed.
    @exception PvException if \a parname is not parameter.
 */
DLL_INTERFACE bool ParxRelsParGetIntValue(const char * parname, int * value);

/** Get int64_t value of a parameter (element).
    @param[in] parname The int parameter (element) name.
    @param[out] value Memory to store the int value. 
    @retval true if parameter has a value. In this case the 64 bit integer value
      is copied into \a value.
    @retval false if the parameter has no value. In this case \a value
      is not changed.
    @exception PvException if \a parname is not parameter.
 */
DLL_INTERFACE bool ParxRelsParGetInt64Value(const char * parname,
    int64_t * value);

/** Get double value of a parameter (element).
    @param[in] parname The double parameter (element) name.
    @param[out] value Memory to store the double value. 
    @retval true if parameter has a value. In this case the double value
      is copied into \a value.
    @retval false if the parameter has no value. In this case \a value
      is not changed.
    @exception PvException if \a parname is not parameter.
 */
DLL_INTERFACE bool ParxRelsParGetDoubleValue(const char * parname,
    double * value);

/** Get string value of a parameter (element).
    @param[in] parname The string parameter (element) name.
    @param[out] value Memory to store the string value. 
    @retval true if parameter has a value. In this case the string value
      is copied into \a value.
    @retval false if the parameter has no value. In this case \a value
      is not changed.
    @exception PvException if \a parname is not parameter.
 */
DLL_INTERFACE bool ParxRelsParGetStringValue(const char * parname,
    char * value);

/**
 * Get string array of a parameter (element) into a string vector.
 * @param parname The string array parameter (element) name.
 * @param value Vector to be filled.
 * @return \c true if parameter has a value, else \c false.
 * @throws PvException if \a parname is not a string array parameter.
 */
DLL_INTERFACE bool ParxRelsParGetStringArrayValue(const char * parname,
    std::vector<std::string> & value);

/** Get \c bool value of a parameter (element).
    @param[in] parname The  \c bool parameter (element) name.
    @param[out] value Memory to store the \c bool value. 
    @retval true if parameter has a value. In this case the \c bool value
      is copied into \a value.
    @retval false if the parameter has no value. In this case \a value
      is not changed.
    @exception PvException if \a parname is not parameter.
 */
DLL_INTERFACE bool ParxRelsParGetYesNoValue(const char * parname,
    YesNo * value);

/** Get enumeration value as integer value of a parameter (element).
    @param[in] parname The enumeration parameter (element) name.
    @param[out] value Memory to store the enumeration integer value. 
    @retval true if parameter has a value. In this case the enumeration integer 
      value is copied into \a value.
    @retval false if the parameter has no value. In this case \a value
      is not changed.
    @exception PvException if \a parname is not parameter.
 */
DLL_INTERFACE bool ParxRelsParGetEnumValueAsInt(const char * parname,
    int * value);

/** Set the value of a parameter (element).
    @param[in] parname The parameter (element) name.
    @param[in] value The new value for the parameter (element).
    @exception PvException if \a parname is not parameter.
 */
DLL_INTERFACE void ParxRelsParSetValue(const char * parname,
    const void * value);
/** Set the value of an int parameter (element).
    @param[in] parname The int parameter (element) name.
    @param[in] value The new value for the int parameter (element).
    @exception PvException if \a parname is not parameter.
 */
DLL_INTERFACE void ParxRelsParSetIntValue(const char * parname, int value);

/** Set the value of an int64_t parameter (element).
    @param[in] parname The int64_t parameter (element) name.
    @param[in] value The new value for the int64_t parameter (element).
    @exception PvException if \a parname is not parameter.
 */
DLL_INTERFACE void ParxRelsParSetInt64Value(const char * parname,
    int64_t value);

/** Set the value of a double parameter (element).
    @param[in] parname The double parameter (element) name.
    @param[in] value The new value for the double parameter (element).
    @exception PvException if \a parname is not parameter.
 */
DLL_INTERFACE void ParxRelsParSetDoubleValue(const char * parname,
    double value);

/** Set the value of a string parameter (element).
    @param[in] parname The string parameter (element) name.
    @param[in] value The new value for the string parameter (element).
    @exception PvException if \a parname is not parameter.
 */
DLL_INTERFACE void ParxRelsParSetStringValue(const char * parname,
    const std::string & value);

/**
 * Set the value of a string array parameter.
 * 
 * This function tries to adapt the length of the 2-dimensional parameter 
 * (element) character array.
 * 
 * @param parname The string array parameter (element) name.
 * @param value The new value for the string parameter.
 * @throws PvException if \a parname is not a parameter or if it is not
 * possible to adapt the length of the parameter character array to the length 
 * of \a value array and the maximum length of all strings in the array.
 */
DLL_INTERFACE void ParxRelsParSetStringArrayValue(const char * parname,
    const std::vector<std::string> & value);


/** Set the value of a \c bool parameter (element).
    @param[in] parname The \c bool parameter (element) name.
    @param[in] value The new value for the \c bool parameter (element).
    @exception PvException if \a parname is not parameter.
 */
DLL_INTERFACE void ParxRelsParSetYesNoValue(const char * parname, YesNo value);

/** Set the an enumeration parameter (element) with an integer value.
    @param[in] parname The enumeration parameter (element) name.
    @param[in] value The new value for the enumeration parameter element as integer.
    @exception PvException if \a parname is not parameter.
 */
DLL_INTERFACE void ParxRelsParSetEnumValueFromInt(const char * parname,
    int value);

/** Get size of a parameter (element) value in bytes.
    @param[in] parname The parameter (element) name.
    @return The size of the parameter element in bytes.
    @exception PvException if \a parname is not parameter.
 */
DLL_INTERFACE int ParxRelsParGetValueSize(const char * parname);

/* gen/src/lib/Parx/libParxRels/ParStates.c */

/** Tests if the parameter exists in the current parameter space.
    @param[in] parname parameter name.
    @retval true if <code>parname</code> is an existing parameter in the current
       parameter space.
    @retval false if the no parameter with name \a parname exists in the current
       parameter space.
 */
DLL_INTERFACE bool ParxRelsParExists(const char * parname);

/** Tests if a parameter is a member of a group.
    The parameter is searched in the group and all sub-groups.
    @param[in] param Parameter name.
    @param[in] group Group name.
    @retval true if parameter <code>param</code> is in group
       \a group.
    @retval false if parameter \a param is not in group \a group.
 */
DLL_INTERFACE bool ParxRelsParParamIsInGroup(const char * param,
    const char * group);

/** Reset all parameters in a group.
    All parameters in the group are cleared, i.e. they do not
    have a value and the attributes are reset to the default when
    initializing the parameters overlay or method.
    Main routines and relations are not called.
    @param[in] group Group to be reset.
    @exception PvException if it is not possible to reset the group.
 */
DLL_INTERFACE void ParxRelsResetGroup(const char * group);

/** Reset a parameter.
    A single parameter is cleared, i.e. it does not
    have a value and the attributes are reset to the default when
    pushing the parameters overlay or method . Relations are not called.
    @param[in] parName parameter name to be reset.
    @exception PvException if it is not possible to reset the parameter.
 */
DLL_INTERFACE void ParxRelsResetPar(const char * parName);

/** Test if a parameter has a value.
 *  This test ignores the in-use flag.
 *  @param parname parameter name to test.
 *  @retval true if parameter has a value,
 * @retval false if parameter has no value.
 */
DLL_INTERFACE bool ParxRelsParHasValue(const char * parname);


/** Test if a parameter has been changed.
    The change flag of a parameter is tested. The change flag is set
    if a parameter is read from a file or the parameter is set by the java
    editor. It is not set if the parameter is set by overlay and method code.
    It is cleared after parameter notifications are set or the parameter is
    stored in a file.
    @param[in] parname The parameter (element) name.
    @retval true if parameter change flag has been set.
    @retval false if parameter change flag is not set.
    @exception PvException if \a parname is not a parameter.
 */
DLL_INTERFACE bool ParxRelsParHasChanged(const char * parname);

/** Tests if a parameter is editable.
    @param[in] parname The parameter (element) name.
    @retval true if parameter is editable
    @retval false if parameter is not editable.
    @exception PvException if \a parname is not a parameter.
    @see ParxRelsMakeNonEditable
    @see ParxRelsMakeEditable
 */
DLL_INTERFACE bool ParxRelsParIsEditable(const char * parname);

/** Tests if a parameter is hidden in file, or visibility.
    @param[in] parname The parameter (element) name.
    @retval 0 if parameter is not hidden (\c NOT_HIDDEN).
    @retval 1 if parameter is not visible in editor (\c HIDE_FOR_EDIT).
    @retval 2 if parameter is hidden in file (\c HIDE_IN_FILE).
    @retval 3 if parameter is hidden in file and not visible in editor
       (\c HIDE_ALWAYS).
    @exception PvException if \a parname is not a parameter.
    @see NOT_HIDDEN
    @see HIDE_FOR_EDIT
    @see HIDE_IN_FILE
    @see HIDE_ALWAYS
    @see PARX_hide_pars
    @see ParxRelsVisibleForEdit
    @see ParxRelsVisibleInFile
 */
DLL_INTERFACE int ParxRelsParIsHidden(const char * parname);

/** Tests if the parameter is a method local parameter.
    @param[in] parname The parameter (element) name.
    @retval true if parameter is locally defined in the current method,
    @retval false if parameter is globally defined.
    @exception PvException if \a parname is not a parameter.
 */
DLL_INTERFACE bool ParxRelsParIsMethodLocal(const char * parname);

/** Tests if a parameter is visible in editor.
    @param[in] parname The parameter (element) name.
    @retval 1 if parameter is visible in editor.
    @retval 0 if parameter is not visible in editor.
    @exception PvException if \a parname is not a parameter.
    @see ParxRelsParIsHidden
    @see ParxRelsHideInEditor
    @see ParxRelsShowInEditor
 */
DLL_INTERFACE bool ParxRelsVisibleForEdit(const char * parname);

/** Tests if the parameter flag "stored in file" is set.
    @param[in] parname The parameter (element) name.
    @retval 1 if parameter flag "store in file" is set.
    @retval 0 if parameter flag "store in file" is not set.
    @exception PvException if \a parname is not a parameter.
    @see ParxRelsParIsHidden
    @see ParxRelsShowInFile
    @see ParxRelsHideInFile
 */
DLL_INTERFACE bool ParxRelsVisibleInFile(const char * parname);

/** Get enum value as int from string representation.
    This routine does not change the parameter <code>parname</code>.
    @param[in] parname enum parameter name.
    @param[in] enumStr string representation of enum value.
    @return integer representation of enum value for the
       given <code>enumStr</code> value. It returns -1 if
       <code>enumStr</code> is not a valid enum string value
       representation.
    @exception PvException if <code>parname</code> is not a parameter
       or not of enum type.
    @see ParxRelsIntToEnumStr

 */
DLL_INTERFACE int ParxRelsEnumStrToInt(const char * parname,
    const char * enumStr);

/** Get string representation of an enum value.
    @param[in] parname The enum parameter (element) name.
    @param[in] val The value for which the string representation is returned.
 * @return The string representation  if the parameter (element) is not
 *     an enumeration.
 * @throws PvException if \a parname is not an enumeration parameter or
 *      the value is out of range.
    @see ParxRelsEnumStrToInt
 */
DLL_INTERFACE const char * ParxRelsIntToEnumStr(const char * parname,
    int val);

/** Get number of possible values for an enum parameter.
    @param[in] parname The enum parameter (element).
    @return The size of the value set for this enum, 0 if \a parname is
    not an enum.
    @exception PvException if \a parname is not a parameter.
 */
DLL_INTERFACE int ParxRelsEnumSize(const char * parname);

/** 
 * Get the enum value set of a given enum parameter.
    @param[in] parname The enum parameter.
 * @param enumValues List of enumeration values set by this function.
    @exception PvException if \a parname is not a parameter.
 */
DLL_INTERFACE void ParxRelsParGetEnumValueSet(const char * parname,
    std::vector<std::string> & enumValues);

/* gen/src/lib/Parx/libParxRels/Relations.c */

/**
 * Call relation routine for a global parameter in a method.
 * If no relation is defined for the parameter this function does nothing.
 * @param parname Parameter for which relations are called.
 * @param forceDefault If the parameter relation is not redirected into a
 *     method, or the parameter is locally defined in the  method, or
 *    \a forceDefault is \c false the relation defined by the \a ParxCallMode
 *     \a METHOD_HIERARCHY is
 *     called. If the relation is redirected into a method, the parameter is
 *     locally defined in the method, and  \a forceDefault is \c true
 *     the relation defined by the \a ParxCallMode  \c MODALITY_HIERARCHY
 *     is called.
 * @see ParxCallMode
 * @exception PvException In case the relation signals an error.
 */
DLL_INTERFACE void ParxRelsParRelations(const char * parname,
    bool forceDefault);

/**
 * Call relation routine for a parameter.
    If no relation is defined for the parameter this function does nothing.
    @param parname Parameter for which relations are called.
    @param callMode Indicates if or which relation is called.
 * @param callContext Relation call context, always empty if relation is not
 * called from another relation.
    @exception PvException In case the relation signals an error.
 */
DLL_INTERFACE void ParxRelsParRelations(const char * parname,
    ParxCallMode callMode, const std::string & callContext = "");

/**
 * Call a group routine (initialization or post-read).
 * @param groupName The group name.
 * @param callMode The call mode.
 * @param routineType The routine type (initialization, post-read).
 * @throws PvException in case of an error.
 */
DLL_INTERFACE void ParxRelsGroupRoutine(const char * groupName,
    ParxCallMode callMode, GroupRoutineType routineType);

/// Get the current relation context object.
DLL_INTERFACE const Parx::Relation::Context * ParxRelsGetRelationContext();

/**
 * Get the primary relation context.
 * A primary relation context is the relation context from the first relation
 * entered. If this relation calls further relation the primary context
 * remains the same. It may be 0 if a it was not a relation that first entered
 * the overlay / method code but an initialization or post-read call.
 * @return The primary relation context, if no context exists 0.
 * @throws PvException if not called inside of relation code or on another error.
 */
DLL_INTERFACE const Parx::Relation::Context * ParxRelsGetPrimaryRelationContext();



/** Add a parameter as source for conflicts.
    The parameter will be removed from the conflicting parameters
    list and added to the conflict causing parameters.
    This is only valid for the current relation call.
    @param[in] context A relation context for which the conflict source is set.
                   If \c 0 the current relation context is used.
    @param[in] parname The new conflict source parameter.
    @see ParxRelsRemoveConflictSource
 */
DLL_INTERFACE void ParxRelsAddConflictSource(
    const Parx::Relation::Context * context, const char * parname);

/** Remove a parameter as source for conflicts.
    The parameter is removed as cause for conflicts. It is not added to
    the list of conflict parameters even if it was configured as conflict
    parameter in the overlay extension. This is only valid for the
    current relation call.
    If the conflict causing list would be empty after the removal the
    removal is not performed. Therefore, if you want to be sure that the
    removal is performed first add a conflict causing parameter and then
    delete the old one.
    @param[in] context A relation context for which the conflict source is set.
                   If \c 0 the current relation context is used.
    @param[in] parname The removed conflict source parameter.
    @see ParxRelsAddConflictSource
 */
DLL_INTERFACE void ParxRelsRemoveConflictSource(
    const Parx::Relation::Context * context, const char * parname);

/* gen/src/lib/Parx/libParxRels/Status.c */

/** Print an information message from a PARX routine.
    The message is propagated to the caller or the gui and printed to the user.
    @param[in] format <code>printf(3)</code> compatible format string.
    @param[in] ... <code>printf(3)</code> compatible argument list.
    @return Number of characters in the formatted message.
 */
DLL_INTERFACE int PARX_sprintf(const char * format, ...)
VA_PRINTF_FORMAT(1, 2);


/** 
 * Print a warning message from a PARX routine.
 * The message is propagated to the caller or the GUI and printed to the user.
 * @param[in] format <code>printf(3)</code> compatible format string.
 * @param[in] ... <code>printf(3)</code> compatible argument list.
 * @return Number of characters in the formatted message.
 */
DLL_INTERFACE int ParxWarningMsg(const char * format, ...)
VA_PRINTF_FORMAT(1, 2);

/** Print an error message from a PARX routine.
    The error message is propagated to the caller or the gui
    and printed to the user.
    @param[in] format <code>printf(3)</code> compatible format string.
    @param[in] ... <code>printf(3)</code> compatible argument list.
    @return Number of characters in the formatted message.
 */
DLL_INTERFACE int ParxErrorMsg(const char * format, ...)
VA_PRINTF_FORMAT(1, 2);

/* gen/src/lib/Parx/libParxRels/methAdmin.c */

/// Unselect the current method and clear the method enum.
DLL_INTERFACE void ParxRelsClearMethod(void);

/*@}*/

/** @internal
    @ingroup ParxRelation
    Update overlay extension associated dynamic enum to reflect
    current status of method list.
    This routine should only be called from overlays which can have
    overlay extensions (i.e. methManag overlay).
    @exception PvException if an error appears.
 */
DLL_INTERFACE void ParxRelsUpdateMethodEnum();

/** @internal
    @ingroup PvParx
    Get the current method name.
    @return The current method name, 0 if no method is active.
    @exception PvException if an error appears.
 */
DLL_INTERFACE std::string ParxRelsGetCurrMethodName();

/** @internal
    @ingroup ParxRelation
    Select a new method (overlay extension).
    This routine should only be called from overlays which can have
    overlay extensions (i.e. methManag overlay).
    @param name Name of new method (overlay extension).
    @exception PvException if an error appears.
 */
DLL_INTERFACE void ParxRelsSelectMethod(const char * name);

/** @internal
    @ingroup ParxRelation
    Set clear mark for methods.
    If the clear mark is set the current method is cleared after the relation
    was performed.
    @param clear If \c true clear mark is set.
 */
DLL_INTERFACE void ParxRelsSetClearMethodMark(bool clear);

/* gen/src/lib/Parx/libParxRels/DebugInfo.c */

/** @internal
    @ingroup ParxRelation
    Debugging routine to retrieve a parameter value.
    In gdb with <code>p dbg_pv("parname")</code> it is possible to
    show the value of a parameter in a parameter space.
    @param[in] parname Parameter (element) name.
    @exception PvException if \a parname is not a parameter (element)..
 */
DLL_INTERFACE const void *dbg_pv(const char * parname);


/** @internal
    @ingroup ParxRelation
    Debugging routine to retrieve a string representation of a parameter value.
    In gdb with <code>p dbg_ps("parname")</code> it is possible to
    show the string value of a parameter in a parameter space.
 * Should only be used in a debugger, not thread safe and creates a one-time
 * memory leak.
    @param[in] parname Parameter (element) name.
    @return String representation of the parameter value. An empty string is
      returned if the parameter has not value or is not defined in the
      current parameter space.
 */
DLL_INTERFACE const char * dbg_ps(const char * parname);

/* gen/src/lib/Parx/libParxRels/dynEnum.c */

/** @internal
    @ingroup ParxRelation
    Bind an enum value set to a dynamic enum parameter.
    This function binds a value set to a dynamic enum parameter (element).
    This procedure does NOT affect the current 'integer' value of the
    dynamic enum parameter. It will only affect the way in which that
    integer value is converted to a string.
    @param[in] parName Name of a dynamic enum parameter (element).
    @param[in] enumVals Array of valid enum values (value set).
           Each entry of the set
           has the format ValueName[\\Display Name], e.g. if using
           the enum value name "FirstValue" and the value display name
           "This is the first value" the entry should be (in C Notation)
           "FirstValue\\\\This is the first value". If a display name is not
           specified the enum value name is used as display name. In this
           case the entry may be  simply "FirstName".
    @param[in] valCnt Count of valid enum values in <code>enumVals</code>.
        A value count of zero is legal and will cause the dynamic enum
        definition to be discarded.
 * @return the number of bound enumeration values.
    @exception PvException if an error appears (e.g. the parameter name is
     not an enum, a value from the value set is invalid, etc.).
    @see ParxRelsParBindEnum(const char * parName, const std::vector<std::string> & enumVals)
 */
DLL_INTERFACE int ParxRelsParBindEnum(const char * parName,
    const char * const * enumVals, int valCnt);


/** @internal
    @ingroup ParxRelation
    Bind an enumeration value set to a dynamic enumeration parameter.
    This function binds a value set to a dynamic enumeration parameter.
    This procedure does NOT affect the current 'integer' value of the
    parameter. It will only affect the way in which that integer value is 
 * converted to a string.
    @param parName Name of a dynamic enumeration parameter.
    @param enumVals Vector of valid enumeration values (value set).
           Each entry of the set
           has the format ValueName[\\Display Name], e.g. if using
           the enum value name "FirstValue" and the value display name
           "This is the first value" the string should be (in C Notation)
           "FirstValue\\\\This is the first value". If a display name is not
           specified the enum value name is used as display name. In this
           case the entry may be  simply "FirstName".
 * @return the number of bound enumeration values.
    @exception PvException if an error appears (e.g. the parameter name is
     not an enumeration, a value from the value set is invalid, etc.).
 */
DLL_INTERFACE int ParxRelsParBindEnum(const char * parName,
    const std::vector<std::string> & enumVals);

/** @internal
    @ingroup ParxRelation
    Bind an enumeration value set to a dynamic enumeration parameter.
    This function binds a value set to a dynamic enumeration parameter.
    This procedure does NOT affect the current 'integer' value of the
    parameter. It will only affect the way in which that integer value is 
 * converted to a string.
    @param parName Name of a dynamic enumeration parameter.
    @param enumVals Vector of valid enumeration values (value set).
           Each entry of the set is a pair of the enumeration value name 
 *         and display name.
 * @return the number of bound enumeration values.
    @exception PvException if an error appears (e.g. the parameter name is
     not an enumeration, a value from the value set is invalid, etc.).
 */
DLL_INTERFACE int ParxRelsParBindEnum(const char * parName,
    const std::vector<std::pair<std::string, std::string>> &enumVals);


/** @internal
    @ingroup ParxRelation
    Returns indication if enum has a dynamic value set
    @param parname Parameter name of a dynamic enum.
    @retval 1 if parameter is dynamic enum and has dynamic value set.
    @retval 0 if parameter is not a dynamic enum or has not a value set.
 */
DLL_INTERFACE int ParxRelsEnumHasDynamicValueSet(const char * parname);

#ifdef METHCOMP

/** Deprecated function name for @ref ParxRelsMakeGroupNonEditable. */
#define ParxRelsMakeClassNonEditable(list) ParxRelsMakeGroupNonEditable(list)

/** Deprecated function for @ref ParxRelsMakeGroupEditable. */
#define ParxRelsMakeClassEditable(list) ParxRelsMakeGroupEditable(list)

/** Deprecated function name for @ref ParxRelsGroupIsEditable. */
#define ParxRelsClassIsEditable(groupName) ParxRelsGroupIsEditable(groupName)

/** Deprecated function name for @ref ParxRelsHideGroupInEditor. */
#define ParxRelsHideClassInEditor(list) ParxRelsHideGroupInEditor(list)

/** Deprecated function name for @ref ParxRelsShowGroupInEditor. */
#define ParxRelsShowClassInEditor(list) ParxRelsShowGroupInEditor(list)

/** Deprecated function name for @ref  ParxRelsGroupIsVisibleInEditor. */
#define ParxRelsClassIsVisibleInEditor(groupName) ParxRelsGroupIsVisibleInEditor(groupName)

/** Deprecated function name for @ref ParxRelsWriteGroup. */
#define ParxRelsWriteClass(group,file,mode) ParxRelsWriteGroup(group,file,mode)

/** Deprecated function name for @ref ParxRelsReadGroup. */
#define ParxRelsReadClass(group,file,printErrors) ParxRelsReadGroup(group,file,printErrors)

/** Deprecated function name for @ref ParxRelsParParamIsInGroup. */
#define ParxRelsParParamIsInClass(param, group) ParxRelsParParamIsInGroup(param,group)

/*** Deprecated function name for @ref ParxRelsResetGroup. */
#define ParxRelsResetClass(group) ParxRelsResetGroup(group)

#endif



#endif

#endif /* ifndef PARX_LIBPARXRELS_H_ */
