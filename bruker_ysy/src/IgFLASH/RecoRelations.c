/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2011
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

  /* number of Movie Frames is Method Reco parameter ! */
  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  int echoImages = OutputMovieFrames;
  int recoObjects = echoImages * nSlices;
  int phaseFactor = ((SelfGating == No) && (AngioMode == Yes)) ? PVM_RareFactor : 1;
 
  RecoDescriptionInSliceCount = echoImages;
  RecoDescriptionSliceCount = nSlices;
  RecoDescriptionOutSliceCount = 1;
  
  if(!MRT_RecoCheckCS())
  {    
    CompressedSensing = No;
    ParxRelsMakeNonEditable("CompressedSensing,RecoCS");
  }else{
    ParxRelsMakeEditable("CompressedSensing,RecoCS");
  }   
  RecoCS = CompressedSensing;

  /* create an object order according to the number of movie-frames 
     to be reconstructed */

  int* acq_obj_order = new int[recoObjects];

  int j = 0;
  while(j< OutputMovieFrames)
  {
    for(int i=0;i<nSlices;i++)
    {
      int k=j*nSlices+i;
      acq_obj_order[k] = PVM_ObjOrderList[i] * OutputMovieFrames + j;
    }
    j=j+1;
  }
  
  ATB_InitUserModeReco(
    dim,
    0,
    PVM_EncMatrix,
    PVM_Matrix,
    PVM_AntiAlias,
    PVM_EncPftOverscans,
    echoImages * nSlices,
    acq_obj_order,
    phaseFactor,
    PVM_EncSteps1,
    PVM_EncSteps2,
    NULL,
    PVM_EncNReceivers,
    PVM_EncChanScaling,
    dim>1? PVM_EncPpiRefLines[1]:0,
    dim>1? PVM_EncPpi[1]:0);

  RecoObjectsPerSetupStep = AngioMode == Yes? 1:nSlices;
  

  ATB_SetRecoRotate(
    NULL,
    PVM_Fov[0] * PVM_AntiAlias[0],
    nSlices,
    echoImages,
    0);


  if(dim>1)
  {
    ATB_SetRecoRotate(
      PVM_EffPhase1Offset,
      PVM_Fov[1] * PVM_AntiAlias[1],
      nSlices,
      echoImages,
      1);
  }

  if (dim == 3)
  {
    ATB_SetRecoRotate(
      PVM_EffPhase2Offset,
      PVM_Fov[2] * PVM_AntiAlias[2],
      nSlices,
      echoImages,
      2);
  }
  
  for (int i = 0; i < dim; i++)
  {
    ATB_SetRecoPhaseCorr(50.0, 0.0, i);
  }

  ATB_SetRecoTranspositionFromLoops(
    "PVM_SliceGeo",
    dim,
    1,
    recoObjects);


  /* special settings */
  CalcFreq();
  RecoMethModeVisPar();

  // release memory for local object
  delete[] acq_obj_order;

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
  
  if(NavSigSetupYesNo==No)
    ACQ_SetupShuffleProfiles=Yes;
  else
    ACQ_SetupShuffleProfiles=No;  

  /* create standard reconstruction network */
  ParxRelsParRelations("RecoUserUpdate", Yes);
  
  /* special reconstruction */
  if(ACQ_scan_type != Setup_Experiment)
  {
    if (SelfGating == No)
    {
      DB_MSG(("<--RecoDerive: no SelfGating\n"));
      return;
    }
 
    /* standard settings for reconstruction */
    if(RecoPrototype == No)
      SetRecoParam();
    
    /* special settings for reconstruction */
    IgRecoRel();
  } 
  else
  {
    if(NavSigSetupYesNo==No)
    {
      RecoObjectsPerRepetition = ACQ_jobs[0].transactionBlocks;
      ParxRelsParRelations("RecoObjectsPerRepetition",Yes);
    }
    else
    {
      int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
      RecoObjectsPerRepetition = nSlices; 
      ParxRelsParRelations("RecoObjectsPerRepetition",Yes);
      IgSetupRecoRel();
    }
  }

  DB_MSG(("<--RecoDerive\n"));
}

