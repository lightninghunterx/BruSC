/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2020
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2002 Bruker BioSpin MRI GmbH";

#define DEBUG   0
#define DB_MODULE 0
#define DB_LINE_NR  0


/****************************************************************/
/****************************************************************/
/*		I N T E R F A C E   S E C T I O N		*/
/****************************************************************/
/****************************************************************/

/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "method.h"
#include "lib/PvSysinfoClient.h"

#ifndef CPROTO
#include <sstream>

static void checkPulseTrainDutyCycle(const std::string & parname);
#endif

/****************************************************************/
/*	I M P L E M E N T A T I O N   S E C T I O N		*/
/****************************************************************/


/****************************************************************/
/*		G L O B A L   F U N C T I O N S			*/
/****************************************************************/


/* ---------------------------------------------------------------
  backbone
  The main part of method code. Consitency of all parameters is
  checked here, relations between them are resolved, functions to
  set the base level parameters are called.
  --------------------------------------------------------------*/
void backbone( void )
{
  DB_MSG(("--> backbone"));


  if( CFG_IsDatastation() )
  {
    std::ostringstream msg;
    msg << "running as DataStation: data are simulated" << std::endl;
    setmessage( msg.str().c_str());
  }

  if(Yes==InAdjPlatform)
  /* Do not restore/reset result parameters (as PVM_RefPow) when
     in AdjPlatform, but allow manual editing and storing as new
     adjustment result */
    STB_UpdateNuclei(Yes,No);
  else
    STB_UpdateNuclei(Yes);

  /* handle RF pulses */
  STB_UpdateRFPulse("ExcPulse", 1, PVM_DeriveGains, Conventional);

  PVM_NEchoImages = 1;

  /* ------------- spectroscopy part ---------------------------*/
  STB_UpdateSpectroscopy( PVM_Nucleus1 );

  STB_UpdateEncoding();

  UpdateSliceSelection(PVM_Nucleus1);

  UpdateScanMode();

  UpdateSearchRange();

  UpdatePkRange();

  /* handling of modules */
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains);

  LocalDecNoeHandling();
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);

  double excDelay =   PVM_FovSatModuleTime
                    + 0.01 /* freqRx */
                    + 0.01 /* freqTx */
                    + 5e-4*ExcDurList[0]; //us->ms & half of dur

  if (SliceSelOnOff)
    excDelay += (PVM_RiseTime + SliceSpoilDur);

  STB_UpdateWsModule(0.0, 1, excDelay); //after FovSat
  STB_UpdateTriggerModule();

  //STB_UpdateNavModule();
  STB_UpdateDriftCompModule(PVM_SliceThick, PVM_DeriveGains);

  /* echo time */
  UpdateEchoTime();

  /* repetition time */
  UpdateRepetitionTime();

  /* update ReferenceScan pargroup */
  STB_UpdateMCRefScan(PVM_EncNReceivers);

  /* DriftCompensation */
  STB_UpdateDriftComp(PVM_RepetitionTime/SliceNum);

  /* set up adjustment list */
  SetAdjustments();

  /* update mapshim parameter class */
  STB_UpdateMapShim(PVM_Nucleus1,"");

  ParxRelsGroupRoutine("PVM_CfgInfoMrGroup", MODALITY_HIERARCHY, GroupRelation);

  /* set baselevel acquisition parameter */
  SetBaseLevelParam();

  /* set baselevel reconstruction parameter */
  SetRecoParam();

  DB_MSG(("<--backbone"));
}


