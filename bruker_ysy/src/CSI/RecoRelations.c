/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002 - 2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2002 - 2016 Bruker BioSpin MRI GmbH";

#define DEBUG           0
#define DB_MODULE       0
#define DB_LINE_NR      0



#include "method.h"

void SetRecoParam( void )
{
  DB_MSG(("-->SetRecoParam\n"));
 
  /* set baselevel reconstruction parameter */

  int spatDim = PTB_GetSpatDim(); 
  int specDim = MRT_GetSpecDim();
  int dim = spatDim+specDim;
  int echoImages = PVM_NEchoImages;

  int     effEncMatrix[specDim+spatDim], 
    effMatrix[specDim+spatDim],
    effPftOverscans[specDim+spatDim];
  double  effAntiAlias[specDim+spatDim];

  for(int i=0; i<specDim; i++)
  {
    effEncMatrix[i] = effMatrix[i] = PVM_SpecMatrix[i];
    effAntiAlias[i] = 1.0;
    effPftOverscans[i] = (int)PVM_SpecMatrix[i]/2;
  }
  for(int i=0; i<spatDim; i++)
  {
    effEncMatrix[i+specDim] = PVM_EncMatrix[i];
    effMatrix[i+specDim] = PVM_Matrix[i];
    effAntiAlias[i+specDim] = PVM_AntiAlias[i];
    effPftOverscans[i+specDim] = PVM_EncPftOverscans[i];
  }


  ATB_InitUserModeReco(
    specDim+spatDim,
    0,
    effEncMatrix,
    effMatrix,
    effAntiAlias,
    effPftOverscans,
    NI,
    ACQ_obj_order,
    1,
    PVM_EncSteps0,  //CSI: encoding in first spat dim ("read"),
    PVM_EncSteps1,  //CSI: encoding in second spat dim ("phase"),
    PVM_EncSteps2,  //CSI: encoding in third spat dim ("slice"),
    PVM_EncNReceivers,
    PVM_EncChanScaling,
    spatDim>1? PVM_EncPpiRefLines[1]:0,
    spatDim>1? PVM_EncPpi[1]:0);

  // complex channel combination:
  RecoCombineMode = AddImages;
  
  // spectral dim (0): rotate=0
  ATB_SetRecoRotate(
    NULL,
    PVM_Fov[0] * PVM_AntiAlias[0],
    NSLICES,
    echoImages,
    0);

  // first spatial dim (1): "read offset"
  ATB_SetRecoRotate(
    PVM_EffReadOffset,
    PVM_Fov[0] * PVM_AntiAlias[0],
    NSLICES,
    echoImages,
    1);

  // second spatial dim
  if(dim > 2) 
  {
    ATB_SetRecoRotate(
      PVM_EffPhase1Offset,
      PVM_Fov[1] * PVM_AntiAlias[1],
      NSLICES,
      echoImages,
      2);
  }

  // third spatial dim
  if (dim > 3)
  {
    ATB_SetRecoRotate(
      PVM_EffPhase2Offset,
      PVM_Fov[2] * PVM_AntiAlias[2],
      NSLICES,
      echoImages,
      3);
  }

  ATB_SetRecoPhaseCorr(0.0, 0.0, 0);
  for (int i = 1; i < dim; i++)
  {
    ATB_SetRecoPhaseCorr(50.0, 0.0, i);
  }

  ATB_SetRecoTranspositionFromLoops("PVM_SliceGeo",
                                    spatDim,
                                    1,
                                    NI);


  // correction for CSI

  int trdim = PARX_get_dim("RECO_transposition",1);  
  for(int i=0; i<trdim; i++)
  {
    if(RECO_transposition[i] == 1)
      RECO_transposition[i] = 2;

    //no transposition for 1D spatial data
    if (spatDim<2)  
      RECO_transposition[i] = 0;
  }

  // window (spatial filter) setting
  KFilteringRange(); //init if not set

  if (KFiltering==On)
    setWindowParameters(dim);

  // combine spectra in GSP mode
  ACQ_SetupShuffleProfiles=No;

  /* set Reco Params for RFL*/
  setOptimizationRecoParams(); //sets RecoPhaseChan, too

  DB_MSG(("<--SetRecoParam\n"));

}

