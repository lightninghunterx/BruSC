/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2006-2015
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 * ***************************************************************/
static const char resid[] = "$Id$ (C) 2006-2015 Bruker BioSpin MRI GmbH";


#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	1

#define MIN_PEAK_NOISE 10.0

#include "method.h"


void InitOptimization(void)
{
  DB_MSG(("-->InitOptimization"));


  OPT_ManAdjustmentRange();
  OPT_FOVRange();

  PARX_change_dims("PVM_Matrix", 1);
  PVM_Matrix[0] = 128;
  PVM_EffSWh = 50000.0;

  AutoSFCounter = 0;

  DB_MSG(("<--InitOptimization"));
  return;
}

void HandleAdjustmentRequests(void)
{
  YesNo adjPossible=No;

  DB_MSG(("-->HandleAdjustmentRequests"));

  if (Yes==STB_AtsAdj()) {
      return;
  }

  if(No == PTB_AdjMethSpec())
  {
    DB_MSG(("<--HandleAdjustmentRequests: no method specific adjustment desired"));
    ParxRelsParRelations("PVM_AdjHandler",Yes);
    return;
  }

  /* array phase adjustment */
  if (STB_ArrayPhaseHandleAdjustmentRequests() == 1)
  {
    /*Turn water suppression pulses off*/
    PVM_WsOnOff = Off;
    ParxRelsParRelations("PVM_WsOnOff",No);
    PVM_NavOnOff = Off;
    ParxRelsParRelations("PVM_NavOnOff",No);
    Edc_OnOff = Off;
    ParxRelsParRelations("Edc_OnOff",No);
    PVM_NAverages = 1;
    ParxRelsParRelations("PVM_NAverages",No);
    adjPossible = Yes;
  }

  const char * adjName = PTB_GetAdjName();

  if(!strcmp(adjName,"AutoLocShim"))
  {
      PVM_NavOnOff = Off;
      ParxRelsParRelations("PVM_NavOnOff",No);
      Edc_OnOff = Off;
      ParxRelsParRelations("Edc_OnOff",No);
      PVM_NAverages = 1;
      ParxRelsParRelations("PVM_NAverages",No);
      PVM_DummyScans=0;
      ParxRelsParRelations("PVM_DummyScans",No);
      PVM_WsOnOff = Off;
      ParxRelsParRelations("PVM_WsOnOff",No);
      ACQ_SetupShowDigFilling = Yes;


      ADJ_HANDLER_CONTEXT mode;
      if(PTB_AdjHandlerCall(&mode) &&
         mode==HANDLE_PLATFORM) // setting TR=0 once when adjplatform is entered, but NOT when adj is started from there
      {
          PVM_RepetitionTime = 0.0;
      }
      ParxRelsParRelations("PVM_RepetitionTime",No);
      OPT_ManAdjustment=NoManAdjustment;
      ParxRelsParRelations("OPT_ManAdjustment",No);
      ParxRelsParRelations("ACQ_SetupShowDigFilling",Yes);
      strcpy(ACQ_SetupAutoName,"PVM_TuneShimCounter");

      /* Use calculated frequency as adj result. Store PVM_AdjActShimset to avoid re-adjustment of global Frequ by ScanShim */
      static const ADJ_RESULT_PAR locShimResPars[] =
          {
              { study_result, "PVM_FrqAdj1H"      },   // - result for 1H
              { study_result, "PVM_AdjShimset"  }     // - adjusted shimset
          };
      PvAdjManSetAdjParameterList(locShimResPars, ARRAY_LEN(locShimResPars));
      SetdisplayInfonames();
      adjPossible = Yes;
  }
  else if(!strcmp(adjName,"AutoSF"))
  {
      PVM_NavOnOff = Off;
      ParxRelsParRelations("PVM_NavOnOff",No);
      Edc_OnOff = Off;
      ParxRelsParRelations("Edc_OnOff",No);
      PVM_NAverages = 1;
      ParxRelsParRelations("PVM_NAverages",No);
      PVM_DummyScans=0;
      ParxRelsParRelations("PVM_DummyScans",No);
      PVM_WsOnOff = Off;
      ACQ_SetupShowDigFilling = Yes;
      ParxRelsParRelations("ACQ_SetupShowDigFilling",Yes);
      strcpy(ACQ_SetupAutoName,"AutoSFCounter");
      adjPossible = Yes;
  }
  else if (0 == strcmp(adjName, RG_ADJNAME))
  {
      PVM_DummyScans=1;
      ParxRelsParRelations("PVM_DummyScans",No);
      PVM_NAverages=1;
      ParxRelsParRelations("PVM_NAverages",No);
      ParxRelsParRelations("PVM_AdjHandler",Yes);
      adjPossible = Yes;
  }
  else if(STB_RefScanHandleAdjustmentRequests() == 1)
  {
      PVM_DecPower = PVM_NoePower =
          CFG_RFPulseHighestAttenuation();
      PVM_WsOnOff  = Off;
      ParxRelsParRelations("PVM_WsOnOff",No);
      Edc_OnOff = Off;
      ParxRelsParRelations("Edc_OnOff",No);
      PVM_NavOnOff = Off;
      ParxRelsParRelations("PVM_NavOnOff",No);
      adjPossible  = Yes;
  }

  if(adjPossible == Yes)
  {
    OPT_ManAdjustment =  NoManAdjustment;
    backbone();
  }
  else
  {
    PARX_sprintf("Unknown Adjustment required for method STEAM");
    /* make illegal adjustment request fail by setting ACQ_SetupAutoName to empty string */
    ACQ_SetupAutoName[0]='\0';
  }

  DB_MSG(("<--HandleAdjustmentRequests"));
  return;
}

