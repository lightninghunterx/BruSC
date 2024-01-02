
//===============================================================
//
// Common parameter interface for functions used in libParxRels and libParx.
//
// Copyright (c) 2009 - 2019
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//
//===============================================================

#ifndef PARX_COMMON_INTERFACE_HH
#define PARX_COMMON_INTERFACE_HH

//---------------------------------------------------------------
// includes
//---------------------------------------------------------------
#include <string>
#include <vector>
#include <map>
#include <utility>
#include "PvUtil/DataPathType.h"
#include "Parx/Common/EnumTypeClass.hh"
#include "Parx/publicTypes.h"
#include "Parx/Common/CommonTypes.hh"
#include "Parx/Utils/ParamFiles.hh"


//---------------------------------------------------------------
// interface of class ParamInterface
//---------------------------------------------------------------

/**
   @ingroup PvParx
 */
namespace Parx
{

    class PsAttribute;

    /** Name space for interfaces that are used with libParx, libParxBase,
        and libParxRels interface.
     */
    namespace Common
    {

        /**
           @defgroup CommonInterface Common Parx interface classes.
           @{
         */

        /** @class ParamInterface ParamInterface.hh "Parx/Common/ParamInterface.hh"
            A common parameter interface.
            All methods are abstract.
            Implementations exists for using with libParxBase, libParx,
            libParxRels, and libParxClient.
         */
        class ParamInterface
        {
        public:

            /** Destructor. */
            virtual ~ParamInterface() { }

            /// Get the parameter space key.
            virtual std::string getKey() const = 0;

            /// Get the modality of the parameter space.
            virtual PvModalityType getModalityType() const = 0;

            /** Clear all parameters in the parameter list.
                @param parnames List of parameter names to be cleared.
                @exception PvException in case of an error.
             */
            virtual void clearPars(
                const std::vector<std::string> & parnames) const = 0;

            /** Call a parameter relation.
                @param parName The (qualified) parameter name. If the
                    name is qualified the qualification if given to
                    the relation as a relation context.
                @param callMode Indicates if and which parameter relations 
                are called.
             * @param callContext Relation call context, always empty if 
             * relation is not called from another relation.
                @throws PvException in case of an error.
             */
            virtual void callParamRelation(const char * parName,
                ParxCallMode callMode, const std::string & callContext) const = 0;

            /** Set parameters defined in a list of parameter name / value pairs.
                The parameters are set and then the relations are called
                as defined in the parameter \c callRels.
                @param setList The list of parameter name / value pairs.
                @param callRelsMode Indicates if and which relations for all 
             *      parameters are called.
                @exception PvException in case of an error.
                @see ParxParamSetStruct
             */
            virtual void setPars(
                const std::vector<ParxParamSetStruct> & setList,
                ParxCallMode callRelsMode) const = 0;

            /** 
             * Get parameter value / attributes.
             * @param getList Array of parameter name / value/ attributes.
             * @param startIdx first value to get.
             * @see ParxParamGetStruct
             */
            virtual void getPars(std::vector<ParxParamGetStruct> & getList,
                size_t startIdx = 0) const = 0;

            /** 
             * Get parameter values / attributes.
             * @param getList Array of parameter name / value/ attributes.
             * @param cnt Number of parameters.
             * @exception PvException in case of an error.
             * @see ParxParamGetStruct
             */
            virtual void getParsArray(ParxParamGetStruct * getList,
                size_t cnt) const = 0;

            /**
             * Get a parameter value in string representation.
             * @param parName The parameter name.
             * @param includeUnit indicates if unit should be included.
             * @param hasValue Indicates on return if parameter has a value.
             * @return The parameter value in string representation, an
             *    empty string if parameter has no value and \a hasValue true.
             */
            virtual std::string getParameterAsString(const char * parName,
                bool includeUnit, bool & hasValue) const = 0;

            /** Get enumeration type information.
                @param param The enumeration parameter.
                @return The enumeration type information. The returned
                    memory must be deallocated using \c delete.
                @throws PvException in case of an error.
             */
            virtual const Parx::Common::EnumType * getEnumTypeInformation(
                const char * param) const = 0;

            /**
             * Indicates if the given parameter has a value.
             * @param param The parameter to be tested.
             * @return Indication if parameter \a param has a value.
             */
            virtual bool hasValue(const char * param) const = 0;

            /**
             * Get type information for a parameter (element).
             * For simple types and arrays of simple types only the type id 
             * is filled.For structs, enums and struct / enum arrays also 
             * the type name is returned.
             * @param name Parameter element name.
             * @param ti The type information structure to be filled.
             * @throws PvException in case of an error.
             */
            virtual void getParameterType(const char * name,
                ParxElementType & ti) const = 0;

