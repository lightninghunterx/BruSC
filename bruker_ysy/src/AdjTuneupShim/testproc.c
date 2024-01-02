/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2017
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2001-2017 Bruker BioSpin MRI GmbH";

#define DEBUG		  0 
#define DB_MODULE	 0
#define DB_LINE_NR 0

#include "method.h"
#include "lib/PvSysinfoClient.h"

#ifndef CPROTO
#include <stdlib.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "PvUtil/PvExcept.hh"
#include "PvUtil/PvUtil.h"
#include "localUtilities.h"
// static function declaration


static void testAndReportShimSysinfofunctions(void);

static std::string printout_shimvalues(std::vector<std::string> & sn,
                                       double *shimset);

static std::string printout_shimvalues(std::vector<std::string> & sn,
                                       double *shimset)
{
    std::ostringstream msg;
    std::vector<std::string>::iterator sit;
    double *sval;
    int i;
    for(i=0,sit=sn.begin(),sval=shimset;
        sit != sn.end();
        i++,sit++,sval++)
    {
       msg << "Shim[" << std::setw(2) << i << "]: "
           << std::setw(10) << *sit 
           << " " << std::setw(10) << *sval << std::endl; 
    }
    return msg.str();
}

static std::string printout_shimvalues(std::vector<std::string> & sn,
                                double *shim1, double *shim2);
static std::string printout_shimvalues(std::vector<std::string> & sn,
                                double *shim1, double *shim2)
{
    std::ostringstream msg;
    std::vector<std::string>::iterator sit;
    double *sv1,*sv2;
    int i;
    for(i=0,sit=sn.begin(),sv1=shim1,sv2=shim2;
        sit != sn.end();
        i++,sit++,sv1++,sv2++)
    {
       msg << "Shim[" << std::setw(2) << i << "]: "
           << std::setw(10) << *sit 
           << " " << std::setw(10) << *sv1 
           << " " << std::setw(10) << *sv2 
           << " " << std::setw(10) << (fabs(*sv1-*sv2)) << std::endl; 

    }
    return msg.str();
}

static int UpdateShimCoilSelectPar(const std::vector<std::string>  & cid, int *actcoil)
{
  int retval=0;
  int nshims=static_cast<int>(cid.size());
  if(nshims < 1)
    throw PvStdException("UpdateShimCoilSelectPar: Illegal shim identifier (size 0)");

  if(actcoil)
  {
    retval =*actcoil;
    retval=*actcoil=MAX_OF(MIN_OF(nshims-1,retval),0);
  }
        
  if(ParxRelsParHasValue("SelectShimCoil"))
  {
    
    if(UT_RelContext_ParName()=="SelectShimCoil" && actcoil)
    {
      try
      {
        ParxRelsParGetEnumValueAsInt("SelectShimCoil", actcoil);
        retval=*actcoil;
        if(retval < 0 || retval >= nshims)
        {
          *actcoil=retval=0;
          ParxRelsParBindEnum("SelectShimCoil", cid);
        }
      }
      catch(const PvException &ex)
      {
        retval=*actcoil = 0;
        ParxRelsParBindEnum("SelectShimCoil", cid);
      }
    }
  }
  else
  {
      ParxRelsParBindEnum("SelectShimCoil",cid);
  }

  ParxRelsParSetEnumValueFromInt("SelectShimCoil", retval);

  return retval;
}

