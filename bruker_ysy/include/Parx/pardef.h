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

#ifndef PARX_PARDEF_H
#define PARX_PARDEF_H

#ifndef CPROTO

#include <stdint.h>
#include <time.h>
#include "generated/KeyUids.h"
#include "Parx/publicTypes.h"
#include "PvUtil/perfhash.h"
#include "Parx/Base/DLLMacro.h"

/**
   @ingroup PvParx
   @addtogroup Base 
   @{
 */


/*--------------------------------------------------------------*
 * PARX constants.
 *--------------------------------------------------------------*/

/** PARX version number.
    The version number is compiled into every overlay or overlay
    extension and the PARX libraries. When loading an overlay or
    overlay extension the versions in the PARX library and the loaded 
    object are compared. If they are not equal the loading is 
    aborted. */
#define	PARX_VERSION		20190307


/** Constant for an enumeration out of range value. */
#define PARDEF_OUTOFRANGE_STRING "Out-Of-Range-"

/** Registered standard overlay array. */
#define PARDEF_REG_OVL_ARR { "curd", "dataset", "visu", "generic", "methTest" }


/*--------------------------------------------------------------*
 * Macros for operations on parameters.
 *--------------------------------------------------------------*/
/** Indicates if a bit is set in a numeric value. */
#define PARX_BITS_ARE_SET(a,b)	(((a)&(b))==(b))

/** Indicates if a PARX flag is set. */
#define PARX_FLAGS_ARE_SET(a,b)	PARX_BITS_ARE_SET((a)->flags,b)

/** Checks if a parameter or element is a string. */
#define PARX_IS_STRING(a)	(PARX_IS_CHAR_ARR(a)&&((a).dims[1]==0))

/** Checks if a parameter value is buffered. */
#define PARX_IS_BUFFERED(a)	(PARX_IS_STRUCT(a)||PARX_IS_ARRAY(a)||PARX_IS_BASE_TYPE(a,PARX_TYPE_TIME_T))

/*--------------------------------------------------------------*
 * These are the flags contained in the parameter space struct
 *--------------------------------------------------------------*/

/** A notification about a parameter must be sent to a listener. */
#define PARDEF_NOTIFY_FLAG   0x02 

/*--------------------------------------------------------------*
 * These are the flags contained in the parameter definitions
 *--------------------------------------------------------------*/

/** Flag to indicate if a parameter has a value. */
#define PARDEF_ASSIGNED_FLAG    0x0001	

/** Flag to indicate if group or parameter is not editable. */
#define PARDEF_NOEDIT_FLAG      0x0002	

/** Flag to indicate if group or parameter is invisible in editor. */
#define PARDEF_EDHID_FLAG       0x0004	

/** Flag to indicate if parameter is not stored in a file. */
#define PARDEF_FIOHID_FLAG      0x0008	

/** Flag to indicate if group or parameter is read-only. */
#define PARDEF_READ_ONLY        0x0010

/** Flag to indicate if parameter needs a separate value buffer. */
#define PARDEF_BUFFERED_FLAG    0x0020	

/** Flag to indicate if a group to which parameter belongs is not editable. */
#define PARDEF_GRP_NOEDIT_FLAG  0x0040 

/** Flag to indicate if a group to which parameter belongs is invisible. */
#define PARDEF_GRP_EDHID_FLAG   0x0080 

/** Flag to indicate if the  dataset status makes this non editable. */
#define PARDEF_DSET_NOEDIT_FLAG 0x0100

/** Flag to the stored parameter visibility flag, fixed in definition. */
#define PARDEF_FIOVISHID_FLAG   0x0200

/*--------------------------------------------------------------*
 * These are the flags contained in the group definitions
 * Definitions here are extensions to the parameter definitions.
 *--------------------------------------------------------------*/
/* PARDEF_NOEDIT_FLAG see parameter definition. */
/* PARDEF_EDHID_FLAG see parameter definition. */
/* PARDEF_READ_ONLY see parameter definition. */

/** Flag to indicate that a group has no sub groups. */
#define PARDEF_LEAF_GRP_FLAG   0x1000 /* toplevel or leaf group ? */



/** Perform test suite acquisition tests on method. */
#define TESTSUITE_ACQ      0x01
/** Perform test suite GOPTST tests on method. */
#define TESTSUITE_GOPTST   0x02
/** Perform test suite gradient duty cycle tests on method. */
#define TESTSUITE_GRADDUTY 0x04
/** Perform test suite load and skip tests on method. */
#define TESTSUITE_LOADSKIP 0x08


/*--------------------------------------------------------------*
 * Symbol macros.
 *--------------------------------------------------------------*/

