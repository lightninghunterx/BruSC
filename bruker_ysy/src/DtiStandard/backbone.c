/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2003-2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/


static const char resid[] = "$Id$ (C) 2003-2007 Bruker BioSpin MRI GmbH";

#define DEBUG    0
#define DB_MODULE  0
#define DB_LINE_NR  0


#include "method.h"


void backbone(void)
{

  double minFov[3];
  double sliceThick,minSliceThick;
  double readGradRatio,sliceGradRatio;
  double minte1, minte2,prepEchoTime;
  int dim,status;


  DB_MSG(("-->backbone\n"));


  /* update nuclei parameter group                            */
  STB_UpdateNuclei(No);

   /* update encoding parameter group                          */
  STB_UpdateEncoding(
    NULL,
    SEG_SEQUENTIAL,
    Yes,
    Yes,
    Yes);

  PVM_EchoPosition = 50;

  /* update excitation pulse                                  */
  UpdateRFPulses();


  /* fix timing according to method specific constraints */
  EncGradDurRange();

  ReadDephTime = EncGradDur;

  /* allow higher gradient amplitudes for non oblique slices */
  ControlGradientLimits(PVM_MajSliceOri);


  /* calculate limits for read phase and slice */

  minFov[0] = FreqEncodingLimits( PVM_Matrix[0],
                                  PVM_AntiAlias,
                                  &readGradRatio,
                                  PVM_GradCalConst);


  minFov[1] = Phase2DEncodingLimits(PVM_Matrix[1],
                                    PVM_GradCalConst);

  dim = PTB_GetSpatDim();

  if(dim == 3)
  {
    minFov[2] = Phase3DEncodingLimits(PVM_Matrix[2],
                                      PVM_GradCalConst);
  }
  else
  {
    minFov[2] = 1e-6;
  }


  minSliceThick = SliceSelectionLimits(&ExcPulse1,
                                       PVM_GradCalConst,
                                       &sliceGradRatio);

  if(dim == 3)
  {
    double min;

    min=MAX_OF(minFov[2],minSliceThick);
    minFov[2] = minSliceThick = min;

  }



  sliceThick = MAX_OF(PVM_SliceThick,minSliceThick);

  UpdateMinEchoTime(&minte1,&minte2);
  prepEchoTime = PVM_EchoTime;

  status = STB_UpdateDiffusionPreparation(&sliceThick,
                                          PVM_DeriveGains,
                                          PVM_Nucleus1,
                                          PtrType3x3 PVM_SPackArrGradOrient[0],
                                          minte1,
                                          minte2,
                                          &prepEchoTime);

  if(status == 1)
  {
    DB_MSG(("DIFFUSION UPDATER: changed min slice thickness"));
    minSliceThick=sliceThick;
    if(dim==3)
    {
      minFov[2]=minSliceThick;
    }
  }

  PVM_NEchoImages = PVM_DwNDiffExp;

  /* update geometry parameters                               */

  // only one package with one slice if 3D
  int maxPerPackage = (dim>2) ? 1:0;


  STB_UpdateAtsReferencePosition("PVM_SliceGeoObj");
  STB_UpdateImageGeometry(dim,
                          PVM_Matrix,
                          minFov,
                          0, // total slices (no restr)
                          1, // max packages constrained to 1
                          maxPerPackage,
                          minSliceThick,
                          1.0); // sliceFovRatio in 3D



  FreqEncodingGradients(PVM_Fov[0],readGradRatio,PVM_GradCalConst);
  UpdatePhase2DGradients(PVM_Matrix[1],PVM_Fov[1],PVM_GradCalConst);

  if(dim == 3)
  {
    UpdatePhase3DGradients(PVM_Matrix[2],PVM_Fov[2],PVM_GradCalConst);
  }

  UpdateSliceSelectionGradients(PVM_SliceThick,
                                sliceGradRatio,
                                ExcPulse1.Bandwidth,
                                PVM_GradCalConst);


  /* handling of modules */
  double spoilerThick = dim>2 ? PVM_SpatResol[2]*PVM_EncZf[2] : PVM_SliceThick;
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, spoilerThick);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);

  /* calculate frequency offsets */
  LocalFrequencyOffsetRels();

  /* update sequence timing */

  PVM_MinEchoTime =  prepEchoTime;
  PVM_EchoTime = MAX_OF(PVM_EchoTime,PVM_MinEchoTime);
  PVM_EchoTime1 =PVM_EchoTime;

  /* Dummy Scans                                              */
  STB_UpdateDummyScans(PVM_RepetitionTime);

  UpdateRepetitionTime();

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");



  SetBaseLevelParam();
  SetRecoParam();


  DB_MSG(("<--backbone\n"));
  return;
}

/*-------------------------------------------------------
 * local utility routines to simplify the backbone
 *------------------------------------------------------*/