static void testAndReportShimSysinfofunctions(void)
{
    // SysInfo_ShimMaxPow
    // SysInfo_NShims
    // SysInfo_DynamicShimming
    // SysInfo_ShimRiseTime
    // SysInfo_ShimNSPS(
    // SysInfo_ShimMaxPow
    // SysInfo_ShimMaxPowSPS
    // SysInfo_MaxCurrentSPS
    // SysInfo_MaxShimCurrent
    // SysInfo_ShimIdToName
    // SysInfo_ShimCurrConstrained
    // SysInfo_ShimPowConstrained
    // SysInfo_ShimMaxSumCurrent
    // SysInfo_ShimResistance
    // SysInfo_ShimIdentifiers
    // SysInfo_ShimState
    // SysInfo_ShimStatus
    // SysInfo_ShimFunctions
    // SysInfo_ShimSolidHarmField
    // SysInfo_ShimSolidHarmonicCoefficients
    // SysInfo_ShimParToSolidHarm
    // SysInfo_ShimSolidHarmToShimPar
    // SysInfo_ShimUnitBSMS
    // SysInfo_ShimOrder
    // SysInfo_ShimB0
    // SysInfo_NShimFunctions
    // SysInfo_B0CC
    // SysInfo_ShimIdToName
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    
    
    
    
    return;
}
#endif // CPROTO

void shimRedim(int nsets,int nshims)
{

  bool hasval=ParxRelsParHasValue("BasicShim");
  if(nshims != (int) PARX_get_dim("BasicShim",1))
    PARX_change_dims("BasicShim",nshims);
  if(!hasval)
    memset(BasicShim,0,nshims*sizeof(double));

  hasval=ParxRelsParHasValue("BasicShimAmp");
  if(nshims != (int) PARX_get_dim("BasicShimAmp",1))
    PARX_change_dims("BasicShimAmp",nshims);
  if(!hasval)
    memset(BasicShimAmp,0,nshims*sizeof(double));



  hasval=ParxRelsParHasValue("DynamicShims");
  if(nsets != (int) PARX_get_dim("DynamicShims",1) ||
     nshims!= (int) PARX_get_dim("DynamicShims",2))
    PARX_change_dims("DynamicShims",nsets,nshims);
  if(!hasval)
    memset(DynamicShims[0],0,nshims*nsets*sizeof(double));
  
}


