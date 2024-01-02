/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

static const char resid[] = "$Id$ (C) 2009 Bruker BioSpin MRI GmbH";


#define DEBUG      0
#define DB_MODULE  0
#define DB_LINE_NR 0


#if DEBUG
#define PRINTARRAY(file, array, nelem)  \
{\
int ix; \
FILE *dfp; \
if((dfp=fopen(file,"w"))!=NULL) \
{ \
for(ix=0; ix<(nelem); ix++) fprintf(dfp, " %g\n", (double) array[ix]);\
fclose(dfp); \
} \
}
#else
#define PRINTARRAY(file, array, nelem) ;
#endif

#include "method.h"

void SetBaseLevelParam()
{

  DB_MSG(("-->SetBaseLevelParam"));

  SetBasicParameters();

  SetFrequencyParameters();

  SetAcquisitionParameters();

  SetGradientParameters();

  SetPpgParameters();

  SetInfoParameters();

  ATB_SetReceiverGains();
  /* setting baselevel parameters used by modules */
  ATB_SetFovSatBaseLevel();
  ATB_SetFatSupBaselevel();
  ATB_SetSatTransBaseLevel();

  PrintTimingInfo();

  DB_MSG(("<--SetBaseLevelParam"));
}

void SetBasicParameters( void )
{
  int nSlices;

  DB_MSG(("-->SetBasicParameters"));

  /* NSLICES */
  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  ATB_SetNSlices( nSlices );

  /* NR */
  ATB_SetNR( PVM_SatTransRepetitions * PVM_NRepetitions );

  /* NI */
  ATB_SetNI( nSlices * PVM_NEchoImages );

  /* AVERAGING */
  ATB_SetNA( 1 );
  ATB_SetNAE( PVM_NAverages );

  /* NECHOES */
  NECHOES = PVM_NEchoImages;

  /* ACQ_obj_order */
  PARX_change_dims("ACQ_obj_order",NI);

  ATB_SetAcqObjOrder( nSlices, PVM_ObjOrderList, PVM_NEchoImages, 1);

  /* online reconstruction */
  ACQ_ScanOnlineReco = RecoOnline;

  DB_MSG(("<--SetBasicParameters"));
}

void SetFrequencyParameters( void )
{
  int nslices;

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


  DB_MSG(("<--SetFrequencyParameters"));
}


void SetGradientParameters( void )
{

  DB_MSG(("-->SetGradientParameters"));

  ATB_SetAcqGradMatrix("PVM_SliceGeoObj");

  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 1.0;
  ACQ_scaling_slice = 1.0;

  /* Calculation of gradient shapes */
  CalcGradShapes();

  /* print shapes for plotting */
  PRINTARRAY("/tmp/UTE3D_GradShape1",GradShape1, GradShapeSize1);
  PRINTARRAY("/tmp/UTE3D_GradShape2",GradShape2, GradShapeSize2);
  PRINTARRAY("/tmp/UTE3D_GradShape3",GradShape3, GradShapeSize3);

  /* gradient amplitudes */
  ACQ_gradient_amplitude[0] =  ReadGrad;     /* g0 */
  ACQ_gradient_amplitude[1] =  PhaseGrad;    /* g1 */
  ACQ_gradient_amplitude[2] =  SliceGrad;    /* g2 */
  ACQ_gradient_amplitude[3] = -RewReadGrad;  /* g3 */
  ACQ_gradient_amplitude[4] = -RewPhaseGrad; /* g4 */
  ACQ_gradient_amplitude[5] = -RewSliceGrad; /* g5 */

  DB_MSG(("<--SetGradientParameters"));

}

void SetPpgParameters(void)
{
  DB_MSG(("-->SetPpgParameters"));

  ATB_SetPulprog("UTE3D.ppg");

  double riseT = CFG_GradientRiseTime();

  D[0] = (PVM_RepetitionTime - PVM_MinRepetitionTime + 0.06)/1000.0;
  D[1] = riseT/1000.0;
  D[3] = (PVM_EchoTime - PVM_MinEchoTime)/1000.0;
  D[4] = ShapeDur1/1000.0;
  D[5] = ShapeDur2/1000.0;
  D[6] = ShapeDur3/1000.0;
  D[7] = StabDelay/1000.0;

  /* set shaped pulses, in this method ACQ_RfShapes[0] is used
     the pulse duration is stored in baselevel parameter P[0] */
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");

  /* set radial projections */
  PARX_change_dims("GradAmpR",NPro);
  PARX_change_dims("GradAmpP",NPro);
  PARX_change_dims("GradAmpS",NPro);

  SetProj3D(GradAmpR,
	    GradAmpP,
	    GradAmpS,
	    1,1,1);


  DB_MSG(("<--SetPpgParameters"));
}


