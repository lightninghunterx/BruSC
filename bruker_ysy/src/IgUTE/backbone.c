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
 *
 * $Id$
 *
 * ***************************************************************/


static const char resid[] = "$Id$ (C) 2004 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"


void backbone(void)
{

  double minFov[3];
  double minSliceThick;
  double minte1;
  double pi;


  DB_MSG(("-->backbone"));


  STB_UpdateNuclei(No);

  /* constrain AntiAlias for radial acquisition */
  PVM_AntiAlias[0] = PVM_AntiAlias[1] = 1.0;

  /* encoding */
  STB_UpdateEncodingSpecial(2, 2, Yes);   
  
  /* set limits for read, phase and slice gradients            */
  ControlGradientLimits();

   
  minFov[0] = EncodingLimits( PVM_EncMatrix[0]/2,
			      PVM_AntiAlias,
			      PVM_GradCalConst);

  /* radial projection constraint  */
  minFov[1] = minFov[0];
  
  /* update excitation pulse */   
  UpdateRFPulses();

  minSliceThick =  ExcPulse1.Bandwidth /
                  (1e-2*ExcSliceGradLim * PVM_GradCalConst);

  
  /* update geometry parameters */
  UpdateGeometry(minFov,minSliceThick);

      
  /* calculate number of projections */
  pi=M_PI;
  NPro = (int) ((pi/2) * PVM_EncMatrix[0]*2/ProUndersampling);

  if(NPro < 4)
  {
    NPro = 4;
    ProUndersampling = (pi/2) * PVM_EncMatrix[0]*2/NPro;
  }

  if(NPro%2) NPro++;

  EncodingGradients(PVM_Fov[0], PVM_Fov[1],PVM_GradCalConst);


  UpdateSliceSelectionGradients(PVM_SliceThick,
                                ExcPulse1.Bandwidth,
				PVM_GradCalConst);

  double _dw = 1000 / (PVM_EffSWh * PVM_AntiAlias[0]);

  if(RampCompYN==Yes)
  {
    double points= CFG_GradientRampTime()/(2.0*_dw);
    RampPoints = (int) (0.5 + points);
    RampCompTime = RampPoints * _dw;
    RampCompTime = ceil(RampCompTime * 1000.0)/1000.0; 
    ParxRelsShowInEditor("RampPoints");
  }
  else
  {
    RampPoints = 0;
    RampCompTime = 0.0;
    ParxRelsHideInEditor("RampPoints");
  }


  /* update slice spoiler */
  double mindurSlice = 2.5*CFG_GradientRiseTime();
  MRT_UpdateSpoiler("SliceSpoiler",2.0,ExcSliceGradLim,mindurSlice,PVM_GradCalConst,PVM_SliceThick);

  /* handling of modules */
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, PVM_SliceThick);
  STB_UpdateSatTransModule(PVM_DeriveGains);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  STB_UpdateTriggerModule(); //only used for trajectory module
  if(PVM_TriggerModule == On)
    PVM_TriggerMode = per_PhaseStep;

  /* update of navigator module */
  if(PVM_NavPosMode==Nav_in_slice)
  {
    PVM_NavOnOff = Off;
    ParxRelsHideInEditor("PVM_NavPoints,PVM_NavSWh");
    PVM_DriftCompYesNo=No;
    ParxRelsHideInEditor("PVM_DriftCompYesNo");
    ParxRelsShowInEditor("AddCentralPoints");
  }
  else
  {
    PVM_NavOnOff = On;  
    ParxRelsShowInEditor("PVM_NavPoints,PVM_NavSWh");  
    ParxRelsShowInEditor("PVM_DriftCompYesNo");
    AddCentralPoints=0;
    ParxRelsHideInEditor("AddCentralPoints");
  }
  STB_UpdateNavModule();
  ParxRelsShowInEditor("PVM_NavPosMode");
 
  /* update of visibility of self gating parameters */
  UpdateSelfGating();
  
  /* calculate frequency offsets */
  LocalFrequencyOffsetRels();

  /* update sequence timing */
  UpdateEchoTime(&minte1);
  UpdateRepetitionTime();
  STB_UpdateDummyScans(PVM_RepetitionTime,PVM_NAverages);
  STB_UpdateDriftComp(PVM_RepetitionTime); 

  /* for PET/MR study */
  UpdateTTLOut();

  /* calculate dephasing gradients */
  EncodingDephGradients();

  PVM_NEchoImages = 1;

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  SetBaseLevelParam();

  /* update trajectory module */

  double gradAmp[2]={ReadGrad, PhaseGrad};
  double gradMax[2]={ReadGrad, PhaseGrad};
  int nPoints = PVM_EncMatrix[0]/2 + RampPoints + AddCentralPoints;
  int nJunk = 0;

  STB_UpdateTraj(PVM_SpatResol, PVM_EncZf, "PVM_SliceGeo", 2, NPro, nPoints+nJunk, 0, _dw, gradMax, 
                 PVM_TriggerModule, GradSync, 0.5, 0.0, GradRes, gradAmp); 

  UpdateAdjustments();

  OutputMovieFrames = PVM_NMovieFrames;
  SetRecoParam();
  

  DB_MSG(("<--backbone"));
  return;
}

