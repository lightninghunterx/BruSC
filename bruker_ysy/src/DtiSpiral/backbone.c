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

#define DEBUG       0
#define DB_MODULE   0
#define DB_LINE_NR  0

#include "method.h"


void backbone(void)
{
  DB_MSG(("-->backbone"));

  /* nucleus */

  STB_UpdateNuclei(No);

  /* encoding */

  UpdateEncoding();

  /* get valid bandwidth */

  MRT_UpdateDigPars( &PVM_EffSWh,
                      PVM_EncMatrix[0] * PVM_EncMatrix[1],
                      NULL,
                      NULL
                   );

  /* excitation pulse */

  UpdateRFPulses();

  /* gradients & geometry */

  UpdateGradientLimits();
  UpdateGeometry();
  UpdateSliceGradients();

  /* spiral */

  STB_UpdateSpiral(PVM_EncMatrix[0], PVM_EffSWh, PVM_Fov[0]);

  double gradMax[2] = {PVM_SpiralGradientLimit, PVM_SpiralGradientLimit};

  STB_UpdateTraj(PVM_SpatResol, PVM_EncZf, "PVM_SliceGeo", 2, PVM_SpiralNbOfInterleaves,
                 PVM_SpiralNbOfAcqPoints, PVM_SpiralNavSize, PVM_SpiralAcqDwellTime, gradMax,
                 PVM_TriggerModule, GradSync, 0.5, CFG_GradientRiseTime(), PVM_SpiralGradDwellTime * 1e-3);

  /* frequency offsets */

  UpdateFrequencyOffsets();

  /* modules */
  int dim=PTB_GetSpatDim();
  double spoilerThick = dim>2 ? PVM_SpatResol[2]*PVM_EncZf[2] : PVM_SliceThick;
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, spoilerThick);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  STB_UpdateTaggingModule(dim,PVM_Fov,PVM_Matrix,PVM_SpatResol[0]*PVM_EncZf[0],PVM_DeriveGains);
  STB_UpdateTriggerModule();
  STB_UpdateTriggerOutModule();

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");

  /* minimum TE */
  UpdateMinEchoTime();

  /* diffusion */
  double prepEchoTime = PVM_EchoTime;
  double sliceThick = MAX_OF(PVM_SliceThick,MinSliceThick);

  int status = STB_UpdateDiffusionPreparation(&sliceThick,
					      PVM_DeriveGains,
					      PVM_Nucleus1,
					      PtrType3x3 PVM_SPackArrGradOrient[0],
					      PVM_MinEchoTime,
					      PVM_SpiralEchoTime,
					      &prepEchoTime);

  if(status == 1)
  {
    MinSliceThick = sliceThick;
    /* update geometry again */
    if (PTB_GetSpatDim() == 3)
    {
      /* constrain to one package with one slice */
      MinFov[2] = MinSliceThick;
      STB_UpdateImageGeometry(3, PVM_Matrix, MinFov, 0, 1, 1, MinSliceThick, 1.0);
    }
    else
    {
      /* constrain to one package */
      STB_UpdateImageGeometry(2, PVM_Matrix, MinFov, 0, 1, 0, MinSliceThick);
    }
  }

  /* sequence timing */
  UpdateEchoTime(prepEchoTime);
  UpdateRephaseGradients();
  UpdateRepetitionTime();
  STB_UpdateDummyScans(PVM_RepetitionTime);

  /* adjustments */

  UpdateAdjustments();

  /* baselevel */

  SetBaseLevelParam();
  SetRecoParam();

  DB_MSG(("<--backbone"));
  return;
}


/*-------------------------------------------------------
 * local utility routines
 *------------------------------------------------------*/

void UpdateEncoding(void)
{
  DB_MSG(("-->UpdateEncoding"));

  SEG_MODE seg_mode       = SEG_SEQUENTIAL;
  const YesNo ppiAllow    = Yes;
  const YesNo ppiRefAllow = Yes;
  const YesNo zfAllow     = Yes;
  const YesNo autoStart   = Yes;

  STB_UpdateEncodingSpecial(
      2,
      2,
      Yes,
      NULL,
      &seg_mode,
      &ppiAllow,
      &ppiRefAllow,
      NULL,
      &zfAllow,
      &autoStart);

  DB_MSG(("<--UpdateEncoding"));
}

