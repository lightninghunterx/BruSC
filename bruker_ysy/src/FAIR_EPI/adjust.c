/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2006-2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 * ***************************************************************/
static const char resid[] = "$Id$ (C) 2006-2016 Bruker BioSpin MRI GmbH ";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"
#include "common/ovl_toolbox/AdjTools.h"


/* -------------------------------------------------------------------
  Relations of PVM_AdjHandler, called when an adjustment is starting.
  Parameter changes made her will be used for the adjustment and then
  disarded (the current scan will not be affected).
  -------------------------------------------------------------------*/
void HandleAdjustmentRequests(void)
{
  YesNo adjPossible=No,TrajAdjRequired=Yes;
  int virtualAccel,nSubAdj;
  char  adjSequence[100];

  DB_MSG(("-->HandleAdjustmentRequests"));

  if (Yes==STB_AtsAdj()) {
      return;
  }

  if(No == PTB_AdjMethSpec())
  {
    DB_MSG(("<--HandleAdjustmentRequests: no method specific adjustment required"));
    ParxRelsParRelations("PVM_AdjHandler",Yes);
    return;
  }

  if(No == PTB_AdjustmentStarting())
  {
    DB_MSG(("<--HandleAdjustmentRequests: method should not be rebuilt in adjustment platform"));
    return;
  }

  if((PVM_NSPacks==1)&&(PVM_EpiTrajAdjMeasured==No)&&(PVM_EpiTrajAdjYesNo==Yes)&&(PVM_EpiTrajAdjAutomatic==Yes))
    TrajAdjRequired=Yes;
  else
    TrajAdjRequired=No;

  // Sequence of RG sub-adjustments:
  nSubAdj = 0;
  if(TrajAdjRequired==Yes)
  {
   strcpy(adjSequence,"Traj,");
   nSubAdj++;
  }
  else
    adjSequence[0] = '\0';

  if(PVM_EpiAutoGhost==No || PVM_EpiCombine==Yes)
  {
    strcat(adjSequence,"Rg");
    nSubAdj++;
  }
  else
  {
    if(PVM_SignalType == SignalType_Fid)
    {
      strcat(adjSequence,"SeGhost,Rg");
      nSubAdj+=2;
    }
    else
    {
      strcat(adjSequence,"RgGhost");
      nSubAdj++;
    }
  }

  DB_MSG(("RG sub-adjustment sequence: %s", adjSequence));

  const char * adjName = PTB_GetAdjName();

  if(Yes==PTB_AdjMethSpec() &&
     0 == strcmp(adjName, RG_ADJNAME))
  {
      DB_MSG(("setting up RG adjustment"));

      DB_MSG(("Subprocess: %s",PVM_AdjHandler.subprocess));

      if(PTB_CheckSubProcess(0)==Yes)
      {
          if(nSubAdj != PTB_InitSubProcess(adjSequence)) {
              DB_MSG(("Could not initialize subprocesses"));
          }
          DB_MSG(("<--HandleAdjustmentRequests (init subprocesses)"));
          return;
      }
      else if(PTB_CheckSubProcess("Traj")==Yes)
      { // adjust trajectory
          DB_MSG(("RG-Subadjustment: Traj"));
          setTrajAdj();
          adjPossible=Yes;
      }
      else if(PTB_CheckSubProcess("SeGhost")==Yes)
      { // adapt to SE, and adjust RG and ghost
          DB_MSG(("RG-Subadjustment: SeGhost"));
          setSpinEcho();
          setRGGhostAdj(Yes);
          adjPossible=Yes;
      }
      else if(PTB_CheckSubProcess("RgGhost")==Yes)
      { // adjust RG and ghost
          DB_MSG(("RG-Subadjustment: RgGhost"));
          setRGGhostAdj(Yes);
          adjPossible=Yes;
      }
      else if(PTB_CheckSubProcess("Rg")==Yes)
      { // adjust RG only
          DB_MSG(("RG-Subadjustment: Rg"));
          PVM_EpiAutoGhost=No;
          setRGGhostAdj(No);
          adjPossible=Yes;
      }
      else
          DB_MSG(("Unknown sub-adjustment"));
  }
  else if(Yes==PTB_AdjMethSpec() && !strcmp(adjName,"EpiTraj"))
  {
      DB_MSG(("setting up epi trajectory adjustment"));

      setTrajAdj();
      adjPossible = Yes;
  }
  else if(Yes==PTB_AdjMethSpec() &&
          !strcmp(adjName,"EpiGrappa"))
  {
      /* Set up a multi-shot, non-accelerated scan to adjust
         the GRAPPA parameters */

      DB_MSG(("setting up EpiGrappa adjustment"));

      ATB_EpiSetGrappaAdj();

      virtualAccel = PVM_EncPpi[1];
      if(PVM_EpiGrappaSegAdj==Yes)
      {
          NSegments *= PVM_EncPpi[1];
          /* Ds for at least 3 sec (do not set
             PVM_DummyScans which has lower priority) */
          PVM_DummyScansDur=3000; 
      }
      else
          PVM_EncZf[1] *= PVM_EncPpi[1];

      PVM_EncPpi[1] = 1;
      PVM_NRepetitions = 1;
      PVM_NAverages = 1;

      PVM_FairMode = NONSELECTIVE;
      PVM_FairTIR_NExp = 1;

      adjPossible = Yes;
      backbone();

      ACQ_RfShapes[47].powerWatt = 0.0; /* inversion off */

      /* derive reco, with acceleration */
      SetRecoParam(virtualAccel);

  }

  if(adjPossible == No)
  {
    PARX_sprintf("Unknown adjustment required");
    /* make illegal adjustment request fail by setting ACQ_SetupAutoName to empty string */
    ACQ_SetupAutoName[0]='\0';
  }

  DB_MSG(("<--HandleAdjustmentRequests"));
  return;
}


