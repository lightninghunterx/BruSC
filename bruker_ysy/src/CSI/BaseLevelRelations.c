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

  SetLocalizationParameters(); //after FrequencyParams

  SetGradientParameters();
  
  SetInfoParameters();

  ATB_SetReceiverGains();  
  /* setting baselevel parameters used by modules */ 
  ATB_SetFovSatBaseLevel();

  ATB_SetWsBaseLevel();

  ATB_SetOvsBaseLevel();

  ATB_SetDecBaseLevel();

  ATB_SetNoeBaseLevel();

  ATB_SetFatSupBaselevel();

  #if DEBUG
  printTiming();
  #endif

  DB_MSG(("<--SetBaseLevelParam\n"));
  
}


/* Toolboxes referenced in this file: ATB, GTB, PTB, STB, UT */


void SetBasicParameters( void )
{
  int nSlices;

  DB_MSG(("-->SetBasicParameters\n"));
    
  /* NSLICES */
  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

 
  ATB_SetNSlices( nSlices );
 
  /* NR */
  ATB_SetNR( PVM_NRepetitions );
 
  /* NI */
  ATB_SetNI(nSlices);

  /* ACQ_ns */
  ACQ_ns_list_size = 1;
  ParxRelsParRelations("ACQ_ns_list_size",Yes);
  ACQ_ns_list[0] = 1;
  
  
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

  nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
 
  ATB_SetAcqO1List( nslices,
                    PVM_ObjOrderList,
                    PVM_SliceOffsetHz );
  
  int OrderList[1] = {0}; 
  ATB_SetAcqO1BList( 1,
                     OrderList,
                     &PVM_SpecOffsetHz);
   
  
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
      ACQ_phase_encoding_mode[0] = User_Defined_Encoding;
      ACQ_spatial_size_0 = PVM_EncMatrix[0];
      ACQ_phase_enc_start[0] = -1;
      ParxRelsCopyPar("ACQ_spatial_phase_0","PVM_EncValues0");
  }

  DB_MSG(("mode set"));
  ParxRelsParRelations("ACQ_phase_encoding_mode",Yes);

  ATB_SetAcqGradMatrix("PVM_SliceGeoObj");
  
  
  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 1.0;
  ACQ_scaling_slice = 1.0;
  

  ACQ_gradient_amplitude[0]  =  ExcSliceGrad;
  ACQ_gradient_amplitude[1]  = (LocalizationMode==Slice_SE)? ExcSliceRephGrad:-ExcSliceRephGrad;
  ACQ_gradient_amplitude[2]  =  Phase1DGrad;
  ACQ_gradient_amplitude[3]  =  Phase2DGrad;
  ACQ_gradient_amplitude[4]  = -Phase3DGrad;
  ACQ_gradient_amplitude[5]  =  Spoiler.ampl;
                   
   
  DB_MSG(("<--SetGradientParameters\n"));
}

