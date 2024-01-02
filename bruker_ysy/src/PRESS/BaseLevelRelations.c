/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002 - 2014
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

const char * PRESS_BaseLevelRelations_id = "$Id$ (C) 2002 - 2014 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"


void SetBaseLevelParameter( void )
{
  DB_MSG(("-->SetBaseLevelParameter\n"));
    
    SetBasicParameters();
    
    SetFrequencyParameters();

    SetPpgParameters();

    SetAcquisitionParameters();
    
    SetGradientParameters();

    SetInfoParameters();

    SetOptimiseParameter();

    ATB_SetReceiverGains();

    ATB_SetWsBaseLevel();

    ATB_SetOvsBaseLevel();

    ATB_SetDecBaseLevel();

    ATB_SetNoeBaseLevel();

#if DEBUG
  printTiming();
#endif

  DB_MSG(("<--SetBaseLevelParameter\n"));
}



void SetBasicParameters( void )
{
  DB_MSG(("-->SetBasicParameters()"));
  
  /* NSLICES */
  ATB_SetNSlices( 1 );

  /* NR */
  ATB_SetNR( PVM_NRepetitions );
  

  /* NI */
  ATB_SetNI( 1 );
 
  /*Averaging via jobs and reco -> NA=1*/
  ATB_SetNA(1);
  PARX_change_dims("AverageList",1);
  AverageList[0]= PVM_NAverages;

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

  DB_MSG(("<--SetBasicParameters()"));
}

void SetFrequencyParameters( void )
{
    DB_MSG(("-->SetFrequencyParameters()"));

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
    SF0ppm=PVM_FrqWork[0]+STB_GetFreqOffset(1,0.0)*1e-6; //reference freq for topspin
    /* call relations of BF1 (no need for other BF's) */
    ParxRelsParRelations("BF1", Yes);
 
    /*detection frequency */
    ACQ_O1B_list_size = 1;
    ParxRelsParRelations("ACQ_O1B_list_size",Yes);
    ACQ_O1B_list[0] = 0.0;
    
    /* voxel frequencies */
    ACQ_O1_list_size = 3*PVM_NVoxels;
    ParxRelsParRelations("ACQ_O1_list_size",Yes);
    {
      int nv, l, i;
      l=0;
      for ( nv = 0; nv < PVM_NVoxels; nv++ )
        for ( i = 0; i < 3; i++ )
        {
          ACQ_O1_list[l] = PVM_VoxArrFrequency[nv][i];
          l += 1;
        }
    }
    ParxRelsParRelations("ACQ_O1_list",Yes);

    DB_MSG(("<--SetFrequencyParameters()"));
}


