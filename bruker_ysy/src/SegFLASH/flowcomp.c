/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2019
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2002 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0


/****************************************************************/
/****************************************************************/
/*		I N T E R F A C E   S E C T I O N		*/
/****************************************************************/
/****************************************************************/

/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "method.h"

double L1r=0.0, L2r=0.0, L1s=0.0, L2s=0.0;

void  FlowCompMinEncDur( double * encDur, double pulseDel, double echoPos,
                         double readGrad, double sliceGrad,
                         double readDephGradLim,
                         double phase2DGradLim,
                         double sliceRephGradLim )
/* ----------------------------------------------------------------*
   pulseDel : time from pulse centre to onset of falling slice ramp.
   min duration of encoding, read-deph and slice rephasing
   (flow compensated) counted from onset of first enc ramp
   to onset of last enc ramps, assuming equal durations of
   both lobes. PVM_AcquisitionTime and PVM_EchoPosition must
   be set before.
   ---------------------------------------------------------------*/
{
  DB_MSG(("-->FlowCompMinEncDur"));

  if (FlowCompOnOff == Off)
    return;

  int    dim           = PTB_GetSpatDim();
  double rise          = CFG_GradientRiseTime();
  double gcc           = PVM_GradCalConst;
  double *resol        = PVM_SpatResol;
  double encdur        = 4 * rise; // initial value, will be incremented later
  double increment     = 0.1;
  int    grad_exceeded = 0;
  double phsInt        = 0.0;             // integral in phase-dir
  double l1r, l2r, l1s, l2s, l1p, l2p;    // lobe durations on r/p/s
  double gval[2], gvalSl[2];

  encdur -= increment;

  do
  {
    encdur += increment;

    findLobes(encdur, pulseDel, echoPos, &l1r, &l2r, &l1p, &l2p, &l1s, &l2s);

    grad_exceeded = 0;

    // flow comp in read direction
    if (FlowCompDirection[0] == Yes)
    {
      MRT_FlowCompGradRead(rise, l1r, l2r, echoPos, readGrad, gval);

      for (int i=0; i<2; i++)
        if (fabs(gval[i]) > readDephGradLim)
          grad_exceeded = 1;
    }

    //flow comp in slice direction
    if (FlowCompDirection[2] == Yes)
    {
      MRT_FlowCompGradSlice(rise, l1s, l2s, pulseDel, sliceGrad, gvalSl );

      for (int i=0; i<2; i++)
        if (fabs(gvalSl[i]) > sliceRephGradLim)
          grad_exceeded = 1;
    }

    // flow comp in 1st phase direction
    if (FlowCompDirection[1] == Yes)
    {
      phsInt = 1e5 / (2*gcc*resol[1]);

      MRT_FlowCompGradPhase(rise, l1p, l2p, phsInt, gval);

      for (int i=0; i<2; i++)
        if (fabs(gval[i]) > phase2DGradLim)
          grad_exceeded = 1;
    }

    //flow comp in 2nd phase direction
    if(dim>2)
    {
      if (FlowCompDirection[1] == Yes)
      {
        phsInt = 1e5 / (2*gcc*resol[2]);

        MRT_FlowCompGradPhase(rise, l1p, l2p, phsInt, gval);

        for (int i=0; i<2; i++)
          if (fabs(gvalSl[i]+gval[i]) > sliceRephGradLim)
            grad_exceeded = 1;
      }
    }

  } while (grad_exceeded && encdur<200);

  *encDur = encdur;

  DB_MSG(("<--FlowCompMinEncDur"));
}



