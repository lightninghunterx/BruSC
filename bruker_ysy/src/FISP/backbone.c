 /****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999-2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2003 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	0

#include "method.h"

//del of acq after read ramp
#define ACQDEL 0.02


void backbone( void )
{

  DB_MSG(("---> fisp:backbone"));


  STB_UpdateNuclei(Yes);



  /* handle visibility */
  switch(Ssfp)
    {
      default:
      case TRUE_FISP:
        PVM_EchoPosition = 50;
	RFSpoilerOnOff = Off;
        ParxRelsHideInEditor("RFSpoilerOnOff");
        ParxRelsHideInFile("RFSpoilerOnOff");
	break;
      case ECHO:
	RFSpoilerOnOff = Off;
        ParxRelsHideInEditor("RFSpoilerOnOff");
        ParxRelsHideInFile("RFSpoilerOnOff");
	break;
      case FID:
        ParxRelsShowInEditor("RFSpoilerOnOff");
        ParxRelsShowInFile("RFSpoilerOnOff");
	break;
    }


  /* update RF pulses */
  UpdateRFPulses();

  /* update encoding parameter group */        
  int dim = PTB_GetSpatDim();
  if(dim == 3)
  {
    ParxRelsMakeNonEditable("Nsegments");     
    if(KeepSteadyState3D){
        PVM_RareFactor = 1;
        PVM_NMovieFrames = 1;
        ParxRelsMakeNonEditable("PVM_RareFactor,PVM_NMovieFrames");
    }else{
        ParxRelsMakeEditable("PVM_RareFactor,PVM_NMovieFrames");
    }
  }
  else
  {
    ParxRelsMakeEditable("Nsegments,PVM_NMovieFrames");
    ParxRelsMakeNonEditable("PVM_RareFactor"); 
    
    PVM_RareFactor = 1;
    
    STB_UpdateEncoding(
      &PVM_RareFactor,
      Seg_mode == sequential ? SEG_SEQUENTIAL: SEG_INTERLEAVED,
      Yes,
      Yes,
      Yes,
      Ssfp==TRUE_FISP? NULL : &PVM_EchoPosition,
      3);

    PVM_RareFactor = PVM_EncGenTotalSteps/Nsegments;
  }
  
  STB_UpdateEncoding(
    &PVM_RareFactor,
    Seg_mode == sequential ? SEG_SEQUENTIAL: SEG_INTERLEAVED,
    Yes,
    Yes,
    Yes,
    Ssfp==TRUE_FISP? NULL : &PVM_EchoPosition,
    3);

  Nsegments = PVM_EncGenTotalSteps/PVM_RareFactor; 

  if(KeepSteadyState3D){
      PVM_RareFactor = Nsegments;
      Nsegments = 1;
  }

  /* update parameters controlling the data sampling period   */

  MRT_UpdateDigPars( &PVM_EffSWh,
                      PVM_EncMatrix[0],
                     &PVM_AntiAlias[0],
                     &PVM_AcquisitionTime);


 /* calculate minima for FOV and slice thickness             */

  UpdateGeometryMinima();


  /* update geometry parameters                               */
  STB_UpdateAtsReferencePosition("PVM_SliceGeoObj");

  // only one slice per package if 3D
  int maxPerPackage = dim>2? 1:0;

  STB_UpdateImageGeometry(dim,
                          PVM_Matrix,
                          Local_MinFov,
                          0, // total slices (no restr)
                          0,
                          maxPerPackage,
                          MinSliceThick,
                          1.0); // sliceFovRatio in 3D

  std::vector<double> spatres=STB_GetSpatResol();
  STB_AtsUpdate(&spatres);


  /* Calculate read and slice gradients */
  UpdateReadSliceGradients();

  /* calculate frequency offsets */
  UpdateFrequencyOffsets();

  UpdateMovie();

  STB_UpdateTriggerModule();

  UpdateEchoTime();

  UpdateRepetitionTime();

  if(Ssfp_preparation==Starter_Sequence)
  {
     PVM_DummyScans=0;
     PVM_DummyScansDur=0.0;
  }
  else
    STB_UpdateDummyScans(PVM_RepetitionTime);

  STB_UpdateEvolutionModule();

  /* update slice spoiler */
  double mindurSlice = 1.5*CFG_GradientRiseTime();
  double mindurPrep = 2.0*CFG_GradientRiseTime();
  double spoilerThick = dim>2 ? PVM_SpatResol[2]:PVM_SliceThick;
  MRT_UpdateSpoiler("SliceSpoiler",2.0,SliceGradLim,mindurSlice,PVM_GradCalConst,spoilerThick);
  MRT_UpdateSpoiler("PrepSpoiler",2.0,ReadGradLim, mindurPrep,PVM_GradCalConst,PVM_SpatResol[0]);

  UpdatePrepTime(); /* dummy scans and spoiler must be set before */
  UpdateTotalTime();

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  /* set baselevel acquisition parameter */
  SetBaseLevelParam();

  /* set baselevel reconstruction parameter */
  SetRecoParam();

  DB_MSG(("<--- fisp:backbone"));
}


