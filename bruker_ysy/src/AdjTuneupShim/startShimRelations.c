/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2010
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2001-2004 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR 0


#include "method.h"
#include "lib/PvSysinfoClient.h"
#ifndef CPROTO
#include<vector>
#endif

void StartShimNameRange(void)
{
    if(!ParxRelsParHasValue("StartShimName"))
    {
        if(ParxRelsParHasValue("PVM_StartupShimList"))
        {
            int ival=0;
            StartShimName[63]='\0';
            ParxRelsParGetEnumValueAsInt("PVM_StartupShimList",&ival);
            if((int)PARX_get_dim("PVM_StartupShimNames",1)>ival && ival>=0)
                strncpy(StartShimName,
                        PVM_StartupShimNames[ival],
                        63);    
        }
    }
    else
    {
        strncpy(StartShimName,"Shim1",63);
    }
}

void DeleteTuneupShimRel(void)
{
  DB_MSG(("-->DeleteTuneupShimRel"));

  try
  {
    ParxRelsResetPar("PVM_TuneupShim");
    /*
    int nshims=SysInfo_NShims();
    if(nshims < 1)
      throw PvStdException("SysInfo_NShims returns %d",nshims);
    PARX_change_dims("PVM_TuneupShim",nshims);
    memset(PVM_TuneupShim,0,nshims*sizeof(double));
    */
    PVM_TuneupShimAvailable=No;
  }
  catch (const PvException &ex )
  {
    throw PvStdException("DeleteTuneupShim failed:\n%s",ex.what());
  }

  DB_MSG(("<--DeleteTuneupShimRel"));
}




void DeleteStartShimRel(void)
{
    DB_MSG(("-->DeleteStartupShimRel"));
    int sidx=0;
    
    int nshims=SysInfo_NShims();
    int nstartshims=STB_CheckStartupShims(nshims);
    if(nstartshims < 1)
      return;
    ParxRelsParGetEnumValueAsInt("PVM_StartupShimList",&sidx);
    
    if(nstartshims > 1)
      for(int i=sidx; i<nstartshims;i++)
      {
        if(i!=nstartshims-1)
        {
          memcpy(PVM_StartupShims[i],
                 PVM_StartupShims[i+1],
                 nshims*sizeof(int));
         strncpy(PVM_StartupShimNames[i],PVM_StartupShimNames[i+1],64);
        }
      }
    nstartshims--;
    
    if(nstartshims>0)
    {
        ParxRelsShowInEditor("PVM_StartupShimList");
        PARX_change_dims("PVM_StartupShimNames",nstartshims,64);
        PARX_change_dims("PVM_StartupShims",nstartshims,nshims);
        ParxRelsResetPar("PVM_StartupShimList");
        std::vector<std::pair<std::string, std::string>> ssnames;
       
        for(int i=0;i<nstartshims;i++)
        {
            ssnames.push_back(std::make_pair("S" + std::to_string(i), 
                std::string(PVM_StartupShimNames[i]) + "."));
        }
        ParxRelsParBindEnum("PVM_StartupShimList",ssnames);
        sidx=MAX_OF(MIN_OF(nstartshims-1,sidx),0);
        ParxRelsParSetEnumValueFromInt("PVM_StartupShimList",sidx);
        PVM_StartupShimAvailable=Yes;
    }
    else
    {
        ParxRelsResetPar("PVM_StartupShimNames");
        ParxRelsResetPar("PVM_StartupShims");           
        ParxRelsResetPar("PVM_StartupShimList");
        ParxRelsHideInEditor("PVM_StartupShimList");
        strncpy(StartShimName,"Shim1",64);
        PVM_StartupShimAvailable=No;
    }   
    
    DB_MSG(("<--DeleteStartupShimRel"));
}


