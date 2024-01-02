/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2007
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

static const char resid[] = "$Id$ (c) 2007 Bruker BioSpin MRI GmbH";


#define DEBUG       1


#include "method.h"

void SetBaseLevelParam()
{
  DB_MSG(("-->SetBaseLevelParam"));

  SetBasicParameters();

  SetFrequencyParameters();

  SetGradientParameters();

  SetPpgParameters();

  SetAcquisitionParameters();

  ATB_SetReceiverGains();

  ATB_SetFatSupBaselevel();
  ATB_SetSatTransBaseLevel();
  ATB_SetFovSatBaseLevel();
  ATB_SetTaggingBaseLevel();

  SetInfoParameters();

  #if DEBUG
    printTiming();
  #endif

  DB_MSG(("<--SetBaseLevelParam"));
}

void SetBasicParameters( void )
{
  DB_MSG(("-->SetBasicParameters"));


   /* NSLICES */

  int nSlices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);
  ATB_SetNSlices(nSlices);

  /* NR */

  ATB_SetNR( PVM_SatTransRepetitions * PVM_NRepetitions * PVM_EvolutionCycles );

  /* NI */

  ATB_SetNI(nSlices * PVM_NMovieFrames);

  /* AVERAGING */

  switch(PVM_MotionSupOnOff)
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

  DB_MSG(("<--SetBasicParameters"));
}

void SetFrequencyParameters( void )
{
  DB_MSG(("-->SetFrequencyParameters"));

  ATB_SetNuc1(PVM_Nucleus1);

  sprintf(NUC2, "off");
  sprintf(NUC3, "off");
  sprintf(NUC4, "off");
  sprintf(NUC5, "off");
  sprintf(NUC6, "off");
  sprintf(NUC7, "off");
  sprintf(NUC8, "off");

  ATB_SetNucleus(PVM_Nucleus1);

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

  int nslices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);

  ATB_SetAcqO1List(nslices,
                   PVM_ObjOrderList,
                   PVM_SliceOffsetHz);

  ATB_SetAcqO1BList(nslices,
                    PVM_ObjOrderList,
                    PVM_ReadOffsetHz);



  DB_MSG(("<--SetFrequencyParameters"));
}

void SetGradientParameters( void )
{
  DB_MSG(("-->SetGradientParameters"));

  int spatDim = PTB_GetSpatDim();
  PARX_change_dims("ACQ_phase_encoding_mode", spatDim );
  PARX_change_dims("ACQ_phase_enc_start", spatDim );
  switch(spatDim)
  {
    case 3:
      ACQ_phase_encoding_mode[2] = User_Defined_Encoding;
      ACQ_phase_enc_start[2] = -1.0; /* set, but no used */
      ACQ_spatial_size_2 = PVM_EncMatrix[2];
      ParxRelsCopyPar("ACQ_spatial_phase_2", "PVM_EncValues2");
      /* no break */
    default:
      ACQ_phase_encoding_mode[1] = Read;
      ACQ_phase_enc_start[1] = -1.0;
      ACQ_phase_encoding_mode[0] = Read;
      ACQ_phase_enc_start[0] = -1.0;
  }
  ParxRelsParRelations("ACQ_phase_encoding_mode", Yes);

  ATB_SetAcqGradMatrix("PVM_SliceGeoObj");

  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 1.0;
  ACQ_scaling_slice = 1.0;

  DB_MSG(("<--SetGradientParameters"));
}

void SetPpgParameters(void)
{
  DB_MSG(("-->SetPpgParameters"));

  ATB_SetPulprog("SPIRAL.ppg");

  /* set shaped pulses */
  ATB_SetRFPulse("ExcPul", "ACQ_RfShapes[0]", "P[0]");
  if (PVM_SignalType == SignalType_Echo)
  {
    ATB_SetRFPulse("RefPul", "ACQ_RfShapes[1]", "P[1]");
  }

  int spatDim = PTB_GetSpatDim();
  L[2] = spatDim>2 ? PVM_EncMatrix[2] : 1;

  DB_MSG(("<--SetPpgParameters"));
}


