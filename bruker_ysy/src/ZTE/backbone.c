/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2004 - 2011
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

#define DEBUG       0
#define DB_MODULE   0
#define DB_LINE_NR  0

#include "method.h"
#include <libCore/Math/Math.hh>
#include <lib/PvSysinfoClient.h>


void backbone(void)
{
  DB_MSG(("-->backbone"));
  
  /* nucleus */

  STB_UpdateNuclei(No);

  /* encoding */

  UpdateEncoding();

  /* bandwidth */
  
  UpdateBandwidth();

  /* RF pulse */

  UpdateRFPulses();

  /* geometry */
  
  UpdateGeometry();
  
  /* read gradient */
  
  UpdateGradients();

  /* calculate number of projections */
  
  UpdateProjections();
  
  /* sequence timing */

  UpdateAcqDelay();
  UpdateRepetitionTime();
  
  /* modules */

  STB_UpdateDummyScans(PVM_RepetitionTime);
  PVM_TriggerMode = per_PhaseStep;
  ParxRelsHideInEditor("PVM_TriggerMode");
  STB_UpdateTriggerModule();
  double spoilerThick = MIN_OF(MIN_OF(PVM_SpatResol[0]*PVM_EncZf[0],PVM_SpatResol[1]*PVM_EncZf[1]),PVM_SpatResol[2]*PVM_EncZf[2]); 
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, spoilerThick);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  
  STB_UpdateSegmentationModule(
    (PVM_TriggerModule == On || PVM_FatSupOnOff == On || PVM_FovSatOnOff == On) ? Yes : No,
    NPro * PVM_NAverages,
    PVM_RepetitionTime,
    PVM_TriggerModuleTime + PVM_FatSupModuleTime + PVM_FovSatModuleTime);
  
  STB_UpdateEvolutionModule();

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  UpdateTotalTime();
  
  /* baselevel acquisition */
  
  SetBaseLevelParam();
  
  /* reco */
  
  SetRecoParam();

  DB_MSG(("<--backbone"));
}


void UpdateEncoding(void)
{
  DB_MSG(("-->UpdateEncoding"));
    
  PVM_AntiAlias[0] = 1.0;
  PVM_AntiAlias[1] = 1.0;
  PVM_AntiAlias[2] = 1.0;
  
  STB_UpdateEncodingSpecial(3, 3, Yes);

  DB_MSG(("<--UpdateEncoding"));
}    

void UpdateBandwidth()
{
  DB_MSG(("-->UpdateBandwidth"));
  
  /* in case of a user input: set bandwidth to next possible value in indicated direction */
    
  // get last value
  double BandWidthLast = PVM_EffSWh;
  YesNo BandWidthChanged = UT_GetRequest("PVM_EffSWh");
  if (BandWidthChanged == Yes)
  {
    UT_RequestParOrigValue(&BandWidthLast);
    UT_ClearRequest();
  }
  
  // determine step size
  double BandWidthStep = PVM_EffSWh - BandWidthLast;
    
  if (fabs(BandWidthStep) > 0.0)
  {
    // demand a minimum step size
    BandWidthStep = MAX_OF(fabs(BandWidthStep), BandWidthLast * 1e-3) * (BandWidthStep > 0.0 ? 1.0 : -1.0);
    DB_MSG(("BandWidth = %f, BandWidthLast = %f, BandWidthStep = %f", PVM_EffSWh, BandWidthLast, BandWidthStep));
  }

  // number of readout points    
  NPoints = (int)((double)PVM_EncMatrix[0] * 0.5 + 0.5) * OverSampling;
    
  // find next value in requested direction
  const double BandWidthMin = 1e3;
  const double BandWidthMax = 1e6;
  double BandWidthNew = BandWidthLast;
  
  for (;;)
  {
    BandWidthNew += BandWidthStep;
    BandWidthNew = MIN_OF(MAX_OF(BandWidthNew, BandWidthMin), BandWidthMax);
    DB_MSG(("BandWidthNew = %f", BandWidthNew));

    PVM_EffSWh = BandWidthNew;
    double oversampling = (double)OverSampling;
    MRT_UpdateDigPars(&PVM_EffSWh, NPoints, &oversampling, &PVM_AcquisitionTime);

    if (BandWidthChanged == No                 || 
        fabs(BandWidthStep) == 0.0             || 
        fabs(PVM_EffSWh - BandWidthLast) > 0.0 || 
        BandWidthNew <= BandWidthMin           || 
        BandWidthNew >= BandWidthMax) break;
  }
    
  DB_MSG(("<--UpdateBandwidth"));
}

