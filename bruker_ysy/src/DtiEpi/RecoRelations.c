/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999 - 2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Locker$
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id: RecoRelations.c,v 1.1.2.2 2011/02/16 DGEM ";

#define DEBUG    0
#define DB_MODULE  0
#define DB_LINE_NR  0



/****************************************************************/
/*              I N C L U D E   F I L E S                       */
/****************************************************************/

#include "method.h"


void SetRecoParam(int acceleration)
{
  DB_MSG(("Entering SetRecoParam"));

  ATB_EpiSetRecoPars(acceleration, PVM_NEchoImages, 1);

  /* ensure consistent method specific reconstruction parameters */
  RecoMethModeRel();

  SetTensorRecoParam();
   
  DB_MSG(("Exiting SetRecoParam"));
}

/* Relation of RecoUserUpdate, called at start of reconstruction
   to set reconstrution process */

void RecoDerive(void)
{
  DB_MSG(("-->RecoDerive\n"));

  /* standard settings for reconstruction */
  if (RecoPrototype == No && PVM_EpiAdjustMode != 2)  //do not call in grappa adj
  { 
    SetRecoParam(PVM_EncPpi[1]); 
  }

  ATB_EpiSetRecoProcess(
    PVM_EncNReceivers,
    PVM_EncCentralStep1,
    PVM_NEchoImages
  );

  /* adapt reconstruction network if DTI reconstruction is selected */   
  if(ACQ_scan_type != Setup_Experiment && RecoMethMode == TensorReconstruction) 
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
  
   DB_MSG(("<--RecoDerive\n"));
}


void SetTensorRecoParam()
{
  DB_MSG(("-->SetTensorRecoParam"));
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

  DB_MSG(("<--SetTensorRecoParam"));
}
