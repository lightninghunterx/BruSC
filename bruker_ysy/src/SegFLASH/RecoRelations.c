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


static const char resid[] = "$Id$ (C) 2002 Bruker BioSpin MRI GmbH";

#define DEBUG           0
#define DB_MODULE       0
#define DB_LINE_NR      0

#include "method.h"

void SetRecoParam( void )
{
  DB_MSG(("-->SetRecoParam\n"));

  /* set baselevel reconstruction parameter */

  int dim = PTB_GetSpatDim();
  int echoImages = PVM_NMovieFrames;

  RecoDescriptionInSliceCount = PVM_NMovieFrames;
  RecoDescriptionSliceCount = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  RecoDescriptionOutSliceCount = 1;

  ATB_InitUserModeReco(
    dim,
    0,
    PVM_EncMatrix,
    PVM_Matrix,
    PVM_AntiAlias,
    PVM_EncPftOverscans,
    NI,
    ACQ_obj_order,
    SegmentSize,
    PVM_EncGenSteps1,
    PVM_EncGenSteps2,
    NULL,
    PVM_EncNReceivers,
    PVM_EncChanScaling,
    dim>1? PVM_EncPpiRefLines[1]:0,
    dim>1? PVM_EncPpi[1]:0);

  JOBPARAMETERS(jobParameters);
  int idx=jobParameters->getIndex("job0");
  RecoObjectsPerSetupStep = ACQ_jobs[idx].transactionBlocks;


  ATB_SetRecoRotate(
    NULL,
    PVM_Fov[0] * PVM_AntiAlias[0],
    NSLICES,
    echoImages,
    0);

  if(dim>1)
  {
    ATB_SetRecoRotate(
      PVM_EffPhase1Offset,
      PVM_Fov[1] * PVM_AntiAlias[1],
      NSLICES,
      echoImages,
      1);
  }

  if (dim == 3)
  {
    ATB_SetRecoRotate(
      PVM_EffPhase2Offset,
      PVM_Fov[2] * PVM_AntiAlias[2],
      NSLICES,
      echoImages,
      2);
  }

  if(RecoMethMode != SWI)
  {
    for (int i = 0; i < dim; i++)
    {
      ATB_SetRecoPhaseCorr(50.0, 0.0, i);
    }
  }

  ATB_SetRecoTranspositionFromLoops(
    "PVM_SliceGeo",
    dim,
    PVM_NMovieFrames,
    NI);


  /* special settings */

  RecoMethModeVisPar();
  
  if (RecoMethMode == SWI && WeightingMode == phase_image)
  {
    RecoCombineMode = AddImages;
    RECO_image_type = PHASE_IMAGE;
    ParxRelsParRelations("RECO_image_type", Yes);
  }

  DB_MSG(("<--SetRecoParam\n"));
}

void RecoDerive (void)
{
  DB_MSG(("-->RecoDerive\n"));
  
  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  if (RecoUserUpdate == No)
  {
    DB_MSG(("<--RecoDerive: no update\n"));
    return;
  }

  /* standard settings for reconstruction */
  if(RecoPrototype == No)
    SetRecoParam();  

  /* create standard reconstruction network */
  ParxRelsParRelations("RecoUserUpdate", Yes);

  if ( SegmentSliceOrder == Serial && nSlices > 1)
    SortSerialData();

  /* modify for SWI */
  if (RecoMethMode == SWI && ACQ_scan_type != Setup_Experiment)
  {
    SWIRecoRel();
  }

  DB_MSG(("<--RecoDerive\n"));
}


