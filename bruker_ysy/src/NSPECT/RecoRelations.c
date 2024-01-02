/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2003
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

static const char resid[] = "$Id: RecoRelations.c,v 1.1.2.2 2001/01/11 MAWI ";

#define DEBUG           0
#define DB_MODULE       0
#define DB_LINE_NR      0

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
      PVM_SpecMatrix, // acq size
      PVM_SpecMatrix, // image size
      NULL, //effAntiAlias=1,
      NULL, //effPftOverscans=halfsize,
      NSLICES,
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

  /* configure information available during setup mode */
  ACQ_SetupShowDigFilling = Yes;
  ParxRelsParRelations("ACQ_SetupShowDigFilling",Yes); 
  ACQ_SetupShuffleProfiles=No; // combine spectra in GSP mode
 

  /* set Reco Params for EDC and RFL*/
  setOptimizationRecoParams();

  DB_MSG(("<--SetRecoParam"));
}


void DeriveReco(void)
{
  std::stringstream filterOptions;

  char avOptions[200], spectCorrOptions[400], fidFile[PATH_MAX];
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

  /*write reference data to fid.refscan file if reco is derived
    for scan exerpiment or for duplicated processing
    (not for adjustment or for manual call of RecoUserUpdate)*/
  if (ACQ_scan_type == Scan_Experiment || ACQ_completed)
  {
    if (PVM_RefScanYN == Yes)
      writeRefFile();
  }

  /*Averaging*/
  if (ACQ_ScanPipeJobSettings[0].accumMode==ACCUM_none ||
      ACQ_ScanPipeJobSettings[0].pipelineMode==PIPELINE_raw)
  {
    sprintf(avOptions,"avList=<AverageList>; avListSize=1;"
            " nObj=%d; newSize=<RECO_inp_size>;", NI);
    for (int i = 0; i < nrReceivers; i++)
      RecoComputeAppendStage(RECOFIRSTPASS,i,"CAST","RecoAverageFilter","AVE",avOptions);
  }


  if(ACQ_scan_type != Setup_Experiment)
  {
    const char *hook = nrReceivers>1? "SOS":"Q";

    /*SpectCorr*/
    if (RetroFrequencyLock_OnOff==On || Edc_OnOff==On) // in case of Drift or EDC correction
    {
      double effDigShift = ACQ_RxFilterInfo[0].groupDelay; //=remaining group delay (after compensation)
      int filterWidth=0;
  
      if (EdcManualFilter==Yes && Edc_OnOff==On)
      {
        filterWidth=round(EdcFilterWidthHz/PVM_SpecNomRes[0]/2);
      }
      //else: filterWidth==0 -> auto determination.

      for (int i = 0; i < nrReceivers; i++)
      {
        sprintf(spectCorrOptions,
                "useNav = %d;"
                "refPoint = %f;"
                "digShift = %f;"
                "refcor = %d;"
                "refData = <PVM_RefScan[%d]>;"
                "filterWidth = %d;"
                "relNavBw = %f",
                (RetroFrequencyLock_OnOff == On) ? 1:0,
                (RetroFrequencyLock_OnOff==On)?effDigShift:0,
                effDigShift,
                (Edc_OnOff==On) ? 1:0,
                i,
                filterWidth,
                PVM_NavSWh/PVM_SpecSWH[0]);

        /*Drift + EDC Correction*/
        RecoComputeAppendStage(RECOFIRSTPASS,i,"CAST","RecoSpectCorrFilter","SPECTCORR",spectCorrOptions);
      }

      if (RetroFrequencyLock_OnOff==On)
      {
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

 
    /*add filters for CalcLineWidth (if not in prototype mode,
     otherwise, correct calculation and sizes cannot be garantueed)*/
    if (CalcLineWidth==On && RecoPrototype == No)
    {
      InsertDataEvalFilters();
    }

  } // end no setup experiment
  else  //setup experiment
  {
    InsertDataEvalFilters();
  }
  DB_MSG(("<--DeriveReco\n"));
}

void setOptimizationRecoParams(void)
{
  DB_MSG(("-->updateOptimizationReco\n"));

  int nrReceivers  = RecoNrActiveReceivers();

  if (PVM_NavOnOff==On)
  {
    ParxRelsMakeEditable("RetroFrequencyLock_OnOff");
  }
  else
  {
    RetroFrequencyLock_OnOff=Off;
    ParxRelsMakeNonEditable("RetroFrequencyLock_OnOff");
  }


  if (PVM_RefScanYN == Yes)
  {
    ParxRelsMakeEditable("Edc_OnOff");  
  }
  else
  {
    Edc_OnOff = Off;
    ParxRelsMakeNonEditable("Edc_OnOff");
  }

  if (Edc_OnOff==On)
  {
    ParxRelsShowInEditor("EdcManualFilter");
    if (EdcManualFilter==Yes)
    {
      ParxRelsShowInEditor("EdcFilterWidthHz");
    }
    else
    {
      ParxRelsHideInEditor("EdcFilterWidthHz");
    }
  }
  else
  {
    ParxRelsHideInEditor("EdcManualFilter,EdcFilterWidthHz");
  }


  /* Setting phase offsets for multi RX channel combination */
  if (Edc_OnOff==Off)
    ATB_ArrayPhaseSetRecoPhase();
  else
    for (int i = 0; i < nrReceivers; i++)
      RecoPhaseChan[i]=0; /*since EDC shifts channel to 0 phase*/


  /*always reset LineWidth and evaluate (and store) only during reco*/
  LineWidth[0][0][0]=0;

  if (CalcLineWidth==On)
    ParxRelsShowInEditor("LineWidth");
  else
    ParxRelsHideInEditor("LineWidth");


  DB_MSG(("<--updateOptimizationReco\n"));
}


void writeRefFile(void)
{
  DB_MSG(("--> writeRefFile"));
 	 
  uint npoints=RECO_inp_size[0]*2; //real+imag pairs
  uint nchan= RecoNumInputChan;
 	 
  if (PVM_RefScanYN == Yes && ParxRelsParHasValue("PVM_RefScan") == Yes &&
      PARX_get_dim("PVM_RefScan",1)==nchan && PARX_get_dim("PVM_RefScan",2)==npoints)
  {
    FILE *fp=NULL;
 	 
    double *phase = PVM_ArrayPhase;
    double *scale= RecoScaleChan;
 	 
    char fname[PATH_MAX];
    PvOvlUtilGetProcnoPath(fname, PATH_MAX, "fid_refscan.64");
    fp=fopen(fname,"w");
    if(fp!=NULL)
    {
      uint i,k;
      double sumRe, sumIm;
      double re, im;
      for(i=0;i<npoints;i+=2)
      {
        sumRe=0; sumIm=0;
        for (k=0;k<nchan;k++)
        {
          //phase shift and scaling before channel combination
          re=(PVM_RefScan[k][i]*cos(phase[k]/180.0*M_PI) - PVM_RefScan[k][i+1] * sin(phase[k]/180.0*M_PI))*scale[k];
          im=(PVM_RefScan[k][i]*sin(phase[k]/180.0*M_PI) + PVM_RefScan[k][i+1] * cos(phase[k]/180.0*M_PI))*scale[k];
 	           
          sumRe+=re/nchan; //divide by number of channels to avoid integer overflow
          sumIm+=im/nchan;
        }
        fwrite(&sumRe, sizeof(double),1,fp);
        fwrite(&sumIm, sizeof(double),1,fp);
      }
      fclose(fp);
    }
  }
  DB_MSG(("<-- writeRefFile"));
}


/* following functions allow display and calculation of linewidth.
   in PRESS they are also used for LocShim and LocSF */

void SetdisplayInfonames(void)
{

  SetDimOfTuneshimInterfacePars(NSLICES, RecoNrActiveReceivers());
  std::string recoinfopars="PVM_TuneShimFreqOffs/Frequency Offset, LineWidth/Line Width";
  PARX_change_dims("ACQ_RecoInfoNames",recoinfopars.size()+1);
  ParxRelsParSetValue("ACQ_RecoInfoNames",recoinfopars.c_str());
  std::string signalinfopars="NormArea/Normalized Area";
  size_t slen=signalinfopars.size()+1;

  int njobs=0,activejobs=0;
  njobs = (int) PARX_get_dim("ACQ_jobs",1);

  for(int i=0;i<njobs;i++)
  {
    if(ACQ_jobs[i].chanNum != 0)
      activejobs++;
  }
  

  if(activejobs != (int)PARX_get_dim("ACQ_SignalInfoNames",2) ||
     slen > PARX_get_dim("ACQ_SignalInfoNames",2))
  {
    PARX_change_dims("ACQ_SignalInfoNames",activejobs,slen+1);
  }    
  ParxRelsParSetValue("ACQ_SignalInfoNames[0]",signalinfopars.c_str());

}
//SetDimOfDisplayInfoPars
void SetDimOfTuneshimInterfacePars(int nsl, int nchan)
{
    /* PARX_change_dims("NormArea",1,nchan); */
    /* PARX_change_dims("LineWidth",1,1,1); */
    /* PARX_change_dims("Peak",1,1); */
    /* PARX_change_dims("Noise",1,1); */
    /* PARX_change_dims("PVM_TuneShimFreqOffs",1); */

    PARX_change_dims("NormArea", nsl, nchan);
    PARX_change_dims("LineWidth",1 ,nsl, 1);
    PARX_change_dims("Peak",nsl, 1);
    PARX_change_dims("Noise",nsl, 1);
    PARX_change_dims("PVM_TuneShimFreqOffs", nsl);
}

void InsertDataEvalFilters(void)
{
  std::stringstream filterOptions;
  int nrReceivers  = RecoNrActiveReceivers();
  double swh=ACQ_jobs[0].swh;
   /* int scansize=ACQ_jobs[0].scanSize /2; */

  int nsl = NSLICES;

  SetDimOfTuneshimInterfacePars(nsl, nrReceivers);

  SetdisplayInfonames();

  // append filter to calculate normalized area
  RecoComputeAppendStage(RECOFIRSTPASS,
                         0,
                         "Q",
                         "RecoNormalizedAreaFilter",
                         "NAREA",
                         "normalizedArea=<NormArea>;");

  // modify preppass 
  filterOptions.str("");

  /* filterOptions << "nchan=1;" */
  /*               << "nspec=1;" */
  /*               << "swh="     << swh << ";" */
  /*               << "nnoisep=" << MIN_OF(scansize/10,100) << ";" */
  /*               << "applyPhase=true;" */
  /*               << "estLorentzPw=true;" */
  /*               << "pwl={50};" */
  /*               << "pwch=<LineWidth>;" */
  /*               << "peak=<Peak>;noise=<Noise>;" */
  /*               << "wafrqoffs=<PVM_TuneShimFreqOffs>;"; */

  filterOptions << "nchan=1;"
                << "nspec=" << nsl << ";"
                << "swh="     << swh << ";"
                << "estLorentzPw=true;"
                << "pwl={50};"
                << "pwch=<LineWidth>;"
                << "wafrqoffs=<PVM_TuneShimFreqOffs>;";


  RecoComputeAppendStage(RECOPREPPASS,
                         0,
                         DEFAULTIMAGETYPEFILTERNAME,
                         "RecoFrqOffsetFilter",
                         "OFRQ",
                         filterOptions.str().c_str()); 
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
