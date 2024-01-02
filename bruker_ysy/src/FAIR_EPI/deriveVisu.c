/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2005 - 2017
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/
const char * methods_FAIR_EPI_deriveVisu_id = "$Id$ (C) 2005-2017 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	1

/*:=INFO=:*******************************************************
 * Description :
 *   This file contains method dependant derivation of the 
 *   visu overlay values.
 *::=info=:******************************************************/

/****************************************************************/
/****************************************************************/
/*		I N T E R F A C E   S E C T I O N		*/
/****************************************************************/
/****************************************************************/

/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "pvmachine.h"
/*--------------------------------------------------------------*
 * system include files...
 *--------------------------------------------------------------*/
#include <ctype.h>

/*--------------------------------------------------------------*
 * ParaVision include files...
 *--------------------------------------------------------------*/

/*--------------------------------------------------------------*
 * local include files...
 *--------------------------------------------------------------*/
#include "method.h"
#include "PvUtil/PvVersionParser.hh"
#include "generated/VisuIds.h"

/****************************************************************/
/*		E X T E R N A L   F U N C T I O N S		*/
/****************************************************************/

/****************************************************************/
/*		E X T E R N A L   V A R I A B L E S		*/
/****************************************************************/

/****************************************************************/
/*		G L O B A L   V A R I A B L E S			*/
/****************************************************************/

/****************************************************************/
/****************************************************************/
/*	I M P L E M E N T A T I O N   S E C T I O N		*/
/****************************************************************/
/****************************************************************/

/****************************************************************/
/*		L O C A L   D E F I N I T I O N S		*/
/****************************************************************/

/****************************************************************/
/*	L O C A L   F U N C T I O N   P R O T O T Y P E S	*/
/*			forward references			*/
/****************************************************************/
static void deriveVisuOldPv();

/****************************************************************/
/*		L O C A L   V A R I A B L E S			*/
/****************************************************************/

/****************************************************************/
/*		G L O B A L   F U N C T I O N S			*/
/****************************************************************/


/*:=MPB=:=======================================================*
 *
 * Global Function: deriveVisu
 * Description:
 *	DTI dependant visu creation.
 * Interface:							*/
void 
deriveVisu(void)
/*:=MPE=:=======================================================*/
{
    int invFrames = PVM_FairTIR_NExp;
    DB_MSG (("Entered deriveVisu()"));

    /* Standard Visu Derivation */
    ParxRelsParRelations("VisuDerivePars", Yes);

    if (0 == VisuFileVersion)
    {

        bool oldPv = !ParxRelsParHasValue("PVM_EpiNEchoes");
      
        if (!oldPv && ParxRelsParHasValue("ACQ_sw_version"))
        {
            try
            {
                PvUtil::PvVersionParser vp(ACQ_sw_version);
                oldPv = vp.getMajor() < 5;
            }
            catch (...)
            {
                oldPv = true;
            }
        }

        if (oldPv)
            deriveVisuOldPv();
        else
        {
            if (1 < invFrames*(PVM_FairMode==INTERLEAVED || PVM_FairMode==INTERLEAVED2 ? 2:1))
                /* Change id of Echo loop for inversion */
            {
                int i;
                double * invTime;
    
                switch(PVM_FairMode)
                {
                  case NONSELECTIVE:
                      MRT_VisuSetMovieLoopName("Non-selective Inversion");
                      break;
      
                  case SELECTIVE:
                      MRT_VisuSetMovieLoopName("Selective Inversion");
                      break;
        
                  case INTERLEAVED: 
                    if (1 < invFrames)

                    {
                      MRT_VisuSetMovieLoopName("Inversion");
                      PTB_VisuInsertLoop(
                        PV_IDL_CONSTANT(visuid_framegroup_MOVIE),
                        PV_IDL_CONSTANT(visuid_framegroup_IRMODE),
                        2, "Inversion Mode");
                      PTB_VisuChangeFGLen(
                        PV_IDL_CONSTANT(visuid_framegroup_MOVIE), invFrames);
                    }
                    else
                    {
                      MRT_VisuSetMovieLoopId(PV_IDL_CONSTANT(visuid_framegroup_IRMODE));
                      PTB_VisuSetLoopName(PV_IDL_CONSTANT(visuid_framegroup_IRMODE),
                                          "Inversion Mode");
                    }
                    {
                      char comments[80];
                      memset(comments, 0, 80);
                      strcpy(comments, "Selective Inversion");
                      strcpy(&comments[40], "Non-selective Inversion");
                      PTB_VisuSetFGElementComment(
                        PV_IDL_CONSTANT(visuid_framegroup_IRMODE),
                        comments, 2, 40);
                    }
                    break;

                  case INTERLEAVED2:
                    MRT_VisuSetMovieLoopId(PV_IDL_CONSTANT(visuid_framegroup_IRMODE));
                    PTB_VisuSetLoopName(PV_IDL_CONSTANT(visuid_framegroup_IRMODE),
                                        "Inversion Mode");
                    if (1 < invFrames)
                    {
                      PTB_VisuInsertLoop(
                        PV_IDL_CONSTANT(visuid_framegroup_IRMODE),
                        PV_IDL_CONSTANT(visuid_framegroup_MOVIE),
                        invFrames, "Inversion");

                      PTB_VisuChangeFGLen(
                        PV_IDL_CONSTANT(visuid_framegroup_IRMODE), 2);

                      char comments[80];
                      memset(comments, 0, 80);
                      strcpy(comments, "Selective Inversion");
                      strcpy(&comments[40], "Non-selective Inversion");
                      PTB_VisuSetFGElementComment(
                        PV_IDL_CONSTANT(visuid_framegroup_IRMODE),
                        comments, 2, 40);
                    }
                    break;
                }

                /* Set inversion time parameter in echo frame group */
                PARX_change_dims("VisuAcqInversionTime", invFrames);
                invTime = VisuAcqInversionTime;
                for (i = 0; i < invFrames; ++i)
                  invTime[i] = ACQ_inversion_time[i];

                if (invFrames > 1)
                {

                  PTB_VisuSetLoopDependentParameter(
                    PV_IDL_CONSTANT(visuid_framegroup_MOVIE), 
                    "VisuAcqInversionTime", 0);

                  if (INTERLEAVED2 != PVM_FairMode)
                    PTB_VisuRemoveLoopDependentParameter(
                      PV_IDL_CONSTANT(visuid_framegroup_MOVIE), 
                      "VisuFGElemComment", Yes);
                }

                VisuAcqEchoTrainLength = PVM_EpiNEchoes;
                double swh = PVM_EffSWh * PVM_AntiAlias[0];

                if (0.0 < swh)
                  VisuAcqPixelBandwidth = swh/(double)PVM_EncMatrix[0];
            }
        }
    }
    DB_MSG (("Leave deriveVisu()"));
}


