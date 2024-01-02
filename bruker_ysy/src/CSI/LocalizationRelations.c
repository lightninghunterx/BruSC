/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2012
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 * ***************************************************************/


static const char resid[] = "$Id $ (c) 2012 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"

void LocalizationHandler(void)
{
  DB_MSG(("-->LocalizationHandler"));
  if (LocalizationMode == Voxel_PRESS)
  {
    /* Voxel geometry */
    double pulseBandwidthArray[3] = {RefPulse1.Bandwidth,RefPulse1.Bandwidth,ExcPulse1.Bandwidth};
 
    DB_MSG(("UpdateVoxelGeometry"));

    STB_UpdateVoxelGeometry( pulseBandwidthArray, PVM_Nucleus1 ); //inserts GeoObj and sets dependencies

    ParxRelsShowInEditor("PVM_NVoxels");
    ParxRelsShowInEditor("PVM_VoxArrSize");
    ParxRelsShowInEditor("PVM_VoxArrPosition");
    ParxRelsShowInEditor("PVM_VoxArrPositionRPS");
    ParxRelsShowInEditor("PVM_VoxArrCSDisplacement");
  }
  else
  {
    PTB_RemoveGeoObject("PVM_VoxelGeoObj");

    ParxRelsHideInEditor("PVM_NVoxels");
    ParxRelsHideInEditor("PVM_VoxArrSize");
    ParxRelsHideInEditor("PVM_VoxArrPosition");
    ParxRelsHideInEditor("PVM_VoxArrPositionRPS");
    ParxRelsHideInEditor("PVM_VoxArrCSDisplacement");
  }

  if (LocalizationMode == Slice_FID)
  {
    ParxRelsHideInEditor("RefPulse1Enum");
    ParxRelsHideInEditor("RefPulse1");
    ParxRelsHideInEditor("RefSpoiler");
  }
  else
  {
    ParxRelsShowInEditor("RefPulse1Enum");
    ParxRelsShowInEditor("RefPulse1");
    ParxRelsShowInEditor("RefSpoiler");
  }


  /* update refocusing spoiler
     use the same spoiler for both pulses and calculate values
     according to minimum extension/limits of all directions */
  double mindur = CFG_GradientRiseTime() + CFG_InterGradientWaitTime();
  MRT_UpdateSpoiler("RefSpoiler",10.0,70.7,mindur,PVM_GradCalConst,MIN_OF(MIN_OF(PVM_VoxArrSize[0][0], PVM_VoxArrSize[0][1]),PVM_VoxArrSize[0][2]));

}

/*===============================================================
 * ExcPulse1EnumRelation
 * Relation of ExcPulse1Enum (a dynamic enmueration parameter which
 * allows to select one of the existing  pulses)
 *===============================================================*/

void RefPulse1EnumRelation(void)
{
  DB_MSG(("-->RefPulse1EnumRelation"));

  UT_SetRequest("RefPulse1Enum");
  backbone();
  
  DB_MSG(("<--RefPulse1EnumRelation"));                                       
}

/*===============================================================
 * ExcPulse1AmplRel
 * Relation of ExcPulseAmpl 
 * This parameter is used in the setup parameter card to adjust
 * the RF pulse amplitude manually
 *===============================================================*/

void RefPulse1AmplRel(void)
{
  DB_MSG(("-->RefPulse1AmplRel"));
  UT_SetRequest("RefPulse1Ampl");
  HandleRFPulseAmplitude();
  DB_MSG(("-->RefPulse1AmplRel"));
}


/* ===================================================================
 * Relation of ExcPulse
 * 
 * All pulses of type PVM_RF_PULSE must have relations like this.
 * However, if you clone this funtion for a different pulse parameter
 * remember to replace the param name in the call to UT_SetRequest!
 *
 * IMPORTANT: this function should not be invoked in the backbone!
 ====================================================================*/

void RefPulse1Relation(void)
{
  DB_MSG(("-->RefPulse1Relation"));
  
  /*
   * Tell the request handling system that the parameter
   * RefPulse has been edited 
   */
  
  UT_SetRequest("RefPulse1");
  
  /* Check the values of RefPulse */
  
  RefPulse1Range();
  
  /* 
   * call the backbone; further handling will take place there
   * (by means of STB_UpdateRFPulse)  
   */
  
  backbone();
  
  DB_MSG(("<--RefPulse1Relation"));
}

void RefPulse1Range(void)
{
  DB_MSG(("-->RefPulse1Range"));
  
  double dval=RefPulse1.Flipangle;
  
  RefPulse1.Flipangle = MIN_OF(270.0,MAX_OF(dval,1.0));

  DB_MSG(("<--RefPulseRange")); 
}

/* relation of refocusing spoiler */
void RefSpoilerRel(void)
{
  DB_MSG(("-->RefSpoilerRel"));
  UT_SetRequest("RefSpoiler");
  backbone();
  DB_MSG(("<--RefSpoilerRel"));
}
