/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2008
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/
static const char resid[] = "$Id$ (C) 2008 Bruker BioSpin MRI GmbH";

#define DEBUG		    0
#define DB_MODULE	  0
#define DB_LINE_NR	0

#include "method.h"

/*
 * method backbone
 */

void backbone(void)
{
  int	i;
  double PulseBandwidthArray[3], bf1;

  DB_MSG(("-->backbone"));

  /*
   * Nucleus related
   */

  STB_UpdateNuclei(Yes);

  if(strcmp(PVM_Nucleus1,"1H")!=0)
  {
    IsisManAdjustment = NoManAdjustment;
    PVM_WsMode = NO_SUPPRESSION;

    ParxRelsHideInEditor("PVM_VoxArrCSDisplacement");
    ParxRelsHideGroupInEditor("Suppression");
  }
  else /* for protons: */
  {
    ParxRelsShowInEditor("PVM_VoxArrCSDisplacement");
    ParxRelsShowGroupInEditor("Suppression");
  }

  /*
   * Disable preparation options when necessary
   */

  IsisAdjOvsPulses = No;	 // used in ppg
  IsisAdjInvPulses = No;

  switch(IsisManAdjustment)
  {
    case NoManAdjustment:
    default:
      break;
    case InvPulse1:
    case InvPulse2:
    case InvPulse3:
      IsisAdjInvPulses = Yes;
      break;
    case OvsPulse1:
    case OvsPulse2:
    case OvsPulse3:
      if(PVM_OvsOnOff==Off)
      {
        IsisManAdjustment=NoManAdjustment;
      }
      else
      {
        IsisAdjOvsPulses = Yes;
      }
      break;
  }

  /*
   * spectroscopic data acquisition
   */

  STB_UpdateSpectroscopy( PVM_Nucleus1 );

  /* update Encoding pargroup for multi receiver experiment */

  STB_UpdateEncoding();


  /* set up adjustment list */
  SetAdjustments();

  /* Effective isis averages */
  IsisNAverages = PVM_NAverages * 8;

  /* `
   * handle ISIS voxel pulses
   */

  UpdateRFPulses();
  PulseBandwidthArray[0] = InvPul1.Bandwidth;
  PulseBandwidthArray[1] = InvPul1.Bandwidth;
  PulseBandwidthArray[2] = InvPul1.Bandwidth;

  /*
   * update spoiling gradient parameters
   */

  IsisSpoilDurRange();
  IsisSpoilRels();

  /*
   * update stabilisation delays before/after Exc. Pulse
   */

  IsisStabDurRange();
  DeadTimeRange();

  /*
   * update voxel
   */
  STB_UpdateAtsReferencePosition("PVM_VoxelGeoObj");
  STB_UpdateVoxelGeometry( PulseBandwidthArray, PVM_Nucleus1 );


  /*
   * update mapshim parameter group
   */
  STB_UpdateMapShim(PVM_Nucleus1,"PVM_VoxelGeoObj");


  /*
   * calculate spatial error in mm per ppm
   */

  bf1 =  PVM_FrqWork[0];
  for (i=0; i<3; i++)
    IsisSpatialDispl[i] =
      (PVM_VoxArrSize[0][i]/PulseBandwidthArray[i]) * bf1;

  /*
   * calculate inversion and spectral excitation bandwidths per ppm
   */

  IsisInvBandwidthPpm = InvPul1.Bandwidth / bf1;
  IsisSpecExcBandwidthPpm = ExcPul1.Bandwidth / bf1;


  /*
   * handle water suppression, OVS and decoupling parameters
   */
  updateModules();

  updateOvsCycles();


  /* Update digitizer for the "imaging" mode */
  if(IsisManAdjustment != NoManAdjustment)
  {
    PVM_EffSWh=50000.0;
    PARX_change_dims("PVM_Matrix",1);
    PVM_Matrix[0] = 128;

    MRT_UpdateDigPars(&PVM_EffSWh,
                      PVM_Matrix[0],
                      NULL,
                      &PVM_AcquisitionTime);
    ParxRelsShowInEditor("PVM_EffSWh,PVM_Matrix,IsisFov");
  }
  else
  {
    ParxRelsHideInEditor("PVM_EffSWh,PVM_Matrix,IsisFov");
  }
  ParxRelsShowInFile("IsisFov"); //allways store in file


  /*
   * handle ISIS timing & gradients
   */

  UpdateIsisLocalization();

  /*
   * calculate ISIS module time
   */

  IsisModDurRelations();

  /*
   * update gradients
   */

  updateGradients();

  /*
   * calculate repetition time
   */

  UpdateRepetitionTime();

  if( PVM_ErrorDetected == Yes )
  {
    UT_ReportError("In function call backbone");
    return;
  }

  /* updating Dummy Scans after RepetitionTime */
  STB_UpdateDummyScans((IsisManAdjustment == NoManAdjustment)?PVM_RepetitionTime*8:PVM_RepetitionTime);


  /*
   * call base level functions
   */

  SetBaseLevelParameters();
  SetRecoParam();

  DB_MSG(("<--backbone"));
}


