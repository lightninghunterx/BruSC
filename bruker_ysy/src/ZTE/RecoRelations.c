/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

const char * methods_zte_reco_relations_id = "$Id$ (C) 2001 - 2016 Bruker BioSpin MRI GmbH";

#define DEBUG           0
#define DB_MODULE       0
#define DB_LINE_NR      0

#include "method.h"
#include "common/ovl_toolbox/Utils.h"
#include "libCore/Math/Math.hh"

void RecoSkipRel(void)
{
  DB_MSG(("-->RecoSkipRel"));

  RecoSkipRange();
  SetRecoParam();

  DB_MSG(("<--RecoSkipRel"));
}

void RecoSkipRange(void)
{
  DB_MSG(("-->RecoSkipRange"));

  RecoSkip = ParxRelsParHasValue("RecoSkip") == No ? 0 : MIN_OF(NPoints / 2, MAX_OF(0, RecoSkip));

  DB_MSG(("<--RecoSkipRange"));
}

void RecoT2FactorRel(void)
{
  DB_MSG(("-->RecoT2FactorRel"));

  T2Estimate = 0.0;

  // calculate mean for all receivers
  for (int k = 0; k < PVM_EncNReceivers; k++)
  {
    T2Estimate += RecoT2Factor[k];
  }
  T2Estimate *= PVM_AcquisitionTime / (double)PVM_EncNReceivers;

  DB_MSG(("<--RecoT2FactorRel"));
}

void RecoB0FactorRel(void)
{
  DB_MSG(("-->RecoB0FactorRel"));

  B0Estimate = 0.0;

  // calculate mean for all receivers
  for (int k = 0; k < PVM_EncNReceivers; k++)
  {
    B0Estimate += RecoB0Factor[k];
  }
  B0Estimate *= KHZ_TO_HZ / (2.0 * M_PI * PVM_AcquisitionTime * (double)PVM_EncNReceivers);

  DB_MSG(("<--RecoB0FactorRel"));
}

/* set baselevel reconstruction parameters */
void SetRecoParam(void)
{
  DB_MSG(("-->SetRecoParam"));

  int dim = PTB_GetSpatDim(); 
  int acqMat[3] = {NPoints, NPro, 1};

  /* set up user reco */
  ATB_InitUserModeReco(
    dim,
    dim,
    PVM_EncMatrix,
    PVM_Matrix,
    PVM_AntiAlias,
    NULL,
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
  unsigned int nProfiles = (NPro - (UseOrigin ? 1 : 0)) / (HalfAcquisition ? 1 : 2);
  char trajFile[PATH_MAX];
  PvOvlUtilGetExpnoPath(trajFile, PATH_MAX, "traj");
  ATB_SetRecoRegridN(
    PVM_EncMatrix[0],
    nProfiles,
    RecoRegridNTrajRadial,
    trajFile,
    RecoRegridNDCPre,
    NULL);
    
  /* B0 demodulation */
  double groupDel = ACQ_RxFilterInfo[0].groupDelay; 
  /* temporary solution for groupDel */
  double swh = PVM_EffSWh * OverSampling;
  if((swh >= 600000.0) && (swh < 1000000.0))
    groupDel = groupDel + 0.5;
  if(swh >= 1000000.0)
    groupDel = groupDel + 1.0;

  double dwell = 1000.0 / (PVM_EffSWh * OverSampling); 
  RecoB0DemodOff   = - RecoOffResonance * HZ_TO_KHZ * dwell;
  RecoB0DemodDelay = AcqDelayTotal * USEC_TO_MSEC / dwell - groupDel;
  ParxRelsParRelations("RecoB0DemodOff", Yes);
  
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
    NI);

  
  /* NR */
  RecoEachNR = PVM_NRepetitions == 1 ? No : Yes;
  
  
  /*** method specific settings ***/

  /* parameters for storing reco results */
  PARX_change_dims("RecoAmp0", PVM_EncNReceivers);
  PARX_change_dims("RecoPhc0", PVM_EncNReceivers);
  PARX_change_dims("RecoT2Factor", PVM_EncNReceivers);
  PARX_change_dims("RecoB0Factor", PVM_EncNReceivers);

  MissingData = ( (int)round(groupDel) + RecoSkip + AcqDelayTotal / (dwell* MSEC_TO_USEC) ) / (double)OverSampling;

  /* visibility */
  ParxRelsHideInEditor("RecoUseOrigin");
  ParxRelsHideInEditor("RecoScaleOrigin");
  if (UseOrigin == Yes)
  {
    ParxRelsShowInEditor("RecoUseOrigin");
    if (RecoUseOrigin == Yes)
    {
      ParxRelsShowInEditor("RecoScaleOrigin");
    }
  }

  DB_MSG(("<--SetRecoParam"));
}

