/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2008-2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/
static const char resid[] = "$Id$ (C) 2009 Bruker BioSpin MRI GmbH";


#define DEBUG	             0
#define DB_MODULE	         0
#define DB_LINE_NR	       0

#include "method.h"

static void SetFrequencyOffsetList(void);
void SetBaseLevelParameters( void )
{
  DB_MSG(("-->SetBaseLevelParameters"));

  SetBasicParameters();
    
  SetFrequencyParameters();

  SetPpgParameters();

  SetAcquisitionParameters();
    
  SetGradientParameters();

  SetInfoParameters();

  ATB_SetReceiverGains();
 
  ATB_SetSatTransBaseLevel();
  ATB_SetWsBaseLevel();
  ATB_SetOvsBaseLevel();
  ATB_SetDecBaseLevel();
  ATB_SetNoeBaseLevel();  

  printTiming();

  DB_MSG(("<--SetBaseLevelParameters"));
  return;
}

void SetBasicParameters( void )
{
  
  DB_MSG(("-->SetBasicParameters"));
    
  /* NSLICES */
  ATB_SetNSlices( 1 );

  /* NR */
  ATB_SetNR( PVM_SatTransRepetitions * PVM_NRepetitions );

  /* NI */
  ATB_SetNI( 1 ); 

  /* NA */
  ATB_SetNA(1);
  AverageList[0]=(IsisManAdjustment == NoManAdjustment)?IsisNAverages:PVM_NAverages;
 
  /* NAE */
  ATB_SetNAE( 1 );

  /* ACQ_ns */
  ACQ_ns_list_size = 1;
  ParxRelsParRelations("ACQ_ns_list_size",Yes);
  ACQ_ns_list[0] = 1;

  /* NECHOES */
  NECHOES = 1;

  /* ACQ_obj_order */
  PARX_change_dims("ACQ_obj_order", 1);
  ACQ_obj_order[0]=0;

  DB_MSG(("<--SetBasicParameters"));
  return;
}

void SetFrequencyParameters( void )
{
  DB_MSG(("-->SetFrequencyParameters"));

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

  ACQ_O2_mode = BF_plus_Offset_list;
  ParxRelsParRelations("ACQ_O2_mode",Yes);
  
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
  SF0ppm=PVM_FrqWork[0]+STB_GetFreqOffset(1,0.0)*1e-6; //reference freq for topspin
  /* call relations of BF1 (no need for other BF's) */
  ParxRelsParRelations("BF1", Yes);

  /* set voxel seletion and detection frequencies */
  SetFrequencyOffsetList();
  
  DB_MSG(("<--SetFrequencyParameters"));
  return;
}

void SetGradientParameters( void )
{

  DB_MSG(("-->SetGradientParameters()"));
  
  ACQ_phase_encoding_mode[0] = Read;
  ParxRelsParRelations("ACQ_phase_encoding_mode",Yes);
  PARX_change_dims("ACQ_phase_enc_start", 1 );
  ACQ_phase_enc_start[0] = 0.0;
  
  ATB_SetAcqGradMatrix("PVM_VoxelGeoObj");

  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 1.0;
  ACQ_scaling_slice = 1.0;
  
  DB_MSG(("<--SetGradientParameters"));
  return;
}

void SetInfoParameters( void )
{
  DB_MSG(("-->SetInfoParameters"));

  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();


  /* ACQ_dim */
  int spatDim=0, specDim=0;
  if(IsisManAdjustment  != NoManAdjustment )
  {
    spatDim = 1;
    specDim = 0;
  }
  else
  {
    spatDim = 0;
    specDim = MRT_GetSpecDim();
  }

  ACQ_dim = spatDim + specDim;
  ParxRelsParRelations("ACQ_dim",Yes);

  /* ACQ_dim_desc */
  ATB_SetAcqDimDesc( specDim, spatDim, NULL );

  ACQ_flip_angle = ExcPul1.Flipangle;

  PARX_change_dims("ACQ_echo_time",1);
  ACQ_echo_time[0] = 0.0;

  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = 0.0;

  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = 0.0;

  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] =  0.0;

  PARX_change_dims("ACQ_inversion_time",1);
  ACQ_inversion_time[0] = 0.0;


  ACQ_fov[0] = IsisFov/10;


  ATB_SetAcqSliceAngle( PtrType3x3 PVM_VoxArrGradOrient[0],
                        PVM_NVoxels );

  ACQ_slice_orient = Arbitrary_Oblique;

  ACQ_slice_thick = PVM_VoxArrSize[0][0];

  ACQ_read_ext = 1;

  ACQ_slice_sepn[0] = 0.0;

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

  ACQ_position_X = 0.1 * PVM_VoxArrPosition[0][0];
  ACQ_position_Y = 0.1 * PVM_VoxArrPosition[0][1];
  ACQ_position_Z = 0.1 * PVM_VoxArrPosition[0][2];

  PARX_change_dims("ACQ_temporal_delay",1);
  ACQ_temporal_delay[0] = 0.0;

  ACQ_RF_power = 0;

  DB_MSG(("<--SetInfoParameters"));
  return;
}


