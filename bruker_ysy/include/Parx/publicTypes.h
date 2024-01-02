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

/*--------------------------------------------------------------*
 * prevent multiple includes
 *--------------------------------------------------------------*/
#ifndef PARX_PUBLICTYPES_H
#define PARX_PUBLICTYPES_H


#include <stdint.h>
#include "PvUtil/TimeDefs.h"
#include "generated/ParxDefs.h"
#include "modality.h"

/**
   @ingroup PvParx
   @defgroup CommonTypes Common Parx data structures.
   @{
 */

/** Maximum number of dimensions for an array parameter. */
#define PARDEF_MAX_DIMENSIONS de_bruker_mri_generated_parx_MaxDimensions

/** ParxParamGetStruct output parameter initializer. 
    All elements of \c ParxParamGetStruct that are only for output
    purposes can be initialized using this define.
    @see ParxParamGetStruct
 */
#define INIT_GETSTRUCT_OUTPUT 0, { 0, 0, 0 }, false, false, false, false, 0

/** Data structure to get information about a parameter value and attributes. */
struct ParxParamGetStruct
{
    /** Parameter name. */
    const char * name;

    /** Parameter indicating if buffer should be allocated by the get routine. */
    bool allocateBuf;

    /** Pointer to parameter value. 
        If <code>allocateBuf == true</code> this value will be allocated by
        the get function using the operator <code>new[]</code> and must 
        be deallocated by the caller of the get function using the operator 
        <code>delete[]</code>. In \c libParxCommon the function 
        \c Parx::Common::Utils::freeParxParamGetStructBuffers is defined that
        frees the buffers according to the \a allocate_buf member.
        @see Parx::Common::Utils::freeParxParamGetStructBuffers
     */
    void * buf;

    /** Size of the buffer in bytes. 
     *  It is also set if the buffer is not filled.
     */
    unsigned int bufSize;

    /** Parameter dimensions, all 0 if parameter is not an array. */
    int dims[de_bruker_mri_generated_parx_MaxDimensions];

    /** Indicator if parameter has value. */
    bool hasValue;

    /** Indicator if parameter is editable. */
    bool isEditable;

    /** Indicator if parameter is visible in Editor. */
    bool isVisibleInEditor;

    /** Indicator if parameter is stored in file. */
    bool isVisibleInFile;

    /** The parameter type indicator. 
     * For enumeration parameters (dynamic and static) PARX_TYPE_ENUM
     * is returned.
     */
    unsigned char type;
};

/** Tuple of parameter name and value for setting of parameter value. */
struct ParxParamSetStruct
{
    /** Parameter name. */
    const char * name;

    /** Pointer to new parameter value to be set. */
    const void * buf;
};



/** Maximum length of PARX error and status messages. */
#define PARX_MSG_LENGTH 1024

/// The level of the PARX status message.

enum ParxStatusMsgLevel
{
    /// An information message.
    STATUSMSG_INFO,

    /// A warning message
    STATUSMSG_WARNING,

    /// An error message.
    STATUSMSG_ERROR,
};

/// The status message key for which a log level can be set.

enum ParxStatusMsgKey
{
    /// Range check for parameters.
    STATUSMSGKEY_RANGE_CHECK
};

/// PARX status message type.

struct ParxStatusMsgType
{
    /// The log level for the message.
    ParxStatusMsgLevel level;
    /// The dataset creating the message
    char dataset[PARX_MSG_LENGTH];
    /// The message context.
    char context[PARX_MSG_LENGTH];
    /// The status message.
    char msg[PARX_MSG_LENGTH];
    /// Time of first issue of message.
    PvTimeStruct ts;
    /// Multiplicity of message.
    int multiplicity;
};

/** Defines which part of a parameter is copied. */
enum ParxCopyParamMode
{
    /// Copy parameter value and state.
    COPY_ALL,
    /// Copy only the parameter value.
    COPY_VALUE,
    /// Copy only the parameter state.
    COPY_STATE
};

/** Defines if and which relations are called. */
enum ParxCallMode
{
    /// Call no relations.
    NO_RELATION,

    /**
     * Search relation in order Method, Modality, Overlay and 
     * call the first relation found.
     * No relation is called if the search is not successful.
     */
    METHOD_HIERARCHY,

