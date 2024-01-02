/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999 - 2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Locker$
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id: RecoRelations.c,v 1.1.2.2 2011/02/16 DGEM ";

#define DEBUG           0
#define DB_MODULE       1
#define DB_LINE_NR      1

/*:=INFO=:*******************************************************
 *
 * Description :
 *   it contains functions necessary for responding to a 'load'
 *      of this method. This function will be automatically
 *      called in response to a method file for this method
 *      having been read.
 *
 *::=info=:******************************************************/

/****************************************************************/
/****************************************************************/
/*              I N T E R F A C E   S E C T I O N               */
/****************************************************************/
/****************************************************************/

/****************************************************************/
/*              I N C L U D E   F I L E S                       */
/****************************************************************/


#include "method.h"


void SetRecoParam(int acceleration)
{
  DB_MSG(("Entering SetRecoParam"));

  ATB_EpiSetRecoPars(acceleration, PVM_NEchoImages, 1);

  RecoEachNR = No;
  /*otherwise, scaling is based on first image,
    which may result in overflow for others*/

  DB_MSG(("Exiting SetRecoParam"));
}

/* Relation of RecoUserUpdate, called at start of reconstruction
   to set reconstrution process */

void RecoDerive(void)
{
  DB_MSG(("-->RecoDerive\n"));

  /* standard settings for reconstruction */
  if(RecoPrototype == No && PVM_EpiAdjustMode != 2) //do not call in grappa adj
  {
     SetRecoParam(PVM_EncPpi[1]);
  }

  ATB_EpiSetRecoProcess(
    PVM_EncNReceivers, 
    PVM_EncCentralStep1,
    PVM_NEchoImages
  );

  DB_MSG(("<--RecoDerive\n"));
}
