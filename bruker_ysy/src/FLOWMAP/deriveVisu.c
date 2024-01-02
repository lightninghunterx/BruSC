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
const char * methods_FLOWMAP_deriveVisu_id = "$Id$ (C) 2005 - 2016 Bruker BioSpin MRI GmbH";

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
void deriveVisu(void)
/*:=MPE=:=======================================================*/
{ 
  DB_MSG (("Entered deriveVisu()"));

  /* Standard Visu Derivation */
  ParxRelsParRelations("VisuDerivePars", Yes);

  if (0 == VisuFileVersion)
  {

    const int flowEncDim = RecoMethMode == FLOWMAP ? FlowImages:FlowEncLoop;
    const FLOW_MODE flowMode = FlowMode;

    if (PVM_NMovieFrames > 1)
    {
      /* Change id of Echo loop to movie loop */
      MRT_VisuSetEchoLoopId(PV_IDL_CONSTANT(visuid_framegroup_MOVIE)); 
      PTB_VisuInsertLoop(
	0,
	PV_IDL_CONSTANT(visuid_framegroup_FLOW),
	flowEncDim, "");
    
      PTB_VisuChangeFGLen(
        PV_IDL_CONSTANT(visuid_framegroup_MOVIE), PVM_NMovieFrames);
      PTB_VisuRemoveLoopDependentParameter(
        PV_IDL_CONSTANT(visuid_framegroup_MOVIE),
        "VisuFGElemComment", Yes);
 
    }
    else
    {
      if(flowMode != FourierFlowImaging)
        MRT_VisuSetEchoLoopId(PV_IDL_CONSTANT(visuid_framegroup_FLOW));
    }   
    
    if (flowMode != FourierFlowImaging && RecoMethMode == FLOWMAP)
    {

      char * comments = (char *)calloc(flowEncDim, 40 * sizeof(char));
  
      PARX_change_dims("VisuCoreDataUnits", flowEncDim, VISU_MAX_NAME_LEN);
      memset(VisuCoreDataUnits, 0, flowEncDim * VISU_MAX_NAME_LEN *sizeof(char));
  

      if(flowMode == VelocityMapping)
      {
        if(FlowEncodingDirection == AllDirections)
        {
          strcpy(&comments[0 * 40],"Flow Read");
          strcpy(VisuCoreDataUnits[0], "cm/s");
          strcpy(&comments[1 * 40],"Flow Phase");
          strcpy(VisuCoreDataUnits[1], "cm/s");
          strcpy(&comments[2 * 40],"Flow Slice");
          strcpy(VisuCoreDataUnits[2], "cm/s");
        }
        else
        {
          strcpy(&comments[0*40],"Flow");
          strcpy(VisuCoreDataUnits[0], "cm/s");
        }
      }

      if(flowMode == PhaseContrastAngiography)
      {
	strcpy(&comments[0*40],"Angiogram");
	strcpy(&comments[1*40],"Magnitude");
      }

      PTB_VisuSetLoopDependentParameter(
        PV_IDL_CONSTANT(visuid_framegroup_FLOW), 
        "VisuCoreDataUnits", 0);

      PTB_VisuSetFGElementComment(
        PV_IDL_CONSTANT(visuid_framegroup_FLOW),
        comments, flowEncDim, 40);

      free(comments);
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