void UpdateEchoTime( void )
/* -------------------------------------------------------------------
   For scan mode CONST_PULSE_POW rf-pulses have different lengths:
   use max pulse length to calc minTE. Compensate shorter pulses via
   TETRFillDelay.
   1. calc min te
   2. calc te-filling times:
   ------------------------------------------------------------------*/
{
  DB_MSG(("-->UpdateEchoTime"));

  int durlstsze  = PARX_get_dim("ExcDurList", 1);
  double *durlst = ExcDurList;
  double riseT   = CFG_GradientRiseTime();
  double rpfac   = ExcPulse.Rpfac/100;
  double max_dur = *std::max_element( durlst, durlst + durlstsze)*1e-3;
  double max_eff_dur = max_dur*rpfac;    /* center of longest pulse  */

  // 1.
  double acqdecdel = 0.02;               /* fix delay in subr acqdec */

  PVM_MinEchoTime = max_eff_dur + acqdecdel;

  if ( SliceSelOnOff==On )
  {
    PVM_MinEchoTime +=  riseT            /* ramp down slice grad       */
                      + SliceRephDur     /* ramp up & plateau rephgrad */
                      + riseT;           /* ramp down reph grad        */
  }

  if (AutoEchoTimer)
    PVM_EchoTime = PVM_MinEchoTime;
  else
    PVM_EchoTime = MAX_OF(PVM_MinEchoTime, PVM_EchoTime);
  
  // 2.
  PARX_change_dims("TETRFillDelay", durlstsze);

  for (int i=0; i<durlstsze; i++)
  {
    TETRFillDelay[i] = PVM_EchoTime - 1e-3*durlst[i]*rpfac - acqdecdel;

    if ( SliceSelOnOff==On )
      TETRFillDelay[i] -= (2*riseT + SliceRephDur);

    TETRFillDelay[i] = MAX_OF(TETRFillDelay[i], 0.0);
  }

  DB_MSG(("<--UpdateEchoTime"));
}