void SetPpgParameters( void )
{
  DB_MSG(("-->SetPpgParameters()"));
    
  switch(IsisManAdjustment)
  {
    case NoManAdjustment:
    default:
	    ATB_SetPulprog("ISIS.ppg");
      break;
    case OvsPulse1:
    case OvsPulse2:
    case OvsPulse3:
    case InvPulse1:
    case InvPulse2:
    case InvPulse3:
	    ATB_SetPulprog("ISISADJ.ppg");
      break; 
  }
    
  /* Delays */

  D[0] = Isis_D0*1e-3;
  D[4] = Isis_D4*1e-3;
  D[6] = Isis_D6*1e-3;
  D[10] = Isis_D10*1e-3;
  D[11] = Isis_D11*1e-3;
  D[13] = Isis_D13*1e-3;
  D[14] = Isis_D14*1e-3;
  
  ParxRelsParRelations("D",Yes);
    
  /* Voxel pulses + pulse durations set by local or module parameters */
  ATB_SetRFPulse("ExcPul1","ACQ_RfShapes[0]","P[0]");
  ATB_SetRFPulse("InvPul1","ACQ_RfShapes[1]","P[1]");

  L[3] = (IsisManAdjustment==NoManAdjustment)? PVM_DummyScans*8:PVM_DummyScans;
  ParxRelsParRelations("L",Yes);


  DB_MSG(("<--SetPpgParameters"));
}

static void SetFrequencyOffsetList(void)
{
  int i,j,k;
  DB_MSG(("-->SetFrequencyOffsetLists"));

  /*excitation frequencies */
  switch(IsisManAdjustment)
  {
    case NoManAdjustment:
    default:
      ACQ_O1_list_size=PVM_NVoxels*3;
      ParxRelsParRelations("ACQ_O1_list_size",Yes);
      for(i=0,j=0;i<PVM_NVoxels;i++)
      {
        for(k=0;k<3;j++,k++)
        {
          ACQ_O1_list[j] = PVM_VoxArrFrequency[i][k];
        }
      }
      break;
    case OvsPulse1:
    case OvsPulse2:
    case OvsPulse3:
      ACQ_O1_list_size = 1;
      ParxRelsParRelations("ACQ_O1_list_size",Yes);
      ACQ_O1_list[0] = 0.0;
      break;
    case InvPulse1:
      ACQ_O1_list_size = 1;
      ParxRelsParRelations("ACQ_O1_list_size",Yes);
      ACQ_O1_list[0] = PVM_VoxArrFrequency[0][0];
      break; 
    case InvPulse2:
      ACQ_O1_list_size = 1;
      ParxRelsParRelations("ACQ_O1_list_size",Yes);
      ACQ_O1_list[0] = PVM_VoxArrFrequency[0][1];
      break; 
    case InvPulse3:
      ACQ_O1_list_size = 1;
      ParxRelsParRelations("ACQ_O1_list_size",Yes);
      ACQ_O1_list[0] = PVM_VoxArrFrequency[0][2];
      break; 
  }

  ACQ_O2_list_size=1;
  ParxRelsParRelations("ACQ_O2_list_size",Yes);
  ACQ_O2_list[0] = 0.0;       

  /*detection frequency */
  ACQ_O1B_list_size = 1;
  ParxRelsParRelations("ACQ_O1B_list_size",Yes);
  ACQ_O1B_list[0] = 0.0;

  DB_MSG(("<--SetFrequencyOffsetLists"));
}