    /**
     * Search relation in order Modality, Overlay and call the
     * first relation found.
     * No relation is called if the search is not successful.
     */
    MODALITY_HIERARCHY,

    /**
     * If a method relation exists call it.
     * If it does not exist call no relation.
     */
    METHOD_ONLY,

    /**
     * If a modality relation exists call it.
     * If it does not exist call no relation.
     */
    MODALITY_ONLY,

    /**
     * If an overlay relation exists call it.
     * If it does not exist call no relation.
     */
    OVERLAY_ONLY
};

/** Structure to define a parameter copied between parameter spaces. */
struct ParxCopyDef
{
    /** The source parameter or group name. 
        It may be 0 if only relation should be called. */
    const char * srcName = 0;
    /// The destination parameter name, may 0 or empty.
    const char * dstName = 0;
    /// Indicates if and how the relation is called.
    ParxCallMode relsMode = NO_RELATION;
    /// Indicates what should be copied.
    ParxCopyParamMode copyMode = COPY_ALL;
};

/** Category information for method templates. */
enum MethodCategory
{
    /** Wild card for all categories. */
    TEMPL_all,

    /** 
     * User category. 
     * These methods are provided by ParaVision users.
     */
    TEMPL_user,

    /** 
     * BRUKER category. 
     * These methods are provided by BRUKER.
     */
    TEMPL_bruker,

    /**
     * Demo category.
     * These methods are provided by BRUKER for teaching and 
     * demonstration purposes.
     */
    TEMPL_demo,

    /** 
     * Service category.
     * These methods are only visible for the NMR super user.
     */
    TEMPL_service,

    /** 
     * Test category.
     * These methods are only installed if the test suite
     * is also installed.
     */
    TEMPL_test
};


/** Type of overlay. */
enum OverlayType
{
    /** A common base overlay */
    BaseOverlay,

    /// A modality overlay.
    ModalityOverlay,

    /// A method overlay .
    MethodOverlay

};

/** Enumeration to indicate which group routine is called. */
enum GroupRoutineType
{
    /** The group initialization routine is called. */
    GroupInit,
    /** The group parameter pre-read routine is called. */
    GroupPreRead,
    /** The group parameter post-read routine is called. */
    GroupPostRead,
    /** The relation for the group routine, can be called from other relation. */
    GroupRelation
};

/*--------------------------------------------------------------*
 * The following list of constants is used to decode the return
 * value of functions getting the type.
 *--------------------------------------------------------------*/

/// Type indication for void.
#define PARX_TYPE_VOID  ((unsigned char)0x0)

/// Type indication for char or a string.
#define PARX_TYPE_CHAR  ((unsigned char)0x1)

/// Type indication for int types.
#define PARX_TYPE_INT    ((unsigned char)0x2)

/// Type indication for int64 types.
#define PARX_TYPE_INT64   ((unsigned char)0x3)

/// Type indication for double types.
#define PARX_TYPE_DOUBLE ((unsigned char)0x4)

/// Type indication for binary time (time_t) types.
#define PARX_TYPE_TIME_T  ((unsigned char)0x5)

/// Type indication for dynamic enum types.
#define PARX_TYPE_DYNENUM ((unsigned char)0x6)

/// Type indication for static enum types.
#define PARX_TYPE_ENUM  ((unsigned char)0x7)

/// Type indication for struct types.
#define PARX_TYPE_STRUCT ((unsigned char)0x8)

/// Type mask for array types.
#define PARX_ARRAY_TYPE  ((unsigned char)0x80)

/// Type mask for scalar types.
#define PARX_SCALAR_TYPE ((unsigned char)(~(PARX_ARRAY_TYPE)))


/// Type indicator.

struct ParxTypeIndicator
{
    /// Generic type indicator containing the defines above.
    unsigned char type;

    /// Base type name (does not indicate an array).
    const char * name;
};

/** Returns the base type of a PARX parameter or element. */
#define PARX_BASE_TYPE(a) ((a).type&PARX_SCALAR_TYPE)

/** Checks the base type of a PARX parameter or element. */
#define PARX_IS_BASE_TYPE(a,b) (PARX_BASE_TYPE(a)==(b))

