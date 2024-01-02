/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/
static const char resid[] = "$Id$ (C) 2002 - 2011 Bruker BioSpin MRI GmbH";

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
  
  /* NI */
  
  ATB_SetNI(1 );

  /* NA */
  
  ATB_SetNA( PVM_NAverages );
  
  /* NAE */
  
  ATB_SetNAE( 1 );
  
  
  /* ACQ_ns */
  
  ACQ_ns_list_size = 1;
  ACQ_ns_list[0] = 1;  
  
  /* NECHOES */
  NECHOES = 1;
  
  
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
 
  ACQ_O1_mode = BF_plus_Offset;
  ParxRelsParRelations("ACQ_O1_mode",Yes);

  O1 = 0.0;
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
  SF0ppm=PVM_FrqWork[0]+STB_GetFreqOffset(1,0.0)*1e-6; //reference freq for topspin
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

  DB_MSG(("Exiting SetGradientParameters()"));
}

void SetInfoParameters( void )
{
  int specDim;
  
  DB_MSG(("Entering SetInfoParameters()"));

  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();
  
  /* ACQ_dim */
  specDim = MRT_GetSpecDim();
  ACQ_dim = specDim;
  ParxRelsParRelations("ACQ_dim",Yes);
  
  /* ACQ_dim_desc */
  ATB_SetAcqDimDesc( specDim, 0, NULL );  

  ACQ_flip_angle = ExcPulse1.Flipangle;
  
  PARX_change_dims("ACQ_echo_time",1);
  ACQ_echo_time[0] = PVM_EchoTime;
  
  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;
  
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

  ATB_SetPulprog("SINGLEPULSE.ppg");

  D[0]  = (PVM_RepetitionTime - PVM_MinRepetitionTime + 0.01) / 1000.0;
  D[1] = DeadTime / 1000.0;
  
  /* set shaped pulses, in this method ACQ_RfShapes[0] is used           
     the pulse duration is stored in baselevel parameter P[0]
  */
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");

  DB_MSG(("Exiting SetPpgParameters"));
}


void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters\n"));

  /* ------------------------------------------------------ */
  /* ----------               Job 0              ---------- */
  /* ------------------------------------------------------ */

  JOBPARAMETERS(jobParameters);
  JOBDESCRIPTION job0 = jobParameters->getOrCreateJob("job0");
  job0->initPars(1,PVM_SpecMatrix[0], PVM_SpecSWH[0]);

  job0->appendLoop(PVM_DummyScans, LOOP_DUMMIES);
  job0->appendLoop(NA, LOOP_AVERAGE, LOOP_SETUP);
  job0->appendLoop(NR);

  job0->setCompensateGrpDelay(0);

  DB_MSG(("<--SetAcquisitionParameters\n"));
}

#if DEBUG
#define d(n) (D[n]*1e3)
#define p(n) (P[n]*1e-3)

void printTiming(void)
{
  double aqq=PVM_SpecAcquisitionTime, tr;

   /* TR */
  tr =0.03+d(0)+p(0)+d(1)+aqq+5.0+0.01;

  DB_MSG(("TR: %f, should be %f, diff %f", tr, PVM_RepetitionTime, tr-PVM_RepetitionTime));

  return;
}

#undef d
#undef p
#endif
