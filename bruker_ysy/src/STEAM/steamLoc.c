/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2006
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 *
 * ***************************************************************/

static const char resid[] = "$Id$ (C) 2006 Bruker BioSpin MRI GmbH";


#define DEBUG		0
#define DB_MODULE	1
#define DB_LINE_NR	0



#include "method.h"


/*----------------- spoiler handling ---------------------*/

static void    SpoilTypeInit(SPOIL_TYPE *spt);
static YesNo   SpoilTypeRange(SPOIL_TYPE *spt,
                              double maxgrad,
                              double mingrad);
static double  SpoilTypeMaxSpoil(SPOIL_TYPE *spt,
                                 const double maxgrad,
                                 const double gcc);
static void    SpoilTypeS2G(SPOIL_TYPE  *spt,double gcc);
static void    SpoilTypeG2S(SPOIL_TYPE  *spt,double gcc);




static void CalcTESpoil(SPOIL_TYPE *spte1,
                        SPOIL_TYPE *spte2,
                        const double *spdiff,
                        const double *minspoil,
                        const double maxgrad,
                        const double gcc,
                        int mode);


void InitSteamLocalization(void)
{
  DB_MSG(("-->InitSteamLocalization"));

  StSpTE1Range();
  StSpTE2Range();
  StSpTMRange();
  StTMRange();
  StGStabDRange();
  ParxRelsHideInEditor("StGscheme");
  ParxRelsShowInFile("StGscheme");
  ParxRelsMakeNonEditable("StDur,StSpTE1");
  

  DB_MSG(("<--InitSteamLocalization"));
  return;
}

