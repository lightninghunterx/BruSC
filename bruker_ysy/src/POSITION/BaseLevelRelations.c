/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2014
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

const char * POSITION_BaseLevelRelations_id = "$Id$ (C) 2001 - 2014 Bruker BioSpin MRI GmbH";

#define DEBUG		0


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


/* Toolboxes referenced in this file: ATB, GTB, PTB, STB, UT */


void SetBasicParameters( void )
{
  DB_MSG(("-->SetBasicParameters\n"));

  /* NSLICES */
  ATB_SetNSlices( 1 );

  /* NR */
  ATB_SetNR( 1 );

  /* NI */
  ATB_SetNI( 1 );

  /* AVERAGING */
  ATB_SetNA( PVM_NAverages );
  ATB_SetNAE( 1 );

  /* NECHOES */
  NECHOES = 1;

  /* ACQ_obj_order */
  PARX_change_dims("ACQ_obj_order",NI);
  ATB_SetAcqObjOrder( 1, PVM_ObjOrderList, 1, 1 );

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

  double offset[1];

  offset[0] = 0.0;

  ATB_SetAcqO1List( 1,
                    PVM_ObjOrderList,
                    offset );

  ATB_SetAcqO1BList( 1,
                     PVM_ObjOrderList,
                     PVM_ReadOffsetHz);


  DB_MSG(("<--SetFrequencyParameters\n"));
}

void SetGradientParameters( void )
{
  int spatDim, dim;

  DB_MSG(("-->SetGradientParameters\n"));

  spatDim = PTB_GetSpatDim();

  dim = PARX_get_dim("ACQ_phase_encoding_mode", 1 );

  if( dim != spatDim )
  {
    PARX_change_dims("ACQ_phase_encoding_mode", spatDim );
  }

  ACQ_phase_encoding_mode[0] = Read;

  ACQ_phase_enc_start[0] = -1;

  ATB_SetAcqGradMatrix(0); // unit matrix
  memset(ACQ_GradientMatrix[0][0],0,9*sizeof(double));

  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 0.0;
  ACQ_scaling_slice = 0.0;
  
  for(int i=0; i<3;i++)
    GradVec[i]=0.0;

  switch(GradientDirection)
  {
    default:
    case Z_dir:
      GradVec[2]=1.0;  
      break;
    case Y_dir:
      GradVec[1]=1.0;     
      break;
    case X_dir:
      GradVec[0]=1.0;
      break;
  } 

  /* gradient amplitudes */
  ACQ_gradient_amplitude[0] =  ReadGradient;
  ACQ_gradient_amplitude[1] =  ReadSpoilerStrength;

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

  PARX_change_dims("ACQ_echo_time",1);
  ACQ_echo_time[0] = 1.0;

  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;

  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] =  PVM_RepetitionTime - ExcPulse1.Length;

  PARX_change_dims("ACQ_inversion_time",1);
  ACQ_inversion_time[0] = PVM_InversionTime;

  ATB_SetAcqSliceAngle( PtrType3x3 PVM_SPackArrGradOrient[0],
			PVM_NSPacks );

  ACQ_slice_orient = Arbitrary_Oblique;

  ACQ_slice_thick = 0.0;

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

  ATB_SetPulprog("POSITION.ppg");

  double igwt = CFG_InterGradientWaitTime();
  double riseT = CFG_GradientRiseTime();

  D[0] = (PVM_RepetitionTime - PVM_MinRepetitionTime + igwt) / 1000.0;
  D[4] = riseT / 1000.0;
  D[5] = riseT / 2000.0;
  D[6] = PVM_AcquisitionTime / 2000.0;
  D[8] = ExcPulse1.Length / 2000.0;


  /* set shaped pulses, in this method ACQ_RfShapes[0] is used
     the pulse duration is stored in baselevel parameter P[0]
  */
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");
  P[1] = 2.0 * P[0];

  ParxRelsParRelations("P",Yes);

  ParxRelsParRelations("L",Yes);

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

  job0->initPars(1,PVM_EncMatrix[0], PVM_EffSWh * PVM_AntiAlias[0]);

  job0->appendLoop(NDummyScans, LOOP_DUMMIES);
  job0->appendLoop(NA, LOOP_AVERAGE, LOOP_SETUP);
  job0->appendLoop(NR);

  DB_MSG(("<--SetAcquisitionParameters"));
}


#if DEBUG
#define d(n) (D[n]*1e3)
#define p(n) (P[n]*1e-3)

void printTiming(void)
{
  double aqq = PVM_AcquisitionTime;

  /* TE */
  double TE1 = p(0)/2 + 8*d(4) + d(6) + p(1)/2;
  DB_MSG(("Excitation pulse center to refocusing pulse center: TE1 = %f ms", TE1));

  double TE2 = p(1)/2 + 7*d(4) + 2*d(5) + aqq/2 + d(8);
  DB_MSG(("Refocusing pulse center to ADC center: TE2 = %f ms", TE2));

  DB_MSG(("TE1 - TE2 = %f ms (should be 0)", TE1 - TE2));

  /* TR */
  double TR = 0.03 + d(0) + 0.020 + p(0)/2 + TE1 + TE2 + aqq/2 + d(4) + 0.010;
  DB_MSG(("TR: %f ms, should be %f ms, diff %f ms", TR, PVM_RepetitionTime, TR - PVM_RepetitionTime));

  return;
}

#undef d
#undef p
#endif