void IgRecoRel (void)
{
  DB_MSG(("-->IgRecoRel"));
  
  int dim = PTB_GetSpatDim();
  char arg0[RECOSTAGENODESIZE],arg1[RECOSTAGENODESIZE],arg2[RECOSTAGENODESIZE];
  const int nrReceivers = RecoNrActiveReceivers();
  bool timegrid = TimeGridding==Yes? true:false;
  bool cardOnly = false;
  bool angioMode = AngioMode == Yes? true:false;
  if(StoreOutputFiles == Yes)
    IgRecoDebug = Yes;
  bool debugFiles = IgRecoDebug == No? false:true;
  int acqSize = PVM_EncMatrix[1];
  if(dim == 3)
    acqSize = acqSize * PVM_EncMatrix[2];
  int segSize = PVM_RareFactor;
  int nSlices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);
  int signalType = 0;
  double phase0=0.0, range0=1.0;
  double cardFreq = CardFreq;
  double respFreq = RespFreq;
 
 switch(AnalysisOfMotion)
  {
  case 0: //Cardiac and Respiration
    phase0 = FreezePhase0;
    range0 = FreezePhaseRange0;
    cardOnly = false;
    break;
  case 1: //Only Cardiac
    phase0 = FreezePhase0;
    range0 = FreezePhaseRange0;
    cardOnly = true;    
    break; 
  case 2: //Only Respiration
    phase0 = FreezePhase;
    range0 = FreezePhaseRange;
    cardFreq = RespFreq;
    cardOnly = true;    
    break;
  }

 switch(NavigatorAnalysis)
 {
 default:
 case 0:
   signalType = 0;
   break;
 case 1:
   signalType = 1;
   break;
 case 2:
   signalType = 2;
   break;
 }

  snprintf(arg0,RECOSTAGENODESIZE,"sortDim=1;sortSize={0};sortRange={0};sortSegment={1};");    
  snprintf(arg1,RECOSTAGENODESIZE,"nslices=%d;acqSize=%d;segSize=%d;overs=%d;movieType=%d;"
           "movieSize=%d;cardOnly=%d;freezePhase0=%f;freezePhaseRange0=%f;freezePhase=%f;"
           "freezePhaseRange=%f;timeGridding=%d;GaussWidth=%f;angioMode=%d",
              nSlices,acqSize,segSize,Oversampling,MovieType==Cardiac ? 0 : 1,
	   OutputMovieFrames,cardOnly,phase0,range0,FreezePhase,FreezePhaseRange,timegrid,GaussWidth,angioMode);
  snprintf(arg2,RECOSTAGENODESIZE,"cardFrq=%f;respFrq=%f;cardOnly=%d;setupMode=false;navigMode=0;debugFiles=%d;"
                                  "cardFrqReco=<CardFreqResult>;respFrqReco=<RespFreqResult>;signalType=%d",
	   cardFreq,respFreq,cardOnly,debugFiles,signalType);

  // the sink created by default is not needed
  RecoComputeRemoveStage(RECOFIRSTPASS,-1,"JS1");

  RecoComputeAddStage(RECOFIRSTPASS,-1,"RecoRGateFindOrderFilter","FIND",arg2);

  // Reconstruction of job1 (navigator)
  // for multi channel reconstruction, data must be split into
  // different lanes.
  
  if (nrReceivers > 1)
  {
      // extract first channel
      RecoComputeAddStage(RECOFIRSTPASS,-1,"RecoSplitFilter","JSPLIT",
                          "dim=1;keepBlocksize=1;");
      RecoComputeConnectStages(RECOFIRSTPASS,-1,"JQ1","JSPLIT");
      
      // in parallel mode, use only first channel ...
      for (int i = 0; i < nrReceivers; i++)
      {
          RecoComputeAddStage(RECOFIRSTPASS,i,"RecoCastFilter","JCAST",
                              "dataRep=FLOAT;wordSize=8;");
          char c1[20];
          sprintf(c1,"JCAST%d",i);          
          RecoComputeConnectStages(RECOFIRSTPASS,-1,"JSPLIT",c1);
          RecoComputeAddStage(RECOFIRSTPASS,i,"RecoSortFilter","SORTNAV",arg0);
          RecoComputeConnectStages(RECOFIRSTPASS,i,"JCAST","SORTNAV");
          char c2[20];
          sprintf(c2,"SORTNAV%d",i);
          RecoComputeConnectStages(RECOFIRSTPASS,-1,c2,"FIND");
      } 
      RecoComputeAddStage(RECOFIRSTPASS,-1,"RecoTeeFilter","TEEF","");
      RecoComputeConnectStages(RECOFIRSTPASS,-1,"FIND","TEEF");  
  }
  else
  {
      RecoComputeAddStage(RECOFIRSTPASS,-1,"RecoCastFilter","JCAST",
                          "dataRep=FLOAT;wordSize=8;");
      RecoComputeConnectStages(RECOFIRSTPASS,-1,"JQ1","JCAST");
      RecoComputeAddStage(RECOFIRSTPASS,-1,"RecoSortFilter","SORTNAV",arg0);
      RecoComputeConnectStages(RECOFIRSTPASS,-1,"JCAST","SORTNAV");
      RecoComputeConnectStages(RECOFIRSTPASS,-1,"SORTNAV","FIND");
  }

  // Reconstruction of job0 (echo)
  // The place for integrating the apply order filter depends 
  // on the context.

  const char* follower;
  if ((PVM_EncPpi[1] > 1.0) || (RecoHalfFT[0]==Yes) || RecoCS || RecoNoFT || RecoGrappa3D)
      follower = "REORD";
  else
      follower = "FTS";
 
  for (int n = 0; n < nrReceivers; n++)
  {
    if(RECO_ft_size[0] > RECO_inp_size[0])
      RecoComputeAppendStage(RECOFIRSTPASS,n,"Z","RecoSortFilter","SORT",arg0);
    else
      RecoComputeAppendStage(RECOFIRSTPASS,n,"CAST","RecoSortFilter","SORT",arg0);

      RecoComputeDisconnectStages(RECOFIRSTPASS,n,"SORT",follower);
      
      RecoComputeAddStage(RECOFIRSTPASS,n,"RecoRGateApplyOrderFilter",
                          "APPLY",arg1);  
      char c1[20];
      char c2[20];
      sprintf(c1,"APPLY%d.0",n);
      sprintf(c2,"SORT%d",n);
      RecoComputeConnectStages(RECOFIRSTPASS,-1,c2,c1);
      RecoComputeConnectStages(RECOFIRSTPASS,n,"APPLY",follower);
      sprintf(c1,"APPLY%d.1",n);
      if (nrReceivers > 1)
      {
          sprintf(c2,"TEEF");
          RecoComputeConnectStages(RECOFIRSTPASS,-1,c2,c1);
      }
      else
          RecoComputeConnectStages(RECOFIRSTPASS,-1,"FIND",c1);

      if(dim == 3)
      {
	char arg4[RECOSTAGENODESIZE];
	snprintf(arg4,RECOSTAGENODESIZE,"procDim=4;newSizes={%d,%d,%d,%d};",PVM_Matrix[0],PVM_EncMatrix[1],PVM_EncMatrix[2],OutputMovieFrames); 
	RecoComputeAppendStage(RECOFIRSTPASS,n,"APPLY","RecoReformatFilter","REFORMAT",arg4);
      }      
  }
  
  if(StoreOutputFiles == Yes)
  {
    // PVM_StudyIGAssignment will be stored as study result
    CMN_StudyIGAssignmentRel();
  }
 
  DB_MSG(("<--IgRecoRel"));
}