void UpdateShimIncrements(const double targ_fdev,
                          const double radius,
                          const double max_inc,
                          bool autoInc,
                          bool sens_calib)
{
  PvShimSystem shimInt;

  if(targ_fdev < 0.0)
    throw PvStdException("UpdateShimIncrements Arg 1 (targ_fdev):"
                         " %f Hz < 0.0",targ_fdev);
  if(radius <= 0.0)
    throw PvStdException("UpdateShimIncrements Arg 2 (radius):"
                         " %f mm <= 0.0",radius);
  if(max_inc < 0.0)
    throw PvStdException("UpdateShimIncrements Arg 3 (max_inc):"
                         " %f < 0.0",max_inc);
  if(sens_calib==true)
    throw PvStdException("UpdateShimIncrements Arg 6 "
                         "(sens_calib) == true : not implemented");


  if(autoInc)
    ParxRelsMakeNonEditable("ScIncPerc,ScIncAmp,ScIncHz");
  else
    ParxRelsMakeEditable("ScIncPerc,ScIncAmp,ScIncHz");
      

  std::vector<double>cnm,currm_sps,currm_coil;

  int nshims = shimInt.nShims();
  currm_sps=shimInt.shimMaxCurrentSPS();
  currm_coil=shimInt.shimMaxCurrentCoil();


  bool hasval=ParxRelsParHasValue("ScIncPerc");
  if(nshims != (int) PARX_get_dim("ScIncPerc",1))
    PARX_change_dims("ScIncPerc",nshims);
  if(!hasval)
    memset(ScIncPerc,0,nshims*sizeof(double));

  hasval=ParxRelsParHasValue("ScIncHz");
  if(nshims != (int) PARX_get_dim("ScIncHz",1))
    PARX_change_dims("ScIncHz",nshims);
  if(!hasval)
    memset(ScIncHz,0,nshims*sizeof(double));

  hasval=ParxRelsParHasValue("ScIncAmp");
  if(nshims != (int) PARX_get_dim("ScIncAmp",1))
    PARX_change_dims("ScIncAmp",nshims);
  if(!hasval)
    memset(ScIncAmp,0,nshims*sizeof(double));


  unsigned int maxdeg;
  double fdev,dval;
    
  std::vector<double>fdev1A;
  std::vector<double>max_amp=shimInt.shimMaxCurrent();
  

  for(int coil=0;coil< nshims; coil++)
  {
    shimInt.solidHarmField(coil,cnm,maxdeg,NcMaxUnitSphere);
    shimInt.maxFieldDev(cnm,NcMaxUnitSphere,radius,fdev,true,false);
    fdev1A.push_back(fdev);
  }

  double *sv_perc=ScIncPerc;
  double *sv_Hz=ScIncHz;
  double *sv_amp=ScIncAmp;


  if(autoInc)
  {
    for(int coil=0;coil<nshims;coil++)
    {
      dval=fabs(fdev1A[coil]);
      if(dval > 1.0e-3)
      {
        dval=fabs(targ_fdev/dval);
      }
      else
        dval=0.0;

      if(dval > max_amp[coil]+1.0e-3)
        dval= max_amp[coil];

      dval = 100.0*dval/max_amp[coil];
      sv_perc[coil] = MIN_OF(max_inc,dval);
      sv_amp[coil]  = max_amp[coil] * sv_perc[coil]/100.0;
      sv_Hz[coil]   = sv_amp[coil]*fdev1A[coil];
    }
  }
  else // manually changed increment
  {
    int coil;
    std::string modpar=UT_RelContext_ParName();
    if("ScIncHz"==modpar)
    {
      coil=UT_RelContext_ParArrayIndices()[0];
      dval=fdev1A[coil];
      if(fabs(dval) > 1.0e-3)
        dval=sv_Hz[coil]/fabs(dval);
      else
        dval=0.0;

      if(fabs(dval) > (max_amp[coil]+1.0e-3))
        dval= (dval>0.0 ? max_amp[coil]:-max_amp[coil]);

      sv_perc[coil]= 100.0*dval/max_amp[coil];
    }
    else if("ScIncAmp" == modpar)
    {
      coil=UT_RelContext_ParArrayIndices()[0];
      dval=sv_amp[coil];
      if(fabs(dval) > (max_amp[coil]+1.0e-3))
        dval = (dval > 0.0 ? max_amp[coil]:-max_amp[coil]);
      
      sv_perc[coil] =  100.0*dval/max_amp[coil];

    }

    for(coil=0;coil < nshims; coil++)
    {
      dval=fabs(sv_perc[coil]);
      dval=MIN_OF(max_inc,dval);
      sv_perc[coil] = (sv_perc[coil] > 0.0 ? dval:-dval);
      sv_amp[coil]  = 0.01*sv_perc[coil]*max_amp[coil];
      sv_Hz[coil]   = sv_amp[coil]*fdev1A[coil];
    }
  }

}


void Test1_CheckSysinfoCallsRel(void)
{
  DB_MSG(("-->Test1_CheckSysinfoCallsRel"));

  testAndReportShimSysinfofunctions ();

  PvShimSystem shimInt;

  std::cout << shimInt.printShimInfo() << std::flush;

  DB_MSG(("<--Test1_CheckSysinfoCallsRel"));
  return;
}




