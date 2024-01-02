/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id: RecoRelations.c,v 1.1.2.2 2001/01/11 MAWI ";

#define DEBUG           0
#define DB_MODULE       1
#define DB_LINE_NR      1



#include "method.h"

void SetRecoParam( void )
{
  DB_MSG(("-->SetRecoParam\n"));
 
  int dim = 1;
  
  /* set baselevel reconstruction parameter */
  /* default initialization of reco based on acqp pars allready set */
  
  ATB_InitUserModeReco(
    dim,
    0,
    //    PVM_EncMatrix, // acq size
    PVM_Matrix, // acq size
    PVM_Matrix, // image size
    PVM_AntiAlias,
    NULL, //effPftOverscans=halfsize,
    NI,
    ACQ_obj_order,
    1,
    NULL,  //linear order,
    NULL,  //linear order (not used)
    NULL,  //linear order (not used)
    PVM_EncNReceivers,
    PVM_EncChanScaling,
    0,
    0);
  
  
  ATB_SetRecoRotate(
    NULL,
    PVM_Fov[0] * PVM_AntiAlias[0],
    NSLICES,
    1,
    0);

 
  DB_MSG(("<--SetRecoParam\n"));
}