/*-------------------------------------------------------
 * local utility routines to simplify the backbone 
 *------------------------------------------------------*/

void UpdateEchoTime(double *const minte1)
{  

  DB_MSG(("-->UpdateEchoTime"));

  double _dw = 1000 / (PVM_EffSWh * PVM_AntiAlias[0]);

   *minte1 = 
    ExcPulse1.Length*ExcPulse1.Rpfac/100    +
    GrShapeT1                               +
    2.0*GrShapeT2                           +
    GrShapeT3                               +
    AddCentralPoints*_dw; 
   
   PVM_MinEchoTime = *minte1;
   
   PVM_EchoTime = MAX_OF(PVM_MinEchoTime,PVM_EchoTime);

   
   if(YesNoMinEchoTime==Yes)
   {
     TeFillDelay=0.0;
     PVM_EchoTime = PVM_MinEchoTime;
     ParxRelsMakeNonEditable("PVM_EchoTime");
   }
   else
   {
     TeFillDelay = PVM_EchoTime - PVM_MinEchoTime;
     ParxRelsMakeEditable("PVM_EchoTime");
   }

  DB_MSG(("<--UpdateEchoTime"));
  return;
}



void UpdateRepetitionTime(void)
{
  int nslices;
  double TotalTime, rephaseT;

  DB_MSG(("-->UpdateRepetitionTime"));

  /* adapt timing for sufficient spoiling */
  ReadSpoilDur = 1e5*ReadSpoiling/(ReadGrad*PVM_GradCalConst*PVM_SpatResol[0]);

  nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
 
  rephaseT = CFG_GradientRampTime()
           + PVM_AcquisitionTime + RampCompTime    // PVM_AcquisitionTime + RampCompTime = aq_job0 !!
           + CFG_GradientRiseTime() + ReadSpoilDur;
  
  double delay = PVM_NavOnOff == On ? 0.04 : 0.00;

  PVM_MinRepetitionTime = 
    nslices*(
       0.03                                       + /*UpdateDynPars*/
       0.01                                       +
       delay                                      +
       PVM_FovSatModuleTime                       +
	     PVM_FatSupModuleTime                       +
	     PVM_SatTransModuleTime                     +
	     PVM_NavigatorModuleTime                    +
	     SliceSpoiler.dur                           +
	     2.0*CFG_GradientRiseTime()                 +
	     ExcPulse1.Length*(1 - ExcPulse1.Rpfac/100) +
	     PVM_EchoTime                               +
	     rephaseT                                   +
       0.012);                                      /*MinD0* + 2u */
  
  PVM_RepetitionTime=MAX_OF(PVM_MinRepetitionTime,PVM_RepetitionTime);
  
  TotalTime = PVM_RepetitionTime 
              * NPro
              * Oversampling
              * PVM_NAverages
              * PVM_SatTransRepetitions
              * PVM_NRepetitions;
    
  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<--UpdateRepetitionTime"));
  return;
}


void LocalFrequencyOffsetRels( void )
{
  int spatDim;
  int i,nslices;

  spatDim = PTB_GetSpatDim();
  nslices = GTB_NumberOfSlices(PVM_NSPacks,PVM_SPackArrNSlices);

  MRT_FrequencyOffsetList(nslices,
			  PVM_EffReadOffset,
			  ReadGrad,
			  PVM_GradCalConst,
			  PVM_ReadOffsetHz );

  MRT_FrequencyOffsetList(nslices,
			  PVM_EffSliceOffset,
			  ExcSliceGrad,
			  PVM_GradCalConst,
			  PVM_SliceOffsetHz );

  if(spatDim == 3)
  {
    for(i=0;i<nslices;i++)
      PVM_EffPhase2Offset[i] = -PVM_EffSliceOffset[i];
  }


}


