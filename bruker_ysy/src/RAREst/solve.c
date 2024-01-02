/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2007-2010
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id:";


#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"



/* =============================================================================================
   This function solves the section from the 90deg pulse to the first 180 deg pulse:
   - Constrains spoilRead, spoilSlice
   - Increases echoSpacing if needed.
   - Calculates gradients between 90 and 180: 
       excSliceGrad, refSliceGrad, sliceDephGrad, readDephGrad
   Returns: 0 - echoSpacing accepted, 1 - echoSpacing increased
   ==============================================================================================*/

int solve90_180( double *echoSpacing,                          /* required(in)/possible(out) in ms */
                 double *spoilRead,    double *spoilSlice,     /* req/poss in cycles/mm */
                 double *excSliceGrad, double *refSliceGrad,   /* (out) 90/180 slice grad, in % of max */
                 PVM_RF_PULSE *excPulse, PVM_RF_PULSE *refPulse, 
                 double stabTime,                              /* extra delay before ref pulse, after
                                                                  the rising grad ramp */
                 double *readDephGrad, double *sliceDephGrad,  /* (out) in % of max) */
                 double gradCalConst,                          /* max grad Hz/mm */
                 double sliceThick,    double /* resolRead */,       /* mm */
		 double bandWidthScaling)                      /* bw correction in % */

{
  double minSpoilSlice, pulseBandWidth,
         gradMax_kHzMm, excSliceGrad_kHzMm, refSliceGrad_kHzMm,
         excSliceDur, refSliceDur, minDurRead, minDurSlice, minEchoSpacing, durDeph;
  int ret;

  /* effective gradient durations (in ms) */
  excSliceDur = excPulse->Length*excPulse->Rpfac/100 + CFG_GradientRiseTime()/2;
  refSliceDur = refPulse->Length*0.5 + stabTime + CFG_GradientRiseTime()/2;

  /* gradients (percent to max) */
  pulseBandWidth = excPulse->Bandwidth * bandWidthScaling/100.0;
  *excSliceGrad = MRT_SliceGrad( pulseBandWidth, sliceThick, gradCalConst );
  *refSliceGrad = *excSliceGrad;

  /* max grad in kHz/mm (when multiplied by delay in ms, gives cycles/mm) */
  gradMax_kHzMm = gradCalConst * 1e-3;

  /* gradients in kHz/mm */
  excSliceGrad_kHzMm = *excSliceGrad * 0.01 * gradMax_kHzMm;
  refSliceGrad_kHzMm = *refSliceGrad * 0.01 * gradMax_kHzMm;

  /* min slice spoiler (in cycles/mm) correponds to the slice gradient 
     being switched off between the 90 and 180 pulses... */
  minSpoilSlice = (  excSliceDur*excSliceGrad_kHzMm + refSliceDur*refSliceGrad_kHzMm ); 
  /* but we take a bit less than that as a limit: */
  *spoilSlice = MAX_OF(0.9*minSpoilSlice,*spoilSlice);  

  /* min read spoiler: Value of 1cycle/resol would bring the FID signals
     to k-space edge, so 1.2 should be sufficient ... but we allow down to 0 (true-fisp case) */

  *spoilRead = MAX_OF(0.0,*spoilRead);  

  /* min duration of the read dephasing */
  minDurRead =  *spoilRead / (0.7*gradMax_kHzMm); /* 0.7 = 1/sqrt(2) beacuse slice+read ar simult. */

  /* min duration of the slice dephasing */
  minDurSlice =  fabs(*spoilSlice-minSpoilSlice)/ (0.7*gradMax_kHzMm); /* 0.7, see above*/
  minDurSlice = MAX_OF(CFG_GradientRiseTime(), minDurSlice);
  DB_MSG(("minDurR=%f, minDurS=%f", minDurRead, minDurSlice));

  /* constrain echoSpacing (ms) */
  minEchoSpacing = 2 * (MAX_OF(minDurRead,minDurSlice) + excSliceDur + refSliceDur);
  if(*echoSpacing < minEchoSpacing)
  {
    *echoSpacing = minEchoSpacing;
    ret = 1;
  }
  else
  {
    ret = 0;
  }  
  
  /* for this spacing, calculate dephasing gradients */
  durDeph  = *echoSpacing/2 - excSliceDur - refSliceDur;
  DB_MSG(("durDeph=%f, esp=%f", durDeph, *echoSpacing));
  *readDephGrad  = 100*(*spoilRead) / (durDeph*gradMax_kHzMm);
  *sliceDephGrad = 100*(*spoilSlice-minSpoilSlice) / (durDeph*gradMax_kHzMm);
  DB_MSG(("readDephGrad=%f, sliceDephGrad=%f", *readDephGrad, *sliceDephGrad));
  return ret;
}


