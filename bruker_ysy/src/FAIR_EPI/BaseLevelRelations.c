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


/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "method.h"



void SetBaseLevelParam( void )
{

  DB_MSG(("Entering SetBaseLevelParam()"));

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
  ATB_EpiSetBaseLevel();
  ATB_SetFairBaseLevel();

  DB_MSG(("Exiting SetBaseLevelParam"));

}



void SetBasicParameters( void )
{
  DB_MSG(("Entering SetBasicParameters()"));

  /* NSLICES */

  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  ATB_SetNSlices( nSlices );

  /* NR */

  ATB_SetNR( PVM_SatTransRepetitions * PVM_NRepetitions * PVM_FairTIR_NExp *
             (PVM_FairMode==INTERLEAVED || PVM_FairMode==INTERLEAVED2 ? 2:1));

  /* NI */

  ATB_SetNI( nSlices * PVM_NEchoImages );

  /* NA */

  ATB_SetNA( PVM_NAverages );

  /* NAE */

  ATB_SetNAE( 1 );

  /* ACQ_ns */

  ACQ_ns_list_size = PVM_NEchoImages;
  PARX_change_dims( "ACQ_ns_list",1 );
  ACQ_ns_list[0] = 1;

  /* NECHOES */

   NECHOES = PVM_NEchoImages;

  /* ACQ_obj_order */

  PARX_change_dims("ACQ_obj_order",NI);
  ATB_SetAcqObjOrder( nSlices, PVM_ObjOrderList, PVM_NEchoImages, 1 );

  DB_MSG(("Exiting SetBasicParameters()"));
}



void SetFrequencyParameters( void )
{
  int nslices;

  DB_MSG(("Entering SetFrequencyParameters()"));

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

  nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  ATB_SetAcqO1List( nslices,
                    PVM_ObjOrderList,
                    PVM_SliceOffsetHz );

  ATB_SetAcqO1BList( nslices,
                     PVM_ObjOrderList,
                     PVM_ReadOffsetHz);

  DB_MSG(("Exiting SetFrequencyParameters()"));
}



void SetGradientParameters( void )
{
  DB_MSG(("Entering SetGradientParameters()"));

  int spatDim = PTB_GetSpatDim();

  PARX_change_dims("ACQ_phase_encoding_mode", spatDim );
  PARX_change_dims("ACQ_phase_enc_start", spatDim );

  ACQ_phase_encoding_mode[0] = Read;
  ACQ_phase_encoding_mode[1] = Linear;
  if(spatDim==3)
  {
    ACQ_phase_encoding_mode[2] = User_Defined_Encoding;
    ACQ_spatial_size_2 = PVM_EncMatrix[2];
    ParxRelsCopyPar("ACQ_spatial_phase_2","PVM_EncValues2");
  }

  ParxRelsParRelations("ACQ_phase_encoding_mode",Yes);

  for(int i=0; i<spatDim; i++ )
  {
    ACQ_phase_enc_start[i] = -1;
  }

  ATB_SetAcqGradMatrix("PVM_SliceGeoObj");

  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 1.0;
  ACQ_scaling_slice = 1.0;

  /* gradient amplitudes */
  ACQ_gradient_amplitude[0] = ExSliceGradient;         /* g0 */
  ACQ_gradient_amplitude[1] = -ExSliceRephaseGradient; /* g1 */
  ACQ_gradient_amplitude[2] = SliceSpoiler.ampl;       /* g2 */

  DB_MSG(("Exiting SetGradientParameters()"));
}



