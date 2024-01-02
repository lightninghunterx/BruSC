/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999-2014
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

const char * FISP_BaseLevelRelations_id = "$Id$ (C) 1995-2014 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0


#include "method.h"


void SetBaseLevelParam( void )
{

  DB_MSG(("---> SetBaseLevelParam\n"));


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

  DB_MSG(("<--- SetBaseLevelParam\n"));

}


void SetBasicParameters( void )
{
  DB_MSG(("-->SetBasicParameters\n"));

  /* NSLICES */
  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  ATB_SetNSlices( nSlices );

  /* NR */
  ATB_SetNR( PVM_NRepetitions * PVM_EvolutionCycles );

  /* NI - counts moovie frames and slices */
  ATB_SetNI( PVM_NMovieFrames * nSlices );

  /* NA */
  ATB_SetNA( 1 );

  /* NAE counts averages of experiments */
  ATB_SetNAE( PVM_NAverages );

  /* NECHOES */
  NECHOES = PVM_NMovieFrames;

  /* ACQ_obj_order */
  PARX_change_dims("ACQ_obj_order",NI);
  ATB_SetAcqObjOrder( nSlices, PVM_ObjOrderList, PVM_NMovieFrames, 1 );

  DB_MSG(("<--Exiting SetBasicParameters\n"));
}