/*
 * subroutines called by the method handler
 */

void LocalDecNoeHandling(void)
{
 if((PVM_DecOnOff == On ) || (PVM_NoeOnOff == On))
 {
   if(PVM_NumberOfNuclei!=2)
   {
     STB_InitNuclei(2);
     if(PVM_NumberOfNuclei < 2)
     {
       /* system configuration does not support 2nd RF channel */
       PVM_DecOnOff = PVM_NoeOnOff = Off;
     }
   }
 }
 else if( (PVM_DecOnOff ==Off ) && (PVM_NoeOnOff == Off))
 {
   if(PVM_NumberOfNuclei!=1)
   {
     STB_InitNuclei(1);
   }
 }

 STB_UpdateDecModule(PVM_Nucleus2,PVM_SpecAcquisitionTime);
 STB_UpdateNoeModule(PVM_Nucleus2);


  if( (PVM_DecOnOff ==On ) && (PVM_NoeOnOff == On)
      && PVM_DecMode == Composite_Pulse && PVM_NoeMode == Composite_Pulse_Noe)
  {
    /* both modules use F2 channel and share therefor the CPD pulse
       element duration PCPD[1] so the element durations have to be matched */

    if(PVM_NoePulseElementDuration != PVM_DecPulseElementDuration)
    {
      PVM_NoePulseElementDuration = PVM_DecPulseElementDuration;
      STB_UpdateNoeModule(PVM_Nucleus2);
    }
  }

}

void updateModules(void)
{
  DB_MSG(("-->updateModules"));

  double minext=PVM_VoxArrSize[0][0];

  for(int k=1;k<3;k++)
  {
    minext=MIN_OF(minext,PVM_VoxArrSize[0][k]);
  }

  STB_UpdateSatTransModule(PVM_DeriveGains,minext);

  /*
   * outer volume supression handler
   * if NVoxels > 1, PVM_VoxMinDistance is the limiting factor
   * for the saturation slice thickness.
   */

  STB_UpdateOvsModule( PVM_VoxMinDistance,
                       PtrType1x3 PVM_VoxArrSize,
                       PtrType1x3 PVM_VoxArrPositionRPS,
                       PVM_NVoxels,
                       PVM_DeriveGains,
                       PVM_Nucleus1);

  /*
   * handle water suppression
   */
  double excDelay= IsisModDur - Isis_P0/2.0; //ms
  STB_UpdateWsModule( PVM_OvsModuleDuration,
                      PVM_NVoxels,
                      excDelay);

  LocalDecNoeHandling();

  DB_MSG(("<--updateModules"));
}

void updateOvsCycles(void)
{
  DB_MSG(("-->UpdateOvsCycles"));

  /* number of ovs cycles in the Water-suppr module: */
#define NOVS_IN_WS 3

  if(PVM_OvsOnOff == Off)
  {
    ParxRelsHideInEditor("IsisNumOvsCycles");
    IsisNumAddOvsCycles = 0;
  }
  else
  {
    ParxRelsShowInEditor("IsisNumOvsCycles");
    IsisNumOvsCycles = MAX_OF( IsisNumOvsCycles, NOVS_IN_WS);
    IsisNumAddOvsCycles = IsisNumOvsCycles - NOVS_IN_WS;
  }

  DB_MSG(("<--UpdateOvsCycles"));
  return;

#undef NOVS_IN_WS
}

/* calculates min repetition time and total scantime */

