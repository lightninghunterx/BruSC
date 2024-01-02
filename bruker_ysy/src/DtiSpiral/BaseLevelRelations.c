/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2007
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

static const char resid[] = "$Id$ (c) 2007 Bruker BioSpin MRI GmbH";


#define DEBUG       0
#define DB_MODULE   0
#define DB_LINE_NR  0


#include "method.h"

void SetBaseLevelParam()
{
  DB_MSG(("-->SetBaseLevelParam"));

  SetBasicParameters();

  SetFrequencyParameters();

  SetGradientParameters();

  SetPpgParameters();

  SetAcquisitionParameters();

  ATB_SetReceiverGains();

  ATB_DwAcq( ACQ_O1_list,
	           GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices ),
	           SliceGrad,No
           );

  ATB_SetFatSupBaselevel();
  ATB_SetFovSatBaseLevel();
  ATB_SetTaggingBaseLevel();

  SetInfoParameters();

  DB_MSG(("<--SetBaseLevelParam"));
}

void SetBasicParameters( void )
{
  DB_MSG(("-->SetBasicParameters"));

  /* NSLICES */

  int nSlices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);
  ATB_SetNSlices(nSlices);

  /* NR */

  ATB_SetNR(PVM_NRepetitions * PVM_DwNDiffExp);
  
  /* NI */

  ATB_SetNI(nSlices);

  /* AVERAGING */

  switch(PVM_MotionSupOnOff)
  {
    default:
    case Off:
      ATB_SetNA(PVM_NAverages);
      ATB_SetNAE(1);
      break;
    case On:
      ATB_SetNAE(PVM_NAverages);
      ATB_SetNA(1);
      break;
  }

  /* NECHOES */

  NECHOES = PVM_NEchoImages;

  /* ACQ_obj_order */

  PARX_change_dims("ACQ_obj_order", NI);
  ATB_SetAcqObjOrder(nSlices, PVM_ObjOrderList, 1, 1);

  DB_MSG(("<--SetBasicParameters"));
}

void SetFrequencyParameters( void )
{
  DB_MSG(("-->SetFrequencyParameters"));

  ATB_SetNuc1(PVM_Nucleus1);

  sprintf(NUC2, "off");
  sprintf(NUC3, "off");
  sprintf(NUC4, "off");
  sprintf(NUC5, "off");
  sprintf(NUC6, "off");
  sprintf(NUC7, "off");
  sprintf(NUC8, "off");

  ATB_SetNucleus(PVM_Nucleus1);

  ACQ_O1_mode = BF_plus_Offset_list;
  ParxRelsParRelations("ACQ_O1_mode", Yes);

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
  ParxRelsParRelations("BF1", Yes); // no need for other BF's

  int nslices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);

  ATB_SetAcqO1List(nslices,
                   PVM_ObjOrderList,
                   PVM_SliceOffsetHz);

  ATB_SetAcqO1BList(nslices,
                    PVM_ObjOrderList,
                    PVM_ReadOffsetHz);

  

  DB_MSG(("<--SetFrequencyParameters"));
}

void SetGradientParameters( void )
{
  DB_MSG(("-->SetGradientParameters"));

  int spatDim = PTB_GetSpatDim();
  PARX_change_dims("ACQ_phase_encoding_mode", spatDim );
  PARX_change_dims("ACQ_phase_enc_start", spatDim );
  switch(spatDim)
  {
    case 3:
      ACQ_phase_encoding_mode[2] = User_Defined_Encoding;
      ACQ_phase_enc_start[2] = -1.0; /* set, but no used */
      ACQ_spatial_size_2 = PVM_EncMatrix[2];
      ParxRelsCopyPar("ACQ_spatial_phase_2", "PVM_EncValues2");
      /* no break */
    default:
      ACQ_phase_encoding_mode[1] = Read;
      ACQ_phase_enc_start[1] = -1.0;
      ACQ_phase_encoding_mode[0] = Read;
      ACQ_phase_enc_start[0] = -1.0;
  }
  ParxRelsParRelations("ACQ_phase_encoding_mode", Yes);

  ATB_SetAcqGradMatrix("PVM_SliceGeoObj");

  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 1.0;
  ACQ_scaling_slice = 1.0;

  DB_MSG(("<--SetGradientParameters"));
}

void SetPpgParameters(void)
{
  DB_MSG(("-->SetPpgParameters"));

  ATB_SetPulprog("DtiSpiral.ppg");

  /* set shaped pulses */
  ATB_SetRFPulse("ExcPul", "ACQ_RfShapes[0]", "P[0]");

  int spatDim = PTB_GetSpatDim();
  L[2] = spatDim>2 ? PVM_EncMatrix[2] : 1;
  
  DB_MSG(("<--SetPpgParameters"));
}

