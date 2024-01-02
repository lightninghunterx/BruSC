/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2020
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2001-2009 Bruker BioSpin MRI GmbH";

#define DEBUG           0
#define DB_MODULE       0
#define DB_LINE_NR      0

#include "method.h"

void SetBaseLevelParam( void )
{
  DB_MSG(("-->SetBaseLevelParam"));

  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  SetBasicParameters();
  SetFrequencyParameters();
  SetPpgParameters();
  SetAcquisitionParameters();
  SetGradientParameters();
  ATB_SetReceiverGains();

  /* setting baselevel parameters used by modules */
  ATB_SetFatSupBaselevel();
  ATB_SetSatTransBaseLevel();
  ATB_SetFovSatBaseLevel();
  ATB_SetFlowSaturationBaseLevel();
  ATB_SetTaggingBaseLevel();

  if (InversionOnOff == On)
  {
    ATB_SetSelIrBaseLevel( nSlices );

    if (InversionMode == NonSelInversion)
    {
      PVM_SelIrGrad=0.0;
      PVM_SelIrGradAmp=0.0;
      PARX_change_dims( "PVM_SelIrFL",1 );
      PVM_SelIrFL[0]=0.0;
    }
  }

  SetInfoParameters();

  printTiming();
    
  DB_MSG(("<--SetBaseLevelParam"));
}


/* Toolboxes referenced in this file: ATB, GTB, PTB, STB, UT */
void SetBasicParameters( void )
{
  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  DB_MSG(("-->SetBasicParameters"));

  /* NSLICES */
  ATB_SetNSlices( nSlices );
 
  /* NR */
  ATB_SetNR( PVM_SatTransRepetitions * PVM_NRepetitions * PVM_EvolutionCycles );

  /* NI */
  ATB_SetNI( nSlices * PVM_NMovieFrames);
 
  /* NA */
  ATB_SetNA( 1 );

  /* NAE */
  ATB_SetNAE( PVM_NAverages);

  /* NECHOES */
  NECHOES = PVM_NMovieFrames;
  
  /* ACQ_obj_order */
  PARX_change_dims("ACQ_obj_order",NI);
  ATB_SetAcqObjOrder( nSlices, PVM_ObjOrderList, PVM_NMovieFrames, 1 );

  DB_MSG(("<--SetBasicParameters"));
}

