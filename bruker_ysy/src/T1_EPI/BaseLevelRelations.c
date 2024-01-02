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
  DB_MSG(("-->  SetBaseLevelParam()"));

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

  DB_MSG(("<--  SetBaseLevelParam"));
}



void SetBasicParameters( void )
{
  int index,i,j,k,l,m,n,*x,*sort_array;

  DB_MSG(("-->  SetBasicParameters()"));

  /* NSLICES */

  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  if (SliceGroupRotation == No) {
    Sli_ppg = SliceGroupSize * NIRPoints;
  }
  else {
    Sli_ppg = NIRPoints;
  }

  ATB_SetNSlices( nSlices );

  /* NR */

  ATB_SetNR( PVM_SatTransRepetitions * PVM_NRepetitions );

  if( SliceGroupRotation == No) {
    Rep_ppg = NSLICES / SliceGroupSize;
  }
  else {
    Rep_ppg = NSLICES;
  }

  /* NI */

  ATB_SetNI( nSlices * NIRPoints);  /* total number of images per Inversion */

  /* NA */

  ATB_SetNA( PVM_NAverages );

  /* NAE */

  ATB_SetNAE( 1 );

  /* ACQ_ns */

  ACQ_ns_list_size = PVM_NEchoImages;

  PARX_change_dims( "ACQ_ns_list",1 );
  ACQ_ns_list[0] = 1;
  ParxRelsParRelations("ACQ_ns",Yes);

  /* NECHOES */

  NECHOES = PVM_NEchoImages;

  /* ACQ_obj_order */

  PARX_change_dims("ACQ_obj_order",NI);

  sort_array=(int *)malloc(sizeof(int)*NI);
  if( sort_array == 0)
  {
    UT_ReportError("SetBasicParameters: In malloc");
    return;
  }

  /* (loop structure corresponds to measurement) */

  if(SliceGroupRotation==No) {
    k=0;
    for(l=0;l<nSlices/SliceGroupSize;l++)
    {
      for(m=0;m<NIRPoints;m++)
      {
        for(i=0;i<SliceGroupSize;i++)
        {
          j= PVM_ObjOrderList[l*SliceGroupSize+i];

          index=nSlices*m + j;
          sort_array[index]=k;

          k++;
        }
      }
    }
  }
  else
  {  /* SliceGroupRotation==Yes  */
    k=0;
    for(l=0;l<nSlices/SliceGroupSize;l++)
    {
      for(n=0;n<SliceGroupSize;n++)
      {  /* extra loop for Slice Group Rotation */
        for(m=0;m<NIRPoints/SliceGroupSize;m++)
        {
          for(i=0;i<SliceGroupSize;i++)
          {
            j= PVM_ObjOrderList[l*SliceGroupSize+((i+n) % SliceGroupSize)];

            index=nSlices*(m*SliceGroupSize+i) + j;
            sort_array[index]=k;

            k++;
          }
        }
      }
    }
  }

  x=ACQ_obj_order;
  for(i=0;i<NI;i++)
  {
    for(j=0;j<NI;j++)
    {
      if(sort_array[j]==i) x[i]=j;
    }
  }

  free(sort_array);

  DB_MSG(("<--  SetBasicParameters()"));
}

void SetFrequencyParameters( void )
{
  int nslices,i,j,k,m,index1,index2;
  double *f_array1,*f_array2;

  DB_MSG(("-->  SetFrequencyParameters()"));

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

  nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  ATB_SetAcqO1List( nslices,
                    PVM_ObjOrderList,
                    PVM_SliceOffsetHz );

  ACQ_O1_list_size = nslices * NIRPoints;
  ParxRelsParRelations("ACQ_O1_list_size",Yes);

  ATB_SetAcqO1BList( nslices,
                     PVM_ObjOrderList,
                     PVM_ReadOffsetHz);

  ACQ_O1B_list_size = nslices * NIRPoints;
  ParxRelsParRelations("ACQ_O1B_list_size",Yes);

  f_array1=(double *)malloc(sizeof(double)*NSLICES);
  f_array2=(double *)malloc(sizeof(double)*NSLICES);

  if(( f_array1 == 0) || (f_array2 == 0))
  {
    UT_ReportError("SetBasicParameters: In malloc");
    return;
  }

  for(i=0;i<NSLICES;i++) {
    f_array1[i]= ACQ_O1_list[i]; /* PVM_ObjOrder already considered in lists */
    f_array2[i]= ACQ_O1B_list[i];
  }

  if(SliceGroupRotation == No)
  {
    for(k=0;k<(nslices/SliceGroupSize);k++)
      {
      for(i=0;i<NIRPoints;i++)
      {
        for(j=0;j<SliceGroupSize;j++)
        {
          index1=k*(SliceGroupSize*NIRPoints)+i*SliceGroupSize+j;
          index2=k*SliceGroupSize+j;
          ACQ_O1_list[index1]=f_array1[index2];
          ACQ_O1B_list[index1]=f_array2[index2];
        }
      }
    }
  }
  else
  {  /* SliceGroupRotation == Yes */
    for(k=0;k<(nslices/SliceGroupSize);k++)
    {
      for(m=0;m<SliceGroupSize;m++)
      {
        for(i=0;i<NIRPoints/SliceGroupSize;i++)
        {
          for(j=0;j<SliceGroupSize;j++)
          {
            index1=(k*NIRPoints*SliceGroupSize)+NIRPoints*m+i*SliceGroupSize+j;
            index2=k*SliceGroupSize+((j+m)%SliceGroupSize);
            ACQ_O1_list[index1]=f_array1[index2];
            ACQ_O1B_list[index1]=f_array2[index2];
          }
        }
      }
    }
  }

  free(f_array1);
  free(f_array2);

  if(SliceGroupRotation == No) ACQ_O2_list_size = nslices / SliceGroupSize;
  else ACQ_O2_list_size = nslices;

  ParxRelsParRelations("ACQ_O2_list_size",Yes);
  for(i=0; i<ACQ_O2_list_size; i++) {
    ACQ_O2_list[i] = 0.0;  /* for slice selective: InvSlabOffsetHz */
  }

  DB_MSG(("<--  SetFrequencyParameters()"));
}



