/****************************************************************
 *
 * $Source$
 * 
 * Copyright (c) 1988-2008
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 * 
 * All Rights Reserved 
 * 
 * $Id$
 *
 ****************************************************************/

#ifndef TA_CONFIG_H
#define TA_CONFIG_H

#include "Parx/Dynenum.h"

#define CONFIG_SCAN_CURRENT_VERSION 1

/****************************************************************
 *	TYPEDEF's
 ****************************************************************/

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type USER_TYPE

  @ptypePurpose  Specifies the type of user
    Routine:     Routine user 
    Research:    Expert user. 
    When editing a measuing method, the expert user sees 
    more parameters and he is allowed to edit more of the 
    low level parameters.
  @ptypeAuthor  MAH                                     */
/*------------------------------------------------------*/

typedef enum
{
  Routine,
  Research
} USER_TYPE;


/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type DISCLAIMER_LANGUAGE

  @ptypePurpose  Set Language of disclaimer independant of OS language.
     This is a dynamic enum. Values come from libPvDisclaimer.

  @ptypeAuthor  ANBA                                     */
/*------------------------------------------------------*/
typedef dynenum LANGUAGE_TYPE;

#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