void UpdateRepetitionTime( void )
/* -------------------------------------------------------------------
   1. Use max pulse length for minRepTime calculation. Shorter pulses
      need compensation delays: TETRFillDelay
   2. calc minRepTime
   3. check for duty cycle: a subsequent parameter adaption to keep
      limit depends on parameter changed by user
   4. calc TotScanTime
   ------------------------------------------------------------------*/
{
  DB_MSG(("-->UpdateRepetitionTime"));

  double TotalTime = 0.0;

  // 1.
  int  durlstsze     = PARX_get_dim("ExcDurList", 1);
  double *durlst     = ExcDurList;

  double max_dur     = *std::max_element( durlst, durlst + durlstsze)*1e-3;
  double eff_dur     = max_dur * (1- ExcPulse.Rpfac/100);

  double *vdlst      = TETRFillDelay;
  double min_vd      = *std::min_element( vdlst, vdlst + durlstsze)*1e-3;
  // 2.
  double riseTime    = CFG_GradientRiseTime();
  double rampupSlice = SliceSelOnOff == On ? riseTime : 0.0;

  PVM_MinRepetitionTime = SliceNum
                          * ( 0.03                     + //update_dyn_pars
                              PVM_TriggerModuleTime    +
                              PVM_FatSupModuleTime     +
                              PVM_NoeModuleTime        +
                              PVM_WsModuleDuration     +
                              PVM_FovSatModuleTime     +
                              min_vd                   +
                              0.01                     + //freqTx
                              0.01                     + //freqRx
                              rampupSlice              + // d4
                              SliceSpoilDur            +
                              max_dur - eff_dur        +
                              PVM_EchoTime             +
                              PVM_SpecAcquisitionTime  +
                              1.0                      +  //ADC_END
                              PVM_DriftCompModuleTime
                            );

  PVM_RepetitionTime = MAX_OF(PVM_RepetitionTime, PVM_MinRepetitionTime);

  // 3.
  std::string parname = UT_RelContext_ParName();

  if (NExcSteps > 1)
    checkPulseTrainDutyCycle(parname);

  PVM_RepetitionTime = MAX_OF(PVM_RepetitionTime, PVM_MinRepetitionTime);

  /* updating Dummy Scans after RepetitionTime and before TotalScanTime */
  STB_UpdateDummyScans(PVM_RepetitionTime);

  // 4.
  TotalTime  = ( NoiseScan+NExcSteps ) * PVM_NAverages;
  TotalTime += PVM_DummyScans;
  TotalTime *= PVM_RepetitionTime;

  PVM_ScanTime = TotalTime;

  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<--UpdateRepetitionTime"));
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


void UpdatePkRange(void)
/* -------------------------------------------------------------------
   reacts on PVM_SpecSWH changed in UI
   ------------------------------------------------------------------*/
{
  double lb = -0.5*PVM_SpecSWH[0];  // max lower border
  double ub = +0.5*PVM_SpecSWH[0];  // min upper border

  PkRange[0] = MAX_OF( MIN_OF(ub, PkRange[0]), lb);
  PkRange[1] = MAX_OF( MIN_OF(ub, PkRange[1]), lb);
}


void UpdateSearchRange(void)
/* -------------------------------------------------------------------
   ------------------------------------------------------------------*/
{
  if (ScanMode == CONST_PULSE_POW)
  {
    ParxRelsHideInEditor("SearchVoltRange");
    ParxRelsHideInEditor("SearchPowRange");
    ParxRelsShowInEditor("SearchDurRange");

    SearchDurRange[0] = ExcDurList[0];
    SearchDurRange[1] = ExcDurList[NExcSteps-1];

    SearchVoltRange[0] =  SearchVoltRange[1] = ExcVoltList[0];
    SearchPowRange[0]  =  SearchPowRange[1]  = ExcPowList[0];
  }
  else
  {
    ParxRelsShowInEditor("SearchVoltRange");
    ParxRelsShowInEditor("SearchPowRange");
    ParxRelsHideInEditor("SearchDurRange");

    SearchDurRange[0] =  SearchDurRange[1] = ExcDurList[0];

    SearchVoltRange[0] = ExcVoltList[0];
    SearchVoltRange[1] = ExcVoltList[NExcSteps-1];
    SearchPowRange[0]  = ExcPowList[0];
    SearchPowRange[1]  = ExcPowList[NExcSteps-1];
  }
}


void UpdateScanMode(void)
/* -------------------------------------------------------------------
   ScanMode is either CONST_PULSE_POW or CONST_PULSE_DUR
   1. change editibility according to ScanMode.
   2. derive values of ExcDurList and ExcPowList from ranges.
      limit checks implied w STB_UpdateRFPulse are overruled here as rf-
      dur and power are non-constants during scan -> own check necessary
   a. CONST_PULSE_POW: space pulse durations equally
   b. CONST_PULSE_DUR is a more complex situation:
     - operator should specify power ranges in Watt. Values for cw_pwr,
       max_pwr and max_dur can be found easily ('Show System Information')
     - In report the plot should be versus volt: proportionality to flip
       has 2 advantages:
       1 in principle curves can look sinusoidal if chosen parameter are
         OK & they do not if not
       2 by-eye interpolation betw 2 data pts easy for [V], not for [W]
       Last: Space data equally for [V]-plot => calc increments in volt.
   -----------------------------------------------------------------*/
{
  DB_MSG(("-->UpdateScanMode"));

  //1.
  ScanModeCheck();

  //2.
  double *durlst  = ExcDurList;
  double *pwrlst  = ExcPowList;
  double* voltlst = ExcVoltList;
  double *drange  = ExcDurRange;
  double *prange  = ExcPowRange;
  int lstsze      = NExcSteps;

  double cw_pwr   = 0.0;
  double pk_pwr   = 0.0, pk_dur  = 0.0;
  double lim_pwr  = 0.0, lim_dur = 0.0;
  double max_dur  = 0.0, max_pwr = 0.0; //max vals in use
  double incr     = 0.0;

  double vrange[] = {0.0,0.0}; //range for voltages

  SysInfo_TxMaxAllowedCWPower( 1, &cw_pwr );
  SysInfo_TxCoilMaxPowPulselength( 1, &pk_dur );
  pk_pwr = SysInfo_TxMaxAllowedPeakPower(1);

  if (ScanMode == CONST_PULSE_POW)
  {
    if ( lstsze > 1 )
    {
      // 2a
      incr =  ( *(drange+1) - *drange ) / (lstsze-1);

      for (int i = 0; i < lstsze; i++)
        *(durlst+i) = *drange + i*incr;
    }
    else
    {
      *(drange+1) = *drange = *durlst;
    }

    max_pwr = *std::max_element( pwrlst, pwrlst + lstsze);
    max_dur = *std::max_element( durlst, durlst + lstsze)*1e-3;

    if (max_pwr > cw_pwr)
    {
      lim_pwr = MIN_OF( pk_pwr*pk_dur / max_dur, pk_pwr );

      *pwrlst = MIN_OF(*pwrlst, lim_pwr);
    }

    *(prange+1) = *prange = *pwrlst;
    *voltlst = sqrt(50*(*pwrlst));

  }
  else                // VARY_POWER
  {
    if ( lstsze > 1 )
    {
      //check limit of range input
     *prange     = MIN_OF( *prange     , pk_pwr);
     *(prange+1) = MIN_OF( *(prange+1) , pk_pwr);

     // 2b
     //calc start- & end-voltages from power range
     vrange[0] = sqrt( 50*prange[0]);
     vrange[1] = sqrt( 50*prange[1]);

     incr =  ( vrange[1] - vrange[0] ) / (lstsze-1);

     for (int i = 0; i < lstsze; i++)
     {
       voltlst[i] = vrange[0] + i*incr;
       pwrlst[i]  = voltlst[i]*voltlst[i]/50.0;
     }

    }
    else
    {
      *pwrlst = MIN_OF(*pwrlst, pk_pwr);

      *(prange+1) = *prange = *pwrlst;
    }

    max_pwr = *std::max_element( pwrlst, pwrlst + lstsze);
    max_dur = *std::max_element( durlst, durlst + lstsze)*1e-3;

    if (max_pwr > cw_pwr)
    {
      lim_dur = MIN_OF( pk_pwr*pk_dur / max_pwr, max_dur )*1e3;

      *durlst = MIN_OF(*durlst, lim_dur);
    }

    *(drange+1) = *drange = *durlst;
  }

  DB_MSG(("<--setPulseLists"));
}

#ifndef CPROTO
static void checkPulseTrainDutyCycle(const std::string & parname)
/* -------------------------------------------------------------------
   limit pulse train: average power over tr should stay within cw power
   1. pwr, pwr-range, dur or dur-range edited => adapt tr
   2. tr edited => adapt dur or pwr whichever is the one-element array
   -----------------------------------------------------------------*/
{
  DB_MSG(("-->checkPulseTrainDutyCycle"));

  double cw_pwr   = 0.0;
  double ave_pwr  = 0.0, sum_pwr = 0.0, sum_dur = 0.0;
  double *pwrlst  = ExcPowList, *durlst  = ExcDurList;
  double *drange  = ExcDurRange, *prange = ExcPowRange;
  double *tr      = &PVM_RepetitionTime;
  bool changedPul = false, changedTR = false;
  double p_int    = ExcPulse.Pint;
  int  nsteps     = NExcSteps;
  char errbuff[1024];


  if ( parname == "PVM_RepetitionTime" )
    changedTR = true;
  else if ( parname == "ExcDurList" || parname == "ExcDurRange" ||
            parname == "ExcPowList" || parname == "ExcPowRange"   )
    changedPul = true;

  SysInfo_TxMaxAllowedCWPower( 1, &cw_pwr );

  if ( ScanMode == CONST_PULSE_POW )
  {
    for (int i=0; i<nsteps; i++)
      sum_dur += *(durlst + i); //ExcDurList[i] in us

    sum_pwr = *pwrlst;
  }
  else                          // VARY_POWER
  {
    for (int i=0; i<nsteps; i++)
      sum_pwr += *(pwrlst + i);

    sum_dur = *durlst;
  }

  ave_pwr  = p_int * sum_dur * sum_pwr;
  ave_pwr /= ( 1e3 * nsteps * (*tr) ); // [durlist]=us, [RepTime]=ms

  if ( ave_pwr > cw_pwr )
  {
    if ( changedTR )
    {
      if ( ScanMode == CONST_PULSE_POW )
      {
        *pwrlst = *tr * 1e3 * nsteps * cw_pwr / (p_int * sum_dur);

        *prange = *(prange + 1) = *pwrlst;
      }
      else
      {
        *durlst = *tr * 1e3 * nsteps * cw_pwr / (p_int * sum_pwr);

        *drange = *(drange +1) = *durlst;
      }
    }
    else if ( changedPul )
    {
      *tr = 1e-3 * p_int * sum_dur * sum_pwr / (nsteps * cw_pwr);
    }
    else
    {
      snprintf(errbuff,1023,
               "checkPulseTrainDutyCycle: average power exceeds "
               "cw power for unknown reason.\n Average: %f W, "
               "allowed cw: %f W", ave_pwr, cw_pwr);

      UT_ReportError(errbuff);

      setmessage(errbuff);
    }
  }

  DB_MSG(("<--checkPulseTrainDutyCycle"));
}
#endif
/****************************************************************/
/*		E N D   O F   F I L E				                              */
/****************************************************************/