/****************************************************************/
/*		L O C A L   F U N C T I O N S			*/
/****************************************************************/
static void 
deriveVisuOldPv(void)
/*:=MPE=:=======================================================*/
{
  DB_MSG (("Entered deriveVisuOldPv()"));
  
  int invFrames = PARX_get_dim("ACQ_inversion_time", 1);

  FAIR_INV_TYPE fairMode = INTERLEAVED;
  if (ACQ_n_echo_images == invFrames)
  {
      if (0 == strncmp(ACQ_echo_descr[0], " NS ", 4))
          fairMode = NONSELECTIVE;
      else
          fairMode = SELECTIVE;
  }


  if (1 < invFrames*(fairMode==INTERLEAVED? 2:1))
    /* Change id of Echo loop for inversion */
  {
    int i;
    
    switch(fairMode)
    {
      case NONSELECTIVE:
        MRT_VisuSetEchoLoopName("Non-selective Inversion");
        break;
      
      case SELECTIVE:
        MRT_VisuSetEchoLoopName("Selective Inversion");
        break;
        
      case INTERLEAVED2:   /* did not exist */
      case INTERLEAVED:  
        MRT_VisuSetEchoLoopName("Inversion");
        if (1 < invFrames)
        {
          PTB_VisuInsertLoop(
            PV_IDL_CONSTANT(visuid_framegroup_ECHO),
            PV_IDL_CONSTANT(visuid_framegroup_IRMODE),
            2, "Inversion Mode");
          PTB_VisuChangeFGLen(
            PV_IDL_CONSTANT(visuid_framegroup_ECHO), invFrames);
        }
        else
        {
          MRT_VisuSetMovieLoopId(PV_IDL_CONSTANT(visuid_framegroup_IRMODE));
          PTB_VisuSetLoopName(PV_IDL_CONSTANT(visuid_framegroup_IRMODE),
                              "Inversion Mode");
        }
        {
          char comments[80];
          memset(comments, 0, 80);
          strcpy(comments, "Selective Inversion");
          strcpy(&comments[40], "Non-selective Inversion");
          PTB_VisuSetFGElementComment(
            PV_IDL_CONSTANT(visuid_framegroup_IRMODE),
            comments, 2, 40);
        }
        break;

    }

    /* Set inversion time parameter in echo frame group */
    PARX_change_dims("VisuAcqInversionTime", invFrames);
    double * invTime = VisuAcqInversionTime;
    const double * acq_inversion_time = ACQ_inversion_time;
    for (i = 0; i < invFrames; ++i)
      invTime[i] = acq_inversion_time[i];

    PTB_VisuSetLoopDependentParameter(
      PV_IDL_CONSTANT(visuid_framegroup_ECHO), 
      "VisuAcqInversionTime", 0);

    /* Remove VisuAcqEchoTime from dependant parameters */
    PTB_VisuRemoveLoopDependentParameter(
      PV_IDL_CONSTANT(visuid_framegroup_ECHO), 
      "VisuAcqEchoTime", No);
    PARX_change_dims("VisuAcqEchoTime", 1);
    VisuAcqEchoTime[0] = EchoTime;

    PTB_VisuRemoveLoopDependentParameter(
      PV_IDL_CONSTANT(visuid_framegroup_ECHO), 
      "VisuFGElemComment", Yes);
  }

  DB_MSG (("Leave deriveVisuOldPv()"));
}


/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