            /**
             * Get the constant information for a parameter element.
             * The constant information is
             * - the parameter element display name.
             * - the parameter element short description.
             * - the parameter element long description.
             * - the parameter element unit.
             * - the parameter element format.
             * 
             * @param name The name of the parameter element.
             * @param ti The constant information structure to be filled.
             * @throws PvException on error.
             */
            virtual void getParameterInfo(const char * name,
                ParxElementInfo & ti) const = 0;

            /** Pair or a parameter name and a dimension. */
            typedef std::pair<const char *, std::vector<int> > ChangeDims;
            /** List type to change dimensions. 
                The list elements are a pair of a parameter name and
                a dimension vector.
             */
            typedef std::vector<ChangeDims> ChangeDimsList;

            /** Change dimension of a list of parameters.
                @param parDims List of parameter / dimension pair.
                @exception PvException on error.
             */
            virtual void changeParDims(
                const ChangeDimsList & parDims) const = 0;


            /** Pair of a ParxParamSetStruct and a parameter dimension. */
            typedef std::pair<ParxParamSetStruct, std::vector<int> >
            ChangeDimsParVal;
            /** List type to change dimensions and values of a parameter.
                The list elements are pairs of a ParxParamSetStruct and an
                dimension vector.
                @see ParxParamSetStruct
             */
            typedef std::vector<ChangeDimsParVal> ChangeDimsParValList;

            /** Change dimensions and value for a list of parameters.
                @param parDimsVals List of parameter dimensions and values.
                @param callRelsMode Indicates if and what relations for all
             *      parameters are called.
                @exception PvException on error.
             */
            virtual void changeDimsAndSetPars(
                const ChangeDimsParValList & parDimsVals,
                ParxCallMode callRelsMode) const = 0;


            /** Write parameters or groups to a parameter file.
                @param file The parameter file to be written.
                @param parnames List of parameters / groups to be written.
                @param mode The write mode for the parameter group.
                @param clearChangedFlag Indicates if change flag 
                  should be cleared after writing. If it is cleared the 
                  gui will not ask to save the paraemter when closing 
                  the parameter space.
                @exception PvException on error.
             */
            virtual void writePars(
                const char * file,
                const std::vector<std::string> & parnames,
                Parx::Utils::WriteMode mode, bool clearChangedFlag) const = 0;


            /** Read groups from a parameter file.
             * @param file The parameter file to be read from.
             * @param group The parameter group name.
             * @param callPostRead Indicates if group post-read routines 
             *    should be called after reading. Parameter relations
             *     are not performed.
             * @param forceReadOnly Indicates if group post-read routines
             *     are forced to be performed in read-only mode. Only used
             *     is \a callPostRead is \c true.
             * @exception PvException on Error.
             */
            virtual void readGroup(const char * file, const char * group,
                bool callPostRead, bool forceReadOnly) const = 0;


            /** Read parameter group from a jcamp formated buffer.
                @param[in] buffer The parameter file to be read from.
                @param[in] identifier Human readable name of the buffer 
                   (e.g. file name).
                @param[in] group The parameter group name.
                @param[in] isDsetFile Indicates if this buffer contains 
                    contents from a dataset parameter file.
             * @param callPostRead Indicates if group post-read routines 
             *    should be called after reading. Parameter relations
             *     are not performed.
             * @param forceReadOnly Indicates if group post-read routines
             *     are forced to be performed in read-only mode. Only used
             *     is \a callPostRead is \c true.
                @param[in] clearParamChangeFlags Indicates that the parameter
                         change flags (used for dataset save indication) should
                         be cleared. This is normally the case if a
                         dataset file or protocol is read.
                @exception PvException on Error.
             */
            virtual void scanGroup(const char * buffer, const char * identifier,
                const char * group, bool isDsetFile, bool callPostRead,
                bool forceReadOnly, bool clearParamChangeFlags) const = 0;

            /** Read parameters of groups from a parameter file.
               @param file The parameter file to be read from.
               @param parnames The array of parameters / groups to be read.
               @param callRels Indicated if parameter relation and group 
                  main routines should be called after reading.
               @exception PvException on Error.
             */
            virtual void readPars(const char * file,
                const std::vector<std::string> & parnames,
                bool callRels) const = 0;

            /** Read parameters values from a text buffer in jcamp format.
                @param[in] buffer The text buffer.
                @param[in] identifier Human readable name of the buffer 
                   (e.g. file name).
                @param[in] parnames The array of parameters / groups to be read.
                @param isDsetFile Indicates if this buffer contains 
                    contents from a dataset parameter file.
                @param[in] callRels Indicated if parameter relation and group 
                   main routines should be called after reading.
                @param[in] clearParamChangeFlags Indicates that the parameter
                         change flags (used for dataset save indication) should
                         be cleared. This is normally the case if a
                         dataset file or protocol is read.
                @exception PvException on Error.
             */