/* default relations and range checking for OPT parameter */

void OPT_ManAdjustmentRange(void)
{
  DB_MSG(("-->OPT_ManAdjustmentRange"));

  if(!ParxRelsParHasValue("OPT_ManAdjustment"))
  {
    OPT_ManAdjustment = NoManAdjustment;
  }
  else
  {
    switch(OPT_ManAdjustment)
    {
      case Pulse1:
      case Pulse2:
      case Pulse3:
      case OVSPulse1:
      case OVSPulse2:
      case OVSPulse3:
        break;
      default:
        OPT_ManAdjustment = NoManAdjustment;
      case NoManAdjustment:
        break;
    }
  }

  DB_MSG(("<--OPT_ManAdjustmentRange"));
  return;
}

void OPT_ManAdjustmentRel(void)
{
  DB_MSG(("-->OPT_ManAdjustmentRel"));

  OPT_ManAdjustmentRange();
  backbone();

  DB_MSG(("<--OPT_ManAdjustmentRel"));
  return;
}

void OPT_FOVRange(void)
{
  DB_MSG(("-->OPT_FOVRange"));

  if(!ParxRelsParHasValue("OPT_FOV"))
  {
    double defext[3];
    CFG_DefaultExtent(defext);
    OPT_FOV=defext[0];
  }

  /* constrain FOV to be posititve */
  OPT_FOV=MAX_OF(0.001,OPT_FOV);

  DB_MSG(("-->OPT_FOVRange"));
  return;
}


void OPT_FOVRel(void)
{
  DB_MSG(("-->OPT_FOVRel"));

  OPT_FOVRange();
  backbone();

  DB_MSG(("<--OPT_FOVRel"));
}



