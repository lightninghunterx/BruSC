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

  /* set baselevel reconstruction parameter */
  /* default initialization of reco based on acqp pars allready set */
  
  ATB_InitUserModeReco(
      1,
      0,
      PVM_SpecMatrix, // acq size
      PVM_SpecMatrix, // image size
      NULL, //effAntiAlias=1,
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
      1);

  /* complex channel combination: */
  RecoCombineMode = AddImages;

  /* Setting phase offsets for multi RX channel combination */
  if(ACQ_scan_type != Setup_Experiment)
      ATB_ArrayPhaseSetRecoPhase();
  
 DB_MSG(("<--SetRecoParam\n"));
}

void DeriveReco(void)
{
  DB_MSG(("-->DeriveReco"));

  if (RecoUserUpdate == No)
  {
    DB_MSG(("<--RecoDerive: no update"));
    return;
  }

  ParxRelsParRelations("RecoUserUpdate", Yes);

  //Filter for ArrayPhase Adjustment
  ATB_GetRecoDataPoints(PVM_RecoDataPointsRequired,  PVM_EncNReceivers*RECO_inp_size[0]*2, "Q");

  DB_MSG(("<--DeriveReco"));
}
