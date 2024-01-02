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
#include <libCore/Math/Math.hh>
#include "lib/PvSysinfoClient.h"

#ifndef CPROTO
#include <vector>
#include "PvMrAdj/MrHardwareContext.hh"
#endif //CPROTO

#define CONFIG_REFPX_ADJNAME "TRANSM_X"
#define CURR_REFPX_ADJNAME   "CurrAdjRefPowX"
#define RG_ADJNAME           "RCVR_Standard"

static int checkRgArray(double*, int);

void SetAdjustments(void )
{
  DB_MSG(("-->SetAdjustments"));

  AdjPerformListType refscantype = PVM_RefScanYN?per_scan:on_demand;

  PTB_ClearAdjustments();


  PTB_AppendAdjustment("ReferenceScan",
                       "Reference Scan",
                       "Ref scan w/o water suppression",
                       refscantype);

  PTB_AppendAdjustment(CURR_REFPX_ADJNAME,
                       "Reference Power X",
                       "Find Ref gain for x nuc w protocol in instruction list",
                       on_demand,
                       nullptr,
                       CONFIG_REFPX_ADJNAME,
                       true);

  if(!SkipSubAdjustments)
  {
    PTB_AppendOrderAdjustment(per_scan, per_scan, RG_ADJNAME);
    
    if (SliceSelOnOff==On)
      STB_ArrayPhaseAppendAdjustment(PVM_EncNReceivers, No, 0, &SliceGeoCub[0]);
    else
      STB_ArrayPhaseAppendAdjustment(PVM_EncNReceivers, No);
  }

  DB_MSG(("<--SetAdjustments"));
}


void AdjHandler(void)
{
  DB_MSG(("-->AdjHandler"));

  const char * const adjName = PTB_GetAdjName();

  if (Yes==STB_AtsAdj())
      return;

  //non-method-specific adjustment
  if (PTB_AdjMethSpec() == No)
  {
    DB_MSG(("%s: no method specific adjustment",adjName));
    ParxRelsParRelations("PVM_AdjHandler", Yes);
    return;
  }

  if (STB_ArrayPhaseHandleAdjustmentRequests() == 1)
  {
    prepPars_RxPhaseAdj();
    return;
  }

  if(STB_RefScanHandleAdjustmentRequests() == 1)
  {
    prepPars_RefScanAdj();
  }
  else if( !strcmp(adjName, RG_ADJNAME) )
  {
    prepPars_RgAdj();
  }
  else if (   !strcmp(adjName, CONFIG_REFPX_ADJNAME)
           || !strcmp(adjName, CURR_REFPX_ADJNAME  ) )
  {
    handleRefPowXAdjustment(adjName);
  }

  DB_MSG(("<--AdjHandler"));
}


void handleRefPowXAdjustment(const char * const adjName)
/* ----------------------------------------------------------------------
   1. Get info from where adj is called? From Adj platform or within scan
   2. Add new entry to the list of existing adjmnt chains. It specifies hw
      for which adjmnt is done. If hw changes then adjmnt will be redone
   3. Store current state of tx chain into par CurrentTxState for later
      comparison, see SysConfigHandler()
   4. Restore latest adjmnt pars for this hw context. Without it a later
      protocol re-opening in adj-platform wouldn't have values for them.
   5. Prep upcoming adjmnts:
      a. create list of required sub-adjmnts
      b. register sub-adjmnts
      c. prep pars for each sub-adjmnt
   6. check if tx-chan is 1H. Add to report that these vals  won't become
      adjmnt result pars.
    ---------------------------------------------------------------------*/
{
  // 1.
  ADJ_HANDLER_CONTEXT mode;

  if(PTB_AdjHandlerCall(&mode))
  {
    switch(mode)
    {
      default:

      case HANDLE_ACQUISITION:  //called from within a scan
        InAdjPlatform = No;
        AdjRunning    = Yes;
        break;

      case HANDLE_PLATFORM:     //called from adj platform
        InAdjPlatform = Yes;
        AdjRunning    = No;
        ParxRelsMakeNonEditable("CONFIG_SCAN_operation_mode");
        break;
    }
  }

  // 2.
  MRT_AdjSetHwScope(PHYS_TRANS_CHAIN, 1);

  // 3.
  SysInfo_TxState(1, SysInfo_TxRxState::Physical, CurrentTxState);

  // 4.
  restoreAdjResults();

  // 5a.
  std::string subp; int nsubp=0;

  if (!SkipSubAdjustments)
  {                //string of comma-separated listed sub-adjmnts
    subp=RG_ADJNAME;
    subp += ",";
    nsubp ++;

    subp +="ArrayPhase";
    subp += ",";
    nsubp ++;
  }

  subp += adjName;
  nsubp ++;

  // 5b.
  if(PTB_CheckSubProcess(0)==Yes)
  {

    if( nsubp != PTB_InitSubProcess(subp.c_str()) )
      UT_ReportError("AdjRefPowX: Unable to initialize subprocesses"
                   " for RG and RefPow adjustment");
    return;
  }

   // 5c.
  if( PTB_CheckSubProcess(RG_ADJNAME) )
  {
    prepPars_RgAdj();
  }
  else if( PTB_CheckSubProcess("ArrayPhase") )
  {
    prepPars_RxPhaseAdj();
    STB_ArrayPhaseHandleAdjustment();
  }
  else if ( PTB_CheckSubProcess(adjName) )
  {
    prepPars_RefPowXAdj();
  }


 // 6.
  if ( !strcmp(PVM_Nucleus1,"1H") )
  {
    std::string msg = "AdjRefPowX is not intended to adjust reference power ";
    msg += "on channel 1H. If you want to use this result for 1H, then enter ";
    msg += "it manually into AdjRefPow's result card and 'Save Adjustment Results' ";
    msg += "via rigth mouse button.";

    setmessage(msg.c_str());
  }
}

