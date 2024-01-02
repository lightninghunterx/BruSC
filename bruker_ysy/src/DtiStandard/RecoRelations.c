#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0




#include "method.h"

void SetRecoParam( void )
{
  DB_MSG(("-->SetRecoParam\n"));

  /* set baselevel reconstruction parameters */


  int dim = PTB_GetSpatDim();
  int echoImages = PVM_DwNDiffExp;
  int phase_factor = 1;

  ATB_InitUserModeReco(
    dim,
    0,
    PVM_EncMatrix,
    PVM_Matrix,
    PVM_AntiAlias,
    PVM_EncPftOverscans,
    NI,
    ACQ_obj_order,
    phase_factor,
    PVM_EncSteps1,
    PVM_EncSteps2,
    NULL,
    PVM_EncNReceivers,
    PVM_EncChanScaling,
    PVM_EncPpiRefLines[1],
    PVM_EncPpi[1]);

  ATB_SetRecoRotate(
    NULL,
    PVM_Fov[0] * PVM_AntiAlias[0],
    NSLICES,
    echoImages,
    0);

  ATB_SetRecoRotate(
    PVM_EffPhase1Offset,
    PVM_Fov[1] * PVM_AntiAlias[1],
    NSLICES,
    1,
    echoImages,
    1);

  if (dim == 3)
  {
    ATB_SetRecoRotate(
      PVM_EffPhase2Offset,
      PVM_Fov[2] * PVM_AntiAlias[2],
      NSLICES,
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
    echoImages,
    NI);

  /* ensure consistent method specific reconstruction parameters */
  RecoMethModeRel();

  /* additional parameter settings, if DTI reconstruction is selected */
  if( ACQ_scan_type != Setup_Experiment && RecoMethMode == TensorReconstruction)
  {
    int NDtiFrames = 23;

    // no repetition groups
    RecoNumRepetitions = PVM_NRepetitions;
    RecoRepetitionGroupSize = 1;
    
    // enable PREPPASS to process complete diffusion groups
    RecoPreppassProcDim = RecoDim+1;
    
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

  /* standard settings for reconstruction */
  if(RecoPrototype == No)
    SetRecoParam();

  /* create standard reconstruction network */
  ParxRelsParRelations("RecoUserUpdate", Yes);

  /* adapt reconstruction network if DTI reconstruction is selected */ 
  if(ACQ_scan_type != Setup_Experiment && RecoMethMode == TensorReconstruction) 
  {

    if (RECO_image_type != MAGNITUDE_IMAGE)
    {
      UT_ReportError("For DTI reconstruction RECO_image_type must be set to Magnitude");
      return;
    }
    
    char filterOptions[RECOSTAGENODESIZE]="\0";

    // insert a RecoDTIFilter stage into the PREPPASS
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

    
    // re-derive output process for new RecoObjectsPerRepetition
    RecoDeriveOutputProcess();
    
    DB_MSG(("... rederived output process with %d DTI frames.\n",NDtiFrames));
  }

  DB_MSG(("<--RecoDerive\n"));
}
