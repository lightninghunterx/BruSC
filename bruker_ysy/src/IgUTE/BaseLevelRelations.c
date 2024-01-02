/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2013
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

static const char resid[] = "$Id$ (C) 2001 - 2011 Bruker BioSpin MRI GmbH";


#define DEBUG		1
#define DB_MODULE	0
#define DB_LINE_NR	0


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
  ATB_SetFatSupBaselevel();
  ATB_SetSatTransBaseLevel();
  ATB_SetFovSatBaseLevel();
  
  
  PrintTimingInfo();

  DB_MSG(("<--SetBaseLevelParam"));
  

}

void SetBasicParameters( void )
{
  DB_MSG(("-->SetBasicParameters"));
  
  /* NSLICES */  
  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  ATB_SetNSlices( nSlices );

  /* NR */  
  ATB_SetNR( PVM_SatTransRepetitions * PVM_NRepetitions );
  
  /* NI */
  ATB_SetNI( nSlices );
 
  /* AVERAGING */
  ATB_SetNA( PVM_NAverages  );
  ATB_SetNAE( 1 );

  /* NECHOES */
  NECHOES = 1;
  
  /* ACQ_obj_order */  
  PARX_change_dims("ACQ_obj_order",NI);  
  ATB_SetAcqObjOrder( nSlices, PVM_ObjOrderList, 1, 1);
  
  DB_MSG(("<--SetBasicParameters"));
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
  
 
  DB_MSG(("<--SetFrequencyParameters"));
}


