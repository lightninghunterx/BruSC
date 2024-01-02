/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2012
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 * ***************************************************************/


static const char resid[] = "$Id $ (c) 2012 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE       0
#define DB_LINE_NR	0


#include "method.h"


void InitLocalization(void)
{
  DB_MSG(("--> InitLocalization"));

  /* Voxel geometry */
  STB_InitVoxelGeometry(1);
  
  /* Initialisation of rf pulse parameters */
  
  STB_InitRFPulse("RefPulse1",      // name of pulse structure
                  "RefPulse1Enum",  // name of pulse list parameter
                  "RefPulse1Ampl",  // name of pulse amplitude parameter
                  "RefPulse1Shape", // name of pulse shape (for calc. pulses)
                  1,                // used for refocusing
                  "Calculated",    // default shape
                  5000.0,           // default bandwidth
                  180.0);            // default pulse angle

  // check valid range for this specific pulse see parsRelations.c
  RefPulse1Range();


  /* Initialisation of spoilers */
  MRT_InitSpoiler("RefSpoiler");

  DB_MSG(("<-- InitLocalization"));
}
