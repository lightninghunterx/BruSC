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

#ifndef MR_EPI_TYPES_H
#define MR_EPI_TYPES_H

/* type definitions for the EPI parameter group */

typedef enum
{
  SystemRamp,
  UserRamp,
  UserSlope
} EpiRampMode;

typedef enum
{
  No_navigators,
  InitNav,         /* initial prescan as navigator */
  PrefixNav,       /* navigator as prefix to every scan */
  InitAndPrefixNav /* initial prescan and prefix */
} EpiNavigatorMode;

typedef enum
{
  Line,
  Sine
} EpiRampForm;

typedef enum
{
  Never,
  In_Multishot,
  In_All
} DoubleShotMode;

#endif
