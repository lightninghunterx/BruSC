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
#include <libCore/Math/Math.hh>
#include "lib/PvSysinfoClient.h"

#include <string.h>

/****************************************************************/
/*	I M P L E M E N T A T I O N   S E C T I O N		*/
/****************************************************************/


/****************************************************************/
/*		G L O B A L   F U N C T I O N S			*/
/****************************************************************/


/****************************************************************/
/*	         L O C A L   F U N C T I O N S			*/
/****************************************************************/

void ExcPulseEnumRelation(void)
{
  DB_MSG(("-->ExcPulseEnumRelation"));

  UT_SetRequest("ExcPulseEnum");
  backbone();

  DB_MSG(("<--ExcPulseEnumRelation"));
}


void AutoEchoTimerRel(void)
{
  if (AutoEchoTimer)
    ParxRelsMakeNonEditable("PVM_EchoTime");
  else
    ParxRelsMakeEditable("PVM_EchoTime");

  backbone();
}

void ExcPulseRelation(void)
{
  DB_MSG(("-->ExcPulseRelation"));

  PARX_change_dims("PkArea", NExcSteps);

  NoiseScan = NExcSteps>1 ? 1 : 0 ;

  backbone();

  DB_MSG(("<--ExcPulseRelation"));
}


void ExcPulseRangeCheck(void)
{
  DB_MSG(("-->ExcPulseRangeCheck"));

  char errbuf[1024];
  double cw_pwr  = 0.0;   //[W]
  double def_dur = 100.0; //[us]

  // set cw pwr as init val for power
  int sysstate = SysInfo_TxMaxAllowedCWPower( 1, &cw_pwr );
  if( sysstate )
  {
    snprintf(errbuf,1023, "SysInfo_TxMaxAllowedCWPower: "
             "Error retrieving cw power.\nReason: %s",
              SysInfo_getErrorMessage(sysstate));

    UT_ReportError(errbuf);
  }

  if(ParxRelsParHasValue("PkArea") == No)
  {
    PARX_change_dims("PkArea", NExcSteps);
    for (int i=0; i<NExcSteps;i++) PkArea[0] = 0.0;
  }

  if(ParxRelsParHasValue("ExcDurRange") == No ||
     ParxRelsParHasValue("ExcDurList")  == No   )
  {
    ExcDurRange[0] = ExcDurRange[1] = ExcDurList[0] = def_dur;
  }

  if(ParxRelsParHasValue("ExcPowRange") == No ||
     ParxRelsParHasValue("ExcPowList")  == No   )
  {
    ExcPowRange[0] = ExcPowRange[1] = ExcPowList[0] = cw_pwr;
  }

  if(ParxRelsParHasValue("ExcVoltList")  == No   )
  {
    ExcVoltList[0] = sqrt(50.0*ExcPowList[0]);
  }
  DB_MSG(("<--ExcPulseRangeCheck"));
}


void ScanModeRelation(void)
{
  DB_MSG(("-->ScanModeRelation"));

  ScanModeCheck();
  backbone();

  DB_MSG(("<--ScanModeRelation"));
}


void ScanModeCheck(void)
/* -------------------------------------------------------------------
   check/adapt sizes of dur- & powlist
   set visibility
   ------------------------------------------------------------------*/
{
  DB_MSG(("-->ScanModeCheck"));

  // check/adapt sizes of dur- & powlist
  // set visibility

  if (ScanMode == CONST_PULSE_POW)
  {
    if (SliceSelOnOff)
      SliceSelOnOff = Off;

    ParxRelsMakeNonEditable ( "ExcPowRange"      );
    ParxRelsMakeEditable    ( "ExcPowList"       );

    //on result card:
    ParxRelsMakeNonEditable ( "PVM_AdjRefPowXPwr");
    ParxRelsMakeEditable    ( "PVM_AdjRefPowXDur");

    PARX_change_dims        ( "ExcPowList",  1 );
    PARX_change_dims        ( "ExcVoltList", 1 );
    PARX_change_dims        ( "ExcDurList", NExcSteps );

    if (NExcSteps > 1)
    {
      ParxRelsMakeNonEditable( "ExcDurList"  );
      ParxRelsMakeEditable   ( "ExcDurRange" );
    }
    else
    {
      ParxRelsMakeEditable("ExcDurList");
      ParxRelsMakeNonEditable("ExcDurRange");
    }
  }
  else  // VARY_POWER
  {
    ParxRelsMakeNonEditable( "ExcDurRange" );
    ParxRelsMakeEditable   ( "ExcDurList"  );

    //on result card:
    ParxRelsMakeEditable ( "PVM_AdjRefPowXPwr");
    ParxRelsMakeNonEditable( "PVM_AdjRefPowXDur");

    PARX_change_dims ( "ExcDurList", 1 );
    PARX_change_dims ( "ExcPowList", NExcSteps);
    PARX_change_dims ( "ExcVoltList", NExcSteps );

    if (NExcSteps > 1)
    {
      ParxRelsMakeNonEditable("ExcPowList");
      ParxRelsMakeEditable("ExcPowRange");
    }
    else
    {
      ParxRelsMakeEditable("ExcPowList");
      ParxRelsMakeNonEditable("ExcPowRange");
    }
  }

  DB_MSG(("<--ScanModeCheck"));
}