void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters\n"));

  /* ------------------------------------------------------ */
  /* ----------               Job 0              ---------- */
  /* ------------------------------------------------------ */

  JOBPARAMETERS(jobParameters);
  JOBDESCRIPTION job0 = jobParameters->getOrCreateJob("job0");

  if (IsisManAdjustment==NoManAdjustment)
    job0->initPars(1,PVM_SpecMatrix[0], PVM_SpecSWH[0]);
  else
    job0->initPars(1,PVM_Matrix[0], PVM_EffSWh);

  job0->appendLoop(L[3], LOOP_DUMMIES);

  //setup loop before averaging simplifies ArrayPhaseAdj
  job0->appendLoop(1, LOOP_SETUP);

  job0->appendLoopList(AverageList, 1, LOOP_AVERAGE);

  job0->appendLoop(NR);

  //compensate group delay for spatial acquisition during pulse adj
  job0->setCompensateGrpDelay((IsisManAdjustment == NoManAdjustment) ? 0 : 1);
  //store and process non-averaged data
  job0->setStoreDataMode(STORE_raw);
  job0->setPipelineMode(PIPELINE_raw);


  DB_MSG(("<--SetAcquisitionParameters\n"));
}





void printTiming(void)
{

#if DEBUG
#define d(n) (D[n]*1e3)
#define p(n) (P[n]*1e-3)


  double	tr=0.0, modulesDuration=0.0;

  DB_MSG(("-->printTiming()"));
  printf( "\n" );
  printf( "*** Durations ***\n" );
  printf( "Isis_D0 = %.6f\n", Isis_D0 );
  printf( "Isis_D4 = %.6f\n", Isis_D4 );
  printf( "Isis_D6 = %.6f\n", Isis_D6 );
  printf( "Isis_D10 = %.6f\n", Isis_D10 );
  printf( "Isis_D11 = %.6f\n", Isis_D11 );
  printf( "Isis_D13 = %.6f\n", Isis_D13 );
  printf( "Isis_D14 = %.6f\n", Isis_D14 );
  printf( "Isis_P0 = %.6f\n", Isis_P0 );
  printf( "Isis_P1 = %.6f\n", Isis_P1 );
  printf( "\n" );


  modulesDuration = PVM_TriggerModuleTime +
                    PVM_SatTransModuleTime + 
                    IsisNumAddOvsCycles * PVM_OvsModuleDuration + 
                    PVM_WsModuleDuration +
                    PVM_NoeModuleTime;

  printf( "modulesDuration = %.6f\n", modulesDuration );
  printf( "PVM_SpecAcquisitionTime = %.6f\n", PVM_SpecAcquisitionTime );
  printf( "PVM_AcquisitionTime = %.6f\n", PVM_AcquisitionTime );

  switch(IsisManAdjustment)
  {
    case NoManAdjustment:
    default:
      tr =
        0.03 + /* UpdateDynPars */
        1.0 +
        modulesDuration +
        3.0 * (d(4) + 0.01 + p(1) + d(11)) +
        d(4) + d(13) + p(0) + d(14) +
        PVM_SpecAcquisitionTime + 
        1.0 + d(0);
      break;
    case InvPulse1:
    case InvPulse2:
    case InvPulse3:
    case OvsPulse1:
    case OvsPulse2:
    case OvsPulse3:
      tr =
        0.03 + /* UpdateDynPars */
        1.0 +
        modulesDuration + 
        3.0 * (d(4) + 0.01 + p(1) + d(11)) +
        d(4) + d(13) + p(0) + d(6) + 2*d(4) +
        PVM_AcquisitionTime + 
        d(4) + 1.0 + d(0);
      break;
  }
  DB_MSG(("TR: %f, should be %f, diff %f", tr, PVM_RepetitionTime, tr-PVM_RepetitionTime));
  printf( "\n" );
  printf( "*** Gradient Strengths  ***\n" );
  printf( "Isis_Gr1 = %.6f\n", Isis_Gr1 );
  printf( "Isis_Gr2 = %.6f\n", Isis_Gr2 );
  printf( "Isis_Gr3 = %.6f\n", Isis_Gr3 );
  printf( "Isis_Gr4 = %.6f\n", Isis_Gr4 );
  printf( "Isis_Gp1 = %.6f\n", Isis_Gp1 );
  printf( "Isis_Gp2 = %.6f\n", Isis_Gp2 );
  printf( "Isis_Gp3 = %.6f\n", Isis_Gp3 );
  printf( "Isis_Gp4 = %.6f\n", Isis_Gp4 );
  printf( "Isis_Gs1 = %.6f\n", Isis_Gs1 );
  printf( "Isis_Gs2 = %.6f\n", Isis_Gs2 );
  printf( "Isis_Gs3 = %.6f\n", Isis_Gs3 );
  printf( "Isis_Gs4 = %.6f\n", Isis_Gs4 );
  printf( "\n" );

  DB_MSG(("<--printTiming"));

#undef d
#undef p
#endif

}