void OptHandler ( void )
{
  DB_MSG(("-->OptHandler"));


   switch(OPT_ManAdjustment)
  {
    case OVSPulse1:
    case OVSPulse2:
    case OVSPulse3:
      if(PVM_OvsOnOff==Off)
        OPT_ManAdjustment=NoManAdjustment;
      break;
    default:
      break;
    case NoManAdjustment:
      break;
  }

  /* handle manual Adjustments by creating profiles */
  if ( OPT_ManAdjustment != NoManAdjustment )
  {
    PVM_EffSWh=50000.0;
    PARX_change_dims("PVM_Matrix",1);
    PVM_Matrix[0] = 128;

    MRT_UpdateDigPars(&PVM_EffSWh,PVM_Matrix[0],NULL,&PVM_AcquisitionTime);

    /* calculation of read gradients is done in steamLoc.c (as Gr5) */

    ParxRelsShowInEditor("OPT_FOV");
  }
  else
  {
    ParxRelsHideInEditor("OPT_FOV");
  }
  ParxRelsShowInFile("OPT_FOV"); //allways store in file

  DB_MSG(("<--OptHandler"));
}



/*---------- Method specific adjustment algorithms -------------------------*/

/* ***************************************************************/
/*		AutoSF localized   		                */
/* ***************************************************************/

void AutoSFCounterRel ( void )
{
  DB_MSG(("-->AutoSfCounterRel"));

  /*
     this is the autocounter function for the Auto SF determination.
  */

  if( AutoSFCounter == 0 )
  {
    initAutoSF();
  }
  else if ( AutoSFCounter > 1 )
  {
    AdjustSF();
  }

  DB_MSG(("<--AutoSfCounterRel"));
}


void initAutoSF( void )
{
  DB_MSG(("-->initAutoSF"));
  /*
     this initialises the AutoSF measurement procedure.
     ths principle is to perform a single scan, measure
     the offset of the dominat resonance line (water..)
     and calculate and correct the corresponding frequency offset.
  */


  AutoSFCounter = 1;

  //init RG adjustment
  ParxRelsParRelations("PVM_AutoRgInitHandler", Yes);

  static const ADJ_RESULT_PAR locSfResultPars[]=
    { { study_result, "PVM_FrqAdj1H"      },   // - result for 1H (frequency of highest peek)
      { study_result, "PVM_FrqAdj1HPpm"   },   // - result for 1H (ppm of highest peek)
      { study_result, "PVM_NucleiPpmRef"  },   // - reference point of x nucleus frequency
      { study_result, "PVM_AdjShimset"   }   //  -  shim at end of adjustment
    };
  PvAdjManSetAdjParameterList(locSfResultPars, ARRAY_LEN(locSfResultPars));

}