/* set up reconstruction */
void RecoDerive(void)
{
  DB_MSG(("-->RecoDerive"));

  if (RecoUserUpdate == No)
  {
    DB_MSG(("<--RecoDerive: no update"));
    return;
  }
  
  /*** parameter setup ***/
 
  const int dim = PTB_GetSpatDim();                    // dimensions
  const unsigned int matrix = PVM_EncMatrix[0];        // matrix size of reconstructed image
  const unsigned int nrReceivers = PVM_EncNReceivers;  // # receivers
  unsigned int nProfiles = RecoRegridNInterleaves;     // number of profiles used for regridding
  bool recoFTShift = false;                            // perform FT shift in real space during AR
  unsigned int procDim = 2;                            // processing dimension for reco pass
  double groupDel = ACQ_RxFilterInfo[0].groupDelay;
  /* temporary solution for groupDel */
  double swh = PVM_EffSWh * OverSampling;
  if((swh >= 600000.0) && (swh < 1000000.0))
    groupDel = groupDel + 0.5;
  if(swh >= 1000000.0)
    groupDel = groupDel + 1.0;     
  double dwell = 1000.0 / (PVM_EffSWh * OverSampling); 
   
  /* special settings for setup */
  if (ACQ_scan_type == Setup_Experiment)
  {    
    ATB_SetRecoRegridNOff();
      
    for (int i = 0; i < dim; i++)
    {
      RECO_size[i]     = matrix;
      RECO_inp_size[i] = matrix;
      RECO_ft_size[i]  = matrix;
    }

    nProfiles = 1;
    procDim = 1;
    recoFTShift = true;
  }  
  
  /*** create trajectory ***/
    
  if (ACQ_scan_type != Setup_Experiment)
  {
    /* standard settings for reconstruction */
    if(RecoPrototype == No)
      SetRecoParam();

    /* make sure that gradient values are known */
    PARX_change_dims("GradR", NPro);
    PARX_change_dims("GradP", NPro);
    PARX_change_dims("GradS", NPro);
      
    radialGrad3D(matrix,
                 ProUnderSampling,
                 HalfAcquisition,
                 UseOrigin,
                 Reorder,
                 GradR,
                 GradP,
                 GradS);
               
    radialTraj3D(matrix,
                 UseOrigin,
                 nProfiles,
                 GradR,
                 GradP,
                 GradS,
                 RecoRegridNTrajFile);
  }
    
  /*** set up reconstruction network ***/
    
  /* initialise */
  const char* bufferId = NULL;              // buffer ID of a pass
  char filterArguments[RECOSTAGENODESIZE];  // stores filter arguments
  char moreArguments[RECOSTAGENODESIZE];    // stores intermediate filter arguments
  char filterId[RECOSTAGENODESIZE];         // stores a filter ID
  RecoDeriveInputProcess();
  RecoInitComputeProcess();
    
  /* first pass */
  bufferId = RecoComputeAddFirstPass(RECOFIRSTPASS, bufferId, nrReceivers, No);

  /* analyse pass */
  if (UseOrigin == Yes)
  {    
    bufferId = RecoComputeAddEmptyPass("ANALYSEPASS", bufferId, 2, nrReceivers);
    
    for (unsigned int n = 0; n < nrReceivers; n++)
    {
      sprintf(filterId, DEFAULTSOURCENAME);
      
      /* extract first profile */
      RecoComputeAddStage("ANALYSEPASS", n, "RecoExtractFilter", "EXTRACT", "");
      RecoComputeDisconnectStages("ANALYSEPASS", n, filterId, DEFAULTSINKNAME);
      RecoComputeConnectStages("ANALYSEPASS", n, filterId, "EXTRACT");
      RecoComputeConnectStages("ANALYSEPASS", n, "EXTRACT.rest", DEFAULTSINKNAME);
        
      /* analyse profile */
      sprintf(filterArguments,
                  "skip = %d; "
                  "phase = <RecoPhc0[%d]>; "
                  "amplitude = <RecoAmp0[%d]>; "
                  "t2 = <RecoT2Factor[%d]>; "
                  "b0 = <RecoB0Factor[%d]>",          
	          (int)ceil(groupDel), n, n, n, n);
      RecoComputeAddStage("ANALYSEPASS", n, "RecoAnalyseFidFilter", "ANALYSE", filterArguments);
      RecoComputeAddStage("ANALYSEPASS", n, "RecoDummySink", "SINK", "");
      RecoComputeConnectStages("ANALYSEPASS", n, "EXTRACT.extr", "ANALYSE");
      RecoComputeConnectStages("ANALYSEPASS", n, "ANALYSE", "SINK");
    }
  }
    
  /* reco pass using results from FID analysis */
  bufferId = RecoComputeAddEmptyPass("RECOPASS", bufferId, procDim, nrReceivers); 
     
  for (unsigned int n = 0; n < nrReceivers; n++)
  {       
    sprintf(filterId, DEFAULTSOURCENAME);
       
    /* re-arrange */
    if (HalfAcquisition == No)
    {
      RecoComputeAppendStage("RECOPASS", n, filterId, "RecoShuffleProfilesFilter", "SHUFFLE", "");
      sprintf(filterId, "SHUFFLE");
    }

    /* algebraic reco */
    sprintf(filterArguments,
                  "sampOver = <OverSampling>; "
                  "recoN = %d; "
                  "recoOver = <RecoOver>; "
                  "groupDelay = %.2f; "
                  "filterDelay = %.2f; "
                  "acqDelay = %.2f; "
                  "skipData = <RecoSkip>; "
                  "recoT2 = %.2f; "
                  "halfAcq = <HalfAcquisition>; "
                  "recoFTShift = %d; "
                  "recoRegularise = <RecoRegularise>",
                  matrix,
	          groupDel,                 
	          0.0,
	          AcqDelayTotal / (dwell * MSEC_TO_USEC),
                  RecoT2Filter / PVM_AcquisitionTime,
                  recoFTShift);
    if (UseOrigin == Yes)
    {
      sprintf(moreArguments,
                  "; "
                  "recoPhc0 = <RecoPhc0[%d]>",
                  n);
      strcat(filterArguments, moreArguments);
        
      if (RecoUseOrigin == Yes)
      {
        sprintf(moreArguments,
                  "; "
                  "recoAmp0 = <RecoAmp0[%d]>; "
                  "recoScaleAmp0 = <RecoScaleOrigin>",
                  n);
        strcat(filterArguments, moreArguments);
      }
    }
    RecoComputeAppendStage("RECOPASS", n, filterId, "RecoInverseFidFilter", "INV", filterArguments);
    sprintf(filterId, "INV");
      
    /* projection correction */
    if (RecoProCorr != ProCorr_None)
    {
      sprintf(filterArguments,
                "over = <OverSampling>; "
                "origin = %d", 
                recoFTShift ? 1 : 0);
      if (RecoProCorr == ProCorr_Pulse || RecoProCorr == ProCorr_Both)
      {
        sprintf(moreArguments, "; pulse = %f", ExcPul.Length / dwell); 
        strcat(filterArguments, moreArguments);
      }
      if (RecoProCorr == ProCorr_Filter || RecoProCorr == ProCorr_Both)
      {
        strcat(filterArguments, "; order = 3");
      }        
      RecoComputeAppendStage("RECOPASS", n, filterId, "RecoFidCorrFilter", "CORR", filterArguments);
      sprintf(filterId, "CORR");
    }

    /* inverse 1D FT */
    if (ACQ_scan_type != Setup_Experiment)
    {
      RecoComputeAppendStage("RECOPASS", n, filterId, RECOFTSHIFTFILTER, "IFS", "winDirection = 0; shift = 0.5");
      RecoComputeAppendStage("RECOPASS", n, "IFS", RECOFTFILTER, "IFT", "direction = 0; exponent = -1");        
      sprintf(filterId, "IFT");        
    }
        
    /* undo reco oversampling */
    if (RecoOver > 1)
    {
      sprintf(filterArguments, "frameDim = 1; sizes = %d; offset = %d", matrix, matrix / 2 * (RecoOver - 1));
      RecoComputeAppendStage("RECOPASS", n, filterId, RECOCUTOFFFILTER, "CUT", filterArguments);
      sprintf(filterId, "CUT");
    }     
      
    if (ACQ_scan_type != Setup_Experiment)
    {
      /* store 1D projections for debugging */
      if (RecoStorePro == Yes)
      {       
        RecoComputeAppendStage("RECOPASS", n, filterId, RECOTEEFILTER, "TEE", "");
        sprintf(filterId, "TEE");
          
        char profilesFile[PATH_MAX];
        PvOvlUtilGetProcnoPath(profilesFile, PATH_MAX, "profiles");
        sprintf(filterArguments, "filename = \"%s%d\"", profilesFile, n);
        RecoComputeAddStage("RECOPASS", n, RECOFILESINK, "PROFILES", filterArguments);
        RecoComputeConnectStages("RECOPASS", n, "TEE", "PROFILES");
        RecoComputeInsertStage("RECOPASS", n, "PROFILES", RECOCASTFILTER, "CAST", "dataRep = SIGNED; wordSize = 4");
      }
    }
    else
    {
      /* multiply data (same number of profiles expected for input and output), sako: not necessary  */
      /* if (NShots > 1) */
      /* { */
      /*   RecoComputeAppendStage("RECOPASS", n, filterId, "RecoProfileToDisplayFilter", "DISP", "factor = <NShots>"); */
      /* } */
    }
  }

  /* 3D regridding */
  if (ACQ_scan_type != Setup_Experiment)
  {
    bufferId = RecoComputeAddRegridNPasses(bufferId, nrReceivers);   
    if(RecoCS){
        /* compressed sensing pass */
        bufferId = RecoComputeAddCSPass("RECOCSPASS",bufferId,nrReceivers,dim); 
    }    
    if(No == RecoNoFT){
        static const char* RecoFTPassName[] = \
        {                                     \
          RECOFTPASS"0",                      \
          RECOFTPASS"1",                      \
          RECOFTPASS"2"                       \
        };
        for (int n = 0; n < dim; n++)
        {
            bufferId = RecoComputeAddFTPass(RecoFTPassName[n], bufferId, n, 1, nrReceivers);
        }
    }
  }

  /* image preparation pass */
  bufferId = RecoComputeAddPrepPass(RECOPREPPASS, bufferId, nrReceivers);

  /* histogram pass */
  if (RECO_map_mode == PERCENTILE_MAPPING)
  {
    bufferId = RecoComputeAddHistogramPass(RECOHISTOGRAMPASS, bufferId);
  }
       
  /* mapping pass */
  bufferId = RecoComputeAddMapPass(RECOMAPPASS, bufferId);

  /* output */
  RecoDeriveOutputProcess();

  DB_MSG(("<--RecoDerive"));
}