void RecoModeRelation(void)
/* -------------------------------------------------------------------
   For magnitude reco there is no zero-crossing from which to identify
   180 degree transition:
   ------------------------------------------------------------------*/
{
  if (RecoMode == MAGN_RECO_MODE)
    SearchMode = search_90;
  else
    ParxRelsMakeEditable("SearchMode");
}


void SearchModeRelation(void)
/* -------------------------------------------------------------------
   For magnitude reco there is no zero-crossing from which to identify
   180 degree transition:
   ------------------------------------------------------------------*/
{
  if (SearchMode == search_180)
    RecoMode = CMPLX_RECO_MODE;
  else
    ParxRelsMakeEditable("RecoMode");
}


void SearchRangeRelation()
/* -------------------------------------------------------------------
   Parameter SearchRange allows to narrow region within which PkArea is
   analyzed to find 90 or 180 deg signal transition.
   1. SearchRange is derived from ExcDur- or PowList depend. on ScanMode.
   2. react on value changed by user int change_search_range:
      3 possibilities
      a) spinning up   b) spinning down  c) direct value change
      If old,new value differ by exactly +/-1 then presumably case a)/b)
      => search next upper/lower entry in list. If c) search nearest
   ------------------------------------------------------------------*/
{
  DB_MSG(("-->SearchRangeRelation"));

  int nsteps = NExcSteps;
  double value = 0.0;

  double* sdr = SearchDurRange;
  double* svr = SearchVoltRange;
  double* spr = SearchPowRange;

  int idx = 0;


  std::string parname = UT_RelContext_ParName(true,true);

  if (ScanMode == CONST_PULSE_POW)
  {
    if (parname == "SearchDurRange[0]" || parname == "SearchDurRange[1]")
    {
      value = parname == "SearchDurRange[0]" ? sdr[0] : sdr[1];

      change_search_range(ExcDurList, nsteps, &value);

      ParxRelsParSetDoubleValue(parname.c_str(), value);
    }
  }
  else
  {
    if (parname == "SearchPowRange[0]")
    {
      idx = change_search_range(ExcPowList, nsteps, spr);
      svr[0] =ExcVoltList[idx] ;
    }
    else if (parname == "SearchPowRange[1]")
    {
      idx = change_search_range(ExcPowList, nsteps, &spr[1]);
      svr[1] =ExcVoltList[idx] ;
    }
    else if (parname == "SearchVoltRange[0]")
    {
      idx = change_search_range(ExcVoltList, nsteps, svr);
      spr[0] =ExcPowList[idx] ;
    }
    else if (parname == "SearchVoltRange[1]")
    {
      idx = change_search_range(ExcVoltList, nsteps, &svr[1]);
      spr[1] =ExcPowList[idx] ;
    }
  }

  DB_MSG(("<--SearchRangeRelation"));
}


