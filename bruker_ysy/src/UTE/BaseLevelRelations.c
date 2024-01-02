/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

static const char resid[] = "$Id$ (C) 2001 - 2011 Bruker BioSpin MRI GmbH";


#define DEBUG		0
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
  ATB_SetNI( nSlices * PVM_NMovieFrames);
 
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
  
  /* NECHOES */
  if(SliceAlter == Yes)
  {
    NECHOES = 2;
  }
  else
  {
    NECHOES = 1;
  }
  
  /* ACQ_obj_order */  
  PARX_change_dims("ACQ_obj_order",NI);
  if( PVM_NMovieFrames > 1)
    SetACQ_obj_orderForMovie();
  else
    ATB_SetAcqObjOrder( nSlices, PVM_ObjOrderList, PVM_NMovieFrames, 1 );
 
  
  DB_MSG(("<--SetBasicParameters"));
}

void SetFrequencyParameters( void )
{
  int i, nslices;

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

  if(SliceAlter == Yes)
  {
    ParxRelsCopyPar("FreqList1","ACQ_O1_list");
    
    int movies = PVM_NMovieFrames;
    ACQ_O1_list_size=2*nslices*movies;
    PARX_change_dims("ACQ_O1_list",2*nslices*movies);
        
    double *aol=ACQ_O1_list;
    double *fol=FreqList1;

    for(int j=0;j<movies;j++)
    {
      for(i=0;i<nslices;i++)
	aol[j*nslices+i] = fol[i];
    }

    for(i=0;i<nslices*movies;i++)
    {
      aol[i+nslices*movies] = -aol[i];
    }

  }
 
  DB_MSG(("<--SetFrequencyParameters"));
}


