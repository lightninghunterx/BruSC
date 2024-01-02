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

#define DEBUG      1
#define DB_MODULE  0
#define DB_LINE_NR 0

#include "method.h"
#include "localUtilities.h"
#ifndef CPROTO

static void InitShimEffectAdj (void);

static void InitShimEffectAdj (void)
{
    PvShimSystem shimInt;
    int nshims = shimInt.nShims ();

    std::vector<double> shimvec_perc, shimvec_amp, shimvec_maxperc;
    shimvec_amp = shimInt.shimMaxCurrentSPS ();
    shimvec_maxperc = shimInt.shimMaxPerc ();

    if (No == UT_ArrParsHaveSameDimension ("ScIncPerc,ScIncAmp,ScIncHz") ||
        nshims != (int) PARX_get_dim ("ScIncPerc", 1))
    {
        throw PvStdException ("Illegal array sizes of ScIncPerc,ScIncAmp,ScIncHz");
    }

    ATB_GetActualShim (shimvec_perc);

    if (nshims != (int) PARX_get_dim ("BasicShim", 1))
        PARX_change_dims ("BasicShim", nshims);
    if (nshims != (int) PARX_get_dim ("BasicShimAmp", 1))
        PARX_change_dims ("BasicShimAmp", nshims);
    
    PARX_change_dims ("Testshimsets", nshims, nshims);




    std::copy (shimvec_perc.begin (), shimvec_perc.end (), BasicShim);
    double *basic_shim = BasicShimAmp;
    for(int i=0;i<nshims;i++)
        basic_shim[i] = shimvec_amp[i] * shimvec_perc[i] / 100.0;
    
    
    basic_shim=BasicShim;
    double *scinc = ScIncPerc;
    std::string err_msg;
    for (int sc = 0; sc < nshims; sc++)
    {
        std::copy (basic_shim, basic_shim + nshims, shimvec_perc.begin ());
        if (fabs (shimvec_perc[sc] + scinc[sc]) > shimvec_maxperc[sc])
            shimvec_perc[sc] -= scinc[sc];
        else
            shimvec_perc[sc] += scinc[sc];

        if (fabs (shimvec_perc[sc]) > shimvec_maxperc[sc])
        {
            shimvec_perc[sc] = shimvec_perc[sc] > 0.0 ?
                    shimvec_maxperc[sc] : -shimvec_maxperc[sc];
        }
        if (false == shimInt.shimVectorValid (shimvec_perc, 0, 0, 0, &err_msg))
            throw PvStdException ("Illegal shim vector for test shim %d (ID: %s):\n%s",
                                  sc + 1, ShimIDs[sc], err_msg.c_str ());
        std::copy (shimvec_perc.begin (),
                   shimvec_perc.end (), Testshimsets[sc]);
    }

    int nfid = NFIDRange ();
    
    
    PARX_change_dims ("FIDArea",nshims+1,nfid);
    PARX_change_dims ("FIDFoffset",nshims+1,nfid);
    PARX_change_dims ("FIDPeakwidth",nshims+1,nfid);
    memset(FIDArea[0],0,(nshims+1)*nfid*sizeof(double));
    memset(FIDFoffset[0],0,(nshims+1)*nfid*sizeof(double));
    memset(FIDPeakwidth[0],0,(nshims+1)*nfid*sizeof(double));
    
    
    
    memset (&BaseFIDResult, 0, sizeof (FID_SHIM_RESULT));
    PARX_change_dims ("ShimFIDResults", nshims);
    NShimFID = nshims;
    ActbaseFID = ActShim = ActSteadyStateCount=0;
    NFIDSteadyStateRange();
    PARX_change_dims ("TestSucceeded", nshims);
    YesNo *succeeded = TestSucceeded;
        for (int i = 0; i < nshims; i++)succeeded[i] = No;
    
}

#endif //CPROTO