            virtual void scanPars(const char * buffer,
                const char * identifier,
                const std::vector<std::string> & parnames,
                bool isDsetFile, bool callRels,
                bool clearParamChangeFlags) const = 0;

            /** Get a dataset path associated with parameter space.
                @param path Buffer to be filled with parameter path.
                @param maxLen Maximum length of \c path buffer.
                @param datPathType Path type to be filled in.
                @param suffix Suffix of path, will be appended to path
                using a directory separator.
                @exception PvException on error, e.g. parameter space contains
                no dataset parameter space, ...
             */
            virtual void getDataPath(char * path, unsigned int maxLen,
                DataPathType datPathType,
                const char * suffix) const = 0;


            /**
             * Copy parameters from the current parameter space into
             * a destination parameter space.
             * @param dst The destination parameter space.
             * @param copyDefs The list of parameters to be copied.
             * @param ignoreUndefined Indicated if undefined parameters are 
             *    ignored.
             * @throws PvException in case of an error.
             */
            virtual void copyParameters(const Parx::Common::ParamInterface & dst,
                const std::vector<ParxCopyDef> & copyDefs,
                bool ignoreUndefined) const = 0;

            /** 
             * Set edit flag for a list of parameters.
             * @param list List of parameter and group names. If a group name 
             *          is given in the list all parameters of the group change 
             *          the flag.
             * @param editFlag Indicates if parameter can be edited in editor.
             * @throws PvException in case of error.
             */
            virtual void setParEditFlag(const std::vector<std::string> & list,
                bool editFlag) const = 0;

            /** 
             * Set file visible flag for a list of parameters.
             * @param list List of parameter and group names. If a group name 
             *          is given in the list all parameters of the group change 
             *          the flag.
             * @param flag Indicates if parameter is saved into a file.
             * @throws PvException in case of an error.
             */
            virtual void setParVisibleInFileFlag(
                const std::vector<std::string> & list, bool flag) const = 0;

            /** 
             * Set editor visible flag for a list of parameters.
             * @param list List of parameter and group names. If a group name 
             *          is given in the list all parameters of the group change 
             *          the flag.
             * @param visFlag Indicates if parameter is visible in an editor.
             * @throws PvException in case of an error.
             */
            virtual void setParVisibleInEditorFlag(
                const std::vector<std::string> & list, bool visFlag) const = 0;

            /** 
             * Set edit flag for a list of groups.
             * The parameters in the groups are also affected. Their group edit flag 
             * is set. 
             * If the group is not editable all parameters contained in the group are 
             * also not editable independent of their own edit flag. 
             * @param list List of group names. 
             * @param editFlag Indicates if group (parameters) can be edited in editor.
             * @throws PvException in case of an error.
             */
            virtual void setGroupEditFlag(
                const std::vector<std::string> & list, bool editFlag) const = 0;

            /** 
             * Set editor visibility for a list of groups.
             * The parameters in the groups are also affected. Their group visible flag 
             * in editor is set. 
             * If the group is not visible in editor all parameters contained 
             * in the group are also not visible in editor independent of their 
             * own editor visible flag. 
             * @param list List of group names. 
             * @param visFlag Indicates if group (parameters) are visible in editor.
             * @throws PvException in case of an error.
             */
            virtual void setGroupVisibleInEditorFlag(
                const std::vector<std::string> & list, bool visFlag) const = 0;


            /// Indicates if interface support parameter space attributes.
            virtual bool interfaceSupportsPsAttributes() const = 0;

            /**
             * A a parameter space attribute to the given parameter space.
             * @param psAttribute Add the given attributes to the 
             * parameter space. 
             * @throws PvException if adding a parameter space attribute is not
             * possible or if the interface does not support parameter space
             * attributes.
             * @see @ref interfaceSupportsPsAttributes
             */
            virtual void addPsAttribute(PsAttribute * psAttribute) const = 0;

            /**
             * Get a parameter space attributes identified by the given name.
             * @param name The name of the parameter space attributes.
             * @return A parameter space attribute of given name if it exists, 
             *   else \c 0.
             * @throws PvException if the interface does not support parameter 
             * space attributes. 
             */
            virtual PsAttribute * getPsAttribute(const char * name) const = 0;

            /**
             * Remove a parameter space attributes identified by the given name.
             * @param name The name of the parameter space attributes.
             * @throws PvException if the interface does not support  parameter 
             * space attributes or removal is not possible.
             */
            virtual void removePsAttribute(const char * name) const = 0;



        };
    }
}

/*@}*/

#endif

