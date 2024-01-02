/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2013 - 2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/
const char *  methods_ute_derivevisu_id = "$Id$ (C) 2005 - 2016 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	1

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
#include "Parx/Utils/ParxArray.hh"
#include "generated/VisuIds.h"
#include "Visu/VisuTypes.h"

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
 *	UTE dependant VISU creation.
 * Interface:							*/
void deriveVisu(void)
/*:=MPE=:=======================================================*/
{
  DB_MSG (("Entered deriveVisu()"));

  /* Standard VISU Derivation */
  ParxRelsParRelations("VisuDerivePars", Yes);

  if (0 == VisuFileVersion)
  {

    const char * movieFgGroup;
    const char * movieFgComment;
    const char * frameTimesPar = 0;
    VisuTriggerTimeType * frameTimes = 0;
    int cycleTime = 0;
    const int numMovieFrames = OutputMovieFrames;
    if (MovieType == Cardiac)
    {
      VisuCardiacSynchUsed = Yes;
      VisuCardiacSynchTechnique = CARDIAC_RETROSPECTIVE;
      VisuCardiacSignalSource = CARDIAC_MR;
      VisuCardiacHeartRate = HeartRate;
      cycleTime = CardiacPeriod;
      VisuCardiacCycle = cycleTime;
      movieFgGroup = PV_IDL_CONSTANT(visuid_framegroup_CARDIAC_MOVIE);
      movieFgComment = "Cardiac Frame";
      frameTimesPar = "VisuCardiacMovieFrameTime";
      PARX_change_dims("VisuCardiacMovieFrameTime", numMovieFrames);
      frameTimes = VisuCardiacMovieFrameTime;
    }
    else
    {
      VisuRespSynchUsed = Yes;
      VisuRespSynchTechnique = RESP_RETROSPECTIVE;
      VisuRespSignalSource = RESP_NAVIGATOR;
      cycleTime = RespPeriod;
      VisuRespCycle = cycleTime;
      movieFgGroup =  PV_IDL_CONSTANT(visuid_framegroup_RESP_MOVIE);
      movieFgComment = "Respiration Frame";
      frameTimesPar = "VisuRespMovieFrameTime";
      PARX_change_dims("VisuRespMovieFrameTime", numMovieFrames);
      frameTimes = VisuRespMovieFrameTime;
    }

    /* Change id of Echo loop to movie loop */
    MRT_VisuSetEchoLoopId(movieFgGroup);

    Parx::Utils::ParxStringArray comments(numMovieFrames, 30);
    const double frameDist = cycleTime / numMovieFrames;
    for (int i = 0; i < numMovieFrames; i++)
    {
      snprintf(comments[i],30,"%s %d",movieFgComment, i+1);
      frameTimes[i].nominal = i * frameDist;
      frameTimes[i].actual = i * frameDist;
    }
        
    PTB_VisuSetFGElementComment(movieFgGroup, comments.get(), 
				numMovieFrames, 30);
    PTB_VisuSetLoopDependentParameter(movieFgGroup, frameTimesPar, 0);

  }

 
  if (3 > VisuFileVersion)
    VisuAcqKSpaceTraversal = RadialTraversal;

  DB_MSG (("Leave deriveVisu()"));
}


/****************************************************************/
/*		L O C A L   F U N C T I O N S			*/
/****************************************************************/


/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