void SetGradientParameters( void )
{
    DB_MSG(("-->SetGradientParameters()"));
  
    PARX_change_dims("ACQ_phase_encoding_mode", 1);
    
    ACQ_phase_encoding_mode[0] = Read;
    
    ParxRelsParRelations("ACQ_phase_encoding_mode",Yes);
    
    PARX_change_dims("ACQ_phase_enc_start", 1 );
    
    ACQ_phase_enc_start[0] = -1;
   
    ATB_SetAcqGradMatrix("PVM_VoxelGeoObj");

    ACQ_scaling_read  = 1.0;
    ACQ_scaling_phase = 1.0;
    ACQ_scaling_slice = 1.0;

    /* ACQ_trims */
    {
      int i;

      /* set gradient values for Voxels */

      ACQ_gradient_amplitude[1] = PVM_VoxArrGradient[0][0];
      ACQ_gradient_amplitude[2] = PVM_VoxArrGradient[0][1];
      ACQ_gradient_amplitude[3] = PVM_VoxArrGradient[0][2];

      ACQ_gradient_amplitude[4] = SpoilerStrengthArr[0][0]+SliceRephase;
      ACQ_gradient_amplitude[5] = SpoilerStrengthArr[0][0];
      ACQ_gradient_amplitude[6] = SpoilerStrengthArr[0][1];
      ACQ_gradient_amplitude[7] = SpoilerStrengthArr[0][2];
      ACQ_gradient_amplitude[8] = SpoilerStrengthArr[1][0];	
      ACQ_gradient_amplitude[9] = SpoilerStrengthArr[1][1];
      ACQ_gradient_amplitude[10]= SpoilerStrengthArr[1][2];

      /* imaging gradients */
      for ( i = 11; i <= 16; i++ )
        ACQ_gradient_amplitude[i] = 0.0;
      switch (OPT_ManAdjustment)
      {
        case OVSPulse1:
          ACQ_gradient_amplitude[1] = 0.0;
          ACQ_gradient_amplitude[4] = ACQ_gradient_amplitude[5];
        case Pulse1:
          ACQ_gradient_amplitude[14] = OPT_GradStrengthRead ;
          ACQ_gradient_amplitude[11] = OPT_GradStrengthReadDephase;
          break;
        case OVSPulse2:
          ACQ_gradient_amplitude[2] = 0.0;
        case Pulse2:
          ACQ_gradient_amplitude[15] = OPT_GradStrengthRead ;
          ACQ_gradient_amplitude[12] = OPT_GradStrengthReadDephase;
          break;
        case OVSPulse3:
          ACQ_gradient_amplitude[3] = 0.0;
        case Pulse3:
          ACQ_gradient_amplitude[16] = OPT_GradStrengthRead ;
          ACQ_gradient_amplitude[13] = OPT_GradStrengthReadDephase;
          break; 
        default:
        case NoManAdjustment:
          break;
	    
      }
    }

    DB_MSG((">--SetGradientParameters()"));    
}

void SetInfoParameters( void )
{
  DB_MSG(("-->SetInfoParameters()"));

  int spatDim=0, specDim=0;

  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();

 /* ACQ_dim */
  if(OPT_ManAdjustment  != NoManAdjustment )
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

  
  ACQ_fov[0] = OPT_FOV/10;

  ACQ_flip_angle = VoxPul1.Flipangle;

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

  ACQ_position_X = 0.0;
  ACQ_position_Y = 0.0;
  ACQ_position_Z = 0.0;

  PARX_change_dims("ACQ_temporal_delay",1);
  ACQ_temporal_delay[0] = 0.0;

  ACQ_RF_power = 0;

  DB_MSG(("<--SetInfoParameters()"));
}


void SetPpgParameters( void )
{
  int i;
    
  DB_MSG(("-->SetPpgParameters()"));

  for ( i = 0; i < 32; i++ ) L[i] = 0;
  ParxRelsParRelations("L",Yes);
    
  ATB_SetPulprog("PRESS.ppg");
 
  /* set timing parameter */
  double riseT = CFG_GradientRiseTime();  

  for ( i = 0; i < 32; i++ )
    D[i] = 0.0;
  D[1] = TrDel;
  D[4] = riseT;
  D[6] = GradStabDelay;
  D[7] = AddDelay[0] + riseT;
  D[8] = SpoilerDuration[0][0] - riseT;
  D[9] = AddDelay[1] + riseT + AddDelay[2];
  D[10] = SpoilerDuration[1][0] - riseT;
  D[11] = AddDelay[3] + riseT;
    
  /* imaging delays */
  D[12] = OPT_GradReadDephaseDelay;

    
  /* set delays in units of seconds */
  for ( i = 0; i < 32; i++ )
    D[i] /= 1000.0;
    
  ParxRelsParRelations("D",Yes);	  
    

  /* set shaped pulses (Voxel pulses + Navigator)
     the pulse duration is stored in baselevel parameter P[0]
  */
  ATB_SetRFPulse("VoxPul1","ACQ_RfShapes[1]","P[1]");
  ATB_SetRFPulse("VoxPul2","ACQ_RfShapes[2]","P[2]");
  ATB_SetRFPulse("VoxPul3","ACQ_RfShapes[3]","P[3]");

  DB_MSG(("<--SetPpgParameters()"));
}