void PkRangeRelation(void)
/* -------------------------------------------------------------------
   PkRange: peak range borders to sum over in RecoMethodFilter. Check
   1. SpecSWH[0] <= PkRange[*] <=SpecSWH[0]
   2. PkRange[0] < PkRange[1]
   ------------------------------------------------------------------*/
{
  DB_MSG(("-->PkRangeRelation"));

  double lb = -0.5*PVM_SpecSWH[0];  // max lower border
  double ub = +0.5*PVM_SpecSWH[0];  // min upper border

  if (!ParxRelsParHasValue("PkRange"))
  {
    PkRange[0]=lb;
    PkRange[1]=ub;
  }
  else
  {
    std::string parname = UT_RelContext_ParName(true,true);

    if (parname == "PkRange[0]")
      PkRange[0] = MAX_OF( MIN_OF(PkRange[1]-1, PkRange[0]), lb);

    if (parname == "PkRange[1]")
      PkRange[1] = MAX_OF( MIN_OF(ub, PkRange[1]), PkRange[0]+1);
  }

  DB_MSG(("<--PkRangeRelation"));
}


void StudyRefPowRelation(void)
{
  DB_MSG(("-->StudyRefPowRelation"));

  convertPulseAmplcheckLimits("PVM_AdjRefPowXDur",
                              "PVM_AdjRefPowXPwr",
                              "PVM_StudyRefPow"  );

  UpdateResultPars(AdjRunning);

  DB_MSG(("<--StudyRefPowRelation"));
}


void UpdateResultPars( YesNo setrefpow )
/* -------------------------------------------------------------------
   called after finished adjustment (GopAdjResultHandler) and after
   manual edition of an adj result par (by setrefpow=1 or 0)

   1. after finished adj:
      a. if CONST_PULSE_POW: get PVM_AdjRefPowXDur from data max
      b. if CONST_PULSE_DUR: get PVM_AdjRefPowXPwr from data max
                             calc pampl & patt
   2. after manual edition: Set ref power status to "manually changed"
   ------------------------------------------------------------------*/
{
  DB_MSG(("-->UpdateResultPars"));

  std::string ref_stat;
  REFPOW_TYPE*           adj_ref = &PVM_StudyRefPow;
  double*                adj_dur = &PVM_AdjRefPowXDur;
  PVM_RF_PULSE_AMP_TYPE* adj_pwr = &PVM_AdjRefPowXPwr;


  // 1.
  if (setrefpow)
  {
    if (ScanMode == CONST_PULSE_POW)
    {
      *adj_dur       = ExcDurList[PkMaxIdx] + PkMaxIdxFrac;
      adj_pwr->ppow  = ExcPowList[0];
    }
    else
    {
      *adj_dur       = ExcDurList[0];
      adj_pwr->ppow  = ExcPowList[PkMaxIdx] + PkMaxIdxFrac;
    }

    adj_pwr->pampl = sqrt( 50 * adj_pwr->ppow );
    adj_pwr->patt  = PvOvlUtilConvWattToDb( adj_pwr->ppow);

    if ( strcmp(PVM_Nucleus1,"1H") )
      ref_stat     = "set by AdjRefPowX";
    else
      ref_stat     = "set by AdjRefPowX (not used: 1H-chan)";

    sprintf(PVM_StudyRefPow.Stat,ref_stat.c_str());
  }
  // 2.
  else
  {
    std::string parname = UT_RelContext_ParName();

    if (   parname == "PVM_StudyRefPow"
        || parname == "PVM_AdjRefPowXDur"
        || parname == "PVM_AdjRefPowXPwr"
       )
    {
      if ( strcmp(PVM_Nucleus1,"1H") )
        ref_stat = "manually set by AdjRefPowX";
      else
        ref_stat     = "manually set by AdjRefPowX (not used: 1H chan)";

      sprintf(PVM_StudyRefPow.Stat,ref_stat.c_str());
    }
  }

  adj_ref->RefPow = 1e-6 * adj_pwr->ppow * pow(*adj_dur * ExcPulse.Sint, 2);

  DB_MSG(("<--UpdateResultPars"));
}


void ExcDurPowResultRel(void)
{
  DB_MSG(("-->ExcDurPowResultRel"));

  convertPulseAmplcheckLimits("PVM_AdjRefPowXDur",
                              "PVM_AdjRefPowXPwr",
                              "PVM_StudyRefPow"  );

  UpdateResultPars(AdjRunning);

  DB_MSG(("<--ExcDurPowResultRel"));
}


void convertPulseAmplcheckLimits(const char * const durname,
                                 const char * const pwrname,
                                 const char * const refname)
