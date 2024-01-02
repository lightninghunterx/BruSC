/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999-2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2003 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	0

/*:=INFO=:*******************************************************
 *
 * Description :
 *   it contains functions necessary for responding to a 'load'
 *	of this method. This function will be automatically
 *	called in response to a method file for this method
 *	having been read.
 *
 *::=info=:******************************************************/

/****************************************************************/
/****************************************************************/
/*		I N T E R F A C E   S E C T I O N		*/
/****************************************************************/
/****************************************************************/

/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "method.h"


/****************************************************************/
/*	I M P L E M E N T A T I O N   S E C T I O N		*/
/****************************************************************/

/****************************************************************/
/*		G L O B A L   F U N C T I O N S			*/
/****************************************************************/


/* ------------------------------------------------------------ 
  backbone 
  The main part of method code. The consitency of all parameters is checked
  chere, relations between them are resolved and, finally, functions setting
  the base level parameters are called.
  --------------------------------------------------------------*/
void backbone( void )
{
  int dim, ret, nImagesPerRep;
  double minFov[3] = {1e-3, 1e-3, 1e-3},
         minThickness;
  double limExSliceGradient = 100;
  double limExSliceRephaseGradient = 50;

  
  
  DB_MSG(("Entering EPI/backbone.c:backbone"));


  /* Nuclei and  PVM_GradCalConst  are handled by this funtion: */
  STB_UpdateNuclei(Yes);
 
 
  /* do not allow a-aliasing */
  PVM_AntiAlias[1] = 1.0;

  /* Encoding (used in EPSI for spatial dimentions)*/
  STB_UpdateEncoding(
    NULL,
    SEG_SEQUENTIAL,
    No,
    No,
    No);

  /* size and segments for spectral dimension: */
  SpecSizeRange(); //reset to minimum first, since following int conversion may fall below minimum
  SpecSize = ((int)(0.5+SpecSize/NSegments))*NSegments;
  dim = PTB_GetSpatDim();
  

  /* handle RF pulse */   
  
  STB_UpdateRFPulse("ExcPul",1,PVM_DeriveGains,Conventional);
  STB_UpdateRFPulse("RefPul",1,PVM_DeriveGains,Conventional);
  
  if(PVM_DeriveGains==Yes)
  {
    ParxRelsHideInEditor("ExcPulseAmpl");
  }
  else
  {
    ParxRelsShowInEditor("ExcPulseAmpl");
  }
  
  if (PVM_SignalType == SignalType_Fid)
  {
    ParxRelsHideInEditor("RefPul, RefPulseEnum, RefPulseAmpl");
  }
  else
  {
    ParxRelsShowInEditor("RefPul, RefPulseEnum");
    if (PVM_DeriveGains == Yes)
    {
      ParxRelsHideInEditor("RefPulseAmpl");
    }
    else
    {
      ParxRelsShowInEditor("RefPulseAmpl");
    }
  }
    
  /*** Update Geometry **/
  
  LocalGeometryMinimaRels(limExSliceGradient, limExSliceRephaseGradient, &minThickness);

  /* do not allow isotropic geometry */
  PVM_IsotropicFovRes = Isot_None;

  /* number of slices:
     - one slice if 3D
     - only one package for EPSI since different
       orientations / transpositions in combination with
       spectral dim cannot be handled by reco.
     - multi slice allowed for traj adj.
   */

  int maxPackages = 1;
  int maxPerPackage = 0;
  if (dim>2 && PVM_EpiAdjustMode != 3)
  {
    maxPerPackage=1;
  }

  STB_UpdateImageGeometry(dim, 
                          PVM_Matrix,
                          minFov,
                          0, // total slices (no restr)
                          maxPackages,
                          maxPerPackage,
                          minThickness,
                          1.0); // sliceFovRatio in 3D



  /** Update EPI parameter group */

  PVM_NRepetitions = MAX_OF(1,PVM_NRepetitions);
  PVM_NEchoImages = 1;
  nImagesPerRep = PVM_NEchoImages * GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  int epsiMatrix[4];

  epsiMatrix[0] = PVM_EncMatrix[0];
  epsiMatrix[1] = SpecSize;
  if(dim>1)
    epsiMatrix[2] = PVM_EncMatrix[1];
  if(dim>2)
    epsiMatrix[3] = PVM_EncMatrix[2];
 
  PVM_EpiCombine = No; // currently not supported by EPSI
  ParxRelsHideInEditor("PVM_EpiCombine");

  ret = STB_EpiUpdate(Yes,  //epsi mode
                      dim, 
                      epsiMatrix, 
                      PVM_AntiAlias,
                      PVM_Fov, 
                      minFov, 
                      &PVM_EffSWh, 
                      PVM_GradCalConst, 
                      nImagesPerRep,
                      NSegments,
                      1,    // 'central step' not relevant (no blips)
                      1.0,  // no ppi acceleration
                      PVM_EncNReceivers);
  
  if(ret <0)
    DB_MSG(("--!!!!!!!!!!!!!!! Illegal arguments for STB_UpdateEPI: EPI Module not ready !"));
 
  DB_MSG(("minFov: %f %f %f", minFov[0], minFov[1], minFov[2]));



  STB_EpiConstrainGeometry((PVM_Fov[0] / 2)*(PVM_AntiAlias[0] - 1.0)); /* special for EPI */
  /* Update geometry again (geometry adapted to read offset) */
  STB_UpdateImageGeometry(dim, 
                          PVM_Matrix,
                          minFov,
                          0, // total slices (no restr)
                          maxPackages,
                          maxPerPackage,
                          minThickness,
                          1.0); // sliceFovRatio in 3D


                             
  /*** end Update Geometry **/


  /* spectroscopic parameters */
  SpecBand = PVM_EpiEffBandwidth;
  SpecResol = SpecBand/SpecSize;
  SpecBandPpm = SpecBand/PVM_FrqWork[0];

  /* gradients */
  LocalGradientStrengthRels();  
  LocalFrequencyOffsetRels();

  if (ParxRelsParHasValue("PVM_NAverages") == 0)
     PVM_NAverages = 1;

  /* update slice spoiler */
  double mindurSlice = 2.0*CFG_GradientRiseTime();
  double spoilerThick = dim>2 ? PVM_SpatResol[2]:PVM_SliceThick;
  MRT_UpdateSpoiler("SliceSpoiler",1.0,limExSliceGradient,mindurSlice,PVM_GradCalConst,spoilerThick);

  /* handling of modules */
  spoilerThick = dim>1? MIN_OF(PVM_SpatResol[0],PVM_SpatResol[1]):PVM_SpatResol[0];
  STB_UpdateFatSupModule(PVM_Nucleus1, PVM_DeriveGains, spoilerThick);
  STB_UpdateSatTransModule(PVM_DeriveGains);
  STB_UpdateFovSatModule(PVM_Nucleus1, PVM_DeriveGains);
  STB_UpdateTriggerModule();
  STB_UpdateTriggerOutModule();
  STB_UpdateTaggingModule(dim,PVM_Fov,PVM_Matrix,PVM_SpatResol[0],PVM_DeriveGains); 

  rfcSpoilerUpdate();

  echoTimeRels();

  repetitionTimeRels();

  PTB_ClearAdjustments();

  double echoPos=0.0; //take navigator signal for ArrayPhaseAdj

  STB_ArrayPhaseAppendAdjustment(PVM_EncNReceivers, No, echoPos, &PVM_SliceGeo[0], PARX_get_dim("PVM_SliceGeo",1));

  PTB_AppendOrderAdjustment(per_scan, per_scan, RG_ADJNAME);

  PTB_AppendAdjustment("EpiTraj",
                       "Adjust Traj.",
                       "Adjust EPI Trajectory",
                       on_demand);

  if(PVM_EncPpi[1] > 1)
  {
    PTB_AppendAdjustment("EpiGrappa",
                         "Adjust GRAPPA Coeff.",
                         "Adjustment of GRAPPA Coefficients",
                         per_scan);
  }

  STB_UpdateDummyScans(PVM_RepetitionTime);
  
  /* set baselevel acquisition parameter */
  SetBaseLevelParam();

  STB_UpdateMapShim(PVM_Nucleus1,"PVM_SliceGeoObj");
  
  /* set baselevel reconstruction parameter */
  SetRecoParam(PVM_EncPpi[1]);


  /* adapt size of trajectory arrays if necessary and set 
     PVM_EpiTrajAdj to No if a trajectory relevant parameter has changed */

  STB_EpiCheckTrajectory(PVM_Fov[0],PVM_EffSWh,PVM_SPackArrGradOrient[0][0]);
 
  DB_MSG(("Exiting EPI/backbone.c:backbone"));
}


