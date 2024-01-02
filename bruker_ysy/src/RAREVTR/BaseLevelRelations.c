/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002-2014
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

const char * RAREVTR_BaseLevelRelations_id = "$Id$(C) 2002-2014 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	0


#include "method.h"

void SetBaseLevelParam( void )
{

  DB_MSG(("-->SetBaseLevelParam\n"));

  SetBasicParameters();

  SetFrequencyParameters();

  SetPpgParameters();

  SetAcquisitionParameters();

  SetGradientParameters();

  SetInfoParameters();

  ATB_SetReceiverGains();

  /* setting baselevel parameters used by modules */
  ATB_SetFatSupBaselevel();
  ATB_SetSatTransBaseLevel();
  ATB_SetFovSatBaseLevel();
  ATB_SetFlowSaturationBaseLevel();

  #if DEBUG
    printTiming();
  #endif

  DB_MSG(("<--SetBaseLevelParam\n"));
}



void SetBasicParameters( void )
{
  DB_MSG(("-->SetBasicParameters\n"));

  /* NSLICES */
  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  ATB_SetNSlices( nSlices );

  /* NR */
  ATB_SetNR( T1Exp * PVM_SatTransRepetitions * PVM_NRepetitions );

  /* NI */
  ATB_SetNI( nSlices * PVM_NEchoImages );

  /* AVERAGING */
  switch(PVM_MotionSupOnOff)
  {
    default:
    case Off:
      ATB_SetNA( PVM_NAverages );
      ATB_SetNAE( 1 );
      break;
    case On:
      ATB_SetNAE( PVM_NAverages );
      ATB_SetNA( 1 );
      break;
  }

  /* ACQ_ns */
  ACQ_ns_list_size = PVM_NEchoImages;
  ParxRelsParRelations("ACQ_ns_list_size",Yes);
  for(int echo=0; echo<PVM_NEchoImages; echo++)
    ACQ_ns_list[echo] = 1;

  /* NECHOES */
  NECHOES = NEchoes;

  /* ACQ_obj_order */
  PARX_change_dims("ACQ_obj_order",NI);
  ATB_SetAcqObjOrder( nSlices, PVM_ObjOrderList, PVM_NEchoImages, 1 );


  DB_MSG(("<--SetBasicParameters\n"));
}



void SetFrequencyParameters( void )
{
  DB_MSG(("-->SetFrequencyParameters\n"));


  ATB_SetNuc1(PVM_Nucleus1);

  sprintf(NUC2,"off");
  sprintf(NUC3,"off");
  sprintf(NUC4,"off");
  sprintf(NUC5,"off");
  sprintf(NUC6,"off");
  sprintf(NUC7,"off");
  sprintf(NUC8,"off");

  ATB_SetNucleus(PVM_Nucleus1);

  ACQ_O1_mode = BF_plus_Offset_list;
  ParxRelsParRelations("ACQ_O1_mode",Yes);

  O1 = 0.0;
  O2 = 0.0;
  O3 = 0.0;
  O4 = 0.0;
  O5 = 0.0;
  O6 = 0.0;
  O7 = 0.0;
  O8 = 0.0;

  /* Set BF's to working frequencies on used channels */
  ACQ_BF_enable = No;
  BF1 = PVM_FrqWork[0];
  BF2 = PVM_FrqWork[1];
  /* call relations of BF1 (no need for other BF's) */
  ParxRelsParRelations("BF1", Yes);

  int nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  ATB_SetAcqO1List( nslices,
                    PVM_ObjOrderList,
                    PVM_SliceOffsetHz );

  ATB_SetAcqO1BList( nslices,
                     PVM_ObjOrderList,
                     PVM_ReadOffsetHz);

  DB_MSG(("<--SetFrequencyParameters\n"));
}



