/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2001 - 2009 Bruker BioSpin MRI GmbH";

#define DEBUG       0
#define DB_MODULE   1
#define DB_LINE_NR  1


/*:=INFO=:*******************************************************
 *
 * Description :
 *   it contains functions necessary for responding to a 'load'
 *  of this method. This function will be automatically
 *  called in response to a method file for this method
 *  having been read.
 *
 *::=info=:******************************************************/

/****************************************************************/
/****************************************************************/
/*    I N T E R F A C E   S E C T I O N    */
/****************************************************************/


/****************************************************************/

/****************************************************************/
/*    I N C L U D E   F I L E S      */
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
  ATB_SetTaggingBaseLevel();

#if DEBUG
  printTiming();
#endif

  DB_MSG(("Exiting SetBaseLevelParam"));

}



void SetBasicParameters( void )
{
  DB_MSG(("Entering SetBasicParameters()"));

  /* NSLICES */

  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  ATB_SetNSlices( nSlices );

  /* NR */

  ATB_SetNR( PVM_SatTransRepetitions * PVM_NRepetitions );

  /* NI */

  ATB_SetNI( nSlices * PVM_NEchoImages );

  /* NA */

  ATB_SetNA( PVM_NAverages );

  /* NAE */

  ATB_SetNAE( 1 );

  /* ACQ_ns */

  ACQ_ns_list_size = 1;
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

  int nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

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
  int i;

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

  for( i=0; i<spatDim; i++ )
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
  ACQ_gradient_amplitude[3] = RfcSpoilerStrength;      /* g3 */

  DB_MSG(("Exiting SetGradientParameters()"));
}



void SetInfoParameters( void )
{
  int slices, i, spatDim;

  DB_MSG(("Entering SetInfoParameters()"));

  spatDim = PTB_GetSpatDim();

  ACQ_dim = spatDim;
  ParxRelsParRelations("ACQ_dim",Yes);

  ATB_SetAcqDimDesc( 0, spatDim, NULL );

  ATB_SetAcqFov( Spatial, spatDim, PVM_Fov, PVM_AntiAlias );

  ACQ_flip_angle = ExcPul.Flipangle;

  PARX_change_dims("ACQ_echo_time",PVM_NEchoImages); /* 1 */

  ACQ_echo_time[0] = SpinEchoTime;
  EffectiveEchoTime[0] = SpinEchoTime;
  for(i=1;i<PVM_NEchoImages;i++) {
    ACQ_echo_time[i]=SpinEchoTime+i*SpinEchoTime;
    EffectiveEchoTime[i] = SpinEchoTime+i*SpinEchoTime;
  }

  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;

  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] =  PVM_RepetitionTime - ExcPul.Length;

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

  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();
  ACQ_n_echo_images = PVM_NEchoImages;

  strcpy(ACQ_fit_function_name,"t2vtr");

  DB_MSG(("Exiting SetInfoParameters()"));

}



void SetPpgParameters( void )
{
  DB_MSG(("Entering SetPpgParameters()"));

  ATB_SetPulprog("T2_EPI.ppg");

  double riseTime = CFG_GradientRiseTime();

  D[0]  = ((PVM_RepetitionTime - PVM_MinRepetitionTime)/NSLICES) / 1000.0;
  D[1] = (RephaseTime - riseTime) / 1000.0;
  D[2]  = (SpinEchoTime/2 - MinTE_right) / 1000.0;
  D[4]  = riseTime / 1000.0;
  D[5] = SliceSpoiler.dur*1e-3;
  D[6] = PackDel*1e-3;
  D[7]  = (SpinEchoTime/2 - MinTE_left) / 1000.0;
  D[9] = (RfcSpoilerDuration-riseTime)*1e-3;
  D[10] = (SpinEchoTime-PVM_EpiModuleTime) / 1000.0;
  D[11] = (SpinEchoTime/2.0-MinTE_left2) / 1000.0;
  D[12] = (SpinEchoTime/2.0-MinTE_right2) / 1000.0;

  /* set shaped pulses */
  ATB_SetRFPulse("ExcPul","ACQ_RfShapes[0]","P[0]");
  ATB_SetRFPulse("RefPul","ACQ_RfShapes[1]","P[1]");

  /* loop counters */
  L[1] = (PTB_GetSpatDim() > 2) ? PVM_EncMatrix[2] : 1;

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

  job0->appendLoop(PVM_NEchoImages);

  job0->appendLoop(NSLICES);

  job0->appendLoop(PVM_DummyScans, LOOP_DUMMIES);

  job0->appendLoop(NA, LOOP_AVERAGE);

  if (ACQ_SetupDim > 2)
  {
    job0->appendLoop(PVM_EpiNShots); 
    job0->appendLoop(L[1], LOOP_SETUP);  //3D loop
  }
  else
  {
    job0->appendLoop(PVM_EpiNShots, LOOP_SETUP); //setup here, for online reco
    job0->appendLoop(L[1]);  //3D loop
  }

  job0->appendLoop(NR);

  DB_MSG(("<--SetAcquisitionParameters"));
}