void SetOptimiseParameter( void )
{
  DB_MSG(("-->SetOptimiseParameter"));

  if ( OPT_ManAdjustment != NoManAdjustment )
  {
    ACQ_O1B_list[0]=0.0;
	
    /* frequency for ovs adjustments */
    switch (OPT_ManAdjustment)
    {
      case OVSPulse1: ACQ_O1_list[0] = 0.0;
        break;
      case OVSPulse2: ACQ_O1_list[1] = 0.0;
        break;
      case OVSPulse3: ACQ_O1_list[2] = 0.0;
        break;
      default:
        break;
    }
	
    NA = 1;
    ParxRelsParRelations("NA",Yes);
  }
  DB_MSG(("<--SetOptimiseParameter"));
}



void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters\n"));

  /* ------------------------------------------------------ */
  /* ----------               Job 0              ---------- */
  /* ------------------------------------------------------ */

  JOBPARAMETERS(jobParameters);
  JOBDESCRIPTION job0 = jobParameters->getOrCreateJob("job0");

  if (OPT_ManAdjustment==NoManAdjustment)
    job0->initPars(1,PVM_SpecMatrix[0], PVM_SpecSWH[0]);
  else
    job0->initPars(1,PVM_Matrix[0], PVM_EffSWh);

  job0->appendLoop(PVM_DummyScans, LOOP_DUMMIES);

  job0->appendLoopList(AverageList, 1, LOOP_AVERAGE, LOOP_SETUP);
  
  job0->appendLoop(NR);

  //compensate group delay for spatial acquisition during pulse adj
  job0->setCompensateGrpDelay((OPT_ManAdjustment == NoManAdjustment) ? 0 : 1);
  //show averaged data in acq display but store and process non-averaged data
  job0->setStoreDataMode(STORE_raw);
  job0->setPipelineMode(PIPELINE_raw);


  /* ------------------------------------------------ */
  /* -------            Navigator             ------- */
  /* ------------------------------------------------ */

  int nTotalScansNav = NR * PVM_NAverages; //do not use NA (was set to 1)
  int transBlocks = PVM_NAverages;

  JOBDESCRIPTION navigator;
  navigator = ATB_SetNavBaseLevel(jobParameters, 0, transBlocks, nTotalScansNav, PVM_DummyScans);
  //pars, pulse shape, transBlocks, totalScans, dummyScans

  ATB_LinkJobToDriftCompModule(jobParameters, navigator);

  DB_MSG(("<--SetAcquisitionParameters\n"));
}


#if DEBUG
#define d(n) (D[n]*1e3)
#define p(n) (P[n]*1e-3)

void printTiming(void)
{
  double aqq=PVM_SpecAcquisitionTime,tr,te;

  te=p(1)/2.0+d(6)+d(7)+d(8)+d(4)+d(6)+p(2)+d(6)+d(8)+d(9)+0.02+d(10)+d(4)+d(6)+p(3)+d(6)+d(10)+d(11)+0.02;
 
   /* TR */
  tr =0.03+1.0+ PVM_TriggerModuleTime+ PVM_NoeModuleTime + PVM_WsModuleDuration+d(4)+d(6)+p(1)/2.0+te+aqq+d(4)+1.0+d(1);

  /*  if (PVM_NavOnOff == On)
    tr+=0.02+PVM_NavigatorModuleTime;
  */

  DB_MSG(("TE: %f, should be %f, diff %f", te, PVM_EchoTime, te-PVM_EchoTime));
  DB_MSG(("TR: %f, should be %f, diff %f", tr, PVM_RepetitionTime, tr-PVM_RepetitionTime));

  return;
}

#undef d
#undef p
#endif

