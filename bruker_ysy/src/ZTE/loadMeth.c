/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002-2004
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2003 Bruker BioSpin MRI GmbH";

#define DEBUG       0
#define DB_MODULE   0
#define DB_LINE_NR  0


#include "method.h"


void loadMeth(const char *className)
{
  DB_MSG(( "-->ZTE:loadMeth( %s )", className ));

  if (0 != className)
  {
      if (0 == strcmp( className, "MethodClass"))
      {
          /* handle old protocols */
          if (ParxRelsParHasValue("BandWidth"))
          {
            DB_MSG(( "Load and reset BandWidth"));
            PVM_EffSWh = BandWidth;
            ParxRelsResetPar("BandWidth");
            ParxRelsHideInFile("BandWidth");
          }
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

  DB_MSG(( "<--ZTE:loadMeth( %s )", className ));
}

/* ***************************************************************/
/*		E N D   O F   F I L E				 */
/* ***************************************************************/


