/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2005-2015
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2005-2015 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0


#include "method.h"

#define PROTON_PPM_DEFAULT 4.7
#define FILT_STOP_FAC 0.04

void backbone(void)
{
  double dval,ov=0.0;
  int n=0,i;

  DB_MSG(("-->backbone %d",  Nexc));


  PVM_NAverages = 1;
  PVM_NRepetitions =1;
  AcqSize = 8192;

  STB_UpdateNuclei(Yes);

  STB_UpdateAtsReferencePosition();

  if(!strcmp(PVM_Nucleus1, "1H"))
  {
    if(!ParxRelsParIsEditable("FrqRef"))
      ParxRelsMakeEditable("FrqRef");
  }
  else 
  {
    if(ParxRelsParIsEditable("FrqRef"))
      ParxRelsMakeNonEditable("FrqRef");
  }
 
  FrqRef=PVM_FrqRef[0];
  FrqRefPpm=PVM_FrqRefPpm[0];

  PVM_EffSWh = FreqRes*AcqSize;
  MRT_UpdateDigPars(&PVM_EffSWh,AcqSize,0,&PVM_AcquisitionTime);
  FreqRes = PVM_EffSWh/AcqSize;

  PARX_change_dims("PVM_AntiAlias", 1);
  PVM_AntiAlias[0]=1.0;

  STB_UpdateEncoding();

  ov = FILT_STOP_FAC*PVM_EffSWh;
  FreqAdjRange = Nexc*PVM_EffSWh - (Nexc-1)*ov;
  
  PARX_change_dims("FreqOffsList",Nexc);
  
  FreqOffsList[0]=0.0;
  ov = FILT_STOP_FAC*PVM_EffSWh;
  dval = PVM_EffSWh-ov;

  /*  algorithm to set up alternating frequency list  */
  /*  n = Nexc/2; */
  /*   DB_MSG(("n=%d Nexc = %d",n,Nexc)); */

  /*   for(i=1;i<=n;i++) */
  /*   { */
  /*     int j; */
  /*     j=2*i-1; */
  /*     FreqOffsList[j] = dval*(n-i+1); */
  /*     FreqOffsList[j+1] = -FreqOffsList[j]; */
  /*   } */
  n = Nexc;

  DB_MSG(("n=%d Nexc = %d",n,Nexc));

  double * freqOffsList = FreqOffsList;

  for(i=0;i<n;i++)
  {
    freqOffsList[i] = dval*(i-(int)(n/2));
  }
 
  DB_MSG(("Frequency list calculated"));
  FreqRes = 1000.0/(PVM_AcquisitionTime);
 
  PVM_TriggerMode   = per_PhaseStep;
  dval = STB_UpdateTriggerModule();

  int nchan=RecoNrActiveReceivers();

  if(nchan < 1)
  {
    throw PvStdException("RecoNrActiveReceivers returns illegal"
                         " value %d\n",nchan);
  }

  // 2 peak width limits 
  RedimFrqOffsetFilterPars(nchan,Nexc,2);

  dval =
    0.03                      +  /* UpdateDynPars */
    0.01                      +  /* freqRx:f1 */
    dval                      +
    0.01                      +  /* freqTx:f1 */
    ExcPulse1.Length          +
    0.05                      +  /* denab+de  */
    PVM_AcquisitionTime       +
    5.0                       +
    0.01                      +  /* ADC_END   */
    5.0;                         /* min d0    */
    
  /* constrain min repetition time to 300 ms */
  PVM_RepetitionTime = MAX_OF(PVM_RepetitionTime,300);
  PVM_RepetitionTime = MAX_OF(PVM_RepetitionTime,dval);
  PVM_MinRepetitionTime = dval;


  PTB_ClearAdjustments(No); // clears method specific adjustments
  PTB_AppendAdjustment("ThisAutoSf",
                       "Frequency Adjustment",
                       "Adjustment of basic frequency",
                       on_demand);

  PTB_AppendOrderAdjustment(per_scan, per_scan, RG_ADJNAME);

  SetBaseLevelParam();
  SetRecoParam();


  DB_MSG(("<--backbone"));
  return;
}



static int NextPowOfTwo(int size)
{

  int si1,si2;
  double dval;


  
  dval = log((double)size)/log(2.0);

  si1  = 1 << (int)ceil(dval);
  si2  = 1 << (int)floor(dval);


  return fabs(size - si1) <= fabs(size - si2) ? si1:si2;
}


void CheckAcqSize(int *size)
{
  DB_MSG(("-->CheckAcqSize"));

  *size = MAX_OF(*size,128);
  *size = MIN_OF(MAX_OF(*size,128),8192);
  *size = NextPowOfTwo(*size);

  DB_MSG(("<--CheckAcqSize"));
}

