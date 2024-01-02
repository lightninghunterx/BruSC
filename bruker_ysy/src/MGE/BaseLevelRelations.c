/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001-2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2001 - 2011 Bruker BioSpin MRI GmbH";


#define DEBUG		0
#define DB_MODULE	0
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
  ATB_SetTaggingBaseLevel();
  ATB_SetSelIrBaseLevel(GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices ));
  ATB_SetBlBloodBaseLevel();


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
  ATB_SetNR( PVM_SatTransRepetitions * PVM_NRepetitions * PVM_EvolutionCycles);

  /* NI */
  ATB_SetNI( nSlices * PVM_NEchoImages);

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
  PARX_change_dims("ACQ_ns_list", ACQ_ns_list_size);
  for (int i = 0; i < ACQ_ns_list_size; i++) {
    ACQ_ns_list[i] = 1;
  }

  /* NECHOES */
  NECHOES = PVM_NEchoImages;

  /* ACQ_obj_order */
  PARX_change_dims("ACQ_obj_order", NI);
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



  /* setting of SW_h DIGMOD, DSPFIRM and AQ_mod */
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

  /* Set BF's to working freuncies on used channels */
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

  ACQ_O1B_list_size = nslices *PVM_NEchoImages;
  PARX_change_dims("ACQ_O1B_list",ACQ_O1B_list_size);

  int i,j,k;
  i=nslices-1;
  while(i>=0)
  {
    for(j=0;j<PVM_NEchoImages;j++)
    {k = i*PVM_NEchoImages+j;
      ACQ_O1B_list[k] = ACQ_O1B_list[i];
    }
    i=i-1;
  }

  if(EchoAcqMode == allEchoes)
  {
    for(i=1;i<PVM_NEchoImages;i=i+2)
      for(j=0;j<nslices;j++)
      {
	k= j*PVM_NEchoImages+i;
	ACQ_O1B_list[k] = -1.0*ACQ_O1B_list[k-1];
      }
  }

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
      /* no break */
    case 2:
      ACQ_phase_encoding_mode[1] = User_Defined_Encoding;;
      ACQ_phase_enc_start[1] = -1.0; /* set, but no used */
      ACQ_spatial_size_1 = PVM_EncGenTotalSteps;
      ACQ_spatial_size_2 = PVM_EncGenTotalSteps;
      ParxRelsCopyPar("ACQ_spatial_phase_1","PVM_EncGenValues1");
      ParxRelsCopyPar("ACQ_spatial_phase_2","PVM_EncGenValues2");
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

  /* gradient amplitudes */
  ACQ_gradient_amplitude[0] =  ExcSliceGrad;
  ACQ_gradient_amplitude[1] = -ExcSliceRephGrad;
  ACQ_gradient_amplitude[2] = -ReadDephGrad;
  ACQ_gradient_amplitude[3] =  Phase2DGrad;
  ACQ_gradient_amplitude[4] = -Phase3DGrad;
  ACQ_gradient_amplitude[5] =  ReadGrad;
  ACQ_gradient_amplitude[6] =  ReadSpoiler.ampl;
  ACQ_gradient_amplitude[7] = -Phase2DGrad;
  ACQ_gradient_amplitude[8] =  Phase3DGrad;
  ACQ_gradient_amplitude[9] =  SliceSpoiler.ampl;
  ACQ_gradient_amplitude[10] = RephaseGrad;

  DB_MSG(("<--SetGradientParameters"));

}



