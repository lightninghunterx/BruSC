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

static const char resid[] = "$Id$ (C) 2001-2009 Bruker BioSpin MRI GmbH";

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
  ATB_SetNR(PVM_NRepetitions * PVM_EvolutionCycles );
  
  /* NI */
  ATB_SetNI( nSlices * PVM_NEchoImages); 
  
  /* AVERAGING */
  ATB_SetNAE( PVM_NAverages );
  ATB_SetNA( 1 );
  
  /* NECHOES */
  NECHOES = PVM_NEchoImages;
  
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
    
  
  DB_MSG(("<--SetFrequencyParameters\n"));
}

void SetGradientParameters( void )
{
  int spatDim;
  
  DB_MSG(("-->SetGradientParameters\n"));
  
  spatDim = PTB_GetSpatDim();
  
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
  
   
  /* gradient amplitudes */
  ACQ_gradient_amplitude[0] =  ExcSliceGrad;
  ACQ_gradient_amplitude[1] = -ExcSliceRephGrad;
  ACQ_gradient_amplitude[2] = -ReadDephGrad;
  ACQ_gradient_amplitude[3] =  Phase2DGrad;
  ACQ_gradient_amplitude[4] = -Phase3DGrad;
  ACQ_gradient_amplitude[5] =  ReadGrad;
  ACQ_gradient_amplitude[7] = -DESS_Phase2Rew; 
  ACQ_gradient_amplitude[8] =  DESS_Phase3Rew; 
  ACQ_gradient_amplitude[10] = DESS_ReadSpoilGrad; 
  ACQ_gradient_amplitude[11] = DESS_SliceSpoilGrad; 
  ACQ_gradient_amplitude[12] = -DESS_SlicePreGradAmp;
  ACQ_gradient_amplitude[13] = -DESS_ReadRew;
  
  
  DB_MSG(("<--SetGradientParameters\n"));
}

void SetInfoParameters( void )
{
  int slices, i, spatDim;

  
  DB_MSG(("-->SetInfoParameters\n"));

  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();

  spatDim = PTB_GetSpatDim();

  /* ACQ_dim */  
  ACQ_dim = spatDim;
  ParxRelsParRelations("ACQ_dim",Yes);
  
  /* ACQ_dim_desc */
  ATB_SetAcqDimDesc( 0, spatDim, NULL );

  
  ATB_SetAcqFov( Spatial, spatDim, PVM_Fov, PVM_AntiAlias );
 
  ACQ_flip_angle = ExcPulse1.Flipangle;
  
  ACQ_n_echo_images = PVM_NEchoImages; 
  ACQ_n_movie_frames = 1;               
  
  PARX_change_dims("ACQ_echo_time",2);
  ACQ_echo_time[0] = PVM_EchoTime;
  ACQ_echo_time[1] = DESS_TE2;
  
  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;
  
  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;
  
  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] =  PVM_RepetitionTime - ExcPulse1.Length;

  /* calculation of ACQ_time_points */
  ATB_EvolutionSetTimePoints(PVM_NRepetitions, OneRepTime);
  
  PARX_change_dims("ACQ_inversion_time",1);
  ACQ_inversion_time[0] = 0;
  
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
  
  ACQ_n_echo_images = PVM_NEchoImages;
 
  DB_MSG(("<--SetInfoParameters\n"));
  
}


void SetPpgParameters( void )
{
  
  DB_MSG(("-->SetPpgParameters\n"));
  
  ATB_SetPulprog("DESS.ppg");
  
  double igwt = CFG_InterGradientWaitTime();
  double riseTime = CFG_GradientRiseTime();
  
  D[0]  = igwt / 1000.0 + 0.01 / 1000.0;
  D[2]  = (TeFillDelay + riseTime) / 1000.0;
  D[3]  = riseTime / 1000.0;
  D[10] = EncGradDur / 1000.0;  
  D[13] = (DESS_spoiler_duration - riseTime)/ 1000.0; 
  D[14] = DESS_TE2Delay/1000.0;
  D[15] = DESS_RewDur / 1000.0;
  
  /* set shaped pulses, in this method ACQ_RfShapes[0] is used           
     the pulse duration is stored in baselevel parameter P[0]
  */
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");

  int dim = PTB_GetSpatDim();

  L[1] = dim > 1 ? PVM_EncMatrix[1]:1;
  L[2] = dim > 2 ? PVM_EncMatrix[2]:1;
  L[5] = PVM_DummyScans;

  ParxRelsParRelations("L",Yes);
  
  
  DB_MSG(("<--SetPpgParameters\n"));
}

void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters\n"));

  
  int dim = PTB_GetSpatDim();
  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices ); 

  /* ------------------------------------------------ */
  /* -------               Job 0              ------- */
  /* ------------------------------------------------ */
  JOBPARAMETERS(jobParameters);
  JOBDESCRIPTION job0=jobParameters->getOrCreateJob("job0");

  job0->initPars( 1,PVM_EncMatrix[0], PVM_EffSWh * PVM_AntiAlias[0] );

  job0->appendLoop(PVM_NEchoImages);

  if(dim > 1)
    job0->appendLoop(PVM_EncMatrix[1]);   

  job0->appendLoop(nSlices, LOOP_SETUP);

  if(dim > 2)
    job0->appendLoop(PVM_EncMatrix[2]);    

  job0->appendLoop(NAE, LOOP_AVERAGE);
  job0->appendLoop(NR);

  DB_MSG(("<--SetAcquisitionParameters\n"));
}


#if DEBUG
#define d(n) (D[n]*1e3)
#define p(n) (P[n]*1e-3)


void printTiming(void)
{
  double te1,te2,aqq=PVM_AcquisitionTime,tr1,tr2;
 
 
  te1 = p(0)/2+d(3)+d(10)+d(2)+d(3)+aqq/2;
  te2 = p(0)/2+aqq/2+d(3)+d(15)+d(3)+d(14)+d(0)+0.03+d(3);

  DB_MSG(("te1: %f should be %f diff %f\n",
	  te1,PVM_EchoTime,te1-PVM_EchoTime));
  DB_MSG(("te2: %f should be %f diff %f\n",
	  te2,DESS_TE2,te2-DESS_TE2));
  
  tr1 = aqq/2+te1+d(3)+d(13)+d(3)+aqq/2+te2;
  tr2 = 0.03+7*d(3)+p(0)+d(10)+d(2)+2*aqq+d(13)+d(15)+d(14)+d(0);
  
  DB_MSG(("TR1: %f, should be %f, diff %f\n", tr1, PVM_RepetitionTime, tr1-PVM_RepetitionTime));
  DB_MSG(("TR2: %f, should be %f, diff %f\n", tr2, PVM_RepetitionTime, tr2-PVM_RepetitionTime));
  return;
}

#undef d
#undef p
#endif