void SetGradientParameters( void )
{
  DB_MSG(("-->SetGradientParameters")); 
  
  ATB_SetAcqGradMatrix("PVM_SliceGeoObj");
   
  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 1.0;
  ACQ_scaling_slice = 1.0;
   
  /*
   *  creation of gradient shapes
   */

  double dur1, dur2, dur3;
  double dur4, dur5, dur6;
  double rampT = CFG_GradientRampTime();
  double riseT = CFG_GradientRiseTime();

  /* time stamps for GradShape1 */
  dur1 = riseT +  ExcPulse1.Length;
  dur2 = dur1 + GrShapeT1 + GrShapeT2;
  dur3 = dur2 + GrShapeT3; 

  /* gradient shape resolution in ms */
  GradShape1Size = (int)(GrShape1Time/GradRes + 0.5);
  PARX_change_dims("GradShape1",GradShape1Size); 

  /* PVM_ppgGradShape1 */
  MRT_MakeRamp(GradShape1,
	       GradShape1Size,
	       GrShape1Time,
	       0.0,
	       riseT,
               0.0,
	       ExcSliceGrad,
	       ramp_lin);

  MRT_MakeRamp(GradShape1,
	       GradShape1Size,
	       GrShape1Time,
	       riseT,
	       dur1,
	       ExcSliceGrad,
	       ExcSliceGrad,
	       ramp_lin);

  MRT_MakeRamp(GradShape1,
	       GradShape1Size,
	       GrShape1Time,
	       dur1,
	       dur2,
	       ExcSliceGrad,
	       GrShapeG2,
	       ramp_lin);

  MRT_MakeRamp(GradShape1,
	       GradShape1Size,
	       GrShape1Time,
	       dur2,
	       dur3,
	       GrShapeG2,
	       GrShapeG2,
	       ramp_lin);

  MRT_MakeRamp(GradShape1,
	       GradShape1Size,
	       GrShape1Time,
	       dur3,
	       GrShape1Time,
	       GrShapeG2,
               0,
	       ramp_lin);

  /* time stamps for GradShape2 */
  if(AddCentralPoints > 0)
  {
    double dur7;
    dur7 = AddCentralPoints * 1000/(PVM_EffSWh*PVM_AntiAlias[0] );
    dur4 = dur7 + rampT;
    dur5 = dur4 + PVM_AcquisitionTime + RampCompTime + ReadSpoilDur;
    dur6 = dur5 + riseT;
    GrShape2Time=dur6;
  
    GradShape2Size = (int)(dur6/GradRes + 0.5);
    PARX_change_dims("GradShape2",GradShape2Size); 
 
    /* PVM_ppgGradShape2 */
    MRT_MakeRamp(GradShape2,
		 GradShape2Size,
		 dur6,
		 0,
		 dur7,
		 0.0,
		 0,
		 ramp_lin);


    MRT_MakeRamp(GradShape2,
		 GradShape2Size,
		 dur6,
		 dur7,
		 dur4,
		 0.0,
		 1,
		 ramp_lin);
    
    MRT_MakeRamp(GradShape2,
		 GradShape2Size,
		 dur6,
		 dur4,
		 dur5,
		 1,
		 1,
		 ramp_lin);
    
    MRT_MakeRamp(GradShape2,
		 GradShape2Size,
		 dur6,
		 dur5,
		 dur6,
		 1,
		 0,
		 ramp_lin);
  }
  else
  {
    dur4 = rampT;
    dur5 = dur4 + PVM_AcquisitionTime + RampCompTime + ReadSpoilDur;
    dur6 = dur5 + riseT;
    GrShape2Time=dur6;
  
    GradShape2Size = (int)(dur6/GradRes + 0.5);
    PARX_change_dims("GradShape2",GradShape2Size); 
 
    /* PVM_ppgGradShape2 */
    MRT_MakeRamp(GradShape2,
		 GradShape2Size,
		 dur6,
		 0,
		 dur4,
		 0.0,
		 1,
		 ramp_lin);

    MRT_MakeRamp(GradShape2,
		 GradShape2Size,
		 dur6,
		 dur4,
		 dur5,
		 1,
		 1,
		 ramp_lin);

    MRT_MakeRamp(GradShape2,
		 GradShape2Size,
		 dur6,
		 dur5,
		 dur6,
		 1,
		 0,
		 ramp_lin);
    
  }
 

  /* print shapes for plotting */
  PRINTARRAY("/tmp/ute_shape1",GradShape1, GradShape1Size);
  PRINTARRAY("/tmp/ute_shape2",GradShape2, GradShape2Size);

  /* gradient amplitudes */
  ACQ_gradient_amplitude[0] = ExcSliceGrad;
  ACQ_gradient_amplitude[1] = ReadGrad;
  ACQ_gradient_amplitude[2] = PhaseGrad;
  ACQ_gradient_amplitude[3] = -ReadDephGrad;
  ACQ_gradient_amplitude[4] = -PhaseDephGrad;
  ACQ_gradient_amplitude[5] = SliceSpoiler.ampl;

  
  DB_MSG(("<--SetGradientParameters"));

}

void SetPpgParameters(void)
{
  DB_MSG(("-->SetPpgParameters"));
    
  ATB_SetPulprog("IgUTE.ppg");

  int slices  = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
 
  D[0]  = (PVM_RepetitionTime - PVM_MinRepetitionTime)/(slices * 1000.0) + 0.01/1000.0;
  D[1]  = CFG_GradientRiseTime()/1000.0;
  D[3]  = ReadDephTime / 1000.0;
  D[5]  = TeFillDelay/1000.0;
  D[6]  = SliceSpoiler.dur / 1000.0;
  D[7]  = CFG_GradientRampTime()/1000.0;
  D[11] = GrShape1Time/1000.0; 
  D[12] = (GrShapeT1 + 2*GrShapeT2+GrShapeT3)/1000.0;
  D[15] = GrShape2Time/1000.0;
  D[16] = ReadSpoilDur/1000.0;
  D[17] = RampCompTime/1000.0;
  D[18] = AddCentralPoints * 1000/(PVM_EffSWh*PVM_AntiAlias[0]);
  
  /* loop counters */
  L[11] = PVM_DummyScans;
  L[12] = Oversampling;
  
  /* set shaped pulses, in this method ACQ_RfShapes[0] is used           
     the pulse duration is stored in baselevel parameter P[0] */
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");
  
  /* set radial projections */
  PARX_change_dims("GradAmpR",NPro);
  PARX_change_dims("GradAmpP",NPro);

  SetProj(GradAmpR,
	  GradAmpP,
	  NPro,
	  1);

  DB_MSG(("<--SetPpgParameters"));
}