void AcqSizeRange(void)
{
  DB_MSG(("-->AcqSizeRange"));

  if(!ParxRelsParHasValue("AcqSize"))
  {
    AcqSize=1024;
  }
  else
  {
    CheckAcqSize(&AcqSize);
  }
  DB_MSG(("<--AcqSizeRange"));
  return;
}



void AcqSizeRel(void)
{
  UT_SetRequest("AcqSize");
  AcqSizeRange();
  backbone();
}

void EffSWhRange(void)
{
  DB_MSG(("-->EffSWhRange"));


  if(!ParxRelsParHasValue("PVM_EffSWh"))
  {
    PVM_EffSWh = 50000;
  }
  else
  {
    double dval;

    dval = PVM_EffSWh;
    PVM_EffSWh = MAX_OF(MIN_OF(dval,50000),1000);
  }

  DB_MSG(("<--EffSWhRel"));
  return;
}


void EffSWhRel(void)
{
  DB_MSG(("-->EffSWhRel"));

  EffSWhRange();
  backbone();
  DB_MSG(("<--EffSWhRel"));
  return;
}




/* RF pulse parameter handling */

void ExcPulse1EnumRel(void)
{
  DB_MSG(("-->ExcPulse1sEnumRel"));

  UT_SetRequest("ExcPulse1Enum");
  UpdateRfPulsePars();
  
  DB_MSG(("<--ExcPulseEnumRel"));                                       
}



void ExcPulse1Range(void)
{
  DB_MSG(("-->ExcPulse1Range"));
  double dval=ExcPulse1.Flipangle;

  ExcPulse1.Flipangle = MAX_OF(MIN_OF(360.0,dval),1.0);

  
  DB_MSG(("<--ExcPulseRange"));
  
}


void ExcPulse1Rel(void)
{
  DB_MSG(("-->ExcPulse1Rel"));

  UT_SetRequest("ExcPulse1");
  ExcPulse1Range();
  UpdateRfPulsePars();


  DB_MSG(("<--ExcPulse1Rel"));
  return;
}

void ExcPulse1AmplRel(void)
{
  UT_SetRequest("ExcPulse1Ampl");
  RfPulseAmplRel();
}

void RfPulseAmplRel(void)
{
  STB_UpdateRFShapeAmplitude("ExcPulse1Ampl",No);
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]");
}

void RfPulseDurRel(void)
{
  DB_MSG(("-->RfPulseDurRel"));

  ExcPulse1.Length=RfPulseDur;  
  ParxRelsParRelations("ExcPulse1.Length",No);

  DB_MSG(("<--RfPulseDurRel"));
}

void PulseDurRel(void)
{
  DB_MSG(("-->PulseDurRel"));

  UT_SetRequest("P");
  UpdateRfPulsePars();

  DB_MSG(("<--PulseDurRel"));
  return;
}

void UpdateRfPulsePars(void)
{
  const char *const parlist = \
    "RfPulseDur,"     /* 1 */
    "ExcPulse1Enum,"  /* 2 */
    "ExcPulse1,"      /* 3 */
    "P";              /* 4 */


  YesNo requestDetected = No;
  YesNo callBackbone = No;
  int par=0;


  DB_MSG(("-->UpdateRfPulsePars"));


  requestDetected = UT_GetRequest(parlist,&par);


  if(Yes==requestDetected)
  {
    switch(par)
    {
      case 4:
        ExcPulse1.Length = P[0]/1000.0;
        callBackbone = Yes;
        break;
      default:
        callBackbone = Yes;
        break;
    }
  }
  else
  {
    callBackbone=Yes;
  }
		    

  STB_UpdateRFPulse("ExcPulse1",1,No,Conventional);

  RfPulseDur = ExcPulse1.Length;

  if(Yes == callBackbone)
  {
    backbone();
  }

  DB_MSG(("<--UpdateRfPulsePars"));
}

void FreqResRange(void)
{
  DB_MSG(("-->FreqResRange"));

  if(!ParxRelsParHasValue("FreqRes"))
  {
    FreqRes = 6.0;
  }
  else
  {
    double dval;
    dval = FreqRes;
    FreqRes = MAX_OF(MIN_OF(15,dval),3.2);
  }


  DB_MSG(("<--FreqResRange"));
}

void FreqResRel(void)
{
  DB_MSG(("-->FreqResRel"));

  FreqResRange();
  backbone();

  DB_MSG(("<--FreqResRel"));
}

void FreqAdjRangeRel(void)
{
  DB_MSG(("-->FreqAdjRangeRel"));

  backbone();

  DB_MSG(("<--FreqAdjRangeRel"));
}