void IgSetupRecoRel(void)
{
  DB_MSG(("-->IgSetupRecoRel"));

  char arg1[RECOSTAGENODESIZE],arg2[RECOSTAGENODESIZE],arg3[RECOSTAGENODESIZE];
  const int nrReceivers = RecoNrActiveReceivers();
  int nSlices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);
  int signalType = 0;
  bool cardOnly= AnalysisOfMotion==0? false:true;
  double cardFreq = AnalysisOfMotion==2? RespFreq:CardFreq;

  switch(NavigatorAnalysis)
  {
  default:
  case 0:
    signalType = 0;
    break;
  case 1:
    signalType = 1;
    break;
  case 2:
    signalType = 2;
    break;
  }

  snprintf(arg1,RECOSTAGENODESIZE,"procDim=2;sortDim=1;sortSize={0};sortRange={0};sortSegment={0};"); 
  snprintf(arg2,RECOSTAGENODESIZE,"cardFrq=%f;respFrq=%f;cardOnly=%d;setupMode=true;navigMode=0;"
                                  "cardFrqReco=<CardFreqResult>;respFrqReco=<RespFreqResult>;signalType=%d",
	                          cardFreq,RespFreq,cardOnly,signalType);   
  snprintf(arg3,RECOSTAGENODESIZE,"nslices=%d;",nSlices);
  
  for (int i = 0; i < nrReceivers; i++)
  {
    RecoComputeAppendStage(RECOFIRSTPASS,i,"FT","RecoSortFilter","SORT",arg1);  
    RecoComputeAppendStage(RECOFIRSTPASS,i,"SORT","RecoRGateDisplayFilter","DISP",arg3);  
  }


  RecoComputeAddStage(RECOFIRSTPASS,-1,"RecoRGateFindOrderFilter","FIND",arg2);

  if (nrReceivers > 1)
  {
      // extract first channel
      RecoComputeAddStage(RECOFIRSTPASS,-1,"RecoSplitFilter","JSPLIT",
                          "dim=1;keepBlocksize=1;");
      RecoComputeConnectStages(RECOFIRSTPASS,-1,"JQ1","JSPLIT");
     
      RecoComputeAddStage(RECOFIRSTPASS,-1,"RecoTeeFilter","TEEF","");
      RecoComputeConnectStages(RECOFIRSTPASS,-1,"FIND","TEEF");  

      // in parallel mode, use only first channel ...
      for (int i = 0; i < nrReceivers; i++)
      {
          RecoComputeAddStage(RECOFIRSTPASS,i,"RecoCastFilter","JCAST",
                              "dataRep=FLOAT;wordSize=8;");
          char c1[64];
          sprintf(c1,"JCAST%d",i);      
          RecoComputeConnectStages(RECOFIRSTPASS,-1,"JSPLIT",c1);
	  RecoComputeAddStage(RECOFIRSTPASS,i,"RecoSortFilter","SORTNAV",arg1);
	  RecoComputeConnectStages(RECOFIRSTPASS,i,"JCAST","SORTNAV");
	  char c2[64];
	  sprintf(c2,"SORTNAV%d",i);
	  RecoComputeConnectStages(RECOFIRSTPASS,-1,c2,"FIND");
	  char c3[64];
	  sprintf(c3,"DISP%d.1",i);
	  char c4[64];
	  sprintf(c4,"TEEF");
	  RecoComputeConnectStages(RECOFIRSTPASS,-1,c4,c3);
      }   
  }
  else
  {
      RecoComputeAddStage(RECOFIRSTPASS,-1,"RecoCastFilter","JCAST",
                          "dataRep=FLOAT;wordSize=8;");
      RecoComputeConnectStages(RECOFIRSTPASS,-1,"JQ1","JCAST");
      RecoComputeAddStage(RECOFIRSTPASS,-1,"RecoSortFilter","SORTNAV",arg1);
      RecoComputeConnectStages(RECOFIRSTPASS,-1,"JCAST","SORTNAV");
      RecoComputeConnectStages(RECOFIRSTPASS,-1,"SORTNAV","FIND");
      RecoComputeConnectStages(RECOFIRSTPASS,-1,"FIND","DISP0.1");
  }

  // the sink created by default is not needed
  RecoComputeRemoveStage(RECOFIRSTPASS,-1,"JS1");

  DB_MSG(("<--IgSetupRecoRel"));
}

