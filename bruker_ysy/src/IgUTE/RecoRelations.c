/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2013
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

const char * methods_ute_reco_relations_id = "$Id$ (C) 2001 - 2011 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0



#include "method.h"

void SetRecoParam( void )

{
  DB_MSG(("-->SetRecoParam"));

  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  int echoImages = OutputMovieFrames;
  int recoObjects = echoImages * nSlices;
  int dim = PTB_GetSpatDim();
  const int acqPoints = PVM_EncMatrix[0]/2 + RampPoints + AddCentralPoints; 
  int acqMat[2] = {acqPoints, NPro}; 

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
  
  /* set up user reco */
  ATB_InitUserModeReco(
    dim,
    dim,
    PVM_EncMatrix,
    PVM_Matrix,
    PVM_AntiAlias,
    PVM_EncPftOverscans,
    echoImages * nSlices,
    acq_obj_order,
    1,
    NULL,
    NULL,
    NULL,
    PVM_EncNReceivers,
    PVM_EncChanScaling,
    0,
    0,
    0,
    acqMat);

  RecoObjectsPerSetupStep = nSlices;

  /* regridding */
  char trajFile[PATH_MAX];
  char trajDCFile[PATH_MAX];
  PvOvlUtilGetExpnoPath(trajFile, PATH_MAX, "traj");
  PvOvlUtilGetExpnoPath(trajDCFile, PATH_MAX, "trajDC");
  ATB_SetRecoRegridN(
    acqPoints,
    NPro,
    RecoRegridNTrajRadial,
    trajFile,
    RecoRegridNDCPre,
    NULL);
  
  /* geometry */

  for (int i = 0; i < dim; i++)
  {
    ATB_SetRecoRotate(
      NULL,
      PVM_Fov[i] * PVM_AntiAlias[i],
      NSLICES,     
      1,
      i);      
            
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

  DB_MSG(("<--SetRecoParam"));
}


void RecoDerive(void)
{

  DB_MSG(("-->RecoDerive"));

  if (RecoUserUpdate == No)
  {
    DB_MSG(("<--RecoDerive: no update"));
    return;
  }

  /* trajectory measurement: use different reco */
  if (ATB_TrajSetupReco() == Yes)
  {
    DB_MSG(("<--RecoDerive: trajectory measurement"));
    return;
  }
  
  /* scan experiment */

  if (ACQ_scan_type != Setup_Experiment)
  {
    /*** scan experiment or off-line reco ***/

    /* standard settings for reconstruction */
    if(RecoPrototype == No)
      SetRecoParam();
    
    /* provide interleave weights */
    PARX_change_dims("GradAmpR",NPro);
    PARX_change_dims("GradAmpP",NPro);
    SetProj(GradAmpR,
	    GradAmpP,
	    NPro,
	    1);

    /* get trajectory */   
    char b0File[PATH_MAX]; 
    const int acqPoints = PVM_EncMatrix[0]/2 + RampPoints + AddCentralPoints;
    int nPre=0, nUsed=PVM_TrajRecoMode == Traj_Measured ? acqPoints : 0, nPost=0;
    PvOvlUtilGetExpnoPath(b0File, PATH_MAX, "b0");

    if(PVM_TrajRecoMode == Traj_Measured)
    {
      if (PVM_TrajUpToDate == No)
      {
	UT_ReportError("Measured trajectory is not up-to-date");      
      }
      STB_TrajCreateTrajectory(RecoRegridNTrajFile,b0File,&nPre,&nUsed,&nPost,0);
    }
    else
    {
      STB_TrajCreateTrajectory(RecoRegridNTrajFile,NULL,&nPre,&nUsed,&nPost,0);
    }

    /* set up default network */
    ParxRelsParRelations("RecoUserUpdate", Yes);
 

    if(SelfGating == Yes)
    {
      if(PVM_NavOnOff == Off)
      	RecoInSliceNav(); // in-slice navigator
      else
      	RecoNavModule(); // navigator module active
      
      if(StoreOutputFiles == Yes)
      {
	// PVM_StudyIGAssignment will be stored as study result
	CMN_StudyIGAssignmentRel();
      }

    }
    
    /* B0 correction */
    if(PVM_TrajRecoMode == Traj_Measured && B0Corr == Yes)
    {
      ATB_TrajInsertB0Correction(b0File, nUsed, RecoRegridNDim);
    }

  }
  else
  {
    /*** setup experiment ***/
  
    ATB_SetRecoRegridNOff();
    const int acqPoints = PVM_EncMatrix[0]/2 + RampPoints + AddCentralPoints;  
    const int size[2] = {acqPoints, 1};
    
    for (int i = 0; i < PTB_GetSpatDim(); i++)
    {
      RECO_size[i]     = size[i];
      RECO_inp_size[i] = size[i];
      RECO_ft_size[i]  = size[i];
    }
    
    /* set up default network */
    ParxRelsParRelations("RecoUserUpdate", Yes);

  }

  DB_MSG(("<--RecoDerive"));
}

void CalcFreq(void)
{
  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  CardiacPeriod = 60 * 1e3 / HeartRate;
  RespPeriod =  60 * 1e3 / RespRate;
  CardFreq = 2*PVM_RepetitionTime/nSlices * HeartRate /(60*1e3);
  RespFreq = 2*PVM_RepetitionTime/nSlices * RespRate/(60*1e3);
}

void RecoInSliceNav(void)
{
  
  DB_MSG(("-->RecoInSliceNav")); 
  
  char arg0[RECOSTAGENODESIZE], arg1[RECOSTAGENODESIZE], arg2[RECOSTAGENODESIZE];
  const int nrReceivers = RecoNrActiveReceivers(); 
  bool timegrid = TimeGridding==Yes? true:false;
  bool cardOnly = false;
  const int acqSize = NPro;  
  const int segSize = NPro;
  const int nSlices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);
  int signalType = 0;
  double phase0=0.0, range0=1.0;
  double cardFreq = CardFreq;
  double respFreq = RespFreq; 
  if(StoreOutputFiles == Yes)
    IgRecoDebug = Yes;

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
           "freezePhaseRange=%f;timeGridding=%d;GaussWidth=%f;",
	   nSlices,acqSize,segSize,Oversampling,MovieType==Cardiac ? 0 : 1,
	   OutputMovieFrames,cardOnly,phase0,range0,FreezePhase,FreezePhaseRange,timegrid,GaussWidth);

  if(AddCentralPoints >0)
  {
    snprintf(arg2,RECOSTAGENODESIZE,"cardFrq=%f;respFrq=%f;cardOnly=%d;setupMode=false;navigMode=2;"
                                    "debugFiles=%d;addPoints=%d;signalType=%d;",
	     cardFreq,respFreq,cardOnly,IgRecoDebug==No ? 0:1,AddCentralPoints,signalType);
  }
  else
  {
    snprintf(arg2,RECOSTAGENODESIZE,"cardFrq=%f;respFrq=%f;cardOnly=%d;setupMode=false;navigMode=1;debugFiles=%d;"
                                    "signalType=%d;cardFrqReco=<CardFreqResult>;respFrqReco=<RespFreqResult>;",
	     cardFreq,respFreq,cardOnly,IgRecoDebug==No ? 0:1,signalType);
  }
 
  for (int n=0; n < nrReceivers; n++)
  {   
    RecoComputeAppendStage(RECOFIRSTPASS,n,"CAST","RecoSortFilter","SORT",arg0);
    RecoComputeAppendStage(RECOFIRSTPASS,n,"SORT","RecoTeeFilter","TEE","");
    RecoComputeDisconnectStages(RECOFIRSTPASS,n,"TEE","REORD");
    RecoComputeAddStage(RECOFIRSTPASS,n,"RecoRGateApplyOrderFilter",
			"APPLY",arg1); 
    char c1[20];
    char c2[20];
    sprintf(c1,"APPLY%d.0",n);
    sprintf(c2,"TEE%d",n);
    RecoComputeConnectStages(RECOFIRSTPASS,-1,c2,c1);
    RecoComputeConnectStages(RECOFIRSTPASS,n,"APPLY","REORD");
  } 
 
  RecoComputeAddStage(RECOFIRSTPASS,-1,"RecoRGateFindOrderFilter","FIND",arg2);
  if(nrReceivers >1)
  {
    RecoComputeAddStage(RECOFIRSTPASS,-1,"RecoTeeFilter","TEEA","");
    RecoComputeConnectStages(RECOFIRSTPASS,-1,"FIND","TEEA");  
  }
  for (int n=0; n < nrReceivers; n++)
  { 
    char c1[20];
    sprintf(c1,"APPLY%d.1",n); 
    char c2[20];
    sprintf(c2,"TEE%d",n); 
    RecoComputeConnectStages(RECOFIRSTPASS,-1,c2,"FIND");   
    if(nrReceivers >1)
    {
      RecoComputeConnectStages(RECOFIRSTPASS,-1,"TEEA",c1);
    }
    else
    {
      RecoComputeConnectStages(RECOFIRSTPASS,-1,"FIND",c1);
    }
  }
  
  DB_MSG(("<--RecoInSliceNav")); 
}

