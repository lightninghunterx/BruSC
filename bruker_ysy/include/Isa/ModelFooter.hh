//==============================================================================
//
// $Source$
// Footer file for ISA model implementations
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

#ifndef MODELFOOTER_H
#define MODELFOOTER_H

#include "Isa/ModelFactory.hh"


//==============================================================================
// FOOTER FILE FOR ISA MODEL IMPLEMENTATIONS - DO NOT EDIT
//==============================================================================

};

class ThisModelFactory : public ModelFactory {
public:

    ModelInterface* getNewModelImpl(ModelBase& modelBase) {
        return new ModelImpl(modelBase);
    }
};

PVPLUMA_INHERIT_PROVIDER(ThisModelFactory, ModelFactory)

PVPLUMA_INHERIT_PROVIDER_SOURCE(ThisModelFactory)

// Plugin connector to host:
bool pluma_connect(pluma::Host& host) {
    host.add(new ThisModelFactoryProvider());
    return true;
}

//==============================================================================
// END ISA MODEL IMPLEMENTATION
//==============================================================================

#endif // MODELFOOTER_H

//==========================================================================EOF=