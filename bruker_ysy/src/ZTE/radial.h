/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2004
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

/* editable */

YesNo parameter
{
  display_name "Reorder Projections";
  short_description "Improves spoiling.";
  relations backbone;
} Reorder;

/* non-editable */

int    parameter RadialNPhi[];         // number of Phi angles for each Theta angle

/* arrays required temporarily for reordering */

int    parameter ReorderMask[][];      // mask used for reordering projections
double parameter ReorderGR[];          // reordered read gradient values
double parameter ReorderGP[];          // reordered phase gradient values
double parameter ReorderGS[];          // reordered slice gradient values