void SetGradientParameters( void )
{
  DB_MSG(("-->SetGradientParameters\n"));


  int spatDim = PTB_GetSpatDim();

  PARX_change_dims("ACQ_phase_encoding_mode", spatDim );
  PARX_change_dims("ACQ_phase_enc_start", spatDim );
  switch(spatDim)
  {
    case 3:
      ACQ_phase_encoding_mode[2] = User_Defined_Encoding;
      ACQ_phase_enc_start[2] = -1; /* set, but no used */
      ACQ_spatial_size_2 = PVM_EncMatrix[2];
      ParxRelsCopyPar("ACQ_spatial_phase_2","PVM_EncValues2");
      /* no break */
    case 2:
      ACQ_phase_encoding_mode[1] = User_Defined_Encoding;;
      ACQ_phase_enc_start[1] = -1.0; /* set, but no used */
      ACQ_spatial_size_1 = PVM_EncMatrix[1];
      ParxRelsCopyPar("ACQ_spatial_phase_1","PVM_EncValues1");
      /* no break */
    default:
      ACQ_phase_encoding_mode[0] = Read;
      ACQ_phase_enc_start[0] = -1;
  }
  ParxRelsParRelations("ACQ_phase_encoding_mode",Yes);


  ATB_SetAcqGradMatrix("PVM_SliceGeoObj");


  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 1.0;
  ACQ_scaling_slice = 1.0;

  ACQ_gradient_amplitude[0] = ExcSliceGrad;
  ACQ_gradient_amplitude[1] = RepetitionSpoiler.ampl;
  ACQ_gradient_amplitude[2] = ReadDephGrad + ReadSpoilGradL;
  ACQ_gradient_amplitude[3] = SliceRephGrad + SliceSpoilGradR;
  ACQ_gradient_amplitude[4] = Phase2DGrad;
  ACQ_gradient_amplitude[5] = ReadGrad;
  ACQ_gradient_amplitude[6] = Phase3DGrad;
  ACQ_gradient_amplitude[7] = SliceSpoilGradL;
  ACQ_gradient_amplitude[8] = ReadSpoilGradR;



  DB_MSG(("<--SetGradientParameters\n"));
}

