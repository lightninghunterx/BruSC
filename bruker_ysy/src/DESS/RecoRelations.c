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
#define DB_MODULE       1
#define DB_LINE_NR      1



#include "method.h"

void SetRecoParam( void )
{
  DB_MSG(("-->SetRecoParam\n"));

  /* set baselevel reconstruction parameter */
  
  int dim = PTB_GetSpatDim();
  
  if(dim<3 && RecoMethMode==FieldMap)
    RecoMethMode = Default;
  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );  
  int ni = RecoMethMode == Default ? NI:FieldMapImages*nSlices;
  int echoImages = RecoMethMode == Default ? PVM_NEchoImages:1;
  int phaseFactor = dim == 2 ? PVM_EncMatrix[1]:1;
  int sortni=NI;

  RecoDescriptionInSliceCount = echoImages;   
  RecoDescriptionSliceCount = nSlices;
  RecoDescriptionOutSliceCount = 1;

  ATB_InitUserModeReco(
    dim,
    0,
    PVM_EncMatrix,
    PVM_Matrix,
    PVM_AntiAlias,
    PVM_EncPftOverscans,
    ni,
    ACQ_obj_order,
    phaseFactor,
    PVM_EncSteps1,
    PVM_EncSteps2,
    NULL,
    PVM_EncNReceivers,
    PVM_EncChanScaling,
    dim>1? PVM_EncPpiRefLines[1]:0,
    dim>1? PVM_EncPpi[1]:0,
    sortni);

  
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
 

  if(1 /*RecoMethMode != SWI*/)
  {
    for (int i = 0; i < dim; i++)
    {
      ATB_SetRecoPhaseCorr(50.0, 0.0, i);
    }
  }

  ATB_SetRecoTranspositionFromLoops(
    "PVM_SliceGeo",
    dim,
    PVM_NEchoImages,
    ni);


  

  /* special settings */
  
  RECO_wordtype = _32BIT_SGN_INT;
  ParxRelsParRelations("RECO_wordtype",Yes);
  if(RecoMethMode == FieldMap)
  {
    RecoCombineMode = AddImages;
    ParxRelsShowInEditor("MapSnr");
  }
  else
  {
    RecoCombineMode=SumOfSquares;
    ParxRelsHideInEditor("MapSnr");
  }

  DB_MSG(("<--SetRecoParam\n"));
}