void UpdateGradientLimits(void)
{
  DB_MSG(("-->UpdateGradientLimits"));

  SliceRephGradLim = PTB_GetSpatDim() > 2 ? 30.0 : 100.0;
  Phase3DGradLim   = 100.0 - SliceRephGradLim;
  SliceGradLim     = 100.0;

  DB_MSG(("<--UpdateGradientLimits"));
}

void UpdateGeometry(void)
{
  DB_MSG(("-->UpdateGeometry"));

  /* get minima */

  /* spiral isotropic */
  MinFov[0] =
  MinFov[1] = PVM_EffSWh / PVM_GradCalConst;

  /* minimum slice thickness */
  MinSliceThick = MRT_MinSliceThickness(
                    ExcPul.Bandwidth,
		    0,
		    SliceGradLim,
		    0,
		    PVM_GradCalConst);

  /* spiral-specific geometry settings */
  STB_SpiralUpdateGeometry(PVM_Matrix, MinFov);

  /* update geometry */
  if (PTB_GetSpatDim() == 3)
  {
    /* constrain to one package with one slice */
    MinFov[2] = MinSliceThick;
    STB_UpdateImageGeometry(3, PVM_Matrix, MinFov, 0, 1, 1, MinSliceThick, 1.0);
  }
  else
  {
    /* constrain to one package */
    STB_UpdateImageGeometry(2, PVM_Matrix, MinFov, 0, 1, 0, MinSliceThick);
  }

  DB_MSG(("<--UpdateGeometry"));
}


void UpdateMinEchoTime(void)
{
  DB_MSG(("-->UpdateMinEchoTime"));

  /* rephasing / phase duration */

  double minSliceReph = MRT_DephaseTime(
                          EffPulseDur,
                          RiseT,
                          SliceGrad,
                          SliceRephGradLim);

  double minPhase3d = PTB_GetSpatDim() > 2 ? MRT_EncodingTime(PVM_SpatResol[2], PVM_GradCalConst * Phase3DGradLim / 100.0) : 0.0;

  RephGradDur = MAX_OF(MAX_OF(minSliceReph, minPhase3d), 2.0 * CFG_GradientRiseTime()); // allow switching slice gradient from -100 to 100 %

  PVM_MinEchoTime =
    ExcPul.Length / 2.0            +
    RephGradDur                    +
    RiseT * 2.0                    +
    GradWait;

  DB_MSG(("<--UpdateMinEchoTime"));
}

void UpdateEchoTime(double minechotime)
{
  DB_MSG(("-->UpdateEchoTime"));

  PVM_MinEchoTime = minechotime;
  PVM_EchoTime = MAX_OF(PVM_MinEchoTime,PVM_EchoTime);

  /* acquisition time */

  PVM_AcquisitionTime = PVM_SpiralAcquisitionTime;

  DB_MSG(("<--UpdateEchoTime"));
}


void UpdateRepetitionTime(void)
{
  DB_MSG(("-->UpdateRepetitionTime"));

  const int dim = PTB_GetSpatDim();

  /* inside movie loop */

  PVM_MinRepetitionTime =
      GradWait                 +
      SliceSpoiler.dur         +
      RiseT                    +
      ExcPul.Length / 2.0      +  // second half contained in PVM_EchoTime
      PVM_EchoTime             +
      PVM_SpiralModuleTime     -
      PVM_SpiralEchoTime;         // contained both in PVM_EchoTime and PVM_SpiralModuleTime

  if (dim > 2)
  {
    PVM_MinRepetitionTime +=
      RephGradDur              +
      RiseT;
  }

  /* inside slice loop */

  PVM_MinRepetitionTime +=
      FreqSwitch * 2.0         +
      PVM_FatSupModuleTime     +
      PVM_FovSatModuleTime;

  if (PVM_TriggerMode == per_Slice)
  {
    PVM_MinRepetitionTime +=
      PVM_TriggerModuleTime;
  }

  if (PVM_TriggerOutMode == PER_SLICE)
  {
    PVM_MinRepetitionTime +=
      PVM_TriggerOutModuleTime;
  }

  double nSlices = (double)GTB_NumberOfSlices(PVM_NSPacks, PVM_SPackArrNSlices);
  PVM_MinRepetitionTime *= nSlices;

  /* inside TR loop */

  PVM_MinRepetitionTime +=
      PVM_TaggingModuleTime;

  if (PVM_TriggerMode == per_PhaseStep)
  {
    PVM_MinRepetitionTime +=
      PVM_TriggerModuleTime;
  }

  if (PVM_TriggerOutMode == PER_VOLUME)
  {
    PVM_MinRepetitionTime +=
      PVM_TriggerOutModuleTime;
  }

  PVM_RepetitionTime = MAX_OF(PVM_MinRepetitionTime + MinDelay * nSlices, PVM_RepetitionTime);

  TRWait = (PVM_RepetitionTime - PVM_MinRepetitionTime) / nSlices;

  /* total time */

  PVM_ScanTime =
      PVM_RepetitionTime
    * PVM_SpiralNbOfInterleaves
    * PVM_NAverages;

  if (dim > 2)
  {
    PVM_ScanTime *= PVM_EncMatrix[2];
  }

  OneRepTime = PVM_ScanTime * 1e-3;

  PVM_ScanTime *= PVM_DwNDiffExp;
  PVM_ScanTime *= PVM_NRepetitions;

  UT_ScanTimeStr(PVM_ScanTimeStr, PVM_ScanTime);

  DB_MSG(("<--UpdateRepetitionTime"));
}