/*
 *  This function is called each time an adjustment is finished. Changes made here
 *  will stay for the active scan. This routine is performed in the parameter space
 *  of the active scan and NOT in the parameter space of the adjustment.
 */

void HandleAdjustmentResults(void)
{
  DB_MSG(("-->HandleAdjustmentResults"));

  const char * adjName = PTB_GetAdjResultName();

  if (0 == strcmp(adjName, RG_ADJNAME))
  {
      DB_MSG(("RCVR adjustment result"));

      DB_MSG(("Subadj: %s",PTB_GetAdjResultSubProcess()));

      if(!strcmp(PTB_GetAdjResultSubProcess(),"Traj"))
        STB_EpiHandleTrajAdjResults(PVM_Fov[0],PVM_EffSWh,PVM_SPackArrGradOrient[0][0]);
      /* Mirror parameters:
       * PVM_EpiTrajAdjFov0, PVM_EpiTrajAdjMatrix0, PVM_EpiTrajAdjBw, PVM_EpiTrajAdjComp,
       * PVM_EpiTrajAdjRampmode, PVM_EpiTrajAdjRampform, PVM_EpiTrajAdjRamptime, PVM_EpiTrajAdjReadvec
       */
  }
  else if (!strcmp(adjName,"EpiTraj"))
  {
      STB_EpiHandleTrajAdjResults(PVM_Fov[0],PVM_EffSWh,PVM_SPackArrGradOrient[0][0]);
  }
  DB_MSG(("Calling backbone"));
  backbone();
  DB_MSG(("<--HandleAdjustmentResults"));

}

void setRGGhostAdj(YesNo DoubleShot)
{
  ATB_EpiSetRgAndGhostAdj();
  PVM_FairMode = NONSELECTIVE;
  PVM_FairTIR_NExp = 1;

  /* change to to 1-shot with the same echo train length */

  double tempSeg=1; //double important, can be .5

  if((DoubleShot==Yes)&&((PVM_EpiDoubleShotAdj==In_All)||((PVM_EpiDoubleShotAdj==In_Multishot)&&(PVM_EpiNShots>1))))
  {
    tempSeg=NSegments/2.0;
    PVM_EncZf[1] *= (NSegments/2.0);
    NSegments = 2;
    PVM_EpiEchoTimeShifting=Yes;
  }
  else
  {
    tempSeg=NSegments;
    PVM_EncZf[1] *= NSegments;
    NSegments = 1;
  }
  if(PTB_GetSpatDim() >2) /* limit dimensions to 2 */
  {
    int dimRange[2] = {2,2};
    int lowMat[3] = { 16, 16, 8 };
    int upMat[3]  = { 512, 512, 256 };
    STB_InitStandardInplaneGeoPars(2,dimRange,lowMat,upMat,No);
  }

  /* FOV reduction to get same blips and same EpiRampTime as
     before segment reduction. Otherwise adjusted trajectory
     is not used during ghost adjustment and effects are
     compensated twice -> remaining ghosting artifacts.
     FOV reduction does not exceed given limits, since preceding
     segment reduction relaxes limits equivalently. */
  PVM_Fov[1]/=tempSeg;

  backbone();

  ACQ_RfShapes[47].powerWatt = 0.0; /* inversion off */

}

void setSpinEcho(void)
{
  PVM_SignalType=SignalType_Echo;  /* adapt to SE */
  if(PVM_RepetitionTime<3000.0) PVM_RepetitionTime=3000.0;
  if(RecoveryTime<3000.0) RecoveryTime=3000.0;
  RefPulse1.Flipangle=180.0;
  ExcPulse1.Flipangle=90.0;
}

