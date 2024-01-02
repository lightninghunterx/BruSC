/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001-2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$ 
 *
 * ***************************************************************/

static const char resid[] = "$Id$ (C) 2006 Bruker BioSpin MRI GmbH";

#define DEBUG           0
#define DB_MODULE       1
#define DB_LINE_NR      0



#include "method.h"

/*----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
void FlowModeRel(void)
{
  UT_SetRequest("FlowMode");
  FlowModeRange();
  ParxRelsParRelations("FlowHandler",No);
}

void FlowModeRange(void)
{
  if( !ParxRelsParHasValue( "FlowMode" ) )
    FlowMode = PhaseContrastAngiography;
  switch(FlowMode)
  {
  default:
    FlowMode = PhaseContrastAngiography;
    break;
  case PhaseContrastAngiography:
    break;
  case VelocityMapping:
    break;
  case FourierFlowImaging:
    break;
  }   
}

void FlowTwisterRel(void)
{
  DB_MSG(("-->FlowTwisterRel()"));

  UT_SetRequest("FlowTwister");
  //FlowTwisterRange();
  ParxRelsParRelations("FlowHandler",No);

  DB_MSG(("-->FlowTwisterRel()"));

}

void FlowTwisterRange(void)
{
   if( ParxRelsParHasValue( "FlowTwister" ) == No )
       FlowTwister = Off;
     switch(FlowTwister)
     {
     default:
     FlowTwister = Off;
     break;
     case Off:
     break;
     case On:
     break;
     }

}

void FlowEncodingDirectionRel( void )
{
  UT_SetRequest("FlowEncodingDirection");
  FlowEncodingDirectionRange();
  ParxRelsParRelations("FlowHandler",No);
}


void FlowEncodingDirectionRange( void )
{
  DB_MSG(("--> FlowEncodingDirectionRange"));

  if( ParxRelsParHasValue( "FlowEncodingDirection" ) == No )
    FlowEncodingDirection = AllDirections;
        switch(FlowEncodingDirection)
     {
     default:
     FlowEncodingDirection = AllDirections;
     break;
     case AllDirections:
     break;
     case SliceDirection:
     break;
     case PhaseDirection:
     break;
     case ReadDirection:
     break;
     }


  DB_MSG(("<-- FlowEncodingDirectionRange"));
}



void FlowEncDirNumberRel( void )
{
  UT_SetRequest("FlowEncDirNumber");
  FlowEncDirNumberRange();
  ParxRelsParRelations("FlowHandler",No);
}


void FlowEncDirNumberRange( void )
{
  DB_MSG(("--> FlowEncDirNumberRange"));

  if( ParxRelsParHasValue( "FlowEncDirNumber" ) == No )
      FlowEncDirNumber = 1;

  if(FlowEncDirNumber < 1)
      FlowEncDirNumber =1;
  if(FlowEncDirNumber > 3)
      FlowEncDirNumber =3;


  DB_MSG(("<-- FlowEncDirNumberRange"));
}

void FlowRangeRel( void )
{
   UT_SetRequest("FlowRange");
   FlowRangeRange();
   ParxRelsParRelations("FlowHandler",No);


}


void FlowRangeRange(void)
{
   DB_MSG(("--> FlowRangeRange"));
   if( ParxRelsParHasValue("FlowRange") == No)
   FlowRange = 20.0;
   if(FlowRange < 0.0)
   FlowRange = 0.0;
   DB_MSG(("Exiting FlowRangeRange()"));
}

void FlowResolutionRel( void )
{
   UT_SetRequest("FlowResolution");
   FlowResolutionRange();
   ParxRelsParRelations("FlowHandler",No);


}


void FlowResolutionRange(void)
{
   DB_MSG(("--> FlowResolutionRange"));
   if( ParxRelsParHasValue("FlowResolution") == No)
   FlowResolution = 20.0;
   if(FlowResolution < 0.0)
   FlowResolution = 0.0;
   DB_MSG(("Exiting FlowResolutionRange()"));
}

void FlowMaxResolutionRel( void )
{
   UT_SetRequest("FlowMaxResolution");
   FlowMaxResolutionRange();
   ParxRelsParRelations("FlowHandler",No);


}


void FlowMaxResolutionRange(void)
{
   DB_MSG(("--> FlowMaxResolutionRange"));
   if( ParxRelsParHasValue("FlowMaxResolution") == No)
   FlowMaxResolution = 20.0;
   if(FlowMaxResolution < 0.0)
   FlowMaxResolution = 0.0;
   DB_MSG(("Exiting FlowMaxResolutionRange()"));
}

void MinFlowRangeRel( void )
{
   UT_SetRequest("MinFlowRange");
   MinFlowRangeRange();
   ParxRelsParRelations("FlowHandler",No);


}

void MinFlowRangeRange(void)
{
   DB_MSG(("--> MinFlowRangeRange"));
   if( ParxRelsParHasValue("MinFlowRange") == No)
   MinFlowRange = 0.0;
   if(MinFlowRange < 0.0)
   MinFlowRange = 0.0;
   DB_MSG(("Exiting MinFlowRangeRange()"));
}

void FlowZeroFillRel(void)
{
   DB_MSG(("--> FlowZeroFillRel"));
   FlowZeroFillRange();
   SetRecoParam();
   DB_MSG(("<-- FlowZeroFillRel"));
}

void FlowZeroFillRange(void)
{
   DB_MSG(("--> FlowZeroFillRange"));
  
   if( ParxRelsParHasValue("FlowZeroFillFactor") == No)
   {
     FlowZeroFillFactor = 1.0;
   }
   else
   {
     FlowZeroFillFactor = MIN_OF(MAX_OF(1.0,FlowZeroFillFactor),32);
   }
   
   DB_MSG(("<-- FlowZeroFillRange"));
}

void FlowTwisterGradientRel( void )
{
   UT_SetRequest("FlowTwisterGradient");
   FlowTwisterGradientRange();
   ParxRelsParRelations("FlowHandler",No);


}


void FlowTwisterGradientRange(void)
{
   DB_MSG(("--> FlowTwisterGradientRange"));
   if( ParxRelsParHasValue("FlowTwisterGradient") == No)
   FlowTwisterGradient = 0.0;
   if(FlowTwisterGradient < 0.0)
   FlowTwisterGradient = 0.0;
   if(FlowTwisterGradient > 999.0)
   FlowTwisterGradient = 999.0;
   DB_MSG(("Exiting FlowTwisterGradientRange()"));
}


void FlowEncStepsRel( void )
{
  UT_SetRequest("FlowEncSteps");
  FlowEncStepsRange();
  ParxRelsParRelations("FlowHandler",No);
}


void FlowEncStepsRange( void )
{
  DB_MSG(("--> FlowEncStepsRange"));

  if( ParxRelsParHasValue( "FlowEncSteps" ) == No )
  {
    FlowEncSteps = 1;
  }
  else
  {
    FlowEncSteps =MIN_OF(MAX_OF(1,FlowEncSteps),2048);
  }
  
  DB_MSG(("<-- FlowEncStepsRange"));
}

void InitFlow(double slicethick)
{
    if(ParxRelsParHasValue("FlowTwisterGradient") == No)
    FlowTwisterGradient = slicethick;
    FlowModeRange();
    FlowEncodingDirectionRange();
    FlowEncDirNumberRange();
    FlowRangeRange();
    FlowResolutionRange();
    FlowMaxResolutionRange();
    MinFlowRangeRange();
    FlowTwisterRange();
    FlowTwisterGradientRange();
    FlowEncStepsRange();
    FlowZeroFillRange();
    ParxRelsMakeNonEditable("MinFlowRange,FlowEncDirNumber,FlowEncGradMatrix"); 
    ParxRelsHideInEditor("FlowEncGradMatrix,FlowEncGradientAmp");
    ParxRelsHideInEditor("FlowSpatDim");
}    
 
void UpdateFlow(double slicethick,int spatdim, double encLobe)
/* ------------------------------------------------------------------------
   updates flow parameters
   encLobe is the duration of the bipolar flow encoding 
   (both lobes, in ms)

   ExSliceFcGrad, ReadFcGrad, PhaseFcGrad must be set before.
   ----------------------------------------------------------------------*/
{
  int i,j;
  double MaxFlowEncGrad = 0.0,diffgrad,phaseshift,gradcalconst,ramptime,igwtime;


  DB_MSG(("-->UpdateFlow: encLobe=%f", encLobe));

  ParxRelsHideInEditor("FlowResolution,FlowMaxResolution,FlowEncSteps");
  ParxRelsShowInEditor("FlowTwister");
  if(FlowTwister == On) ParxRelsShowInEditor("FlowTwisterGradient");
  if(FlowTwister == Off) ParxRelsHideInEditor("FlowTwisterGradient");
  ParxRelsShowInEditor("MinFlowRange,FlowEncLoop");
  ParxRelsMakeEditable("FlowRange");
  ParxRelsMakeNonEditable("FlowEncLoop");
  FlowSpatDim = PTB_GetSpatDim();
  gradcalconst = CFG_GradCalConst(PVM_Nucleus1); 
  ramptime = CFG_GradientRampTime();
  igwtime = 0; // not CFG_InterGradientWaitTime(), because we use shapes;



  // phase shift (for the flow encoding gradient)
  switch(FlowMode)
  {
    case VelocityMapping:
      if(FlowEncodingDirection == AllDirections)
        phaseshift = M_PI/4;  
      else
        phaseshift = M_PI/2;
      break;

    default:
      phaseshift = M_PI;
  }



  switch(FlowMode)
  {
    default:
      FlowMode = PhaseContrastAngiography;
    case PhaseContrastAngiography:
    case VelocityMapping:
      {
        
        switch(FlowEncodingDirection)
        {
          case SliceDirection:
            FlowEncDirNumber = 1;
            MaxFlowEncGrad = ExcSliceRephGradLim - fabs(ExcSliceFcGrad[0]);
            diffgrad = ExcSliceRephGradLim - fabs(ExcSliceFcGrad[1]) ;
            MaxFlowEncGrad = MIN_OF(MaxFlowEncGrad,diffgrad);
            MinFlowRange = MRT_FlowEncGradient(encLobe,MaxFlowEncGrad,phaseshift,ramptime,igwtime,gradcalconst);
            if(FlowRange < MinFlowRange) 
              FlowRange = MinFlowRange;
            FlowEncGradientAmp = MRT_FlowEncGradient(encLobe,FlowRange,phaseshift,ramptime,igwtime,gradcalconst);
            PARX_change_dims("FlowEncGradMatrix",2,3);
            FlowEncLoop = 2;
            for(i=0;i<2;i++)
            {
              for(j=0;j<3;j++)
                FlowEncGradMatrix[i][j]=0.0;                  
            }
            FlowEncGradMatrix[0][2]= 1.0;
            FlowEncGradMatrix[1][2]= -1.0;
            break;
          case PhaseDirection:
            FlowEncDirNumber = 1;
            MaxFlowEncGrad = Phase2DGradLim - fabs(Phase2DFcGrad[0]);
            diffgrad = Phase2DGradLim - fabs(Phase2DFcGrad[1]);
            MaxFlowEncGrad = MIN_OF(MaxFlowEncGrad,diffgrad);
            MinFlowRange = MRT_FlowEncGradient(encLobe,MaxFlowEncGrad,phaseshift,ramptime,igwtime,gradcalconst);
            if(FlowRange < MinFlowRange) FlowRange = MinFlowRange;
            FlowEncGradientAmp = MRT_FlowEncGradient(encLobe,FlowRange,phaseshift,ramptime,igwtime,gradcalconst);
            PARX_change_dims("FlowEncGradMatrix",2,3);
            FlowEncLoop = 2;
            for(i=0;i<2;i++)
            {
              for(j=0;j<3;j++)
                FlowEncGradMatrix[i][j]=0.0;
            }
            FlowEncGradMatrix[0][1]= 1.0;
            FlowEncGradMatrix[1][1]= -1.0;
            break;
          case ReadDirection:
            FlowEncDirNumber = 1;
            MaxFlowEncGrad = ReadDephGradLim - fabs(ReadFcGrad[0]);
            diffgrad = ReadDephGradLim - fabs(ReadFcGrad[1]);
            MaxFlowEncGrad = MIN_OF(MaxFlowEncGrad,diffgrad);
            MinFlowRange = MRT_FlowEncGradient(encLobe,MaxFlowEncGrad,phaseshift,ramptime,igwtime,gradcalconst);
            if(FlowRange < MinFlowRange) FlowRange = MinFlowRange;
            FlowEncGradientAmp = MRT_FlowEncGradient(encLobe,FlowRange,phaseshift,ramptime,igwtime,gradcalconst);
            PARX_change_dims("FlowEncGradMatrix",2,3);
            FlowEncLoop = 2;
            for(i=0;i<2;i++)
            {
              for(j=0;j<3;j++)
                FlowEncGradMatrix[i][j]=0.0;
            }
            FlowEncGradMatrix[0][0]= 1.0;
            FlowEncGradMatrix[1][0]= -1.0;
            break;
          case AllDirections:
            FlowEncDirNumber = 3;
            MaxFlowEncGrad = ExcSliceRephGradLim - fabs(ExcSliceFcGrad[0]);
            diffgrad = ExcSliceRephGradLim - fabs(ExcSliceFcGrad[1]);
            MaxFlowEncGrad = MIN_OF(MaxFlowEncGrad,diffgrad);
            diffgrad = ReadDephGradLim - fabs(ReadFcGrad[0]) ;
            MaxFlowEncGrad = MIN_OF(MaxFlowEncGrad,diffgrad);
            diffgrad = ReadDephGradLim - fabs(ReadFcGrad[1]) ;
            MaxFlowEncGrad = MIN_OF(MaxFlowEncGrad,diffgrad);
            diffgrad = Phase2DGradLim - fabs(Phase2DFcGrad[0]);
            MaxFlowEncGrad = MIN_OF(MaxFlowEncGrad,diffgrad);
            diffgrad = Phase2DGradLim - fabs(Phase2DFcGrad[0]) ;
            MaxFlowEncGrad = MIN_OF(MaxFlowEncGrad,diffgrad);
            MinFlowRange = MRT_FlowEncGradient(encLobe,MaxFlowEncGrad,phaseshift,ramptime,igwtime,gradcalconst);
            if(FlowRange < MinFlowRange) FlowRange = MinFlowRange;
            FlowEncGradientAmp = MRT_FlowEncGradient(encLobe,FlowRange,phaseshift,ramptime,igwtime,gradcalconst);

            PARX_change_dims("FlowEncGradMatrix",4,3);
            FlowEncLoop = 4;
            FlowEncGradMatrix[0][0]= -1.0;
            FlowEncGradMatrix[1][0]= 1.0;
            FlowEncGradMatrix[2][0]= 1.0;
            FlowEncGradMatrix[3][0]= -1.0;
         
            FlowEncGradMatrix[0][1]= -1.0;
            FlowEncGradMatrix[1][1]= 1.0;
            FlowEncGradMatrix[2][1]= -1.0;
            FlowEncGradMatrix[3][1]= 1.0;
         
            FlowEncGradMatrix[0][2]= -1.0;
            FlowEncGradMatrix[1][2]= -1.0;
            FlowEncGradMatrix[2][2]= 1.0;
            FlowEncGradMatrix[3][2]= 1.0;
            break;
        }

      }
      break;

    case FourierFlowImaging:
      {
        int dirNdx=0;     
        ParxRelsMakeNonEditable("FlowMaxResolution,FlowResolution,"
                                "FlowEncLoop");
        ParxRelsShowInEditor("FlowResolution,FlowMaxResolution,FlowEncSteps");
        ParxRelsHideInEditor("MinFlowRange,FlowTwister,FlowTwisterGradient,FlowEncLoop");
        FlowEncDirNumber = 1;
        if(FlowEncSteps < 8) FlowEncSteps = 8;
        FlowEncLoop = FlowEncSteps;
        PARX_change_dims("FlowEncGradMatrix",FlowEncLoop,3);
        for(i=0;i<FlowEncLoop;i++)
        {
          for(j=0;j<3;j++)
            FlowEncGradMatrix[i][j] = 0.0;
        }
        switch(FlowEncodingDirection)
        {
          case AllDirections:
            FlowEncodingDirection = SliceDirection;
          case SliceDirection:
            MaxFlowEncGrad = ExcSliceRephGradLim - fabs(ExcSliceFcGrad[0]);
            diffgrad = ExcSliceRephGradLim - fabs(ExcSliceFcGrad[1]);
            MaxFlowEncGrad = MIN_OF(MaxFlowEncGrad,diffgrad);
            dirNdx = 2;
            break;
          case ReadDirection:
            MaxFlowEncGrad = ReadDephGradLim - fabs(ReadFcGrad[0]) ;
            diffgrad = ReadDephGradLim - fabs(ReadFcGrad[1]) ;
            MaxFlowEncGrad = MIN_OF(MaxFlowEncGrad,diffgrad);
            dirNdx = 0;
            break;

          case PhaseDirection:
            MaxFlowEncGrad = Phase2DGradLim - fabs(Phase2DFcGrad[0]);
            diffgrad = Phase2DGradLim - fabs(Phase2DFcGrad[1]);
            MaxFlowEncGrad = MIN_OF(MaxFlowEncGrad,diffgrad);
            dirNdx = 1;
            break;
        }
        FlowResolution = 2.0*FlowRange/FlowEncLoop;
        FlowMaxResolution = MRT_FlowEncGradient(encLobe,MaxFlowEncGrad,phaseshift,
                                                ramptime,igwtime,gradcalconst);
        if(FlowResolution < FlowMaxResolution) 
          FlowResolution = FlowMaxResolution;
        FlowEncGradientAmp = MRT_FlowEncGradient(encLobe,FlowResolution,phaseshift,
                                                       ramptime,igwtime,gradcalconst);
        FlowRange = FlowResolution * FlowEncLoop/2;
        for(i=0;i<FlowEncLoop;i++)
          FlowEncGradMatrix[i][dirNdx]= -1.0 + 2.0 * i /FlowEncLoop;
       
      }
      break;
  }


  // number of output images
  if(FlowMode==VelocityMapping)
  {
    if(FlowEncDirNumber==3)
      FlowImages=3;
    else
      FlowImages=1;
  }
  else if(FlowMode==PhaseContrastAngiography)
    FlowImages=2;
  else
    FlowImages=1;

  // flow encoding gradient functions

  PARX_change_dims("FlowGradR",FlowEncLoop);
  PARX_change_dims("FlowGradP",FlowEncLoop);
  PARX_change_dims("FlowGradS",FlowEncLoop);
 
  for(i =0;i < FlowEncLoop ;i++)
  {
    
    FlowGradR[i] =  100*FlowEncGradMatrix[i][0];
    FlowGradP[i] =  100*FlowEncGradMatrix[i][1];
    FlowGradS[i] =  100*FlowEncGradMatrix[i][2];
  }

  DB_MSG(("twister 1"));

  if(FlowMode == FourierFlowImaging)
  {
    TwisterGradStrength = 0.0;
    ParxRelsHideInEditor("FlowTwister,FlowTwisterGradient");
  }
  else
  {
    if(FlowTwister == Off)
    {
      ParxRelsHideInEditor("FlowTwisterGradient");
      TwisterGradStrength = 0.0;
    }
    else
    {
      ParxRelsShowInEditor("FlowTwisterGradient");
      if( ParxRelsParHasValue( "FlowTwisterGradient" ) == No )
        FlowTwisterGradient = slicethick;
      if(FlowTwisterGradient > 999.0)
        FlowTwisterGradient = 999.0;
      if(FlowTwisterGradient < 1e-3)
        FlowTwisterGradient = 1e-3;

      MaxTwisterStrength = ExcSliceRephGradLim + TWISTER_MARGIN - fabs(ExcSliceFcGrad[0]) - fabs(FlowEncGradientAmp);
      diffgrad = ExcSliceRephGradLim + TWISTER_MARGIN - fabs(ExcSliceFcGrad[1]) - fabs(FlowEncGradientAmp); 
      MaxTwisterStrength = MIN_OF(MaxTwisterStrength,diffgrad);
      TwisterGradStrength = 1.0e5/CFG_GradCalConst(PVM_Nucleus1)/encLobe/FlowTwisterGradient;
      if(TwisterGradStrength > MaxTwisterStrength)
        TwisterGradStrength = MaxTwisterStrength; 
      FlowTwisterGradient = 1.0e5/CFG_GradCalConst(PVM_Nucleus1)/encLobe/TwisterGradStrength;
      DB_MSG(("twistgradstrngth %f", TwisterGradStrength));
    }
  }
  if(spatdim >= 3)
  {
    TwisterGradStrength=0;
    ParxRelsHideInEditor("FlowTwister,FlowTwisterGradient");
  } 
  
  DB_MSG(("<-- UpdateFlow"));
}