void UpdateSteamLocalization(const char    *const nuc,
                             double (*vsiz)[3] ,
                             double (*vgrad)[3],
                             double        *const te,
                             double        *const minte,
                             PVM_RF_PULSE *const pul1,
                             PVM_RF_PULSE *const pul2,
                             PVM_RF_PULSE *const pul3,
                             VSEL_ManAdjType   *const opt,
                             double *const fov)
{

  double maxte=20.0;
  double gcc=0,pdp[3],effpd[3],riset,dval,dval1,tm;
  int    parind=0,sind=0,i;
  YesNo  request=No,vproj=No;
  double minte1,minte2,minspoil[3];
  double read_deph=0.0,minfov=0.001;
  double read_grad=0.0;

  const char *const parlist =
    "StSpTE1,StSpTE2,StSpTM";
  /*    1       2      3     */
  const char *const strlist =
    "dur,grad_dir1,grad_dir2,grad_dir3,spoil_dir1,spoil_dir2,spoil_dir3";
  /*   1     2       3       4       5       6     */

  DB_MSG(("-->UpdateSteamLocalization"));


  gcc=CFG_GradCalConst(nuc);
  riset=CFG_GradientRiseTime();


  request = UT_GetRequest(parlist,&parind,strlist,&sind);


  for(i=0;i<3;i++)
  {
    dval = vsiz[0][i];
    dval = MAX_OF(1e-3,dval);
    minspoil[i] = 2.0/dval;
  }

  

  /*
   *   calculate min timing
   */

  /* 1st excitation pulse */
  effpd[0] = pul1->Length*pul1->Rpfac/100*100/100;

  /* 2nd backrotation/storage pulse */
  effpd[1] = pul2->Length*(1.0-pul2->Rpfac/100*100/100);

  /* 3rd pulse */
  effpd[2] = pul3->Length*pul3->Rpfac/100*100/100;

  /* assure a sufficient long spoiling duration to compensate dephasing of slice gradients */
  for(i=0,dval1=0;i<3;i++)
  {
    dval= riset + (StGStabD+effpd[i])*vgrad[0][i]/57.0;
    dval1=MAX_OF(dval1,dval);
  }

  /* equalize the spoiling duration in the TE/2 periods */

  if(request == Yes && sind == 1 && parind == 2)
  {
    dval=2*riset+StGStabD+CFG_InterGradientWaitTime();
    dval=MAX_OF(dval,StSpTE2.dur);
    StSpTE1.dur = StSpTE2.dur = MAX_OF(dval,dval1);
    request     = No;
    UT_ClearRequest();
  }
  else
  {
    dval=2*riset+StGStabD+CFG_InterGradientWaitTime();
    dval=MAX_OF(dval,StSpTE1.dur);
    StSpTE2.dur = StSpTE1.dur = MAX_OF(dval,dval1);
  }

  /* 
   * handle the voxel projection mode: for OVS, the slice gradient in the desired direction
   * is switched off
   */

  switch(*opt)
  {
    case  OVSPulse1:
      vgrad[0][0] = 0.0;
    case Pulse1:
      vproj=Yes;
      break;
    case  OVSPulse2:
      vgrad[0][1] = 0.0;
    case Pulse2:
      vproj=Yes;
      break;
    case  OVSPulse3:
      vgrad[0][2] = 0.0;
    case Pulse3:
      vproj=Yes;
      break;
    default:
    case NoManAdjustment:
      vproj=No;
      break;
  }

  /*
   * calculate the minimum TE dependent on
   * gradient switching scheme
   */

  StGscheme=0;
  maxte=1000;
  minte1= effpd[0] + StSpTE1.dur + effpd[1] + StGStabD;
  minte2= effpd[2] + StSpTE2.dur;

  *minte = 2*MAX_OF(minte2,minte1);
  *te=MAX_OF(MIN_OF(maxte,*te),*minte);


  /*
   * establish a zero gradient TE/2 filling period in case the desired TE is long
   * enough
   */


  if((*te/2 - minte1) > riset + CFG_InterGradientWaitTime()||vproj==Yes)
  {
    StGscheme = 1;
  }
  else
  {
    StGscheme = 0;
  }
 

  /* assure a TM spoiling duration 3 times longer than TE spoiling */

  dval = pul2->Length-effpd[1]+3*StSpTE1.dur+pul3->Length-effpd[2]+StGStabD;
  tm  =  StTM;

  tm  =  MAX_OF(tm,dval);
  StSpTM.dur = tm - dval+3*StSpTE1.dur;
  StTM = tm;

  /* 
   * calculate slice gradient dephasing (pdp) in units of cycles per mm,
   * the dephasing in the 2nd TE/2 period must differ by pdp  in the 
   * first TE/2 period
   */


  if(StGscheme==0)
  {
    dval= *te/2 - minte1;
    dval = MAX_OF(dval,0.0);
  }
  else
  {
    dval = 0.0;
  }

  pdp[0]=  gcc*vgrad[0][0]/100*(effpd[0] + riset/2)/1000.0;
  pdp[1]=  gcc*vgrad[0][1]/100*(effpd[1] + dval + StGStabD+riset/2)/1000.0;
  pdp[2]= -gcc*vgrad[0][2]/100*(effpd[2] + riset/2)/1000.0;

  /* 
   * calculate read dephasing in units of cycles per mm for the 
   * voxel projection mode
   */

  if(vproj==Yes)
  {
    /* 0.02: decodur delay, PVM_AcquisitionTime: duration of acquisition period   */
    read_deph= 0.5*riset+0.02+PVM_AcquisitionTime/2.0;
    /* calculate ratio between read gradient and read dephasing gradient */
    dval = read_deph/(StSpTE1.dur-riset);
    /* calculate the min FOV for this ratio and maximum gradient amplitudes */
    minfov=MRT_MinReadFov(PVM_EffSWh,dval,100.0,57.0,gcc);
    *fov=MAX_OF(minfov,*fov);
    /* calculate the read gradient for the voxel projection */
    read_grad = MRT_ReadGrad(PVM_EffSWh,*fov,gcc);
    /* calculate read dephasing in units of cycles per mm */
    read_deph *= read_grad/1000*gcc/100;
    /* add this additional dephasing to the slice dephasing */
    switch(*opt)
    {
      case OVSPulse1:
      case Pulse1:
        St_Gr5=read_grad;
        St_Gp5=St_Gs5=0.0;
        pdp[0] -= read_deph;
        break;
      case OVSPulse2:
      case Pulse2:
        St_Gp5=read_grad;
        St_Gr5=St_Gs5=0.0;
        pdp[1] -= read_deph;
        break;
      case OVSPulse3:
      case Pulse3:
        St_Gs5=read_grad;
        St_Gr5=St_Gp5=0.0;
        pdp[2] -= read_deph;
        break;
      default:
      case NoManAdjustment:
        break;
   
    }
  }

  /* 
   * calculate the final TE/2 spoiling in each direction, dependent on 
   * user requests (edit of grad_dir fields, spoil_dir fields and parameter
   * StSpTE1, StSpTE2.
   */

  if(request==Yes)
  {
    switch(sind)
    {
      case 2:    /* grad_dir1 edited */
      case 3:    /* grad_dir2 edited */
      case 4:    /* grad_dir3 edited */
        switch(parind)
        {
          case 3:  /* StSpTM edited */
            SpoilTypeG2S(&StSpTM,gcc);
            break;
          case 2:  /* StSpTE2 edited */
          default:
            SpoilTypeG2S(&StSpTE2,gcc);
            CalcTESpoil(&StSpTE2,&StSpTE1,pdp,minspoil,57.0,gcc,2);
            break; /* StSpTE1 edited */
          case 1:
            SpoilTypeG2S(&StSpTE1,gcc);
            CalcTESpoil(&StSpTE1,&StSpTE2,pdp,minspoil,57.0,gcc,1);
            break;
        }
        break;
      default: /* spoil_dirx edited*/
      case 1:
      case 5:
      case 6:
      case 7:
        switch(parind)
        {
          case 2: /* StSpTE2 edited */
            CalcTESpoil(&StSpTE2,&StSpTE1,pdp,minspoil,57.0,gcc,2);
            break;
          case 1: /* StSpTE1 edited */
            CalcTESpoil(&StSpTE1,&StSpTE2,pdp,minspoil,57.0,gcc,1);
            break;
        }
        break;
    }
    UT_ClearRequest();
  }
  else
  {

    CalcTESpoil(&StSpTE1,&StSpTE2,pdp,minspoil,57.0,gcc,1);
  }

  /*  
   *  assure TM spoiling at least 3 times larger than during
   *  TE/2 intervals to prevent refocusing of unwanted coherences
   */

  dval  = 3*MAX_OF(StSpTE1.spoil_dir1,StSpTE2.spoil_dir1);
  dval1 = StSpTM.spoil_dir1;
  StSpTM.spoil_dir1=MAX_OF(dval,dval1);



  dval  = 3*MAX_OF(StSpTE1.spoil_dir2,StSpTE2.spoil_dir2);
  dval1 = StSpTM.spoil_dir2;
  StSpTM.spoil_dir2=MAX_OF(dval,dval1);



  dval  = 3*MAX_OF(StSpTE1.spoil_dir3,StSpTE2.spoil_dir3);
  dval1 = StSpTM.spoil_dir3;
  StSpTM.spoil_dir3=MAX_OF(dval,dval1);

  SpoilTypeMaxSpoil(&StSpTM,57.0,gcc);
  SpoilTypeS2G(&StSpTM,gcc);

  /* derive delays and gradientes, used in Pulseprogram */

  St_D0 = riset/1000;
  St_D1 = (StGStabD)/1000.0;
  St_P0 = pul1->Length*1000;
  St_D2 = (StSpTE1.dur-riset)/1000;
  St_D3 = (riset+StGStabD)/1000;
  St_P1 = pul2->Length*1000;
  St_D4 = (StSpTM.dur-riset)/1000;
  St_D5 = (riset+StGStabD)/1000;
  St_P2 = pul3->Length*1000;
  St_D6 = (StSpTE2.dur-riset)/1000;
  St_D7 = (*te/2 - minte2+riset)/1000;
  St_D8 = 0.0; // former DigEndDelOpt
  St_D9 = (*te/2 - minte1)/1000.0;

  if(StGscheme == 1 && vproj==Yes)
  {
    St_D9 += (PVM_AcquisitionTime/2+riset)/1000.0;
  }
  else if (StGscheme == 0)
  {
    St_D3 += St_D9;
    St_D9 = 0.0;
  }

  dval = 1000*(
    St_D0 + 
    St_D1 + 
    St_D2 + 
    St_D3 + 
    St_D4 + 
    St_D5 + 
    St_D6 + 
    St_D7 + 
    St_D9)+
    (St_P0+St_P1+St_P2)/1000;
                               

  if(vproj==Yes)
  {
    dval += PVM_AcquisitionTime;
    dval += 2*St_D0;
  }
  else
  {
    dval += PVM_SpecAcquisitionTime;  
  }


  StDur = dval;

  St_Gr1 = vgrad[0][0];
  St_Gp1 = vgrad[0][1];
  St_Gs1 = vgrad[0][2];

  St_Gr2 = StSpTE1.grad_dir1;
  St_Gp2 = StSpTE1.grad_dir2;
  St_Gs2 = StSpTE1.grad_dir3;
  
  St_Gr3 = StSpTM.grad_dir1;
  St_Gp3 = StSpTM.grad_dir2;
  St_Gs3 = StSpTM.grad_dir3;
  
  St_Gr4 = StSpTE2.grad_dir1;
  St_Gp4 = StSpTE2.grad_dir2;
  St_Gs4 = StSpTE2.grad_dir3;


  dval = effpd[0]+effpd[1] + (St_D2+St_D3+St_D9)*1000;

  DB_MSG(("--> TE1/2: soll %f ist %f",*te/2,dval));

  dval = effpd[2] + (St_D6 + St_D7)*1000;

  DB_MSG(("--> TE2/2: soll %f ist %f",*te/2,dval));

  dval = (St_P1 + St_P2)/2000 + (St_D4 + St_D5)*1000;

  DB_MSG(("--> TM: soll %f ist %f",StTM,dval));

  DB_MSG(("Spoil diff spoil_dir1: soll %f ist %f diff %f"
          "Spoil diff spoil_dir2: soll %f ist %f diff %f"
          "Spoil diff spoil_dir3: soll %f ist %f diff %f",
          pdp[0],
          StSpTE2.spoil_dir1-StSpTE1.spoil_dir1,
          pdp[0]-StSpTE2.spoil_dir1+StSpTE1.spoil_dir1,
          pdp[1],
          StSpTE2.spoil_dir2-StSpTE1.spoil_dir2,
          pdp[1]-StSpTE2.spoil_dir2+StSpTE1.spoil_dir2,
          pdp[2],
          StSpTE2.spoil_dir3-StSpTE1.spoil_dir3,
          pdp[2]-StSpTE2.spoil_dir3+StSpTE1.spoil_dir3));


  /*
   *  make pargroup PPGparameters not visible in editor
   *  but store them into the method file
   */

  ParxRelsHideGroupInEditor("PPGparameters");
  ParxRelsHideInEditor("PPGparameters");
  ParxRelsShowInFile("PPGparameters");
  DB_MSG(("<--UpdateSteamLocalization"));
  return;
}