void CalcFreq(void)
{
  int nSlices = AngioMode==Yes? 1: GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  CardiacPeriod = 60 * 1e3 / HeartRate;
  RespPeriod =  60 * 1e3 / RespRate;
  CardFreq = 2*PVM_RepetitionTime/nSlices * HeartRate /(60*1e3);
  RespFreq = 2*PVM_RepetitionTime/nSlices * RespRate/(60*1e3);
}

void CardFreqResultRel(void)
{
  DB_MSG(("-->CardFreqResultRel"));

  int nSlices = AngioMode==Yes?1:GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);

  switch(AnalysisOfMotion)
  {
  case 0:
    HeartRateRecoResult = CardFreqResult * nSlices * 60*1e3 /(2*PVM_RepetitionTime); 
    break;
  case 1:
    HeartRateRecoResult = CardFreqResult * nSlices * 60*1e3 /(2*PVM_RepetitionTime);
    RespRateRecoResult = 0;
    break;
  case 2:
    HeartRateRecoResult = 0;
    RespRateRecoResult = CardFreqResult * nSlices * 60*1e3 /(2*PVM_RepetitionTime);
    break;
  }
   
  DB_MSG(("<--CardFreqResultRel"));
}

void RespFreqResultRel(void)
{
  DB_MSG(("-->RespFreqResultRel"));

  int nSlices = AngioMode==Yes?1:GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);
  RespRateRecoResult = RespFreqResult * nSlices * 60*1e3 /(2*PVM_RepetitionTime);

  DB_MSG(("<--RespFreqResultRel"));
}
