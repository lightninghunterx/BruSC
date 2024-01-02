//==============================================================================
//
// $Source$
// ModelFactory class for creating ISA model instances.
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
//==============================================================================

#ifndef MODELFACTORY_HH
#define MODELFACTORY_HH

//==============================================================================
// MODEL FACTORY CLASS - DO NOT EDIT
//==============================================================================

#include "Isa/ModelBase.hh"
#include <string>

class ModelFactory {
public:

    virtual ~ModelFactory() {
    };

    virtual ModelInterface* getNewModelImpl(/*ParamInterface&, */ModelBase&) = 0;
};

#endif // MODELFACTORY_HH