void UpdateGeometry(void)
{
  DB_MSG(("-->UpdateGeometry"));
  
  double minFov[3];

  minFov[0] = MRT_Fov(PVM_EffSWh,
                      100.0,
                      PVM_GradCalConst);
  minFov[1] = minFov[0];
  minFov[2] = minFov[0];



  STB_UpdateImageGeometry(3, PVM_Matrix, minFov);
  STB_UpdateAtsReferencePosition("PVM_SliceGeoObj");
  
  ParxRelsHideInEditor("PVM_SliceThick");
                            
  DB_MSG(("<--UpdateGeometry"));
}

void UpdateGradients(void)
{
  DB_MSG(("-->UpdateGradients"));
  
  GradAmpR =  MRT_ReadGrad(PVM_EffSWh, PVM_Fov[0], PVM_GradCalConst);
  GradAmpP =  MRT_ReadGrad(PVM_EffSWh, PVM_Fov[1], PVM_GradCalConst);
  GradAmpS = -MRT_ReadGrad(PVM_EffSWh, PVM_Fov[2], PVM_GradCalConst);
  /* negative slice gradient is according to Bruker convention */
  
  ReadGrad = MAX_OF(MAX_OF(fabs(GradAmpR), fabs(GradAmpP)), fabs(GradAmpS));
  
  DB_MSG(("<--UpdateGradients"));
}

void UpdateProjections(void)
{
  DB_MSG(("-->UpdateProjections"));
  
  if (MinimumUnderSampling == Yes)
  {
    ProUnderSampling = 1.0;
    ParxRelsMakeNonEditable("ProUnderSampling");
  }
  else
  {
    ParxRelsMakeEditable("ProUnderSampling");
  }
    
  /* half acquisition requires zero order phase */
  if (HalfAcquisition == Yes)
  {
    UseOrigin = Yes;
  }
  
  /* calculate number of projections */
  for (;;)
  {
    NPro = radialGrad3D(PVM_EncMatrix[0], ProUnderSampling, HalfAcquisition, UseOrigin, Reorder, NULL, NULL, NULL);
    DB_MSG(("ProUnderSampling = %.4f, NPro = %d", ProUnderSampling, NPro));
        
    /* adapt undersampling to maximum array size */
    if (NPro <= PTB_MaxArraySize()) break;
    ProUnderSampling *= (double)NPro / (double)PTB_MaxArraySize();
  }
  
  /* apply memory restrictions */  
  if (RecoOnline == Yes)
  {
    RecoRegridNSetDefaults = Off;
    
    ProUnderSampling *= RecoOptimiseMemory(PVM_Matrix[0], NPro, NPoints / OverSampling, OverSampling, PVM_EncNReceivers, UseOrigin, &RecoRegridNOver);
    if ((ProUnderSampling < 1.0) || (ProUnderSampling > 10.0))
    {
      ProUnderSampling=MAX_OF(MIN_OF(ProUnderSampling,10.0),1.0);
      UT_ReportError("Reco memory optimisation failed! Try smaller matrix size.");
    }
    NPro = radialGrad3D(PVM_EncMatrix[0], ProUnderSampling, HalfAcquisition, UseOrigin, Reorder, NULL, NULL, NULL);
    DB_MSG(("ProUnderSampling = %.4f, NPro = %d", ProUnderSampling, NPro));
  }
  
  NShots = (HalfAcquisition ? 1 : 2) + (UseOrigin ? 1 : 0);
  DB_MSG(("NShots = %d", NShots));
    
  DB_MSG(("<--UpdateProjections"));
}

