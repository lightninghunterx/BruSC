/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/
static const char resid[] = "$Id$ (C) 2002 - 2009 Bruker BioSpin MRI GmbH";

#define DEBUG 0


/****************************************************************/
/****************************************************************/
/*		I N T E R F A C E   S E C T I O N		*/
/****************************************************************/


/****************************************************************/

/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "method.h"



void SetBaseLevelParam( void )
{

  DB_MSG(("Entering SetBaseLevelParam()"));

  SetBasicParameters();

  SetFrequencyParameters();

  SetPpgParameters();

  SetAcquisitionParameters();

  SetGradientParameters();

  SetInfoParameters();

  ATB_SetReceiverGains();
#if DEBUG
  printTiming();
#endif

  DB_MSG(("Exiting SetBaseLevelParam"));

}


void SetBasicParameters( void )
{
  DB_MSG(("Entering SetBasicParameters()"));

  /* NSLICES */
  ATB_SetNSlices( 1 );

  /* NR */
  ATB_SetNR( PVM_NRepetitions );

  /* NA */
  ATB_SetNA( PVM_NAverages );

  /* NAE */
  ATB_SetNAE( 1 );

  /* NECHOES */
  NECHOES = NEchoes;

  /* NI */
  ATB_SetNI( NECHOES );

  /* ACQ_obj_order */
  PARX_change_dims("ACQ_obj_order",NI);
  ACQ_obj_order[0] = 0;

  DB_MSG(("Exiting SetBasicParameters()"));
}

void SetFrequencyParameters( void )
{

  DB_MSG(("Entering SetFrequencyParameters()"));

  ATB_SetNuc1(PVM_Nucleus1);

  sprintf(NUC2,"off");
  sprintf(NUC3,"off");
  sprintf(NUC4,"off");
  sprintf(NUC5,"off");
  sprintf(NUC6,"off");
  sprintf(NUC7,"off");
  sprintf(NUC8,"off");

  ATB_SetNucleus(PVM_Nucleus1);

  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("SetFrequencyParameters: In function call!");
    return;
  }


  ACQ_O1_mode = BF_plus_Offset_list;
  ParxRelsParRelations("ACQ_O1_mode",Yes);

  O1 = 0.0;
  ParxRelsParRelations("O1",Yes);

  O2 = 0.0;
  O3 = 0.0;
  O4 = 0.0;
  O5 = 0.0;
  O6 = 0.0;
  O7 = 0.0;
  O8 = 0.0;

  /* Set BF's to working frequencies on used channels */
  ACQ_BF_enable = No;
  BF1 = PVM_FrqWork[0];
  BF2 = PVM_FrqWork[1];
  /* call relations of BF1 (no need for other BF's) */
  ParxRelsParRelations("BF1", Yes);

  int OrderList[1] = {0};
  double OffsetHz[1] = {0.0};

  ATB_SetAcqO1List( 1,
		    OrderList,
		    OffsetHz );

  ATB_SetAcqO1BList( 1,
		     OrderList,
		     OffsetHz);


  DB_MSG(("Exiting SetFrequencyParameters()"));
}

void SetGradientParameters( void )
{
  DB_MSG(("Entering SetGradientParameters()"));

  ATB_SetAcqGradMatrix(0);

  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 1.0;
  ACQ_scaling_slice = 1.0;

  ACQ_gradient_amplitude[0] = Spoiler_strength;


  DB_MSG(("Exiting SetGradientParameters()"));
}

void SetInfoParameters( void )
{
  int i;

  DB_MSG(("Entering SetInfoParameters()"));


  ACQ_flip_angle = ExcPulse1.Flipangle;

  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();

  /* ACQ_dim */
  int specDim = MRT_GetSpecDim();
  ACQ_dim = specDim;
  ParxRelsParRelations("ACQ_dim",Yes);

  /* ACQ_dim_desc */
  for(int dim=0; dim<ACQ_dim; dim++)
    ACQ_dim_desc[dim] = Spectroscopic;
  ParxRelsParRelations("ACQ_dim_desc", Yes);

  ACQ_n_echo_images = PVM_NEchoImages;

  PARX_change_dims("ACQ_echo_time",NEchoes);

  for (i=0;i<NEchoes;i++)
  {
    ACQ_echo_time[i] = EchoSpacing*(i+1);
  }

  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = EchoSpacing;

  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;

  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] =  PVM_RepetitionTime - ExcPulse1.Length;

  PARX_change_dims("ACQ_inversion_time",1);
  ACQ_inversion_time[0] = PVM_InversionTime;

  DB_MSG(("Exiting SetInfoParameters()"));

}

void SetPpgParameters( void )
{
  DB_MSG(("Entering SetPpgParameters()"));

  ATB_SetPulprog("CPMG.ppg");

  double riseT = CFG_GradientRiseTime();

  D[0] = (PVM_RepetitionTime - PVM_MinRepetitionTime + 0.01) / 1000.0;
  D[2] = (EchoSpacing/2.0 - MinTE1) / 1000.0;
  D[3] = (EchoSpacing/2.0 - MinTE2 + 0.02) / 1000.0;
  D[4] = riseT / 1000.0;
  D[5] = (Spoiler_duration - 2*riseT) / 1000.0;

  /* set shapes and durations of RF pulses     */
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");
  ATB_SetRFPulse("RefPulse1","ACQ_RfShapes[1]","P[1]");

  DB_MSG(("Exiting SetPpgParameters"));
}

void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters"));

  /* ------------------------------------------------ */
  /* -------               Job 0              ------- */
  /* ------------------------------------------------ */
  JOBPARAMETERS(jobParameters);
  JOBDESCRIPTION job0=jobParameters->getOrCreateJob("job0");

  job0->initPars( 1,PVM_SpecMatrix[0], PVM_SpecSWH[0] );

  job0->appendLoop(NI);

  job0->appendLoop(PVM_DummyScans,LOOP_DUMMIES);

  job0->appendLoop(NA, LOOP_AVERAGE, LOOP_SETUP);

  job0->appendLoop(NR);

  DB_MSG(("<--SetAcquisitionParameters"));
}


#if DEBUG
#define d(n) (D[n]*1e3)
#define p(n) (P[n]*1e-3)

void printTiming(void)
{
  double te1 = p(0)/2 + d(2) + 0.01 + p(1)/2;
  double te2 = p(1)/2 + 0.01 + d(3) + PVM_SpecAcquisitionTime/2;
  double tr = 0.03 + 2*NEchoes*te1 + PVM_SpecAcquisitionTime/2 + d(3) + d(0) + p(0)/2;

  if (SpoilerOnOff) {
    double spoil_dur = 2*d(4) + d(5);
    te1 += spoil_dur;
    te2 += spoil_dur;
    tr += 2 * NEchoes * spoil_dur;
  }

  DB_MSG(("te1 = %f ms, should be %f ms, diff = %f ms", te1, EchoSpacing/2, te1 - EchoSpacing/2));
  DB_MSG(("te2 = %f ms, should be %f ms, diff = %f ms", te2, EchoSpacing/2, te2 - EchoSpacing/2));
  DB_MSG(("tr = %f ms, should be %f ms, diff = %f ms", tr, PVM_RepetitionTime, tr - PVM_RepetitionTime));

  return;
}
#undef d
#undef p
#endif
