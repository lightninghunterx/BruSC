/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2014
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * Plugin main program to test linkage.
 *
 ****************************************************************/
#include "Pluma/Host.hpp"

extern "C" {
bool pluma_connect(pluma::Host& host);
}

int main(int argc, const char * argv[]);

int main(int /* argc */, const char * /* argv */[])
{
    pluma::Host *hostPtr = nullptr;
    pluma_connect(*hostPtr);
}