void UpdateAcqDelay()
{
  DB_MSG(("-->UpdateAcqDelay"));
  
  /* set DE (time for ADC_INIT) to minimum */
  DE = SysInfo_MinPreScanDelay();
  
  AcqDelayTotal = ExcPul.Length * MSEC_TO_USEC * 0.5 + DE + AcqDelay;
  
  DB_MSG(("<--UpdateAcqDelay"));
}

void UpdateRepetitionTime(void)
{
  DB_MSG(("-->UpdateRepetitionTime"));
  
  const double spoilFactor = PVM_EffSWh * HZ_TO_KHZ / PVM_Matrix[0]; // cycles per pixel per ms
  
  RiseTime = CFG_GradientRiseTime();

  const double baseTime =
      0.01                                     +
      ExcPul.Length                            +
      AcqDelay                 * USEC_TO_MSEC  +
      DE                       * USEC_TO_MSEC  +
      PVM_AcquisitionTime                      +                             
      0.005                                    +             
      EndOfScan                                +
      RiseTime                                 ;
  
  GradSettle = RiseTime; // allow at least one rise time for gradient settling

  if (MinimumTR == Yes)
  {
    PVM_RepetitionTime = 0.0;
    ParxRelsMakeNonEditable("PVM_RepetitionTime");
    ParxRelsAddConflictSource(NULL, "PVM_RepetitionTime");
  }
  else
  {
    ParxRelsMakeEditable("PVM_RepetitionTime");
  }

  double gradOnTime;
  
  for (;;)
  {
    PVM_MinRepetitionTime =
      CFG_InterGradientWaitTime()              +
      RiseTime                                 +    
      GradSettle                               +
      baseTime                                 ;

    PVM_RepetitionTime = MAX_OF(PVM_MinRepetitionTime, PVM_RepetitionTime);
  
    if (GradOff == No)
    {
      gradOnTime = PVM_RepetitionTime;
    }
    else
    {
      gradOnTime = GradSettle + baseTime; // count rise time only once
    }
    
    /* adapt timing for sufficient spoiling */
        
    double dSpoil = Spoiling / spoilFactor - gradOnTime;
    
    if (dSpoil / gradOnTime < 1e-3) break;
    
    if (GradOff == Yes)
    {
      GradSettle += dSpoil;
    }
    else
    {
      PVM_RepetitionTime += dSpoil;
    }
    DB_MSG(("increase TR by %.3f ms for spoiling", dSpoil));
  }

  TRWait = PVM_RepetitionTime                  -
      // InterGradientWaitTime is included
      RiseTime                                 - 
      GradSettle                               - 
      baseTime                                 ; 
  
  GradDutyCycle = gradOnTime / PVM_RepetitionTime * pow(ReadGrad * 1e-2, 2.0) * GradMaxDutyCycle;
  
  DB_MSG(("<--UpdateRepetitionTime"));
}

void UpdateTotalTime(void)
{
  DB_MSG(("-->UpdateTotalTime"));
  
  PVM_ScanTime = PVM_RepetitionTime * NPro * PVM_NAverages + PVM_SegmentationTotalTime;
  
  if (PVM_EvolutionOnOff == On)
  {
    PVM_ScanTime += RiseTime + PVM_RepetitionTime * PVM_DummyScans;
  }
  OneRepTime = PVM_ScanTime * 1e-3;
  
  PVM_ScanTime = PVM_ScanTime * PVM_EvolutionCycles + PVM_EvolutionModuleTime;
  PVM_ScanTime *= PVM_NRepetitions;
  UT_ScanTimeStr(PVM_ScanTimeStr, PVM_ScanTime);
  
  PVM_EchoTime      = 0.0;
  PVM_InversionTime = 0.0;
  
  DB_MSG(("<--UpdateTotalTime"));
}