/****************************************************************/
/*	         L O C A L   F U N C T I O N S			*/
/****************************************************************/

void UpdateGeometryMinima(void)
{
  DB_MSG(("-->UpdateGeometryMinima"));

  SliceGradLim = 50.0;
  ReadGradLim  = 50.0;

  Local_MinFov[0] = 100.0*PVM_EffSWh/(ReadGradLim*PVM_GradCalConst);

  /* in this method, TE is adjusted to FOV[1,2] therefore no
     constraints (or just very rough ones):;  */
  Local_MinFov[1] = Local_MinFov[2] = 5.0;

  /** minimum Slice Thickness  **/
  MinSliceThick = 100.0*ExcPulse1.Bandwidth/
                      (SliceGradLim*PVM_GradCalConst);

  DB_MSG(("<--UpdateGeometryMinima"));
}

void UpdateReadSliceGradients(void)
{
  DB_MSG(("-->UpdateReadSliceGradients"));

  ReadGrad = MRT_ReadGrad(PVM_EffSWh,
                          PVM_Fov[0],
                          PVM_GradCalConst);

  SliceGrad = MRT_SliceGrad(ExcPulse1.Bandwidth,
                            PVM_SliceThick,
                            PVM_GradCalConst);

  DB_MSG(("<--UpdateReadSliceGradients"));
}

void UpdateMovie(void)
{
  if(PVM_NMovieFrames > 1)
  {
    ParxRelsShowInEditor("Const_separation,Seg_Sep_Fra_time");
  }
  else
  {
    ParxRelsHideInEditor("Const_separation,Seg_Sep_Fra_time");
  }
}