void
TestShimEffectAutoCounterRel (void)
{
    DB_MSG (("-->TestShimEffectAutoCounterRel"));
    int acval = 0;
    if (!ParxRelsParHasValue ("TestShimEffectAutoCounter"))
        acval = 0;
    else
        acval = TestShimEffectAutoCounter;


    if (acval == 0)
    {
        DB_MSG (("Initialize test shim effect adjustment..."));
        InitShimEffectAdj();
        acval=1;
    }
    else
    {

        if(ActSteadyStateCount < NFIDSteadyState)
        {
            ActSteadyStateCount++;
            return;
        }
        


        int actfid=ActbaseFID;
        int actshim=ActShim;
        int nfid=NFID;
        int nshim=NShimFID;
        actfid = MAX_OF(actfid, 0);
        actshim=MAX_OF(actshim,0);

        
        
        int arrsiz=PARX_get_dim("Narea",2);
        double *narea=Narea[0];
        bool base_fid_finished=false;
        bool shim_fid_finished=false;
        double *foffs=0;
        double *farea=0;
        double *fpeakw=0;
        
        if(actfid < nfid)
        {
            foffs=FIDFoffset[actshim];
            foffs[actfid]=WfrqOffs[0];
            farea=FIDArea[actshim];
            farea[actfid] =
                    (arrsiz == 1 ? narea[0] : UT_MeanOfDoubleArray (narea, arrsiz));
            fpeakw=FIDPeakwidth[actshim];
            fpeakw[actfid]=Wpwidth[0][0];
            actfid++;
            ActbaseFID=actfid%nfid;
        }
        
        FID_SHIM_RESULT *fid_shim_result=0;
        if(actfid==nfid)
        {
            if(actshim == 0)
               fid_shim_result=&BaseFIDResult;
            else
               fid_shim_result=&ShimFIDResults[actshim-1];
            
            fid_shim_result->area=
                    UT_MeanOfDoubleArray(farea,nfid,0,&fid_shim_result->area_sd);
            fid_shim_result->foffset=
                    UT_MeanOfDoubleArray(foffs,nfid,0,&fid_shim_result->foffset_sd);
            fid_shim_result->peakwidth =
                    UT_MeanOfDoubleArray (fpeakw, nfid, 0, &fid_shim_result->peakwidth_sd);

            base_fid_finished = true;
            if(actshim < nshim)
            {
                double *svpointer = Testshimsets[actshim];
                std::vector<double> svec;
                svec.assign (svpointer, svpointer + nshim);
                ATB_LoadShim (svec);
            }
            actshim++;
            // settle scans after changing shim
            ActSteadyStateCount=0;
        }
        
        ActShim=actshim;

        if (actshim == nshim+1)
        {
            double mean_area;
            fid_shim_result=&BaseFIDResult;
            mean_area = fid_shim_result->area;
            fid_shim_result->area=1.0;
            fid_shim_result->area_sd /= fabs(mean_area);
            fid_shim_result=ShimFIDResults;
            for(int i=0;i<nshim;i++)
            {
                fid_shim_result[i].area /= mean_area;
                fid_shim_result[i].area_sd /= fabs(mean_area);
                
            }
            ATB_LoadShim("BasicShim");
            shim_fid_finished=true;
        }
        
        if(shim_fid_finished==true && base_fid_finished == true)
        {
            // finish adjustment
            FID_SHIM_RESULT *fid_base_result=&BaseFIDResult;
            fid_shim_result=ShimFIDResults;
            YesNo *succeeded=TestSucceeded;
            std::vector<std::string> failure_reasons;
            std::ostringstream oerr;
            
            ShimFIDTestSuccess=Yes;
            
            for(int i=0;i<nshim;i++) 
            {
                bool significant_value[3]={true,true,true};
                succeeded[i]=Yes;
                
                if( fabs(fid_shim_result[i].area - fid_base_result->area) < 
                   10*(fid_base_result->area_sd+fid_shim_result[i].area_sd))
                {
                    oerr.str("");
                    oerr << "Shim " << i+1 << ": no significant change of normalized area";
                    failure_reasons.push_back (oerr.str());
                    significant_value[0]=false;
                }
                if( fabs(fid_shim_result[i].foffset - fid_base_result->foffset) <
                   10 * (fid_base_result->foffset_sd+fid_shim_result[i].foffset_sd))
                {
                    oerr.str("");
                    oerr << "Shim " << i+1 << ": no significant change of frequency offset";
                    failure_reasons.push_back (oerr.str());
                    significant_value[1]=false;
                }
                if( fabs(fid_shim_result[i].peakwidth - fid_base_result->peakwidth) < 
                    10*(fid_base_result->peakwidth_sd+fid_shim_result[i].peakwidth_sd))
                {
                    oerr.str("");
                    oerr << "Shim " << i+1 << ": no significant change of peak width";
                    failure_reasons.push_back (oerr.str());
                    significant_value[2]=false;
                }
                if(significant_value[0] == false &&
                   significant_value[1] == false &&
                   significant_value[2] == false)
                {
                    ShimFIDTestSuccess = succeeded[i] = No;   
                }
            }
            if(failure_reasons.empty ()) failure_reasons.push_back ("Succeeded");
            ParxRelsParSetStringArrayValue ("TestMessages", failure_reasons);
            // prepare report parameter
            ParxRelsGroupRoutine("PVM_CfgInfoMrGroup", MODALITY_HIERARCHY, GroupRelation);
            PvShimSystem shimsys;
            PARX_change_dims("ShimCoeffHwStatus",shimsys.shimState().size ()+1);
            ShimCoeffHwStatus[shimsys.shimState().size ()]='\0';
            PARX_change_dims("ShimMaxSPS",shimsys.nShims ());
            PARX_change_dims("ShimMaxCoil",shimsys.nShims ());
            std::copy(shimsys.shimMaxCurrentCoil ().begin (),
                      shimsys.shimMaxCurrentCoil ().end (),
                      ShimMaxCoil);
            std::copy(shimsys.shimMaxCurrentSPS ().begin (),
                      shimsys.shimMaxCurrentSPS().end (),
                      ShimMaxSPS);
            shimsys.shimState().copy(ShimCoeffHwStatus,shimsys.shimState().size());

            // write report results into parameter file
            std::string parfile = pathToParentDset(true,0);
            parfile += (std::string ("/") + "ShimFidEffectPars");
            ParxRelsWriteParList({ "TestShimEffectPars" }, parfile.c_str(), 
                Parx::Utils::WriteMode::NormalMode);
            
            // set autocounter to 0: finish adjustment
            acval=0;
        }
        
        DB_MSG (("<-- Perform test shim effect adjustment "
                "actbase = %d of %d actshim = %d of %d",actfid,nfid,actshim,nshim));
    }


    TestShimEffectAutoCounter = acval;
    DB_MSG (("<--TestShimEffectAutoCounterRel"));
    return;
}

