/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2010 - 2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/
const char * methods_FAIR_RARE_deriveVisu_id = "$Id$ (C) 2010 - 2016 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

/*:=INFO=:*******************************************************
 * Description :
 *   This file contains method dependant derivation of the 
 *   VISU overlay values.
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

/*--------------------------------------------------------------*
 * ParaVision include files...
 *--------------------------------------------------------------*/

/*--------------------------------------------------------------*
 * local include files...
 *--------------------------------------------------------------*/
#include "method.h"
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
        if (1 < invFrames*(PVM_FairMode==INTERLEAVED || PVM_FairMode==INTERLEAVED2 ? 2:1))
            /* Change id of Movie loop for inversion */
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

            PTB_VisuSetLoopDependentParameter(
                PV_IDL_CONSTANT(visuid_framegroup_MOVIE), 
                "VisuAcqInversionTime", 0);

            if (INTERLEAVED2 != PVM_FairMode)
              PTB_VisuRemoveLoopDependentParameter(
                PV_IDL_CONSTANT(visuid_framegroup_MOVIE), 
                "VisuFGElemComment", Yes);
        }

    }

    DB_MSG (("Leave deriveVisu()"));
}


/****************************************************************/
/*		L O C A L   F U N C T I O N S			*/
/****************************************************************/


/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