void SetInfoParameters( void )
{
  int slices, i;

  DB_MSG(("-->SetInfoParameters\n"));

  int spatDim = PTB_GetSpatDim();

  ACQ_dim = spatDim;
  ParxRelsParRelations("ACQ_dim",Yes);
  ATB_SetAcqDimDesc( 0, spatDim, NULL );


  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();

  ATB_SetAcqFov( Spatial, spatDim, PVM_Fov, PVM_AntiAlias );

  ACQ_flip_angle = RefPulse1.Flipangle;

  PARX_change_dims("ACQ_echo_time",PVM_NEchoImages);
  for(i=0; i<PVM_NEchoImages; i++)
    ACQ_echo_time[i] = EffectiveTE[i];

  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_repetition_time",T1Exp);
  for(i=0; i < T1Exp; i++ )
  ACQ_repetition_time[i] = MultiRepTime[i];

  PARX_change_dims("ACQ_recov_time",T1Exp);
  for(i=0; i < T1Exp; i++ )
  ACQ_recov_time[i] =  MultiRepTime[i] -
	PVM_EchoTime * ((double)PVM_RareFactor*PVM_NEchoImages - 0.5);

  PARX_change_dims("ACQ_inversion_time",1);
  ACQ_inversion_time[0] = PVM_InversionTime;

  ACQ_n_echo_images = PVM_NEchoImages;
  ACQ_n_movie_frames = T1Exp;
  PARX_change_dims("ACQ_movie_descr",T1Exp,20);
  for(i=0;i<T1Exp;i++)
  {
    snprintf(ACQ_movie_descr[i],20,"VTR=%8.1f ms. ",ACQ_repetition_time[i]);
  }

  ATB_SetAcqSliceAngle( PtrType3x3 PVM_SPackArrGradOrient[0],
			PVM_NSPacks );

  ACQ_slice_orient = Arbitrary_Oblique;

  ACQ_slice_thick = PVM_SliceThick;

  slices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  PARX_change_dims("ACQ_slice_offset",slices);
  PARX_change_dims("ACQ_read_offset",slices);
  PARX_change_dims("ACQ_phase1_offset",slices);
  PARX_change_dims("ACQ_phase2_offset",slices);

  for(i=0;i<slices;i++)
  {
    ACQ_slice_offset[i]  = PVM_SliceOffset[i];
    ACQ_read_offset[i]   = PVM_ReadOffset[i];
    ACQ_phase1_offset[i] = PVM_Phase1Offset[i];
    ACQ_phase2_offset[i] = PVM_Phase2Offset[i];
  }

  ACQ_read_ext = (int)PVM_AntiAlias[0];

  PARX_change_dims("ACQ_slice_sepn", slices==1 ? 1 : slices-1);

  if( slices == 1 )
  {
    ACQ_slice_sepn[0] = 0.0;
  }
  else
  {
    for( i=1; i<slices;i++ )
    {
      ACQ_slice_sepn[i-1]=PVM_SliceOffset[i]-PVM_SliceOffset[i-1];
    }
  }

  ATB_SetAcqSliceSepn( PVM_SPackArrSliceDistance,
                       PVM_NSPacks );



  ACQ_n_t1_points = 1;

  if( ParxRelsParHasValue("ACQ_transmitter_coil") == No )
  {
    ACQ_transmitter_coil[0] = '\0';
  }

  if( ParxRelsParHasValue("ACQ_contrast_agent") == No )
  {
    ACQ_contrast_agent[0] = '\0';
  }

  if( ParxRelsParHasValue("ACQ_contrast") == No )
  {
    ACQ_contrast.volume = 0.0;
    ACQ_contrast.dose = 0.0;
    ACQ_contrast.route[0] = '\0';
    ACQ_contrast.start_time[0] = '\0';
    ACQ_contrast.stop_time[0] = '\0';
  }

  ParxRelsParRelations("ACQ_contrast_agent",Yes);

  ACQ_position_X = 0.0;
  ACQ_position_Y = 0.0;
  ACQ_position_Z = 0.0;

  PARX_change_dims("ACQ_temporal_delay",1);
  ACQ_temporal_delay[0] = 0.0;

  ACQ_RF_power = 0;

  DB_MSG(("<--SetInfoParameters\n"));

}



void SetPpgParameters( void )
{
  DB_MSG(("-->SetPpgParameters\n"));

  // variable MultiRepTime fill delay
  ACQ_vd_list_size = T1Exp;
  ParxRelsParRelations("ACQ_vd_list_size",Yes);
  for(int i=0; i < T1Exp; i++ )
  {
    ACQ_vd_list[i] = (MultiRepTime[i] - PVM_MinRepetitionTime) * 1.0e-3 / NSLICES;
    ACQ_vd_list[i] = MAX_OF(ACQ_vd_list[i], 0.0);
  }
  ParxRelsParRelations("ACQ_vd_list",Yes);

  ATB_SetPulprog("RAREVTR.ppg");

  double ramp = CFG_GradientRampTime() + CFG_InterGradientWaitTime();
  double rise = CFG_GradientRiseTime();

  D[1]  = (ramp + TeFillDelay1)/1000;
  D[2]  = ReadDephDur / 1000.0;
  D[3]  = rise / 1000.0;
  D[5]  = EncGradDur/1000.0;
  D[6]  = (ramp + TeFillDelay2)/1000.0;
  D[9]  = RepetitionSpoiler.dur/1000.0;

  /* set shapes and durations of RF pulses     */
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");
  ATB_SetRFPulse("RefPulse1","ACQ_RfShapes[1]","P[1]");

  /* counters used in pulse program */
  L[1] = PVM_DummyScans;
  L[2] = PVM_EncMatrix[1]/PVM_RareFactor;
  L[3] = T1Exp;
  L[4] = PVM_SatTransRepetitions * PVM_NRepetitions;
  L[5] = (PTB_GetSpatDim() == 3) ? PVM_EncMatrix[2] : 1;

  DB_MSG(("<--SetPpgParameters\n"));
}