void SortSerialData(void)
{
  DB_MSG(("-->SortSerialData\n"));

  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  PARX_change_dims("SortArgs", 2);
  for (int i=0; i<2; i++) SortArgs[i]=0;

  PARX_change_dims("RecoReformatSize", RecoSortDim+1);
  RecoReformatSize[0] = RECO_ft_size[0];
  RecoReformatSize[1] = PVM_EncGenTotalSteps;
  if (RecoSortDim > 1) RecoReformatSize[2] = RecoSortSize[1];
  if (RecoSortDim > 2) RecoReformatSize[3] = RecoSortSize[2];
  
  PARX_change_dims("RecoReformatSerialData", 3);
  RecoReformatSerialData[0] = RECO_ft_size[0];
  RecoReformatSerialData[1] = SegmentSize * PVM_NMovieFrames * nSlices;
  if (PVM_SpatDim == 2)
    RecoReformatSerialData[2] = RecoSortSize[0] * RecoSortSize[1] / RecoReformatSerialData[1];
  if (PVM_SpatDim > 2)
    RecoReformatSerialData[2] = PVM_EncGenTotalSteps / SegmentSize;

  PARX_change_dims("RecoSortSerialData" , 2);
  RecoSortSerialData[0]  = SegmentSize * PVM_NMovieFrames;
  RecoSortSerialData[1]  = 0;

  char arg1[RECOSTAGENODESIZE];
  snprintf(arg1,RECOSTAGENODESIZE,"sortDim = 1; sortSize = {0}; sortRange = {0}; sortMaps= {0} ; sortSegment = {0} ; procDim = 2");
  char arg2[RECOSTAGENODESIZE];
  snprintf(arg2,RECOSTAGENODESIZE,"newSizes=<RecoReformatSerialData>; procDim=3");
  char arg3[RECOSTAGENODESIZE];
  snprintf(arg3,RECOSTAGENODESIZE,"sortDim = 2; sortSize=<SortArgs>; sortRange=<SortArgs>; sortMaps=<SortArgs>; sortSegment=<RecoSortSerialData>; procDim=3; ");
  char arg4[RECOSTAGENODESIZE];
  snprintf(arg4,RECOSTAGENODESIZE,"newSizes=<RecoReformatSize>; procDim=%d", PVM_SpatDim > 2 ? 4:3);

 
  if (ACQ_scan_type == Setup_Experiment)
  {
      DB_MSG(("<--SortSerialData\n"));
      return;
  }
  else if (ACQ_scan_type != Setup_Experiment)
    {

    for (int n = 0; n < RecoNrActiveReceivers(); n++)
    {
      RecoComputeInsertStage( RECOFIRSTPASS, n, "REORD", "RecoSortFilter"    , "nosort"                   , arg1 );
      RecoComputeInsertStage( RECOFIRSTPASS, n, "REORD", "RecoReformatFilter", "reformat"                 , arg2 );
      RecoComputeInsertStage( RECOFIRSTPASS, n, "REORD", "RecoSortFilter"    , "sort_Serial_to_Alternate" , arg3 );
      RecoComputeInsertStage( RECOFIRSTPASS, n, "REORD", "RecoReformatFilter", "reformat_for_reord"       , arg4 );
    }

  }

  DB_MSG(("<--SortSerialData\n"));

}