void RecoNavModule(void)
{

  DB_MSG(("-->RecoNavModule")); 
  
  char arg0[RECOSTAGENODESIZE], arg1[RECOSTAGENODESIZE], arg2[RECOSTAGENODESIZE];
  const int nrReceivers = RecoNrActiveReceivers(); 
  bool timegrid = TimeGridding==Yes? true:false;
  bool cardOnly = false;
  const int acqSize = NPro;  
  const int segSize = NPro;
  const int nSlices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);
  int signalType = 0;
  double phase0=0.0, range0=1.0;
  double cardFreq = CardFreq;
  double respFreq = RespFreq;
  if(StoreOutputFiles == Yes)
    IgRecoDebug = Yes;

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
           "freezePhaseRange=%f;timeGridding=%d;GaussWidth=%f;",
	   nSlices,acqSize,segSize,Oversampling,MovieType==Cardiac ? 0 : 1,
	   OutputMovieFrames,cardOnly,phase0,range0,FreezePhase,FreezePhaseRange,timegrid,GaussWidth);

  snprintf(arg2,RECOSTAGENODESIZE,"cardFrq=%f;respFrq=%f;cardOnly=%d;setupMode=false;navigMode=0;debugFiles=%d;"
           "cardFrqReco=<CardFreqResult>;respFrqReco=<RespFreqResult>;signalType=%d;",
	   cardFreq,respFreq,cardOnly,IgRecoDebug==No ? 0:1,signalType);

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

  const char* follower = "REORD";
 
  for (int n = 0; n < nrReceivers; n++)
  {   
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
  }

  DB_MSG(("<--RecoNavModule")); 
}


void CardFreqResultRel(void)
{
  DB_MSG(("-->CardFreqResultRel"));

  int nSlices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);

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

  int nSlices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);
  RespRateRecoResult = RespFreqResult * nSlices * 60*1e3 /(2*PVM_RepetitionTime);

  DB_MSG(("<--RespFreqResultRel"));
}