void SetFrequencyParameters( void )
{
  int nslices, i, *sort, ssfpsize;
  double d1,d9, freq1, freq2, cyc, cyc2, cyc_frac, cyc_frac2;
  double *O2_list, *O3_list;

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


  /* delay used for phase advance in ppg in a TR */
  d1 = (EncGradDur - CFG_GradientRiseTime() - 0.015) / 1000.0;
  /* delay used for phase advance in ppg after alpha/2 */
  d9 = (D[9]-1e-5);


  if(RFSpoilerOnOff != On)
  {
    O2_list = (double *)(malloc(nslices*sizeof(double)));
    O3_list = (double *)(malloc(nslices*sizeof(double)));
    sort =    (int *)(malloc(nslices*sizeof(int)));

    for( i=0; i<nslices; i++ )
    {
      freq1 = ACQ_O1_list[i];
      /* RF phase rotation between alpha pulses, in cycles */
      cyc = freq1*1e-3*PVM_RepetitionTime;
      cyc_frac = cyc - floor(cyc);

      /* RF phase rotation between alpha/2 and alpha, in cycles */
      cyc2 = freq1*1e-3*PVM_RepetitionTime*Alpha2_Prep_time;
      cyc_frac2 = cyc2 - floor(cyc2);

      freq2 = freq1 + (PhaseAdvance/360.0 - cyc_frac) / d1; /*30.01.*/
      O2_list[i] = freq2;


      freq2 = freq1 + (PhaseAdvance/360.0 - cyc_frac2) / d9;
      O3_list[i] = freq2;
      sort[i] =i;
    }
    ATB_SetAcqO2List( nslices, sort, O2_list );
    ATB_SetAcqO3List( nslices, sort, O3_list );
    free(O2_list);
    free(O3_list);
    free(sort);
  }
  else
  {
    /* ssfpsize = L[6]+(L[4]+L[1])*L[3]; */

    ssfpsize = (int)(PVM_DummyScans + (PVM_Matrix[1]*PVM_AntiAlias[1]/Nsegments + SeparationEchoes)*PVM_NMovieFrames);
    ACQ_O2_list_size = ssfpsize;
    ParxRelsParRelations("ACQ_O2_list_size", Yes);
    MRT_RFSpoilFreqList(117,ssfpsize,ACQ_O2_list,d1*1000);

    ACQ_O3_list_size = 1;
    ParxRelsParRelations("ACQ_O3_list_size", Yes);
    ACQ_O3_list[0] = 0.0;
  }


  ATB_SetAcqO1BList( nslices,
                     PVM_ObjOrderList,
                     PVM_ReadOffsetHz);

  DB_MSG(("<-- SetFrequencyParameters\n"));
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
  ACQ_gradient_amplitude[0]  =  SliceGrad;
  ACQ_gradient_amplitude[1]  =  (Ssfp==ECHO? -1.0:1.0) * SliceRephaseGrad;
  ACQ_gradient_amplitude[2]  =  (Ssfp==ECHO? -1.0:1.0) * ReadDephGrad;
  ACQ_gradient_amplitude[3]  =  Phase2DGrad;
  ACQ_gradient_amplitude[4]  = -Phase3DGrad;
  ACQ_gradient_amplitude[5]  =  ReadGrad;
  ACQ_gradient_amplitude[6]  =  PrepSpoiler.ampl;
  ACQ_gradient_amplitude[7]  = -Phase2DGrad;
  ACQ_gradient_amplitude[8]  =  Phase3DGrad;
  ACQ_gradient_amplitude[9]  =  (Ssfp==FID? -1.0:1.0) * ReadDephGrad;
  ACQ_gradient_amplitude[10] =  (Ssfp==FID? -1.0:1.0) * SliceRephaseGrad;
  ACQ_gradient_amplitude[11] =  Alpha2SliceRephGrad;
  ACQ_gradient_amplitude[12] =  SliceSpoiler.ampl;


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
  nrep = PVM_NRepetitions;

  /* ACQ_dim */
  ACQ_dim = spatDim;
  ParxRelsParRelations("ACQ_dim",Yes);

  /* ACQ_dim_desc */
  ATB_SetAcqDimDesc( 0, spatDim, NULL );

  ATB_SetAcqFov( Spatial, spatDim, PVM_Fov, PVM_AntiAlias );

  ACQ_flip_angle = ExcPulse1.Flipangle;

  PARX_change_dims("ACQ_echo_time",1);
  ACQ_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;

  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] =  PVM_RepetitionTime - ExcPulse1.Length;

  /* calculation of ACQ_time_points */
  ATB_EvolutionSetTimePoints(nrep, OneRepTime);

  if(FISP_inversion_enable == Yes)
  {
    PARX_change_dims("ACQ_inversion_time",PVM_NMovieFrames);
    ACQ_inversion_time[0] = PVM_InversionTime;
    for( i=1; i < PVM_NMovieFrames; i++ )
    {
      ACQ_inversion_time[i] = PVM_InversionTime + i * Seg_Sep_time;
    }
  }
  else
    ParxRelsResetPar("ACQ_inversion_time");

  ATB_SetAcqSliceAngle( PtrType3x3 PVM_SPackArrGradOrient[0],
			PVM_NSPacks );

  ACQ_slice_orient = Arbitrary_Oblique;

  ACQ_slice_thick = PVM_SliceThick;

  slices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  ParxRelsCopyPar("ACQ_slice_offset","PVM_SliceOffset");
  ParxRelsCopyPar("ACQ_read_offset","PVM_ReadOffset");
  ParxRelsCopyPar("ACQ_phase1_offset","PVM_Phase1Offset");

  if( spatDim == 3 )
  {
    ParxRelsCopyPar("ACQ_phase2_offset","PVM_Phase2Offset");
  }

  ParxRelsShowInFile("ACQ_slice_offset,ACQ_read_offset,"
		     "ACQ_phase1_offset,ACQ_phase2_offset");

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

  /* frames come inside the slice loop - they need to be declared
     as echo images: */
  ACQ_n_echo_images = PVM_NMovieFrames;
  if(PVM_NMovieFrames>1)
  {
    PARX_change_dims("ACQ_echo_descr", PVM_NMovieFrames, 20);
    for(i=0; i<PVM_NMovieFrames; i++)
    {
      if(FISP_inversion_enable == No)
      {
        snprintf(ACQ_echo_descr[i],20, "frame %d",i+1 );
      }
      else
      {
        int inversiontime,j;
        inversiontime = (int)floor(ACQ_inversion_time[i]);
        j=i+1;
        snprintf(ACQ_echo_descr[i],20, "frame %d TI=%dms",j , inversiontime);
      }
    }
  }
  ACQ_n_movie_frames = 1;

  strcpy(ACQ_fit_function_name,"T1InvRephFisp");

  DB_MSG(("<--SetInfoParameters\n"));

}