void SetGradientParameters( void )
{
  int i;

  DB_MSG(("-->  SetGradientParameters()"));

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
  ACQ_gradient_amplitude[4] = InvSliceSpoiler.ampl;    /* g4 */

  DB_MSG(("<--  SetGradientParameters()"));
}



void SetInfoParameters( void )
{
  int slices, i, spatDim;
  char buffer[20];

  DB_MSG(("-->  SetInfoParameters()"));

  spatDim = PTB_GetSpatDim();

  ACQ_dim = spatDim;
  ParxRelsParRelations("ACQ_dim",Yes);

  ATB_SetAcqDimDesc( 0, spatDim, NULL );

  ATB_SetAcqFov( Spatial, spatDim, PVM_Fov, PVM_AntiAlias );

  ACQ_flip_angle = ExcPul.Flipangle;

  PARX_change_dims("ACQ_echo_time",1); /* 1 */

  if(PVM_SignalType == SignalType_Echo) {
    ACQ_echo_time[0] = InpEchoTime;   /********/
  }
  else {
    ACQ_echo_time[0] = InpEchoTime;
  }

  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;

  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;

  PARX_change_dims("ACQ_inversion_time",NIRPoints);
  for(i=0;i<NIRPoints;i++) {
    ACQ_inversion_time[i] = IROffset + IRSpacing *i;
  }

  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] = ACQ_repetition_time[0]-ACQ_inversion_time[NIRPoints-1];

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
  ACQ_n_movie_frames = NIRPoints;

  PARX_change_dims("ACQ_movie_descr",NIRPoints,20);

  /* ACQ_echo_descr */
  for(i=0;i<NIRPoints;i++) {
    snprintf(buffer,19, "                        ");
    snprintf(buffer,19, "TI %.1f",IROffset+i*IRSpacing);
    strncpy(ACQ_movie_descr[i],buffer,19*sizeof(char));
    ACQ_movie_descr[i][19]='\0';
  }

  strcpy(ACQ_fit_function_name,"t1epic");

  DB_MSG(("<--  SetInfoParameters()"));

}



