/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2018
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
#include <libCore/Math/Math.hh>



void SetBaseLevelParam( void )
{

  DB_MSG(("-->SetBaseLevelParam()"));

  SetBasicParameters();
  
  SetFrequencyParameters();
   
  SetPpgParameters();

  SetAcquisitionParameters();
  
  SetGradientParameters();
  
  SetInfoParameters();

  ATB_SetReceiverGains();

  /* setting baselevel parameters used by modules */
  ATB_SetFatSupBaselevel();
  ATB_SetDecBaseLevel();
  ATB_SetNoeBaseLevel();
  ATB_SetWsBaseLevel();
  ATB_SetFovSatBaseLevel();
  ATB_SetSatTransBaseLevel();

  printTiming();
  
  DB_MSG(("<--SetBaseLevelParam"));
}


void SetBasicParameters( void )
{
  DB_MSG(("-->SetBasicParameters()"));
  
  /* NSLICES */  
  ATB_SetNSlices( SliceNum );
  
  /* NR */  
  ATB_SetNR( PVM_SatTransRepetitions * PVM_NRepetitions );
  
  /* NI */  
  ATB_SetNI( 1 );

  /* Averaging via jobs and Reco -> NA=1 */  
  ATB_SetNA(1);
  AverageList[0]= PVM_NAverages;


  /* NAE */  
  ATB_SetNAE( 1 );
 
  /* ACQ_ns_list */
  ACQ_ns_list_size = 1;
  ParxRelsParRelations("ACQ_ns_list_size",Yes);
  ACQ_ns_list[0] = 1;
  
  /* NECHOES */ 
  NECHOES = 1;
  
  
  /* ACQ_obj_order */  
  PARX_change_dims("ACQ_obj_order",NI);
  ACQ_obj_order[0] = 0;
    
  DB_MSG(("<--SetBasicParameters()"));
}

void SetFrequencyParameters( void )
{
  
  DB_MSG(("-->SetFrequencyParameters()"));
  
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

  ATB_SetAcqO1List( SliceNum,
                    SliceOrderList,
                    SliceFL );

  ATB_SetAcqO1BList( 1,
                     OrderList,
                     OffsetHz); 

  DB_MSG(("<--SetFrequencyParameters()"));
}

void SetGradientParameters( void )
{
  DB_MSG(("-->SetGradientParameters()"));
  
  // new function to set up gradient matrix */
  if ( SliceSelOnOff == Off )
    ATB_SetAcqGradMatrix(0);
  else
    ATB_SetAcqGradMatrix("SliceGeoObj");

  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 1.0;
  ACQ_scaling_slice = 1.0;

  DB_MSG(("<--SetGradientParameters()"));
}

void SetInfoParameters( void )
{
  
  DB_MSG(("-->SetInfoParameters()"));

  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();

   /* ACQ_dim */
  ACQ_dim = MRT_GetSpecDim();
  ParxRelsParRelations("ACQ_dim",Yes);
  
  /* ACQ_dim_desc */
  ATB_SetAcqDimDesc( ACQ_dim, 0, NULL );

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
  
  DB_MSG(("<--SetInfoParameters()"));
  
}


void SetPpgParameters( void )
{
  DB_MSG(("-->SetPpgParameters()"));

  double riseTime = CFG_GradientRiseTime();
  double rpTime = SliceSelOnOff == On ? riseTime : 0.0;

  ACQ_vd_list_size=1;
  PARX_change_dims("ACQ_vd_list",1);
  ACQ_vd_list[0] = 1e-6;
  ParxRelsParRelations("ACQ_vd_list",Yes);

  ATB_SetPulprog("NSPECT.ppg");

  D[0] = (PVM_RepetitionTime - PVM_MinRepetitionTime) / SliceNum / 1000.0;
  D[1] = DeadTime/ 1000.0;
  D[2] = rpTime  / 1000.0;
  D[3] = SliceRephDur / 1000.0;
  D[4] = (rpTime + SliceSpoilDur) /1000.0;

  ParxRelsParRelations("D",Yes);

  /* set shaped pulses, in this method ACQ_RfShapes[0] and ACQ_RfShapes[1] is used           
     the pulse duration is stored in baselevel parameter P[0] and P[1]
  */
  ATB_SetRFPulse("ExcPulse1","ACQ_RfShapes[0]","P[0]");
    
  
  DB_MSG(("<--SetPpgParameters"));
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

  job0->appendLoop(NSLICES);

  job0->appendLoopList(AverageList, 1, LOOP_AVERAGE, LOOP_SETUP);

  job0->appendLoop(NR);

  job0->setCompensateGrpDelay(0);
  //show averaged data in acq display but store and process non-averaged data
  job0->setStoreDataMode(STORE_raw);
  job0->setPipelineMode(PIPELINE_raw);


  /* ------------------------------------------------ */
  /* -------             Navigator            ------- */
  /* ------------------------------------------------ */

  int nTotalScansNav = NSLICES * NR * PVM_NAverages; //do not use NA (set to 1)
  int transBlocks = NSLICES * PVM_NAverages;

  JOBDESCRIPTION navigator;
  navigator = ATB_SetNavBaseLevel(jobParameters, 1, transBlocks, nTotalScansNav, PVM_DummyScans); //pars, pulse shape, transBlocks, totalScans, dummyScans

  ATB_LinkJobToDriftCompModule(jobParameters, navigator);
 
  DB_MSG(("<--SetAcquisitionParameters\n"));
}


void printTiming(void)
{

#if DEBUG
#define d(n) (D[n]*1e3)
#define p(n) (P[n]*1e-3)

  DB_MSG(("-->printTiming"));

  double aqq=PVM_SpecAcquisitionTime, te, tr;
 
  /* TE */
  te = 0.5*p(0) + d(2) + d(3) + d(2) + d(1); /* d(1)=dur1+10u in acqdec */


  /* TR */
  tr =   0.03   /* update_dyn_pars */
       + 0.01   /* freqTx */
       + 0.01   /* freqRx */
       + d(4) + 0.5*p(0) + te + aqq 
       + 1.0    /* ADC_END */ 
       + d(0);

  tr += PVM_TriggerModuleTime
       + PVM_FatSupModuleTime
       + PVM_NoeModuleTime
       + PVM_WsModuleDuration
       + PVM_FovSatModuleTime
       + PVM_SatTransModuleTime;

  tr *= SliceNum;

  if (PVM_NavOnOff == On)
    tr+=0.02+PVM_NavigatorModuleTime;

  DB_MSG(("   TE: %f, should be %f, diff %f", te, EchoTime, te-EchoTime));
  DB_MSG(("   TR: %f, should be %f, diff %f", tr, PVM_RepetitionTime, tr-PVM_RepetitionTime));

  DB_MSG(("<--printTiming"));

#undef d
#undef p
#endif

}