void UpdateFlowComp(double encDur, double effPulseDur, double echoPos,
                    double readGrad, double sliceGrad)
{
  DB_MSG(("-->UpdateFlowComp"));

    // UI visibility
    if (   FlowCompDirection[0]==No
        && FlowCompDirection[1]==No
        && FlowCompDirection[2]==No )
    {
      FlowCompOnOff=Off;
    }

    if (FlowCompOnOff)
    {
      ParxRelsShowInEditor("FlowCompDirection");
    }
    else
    {
      ParxRelsHideInEditor("FlowCompDirection");
      //in this case we need to switch one dir on again
      //otherwise they will not be visualized again
      FlowCompDirection[0] = Yes;
      FlowCompDirection[1] = Yes;
      FlowCompDirection[2] = Yes;
    }

    if (FlowCompOnOff == Off)
      return;

    double rise      = CFG_GradientRiseTime();
    double gradDwell = MAX_OF(CFG_GradientShapeResolution(),0.002);
    double *resol    = PVM_SpatResol;
    double gcc       = PVM_GradCalConst;
    double phaseInt  = 0.0;                     // phaseIntegral
    double l1r=0.0, l2r=0.0, l1s=0.0, l2s=0.0, l1p=0.0, l2p=0.0;// lobe durations on r/p/s

    int    dim       = PTB_GetSpatDim();
    int    shpsze    = floor ( EncGradDur/gradDwell );

    findLobes(encDur, effPulseDur, echoPos, &l1r, &l2r, &l1p, &l2p, &l1s, &l2s);

    if (FlowCompDirection[0] == Yes)
    {
      MRT_FlowCompGradRead(rise, l1r, l2r, echoPos, readGrad, ReadFcGrad);
    }
    else
    {
      ReadFcGrad[0] = 0.0;
      ReadFcGrad[1] = -MRT_DephaseGrad(l2r-rise, EchoDelay, rise, readGrad);
    }


    if (FlowCompDirection[2] == Yes)
    {
      MRT_FlowCompGradSlice(rise, l1s, l2s, effPulseDur, sliceGrad, ExcSliceFcGrad);
    }
    else
    {
      ExcSliceFcGrad[0] = -MRT_DephaseGrad( l1s-rise, effPulseDur, rise, sliceGrad ) ;
      ExcSliceFcGrad[1] = 0.0;
    }


    if (FlowCompDirection[1] == Yes)
    {
      if(dim>1)
      {
        phaseInt = 1e5/( 2*gcc*resol[1] );
        MRT_FlowCompGradPhase(rise, l1p, l2p, phaseInt, Phase2DFcGrad);
      }

      if(dim>2)
      {
        phaseInt = 1e5/( 2*gcc*resol[2] );
        MRT_FlowCompGradPhase(rise, l1p, l2p, phaseInt, Phase3DFcGrad);
      }
      else
      {
        memset( Phase3DFcGrad,0,2*sizeof(double));
      }
    }
    else
    {
      if(dim>1)
      {
        Phase2DFcGrad[0] = 0.0;
        Phase2DFcGrad[1] = dim > 1 ?
          MRT_PhaseGrad( l2p-rise, PVM_Matrix[1], PVM_Fov[1], gcc ) : 0.0;
      }

      if(dim>2)
      {
        Phase3DFcGrad[0] = 0.0;
        Phase3DFcGrad[1] = dim == 3 ?
          -MRT_PhaseGrad( l2p-rise, PVM_Matrix[2], PVM_Fov[2], gcc ) : 0.0;
      }
      else
      {
        memset( Phase3DFcGrad,0,2*sizeof(double));
      }
    }

    // encoding shapes

    if ( TeFillDelay > EncGradDur )  //long enough to place 2 shapes
    {
      ShapeSize    = shpsze;

      RdVar = EncGradDur - gradDwell*ShapeSize;

      TeFillDelay -= EncGradDur ;

      FlowCompLongTEScan = Yes;
    }
    else
    {
      ShapeSize  = floor ( (EncGradDur + TeFillDelay) / gradDwell );

      RdVar = EncGradDur + TeFillDelay - gradDwell*ShapeSize;

      FlowCompLongTEScan = No;
    }

    if(shpsze<10)
    {
      char msg[200];
      sprintf(msg, "SegFLASH: min grad dwell too high (%f us)", 1000*gradDwell);
      UT_ReportError(msg);
    }

    PARX_change_dims("RShape" , ShapeSize);
    PARX_change_dims("P2Shape", ShapeSize);
    PARX_change_dims("P3Shape", ShapeSize);
    PARX_change_dims("SShape" , ShapeSize);

    memset(RShape , 0.0, ShapeSize*sizeof(double));
    memset(P2Shape, 0.0, ShapeSize*sizeof(double));
    memset(P3Shape, 0.0, ShapeSize*sizeof(double));
    memset(SShape , 0.0, ShapeSize*sizeof(double));

    double rshape[shpsze] ={0.},   p2shape[shpsze]={0.};
    double sshape[shpsze] ={0.},   p3shape[shpsze]={0.};

    L1r=l1r; L2r=l2r; L1s=l1s; L2s=l2s;

    makeShape( rshape, shpsze, rise, l1r, l2r, ReadFcGrad[0]/100,     ReadFcGrad[1]/100  );
    makeShape(p2shape, shpsze, rise, l1p, l2p, Phase2DFcGrad[0]/100,  Phase2DFcGrad[1]/100 );
    makeShape(p3shape, shpsze, rise, l1p, l2p, Phase3DFcGrad[0]/100,  Phase3DFcGrad[1]/100 );
    makeShape( sshape, shpsze, rise, l1s, l2s, ExcSliceFcGrad[0]/100, ExcSliceFcGrad[1]/100);

    std::copy( sshape,  sshape + shpsze, SShape);
    std::copy(p2shape, p2shape + shpsze, P2Shape + ShapeSize - shpsze);
    std::copy(p3shape, p3shape + shpsze, P3Shape + ShapeSize - shpsze);
    std::copy( rshape,  rshape + shpsze,  RShape + ShapeSize - shpsze);

  DB_MSG(("<--UpdateFlowComp"));
}



