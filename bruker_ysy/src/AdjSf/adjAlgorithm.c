/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (c) 2016 Bruker BioSpin MRI GmbH";


#define DEBUG           0
#define DB_MODULE       0
#define DB_LINE_NR      0


#include "method.h"
#ifndef CPROTO
#include "libCore/Base/AutoPtr.hh" //for ArrayAutoPointer
#endif


void InitSfAdj(void)
{
  int nrec=0,i;
  // force method controlled setting of RECO parameter 
  SetdisplayInfonames();

  SetRecoParam();
  ACQ_SetupOnlineReco  = Yes;
  ParxRelsParRelations("ACQ_SetupOnlineReco",Yes);
  ACQ_SetupRecoDisplay = Yes;
  ParxRelsParRelations("ACQ_SetupRecoDisplay",Yes);
  ACQ_SetupType          = Spectrometer_Parameters;
  ParxRelsParRelations("ACQ_SetupType",Yes);
  ACQ_SetupShowDigFilling = Yes;
  ParxRelsParRelations("ACQ_SetupShowDigFilling",Yes);
  FreqAdjCounter = 1;

  // initialize receiver gain

  JOB_DESCRIPTION_TYPE *ajobs=ACQ_jobs;
  ajobs[0].receiverGain = ATB_GetAutoRgValue(ajobs[0].title);

  ATB_AdjRgAlg(-1,0,1,50, Yes);
  nrec = RecoNrActiveReceivers();

  PARX_change_dims("SigFail",nrec);
  PARX_change_dims("NormArea",Nexc,nrec);
  PARX_change_dims("OFrq",Nexc,nrec);
  PARX_change_dims("WOFrq",Nexc);
  PARX_change_dims("Peak",Nexc,nrec);
  PARX_change_dims("Noise",Nexc,nrec);
  PARX_change_dims("PWidth",2,Nexc,nrec);

  for(i=0;i<nrec;i++)
  {
    SigFail[i] = No;
  }

  O1=0.0;
  ParxRelsParRelations("O1",Yes);
 
  return;
}

void SfAdjCounterRel(void)
{
  DB_MSG(("-->SfAdjCounterRel"));
  try
  {
    double digfill;
    YesNo failure=No;
    int nchan=RecoNrActiveReceivers();

    if(!ParxRelsParHasValue("FreqAdjCounter") ||
       FreqAdjCounter == 0)
    {
      InitSfAdj();
      DB_MSG(("<--SfAdjCounterRel"));
      return;
    }

    if(Yes==CFG_IsDatastation())
    {
      FreqAdjCounter=0;
      FinishSfAdj();
    }

    /*
     *  adjust receiver gain
     */

#if DEBUG
    for(int i=0; i < Nexc; i++)
      for(int j=0;j<nchan; j++)
        DB_MSG(("NormArea[%d][%d] %f",
                i,j,NormArea[i][j]));
#endif



    digfill= ATB_GetDigFill();
  
    switch(ATB_AdjRgAlg(digfill,
                        &ACQ_jobs[0].receiverGain,
                        10,
                        50,
                        No))
    {
      default:
      case 1:
        ParxRelsParRelations("ACQ_jobs[0].receiverGain",Yes);
        DB_MSG(("<--SfAdjCounterRel, adjusting RG"));
        return;
        break;
      case -6:
        sprintf(AdjErrorComment,
                "Illegal limits for digfilling passed:\n"
                " %5.1f (lower limit) %5.1f (upper limit)",10.0,50.0);
        FreqAdjCounter=-1;
        FinishSfAdj();
        return;
      case -2:
        sprintf(AdjErrorComment,"Signal Intensity too High:\n"
                "Digitizer overflow at lowest possible receiver gain");
        FreqAdjCounter=-1;
        FinishSfAdj();
        return;
        break;
      case -1:
        /* signal underflow, allowed */
        break;
      case -3:
        sprintf(AdjErrorComment,"Freq: RG Adjustment:\nStop due to unstable signal");
        FreqAdjCounter=-1;
        FinishSfAdj();
        break;
      case 0:
        break;
    }
    // RG adjusted, now evaluate frequency offsets

    std::vector<double> peak2noise(nchan,0);
    double dval,max;
    int nbands=Nexc;
    int band = 0;
    // find max peak in frequency list
    max=0.0;
    for(int i=0;i<nbands;i++)
    {
      for(int j=0;j<nchan;j++)
      {
        dval=Peak[i][j];
        if(max < dval)
        {
          band=i;
          max=dval;
        }
      }
    }

    std::stringstream errmsg;
    errmsg << "Signal failure on active channel " << std::endl;
    for(int chan=0;chan<nchan;chan++)
    {  
      dval = Noise[band][chan];

      peak2noise[chan] =  fabs(dval) > 1.0e-3 ? 
        fabs(Peak[band][chan]/dval) : 0.0;

      if(peak2noise[chan] < 10)
      {
        SigFail[chan]=failure=Yes;
        errmsg << "("
               << chan+1 << ") " 
               << "P/Noise : " << peak2noise[chan] << std::endl; 
      }
    }

    if(failure==Yes)
    {
      int nrec=CFG_NReceivers(),rec;
      int chan;
      errmsg << "Attached on receiver ";
      for(rec=0,chan=0;rec < nrec;rec++)
      {
        if(nchan != 1 && ACQ_ReceiverSelect[rec]==No)
          continue;
        else
        {
          if(chan >= nchan)
            break;
          else if(SigFail[chan]==No)
            chan++;
          else
          {
            errmsg << "(" << rec+1 << ") ";
          }
        }
      }
      errmsg << std::endl;
      throw PvStdException(errmsg.str().c_str());
    }
    BF1 -= (WOFrq[band]-FreqOffsList[band])*1e-6;
    ParxRelsParRelations("BF1",Yes);
    FreqAdjCounter=0;
    FinishSfAdj();
  }
  catch (const PvException &ex)
  {
    DB_MSG(("Exception occured:\n%s\n",ex.what()));

    int len=(int) PARX_get_dim("AdjErrorComment",1);
    
    snprintf(AdjErrorComment,len,ex.what());
    AdjErrorComment[len-1]='\0';
    FreqAdjCounter = -1;
    FinishSfAdj();
  }

  DB_MSG(("<--SfAdjCounterRel"));
}