void SetPpgParameters( void )
{
  double da_prime,db_prime,da,db,dc;

  DB_MSG(("-->  SetPpgParameters()"));

  ATB_SetPulprog("T1_EPI.ppg");

  double riseTime = CFG_GradientRiseTime();

  /* A measurement with SliceGroupRotation==Yes correspond             */
  /* to SliceGroupSize measurements with SliceGroupRotation== No       */
  /* using effective parameter IRSpacing_eff=IR_Spacing*SliceGroupSize */
  /* and NIRPoints_eff=NIRPoints/SliceGroupSize                        */

  if(SliceGroupRotation == No)
    db_prime  = ((IRSpacing-MinIRSpacing)/SliceGroupSize)/1000.0;  /* fill delay for IRspacing */
  else
    db_prime  = ((IRSpacing-MinIRSpacing))/1000.0;

  da_prime = (IROffset0-MinIROffset0) / 1000.0;

  /* Optimization to guarantee shortest possible minTR;
     alternative: leaving out the delay after the last read-out */

  if(da_prime>db_prime)
  {
    dc=db_prime;
    db=0.0;
    da=da_prime-db_prime;
  }
  else
  {
    da=0.0;
    db=db_prime-da_prime;
    dc=da_prime;
  }
  D[0] = db;   /* fill delay for IRspacing */
  D[10] = 2.0/1000.0 + da;  /* fill delay for IROffset, 2.0ms min */
  D[11] = dc;

  D[1] = (RephaseTime - riseTime) / 1000.0;
  if (PVM_SignalType == SignalType_Echo)
    D[2]  = (InpEchoTime/2 - MinTE_right) / 1000.0;
  else
    D[2] = (InpEchoTime - PVM_MinEchoTime) / 1000.0;
  D[4] = riseTime / 1000.0;
  D[5] = SliceSpoiler.dur / 1000.0;
  /* fill delay for repetition time: */
  D[6]  = (PVM_RepetitionTime - PVM_MinRepetitionTime)/1000.0; /* refers to one slice group */
 
  D[7]  = (InpEchoTime/2.0 - MinTE_left) / 1000.0;
  D[9] = (RfcSpoilerDuration-riseTime)*1e-3;
  D[17] = InvSliceSpoiler.dur / 1000.0;

  /* set shaped pulses */
  ATB_SetRFPulse("ExcPul","ACQ_RfShapes[0]","P[0]");
  ATB_SetRFPulse("RefPul","ACQ_RfShapes[1]","P[1]");
  ATB_SetRFPulse("InvPul","ACQ_RfShapes[2]","P[2]");

  /* loop counters */
  L[1] = (PTB_GetSpatDim() > 2) ? PVM_EncMatrix[2] : 1;

  DB_MSG(("<--  SetPpgParameters"));
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

  job0->appendLoop(Sli_ppg);

  job0->appendLoop(Rep_ppg);

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
  double prep, te,part1,part2,part3,part4,part5;
  double tr,ti,offset,full_offset,spacing;
  double trigOutSlice, trigOutVol,IRSpacing_eff;
  int i,NIRPoints_eff;

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

  prep=d(6) + PVM_TaggingModuleTime;
  prep+=0.01 + p(2)/2.0;
  part1= p(2)/2.0 + d(17) + d(4) + d(10);
  part2= 0.030 + 0.010 + trigOutSlice + PVM_FovSatModuleTime +
    PVM_SatTransModuleTime + PVM_FatSupModuleTime+d(11)+d(5)+d(4)+p(0)/2.0;
  /* (1-ExcPul.Rpfac/100); */
  part3=p(0)/2.0+d(4)+d(1)+d(4);
  part4=d(7)+d(9)+d(4)+p(1)+d(9)+d(4);
  part5=d(2)+(PVM_EpiModuleTime)+d(0);


  te=ExcPul.Rpfac/100*p(0)+d(4)+d(1)+d(4)+d(2)+(PVM_EpiEchoDelay);
  if(SliceGroupRotation==Yes)
  {
    NIRPoints_eff=NIRPoints/SliceGroupSize;
    IRSpacing_eff=IRSpacing*SliceGroupSize;
  }
  else
  {
    NIRPoints_eff=NIRPoints;
    IRSpacing_eff=IRSpacing;
  }
  tr=prep+part1+NIRPoints_eff*SliceGroupSize*(part2+part3+part5);
  tr+=PackDel;

  spacing=SliceGroupSize*(part2+part3+part5);

  if (PVM_SignalType == SignalType_Echo) {
    tr=tr+NIRPoints_eff*SliceGroupSize*part4;
    te=te+part4;
    spacing=spacing+SliceGroupSize*part4;
  }
  offset=part1;
  full_offset=part1+part2;

  DB_MSG(("prep: %fms, part1: %fms, part2: %fms, part3: %fms, part4: %fms, part5: %fms",
          prep,part1,part2,part3,part4,part5));
  DB_MSG(("TR: %fms, TR (soll): %fms (diff: %f)",tr, PVM_RepetitionTime, tr-PVM_RepetitionTime));
  DB_MSG(("TE: %fms, TE (soll): %fms (diff: %f)",te, InpEchoTime, te-InpEchoTime));
  DB_MSG(("IROffset:  %fms, IROffset  (soll): %fms (diff: %f)",full_offset, IROffset, full_offset-IROffset));
  DB_MSG(("IRSpacing: %fms, IRSpacing (effective) (soll): %fms (diff: %f)",spacing, IRSpacing_eff, spacing-IRSpacing_eff));

  ti=IROffset;
  for(i=0;i<NIRPoints;i++) {
    DB_MSG(("TI: %fms ",ti));
    ti=ti+IRSpacing;
  }

  DB_MSG(("ExcPulLength %f, rephase %f, spoilerdur %f, refpulse %f  l %f r %f",
          ExcPul.Length, RephaseTime, RfcSpoilerDuration, RefPul.Length, MinTE_left, MinTE_right ));
  DB_MSG(("<--printTiming"));
}

#undef d
#undef p
#endif
