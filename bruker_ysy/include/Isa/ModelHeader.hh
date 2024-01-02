//==============================================================================
//
// Header file for ISA model implementations
//
// Copyright (c) 2015 - 2018
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//
//==============================================================================

#ifndef MODELHEADER_HH
#define MODELHEADER_HH

//==============================================================================
// HEADER FILE FOR ISA MODEL IMPLEMENTATIONS - DO NOT EDIT
//==============================================================================

#include "pvmachine.h"

#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <float.h>
#include <limits.h>
#include <cstdlib>

#include "debugdef.h"
#include "Parx/Parx.h"
#include "imagetyp.h"
#include "Visu/Common/VisuTypes.h"
#include "generated/ParxDefs.h"


#include <Pluma/PvPluma.hpp>
#include <Pluma/PvConnector.hpp>

// Symbol Visibility setting.
#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#  ifdef IsaModel_INTERNAL 
#    define DLL_INTERFACE __declspec(dllexport)
#  else
#    define DLL_INTERFACE __declspec(dllimport)
#  endif
#elif defined (__GNUC__) && __GNUC__ >= 4
#    define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#    define DLL_INTERFACE
#endif 

#include "Isa/ModelInterface.hh"
#include "Isa/ModelFactoryProvider.hh"


class ModelImpl : public ModelInterface {
private:
    ModelImpl(const ModelImpl&) = delete;

    ModelImpl& operator=(const ModelImpl&) = delete;

    ModelBase& b_;

public:

    ModelImpl(ModelBase& modelBase) : b_(modelBase) {
        DB_MSG(("ModelImpl constructor for %s", modelBase.getName()));
    };

#include "Isa/ModelAccess.hh"

#endif // MODELHEADER_HH