/* Check memory requirements of 3D radial algebraic + regridding reconstruction.

  The following steps are performed:
  - Determine maximum available memory.
  - Determine optimum oversampling factor for given parameters.
  - Calculate reduction factor, if required memory is too large.

  Calculations are based on:
  MEM = [(4(2) * aqOver + 9) * nPro * nSamp + 2 * (1 + over^3) * matrix^3] * nCoils * sizeof(double)

  Example call:
  ProUnderSampling *= RecoOptimiseMemory(PVM_Matrix[0], NPro, PVM_EncMatrix[0], OverSampling, &RecoRegridNOver);

  @param matrix  size of reconstructed image (isotropic)
  @param nPro    number of projections
  @param nSamp   number of complex samples per projection
  @param aqOver  acquisition oversampling factor
  @param nCoils  number of coils
  @param origin  Yes, if origin is acquired
  @param over    oversampling factor used for regridding (may be modified within range [1.25, 2.0])

  @return factor by which the number of incoming data points must be reduced
          - 0 if matrix is too large
          - otherwise >= 1
*/
double RecoOptimiseMemory(
  int matrix,
  int nPro,
  int nSamp,
  int aqOver,
  int nCoils,
  YesNo origin,
  double *over)
{
  DB_MSG(("-->RecoOptimiseMemory"));
  DB_MSG(("nCoils = %d", nCoils));

  const double sizePerPoint = (double)sizeof(double);                          // size of a real data point in double precision
  const double nIn          = ((origin ? 4.0 : 2.0) * (double)aqOver + 9.0) *
                              (double)nPro * (double)nSamp * (double)nCoils;   // number of complex points required for input data
  const double nOut         = pow((double)matrix, 3.0) * (double)nCoils;       // number of complex output data points

  int64_t physRam=0;
  int ret = UT_AvailableMemory(&physRam);
  if(ret == -1)
  {
    UT_ReportError("Reco memory optimisation failed: information about physical memory is missing.");
  }
  
  DB_MSG(("Memory available = %f GB", physRam/(1024.0*1024.0*1024)));

  double maxMem = 0.8*physRam;

  // oversampling factor
  *over = (maxMem / sizePerPoint - nIn) / (2.0 * nOut) - 1.0;
  *over = MAX_OF(*over, 1.0);
  *over = pow(*over, 1.0 / 3.0);
  *over = MAX_OF(*over, 1.25);
  *over = MIN_OF(*over, 2.0);
  DB_MSG(("oversampling factor = %f", *over));

  // reduction factor
  double factor = (maxMem / sizePerPoint - 2.0 * (1.0 + pow(*over, 3.0)) * nOut) / nIn;
  double reductionFactor;
  if (factor > 0.0 && factor < 1.0)
  {
    reductionFactor = 1.0 / factor;
  }
  else
  {
    factor = MAX_OF(factor, 0.0);
    factor = MIN_OF(factor, 1.0);
    reductionFactor = factor;
  }
  DB_MSG(("factor = %f", factor));
  DB_MSG(("reduction factor = %f", reductionFactor));

  // required memory
#if DEBUG
  double mem = (nIn * factor + 2.0 * (1.0 + pow(*over, 3.0)) * nOut) * sizePerPoint;
  DB_MSG(("memory required = %lu", (unsigned long)mem));
#endif

  DB_MSG(("<--RecoOptimiseMemory"));
  return reductionFactor;
}

