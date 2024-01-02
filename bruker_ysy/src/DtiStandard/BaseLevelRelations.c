/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002-2014
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

const char * DtiStandard_BaseLevelRelations_id = "$Id$ (C) 2002-2014 Bruker BioSpin MRI GmbH";

#define DEBUG    0
#define DB_MODULE  0
#define DB_LINE_NR  0




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

  int nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  ATB_DwAcq(ACQ_O1_list, nslices, ExcSliceGrad,Yes);

  PrintTimingInfo();

  DB_MSG(("<--SetBaseLevelParam\n"));
}



void SetBasicParameters( void )
{
  DB_MSG(("-->SetBasicParameters\n"));

  /* NSLICES */
  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  ATB_SetNSlices( nSlices );

  /* NR */
  ATB_SetNR( PVM_NRepetitions);

  /* NI */
  ATB_SetNI( nSlices*PVM_DwNDiffExp);

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
  ACQ_ns_list_size = 1;
  ParxRelsParRelations("ACQ_ns_list_size",Yes);

  ACQ_ns_list[0] = 1;

  /* NECHOES */
  NECHOES = 1;

  /* ACQ_obj_order */
  PARX_change_dims("ACQ_obj_order",NI);
  ATB_SetAcqObjOrder( nSlices, PVM_ObjOrderList, 1, PVM_DwNDiffExp );

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
  DB_MSG(("-->SetGradientParameters\n"));


  int spatDim = PTB_GetSpatDim();

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
  ACQ_gradient_amplitude[6] = -Phase2DGrad;
  ACQ_gradient_amplitude[7] =  Phase3DGrad;

  DB_MSG(("<--SetGradientParameters\n"));

}



void SetPpgParameters( void )
{
  DB_MSG(("-->SetPpgParameters\n"));


  ATB_SetPulprog("DtiStandard.ppg");

  L[0] = PVM_EncMatrix[1];
  L[1] = (PTB_GetSpatDim() == 3) ? PVM_EncMatrix[2] : 1;


  int slices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  double igwT = CFG_InterGradientWaitTime();
  double riseT = CFG_GradientRiseTime();

  D[0]  = ((PVM_RepetitionTime - PVM_MinRepetitionTime)/slices + igwT)/1000.0;
  D[1]  =  riseT/1000.0;
  D[2]  = (ExcSliceRephTime - riseT)/1000.0;
  D[3]  = (riseT+igwT)/1000.0;
  D[4]  = (riseT + igwT)/1000.0;
  D[5]  = (ReadDephTime - riseT)/1000.0;
  D[6]  = riseT/1000.0;
  D[7]  = riseT/1000.0;
  D[8]  = igwT/1000.0;



  /* set shaped pulses, in this method ACQ_RfShapes[0] is used
     the pulse duration is stored in baselevel parameter P[0]
  */
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");


  DB_MSG(("<--SetPpgParameters\n"));
}



void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters\n"));

  /* ------------------------------------------------ */
  /* -------               Job 0              ------- */
  /* ------------------------------------------------ */
  JOBPARAMETERS(jobParameters);
  JOBDESCRIPTION job0=jobParameters->getOrCreateJob("job0");

  job0->initPars(1, PVM_EncMatrix[0], PVM_EffSWh * PVM_AntiAlias[0]);

  job0->appendLoop(NSLICES);

  job0-> appendLoop(PVM_DummyScans, LOOP_DUMMIES);

  job0->appendLoop(PVM_DwNDiffExp);

  job0->appendLoop(NA, LOOP_AVERAGE, LOOP_SETUP);

  job0->appendLoop(L[0]);

  job0->appendLoop(L[1]);

  job0->appendLoop(NAE, LOOP_AVERAGE);

  job0->appendLoop(NR);

  DB_MSG(("<--SetAcquisitionParameters\n"));
}



void SetInfoParameters( void )
{
  DB_MSG(("-->SetInfoParameters\n"));

  int spatDim = PTB_GetSpatDim();
  int nrep = PVM_NRepetitions*PVM_DwNDiffExp;

  ACQ_dim = spatDim;
  ParxRelsParRelations("ACQ_dim",Yes);

  ATB_SetAcqDimDesc( 0, spatDim, NULL );

  ATB_SetAcqFov( Spatial, spatDim, PVM_Fov, PVM_AntiAlias );

  ACQ_flip_angle = ExcPulse1.Flipangle;

  PARX_change_dims("ACQ_echo_time",1);

  ACQ_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;

 /* calculation of ACQ_time_points */
  PARX_change_dims("ACQ_time_points",nrep);
  ACQ_time_points[0] = 0;
  for(int i=1; i<nrep; i++)
    ACQ_time_points[i] = OneRepTime * i;

  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] =
    PVM_RepetitionTime -
    (PVM_EchoTime/2 + ExcPulse1.Length/2);

  PARX_change_dims("ACQ_inversion_time",1);
  ACQ_inversion_time[0] = PVM_InversionTime;
  strcpy(ACQ_fit_function_name,"dtraceb");

  ATB_SetAcqSliceAngle( PtrType3x3 PVM_SPackArrGradOrient[0],
      PVM_NSPacks );

  ACQ_slice_orient = Arbitrary_Oblique;

  ACQ_slice_thick = PVM_SliceThick;

  int slices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  PARX_change_dims("ACQ_slice_offset",slices);
  PARX_change_dims("ACQ_read_offset",slices);
  PARX_change_dims("ACQ_phase1_offset",slices);
  PARX_change_dims("ACQ_phase2_offset",slices);

  for(int i=0;i<slices;i++)
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
    for(int i=1; i<slices;i++ )
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

  /*
   *  set correct image labelling, code defined in this module
   *  see below
   */

  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();

  SetDiffImageLabels(PVM_DwAoImages,
                     PVM_DwNDiffDir,
                     PVM_DwNDiffExpEach,
                     PVM_DwEffBval,
                     PVM_NRepetitions,
                     No);                  /* No Echo loop, diff loop is
                                              outside of the slices loop */


  DB_MSG(("<--SetInfoParameters\n"));

}