void AddStartShimRel(void)
{
  DB_MSG(("-->AddStartShimRel"));
  try
  {
    int nshimsets;
    int suppShims=SysInfo_NShims();
    if(suppShims < 1)
      throw PvStdException("SysInfo_NShims returns %d",suppShims);

    if(!ParxRelsParHasValue("PVM_StartupShimList"))
    {
      nshimsets=1;
      PARX_change_dims("PVM_StartupShims",nshimsets,suppShims);
      PARX_change_dims("PVM_StartupShimNames",nshimsets,64);
      std::vector<std::pair<std::string, std::string>> stshn;
      stshn.push_back(std::make_pair("S"+std::to_string(nshimsets-1),
          std::string(StartShimName)+"."));
      
      ParxRelsParBindEnum("PVM_StartupShimList",stshn);

      ParxRelsParSetEnumValueFromInt("PVM_StartupShimList",0);
      strncpy(PVM_StartupShimNames[0],StartShimName,64);
      memcpy(PVM_StartupShims[0],
             &LoadedShimset,
             suppShims*sizeof(double));
    }// end if PVM_StartupShimList has no value
    else // modify PVM_StartupShimList and startup shims / -names
    {
      int actdim=0;

      if(!ParxRelsParHasValue("PVM_StartupShims"))
      {
        throw PvStdException("PVM_StartupShims: no value\n");
      }
      nshimsets=(int)PARX_get_dim("PVM_StartupShims",1);
      if(!ParxRelsParHasValue("PVM_StartupShimNames"))
      {
        throw PvStdException("PVM_StartupShimNames: no value\n");
      }
      actdim=(int)PARX_get_dim("PVM_StartupShimNames",1);
      if(actdim != nshimsets)
      {
        throw PvStdException("PVM_StartupShimNames: dim %d\n"
                             "PVM_StartupShims    : dim %d differs",
                             actdim,nshimsets);
      }
      std::vector<std::pair<std::string,std::string>> stshn;
      bool found=false;
      const std::string nshn=StartShimName;
      int shidx=0,i=0;  
      for(shidx=0,i=0;i<nshimsets;i++)
      {
          stshn.push_back(std::make_pair("S"+std::to_string(i),
              std::string(PVM_StartupShimNames[i])+"."));
        
        if(nshn == PVM_StartupShimNames[i])
        {
            found=true;
            shidx=i;
            ParxRelsParSetEnumValueFromInt("PVM_StartupShimList",shidx);
        }
      }
      
      if(found==false)
      {
        
        stshn.push_back(std::make_pair("S"+std::to_string(nshimsets), 
            nshn+"."));
        nshimsets++;
        ParxRelsParBindEnum("PVM_StartupShimList",stshn);

        PARX_change_dims("PVM_StartupShims",nshimsets,suppShims);
        PARX_change_dims("PVM_StartupShimNames",nshimsets,64);
        strncpy(PVM_StartupShimNames[nshimsets-1],nshn.c_str(),64);
        memcpy(PVM_StartupShims[nshimsets-1],
               LoadedShimset,suppShims*sizeof(double));
        ParxRelsParSetEnumValueFromInt("PVM_StartupShimList",nshimsets-1);
      }
      else
      {
        memcpy(PVM_StartupShims[shidx],
               LoadedShimset,suppShims*sizeof(double));
      }
    }// modify PVM_StartupShimList and startup shims / -names
  }
  catch (const PvException &ex )
  {
    throw PvStdException("AddStartShim failed:\n%s",ex.what());
  }

  if(ParxRelsParHasValue("PVM_StartupShimList"))
  {
    ParxRelsShowInEditor("PVM_StartupShimList");
  }
  else
  {
    ParxRelsHideInEditor("PVM_StartupShimList");
  }


  DB_MSG(("<--AddStartShimRel"));
}


