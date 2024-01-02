/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2008-2010
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/
static const char resid[] = "$Id$ (C) 2008-2010 Bruker BioSpin MRI GmbH";

#define DEBUG		     0
#define DB_MODULE	   1
#define DB_LINE_NR	 1


#include "method.h"

void loadMeth  (const char *	className)
{
  DB_MSG(( "-->ISIS:loadMeth( %s )", className ));


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
      }
  }
  else
  {
    DB_MSG(( "...ignoring loadMeth call - I don't know this class" ));
  }

  DB_MSG(( "<--ISIS:loadMeth( %s )", className ));
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/


