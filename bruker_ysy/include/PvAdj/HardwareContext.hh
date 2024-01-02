//===============================================================
//
// $Source$
//   Adjustment result administration class
//
// Copyright (c) 2009 - 2015
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//
//===============================================================

#ifndef PVADJ_HARDWARE_CONTEXTS_HH
#define PVADJ_HARDWARE_CONTEXTS_HH

#include <iosfwd>
#include <set>
#include <stdint.h>
#include "adjManagerTypes.h"
#include "PvAdj/DLLMacro.hh" // must be the last include

namespace Parx
{
    namespace Common
    {
        class ParamInterface;
    }
}

namespace PvAdj
{

    /** The hardware context class. 
        This class is reference counted.
     */
    class DLL_INTERFACE HardwareContext
    {
    public:

        /// Destructor
        virtual ~HardwareContext();

        /// Helper class to sort a hardware context.

        class Sorter
        {
            /** Less operator for hardware contexts.
                The less operator is defined a follows:
                - return \c false if both are 0
                - return \c true if  \c hc1 is 0.
                - return \c false if \c hc2 if 0.
                - if number of chain elements in \c hc1 is less than 
                  number of chain elements in \c hc2 return \c true.
                - if number of chain elements in \c hc1 is greater than
                  number  of chain elements in \c hc2 return \c false.
                - if number of chain elements is equal compare chain elements.
                  If number of equal chain elements if less than half
                  of elements return \c true.

                @param hc1 The first hardware context.
                @param hc2 The second hardware context.
                @return \c true if \c hc1 is less than \c hc2.
             */
            bool operator()(const HardwareContext * & hc1,
                const HardwareContext * & hc2) const;
        };


        /// Compare hardware context.
        bool operator==(const HardwareContext & hw) const;


        /// Return number of chains stored in the hardware context.

        int getChainCnt() const
        {
            return chains_.size();
        }

        /** Compare stored hardware context with a hardware context from a
            parameter space.
            @param psInterface Parameter space interface.
            @return \c true if the hardware context is identical to
              the current hardware context from the given parameter space
              else \c false.
         */
        bool compareChains(
            const Parx::Common::ParamInterface & psInterface) const;

        /** Fill overlay parameters.
            @param chainNdx A chain index structure to fill.
            @param contexts The contexts array to be filled.
            @param startNdx Start index of free entries in context array.
            @return Number of elements filled into context array.
         */
        int toOverlayRep(AdjChainNdx & chainNdx, AdjHardwareContext * contexts,
            int startNdx) const;

        /** Print string representation of context to output stream.
            @param os The output stream.
            @return The output stream.
         */
        std::ostream & toString(std::ostream & os) const;

        /**
         * Get the hardware contex for the parameter space.
         * @param ps The parameter space.
         * @param hwContext The hardware context to be filled.
         * @throws PvException on PARX error.
         */
        virtual void getCurrentContext(
            const Parx::Common::ParamInterface & ps,
            AdjHardwareContext & hwContext) const = 0;

    protected:

        /** Constructor.
            @param chains An array of hardware chains.
            @param numChains The number of hardware chains.
         */
        HardwareContext(const AdjHardwareContext * chains,
            int numChains);


        /// Default constructor.
        HardwareContext();

        /// Unimplemented copy constructor.
        HardwareContext(const HardwareContext &) = delete;
        

        /// Unimplemented assignment operator.
        void operator=(const HardwareContext &) = delete;
        /// Helper class to sort adjustment chain definitions.
        
        void setChains(const AdjHardwareContext * chains, 
        int numChains);

        /// Sorter class for adjustment chains.
        class AdjChainContextSort
        {
        public:
            /// Less operator for adjustment chains.
            bool operator()(const AdjHardwareContext & acc1,
                const AdjHardwareContext & acc2) const;
        };

        /// Type definition for adjustment context sets.
        typedef std::set<AdjHardwareContext, AdjChainContextSort>
        AdjChainContextSet;

        /// The hardware chain information. 
        AdjChainContextSet chains_;
    };

}



#endif // #ifndef PVCFG_HARDWARE_CONTEXTS_HH

//***************************************************************/
//	E N D   O F   F I L E					*/
//***************************************************************/