void UpdateGeometry(double *minFov, double minSliceThick)
{
  // fix shifts along read and phase, slice selection allowed
  STB_ConstrainSlicePosition(true,true,false);
  // only one slice allowed if navigator is in-slice
  int maxSlices = PVM_NavPosMode == Nav_in_slice? 1:0;

  STB_UpdateImageGeometry(2, PVM_Matrix, minFov, maxSlices, 1, 0, minSliceThick, 1);
}

/*------------------------------------------------------
 * routines for parameter group initialization and 
 * update
 *-----------------------------------------------------*/

double EncodingLimits( int dpoints, 
		       double *antiAlias,
		       double gradCalConst) 
{
  double minFovRead = 0.0;

  DB_MSG(("-->EncodingLimits"));

  MRT_UpdateDigPars( &PVM_EffSWh,
                      dpoints,
                      antiAlias,
                     &PVM_AcquisitionTime
                   );
 
  /* minimum field of view in the read direction */
  minFovRead = PVM_EffSWh/(1e-2*ReadGradLim * gradCalConst);

  DB_MSG(("<--EncodingLimits"));

  return minFovRead;
}


void EncodingGradients(double readFov,
		       double phaseFov,
		       double gradCalConst)
{
  DB_MSG(("-->EncodingGradients"));

  ReadGrad  = MRT_ReadGrad(PVM_EffSWh,readFov,gradCalConst);

  PhaseGrad = MRT_ReadGrad(PVM_EffSWh,phaseFov,gradCalConst);

  if((ReadGrad - ReadGradLim) > 1.0e-3)
  {
    UT_ReportError("ReadGrad exceeds its limits\n");
  }

  if((PhaseGrad - ReadGradLim) > 1.0e-3)
  {
    UT_ReportError("PhaseGrad exceeds its limits\n");
  }

  ReadGrad = MIN_OF(ReadGrad,ReadGradLim);
  PhaseGrad = MIN_OF(PhaseGrad,ReadGradLim);

  /* Part of the echo to be refocused */
  double EchoDelay = PVM_AcquisitionTime * PVM_EchoPosition / 100;

  double minDephRead  = MRT_DephaseTime(EchoDelay, 
                                 CFG_GradientRiseTime(),
                                 ReadGrad,
                                 ReadDephGradLim);

  double minDephPhase  = MRT_DephaseTime(EchoDelay, 
                                 CFG_GradientRiseTime(),
				 PhaseGrad,
                                 ReadDephGradLim);

  double max = MAX_OF(minDephRead,minDephPhase);
  ReadDephTime = MAX_OF(max+CFG_GradientRiseTime(),2.5*CFG_GradientRiseTime());

  DB_MSG(("<--EncodingGradients"));
  return;
}

void EncodingDephGradients(void)
{
  
  DB_MSG(("-->EncodingDephGradients"));
   
  double EchoDelay = PVM_AcquisitionTime * PVM_EchoPosition / 100;
  double riseT = CFG_GradientRiseTime();

  ReadDephGrad =  MRT_DephaseGrad(ReadDephTime-riseT, EchoDelay, riseT, ReadGrad);
  
  PhaseDephGrad = MRT_DephaseGrad(ReadDephTime-riseT, EchoDelay, riseT, PhaseGrad);

  DB_MSG(("<--EncodingDephGradients"));
  return;
}


