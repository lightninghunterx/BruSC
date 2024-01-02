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

void SetLocalizationParameters(void)
{

  int s012[3] = {0,1,2};
  double voxFreq[3];

  ATB_SetRFPulse("RefPulse1","ACQ_RfShapes[1]","P[1]");

  RefSpAmp = RefSpoiler.ampl;
  RefSpDur = RefSpoiler.dur/1000.0;

  switch (LocalizationMode)
  {
    case Voxel_PRESS:
      //Since the voxel dependency ensures equal orientation of voxel of imaging sclice, the native gradient values for the voxel can be used. They are distributed to the physical gradient channels according to the GradMatrix which is set according to the imaging slice.

      RefGrad1 = PVM_VoxArrGradient[0][0];
      RefGrad2 = PVM_VoxArrGradient[0][1];
 
      /*Frequency for VoxelSelection*/
      voxFreq[0]=PVM_VoxArrFrequency[0][2];
      voxFreq[1]=PVM_VoxArrFrequency[0][0];
      voxFreq[2]=PVM_VoxArrFrequency[0][1];
      ATB_SetAcqO1List( 3,            
                        s012,
                        voxFreq );
      break;

    case Slice_SE:
      RefGrad1=MRT_SliceGrad(RefPulse1.Bandwidth,
                             PVM_SliceThick,
                             PVM_GradCalConst);
      RefGrad2=0.0;

      /* Calculate frequency for refocusing pulse */
      MRT_FrequencyOffsetList( NSLICES,
                               PVM_EffSliceOffset,
                               RefGrad1,
                               PVM_GradCalConst,
                               PVM_SliceOffsetHz);
      ATB_SetAcqO2List( NSLICES,
                        PVM_ObjOrderList,
                        PVM_SliceOffsetHz );
      break;

    case Slice_FID:
      RefGrad1=0.0;
      RefGrad2=0.0;
      break;
  }

switch LocalizationMode
 {
   case Slice_FID:
     FillTE1_1=(PVM_EchoTime - MinTE1_1)/1000.0;
     FillTE1_2=FillTE2_1=FillTE2_2=1;
     break;
   case Slice_SE:
     FillTE1_1=(PVM_EchoTime/2.0 - MinTE1_1)/1000.0;
     FillTE1_2=(PVM_EchoTime/2.0 - MinTE1_2)/1000.0;
     FillTE2_1=FillTE2_2=1;
     break;
   case Voxel_PRESS:
     double te1=2.0*MAX_OF(MinTE1_1,MinTE1_2); //use minimum for first echo time
     double te2=PVM_EchoTime-te1;

     FillTE1_1=(te1/2.0-MinTE1_1)/1000.0;
     FillTE1_2=(te1/2.0-MinTE1_2)/1000.0;
     FillTE2_1=(te2/2.0-MinTE2_1)/1000.0;
     FillTE2_2=(te2/2.0-MinTE2_2)/1000.0;
     break;
 }

 double eps=1e-10; //0.1ns
 if (fabs(FillTE1_1) < eps) FillTE1_1=0.0;
 if (fabs(FillTE1_2) < eps) FillTE1_2=0.0;
 if (fabs(FillTE2_1) < eps) FillTE2_1=0.0;
 if (fabs(FillTE2_2) < eps) FillTE2_2=0.0;


}