void rfcSpoilerUpdate(void)
{
  RfcSpoilerStrength = 2*ExSliceGradient;
  RfcSpoilerStrength = MIN_OF(RfcSpoilerStrength,80);
  RfcSpoilerStrength = MAX_OF(RfcSpoilerStrength,30);

  RfcSpoilerDuration = MAX_OF(RefPul.Length, 2*CFG_GradientRiseTime());
}

/****************************************************************/
/*	         L O C A L   F U N C T I O N S			*/
/****************************************************************/


void echoTimeRels( void )
{
  DB_MSG(("Entering EPI/backbone.c:echoTimeRels()"));

  double igwt = CFG_InterGradientWaitTime();
  double riseT = CFG_GradientRiseTime();

  if(PVM_SignalType == SignalType_Fid)
  {
    PVM_MinEchoTime =      /* min gradient echo time */
      ExcPul.Length * ExcPul.Rpfac/100 +
      riseT                      +
      igwt                       +
      RephaseTime                +
      igwt                       +
      PVM_EpiEchoDelay;
    MinTE_right  = MinTE_left = 0.0; /* not used */
  }
  else
  {
    MinTE_left =  /* min half spinecho-time given by left hand side of pi */
    ExcPul.Length * ExcPul.Rpfac/100  +
    riseT                      +
    igwt                       + 
    RephaseTime                +
    igwt                       +
    RfcSpoilerDuration         + 
    RefPul.Length/2.0;

    MinTE_right = /* min half spinecho-time given by right hand side of pi */
    RefPul.Length/2.0 +
    RfcSpoilerDuration +
    igwt               +
    PVM_EpiEchoDelay;

    PVM_MinEchoTime = 2 * MAX_OF(MinTE_left, MinTE_right);
  }

  EchoTime = EchoTime < PVM_MinEchoTime ?
    PVM_MinEchoTime : EchoTime;

  
  /* Set Echo Parameters for Scan Editor   */
  PVM_EchoTime = PVM_Matrix[0]*1000.0/(PVM_EffSWh * PVM_AntiAlias[0]);  /* echo spacing */
  
  PVM_EchoPosition = 50.0;
 
  DB_MSG(("Exiting EPI/backbone.c:echoTimeRels"));
}