void UpdateMinEchoTime(double *const minte1, double *const minte2)
{
  double riseTime, igwt;


  DB_MSG(("-->UpdateEchoTime\n"));

  riseTime = CFG_GradientRiseTime();
  igwt     = CFG_InterGradientWaitTime();

  *minte1 =
    ExcPulse1.Length/2                  +
    riseTime   + igwt                   + /* min TE1/2 filling delay */
    ExcSliceRephTime                    +
    igwt;

  *minte2 =
    ReadDephTime                           +
    igwt                                   +
    riseTime                               + /* min TE2/2 filling delay */
    PVM_AcquisitionTime * PVM_EchoPosition/100;


  DB_MSG(("<--UpdateEchoTime\n"));
  return;
}



void UpdateRepetitionTime(void)
{
  int nslices, dim;
  double TotalTime,triggPhase=0,triggSlice=0,slrept,igwT;

  DB_MSG(("-->UpdateRepetitionTime\n"));

  nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  triggPhase = STB_UpdateTriggerModule();
  igwT = CFG_InterGradientWaitTime();


  switch(PVM_TriggerMode)
  {
    default:
    case per_Slice:
      triggSlice = triggPhase;
      triggPhase   = 0.0;
      break;
    case per_PhaseStep:
      triggSlice = 0.0;
  }

  slrept =
    0.03                                             +  /* UpdateDynPars */
    0.01                                             +  /* freqRx */
    triggSlice                                       +
    PVM_FatSupModuleTime                             +
    PVM_FovSatModuleTime                             +
    CFG_GradientRiseTime()                           +
    ExcPulse1.Length/2                               +
    PVM_EchoTime                                     +
    PVM_DwModDur - PVM_DwModEchDel                   +
    PVM_AcquisitionTime *(1.0 - PVM_EchoPosition/100)+
    ReadDephTime                                     +
    2*igwT;

  PVM_MinRepetitionTime = slrept * nslices +triggPhase ;


  PVM_RepetitionTime=MAX_OF(PVM_MinRepetitionTime,PVM_RepetitionTime);

  dim = PTB_GetSpatDim();

  TotalTime = PVM_RepetitionTime
            * PVM_EncMatrix[1]
            * PVM_NAverages *PVM_DwNDiffExp*PVM_NRepetitions;

  if(dim == 3)
  {
    TotalTime *= PVM_EncMatrix[2];
  }

  /* time for one repetition */
  OneRepTime = TotalTime/(PVM_NRepetitions*PVM_DwNDiffExp*1000.0);

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<--UpdateRepetitionTime\n"));
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

double SliceSelectionLimits(PVM_RF_PULSE *const excPulse,
          const double gradCalConst,
          double *const sliceRatio)

{

  double  minSlThk;
  double  sliceIntegral,rephIntegral;
  double  riseT;

  DB_MSG(("-->SliceSelectionLimits\n"));


  /* range check of arguments */

  STB_CheckRFPulse("excPulse");


  /* calculate exc slice integrals and minSlThk */

  riseT = CFG_GradientRiseTime();
  ExcSliceRephTimeRange();


  sliceIntegral =
    excPulse->Length              *
    excPulse->Rpfac/100.0         +/* dephasing of excPulse */
    + riseT/2.0    ;               /* dephasing of ramp down*/

  rephIntegral  = ExcSliceRephTime - riseT;

  *sliceRatio    = sliceIntegral / rephIntegral;

  DB_MSG(("SLICE RATIO: %f",*sliceRatio));

  minSlThk = MRT_MinSliceThickness(excPulse->Bandwidth,
           *sliceRatio,
           ExcSliceGradLim,
           ExcSliceRephGradLim,
           gradCalConst);

  DB_MSG(("<--SliceSelectionLimits\n"));
  return minSlThk;
}

double FreqEncodingLimits( int dpoints,
         double *antiAlias,
         double *const readGradRatio,
         double gradCalConst)
{
  double minFovRead = 0.0;
  double riseTime;
  double readDephInteg;
  double readInteg;


  DB_MSG(("-->FreqEncodingLimits\n"));


  *antiAlias = MAX_OF(1.0,*antiAlias);

  riseTime      = CFG_GradientRiseTime();


  /* Calculate the normalised integral of the read dephase gradient */
  readDephInteg = ReadDephTime - riseTime;


  /* Set the effective sweep width to a legal value */
  MRT_UpdateDigPars(&PVM_EffSWh, dpoints, antiAlias, &PVM_AcquisitionTime);


  /* Calculate the normalised integral of the read gradient */
  readInteg     = 0.5 * riseTime + PVM_AcquisitionTime * PVM_EchoPosition/100.0;
  *readGradRatio = readInteg/readDephInteg;


  /* Calculate the minimum field of view in the read direction */
  minFovRead = MRT_MinReadFov(PVM_EffSWh,
          *readGradRatio,
          ReadGradLim,
          ReadDephGradLim,
          gradCalConst);

  DB_MSG(("readInteg = %f\n"
    "readDephInteg = %f\n"
    "readGradRatio = %f\n"
    "minFovRead = %f\n",readInteg,readDephInteg,
    *readGradRatio,minFovRead));

  DB_MSG(("<--FreqEncodingLimits\n"));

  return minFovRead;
}

double Phase2DEncodingLimits( int dpoints,
            double gradCalConst)
{

  DB_MSG(("-->Phase2DEncodingLimits\n"));


  double phaseInteg = EncGradDur - CFG_GradientRiseTime();


  DB_MSG(("<--Phase2DEncodingLimits\n"));

  return MRT_PhaseFov(phaseInteg,dpoints,Phase2DGradLim,gradCalConst);

}

double Phase3DEncodingLimits( int dpoints,
            double gradCalConst)
{

  DB_MSG(("-->Phase3DEncodingLimits\n"));


  double phaseInteg = EncGradDur - CFG_GradientRiseTime();


  DB_MSG(("<--Phase3DEncodingLimits\n"));

  return MRT_PhaseFov(phaseInteg,dpoints,Phase3DGradLim,gradCalConst);

}

void FreqEncodingGradients(double readFov,
         double readGradRatio,
         double gradCalConst)
{
  DB_MSG(("-->FreqEncodingGradients\n"));

  ReadGrad     = MRT_ReadGrad(PVM_EffSWh,readFov,gradCalConst);
  ReadDephGrad = MRT_ReadDephaseGrad(readGradRatio,ReadGrad);

  if((ReadGrad - ReadGradLim) > 1.0e-3)
  {

    UT_ReportError("FreqEncodingGradients: "
       "ReadGrad exceeds its limits\n");

  }

  ReadGrad = MIN_OF(ReadGrad,ReadGradLim);

  if((ReadDephGrad - ReadDephGradLim) > 1.0e-3)
  {
    UT_ReportError("FreqEncodingGradients: "
       "ReadDephGrad exceeds its limits\n");

  }

  ReadDephGrad = MIN_OF(ReadDephGrad,ReadDephGradLim);


  DB_MSG(("<--FreqEncodingGradients\n"));
  return;
}

void UpdatePhase2DGradients(int dpoints,
          double fov,
          double gradCalConst)
{
  DB_MSG(("-->UpdatePhase2DGradients\n"));


  Phase2DGrad = MRT_PhaseGrad( EncGradDur - CFG_GradientRiseTime(),
             dpoints,
             fov,
             gradCalConst );

  if((Phase2DGrad - Phase2DGradLim) > 1.0e-3)
  {
    UT_ReportError("UpdatePhase2DGradients: "
       "Phase2DGrad exceeds its limits\n");

  }

  Phase2DGrad = MIN_OF(Phase2DGrad,Phase2DGradLim);

  DB_MSG(("<--UpdatePhase2DGradients\n"));

  return;

}

void UpdatePhase3DGradients(int dpoints,
          double fov,
          double gradCalConst)
{
  DB_MSG(("-->UpdatePhase3DGradients\n"));


  Phase3DGrad = MRT_PhaseGrad( EncGradDur - CFG_GradientRiseTime(),
             dpoints,
             fov,
             gradCalConst );

  if((Phase3DGrad - Phase3DGradLim) > 1.0e-3)
  {
    UT_ReportError("UpdatePhase3DGradients: "
       "Phase3DGrad exceeds its limits\n");

  }

  Phase3DGrad = MIN_OF(Phase3DGrad,Phase3DGradLim);

  DB_MSG(("<--UpdatePhase3DGradients\n"));

  return;

}

void ControlGradientLimits(YesNo NotOblique)
{

  int dim = PTB_GetSpatDim();

  if(NotOblique == Yes)
  {
    ReadGradLim = ReadDephGradLim = Phase2DGradLim =  Phase3DGradLim = 100.0;
  }
  else
  {
    if(dim==3)
    {
      ReadGradLim = ReadDephGradLim = Phase2DGradLim = Phase3DGradLim = 57.0;
    }
    else
    {
      ReadGradLim = ReadDephGradLim = Phase2DGradLim = 70.0;
    }
  }

  ExcSliceGradLim = ExcSliceRephGradLim = 100.0;
}

YesNo UpdateSliceSelectionGradients(const double slthk,
            const double sliceRatio,
            const double excPulseBW,
            double gradCalConst)
{
  DB_MSG(("-->UpdateSliceSelectionGradients\n"));

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
       "Illegal value of argument 5\n");
    return No;

  }

  /* update of slice gradients */

  ExcSliceGrad     = MRT_SliceGrad(excPulseBW,slthk,gradCalConst);
  ExcSliceRephGrad = sliceRatio * ExcSliceGrad;



  if((ExcSliceGrad - ExcSliceGradLim)>1.0e-3)
  {
    UT_ReportError("UpdateSliceSelectionGradients: "
       "ExcSliceGrad exceeded maximum!\n");
    return No;

  }

  ExcSliceGrad = MIN_OF(ExcSliceGrad,ExcSliceGradLim);


  if((ExcSliceRephGrad - ExcSliceRephGradLim)>1.0e-3)
  {
    UT_ReportError("UpdateSliceSelectionGradients: "
       "ExcSliceRephGrad exceeded maximum!\n");
    return No;

  }

  ExcSliceRephGrad = MIN_OF(ExcSliceRephGrad,ExcSliceRephGradLim);

  DB_MSG(("-->UpdateSliceSelectionGradients\n"));
  return Yes;
}
