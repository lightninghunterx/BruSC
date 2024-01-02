/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2005
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

static const char resid[] = "$Id$ (C) 2001 - 2005 Bruker BioSpin MRI GmbH";


#define DEBUG       0
#define DB_MODULE   0
#define DB_LINE_NR  0


#include "method.h"


void SetBaseLevelParam()
{
  DB_MSG(("-->SetBaseLevelParam"));
  
  /* must be called in this order */

  SetBasicParameters();
  
  SetFrequencyParameters();
    
  SetGradientParameters();
  
  SetPpgParameters();
  
  SetAcquisitionParameters(NPro, 1);
  
  ATB_SetFatSupBaselevel();
  ATB_SetFovSatBaseLevel();

  ATB_SetReceiverGains();

  SetInfoParameters();
  
  DB_MSG(("<--SetBaseLevelParam"));
}


void SetBasicParameters( void )
{
  DB_MSG(("-->SetBasicParameters"));
  
  /* NSLICES */
  
  int nSlices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);
  ATB_SetNSlices(nSlices);
  
  /* NR */
  
  ATB_SetNR(PVM_NRepetitions * PVM_EvolutionCycles);
  
  /* NI */
  
  ATB_SetNI(nSlices * PVM_NMovieFrames);
  
  /* AVERAGING */

  switch (PVM_MotionSupOnOff)
  {
    default:
    case Off:
      ATB_SetNA(PVM_NAverages);
      ATB_SetNAE(1);
      break;
    case On:
      ATB_SetNAE(PVM_NAverages);
      ATB_SetNA(1);
      break;
  }
   
  /* NECHOES */
  
  NECHOES = PVM_NEchoImages;
  
  /* ACQ_obj_order */
  
  PARX_change_dims("ACQ_obj_order", NI);
  ATB_SetAcqObjOrder(nSlices, PVM_ObjOrderList, PVM_NMovieFrames, 1);
  
  /* online reconstruction */
  
  ACQ_ScanOnlineReco = RecoOnline; 
  
  DB_MSG(("<--SetBasicParameters"));
}

void SetFrequencyParameters( void )
{
  DB_MSG(("-->SetFrequencyParameters"));

  /* nuclei */

  ATB_SetNuc1(PVM_Nucleus1);
  ATB_SetNucleus(PVM_Nucleus1);
   
  sprintf(NUC2,"off");
  sprintf(NUC3,"off");
  sprintf(NUC4,"off");
  sprintf(NUC5,"off");
  sprintf(NUC6,"off");
  sprintf(NUC7,"off");
  sprintf(NUC8,"off");
    
  /* frequency offsets */
  
  ACQ_O1_mode = BF_plus_Offset_list;
  ParxRelsParRelations("ACQ_O1_mode", Yes);
    
  O1 = 0.0;
  O2 = 0.0;
  O3 = 0.0;
  O4 = 0.0;
  O5 = 0.0;
  O6 = 0.0;
  O7 = 0.0;
  O8 = 0.0;
  
  /* Set BF's to working freuncies on used channels */
  ACQ_BF_enable = No;
  BF1 = PVM_FrqWork[0];
  ParxRelsParRelations("BF1", Yes); // no need for other BF's

  
  // setting of ACQ_O1_list and ACQ_O1B_list is done in
  // function SetBeforeAcquisition()
  
  DB_MSG(("<--SetFrequencyParameters"));
}


void SetGradientParameters( void )
{
  DB_MSG(("-->SetGradientParameters"));
  
  // sets gradient matrix
  ATB_SetAcqGradMatrix("PVM_SliceGeoObj");

  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 1.0;
  ACQ_scaling_slice = 1.0;
  
  DB_MSG(("<--SetGradientParameters"));
}

void SetPpgParameters(void)
{
  DB_MSG(("-->SetPpgParameters"));
    
  ATB_SetPulprog("ZTE.ppg");

  /* set shaped pulse */
  ATB_SetRFPulse("ExcPul", "ACQ_RfShapes[0]", "P[0]");

  DB_MSG(("<--SetPpgParameters"));
}