void RecoDerive (void)
{
  DB_MSG(("-->RecoDerive"));

  if (RecoUserUpdate == No)
  {
    DB_MSG(("<--RecoDerive: no update\n"));
    return;
  }
  
  if (ACQ_scan_type == Setup_Experiment)
  {
    RecoObjectsPerRepetition=NI;
    ParxRelsParRelations("RecoObjectsPerRepetition",Yes);
    FieldMapImages=NI;
    RecoMethMode=Default;
    SetRecoParam();
    /* create standard reconstruction network */
    ParxRelsParRelations("RecoUserUpdate", Yes);
    DB_MSG(("<--RecoDerive: setup experiment"));
    return;
  }
  
  FieldMapImages=1;
  
  if(RecoMethMode != Default)
    SetRecoParam();

  /* create standard reconstruction network */
  /* ParxRelsParRelations("RecoUserUpdate", Yes);
     workaround to handle procDim
  */
  {
    RecoDeriveInputProcess();
    RecoDeriveComputeProcess(GrappaProcStandard, Yes);
    RecoDeriveOutputProcess();
  }
  

  const int dim = PTB_GetSpatDim();
  int phaseFactor = dim == 2 ? PVM_EncMatrix[1]:1;
  
  if(dim<3)
  {
    const int nrReceivers = RecoNrActiveReceivers();
       
    char arg1[RECOSTAGENODESIZE];
    snprintf(arg1,RECOSTAGENODESIZE,"newSizes={%d,%d,%d}; procDim=3",PVM_EncMatrix[0],PVM_EncMatrix[1]*2,NI/2);
    char arg2[RECOSTAGENODESIZE];
    snprintf(arg2,RECOSTAGENODESIZE,"dim=1;blockCnt=%d;keepBlocksize=false;",phaseFactor);
    char arg3[RECOSTAGENODESIZE];
    snprintf(arg3,RECOSTAGENODESIZE,"newSizes={%d,%d,%d}; procDim=3",PVM_EncMatrix[0],PVM_EncMatrix[1],NI);
 
    for (int n = 0; n < nrReceivers; n++)
    {
      RecoComputeAppendStage(RECOFIRSTPASS,n,"CAST","RecoReformatFilter","REFORMAT1",arg1);
      RecoComputeAppendStage(RECOFIRSTPASS,n,"REFORMAT1","RecoSplitFilter","SPLIT","dim=1;blockCnt=1;keepBlocksize=true;");
      RecoComputeAppendStage(RECOFIRSTPASS,n,"SPLIT","RecoCombineFilter","COMB",arg2); 
      RecoComputeConnectStages(RECOFIRSTPASS,n,"SPLIT","COMB"); 
      RecoComputeAppendStage(RECOFIRSTPASS,n,"COMB","RecoReformatFilter","REFORMAT2",arg3);
    }

  }

  if (RecoMethMode == Default)
  {
    DB_MSG(("<--RecoDerive: Magnitude Reconstruction"));
    return;
  }

  const int nrReceivers = RecoNrActiveReceivers();

  if (RecoMethMode == FieldMap)
  {
 
  char cmdUnrwap[RECOSTAGENODESIZE], cmdScale[RECOSTAGENODESIZE], cmdComb[RECOSTAGENODESIZE], cmdMask[RECOSTAGENODESIZE];

  sprintf(cmdUnrwap,"seedPointX=%d;seedPointY=%d;seedPointZ=%d;noiseBlur=5;thresMax=250;signalRatio=0.2",0,0,0);
  sprintf(cmdScale,"scaleFactor=%f",-1.0/(2.0*M_PI*(DESS_TE1+DESS_TE2)*1e-3)); //DESSComment, fuer 0 180
  sprintf(cmdComb,"procDim=%d;dim=%d;blockCnt=1;keepBlocksize=true",dim + 1,dim);
  sprintf(cmdMask,"snr=%.0f;cornerPart=0.2",MapSnr);

  /* calculation of product: (Z1*) * Z2 */
  for (int n = 0; n < nrReceivers; n++)
  {
    if (nrReceivers > 1)
    {
      char position[RECOSTAGENODESIZE], stage[RECOSTAGENODESIZE];
      sprintf(position, "SOS0.%d", n);
      sprintf(stage, "SP%d", n);
      RecoComputeInsertStage(RECOPREPPASS,-1,position,RECOSPLITFILTER,stage,"dim=3;keepBlocksize=true");
    }
    else
    {
      RecoComputeInsertStage(RECOPREPPASS,n,DEFAULTIMAGETYPEFILTERNAME,RECOSPLITFILTER,"SP","dim=3;keepBlocksize=true");
    }
    RecoComputeAppendStage(RECOPREPPASS,n,"SP",RECOCONJQNEGFILTER,"CON","direction=0;conjugate=true;negate=false");
    RecoComputeAppendStage(RECOPREPPASS,n,"CON",RECOMULTFILTER,"MUL","");
    RecoComputeConnectStages(RECOPREPPASS,n,"SP","MUL");
  }
  
  RecoComputeAppendStage(RECOPREPPASS,0,nrReceivers > 1 ? "SOS" : DEFAULTIMAGETYPEFILTERNAME,RECOTEEFILTER,"TEE","");

  /* calculation of phase */
  RecoComputeAppendStage(RECOPREPPASS,0,"TEE","RecoPhaseFilter","PHASE","");
  /* Unwrap filter (data -> UNW.data, mask->UNW.mask) */
  RecoComputeAppendStage(RECOPREPPASS,0,"PHASE","RecoUnwrapFilter","UNW",cmdUnrwap);
  /* creation of signal mask on magnitude data  */
  RecoComputeAddStage(RECOPREPPASS,0,RECOMAGNITUDEFILTER,"MAG","");
  RecoComputeConnectStages(RECOPREPPASS,0,"TEE","MAG");
  RecoComputeAddStage(RECOPREPPASS,0,"RecoSquareRootFilter","ROOT","");
  RecoComputeConnectStages(RECOPREPPASS,0,"MAG","ROOT");
  RecoComputeAddStage(RECOPREPPASS,0,"RecoCreateMaskFilter","CM",cmdMask);
  RecoComputeConnectStages(RECOPREPPASS,0,"ROOT","CM");
  RecoComputeConnectStages(RECOPREPPASS,0,"CM.mask","UNW.mask");
  /* scaling of phase image in Hz */
  RecoComputeAppendStage(RECOPREPPASS,0,"UNW",RECOSCALEFILTER,"SCF",cmdScale);
  //RecoComputeAppendStage(RECOPREPPASS,0,"SCF",RECOCOMBINEFILTER,"COM",cmdComb);
  RecoComputeAddStage(RECOPREPPASS,0,"RecoDummySink","SINK","");
  RecoComputeConnectStages(RECOPREPPASS,0,"CM.data","SINK");
 
  /* remove Magnitude Filter */
  RecoComputeRemoveStage(RECOPREPPASS,0,DEFAULTIMAGETYPEFILTERNAME);
  }


  if (RecoMethMode == DESS)
  {
   
    int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices ); 

    if(dim < 3 )
    {
      if(nSlices >1)
	RecoComputeInsertStage(RECOPREPPASS,0,DEFAULTIMAGETYPEFILTERNAME,RECOSPLITFILTER,"SP","procDim=3;dim=2;blockCnt=1;keepBlocksize=true");
      else
	RecoComputeInsertStage(RECOPREPPASS,0,DEFAULTIMAGETYPEFILTERNAME,RECOSPLITFILTER,"SP","dim=2;keepBlocksize=true");
    }
    else
      RecoComputeInsertStage(RECOPREPPASS,0,DEFAULTIMAGETYPEFILTERNAME,RECOSPLITFILTER,"SP","dim=3;keepBlocksize=true");	
      
    RecoComputeAppendStage(RECOPREPPASS,0,"SP","RecoSumOfSquaresFilter","SUM","");
    RecoComputeConnectStages(RECOPREPPASS,0,"SP","SUM");

    /* remove Magnitude Filter */
    RecoComputeRemoveStage(RECOPREPPASS,0,"G");
    
  }

  DB_MSG(("<--RecoDerive"));
}