void SWIRecoRel (void)
{
  DB_MSG(("-->SWIRecoRel"));

  char arg0[RECOSTAGENODESIZE], arg1[RECOSTAGENODESIZE], arg2[RECOSTAGENODESIZE], argSWI[RECOSTAGENODESIZE];
  double delta[3]={0.0,0.0,0.0}, tau[3]={0.0,0.0,0.0}, gamma[3]={0.0,0.0,0.0};
  int dim = PTB_GetSpatDim();

  /* parameters for RecoGaussWinMultFilter: exp(-delta*t - tau*t^2 -gamma)
     tau = (N*pi*broadening/(2*FOV))^2
     delta = -2*tau*t0
     gamma = tau * t0^2 */

  tau[0] = pow((PVM_Matrix[0]*M_PI*GaussBroadening/(2*PVM_Fov[0])),2);  
  delta[0] = -2.0*tau[0]*0.5;
  gamma[0] = tau[0]*pow(0.5, 2);

  for(int i=1;i<dim;i++)
  {
    tau[i] = pow((PVM_Matrix[i]*M_PI*GaussBroadening/(2*PVM_Fov[i])),2);
    delta[i] = -tau[i]; 
    gamma[i] = 0.25*tau[i];
  }
 

  snprintf(arg0,RECOSTAGENODESIZE,"winDirection=0;delta=%f;tau=%f;gamma=%f",delta[0],tau[0],gamma[0]);
  snprintf(arg1,RECOSTAGENODESIZE,"winDirection=1;delta=%f;tau=%f;gamma=%f",delta[1],tau[1],gamma[1]);
  snprintf(arg2,RECOSTAGENODESIZE,"winDirection=2;delta=%f;tau=%f;gamma=%f",delta[2],tau[2],gamma[2]);

  switch (WeightingMode)
  {
    case positive_mask:  snprintf(argSWI,RECOSTAGENODESIZE,"mask=1;weighting=%f",MaskWeighting); break;
    case negative_mask:  snprintf(argSWI,RECOSTAGENODESIZE,"mask=2;weighting=%f",MaskWeighting); break;
    case magnitude_mask: snprintf(argSWI,RECOSTAGENODESIZE,"mask=3;weighting=%f",MaskWeighting); break;
    case phase_image:    snprintf(argSWI,RECOSTAGENODESIZE,"mask=4;weighting=%f",MaskWeighting); break;
  }

  /* apply gauss filtering and phase weighting to data */

  const int nrReceivers = RecoNrActiveReceivers();

  for (int n = 0; n < nrReceivers; n++)
  {
    if (nrReceivers > 1)
    {
      char position[RECOSTAGENODESIZE], stage[RECOSTAGENODESIZE];
      snprintf(position,RECOSTAGENODESIZE, "SOS0.%d", n);
      snprintf(stage,RECOSTAGENODESIZE, "TEE%d", n);
      RecoComputeInsertStage(RECOPREPPASS,-1,position,"RecoTeeFilter",stage,"");
    }
    else
    {
      RecoComputeInsertStage(RECOPREPPASS,n,DEFAULTIMAGETYPEFILTERNAME,"RecoTeeFilter","TEE","");
    }
    RecoComputeAppendStage(RECOPREPPASS,n,"TEE","RecoPhaseWeightingFilter","SWI",argSWI);
    RecoComputeConnectStages(RECOPREPPASS,n,"TEE","SWI.mask");
    RecoComputeInsertStage(RECOPREPPASS,n,"SWI.mask",RECOFTFILTER,"IFT0","direction=0;exponent=-1");
    RecoComputeInsertStage(RECOPREPPASS,n,"SWI.mask",RECOFTFILTER,"IFT1","direction=1;exponent=-1");
    if (dim == 3)
    {
      RecoComputeInsertStage(RECOPREPPASS,n,"SWI.mask",RECOFTFILTER,"IFT2","direction=2;exponent=-1");
    }
    RecoComputeInsertStage(RECOPREPPASS,n,"SWI.mask",RECOGAUSSWINMULTFILTER,"GAUSS0",arg0);
    RecoComputeInsertStage(RECOPREPPASS,n,"SWI.mask",RECOGAUSSWINMULTFILTER,"GAUSS1",arg1);
    if (dim == 3)
    {
      RecoComputeInsertStage(RECOPREPPASS,n,"SWI.mask",RECOGAUSSWINMULTFILTER,"GAUSS2",arg2);
    }
    RecoComputeInsertStage(RECOPREPPASS,n,"SWI.mask",RECOFTFILTER,"FT0","direction=0;exponent=1");
    RecoComputeInsertStage(RECOPREPPASS,n,"SWI.mask",RECOFTFILTER,"FT1","direction=1;exponent=1");
    if (dim == 3)
    {
      RecoComputeInsertStage(RECOPREPPASS,n,"SWI.mask",RECOFTFILTER,"FT2","direction=2;exponent=1");
    }

  }

  DB_MSG(("<--SWIRecoRel"));
}