/*------------------------ Group handler of STEAM localization pars ---------*/

void StHandlerRel(void)
{
  DB_MSG(("-->StHandlerRel"));
  backbone();
  DB_MSG(("<--StHandlerRel"));
  return;
}


/*----------------------- default relations of steam localization pars -------*/

void StGschemeRel(void)
{
  DB_MSG(("-->StGschemeRel"));

  ParxRelsParRelations("StHandler",No);

  DB_MSG(("<--StGschemeRel"));
  return;
}

void StTMRel(void)
{
  DB_MSG(("-->StTMRel"));
  UT_SetRequest("StTM");
  StTMRange();
  ParxRelsParRelations("StHandler",No);
  DB_MSG(("<--StTMRel"));
  return;
}

void StTMRange(void)
{
  double dval,dval1;
  
  DB_MSG(("-->StTMRange"));

  dval = 2*CFG_GradientRiseTime()+CFG_InterGradientWaitTime();
  dval *=3;


  if(!ParxRelsParHasValue("StTM"))
  {
    StTM=MAX_OF(10.0,dval);
  }
  else
  {

    dval1=StTM;
    StTM=MAX_OF(MIN_OF(2000.0,dval1),dval);
  }


  DB_MSG(("<--StTMRange"));
}

/*--------------------- functions to handle spoiling ---------------------*/



