/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2006
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $SId$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2006 Bruker BioSpin MRI GmbH";

#define DEBUG           0
#define DB_MODULE       0
#define DB_LINE_NR      0

#include "method.h"

void SetRecoParam( void )
{
  DB_MSG(("-->SetRecoParam"));

  /* set baselevel reconstruction parameter */
  
  if(FlowMode == FourierFlowImaging)
    RecoMethMode = FLOWMAP;  

  int dim = PTB_GetSpatDim();
  int acqDim = FlowMode == FourierFlowImaging ? dim+1 : dim;
  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  int movieframes = RecoMethMode == FLOWMAP ? PVM_NMovieFrames * FlowImages:PVM_NMovieFrames * FlowEncLoop;
  int ni = nSlices * movieframes;
  int sortni=NI;

  RecoDescriptionInSliceCount = movieframes;
  RecoDescriptionSliceCount = nSlices;
  RecoDescriptionOutSliceCount = 1;
  
  int* acq_obj_order = new int[movieframes*nSlices];

  int j = 0;
  while(j< movieframes)
  {
    for(int i=0;i<nSlices;i++)
    {
      int k=j*nSlices+i;
      acq_obj_order[k] = PVM_ObjOrderList[i] * movieframes + j;
    }
    j=j+1;
  }

  if (FlowMode == FourierFlowImaging)
  {
    int aqarr[3];
    int ftSize[3];
    int pftOverscans[3];
 
    memcpy(aqarr,PVM_EncMatrix,dim*sizeof(int));  /* copy PVM_EncMatrix to aqarr */
    aqarr[dim] = FlowEncLoop;
    for(int i=0; i<dim; i++)
      ftSize[i] = (int)(PVM_Matrix[i]*PVM_AntiAlias[i]);
    ftSize[dim]= (int) (FlowEncLoop*FlowZeroFillFactor);
    memcpy(pftOverscans,PVM_EncPftOverscans,dim*sizeof(int));
    pftOverscans[dim]=FlowEncLoop/2;
    
    ATB_InitUserModeReco(
      acqDim,
      0,
      aqarr,
      ftSize,
      NULL,
      pftOverscans,
      NI,
      ACQ_obj_order,
      1,
      PVM_EncSteps1,
      NULL,
      NULL,
      PVM_EncNReceivers,
      PVM_EncChanScaling,
      PVM_EncPpiRefLines[1],
      PVM_EncPpi[1]);
  }
  else
  {
    ATB_InitUserModeReco(
      dim,
      0,
      PVM_EncMatrix,
      PVM_Matrix,
      PVM_AntiAlias,
      PVM_EncPftOverscans,
      ni,
      ACQ_obj_order,
      1,
      PVM_EncSteps1,
      PVM_EncSteps2,
      NULL,
      PVM_EncNReceivers,
      PVM_EncChanScaling,
      PVM_EncPpiRefLines[1],
      PVM_EncPpi[1],
      sortni);
  }

 
  ATB_SetRecoRotate(
    NULL,
    PVM_Fov[0] * PVM_AntiAlias[0],
    NSLICES,
    movieframes, 
    0);

  ATB_SetRecoRotate(
    PVM_EffPhase1Offset,
    PVM_Fov[1] * PVM_AntiAlias[1],
    NSLICES,
    movieframes,
    1);

  if (dim == 3)
  {    
    ATB_SetRecoRotate(
      PVM_EffPhase2Offset, 
      PVM_Fov[2] * PVM_AntiAlias[2],
      NSLICES,     
      movieframes,
      2);
  }

  for (int i = 0; i < dim; i++)
  {
    ATB_SetRecoPhaseCorr(50.0, 0.0, i);
  }
     
  
  ATB_SetRecoTranspositionFromLoops(
    "PVM_SliceGeo",
    dim,
    PVM_NMovieFrames*FlowImages,
    ni);


  // release memory for local object
  delete[] acq_obj_order;

  /*** special settings ***/
  
  if ((FlowMode == VelocityMapping) ||
      (FlowMode == PhaseContrastAngiography && FlowEncodingDirection == AllDirections && RecoMethMode == FLOWMAP))
  {
    RecoCombineMode = AddImages;
  }
  else
  {
    RecoCombineMode = SumOfSquares;
  }

  if (FlowMode == VelocityMapping && RecoMethMode == FLOWMAP)
  {
    RECO_image_type = PHASE_IMAGE;
    ParxRelsParRelations("RECO_image_type",Yes);
    RECO_map_mode = USER_RANGE_MAPPING;
    ParxRelsParRelations("RECO_map_mode",Yes);
    RECO_map_range[0] = -FlowRange;
    RECO_map_range[1] =  FlowRange;
  }
  else
  {
    RECO_image_type = MAGNITUDE_IMAGE;
    ParxRelsParRelations("RECO_image_type",Yes);
    RECO_map_mode = ABSOLUTE_MAPPING;
    ParxRelsParRelations("RECO_map_mode",Yes);
  }

  RecoMethModeVisPar();
  
  DB_MSG(("<--SetRecoParam\n"));

}