void StoreShimRel(void)
{
  switch(StoreShim)
  {
    default:
    case StoreTuneupShim:
      ParxRelsHideInEditor("StartShimName,AddStartupShim,"
                           "DeleteStartupShim,PVM_StartupShimList");
      ParxRelsShowInEditor("DeleteTuneupShim");
      PVM_StartupShimCond=TuneupShim;
      break;
    case StoreStartupShim:
      ParxRelsShowInEditor("StartShimName,AddStartupShim,"
                           "DeleteStartupShim,PVM_StartupShimList");
      ParxRelsHideInEditor("DeleteTuneupShim");
      
      if(!ParxRelsParHasValue("PVM_StartupShimList"))
      {
        ParxRelsHideInEditor("PVM_StartupShimList");
        PVM_StartupShimCond=TuneupShim;
      }
      else
        PVM_StartupShimCond=StartupShim;
      break;
  }
  ParxRelsShowInEditor("StoreShim");
}
void HandleBasicShimAdj(void)
{
    static const ADJ_RESULT_PAR loadShimResultPars[] =
    {
        { scan_result, "PVM_TuneupShimAvailable" },
        { scan_result, "PVM_StartupShimAvailable" }
    };
    
    PvAdjManSetAdjParameterList(loadShimResultPars,
                                ARRAY_LEN(loadShimResultPars));
    ParxRelsShowInEditor("PVM_StartupShimAvailable,PVM_TuneupShimAvailable,PVM_StartupShimCond");
    STB_UpdateStartupShims();
        
    ParxRelsHideInEditor("StoreShim,DeleteTuneupShim,StartShimName,AddStartupShim,DeleteStartupShim");
    
    

    if(PTB_AdjustmentStarting() ==Yes)
    {
        bool tuneupshim=true;
        if(ParxRelsParHasValue("PVM_StartupShimCond"))
        {
            bool doadj=true;
            switch(PVM_StartupShimCond)
            {
                default:
                case TuneupShim:
                    break;
                case StartupShim:
                    tuneupshim=false;
                    break;
                case CurrentShim:
                    doadj=false;
                    break;
            }
            bool hasTuneupShim=false,hasStartupShim=false;
            if(!ParxRelsParHasValue("PVM_TuneupShimAvailable"))
                PVM_TuneupShimAvailable=No;
            else
                hasTuneupShim = PVM_TuneupShimAvailable==Yes ? true:false;
            if(!ParxRelsParHasValue("PVM_StartupShimAvailable"))
                PVM_StartupShimAvailable=No;    
            else
                hasStartupShim = PVM_StartupShimAvailable==Yes ? true:false;
                
            if(doadj==false ||
               (false==hasTuneupShim  && false==hasStartupShim) ||
               (false==hasTuneupShim  && tuneupshim==true) ||
               (false==hasStartupShim && tuneupshim==false))     
            {
               // nothing to do or possible
               AdjPerformState=adj_successful;
               return;
            }
            std::vector<double> shim;
            double  *ss=0,*se=0;
            int sidx=0;
            
            try
            {
              if(tuneupshim==true)
              {
                if(ParxRelsParHasValue("PVM_TuneupShim"))
                {
                    ss=PVM_TuneupShim;
                    se=ss+PARX_get_dim("PVM_TuneupShim",1);
                }
                else
                {
                    doadj=false;
                }
              }
              else // handle startup shim
              {
                 if(!(ParxRelsParHasValue("PVM_StartupShimList") &&
                      ParxRelsParHasValue("PVM_StartupShims")))
                 {
                    doadj=false;
                 }
                 else
                 {
                    
                    ParxRelsParGetEnumValueAsInt("PVM_StartupShimList",&sidx);
                    int nstartupshims=(int)PARX_get_dim("PVM_StartupShims",1);
                    if(sidx >= nstartupshims || sidx < 0)
                    {
                        doadj=false;
                    }
                    else
                    {
                        unsigned int nshims=PARX_get_dim("PVM_StartupShims",2);
                        ss=PVM_StartupShims[sidx];
                        se=ss+nshims;
                    }
                 }
               }// handle startup or tuneup shim
               if(doadj==true)
               {
                 shim.assign(ss,se);
                 ATB_LoadShim(shim);
               }
            }
            catch(const PvException &ex)
            {
                std::ostringstream oerr;
                oerr << "Failed to set ";
                if(tuneupshim)
                  oerr << "Tuneup Shim ";
                else
                  oerr << "Startup shim " << sidx << ".";
                oerr << " Reason:" << std::endl << ex.what();
                throw PvStdException(oerr.str().c_str());
            }
        }// end startup shim cond has value
        AdjPerformState=adj_successful;
        return; 
    }// end adj starting
}


void HandleStartShimAdj(int suppShims)
{
   
    
  if(!ParxRelsParHasValue("StoreShim"))
  {
    StoreShim=StoreTuneupShim;
  }
  ParxRelsHideInEditor("PVM_StartupShimAvailable,PVM_TuneupShimAvailable,PVM_StartupShimCond");
  ParxRelsShowInEditor("StoreShim,DeleteTuneupShim,StartShimName,AddStartupShim,DeleteStartupShim");
  StoreShimRel();
  
  static const ADJ_RESULT_PAR storeShimResultPars[] =
    {
      { study_result, "PVM_TuneupShim" },
      { study_result, "PVM_TuneupShimAvailable" },
      { study_result, "PVM_StartupShimAvailable" },
      { study_result, "PVM_StartupShims" },
      { study_result, "PVM_StartupShimNames" }
    };
  

  PvAdjManSetAdjParameterList(storeShimResultPars,
                              ARRAY_LEN(storeShimResultPars));

  AdjEnableGlobalResultSave=Yes;
  AdjEnableUserResultSave=No;
  MRT_AdjSetHwScope(SHIM_CHAIN,0);

  if(suppShims < 1)
    throw PvStdException("SysInfo_NShims returns %d!",suppShims);

  if(StoreShim==StoreStartupShim)
  {
    if(ParxRelsParHasValue("PVM_StartupShimList"))
    {
      int shidx;
      ParxRelsParGetEnumValueAsInt("PVM_StartupShimList",&shidx);
      if(shidx >= 0 && shidx < (int) PARX_get_dim("PVM_StartupShims",1))
      {
        memcpy(PVM_StartupShims[shidx],
               &LoadedShimset,
               suppShims*sizeof(double));
      }
    }
    else
    {
      PVM_StartupShimAvailable=No;
    }
  }
  else if(StoreShim==StoreTuneupShim)
  {
    PARX_change_dims("PVM_TuneupShim",suppShims);
    if(ParxRelsParHasValue("LoadedShimset"))
    {
      memcpy(PVM_TuneupShim,
             &LoadedShimset,
             suppShims*sizeof(double));
  
    }
    else
    {
      memset(PVM_TuneupShim,0,suppShims*sizeof(int));
      PVM_TuneupShimAvailable=No;
    }
  }
  
  PVM_StartupShimAvailable=
    ParxRelsParHasValue("PVM_StartupShimList") ? Yes:No;
  
  PVM_TuneupShimAvailable=
    ParxRelsParHasValue("PVM_TuneupShim") ? Yes:No;

  AdjPerformState=adj_successful;

}