void prepPars_RxPhaseAdj(void)
/* ----------------------------------------------------------------------
   Rx phase adjmnt needs only one NExcStep. But which pulse duration or
   power is suitable? If an rg-adjmnt preceded there is some knowledge
   stored as adjmnt-result. 
   1. restore rg-adjmnt result containing an appropriate ExcDur/Pow
   2. if no result from rg-adjmnt for pulse setting take first from list
   3. prepare adjmnt protocol 
   --------------------------------------------------------------------- */
{
  DB_MSG(("-->prepPars_RxPhaseAdj"));

    // 1.
    PvAdjManRestorePar("RgResult", 0, 0, 0);

    // 2.
    if( RgResult > 0)
    {
      if (ScanMode == CONST_PULSE_POW)
        ExcDurList[0] = RgResult;
      else
        ExcPowList[0] = RgResult;
    }

    // 3.
    NExcSteps=1;
    NoiseScan = 0;
    PVM_ArrayPhaseAdjMode = ArrayPhase_Adj_Force;

    /*Turn water suppression pulses off*/
    PVM_WsOnOff = Off;
    ParxRelsParRelations("PVM_WsOnOff",No);
    PVM_DriftCompYesNo = No;
    ParxRelsParRelations("PVM_DriftCompYesNo",Yes);
    PVM_NAverages = 1;
    ParxRelsParRelations("PVM_NAverages",No);
    PVM_NRepetitions = 1;
    ParxRelsParRelations("PVM_NRepetitions",No);

    backbone();

  DB_MSG(("<--prepPars_RxPhaseAdj"));
}


void prepPars_RefScanAdj(void)
{
  DB_MSG(("-->prepPars__RefScanAdj"));

    PVM_DecPower = PVM_NoePower = CFG_RFPulseHighestAttenuation();
    PVM_WsOnOff = Off;
    ParxRelsParRelations("PVM_WsOnOff",No);
    PVM_DriftCompYesNo = No;
    ParxRelsParRelations("PVM_DriftCompYesNo",No);
    NoiseScan = 0;
    backbone();

  DB_MSG(("<--prepPars__RefScanAdj"));
}