void SetFrequencyParameters( void )
{
  DB_MSG(("-->SetFrequencyParameters"));

  int nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

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

  int spatDim = PTB_GetSpatDim();

  if (SegmentSliceOrder == Alternate)
    ACQ_rare_factor  = SegmentSize;
  else
    ACQ_rare_factor  = PVM_EncMatrix[1];
 
  PARX_change_dims("ACQ_phase_encoding_mode", spatDim );
  PARX_change_dims("ACQ_phase_enc_start", spatDim );

  switch(spatDim)
  {
    case 3:
      ACQ_phase_encoding_mode[2] = User_Defined_Encoding;
      ACQ_phase_enc_start[2]     = -1;                   /* set, but not used */
      /* no break */
    case 2:
      ACQ_phase_encoding_mode[1] = User_Defined_Encoding;;
      ACQ_phase_enc_start[1]     = -1.0; /* set, but no used */
      ACQ_spatial_size_1 = PVM_EncGenTotalSteps;
      ACQ_spatial_size_2 = PVM_EncGenTotalSteps;
      ParxRelsCopyPar("ACQ_spatial_phase_1","PVM_EncGenValues1");
      ParxRelsCopyPar("ACQ_spatial_phase_2","PVM_EncGenValues2");
      /* no break */
    default:
      ACQ_phase_encoding_mode[0] = Read;
      ACQ_phase_enc_start[0]     = -1;
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
  ACQ_gradient_amplitude[6] =  ReadSpoiler.ampl;
  ACQ_gradient_amplitude[7] = -Rew2DGrad;
  ACQ_gradient_amplitude[8] =  Rew3DGrad;
  ACQ_gradient_amplitude[9] =  SliceSpoiler.ampl;
  ACQ_gradient_amplitude[10]=  SegSpoiler.ampl;
  
  DB_MSG(("<--SetGradientParameters"));
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

 /* ACQ_size */
  ATB_SetAcqSize( Spatial, spatDim, PVM_EncMatrix, NULL, No );

  ATB_SetAcqFov( Spatial, spatDim, PVM_Fov, PVM_AntiAlias );
 
  ACQ_flip_angle = ExcPulse1.Flipangle;
  
  PARX_change_dims("ACQ_echo_time",1);
  ACQ_echo_time[0] = PVM_EchoTime;
  
  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = EchoRepTime;
  
  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = SegmentRepTime;
  
  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] = PVM_RepetitionTime - ExcPulse1.Length;

  /* calculation of ACQ_time_points */
  ATB_EvolutionSetTimePoints(PVM_NRepetitions, OneRepTime);

  /* calculation of movie frame times after trigger */
  float thisdelay = 0.0;

  if (AcqTrigger==Yes)
    thisdelay     = ModulesTimeToKzero;

  ACQ_n_echo_images = PVM_NMovieFrames;

  if(ACQ_n_echo_images > 1)
  {
    PARX_change_dims("ACQ_echo_descr",ACQ_n_echo_images,20);
    
    if (InversionOnOff == Off)
     for(i=0; i<ACQ_n_echo_images; i++)
       sprintf(ACQ_echo_descr[i], "%.2f ms", thisdelay + i*FrameRepTime);
  }

  /*calculation of inversion times for the movie frames*/
  if (InversionOnOff==On)
  {
    PARX_change_dims("ACQ_inversion_time", ACQ_n_echo_images);
    for(i=0; i<ACQ_n_echo_images; i++)
      ACQ_inversion_time[i] = PVM_InversionTime + i*FrameRepTime;
  }
  else
  {
    PARX_change_dims("ACQ_inversion_time", 1);
    ACQ_inversion_time[0] = PVM_InversionTime;
  }


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
    ACQ_contrast.volume        = 0.0 ;
    ACQ_contrast.dose          = 0.0 ;
    ACQ_contrast.route[0]      = '\0';
    ACQ_contrast.start_time[0] = '\0';
    ACQ_contrast.stop_time[0]  = '\0';
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


void SetPpgParameters( void )
{
  double riseTime  = CFG_GradientRiseTime();
  double rampTime  = CFG_GradientRampTime() + CFG_InterGradientWaitTime();

  double gDwell    = MAX_OF(CFG_GradientShapeResolution(),0.002);
  int nslices      = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  DB_MSG(("-->SetPpgParameters"));

  ATB_SetPulprog("SegFLASH.ppg");

  D[0]  = 1e-3 * ( CFG_InterGradientWaitTime() + 0.01 );

  D[1]  = 1e-3 * (  SegmentRepTime - MinSegmentRepTime);
  if (SegmentSliceOrder == Alternate)
    D[1] = D[1]/nslices;


  // Delay for TEFillDelay: Differs for FlowComp On/off:
  // for 'Off' add one ramp down
  // for 'On' ramp down is part of shape
  if(FlowCompOnOff == Off)
    D[2]  = 1e-3 * ( TeFillDelay + rampTime);
  else 
    D[2]  = 1e-3 *   TeFillDelay;

  D[3]  = 1e-3 *   riseTime;
  D[4]  = 1e-3 *   rampTime;
  D[5]  = PVM_NMovieFrames > 1 ? 1e-3*(FrameRepTime - EchoRepTime * SegmentSize): 0.0;
  D[6]  = 1e-3 *   SliceSpoiler.dur;
  // rounding error comes up as we wanted to get EncGradDur on GradientRasterTime
  // this happens twice in LongTEScan (2 shapes)
  D[7]  = FlowCompLongTEScan == Yes ? 2*RdVar/1000.0 : RdVar/1000.0 ;

  D[9]  = 1e-3 *   FixDelay_1;

  // Delay D[10] differs for FlowComp On/off:
  // for 'Off' it is EncGradDur
  // for 'On' the start of r,p,s-lobes can be shifted relative to each other.
  // Then the shape length is not EncGradDur anymore
  if(FlowCompOnOff == Off)
    D[10] = 1e-3 * EncGradDur;
  else
    D[10] = 1e-3 * ShapeSize*gDwell;

  D[11] = 1e-3 *   RewGradDur;
  D[12] = 1e-3 * ( ReadSpoiler.dur - RewGradDur );
  D[13] = 1e-3 *   SegSpoiler.dur;

  D[14] = 1e-3* PVM_DriftCompModuleTime;

  /* set shaped pulses, in this method ACQ_RfShapes[0] is used           
     the pulse duration is stored in baselevel parameter P[0] */
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");
 
  L[1] = SegmentSize;
  L[2] = PVM_NMovieFrames;

  if (SegmentSliceOrder == Alternate)
  {
    L[3] = NSLICES;
    L[4] = PVM_EncGenTotalSteps / PVM_EncGenLoopOuter / SegmentSize;
  }
  else
  {
    L[3] = PVM_EncGenTotalSteps / PVM_EncGenLoopOuter / SegmentSize;
    L[4] = NSLICES;
  }

  L[5] = PVM_EncGenLoopOuter;

  ParxRelsParRelations("L",Yes);

  DB_MSG(("<--SetPpgParameters"));
}

void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters"));

  int nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  JOBPARAMETERS(jobParameters);

  /* ------------------------------------------------ */
  /* -------               Job 0              ------- */
  /* ------------------------------------------------ */
  JOBDESCRIPTION job0=jobParameters->getOrCreateJob("job0");
  job0->initPars(1, PVM_EncMatrix[0], PVM_EffSWh * PVM_AntiAlias[0]);

  //SegmentSize
  job0->appendLoop( L[1] );
  //MovieFrames
  job0->appendLoop( L[2], SegmentSliceOrder==Serial ? LOOP_SETUP : LOOP_NONE );
  //for alternate: next slice,   for serial: next segment
  job0->appendLoop( L[3], SegmentSliceOrder==Alternate ? LOOP_SETUP : LOOP_NONE);
  //for alternate: next segment, for serial: next slice
  job0->appendLoop( L[4] );
  //3D-loop
  job0->appendLoop( L[5] );
  //only motion averaging NAE
  job0->appendLoop( NAE, LOOP_AVERAGE );
  //saturation transfer, repetition and evolution.
  job0->appendLoop( NR );
 

  /* ------------------------------------------------ */
  /* -------         DriftCompensation        ------- */
  /* ------------------------------------------------ */

  int nRepetitions    = PVM_SatTransRepetitions * PVM_NRepetitions ;
  //PVM_EvolutionCycles keeps its number even if module's switched off. Therefore:
  nRepetitions *= (PVM_EvolutionOnOff == On) ? PVM_EvolutionCycles : 1;

  int nScansJobDrift  = ( PVM_EncGenTotalSteps / PVM_EncGenLoopOuter / SegmentSize ) * PVM_NAverages * nRepetitions;

  if (SegmentSliceOrder == Serial)
    nScansJobDrift *= nSlices;

  if (PTB_GetSpatDim() == 3)
    nScansJobDrift *= PVM_EncGenLoopOuter;

  ATB_SetDriftCompBaseLevel(jobParameters, 1, nScansJobDrift , 0 );

  DB_MSG(("<--SetAcquisitionParameters"));
}