void SetInfoParameters( void )
{
  DB_MSG(("-->SetInfoParameters"));

  /* ACQ_dim */

  int spatDim = PTB_GetSpatDim();
  ACQ_dim = spatDim;
  ParxRelsParRelations("ACQ_dim", Yes);

  /* ACQ_dim_desc */

  ATB_SetAcqDimDesc(0, spatDim, NULL);

  ATB_SetAcqFov(Spatial, spatDim, PVM_Fov, PVM_AntiAlias);

  ACQ_flip_angle = ExcPul.Flipangle;

  PARX_change_dims("ACQ_echo_time", 1);

  ACQ_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_inter_echo_time", 1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_repetition_time", 1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;

  PARX_change_dims("ACQ_recov_time", 1);
  ACQ_recov_time[0] = PVM_RepetitionTime - (PVM_EchoTime / 2.0 + ExcPul.Length / 2.0);

  ATB_EvolutionSetTimePoints(PVM_NRepetitions, OneRepTime);

  PARX_change_dims("ACQ_inversion_time", 1);
  ACQ_inversion_time[0] = PVM_InversionTime;

  ATB_SetAcqSliceAngle(
    PtrType3x3 PVM_SPackArrGradOrient[0],
		PVM_NSPacks);

  ACQ_slice_orient = Arbitrary_Oblique;

  ACQ_slice_thick = PVM_SliceThick;

  int nSlices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);

  PARX_change_dims("ACQ_slice_offset",  nSlices);
  PARX_change_dims("ACQ_read_offset",   nSlices);
  PARX_change_dims("ACQ_phase1_offset", nSlices);
  PARX_change_dims("ACQ_phase2_offset", nSlices);

  for (int i = 0; i < nSlices; i++)
  {
    ACQ_slice_offset[i]  = PVM_SliceOffset[i];
    ACQ_read_offset[i]   = PVM_ReadOffset[i];
    ACQ_phase1_offset[i] = PVM_Phase1Offset[i];
    ACQ_phase2_offset[i] = PVM_Phase2Offset[i];
  }

  ACQ_read_ext = (int)PVM_AntiAlias[0];

  PARX_change_dims("ACQ_slice_sepn", nSlices == 1 ? 1 : nSlices - 1);

  if (nSlices == 1)
  {
    ACQ_slice_sepn[0] = 0.0;
  }
  else
  {
    for (int i = 1; i < nSlices; i++)
    {
      ACQ_slice_sepn[i-1] = PVM_SliceOffset[i] - PVM_SliceOffset[i-1];
    }
  }

  ATB_SetAcqSliceSepn(
    PVM_SPackArrSliceDistance,
    PVM_NSPacks);

  ACQ_n_t1_points = 1;

  if (!ParxRelsParHasValue("ACQ_contrast_agent"))
  {
    ACQ_contrast_agent[0] = '\0';
  }

  if (!ParxRelsParHasValue("ACQ_contrast"))
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

  PARX_change_dims("ACQ_temporal_delay", 1);
  ACQ_temporal_delay[0] = 0.0;

  ACQ_RF_power = 0;

  /* frames come inside the slice loop - they need to be declared as echo images */
  ACQ_n_echo_images = PVM_NMovieFrames;
  if (ACQ_n_echo_images > 1)
  {
    PARX_change_dims("ACQ_echo_descr", ACQ_n_echo_images, 20);
    for (int i = 0; i < ACQ_n_echo_images; i++)
    {
      snprintf(ACQ_echo_descr[i], 20, "frame %d", i+1);
    }
  }
  ACQ_n_movie_frames = 1;

  DB_MSG(("<--SetInfoParameters"));
}


void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters"));

  int spatDim = PTB_GetSpatDim();
  /* ------------------------------------------------ */
  /* -------               Job 0              ------- */
  /* ------------------------------------------------ */
  JOBPARAMETERS(jobParameters);
  JOBDESCRIPTION job0=jobParameters->getOrCreateJob("job0");

  job0->initPars( 1, PVM_SpiralNbOfAcqPoints, PVM_EffSWh );

  job0->appendLoop(PVM_NMovieFrames);
  job0->appendLoop(NSLICES);
  job0->appendLoop(NA, LOOP_AVERAGE, LOOP_SETUP);
  job0->appendLoop(PVM_DummyScans, LOOP_DUMMIES);
  job0->appendLoop(PVM_SpiralNbOfInterleaves);
  for (int i = 2; i < spatDim; i++)
    job0->appendLoop(PVM_EncMatrix[i]);
  job0->appendLoop(NAE, LOOP_AVERAGE);
  job0->appendLoop(NR);

  DB_MSG(("<--SetAcquisitionParameters"));
}


#if DEBUG
#define p(n) (P[n]*1e-3)

void printTiming(void)
{
  double aqq    = PVM_SpiralModuleTime;  // duration of subroutine spiralAcq (minus DE)
  double dGrad  = CFG_InterGradientWaitTime();
  double dRise  = CFG_GradientRiseTime();
  double dFq    = FreqSwitch;
  double dSpoil = SliceSpoiler.dur;
  double dReph  = RephGradDur;
  double dRfc   = RfcSpoilDur;
  double dTE    = TEWait;
  double dTErfc = TEWaitRfc;
  double dTR    = TRWait;


  /* Echo Time */
  double te = p(0)/2 + 2*dRise + dGrad + dReph + dTE;
  if (PVM_SignalType == SignalType_Echo) {
    te += 2*dRfc + 2*dRise + p(1) + dGrad + dTErfc;
  }
  DB_MSG(("te = %f ms, should be %f ms, diff = %f ms\n", te, PVM_EchoTime, te - PVM_EchoTime));

  /* TR */
  double tr = dGrad + dSpoil + dRise + p(0)/2 + te + aqq;
  if (PVM_SpatDim == 3)
  {
    tr += dReph + dRise;
  }
  tr *= PVM_NMovieFrames;

  tr += dTR + 2*dFq;
  tr *= NSLICES;

  DB_MSG(("tr = %f ms, should be %f ms, diff = %f ms", tr, PVM_RepetitionTime, tr - PVM_RepetitionTime));
}

#undef p
#endif