void RecoDerive (void)
{
  DB_MSG(("-->RecoDerive"));

  if (RecoUserUpdate == No)
  {
    DB_MSG(("<--RecoDerive: no update"));
    return;
  }

  if (ACQ_scan_type == Setup_Experiment)
  {   
    if (FlowMode != FourierFlowImaging)
    {
      FlowImages=FlowEncLoop;
      SetRecoParam();
      RECO_image_type = MAGNITUDE_IMAGE;
      ParxRelsParRelations("RECO_image_type",Yes);
      RECO_map_mode = ABSOLUTE_MAPPING;
      ParxRelsParRelations("RECO_map_mode",Yes);
    }
    RecoObjectsPerRepetition=NI;
    ParxRelsParRelations("RecoObjectsPerRepetition",Yes);
    /* create standard reconstruction network */
    ParxRelsParRelations("RecoUserUpdate", Yes);
  
    DB_MSG(("<--RecoDerive: setup experiment"));
    return;
  }
 
  /* standard settings for reconstruction */
  if(RecoPrototype == No) 
    SetRecoParam();

  /* set up default network */
  ParxRelsParRelations("RecoUserUpdate", Yes);
  
  if (FlowMode == FourierFlowImaging || RecoMethMode == Magnitude)
  {
    DB_MSG(("<--RecoDerive: FourierFlowImaging"));
    return;
  }

  const int nrReceivers = RecoNrActiveReceivers();
  char cmdComb[RECOSTAGENODESIZE];
  sprintf(cmdComb, "procDim=%d;keepBlocksize=true", PTB_GetSpatDim() + 1);

  /* Find out if cutoff filters are present (anti aliasing).
   * If so, the split filter must be inserted in front of the cutoff filter(s) */
  
  YesNo cutoffNeeded = RecoRegridN == On ? Yes : No;

  int dim = PTB_GetSpatDim();
  int recoDim = FlowMode == FourierFlowImaging ? dim+1 : dim;

  if (cutoffNeeded == No)
  {
    for (int i = 0; i < recoDim; i++)
    {
      if (RECO_size[i] != RECO_ft_size[i])
      {
        cutoffNeeded = Yes;
        break;
      }
    }
  }
  DB_MSG(("cutoffNeeded = %s",(cutoffNeeded==Yes)?"Yes":"No"));

  /* split different images */
  if (nrReceivers > 1)
  {
    for (int n = 0; n < nrReceivers; n++)
    {
      char position[RECOSTAGENODESIZE], stage[RECOSTAGENODESIZE];
      if (cutoffNeeded==Yes)
        sprintf(position, "CUT%d", n);
      else
        sprintf(position, "SOS0.%d", n);          
      sprintf(stage, "SP%d", n);
      DB_MSG(("Position = %s",position));
      RecoComputeInsertStage(RECOPREPPASS,-1,position,RECOSPLITFILTER,stage,"keepBlocksize=true");
    }
  }
  else
  {
    if (cutoffNeeded==Yes)
      RecoComputeInsertStage(RECOPREPPASS,0,"CUT",RECOSPLITFILTER,"SP","keepBlocksize=true");
    else
      RecoComputeInsertStage(RECOPREPPASS,0,DEFAULTIMAGETYPEFILTERNAME,RECOSPLITFILTER,"SP","keepBlocksize=true");        
  }

  /* remove image type filter */
  if (!(FlowMode == PhaseContrastAngiography && FlowEncodingDirection != AllDirections && nrReceivers == 1))
  {
    RecoComputeRemoveStage(RECOPREPPASS,0,DEFAULTIMAGETYPEFILTERNAME);
  }

  if (FlowMode == PhaseContrastAngiography)
  {
    DB_MSG(("PhaseContrastAngiography"));

    if (FlowEncodingDirection != AllDirections)
    {
      DB_MSG(("!AllDirections"));

      for (int n = 0; n < nrReceivers; n++)
      {
        /* PhaseContrastAngiography (1D):
           Calculation of S = S1 - S2
           Output: Image1 = S, Image2 = Mag(S1) */

        RecoComputeAppendStage(RECOPREPPASS,n,"SP","RecoAddFilter","ADD","");
        DB_MSG(("ADD filter %d added", n));
        RecoComputeAddStage(RECOPREPPASS,n,RECOTEEFILTER,"TEE","");
        RecoComputeConnectStages(RECOPREPPASS,n,"SP","TEE");
        RecoComputeAddStage(RECOPREPPASS,n,RECOSCALEFILTER,"SCAL","scaleFactor=-1.0");
        RecoComputeConnectStages(RECOPREPPASS,n,"TEE","SCAL");
        RecoComputeConnectStages(RECOPREPPASS,n,"SCAL","ADD");
        RecoComputeAppendStage(RECOPREPPASS,n,"ADD",RECOCOMBINEFILTER,"COM",cmdComb);
        DB_MSG(("COM filter %d added", n));
        RecoComputeConnectStages(RECOPREPPASS,n,"TEE","COM");
      }
    }
    else
    {
      DB_MSG(("AllDirections"));

      /* PhaseContrastAngiography (3D: Hadamard Encoding):
         Calculation of S ~ sqrt(vx2 + vy2 + vz2)
         Ouput: Image1 = Angiogram,
                Image2 = Magnitude of stationary signal */

      for (int n = 0; n < nrReceivers; n++)
      {
        /* Hadamard Encoding: linear data combination */
        RecoComputeAppendStage(RECOPREPPASS,n,"SP","RecoPCAFilter","PCA","");
        RecoComputeConnectStages(RECOPREPPASS,n,"SP","PCA");
        RecoComputeConnectStages(RECOPREPPASS,n,"SP","PCA");
        RecoComputeConnectStages(RECOPREPPASS,n,"SP","PCA");
        /* SumOfSquares combination of x-,y-, and z-component */
        RecoComputeAppendStage(RECOPREPPASS,n,"PCA",RECOSUMOFSQUARESFILTER,"SOSVEL","");
        RecoComputeConnectStages(RECOPREPPASS,n,"PCA","SOSVEL");
        RecoComputeConnectStages(RECOPREPPASS,n,"PCA","SOSVEL");
        RecoComputeAddStage(RECOPREPPASS,n,RECOMAGNITUDEFILTER,"MAG","");
        RecoComputeConnectStages(RECOPREPPASS,n,"PCA","MAG");
        RecoComputeAppendStage(RECOPREPPASS,n,"SOSVEL",RECOCOMBINEFILTER,"COM",cmdComb);
        RecoComputeConnectStages(RECOPREPPASS,n,"MAG","COM");
 
      }
    }
  }

  if (FlowMode == VelocityMapping)
  {
    DB_MSG(("VelocityMapping"));

    char cmdScale[RECOSTAGENODESIZE], cmdPhase[RECOSTAGENODESIZE];
    sprintf(cmdScale,"scaleFactor=%f", FlowRange / M_PI);
    sprintf(cmdPhase,"relativeNoise=%f", SignalMask / 100.0);

    if (FlowEncodingDirection != AllDirections)
    {
      DB_MSG(("!AllDirections"));

      /* VelocityMapping (1D):
         Calculation of S = (S1 * S2*)
         Output: Image = Phase(S)  */

      for (int n = 0; n < nrReceivers; n++)
      {
        RecoComputeAppendStage(RECOPREPPASS,n,"SP",RECOMULTFILTER,"MUL","");
        RecoComputeAddStage(RECOPREPPASS,n,RECOCONJQNEGFILTER,"CON","direction=0;conjugate=true;negate=false");
        RecoComputeConnectStages(RECOPREPPASS,n,"SP","CON");
        RecoComputeConnectStages(RECOPREPPASS,n,"CON","MUL");
      }
      
      /* phase calculation */
      RecoComputeAppendStage(RECOPREPPASS,0,nrReceivers > 1 ? "SOS" : "MUL","RecoPhaseFilter","PHASE",cmdPhase);
      /* scaling: v[cm/s] */
      RecoComputeAppendStage(RECOPREPPASS,0,"PHASE",RECOSCALEFILTER,"SCL",cmdScale);
    }
    else
    {
      DB_MSG(("AllDirections"));

      /* VelocityMapping (3D: Hadamard Encoding):
         Calculation of v(read), v(phase), v(slice)
         Output: Image1 = v(read), Image2 = v(phase),
                 Image3 = v(slice) */

      for (int n = 0; n < nrReceivers; n++)
      {
        /* Hadamard Encoding: linear data combination */
        RecoComputeAppendStage(RECOPREPPASS,n,"SP","RecoFlowFilter","FLOW","");
        RecoComputeConnectStages(RECOPREPPASS,n,"SP","FLOW");
        RecoComputeConnectStages(RECOPREPPASS,n,"SP","FLOW");
        RecoComputeConnectStages(RECOPREPPASS,n,"SP","FLOW");
      }

      if (nrReceivers > 1)
      {
        /* combine images, combine receivers, and split images again */
        for (int n = 0; n < nrReceivers; n++)
        {
          RecoComputeAppendStage(RECOPREPPASS,n,"FLOW",RECOCOMBINEFILTER,"COM","");
          RecoComputeConnectStages(RECOPREPPASS,n,"FLOW","COM");
          RecoComputeConnectStages(RECOPREPPASS,n,"FLOW","COM");
          RecoComputeConnectStages(RECOPREPPASS,n,"FLOW","COM");
          if(cutoffNeeded==Yes)
            RecoComputeRemoveStage(RECOPREPPASS,n,"CUT"); // move cutoff behind the second split filter
        }
        /* RecoCombineMode = AddImages(called SOS in reco network) */
        RecoComputeAppendStage(RECOPREPPASS,0,"SOS",RECOSPLITFILTER,"SPLIT","keepBlocksize=false");
        if(cutoffNeeded==Yes)
        {
          char cmdCut[RECOSTAGENODESIZE];
          sprintf(cmdCut, "sizes=<RECO_size>; frameDim=%d; offset=", recoDim);
          if (RecoRegridN == Off)
          {
            strcat(cmdCut, "<RECO_offset>;");
          }
          else
          {
            strcat(cmdCut, "<RecoRegridNOffset>;");
          }
          RecoComputeAppendStage(RECOPREPPASS,0,"SPLIT",RECOCUTOFFFILTER,"CUT",cmdCut);
        }
      }

      const char* filterOut = nrReceivers > 1 ? "SPLIT" : "FLOW";

      /* phase calculation */
      RecoComputeAppendStage(RECOPREPPASS,0,filterOut,"RecoPhaseFilter","PHASE1",cmdPhase);
      RecoComputeAddStage(RECOPREPPASS,0,"RecoPhaseFilter","PHASE2",cmdPhase);
      RecoComputeAddStage(RECOPREPPASS,0,"RecoPhaseFilter","PHASE3",cmdPhase);
      RecoComputeAddStage(RECOPREPPASS,0,RECOMAGNITUDEFILTER,"MAG","");
      RecoComputeConnectStages(RECOPREPPASS,0,filterOut,"PHASE2");
      RecoComputeConnectStages(RECOPREPPASS,0,filterOut,"PHASE3");
      RecoComputeConnectStages(RECOPREPPASS,0,filterOut,"MAG");
      RecoComputeAddStage(RECOPREPPASS,0,"RecoDummySink","SINK","");
      RecoComputeConnectStages(RECOPREPPASS,0,"MAG","SINK");
      /* combine */
      RecoComputeAppendStage(RECOPREPPASS,0,"PHASE1",RECOCOMBINEFILTER,"COMB",cmdComb);
      RecoComputeConnectStages(RECOPREPPASS,0,"PHASE2","COMB");
      RecoComputeConnectStages(RECOPREPPASS,0,"PHASE3","COMB");
      /* scaling: v[cm/s] */
      RecoComputeAppendStage(RECOPREPPASS,0,"COMB",RECOSCALEFILTER,"SCL",cmdScale);
    }
 
  }

  DB_MSG(("<--RecoDerive"));
}

void RecoMethModeVisPar(void)
{
  DB_MSG(("-->RecoMethModeVisPar"));

  if(RecoMethMode == FLOWMAP && FlowMode == VelocityMapping)
    ParxRelsShowInEditor("SignalMask"); 
  else if (RecoMethMode == FLOWMAP && FlowMode == FourierFlowImaging)
    ParxRelsShowInEditor("FlowZeroFillFactor");
  else
    ParxRelsHideInEditor("SignalMask,FlowZeroFillFactor");
  
  DB_MSG(("<--RecoMethModeVisPar"));
}