void HandleTestShimServerAdj(void)
{
  DB_MSG(("-->HandleTestShimServerAdj"));
     
  PvShimSystem shimInt; 
  int nshims = shimInt.nShims();
  bool test_failed UNUSED_PARAM = true;
     
  bool tshim_init=ParxRelsParHasValue("Testshimsets") ? true:false;
  if((int)PARX_get_dim("Testshimsets",1) !=3 &&
     (int)PARX_get_dim("Testshimsets",2) !=nshims)
  {   
    PARX_change_dims("Testshimsets",3,nshims);
    tshim_init=false;
  }
     
  if(tshim_init==false)
  {
    for(int i=0;i<3;i++)
      memset(Testshimsets[i],0,nshims*sizeof(double));
    tshim_init=true;
  }
               
  if(No==PTB_AdjustmentStarting())
  {
    return;
  }
  std::vector<double> actshim;
  bool subpInitialized=false;
  subpInitialized = PTB_CheckSubProcess(0) == Yes ? false:true;
  std::string actsubp = subpInitialized ? PTB_GetAdjSubProcess() : "";
  AdjExecutionState adjstate = adj_failed;    
  std::stringstream msg;
  std::vector<std::string> sn=shimInt.shimCoilNames();


  msg << "Adjustment testshiminterface: ";
  if(subpInitialized)
    msg << "Subprocess " << actsubp << std::endl;
  else
    msg << "Initialization" << std::endl;
  
     
  static const ADJ_RESULT_PAR resultPars[] = \
    {{ scan_result, "Testshimsets" }};
  PvAdjManSetAdjParameterList(resultPars,
                              ARRAY_LEN(resultPars));
     
     
     
  ATB_GetActualShim(actshim);
  if(true==actshim.empty())
  {
    DB_MSG(("ATB_GetActualShim returns empty vector"));
    adjstate=adj_failed;
    ParxRelsParSetValue("AdjPerformState",&adjstate);
    ParxRelsParSetValue("AdjErrorComment",
                        "ATB_GetActualShim returns empty vector");
    throw PvStdException("ATB_GetActualShim returns empty vector");
  }
  else
  {
    msg << "Actual Shim" << std::endl
        << printout_shimvalues(sn,actshim.data());
    DB_MSG((msg.str().c_str()));
  }
     
  if(!subpInitialized)
  {
    std::vector<AdjSubAdjType> subadjlist;
    AdjSubAdjType csubadj;
    memset(&csubadj,0,sizeof(AdjSubAdjType));
    csubadj.methodContext[0]='\0';
    strcpy(csubadj.name,"SetShim");
    subadjlist.push_back(csubadj);
    strcpy(csubadj.name,"TestShim");
    subadjlist.push_back(csubadj);
    strcpy(csubadj.name,"CheckShim");
    subadjlist.push_back(csubadj);
    if(PTB_InitSubProcess(subadjlist)!=(int)subadjlist.size())
    {
      throw PvStdException("Failed to set up %d subadjustments!\n",
                           (int)subadjlist.size());
    }
    test_failed=false;
  }
  else if(!actsubp.find("SetShim"))
  {
    // copy actual shim to testshimlist
    memcpy(Testshimsets[0],actshim.data(),nshims*sizeof(double));
    double *testshim=Testshimsets[1];
    double dval=0.0;
    for(int i=0;i<nshims;i++)
    {
      dval= i + ((i+1)%10)/10.0 + ((i+2)%10)/100.0;
      testshim[i]=dval;
    }
    std::vector<double> s2unit;
    s2unit.assign(testshim,testshim+nshims);
    ATB_LoadShim(s2unit);
    msg << "Loaded shim 1" << std::endl
        << printout_shimvalues(sn,testshim,s2unit.data());

    // copy rounded values back to testshim
    std::copy(s2unit.begin(),s2unit.end(),testshim);

                                                                

    adjstate=adj_successful;
    test_failed=false;
  }
  else if(!actsubp.find("TestShim"))
  {
    if(memcmp(actshim.data(),Testshimsets[1],nshims*sizeof(double)))
    {
      msg << "Test of shim failed:" 
          << "Expected shim:" << std::endl
          << printout_shimvalues(sn,actshim.data(),Testshimsets[1]);
    }
    else
    {
      msg << "Test successful, load initial shim back to hardware." << std::endl;
      test_failed=false;
    }
    // load previous shim back
    std::vector<double> s2unit;
    double *testshim=Testshimsets[0];
    s2unit.assign(testshim,testshim+nshims);
    ATB_LoadShim(s2unit);
    adjstate=adj_successful;
  }
  else if(!actsubp.find("CheckShim"))
  {
    if(memcmp(actshim.data(),Testshimsets[0],nshims*sizeof(double)))
    {
      msg << "Test of shim failed:" 
          << "Expected shim:" 
          << printout_shimvalues(sn,actshim.data(),Testshimsets[0]);
    }
    else
    {
      msg << "Test successful, initial shim restored." << std::endl;
      test_failed=false;
      adjstate=adj_successful;
    }        
  }
    
     
  std::cout << msg.str();     
  AdjPerformState=adjstate;
     
    
     
  DB_MSG(("<--HandleTestShimServerAdj : test failed: %s",test_failed ? "true":"false"));
  return;
}