/* -------------------------------------------------------------------
   1. check types of char* arguments
   2. get values of char* arguments
   3. get hw-info, calculate hw limits
   4. get parname edited manufally, check for limits:
      - editable is refname and either durname or pwrname depend. on
      - scan mode. The other par is non-editable, fixed & will not be
      - adapted i.c.o. limit excess to avoid misleading report diagram
   ------------------------------------------------------------------*/
{
  DB_MSG(("-->convertPulseAmplcheckLimits"));

  // 1.
  char partype[128], errmsg [512];

  const char* const args[3] = {durname, pwrname, refname};
  const char * type_args[3] ={"double", "PVM_RF_PULSE_AMP_TYPE", "REFPOW_TYPE"};

  int failed = 0;

  for (int i=0; i<3; i++)
  {
    failed = UT_GetParType(args[i], partype);

    if (failed)
    {
      snprintf(errmsg,511,"Error in convertPulseAmplcheckLimits. 1st arg "
               "%s does not exist", args[i]);
    }

    if ( strcmp(partype, type_args[i]) )
    {
      snprintf(errmsg,511,"Error in convertPulseAmplcheckLimits. Wrong type "
               "for arg %d. Is %s, should: %s", i, partype, type_args[i]);
      failed = 1;
    }

    if (failed)
    {
      UT_ReportError(errmsg);
      return;
    }
  }

  //2.
  double                dur = 0;
  PVM_RF_PULSE_AMP_TYPE pwr;
  REFPOW_TYPE           ref;

  ParxRelsParGetValue(durname, &dur);
  ParxRelsParGetValue(pwrname, &pwr);
  ParxRelsParGetValue(refname, &ref);

  //3.
  double cw_pwr = 0.0, pk_pwr = 0.0, pk_dur = 0.0;
  double lim_dur, lim_pwr;

  SysInfo_TxCoilMaxPowPulselength( 1, &pk_dur );
  SysInfo_TxMaxAllowedCWPower    ( 1, &cw_pwr );

  pk_pwr = SysInfo_TxMaxAllowedPeakPower(1);

  // upper lim of energy
  double pk_e = 1e3*pk_dur*pk_pwr ; // 1e3: dur=[us], pk_dur=[ms]

  //4.
  std::string parname = UT_RelContext_ParName();

  if ( !parname.compare(durname) )
  {
    if (pwr.ppow > cw_pwr)
    {
      lim_dur = pk_e / pwr.ppow;
      dur     = MIN_OF ( lim_dur, dur );
    }
  }
  else if ( !parname.compare(pwrname) )
  {
    std::vector<std::string> strct = UT_RelContext_ParStructNames();

    if (!strct.empty())
    {

      if ( !strct[0].compare("ppow") )
      {
        pwr.pampl = sqrt( 50 * pwr.ppow );
        pwr.patt  = PvOvlUtilConvWattToDb( pwr.ppow);
      }
      else if ( !strct[0].compare("pampl") )
      {
        pwr.ppow = pow( pwr.pampl, 2) / 50;
        pwr.patt = PvOvlUtilConvWattToDb( pwr.ppow );
      }
      else if ( !strct[0].compare("patt") )
      {
        pwr.ppow  = PvOvlUtilConvDbToWatt( pwr.patt );
        pwr.pampl = sqrt( 50 * pwr.ppow );
      }

      // check limits
      if (pwr.ppow > cw_pwr)
      {
        lim_pwr   = pk_e/dur;
        pwr.ppow  = MIN_OF ( lim_pwr, pwr.ppow);
        pwr.pampl = sqrt( 50 * pwr.ppow );
        pwr.patt  = PvOvlUtilConvWattToDb( pwr.ppow );
      }
    }
  }
  else if ( !parname.compare(refname) )
  {
    std::vector<std::string> strct = UT_RelContext_ParStructNames();

    if (!strct.empty())
    {
      if ( !strct[0].compare("RefPow") )
      {
        if (ScanMode == CONST_PULSE_POW)
        {
          // set a default if no value or zero
          if (ParxRelsParHasValue(pwrname) == No || pwr.ppow == 0.0)
            pwr.ppow = ref.RefPow;

          // check limits
          if ( pwr.ppow > pk_pwr)
            pwr.ppow  = pk_pwr;

          dur = 1e3 * sqrt( ref.RefPow / pwr.ppow) / ExcPulse.Sint;

          // adapt pwr if needed
          if (pwr.ppow > cw_pwr)
          {
            lim_dur = pk_e/pwr.ppow;
            dur     = MIN_OF ( dur, lim_dur );
          }
        }
        else
        {
          // set a default if no value or zero
          if (ParxRelsParHasValue(durname) == No || dur == 0.0)
            dur = 1000; //1 ms as in ref bp

          pwr.ppow  = 1e6 * ref.RefPow / pow( dur * ExcPulse.Sint, 2 );

          // check limits
          if ( pwr.ppow > pk_pwr)
            pwr.ppow  = pk_pwr;

          //adapt dur if needed
          if (pwr.ppow > cw_pwr)
          {
            lim_pwr   = pk_e/dur;
            pwr.ppow  = MIN_OF ( pwr.ppow, lim_pwr );
          }

          pwr.pampl = sqrt( 50 * pwr.ppow );
          pwr.patt  = PvOvlUtilConvWattToDb( pwr.ppow);
        }
      }
    }
  }

  ref.RefPow = 1e-6 * pwr.ppow * pow( dur*ExcPulse.Sint, 2);

  ParxRelsParSetValue(durname, &dur);
  ParxRelsParSetValue(pwrname, &pwr);
  ParxRelsParSetValue(refname, &ref);

  DB_MSG(("<--convertPulseAmplcheckLimits"));
}