/** Symbol is an external group. */
#define PARDEF_EXT_GROUP    (PARDEF_IS_GROUP|PARDEF_IS_EXTERN)

/** Test if symbol or group member type is a group or parameter. */
#define PardefBaseObjType(t)	((t)&PardefBaseTypeMask)
/** Test if symbol or group member type is a parameter. */
#define PardefBaseTypeIsPar(t)	(((t)&PardefBaseTypeMask)==PARDEF_IS_PAR)
/** Test if symbol or group member type is a group. */
#define PardefBaseTypeIsGroup(t) (((t)&PardefBaseTypeMask)==PARDEF_IS_GROUP)
/** Test if symbol or group member type is extern. */
#define PardefIsExtern(t)	(((t)&PARDEF_IS_EXTERN)==PARDEF_IS_EXTERN)

/** Test if symbol or group member type is a local group. */
#define PardefIsLocalGroup(t)	((t)==PARDEF_IS_GROUP)
/** Test if symbol or group member type is an external group. */
#define PardefIsExternGroup(t)  ((t) == PARDEF_EXT_GROUP)

/** Test if symbol is a local parameter. */
#define PardefIsLocalPar(t)     ((t)==PARDEF_IS_PAR)
/** Test if symbol is a parameter alias definition.*/
#define PardefIsAliasPar(t)     ((t)==PARDEF_IS_ALIAS_PAR)
/** Test if symbol or group member type is an external parameter. */
#define PardefIsExternPar(t)    ((t)==PARDEF_EXT_PAR)

/// Get the next lower modulo value.
#define PARDEF_CEIL_MODULO(x,b)  ( ((x) + (b) - 1) - ((x) + (b) - 1)%(b) )

/** PARX symbol (parameter or group). */
struct PardefSymbol
{
    /** Type of symbol. */
    unsigned char type;

    /** Symbol index.
        For local parameters it is the index into the local parameter array.
        For groups it is the index into the local groups array.
        For external parameters it has no meaning. */
    unsigned int ndx;

    /** Symbol name. */
    const char * name;
};

/** Type for parameter members. */
struct PardefMember
{
    /** Parameter symbol definition for the member */
    PardefSymbol sym;

    /** Registered overlay index for the external parameter / . */
    int ov_ndx;
    
    /** The overlay type for the external parameter / group. */
    OverlayType ovType;
    
    /** 
     * Indicates if this member is the base group to the group which contains 
     * the member.
     */
    bool baseGroup;
};

/** 
 * Function type for group initialization and post-read functions. 
 * @param readOnly Indicates if function is called in read-only mode.
 */
typedef void (* PARDEF_GROUP_FCN)(bool readOnly);

/** Definition of a parameter group functions. */
struct PardefGroupFcn
{
    /** The function name. */
    const char * name;
    /** Pointer to the group function. */
    PARDEF_GROUP_FCN fcn;
};

/** Parameter group definition. */
struct PardefGroup
{
    /** Compile time groups flags */
    unsigned short flags;

    /** Number of group members. */
    unsigned short member_cnt;

    /** Group name. */
    const char * name;

    /** Group display name. */
    const char * displ_name;

    /** Group short description (max  80 characters). */
    const char * short_descr;

    /** Group long description. */
    const char * long_descr;

    /** Index to instance flags in group flags array or overlay (extension). */
    unsigned int group_nr;

    /** Overlay type of group. */
    OverlayType ovType;

     /** Group initialization function. */
    PardefGroupFcn initFcn;
    
    /** The group pre-read function. */
    PardefGroupFcn preReadFcn;

    /** Parameter post-read function. */
    PardefGroupFcn postReadFcn;
    
    /** Group relation function. */
    PardefGroupFcn relsFcn;
    
   /** Array of group members. */
    const PardefMember * members;
};


/*--------------------------------------------------------------*
 * The parameter template definitions.
 *--------------------------------------------------------------*/

/*--------------------------------------------------------------*
 * These are the flags contained in the attribute definitions
 *--------------------------------------------------------------*/
/** Definition indicating to set out of range value to minimum. */
#define PARDEF_MIN_ATTR_NEAREST_LIMIT 0x1
/** Definition indicating to set out of range value to maximum. */
#define PARDEF_MAX_ATTR_NEAREST_LIMIT 0x2

/** Identifier for int attributes, used for debugging. */
#define PARDEF_INT_ATTR_IDENT 0xabd1u
/** Identifier for int64_t attributes, used for debugging. */
#define PARDEF_INT64_ATTR_IDENT 0xabd2u
/** Identifier for double attributes, used for debugging. */
#define PARDEF_DOUBLE_ATTR_IDENT 0xabd3u
/** Identifier for struct attributes, used for debugging. */
#define PARDEF_STRUCT_ATTR_IDENT 0xabd4u

