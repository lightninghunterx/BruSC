/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2007
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 * ***************************************************************/


static const char resid[] = "$Id $ (c) 2007 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"

void backbone(void)
{
  double minFov[3];
  double minSliceThick;
  double readGradRatio,sliceGradRatio;
  double dval;
  int dim;

  DB_MSG(("-->backbone"));

  /* update nuclei parameter group                            */

  STB_UpdateNuclei(No);


  /* update encoding parameter group                          */
 
  STB_UpdateEncoding(
    NULL,               /* pointer to required segment size */
    SEG_SEQUENTIAL,     /* segmenting mode      */
    Yes,                /* allow PI in 2nd dim  */ 
    Yes,                /* allow PI ref lines in 2nd dim  */
    Yes,                /* partial ft in 2nd dim */
    &PVM_EchoPosition); /* partial ft in 1nd dim */


  /* update parameters controlling the data sampling period   */

  MRT_UpdateDigPars(&PVM_EffSWh,
                     PVM_EncMatrix[0],
                    &PVM_AntiAlias[0],
                    &PVM_AcquisitionTime);

  /* update excitation pulse                                  */  
  
  STB_UpdateRFPulse("ExcPulse1",1,PVM_DeriveGains,Conventional);
  
  
  /*  final handling of PVM_NRepetitions by range checker
   *  to be implemented as exercise, consider the
   *  relation redirection in callbackDefs.h                  */

  PVM_NRepetitions = 1;


  /* fix timing according to method specific constraints      */

  dval = EncGradDur +  CFG_GradientRiseTime();
  ReadSpoilGradDur = MAX_OF(dval, ReadSpoilGradDur);


  /* calculate limits for read phase and slice                */
  
  ReadDephGradLim       = 
    ReadGradLim         =
    Phase2DGradLim      = 
    Phase3DGradLim      =
    ExcSliceRephGradLim =
    ExcSliceGradLim     = 50.0;

  /* calculate minima for FOV and slice thickness             */

  UpdateGeometryMinima(minFov,
                       &minSliceThick,
                       &readGradRatio,
                       &sliceGradRatio);

 
  /* update geometry parameters                               */

  /* only one slice per package if 3D */
  dim=PTB_GetSpatDim();
  int maxPerPackage = dim>2? 1:0;

  STB_UpdateImageGeometry(dim, 
                          PVM_Matrix,
                          minFov,
                          0, /* total slices (no restr) */
                          0, /* total slice packages (no restr) */
                          maxPerPackage,
                          minSliceThick,
                          1.0); /* sliceFovRatio in 3D */


  /* calculate gradients in logical directions                */
  UpdateGradients(readGradRatio,sliceGradRatio);

  /* calculate frequency offsets                              */
  UpdateFrequencyOffsets();

  /*  update sequence timing                                  */
  UpdateEchoTime();
  UpdateRepetitionTime();

  PVM_NEchoImages = 1;

  SetBaseLevelParam();
  SetRecoParam();
  
  DB_MSG(("<--backbone"));

}

/*-------------------------------------------------------
 * local utility routines to simplify the backbone 
 *------------------------------------------------------*/

void UpdateEchoTime(void)
{
  double riseTime;
  
  DB_MSG(("-->UpdateEchoTime"));

  riseTime = CFG_GradientRiseTime();

  PVM_MinEchoTime = 
    ExcPulse1.Length/2                  +
    EncGradDur                          +
    riseTime                            + 
    PVM_AcquisitionTime * PVM_EchoPosition/100;

  PVM_EchoTime    = MAX_OF(PVM_MinEchoTime,PVM_EchoTime); 

  DB_MSG(("<--UpdateEchoTime"));
}



void UpdateRepetitionTime(void)
{
  int nslices, dim;
  double TotalTime,mindur,riseTime;

  DB_MSG(("-->UpdateRepetitionTime"));

  nslices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  riseTime = CFG_GradientRiseTime();
  
  mindur = EncGradDur + riseTime ;

  ReadSpoilGradDur=MAX_OF(mindur,ReadSpoilGradDur);

  PVM_MinRepetitionTime = 
    nslices *
    (
      0.01                                              +
      riseTime                                          +
      ExcPulse1.Length/2                                +
      PVM_EchoTime                                      +
      PVM_AcquisitionTime *(1.0 - PVM_EchoPosition/100) +
      ReadSpoilGradDur                                  +
      0.01    /* ADC_END */
    );

  PVM_RepetitionTime=MAX_OF(PVM_MinRepetitionTime,PVM_RepetitionTime);

  dim = PTB_GetSpatDim();

  TotalTime = PVM_RepetitionTime 
            * PVM_EncMatrix[1]
            * PVM_NAverages;
                
  if(dim == 3)
  {
    TotalTime *= PVM_EncMatrix[2];
  }

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<--UpdateRepetitionTime"));
}


