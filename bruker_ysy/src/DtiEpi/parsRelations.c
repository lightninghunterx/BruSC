/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2003-2009
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


#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"


/*--------------------------------------------------------
 *
 * single parameter range checkers and relations
 *
 *-------------------------------------------------------*/


void ShowAllParsRange(void)
{
  DB_MSG(("-->ShowAllParsRange\n"));

  switch(ShowAllPars)
  {
    case Yes:
      break;
    default:
      ShowAllPars = No;
    case No:
      break;
  }

  DB_MSG(("<--ShowAllParsRange\n"));

}

void ShowAllParsRel(void)
{
  DB_MSG(("-->ShowAllParsRel\n"));

  ShowAllParsRange();

  SliceSelectionParsVisibility(ShowAllPars);
  if(PTB_GetSpatDim()==3)
  {
    Phase3DEncodingParsVisibility(ShowAllPars);
  }
  DB_MSG(("<--ShowAllParsRel\n"));
  return;
}

void RepetitionTimeRange(void)
{
  
  DB_MSG(("-->RepetitionTimeRange\n"));

  if(ParxRelsParHasValue("PVM_RepetitionTime")==No)
  {
    PVM_RepetitionTime = 1000.0;

  }
  else
  {
    PVM_RepetitionTime = MAX_OF(1e-3,PVM_RepetitionTime);
  }

  
  DB_MSG(("<--RepetitionTimeRange\n"));
  return;
}

void RepetitionTimeRel(void)
{
  DB_MSG(("-->RepetitionTimeRel\n"));

  RepetitionTimeRange();
  backbone();

  DB_MSG(("<--RepetitionTimeRel\n"));
  return;
}

void AveragesRange(void)
{
  
  DB_MSG(("-->AveragesRange\n"));

  if(ParxRelsParHasValue("PVM_NAverages")==No)
  {
    PVM_NAverages = 1;

  }
  else
  {
    PVM_NAverages = MAX_OF(1,PVM_NAverages);
  }

  
  DB_MSG(("<--AveragesRange\n"));
  return;
}

void AveragesRel(void)
{
  DB_MSG(("-->AveragesRel\n"));

  AveragesRange();
  backbone();

  DB_MSG(("<--AveragesRel\n"));
  return;
}

void RepetitionsRange(void)
{
  
  DB_MSG(("-->RepetitionsRange\n"));

  if(ParxRelsParHasValue("PVM_NRepetitions")==No)
  {
    PVM_NRepetitions = 1;

  }
  else
  {
    PVM_NRepetitions = MAX_OF(1,PVM_NRepetitions);
  }
  
  DB_MSG(("<--RepetitionsRange\n"));
  return;
}

void RepetitionsRel(void)
{
  DB_MSG(("-->RepetitionsRel\n"));

  RepetitionsRange();
  backbone();

  DB_MSG(("<--RepetitionsRel\n"));
  return;
}

void EchoTimeRange(void)
{
  
  DB_MSG(("-->EchoTimeRange\n"));

  if(ParxRelsParHasValue("PVM_EchoTime")==No)
  {
    PVM_EchoTime = 20.0;

  }
  else
  {
    PVM_EchoTime = MAX_OF(1e-3,PVM_EchoTime);
  }

  
  DB_MSG(("<--EchoTimeRange\n"));
  return;
}

void EchoTimeRel(void)
{
  DB_MSG(("-->EchoTimeRel\n"));

  EchoTimeRange();
  backbone();

  DB_MSG(("<--EchoTimeRel\n"));
  return;
}

void GradStabTimeRange(void)
{
  double max;
  DB_MSG(("-->GradStabTimeRange\n"));

  max = CFG_GradientRiseTime();

  if(ParxRelsParHasValue("GradStabTime")==No)
  {
    GradStabTime = 0.0;
  }
  else
  {
    GradStabTime = MAX_OF(MIN_OF(max,GradStabTime),0.0);
  }
    
  DB_MSG(("<--GradStabTimeRange\n"));
  return;
  
}

void GradStabTimeRel(void)
{
  DB_MSG(("-->GradStabTimeRel\n"));

  GradStabTimeRange();
  backbone();

  DB_MSG(("-->GradStabTimeRel\n"));
  return;
}

void EffSwRange(void)
{

  DB_MSG(("-->EffSwRange\n"));
  double minmax[2]={50000.0,1000000.0};

  if(!ParxRelsParHasValue("PVM_EffSWh"))
    PVM_EffSWh=200000.0;

  MRT_GetNearestBandwidthInHz("PVM_EffSWh",minmax);

  DB_MSG(("<--EffSwRange\n"));
  return;
}

void EffSwRel(void)
{
  DB_MSG(("-->EffSwRel\n"));
  
  EffSwRange();
  backbone();

  DB_MSG(("<--EffSwRel\n"));

}

void PackDelRange(void)
{
  DB_MSG(("-->PackDelRange\n"));

  if(ParxRelsParHasValue("PackDel")==No)
  {
    PackDel = 0.001;
  }
  else
  {
    PackDel = MAX_OF(0.001,PackDel);
  }

  DB_MSG(("<--PackDelRange\n"));
  return;
}

void PackDelRel(void)
{
  DB_MSG(("-->PackDelRel\n"));
  PackDelRange();
  backbone();
  DB_MSG(("<--PackDelRel\n"));
  return;
}

/* relations of NSegments */

void NSegmentsRels(void)
{
  NSegmentsRange();
  backbone();
}

void NSegmentsRange(void)
{
  if(!ParxRelsParHasValue("NSegments"))
    NSegments = 1;
  else
    NSegments = MAX_OF(1,NSegments);
}

/* relations of TensorReconstruction parameters */

void RecoMethModeRel(void)
{
  DB_MSG(("-->RecoMethModeRel\n"));
  
  RecoMethModeRange();
  
  if(RecoMethMode==Default)
    ParxRelsHideInEditor("TraceWeight");
  else
    ParxRelsShowInEditor("TraceWeight");
  
  DB_MSG(("<--RecoMethModeRel\n"));  
}

void RecoMethModeRange(void)
{
  DB_MSG(("-->RecoMethModeRange\n")); 
    
  if(!ParxRelsParHasValue("TraceWeight"))
    TraceWeight = 1000.0;
  else
    TraceWeight = MAX_OF(0.0,TraceWeight);
  
  if(!ParxRelsParHasValue("RecoMethMode"))
    RecoMethMode=Default;
  
  // Require at least 7 diffusion experiments, from which 6 or more have different
  // diffusion directions (at least nominally).
  if((!ParxRelsParHasValue("PVM_DwAoImages") || !ParxRelsParHasValue("PVM_DwNDiffDir") ||
      PVM_DwNDiffDir + PVM_DwAoImages < 7 || PVM_DwNDiffDir < 6))
  {
    RecoMethMode = Default;
    ParxRelsMakeNonEditable("RecoMethMode");
  }
  else
  {
    ParxRelsMakeEditable("RecoMethMode");
  }

  DB_MSG(("<--RecoMethModeRange\n"));
}
