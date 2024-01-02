//===============================================================
//
// Copyright (c) 2006-2018
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//
//===============================================================

#ifndef PVADJ_ADJUTILS_HH
#define PVADJ_ADJUTILS_HH

//---------------------------------------------------------------
// includes
//---------------------------------------------------------------
#include <iosfwd>
#include "modality.h"
#include "PvAdj/adjManagerTypes.h"
#include "PvAdj/DLLMacro.hh" // must be the last include

struct AdjConfigType;

namespace Parx
{
    namespace Common
    {
        class ParamInterface;
    }
}

namespace PvAdj
{
    class Adjustment;
    class AdjStateAdmin;
    class ResultAdmin;
    class AdjConfiguration;

    /** Static Utility functions for adjustment handling. */
    namespace AdjUtils
    {

        /** An empty adjustment context. */
        extern const AdjContextType Empty;

        /** Format a message about an adjustment context.
            The message is formated in the format
            <code>message : ContextName</code>. 
            @param os stream where format taken place.
            @param message The message to be formated.
            @param context The adjustment context.
         */
        DLL_INTERFACE void FormatMessage(std::ostream & os, const char* message,
            const AdjContextType* context);

        /**
         * Convert an adjustment configuration into an adjustment context.
         * @param adjContext The destination adjustment context.
         * @param adjConfig The source adjustment configuration.
         */
        DLL_INTERFACE void ConvertAdjConfig(AdjContextType & adjContext,
            const AdjConfigType & adjConfig);

        /**
         * Get the default adjustment configuration name for a given modality.
         * @param modalityType The modality.
         * @return The default configuration name, may be {@code 0}.
         */
        DLL_INTERFACE const char * GetDefaultAdjConfigName(
            PvModalityType modalityType);
    };

    /** Equal operator for adjustment contexts.
        Two contexts are equal if the names, protocols, adjusted parameters,
        and method context are equal.
        @param adj1 first adjustment to be compared.
        @param adj2 second adjustment to be compared.
        @return \c true if contexts are equal.
        @see operator!=
     */
    bool operator==(const AdjContextType & adj1, const AdjContextType & adj2);

    /** Unequal operator for adjustment contexts.
        This is <code>! operator==</code>.
        @param adj1 first adjustment to be compared.
        @param adj2 second adjustment to be compared.
        @return \c true if contexts are not equal.
        @see operator==

     */
    bool operator!=(const AdjContextType & adj1, const AdjContextType & adj2);

}
#endif  

//***************************************************************/
//	E N D   O F   F I L E					*/
//***************************************************************/
