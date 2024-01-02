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

  ACQ_O1_mode = BF_plus_Offset_list;
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
  memset(ACQ_GradientMatrix[0][0],0,9*sizeof(double));

  ACQ_scaling_read  = 1.0;
  ACQ_scaling_phase = 1.0;
  ACQ_scaling_slice = 1.0;
  
  switch(GradientDirection)
  {
  default:
  case Z_dir:
    ACQ_GradientMatrix[0][0][2] = 1.0;
    ACQ_GradientMatrix[0][1][1] = 1.0;
    break;
  case Y_dir:
    ACQ_GradientMatrix[0][0][1] = 1.0;
    ACQ_GradientMatrix[0][1][2] = 1.0;
    break;
  case X_dir:
    ACQ_GradientMatrix[0][0][0] = 1.0;
    ACQ_GradientMatrix[0][1][1] = 1.0;
    break;
  } 
  
  ACQ_gradient_amplitude[0] =  ReadGradient;
  ACQ_gradient_amplitude[1] =  Spoiler_strength;

  DB_MSG(("Exiting SetGradientParameters()"));
}

void SetInfoParameters( void )
{
  
  DB_MSG(("Entering SetInfoParameters()"));
  
  // initialize ACQ_n_echo_images ACQ_echo_descr
  //            ACQ_n_movie_frames ACQ_movie_descr
  ATB_ResetEchoDescr();
  ATB_ResetMovieDescr();

  int spatDim = 1;

  /* ACQ_dim */
  ACQ_dim = spatDim;
  ParxRelsParRelations("ACQ_dim",Yes);
  
  /* ACQ_dim_desc */
  ATB_SetAcqDimDesc( 0, spatDim, NULL );
 
  ATB_SetAcqFov( Spatial, spatDim, PVM_Fov, PVM_AntiAlias );

  ACQ_flip_angle = SPExamExc.Flipangle;
  
  PARX_change_dims("ACQ_echo_time",1);
  ACQ_echo_time[0] = PVM_EchoTime;
  
  PARX_change_dims("ACQ_inter_echo_time",1);
  ACQ_inter_echo_time[0] = PVM_EchoTime;
  
  PARX_change_dims("ACQ_repetition_time",1);
  ACQ_repetition_time[0] = PVM_RepetitionTime;
  
  PARX_change_dims("ACQ_recov_time",1);
  ACQ_recov_time[0] =  PVM_RepetitionTime;
  
  PARX_change_dims("ACQ_inversion_time",1);
  ACQ_inversion_time[0] = InversionTime;
  
  DB_MSG(("Exiting SetInfoParameters()"));
  
}

void SetPpgParameters( void )
{
  DB_MSG(("Entering SetPpgParameters()"));

  ATB_SetPulprog("RfProfile.ppg");
  
  double igwt = CFG_InterGradientWaitTime();
 
  D[0]  = (PVM_RepetitionTime - PVM_MinRepetitionTime 
            + 1.0) / 1000.0;
  D[1] = ((PVM_EchoTime - MinTE1)/2.0 + igwt) / 1000.0;
  D[2] = Spoiler_duration/1000.0;
  D[3] = ((PVM_EchoTime - MinTE2)/2.0 + igwt) / 1000.0;
  D[4]  = CFG_GradientRiseTime() / 1000.0;

  if(ProfileMode == Inversion_Profile)
  {
    D[5] = ( InversionTime - SPExamInv.Length/2 - HardPulse.Length/2
            - CFG_GradientRiseTime() - 5.0 - CFG_GradientRiseTime() )/1000.0;
  }
  if(ProfileMode == Saturation_Profile)
  {
    D[5] = ( InversionTime - SPExamExc.Length/2 - HardPulse.Length/2
            - CFG_GradientRiseTime() - 5.0 - CFG_GradientRiseTime() )/1000.0;
  }

  D[7] = Spoiler_duration2/1000.0;
  
  /* set shaped pulses    */

  switch(ProfileMode)
  { 
    default:
    case Excitation_Profile:
      ATB_SetRFPulse("SPExamExc","ACQ_RfShapes[0]","P[0]");     
      break;
    case Refocusing_Profile:
      ATB_SetRFPulse("SPExamRfc","ACQ_RfShapes[0]","P[0]");     
      break;
    case Inversion_Profile:
      ATB_SetRFPulse("SPExamInv","ACQ_RfShapes[0]","P[0]");     
      break;
    case Saturation_Profile:
      ATB_SetRFPulse("SPExamExc","ACQ_RfShapes[0]","P[0]");     
      break;
  }

  ATB_SetRFPulse("HardPulse","ACQ_RfShapes[1]","P[1]");
  ATB_SetRFPulse("HardPulse","ACQ_RfShapes[2]");

  // P[1] set by HardPulse.Length P[2] to be set now
  P[2] = 2*P[1];
  ParxRelsParRelations("P",Yes);
  
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

  job0->initPars(1, PVM_Matrix[0], PVM_EffSWh * PVM_AntiAlias[0] );

  job0->appendLoop(Ndummy, LOOP_DUMMIES, LOOP_SETUP);
  job0->appendLoop(NA, LOOP_AVERAGE);
  job0->appendLoop(NR);

  DB_MSG(("<--SetAcquisitionParameters"));
}



#if DEBUG
#define d(n) (D[n]*1e3)
#define p(n) (P[n]*1e-3)

void printTiming(void)
{
  double ti=0.0, te=0.0, tr=0.0, aqq=PVM_AcquisitionTime;
  double te_all=0.0; /*delays outside any if-statement counting to TE*/
  double tr_all=0.0; /*delays outside any if-statement counting to TR*/

  te_all  = 0.1 + d(4) + d(7) + d(4) + d(1);
  te_all += d(3) + d(4) + d(2) /*dur2 + de*/ + aqq/2;

  tr_all = 0.03 /*start label*/ + 0.01 /*freqRx*/ + aqq/2 + 0.02 + d(4)+ 1.0 + 0.01 + 0.01 + d(0);

  if (ProfileMode == Inversion_Profile || ProfileMode == Saturation_Profile )
  {
    ti = p(0)/2 + 5.0 + d(4) + d(5) + d(4) + p(1)/2;
    te = p(1)/2 + d(4) + d(4) + p(2) + d(4) + te_all;
    tr =   d(4) + p(0)/2  /*left side of saturation/inversion*/
         + ti + te + tr_all;

    DB_MSG((" ti: %f, should be %f, diff %f", ti, InversionTime,      ti - InversionTime));
  }

  if(ProfileMode == Excitation_Profile)
  {
    te = p(0)/2 + d(4) + d(4) + p(1) + d(4) + te_all;
    tr = d(4) + p(0)/2 + te + tr_all;
  }

  if(ProfileMode == Refocusing_Profile)
  {
    te = p(1)/2 + d(4) + d(4) + p(0) + d(4) + te_all;
    tr =   d(4) + p(1)/2  /*left side of excitation*/ + te + tr_all;
  }

    DB_MSG((" te: %f, should be %f, diff %f", te, PVM_EchoTime,       te - PVM_EchoTime));
    DB_MSG((" tr: %f, should be %f, diff %f", tr, PVM_RepetitionTime, tr - PVM_RepetitionTime));

  return;
}

#undef d
#undef p
#endif