/** The attribute definition for int parameter / element values. */
struct PardefIntAttr
{
    /** Identification of integer attribute INT_ATTR_IDENT. */
    unsigned short ident;
    
    /** Minimum value for parameter or element. */
    int min;

    /** Maximum value for parameter or element. */
    int max;

    /** Flags describing actions when out of range values are detected. */
    unsigned short flags;
};

/** The attribute definition for int64_t parameter / element values. */
struct PardefInt64Attr
{
    /** Identification of integer attribute INT64_ATTR_IDENT. */
    unsigned short ident;
    
    /** Minimum value for parameter or element. */
    int64_t min;

    /** Maximum value for parameter or element. */
    int64_t max;

    /** Flags describing actions when out of range values are detected. */
    unsigned short flags;
};

/** The attribute definition for double parameter / element values. */
struct PardefDoubleAttr
{
    /** Identification of integer attribute DOUBLE_ATTR_IDENT. */
    unsigned short ident;
    
    /** Minimum value for parameter or element. */
    double min;

    /** Maximum value for parameter or element. */
    double max;

    /** Flags describing actions when out of range values are detected. */
    unsigned short flags;
};

union PardefAttribute;

/** The attribute definition for struct parameter / element values. */
struct PardefStructAttr
{
    /** Identification of integer attribute STRUCT_ATTR_IDENT. */
    unsigned short ident;
    
    /** Pointer to an array of element attributes. */
    PardefAttribute ** elem_attributes;
};

/** Union containing the different parameter / element attribute definition. */
union PardefAttribute
{
    /** Int parameter / element attribute definition. */
    PardefIntAttr intAttr;

    /** Int64 parameter / element attribute definition. */
    PardefInt64Attr int64Attr;

    /** Double parameter / element attribute definition. */
    PardefDoubleAttr doubleAttr;

    /** Struct parameter / element attribute definition. */
    PardefStructAttr structAttr;
};

/** Definition of dimension mode. */
enum PardefDimsMode
{
    /** Dimension is static. */
    DIM_STATIC,

    /** Dimension is variable but cannot be changed in gui directly. */
    DIM_VARIABLE,

    /** Dimension is variable and can be changed in gui directly. */
    DIM_GUI_VARIABLE
};

/** Definition of the GUI widget type. */
enum PardefGuiWidgetType
{
    /** A checkbox is used to display the parameter. */
    GUI_CHECKBOX,

    /** A combobox is used to display the parameter. */
    GUI_COMBOBOX,

    /** A push button is used to display the parameter. */
    GUI_PUSHBUTTON,

    /** A radio button is used to display the parameter. */
    GUI_RADIOBUTTONS,

    /** A slider is used to display the parameter. */
    GUI_SLIDER,

    /** A spinner is used to display the parameter. */
    GUI_SPINNER,

    /** A text field is used to display the parameter. */
    GUI_TEXTFIELD,

    /** A gui default element. This is determined from the GUI. */
    GUI_DEFAULT
};

/// The GUI style for parameter editor elements.

enum PardefGuiStyleType
{
    /** Inline style for arrays. */
    GUI_STYLE_INLINE_ARRAY,

    /** Default style. */
    GUI_STYLE_DEFAULT
};

/** Type definition for parameter relation functions. */
typedef void (*PARDEF_RELS_FCN)(void);

/// Structure to defined a relation function.  

struct PardefRels
{
    /** Relation name. */
    const char * name;

    /** Pointer to the parameter relation function. */
    PARDEF_RELS_FCN fcn;
};

/** Parameter template definition. */
struct PardefParTemp
{
    /** Parameter index in overlay. */
    unsigned int ndx;

    /** Compile time dimensions for arrays. */
    int dims[PARDEF_MAX_DIMENSIONS];

    /** Compile time dimension range. */
    int dimsRange[2][PARDEF_MAX_DIMENSIONS];

    /** Indicates if range element can only be set once. */
    bool dimsRangeOnce[2][PARDEF_MAX_DIMENSIONS];

    /** Compile indication if array dimension can be changed. */
    PardefDimsMode dimsVariable[PARDEF_MAX_DIMENSIONS];

    /** Compile time parameter flags. */
    unsigned short flags;

    /** Parameter type indicator. */
    unsigned char type;

    /** Index in type array. */
    const void * type_def;

    /** Type checksum. */
    uint32_t typeChecksum;
    