void SetPpgParameters( void )
{

  DB_MSG(("-->SetPpgParameters\n"));

  ATB_SetPulprog("FISP.ppg");

  double riseT = CFG_GradientRiseTime();

  D[0]  = 1e-6;  /* not used */
  D[1]  = (EncGradDur - riseT) / 1000.0;
  D[2]  = (ReadDephaseTime - EncGradDur)/1000.0;
  D[3]  = 1e-6;  /* not used */
  D[4]  = riseT / 1000.0;
  D[5]  = 1e-6;  /* not used */
  if(FISP_inversion_enable == Yes)
  {
    D[6]  = (Inv_Rep_time-PVM_InversionTime-InvPulse1.Length/2-0.02-riseT-Seg_Sep_Fra_time+(PVM_EncCentralStep1 -1)*PVM_RepetitionTime) / 1000.0;
  }
  else
  {
    D[6]  = (Inv_Rep_time-Prescan_Delay-Seg_Sep_Fra_time-0.02) / 1000.0;
  }
  D[7]  = 1e-3;  /* Trigger delay */
  D[9]  = (Alpha2Delay - riseT)/1000.0;
  D[10] = SliceSpoiler.dur/1000.0;
  D[11] = PrepSpoiler.dur/1000.0;
  D[16] = (PVM_InversionTime - MinInversionTime + 0.01)/1000.0;
  D[20] = FID_delay/1000.0;
  D[21] = ECHO_delay/1000.0;


  /*** set shaped pulses ***/
  SetRfPulses();

  /* loop counters */

  L[1] = SeparationEchoes;
  L[3] = PVM_NMovieFrames;
  L[4] = PVM_RareFactor;
  L[5] = Nsegments;
  L[6] = PVM_DummyScans;
  L[7] = NumPrep;


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

  job0->appendLoop(PVM_RareFactor);
  job0->appendLoop(NI, LOOP_SETUP);
  job0->appendLoop(Nsegments);
  job0->appendLoop(NAE, LOOP_AVERAGE);
  job0->appendLoop(NR);

  DB_MSG(("<--SetAcquisitionParameters"));
}

void SetRfPulses(void)
{
  DB_MSG(("-->SetRfPulses"));

  double alphaRef = ExcPulse1.Flipangle;

  /* alpha pulse */
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[1]","P[1]");
  SetPrepPulseList(alphaRef);

  /* alpha/2 pulse */
  ExcPulse1.Flipangle = alphaRef/2;
  STB_UpdateRFPulse("ExcPulse1",1,PVM_DeriveGains,Conventional);
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[3]","P[3]");

  /* inversion pulse */
  ATB_SetRFPulse("InvPulse1","ACQ_RfShapes[4]","P[4]");

  /* pulses for starter sequence */
  ExcPulse1.Flipangle = MAX_OF(2,FlipAngMz);
  STB_UpdateRFPulse("ExcPulse1",1,PVM_DeriveGains,Conventional);
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[5]","P[5]");

  ExcPulse1.Flipangle = MAX_OF(2,(90-FlipAngMz));
  STB_UpdateRFPulse("ExcPulse1",1,PVM_DeriveGains,Conventional);
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[6]","P[6]");

  // reset ExcPulse1
  ExcPulse1.Flipangle = alphaRef;
  STB_UpdateRFPulse("ExcPulse1",1,PVM_DeriveGains,Conventional);

  DB_MSG(("<--SetRfPulses"));
}


void SetPrepPulseList(double alphaRef)
{
  DB_MSG(("-->SetPrepPulseList"));

  bool calculated = ExcPulse1.Calculated == Yes ? true:false;
  int npts=0;
  int prep = NumPrep;
  char shape[128];

  if((prep > 0) && (Ssfp_preparation==Starter_Sequence))
  {
    PARX_change_dims("PrepPulseList",prep);

    if(calculated)
    {
      npts=(int)PARX_get_dim("ExcPulse1Shape",1);
      PARX_change_dims("PrepPulseShapeList",prep,npts);

    }

    for(int i=0;i<prep;i++)
    {
      ExcPulse1.Flipangle = alphaRef*(2*i+1)/(2*prep+1);
      STB_UpdateRFPulse("ExcPulse1",1,PVM_DeriveGains,Conventional);
      snprintf(shape,128,"PrepPulseList[%d]",i);
      ATB_SetRFPulse("ExcPulse1",shape);
      if(calculated)
      {
	memcpy(PrepPulseShapeList[i],ExcPulse1Shape,npts*sizeof(double));
	snprintf(PrepPulseList[i].name,32,"$PrepPulseShapeList[%d]",i);
      }

      snprintf(PrepPulseList[i].name,32,ExcPulse1.Shape);
    }
  }
  else
  {
    PARX_change_dims("PrepPulseList",1);
    PARX_change_dims("PrepPulseShapeList",1,1);
  }

  DB_MSG(("<--SetPrepPulseList"));
}


#if DEBUG
#define d(n) (D[n]*1e3)
#define p(n) (P[n]*1e-3)

void printTiming(void)
{
  double te,aqq=PVM_AcquisitionTime,tr;
  /* TE */
  te = p(0)/2+d(1)+d(4)+d(20)+aqq/2+0.01;

  DB_MSG(("te: %f should be %f diff %f\n",te,PVM_EchoTime,te-PVM_EchoTime));

  /* TR */
  tr = 0.01+p(0)/2+aqq/2+d(21)+d(1)+d(4)+te;

  DB_MSG(("TR: %f, should be %f, diff %f", tr, PVM_RepetitionTime, tr-PVM_RepetitionTime));

  return;
}

#undef d
#undef p
#endif
