/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002-2010
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2002-2010 Bruker BioSpin MRI GmbH";

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
  DB_MSG(( "-->MDEFT:loadMeth( %s )\n", className ));



  if (0 != className)
  {
      if (0 == strcmp( className, "MethodClass"))
      {
          /* For PV5.1 protocols: Conversion preparation mode parameters */
          if(ParxRelsParHasValue("Mdeft_PreparationMode") == Yes)
          {
            if(Mdeft_PreparationMode==No_preparation) 
              PreparationMode=MDEFT_prep;
            if(Mdeft_PreparationMode==MDEFT_preparation) 
              PreparationMode=MDEFT_prep;
            if(Mdeft_PreparationMode==Inversion_preparation) 
              PreparationMode=Inversion_prep;
          }
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

  DB_MSG(( "<--MDEFT:loadMeth( %s )\n", className ));

}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/