void AdjustSF( void )
{
  /*
    this is the main part for the SF determination:
    After the initialisation, only one scan is acquired
    and the pipeline is stopped. The results of the first scan
    are evaluated and the corresponding actual offset frequency
    is calculated and set. A possible desired acquisition offset
    frequency is taken into account. Therefore, the transmitter
    frequency is set on-resonance.

    The procedure evaluates the current P/N ratio of the biggest peak
    and sets the BF1 only if the S/N value is bigger than MIN_PEAK_NOISE
    The noiselevel is estimated in frequency domain at the edge of the
    spectrum farest away from max peak (see setting of filter options
    of RecoFrqOffsetFilter in reco network).
    
  */


  DB_MSG(("-->AdjustSF"));

  /* adjust RG */
  int rgstate=ATB_AdjCheckRg(10.0,60);
  switch(rgstate)
  {
    default:
      // failure during RG adjustment -> abort
      PVM_TrajAdjustmentCounter=-1;
      break;
    case 0:
      // RG adjusted -> go on
      break;
    case 1:
      // RG needs to be adjusted -> repeat
      return;
      break;
  }

  /* RG adjusted, adjust frequency now */

  AutoSFCounter = 0; //finish adj after the current iteration

  std::stringstream errmsg;
  double p2n,nc;
  bool failed=false;

  p2n=fabs(Peak[0][0]);
  if(p2n < 1.0e-3)
  {
    errmsg  << "Signal failure peak " << p2n << std::endl;
    failed=true;
  }
  nc=fabs(Noise[0][0]);
  if(nc < 1e-3)
  {
    errmsg << "Signal failure noise " << nc  << std::endl;
    failed=true;
  }
  p2n /= nc;
  if(p2n < MIN_PEAK_NOISE)
  {
    errmsg << "Signal failure P/N =" << p2n << " < " << MIN_PEAK_NOISE 
           << std::endl;
    failed=true;
  }    
 

  if(failed==false)
  {
  

    BF1 -= PVM_TuneShimFreqOffs[0]*1.0e-6;
    ParxRelsParRelations("BF1",Yes);

    if(!strcmp(PVM_Nucleus1, "1H"))
    {
      PVM_FrqAdj1H = BF1;
    }
    else
    {
      double frq1H0ppm = PVM_FrqAdj1H/(1+1e-6*PVM_FrqAdj1HPpm);
      PVM_FrqRefPpm[0] = PVM_FrqWorkPpm[0]
        + 1e6*(1.0-BF1/(CFG_GammaRatio(PVM_Nucleus1)*frq1H0ppm));
      MRT_NucleiPpmRefSet(PVM_Nucleus1,PVM_FrqRefPpm[0]);
    }

    ATB_GetActualShim("PVM_AdjShimset");
    AdjErrorComment[0]='\0';

  }
  else
  {
    std::ostringstream oerr;
    oerr << "Frequency Adjustment (STEAM) failed due to small Peak/Noise ratio"
         << std::endl << errmsg.str();
    snprintf(AdjErrorComment,
             PARX_get_dim("AdjErrorComment",1)-1,
             oerr.str().c_str());
    
    AutoSFCounter = -1;
    DB_MSG((oerr.str().c_str()));
  }
  const char *const groupList[] = \
    {"LocShimFrqAdjGroup","MethodClass"};
  int id=0;
  PTB_StoreInParentDset(No,
                        groupList,
                        2,
                        "LocalFrequencyAdjustment",
                        "Results",
                        &id,
                        No);

  DB_MSG(("<--AdjustSF"));
  return;
}

void SetdisplayInfonames(void)
{

  SetDimOfTuneshimInterfacePars(RecoNrActiveReceivers());
  std::string recoinfopars="PVM_TuneShimFreqOffs[0]/Frequency Offset,LineWidth[0][0][0]/Line Width";
  PARX_change_dims("ACQ_RecoInfoNames",recoinfopars.size()+1);
  ParxRelsParSetValue("ACQ_RecoInfoNames",recoinfopars.c_str());
  std::string signalinfopars="NormArea[0]/Normalized Area";
  size_t slen=signalinfopars.size()+1;

  int njobs=0,activejobs=0;
  njobs = (int) PARX_get_dim("ACQ_jobs",1);

  for(int i=0;i<njobs;i++)
  {
    if(ACQ_jobs[i].chanNum != 0)
      activejobs++;
  }
  

  if(activejobs != (int)PARX_get_dim("ACQ_SignalInfoNames",2) ||
     slen > PARX_get_dim("ACQ_SignalInfoNames",2))
  {
    PARX_change_dims("ACQ_SignalInfoNames",activejobs,slen+1);
  }    
  ParxRelsParSetValue("ACQ_SignalInfoNames[0]",signalinfopars.c_str());

}




void TuneShimQPRel(void)
{

  if(!ParxRelsParHasValue("NormArea"))
  {
    PVM_TuneShimQP=0.0;
    return;
  }

  try
  {
    
    double sum=0.0;

    UT_MeanOfDoublePar("NormArea[0]",0,0,&sum);
    PVM_TuneShimQP=sum;

  }
  catch (const PvException &ex)
  {
    throw PvStdException("Setting PVM_TuneShimQP failed. Reason:\n%s",
                         ex.what());
  }

}

void TuneShimFinishAdj(void)
{
  // handle adjustment results by default relations
  ParxRelsParRelations("PVM_TuneShimFinishAdjHandler",Yes);
  const char *const groupList[] = \
    {"LocShimFrqAdjGroup","MethodClass"};
  int id=0;
  PTB_StoreInParentDset(No,
                        groupList,
                        2,
                        "LocalShimAdjustment",
                        "Results",
                        &id,
                        No);
}
