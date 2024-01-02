/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 * ***************************************************************/

static const char resid[] = "$Id$ (C) 2011 Bruker BioSpin MRI GmbH";

#define DEBUG       0
#define DB_MODULE   0
#define DB_LINE_NR  0

#include "method.h"
#include <lib/PvSysinfoClient.h>

void backbone(void)
{

  double minFov[3];
  double readGradRatio;
  double minte;

  DB_MSG(("-->backbone"));

  STB_UpdateNuclei(No);

  /* calculate limits for read phase and slice */

  ReadGradLim     = 100.0;

  /* encoding */
  PVM_AntiAlias[0] = PVM_AntiAlias[1] = PVM_AntiAlias[2] = 1.0;
  STB_UpdateEncodingSpecial(3, 3, Yes);

  minFov[0] = FreqEncodingLimits( PVM_EncMatrix[0]/2,
				  PVM_AntiAlias,
				  &readGradRatio,
				  PVM_GradCalConst);

  /* constraint to isotropic Fov */
  minFov[2] = minFov[1] = minFov[0];

  /* update geometry parameters */
  UpdateGeometry(minFov);

  /* update excitation pulse */
  UpdateRFPulses();

  /* calculation of ReadGrad, PhaseGrad and SliceGrad */
  FreqEncodingGradients(PVM_Fov[0],PVM_Fov[1],PVM_Fov[2],PVM_GradCalConst);

  if(VarRampTimeYN == Yes)
  {
    double rampT, maxGrad;

    rampT=CFG_GradientRampTime();
    maxGrad=MAX_OF(MAX_OF(ReadGrad,PhaseGrad),SliceGrad);
    RampTime = rampT * maxGrad / EffSlewRate;
    ParxRelsShowInEditor("EffSlewRate");
  }
  else
  {
    int rampP = (int)ceil(CFG_GradientRampTime()/GradRes);
    RampTime = rampP * GradRes;
    ParxRelsHideInEditor("EffSlewRate");
  }

  ParxRelsMakeNonEditable("RampTime");

  double dwell = 1000 / PVM_EffSWh;

  if(RampCompYN==Yes)
  {
    double points= 0.5 * RampTime / dwell;
    RampPoints = (int) (0.5 + points);
    RampCompTime = RampPoints * dwell;
  }
  else
  {
    RampPoints = 0;
    RampCompTime = 0.0;
  }

  /* setting up dual-echo acquisition */
  SetDualEcho();

 /* setting number of radial projections */
  SetNPro();

  /* handling of modules */
  double spoilerThick = MIN_OF(MIN_OF(PVM_SpatResol[0]*PVM_EncZf[0],PVM_SpatResol[1]*PVM_EncZf[1]),PVM_SpatResol[2]*PVM_EncZf[2]);
  STB_UpdateDriftCompModule(spoilerThick, PVM_DeriveGains);
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, spoilerThick);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  STB_UpdateSatTransModule(PVM_DeriveGains);
  STB_UpdateTriggerModule();
  if(PVM_TriggerModule == On)
    PVM_TriggerMode = per_PhaseStep;

  /* update sequence timing */
  UpdateEchoTime(&minte);
  UpdateRepetitionTime();

  STB_UpdateDummyScans(PVM_RepetitionTime);

  STB_UpdateSegmentationModule(
     (PVM_TriggerModule == On || PVM_FatSupOnOff == On || PVM_FovSatOnOff == On ||PVM_SatTransOnOff == On || PVM_DriftCompYesNo == Yes) ? Yes : No,
     NPro * PVM_NAverages,
    PVM_RepetitionTime,
     PVM_TriggerModuleTime + PVM_FatSupModuleTime + PVM_FovSatModuleTime + PVM_SatTransModuleTime + PVM_DriftCompModuleTime);

  /* Set correct DriftCompUpdateTime after TR-upate */
  STB_UpdateDriftComp( PVM_SegmentationDur );

  UpdateTotalTime();

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  SetBaseLevelParam();

  /* update trajectory module */
  double gradAmp[3]={ReadGrad, PhaseGrad, SliceGrad};
  PostPoints = 20;
  double nPoints = PVM_EncMatrix[0]/2 + RampPoints + PostPoints;

  STB_UpdateTraj(PVM_SpatResol, PVM_EncZf, "PVM_SliceGeo", 3, NPro, nPoints, 0, dwell, gradAmp, PVM_TriggerModule, GradSync, 0.5, 0.0, GradRes, gradAmp);
  UpdateAdjustments();

  SetRecoParam();

  DB_MSG(("<--backbone"));
  return;
}

