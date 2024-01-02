/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2018
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2002 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0


/****************************************************************/
/****************************************************************/
/*		I N T E R F A C E   S E C T I O N		*/
/****************************************************************/
/****************************************************************/

/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "method.h"

/****************************************************************/
/*	I M P L E M E N T A T I O N   S E C T I O N		*/
/****************************************************************/


/****************************************************************/
/*		G L O B A L   F U N C T I O N S			*/
/****************************************************************/


/* ------------------------------------------------------------ 
  backbone 
  The main part of method code. The consitency of all parameters is checked
  chere, relations between them are resolved and, finally, functions setting
  the base level parameters are called.
  --------------------------------------------------------------*/
void backbone( void )
{
  DB_MSG(("--> backbone"));

  /*
   *  control appearance in GeoEditor: method doesn't support any geometric operation
   */

  /* Nucleus and  PVM_GradCalConst
     are handled by this funtion: */
  STB_UpdateNuclei(Yes);
 
  DB_MSG(("nucleus ok"));

  /* handle RF pulses */   
  UpdateRFPulses();

  PVM_NEchoImages = 1;


  /* ------------- spectroscopy part ---------------------------*/
  STB_UpdateSpectroscopy( PVM_Nucleus1 );

  STB_UpdateEncoding();

  UpdateSliceSelection(PVM_Nucleus1);

  /* handling of modules */
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains);

  /*SatTrans Module: for spoiling: usually all slices of a package are of the same thickness.
   Not so necessarily here. bypass by using smallest one*/
  double slthck=10.0;
  for (int i=0; i<SliceNum; i++)
    slthck = MIN_OF(slthck, SliceThick[i]);
  STB_UpdateSatTransModule(PVM_DeriveGains, slthck);

  LocalDecNoeHandling();
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);

  double excDelay =   PVM_FovSatModuleTime
                    + PVM_SatTransModuleTime
                    + 0.01 /* freqRx */
                    + 0.01 /* freqTx */
                    + ExcPulse1.Length/2.0; //ms

  if (SliceSelOnOff)
    excDelay += (PVM_RiseTime + SliceSpoilDur);

  STB_UpdateWsModule(0.0,1,excDelay); //after FovSat
  STB_UpdateTriggerModule();

  /* optimization params*/
  if (PVM_NavOnOff==On)
  {
    ParxRelsMakeEditable("PVM_DriftCompYesNo");
  }
  else
  {
    PVM_DriftCompYesNo=No;
    ParxRelsMakeNonEditable("PVM_DriftCompYesNo");
  }

  STB_UpdateNavModule();
  
  /* DeadTime */
  updateDeadTime();

  /* repetition time */
  repetitionTimeRels();

  /*
   * update ReferenceScan pargroup
   */
  STB_UpdateMCRefScan(PVM_EncNReceivers);

  /* DriftCompensation */
  STB_UpdateDriftComp(PVM_RepetitionTime/SliceNum);

  /* set up adjustment list */
  SetAdjustments();

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"");

  /* set baselevel acquisition parameter */
  SetBaseLevelParam();

  /* set baselevel reconstruction parameter */
  SetRecoParam();

  DB_MSG(("<-- backbone"));
}

void UpdateRFPulses(void)
{
  DB_MSG(("--->UpdateRFPulses"));

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

  DB_MSG(("<---UpdateRFPulses"));

  return;
}

