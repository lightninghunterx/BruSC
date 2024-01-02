/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001-2014
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

const char * MDEFT_BaseLevelRelations_id = "$Id$ (C) 2001-2014 Bruker BioSpin MRI GmbH";

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
  ATB_SetFovSatBaseLevel();


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
  ATB_SetNR( PVM_NRepetitions );
  
  /* NI */
  ATB_SetNI( nSlices * PVM_NEchoImages );

  /* AVERAGING */
  ATB_SetNA( 1 );
  ATB_SetNAE( PVM_NAverages );
  
  /* NECHOES */  
  NECHOES = PVM_NEchoImages;
  
  /* ACQ_obj_order */
  PARX_change_dims("ACQ_obj_order",NI);
  ATB_SetAcqObjOrder( nSlices, PVM_ObjOrderList, PVM_NEchoImages, 1 );
  
  DB_MSG(("<--SetBasicParameters\n"));
}

void SetFrequencyParameters( void )
{
  int nslices;

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
  
  nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  
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
      
  DB_MSG(("<--SetGradientParameters\n"));
}

void SetInfoParameters( void )
{
  int slices, i, spatDim, nrep; 

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
  
  nrep = PVM_NRepetitions;
  
  ATB_SetAcqFov( Spatial, spatDim, PVM_Fov, PVM_AntiAlias );
  
  ACQ_flip_angle = ExcPulse1.Flipangle;
  
  PARX_change_dims("ACQ_echo_time",1);
  ACQ_echo_time[0] = EchoTime; /* PVM_EchoTime; */
  
  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = EchoRepTime; /* PVM_EchoTime; */
  
  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = SegmRepTime; /* PVM_RepetitionTime; */
  
  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] =  PVM_RepetitionTime - ExcPulse1.Length;

  /* calculation of ACQ_time_points */
  PARX_change_dims("ACQ_time_points",nrep);
  ACQ_time_points[0] = 0;
  for(i=1; i<nrep; i++)
    ACQ_time_points[i] = OneRepTime * i; 
  
  PARX_change_dims("ACQ_inversion_time",1);
  ACQ_inversion_time[0] = PVM_InversionTime;
  
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
  
  ATB_SetPulprog("MDEFT.ppg");

  double igwt = CFG_InterGradientWaitTime();
  double riseTime = CFG_GradientRiseTime();

  D[1]  = ((EchoRepTime - MinEchoRepTime)/NSLICES + igwt) / 1000.0 +0.01 / 1000.0;
  D[2]  = (TeFillDelay + riseTime) / 1000.0;
  D[3]  = riseTime / 1000.0;
  D[6]  = SliceSpoiler.dur/1000.0;
  D[10] = EncGradDur / 1000.0;
  D[11] = D[10];
  D[13] = (ReadSpoiler.dur - EncGradDur)/1000.0;
  D[22] = (Mdeft_SliceSpoiler.dur - riseTime)/1000.0; 
  if(PreparationMode == Inversion_prep)
  {
    D[0] = SegmPad/1000.0;
    D[12] = (PVM_InversionTime - MinInversionTime + riseTime)/1000.0;
  }
  else
  {
    D[0]  = (PVM_InversionTime - Mintir1)/1000;
    D[12] = (PVM_InversionTime - Mintir2 + riseTime)/1000.0;
  }
  
  L[4] =  SegmNumber; 
  L[5] =  SegmentSize; 

  L[12] = NDummyEchoes;
  L[13] = PVM_DummyScans; 
  L[14] = L[5] * PVM_DummyScans;

  ParxRelsParRelations("L",Yes);

  /* set shaped pulses, in this method ACQ_RfShapes[0] is used           
     the pulse duration is stored in baselevel parameter P[0]
  */
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");  
  ATB_SetRFPulse("Mdeft_ExcPulse1","ACQ_RfShapes[1]","P[1]");  
  ATB_SetRFPulse("Mdeft_InvPulse1","ACQ_RfShapes[2]","P[2]");  
 
  DB_MSG(("<--SetPpgParameters\n"));
}

void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters"));

  /* ------------------------------------------------ */
  /* -------               Job 0              ------- */
  /* ------------------------------------------------ */
  JOBPARAMETERS(jobParameters);
  JOBDESCRIPTION job0=jobParameters->getOrCreateJob("job0");

  job0->initPars( 1,PVM_EncMatrix[0], PVM_EffSWh * PVM_AntiAlias[0] );

  job0->appendLoop(SegmentSize, LOOP_SETUP);
  job0->appendLoop(PVM_DummyScans, LOOP_DUMMIES);
  job0->appendLoop(SegmNumber);
  job0->appendLoop(NAE, LOOP_AVERAGE);
  job0->appendLoop(NR);

  DB_MSG(("<--SetAcquisitionParameters"));
}



#if DEBUG
#define d(n) (D[n]*1e3)
#define p(n) (P[n]*1e-3)

void printTiming(void)
{
  double te,aqq=PVM_AcquisitionTime,tr;

  /* TE */
  te = p(0)/2+d(3)+d(10)+d(2)+d(3)+aqq/2;

  DB_MSG(("te: %f should be %f diff %f\n",
	  te,EchoTime,te-EchoTime));

  /* TR */
  tr = 0.01+d(6)+d(3)+p(0)/2+aqq/2+d(11)+d(13)+d(3)+d(1)+te;
  tr *= NSLICES;

  DB_MSG(("TR: %f, should be %f, diff %f", tr, EchoRepTime, tr-EchoRepTime));

  return;
}

#undef d
#undef p
#endif
