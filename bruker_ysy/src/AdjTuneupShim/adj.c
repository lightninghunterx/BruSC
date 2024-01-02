/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2017
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2017 Bruker BioSpin MRI GmbH";

#define DEBUG		   0
#define DB_MODULE	 0
#define DB_LINE_NR 0

#include "method.h"
#include "lib/PvSysinfoClient.h"
#include "common/ovl_toolbox/ServiceModeSysInfo.h"

#ifndef CPROTO
#include <vector>
#include <string>
#include <iomanip>
static void AcqAdjHandler(std::string &adjn);
#include "localUtilities.h"
#endif


void HideStartupShimPars(void)
{
          const char *const parnames = \
           "PVM_TuneupShimAvailable,"
            "PVM_StartupShimAvailable,"
            "PVM_StartupShimCond,"
            "PVM_StartupShimList,"
            "StoreShim,"
            "DeleteTuneupShim,"
            "StartShimName,"
            "AddStartupShim,"
 	    "DeleteStartupShim";
          ParxRelsHideInEditor(parnames);
}

void ShowStartupShimPars(void)
{
          const char *const parnames = \
          "PVM_TuneupShimAvailable,"
            "PVM_StartupShimAvailable,"
            "PVM_StartupShimCond,"
            "PVM_StartupShimList,"
            "StoreShim,"
            "DeleteTuneupShim,"
            "StartShimName,"
            "AddStartupShim,"
            "DeleteStartupShim";
          ParxRelsShowInEditor(parnames);
}


void HandleAdjustmentRequests(void)
{
  DB_MSG(("-->HandleAdjustmentRequests"));

  //  bool handleTestshim=false;
  // bool handleBasicshim=false;

  ParxRelsHideInEditor("NFID");
  
  std::string adjn=PTB_GetAdjName();
  if(Yes == PTB_AdjMethSpec())
  {
    if(!adjn.find("startshim"))
    {
      int suppShims = RetrieveCurrentShim();
      // handle startshim adjustment
      ShowStartupShimPars();
      HandleStartShimAdj(suppShims);
    }
    else if(!adjn.find("testshiminterface"))
    {
      HideStartupShimPars();
      HandleTestShimServerAdj();
    }
    else if(!adjn.find("SHIM_Initial"))
    {
      // load startup shim to hardware
      ShowStartupShimPars();
      HandleBasicShimAdj();
    }
    else if(!adjn.find("Test-Shim-Effect"))
    {
       HideStartupShimPars();
       ParxRelsShowInEditor ("NFID");
       HandleTestShimEffectAdj ();
    }
    else if(!adjn.find("Test-Shim-Current-On-Channel"))
    {
      HideStartupShimPars();
      strcpy (ACQ_SetupAutoName, "TestShimCurrentAutocounter");
      // adjustment running in endless loop: no parameter adjusted
      static const ADJ_RESULT_PAR resultPars[] = \
      {
        { scan_result, "TestShimChannel"}
      };
      PvAdjManSetAdjParameterList (resultPars,
                                   ARRAY_LEN (resultPars));            
    }
    else
    {
      HideStartupShimPars();
      AcqAdjHandler(adjn);
    }
  }
  else if(!adjn.find("SHIM_Initial"))
  {
    // load startup shim to hardware
    ShowStartupShimPars();
    HandleBasicShimAdj();
  }
  else
  {
    HideStartupShimPars();
    AcqAdjHandler(adjn);
  }
  DB_MSG(("<--HandleAdjustmentRequests"));
}


void AdjResultHandler()
{
  ParxRelsParRelations("PVM_AdjResultHandler",MODALITY_HIERARCHY);
  backbone();
}

#ifndef CPROTO
static void AcqAdjHandler(std::string &adjn)
{
  DB_MSG(("-->AcqAdjHandler"));

  if (Yes==STB_AtsAdj()) {
      return;
  }

  /* array phase adjustment */
  if (STB_ArrayPhaseHandleAdjustmentRequests() == 1)
  {
    PVM_NAverages = 1;
    ParxRelsParRelations("PVM_NAverages",No);
    backbone();
    return;
  }


  YesNo handled=No;


  /* non-method-specific adjustment */
  if (PTB_AdjMethSpec() == No || (handled=STB_AtsAdj())==No)
  {
    ParxRelsParRelations("PVM_AdjHandler", MODALITY_HIERARCHY);
    return;
  }

  if(Yes==handled)
    return;


  /* receiver gain adjustment */
  if (adjn.find( RG_ADJNAME))
  {
    PVM_NAverages = 1;
    backbone();
    ParxRelsParRelations("PVM_AdjHandler", Yes);
  }

  DB_MSG(("<--AcqAdjHandler"));
}
#endif

