//===============================================================
//
// $Source$
//   Adjustment result administration class
//
// Copyright (c) 2015
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//
//===============================================================

#ifndef PVMRADJ_MRHARDWARE_CONTEXTS_HH
#define PVMRADJ_MRHARDWARE_CONTEXTS_HH

#include <stdint.h>
#include <vector>
#include <set>
#include "PvAdj/adjManagerTypes.h"
#include "PvAdj/HardwareContext.hh"
#include "PvMrAdj/MrAdjManagerTypes.h"

#include "PvMrAdj/DLLMacro.hh" // must be the last include.


namespace Parx
{
    namespace Common
    {
        class ParamInterface;
    }
}

namespace PvMrAdj
{

    /** The hardware context class. 
        This class is reference counted.
     */
    class DLL_INTERFACE MrHardwareContext : public PvAdj::HardwareContext
    {
    public:

        /** Constructor.
            @param chains An array of hardware chains.
            @param numChains The number of hardware chains.
         */
        MrHardwareContext(const AdjHardwareContext * chains,
            int numChains);

        /** Define hardware context from parameter space and chain definitions.
            @param chainDefs Array of chain definitions.
            @param cnt Number of chain definitions.
            @param psInterface Parameter space interface.
            @throw PvException in case of error.
         */
        MrHardwareContext(const AdjChainDef * chainDefs, int cnt,
            const Parx::Common::ParamInterface & psInterface);

        /**
         * Create an MR adjustment hardware context from a given 
         * chain context array.
         * @param chains The array of chain contexts.
         * @param cnt Number of array elements.
         * @throw PvException in case of error.
         */
        MrHardwareContext(const AdjChainContext * chains, int cnt);

        /// Destructor
        virtual ~MrHardwareContext();


        /**
         * Get the current hardware context for a given parameter space.
         * @param ps The parameter space.
         * @param hwContext The hardware context. The element \a key is used
         *   to define the type of the hardware context that is determined and
         *   the element \a context is filled with the hardware context value.
         * @throws PvException in case of an error.
         */
        virtual void getCurrentContext(
            const Parx::Common::ParamInterface & ps,
            AdjHardwareContext & hwContext) const;

    private:

        /// Unimplemented default constructor.
        MrHardwareContext();


        /// Unimplemented copy constructor.
        MrHardwareContext(const MrHardwareContext &);

        /// Unimplemented assignment operator.
        void operator=(const MrHardwareContext &);

        void getCurrentContextFromChainDef(
            char * context, const Parx::Common::ParamInterface & ps,
            HW_SCOPE_ENUM hwScope, int chan) const;


    };

}

#endif // #ifndef PVMRADJ_HARDWARE_CONTEXTS_HH

//***************************************************************/
//	E N D   O F   F I L E					*/
//***************************************************************/
