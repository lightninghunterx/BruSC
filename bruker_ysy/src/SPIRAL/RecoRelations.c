/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2007 - 2012
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

const char * methods_spiral_reco_relations_id = "$Id$ (C) 2007 - 2011 Bruker BioSpin MRI GmbH";

#define DEBUG       0


#include "method.h"

void SetRecoParam(void)
{
  DB_MSG(("-->SetRecoParam"));

  char trajFile[PATH_MAX];
  PvOvlUtilGetExpnoPath(trajFile, PATH_MAX, "traj");

  if (PVM_TrajRecoMode != Traj_Measured)
  {
    /* theoretical trajectory for both reconstruction and DC */
    ATB_SpiralSetRecoPars(trajFile);
  }
  else
  {
    /* measured trajectory for reconstruction and theoretical trajectory for DC */
    char trajDCFile[PATH_MAX];
    PvOvlUtilGetExpnoPath(trajDCFile, PATH_MAX, "trajDC");
    ATB_SpiralSetRecoPars(trajFile, trajDCFile);
  }

  /* GS mode */
  ACQ_SetupOnlineReco = No;
  ParxRelsParRelations("ACQ_SetupOnlineReco", Yes);

  DB_MSG(("<--SetRecoParam"));
}

void RecoDerive(void)
{
  DB_MSG(("-->RecoDerive"));

  if (RecoUserUpdate == No)
  {
    DB_MSG(("<--RecoDerive: no update"));
    return;
  }

  /* trajectory measurement: use different reco */
  if (ATB_TrajSetupReco() == Yes)
  {
    DB_MSG(("<--RecoDerive: trajectory measurement"));
    return;
  }

  /* standard settings for reconstruction */
  if(RecoPrototype == No)
    SetRecoParam();

  if (PVM_TrajRecoMode != Traj_Measured)
  {
    /* theoretical trajectory */
    STB_SpiralTrajectory(RecoRegridNTrajFile);

    /* create network */
    ATB_SpiralSetupReco(PVM_SpiralPreSize,PVM_SpiralSize,PVM_SpiralPostSize);
  }
  else
  {
    if (PVM_TrajUpToDate == No)
    {
      UT_ReportError("Measured trajectory is not up-to-date");
    }

    int nPre    = PVM_SpiralPreSize;
    int nSpiral = PVM_SpiralSize;
    int nPost   = PVM_SpiralPostSize;
    char b0File[PATH_MAX];

    PvOvlUtilGetExpnoPath(b0File, PATH_MAX, "b0");

    /* measured trajectory for reconstruction */
    const int maxGradDelay = (int)(0.3 / PVM_SpiralAcqDwellTime); // [samples]

    STB_TrajCreateTrajectory(
         RecoRegridNTrajFile, b0File,
         &nPre, &nSpiral, &nPost,
         maxGradDelay, PVM_SpiralPointsPerRotation);

    DB_MSG(("nPre = %d, nSpiral = %d, nPost = %d, ", nPre, nSpiral, nPost));

    /* theoretical trajectory for DC */
    STB_SpiralTrajectory(RecoRegridNTrajFileDC, NULL, NULL, 0, nSpiral);

    /* create network */
    ATB_SpiralSetupReco(nPre, nSpiral, nPost, b0File);
  }

  DB_MSG(("<--RecoDerive"));
}