void prepPars_RgAdj(void)
/* ----------------------------------------------------------------------
   Not knowing flip angle/pulse power-relation at this point the signal
   of each NExcStep needs to be checked => rg-adjmnt takes as long as 
   scan itself (neglecting NA).

   1.  Adapt protocol parameters for RG adjustment.
       keep pulse lists and nexcsteps, but analyze signals in RgAnalyze
       one by one to try to shorten adjmnt:
   1a. in auto-gsp first 2 scans do not enter rgdoadjmnt but lists are
       incremented in ppg. rotate pulse list to get the start right
   1b. rebuild of acq_jobs after backbone-call to get transActBlocks=1.
   2.  Define which pars are adjmnt results
   3.  Hand results to AdjParameterList
   4.  Set AutoCounter to start adjmnt
   ---------------------------------------------------------------------*/
{
  DB_MSG(("-->prepPars_RgAdj"));

  double *lst = NULL;
  int  lstsze = NExcSteps;

  lst = ScanMode == CONST_PULSE_POW ? ExcDurList : ExcPowList;

  // 1.
  PVM_DummyScansDur = 2*PVM_RepetitionTime;
  PVM_NAverages = 1;
  RecoMode = MAGN_RECO_MODE;
  backbone();

  NoiseScan = 0;
  ATB_SetNI( 1 );

  // 1a.
  if (lstsze>2)
  {
    std::vector<double> exclst( lst, lst + lstsze);
    std::rotate(exclst.rbegin(), exclst.rbegin() + 2, exclst.rend());
    std::copy  (exclst.begin(),  exclst.end(), lst);
  }

  // 1b.
  JOBPARAMETERS(jobs);
  JOBDESCRIPTION job0 = jobs->getOrCreateJob("job0");

  job0->initPars      (1,PVM_SpecMatrix[0], PVM_SpecSWH[0]);
  job0->appendLoop    (PVM_DummyScans, LOOP_DUMMIES);
  job0->appendLoopList(AverageList, 1, LOOP_AVERAGE);
  job0->appendLoop    (NI+NoiseScan, LOOP_SETUP);

  PARX_change_dims("RgMagn", lstsze);
  memset(RgMagn, 0.0, lstsze*sizeof(double));

  if (PVM_DriftCompYesNo)
  {
    int nTotScansJob1 = NI * PVM_NAverages;

    ATB_SetDriftCompBaseLevel(jobs, 1, nTotScansJob1, PVM_DummyScans);
  }

  // 2.
  static const ADJ_RESULT_PAR rgResultPars[] =
  {
        { adj_type, "ACQ_jobs"},
        { adj_type, "PVM_AutoRg"},
        { adj_type, "RgResult"}
  };

  // 3.
  PvAdjManSetAdjParameterList( rgResultPars, ARRAY_LEN(rgResultPars) );

  // 4.
  strcpy (ACQ_SetupAutoName, "RgAutoCounter");

  DB_MSG(("<--prepPars_RgAdj"));
}


void prepPars_RefPowXAdj(void)
/* ----------------------------------------------------------------------
   1. Adapt protocol parameters for RefPowX adjustment
   2. Define pars which hold adjmnt result
   3. Hand them to AdjParameterList
   4. Create a new expno to hold all data created during gop-adjmnt

   No AutoCounter - this is a GOP-adjmnt.
   ---------------------------------------------------------------------*/
{
  DB_MSG(("-->prepPars_RefPowXAdj"));

  // 1.
  SliceNum = 1;
  PVM_DummyScans = 1;

  /*Define dataset name of GOP adj which is set in deriveVisu*/
  char *prot = AdjProtName;

  const char *const adjprot = PTB_GetAdjProtocol();

  if( adjprot && No==PTB_AdjMethSpec() )
    snprintf( prot, 64, "%s", PTB_GetAdjProtocol() );
  else if(*prot=='\0')
    snprintf(prot, 64, "AdjRefPowX");

  backbone();

  // 2.
  static const ADJ_RESULT_PAR gopAdjPars[] =
    {
      { study_result, "PVM_StudyRefPow"  },
      { study_result, "PVM_AdjRefPowXDur"},
      { study_result, "PVM_AdjRefPowXPwr"},
      { scan_result,  "PVM_RefPowCh1"    },
      { scan_result,  "PVM_RefPowStat1"  }
    };

  // 3.
  PvAdjManSetAdjParameterList( gopAdjPars, ARRAY_LEN(gopAdjPars) );

  // 4.
  PvAdjManRequestNewExpno(Yes);

  DB_MSG(("<--prepPars_RefPowXAdj"));
}


void restoreAdjResults(void)
{
  DB_MSG(("-->RestoreAdjResults"));

    const AdjChainContext chainContext = MRT_AdjGetHwScope(PHYS_TRANS_CHAIN, 1);
    PvMrAdj::MrHardwareContext hwContext(&chainContext, 1);

    AdjResultType adjResTyp = study_result;

    PvAdjManRestoreParFromHwContext("PVM_AdjRefPowXPwr","", CONFIG_REFPX_ADJNAME,
                                    &adjResTyp, hwContext);
    PvAdjManRestoreParFromHwContext("PVM_AdjRefPowXDur","", CONFIG_REFPX_ADJNAME,
                                    &adjResTyp, hwContext);
    PvAdjManRestoreParFromHwContext("PVM_StudyRefPow"  ,"", CONFIG_REFPX_ADJNAME,
                                    &adjResTyp, hwContext);
    SetRecoParam();

  DB_MSG(("<--RestoreAdjResults"));
}