    /** Parameter name. */
    const char * name;

    /** Display format. */
    const char * format;

    /** File format. */
    const char * file_format;

    /** Units definition. */
    const char * units;

    /** Display name. */
    const char * displ_name;

    /** Short description (max  80 characters). */
    const char * short_descr;

    /** Long description. */
    const char * long_descr;

    /** Overlay type. */
    OverlayType ovType;

    /** The default gui widget type. */
    PardefGuiWidgetType gui_widget;

    /** The default gui style. */
    PardefGuiStyleType gui_style;

    /** Index of parameter attributes in attributes pool. */
    const PardefAttribute * attr;

    /** Definition of relation. */
    PardefRels relation;
    
};

/*--------------------------------------------------------------*
 * The parameter instance definitions.
 *--------------------------------------------------------------*/

/*--------------------------------------------------------------*
 * - the ENUM definition structs
 *--------------------------------------------------------------*/

/** The enum type definition. */
struct PardefEnumValueDef
{
    /** Name of enum value. */
    const char * name;

    /** Alias name of enum value. */
    const char * alias;

    /** Display name of enum value. */
    const char * displ_name;
};

/** The dynamic enum type definition. */
struct PardefDynEnumValueDef
{
    /** Name pointer allocated by malloc. */
    char * name;

    /** Alias name pointer allocated by malloc. */
    char * alias;

    /** Display name pointer allocated by malloc. */
    char * displ_name;
};

/** Indication if enumeration is a static. */
#define PARDEF_ENUM_IS_BOOLEAN   0x1

/** The enumeration type definition. */
struct PardefEnum
{
    /** Name of enum type, only valid for static enums. */
    const char * name;

    /** Enum Flags. */
    unsigned short flags;

    /** Number of values in value set. */
    unsigned short value_cnt;

    /** Array of values for static enumeration. */
    const PardefEnumValueDef * values;

    /** Array of values for dynamic enumeration. */
    PardefDynEnumValueDef * dynvalues;
};

/*--------------------------------------------------------------*
 * the STRUCT definition struct
 *--------------------------------------------------------------*/

/** Parameter element definition.
 *  This definition hold information about qualified parameter element 
 *  instances. The qualification may also be empty, 
 *  e.g. the complete parameter instance information is stored here.
 */
struct PardefElement
{
    /** Parameter element dimensions. */
    int dims[PARDEF_MAX_DIMENSIONS];

    /** Parameter element dimension range. */
    int dimsRange[2][PARDEF_MAX_DIMENSIONS];

    /** Indicates if range element can only be set once. */
    bool dimsRangeOnce[2][PARDEF_MAX_DIMENSIONS];

    /** Compile indication if array dimension can be changed. */
    PardefDimsMode dimsVariable[PARDEF_MAX_DIMENSIONS];

    /** Nesting level of element. 
        I.e. in which level defined from the parameter is the element.
        The parameter itself has nesting level 1, every array dimension and struct
        element adds a nesting level, e.g.
        p.e has nesting level 1.
        p.s has nesting level 1.
        p.s.q has nesting level 2.
        pa[1] has nesting level 1.
     */
    char nesting;

    /** Parameter type indicator. */
    unsigned char type;

    /** Index to parameter enum / struct type definition. */
    const void * type_def;

    /** Parameter name string. */
    const char * name;

    /** Parameter format string. */
    const char * format;

    /** File format string. */
    const char * file_format;

    /** Units string. */
    const char * units;

    /** Display name string. */
    const char * displ_name;

    /** Short description string. */
    const char * short_descr;

    /** Long description string. */
    const char * long_descr;

    /** The default gui widget type. */
    PardefGuiWidgetType gui_widget;

    /** The default gui style. */
    PardefGuiStyleType gui_style;

    /** Attributes definitions. */
    const PardefAttribute * attr_templ;

    /** Element index in struct array. */
    unsigned int elem_ndx;

    /** Value offset in bytes from beginning of parameter. */
    unsigned int val_offset;

    /** Parameter attribute. */
    PardefAttribute * attribute;
};

/** Parameter struct definition. */
struct PardefStruct
{
    /** Name of the struct type. */
    const char * name;
    /** Size of struct in bytes. */
    unsigned int size;
    /** Number of struct elements. */
    unsigned short elem_cnt;
    /** Array of struct elements. */
    const PardefElement * elems;
};


namespace Parx
{
    namespace Relation
    {
        class Context;
    }
}

/** 
 * Type definition for a parameter listener function. 
 * @param qualName qualified name of parameter for which the listener
 *        was called.
 */
typedef void (*PARDEF_RELS_LISTENER_FCN)(const Parx::Relation::Context *);