/*-------------------------------------------------------------------------
 * deliver max spoiling capacity for given max gradient and timing
 *------------------------------------------------------------------------*/


static double  SpoilTypeMaxSpoil(SPOIL_TYPE *spt,
                                 const double maxgrad,
                                 const double gcc)
{
  double maxspoil=0.0,riset=0.0;
  DB_MSG(("-->SpoilTypeMaxSpoil"));


  riset=CFG_GradientRiseTime();

  if(maxgrad > 100.0 || maxgrad <=0.0 || gcc <= 0.0 || spt->dur <=riset)
  {
    maxspoil=0.0;
  }
  else
  {
    maxspoil=gcc*(spt->dur-riset)*1e-3*maxgrad/100;
                  
    spt->spoil_dir1=MIN_OF(spt->spoil_dir1,maxspoil);
    spt->spoil_dir2=MIN_OF(spt->spoil_dir2,maxspoil);
    spt->spoil_dir3=MIN_OF(spt->spoil_dir3,maxspoil);
    

  }

  DB_MSG(("<--SpoilTypeMaxSpoil"));
  return maxspoil;
}


/*----------------------------------------------------------------
 *
 * Calculate gradient area and spoiling capacity including 
 * refocusing pars according to spdiff (dephasing due to slice
 * selective excitation). Recursively call of the method assures
 * compliance with max gradient amplitudes of both TE/2 spoilers
 *
 *---------------------------------------------------------------*/


