/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002 - 2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2002 - 2003 Bruker BioSpin MRI GmbH";

#define DEBUG		0


#include "method.h"


/* ------------------------------------------------------------
 * backbone
 * The main part of method code. The consitency of all parameters
 * is checked here, relations between them are resolved and,
 * finally, functions setting the base level parameters are
 * called.
 */
void backbone( void )
{

  double minFov[1] = {1e-3};
  double minThickness = 0.5;


  DB_MSG(("-->backbone\n"));


  /*
   * Nucleus and  PVM_GradCalConst
   * are handled by this funtion:
   */


  STB_UpdateNuclei(Yes);


  /* handle RF pulse */

  UpdateRFPulses();


  /* sequence atoms: */

  /* spoilers */
  ReadSpoilerStrength =  MIN_OF(ReadSpoilerStrength, 100.0);
  ReadSpoilerStrength =  MAX_OF(ReadSpoilerStrength, 0.0);

  /* update encoding parameter group                           */

  STB_UpdateEncoding(
    NULL,
    SEG_INTERLEAVED,
    No,
    No,
    No);

  MRT_UpdateDigPars(&PVM_EffSWh,
                     PVM_EncMatrix[0],
                    &PVM_AntiAlias[0],
                    &PVM_AcquisitionTime);

  /* Update Geometry */
  LocalGeometryMinimaRels(minFov);

  int dim = PTB_GetSpatDim();
  STB_UpdateAtsReferencePosition("PVM_SliceGeoObj");

  STB_UpdateImageGeometry(dim,
                          PVM_Matrix,
                          minFov,
                          1,
                          1,
                          1,
                          minThickness,
                          1.0);


  LocalGradientStrengthRels();
  LocalFrequencyOffsetRels();

  PVM_NRepetitions = MAX_OF(1,PVM_NRepetitions);

  PVM_NEchoImages = 1;

  Local_NAveragesRange();

  repetitionTimeRels();

  /* set baselevel acquisition parameter */

  SetBaseLevelParam();

  /* set baselevel reconstruction parameter */

  SetRecoParam();


  DB_MSG(("<--backbone\n"));
}


void repetitionTimeRels( void )
{
  DB_MSG(("-->repetitionTimeRels\n"));

  double TotalTime = 0.0;

  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("POSITION-backbone: In function call!");
    return;
  }

  PVM_MinRepetitionTime =
      3.5 * ExcPulse1.Length      +
      1.5 * PVM_AcquisitionTime   +
      0.03                        + /*UpdateDynPars*/
      0.03                        + /*3x10u*/
      17 * CFG_GradientRiseTime() +
      CFG_InterGradientWaitTime();  /* min d0 */

  PVM_RepetitionTime = ( PVM_RepetitionTime < PVM_MinRepetitionTime ?
			 PVM_MinRepetitionTime : PVM_RepetitionTime );

  /* Calculate Total Scan Time and Set for Scan Editor */

  TotalTime = PVM_RepetitionTime                    *
              PVM_NAverages;
  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);


  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<-- repetitionTimeRels\n"));
}

void LocalGeometryMinimaRels(double *min_fov)
{

  DB_MSG(("-->LocalGeometryMinimaRels\n"));

      /*
       * Step #6
       * Calculate the minimum field of view in the read direction
       */

      min_fov[0] = MRT_MinReadFov( PVM_EffSWh,
      				   1.0,
      				   100.0,
      				   100.0,
      				   PVM_GradCalConst );


  DB_MSG(("<--LocalGeometryMinimaRels\n"));
}


void LocalGradientStrengthRels( void )
{

  DB_MSG(("-->LocalGradientStrengthRels\n"));

  /*
   * This function calculates all the gradient strengths
   */

  ReadGradient = MRT_ReadGrad( PVM_EffSWh, PVM_Fov[0], PVM_GradCalConst );

  GradientCurrent = ReadGradient;

  DB_MSG(("<--LocalGradientStrengthRels\n"));

}


void LocalFrequencyOffsetRels( void )
{

  DB_MSG(("-->LocalFrequencyOffsetRels\n"));



  /*
   * Calculate offset in read direction
   */

  MRT_FrequencyOffsetList(1,
              PVM_EffReadOffset,
              ReadGradient,
              PVM_GradCalConst,
              PVM_ReadOffsetHz );


  DB_MSG(("<--LocalFrequencyOffsetRels\n"));

}

/*--------------------------------------------------------
 * Routine to update RF pulse parameters
 *-------------------------------------------------------*/

void UpdateRFPulses(void)
{

  /* Updates all parameters that belong to ExcPulse1 pulse structure
     (as initialized by STB_InitRFPulse see initMeth.c)
  */

  STB_UpdateRFPulse("ExcPulse1",1,PVM_DeriveGains,Conventional);


  if(PVM_DeriveGains==Yes)
  {
    ParxRelsHideInEditor("ExcPulse1Ampl");
  }
  else
  {
    ParxRelsShowInEditor("ExcPulse1Ampl");
  }

  ParxRelsShowInFile("ExcPulse1Ampl");

  DB_MSG(("<--UpdateRFPulses"));

  return;
}

/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/








