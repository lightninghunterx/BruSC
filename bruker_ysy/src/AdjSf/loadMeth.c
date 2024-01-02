/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2005-2015
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

const char  * AdjSf_loadMeth_id = "$Id$ (C) 2005-2015 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0


#include "method.h"


void loadMeth(const char * groupName)
{
  DB_MSG(( "-->AdjSf:loadMeth( %s )\n", groupName ));


  if (0 != groupName)
  {
      if (0 == strcmp( groupName, "MethodClass") || 
          0 == strcmp("MethodProtocolGroup", groupName))
      {
          initMeth();
      }
      else if (0 == strcmp(groupName, "MethodRecoGroup"))
      {
          DB_MSG(("...responding to loadMeth call for MethodRecoGroup."));
      }
  }
  else
  {
    DB_MSG(( "...ignoring loadMeth call - I don't know this class" ));
  }

  DB_MSG(( "<--AdjSf:loadMeth( %s )\n", groupName ));
}

void methodProtocolPostRead(bool readOnly) 
{ 
    if (!readOnly) 
        loadMeth("MethodProtocolGroup"); 
};

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/