void GopAdjResultHandler(void)
{
  DB_MSG(("-->GopAdjResultHandler"));

  int  overflowDetected = check_adc_overflow();

  if(overflowDetected)
  {
    /* AdjPerformState=adj_failed; */
    /* PTB_RegisterGopAdj(); */
    /* snprintf(AdjErrorComment,255,"%s", oerr.str().c_str()); */
    /* AdjErrorComment[255]='\0'; */
    /* UT_ReportError(AdjErrorComment); */
    AdjPerformState=adj_successful;
    PTB_RegisterGopAdj();
  }
  else
  {
    AdjPerformState=adj_successful;
    PTB_RegisterGopAdj(); // makes the map available for database
  }

  UpdateResultPars(AdjRunning);

  DB_MSG(("<--GopAdjResultHandler"));
}


void RgAutoCounterRel(void)
/* -------------------------------------------------------------------

   ------------------------------------------------------------------*/
{
  DB_MSG(("-->RgAutoCounterRel"));

  if ( !ParxRelsParHasValue("RgAutoCounter") )
  {
    RgAutoCounter = 0;
    RgScanCounter = 0;
  }

  if ( SkipSubAdjustments )
  {
    RgAutoCounter = 0;
    return;
  }

  if( RgAutoCounter == 0 )
  {
    ATB_RgAdjSetGsPars("RgAutoCounter");
    ATB_AutoRgInit();
    RgAutoCounter = 1;
    RgScanCounter = 0;
  }
  else
    rgDoAdjustment();

  RgScanCounter++;

  if (RgScanCounter>NExcSteps-1)
    RgScanCounter=0;

  DB_MSG(("<--RgAutoCounterRel"));
}


void rgDoAdjustment()
/* ----------------------------------------------------------------------
   With transActBlock 1 standard ATB_AdjRgAlg would finish after a single
   scan (case 0). But other rf-pulses need to be checked also. RgAnalyze()
   decides if to finish or not by overwriting RgAutoCounter
   1. get access to jobs' receiver gains to be set & receiver filling
   2. get signal prepped by Reco's InsertRgFilter, provide to RgAnalyze.
   3. control adjmnt outcome 
   ---------------------------------------------------------------------*/
{
  DB_MSG(("-->rgDoAdjustment"));

  // 1.
  JOB_DESCRIPTION_TYPE *jobs = ACQ_jobs;
  int        idx0 = 0,         idx1 = 1;
  double digfill0 = 101.0, digfill1 = 101.0;

  if (PVM_DriftCompYesNo)
  {
    idx1=1;
    digfill1=ATB_GetDigFill(idx1);
    ATB_AdjRgAlg(digfill1, &ACQ_jobs[idx1].receiverGain, 3, 60, No);
  }

  digfill0 = ATB_GetDigFill(idx0);

  // 2.
  RgMagn[RgScanCounter] =(*NormFtArea) / ACQ_jobs[idx0].receiverGain;
 
  // 3.
  switch(  ATB_AdjRgAlg(digfill0, &jobs[0].receiverGain, 5, 80, No)  )
  {
    default:

      case 1:
        DB_MSG(("   Adjusting RG: digfill %f",digfill0));
        RgScanCounter++;
        return;
        break;

      case -6:
        sprintf(AdjErrorComment, "   RG Adjustment: Illegal limits for digfilling "
                    "passed :\n %5.1f (lower limit) %5.1f (upper limit)",5.0, 80.0);
        RgAutoCounter=-1;

        return;

      case -2:          //ultimate signal overflow
        sprintf(AdjErrorComment, "   RG Adjustment: Signal too larger:\n. Digitizer "
                                                  "overflow at lowest possible gain");
        RgAutoCounter=-1;

        return;
        break;

      case -1:          //signal underflow, allowed
        RgAutoCounter = 0;

        break;

      case -3:
        sprintf(AdjErrorComment, "   RG Adjustment: Stop due to unstable signal");
        RgAutoCounter = -1;
        break;

      case 0:           // OK, finish normally
        RgAutoCounter = rgAnalyze( RgMagn, NExcSteps, RgScanCounter );

        ATB_SetAutoRg( jobs[idx0].title, jobs[idx0].receiverGain );

        if (PVM_DriftCompYesNo)
          ATB_SetAutoRg( jobs[idx1].title, jobs[idx1].receiverGain );

        break;
    }

  DB_MSG(("<--rgDoAdjustment"));
}