void SetInfoParameters( void )
{
  int slices, i, spatDim;

  DB_MSG(("-->SetInfoParameters"));

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

  PARX_change_dims("ACQ_phase_encoding_mode", spatDim );
  ACQ_phase_encoding_mode[0] = Read;
  for(i=1; i<spatDim; i++ )
    ACQ_phase_encoding_mode[i] = User_Defined_Encoding;
  ParxRelsParRelations("ACQ_phase_encoding_mode",Yes);

  ATB_SetAcqFov( Spatial, spatDim, PVM_Fov, PVM_AntiAlias );

  ACQ_flip_angle = ExcPulse1.Flipangle;

  PARX_change_dims("ACQ_echo_time",PVM_NEchoImages);

  for(i=0;i<PVM_NEchoImages;i++)
    ACQ_echo_time[i] = EffectiveTE[i];

  ACQ_n_echo_images = PVM_NEchoImages;

  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;

  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] =
    PVM_RepetitionTime -
    (PVM_EchoTime/2 + ExcPulse1.Length/2);

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

  DB_MSG(("<--SetInfoParameters"));

}


void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters"));


  /* ------------------------------------------------ */
  /* -------               Job 0              ------- */
  /* ------------------------------------------------ */
  JOBPARAMETERS(jobParameters);
  JOBDESCRIPTION job0=jobParameters->getOrCreateJob("job0");

  int acqPoints = PVM_EncMatrix[0]/2 + RampPoints;
  job0->initPars( 1, acqPoints, PVM_EffSWh );

  job0->appendLoop(NI, LOOP_SETUP);
  job0->appendLoop(NPro);
  job0->appendLoop(NAE, LOOP_AVERAGE);
  job0->appendLoop(NR);

  job0->setForceShortFilter(1);

  /* ------------------------------------------------ */
  /* -------         DriftCompensation        ------- */
  /* ------------------------------------------------ */
  int nTotalScansJob1 = ceil(PVM_SegmentationSegments) *
                             PVM_SatTransRepetitions   *
                             PVM_NRepetitions;

  ATB_SetDriftCompBaseLevel(jobParameters, 1, nTotalScansJob1, 0);

  DB_MSG(("<--SetAcquisitionParameters"));
}


void PrintTimingInfo(void)
{
  double te UNUSED_PARAM,tr UNUSED_PARAM;

  DB_MSG(("-->PrintTimingInfo"));

  te=(P[0]/2000)+D[3]*1000.0 + DE/1000.0;

  double spacing;
  if(DualEcho == Yes)
    spacing = (D[5]+D[6]+D[7])*1000 +0.016;
  else
    spacing = 0.0;

  tr = (te + P[0]/2000.0 +
      (D[4]+D[1]+D[0])*1000+0.01 + spacing);

  DB_MSG(("te: %f should be: %f diff %f\n",
	  te,PVM_EchoTime,
	  te-PVM_EchoTime));


  DB_MSG(("tr: %f  should be: %f  diff %f\n",tr,PVM_RepetitionTime,
	  tr-PVM_RepetitionTime));


  DB_MSG(("<--PrintTimingInfo"));

}

void CalcGradShapes(void)
{
  int i, size1, size2, size3;

  DB_MSG(("-->CalcGradShapes"));

  /* calculation of GradShape1: readout + spoiler */
  size1 = (int) (RampTime/GradRes + 0.5);
  size2 = (int) ((GradPlateau + SpoilDur)/GradRes + 0.5);
  GradShapeSize1 = 2*size1 + size2;
  ShapeDur1 = GradShapeSize1 * GradRes;
  PARX_change_dims("GradShape1",GradShapeSize1);

  GradShape1[0]=0.0;
  for(i=1;i<size1;i++)
    GradShape1[i] =GradShape1[i-1] + (double) 1/(size1-1);

  for(i=size1;i<size1+size2+1;i++)
    GradShape1[i]=1.0;

  for(i=size1+size2+1;i<size1+size2+size1;i++)
    GradShape1[i] = GradShape1[i-1] -(double) 1/(size1-1);

  GradShape1[size1+size2+size1-1] = 0.0;

  /* calculation of GradShape2: readout */
  size1 = (int) (RampTime/GradRes + 0.5);
  size2 = (int) (GradPlateau/GradRes + 0.5);
  GradShapeSize2 = 2*size1 + size2;
  ShapeDur2 = GradShapeSize2 * GradRes;
  PARX_change_dims("GradShape2",GradShapeSize2);

  GradShape2[0]=0.0;
  for(i=1;i<size1;i++)
    GradShape2[i] =GradShape2[i-1] + (double) 1/(size1-1);

  for(i=size1;i<size1+size2+1;i++)
    GradShape2[i]=1.0;

  for(i=size1+size2+1;i<size1+size2+size1;i++)
    GradShape2[i] = GradShape2[i-1] -(double) 1/(size1-1);

  GradShape2[size1+size2+size1-1] = 0.0;

  /* calculation of GradShape3: rewinding  */
  size3 = (int) ((RewTime - RampTime)/GradRes+0.5);
  GradShapeSize3 = 2*size1 + size3;
  PARX_change_dims("GradShape3",GradShapeSize3);
  ShapeDur3 =  GradShapeSize3 * GradRes;

  GradShape3[0]=0;

  for(i=1;i<size1;i++)
    GradShape3[i] =GradShape3[i-1] + (double) 1/(size1-1);

  for(i=size1;i<size1+size3+1;i++)
    GradShape3[i]= 1.0;

  for(i=size1+size3+1;i<size1+size3+size1;i++)
    GradShape3[i] = GradShape3[i-1] - (double) 1/(size1-1);

  GradShape3[size1+size3+size1-1] = 0.0;

  DB_MSG(("<--CalcGradShapes"));
}
