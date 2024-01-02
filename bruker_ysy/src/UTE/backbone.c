/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2004
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
  double minte1, minte2;
  double pi;


  DB_MSG(("-->backbone"));


  STB_UpdateNuclei(No);
  
  if(AcqMode == FID)
    ParxRelsShowInEditor("RampCompYN");
  else
    ParxRelsHideInEditor("RampCompYN"); 

  /* constrain AntiAlias for radial acquisition */
  PVM_AntiAlias[0] = PVM_AntiAlias[1] = 1.0;

  /* encoding */
  STB_UpdateEncodingSpecial(2, 2, Yes);   
  
  /* set limits for read, phase and slice gradients            */
  ControlGradientLimits();

   
  minFov[0] = EncodingLimits( AcqMode == ECHO ? PVM_EncMatrix[0] : PVM_EncMatrix[0]/2,
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
  NPro = (int) ((pi/2) * PVM_EncMatrix[0]*(AcqMode == ECHO ? 1:2)/ProUndersampling);

  if(NPro < 4)
  {
    NPro = 4;
    ProUndersampling = (pi/2) * PVM_EncMatrix[0]*(AcqMode == ECHO ? 1:2)/NPro;
  }

  if(NPro%2) NPro++;

  EncodingGradients(PVM_Fov[0], PVM_Fov[1],PVM_GradCalConst);


  UpdateSliceSelectionGradients( PVM_SliceThick,
                                 ExcPulse1.Bandwidth,
                                 PVM_GradCalConst
                               );

  double _dw = 1000 / (PVM_EffSWh * PVM_AntiAlias[0]);

  if(RampCompYN==Yes)
  {
    double points = CFG_GradientRampTime()/(2.0 * _dw);
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
  STB_UpdateTriggerModule();

  /* calculate frequency offsets */
  LocalFrequencyOffsetRels();

  /* update sequence timing */
  UpdateEchoTime(&minte1,&minte2);
  UpdateRepetitionTime();
  STB_UpdateDummyScans(PVM_RepetitionTime);

  /* calculate dephasing gradients */
  EncodingDephGradients();

  PVM_NEchoImages = 1;

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  SetBaseLevelParam();

  /* update trajectory module */

  double gradAmp[2]={ReadGrad, PhaseGrad};
  double gradMax[2]={ReadGrad, PhaseGrad};
  int nPoints = PVM_EncMatrix[0]/2 + RampPoints;
  int nJunk = 0;
  PostPoints = 20;

  if(AcqMode==ECHO)
  {
    gradMax[0] = MAX_OF(ReadGrad, ReadDephGrad);
    gradMax[1] = MAX_OF(PhaseGrad, PhaseDephGrad);
    nPoints = PVM_EncMatrix[0];
    nJunk = ReadDephPoints;
  }
  STB_UpdateTraj(PVM_SpatResol, PVM_EncZf, "PVM_SliceGeo", 2, NPro, nPoints+nJunk+PostPoints, 0, _dw, gradMax, 
                 PVM_TriggerModule, GradSync, 0.5, 0.0, GradRes, gradAmp); 

  UpdateAdjustments();

  SetRecoParam();
  

  DB_MSG(("<--backbone"));
  return;
}

/*-------------------------------------------------------
 * local utility routines to simplify the backbone 
 *------------------------------------------------------*/

void UpdateEchoTime(double *const minte1, double *const minte2)
{  

  DB_MSG(("-->UpdateEchoTime"));

   *minte1 = 
    ExcPulse1.Length*ExcPulse1.Rpfac/100    +
    GrShapeT1                               +
    2.0*GrShapeT2                           +
    GrShapeT3;

   *minte2 = 
    ExcPulse1.Length*ExcPulse1.Rpfac/100    +
    GrShapeT1                               +
    2.0*GrShapeT2                           +
    GrShapeT3                               +
    ReadDephTime                            +
    PVM_AcquisitionTime * PVM_EchoPosition / 100.0;


   if(AcqMode == FID)
   {
     PVM_MinEchoTime = *minte1;
   }
   else
   {
     PVM_MinEchoTime = *minte2;
   }

   PVM_EchoTime = MAX_OF(PVM_MinEchoTime,PVM_EchoTime);
  
   if(AcqMode == ECHO)
   {
     double extension=0.0;
     if(YesNoMinEchoTime==No)
     {
       double extensionPossible = PVM_EchoTime - PVM_MinEchoTime;
       double max = 2*PVM_AcquisitionTime;
       extension = MIN_OF(extensionPossible,max);
       extension = MAX_OF(extension, 0);
     }
     
     /* ReadDephTime should be a multiple of Grid */ 
     double dephaseT = ReadDephTime + extension;
     int gradres = (GradRes*1000.0+0.5);
     double _dw = 1000 / (PVM_EffSWh * PVM_AntiAlias[0]);
     int dwell = (1000.0 * _dw + 0.5);
     Grid = lcm(gradres,dwell)/1000.0;  
     int points = (int)(dephaseT/Grid+0.5);
     dephaseT = points*Grid;
     if(dephaseT<ReadDephTime)
     {
       points = points+1;
       dephaseT = points*Grid;
     }
     double fill = MAX_OF((dephaseT - ReadDephTime),0);
     ReadDephTime = dephaseT;
     ReadDephPoints = (int)(ReadDephTime/_dw + 0.5);
     PVM_MinEchoTime += fill;
     PVM_EchoTime = MAX_OF(PVM_MinEchoTime,PVM_EchoTime);
   }
 
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
  ReadSpoilDur = ceil(ReadSpoilDur*1000.0)/1000.0;

  nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );
 
  if(AcqMode == FID)
  {  
    rephaseT = CFG_GradientRampTime() + 2*CFG_GradientRiseTime() + PVM_AcquisitionTime + RampCompTime + ReadSpoilDur;
  }
  else
  {
    rephaseT = PVM_AcquisitionTime * (1-PVM_EchoPosition / 100.0) + ReadSpoilDur +CFG_GradientRiseTime();
  }

  PVM_MinRepetitionTime = nslices                      *
     (PVM_FovSatModuleTime                             +
      PVM_FatSupModuleTime                             +
      PVM_SatTransModuleTime                           +
      0.03                                             +  /* UpdateDynPars */
      SliceSpoiler.dur                                 +
      2.0*CFG_GradientRiseTime()                       +
      ExcPulse1.Length*(1 - ExcPulse1.Rpfac/100)       +
      PVM_EchoTime                                     +
      rephaseT                                         +
      CFG_InterGradientWaitTime());

  PVM_RepetitionTime=MAX_OF(PVM_MinRepetitionTime,PVM_RepetitionTime);

  if(SliceAlter == Yes)
  {
    NECHOES = 2;
  }
  else
  {
    NECHOES = 1;
  }

  TotalTime = PVM_RepetitionTime 
              * NPro
              * NECHOES
              * PVM_NMovieFrames
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
  if(AcqMode == FID)
  {
    // force zero offsets in read phase and slice
    STB_ConstrainSlicePosition(true,true,false);
    ParxRelsMakeNonEditable("PVM_SPackArrReadOffset,PVM_SPackArrPhase1Offset");     
  }
  else
  {
    STB_UpdateAtsReferencePosition("PVM_SliceGeoObj");
    ParxRelsMakeEditable("PVM_SPackArrReadOffset,PVM_SPackArrPhase1Offset"); 
  }  
 
  if(SysInfo_UseAts())
  {
    GTB_GeoObjFreezePos0("PVM_SliceGeoObj", Yes);
    GTB_GeoObjFreezePos1("PVM_SliceGeoObj", Yes);
  }
  else
  {
    GTB_GeoObjFreezePos0("PVM_SliceGeoObj", No);
    GTB_GeoObjFreezePos1("PVM_SliceGeoObj", No);
  }


  ParxRelsMakeNonEditable("PVM_SPackArrReadOrient"); 

  STB_UpdateImageGeometry(2, PVM_Matrix, minFov, 0, 1, 0, minSliceThick, 1);

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
                     &PVM_AcquisitionTime);
 
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
  
  if(AcqMode == FID)
  {
    ReadGradLim = ReadDephGradLim = 100.0;
  }
  else
  {
    ReadGradLim = ReadDephGradLim = 50.0;
  }

  DB_MSG(("<--ControlGradientLimits"));
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