int UpdateDynShimTypePar(void)
{

    std::vector<std::pair < std::string, std::string >> evalset
    {
        {
            "Manual", "Manual Setup."
        },
        {
            "SCInc", "Serial increment."
        },
        {
            "SCDec", "Serial decrement."
        },
        {
            "SelCoil", "Selected Coil"
        },
        {
            "MLoad", "Max. Tot. Curr."
        },
    };

    PvOvlUtilInitDynEnum("DynShimExpTyp", evalset, "SelCoil", false);


    int actstate = 0;
    ParxRelsParGetEnumValueAsInt("DynShimExpTyp", &actstate);
    return actstate;
}
  

void UpdateTestDynShims(void)
{

  PvShimSystem shimInt; 
  int nshims = NBShims = shimInt.nShims();
  int nsets = 1;

  if(!ParxRelsParHasValue("NDynShims"))
    NDynShims=nsets;
  else
  {
    nsets=NDynShims;
    NDynShims=nsets=MAX_OF(1,nsets);
  }

  TestDynamicShimsRange();
  
  if(!ParxRelsParHasValue("DynShimAcqExperiment") || TestDynamicShims==No)
  {
    DynShimAcqExperiment=No;
  }

  if(DynShimAcqExperiment==Yes)
    ParxRelsHideInEditor("DynShimLoopsize");
  else
    ParxRelsShowInEditor("DynShimLoopsize");

  if(!ParxRelsParHasValue("DynShimLoopsize"))
    DynShimLoopsize=10000;

  if(!ParxRelsParHasValue("SelectShimCoilIncrements"))
  {
    PARX_change_dims("SelectShimCoilIncrements",nsets);
    memset(SelectShimCoilIncrements,0,nsets*sizeof(double));
  }
  // act state
  std::string dsExpTyp = 
    ParxRelsIntToEnumStr("DynShimExpTyp",UpdateDynShimTypePar());

  DB_MSG(("dsExpTyp: %s",dsExpTyp.c_str()));

  // supported states: 
  //     Manual   - manual setup of shim increments
  //     SCInc    - single coil increment
  //     SCDec    - single coil decrement           
  //     SelCoil  - increment selected coil
  //     MLoad    - switching groups of shims with max total current 





  
  
  const char *const dynShimPars = \
    { "DynShimAcqExperiment,DynShimExpTyp,NDynShims,"
      "DynamicShims,DynShimAcqExperiment,SelectShimCoil,DynShimLoopsize,"
      "ShimPlateau,ShimRamptime" };


  if(Yes==TestDynamicShims)
    ParxRelsShowInEditor(dynShimPars);
  else
    ParxRelsHideInEditor(dynShimPars);

  if(DynShimAcqExperiment==Yes)
    ParxRelsHideInEditor("ShimPlateau");

  if(!ParxRelsParHasValue("ShimIncAuto"))
    ShimIncAuto=Yes;
 
  if(!ParxRelsParHasValue("ShimRamptime"))
  {
    if(TestDynamicShims==Yes)
      ShimRamptime=shimInt.shimRiseTime();
    else
      ShimRamptime=0.001;
  }
  

  if(!ParxRelsParHasValue("ShimPlateau"))
    ShimPlateau=ShimRamptime;


  double targfdev=150;
  double radius=10;
  double max_inc=50;

  if(!ParxRelsParHasValue("MaxDsIncr"))
  {
    MaxDsIncr=max_inc;
  }
  else
  {
    max_inc=MaxDsIncr;
  }

  if(!ParxRelsParHasValue("FieldDev"))
    FieldDev=targfdev;
  else
    targfdev=FieldDev;


  if(!ParxRelsParHasValue("DefaultDiameter"))
  {
    DefaultDiameter=Yes;
  }


  if(!ParxRelsParHasValue("SphereDiameter") || DefaultDiameter==Yes)
  {
    double defext[3];
    CFG_DefaultExtent(defext);
    radius=defext[0]/4;
    SphereDiameter=2*radius;
  }
  else
    radius=SphereDiameter/2;





  UpdateShimIncrements(targfdev,
                       radius,
                       max_inc,
                       ShimIncAuto==Yes,
                       false);

  std::vector<std::string> coilId=shimInt.shimIdentifiers();

  ParxRelsParSetStringArrayValue("ShimIDs",coilId);  
  int shimchan_sel=UpdateShimCoilSelectPar(coilId,&Selected_ShimChannel);

  // handle dynamic shim experiment type

  ParxRelsHideInEditor("SelectShimCoilIncrements,SelectShimCoil");

  if(dsExpTyp=="SCInc" || dsExpTyp=="SCDec" )
  {
    ParxRelsMakeNonEditable("NDynShims,DynamicShims");
    nsets=NDynShims=nshims;
    shimRedim(nsets,nshims);
 
    // set increment for first dynamic shim (reference vector)
    memset(DynamicShims[0],0,nsets*nshims*sizeof(double));
    
    for(int coil=0;coil<nshims;coil++)
    {
      DynamicShims[coil][coil]=ScIncPerc[coil];
    }
    DB_MSG(("Handled mode %s",dsExpTyp.c_str()));
  }
  else if(dsExpTyp=="SelCoil")
  {
    ParxRelsMakeEditable("NDynShims");
    ParxRelsMakeNonEditable("DynamicShims");
    ParxRelsShowInEditor("SelectShimCoilIncrements,SelectShimCoil");
    nsets=NDynShims;
    shimRedim(nsets,nshims);
    PARX_change_dims("SelectShimCoilIncrements",nsets);

    double *dp0=DynamicShims[0];
    memset(dp0,0,nshims*nsets*sizeof(double));
    

    for(int set=0;set < nsets ;set++)
    {      
      DynamicShims[set][shimchan_sel]=SelectShimCoilIncrements[set];
    }
    DB_MSG(("Handled mode MStep"));
  }
  else if(dsExpTyp=="MLoad")
  {
    ParxRelsMakeNonEditable("NDynShims,DynamicShims");
    nsets=NDynShims=2;
    shimRedim(nsets,nshims);

    std::vector<int> totcurrconstr=shimInt.shimCoilCurrentConstrained();
    
    std::vector<double> maxsumcurrent=shimInt.shimMaxSumCurrentPerSPS();

    if(maxsumcurrent.size() < 1)
      maxsumcurrent.assign(shimInt.shimPowerSupplies()+1,0.0);
  
    std::vector<int> onsps(maxsumcurrent.size()+1,0);
    std::vector<double> meancurr(onsps.size()+1,0.0);
 


    std::vector<double> totcurr(shimInt.shimPowerSupplies()+1,0.0);
    std::vector<double> maxcurr,shimcurr(nshims,0.0),shimvec;
    maxcurr=shimInt.shimMaxCurrent();

    for(int coil=0;coil<nshims;coil++)
      onsps[totcurrconstr[coil]]++;
    
    int nsps=shimInt.shimPowerSupplies();

    for(int sps=1;sps <= nsps; sps++)
    {
      meancurr[sps] = maxsumcurrent[sps-1];
      if(onsps[sps] > 0)
      {
        meancurr[sps] /= onsps[sps];
      }
    }
  
    for(int coil=0;coil<nshims;coil++)
    {
      // set only shims on real SPS 
      if(totcurrconstr[coil] > 0)
      {
        shimcurr[coil]=meancurr[totcurrconstr[coil]];
        shimcurr[coil]=MIN_OF(shimcurr[coil],maxcurr[coil]);
      }
      totcurr[totcurrconstr[coil]] += fabs(shimcurr[coil]);
    }
    bool valid=shimInt.shimCurrentToPerc(shimcurr,shimvec);
    if(!valid)
    {
        std::string emsg;
        shimInt.shimVectorValid (shimvec,0,0,0,&emsg);
        throw PvStdException("Invalid shim vector:\n%s",emsg.c_str());
    }
#if DEBUG
    {
      std::ostringstream ostr;


      for(int coil=0;coil < nshims;coil++)
        ostr << "Coil " << std::setw(10) << shimInt.shimIdentifiers()[coil]
             << "+- " << std::setw(10) << shimcurr[coil] 
             << " <=> " 
             << std::setw(10) << shimvec[coil] << " %" << std::endl;

      for(int sps=0;sps <= nsps; sps++)
        ostr << "Sum current SPS[" << sps << "]: " << totcurr[sps] << std::endl;

      DB_MSG((ostr.str().c_str()));
    }
#endif

    DB_MSG(("Experiment type %s valid shim increment: %s",
            dsExpTyp.c_str(), valid==true ? "true":"false"));

    double *dp0=DynamicShims[0];
    double *dp1=DynamicShims[1];

    // set max load vector
    for(int coil=0;coil<nshims;coil++)
    {      
      dp0[coil]=shimvec[coil];
      dp1[coil]=-dp0[coil];
    }


    DB_MSG(("Handled mode MLoad"));
  }
  else
  {
    ParxRelsMakeEditable("NDynShims,DynamicShims");
    nsets=NDynShims;
    shimRedim(nsets,nshims);
  }

  int nsps=shimInt.shimPowerSupplies()+1;
  NShimPS=nsps;
  PARX_change_dims("DynShimSumCurr",nsets,nsps);
  PARX_change_dims("DynShimPower",nsets,nsps);
  std::vector<double> baseshim(BasicShim,BasicShim+nshims);
  std::vector<double> actset(nshims,0.0);
  std::vector<double> set_power,set_sumcurrent;
  bool valid UNUSED_PARAM=false;

  double *dynamicShimPointer,dval,totpow=0.0;
  std::string msg;

  for(int set=0;set < nsets;set++)
  {
    dynamicShimPointer=DynamicShims[set];
    for(int coil=0;coil<nshims;coil++)
    {   
      dval=dynamicShimPointer[coil];
      dynamicShimPointer[coil]=MAX_OF(MIN_OF(200.0,dval),-200.0);
    }
    actset.assign(dynamicShimPointer,
                  dynamicShimPointer+nshims);
    valid= shimInt.shimIncrementValid(actset,
                                     &baseshim,
                                     &set_sumcurrent,
                                     &set_power,
                                     &totpow,
                                     &msg);
    DB_MSG(("Set[%d]: valid = %s\n%s",
            set,valid==true ? "true":"false",msg.c_str()));
    memcpy(DynShimSumCurr[set],set_sumcurrent.data(),
           nsps*sizeof(double));
    memcpy(DynShimPower[set],set_power.data(),
           nsps*sizeof(double));
  }

}


void TestDynamicShimsRel(void)
{
  TestDynamicShimsRange();
  backbone();
}
void TestDynamicShimsRange(void)
{
  if(!ParxRelsParHasValue("TestDynamicShims"))
    TestDynamicShims=Yes;

  if(!SysInfo_DynamicShimming())
      TestDynamicShims=No;
}
