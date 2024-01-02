/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2008
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 *
 ****************************************************************/
static const char resid[] = "$Id$ (C) 2008 Bruker BioSpin MRI GmbH";

#define DEBUG	     	0
#define DB_MODULE	  0
#define DB_LINE_NR	0

/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "method.h"

void SetRecoParam( void )
{
    DB_MSG(("-->SetRecoParam"));

    ATB_InitUserModeReco(
      1,
      0,
      (IsisManAdjustment==NoManAdjustment)?PVM_SpecMatrix:&PVM_Matrix[0], // acq size
      (IsisManAdjustment==NoManAdjustment)?PVM_SpecMatrix:&PVM_Matrix[0],// image size
      NULL, //effAntiAlias=1,
      NULL, //effPftOverscans=halfsize,
      1,
      NULL,
      1,
      NULL,  //linear order,
      NULL,  //linear order (not used)
      NULL,  //linear order (not used)
      PVM_EncNReceivers,
      PVM_EncChanScaling,
      0,
      1);

  /* complex channel combination: */
    RecoCombineMode = AddImages;
    
    // combine spectra in GSP mode
    ACQ_SetupShuffleProfiles=No;

    /* Setting phase offsets for multi RX channel combination */
    ATB_ArrayPhaseSetRecoPhase();

    DB_MSG(("<--SetRecoParam"));
}

void DeriveReco(void)
{
  std::stringstream filterOptions;

  char avOptions[200], fidFile[PATH_MAX];
  int nrReceivers  = RecoNrActiveReceivers();

  DB_MSG(("-->DeriveReco, nRx= %i",nrReceivers));

  if (RecoUserUpdate == No)
  {
    DB_MSG(("<--RecoDerive: no update"));
    return;
  }

  /* standard settings for reconstruction */  
  if(RecoPrototype == No)
    SetRecoParam();

  /*no automatic FT during first pass but after topspin file generation*/
  if(ACQ_scan_type != Setup_Experiment)
    RecoFTOrder[0]=-1;
  else
    RecoFTOrder[0]=0; //FT during GSP

  ParxRelsParRelations("RecoUserUpdate", Yes);

  //Filter for RefScan and ArrayPhase Adjustment
  ATB_GetRecoDataPoints(PVM_RecoDataPointsRequired,  PVM_EncNReceivers*RECO_inp_size[0]*2, "Q");

 
  if(ACQ_scan_type != Setup_Experiment)
  {
    const char *hook = nrReceivers>1? "SOS":"Q";

    /*Averaging - not in setup mode!*/
    if (ACQ_ScanPipeJobSettings[0].accumMode==ACCUM_none ||
        ACQ_ScanPipeJobSettings[0].pipelineMode==PIPELINE_raw)
    {
      sprintf(avOptions,"avList=<AverageList>; avListSize=1;"
              " nObj=%d; newSize=<RECO_inp_size>;", NI);
      for (int i = 0; i < nrReceivers; i++)
        RecoComputeAppendStage(RECOFIRSTPASS,i,"CAST","RecoAverageFilter","AVE",avOptions);
    }


    /* insert FT along time after channel combination */
    RecoComputeAppendStage(RECOPREPPASS, 0, hook, "RecoFTShiftFilter", "FTS",
                           "shift=0.5; winDirection=0; exponent=1");
    RecoComputeAppendStage(RECOPREPPASS, 0, "FTS", "RecoFTFilter", "FT",
                           "direction=0; exponent=1");

    /* insert branching for export-to-topspin-file with accumulated (and corrected) data */
    PvOvlUtilGetProcnoPath(fidFile, PATH_MAX, "fid_proc.64");
    filterOptions.str("");
    filterOptions << "filename=\"" << fidFile << "\";shuffle=true";

    RecoComputeAppendStage(RECOPREPPASS, 0, hook, "RecoTeeFilter", "T","");
    RecoComputeAddStage(RECOPREPPASS,0,"RecoFileSink","FSINK",filterOptions.str().c_str());
    RecoComputeConnectStages(RECOPREPPASS, 0,"T","FSINK");
    
    //divide by number of channels to avoid integer overflow after channel summation
    filterOptions.str(""); filterOptions << "divisor=" << nrReceivers;
    RecoComputeInsertStage(RECOPREPPASS,0,"FSINK","RecoDivideFilter","DIV",filterOptions.str().c_str());

  }   
  DB_MSG(("<--DeriveReco\n"));
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/