void SetInfoParameters( void )
{
  DB_MSG(("-->SetInfoParameters"));

  /* ACQ_dim */

  int spatDim = PTB_GetSpatDim();
  ACQ_dim = spatDim;
  ParxRelsParRelations("ACQ_dim", Yes);

  /* ACQ_dim_desc */

  ATB_SetAcqDimDesc(0, spatDim, NULL);

  ATB_SetAcqFov(Spatial, spatDim, PVM_Fov, PVM_AntiAlias);

  ACQ_flip_angle = ExcPul.Flipangle;

  PARX_change_dims("ACQ_echo_time", 1);

  ACQ_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_inter_echo_time", 1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_repetition_time", 1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;

  PARX_change_dims("ACQ_recov_time", 1);
  ACQ_recov_time[0] = PVM_RepetitionTime - (PVM_EchoTime / 2.0 + ExcPul.Length / 2.0);
  
  PARX_change_dims("ACQ_inversion_time", 1);
  ACQ_inversion_time[0] = PVM_InversionTime;

  strcpy(ACQ_fit_function_name,"dtraceb");

  ATB_SetAcqSliceAngle(
    PtrType3x3 PVM_SPackArrGradOrient[0],
		PVM_NSPacks);

  ACQ_slice_orient = Arbitrary_Oblique;

  ACQ_slice_thick = PVM_SliceThick;

  int nSlices = GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);

  PARX_change_dims("ACQ_slice_offset",  nSlices);
  PARX_change_dims("ACQ_read_offset",   nSlices);
  PARX_change_dims("ACQ_phase1_offset", nSlices);
  PARX_change_dims("ACQ_phase2_offset", nSlices);

  for (int i = 0; i < nSlices; i++)
  {
    ACQ_slice_offset[i]  = PVM_SliceOffset[i];
    ACQ_read_offset[i]   = PVM_ReadOffset[i];
    ACQ_phase1_offset[i] = PVM_Phase1Offset[i];
    ACQ_phase2_offset[i] = PVM_Phase2Offset[i];
  }

  ACQ_read_ext = (int)PVM_AntiAlias[0];

  PARX_change_dims("ACQ_slice_sepn", nSlices == 1 ? 1 : nSlices - 1);

  if (nSlices == 1)
  {
    ACQ_slice_sepn[0] = 0.0;
  }
  else
  {
    for (int i = 1; i < nSlices; i++)
    {
      ACQ_slice_sepn[i-1] = PVM_SliceOffset[i] - PVM_SliceOffset[i-1];
    }
  }

  ATB_SetAcqSliceSepn(
    PVM_SPackArrSliceDistance,
    PVM_NSPacks);

  ATB_SetAcqExpType(Imaging);

  ACQ_n_t1_points = 1;

  if (!ParxRelsParHasValue("ACQ_contrast_agent"))
  {
    ACQ_contrast_agent[0] = '\0';
  }

  if (!ParxRelsParHasValue("ACQ_contrast"))
  {
    ACQ_contrast.volume = 0.0;
    ACQ_contrast.dose = 0.0;
    ACQ_contrast.route[0] = '\0';
    ACQ_contrast.start_time[0] = '\0';
    ACQ_contrast.stop_time[0] = '\0';
  }

  ParxRelsParRelations("ACQ_contrast_agent", Yes);

  ACQ_position_X = 0.0;
  ACQ_position_Y = 0.0;
  ACQ_position_Z = 0.0;

  PARX_change_dims("ACQ_temporal_delay", 1);
  ACQ_temporal_delay[0] = 0.0;

  ACQ_RF_power = 0;

  /* frames come inside the slice loop - they need to be declared as echo images */
  ACQ_n_echo_images = 1;
  if (ACQ_n_echo_images > 1) 
  {
    PARX_change_dims("ACQ_echo_descr", ACQ_n_echo_images, 20);
    for (int i = 0; i < ACQ_n_echo_images; i++)
    {
      snprintf(ACQ_echo_descr[i], 20, "frame %d", i+1);
    }
  }
  ACQ_n_movie_frames = 1;

  SetDiffImageLabels(PVM_DwAoImages,
                     PVM_DwNDiffDir,
                     PVM_DwNDiffExpEach,
                     PVM_DwEffBval,
                     PVM_NRepetitions,
                     No);

  DB_MSG(("<--SetInfoParameters"));
}

void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters"));

  int spatDim = PTB_GetSpatDim();

  /* ------------------------------------------------ */
  /* -------               Job 0              ------- */
  /* ------------------------------------------------ */
  JOBPARAMETERS(jobParameters);
  JOBDESCRIPTION job0=jobParameters->getOrCreateJob("job0");

  job0->initPars( 1, PVM_SpiralNbOfAcqPoints, PVM_EffSWh );

  job0->appendLoop( NSLICES);
  job0->appendLoop( PVM_DummyScans, LOOP_DUMMIES);
  job0->appendLoop( NA, LOOP_AVERAGE, LOOP_SETUP);
  job0->appendLoop( PVM_SpiralNbOfInterleaves);
  for (int i = 2; i < spatDim; i++)
    job0->appendLoop(PVM_EncMatrix[i]);
  job0->appendLoop(NAE, LOOP_AVERAGE);
  job0->appendLoop(PVM_DwNDiffExp);
  job0->appendLoop(PVM_NRepetitions);

  DB_MSG(("<--SetAcquisitionParameters"));
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