void
ThisRGAdjustmentCounterRel (void)
{
    DB_MSG (("-->ThisRGAdjustmentCounterRel"));
    int acval = 0;
    if (!ParxRelsParHasValue ("ThisRGAdjustmentCounter"))
        acval = 0;
    else
        acval = ThisRGAdjustmentCounter;

    static const ADJ_RESULT_PAR ADJ_RG_RESULT_PARS[] = \
    {
        { adj_type, "ACQ_jobs"},
        { adj_type, "PVM_AutoRg"}
    };
    
    if (acval == 0)
    {
        DB_MSG (("Initialize RG adjustment..."));
        ATB_RgAdjSetGsPars("ThisRGAdjustmentCounter");
        ATB_AutoRgInit();
        acval=1;
    }
    else
    {
        double digfill=ATB_GetDigFill();
        JOB_DESCRIPTION_TYPE *jobs=ACQ_jobs;
        DB_MSG (("Perform RG adjustment..."));
        switch(ATB_AdjRgAlg(digfill,
                            &jobs[0].receiverGain,
                            10,
                            50,
                            No))
        {
        default:
        case 1:
            DB_MSG((" Adjusting RG: digfill %f",digfill));
            break;
        case -6:
            sprintf(AdjErrorComment,
                    "Illegal limits for digfilling passed");
            acval=-1;
            break;
        case -2:
            sprintf (AdjErrorComment, "Signal Intensity too High:\n"
                     "Digitizer overflow at lowest possible receiver gain");
            acval=-1;
            break;
        case -3:
            sprintf (AdjErrorComment, "RG Adjustment: Stop due to unstable signal.");
            acval=-1;
            break;
        case 0:
            acval = 0;
            DB_MSG ((" RG adjusted: digfill %f", digfill));
            ATB_SetAutoRg(jobs[0].title, jobs[0].receiverGain);
            PvAdjManSetAdjParameterList (ADJ_RG_RESULT_PARS,
                                         ARRAY_LEN (ADJ_RG_RESULT_PARS));
            break;            
        }   
    }


    ThisRGAdjustmentCounter = acval;
    DB_MSG (("<--ThisRGAdjustmentCounterRel"));
    return;
}

void
SetTestShimEffectAdjParList (void)
{

    static const ADJ_RESULT_PAR resultPars[] = \
       {
        { scan_result, "Testshimsets"},
        { scan_result, "BaseFIDResults"},
        { scan_result, "BaseFIDResult"},
        { scan_result, "BaseFIDResultSD"},
        { scan_result, "ShimFIDResults"},
        { scan_result, "NShimFID"},
        { scan_result, "TestSucceeded"},
        { scan_result, "TestMessages"},
        { scan_result, "BasicShim"},
        { adj_type, "ACQ_jobs" },
        { adj_type, "PVM_AutoRg"}
    };
    PvAdjManSetAdjParameterList (resultPars,
                                 ARRAY_LEN (resultPars));

    }


