/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2005 - 2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/
const char *  methods_SegFLASH_deriveVisu_id = "$Id$ (C) 2005 - 2016 Bruker BioSpin MRI GmbH";

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

/*--------------------------------------------------------------*
 * ParaVision include files...
 *--------------------------------------------------------------*/

/*--------------------------------------------------------------*
 * local include files...
 *--------------------------------------------------------------*/
#include "method.h"
#include "Visu/VisuTypes.h"
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
/* void deriveVisu(void) */
/* /\*:=MPE=:=======================================================*\/ */
/* { */
/*     DB_MSG (("Entered deriveVisu()")); */

/*     /\* Standard VISU Derivation *\/ */
/*     ParxRelsParRelations("VisuDerivePars", Yes); */

/*     if (0 == VisuFileVersion) */
/*     { */

/*       if (PVM_NMovieFrames > 1) */
/*       { */
/*         MRT_VisuSetEchoLoopId(PV_IDL_CONSTANT(visuid_framegroup_MOVIE)); */

/*         if (InversionOnOff == On) */
/*         { */
/*           /\* Change id of Echo loop to movie loop *\/ */
/*           MRT_VisuSetMovieLoopName("Inversion"); */

/*           /\* Derive VisuAcInversion time *\/ */
/*           if (ParxRelsParHasValue("ACQ_inversion_time")) */
/*           { */
/*             const int invTimeDim = PARX_get_dim("ACQ_inversion_time", 1); */
/*             PARX_change_dims("VisuAcqInversionTime", invTimeDim); */
/*             memcpy(VisuAcqInversionTime, ACQ_inversion_time, */
/*                    invTimeDim * sizeof(double)); */
              
/*             //Set inversion time as movie loop dependent parameter */
/*             PTB_VisuSetLoopDependentParameter( */
/*               PV_IDL_CONSTANT(visuid_framegroup_MOVIE), */
/*               "VisuAcqInversionTime", 0); */
/*           } */
/*         } */
/*         else */
/*         { */
/*           /\* Change id of Echo loop to movie loop *\/ */
/*           MRT_VisuSetMovieLoopName("Movie"); */
/*         } */
/*       } */
/*     } */


/*     DB_MSG (("Leave deriveVisu()")); */
/* } */


void deriveVisu(void)
/*:=MPE=:=======================================================*/
{
    DB_MSG (("Entered deriveVisu()"));

    /* Standard VISU Derivation */
    ParxRelsParRelations("VisuDerivePars", Yes);

    if (0 == VisuFileVersion)
    {
        if (PVM_NMovieFrames > 1)
            /* Change id of Echo loop to movie loop */
            MRT_VisuSetEchoLoopId(PV_IDL_CONSTANT(visuid_framegroup_MOVIE));
    }


    if (3 > VisuFileVersion)
    {
        if (RFSpoiling == Yes)
            VisuAcqSpoiling = RFAndGradientSpoiled;

        VisuAcqEchoSequenceType = GradientEcho;
    }


    DB_MSG (("Leave deriveVisu()"));
}


/****************************************************************/
/*		L O C A L   F U N C T I O N S			*/
/****************************************************************/


/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