void printTiming(void)
{
#if DEBUG && !defined(NDEBUG)
#define d(n) (D[n]*1e3)
#define p(n) (P[n]*1e-3)

  DB_MSG(("-->printTiming"));

  double te=0.0,  ert=0.0, sd=0.0, srt=0.0, msrt=0.0, aqq=PVM_AcquisitionTime;
  double tst=0.0, imd=0.0, ti=0.0;

  /* Check Echo Time te */
  te = p(0)/2 + d(4);

  if (FlowCompOnOff==Off)
  {
    te += d(10) + d(2);
  }
  else
  {
    if (FlowCompLongTEScan == No)
      te += d(10);
    else
      te += d(10) + d(2) + d(10);

    te += d(7);
  }

  te += d(3) + aqq/2;

  /* Check Echo Repetition Time ert */
  ert = d(9) + d(6) + d(3) + p(0)/2
        + te
        +aqq/2 + d(11) + d(12) + d(3) + d(0);

  /* Check Segment Duration sd */
  sd = ert * L[1] * L[2] + (L[2]-1) * d(5);

  /* Check Inversion Module Duration imd*/
  if (InversionOnOff == On)
  {
    imd = 0.01 + 1e3  * ( PVM_SelIrD4 )                
               + 1e-3 *   PVM_SelIrP0 
               + 1e3  * ( PVM_SelIrD2 + PVM_SelIrD4 + PVM_SelIrD1 );
  }

  /* Check Inversion Time ti*/
  if (InversionOnOff == On)
    ti = 0.5e-3*PVM_SelIrP0 + 1000*( PVM_SelIrD2 + PVM_SelIrD4 + PVM_SelIrD1) + ModulesTimeToKzero ;

  /* Check Minimum Segment Repetition Time msrt */
  msrt =        0.03                    +
                AcqTriggerModuleTime    +
                InvTriggerModuleTime    +
                PVM_SelIrModuleTime     +
                PVM_TaggingModuleTime   +
                PVM_InFlowSatModuleTime +
                PVM_FovSatModuleTime    +
                PVM_SatTransModuleTime  +
                PVM_FatSupModuleTime    +
                sd + d(13) + d(4) ;

  if (SegmentSliceOrder == Alternate)
    msrt *= NSLICES;

  msrt = msrt + PVM_DriftCompModuleTime;

  /* Check Segment Repetition Time srt */

  if (SegmentSliceOrder == Alternate)
    srt = d(1)*NSLICES + msrt;
  else 
    srt = d(1) + msrt;

  /* Check Total Scan Time tst */
  tst =  ( srt * ( PVM_EncMatrix[1] / SegmentSize ) );

  if (SegmentSliceOrder == Serial)
  {
    tst += PVM_DriftCompModuleTime;
    tst *= NSLICES;
  }

  tst *=  NAE;

  if (PVM_SpatDim > 2) tst*=  PVM_EncMatrix[2];
  tst = (tst * PVM_EvolutionCycles + PVM_EvolutionModuleTime) * PVM_SatTransRepetitions * PVM_NRepetitions; 

 /* Print results */
  DB_MSG(("   EchoTime     - used: %8.6lf ms  expected: %8.6lf ms: Diff: %.6lf ms",
          te, PVM_EchoTime, te  - PVM_EchoTime       ));
  DB_MSG(("   EchoRepTime  - used: %8.6lf ms, expected: %8.6lf ms. Diff: %.6lf ms",
          ert,  EchoRepTime, ert - EchoRepTime        ));
  DB_MSG(("   SegmDuration - used: %8.6lf ms, expected: %8.6lf ms. Diff %.6lf ms",
          sd, SegmentDuration, sd  - SegmentDuration    ));
  if (InversionOnOff == On)
  {
    DB_MSG(("   InvModuleTime- used: %8.6lf ms, expected: %8.6lf ms. Diff %.6lf ms",
            imd, PVM_SelIrModuleTime, imd - PVM_SelIrModuleTime)); 
    DB_MSG(("   InvTime      - used: %8.6lf ms, expected: %8.6lf ms. Diff %.6lf ms",
            ti,   PVM_InversionTime, ti  - PVM_InversionTime));
  }

  DB_MSG(("   MinRepTime   - used %8.6lf ms, expected: %8.6lf ms. Diff %.6lf ms",
          msrt, MinSegmentRepTime, msrt -MinSegmentRepTime  ));
  DB_MSG(("   RepTime      - used: %8.6lf ms, expected: %8.6lf ms. Diff: %.6lf ms",
          srt, SegmentRepTime, srt - SegmentRepTime     ));
  DB_MSG(("   ScanTime     - used: %8.3lf ms, expected: %8.3lf ms. Diff: %.3lf ms",
          tst, PVM_ScanTime, tst - PVM_ScanTime       ));

  check_0th_moment();
  check_1st_moment();

  DB_MSG(("<--printTiming"));
#undef d
#undef p
#endif
}


