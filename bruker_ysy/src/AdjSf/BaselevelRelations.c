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
static const char resid[] = "$Id$ (C) 2002 - 2005 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	1
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

  DB_MSG(("-->SetBaseLevelParam()"));

  SetBasicParameters();

  SetAcquisitionParameters();
  ATB_SetReceiverGains();

  SetFrequencyParameters();
  
  SetPpgParameters();

  SetGradientParameters();
  
  SetInfoParameters();

  ATB_SetReceiverGains();

  ATB_InitPrepModuleAcqPars();

  printTiming();

  DB_MSG(("<--SetBaseLevelParam"));
  
}


void SetAcquisitionParameters(void)
{
  DB_MSG(("-->SetAcquisitionParameters"));

  // clear all existing job information that could have been
  // initialized in parent parameter space by a different methd
  ParxRelsResetPar("ACQ_jobs");
  ACQ_jobs_size = 1;
  ParxRelsParRelations("ACQ_jobs_size",Yes);

  /* ------------------------------------------------------ */
  /* ----------               Job 0              ---------- */
  /* ------------------------------------------------------ */
  JOBPARAMETERS(jobParameters);
  JOBDESCRIPTION job0=jobParameters->getOrCreateJob("job0");

  job0->initPars(1, AcqSize, PVM_EffSWh);

  job0->appendLoop(Nexc, LOOP_SETUP);
  job0->appendLoop(NA, LOOP_AVERAGE);
  job0->setCompensateGrpDelay(1);

  DB_MSG(("<--SetAcquisitionParameters"));
}



void SetBasicParameters( void )
{
  int dim;
  
  DB_MSG(("-->SetBasicParameters"));
  
  ACQ_scan_size = One_scan;

  ParxRelsParRelations("ACQ_scan_size",Yes);

  /* NSLICES */
  
  ATB_SetNSlices( 1 );
  
  /* NR */
  
  ATB_SetNR( PVM_NRepetitions );
  
  /* NI */
  
  ATB_SetNI(Nexc);
  
  /* NA */
  
  ATB_SetNA( PVM_NAverages );
  
  /* NAE */
  
  ATB_SetNAE( 1 );
  
  /* ACQ_ns */
  
  dim = PARX_get_dim("ACQ_ns_list",1);
  if( dim != 1 )
  {
    PARX_change_dims( "ACQ_ns_list",1 );
  }
  ACQ_ns_list_size = 1;
  ACQ_ns_list[0] = 1;
 
  /* NECHOES */
  
  NECHOES = 1;
  
  /* ACQ_obj_order */
  
  PARX_change_dims("ACQ_obj_order",Nexc);
  int *oord=ACQ_obj_order;
  int nobj=Nexc;
  for(int i=0;i<nobj;i++) oord[i]=i;
  
  
  DB_MSG(("<--SetBasicParameters"));
}

void SetFrequencyParameters( void )
{
  int noffs;
  DB_MSG(("-->SetFrequencyParameters"));
  
  ATB_SetNuc1(PVM_Nucleus1);

  sprintf(NUC2,"off");
  sprintf(NUC3,"off");
  sprintf(NUC4,"off");
  sprintf(NUC5,"off");
  sprintf(NUC6,"off");
  sprintf(NUC7,"off");
  sprintf(NUC8,"off");
  
  ATB_SetNucleus(PVM_Nucleus1);
  /* Set BF's to working frequencies on used channels */

  BF1 = PVM_FrqWork[0];
  SF0ppm=PVM_FrqWork[0]+STB_GetFreqOffset(1,0.0)*1e-6; //reference freq for topspin
  /* call relations of BF1 (no need for other BF's) */
  ParxRelsParRelations("BF1", Yes); 
  

  O2 = 0.0;
  O3 = 0.0;
  O4 = 0.0;
  O5 = 0.0;
  O6 = 0.0;
  O7 = 0.0;
  O8 = 0.0;

  ACQ_O1_list_size = ACQ_O1B_list_size = noffs = Nexc;

  ParxRelsParRelations("ACQ_O1_list_size",Yes);
  ParxRelsParRelations("ACQ_O1B_list_size",Yes);

  ParxRelsCopyPar("ACQ_O1_list","FreqOffsList");
  ParxRelsCopyPar("ACQ_O1B_list","FreqOffsList");

  
  DB_MSG(("<--SetFrequencyParameters"));
}

void SetGradientParameters( void )
{

  DB_MSG(("-->SetGradientParameters"));


  ATB_SetAcqGradMatrix();
  
  DB_MSG(("<--SetGradientParameters"));
}

void SetInfoParameters( void )
{
  int specDim=1;

  DB_MSG(("-->SetInfoParameters"));

  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();
  
  ACQ_dim=specDim;
  ParxRelsParRelations("ACQ_dim",Yes);

  ATB_SetAcqDimDesc(specDim,0);

  ACQ_read_ext = 1;
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

  ATB_SetAcqMethod();
  
  DB_MSG(("<--SetInfoParameters"));
  
}


void SetPpgParameters( void )
{

  DB_MSG(("-->SetPpgParameters"));


  ATB_SetPulprog("AdjSf.ppg");

  D[0]  = (PVM_RepetitionTime - PVM_MinRepetitionTime 
            + 5.0) / 1000.0;

  ParxRelsParRelations("D",Yes);

  /* set shaped pulses, in this method ACQ_RfShapes[0] is used */
  /* set duration of pulse, in this method P[0] is used        */
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");     
  
  
  DB_MSG(("<--SetPpgParameters"));
}


void printTiming(void)
{
#if DEBUG
#define d(n) (D[n]*1e3)
#define p(n) (P[n]*1e-3)

  DB_MSG(("-->printTiming"));

  double aqq = PVM_AcquisitionTime, tr;

  tr =  0.03 /*UpdateDynPars*/ + 0.01 /* freqRx */ + 0.01 /* freqTx*/ + p(0) + 0.05 /* denab+de */
    + aqq + 5.0 + 0.01 /* ADC_END */ + d(0);

  if (PVM_TriggerModule)
    tr += PVM_TriggerModuleTime;

  DB_MSG(("TR: %f, should be %f, diff %f", tr, PVM_RepetitionTime, 
           tr-PVM_RepetitionTime));

  DB_MSG(("<--printTiming"));

#undef d
#undef p
#endif

  return;
}
