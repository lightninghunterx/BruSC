/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id: RecoRelations.c,v 1.1.2.2 2001/01/11 MAWI ";

#define DEBUG           0
#define DB_MODULE       0
#define DB_LINE_NR      0

#include "method.h"

void
SetRecoParam (void)
{


    DB_MSG (("-->SetRecoParam\n"));

    /* set baselevel reconstruction parameter */
    /* default initialization of reco based on acqp pars allready set */

    ATB_InitUserModeReco (
                          1,
                          0,
                          PVM_SpecMatrix, // acq size
                          PVM_SpecMatrix, // image size
                          NULL, //effAntiAlias=1,
                          NULL, //effPftOverscans=halfsize,
                          1,
                          NULL,
                          1,
                          NULL, //linear order,
                          NULL, //linear order (not used)
                          NULL, //linear order (not used)
                          PVM_EncNReceivers,
                          PVM_EncChanScaling,
                          0,
                          1);

    /* complex channel combination: */
    RecoCombineMode = AddImages;

    /* configure information available during setup mode */

    ACQ_SetupShowDigFilling = Yes;
    ParxRelsParRelations ("ACQ_SetupShowDigFilling", Yes);

    /* Setting phase offsets for multi RX channel combination */
    ATB_ArrayPhaseSetRecoPhase ();
    SetdisplayInfonames ();
    DB_MSG (("<--SetRecoParam\n"));
}

void
DeriveReco (void)
{
    DB_MSG (("-->DeriveReco"));

    if (RecoUserUpdate == No)
    {
        DB_MSG (("<--RecoDerive: no update"));
        return;
    }
    try
    {
        /* standard settings for reconstruction */
        if (RecoPrototype == No)
            SetRecoParam ();
        ParxRelsParRelations ("RecoUserUpdate", Yes);

        if (ACQ_scan_type != Setup_Experiment)
            return;

        int nchan = SetDimOfFrqOffsPars();

        // append filter to calculate normalized area
        RecoComputeAppendStage (RECOFIRSTPASS,
                                0,
                                "Q",
                                "RecoNormalizedAreaFilter",
                                "NAREA",
                                "normalizedArea=<Narea>;");

        DB_MSG (("RecoComputeAppendStage(%s,\n"
                 "                       %d,\n"
                 "                       %s,\n"
                 "                       %s,\n"
                 "                       %s,\n"
                 "                       %s);\n",
                 RECOFIRSTPASS, 0, "Q",
                 "RecoNormalizedAreaFilter",
                 "NAREA",
                 "normalizedArea=<Narea>;"));

        // insert filter to calculate frequency offsets and peak width




        std::stringstream ofrqargs;
        ofrqargs << "nchan=" << nchan << ";"
                << "nspec=1;"
                << "swh=" << PVM_SpecSWH[0] << ";"
                << "nnoisep=" << MIN_OF (PVM_SpecMatrix[0] / 10, 100) << ";"
                << "applyPhase=true;"
                << "estLorentzPw=true"
                << "pwl={ 50 };"
                << "wpw=<Wpwidth>;"
                << "weights=<Narea>;"
                << "wafrqoffs=<WfrqOffs>;";


        DB_MSG (("RecoComputeInsertStage(%s,\n"
                 "                       %d,\n"
                 "                       %s,\n"
                 "                       %s,\n"
                 "                       %s,\n"
                 "                       %s);\n",
                 "RECOPREPPASS",
                 0,
                 "M",
                 "RecoFrqOffsetFilter",
                 "OFRQ",
                 ofrqargs.str ().c_str ()));

        RecoComputeInsertStage (RECOPREPPASS,
                                0,
                                "M",
                                "RecoFrqOffsetFilter",
                                "OFRQ",
                                ofrqargs.str ().c_str ());
        //Filter for RefScan and ArrayPhase Adjustment
        ATB_GetRecoDataPoints (PVM_RecoDataPointsRequired, PVM_EncNReceivers * RECO_inp_size[0]*2, "Q");


    }
    catch (const PvException &ex)
    {
        DB_MSG (("Exception occured:\n%s\n", ex.what ()));
        ParxRelsParRelations ("RecoUserUpdate", Yes);
    }

    DB_MSG (("<--DeriveReco"));
}


int
SetDimOfFrqOffsPars (void)
{
    int nchan = 0;
    try
    {
        nchan = RecoNrActiveReceivers ();
        if (nchan < 1)
        {
            throw PvStdException ("RecoNrActiveReceivers returns illegal"
                                  " value %d\n", nchan);
        }
        PARX_change_dims ("Narea", 1, nchan);
    }
    catch (const PvException &ex)
    {
        throw PvStdException ("SetDimOfTuneshimInterfacePars failed,"
                              " reason:\n%s", ex.what ());

    }
    return nchan;

}

void SetdisplayInfonames(void)
{

  SetDimOfFrqOffsPars();
  std::string recoinfopars="WfrqOffs[0]/Frequency Offset,Wpwidth[0][0]/Line Width (50\%)";
  PARX_change_dims("ACQ_RecoInfoNames",recoinfopars.size()+1);
  ParxRelsParSetValue("ACQ_RecoInfoNames",recoinfopars.c_str());
  std::string signalinfopars="Narea/Normalized Area";
  size_t slen=signalinfopars.size()+1;

  if(1!=(int)PARX_get_dim("ACQ_SignalInfoNames",1) ||
     slen!=PARX_get_dim("ACQ_SignalInfoNames",2))
  {
    PARX_change_dims("ACQ_SignalInfoNames",1,slen);
  }    
  ParxRelsParSetValue("ACQ_SignalInfoNames[0]",signalinfopars.c_str());

}