void check_0th_moment(void)
{
#if DEBUG && !defined(NDEBUG)

  if (FlowCompOnOff==Off)
    return;

  double rise    = CFG_GradientRiseTime();
  double echoPos = PVM_AcquisitionTime*PVM_EchoPosition/100.;

  double mom0_fc_ro, mom0_ro, mom0_fc_sl, mom0_sl;
  double mom0_l1r, mom0_l2r, mom0_l1s, mom0_l2s;
  extern double L1r, L2r, L1s, L2s;

  // calc 0th moment of readout                      - mom0_ro  :
  mom0_ro = (0.5*rise+echoPos) * ReadGrad;
  // calc 0th moment of slice gradient               - mom0_sl  :
  mom0_sl = (EffPulseDur + 0.5*rise)*ExcSliceGrad;
 
  // calc 0th moment of 1st fc lobe in read (l1r)  - mom0_l1r :
  // calc 0th moment of 2nd fc lobe in read (l1r)  - mom0_l2r :
  // calc fc-moment in read as sum of l1r & l2r    - mom_fc_ro:
  mom0_l1r   = (L1r-rise) * ReadFcGrad[0];
  mom0_l2r   = (L2r-rise) * ReadFcGrad[1];
  mom0_fc_ro = mom0_l1r + mom0_l2r;

  // calc 0th moment of 1st fc lobe in slice (l1s) - mom0_l1s :
  // calc 0th moment of 2nd fc lobe in slice (l2s) - mom0_l2s :
  // calc fc-moment in slice as sum of l1 & l2     - mom_fc_sl:
  mom0_l1s   = (L1s-rise) * ExcSliceFcGrad[0];
  mom0_l2s   = (L2s-rise) * ExcSliceFcGrad[1];
  mom0_fc_sl =  mom0_l1s + mom0_l2s;

  DB_MSG(("   M0 (SliceFc) - used: %8.6lf  expected: %8.6lf : Diff: %.6lf",
          mom0_fc_sl, -mom0_sl, fabs(mom0_fc_sl)-fabs(mom0_sl)) );

  DB_MSG(("   M0 (ReadFc)  - used: %8.6lf  expected: %8.6lf : Diff: %.6lf",
          mom0_fc_ro, -mom0_ro, fabs(mom0_fc_ro)-fabs(mom0_ro)) );
#endif
}


