/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2006-2010
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

static const char resid[] = "$Id$ (C) 2006-2010 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	1


#include "method.h"

/* =======================================================*
 *
 * Global Function: loadMeth
 *
 * Description: This procedure is automatically called in
 *	response to a method file for this method being read.
 *
 * Error History: 
 *
 * Interface:							*/
/* =======================================================*/
void loadMeth  (const char *	className)
{
  DB_MSG(( "-->STEAM:loadMeth( %s )", className ));


  if (0 != className)
  {
    if (0 == strcmp( className, "MethodClass"))
    {
      DB_MSG(( "...responding to loadMeth call - I know this class" ));
      MRT_CheckOperationMode();
      initMeth();
    }
    else if (0 == strcmp(className, "MethodRecoGroup"))
    {
      DB_MSG(("...responding to loadMeth call for MethodRecoGroup."));
      SetRecoParam();
    }
  }
  else
  {
    DB_MSG(( "...ignoring loadMeth call - I don't know this class" ));
  }

  DB_MSG(( "<--STEAM:loadMeth( %s )", className ));
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/