void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters\n"));

  JOBPARAMETERS(jobPars);

  /* ------------------------------------------------ */
  /* -------               Job 0              ------- */
  /* ------------------------------------------------ */

  JOBDESCRIPTION job0 = jobPars->getOrCreateJob("job0");
  job0->initPars(1, PVM_EncMatrix[0], PVM_EffSWh * PVM_AntiAlias[0]);

  job0->appendLoop(PVM_NEchoImages * PVM_RareFactor);

  job0->appendLoop(NSLICES);

  job0->appendLoop(NA, LOOP_AVERAGE, LOOP_SETUP);

  job0->appendLoop(PVM_EncMatrix[1] / PVM_RareFactor);

  job0->appendLoop(L[5]);

  job0->appendLoop(NAE, LOOP_AVERAGE);

  job0->appendLoop(L[3]);

  job0->appendLoop(L[4]);



  DB_MSG(("<--SetAcquisitionParameters"));
}



#if DEBUG
#define d(n) (D[n]*1e3)
#define p(n) (P[n]*1e-3)
#define vardel(n) (vardel[n]*1e3)

void printTiming(void)
{
  for (int i = 0; i < 12; i++) {
    std::string msg = "d" + std::to_string(i);
    DB_MSG(("%s = %f ms", msg.c_str(), D[i]*1e3));
  }
  double denab = d(3) - DE*1e-3;
  double de = DE*1e-3;
  double d5m40u = d(5) - 0.04;

  DB_MSG(("de = %f ms", de));
  DB_MSG(("denab = %f ms", denab));
  DB_MSG(("d5m40u = %f ms", d5m40u));

  for (int i = 0; i < 2; i++) {
    std::string msg = "p" + std::to_string(i);
    DB_MSG(("%s = %f ms", msg.c_str(), P[i]*1e-3));
  }

  for (int i = 0; i < 6; i++) {
    std::string msg = "l" + std::to_string(i);
    DB_MSG(("%s = %d", msg.c_str(), L[i]));
  }

  double te1, te2, aqq=PVM_AcquisitionTime, tr;
  double * vardel = ACQ_vd_list;
  for (int i = 0; i < ACQ_vd_list_size; ++i) {
    DB_MSG(("vardel[%d] = %f ms", i, vardel[i]*1e3));
  }

  DB_MSG(("aqq = %f ms", aqq));

  /* 90 - 180 */
  te1 = p(0)/2 + d(2) + d(1) + d(3) + p(1)/2;
  DB_MSG(("te1: %f ms, should be %f ms, diff %f ms", te1, PVM_EchoTime/2, te1-PVM_EchoTime/2));


  /* 180 - 180 */
  te2 = p(1) + aqq + 2*d(3) + 2*d(5) + 2*d(6);
	DB_MSG(("te2: %f ms, should be %f ms, diff %f ms", te2, PVM_EchoTime, te2-PVM_EchoTime));


  /* TR */
  double echo_loop_time = PVM_RareFactor * PVM_NEchoImages * (d(3) + p(1) + d(5) + d(6) + denab + de + aqq + d(6) + d5m40u + 40e-3);
  DB_MSG(("echo_loop_time = %f ms", echo_loop_time));

  for (int i = 0; i < ACQ_vd_list_size; ++i) {
    tr = 0.03 + 0.01 + d(9) + d(3) + p(0) + d(2) + d(1) + echo_loop_time + d(3) + vardel[i]*1e3;

    if(PVM_FlipBackOnOff == On) {
      tr += d(3) + p(1) + d(1) + d(2) + d(3) + p(0);
    }

    tr *= NSLICES;

    DB_MSG(("TR: %f, should be %f, diff %f", tr, MultiRepTime[i], tr - MultiRepTime[i]));
  }

  return;
}

#undef d
#undef p
#endif