void FinishSfAdj(void)
{

  NChan[0]=RecoNrActiveReceivers();
  NChan[1]=CFG_NReceivers();
  

  if(FreqAdjCounter == 0)
  {
    JOB_DESCRIPTION_TYPE *ajobs=ACQ_jobs;

    SetBf1(BF1);
    ATB_SetAutoRg(ajobs[0].title,ajobs[0].receiverGain);
  }

  const char *const grouplist[] = \
    { "AdjResultReportGroup","MethodClass" };

  int id=0;
  PTB_StoreInParentDset(No,
                        grouplist,
                        2,
                        "FreqAdjustment",
                        "Results",
                        &id,
                        No);
  
}

void SetBf1(double bf)
{
  if(!strcmp(PVM_Nucleus1, "1H"))
  {
    PVM_FrqAdj1H = bf;  
    DB_MSG(("bf = %g", bf));
  }
  else
  {
    double frq1H0ppm = PVM_FrqAdj1H/(1+1e-6*PVM_FrqAdj1HPpm);
    PVM_FrqRefPpm[0] = PVM_FrqWorkPpm[0] 
      + 1e6*(1.0-bf/(CFG_GammaRatio(PVM_Nucleus1)*frq1H0ppm));
    MRT_NucleiPpmRefSet(PVM_Nucleus1,PVM_FrqRefPpm[0]); 
  }
}

void SetdisplayInfonames(void)
{

  
  std::string recoinfopars="OFrq/Frequency Offset, PWidth[0]/LineWidth";
  PARX_change_dims("ACQ_RecoInfoNames",recoinfopars.size()+1);
  ParxRelsParSetValue("ACQ_RecoInfoNames",recoinfopars.c_str());
  std::string signalinfopars="NormArea/Normalized Area";
  size_t slen=signalinfopars.size()+1;

  if(1!=(int)PARX_get_dim("ACQ_SignalInfoNames",1) ||
     slen!=PARX_get_dim("ACQ_SignalInfoNames",2))
  {
    PARX_change_dims("ACQ_SignalInfoNames",1,slen);
  }    
  ParxRelsParSetValue("ACQ_SignalInfoNames[0]",signalinfopars.c_str());

}