/*-------------------------------------------------------
 * local utility routines to simplify the backbone
 *------------------------------------------------------*/

void UpdateEchoTime(double *const minte)
{

  DB_MSG(("-->UpdateEchoTime"));

  /* set DE (time for ADC_INIT) to minimum */
  DE = SysInfo_MinPreScanDelay();

  *minte =
    ExcPulse1.Length/2  +
    DE / 1000.0; /* time for ADC_INIT */

  PVM_MinEchoTime = *minte;

  EchoTimeRange();

  DB_MSG(("<--UpdateEchoTime"));
  return;
}


void UpdateRepetitionTime(void)
{
  double shapeTime;
  double riseT;

  DB_MSG(("-->UpdateRepetitionTime"));

  /* adapt timing for sufficient spoiling */
  SpoilDur = 1e5*Spoiling/(ReadGrad*PVM_GradCalConst*PVM_SpatResol[0]);
  riseT = CFG_GradientRiseTime();

  shapeTime = RampTime + GradPlateau + SpoilDur + RampTime;

  if(DualEcho==Yes)
  {
    shapeTime += EchoSpacing;
  }

  PVM_MinRepetitionTime =
      0.07                                             +
      ExcPulse1.Length/2                               +
      PVM_EchoTime                                     +
      shapeTime                                        +
      riseT;


  PVM_RepetitionTime=MAX_OF(PVM_MinRepetitionTime,PVM_RepetitionTime);

  DB_MSG(("<--UpdateRepetitionTime"));
  return;
}

void UpdateTotalTime(void)
{
  double TotalTime;

  DB_MSG(("-->UpdateTotalTime"));

  TotalTime =  ( PVM_RepetitionTime * NPro * PVM_NAverages + PVM_SegmentationTotalTime )
              *  PVM_SatTransRepetitions * PVM_NRepetitions;

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<--UpdateTotalTime"));
  return;
}

void UpdateGeometry(double *minFov)
{

  ParxRelsHideInEditor("PVM_SPackArrPhase1Offset");
  ParxRelsMakeNonEditable("PVM_SPackArrPhase1Offset,PVM_SPackArrReadOrient");

  // force zero offsets in read phase and slice
  STB_ConstrainSlicePosition(true,true,true);



  
  if(!SysInfo_UseAts())
  {
    ParxRelsMakeNonEditable("PVM_SPackArrReadOffset,PVM_SPackArrSliceOffset");
    ParxRelsHideInEditor("PVM_SPackArrReadOffset,PVM_SPackArrSliceOffset");
    GTB_GeoObjFreezePos0("PVM_SliceGeoObj", No);
    GTB_GeoObjFreezePos1("PVM_SliceGeoObj", No);
    GTB_GeoObjFreezePos2("PVM_SliceGeoObj", No);
  }
  else
  {
    ParxRelsMakeEditable("PVM_SPackArrReadOffset,PVM_SPackArrSliceOffset");
    ParxRelsShowInEditor("PVM_SPackArrReadOffset,PVM_SPackArrSliceOffset");
    GTB_GeoObjFreezePos0("PVM_SliceGeoObj", Yes);
    GTB_GeoObjFreezePos1("PVM_SliceGeoObj", Yes);
    GTB_GeoObjFreezePos2("PVM_SliceGeoObj", Yes);

  }// end ATS handling 

  

  STB_UpdateImageGeometry(3, PVM_Matrix, minFov);

}

double FreqEncodingLimits( int dpoints,
			   double *antiAlias,
			   double *const readGradRatio,
			   double gradCalConst)
{
  double minFovRead = 0.0;


  DB_MSG(("-->FreqEncodingLimits"));

  MRT_UpdateDigPars( &PVM_EffSWh,
                      dpoints,
                      antiAlias,
                     &PVM_AcquisitionTime);

  *readGradRatio = 1.0;

  minFovRead     = MRT_MinReadFov(PVM_EffSWh,
				  *readGradRatio,
				  ReadGradLim,
				  100,
				  gradCalConst);


  DB_MSG(("<--FreqEncodingLimits"));

  return minFovRead;
}


