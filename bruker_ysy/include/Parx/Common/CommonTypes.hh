/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2015 - 2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PARX_COMMON_COMMONTYPES_HH
#define	PARX_COMMON_COMMONTYPES_HH

#ifndef CPROTO

#include <string>

#include "Parx/Utils/DLLMacro.h"


/**
    @ingroup CommonInterface 
    @{
 */

/** Parameter element type information. */
struct DLL_INTERFACE ParxElementType
{
    /// Default constructor.
    ParxElementType();
    
    /** The type indication as defined in publicTypes.h. */
    unsigned char type;
    /** The type name. */
    std::string name;
};

/** Constant parameter element information. */
struct DLL_INTERFACE ParxElementInfo
{
    /// Default constructor.
    ParxElementInfo();
    
    /** The parameter element display name. */
    std::string displayName;
    /** The parameter element short description. */
    std::string shortDescription;
    /** The parameter element long description. */
    std::string longDescription;
    /** The parameter element unit. */
    std::string unit;
    /** The parameter element format. */
    std::string format;
};

/** @} */

#endif


#endif	/* PARX_COMMON_COMMONTYPES_HH */