void repetitionTimeRels( void )
{
  int i,dim;
  double TotalTime = 0.0;

  DB_MSG(("--> minRepetitionTimeRels"));

  double effPulseDur = ExcPulse1.Length * (ExcPulse1.Rpfac/100);
  double riseTime    = CFG_GradientRiseTime();
  double rampupSlice = SliceSelOnOff == On ? riseTime : 0.0;

  PVM_MinRepetitionTime = SliceNum
                          * ( 0.03                     + //update_dyn_pars
                              PVM_TriggerModuleTime    +
                              PVM_FatSupModuleTime     +
                              PVM_NoeModuleTime        +
                              PVM_WsModuleDuration     +
                              PVM_FovSatModuleTime     +
                              PVM_SatTransModuleTime   +
                              0.01                     + //freqTx
                              0.01                     + //freqRx
                              rampupSlice              +
                              SliceSpoilDur            +
                              ExcPulse1.Length - effPulseDur  +
                              EchoTime                 +
                              PVM_SpecAcquisitionTime  +
                              1.0                        //ADC_END
                            );
  
  if(PVM_NavOnOff == On)
    PVM_MinRepetitionTime += SliceNum * (0.02 + PVM_NavigatorModuleTime);

  PVM_RepetitionTime = ( PVM_RepetitionTime < PVM_MinRepetitionTime ? 
			 PVM_MinRepetitionTime : PVM_RepetitionTime );


  /* updating Dummy Scans after RepetitionTime and before TotalScanTime */
  STB_UpdateDummyScans(PVM_RepetitionTime);  
  
  /** Calculate Total Scan Time and Set for Scan Editor **/ 
  dim = MRT_GetSpecDim();
  TotalTime = PVM_RepetitionTime*PVM_NAverages;
  for(i=1; i<dim; i++)
     TotalTime *= PVM_SpecMatrix[i];

  TotalTime = TotalTime * PVM_SatTransRepetitions * PVM_NRepetitions;

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
 

  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<-- repetitionTimeRels"));
}

void LocalDecNoeHandling(void)
{
 if((PVM_DecOnOff == On ) || (PVM_NoeOnOff == On))
 {
   if(PVM_NumberOfNuclei!=2)
   {
     STB_InitNuclei(2);
     if(PVM_NumberOfNuclei < 2)
     {
       /* system configuration does not support 2nd RF channel */
       PVM_DecOnOff=PVM_NoeOnOff=Off;
     }
   }
 }
 else if( (PVM_DecOnOff ==Off ) && (PVM_NoeOnOff == Off))
 {
   if(PVM_NumberOfNuclei!=1)
   {
     STB_InitNuclei(1);
   }
 }

 DB_MSG(("Updating DecModule"));
 STB_UpdateDecModule(PVM_Nucleus2,PVM_SpecAcquisitionTime);
 STB_UpdateNoeModule(PVM_Nucleus2);

 if( PVM_DecOnOff == On  && PVM_NoeOnOff == On &&
     PVM_DecMode == Composite_Pulse && PVM_NoeMode == Composite_Pulse_Noe)
 {
   /* both modules use F2 channel and share therefore the CPD pulse 
      element duration PCPD[1] so the element durations have to be matched */
   
   if(PVM_NoePulseElementDuration != PVM_DecPulseElementDuration)
   {
     PVM_NoePulseElementDuration = PVM_DecPulseElementDuration;
     STB_UpdateNoeModule(PVM_Nucleus2);
   }
 }

}
 


void SetAdjustments(void )
{
  DB_MSG(("-->SetAdjustments"));

  PTB_ClearAdjustments();

  if (SliceSelOnOff==On)
    STB_ArrayPhaseAppendAdjustment(PVM_EncNReceivers, No, 0, &SliceGeoCub[0]);
  else
    STB_ArrayPhaseAppendAdjustment(PVM_EncNReceivers, No);

    if(PVM_RefScanYN == Yes)
    {
      PTB_AppendAdjustment("ReferenceScan",
                           "Reference Scan",
                           "Acquisition of a not suppressed reference scan",
                           per_scan);
    }
    else
    {
      PTB_AppendAdjustment("ReferenceScan",
                           "Reference Scan",
                           "Acquisition of a not suppressed reference scan",
                           on_demand);
    }

    PTB_AppendOrderAdjustment(per_scan, per_scan, RG_ADJNAME);

  DB_MSG(("<--SetAdjustments"));
}


/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