void SetAdjustments(void )
{
  DB_MSG(("-->SetAdjustments"));

  PTB_ClearAdjustments();
  STB_ArrayPhaseAppendAdjustment(PVM_EncNReceivers, No);
  PTB_AppendAdjustment("SHIM_Initial",
                       "Setting Startup Shim",
                       "Initializes a proper shim condition",
                       on_demand);
  PTB_AppendAdjustment("startshim",
                       "Store Startup / Tuneup Shim",
                       "Reads in current shim and stores"
                       " it as Startup / tuneup Shim",
                       on_demand);
  if(SysInfo_ServiceModeActive())
  {
    PTB_AppendAdjustment("testshiminterface",
                         "Test Shim Server Communication",
                         "Loads shims from or to hardware",
                         on_demand);
    PTB_AppendAdjustment("Test-Shim-Effect",
                         "Test Effect of Shims on FID",
                         "Test Effect of Shims on FID.",
                         on_demand);
    PTB_AppendAdjustment("Test-Shim-Current-On-Channel",
                         "Test Current on specific Coil",
                         "In an endless loop a specific shim current is switched on and off. Adjustment may modify current shim condition!",
                         on_demand);
  }
    
  PTB_AppendOrderAdjustment(per_scan, per_scan, RG_ADJNAME);

  DB_MSG(("<--SetAdjustments"));

}




int RetrieveCurrentShim(void)
{
  int nshims=0;  
  try
  {
    std::vector<double> sv_curr;
    nshims=SysInfo_NShims();
    if(nshims < 1)
      throw PvStdException("SysInfo_NShims returns %d",nshims);

    if(!ParxRelsParHasValue("LoadedShimset") ||
       (int) PARX_get_dim("LoadedShimset",1) != nshims)
    {
      PARX_change_dims("LoadedShimset",nshims);
      memset(LoadedShimset,0,nshims*sizeof(double));
    }

    if(PTB_AdjustmentStarting() ==Yes)
    {

      ATB_GetActualShim(sv_curr);
      if(false==sv_curr.empty())
      {
#if DEBUG
        PvShimSystem shimInt;
        std::vector<std::string> sn=shimInt.shimCoilNames();

        std::ostringstream msg;
        std::vector<std::string>::iterator sit;
        std::vector<double>::iterator svit;
        for(sit=sn.begin(),svit=sv_curr.begin();
            sit != sn.end() && svit!= sv_curr.end();
            sit++,svit++)
        {
          msg << "Shim[" << std::setw(2) << (svit-sv_curr.begin()) << "]: "
              << std::setw(10) << *sit 
              << " " << std::setw(10) << *svit << std::endl; 
        }
        DB_MSG((msg.str().c_str()));
#endif
        if(nshims != static_cast<int>(sv_curr.size()))
          throw PvStdException("Illegal size of shim vector %u SysInfo_NShims"
                               " returns %d",
                               static_cast<unsigned int>(sv_curr.size()),
                               nshims);
        memcpy(LoadedShimset,sv_curr.data(),nshims*sizeof(double));
      }
      else // empty shim vector
      { 
        if(No==CFG_IsDatastation())
        {
          throw PvStdException("No information about current shim state!");
        }
        memset(LoadedShimset,0,nshims*sizeof(double));
      }
    }
  }
  catch (const PvException &ex)
  {
    throw PvStdException("RetrieveCurrentShim: %s",ex.what());
  }
  return nshims;
}

void HandleReports(void)
{
    DB_MSG(("-->HandleReports"));

    bool reportHandled = false;
    
    if (ParxRelsParHasValue("PVM_ReportHandler")) {
        std::string reportTemplate(PVM_ReportHandler);
        if (!reportTemplate.empty()) {
            if (!reportTemplate.find("ShimStatus.xml")) {
                HandleShimStatusReport();
                reportHandled=true;
            }
        }
    }
    if(!reportHandled)
        ParxRelsParRelations("PVM_ReportHandler",MODALITY_HIERARCHY);
    
    
    DB_MSG(("<--HandleReports"));
    return;
    
}


void HandleShimStatusReport(void)
{
    DB_MSG(("-->HandleShimStatusReport"));
    std::string parfile = pathToParentDset (true, 0);
    parfile += (std::string ("/") + "ShimFidEffectPars");
    ParxRelsReadParList ({ "TestShimEffectPars" }, parfile.c_str (), false);

    DB_MSG(("<--HandleShimStatusReport"));
    return;
}