void findLobes(double encDur, double pulseDel, double echoPos,
               double *l1r,   double *l2r,
               double *l1p,   double *l2p,
               double *l1s,   double *l2s )
{
  // divide encDur to lobes (r,p,s) in the optimal, feasible way
  double rise   = CFG_GradientRiseTime();

  if (FlowCompDirection[0] == Yes)
    MRT_FlowCompTimingRead(rise, encDur, echoPos, l1r, l2r);
  else
    *l2r = encDur-2*rise;

  if (FlowCompDirection[1] == Yes)
    MRT_FlowCompTimingPhase(rise, encDur, l1p, l2p);
  else
    *l2p = encDur-2*rise;

  if (FlowCompDirection[2] == Yes)
    MRT_FlowCompTimingSlice(rise, encDur, pulseDel, l1s, l2s);
  else
    *l1s = encDur-2*rise;

  *l1r = encDur - *l2r;
  *l1p = encDur - *l2p;
  *l2s = encDur - *l1s;
}


void makeShape(double *buf, int n, double ramp, double lobe1, double lobe2,
               double amp1, double amp2)
{
  double t1, t2, t3, t4, t5, tot = lobe1+lobe2;

  t1 = ramp;
  t2 = lobe1-ramp;
  t3 = lobe1;
  t4 = lobe1+ramp;
  t5 = tot-ramp;

  MRT_MakeRamp(buf,n,tot, 0,  t1,   0,     amp1, ramp_lin);
  MRT_MakeRamp(buf,n,tot, t1, t2,   amp1,  amp1, ramp_lin);
  MRT_MakeRamp(buf,n,tot, t2, t3,   amp1,  0,    ramp_lin);
  MRT_MakeRamp(buf,n,tot, t3, t4,   0,     amp2, ramp_lin);
  MRT_MakeRamp(buf,n,tot, t4, t5,   amp2,  amp2, ramp_lin);
  MRT_MakeRamp(buf,n,tot, t5, tot,  amp2,  0,    ramp_lin);
}