void SetInfoParameters( void )
{
  int slices, i, spatDim,specDim,dim;

  DB_MSG(("-->SetInfoParameters\n"));

  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();

  spatDim = PTB_GetSpatDim();
  specDim = MRT_GetSpecDim();

  dim = spatDim + specDim;

  ACQ_dim = dim;
  ParxRelsParRelations("ACQ_dim",Yes);
  
  ATB_SetAcqDimDesc( specDim, spatDim, NULL );

  ACQ_flip_angle = ExcPulse1.Flipangle;
  
  PARX_change_dims("ACQ_fov",dim);
  ACQ_fov[0] = PVM_FovCm[0];
  for(i=0;i<spatDim;i++)
  ACQ_fov[i+specDim] = PVM_FovCm[i]; 
  PARX_change_dims("ACQ_echo_time",1);
 
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
  
  ACQ_slice_thick = PVM_SliceThick;
  
  slices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

 
  PARX_change_dims("ACQ_slice_offset",slices);
  PARX_change_dims("ACQ_read_offset",slices);
  PARX_change_dims("ACQ_phase1_offset",slices);
  PARX_change_dims("ACQ_phase2_offset",slices);
  
  for(i=0;i<slices;i++)
  {
    ACQ_slice_offset[i]  = PVM_SliceOffset[i];
    ACQ_read_offset[i]   = PVM_Phase0Offset[i];
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
  ACQ_n_movie_frames = 1;

  DTYPA=Double; //tell topspin that fid file contains double data
  
  DB_MSG(("<--SetInfoParameters\n"));
  
}


void SetPpgParameters( void )
{
  DB_MSG(("-->SetPpgParameters\n"));

  ATB_SetPulprog("CSI.ppg");
  
  D[0] = ((PVM_RepetitionTime - PVM_MinRepetitionTime)/NSLICES) / 1000.0;
  D[1] = EncGradDur / 1000.0;
  D[4] = CFG_GradientRiseTime() / 1000.0;
  D[6] = Spoiler.dur/1000.0;
  
  /* set shaped pulses, in this method ACQ_RfShapes[0] is used           
     the pulse duration is stored in baselevel parameter P[0]
     RefPulse1 is set in LocalizationBaselevel
  */
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");
  
  /* loop counters */
  int spatDim=PTB_GetSpatDim();
  L[1] = spatDim>0 ? PVM_EncMatrix[0]:1;
  L[2] = spatDim>1 ? PVM_EncMatrix[1]:1;
  L[3] = spatDim>2 ? PVM_EncMatrix[2]:1;
 
  ParxRelsParRelations("P",Yes);
  
  
  DB_MSG(("<--SetPpgParameters\n"));
}


void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters\n"));
  //  int spatDim = PTB_GetSpatDim();

  /* ------------------------------------------------------ */
  /* ----------               Job 0              ---------- */
  /* ------------------------------------------------------ */

  JOBPARAMETERS(jobParameters);
  JOBDESCRIPTION job0 = jobParameters->getOrCreateJob("job0");
  job0->initPars(1,PVM_SpecMatrix[0], PVM_SpecSWH[0]);

  job0->appendLoop(NSLICES, LOOP_SETUP);
  job0->appendLoop(PVM_DummyScans, LOOP_DUMMIES);
  job0->appendLoopList(AverageList, PARX_get_dim("AverageList",1), LOOP_AVERAGE); //no setup loop here! does not support list averaging
  //  for (int i=0; i < spatDim; i++)
  //    job0->appendLoop(PVM_EncMatrix[i]);  //included in AverageList
  job0->appendLoop(NAE, LOOP_AVERAGE);
  job0->appendLoop(NR);

  job0->setCompensateGrpDelay(0);
  //show averaged data in acq display but store and process non-averaged data
  job0->setStoreDataMode(STORE_raw);
  job0->setPipelineMode(PIPELINE_raw);

  job0->setDisplayMode(DISPLAY_each_scan); /*otherwise, all scans in one row in acq disp*/



  /* ------------------------------------------------ */
  /* -------               Job 1              ------- */
  /* ------------------------------------------------ */

  int nSlices =  GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  int nTotalScansNav = PVM_NRepetitions*nSlices*AverageListSum; //AverageListSum contains all phase encoding steps
  int transBlocks = nSlices;

  JOBDESCRIPTION navigator;
  navigator = ATB_SetNavBaseLevel(jobParameters, 2, transBlocks, nTotalScansNav, PVM_DummyScans); //pars, pulse shape, transBlocks, totalScans, dummyScans

  ATB_LinkJobToDriftCompModule(jobParameters, navigator);

  DB_MSG(("<--SetAcquisitionParameters\n"));
}


#if DEBUG
#define d(n) (D[n]*1e3)
#define p(n) (P[n]*1e-3)

void printTiming(void)
{
  double tr, te;
 
  te= p(0)/2.0+d(4)+d(1)+d(4);

  switch (LocalizationMode)
  {
    case Slice_FID:
      te+=FillTE1_1*1000;
      break;
    case Slice_SE:
      te+=FillTE1_1*1000+RefSpDur*1000+d(4)+p(1)+RefSpDur*1000+d(4)+FillTE1_2*1000;
      break;
    case Voxel_PRESS:
      te+=FillTE1_1*1000+RefSpDur*1000+d(4)+p(1)+RefSpDur*1000+d(4)+FillTE1_2*1000+FillTE2_1*1000+RefSpDur*1000+d(4)+p(1)+RefSpDur*1000+d(4)+FillTE2_2*1000;
      break;
  }

  /* TR */
  tr = (0.03 + 0.01 + PVM_TriggerModuleTime + PVM_NoeModuleTime + PVM_WsModuleDuration + PVM_FovSatModuleTime + PVM_FatSupModuleTime+d(6)+d(4)+p(0)/2.0+te+PVM_SpecAcquisitionTime+1.0+d(0))*NSLICES;

  if (PVM_NavOnOff == On)
    tr+=0.02+PVM_NavigatorModuleTime;

  DB_MSG(("TE: %f, should be %f, diff %f", te, PVM_EchoTime, te-PVM_EchoTime));
  DB_MSG(("TR: %f, should be %f, diff %f", tr, PVM_RepetitionTime, tr-PVM_RepetitionTime));

  return;
}
#undef d
#undef p
#endif

