void UpdateEchoTime(void)
/* ------------------------------------------------------------------
   calculates PVM_MinEchoTime, constrains PVM_EchoTime
   and finds refoc. and ph_enc amplitudes.
   Calculates Local_MinFov[1,2] for this PVM_EchoTime.
   ---------------------------------------------------------------- */
{
   double ref_del, sl_ref_amp, rd_ref_amp, half_pulse, ramp, ramp_int,
     ph1_amp, ph2_amp=0.0, ref_del_min, kHz_per_mm, echo_del,
          lim2dgrad, lim3dgrad,lim_read_ref, lim_slice_ref;
   int ch1=0, ch2=0, ch3=0, ch4=0;

   half_pulse = ExcPulse1.Length*ExcPulse1.Rpfac/100.0
                + 0.01;
   echo_del   = PVM_AcquisitionTime*PVM_EchoPosition/100.0 + ACQDEL;
   //   half_acq   = PVM_AcquisitionTime/2 + ACQDEL;
   ramp       = CFG_GradientRiseTime();
   ramp_int   = 50.0;
   ref_del    = PVM_EchoTime - half_pulse - echo_del;

   /* the above line is for single ramps; for double ramps take:
   ref_del    = PVM_EchoTime - half_pulse - half_acq - 2*ramp;
   */
   ref_del_min= 2*ramp;
   kHz_per_mm = PVM_GradCalConst / 1000.0;
   lim2dgrad = 57;
   lim3dgrad = 57;
   lim_read_ref = MIN_OF(57,(100-ReadGrad));
   lim_slice_ref = MIN_OF(57,(100-SliceGrad));


   /* read and slice refocusing */
   ch1 = set_refoc(&ref_del, &rd_ref_amp, echo_del, ReadGrad,
	           ramp, ramp_int, lim_read_ref, ref_del_min);
   ch2 = set_refoc(&ref_del, &sl_ref_amp, half_pulse, SliceGrad,
	           ramp, ramp_int, lim_slice_ref, ref_del_min);
   ch3 = set_pulse(&ref_del, &ph1_amp, PVM_SpatResol[1],ramp,
                   lim2dgrad, kHz_per_mm);
   if(PTB_GetSpatDim() == 3)
   {
     lim3dgrad = 57 - fabs(sl_ref_amp);
     ch4 = set_pulse(&ref_del, &ph2_amp, PVM_SpatResol[2],ramp,
                     lim3dgrad, kHz_per_mm);
   }

   if(ch1 || ch2 || ch3 || ch4)
   { /* ref_del has increased: slice and read refocusing must be calculated
        again and TE must be updated. Current ref_del will be accepted. */

     (void)set_refoc(&ref_del, &rd_ref_amp, echo_del, ReadGrad,
	             ramp, ramp_int, lim_read_ref, ref_del_min);
     (void)set_refoc(&ref_del, &sl_ref_amp, half_pulse, SliceGrad,
	             ramp, ramp_int, lim_slice_ref, ref_del_min);
     ch3 = set_pulse(&ref_del, &ph1_amp, PVM_SpatResol[1],ramp,
                   lim2dgrad, kHz_per_mm);
     if(PTB_GetSpatDim() == 3)
     {
       lim3dgrad = 57 - fabs(sl_ref_amp);
       ch4 = set_pulse(&ref_del, &ph2_amp, PVM_SpatResol[2],ramp,
                     lim3dgrad, kHz_per_mm);
     }

     /* PVM_EchoTime = ref_del + half_pulse + half_acq; */
     /* the above line is for single ramps; for double ramps take:
     PVM_EchoTime = ref_del + half_pulse + half_acq + 2*ramp;
     */

   }

   PVM_EchoTime = ref_del + half_pulse + echo_del;


   /* the following assignments are needed to pass values to
      SetBaseLevelPars() */
   ReadDephaseTime = ref_del;
   SliceRephaseTime = ref_del;
   EncGradDur = ref_del;
   SliceRephaseGrad = sl_ref_amp;
   Phase2DGrad = ph1_amp;
   Phase3DGrad = ph2_amp;
   ReadDephGrad = rd_ref_amp;


}


void UpdateRepetitionTime( void )
/* -----------------------------------------------------------
   calculate PVM_MinRepetitionTime and constrain PVM_RepetitionTime
   ---------------------------------------------------------------*/
{

  DB_MSG(("--> UpdateRepetitionTime"));


  PVM_MinRepetitionTime = 2*PVM_EchoTime;

  // limit the time when read grad is on (fid/echo mode only):
  double maxRepTime = PVM_MinRepetitionTime + 10;

  switch(Ssfp)
  {
    case TRUE_FISP:
      PVM_RepetitionTime = PVM_MinRepetitionTime;
      FID_delay=ACQDEL;
      ECHO_delay=ACQDEL;
      break;

    case ECHO:
      PVM_RepetitionTime = MIN_OF(MAX_OF(PVM_RepetitionTime, PVM_MinRepetitionTime),maxRepTime);
      FID_delay = PVM_RepetitionTime-PVM_MinRepetitionTime + ACQDEL;
      ECHO_delay = ACQDEL;
      break;

    case FID:
      PVM_RepetitionTime = MIN_OF(MAX_OF(PVM_RepetitionTime, PVM_MinRepetitionTime),maxRepTime);
      ECHO_delay = PVM_RepetitionTime-PVM_MinRepetitionTime + ACQDEL;
      FID_delay = ACQDEL;
      break;
  }

  DB_MSG(("--> UpdateRepetitionTime"));

}

