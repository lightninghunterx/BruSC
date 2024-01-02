/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002 - 2010
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$(C) 2002 - 2010 Bruker BioSpin MRI GmbH";


#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"


/*:=MPB=:=======================================================*
 *
 * Global Function: loadMeth
 *
 * Description: This procedure is automatically called in
 *	response to a method file for this method being read.
 *
 * Error History: 
 *
 * Interface:							*/

void loadMeth(const char *	className)
/*:=MPE=:=======================================================*/
{
  DB_MSG(( "-->RAREst:loadMeth( %s )\n", className ));

  
  if (0 != className)
  {
      if ( 0 == strcmp( className, "MethodClass" )) 
      {
          MRT_CheckOperationMode();
          initMeth();
      }
      else if (0 == strcmp( className, "MethodRecoGroup"))
      {
          /* after loading of methreco file. */
      }
  }
  else
  {
    DB_MSG(( "...ignoring loadMeth call - I don't know this class" ));
  }
  
  DB_MSG(( "<--RAREst:loadMeth( %s )\n", className ));
  
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/