void repetitionTimeRels( void )
{
  int nSlices,dim;
  double TotalTime,trigger, trigger_v, trigOutSlice, trigOutVol;

  DB_MSG(("--> minRepetitionTimeRels"));

  trigger = STB_UpdateTriggerModule();
  if(PVM_TriggerMode == per_PhaseStep) /* per volume */
  {
    trigger_v=trigger;
    trigger=0.0; 
  }
  else trigger_v=0.0;

  TotalTime = 0.0;
  nSlices = GTB_NumberOfSlices( PVM_NSPacks, PVM_SPackArrNSlices );

  if(PVM_TriggerOutOnOff == On)
  {
    switch(PVM_TriggerOutMode)
    {
    case PER_SLICE:   
      trigOutSlice = PVM_TriggerOutModuleTime;
      trigOutVol = 0.0;
      break;

    case PER_VOLUME: 
      trigOutSlice = 0.0;
      trigOutVol = PVM_TriggerOutModuleTime;
      break;

    case AT_START:
    default:
      trigOutSlice = 0.0;
      trigOutVol = 0.0;
      
    }
  }
  else
    trigOutSlice = trigOutVol = 0.0;

  dim = PTB_GetSpatDim();
  
  if(dim>2) /* disable inter-volume delay in 3d */
  {
    PackDel=0;
    ParxRelsMakeNonEditable("PackDel");
  }
  else
  {
    ParxRelsMakeEditable("PackDel");
  }


  double dynshim_event=0;
  if(PVM_DynamicShimEnable==Yes)
    dynshim_event = PVM_DynamicShimEventDuration;

  PVM_MinRepetitionTime =
    nSlices * (0.03 + //UpdateDynPars
               dynshim_event +
               0.01 + 
               PVM_FatSupModuleTime +
               PVM_SatTransModuleTime +
               PVM_FovSatModuleTime +
               trigger +
               trigOutSlice +
               SliceSpoiler.dur +
               CFG_GradientRiseTime() +
               ExcPul.Length/2 +
               EchoTime +
               PVM_EpiModuleTime - PVM_EpiEchoDelay) + 
    PVM_TaggingModuleTime +
    trigOutVol +
    trigger_v +
    PackDel;

  PVM_RepetitionTime = ( PVM_RepetitionTime < PVM_MinRepetitionTime ? 
			 PVM_MinRepetitionTime : PVM_RepetitionTime );
  
  /** Calculate Total Scan Time and Set for Scan Editor **/ 
  TotalTime = PVM_RepetitionTime      *
              PVM_EpiNShots           *
              PVM_NAverages           *
              PVM_SatTransRepetitions *
              PVM_NRepetitions;
  if( dim >1 )
    TotalTime*=PVM_EncMatrix[1];
  if( dim >2 )
    TotalTime *= PVM_EncMatrix[2];

  /* time for one repetition */
  OneRepTime = TotalTime/(PVM_NRepetitions*1000.0);

  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
 

  ParxRelsShowInEditor("PVM_ScanTimeStr");
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<-- repetitionTimeRels"));
}