void
HandleTestShimEffectAdj (void)
{
    DB_MSG (("-->HandleTestShimServerAdj"));


    InitShimEffectAdj ();

    bool subpInitialized = false;
    subpInitialized = PTB_CheckSubProcess (0) == Yes ? false : true;
    std::string actsubp = subpInitialized ? PTB_GetAdjSubProcess () : "";


    std::stringstream msg;


    msg << "Adjustment : TestShimEffect";
    if (subpInitialized)
        msg << "Subprocess " << actsubp << std::endl;
    else
        msg << "Initialization" << std::endl;



    if (!subpInitialized)
    {
        std::vector<AdjSubAdjType> subadjlist;
        AdjSubAdjType csubadj;
        memset (&csubadj, 0, sizeof (AdjSubAdjType));
        csubadj.methodContext[0] = '\0';
        strcpy (csubadj.name, "Shimeffect-RGADJ");
        subadjlist.push_back (csubadj);
        strcpy (csubadj.name, "Shimeffect-ADJ");
        subadjlist.push_back (csubadj);

        if (PTB_InitSubProcess (subadjlist) != (int) subadjlist.size ())
        {
            throw PvStdException ("Failed to set up %d subadjustments!\n",
                                  (int) subadjlist.size ());
        }

        SetTestShimEffectAdjParList ();
    }
    else if (!actsubp.find ("Shimeffect-RGADJ"))
    {
        strcpy (ACQ_SetupAutoName, "ThisRGAdjustmentCounter");
    }
    else if (!actsubp.find ("Shimeffect-ADJ"))
    {
        SetTestShimEffectAdjParList ();
        strcpy (ACQ_SetupAutoName, "TestShimEffectAutoCounter");
    }

    TestDynamicShims = No;



    std::cout << msg.str ();
    AdjPerformState = adj_not_done;
    
    if (No == PTB_AdjustmentStarting ())
    {
        return;
    }



    DB_MSG (("<--HandleTestShimEffectAdj:\n%s",msg.str().c_str()));
    return;
}


int NFIDSteadyStateRange(void)
{
    int nsettling=3;
    if(!ParxRelsParHasValue("NFIDSteadyState"))
    {
        NFIDSteadyState=nsettling;
    }
    else
    {
        nsettling=NFIDSteadyState;
        NFIDSteadyState=nsettling=MAX_OF(MIN_OF(10,nsettling),2);
    }

    return nsettling;
}

int
NFIDRange (void)
{
    int nbase = 10;
    if (!ParxRelsParHasValue ("NFID"))
    {
        NFID = nbase;
    }
    else
    {
        nbase = NFID;
        NFID = MAX_OF (MIN_OF (30, nbase), 5);
    }
    return NFID;
}

void TestShimCurrentRel(void)
{
    int testchan=0;
    PvShimSystem shimsys;
    if(!ParxRelsParHasValue("TestShimChannel"))
    {
        TestShimChannel=1;
    }
    else
    {
        int nshims=shimsys.nShims ();
        testchan=TestShimChannel;
        testchan=MAX_OF(MIN_OF(testchan,nshims-1),0);
        TestShimChannel=testchan;
    }
    double maxamp=shimsys.shimMaxCurrent()[testchan];
    double testcurrent=0.0;
    if(!ParxRelsParHasValue("TestShimCurrent"))
    {
        TestShimCurrent=0.1;
    }
    testcurrent=TestShimCurrent;
    testcurrent=MAX_OF(MIN_OF(maxamp,testcurrent),-maxamp);
    TestShimCurrent=testcurrent;
    
    TestShimCurrentPerc=100*testcurrent/(shimsys.shimMaxCurrentSPS ()[testchan]);
    
    
    std::string shimidentifier=shimsys.shimIdentifiers ()[testchan];
    PARX_change_dims("TestShimIdentifier",shimidentifier.size ()+1);
    TestShimIdentifier[shimidentifier.size ()]='\0';
    shimidentifier.copy(TestShimIdentifier,shimidentifier.size ());
}

void TestShimCurrentAutocounterRel(void)
{
    int acval=0;
    
    if(!ParxRelsParHasValue("TestShimCurrentAutocounter"))
    {
        TestShimCurrentAutocounter=acval;
    }
    
    acval=TestShimCurrentAutocounter;
    
    if(acval==0)
    {
        TestDynamicShims=No;
        TestShimCurrentRel();
        TestShimCurrentAutocounter=1;
        return;
    }
    
    std::vector<double> current_shimset;
    
    double shimval= (acval % 2) ? TestShimCurrentPerc : 0.0;
    
    ATB_GetActualShim(current_shimset);
    current_shimset[TestShimChannel] = shimval;
    ATB_LoadShim(current_shimset);
    
}