void UpdateRepetitionTime(void)
{
  double  modulesDuration=0, minTR=0, TotalTime=0;

  DB_MSG(("-->UpdateRepetitionTime"));

  modulesDuration = PVM_TriggerModuleTime +
                    PVM_SatTransModuleTime +
                    IsisNumAddOvsCycles * PVM_OvsModuleDuration +
                    PVM_WsModuleDuration +
                    PVM_NoeModuleTime;

  /* calculate Min.TR duration */

  switch(IsisManAdjustment)
  {
    case NoManAdjustment:
    default:
      minTR =
        0.03 +  /* UpdateDynPars */
        1.0 +
        PVM_TriggerModuleTime +
        modulesDuration +
        IsisModDur +
        Isis_D14 +       /* pre-aqq */
        PVM_SpecAcquisitionTime +
        1.0; /* post-aqq */
      break;
    case InvPulse1:
    case InvPulse2:
    case InvPulse3:
    case OvsPulse1:
    case OvsPulse2:
    case OvsPulse3:
      minTR =
        0.03 +   /* UpdateDynPars */
        1.0 +
        PVM_TriggerModuleTime+
        modulesDuration +
        IsisModDur +
        Isis_D6 + 2.0*Isis_D4 + /* pre-aqq */
        PVM_AcquisitionTime +
        Isis_D4 + 1.0;        /* post-aqq */
      break;
  }

  PVM_MinRepetitionTime = minTR;
  PVM_RepetitionTime = MAX_OF(PVM_RepetitionTime, minTR);

  Isis_D0 = (PVM_RepetitionTime-PVM_MinRepetitionTime);

  /* Calculate Total Scan Time and Set for Scan Editor */

  switch(IsisManAdjustment)
  {
    case NoManAdjustment:
    default:
      TotalTime = PVM_RepetitionTime      *
                  (double)(IsisNAverages) *
                  PVM_SatTransRepetitions *
                  PVM_NRepetitions ;
      break;
    case InvPulse1:
    case InvPulse2:
    case InvPulse3:
    case OvsPulse1:
    case OvsPulse2:
    case OvsPulse3:
      TotalTime = PVM_RepetitionTime  *
                  (double)PVM_NAverages *
                  PVM_SatTransRepetitions *
                  PVM_NRepetitions;
      break;
  }

  /* display total scan time */
  PVM_ScanTime = TotalTime;
  UT_ScanTimeStr(PVM_ScanTimeStr,TotalTime);
  ParxRelsMakeNonEditable("PVM_ScanTimeStr");

  DB_MSG(("<--UpdateRepetitionTime"));
  return;
}

/*
 * Local parameter relations
 */

void IsisModDurRelations(void)
{
  DB_MSG(("-->IsisModDurRelations"));

  /* all parameters in milliseconds */

  Isis_D4 = CFG_GradientRiseTime();
  Isis_D6 = PVM_AcquisitionTime / 2.0 + 0.5*CFG_GradientRiseTime();
  Isis_D10 = InvPul1.Length;
  Isis_P0 = ExcPul1.Length;
  Isis_P1 = InvPul1.Length;

  Isis_D11 = IsisSpoilDur; /* delay after inversion pulse */
  Isis_D13 = IsisStabDur;  /* stab. time after last spoiler  */
  Isis_D14 = DeadTime;

  /* isis module: inversions + exc pulse: */

    IsisModDur = 3*(Isis_D4 + 0.01 + Isis_P1 + Isis_D11) +
      Isis_D4 + Isis_D13 + Isis_P0;


  DB_MSG(("<--IsisModDurRelations"));
}



void IsisSpoilRels(void)
{
    DB_MSG(("-->IsisSpoilRels"));

    double gcc = SysInfo_GRADCC();

    IsisSpoilPerc1 = (100.0 * 1000.0 * IsisSpoilCyMm1) /
			(IsisSpoilDur * gcc * 0.1);
    IsisSpoilPerc2 = (100.0 * 1000.0 * IsisSpoilCyMm2) /
			(IsisSpoilDur * gcc * 0.1);
    IsisSpoilPerc3 = (100.0 * 1000.0 * IsisSpoilCyMm3) /
			(IsisSpoilDur * gcc * 0.1);

    DB_MSG(("<--IsisSpoilRels"));
}