void UpdatePrepTime(void)
{
  DB_MSG(("--> UpdatePrepTime"));
  
  double riseT = CFG_GradientRiseTime();

  /* Alpha2 Preparation */
  Alpha2Delay = PVM_RepetitionTime/2 - ExcPulse1.Length - 0.02 - riseT;
  
  double min = riseT + 0.03;
  if(Alpha2Delay < min)
  { 
    if(Ssfp_preparation == Alpha_2)
      throw PvStdException("Alpha/2 Preparation cannot be realized - TR too short.");
    else
      Alpha2Delay = MAX_OF(Alpha2Delay,min);
  }
  
  Alpha2SliceRephGrad = - MRT_DephaseGrad(Alpha2Delay, ExcPulse1.Length+0.02+riseT, riseT, SliceGrad);
  
  double grad_lim =  MAX_OF(-100, SliceGrad - 100);

  if((Alpha2SliceRephGrad < grad_lim) && (Ssfp_preparation == Alpha_2))
    throw PvStdException("Alpha/2 Preparation cannot be realized - TR too short.");
  
  Alpha2_Prep_time = (ExcPulse1.Length + 0.02 + Alpha2Delay + riseT)/PVM_RepetitionTime;
 
  /* Calculate prescan delay */
  switch(Ssfp_preparation)
  {
  default:
  case Alpha_2:
    Prescan_Delay = SliceSpoiler.dur + Alpha2_Prep_time * PVM_RepetitionTime + PVM_DummyScans*PVM_RepetitionTime;
    break;
  case Starter_Sequence:
    Prescan_Delay = SliceSpoiler.dur + PVM_RepetitionTime + PrepSpoiler.dur + NumPrep*PVM_RepetitionTime + riseT;
    break;
  case No_Preparation:
    Prescan_Delay = SliceSpoiler.dur + PVM_DummyScans*PVM_RepetitionTime;
    break;
  }


  DB_MSG(("<-- UpdatePrepTime"));
}


void UpdateTotalTime(void)
/* ----------------------------------------------------------------------
   calculates Seg_time, Seg_Sep_time, Seg_Sep_Fra_time, Total_scan_time
   ---------------------------------------------------------------------- */
{
  int dim, nSlices;
  double frameloop;
  double riseT = CFG_GradientRiseTime();
  double min=0.0;

  DB_MSG(("--> UpdateTotalTime"));

  Seg_time = PVM_RepetitionTime*PVM_RareFactor;

  Seg_Sep_time = Seg_time * (1 + Const_separation);

  SeparationEchoes = (int)(Const_separation * PVM_RareFactor);
  Const_separation = SeparationEchoes/PVM_RareFactor;

  Seg_Sep_time = Seg_time * (1 + Const_separation);

  Seg_Sep_Fra_time = Seg_Sep_time * PVM_NMovieFrames;

  /* Inversion Delay */

  MinInversionTime = InvPulse1.Length/2
    + Prescan_Delay                               /* approach to ss  */
    + (PVM_EncCentralStep1 -1)*PVM_RepetitionTime /* del. to k=0     */
    + 0.01;                                       /* min d16         */


  YesNo  request=No;
  int parIndex = 0, dummyIndex;
  request=UT_GetRequest("Scan_RepetitionTime, Inv_Rep_time", &parIndex, "", &dummyIndex);

  if(request == Yes)
  {
    if(parIndex==2)
    {
      Scan_RepetitionTime=Inv_Rep_time;
    }
    else
      Inv_Rep_time=Scan_RepetitionTime;

    UT_ClearRequest();
  }

  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
  dim = PTB_GetSpatDim();

  /* condition to keep steady state for 3D experiments */
  if((dim==3) && (nSlices==1) /*&& (Nsegments==1)*/ && (FISP_inversion_enable==No))
  {
    ParxRelsMakeEditable("KeepSteadyState3D");
  }
  else
  {
    KeepSteadyState3D = No;
    ParxRelsMakeNonEditable("KeepSteadyState3D");
  }

  if(KeepSteadyState3D==Yes)
    YesNoMinScanRepTime=Yes;

  if(FISP_inversion_enable == Yes)
  {
     ParxRelsShowInEditor("PVM_InversionTime,InvPulse1Enum,InvPulse1");
     PVM_InversionTime = MAX_OF(PVM_InversionTime, MinInversionTime);
     PVM_InversionTime = MIN_OF(PVM_InversionTime,100000);
     min = PVM_InversionTime+InvPulse1.Length/2+0.02+riseT+Seg_Sep_Fra_time-(PVM_EncCentralStep1 -1)*PVM_RepetitionTime;
     if(YesNoMinScanRepTime==Yes)
       Inv_Rep_time=min;
     else
       Inv_Rep_time = MAX_OF(min,Inv_Rep_time);
     Inv_Rep_time = MIN_OF(100000,Inv_Rep_time);
     ParxRelsMakeEditable("Inv_Rep_time");
     Scan_RepetitionTime=Inv_Rep_time;
  }
  else
  {
    ParxRelsHideInEditor("PVM_InversionTime,InvPulse1Enum,InvPulse1");
    PVM_InversionTime = 0.0;
    ParxRelsMakeNonEditable("Inv_Rep_time");
    min = Prescan_Delay+Seg_Sep_Fra_time+0.02;
    if(YesNoMinScanRepTime==Yes)
      Scan_RepetitionTime=min;
    else
      Scan_RepetitionTime = MAX_OF(min,Scan_RepetitionTime);
    Inv_Rep_time=Scan_RepetitionTime;
  }


  /*  Scan_RepetitionTime = Inv_Rep_time */
  frameloop = Inv_Rep_time;

  /** Calculate Total Scan Time and Set for Scan Editor **/

  if(dim == 3)
  {
    if(KeepSteadyState3D == No)
    {
      Total_scan_time = frameloop * PVM_NAverages * nSlices * PVM_EncGenTotalSteps/PVM_RareFactor;
    }
    else
    {
      Total_scan_time  = (Prescan_Delay+0.02+ Seg_Sep_Fra_time*PVM_EncGenTotalSteps/PVM_RareFactor)*PVM_NAverages;
    }
  }
  else
    Total_scan_time = frameloop * PVM_NAverages * Nsegments * nSlices;

  /* time for one repetition */
  OneRepTime = Total_scan_time/1000.0;

  Total_scan_time = Total_scan_time * PVM_EvolutionCycles + PVM_EvolutionModuleTime;
  Total_scan_time = Total_scan_time * PVM_NRepetitions;

  PVM_ScanTime = Total_scan_time;
  UT_ScanTimeStr(PVM_ScanTimeStr,Total_scan_time);
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");
  if(YesNoMinScanRepTime==Yes)
    ParxRelsMakeNonEditable("Scan_RepetitionTime,Inv_Rep_time");
  else
    ParxRelsMakeEditable("Scan_RepetitionTime,Inv_Rep_time");

  DB_MSG(("<-- UpdateTotalTime"));
}