void SetGradientParameters( void )
{
  double dur1, dur2, dur3;
  double rampT = CFG_GradientRampTime();
  double riseT = CFG_GradientRiseTime();

  DB_MSG(("-->SetGradientParameters"));
      
  ATB_SetAcqGradMatrix("PVM_SliceGeoObj");
   
  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 1.0;
  ACQ_scaling_slice = 1.0;
  
  /*
   *  creation of gradient shapes
   */

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

  if(AcqMode==FID)
  {
    double dur4, dur5, dur6;

    /* time stamps for GradShape2 */
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
  else
  {

    double dur4, dur5, dur6, dur7;
    dur4 = riseT;
    dur5 = dur4 + ReadDephTime-2*riseT;
    dur6 = dur5 + riseT;
    dur7 = dur6 + PVM_AcquisitionTime + ReadSpoilDur;
    GrShape2Time=dur7;

    GradShape2Size = (int)(dur7/GradRes + 0.5);
    PARX_change_dims("GradShape2",GradShape2Size); 

    /* PVM_ppgGradShape2 */
     MRT_MakeRamp(GradShape2,
		 GradShape2Size,
		 dur7,
		 0,
		 dur4,
		 0,
		 -ReadDephGrad/ReadGrad,
		 ramp_lin);
    
     MRT_MakeRamp(GradShape2,
		 GradShape2Size,
		 dur7,
		 dur4,
		 dur5,
		 -ReadDephGrad/ReadGrad,
		 -ReadDephGrad/ReadGrad,
		 ramp_lin);

     MRT_MakeRamp(GradShape2,
		 GradShape2Size,
		 dur7,
		 dur5,
		 dur6,
		 -ReadDephGrad/ReadGrad,
		 1,
		 ramp_lin);

     MRT_MakeRamp(GradShape2,
		 GradShape2Size,
		 dur7,
		 dur6,
		 dur7,
		 1,
		 1,
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
    
  ATB_SetPulprog("UTE.ppg");

  double igwT    = CFG_InterGradientWaitTime();
  int nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  D[0]  = ((PVM_RepetitionTime - PVM_MinRepetitionTime)/nslices + igwT)/1000.0;
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

  
  /* loop counters */
  L[11] = PVM_DummyScans;
  
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
  
  SetAcqO1BList();
 
  DB_MSG(("<--SetPpgParameters"));
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

void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters"));
  
  
  /* ------------------------------------------------ */
  /* -------               Job 0              ------- */
  /* ------------------------------------------------ */
  JOBPARAMETERS(jobParameters);
  JOBDESCRIPTION job0=jobParameters->getOrCreateJob("job0");

  int acqPoints = AcqMode == FID ? PVM_EncMatrix[0]/2 + RampPoints : PVM_EncMatrix[0] + ReadDephPoints;

  job0->initPars(1, acqPoints, PVM_EffSWh);

  job0->appendLoop(NI,LOOP_SETUP);
  job0->appendLoop(NECHOES);
  job0->appendLoop(PVM_DummyScans, LOOP_DUMMIES);
  job0->appendLoop(NA, LOOP_AVERAGE);
  job0->appendLoop(NPro);
  job0->appendLoop(NAE, LOOP_AVERAGE);
  job0->appendLoop(NR);

  job0->setForceShortFilter(1);

  DB_MSG(("<--SetAcquisitionParameters"));
}


void PrintTimingInfo(void)
{
  double te UNUSED_PARAM,tr UNUSED_PARAM;

  DB_MSG(("-->PrintTimingInfo"));

  if(AcqMode == FID)
  {
    te=(P[0]/1000)*ExcPulse1.Rpfac/100+D[12]*1000.0+D[14]*1000.0+D[5]*1000.0;
  }
  else
  {
    te = (P[0]/1000)*ExcPulse1.Rpfac/100+(D[12]+D[5]+D[3])*1000.0+
          PVM_AcquisitionTime/2;
  }

  if(AcqMode == FID)
  {
    tr = (te + P[0]/1000.0*(1-ExcPulse1.Rpfac/100)+
      (D[6]+4*D[1]+D[7]+D[16]+D[17]+D[0])*1000+PVM_AcquisitionTime+0.03);
  }
  else
  {
    tr = (te + P[0]/1000.0*(1-ExcPulse1.Rpfac/100)+
      (D[6]+3*D[1]+D[16]+D[0])*1000+PVM_AcquisitionTime/2+0.03);
  }


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

  if(AcqMode == FID)
  {
    angle = 2 * pi;
  }
  else
  {
    angle = pi;
  }

  for(i=0; i<ploop; i++)
  {
    r[i]=cos(angle*i/ploop)*gr;
    p[i]=sin(angle*i/ploop)*gr;
  }
  return 0;
}

void SetAcqO1BList(void)
{
  DB_MSG(("-->SetAcqO1BList"));

  if(AcqMode == ECHO)
  {
    /* frequency offsets */
    ACQ_O1B_list_size = NPro;
    ParxRelsParRelations("ACQ_O1B_list_size", Yes);

    /* initialise with zero */    
    memset(ACQ_O1B_list, 0, NPro * sizeof(double));  
  
    double *gradR = GradAmpR, *gradP = GradAmpP;
    double *offList = ACQ_O1B_list;

    double gcc=PVM_GradCalConst,
      ero=PVM_EffReadOffset[0],
      epo=PVM_EffPhase1Offset[0],
      readgrad=ReadGrad/100,
      phasegrad=PhaseGrad/100;
    int npro=NPro;

    for (int i = 0; i < npro; i++)
    {
      offList[i] = gcc * (ero* gradR[i]*readgrad + epo* gradP[i]*phasegrad);
    }

    PARX_change_dims("PhaseList",npro);

    double *phl = PhaseList;
    double dval= 1./(PVM_EffSWh*PVM_AntiAlias[0]) *360.0*
      static_cast<double>(ReadDephPoints+PVM_EncMatrix[0]/2);

    for (int i = 0; i < npro; i++) phl[i] = offList[i]*dval;
  }
  else
  {
    ACQ_O1B_list_size = 1;
    ParxRelsParRelations("ACQ_O1B_list_size", Yes);
    ACQ_O1B_list[0] = 0.0; 
    PARX_change_dims("PhaseList",1);
    PhaseList[0] = 0.0;
  }

  DB_MSG(("<--SetAcqO1BList"));
}


void SetACQ_obj_orderForMovie(void)
{
  int k,j,i;
  int nSlices;
  DB_MSG(("-->SetACQ_obj_orderForMovie\n"));

  j=0;
  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  while(j< PVM_NMovieFrames)
  {
    for(i=0;i<nSlices;i++)
    {
      k=j*nSlices+i;
      ACQ_obj_order[k]= PVM_ObjOrderList[i]*PVM_NMovieFrames +j;
    }
    j=j+1;
  }

  DB_MSG(("<--SetACQ_obj_orderForMovie\n"));
}