void PrintTimingInfo(void)
{
  double te1,te2,tr UNUSED_PARAM;

  DB_MSG(("-->PrintTimingInfo\n"));

  te1=(P[0])/2000.0+(D[2]+D[3]+D[4])*1000.0;

  te2=(D[4]+D[5]+D[6])*1000.0 +
      PVM_AcquisitionTime*PVM_EchoPosition/100.0;


  tr = (   0.03                                            /* UpdateDynPars */
          + 0.01                                           /* freqRx */
          + te1 + te2 + PVM_DwModDur 
          + P[0]/2000.0
          + (D[0]+D[1]+D[7]+D[5]+D[8])*1000
          + PVM_FatSupModuleTime
          + PVM_AcquisitionTime*(100.0-PVM_EchoPosition)/100.0)*NSLICES;


  DB_MSG(("te1+te2+PVM_DwModEchDel: %f (ist) soll: %f abw.: %f",
    te1+te2+PVM_DwModEchDel,PVM_EchoTime,
    te1+te2+PVM_DwModEchDel-PVM_EchoTime));

  DB_MSG(("tr: %f  soll: %f  diff %f\n",tr,PVM_RepetitionTime,
    fabs(tr-PVM_RepetitionTime)));



  DB_MSG(("<--PrintTimingInfo\n"));

}



void SetDiffImageLabels(const int na0,
                        const int nd,
                        const int nbpd,
                        const double *bval,
                        const int nr,
                        YesNo EchoLoop)
{
  int ndiffexp;
  int i,j,k,l,tsl;
  char buffer[200];

  ndiffexp=na0+nd*nbpd;
  tsl = ndiffexp*nr;

  DB_MSG(("-->SetDiffImageLabels"));
  DB_MSG(("na0 = %d\nnd = %d\nnpbd= %d\n"
          "ndiffexp = %d\ntsl=%d\n"
          "nr=%d",
          na0,nd,nbpd,ndiffexp,tsl,nr));

  switch(EchoLoop)
  {
    default:
    case No:
      ACQ_n_echo_images = 1;
      ACQ_n_movie_frames = tsl;
      PARX_change_dims("ACQ_movie_descr",tsl,20);
      PARX_change_dims("ACQ_echo_descr",1,20);
      for(l=0;l<nr;l++)
      {
        for(i=0;i<na0;i++)
        {
          sprintf(buffer,"A0 %d B %.0f",i+1,bval[i]);
          buffer[19]='\0';
          strncpy(ACQ_movie_descr[l*ndiffexp+i],buffer,20);
        }
        for(k=na0,i=0; i<nd ; i++)
        {
          for(j=0;j<nbpd;j++,k++)
          {
            sprintf(buffer,"Dir %d B %.0f",i+1,bval[k]);
            buffer[19]='\0';
            strncpy(ACQ_movie_descr[l*ndiffexp+k],buffer,20);
          }
        }
      }
      break;
    case Yes:
      ACQ_n_echo_images = tsl;
      ACQ_n_movie_frames = 1;
      PARX_change_dims("ACQ_movie_descr",1,20);
      PARX_change_dims("ACQ_echo_descr",tsl,20);
      for(l=0;l<nr;l++)
      {
        for(i=0;i<na0;i++)
        {
          sprintf(buffer,"A0 %d B %.0f",i+1,bval[i]);
          buffer[19]='\0';
          strncpy(ACQ_echo_descr[l*ndiffexp+i],buffer,20);
        }

        for(k=na0,i=0; i<nd ; i++)
        {
          for(j=0;j<nbpd;j++,k++)
          {
            sprintf(buffer,"Dir %d B %.0f",i+1,bval[k]);
            buffer[19]='\0';
            strncpy(ACQ_echo_descr[l*ndiffexp+k],buffer,20);
          }
        }
      }

      break;

  }

}