void SetInfoParameters( void )
{
  int slices, i;

  DB_MSG(("-->SetInfoParameters\n"));

  /* ACQ_dim */
  int spatDim = PTB_GetSpatDim();

  ACQ_dim = spatDim;
  ParxRelsParRelations("ACQ_dim",Yes);

  /* ACQ_dim_desc */
  ATB_SetAcqDimDesc( 0, spatDim, NULL );

  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();


  ATB_SetAcqFov( Spatial, spatDim, PVM_Fov, PVM_AntiAlias );

  ACQ_flip_angle = ExcPulse1.Flipangle;

  PARX_change_dims("ACQ_echo_time",PVM_NEchoImages);

  for(i=0;i<PVM_NEchoImages;i++)
    ACQ_echo_time[i] = EffectiveTE[i];

  ACQ_n_echo_images = PVM_NEchoImages;

  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;

  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] =  PVM_RepetitionTime - ExcPulse1.Length;

  /* calculation of ACQ_time_points */
  ATB_EvolutionSetTimePoints(PVM_NRepetitions, OneRepTime);

  PARX_change_dims("ACQ_inversion_time",1);
  ACQ_inversion_time[0] = PVM_InversionTime;

  strcpy(ACQ_fit_function_name,"t2vtr");

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

  ATB_SetPulprog("MGE.ppg");

  double igwt = CFG_InterGradientWaitTime();
  double riseTime = CFG_GradientRiseTime();
  double rampTime = CFG_GradientRampTime() + igwt;

  int slices = NSLICES;
  if(PVM_SelIrOnOff==On)
  {
    D[0]  = igwt / 1000.0 + 10e-6;
    D[20] = SliceSegEndDelay/1000.0 + 20e-6;
  }
  else
  {
    D[0]  = ((PVM_RepetitionTime - PVM_MinRepetitionTime)/slices + igwt) / 1000.0 + 30e-6;
    D[20] = 0.0;
  }
  D[2]  = (TeFillDelay + rampTime) / 1000.0;
  D[4]  = rampTime / 1000.0;
  D[3]  = riseTime / 1000.0;
  D[10] = EncGradDur / 1000.0;
  D[11] = D[10];
  D[12] = (ReadSpoiler.dur - EncGradDur)/1000.0;
  D[13] = (EchoFillDelay+riseTime)/1000.0;
  D[14] = (RephaseTime-riseTime)/1000.0;
  D[6]  = SliceSpoiler.dur/1000.0;

  /* set shaped pulses, in this method ACQ_RfShapes[0] is used
     the pulse duration is stored in baselevel parameter P[0]
  */
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");

  L[1] = PVM_EncGenTotalSteps;
  L[3] = PVM_DummyScans;
  L[10] = EchoAcqMode == allEchoes? PVM_NEchoImages/2 : PVM_NEchoImages;

  ParxRelsParRelations("L",Yes);

  DB_MSG(("<--SetPpgParameters\n"));
}


void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters"));

  JOBPARAMETERS(jobPars);


  /* ------------------------------------------------ */
  /* -------               Job 0              ------- */
  /* ------------------------------------------------ */

  JOBDESCRIPTION job0 = jobPars->getOrCreateJob("job0");
  job0->initPars(1, PVM_EncMatrix[0], PVM_EffSWh * PVM_AntiAlias[0]);

  job0->appendLoop(PVM_NEchoImages);

  job0->appendLoop(NSLICES);

  job0->appendLoop(PVM_DummyScans, LOOP_DUMMIES);

  job0->appendLoop(NA, LOOP_AVERAGE, LOOP_SETUP);

  job0->appendLoop(L[1]);

  job0->appendLoop(NAE, LOOP_AVERAGE);

  job0->appendLoop(NR);

  DB_MSG(("<--SetAcquisitionParameters"));
}


#if DEBUG
#define d(n) (D[n]*1e3)
#define p(n) (P[n]*1e-3)
#define l(n) L[n]

void printTiming(void)
{
  double te, aqq=PVM_AcquisitionTime, tr;
  double echospacing;

  /* TE */
  te = p(0)/2 + d(4) + d(10) + d(2) + d(3) + aqq/2;
  DB_MSG(("TE: %f ms, should be %f ms, diff %f ms", te, PVM_EchoTime, te - PVM_EchoTime));

  /* echo spacing */
  double echo_loop_time = 0;
  if(EchoAcqMode == allEchoes) {
    echospacing = d(3) + aqq + d(13) + 0.01;
    echo_loop_time = 2 * (d(3) + aqq + d(13) + 0.01);
  }
  else {
    echospacing = d(3) + aqq + 2*d(13) + d(14) + 0.02;
    echo_loop_time = d(3) + aqq + 2*d(13) + d(14) + 0.02;
  }
  DB_MSG(("echo_loop_time: %f ms", echo_loop_time));
  DB_MSG(("EchoSpacing: %f, should be %f, diff %f", echospacing, EchoSpacing, echospacing-EchoSpacing));

  tr = 0.03 + d(6) + d(3) + p(0) + d(4) + d(10) + d(2) + echo_loop_time*l(10) + d(11) + d(12) + d(3) + d(0);
  tr *= NSLICES;
  tr += d(20);
  DB_MSG(("TR: %f ms, should be %f ms, diff %f ms", tr, PVM_RepetitionTime, tr-PVM_RepetitionTime));

  double tetr;
  double del;
  if(EchoAcqMode == allEchoes)
    del = d(13) + 0.01;
  else
    del = 2*d(13) + d(14) + 0.02;
  tetr = 0.03 + d(6) + d(3) + p(0)/2 + te + aqq/2 + echospacing*(PVM_NEchoImages-1) + del + d(11) + d(12) + d(3) + d(0);
  tetr *= NSLICES;
  tetr += d(20);
  DB_MSG(("TETR: %f ms, should be %f ms, diff %f ms", tetr, PVM_RepetitionTime, tetr-PVM_RepetitionTime));

  return;
}

#undef d
#undef p
#endif
