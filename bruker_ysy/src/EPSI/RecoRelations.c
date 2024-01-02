/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999 - 2003
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

static const char resid[] = "$Id: RecoRelations.c,v 1.1.2.2 2001/01/11 MAWI ";

#define DEBUG           0
#define DB_MODULE       0
#define DB_LINE_NR      0


/****************************************************************/
/****************************************************************/
/*              I N T E R F A C E   S E C T I O N               */
/****************************************************************/
/****************************************************************/

#include "method.h"

void SetRecoParam(int acceleration)
{
  DB_MSG(("-->SetRecoParam\n"));

  
  ATB_EpiSetRecoPars(acceleration, 1, 1, SpecSize);

  /* corrections for EPSI:
     a) derive default transposition (not done in ATB_EpiSetRecoPars for EPSI)
     b) if transposition is required (i.e. RECO_transposition=1),
        swap data dims 1 and 3 (=1st and 2nd spatial dim)
        by settting RECO_transpose=3=RecoDim (works only for spatDim=2D)
     c) No phase correction for spectroscopic dimension.
  */
  
  //a)
  ATB_SetRecoTranspositionFromLoops("PVM_SliceGeo",
                                    PTB_GetSpatDim(),
                                    1,
                                    NI);
  //b)
  int trdim = PARX_get_dim("RECO_transposition",1);
  for(int i=0; i<trdim; i++)
  {
    if (ACQ_scan_type != Setup_Experiment)
    {
      if(RECO_transposition[i] == 1)
        RECO_transposition[i] = RecoDim;
    }
    else
    {
      RECO_transposition[i] = 0;  //for setup
    }
  }

  //c)
  int spatDim = PTB_GetSpatDim(); 
  int specDim = 1;
  int dim = spatDim+specDim;
  for (int i = 0; i < dim; i++)
  {
    ATB_SetRecoPhaseCorr(50.0, 0.0, i);
  }
  //no phase correction for spectroscopic dim
  ATB_SetRecoPhaseCorr(0.0, 0.0, 1); 
  

  /* complex channel combination*/
  RecoCombineMode = AddImages;

  ATB_ArrayPhaseSetRecoPhase();
  
  DB_MSG(("Exiting SetRecoParam"));
}

/* Relation of RecoUserUpdate, called at start of reconstruction
   to set reconstrution process */
 
void RecoDerive(void)
{
  DB_MSG(("-->RecoDerive\n"));

  /* standard settings for reconstruction */
  if(RecoPrototype == No) //do not call in grappa adj
  {
    if (PVM_EpiAdjustMode != 2)  //do not call in grappa adj
    {
      SetRecoParam(PVM_EncPpi[1]);
    }
    else  //just reset transposition
    {
      int trdim = PARX_get_dim("RECO_transposition",1);
      for(int i=0; i<trdim; i++)
      {
        RECO_transposition[i] = 0;  //for setup
      }
    }      
  }

  int dim = ACQ_dim;
  if(ACQ_scan_type != Setup_Experiment)
  {
    for (int i = 1; i < dim-1; i++) //no FT along spectral(=2nd) dim
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

  ATB_EpiSetRecoProcess(PVM_EncNReceivers, 
                        8,   /*PVM_EncCentralStep1*/
                        PVM_NEchoImages);

  //Filter ArrayPhase Adjustment
  ATB_GetRecoDataPoints(PVM_RecoDataPointsRequired,  PVM_EncNReceivers*PVM_EpiNSamplesPerScan*NI*2, "Q");

  if(ACQ_scan_type != Setup_Experiment)
  {
  char options1[PATH_MAX+200], divOptions[PATH_MAX+200], fidFile[PATH_MAX], scaleOptions[200];
  int nrReceivers  = RecoNrActiveReceivers();

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

  const char *hook =(RECO_image_type==COMPLEX_IMAGE) ? "M":"G";  //G is not inserted for complex recon


  /* insert branching for export-to-topspin-file with data after accumulation and channel combination*/
  PvOvlUtilGetProcnoPath(fidFile, PATH_MAX, "fid_proc.64");
  sprintf(options1,"filename=\"%s\";shuffle=true",fidFile); 
  RecoComputeInsertStage(RECOPREPPASS, 0, hook, "RecoTeeFilter", "T","");

  RecoComputeAddStage(RECOPREPPASS,0,"RecoFileSink","FSINK",options1);
  RecoComputeConnectStages(RECOPREPPASS, 0,"T","FSINK");

  //divide by number of channels to avoid integer overflow after channel summation
  sprintf(divOptions, "divisor=%d", nrReceivers);
  RecoComputeInsertStage(RECOPREPPASS,0,"FSINK","RecoDivideFilter","DIV",divOptions);

   /* insert FT along time after channel combination */
    if (RECO_ft_mode[1] != NO_FT && !RecoNoFT)
    {
       RecoComputeInsertStage(RECOPREPPASS, 0, hook, "RecoFTShiftFilter", "FTS",
                              "shift=0.5; winDirection=1; exponent=1");
       RecoComputeAppendStage(RECOPREPPASS, 0, "FTS", "RecoFTFilter", "FT",
                         "direction=1; exponent=1");
    }
  }
  DB_MSG(("<--RecoDerive\n"));
}

void localTsDeriveProcParsRel (void)
{  
  switch (TsDeriveProcPars)
  {
    case 0:
      ParxRelsParRelations("TsDeriveProcPars",Yes);
      //default rel sets DC
      SI=RECO_size[0];
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
      //default rel sets DTYPA, SW, SW_h, GRPDLY
      TD=RECO_size[0];
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
