/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2007 - 2012
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

const char * methods_spiral_reco_relations_id = "$Id$ (C) 2007 - 2011 Bruker BioSpin MRI GmbH";

#define DEBUG       0
#define DB_MODULE   0
#define DB_LINE_NR  0


#include "method.h"

void SetRecoParam(void)
{
  DB_MSG(("-->SetRecoParam"));
  
  char trajFile[PATH_MAX]; 
  PvOvlUtilGetExpnoPath(trajFile, PATH_MAX, "traj");
  
  if (PVM_TrajRecoMode != Traj_Measured)
  {
    /* theoretical trajectory for both reconstruction and DC */
    ATB_SpiralSetRecoPars(trajFile);
  }
  else
  {
    /* measured trajectory for reconstruction and theoretical trajectory for DC */
    char trajDCFile[PATH_MAX];
    PvOvlUtilGetExpnoPath(trajDCFile, PATH_MAX, "trajDC");
    ATB_SpiralSetRecoPars(trajFile, trajDCFile);
  }

  /* ensure consistent method specific reconstruction parameters */
  RecoMethModeRel(); 

  /* additional parameter settings, if DTI reconstruction is selected */
  if(ACQ_scan_type != Setup_Experiment && RecoMethMode == TensorReconstruction)
  {
    int NDtiFrames = 23;

    // put repetitions containing diffusion weightings into one group
    RecoNumRepetitions = PVM_NRepetitions;
    RecoRepetitionGroupSize = PVM_DwNDiffExp;
   
    // enable PREPPASS to process complete repetition groups
    RecoPreppassProcDim = NSLICES>1?(RecoDim+2):(RecoDim+1);
    
    // describe resulting frame group structure
    RecoDescriptionInSliceCount = 1;    
    RecoDescriptionSliceCount = NSLICES;
    RecoDescriptionOutSliceCount = NDtiFrames;
    
    // required word type for jive DTI viewer
    RECO_wordtype = _32BIT_SGN_INT;
  }
  else
  {
    RecoDescriptionInSliceCount = 1;
    RecoDescriptionSliceCount = NSLICES;
    RecoDescriptionOutSliceCount = PVM_DwNDiffExp;
  }
  
  /* GS mode */
  ACQ_SetupOnlineReco = No;
  ParxRelsParRelations("ACQ_SetupOnlineReco", Yes);
 
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
  
  /* standard settings for reconstruction */
  if(RecoPrototype == No)
    SetRecoParam();

  if (PVM_TrajRecoMode != Traj_Measured)
  {
    /* theoretical trajectory */
    STB_SpiralTrajectory(RecoRegridNTrajFile);
    
    /* create network */
    ATB_SpiralSetupReco(PVM_SpiralPreSize,PVM_SpiralSize,PVM_SpiralPostSize);
  }
  else
  {
    if (PVM_TrajUpToDate == No)
    {
      UT_ReportError("Measured trajectory is not up-to-date");      
    }

    int nPre    = PVM_SpiralPreSize;
    int nSpiral = PVM_SpiralSize;
    int nPost   = PVM_SpiralPostSize;
    char b0File[PATH_MAX];

    PvOvlUtilGetExpnoPath(b0File, PATH_MAX, "b0");

    /* measured trajectory for reconstruction */
    
    const int maxGradDelay = (int)(0.3 / PVM_SpiralAcqDwellTime); // [samples]   
    STB_TrajCreateTrajectory(
         RecoRegridNTrajFile, b0File,
         &nPre, &nSpiral, &nPost,
         maxGradDelay, PVM_SpiralPointsPerRotation);

    DB_MSG(("nPre = %d, nSpiral = %d, nPost = %d, ", nPre, nSpiral, nPost));
         
    /* theoretical trajectory for DC */
    STB_SpiralTrajectory(RecoRegridNTrajFileDC, NULL, NULL, 0, nSpiral);
    
    /* create network */
    ATB_SpiralSetupReco(nPre, nSpiral, nPost, b0File);
  }
 
  /* adapt reconstruction network if DTI reconstruction is selected */ 
  if ( ACQ_scan_type != Setup_Experiment && RecoMethMode == TensorReconstruction) 
  {

    if (RECO_image_type != MAGNITUDE_IMAGE)
    {
      UT_ReportError("For DTI reconstruction RECO_image_type must be set to Magnitude");
      return;
    }
    
    char filterOptions[RECOSTAGENODESIZE]="\0";

    sprintf(filterOptions,"DwBMatImag=<PVM_DwBMatImag>; traceWeight=%f;",TraceWeight);

    RecoComputeInsertStage(RECOPREPPASS,0,"M",RECODTIFILTER,"DTI",filterOptions);

    DB_MSG(("... inserted %s DTI{%s}.\n",RECODTIFILTER,filterOptions));    

    // move RecoMaskFilter behind DTI reco
    RecoComputeRemoveStage(RECOPREPPASS,0,"MASK");    
    RecoComputeAppendStage(RECOPREPPASS,0,"DTI","RecoMaskFilter","MASK","");
    
    // replace the RecoMapFilter by a version with commonMapLoop option
    sprintf(filterOptions,"symmetricMode=true;"
            "minima=<RECO_minima>;maxima=<RECO_maxima>;"
            "slope=<RECO_map_slope>;offset=<RECO_map_offset>;"
            "map_min=<RECO_map_min>;map_max=<RECO_map_max>;"
            "rangeMin=-2147483646.500000;rangeMax=2147483646.500000;"
            "perObjectMode=true;cutoffMode=true;"
            "commonMapLoop={%d,%d,%d}",NSLICES,RecoDescriptionOutSliceCount,1);

    RecoComputeAppendStage(RECOMAPPASS,0,"MAP",RECOMAPFILTER,"DTIMAP",filterOptions);

    RecoComputeRemoveStage(RECOMAPPASS,0,"MAP");            

    DB_MSG(("... inserted %s DTIMAP{%s}.\n",RECOMAPFILTER,filterOptions));

    /* adapt parameters and network for the output process */
    int NDtiFrames = 23;
    RecoObjectsPerRepetition = NSLICES*NDtiFrames; 

    // DtiSpiral: Cannot put this in SetRecoParam, because derivation of 
    // RegridNPass uses original value of RecoObjectsPerRepetition
    
    // update RECO_transposition with new number of objects per repetition
    ParxRelsParRelations("RECO_transposition",Yes);    

    ATB_SetRecoTranspositionFromLoops(
      "PVM_SliceGeo",
      PTB_GetSpatDim(),
      1,
      NDtiFrames*NSLICES);

  
    // Adjust sorting parameters to take into account the diffusion experiments
    // in repetition group
    RecoSortDim = RecoSortDim + 1;
    ParxRelsParRelations("RecoSortDim", Yes);
    int lenRecoSortMap = PARX_get_dim("RecoSortMaps", 1);
    PARX_change_dims("RecoSortSize", RecoSortDim);
    RecoSortSize[RecoSortDim - 1] = RecoRepetitionGroupSize;
    PARX_change_dims("RecoSortSegment", RecoSortDim);
    RecoSortSegment[RecoSortDim - 1] = 1;
    PARX_change_dims("RecoSortRange", RecoSortDim);
    RecoSortRange[RecoSortDim - 1] = RecoRepetitionGroupSize;
    PARX_change_dims("RecoSortMaps", lenRecoSortMap + RecoRepetitionGroupSize);
    for (int j = 0; j < RecoRepetitionGroupSize; ++j)
    {
      RecoSortMaps[lenRecoSortMap + j] = j;
    } 
  
    // re-derive output process for new RecoObjectsPerRepetition
    RecoDeriveOutputProcess();
    
    DB_MSG(("... rederived output process with %d DTI frames.\n",NDtiFrames));
  }
 
  DB_MSG(("<--RecoDerive"));
}

