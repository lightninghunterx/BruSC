//===============================================================
//
// Interface of a Relation Context.
//
// Copyright (c) 2009-2019
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//
//===============================================================

#ifndef PARX_RELSCONTEXT_HH
#define PARX_RELSCONTEXT_HH

//---------------------------------------------------------------
// includes and forward declarations
//---------------------------------------------------------------

#ifndef CPROTO

#include "pvmachine.h"
#include <vector>
#include <set>
#include <string>
#include "Parx/Base/DLLMacro.h"

struct PardefElement;

namespace Parx
{
    namespace Base
    {
        class SavedParamObject;
        class Parameter;
    }
}

//---------------------------------------------------------------
// interfaces 
//---------------------------------------------------------------

namespace Parx
{
    namespace Relation
    {
        /** 
         *  @ingroup PvParx
         *  @addtogroup Relation
         *  @{
         */

        /** @class PostExecutionInfo Context.hh "Parx/Rels/Context.hh"
         * Class holding information about a relation that is used after relation
         * execution.
         * @since ParaVision 6.Alpha.0.76
         * @author Andreas Barth
         */
        class DLL_INTERFACE PostExecutionInfo
        {
        public:
            /// Constructor
            PostExecutionInfo();

            /// Destructor.
            ~PostExecutionInfo();

            /** Set the relation conflict cause.
                @param parname The conflict cause parameter name.
             */
            void addConflictCause(const char * parname);

            /** Remove a relation conflict cause.
                @param parname The conflict cause parameter.
             */
            void removeConflictCause(const char * parname);

            /** Get the relation conflict cause.
                @return The conflict cause set. 
             */
            const std::set<std::string> & getConflictCauseSet() const;

            /** Get the relation conflict cause set to remove.
                @return The conflict case set to remove.
             */
            const std::set<std::string> & getRemoveConflictCauseSet() const;


        private:
            /// The conflict cause set.
            std::set<std::string> conflictCauseSet_;

            /// The remove conflict cause set.
            std::set<std::string> conflictRemoveCauseSet_;

        };

        /** @class Context Context.hh "Parx/Rels/Context.hh"
         *  Class to define a relation context that can be retrieved 
         *  by the relation.
         *
         *  @since ParaVision 6.Alpha.0.65
         *  @author Andreas Barth
         */
        class DLL_INTERFACE Context
        {
        public:

            /// Element type of qualified parameter.

            enum ElemType
            {
                /** Struct Element. */
                STRUCT_ELEM,
                /** Array element. */
                ARRAY_ELEM
            };

            /// Parameter element specification

            struct ParamElementSpec
            {
                /// The element type.
                ElemType type;

                /// Union containing information about element type.

                union
                {
                    /// Only valid for type ARRAY_ELEM, array index.
                    int ndx;

                    /// Only valid for type STRUCT_ELEM, structure element name.
                    const char * name;
                } elemSpec;
            };

            /// Listener Class for this contexts.

            class Listener
            {
            public:
                /// Destructor.

                virtual ~Listener() { }

                /** Notify if the Relation context is deleted.
                    @param context The context for which the listener was called.
                 */
                virtual void notifyDelete(const Context * context) = 0;
            };

            /** Constructor.
                @param param Parameter instance.
                @param elem Element definition, may be 0.
                @param oldVal Old element value, may be 0.
                @param postRelsExecInfo Post-relation execution information.
                @param restore Indicate if this relation was called in a restore
                   operation.
             *  @param callContext The call context
             */
            Context(Parx::Base::Parameter & param,
                const PardefElement * elem,
                const Parx::Base::SavedParamObject * oldVal,
                PostExecutionInfo * postRelsExecInfo,
                bool restore, const std::string & callContext);

            /// Destructor.
            ~Context();

            /// Return parameter name for which relation was called.
            const char * getParName() const;

            /**
             * Get the number of parameter elements.
             * In a qualified parameter name the count of [ and . can be 
             * interpreted as the number of parameter element, e.g. \c A[1].spec
             * has 2 parameter elements, an array and a struct element.
             * @return The number of parameter elements for a
             *   qualified parameter name.
             */
            int getParElemCnt() const;

