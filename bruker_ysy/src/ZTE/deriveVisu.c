/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2005 - 2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/
const char * methods_zte_derivevisu_id = "$Id$ (C) 2005 Bruker BioSpin MRI GmbH";

#define DEBUG       0
#define DB_MODULE   0
#define DB_LINE_NR  0

#include "method.h"
#include "generated/VisuIds.h"
#include "Visu/VisuTypes.h"

void deriveVisu(void)
{
    DB_MSG (("-->deriveVisu"));

    /* standard VISU derivation */
    ParxRelsParRelations("VisuDerivePars", Yes);

    if (0 == VisuFileVersion)
    {
        /* avoid derivation from ACQ_phase_factor */
        VisuAcqEchoTrainLength = 1;

        /* correct derivation */
        VisuAcqPixelBandwidth = PVM_EffSWh / PVM_Matrix[0];
    }
    
    if (3 > VisuFileVersion)
        VisuAcqKSpaceTraversal = RadialTraversal;

    DB_MSG (("<--deriveVisu"));
}

