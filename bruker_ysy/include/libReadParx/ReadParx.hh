//===============================================================
//
// Copyright (c) 2015 - 2019
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//
//===============================================================

#ifndef LIBREADPARX_READPARX_HH
#define	LIBREADPARX_READPARX_HH


//---------------------------------------------------------------------------
// standard include files
//---------------------------------------------------------------------------
#include <vector>
#include <string>
#include <memory>
//---------------------------------------------------------------------------
// local include files
//---------------------------------------------------------------------------
#include "libReadParx/DLLMacro.hh"


//---------------------------------------------------------------------------
// forward declarations
//---------------------------------------------------------------------------
namespace Parx
{
    namespace Common
    {
        class ParamInterface;
    }
    namespace Client
    {
        class ClientParamInterface;
    }
    namespace Relation
    {
        class RelsParamInterface;
    }
}


/**-------------------------------------------------------------------------
 * interface functions to provide similar parameter access from different
 * overlay types using the PARX common parameter interface.  
 *------------------------------------------------------------------------*/
namespace ReadParx
{
    // some defines for cleaner notation
    //--------------------------------------------------------------------------
    using ParIfc = Parx::Common::ParamInterface;
    using CParIfc = Parx::Client::ClientParamInterface;
    using RParIfc = Parx::Relation::RelsParamInterface;
    
    /**-------------------------------------------------------------------------
     *  functions providing information about the currently active receivers 
     *------------------------------------------------------------------------*/
    // return the active receiver numbers [1..N]
    DLL_INTERFACE std::vector<int> GetActiveReceiverNums(const ParIfc& ps);
    DLL_INTERFACE std::vector<int> GetActiveReceiverNumsForJob(const ParIfc& ps, const int jobIdx);
    DLL_INTERFACE std::vector<int> GetActiveReceiverNumsForChan(const ParIfc& ps, const int chanNum);
    
    // return the number active receivers
    DLL_INTERFACE int GetNumActiveReceivers(const ParIfc& ps);
    DLL_INTERFACE int GetNumActiveReceiversForJob(const ParIfc& ps, const int jobIdx);
    DLL_INTERFACE int GetNumActiveReceiversForChan(const ParIfc& ps, const int chanNum);
    
    /**-------------------------------------------------------------------------
     *  functions providing information about the used digitizer
     *------------------------------------------------------------------------*/
    // return the maximum data value possible
    DLL_INTERFACE double GetMaxDataValueForJob(const ParIfc& ps, const int jobIdx);
    
    /**-------------------------------------------------------------------------
     *  functions reading frequency parameters
     *------------------------------------------------------------------------*/
    // return a vector of offset frequencies in [Hz] used during ACQ 
    // the corresponding list name is read from the parameter FQxLIST 
    // [where x = 1...8 equals the channel number]  
    DLL_INTERFACE std::vector<double> GetFreqOffsetsInHzForJob(const ParIfc& ps, const int jobIdx);
    DLL_INTERFACE std::vector<double> GetFreqOffsetsInHzForChan(const ParIfc& ps, const int chanNum);
    
    
    /**-------------------------------------------------------------------------
     *  misc. functions 
     *------------------------------------------------------------------------*/
    DLL_INTERFACE std::vector<std::string> GetAcqCmdDebugArgs(const ParIfc& ps);
    
    
} // ReadParx

#endif /* LIBREADPARX_READPARX_HH */

/****************************************************************/
/*      E N D   O F   F I L E                                   */
/****************************************************************/
