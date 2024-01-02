/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVUTIL_BACKTRACEELEMENT_HH
#define PVUTIL_BACKTRACEELEMENT_HH


#include <stdint.h>
#include <string>
#include <deque>

#include "PvUtil/DLLMacro.h"


/** @file
 *  @addtogroup PvUtil 
 *
 */
/*@{*/


/** A backtrace element class. */
class BacktraceElement
{
public:

#ifdef HAS_UNIX_FEATURES
    /** Constructor from a backtrace line.
        @param btline The backtrace line from \c backtrace_symbols function.
    */
    DLL_INTERFACE BacktraceElement(const char * btline);
#elif defined(HAS_WINDOWS_FEATURES)
    DLL_INTERFACE BacktraceElement(const char * symName, uint64_t address);
#endif

    /// Return the filename of the binary object..
    DLL_INTERFACE const std::string & getDsoName() const;
    
    /// Return the runtime symbol.
    DLL_INTERFACE const std::string & getRuntimeSymbol() const;

    /// Return runtime offset.
    DLL_INTERFACE intptr_t getRuntimeOffset() const;

    /// Return runtime address.
    DLL_INTERFACE uintptr_t getRuntimeAddress() const;

    /// Return the demangled function name of the backtrace element.
    DLL_INTERFACE const std::string & getFunction() const;

private:
    /// Demangle a symbol. 
    static std::string demangle(const std::string & symbol);

    /// the object filename.
    std::string dsoName;

    /// The runtime symbol.
    std::string runtimeSymbol;

    /// The offset to the runtime symbol.
    intptr_t runtimeOffset;

    /// The runtime address of the symbol.
    uintptr_t runtimeAddress;

    /// Demangled function name.
    std::string function;
};

/*@}*/

#endif /* PVUTIL_BACKTRACE_HH */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/