/// Structure to define a listener function.

struct PardefListener
{
    /** Listener name. */
    const char * name;

    /** Pointer to listener function. */
    PARDEF_RELS_LISTENER_FCN fcn;
};

/** Parameter listener template definition. */
struct PardefListenerTemp
{
    /** External parameter definition. */
    PardefMember extpar;

    /** Definition of listener function. */
    PardefListener listener;
};

/** External parameter redefinition relation function. */
struct PardefRelsRedef
{
    /** External parameter definition. */
    PardefMember extpar;

    /** Relation information. */
    PardefRels relation;
};

/** Type to hold information about embedded attachments in a method. */
struct PardefAttachment
{
    /** Attachment identification. */
    const char * id;

    /** Attachment byte buffer. */
    const unsigned char * buffer;

    /** Size attachment in bytes. */
    unsigned int size;
};

/** Type for the hashing function. */
typedef ub4(* PHASH_FCN)(const char *);

/** Common overlay attributes. */
struct PardefOvCommonTemp
{
    /** Number of symbols. */
    unsigned int symCnt;

    /** Array of symbols. */
    const PardefSymbol * symArr;

    /** Number of parameters. */
    unsigned int parCnt;

    /** Array of parameter templates. */
    const PardefParTemp * partArr;

    /** Number of parameter groups. */
    unsigned int groupCnt;

    /** Array for parameter groups. */
    const PardefGroup * groupArr;

    /** Array of defined parameter listener functions. */
    const PardefListenerTemp * relsListeners;

    /** Perfect symbol hashing function for the symbols. */
    PHASH_FCN phash;
};

/** Identification of a method .*/

/** Method template definition. */
struct PardefMethodTemp
{
    /** Unused for compatibility reasons. */
    unsigned short unused;
    
    /** PARX version number. */
    int shlibVersion;

    /** Compile time in seconds since January 1st, 1970. */
    unsigned long ctime;    

    /** Method name. */
    const char * methodName;
    
    /** Method category. */
    MethodCategory methodCategory;
    
    /** Flags for tests of method in ParaVisions test suite. */
    unsigned char testFlags;
    
    /** Method modality. */
    PvModalityType modality;

    /** Number of attachments. */
    unsigned short attachmentCnt;

    /** Array of attachments. */
    const PardefAttachment * attachmentArr;

    /** Size of conflict  parameter pool in bytes. */
    unsigned short conflictCnt; 

    /** Pool for conflicting parameter in this overlay extension. */
    const PardefMember * conflictArr;

    /** Array external parameter relation redefinitions. */
    const PardefRelsRedef * relRedefs;

    /** Common overlay attributes. */
    const PardefOvCommonTemp * common;

};
/** Name of the overlay extension variable. */
#define PARDEF_METHOD_NAME	"PARX_method_template"


/** Modality template definition. */
struct PardefModalityTemp
{
    /** PARX version number. */
    int shlibVersion;

    /** Compile time in seconds since January 1st, 1970. */
    unsigned long ctime;
    
    /** Modality. */
    PvModalityType modality;
    
    /** Name overlay that is extended. */
    const char * ovName;
    
    /** Registration index of overlay. */
    int registeredOvNdx;
    
    /** Array external parameter relation redefinitions. */
    const PardefRelsRedef * relRedefs;

    /** Common overlay attributes. */
    const PardefOvCommonTemp * common;

};
/** Name of the overlay extension variable. */
#define PARDEF_MODALITY_NAME	"PARX_modality_template"

/** Overlay template definition. */
struct PardefOverlayTemp
{
    /** Unused for compatibility reasons. */
    unsigned short unused;
    
    /** PARX version number. */
    int shlibVersion;

    /** Compile time in seconds since January 1st, 1970. */
    unsigned long ctime;
    
    /** Overlay name. */
    const char * ovName;

    /** Registered overlay index, -1 if this overlay is not registered. */
    int registeredNdx;
    
    /** Number of group initialization order instances. */
    unsigned int groupInitOrderCnt;

    /** Array of group initialization order references. */
    const PardefGroup * const * groupInitOrderArr;


    
    /** Number of allowed extensions. */
    unsigned short allowExtensCnt;
    
    /** Indicates which extension types are allowed 
     * for this overlay. 
     */
    const OverlayType * allowExtensions;

    /** Common overlay attributes. */
    const PardefOvCommonTemp * common;
};

/** Name of the overlay template variable in overlays. */
#define PARDEF_OV_TEMP_NAME	"PARX_overlay_template"


/*@}*/

#endif
#endif /* PARX_PARDEF_H */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