void UpdateRFPulses(void)
{

  DB_MSG(("-->UpdateRFPulses"));

  STB_UpdateRFPulse("InvPul1", 1, PVM_DeriveGains, AfpUp);
  STB_UpdateRFPulse("ExcPul1", 1, PVM_DeriveGains, Conventional);

  if(PVM_DeriveGains==Yes)
  {
    ParxRelsHideInEditor("InvPul1Ampl");
    ParxRelsHideInEditor("ExcPul1Ampl");
  }
  else
  {
    ParxRelsShowInEditor("InvPul1Ampl");
    ParxRelsShowInEditor("ExcPul1Ampl");
  }

  ParxRelsShowInFile("InvPul1Ampl");
  ParxRelsShowInFile("ExcPul1Ampl");

  DB_MSG(("<--UpdateRFPulses"));
}


void UpdateIsisLocalization()
{
  DB_MSG(("-->UpdateIsisLocalization"));

  /*
   *  make pargroup PPGparameters not visible in editor
   *  but store them into the method file
   */

  ParxRelsHideGroupInEditor("PPGparameters");
  ParxRelsHideInEditor("PPGparameters");
  ParxRelsShowInFile("PPGparameters");

  DB_MSG(("<--UpdateIsisLocalization"));
  return;
}


void updateGradients(void)
{
  double integ1, integ2, ratio, readgrad, readdephase, min;

  DB_MSG(("-->updateGradients"));

  Isis_Gr1 = PVM_VoxArrGradient[0][0];
  Isis_Gp1 = PVM_VoxArrGradient[0][1];
  Isis_Gs1 = PVM_VoxArrGradient[0][2];

  Isis_Gr2 = IsisSpoilPerc1;
  Isis_Gp2 = IsisSpoilPerc2;
  Isis_Gs2 = IsisSpoilPerc3;

  Isis_Gr3 = Isis_Gr4 = 0.0;
  Isis_Gp3 = Isis_Gp4 = 0.0;
  Isis_Gs3 = Isis_Gs4 = 0.0;

  /* geom limits and gradients for the "imaging" mode: */

  if(IsisManAdjustment !=NoManAdjustment)
  {
    integ1 = Isis_D6;
    integ2 = PVM_AcquisitionTime/2.0 + 0.5*Isis_D4;
    ratio = integ2 / integ1;
    min = MRT_MinReadFov(PVM_EffSWh,
                         ratio,
                         100.0,
                         100.0,
                         PVM_GradCalConst);
    IsisFov = MAX_OF(IsisFov,min);

    readgrad = MRT_ReadGrad(PVM_EffSWh,IsisFov,PVM_GradCalConst);
    readdephase = MRT_ReadDephaseGrad(ratio, readgrad);


    switch(IsisManAdjustment)
    {
      case NoManAdjustment:
      default:
        break;
      case InvPulse1:
      case OvsPulse1:
        Isis_Gp1 = Isis_Gp2 = Isis_Gp3 = Isis_Gp4 = 0.0;
        Isis_Gs1 = Isis_Gs2 = Isis_Gs3 = Isis_Gs4 = 0.0;
        Isis_Gr3 = -readdephase;
        Isis_Gr4 = readgrad;
        break;
      case InvPulse2:
      case OvsPulse2:
        Isis_Gr1 = Isis_Gr2 = Isis_Gr3 = Isis_Gr4 = 0.0;
        Isis_Gs1 = Isis_Gs2 = Isis_Gs3 = Isis_Gs4 = 0.0;
        Isis_Gp3 = -readdephase;
        Isis_Gp4 = readgrad;
        break;
      case InvPulse3:
      case OvsPulse3:
        Isis_Gr1 = Isis_Gr2 = Isis_Gr3 = Isis_Gr4 = 0.0;
        Isis_Gp1 = Isis_Gp2 = Isis_Gp3 = Isis_Gp4 = 0.0;
        Isis_Gs3 = -readdephase;
        Isis_Gs4 = readgrad;
        break;

    }
  }

  DB_MSG(("<--updateGradients"));
}


void SetAdjustments(void )
{
    DB_MSG(("-->SetAdjustments"));

    PTB_ClearAdjustments();

    STB_ArrayPhaseAppendAdjustment(PVM_EncNReceivers, No);
    //no voxel passed for adj context since phases are
    //calibrated globally without inversion.

    PTB_AppendOrderAdjustment(per_scan, per_scan, RG_ADJNAME);

    DB_MSG(("<--SetAdjustments"));
}