            /**
             *  Get the qualified parameter element name.
             *  @param withParamName Indicates if parameter name is included 
             *         in the returned string
             * @return The qualified parameter element name.
             */
            std::string getQualifiedName(bool withParamName) const;


            /** 
             * Get parameter element specification for each parameter element.
             * For example, for a parameter A[1].spec:
             * - \c getParElemSpec(0) returns a \c ParamElementSpec with \c type 
             *   \c ARRAY_ELEM and \c ndx 1.
             * - \c getParElemSpec(1) returns a \c ParamElementSpec with \c type
             *   \c STRUCT_ELEM and \c name set to \c spec.
             * @param ndx Index of parameter element.
             * @return The specified parameter element or 0
             *  if index was lower than 0 or higher that the result of
             *    getParElemCnt.
             * @see #getParElemCnt
             * @see ParamElementSpec
             */
            const ParamElementSpec * getParElemSpec(int ndx) const;

            /// Indicates if original value is stored in this context.

            bool origValueStored() const
            {
                return 0 != old_;
            }

            /**
             * Indicates if original value of the parameter was set.
             * @return \c true if parameter had an original value or 
             *   \c false if parameter has not original value, i.e. was not set.
             */
            bool hasOrigValue() const;

            /** 
             * Get dimension of original parameter element value.
             * The call to this routine may create an invalid result
             * or an exception if called after the parameter has 
             * changed dimensions.
             * @param ndx The dimension index, beginning with 0.
             * @return The number of elements in a dimension, 0 if it is not an
             * array or the dimension does not contain any elements, -1 if
             * no parameter value is stored.
             */
            int getOrigParDim(int ndx) const;


            /// Return size of the original parameter element.
            unsigned int getOrigParSize() const;

            /** Get original parameter element value.
             * The call to this routine may create an invalid result
             * or an exception if called after the parameter has 
             * changed dimensions.
             * @param origParVal The original parameter value is copied 
             *        into the given memory which must be provided by the
             *         caller. The complete parameter element is returned and 
             *         not only the changes part.
                @return \c true if the original value is stored in this context.
             */
            bool getOrigParValue(void * origParVal) const;

            /**
             * Get the call context of the relation.
             * @return The call context of the relation. The call context is 
             * always empty if the relation was not called from another relation.
             */
            const std::string & getCallContext() const { return callContext_; }


            /// Indicates if relation for parameter was called in read-only context.

            bool readOnlyContext() const
            {
                return inReadOnlyContext_;
            }

            /** 
             * Add a listener.
             *  @param listener The new listener.
             */
            void addListener(Listener * listener) const;

            /**
             * Remove a listener.
             * @param listener The listener to be removed.
             */
            void removeListener(Listener * listener) const;

            /**
             * Call all registered listeners if context is removed.
             */
            void fireDelete() const;

            /// Get the post relation execution information.
            PostExecutionInfo * getPostExecutionInfo() const;

            /// Indicate if relation is called in a restore operation.

            bool isRestore() const
            {
                return restore_;
            }

            /// Set the read-only context.
            void setReadOnlyContext(bool readOnly);


        protected:

        private:

            /// Unusable default constructor.
            Context();

            /// Unusable copy constructor.
            Context(const Context &);

            /// Unusable assignment operator.
            void operator=(const Context &);

            /// The parameter.
            Parx::Base::Parameter & param_;

            /// The element definition.
            const PardefElement * elem_;

            /// Indicates if relation is called in a read-only context
            bool inReadOnlyContext_;

            /// Indicate if the relation is called in a restore operation.
            const bool restore_;

            /// The old value.
            const Base::SavedParamObject * old_;

            /// The changed conflict cause parameter.
            PostExecutionInfo * postExecInfo_;

            /// Array of Parameter element specifications.
            mutable std::vector<ParamElementSpec> parElemArray_;

            /// Listener.
            mutable std::set<Listener *> listenerSet_;

            /// The call context string of the relation call.
            const std::string callContext_;

        };
    }

    /* @} */

}

//---------------------------------------------------------------
// interface of non-member functions / methods
//---------------------------------------------------------------

#endif

#endif