/** Checks if PARX parameter or element is an array. */
#define PARX_IS_ARRAY(a) (((a).type&PARX_ARRAY_TYPE)==PARX_ARRAY_TYPE)

/** Checks if the base type of a parameter of element is a char. */
#define PARX_IS_CHAR(a)  PARX_IS_BASE_TYPE(a,PARX_TYPE_CHAR)

/** Checks if the type of a parameter of element is a character array. */
#define PARX_IS_CHAR_ARR(a) (PARX_IS_CHAR(a)&&PARX_IS_ARRAY(a))

/** Checks if a parameter or element is a struct. */
#define PARX_IS_STRUCT(a) PARX_IS_BASE_TYPE(a,PARX_TYPE_STRUCT)

/** Checks if a parameter or element is an enum. */
#define PARX_IS_ENUM(a)   (PARX_IS_STATICENUM(a) || PARX_IS_DYNENUM(a))

/** Checks if a parameter is a dynamic enum. */
#define PARX_IS_DYNENUM(a)  PARX_IS_BASE_TYPE(a,PARX_TYPE_DYNENUM)

/** Checks if a parameter or element is a static enum. */
#define PARX_IS_STATICENUM(a)  PARX_IS_BASE_TYPE(a,PARX_TYPE_ENUM)

/*--------------------------------------------------------------*
 * Constants which may be used in parameter relations...
 * These constants are used to set the visibility attributes of 
 * parameter within the relations...
 *--------------------------------------------------------------*/

/** Parameter is not stored in file and is visible in editors. */
#define NOT_HIDDEN  0
/** Parameter is not visible in editors and stored in file. */
#define HIDE_FOR_EDIT  1
/** Parameter is hidden in file and visible in editor. */
#define HIDE_IN_FILE  2
/** Parameter is hidden in file and not visible in editor. */
#define HIDE_ALWAYS  3

/** Working parameter space mode suffix. */
#define PS_WORK_SUFFIX de_bruker_mri_generated_parx_DsetWorkMode

/** Acquisition parameter space mode suffix. */
#define PS_ACQ_SUFFIX de_bruker_mri_generated_parx_DsetAcqMode


/*--------------------------------------------------------------*
 * CAUTION: Adding defines can only be done
 *    if the corresponding defines in parcomp are also adapted.
 *--------------------------------------------------------------*/

/** Symbol is a parameters. @see PardefSymbol */
#define PARDEF_IS_PAR       0x01
/** Symbol is a group. @see PardefSymbol */
#define PARDEF_IS_GROUP     0x02
/** Symbol is group or parameter. @see PardefSymbol */
#define PardefBaseTypeMask  0x03
/** Symbol is a dummy, only for internal operations. */
#define PARDEF_IS_DUMMY     0x04
/** Symbol is extern to the current overlay. @see PardefSymbol */
#define PARDEF_IS_EXTERN    0x08
/** No PARX symbol. */
#define PARDEF_NO_SYM       0x10
/** Symbol is an alias name for a parameter. */
#define PARDEF_IS_ALIAS_PAR 0x20

/** Symbol is an external parameter. */
#define PARDEF_EXT_PAR  (PARDEF_IS_PAR|PARDEF_IS_EXTERN)

/** Definition of a parameter value as union. */
union ParamValue
{
    /** Self relative pointer to a parameter value instance.
        This is used if the PARDEF_BUFFERED_FLAG is set in a parameter instance.
        In this case the value is not stored in this struct but in a separate
        location. The pointer is allocated by malloc.
     */
    void * as_ptr;

    /** Parameter value of type character. */
    char as_char;

    /** Parameter value of type int. */
    int as_int;

    /** Parameter value of type int64. */
    int64_t as_int64;

    /** Parameter value of type double. */
    double as_double;

    /** Parameter value of type string or string array. */
    char as_char_arr[1];

    /** Parameter value of type int array. */
    int as_int_arr[1];

    /** Parameter value of type int64 array. */
    int64_t as_int64_arr[1];

    /** Parameter value of type double array. */
    double as_double_arr[1];
};


/*@}*/


#endif /* ifndef PARX_PUBLICTYPES_H */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
