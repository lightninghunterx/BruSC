//===========================================================================
// $Source$
//  
//
// Copyright (c) 2014
//
// Bruker BioSpin MRI GmbH 
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//===========================================================================

#ifndef PLUMA_PVCONNECTOR_HPP
#define PLUMA_PVCONNECTOR_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <Pluma/Pluma.hpp>


/////////////////////////////////////////////////////////////////
// Define portable import/export macros for Plugin registration
/////////////////////////////////////////////////////////////////
#ifndef DLL_INTERFACE
#define DLL_INTERFACE
#endif

#ifdef __cplusplus
extern "C" DLL_INTERFACE bool pluma_connect(pluma::Host&);
#else
DLL_INTERFACE bool pluma_connect(pluma::Host&);
#endif


#endif // PLUMA_PVCONNECTOR_HPP