#if DEBUG
#define d(n) D[n]*1e3
#define p(n) P[n]*1e-3

void printTiming(void)
{
  double pre_prep,prep, te,te2,part1,part2,part3,part4,tr;
  double trigOutSlice, trigOutVol;

  DB_MSG(("-->printTiming"));

  if(PVM_TriggerOutOnOff == On)
  {
    switch(PVM_TriggerOutMode)
    {
    case PER_SLICE:
      trigOutSlice = PVM_TriggerOutModuleTime;
      trigOutVol = 0.0;
      break;

    case PER_VOLUME:
      trigOutSlice = 0.0;
      trigOutVol = PVM_TriggerOutModuleTime;
      break;

    case AT_START:
    default:
      trigOutSlice = 0.0;
      trigOutVol = 0.0;

    }
  }
  else
    trigOutSlice = trigOutVol = 0.0;

  pre_prep=trigOutVol + PVM_TaggingModuleTime;
  prep=0.03 + 0.01 + trigOutSlice + PVM_FovSatModuleTime +
       PVM_SatTransModuleTime + PVM_FatSupModuleTime;
  part1=d(5)+d(4)+p(0)+d(4)+d(1)+d(4);
  part2=d(7)+d(9)+d(4)+p(1)+d(9)+d(4);
  part3=d(2)+(0.01+PVM_EpiModuleTime)+d(0);
  if(PVM_NEchoImages>1)
    part4=(0.010+PVM_EpiModuleTime+d(11)+d(9)+d(4)+p(1)+d(9)+d(4)+d(12))*(PVM_NEchoImages-1);
  else part4=0;

  te= ExcPul.Rpfac/100*p(0)+d(4)+d(1)+d(4)+d(2)+(0.01+PVM_EpiEchoDelay) +part2;
  tr=pre_prep+NSLICES*(prep+part1+part2+part3+part4);
  tr+=PackDel;

  te2= te + (PVM_EpiModuleTime-PVM_EpiEchoDelay) +d(11) +d(9) +d(4) +p(1) +d(9) +d(4) +d(12) + 0.010 +PVM_EpiEchoDelay;

  DB_MSG(("TR: %fms, TR (soll): %fms (diff: %f)",tr, PVM_RepetitionTime, tr-PVM_RepetitionTime));
  DB_MSG(("TE: %fms, TE (soll): %fms (diff: %f)",te, SpinEchoTime, te-SpinEchoTime));
  DB_MSG(("TE2: %fms, TE2 (soll): %fms (diff: %f)",te2, 2.0*SpinEchoTime, te2-2.0*SpinEchoTime));



  DB_MSG(("ExcPulseLength %f, rephase %f, spoilerdur %f, refpulse %f  l %f r %f",
          ExcPul.Length, RephaseTime, RfcSpoilerDuration, RefPul.Length, MinTE_left, MinTE_right ));
  DB_MSG(("<--printTiming"));
}

#undef d
#undef p
#endif