int change_search_range(double *lst, int sze, double* val)
/* -------------------------------------------------------------------
   ------------------------------------------------------------------*/
{
  // get value of parameter before edited:

  double prev_val;

  UT_RelContext_ParOrigValue(prev_val);

  int idx = std::find( lst, lst+sze, prev_val ) - lst;

  double diff = *val - lst[idx];

  if ( fabs(diff-1) < 1e-6)
  {
    if (idx<sze-1)
      *val = lst[idx+1];
    else
      *val = lst[idx];
  }
  else if ( fabs(diff + 1.0) < 1e-6)
  {
    if (idx)
      *val = lst[idx-1];
    else
      *val = lst[idx];
  }
  else
  {
    int up_idx = std::upper_bound(lst, lst+sze, *val) - lst;
    up_idx = MIN_OF( up_idx,  sze-1);

    int lo_idx = std::lower_bound(lst, lst+sze, *val) - lst - 1;
    lo_idx = MAX_OF(lo_idx, 0);

    if ( fabs(*val - lst[up_idx]) < fabs(*val - lst[lo_idx]) )
      *val = lst[up_idx];
    else
      *val = lst[lo_idx];
  }

  return (std::find( lst, lst+sze, *val ) - lst);
}


int check_adc_overflow(void)
{
  std::ostringstream msg;

  int ret = 0;

  if(ParxRelsParHasValue("ACQ_adc_overflow"))
  {
    int njobs = PARX_get_dim("ACQ_adc_overflow", 1);
    YesNo *ovfl = ACQ_adc_overflow;

    for(int i = 0; i < njobs; i++)
    {
      if(ovfl[i] == Yes)
      {
        msg << "ADC overflow on Job[" << i << "]." << std::endl;
        setmessage( msg.str().c_str());
        AdjPerformState = adj_failed;
        ret = 1;
      }
    }
  }

  return ret;
}


void setmessage(const char * msg)
/* ----------------------------------------------------------------------
   stores all messages in one parameter. For pdf-report file
   1. prevent message multiplication
   2. increase dim of Messages-array and paste msg
   ---------------------------------------------------------------------*/
{

  if(!ParxRelsParHasValue("Messages"))
  {
    PARX_change_dims("Messages",1,1024);
  }

  // 1.
  int n = PARX_get_dim("Messages", 1 );

  for (int i=0; i<n; i++)
  {
    if ( !strcmp( Messages[i], msg ) )
      return;
  }

  // 2.
  if ( !strcmp( Messages[n-1], "\0" ) )
  {
    strcpy(Messages[n-1], msg);
  }
  else
  {
    PARX_change_dims("Messages", n+1, 1024);
    strcpy(Messages[n], msg);
  }

}
/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