int rgAnalyze(double *magn, int sze, int n)
/* ----------------------------------------------------------------------
   Attempt to shorten rg-adjmnt: pt1-4 trivial
 
   1.  datastation: set defaults & quit
   2.  only one NexcStep: finished & quit
   3.  all pulses equal : finished w one scan & quit
   4.  finish when a magnitude maximum is found:
   4a. if current magn is smaller than previous one and there was a magn
       increase in previous analysis (rgtndncy +1) then we assume that
       peak is passed: finish. tndncy is to protect from taking the first
       pt of a decending curve as max magn.
       store corr. val of pulse for usage in rxphase-adjmnt 

   return :   0: finished w OK, 1: continue
   ---------------------------------------------------------------------*/
{
  int retval = 1;

  // 1.
  if( CFG_IsDatastation() )
  {
    PVM_AutoRg[0].dvalue = ACQ_jobs[0].receiverGain = CFG_RgValue(20.0,2);

    if (PVM_DriftCompYesNo)
      PVM_AutoRg[1].dvalue = ACQ_jobs[1].receiverGain = CFG_RgValue(20.0,2);

    return 0;
  }

  // 2.
  if ( sze==1 )
    return 0;

  // 3.
  if ( ScanMode == CONST_PULSE_POW )
    retval=checkRgArray(ExcDurList, NExcSteps);
  else
    retval=checkRgArray(ExcPowList, NExcSteps);

  if ( retval==0 )
    return retval;

  // 4.
  if (n > 1)
  {
    // at least 1% difference in magnitude.
    // may be improve with help of noise scan
    if ( magn[n] > 1.01 * magn[n-1] )
    {
      RgMagnTndncy = +1;
    }
    else if  ( magn[n] < 0.99*magn[n-1] )
    {

  // 4a.
      if (RgMagnTndncy > 0)
      {
        if ( ScanMode == CONST_PULSE_POW )
          RgResult = ExcDurList[n];
        else
          RgResult = ExcPowList[n];

        retval = 0;
      }

      RgMagnTndncy = -1;
    }
    else
    {
      //RgMagnTndncy = 0;
    }
  }
  else
  {
      RgMagnTndncy = 0;
  }

  // finish when all NExcSteps done
  if ( n >= sze-1 )
   retval = 0;

  return retval;
}


void SysConfigHandler(void)
/* ----------------------------------------------------------------------
   Redirection of SysConfigHandler needed because:
   if in adjustment platform then backbone's call of STB_UpdateNuclei has
   2nd arg 'No' (= no restore of previously adjusted ref pow) to prevent
   overwriting. However, if coil elements or operation modes have been
   changed then STB_UpdateNuclei(Yes) needs to be used for correct array
   dims etc

   1. Get current state of tx chain
   2. Compare with former state when AdjPlatform was entered.
      If different call nuclei updater w restore ref power.
   ---------------------------------------------------------------------*/
{
  DB_MSG(("-->SysConfigHandler"));

  if(InAdjPlatform)
  {
    //1.
    char curTxState[PVSYSINFOCHANSTATE_LEN];
    
    SysInfo_TxState(1, SysInfo_TxRxState::Physical, curTxState);

    //2.
     if( strcmp(curTxState, CurrentTxState) )
      STB_UpdateNuclei(Yes);
  }

  backbone();

  DB_MSG(("<--SysConfigHandler"));
}


static int checkRgArray(double* a, int size)
/* ----------------------------------------------------------------------
   check if all elements of array are equal: this would shorten adjmnt.
   Return val as needed for RgAutoCounter (0: adj OK, 1: adj continue)
   ---------------------------------------------------------------------*/
{
  for (int i=1; i<size; i++)
    if ( *a != *(a+i) )
      return 1;                        // not all equal, need to continue;

  return 0;               // all equal, adjmnt can be finished w one scan;
}