void UpdateSliceGradients()
{
  DB_MSG(("-->UpdateSliceGradients"));

  /* excitation slice gradient */

  SliceGrad = MRT_SliceGrad(
    ExcPul.Bandwidth,
    PVM_SliceThick,
    PVM_GradCalConst);

  /* slice spoiler */

  MRT_UpdateSpoiler(
    "SliceSpoiler",
    2.0,
    SliceGradLim,
    1.5 * RiseT,
    PVM_GradCalConst,
    PTB_GetSpatDim() == 2 ? PVM_SliceThick : PVM_SpatResol[2]);

  DB_MSG(("<--UpdateSliceGradients"));
}

void UpdateRephaseGradients()
{
  DB_MSG(("-->UpdateRephaseGradients"));

  /* slice rephase gradient */

  SliceRephGrad = MRT_DephaseGrad(
    RephGradDur,
    EffPulseDur,
    RiseT,
    SliceGrad);

  /* phase gradient in 3rd dimension */

  if (PTB_GetSpatDim() > 2)
  {
    Phase3dGrad = MRT_PhaseGrad(
      RephGradDur,
      PVM_Matrix[2],
      PVM_Fov[2],
      PVM_GradCalConst);
  }
  else
  {
    Phase3dGrad = 0.0;
  }

  if (SliceRephGrad + Phase3dGrad > 100.0) UT_ReportError("Rephasing gradient exceeds 100.0 %");

  DB_MSG(("<--UpdateRephaseGradients"));
}

void UpdateFrequencyOffsets(void)
{
  DB_MSG(("-->UpdateFrequencyOffsets"));

  int nSlices = GTB_NumberOfSlices(PVM_NSPacks,PVM_SPackArrNSlices);

  /* read offset must be zero (thus using grad=0.0) */
  MRT_FrequencyOffsetList(nSlices,
                          PVM_EffReadOffset,
                          0.0,
                          PVM_GradCalConst,
                          PVM_ReadOffsetHz);

  MRT_FrequencyOffsetList(nSlices,
                          PVM_EffSliceOffset,
                          SliceGrad,
                          PVM_GradCalConst,
                          PVM_SliceOffsetHz);

  if (PTB_GetSpatDim() == 3)
  {
    for(int i = 0; i < nSlices; i++)
    {
      PVM_EffPhase2Offset[i] = -PVM_EffSliceOffset[i];
    }
  }

  DB_MSG(("<--UpdateFrequencyOffsets"));
}

void UpdateRFPulses(void)
{
  DB_MSG(("-->UpdateRFPulses"));

  /* RF pulse */

  STB_UpdateRFPulse("ExcPul", 1, PVM_DeriveGains);


  /* visibility */
  if (PVM_DeriveGains == Yes)
  {
    ParxRelsHideInEditor("ExcPulAmpl");
  }
  else
  {
    ParxRelsShowInEditor("ExcPulAmpl");
  }


  /* part of excitation pulse to be refocused */
  EffPulseDur = ExcPul.Length * ExcPul.Rpfac / 100.0 ;

  DB_MSG(("<--UpdateRFPulses"));
}

void UpdateAdjustments(void)
{
  DB_MSG(("-->UpdateAdjustments"));

  PTB_ClearAdjustments();
  STB_TrajAppendAdjustment(PVM_SpiralContext);
  PTB_AppendOrderAdjustment(per_scan, per_scan, RG_ADJNAME);

  DB_MSG(("<--UpdateAdjustments"));
}