void FreqEncodingGradients(double readFov,
			   double phaseFov,
                           double sliceFov,
			   double gradCalConst)
{
  DB_MSG(("-->FreqEncodingGradients"));

  ReadGrad     = MRT_ReadGrad(PVM_EffSWh,readFov,gradCalConst);
  PhaseGrad    = MRT_ReadGrad(PVM_EffSWh,phaseFov,gradCalConst);
  SliceGrad    = MRT_ReadGrad(PVM_EffSWh,sliceFov,gradCalConst);

  if(((ReadGrad - ReadGradLim) > 1.0e-3) || ((PhaseGrad -100)>1.0e-3) || ((SliceGrad -100)>1.0e-3))
  {

    UT_ReportError("FreqEncodingGradients: "
		   "Gradient exceeds its limits\n");

  }

  ReadGrad = MIN_OF(ReadGrad,ReadGradLim);
  PhaseGrad = MIN_OF(PhaseGrad,100);
  SliceGrad = MIN_OF(SliceGrad,100);

  DB_MSG(("<--FreqEncodingGradients"));
  return;
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


/*==============================================================
 * specific functions for radial imaging
 *==============================================================*/

/* calculate number of projections */

void CalcNPro(void)
{
  int i=0;
  double pi = M_PI;
  int size = PVM_EncMatrix[0];
  int n_phi=0;
  int n_theta=0;

  n_theta = (int) ((pi * size)/(2*ProUndersampling));
  for(i=0;i<n_theta;i++)
  {
    n_phi += (int) (pi*size*sin(pi*i/n_theta));
  }
  NPro = n_phi;
  DB_MSG(("NPro = %d", NPro));
}

void SetNPro(void)
{

  if (MinimumUnderSampling == Yes)
  {
    ProUndersampling = 1.0;
    ParxRelsMakeNonEditable("ProUndersampling");
  }
  else
  {
    ParxRelsMakeEditable("ProUndersampling");
  }

  /* initial calculation of NPro */
  CalcNPro();

  /* limit number of projections */
  int maxPro = PTB_MaxArraySize();

  if(NPro > maxPro)
  {
    /* adapt undersampling to maximum array size */
    ProUndersampling *= (double)NPro / (double)maxPro;
    CalcNPro();
  }

  /* apply memory restrictions */
  if (RecoOnline == Yes)
  {
    RecoRegridNSetDefaults = Off;

    ProUndersampling *= RecoOptimiseMemory(PVM_Matrix[0], NPro, PVM_EncMatrix[0]/2+RampPoints, PVM_EncNReceivers, &RecoRegridNOver);
    if ((ProUndersampling < 1.0) || (ProUndersampling > 10.0))
    {
      ProUndersampling=MAX_OF(MIN_OF(ProUndersampling,10.0),1.0);
      UT_ReportError("Reco memory optimisation failed! Try smaller matrix size.");
    }

    CalcNPro();

    DB_MSG(("ProUnderSampling = %.4f, NPro = %d", ProUndersampling, NPro));
  }
}

/* calculate radial projections */
int SetProj3D( double *r,
	       double *p,
	       double *s,
	       const double gr,
	       const double gp,
	       const double gs)
{
  int i,j, k=0;
  double pi,angle;
  int size = PVM_EncMatrix[0];
  int n_phi=0;
  int n_theta=0;

  pi=M_PI;
  angle = 2 * pi;
  n_theta = (int) ((pi * size)/(2*ProUndersampling));

  for(j=0; j<n_theta;j++)
  {
    //theta = (int) (pi*j/n_theta);
    n_phi = (int) (pi*size*sin(pi*j/n_theta));

    for(i=0; i<n_phi; i++)
    {
      r[i+k]=sin(pi*j/n_theta)*cos(angle*i/n_phi)*gr;
      p[i+k]=sin(pi*j/n_theta)*sin(angle*i/n_phi)*gp;
      s[i+k]=cos(pi*j/n_theta)*gs;
    }
    k += i;
  }
  return 0;
}


void UpdateAdjustments(void)
{
  DB_MSG(("-->UpdateAdjustments"));

  PTB_ClearAdjustments();
  STB_TrajAppendAdjustment("");
  PTB_AppendOrderAdjustment(per_scan, per_scan, RG_ADJNAME);

  DB_MSG(("<--UpdateAdjustments"));
}

void SetDualEcho(void)
{
  DB_MSG(("-->SetDualEcho"));

  if(DualEcho == No)
  {
    PVM_NEchoImages = 1;
    ParxRelsHideInEditor("FWInphase,EchoSpacing,MinEchoSpacing,MinStabDelay,StabDelay");
  }
  else
  {
    PVM_NEchoImages = 2;
    ParxRelsShowInEditor("FWInphase,EchoSpacing,MinEchoSpacing,MinStabDelay,StabDelay");
  }

  /* calculate shape points of gradient plateau on GradRes grid */
  double plateau = MAX_OF((PVM_AcquisitionTime - RampTime),0.0);
  GradPlateau = plateau + RampCompTime;
  int points = (int)ceil(GradPlateau/GradRes);
  GradPlateau = points * GradRes;

  /* calculate RewTime */
  double gradOnTime = GradPlateau + RampTime;
  double maxGrad = MAX_OF(MAX_OF(ReadGrad,PhaseGrad),SliceGrad);
  double minRewTime = gradOnTime * maxGrad / ReadGradLim ;
  RewTime = MAX_OF(minRewTime,2*RampTime); //at least one ramp time for plateau
  points = (int)ceil(RewTime/GradRes);
  RewTime = points * GradRes;

  MinEchoSpacing = gradOnTime + RewTime + 2*RampTime + MinStabDelay + 0.036;

  if(FWInphase==Yes)
  {
    double cshiftsp,mesp,dval;

    cshiftsp = fabs(STB_GetFreqOffset(1,4.7)-STB_GetFreqOffset(1,1.2));
    cshiftsp = 1e3/cshiftsp;

    /* find minimum possible echo spacing mesp that matches
       fat water inphase condition
    */

    dval=cshiftsp;
    mesp=MinEchoSpacing;
    while(dval < mesp) dval+=cshiftsp;
    mesp=dval;

    /* round to closest echospacing that is multiple of
       chemical shift related echospacing
    */
    double esp=EchoSpacing,csp2=cshiftsp/2;

    esp=MAX_OF(esp,mesp);

    for(dval=mesp;fabs(esp-dval)>csp2;dval+=cshiftsp);

    /* round to 1us accuracy */

    EchoSpacing=((int)(dval*1000.0 +0.5))/1000.0;

  }
  else
  {
   EchoSpacing = MAX_OF(MinEchoSpacing,EchoSpacing);
  }

  double extensionPossible = EchoSpacing - MinEchoSpacing;
  double extensionAllowed = gradOnTime - RewTime;

  DB_MSG(("ext possible = %f, allowed = %f", extensionPossible, extensionAllowed));

  double extension = MIN_OF(extensionPossible, extensionAllowed);
  extension = MAX_OF(extension, 0);

  DB_MSG(("extension = %f",extension));

  points = (int)floor(extension/GradRes);
  double extensionOnGrid = points*GradRes;
  RewTime += extensionOnGrid;
  StabDelay = extensionPossible - extensionOnGrid + MinStabDelay + 0.02;
  StabDelay = MAX_OF(StabDelay,0.02);

  DB_MSG(("extensionOnGrid = %f",extensionOnGrid));

  RewReadGrad  = gradOnTime * ReadGrad  / RewTime;
  RewPhaseGrad = gradOnTime * PhaseGrad / RewTime;
  RewSliceGrad = gradOnTime * SliceGrad / RewTime;

  PARX_change_dims("EffectiveTE",PVM_NEchoImages);
  EffectiveTE[0] = PVM_EchoTime;
  if(PVM_NEchoImages >1)
  {
    for(int i=1;i<PVM_NEchoImages;i++)
      EffectiveTE[i] = EffectiveTE[i-1] + EchoSpacing;

  }

  DB_MSG(("<--SetDualEcho"));
}