void setWindowParameters(int dim)
{

  int i, k_range, size=RECO_ft_size[0];
  int *w;
  int *steps[] = {PVM_EncSteps0, PVM_EncSteps1, PVM_EncSteps2};

  RECO_wdw_mode[0] = NO_WDW;
  for(i=1; i<dim; i++)
    RECO_wdw_mode[i] = USER_DEF_WDW;


  for(i=1;i<dim;i++)
    size=MAX_OF(size, RECO_ft_size[i]);

  PARX_change_dims("RECO_usr_wdw",dim, size);
 
  for(i=1;i<dim;i++)
  {
    w = RECO_usr_wdw[i];
    size = RECO_ft_size[i];
    k_range = UT_AbsMaxOfIntArray(PVM_EncMatrix[i-1], steps[i-1], NULL);
    memset(w, 0, size*sizeof(int));
    fillWindow(w+size/2-k_range, 2*k_range, INT_MAX);
  }
}

void fillWindow(int *win, int size, int max)
/* --------------------------------------------------------------
   writes apodization values to *win assuming it covers k-space 
   from -size/2 till size/2-1 (k=0 on point size/2 for even size)
   -------------------------------------------------------------*/
{
  int i;
  double x;

  for(i=0; i<size; i++)
  {
    x = 2*M_PI*i/(double)size;
    win[i] = (int)( (0.54 - 0.46*cos(x)) * max );
  }
}

void DeriveReco(void)
{
  char avOptions[200], spectCorrOptions[400], options1[PATH_MAX+200], divOptions[PATH_MAX+200], fidFile[PATH_MAX],scaleOptions[200];
  int nrReceivers  = RecoNrActiveReceivers();
  int spatDim = PTB_GetSpatDim();
  int specDim = MRT_GetSpecDim();
  int dim = spatDim+specDim;

  DB_MSG(("-->DeriveReco, %d receivers\n", nrReceivers));

  if (RecoUserUpdate == No)
  {
    DB_MSG(("<--RecoDerive: no update\n"));
    return;
  }

  /* standard settings for reconstruction */  
  if(RecoPrototype == No)
    SetRecoParam();

  /* create standard reconstruction network */

  if(ACQ_scan_type != Setup_Experiment)
  {
    for (int i = 0; i < dim-1; i++) //no FT along spectral dim
    {
      RecoFTOrder[i] = i+1;
    }
    RecoFTOrder[dim-1] = -1;
  }

  for (int i = 0; i < dim; i++) //no phasecorrection if no ft
  {
    if (RECO_ft_mode[i]==NO_FT)
      RECO_pc_mode[i]=NO_PC;
  }

  ParxRelsParRelations("RecoUserUpdate", Yes);

  //Filter for ArrayPhase Adjustment
  ATB_GetRecoDataPoints(PVM_RecoDataPointsRequired,  PVM_EncNReceivers*RECO_inp_size[0]*NI*2, "Q");

  /* insert specials for CSI */

  if(ACQ_scan_type != Setup_Experiment)
  {
    const char *avHook = (RetroFrequencyLock_OnOff==On) ? "SPECTCORR":"CAST";

    if (RetroFrequencyLock_OnOff==On) // in case of Drift correction
    {
      double effDigShift = ACQ_RxFilterInfo[0].groupDelay; //=remaining group delay (after compensation)

      for (int i = 0; i < nrReceivers; i++)
      {
        sprintf(spectCorrOptions,
                "useNav = %d;"
                "refPoint = %f;"
                "digShift = %f;"
                "refcor = 0;"
                "relNavBw = %f",
                (RetroFrequencyLock_OnOff == On) ? 1:0,
                (RetroFrequencyLock_OnOff==On)?effDigShift:0,
                effDigShift,
                PVM_NavSWh/PVM_SpecSWH[0]);

        /*Drift + EDC Correction*/
        RecoComputeAppendStage(RECOFIRSTPASS,i,"CAST","RecoSpectCorrFilter","SPECTCORR",spectCorrOptions);
      }

      const char *castHook = nrReceivers>1? "JSPLIT":"JQ1";

      /*the sink created by default for job1 is not needed*/
      RecoComputeRemoveStage(RECOFIRSTPASS,-1,"JS1");
      if (nrReceivers > 1)
      {
        /*split multichannel data into single data streams*/
        RecoComputeAddStage(RECOFIRSTPASS,-1,"RecoSplitFilter","JSPLIT","dim=1;keepBlocksize=1;");
        RecoComputeConnectStages(RECOFIRSTPASS,-1,"JQ1","JSPLIT");
      }

      /*casting job1-data to float*/
      for (int i = 0; i < nrReceivers; i++)
      {
        RecoComputeAddStage(RECOFIRSTPASS,i,"RecoCastFilter","JCAST","dataRep=FLOAT;wordSize=8;");
        char c1[20];
        sprintf(c1,"JCAST%d",i);
        RecoComputeConnectStages(RECOFIRSTPASS,-1,castHook,c1);     
        RecoComputeConnectStages(RECOFIRSTPASS,i,"JCAST","SPECTCORR");
      }
    }


    /*insert slice specific array phase correction*/
    if (NI>1 && nrReceivers >1)
    {
      for (int i = 0; i < nrReceivers; i++)
      {
        sprintf(scaleOptions, "nFrames=%d; phaseOffset=<PVM_ArrayPhasePerObject[%d]>", NI,i);
        //must be before average and sort filter and after cast filter // after average?
                RecoComputeInsertStage(RECOFIRSTPASS,i,"REORD","RecoScaleFilter","ObjectPhaseCorr",scaleOptions);
      }
    }

  
    /* insert k-space dependent averaging - not for setup!*/
    if (ACQ_ScanPipeJobSettings[0].accumMode==ACCUM_none ||
        ACQ_ScanPipeJobSettings[0].pipelineMode==PIPELINE_raw)
    {
      sprintf(avOptions,"avList=<AverageList>; avListSize=%d;"
              " nObj=%d;", PARX_get_dim("AverageList",1), NI);

      for (int i = 0; i < nrReceivers; i++)
        RecoComputeAppendStage(RECOFIRSTPASS,i,avHook,"RecoAverageFilter","AVE",avOptions);
    }
 
    
    /* insert branching for export-to-topspin-file with accumulated (and corrected) data */

    const char *hook = (RECO_image_type==COMPLEX_IMAGE) ? "M":"G";  //G is not inserted for complex recon

    PvOvlUtilGetProcnoPath(fidFile, PATH_MAX, "fid_proc.64");
    sprintf(options1,"filename=\"%s\";shuffle=true",fidFile); 
    RecoComputeInsertStage(RECOPREPPASS, 0, hook, "RecoTeeFilter", "T","");

    RecoComputeAddStage(RECOPREPPASS,0,"RecoFileSink","FSINK",options1);
    RecoComputeConnectStages(RECOPREPPASS, 0,"T","FSINK");

    //divide by number of channels to avoid integer overflow after channel summation
    sprintf(divOptions, "divisor=%d", nrReceivers);
    RecoComputeInsertStage(RECOPREPPASS,0,"FSINK","RecoDivideFilter","DIV",divOptions);


   /* insert FT along time after channel combination */
    if (RECO_ft_mode[0] != NO_FT && !RecoNoFT)
    {
      RecoComputeInsertStage(RECOPREPPASS, 0, hook, "RecoFTShiftFilter", "FTS",
                             "shift=0.5; winDirection=0; exponent=1");
      RecoComputeAppendStage(RECOPREPPASS, 0, "FTS", "RecoFTFilter", "FT",
                             "direction=0; exponent=1");
    }
    
  }
  DB_MSG(("<--DeriveReco\n"));
}