void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters"));

  /* Job 0 */
  JOBPARAMETERS(jobParameters);
  JOBDESCRIPTION job0=jobParameters->getOrCreateJob("job0");

  int acqPoints = PVM_EncMatrix[0]/2 + RampPoints + AddCentralPoints;
  job0->initPars(1, acqPoints, PVM_EffSWh );

  job0->appendLoop(NSLICES);
  job0->appendLoop(PVM_DummyScans, LOOP_DUMMIES);
  job0->appendLoop(NA, LOOP_AVERAGE, LOOP_SETUP);
  job0->appendLoop(NPro);
  job0->appendLoop(Oversampling);  
  job0->appendLoop(NR);
  
  job0->setForceShortFilter(1);
  
  /* Navigator */
  JOBDESCRIPTION navigator;
  int nSlices=GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  int nTotal = NPro * nSlices * Oversampling * NA *
               PVM_SatTransRepetitions * PVM_NRepetitions;

  navigator = ATB_SetNavBaseLevel(jobParameters, 5, nSlices*NA, nTotal, PVM_DummyScans*nSlices*NA);

  ATB_LinkJobToDriftCompModule(jobParameters, navigator);

  DB_MSG(("<--SetAcquisitionParameters"));
}


void SetInfoParameters( void )
{
  int slices, i, spatDim;
  
  DB_MSG(("-->SetInfoParameters"));
  
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
  
  PARX_change_dims("ACQ_echo_time",1);

  ACQ_echo_time[0] = PVM_EchoTime;
  
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
 
  /* frames come inside the slice loop - they need to be declared
     as echo images: */
  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();

  if(PVM_NMovieFrames > 1)
  {
    ACQ_n_echo_images = PVM_NMovieFrames;
    PARX_change_dims("ACQ_echo_descr", PVM_NMovieFrames, 20);
    for(i=0; i<PVM_NMovieFrames; i++)
    {
      sprintf(ACQ_echo_descr[i], "frame %d",i+1 );
    }
  }
  
  DB_MSG(("<--SetInfoParameters"));
  
}


void PrintTimingInfo(void)
{
  double te,tr;

  DB_MSG(("-->PrintTimingInfo"));

  te=(P[0]/1000)*ExcPulse1.Rpfac/100+D[12]*1000.0+D[5]*1000.0;
  
  tr = 0.03 + 0.01 +(D[6]+D[1]+D[1])*1000+ P[0]/1000.0*(1-ExcPulse1.Rpfac/100)
    + te + (D[7]+D[1]+D[16]+D[0])*1000
    + (PVM_AcquisitionTime+D[17]*1000.0) /* PVM_AcquisitionTime+D17 = aq_job0*/
    + PVM_NavigatorModuleTime;

  if (PVM_NavOnOff == On)
    tr+=0.04;

  
  tr *= NSLICES; 

  DB_MSG(("te: %f should be: %f diff %f\n",
	  te,PVM_EchoTime,
	  te-PVM_EchoTime));


  DB_MSG(("tr: %f  should be: %f  diff %f\n",tr,PVM_RepetitionTime,
	  tr-PVM_RepetitionTime));


  DB_MSG(("<--PrintTimingInfo"));
  
}

int SetProj( double *r,
	     double *p,
	     const int ploop,
	     const double gr)
{
  int i;
  double pi,angle;

  pi=M_PI;

  angle = 2 * pi;
  
  for(i=0; i<ploop; i++)
  {
    r[i]=cos(angle*i/ploop)*gr;
    p[i]=sin(angle*i/ploop)*gr;
  }
  return 0;
}



