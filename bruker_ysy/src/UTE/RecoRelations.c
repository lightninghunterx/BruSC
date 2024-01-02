/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2011
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

  int dim = PTB_GetSpatDim();
  const int acqPoints = AcqMode == FID ? PVM_EncMatrix[0]/2 + RampPoints : PVM_EncMatrix[0] + ReadDephPoints;
  const int numProj = SliceAlter == Yes ? 2*NPro : NPro;  
  int acqMat[2] = {acqPoints, numProj};
  int echoImages = PVM_NMovieFrames;

  /* set up user reco */
  ATB_InitUserModeReco(
    dim,
    dim,
    PVM_EncMatrix,
    PVM_Matrix,
    PVM_AntiAlias,
    PVM_EncPftOverscans,
    NI,
    ACQ_obj_order,
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

  /* regridding */
  char trajFile[PATH_MAX];
  char trajDCFile[PATH_MAX];
  PvOvlUtilGetExpnoPath(trajFile, PATH_MAX, "traj");
  PvOvlUtilGetExpnoPath(trajDCFile, PATH_MAX, "trajDC");
  const int readP = AcqMode == FID ? (PVM_EncMatrix[0]/2 + RampPoints) : PVM_EncMatrix[0];
  ATB_SetRecoRegridN(
    readP,
    NPro,
    RecoRegridNTrajRadial,
    trajFile,
    AcqMode == ECHO ? RecoRegridNDCPost : RecoRegridNDCPre,
    AcqMode == ECHO && PVM_TrajRecoMode == Traj_Measured ? trajDCFile : NULL);


  /* geometry */
  for (int i = 0; i < dim; i++)
  {
    ATB_SetRecoRotate(
      NULL,
      PVM_Fov[i] * PVM_AntiAlias[i],
      NSLICES,     
      echoImages,
      i);      
            
    ATB_SetRecoPhaseCorr(50.0, 0.0, i);
  }

 
  ATB_SetRecoTranspositionFromLoops("PVM_SliceGeo",
                                    dim,
                                    echoImages,
                                    NI);


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
    const int readP = AcqMode == FID ? (PVM_EncMatrix[0]/2 + RampPoints) : PVM_EncMatrix[0];
    int nPre=0, nUsed=PVM_TrajRecoMode == Traj_Measured ? readP : 0, nPost=PostPoints;
    PvOvlUtilGetExpnoPath(b0File, PATH_MAX, "b0");
    
    if(AcqMode==ECHO)
    {
      nPre=ReadDephPoints;
    }

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

    if(AcqMode == ECHO && PVM_TrajRecoMode == Traj_Measured)
    {
      if (PVM_TrajUpToDate == No)
      {
	UT_ReportError("Measured trajectory is not up-to-date");      
      }
      nUsed = 0;
      STB_TrajCreateTrajectory(RecoRegridNTrajFileDC,NULL,&nPre,&nUsed,&nPost,0);
    }

    /* set up default network */
    ParxRelsParRelations("RecoUserUpdate", Yes);
 
    const int nrReceivers = RecoNrActiveReceivers();
    
    if(SliceAlter == Yes)
    {  
      for (int n=0; n < nrReceivers; n++)
      {
	RecoComputeInsertStage(RECOFIRSTPASS,n,DEFAULTSINKNAME,"RecoPhasedAccumFilter","PA","ns=2");
      }
    }
    
    if(AcqMode == ECHO)
    {
      /* insert RecoStabCorrFilter */
      for (int n = 0; n < nrReceivers; n++)
      {
        char filterArguments[RECOSTAGENODESIZE];	
	sprintf(filterArguments,
		"navSize = %d; "
		"junkSize = %d; "
		"echoSize = %d; "
		"junkEndSize = %d; "
		"nEchoes = 1; "
		"nSegments = %d; "
		"useNav = false; "
		"drift = false",
		0,
		ReadDephPoints,
		PVM_EncMatrix[0],
		0,
		NPro);
	RecoComputeAppendStage(RECOFIRSTPASS, n, DEFAULTCASTFILTERNAME, "RecoStabCorrFilter", "NAVCORR", filterArguments);
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
    const int acqPoints = AcqMode == FID ? PVM_EncMatrix[0]/2 + RampPoints : PVM_EncMatrix[0] + ReadDephPoints;   
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