void SetInfoParameters( void )
{
  int slices, i,j;
  char buffer[128],mode[16];

  DB_MSG(("Entering SetInfoParameters()"));

  int spatDim = PTB_GetSpatDim();

  ACQ_dim = spatDim;
  ParxRelsParRelations("ACQ_dim",Yes);

  ATB_SetAcqDimDesc( 0, spatDim, NULL );

  ATB_SetAcqFov( Spatial, spatDim, PVM_Fov, PVM_AntiAlias );

  ACQ_flip_angle = ExcPulse1.Flipangle;;

  ACQ_n_echo_images = 1;

  PARX_change_dims("ACQ_echo_time",ACQ_n_echo_images);
  for(i=0;i<ACQ_n_echo_images;i++)
  ACQ_echo_time[i] = EchoTime;

  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = PVM_FairRepSpacing == CONST_REP ? PVM_RepetitionTime:RecoveryTime;

  PARX_change_dims("ACQ_inversion_time",PVM_FairTIR_NExp);
  for(i=0;i<PVM_FairTIR_NExp;i++)
    ACQ_inversion_time[i] = PVM_FairTIR_Arr[i];

  PARX_change_dims("ACQ_recov_time",1);
  if(PVM_FairRepSpacing == CONST_REP)
    ACQ_recov_time[0] =  ACQ_repetition_time[0]-ACQ_inversion_time[PVM_FairTIR_NExp-1];
  else
    ACQ_recov_time[0] =RecoveryTime;

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

  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();

  PARX_change_dims("ACQ_echo_descr",ACQ_n_echo_images,20);
  ACQ_n_movie_frames = PVM_FairTIR_NExp*
    (PVM_FairMode==INTERLEAVED || PVM_FairMode==INTERLEAVED2? 2:1);

 /* calculation of ACQ_repetition_tim */
 if( PVM_FairRepSpacing == CONST_REP)
 {
   PARX_change_dims("ACQ_repetition_time",1);
   ACQ_repetition_time[0] = PVM_RepetitionTime;
 }
 else
 {
   PARX_change_dims("ACQ_repetition_time",ACQ_n_movie_frames);

   switch(PVM_FairMode)
   {
   case NONSELECTIVE:
   case SELECTIVE:
     for(i=0;i<ACQ_n_movie_frames;i++)
     {
       ACQ_repetition_time[i]=MultiRepTime[i];
      }
     break;
   case INTERLEAVED:
     for(i=0;i<PVM_FairTIR_NExp;i++)
     {
       ACQ_repetition_time[i]=MultiRepTime[i];
       ACQ_repetition_time[i+PVM_FairTIR_NExp]=MultiRepTime[i];
     }
     break;
   case INTERLEAVED2:
     for(i=0;i<PVM_FairTIR_NExp;i++)
     {
       ACQ_repetition_time[2*i]=MultiRepTime[i];
       ACQ_repetition_time[2*i+1]=MultiRepTime[i];
     }
     break;
   }
 }

 PARX_change_dims("ACQ_movie_descr",ACQ_n_movie_frames,20);

  switch(PVM_FairMode)
  {

    case NONSELECTIVE:
      for(i=0;i<ACQ_n_movie_frames;i++)
      {
        sprintf(buffer," NS TI: %.1f ms ",ACQ_inversion_time[i]);
        strncpy(ACQ_movie_descr[i],buffer,19*sizeof(char));
        ACQ_movie_descr[i][19] = '\0';
      }
      break;

    case SELECTIVE:
      for(i=0;i<ACQ_n_movie_frames;i++)
      {
        sprintf(buffer," S TI: %.1f ms ",ACQ_inversion_time[i]);
        strncpy(ACQ_movie_descr[i],buffer,19*sizeof(char));
        ACQ_movie_descr[i][19]='\0';
      }
      break;

    case INTERLEAVED:
      for(j=0;j<2;j++)
      {
        if(j==0) sprintf(mode," S ");
        if(j==1) sprintf(mode," NS ");
        for(i=0;i<PVM_FairTIR_NExp;i++)
        {
          sprintf(buffer,"%s TI: %.1f ms",mode,ACQ_inversion_time[i]);
          strncpy(ACQ_movie_descr[i+j*PVM_FairTIR_NExp],buffer,19*sizeof(char));
          ACQ_movie_descr[i+j*PVM_FairTIR_NExp][19]='\0';
        }
      }
      break;

    case INTERLEAVED2:

      for(i=0;i<PVM_FairTIR_NExp;i++)
      {
        sprintf(buffer,"S TI: %.1f ms",ACQ_inversion_time[i]);
        strncpy(ACQ_movie_descr[2*i],buffer,19*sizeof(char));
        ACQ_movie_descr[2*i][19]='\0';

        sprintf(buffer,"NS TI: %.1f ms",ACQ_inversion_time[i]);
        strncpy(ACQ_movie_descr[2*i+1],buffer,19*sizeof(char));
        ACQ_movie_descr[2*i+1][19]='\0';
      }

      break;

  }


  DB_MSG(("Exiting SetInfoParameters()"));

}



void SetPpgParameters( void )
{
  DB_MSG(("Entering SetPpgParameters()"));

  ATB_SetPulprog("FAIR_EPI.ppg");

  double rampT = CFG_GradientRampTime();
  double riseT = CFG_GradientRiseTime();


  /* delays */
  if(PVM_FairRepSpacing==CONST_REP)
  {
    D[0]  = (PVM_RepetitionTime - PVM_MinRepetitionTime) / 1000.0;
  }
  else
  {
    D[0] = (RecoveryTime - MinRecoveryTime) / 1000.0;
  }

  D[1] = (ExSliceRephaseTime - 2.0 * riseT) / 1000.0;

  if(PVM_SignalType == SignalType_Fid)
    D[2]  = (EchoTime - PVM_MinEchoTime) / 1000.0;
  else
    D[2]  = (EchoTime/2 - MinTE_right) / 1000.0;

  D[4]  = rampT / 1000.0;
  D[3]  = riseT / 1000.0;
  D[5] =  SliceSpoiler.dur / 1000.0;
  D[6]  = (EchoTime/2 - MinTE_left) / 1000.0;


  /* loop counters */
  L[1] = (PTB_GetSpatDim() > 2) ? PVM_EncMatrix[2] : 1;


  /* set shapes and durations of RF pulses     */
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");
  ATB_SetRFPulse("RefPulse1","ACQ_RfShapes[1]","P[1]");

  DB_MSG(("Exiting SetPpgParameters"));
}



void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters"));

  JOBPARAMETERS(jobParameters);

  /* ------------------------------------------------ */
  /* -------               Job 0              ------- */
  /* ------------------------------------------------ */

  JOBDESCRIPTION job0 = jobParameters->getOrCreateJob("job0");

  job0->initPars(1, PVM_EpiNSamplesPerScan, PVM_EpiBandwidth);

  job0->appendLoop(NSLICES);

  job0->appendLoop(PVM_DummyScans, LOOP_DUMMIES);

  job0->appendLoop(NA, LOOP_AVERAGE);

  job0->appendLoop(PVM_EpiNShots, LOOP_SETUP);

  job0->appendLoop(L[1]);

  job0->appendLoop(NR);

  DB_MSG(("<--SetAcquisitionParameters"));
}