void LocalGeometryMinimaRels(double limExSliceGradient, double limExSliceRephaseGradient, double *min_thickness )
{
  /*
    This function calculates the minima for the minimum  slice thickness.
    It is always assumed that all slices have the same thickness
    (WE DO NOT set min_fov[0 and 1] here, this will be done by the epi module) 
 */

  double sliceRampInteg; /* normalised integral falling slice gradient ramp */
  double sliceRephInteg; /* normalised integral slice rephase gradient      */
    
  /* min slice thickness: */
  /*  Calculate the normalised integral of the descending gradient ramp after
      the RF pulse */
  sliceRampInteg = 0.5 *  CFG_GradientRiseTime();
  /* Calculate the normalised integral of the slice selection rephasing
     gradient */
  sliceRephInteg = RephaseTime - CFG_GradientRiseTime();
  
  /*
	Calculate the ratio of the strength of the slice selection gradient to
	the strength of the slice selection rephase gradient

  */
  SliceGradRatio = MRT_SliceGradRatio( ExcPul.Length,
                                       ExcPul.Rpfac,
                                       0.0,
                                       sliceRampInteg,
                                       sliceRephInteg );
  /*
    Calculate the minimum slice thickness
  */
      
  *min_thickness = MRT_MinSliceThickness( ExcPul.Bandwidth,
					  SliceGradRatio,
					  limExSliceGradient,
					  limExSliceRephaseGradient,
					  PVM_GradCalConst );
     
    
} /* end of localGeometryMinima() */



void LocalGradientStrengthRels( void )
{
  /*
    This function calculates all the gradient strengths 
  */

  switch( PTB_GetSpatDim() )
    {
    case 3: /* PHASE ENCODING GRADIENT - 3nd DIM */
     
     /* falls through */
    case 1:
      ReadGradient = (PVM_EpiReadEvenGrad+PVM_EpiReadOddGrad)/2.0; /* used in LocFreqOff */
     /* falls through */
    default: /* SLICE SELECTION GRADIENT */
      ExSliceGradient = MRT_SliceGrad( ExcPul.Bandwidth,
				       PVM_SliceThick,
				       PVM_GradCalConst );
      ExSliceRephaseGradient = MRT_SliceRephaseGrad( SliceGradRatio,
							 ExSliceGradient );
      break;
    }
}

void LocalFrequencyOffsetRels( void )
{
  int spatDim;
  int i,nslices;

  spatDim = PTB_GetSpatDim();

  nslices = GTB_NumberOfSlices(PVM_NSPacks,PVM_SPackArrNSlices);

  /* set ReadOffsetHz to zero. In EPI the fov offset is made by reco, not by detection freq. offsets */
  MRT_FrequencyOffsetList(nslices,
			  PVM_EffReadOffset,
			  ReadGradient,
			  0.0, /* instead PVM_GradCalConst; this sets offHz to zero */
			  PVM_ReadOffsetHz );

  MRT_FrequencyOffsetList(nslices,
			  PVM_EffSliceOffset,
			  ExSliceGradient,
			  PVM_GradCalConst,
			  PVM_SliceOffsetHz );

  if(spatDim == 3)
  {
    for(i=0;i<nslices;i++)
      PVM_EffPhase2Offset[i] = -PVM_EffSliceOffset[i];
  }


}



/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