static void CalcTESpoil(SPOIL_TYPE *spte1,
                        SPOIL_TYPE *spte2,
                        const double *spdiff,
                        const double *minspoil,
                        const double maxgrad,
                        const double gcc,
                        int mode)
{
  double maxspoil_dir2,maxspoil_dir1,cspdiff[3],fac=1;
  int i;
  DB_MSG(("-->CalcTESpoil Mode: %d",mode));

  maxspoil_dir2=SpoilTypeMaxSpoil(spte2,maxgrad,gcc);
  maxspoil_dir1=SpoilTypeMaxSpoil(spte1,maxgrad,gcc);


  switch(mode)
  {
    default: 
    case 0:
      SpoilTypeS2G(spte1,gcc);
      SpoilTypeS2G(spte2,gcc);
      DB_MSG(("<--CalcTESpoil Mode: %d",mode));
      return;
    case -1:
      mode = 0;
      fac  = 1.0;
      break;
    case  1:
      mode = -2;
      fac=1.0;
     break;
    case -2:
      mode = 0;
      fac  = -1.0;
    case 2:
      mode = -1;
      fac=-1.0;
      break;

  }

  for(i=0;i<3;i++)
  {
    cspdiff[i]=fac*spdiff[i];
  }


  spte1->spoil_dir1=MAX_OF(MIN_OF(maxspoil_dir1,spte1->spoil_dir1),minspoil[0]);
  spte1->spoil_dir2=MAX_OF(MIN_OF(maxspoil_dir1,spte1->spoil_dir2),minspoil[1]);
  spte1->spoil_dir3=MAX_OF(MIN_OF(maxspoil_dir1,spte1->spoil_dir3),minspoil[2]);


  spte2->spoil_dir1=spte1->spoil_dir1 + cspdiff[0];


  if(spte2->spoil_dir1 > maxspoil_dir2)
  {
    spte1->spoil_dir1 -= spte2->spoil_dir1-maxspoil_dir2;
    spte2->spoil_dir1  = maxspoil_dir2;
  }
  
  spte2->spoil_dir2=spte1->spoil_dir2 + cspdiff[1];
  if(spte2->spoil_dir2 > maxspoil_dir2)
  {
    spte1->spoil_dir2 -= spte2->spoil_dir2-maxspoil_dir2;
    spte2->spoil_dir2  = maxspoil_dir2;
  }


  spte2->spoil_dir3=spte1->spoil_dir3 + cspdiff[2];
  if(spte2->spoil_dir3 > maxspoil_dir2)
  {
    spte1->spoil_dir3 -= spte2->spoil_dir3-maxspoil_dir2;
    spte2->spoil_dir3 = maxspoil_dir2;
  }

  DB_MSG(("<--CalcTESpoil Mode: %d",mode));
  CalcTESpoil(spte2,spte1,spdiff,minspoil,maxgrad,gcc,mode);


  
  DB_MSG(("<--CalcTESpoil Mode: %d",mode));
}

