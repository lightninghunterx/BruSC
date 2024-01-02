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
const char * methods_spiral_derivevisu_id = "$Id$ (C) 2005 Bruker BioSpin MRI GmbH";

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
 *	SPIRAL dependant VISU creation.
 * Interface:							*/
void deriveVisu(void)
/*:=MPE=:=======================================================*/
{
  DB_MSG (("Entered deriveVisu()"));

  /* Standard Visu Derivation */
  ParxRelsParRelations("VisuDerivePars", Yes);

  /* Derive diffusion frame group. */
  if(RecoMethMode == Default)
  { 
    MRT_VisuDeriveDiffusionFrameGroup(PVM_DwNDiffExp, PVM_NRepetitions);
  }
  else
  {
    if (VisuDeriveDTIFrameGroup() < 0)
      UT_ReportError("Error while deriving DTI frame group");

    strcpy(VisuSeriesTypeId,PV_IDL_CONSTANT(visuid_series_DTI));
    strcpy(VisuSeriesComment,"Generated Diffusion Tensor Images");
  }  

  if (3 > VisuFileVersion)
      VisuAcqKSpaceTraversal = SpiralTraversal;

  DB_MSG (("Leave deriveVisu()"));
}


/****************************************************************/
/*		L O C A L   F U N C T I O N S			*/
/****************************************************************/
int VisuDeriveDTIFrameGroup(void)
{
    int ndti=23;
    int fgNdx;
    DB_MSG(("-->DeriveDTIFrameGroup"));

    char DTIids [23 * VISU_MAX_NAME_LEN] = "\0";

    strcpy(&DTIids[ 0 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_FA));
    strcpy(&DTIids[ 1 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_TRACE));
    strcpy(&DTIids[ 2 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_A0));
    strcpy(&DTIids[ 3 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_TWI));
    strcpy(&DTIids[ 4 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_DXX));
    strcpy(&DTIids[ 5 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_DYY));
    strcpy(&DTIids[ 6 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_DZZ));
    strcpy(&DTIids[ 7 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_DXY));
    strcpy(&DTIids[ 8 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_DXZ));
    strcpy(&DTIids[ 9 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_DYZ));
    strcpy(&DTIids[10 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_L1));
    strcpy(&DTIids[11 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_L2));
    strcpy(&DTIids[12 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_L3));
    strcpy(&DTIids[13 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_L1X));
    strcpy(&DTIids[14 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_L1Y));
    strcpy(&DTIids[15 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_L1Z));
    strcpy(&DTIids[16 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_L2X));
    strcpy(&DTIids[17 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_L2Y));
    strcpy(&DTIids[18 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_L2Z));
    strcpy(&DTIids[19 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_L3X));
    strcpy(&DTIids[20 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_L3Y));
    strcpy(&DTIids[21 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_L3Z));
    strcpy(&DTIids[22 * VISU_MAX_NAME_LEN],PV_IDL_CONSTANT(visuid_dti_ERR));

    const char DTIcomments[] =
        "Fractional Anisotropy\0                                           "
        "Trace\0                                                           "
        "Intensity\0                                                       "
        "Trace Weighted Image\0                                            "
        "Tensor Component Dxx\0                                            "
        "Tensor Component Dyy\0                                            "
        "Tensor Component Dzz\0                                            "
        "Tensor Component Dxy\0                                            "
        "Tensor Component Dxz\0                                            "
        "Tensor Component Dyz\0                                            "
        "1st Eigenvalue\0                                                  "
        "2nd Eigenvalue\0                                                  "
        "3rd Eigenvalue\0                                                  "
        "1st Eigenvector X\0                                               "
        "1st Eigenvector Y\0                                               "
        "1st Eigenvector Z\0                                               "
        "2nd Eigenvector X\0                                               "
        "2nd Eigenvector Y\0                                               "
        "2nd Eigenvector Z\0                                               "
        "3rd Eigenvector X\0                                               "
        "3rd Eigenvector Y\0                                               "
        "3rd Eigenvector Z\0                                               "
        "Pixel Valid\0                                                     ";

    const char DTIunits[] =
        "\0                                                                "
        "mm2/s\0                                                           "
        "\0                                                                "
        "\0                                                                "
        "mm2/s\0                                                           "
        "mm2/s\0                                                           "
        "mm2/s\0                                                           "
        "mm2/s\0                                                           "
        "mm2/s\0                                                           "
        "mm2/s\0                                                           "
        "mm2/s\0                                                           "
        "mm2/s\0                                                           "
        "mm2/s\0                                                           "
        "\0                                                                "
        "\0                                                                "
        "\0                                                                "
        "\0                                                                "
        "\0                                                                "
        "\0                                                                "
        "\0                                                                "
        "\0                                                                "
        "\0                                                                "
        "\0                                                                ";

    if (VisuFileVersion == 0)
    {
        /* Rename movie frame group to DTI frame group */
        if (MRT_VisuSetMovieLoopId(PV_IDL_CONSTANT(visuid_framegroup_DTI)) < 0)
            return -1;

        if (PTB_VisuSetLoopName(PV_IDL_CONSTANT(visuid_framegroup_DTI),
                                "Generated Diffusion Tensor Images") < 0)
            return -1;
        
        fgNdx = VisuGetFgNdxFromGroupId(PV_IDL_CONSTANT(visuid_framegroup_DTI));
        if (fgNdx < 0) 
            return -1;

        /* set length */
        if (VisuChangeFGLen(fgNdx,ndti) < 0)
            return -1;

        /* clear units */
        PARX_change_dims("VisuCoreDataUnits", ndti, VISU_MAX_NAME_LEN);      
        memset(VisuCoreDataUnits, 0, ndti*VISU_MAX_NAME_LEN*sizeof(char));

        /* set element ids, comments and units, set id and comment as FG dependent value */
        VisuSetFgElementId(fgNdx, DTIids, ndti, VISU_MAX_NAME_LEN);
        VisuSetFgElementComment(fgNdx, DTIcomments, ndti, VISU_MAX_NAME_LEN);
        for(int i=0; i<ndti; i++)
            strcpy(VisuCoreDataUnits[i], &DTIunits[i*VISU_MAX_NAME_LEN]);

        /* set units as FG dependent value */
        PTB_VisuSetLoopDependentParameter(PV_IDL_CONSTANT(visuid_framegroup_DTI),
                "VisuCoreDataUnits", 0);
    }
    
    DB_MSG(("<--DeriveDTIFrameGroup"));
    return 0;
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
