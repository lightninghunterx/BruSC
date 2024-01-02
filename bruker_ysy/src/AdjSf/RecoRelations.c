/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2005-2010
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (c) 2005-2010 Bruker BioSpin MRI GmbH";

#define DEBUG           0
#define DB_MODULE       0
#define DB_LINE_NR      0



#include "method.h"
#include "PvUtil/PvUtil.h"
#include "PvUtil/PvExcept.hh"

void SetRecoParam( void )
{


  DB_MSG(("-->SetRecoParam\n"));

  /* set baselevel reconstruction parameter */
  /* Initialization of reco according to arguments  */

  int phaseFactor = 1;

  ATB_InitUserModeReco( // Arguments and their meaning:
    ACQ_dim,            // reconstrucion dimensions
    0,                  // number of non cartesian dimensions
    &AcqSize,           // number of acquired complex datapoints
    &AcqSize,           // number of reconstructe datapoints
    PVM_AntiAlias,      // fixed to 1 in this method
    NULL,               // pft overscans, here: centred acquisition
    NI,                 // number of objects to be reconstructed
    ACQ_obj_order,      // ordering of objects
    phaseFactor,        // fixed to 1 in this method
    NULL,               // no 2D encoding
    NULL,               // no 3D encoding
    NULL,               // no 4D encoding
    PVM_EncNReceivers,  // Controlled by encoding group
    PVM_EncChanScaling, // Controlled by encoding group
    0,                  // no reference lines for grappa 
    0);                 // no encoding step handling
    

  // sets reco rotate to 0.5 (position of 0 frequency in center of
  // frequency window)
  ATB_SetRecoRotate(NULL,1.0,NI,1,0) ;
  

  /* configure information available during setup mode */

  ACQ_SetupShowDigFilling     = Yes;
  ParxRelsParRelations("ACQ_SetupShowDigFilling",Yes); 

  ACQ_SetupType = Spectrometer_Parameters;
  //  ACQ_SetupImageType = MAGNITUDE_IMAGE;
  ACQ_SetupImageType = COMPLEX_IMAGE;

  ParxRelsParRelations("ACQ_SetupType",Yes);
  ParxRelsParRelations("ACQ_SetupImageType",Yes);

  
  ACQ_ScanOnlineReco=Yes;
  ACQ_SetupOnlineReco=Yes;
  ParxRelsParRelations("ACQ_ScanOnlineReco",Yes);
  ParxRelsParRelations("ACQ_SetupOnlineReco",Yes);

  ACQ_SetupShuffleProfiles = Yes;

  DB_MSG(("<--SetRecoParam\n"));
}

void DeriveReco(void)
{
  DB_MSG(("-->DeriveReco"));

  if (RecoUserUpdate == No)
  {
    DB_MSG(("<--RecoDerive: no update"));
    return;
  }


  try
  {
    /* standard settings for reconstruction */  
    if(RecoPrototype == No)
      SetRecoParam();
    ParxRelsParRelations("RecoUserUpdate", Yes);
    if(ACQ_scan_type !=Setup_Experiment)
      return;

    int nchan=RecoNrActiveReceivers();

    if(nchan < 1)
    {
      throw PvStdException("RecoNrActiveReceivers returns illegal"
                           " value %d\n",nchan);
    }


    RedimFrqOffsetFilterPars(nchan,Nexc,2);
    SetdisplayInfonames();
 
    // filter to calculate normalized area

    DB_MSG(("RecoComputeAppendStage(%s,\n"
            "                       %d,\n"
            "                       %s,\n"
            "                       %s,\n"
            "                       %s,\n"
            "                       %s);\n",
            RECOFIRSTPASS,0,"Q",
            "RecoNormalizedAreaFilter",
            "NAREA",
            "normalizedArea=<NormArea>;"));
           
    RecoComputeAppendStage(RECOFIRSTPASS,
                           0,
                           "Q",
                           "RecoNormalizedAreaFilter",
                           "NAREA",
                           "normalizedArea=<NormArea>;");

    // filter to calculate frqoffsets
          

    std::stringstream ofrqargs;
    ofrqargs << "nchan="   << nchan << ";" 
             << "nspec="   << Nexc  << ";"
             << "swh="     << PVM_EffSWh << ";"
             << "nnoisep=" << MIN_OF(AcqSize/10,100) << ";"
             << "applyPhase=true;"
             << "pwl={ 50,10 };"
             << "pwch=<PWidth>;"
             << "peak=<Peak>;"
             << "noise=<Noise>;"
             << "weights=<NormArea>;"
             << "cfrqoffs=<OFrq>;"
             << "wafrqoffs=<WOFrq>;";


      DB_MSG(("RecoComputeInsertStage(%s,\n"
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
              ofrqargs.str().c_str()));

    RecoComputeInsertStage(RECOPREPPASS,
                           0,
                           "M",
                           "RecoFrqOffsetFilter",
                           "OFRQ",
                           ofrqargs.str().c_str());

  }
  catch (const PvException &ex)
  {
    DB_MSG(("Exception occured:\n%s\n",ex.what()));
    ParxRelsParRelations("RecoUserUpdate", Yes);   
  }

 
  DB_MSG(("<--DeriveReco"));
}