YesNo UpdateSliceSelectionGradients(const double slthk,
				    const double excPulseBW,
				    double gradCalConst)
{

  double gradslope=0;

  DB_MSG(("-->UpdateSliceSelectionGradients"));

  /* range check of arguments */

  if(slthk <= 0.0)
  {
    UT_ReportError("UpdateSliceSelectionGradients: "
		   "Illegal value of argument 1\n");
    return No;

  }


  if(excPulseBW < 0.0)
  {
    UT_ReportError("UpdateSliceSelectionGradients: "
		   "Illegal value of argument 3\n");
    return No;

  }
    
  if(gradCalConst <= 0.0)
  {
    UT_ReportError("UpdateSliceSelectionGradients: "
		   "Illegal value of argument 4\n");
    return No;

  }

  /* update of slice gradient */
  ExcSliceGrad     = MRT_SliceGrad(excPulseBW,slthk,gradCalConst);
  

  if((ExcSliceGrad - ExcSliceGradLim)>1.0e-3)
  {
    UT_ReportError("UpdateSliceSelectionGradients: "
		   "ExcSliceGrad exceeded maximum!\n");
    return No;

  }

  ExcSliceGrad = MIN_OF(ExcSliceGrad,ExcSliceGradLim);

  /* calculate shape parameters for slice refocusing */  
  if(RampMode == constant_time)
  {
    GrShapeT1 = Ramptime;
    gradslope = ExcSliceGrad/GrShapeT1;
  }
  else
  {
    gradslope = EffSlewRate/CFG_GradientRampTime(); 
    GrShapeT1 = ExcSliceGrad/gradslope;
    GrShapeT1 = ceil(GrShapeT1*1000.0)/1000.0;
    gradslope = ExcSliceGrad/GrShapeT1;
  }

  GrShapeT2 = sqrt(ExcSliceGrad*(0.5*GrShapeT1+ExcPulse1.Length*ExcPulse1.Rpfac/100.0)/gradslope);
  GrShapeT2 = ceil(GrShapeT2*1000.0)/1000.0;
  GrShapeG2 = -gradslope*GrShapeT2;

  if(GrShapeG2 < -95.0)
  {
    GrShapeG2 = -95.0;
    GrShapeT2 = -GrShapeG2/gradslope;
    GrShapeT2 = ceil(GrShapeT2*1000.0)/1000.0;
    GrShapeG2 = -gradslope*GrShapeT2;  
    GrShapeT3 = -(ExcSliceGrad*(0.5*GrShapeT1+ExcPulse1.Length*ExcPulse1.Rpfac/100.0)+GrShapeG2*GrShapeT2)/GrShapeG2;
    GrShapeT3 = ceil(GrShapeT3*1000.0)/1000.0;
  }
  else 
  {
    GrShapeT3 = 0.0;
  }

  GrShape1Time = CFG_GradientRiseTime() +  ExcPulse1.Length + GrShapeT1 + 2*GrShapeT2 + GrShapeT3;
  GrShape1Time = ceil(GrShape1Time*1000.0)/1000.0;

  DB_MSG(("<--UpdateSliceSelectionGradients"));
  return Yes;
}

/*--------------------------------------------------------
 * Routine to update RF pulse parameters
 *-------------------------------------------------------*/

void UpdateRFPulses(void)
{

  /* Updates all parameters that belong to ExcPulse1 pulse structure
     (as initialized by STB_InitRFPulse see initMeth.c)
  */

  STB_UpdateRFPulse("ExcPulse1",1,PVM_DeriveGains,Conventional);


  if(PVM_DeriveGains==Yes)
  {
    ParxRelsHideInEditor("ExcPulse1Ampl");
  }
  else
  {
    ParxRelsShowInEditor("ExcPulse1Ampl");
  }

  ParxRelsShowInFile("ExcPulse1Ampl");

  DB_MSG(("<--UpdateRFPulses"));

  return;
}


void ControlGradientLimits(void)
{
  DB_MSG(("-->ControlGradientLimits"));

  ExcSliceRephGradLim = ExcSliceGradLim = 100.0;
  
  ReadGradLim = ReadDephGradLim = 100.0;

  DB_MSG(("<--ControlGradientLimits"));
}

void UpdateSelfGating(void)
{
  DB_MSG(("-->UpdateSelfGating"));

  if(SelfGating==No)
  {
    Oversampling=1;
    PVM_NMovieFrames=1;
    ParxRelsMakeNonEditable("Oversampling,PVM_NMovieFrames");
    ParxRelsShowInEditor("PVM_NAverages");
  }
  else
  {
    PVM_NAverages=1;
    ParxRelsMakeEditable("Oversampling,PVM_NMovieFrames");  
    ParxRelsHideInEditor("PVM_NAverages"); 
  }

  DB_MSG(("<--UpdateSelfGating"));
}

void UpdateTTLOut(void)
{ 

  double rephaseT = CFG_GradientRampTime()
    + PVM_AcquisitionTime + RampCompTime 
    + CFG_GradientRiseTime() + ReadSpoilDur;

  IgTTLDuration =
    SliceSpoiler.dur                           +
    2.0*CFG_GradientRiseTime()                 +
    ExcPulse1.Length*(1 - ExcPulse1.Rpfac/100) +
    PVM_EchoTime                               +
    rephaseT                                   +
    0.001;

  if(PET_MR_Study == Yes)
    ParxRelsShowInEditor("IgTTLOut, IgTTLDuration");
  else
  {
    IgTTLOut = No;
    ParxRelsHideInEditor("IgTTLOut, IgTTLDuration");
  }

  PpgTTLOut = IgTTLOut;

}

int lcm(int count1, int count2)
//lowest common multiple
{
  return (count1*count2)/gcd(count1,count2);
}

int gcd(int count1, int count2)
// greatest common divisor
{
  if(count2 == 0)
  {
    return count1;
  }
  return gcd(count2, count1%count2);
}