void UpdateFrequencyOffsets( void )
{
  int spatDim;
  int i,nslices;

  DB_MSG(("-->UpdateFrequencyOffsets"));

  spatDim = PTB_GetSpatDim();
  nslices = GTB_NumberOfSlices(PVM_NSPacks,PVM_SPackArrNSlices);

  MRT_FrequencyOffsetList(nslices,
                          PVM_EffReadOffset,
                          ReadGrad,
                          PVM_GradCalConst,
                          PVM_ReadOffsetHz );

  MRT_FrequencyOffsetList(nslices,
                          PVM_EffSliceOffset,
                          SliceGrad,
                          PVM_GradCalConst,
                          PVM_SliceOffsetHz );

  if(spatDim == 3)
  {
    for(i=0;i<nslices;i++)
      PVM_EffPhase2Offset[i] = -PVM_EffSliceOffset[i];
  }

  DB_MSG(("<--UpdateFrequencyOffsets"));
}


/*--------------------------------------------------------
 * Routine to update RF pulse parameters
 *-------------------------------------------------------*/

void UpdateRFPulses(void)
{
  DB_MSG(("-->UpdateRFPulses"));

  STB_UpdateRFPulse("ExcPulse1",1,PVM_DeriveGains,Conventional);
  STB_UpdateRFPulse("InvPulse1",1,PVM_DeriveGains,AfpUp);

  if(PVM_DeriveGains==Yes)
  {
    ParxRelsHideInEditor("ExcPulse1Ampl,InvPulse1Ampl");
  }
  else
  {
    ParxRelsShowInEditor("ExcPulse1Ampl");
    if(FISP_inversion_enable==Yes)
    {
      ParxRelsShowInEditor("InvPulse1Ampl");
    }
  }

  ParxRelsShowInFile("ExcPulse1Ampl,InvPulse1Ampl");

  DB_MSG(("<--UpdateRFPulses"));

  return;
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/