// required for further usage of images
void SetInfoParameters( void )
{
  DB_MSG(("-->SetInfoParameters"));
  
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();
  
  /* ACQ_dim */
  int spatDim = PTB_GetSpatDim();
  ACQ_dim = spatDim;
  ParxRelsParRelations("ACQ_dim", Yes);
  
  /* ACQ_dim_desc */
  ATB_SetAcqDimDesc(0, spatDim, NULL);
 
  PARX_change_dims("ACQ_phase_encoding_mode", spatDim );
  ACQ_phase_encoding_mode[0] = Read;
  for(int i=1; i<spatDim; i++ )
    ACQ_phase_encoding_mode[i] = User_Defined_Encoding;
  ParxRelsParRelations("ACQ_phase_encoding_mode",Yes);
  
  ATB_SetAcqFov(Spatial, PTB_GetSpatDim(), PVM_Fov, PVM_AntiAlias);
  
  ACQ_flip_angle = ExcPul.Flipangle;
  
  PARX_change_dims("ACQ_echo_time", 1);
  ACQ_echo_time[0] = PVM_EchoTime;
  
  PARX_change_dims("ACQ_inter_echo_time", 1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;
  
  PARX_change_dims("ACQ_repetition_time", 1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;
  
  PARX_change_dims("ACQ_recov_time", 1);
  ACQ_recov_time[0] = PVM_RepetitionTime - PVM_MinRepetitionTime;
  
  PARX_change_dims("ACQ_inversion_time", 1);
  ACQ_inversion_time[0] = PVM_InversionTime;
    
  ATB_EvolutionSetTimePoints(PVM_NRepetitions, OneRepTime);
  
  ACQ_read_ext = (int)PVM_AntiAlias[0];
    
  ACQ_slice_sepn[0] = 0.0;
  ATB_SetAcqSliceSepn(PVM_SPackArrSliceDistance, PVM_NSPacks);
  
  ACQ_n_t1_points = 1;

  ACQ_slice_thick = PVM_SliceThick;
  
  int slices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  
  PARX_change_dims("ACQ_slice_offset",slices);
  PARX_change_dims("ACQ_read_offset",slices);
  PARX_change_dims("ACQ_phase1_offset",slices);
  PARX_change_dims("ACQ_phase2_offset",slices);
  
  for(int i=0;i<slices;i++)
  {
    ACQ_slice_offset[i]  = PVM_SliceOffset[i];
    ACQ_read_offset[i]   = PVM_ReadOffset[i];
    ACQ_phase1_offset[i] = PVM_Phase1Offset[i];
    ACQ_phase2_offset[i] = PVM_Phase2Offset[i];
  }
  
  if (ParxRelsParHasValue("ACQ_transmitter_coil") == No)
  {
    ACQ_transmitter_coil[0] = '\0';
  }
  
  if (ParxRelsParHasValue("ACQ_contrast_agent") == No)
  {
    ACQ_contrast_agent[0] = '\0';
  }
  if (ParxRelsParHasValue("ACQ_contrast") == No)
  {
    ACQ_contrast.volume = 0.0;
    ACQ_contrast.dose = 0.0;
    ACQ_contrast.route[0] = '\0';
    ACQ_contrast.start_time[0] = '\0';
    ACQ_contrast.stop_time[0] = '\0';
  }  
  ParxRelsParRelations("ACQ_contrast_agent", Yes);
  
  ACQ_position_X = 0.0;
  ACQ_position_Y = 0.0;
  ACQ_position_Z = 0.0;

  PARX_change_dims("ACQ_temporal_delay",1);
  ACQ_temporal_delay[0] = 0.0;
  
  ACQ_RF_power = 0;
   
  DB_MSG(("<--SetInfoParameters"));
}

/* set parameters just before acquisition */

void SetBeforeAcquisition( void )
{
  DB_MSG(("-->SetBeforeAcquisition"));

  ParxRelsParRelations("PVM_AcqScanHandler", MODALITY_HIERARCHY);

  double offR = PVM_GradCalConst * GradAmpR * 0.01 * PVM_EffReadOffset[0],
         offP = PVM_GradCalConst * GradAmpP * 0.01 * PVM_EffPhase1Offset[0],
         offS = PVM_GradCalConst * GradAmpS * 0.01 * PVM_EffSliceOffset[0];

  if (PVM_AcqScanHandler == SCAN_EXPERIMENT)
  {
    InitBeforeAcquisition(NPro, 1);

    /* set gradients */

    radialGrad3D(PVM_EncMatrix[0],
                 ProUnderSampling,
                 HalfAcquisition,
                 UseOrigin,
                 Reorder,
                 GradR,
                 GradP,
                 GradS);
                 
    /* set frequency offsets */

    double *gradR = GradR, *gradP = GradP, *gradS = GradS;
    double *offList = ACQ_O1_list;

    if(offR == 0.0 && offP == 0.0 && offS == 0.0)
    {
      ACQ_O1_list_size = 1;
      ParxRelsParRelations("ACQ_O1_list_size", Yes);
      ACQ_O1_list[0] = 0.0; 
    }
    else
    {
      for (int i = 0; i < NPro; i++)
      {
	offList[i] = offR * gradR[i] + offP * gradP[i] + offS * gradS[i];
      }
    }
  }
  else
  {
    InitBeforeAcquisition(NShots, NShots);

    int iPro = UseOrigin == No ? 0 : 1;
        
    // positive gradient
    GradS[iPro] = 1.0;
    ACQ_O1_list[iPro] = offS;
    iPro++;
    
    // negative gradient
    if (HalfAcquisition == No)
    {
      GradS[iPro] =-1.0;
      ACQ_O1_list[iPro] = -offS;
    }
  }

  DB_MSG(("<--SetBeforeAcquisition"));
}

/* initialise parameters that are set just before acquisition */
void InitBeforeAcquisition(int n, int phaseFactor)
{
  DB_MSG(("-->InitBeforeAcquisition(%d, %d)", n, phaseFactor));
  
  SetAcquisitionParameters(n, phaseFactor);
  
  /* radial directions */
  PARX_change_dims("GradR", n);
  PARX_change_dims("GradP", n);
  PARX_change_dims("GradS", n);
                   
  /* frequency offsets */
  ACQ_O1_list_size = n;
  ParxRelsParRelations("ACQ_O1_list_size", Yes);

  /* initialise with zero */    
  memset(GradR, 0, n * sizeof(double));
  memset(GradP, 0, n * sizeof(double));
  memset(GradS, 0, n * sizeof(double));
  memset(ACQ_O1_list, 0, n * sizeof(double));

  DB_MSG(("<--InitBeforeAcquisition"));
}


void SetAcquisitionParameters(int proj, int phaseFactor)
{
  DB_MSG(("-->SetAcquisitionParameters"));
  
  /* acquisition parameters for job0 */
  JOBPARAMETERS(jobParameters);
  JOBDESCRIPTION job0=jobParameters->getOrCreateJob("job0");

  job0->initPars(1, NPoints, PVM_EffSWh * OverSampling);

  job0->appendLoop(phaseFactor, LOOP_SETUP);
  job0->appendLoop(PVM_DummyScans, LOOP_DUMMIES);
  job0->appendLoop(proj);
  job0->appendLoop(NAE, LOOP_AVERAGE);
  job0->appendLoop(NR);
  
  job0->setForceShortFilter(1);

  DB_MSG(("<--SetAcquisitionParameters"));
}