void setTrajAdj(void)
{
  static const ADJ_RESULT_PAR adjPars[] =
  {
    { adj_type, "PVM_EpiTrajAdjkx"},
    { adj_type, "PVM_EpiTrajAdjb0" },
    { adj_type, "PVM_EpiTrajAdjMeasured" }
  };
  PvAdjManSetAdjParameterList(adjPars, ARRAY_LEN(adjPars));

  PVM_EpiAdjustMode = 3;
  PVM_EpiTrajAdjMeasured=No; //do not use previous results

  PVM_EncZf[1] *= PVM_EncPpi[1];
  PVM_EncPpi[1]=1;

  strcpy(ACQ_SetupAutoName,"PVM_EpiTrajCounter");

  /* change geometry to 2D, single package, 2 slices: */
  {
    int dimRange[2]={2,2}, lowMat[3]={16,16,8}, upMat[3]={512,512,256};
    int defaultMat[3] = {64,64,64};
    STB_InitEncoding( 2, dimRange, lowMat, upMat, defaultMat);
  }

  PVM_SliceThick = PVM_SpatResol[0];
  if(PVM_SliceThick<1.333333*PVM_MinSliceThick)
  {
    /* longer excitation pulse necessary to achieve slice thickness */
    ExcPulse1.Length*=(PVM_MinSliceThick/PVM_SliceThick)*1.333333;
  }

  PVM_SPackArrNSlices[0] = 2;
  PVM_SPackArrSliceGapMode[0] = non_contiguous;
  PVM_ObjOrderScheme = Sequential;

  double acq_modoffs[3]={0,0,0};
  GObject slicegeo("PVM_SliceGeo");
  double acq_pos[3]={0,0,0};

  slicegeo.getPosToAcqObj(acq_modoffs,0);
  slicegeo.getPosToAcq(acq_pos,0);
  for(int i=0;i<3;i++) acq_modoffs[i] -= acq_pos[i];
  
  
  
  
  PVM_SPackArrSliceOffset[0] = acq_modoffs[2];
  STB_UpdateSliceGeoPars(0,1,0,0.0);  /* only one package */
  PVM_SPackArrSliceDistance[0] = PVM_Fov[0]*PVM_AntiAlias[0]*PVM_EpiTrajAdjDistRatio;

  /* Change to to 1-shot and increase zero filling to get approx. same echo train length.
     Rounding of total lines for zero filling and for NSegments may differ.
     However, this is not critical since traj is determined from 4 central odd lines.
     */
  int tempSeg=NSegments;
  // multiply by NSegments, or lower factor (>=1) (to get at least 10 lines -> required by traj recon filter)
  PVM_EncZf[1] *= MIN_OF(NSegments, MAX_OF(1, (int) (NSegments*PVM_EpiNEchoes/10)));
  NSegments = 1;
  PVM_DummyScans=0;

  PVM_NRepetitions = 1;

  PVM_FairMode = NONSELECTIVE;
  PVM_FairTIR_NExp = 1;
  setSpinEcho(); /* adapt to SE */
  PVM_RepetitionTime=3000.0;

  /* FOV reduction to get same blips and same EpiRampTime as
     before segment reduction. FOV reduction does not exceed
     given limits, since preceding segment reduction relaxes
     limits equivalently */
  PVM_Fov[1]/=tempSeg;

  ParxRelsParRelations("PVM_SPackArrSliceDistance[0]",No); /* ->backbone(); */

  ACQ_RfShapes[47].powerWatt = 0.0; /* inversion off */


  /* Orient slices orthogonal to read direction  */

  ACQ_GradientMatrix[0][2][0]= ACQ_GradientMatrix[0][0][0];
  ACQ_GradientMatrix[0][2][1]= ACQ_GradientMatrix[0][0][1];
  ACQ_GradientMatrix[0][2][2]= ACQ_GradientMatrix[0][0][2];

  ACQ_GradientMatrix[0][1][0]=0.0;
  ACQ_GradientMatrix[0][1][1]=0.0;
  ACQ_GradientMatrix[0][1][2]=0.0;

  ACQ_GradientMatrix[1][2][0]= ACQ_GradientMatrix[1][0][0];
  ACQ_GradientMatrix[1][2][1]= ACQ_GradientMatrix[1][0][1];
  ACQ_GradientMatrix[1][2][2]= ACQ_GradientMatrix[1][0][2];

  ACQ_GradientMatrix[1][1][0]=0.0;
  ACQ_GradientMatrix[1][1][1]=0.0;
  ACQ_GradientMatrix[1][1][2]=0.0;
}