void check_1st_moment(void)
{
#if DEBUG && !defined(NDEBUG)

  if (FlowCompOnOff==Off)
    return;

  /*calculus taken from Bernstein's Handbook of MRI Pulse Sequences*/
  double rise    = CFG_GradientRiseTime();
  double echoPos = PVM_AcquisitionTime*PVM_EchoPosition/100.;
  extern double L1r, L2r, L1s, L2s;

  /* Read direction:*/
  if (FlowCompDirection[0])
  {
    // calc 1st moment of 1st read fc lobe (l1r)       - mom1_l1r -
    // tc_: time of trapez center
    double mom0_l1r   = (L1r-rise) * ReadFcGrad[0];
    double tc_l1r     = 0.5 * L1r;
    double mom1_l1r   = mom0_l1r * tc_l1r;

    // calc 1st moment of 2nd read fc lobe (l2r)       - mom1_l2r -
    double mom0_l2r   = (L2r-rise) * ReadFcGrad[1];
    double tc_l2r     = L1r + 0.5*L2r;
    double mom1_l2r   = mom0_l2r * tc_l2r;

    //calc 1st moment of fc-lobes in read as sum of lobes:
    double mom1_fc_ro = mom1_l1r + mom1_l2r;

    // calculate 1st moment of readout gradient        - mom1_ro -
    // as sum of moments rampup (_ru) & plateau (_pl)
    // ts : start time
    double mom0_ru_ro = 0.5 * rise* ReadGrad;
    double ts_ru_ro   = L1r + L2r;
    double m1_ru_ro   = rise*rise/3 * ReadGrad;
    double mom1_ru_ro = mom0_ru_ro*ts_ru_ro + m1_ru_ro;

    double mom0_pl_ro = echoPos * ReadGrad;
    double ts_pl_ro   = ts_ru_ro + rise;
    double m1_pl_ro   = 0.5 * echoPos * echoPos * ReadGrad;
    double mom1_pl_ro = mom0_pl_ro*ts_pl_ro + m1_pl_ro;

    double mom1_ro    = mom1_ru_ro + mom1_pl_ro;

    DB_MSG(("   M1 (ReadFc)  - used: %8.6lf  expected: %8.6lf : Difference: %8.6lf",
            mom1_fc_ro, mom1_ro, fabs(mom1_fc_ro)-fabs(mom1_ro) ));
  }

  /* Slice direction:*/
  if (FlowCompDirection[2])
  {
    // calc 1st moment of 1st slice fc lobe  (l1s)     - mom1_l1s -
    // as mom1_l1s = mom0_l1s*tc_l1s:
    // tc: time of trapez center
    double mom0_l1s   = (L1s - rise) * ExcSliceFcGrad[0];
    double tc_l1s     = 0.5 * L1s;
    double mom1_l1s   = mom0_l1s * tc_l1s;

    // calc 1st moment of 2nd slice fc lobe  (l2s)     - mom1_l2s -
    double mom0_l2s   = (L2s - rise) * ExcSliceFcGrad[1];
    double tc_l2s     = L1s + 0.5*L2s;
    double mom1_l2s   = mom0_l2s * tc_l2s;

    //calc 1st moment of fc-lobes in read as sum of lobes:
    double mom1_fc_sl = mom1_l1s + mom1_l2s;


    // calculate 1st moment of slice gradient          - mom1_sl
    // as sum of moments' plateau (_pl) & rampdown (_rd)
    // ts :start time
    double mom0_rd_sl = 0.5 * rise * ExcSliceGrad;
    double ts_rd_sl   = 0.0;
    //as we look backwards onto slice grad with ts_rd_sl, ramp
    //down in actually up
    double m1_rd_sl   = rise * rise * ExcSliceGrad/3;
    double mom1_rd_sl = mom0_rd_sl*ts_rd_sl + m1_rd_sl;

    double mom0_pl_sl = EffPulseDur * ExcSliceGrad;
    double ts_pl_sl   = rise;
    double m1_pl_sl   = 0.5 * EffPulseDur * EffPulseDur * ExcSliceGrad;
    double mom1_pl_sl = mom0_pl_sl*ts_pl_sl + m1_pl_sl;

    double mom1_sl    = mom1_rd_sl + mom1_pl_sl;

    DB_MSG(("   M1 (SliceFc) - used: %8.6lf  expected: %8.6lf : Difference: %8.6lf",
            mom1_fc_sl, mom1_sl, fabs(mom1_fc_sl)-fabs(mom1_sl) ));
  }

#endif
}