/*--------------------------------------------------------------------
 *
 * converts spoiling capacity into gradient amplitudes of spoilers
 *
 *-------------------------------------------------------------------*/


static void  SpoilTypeS2G(SPOIL_TYPE  *spt,double gcc)
{
  double dval=0,riset;
  DB_MSG(("-->SpoilTypeS2G"));


  riset=CFG_GradientRiseTime();

  if(gcc>0.0&&spt->dur>0.0)
  {
    dval = 100.0/(gcc*0.001*(spt->dur-riset));
    spt->grad_dir1=dval*spt->spoil_dir1;
    spt->grad_dir2=dval*spt->spoil_dir2;
    spt->grad_dir3=dval*spt->spoil_dir3;
  }

  DB_MSG(("<--SpoilTypeS2G"));
  return;
}


/*--------------------------------------------------------------------
 *
 * derives spoiling capacity from gradient amplitudes and duration
 * of spoilers
 *
 *-------------------------------------------------------------------*/



static void  SpoilTypeG2S(SPOIL_TYPE  *spt,double gcc)
{
  double dval=0,riset;
  DB_MSG(("-->SpoilTypeG2S"));

  riset=CFG_GradientRiseTime();

  if(gcc>0.0&&spt->dur>0.0)
  {
    dval = gcc*0.001*(spt->dur-riset)/100.0;
    spt->spoil_dir1=dval*spt->grad_dir1;
    spt->spoil_dir2=dval*spt->grad_dir2;
    spt->spoil_dir3=dval*spt->grad_dir3;
  }
  
  DB_MSG(("<--SpoilTypeG2S"));
  return;
}

/*----------------------------------------------------------------
 * initializes parameters of type SPOIL_TYPE
 *---------------------------------------------------------------*/

static void  SpoilTypeInit(SPOIL_TYPE *spt)
{
  double defdur;

  DB_MSG(("-->SpoilTypeInit"));

  memset(spt,0,sizeof(SPOIL_TYPE));


  defdur = 2*CFG_GradientRiseTime()+CFG_InterGradientWaitTime();
  defdur = MAX_OF(defdur,2.0);
  spt->dur = defdur;  

  spt->spoil_dir1=spt->spoil_dir2=spt->spoil_dir3=5.0;
  

  DB_MSG(("-->SpoilTypeInit"));
}

/*-------------------------------------------------------------------
 * default range checking functions for parameters of type SPOIL_TYPE
 *------------------------------------------------------------------*/


