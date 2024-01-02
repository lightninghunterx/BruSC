/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$(C) 2002 Bruker BioSpin MRI GmbH";


#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"

void SetRecoParam( void )
{
  DB_MSG(("-->SetRecoParam\n"));

  /* set baselevel reconstruction parameters */
  

  int dim = PTB_GetSpatDim();
  int echoImages = PVM_NEchoImages;
  int niout;

  /*specific Fws parameters*/
  ATB_SetFwsRecoParam(dim, PVM_Matrix, PVM_Fov, PVM_FrqWork[0], PVM_EffSWh);

  niout = NI/PVM_FwsLoop*PVM_FwsImages;

  ATB_InitUserModeReco(
    dim,
    0,
    PVM_EncMatrix,
    PVM_Matrix,
    PVM_AntiAlias,
    PVM_EncPftOverscans,
    niout,                 //N_outImages
    ACQ_obj_order,
    PVM_RareFactor,
    PVM_EncGenSteps1,
    PVM_EncGenSteps2,
    NULL,
    PVM_EncNReceivers,
    PVM_EncChanScaling,
    PVM_EncPpiRefLines[1],
    PVM_EncPpi[1],
    NI);                   //N_sortImages

  /*set dimension of RECO_rotate to handle cases where niout!=NI (e.g. FwsCombine)*/
  RecoObjectsPerRepetition=NI;
  ParxRelsParRelations("RECO_rotate",No);

  ATB_SetRecoRotate(
    NULL,
    PVM_Fov[0] * PVM_AntiAlias[0],
    NSLICES,
    echoImages,
    PVM_FwsLoop,           //outer loop
    0);

  ATB_SetRecoRotate(
    PVM_EffPhase1Offset,
    PVM_Fov[1] * PVM_AntiAlias[1],
    NSLICES,
    echoImages,
    PVM_FwsLoop,           //outer loop
    1);

  if (dim == 3)
  {
    ATB_SetRecoRotate(
      PVM_EffPhase2Offset,
      PVM_Fov[2] * PVM_AntiAlias[2],
      NSLICES,
      echoImages,
      PVM_FwsLoop,         //outer loop
      2);
  }

  /*reset to orig value after setting of RECO_rotate*/
  RecoObjectsPerRepetition=niout;


  for (int i = 0; i < dim; i++)
  {
    ATB_SetRecoPhaseCorr(50.0, 0.0, i);
  }

  ATB_SetRecoTranspositionFromLoops(
    "PVM_SliceGeo",
    dim,
    echoImages,
    niout);


   DB_MSG(("<--SetRecoParam\n"));
}

void RecoDerive (void)
{
  DB_MSG(("-->RecoDerive\n"));

  if (RecoUserUpdate == No)
  {
    DB_MSG(("<--RecoDerive: no update\n"));
    return;
  }

  int dim=PTB_GetSpatDim();

  
  /* standard settings for reconstruction */
  if(RecoPrototype == No)
  {
    SetRecoParam();

    //preparing FWS reco network
    if ((PVM_FwsRecoMode == FWS_SEPARATION || PVM_FwsRecoMode == FWS_COMBINATION) && ACQ_scan_type != Setup_Experiment)
    {
      RecoCombineMode = AddImages;
      RecoPreppassProcDim=RecoDim+1;

      /*Phase correction (checkerboard removal) is achieved by subtraction
        of reference phase (which has same checkerboard pattern)
        -> No explicit phase correction*/
      for(int n=0; n<dim; n++)
         RECO_pc_mode[n] = NO_PC;
    }
  }
  
  /* create standard reconstruction network */
  ParxRelsParRelations("RecoUserUpdate", Yes);

  /* modify for FatWaterSeparation */
  if (ACQ_scan_type != Setup_Experiment)
    ATB_FwsRecoDerive(dim, PVM_Matrix, PVM_Fov, NSLICES);

  DB_MSG(("<--RecoDerive\n"));
}