void setOptimizationRecoParams(void)
{
  DB_MSG(("-->updateOptimizationReco\n"));

  if (PVM_NavOnOff == On && NSLICES<2)
  {
    ParxRelsMakeEditable("RetroFrequencyLock_OnOff");
  }
  else
  {
    RetroFrequencyLock_OnOff=Off;
    ParxRelsMakeNonEditable("RetroFrequencyLock_OnOff");
  }

  /* Setting phase offsets for multi RX channel combination */
  ATB_ArrayPhaseSetRecoPhase();

  DB_MSG(("<--updateOptimizationReco\n"));
}
 	

void localTsDeriveProcParsRel (void)
{  
  switch (TsDeriveProcPars)
  {
    case 0:
      ParxRelsParRelations("TsDeriveProcPars",Yes);
      //default rel sets SI0, DC
      break;
    case 1:
      SI=RECO_size[1];
      if (ACQ_dim==2)
        SI*=NI*NR;
      break;
    case 2:
      SI=RECO_size[2]*NI*NR;
      for (int i=3; i<ACQ_dim; i++)
        SI*=RECO_size[i];
      break;
  }
  PPARMOD=ACQ_dim>2?_3D:_2D;
}


void localTsDeriveAcquParsRel (void)
{
  switch (TsDeriveAcquPars)
  {
    case 0:
      ParxRelsParRelations("TsDeriveAcquPars",Yes);
      //default rel sets TD0, DTYPA, SW, SW_h, GRPDLY
      break;
    case 1:
      TD=RECO_size[1];
      if (ACQ_dim==2)
        TD*=NI*NR;
      break;
    case 2:
      TD=RECO_size[2]*NI*NR;
      for (int i=3; i<ACQ_dim; i++)
        TD*=RECO_size[i];
      break;
  }
  PARMODE=ACQ_dim>2?_3D:_2D;
}