void UpdateGeometryMinima( double *minfov,
                           double *minSliceThick,
                           double *readGradRatio,
                           double *sliceGradRatio)
{
  double riseTime,effEncGradDur,effSliceGradDur,effReadGradDur;
  int dim;

  DB_MSG(("-->UpdateGeometryMinima"));

  /* Retrieve system constants                                     */

  riseTime = CFG_GradientRiseTime();


  /* Calculate the minimum FOV in read direction
   * 
   * Step 1:  effective duration of encoding (read dephase, 
   * and slice rephase) gradient                                   */

  effEncGradDur = EncGradDur - riseTime;

  /* Step 2: normalized duration of the read gradient up to
   * the center of the echo.
   * Note: the value of PVM_AcquisitionTime has been calculated
   *       in backbone by MRT_UpdateDigPars                        */

  effReadGradDur = 0.5*riseTime + PVM_AcquisitionTime * PVM_EchoPosition/100.0;

  /* Step 3: Calculate the ratio of read and read-dephase gradient */

  *readGradRatio = effReadGradDur/effEncGradDur;

  /* Step 4: Calculate the minimum FOV in read direction
   * Note: PVM_EffSWh has been updated in backbone 
   *       by MRT_UpdateDigPars,
   *       PVM_GradCalConst has been updated in backbone
   *       by STB_UpdateNuclei                                     */

  minfov[0] = MRT_MinReadFov(PVM_EffSWh,
                             *readGradRatio,
                             ReadGradLim,
                             ReadDephGradLim,
                             PVM_GradCalConst);

  /* Calculate the minimum slice thickness
   * Step 1: calculate the effective exc slice gradient duration
   * Note: ExcPulse1 has been updated in backbone
   */

  effSliceGradDur = 
    ExcPulse1.Length        *  /* dephasing of               */ 
    ExcPulse1.Rpfac/100     +  /* ExcPulse1                  */
    riseTime/2.0;              /* dephase of ramp down       */


  /* Step 2: Calculate ratio between slice and slice-rephase Grad */

  *sliceGradRatio = effSliceGradDur/effEncGradDur;

  /* Step 3: Calculate min slice thickness                        */
  
  *minSliceThick = MRT_MinSliceThickness(ExcPulse1.Bandwidth,
                                         *sliceGradRatio,
                                         ExcSliceGradLim,
                                         ExcSliceRephGradLim,
                                         PVM_GradCalConst);
  
  /* Calculate min FOV in 2D phase encoding direction             */

  minfov[1] = MRT_PhaseFov(effEncGradDur,
                           PVM_Matrix[1],
                           Phase2DGradLim,
                           PVM_GradCalConst);
  
  dim=PTB_GetSpatDim();
  
  /* Calculate min FOV in 3D phase encoding direction
   * Note: min FOV in 3rd direction and min slice thickness
   *       are bound together.                                    */

  if(dim==3)
  {
    double min;
    
    min = MRT_PhaseFov(effEncGradDur,
                       PVM_Matrix[2],
                       Phase3DGradLim,
                       PVM_GradCalConst);
    minfov[2] = MAX_OF(min,*minSliceThick);
    *minSliceThick = minfov[2];
  }

  DB_MSG(("<--UpdateGeometryMinima"));
}

void UpdateGradients(double readGradRatio,
                     double sliceGradRatio)
{
  double effEncGradDur;
  DB_MSG(("-->UpdateGradients"));

  /* Calculation of read phase and slice gradients,
   * Note: PVM_Fov and PVM_SliceThick have been updated 
   *       in backbone by call of 
   *       STB_UpdateStandardInplaneGeoPars and
   *       STB_UpdateSliceGeoPars with minimum values calculated
   *       in UpdateGeometryMinima before.                       */


  ReadGrad = MRT_ReadGrad(PVM_EffSWh,
                          PVM_Fov[0],
                          PVM_GradCalConst);

  ReadDephGrad = readGradRatio*ReadGrad;

  ExcSliceGrad = MRT_SliceGrad(ExcPulse1.Bandwidth,
                               PVM_SliceThick,
                               PVM_GradCalConst);

  ExcSliceRephGrad = sliceGradRatio * ExcSliceGrad;

  effEncGradDur=EncGradDur-CFG_GradientRiseTime();

  Phase2DGrad = MRT_PhaseGrad(effEncGradDur,
                              PVM_Matrix[1],
                              PVM_Fov[1],
                              PVM_GradCalConst);

  if(PTB_GetSpatDim() == 3)
  {
    Phase3DGrad = MRT_PhaseGrad(effEncGradDur,
                                PVM_Matrix[2],
                                PVM_Fov[2],
                                PVM_GradCalConst);
  }
  else
  {
    Phase3DGrad = 0.0;
  }

  DB_MSG(("<--UpdateGradients"));
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
                          ExcSliceGrad,
                          PVM_GradCalConst,
                          PVM_SliceOffsetHz );

  if(spatDim == 3)
  {
    for(i=0;i<nslices;i++)
      PVM_EffPhase2Offset[i] = -PVM_EffSliceOffset[i];
  }

  DB_MSG(("<--UpdateFrequencyOffsets"));
}