static YesNo SpoilTypeRange(SPOIL_TYPE *spt,double maxgrad,double mingrad)
{
  YesNo retval=No;
  double mindur=0;


  DB_MSG(("-->SpoilTypeRange"));

  mindur=2*CFG_GradientRiseTime()+CFG_InterGradientWaitTime();

  if(maxgrad >100.0)
  {
    retval = Yes;
    maxgrad = 100.0;
  }
  if(mingrad < -100.0)
  {
    retval = Yes;
    mingrad = -100.0;
  }

  if(spt->dur < mindur)
  {
    retval = Yes;
    spt->dur=mindur;
  }

  if(spt->grad_dir1>maxgrad)
  {
    retval = Yes;
    spt->grad_dir1=maxgrad;
  }
  if(spt->grad_dir1<mingrad)
  {
    retval = Yes;
    spt->grad_dir1=mingrad;
  }

  if(spt->grad_dir2>maxgrad)
  {
    retval = Yes;
    spt->grad_dir2=maxgrad;
  }
  if(spt->grad_dir2<mingrad)
  {
    retval = Yes;
    spt->grad_dir2=mingrad;
  }

  if(spt->grad_dir3>maxgrad)
  {
    retval = Yes;
    spt->grad_dir3=maxgrad;
  }
  if(spt->grad_dir3<mingrad)
  {
    retval = Yes;
    spt->grad_dir3=mingrad;
  }
  
  DB_MSG(("<--SpoilTypeRange"));
  return retval;
}


/*---------------------------------------------------------------------
 * Default relations and range checking of Steam parameter group
 *--------------------------------------------------------------------*/


void StSpTE1Range(void)
{
  DB_MSG(("-->StSpTE1Range"));

  if(!ParxRelsParHasValue("StSpTE1"))
  {
    SpoilTypeInit(&StSpTE1);
  }


  SpoilTypeRange(&StSpTE1,57.0,0.0);

  DB_MSG(("<--StSpTE1Range"));
}

void StSpTE1Rel(void)
{
  DB_MSG(("-->StSpTE1Rel"));

  UT_SetRequest("StSpTE1");
  StSpTE1Range();
  ParxRelsParRelations("StHandler",No);

  DB_MSG(("<--StSpTE1Rel"));
}



void StSpTE2Range(void)
{
  DB_MSG(("-->StSpTE2Range"));

  if(!ParxRelsParHasValue("StSpTE2"))
  {
    SpoilTypeInit(&StSpTE2);
  }


  SpoilTypeRange(&StSpTE2,57.0,0.0);

  DB_MSG(("<--StSpTE2Range"));
}

void StSpTE2Rel(void)
{
  DB_MSG(("-->StSpTE2Rel"));

  UT_SetRequest("StSpTE2");
  StSpTE2Range();
  ParxRelsParRelations("StHandler",No);

  DB_MSG(("<--StSpTE2Rel"));
}

void StSpTMRange(void)
{
  DB_MSG(("-->StSpTMRange"));

  if(!ParxRelsParHasValue("StSpTM"))
  {
    SpoilTypeInit(&StSpTM);
    StSpTM.spoil_dir1=StSpTM.spoil_dir2=StSpTM.spoil_dir3=30.0;
  }

  SpoilTypeRange(&StSpTM,57.0,0.0);

  DB_MSG(("<--StSpTMRange"));
}

void StSpTMRel(void)
{
  DB_MSG(("-->StSpTMRel"));

  UT_SetRequest("StSpTM");
  StSpTMRange();
  ParxRelsParRelations("StHandler",No);

  DB_MSG(("<--StSpTMRel"));
}

void StGStabDRange(void)
{
  DB_MSG(("-->StGStabDRange"));

  if(!ParxRelsParHasValue("StGStabD"))
  {
    StGStabD=0;
  }
  else
  {
    double max,val;

    max=3*CFG_GradientRiseTime();
    val = StGStabD;

    StGStabD=MAX_OF(MIN_OF(max,val),0);

  }

  DB_MSG(("<--StGStabDRange"));

}

void StGStabDRel(void)
{
  DB_MSG(("-->StGStabDRel"));

  StGStabDRange();
  ParxRelsParRelations("StHandler",No);

  DB_MSG(("<--StGStabDRel"));

}

void StDurRel(void)
{
  DB_MSG(("-->StDurRel"));

  ParxRelsParRelations("StHandler",No);

  DB_MSG(("<--StDurRel"));
}
