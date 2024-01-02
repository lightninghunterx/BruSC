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
const char * methods_T1_EPI_deriveVisu_id = "$Id$ (C) 2005 - 2016 Bruker BioSpin MRI GmbH";

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
 
    MRT_VisuSetMovieLoopName("Inversion");

    const int invFrames = NIRPoints;
    if (1 == SliceGroupSize || Yes == SliceGroupRotation)
    {
      /* Set inversion time parameter */
      PARX_change_dims("VisuAcqInversionTime", invFrames);
      double * invTime = VisuAcqInversionTime;
      for (int i = 0; i < invFrames; ++i)
        invTime[i] = ACQ_inversion_time[i];
    
      PTB_VisuSetLoopDependentParameter(
        PV_IDL_CONSTANT(visuid_framegroup_MOVIE),
        "VisuAcqInversionTime", 0);
    
      PTB_VisuRemoveLoopDependentParameter(
        PV_IDL_CONSTANT(visuid_framegroup_MOVIE),
        "VisuFGElemComment", Yes); 
    }
    else
    {
      const int nslices = InpSlices;
      PARX_change_dims("VisuAcqInversionTime", VisuCoreFrameCount);

      const int outerloop = VisuCoreFrameCount / (nslices * invFrames);
      double * invTime = VisuAcqInversionTime;
      for (int k = 0; k < outerloop; ++k)
        for (int i = 0; i < invFrames; ++i) 
          for (int j = 0; j < nslices; ++j)
            invTime[k*nslices*invFrames+i*nslices+j] = IRTime[j][i];

      PTB_VisuRemoveLoopDependentParameter(
        PV_IDL_CONSTANT(visuid_framegroup_MOVIE),
        "VisuFGElemComment", Yes); 
    }

    VisuAcqEchoTrainLength = PVM_EpiNEchoes;

    double swh = PVM_EffSWh * PVM_AntiAlias[0];

    if (0.0 < swh)
      VisuAcqPixelBandwidth = swh/(double)PVM_EncMatrix[0];
  }
  DB_MSG (("Leave deriveVisu()"));
}


/****************************************************************/
/*		L O C A L   F U N C T I O N S			*/
/****************************************************************/


/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/

 