void NexcRange(void)
{
  DB_MSG(("-->NexcRange"));

  int NexcLast=0;

  if(!ParxRelsParHasValue("Nexc"))
  {
    Nexc = 3;
  }

  if (UT_GetRequest("Nexc") == Yes)
    UT_RequestParOrigValue(&NexcLast);
 
  int ival = MIN_OF(MAX_OF(Nexc,1),17);

  if(!(ival % 2))
  {
      if (ival > NexcLast) ival++;
      else ival--;
  }

  Nexc = ival;

  DB_MSG(("<--NexcRange"));
}

void NexcRel(void)
{
  DB_MSG(("-->NexcRel"));

  UT_SetRequest("Nexc");
  NexcRange();
  UT_ClearRequest(); 
  backbone();
 
  DB_MSG(("<--NexcRel"));
}

void HandleAdjustmentRequests(void)
{

  DB_MSG(("-->HandleAdjustmentRequests"));
  std::string adjName = PTB_GetAdjName();

  if(Yes==STB_AtsAdj())
    return;

  PVM_AtsChangeRefPos=No;
  ParxRelsHideInEditor("PVM_AtsChangeRefPos");
  ParxRelsMakeNonEditable("PVM_AtsChangeRefPos"); 

  if(PVM_AtsBedPositionMode==ATS_POS_IMAGE_CENTER)
    PVM_AtsBedPositionMode=ATS_POS_REFERENCE;
  STB_UpdateAtsReferencePosition();


  if(0 == adjName.find(RG_ADJNAME))
  {
    ParxRelsParRelations("PVM_AdjHandler",Yes);
    return;
  }

  sprintf(ACQ_SetupAutoName,"FreqAdjCounter");

  static const ADJ_RESULT_PAR bfResultPars[]=
    { {study_result, "PVM_FrqAdj1H"},            // - result for 1H (frequency of highest peek)
      {study_result, "PVM_FrqAdj1HPpm"},         // - result for 1H (ppm of highest peek)
      {study_result, "PVM_NucleiPpmRef"},        // - reference point of x nucleus frequency
      {scan_result,"O1"}                         // - used to apply manually adjusted offsets O1
    };
  PvAdjManSetAdjParameterList(bfResultPars, ARRAY_LEN(bfResultPars));    

  ParxRelsMakeEditable("ApplyManAdjOffset,FrqResetCfg");    // - editability activates mechanism to transfer
                                                            //   value of O1 to the basic frequency
                                                            // - acitvates mechanism to reset basic frequency

  DB_MSG(("<--HandleAdjustmentRequests"));
  return;
}

void ApplyOffsetRel(void)
{
  if(ParxRelsParHasValue("O1") &&
     0.0 != O1               )
  {
    SetReferenceOffset(O1);
    O1=0.0;
  }

  backbone();
}

void FrqResetCfgRel(void)
{
  if(ParxRelsParIsEditable("FrqResetCfg"))
  {
    MRT_NucleiPpmRefInit();

    if(strcmp(PVM_Nucleus1, "1H"))
    {
      MRT_NucleiPpmRefSet(PVM_Nucleus1,0.0);
    }
    PVM_FrqAdj1H = CFG_BasicFrequency();
    PVM_FrqAdj1HPpm = PROTON_PPM_DEFAULT;
    
    backbone();
  }
}



void SetReferenceOffset(double offs)
{
 
  if(!strcmp(PVM_Nucleus1, "1H"))
  {
    PVM_FrqAdj1H += offs*1e-6;  
  }
  else
  {
    // increasing refppm reduces frequency, therefore minus here: 
    PVM_FrqRefPpm[0] -= offs/PVM_FrqRef[0]; 
    MRT_NucleiPpmRefSet(PVM_Nucleus1,PVM_FrqRefPpm[0]); 
  }
}


void FrqRefRel(void)
{
  SetBf1(FrqRef);
  backbone();
}

void FrqRefPpmRel(void)
{
  if(!strcmp(PVM_Nucleus1,"1H"))
  {
    PVM_FrqAdj1HPpm=FrqRefPpm;
  }
  else
  {
    MRT_NucleiPpmRefSet(PVM_Nucleus1,FrqRefPpm);
  }
  backbone();
}


void RedimFrqOffsetFilterPars(int nchan, int nspec, int npwl)
{

    PARX_change_dims("NormArea",nspec,nchan);
    PARX_change_dims("SigFail",nchan);
    PARX_change_dims("OFrq",nspec,nchan);
    PARX_change_dims("WOFrq",nspec);
    PARX_change_dims("Peak",nspec,nchan);
    PARX_change_dims("Noise",nspec,nchan);
    PARX_change_dims("PWidth",npwl,nspec,nchan);

}
