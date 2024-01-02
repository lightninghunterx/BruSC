/* ***************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/pvm_toolboxes/PvSeqTools/Attic/FovSatHandler.c,v $
 *
 * Copyright (c) 2018
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

static const char resid[] = "$Id$ (C) 2001-2006 Bruker BioSpin MRI GmbH";

#define DEBUG           0
#define DB_MODULE       1
#define DB_LINE_NR      1

#include "method.h"

#ifndef CPROTO
#include "pvmachine.h"
#include "libCore/Math/Matrix3x3.hh"
#include "PvUtil/PvExcept.hh"

#define SLICE_PARIMPORT_ID "CopyParameterGroup_Slice"

void deriveSliceGobj();
void transferFromSliceGobj(void);


void deriveSliceGobj()
{
  try
  {
    bool hasval=true;
    hasval = ParxRelsParHasValue("SliceGeoCub") ? true:false;

    GObject slice("SliceGeoCub");

    int nsl=(int) PARX_get_dim("SliceOri",1);
    if(No==UT_ArrParsHaveSameDimension("SliceOri,"
                                       "SliceThick,"
                                       "SliceOffset"))
    {
      throw PvStdException("Inconsistent size of slice parameters!");
    }
    int onsl=slice.getNCubes();
    int selind=0;
    bool gobjhasval= ParxRelsParHasValue("SliceGeoObj") ? true:false;

    if(gobjhasval)
    {
      // selected object
      selind = SliceGeoObj.selInd;
    }
    
    if(onsl!=nsl)
    {
      slice.setNCubes(nsl,selind);
    }

    double *slthk=SliceThick;
    double *soffs=SliceOffset;
    SLOR_TYPE *slor=SliceOri;
    double ext[3];
    SLOR_TYPE curslor=axial;

    std::string parEdited=UT_RelContext_ParName();

    if(parEdited=="SliceOri")
    {
      std::vector<int> idx=UT_RelContext_ParArrayIndices();
      slice.setSlorToOri(SliceOri[idx[0]],idx[0]);
    }

    

    for(int i=0;i<nsl;i++)
    {
      slice.setNSubc(1,1,1,i);
      // handle orientations
      
 
      if(hasval== false || curslor!=slor[i])
      {
        slice.setSlorToOri(slor[i],i);
      }
      // handle extents
      slice.getAcqFov(ext,i);
      ext[2]=slthk[i];
      slice.setThickness(slthk[i],i);
      slice.setExtFromAcqExt(ext,i);
      // handle positions
      slice.setDistToPos(soffs[i],i,true); // origin in object
    }// end slices loop
    // set labels
    if(gobjhasval)
    {
      char *labels[3];
      labels[0] = SliceGeoObj.label[0];
      labels[1] = SliceGeoObj.label[1];
      labels[2] = SliceGeoObj.label[2];

      slice.setAxisLabels(labels);
    }
    else
    {
      const char *label[3] = {"1;first","2;second","3;third"};
      slice.setAxisLabels(label);
    }
    slice.setCuboidPar();
  }
  catch(const PvException &ex)
  {
    throw PvStdException("Derive geometry object failed. Reason:\n%s",
                   ex.what());        
  }

  return;
}

void transferFromSliceGobj(void)
{
  try
  {
    GObject slice("SliceGeoCub");

    int nsl=0;
    if((nsl=slice.getNSubCubes()) != slice.getNCubes())
    {
      throw PvStdException("Number of slices %d "
                           "not equal to number of cubes %d",
                           slice.getNSubCubes(),slice.getNCubes());
    }
    if( (int) PARX_get_dim("SliceOri",1) != nsl ||
        No == UT_ArrParsHaveSameDimension("SliceOri,"
                                          "SliceThick,"
                                          "SliceOffset"))
    {
      UT_RedimensionListOf1d(nsl,
                             "SliceOri,"
                             "SliceThick,"
                             "SliceOffset");
    }

    SliceNum = nsl;

    SLOR_TYPE *fslor = SliceOri;
    unsigned int firstdir=0;
    double *fsthk    = SliceThick;
    double *fsoffs   = SliceOffset;

    for(int i=0;i<nsl;i++)
    {
      slice.getOriToSlor(fslor+i,&firstdir,i);
      slice.getAcqSubcThick(fsthk+i,i);
      slice.getPosToDist(fsoffs[i],i,true);//origin in object
    }


  }
  catch(const PvException &ex)
  {
    throw PvStdException("Transfer from geometry object failed. Reason:\n%s",
                   ex.what());        
  }

  return;
}

#endif //CPROTO

/*** relations and range checkers ***/

void CopySliceParGroup(void)
{
  DB_MSG(("--> CopySlicePars"));

  if(!strcmp(PVM_ImportID,SLICE_PARIMPORT_ID))
  {
    PARIMPORT_LIST_TYPE slparlist[] =
    {
      {"SliceSelOnOff"       , "SliceSelonOff" , ""           , No },
      {"Slice_Sel_Parameters", ""              ,"SliceHandler", No }
    };

    PTB_SetImportList(slparlist, 2);

  }
  else
  {
    STB_ImportID();
  }

  DB_MSG(("<-- CopySlicePars"));
}

void SliceRelation()
{
  DB_MSG(("--> SliceRelation"));
  
  ParxRelsParRelations("SliceHandler", No);
  
  DB_MSG(("<-- SliceRelation"));
}


void SliceGeoObjRelation()
{
  DB_MSG(("--> SliceGeoObjRelation"));
  
  UT_SetRequest("SliceGeoObj");
  GTB_HandleGeoObject("SliceGeoObj");
  
  DB_MSG(("<-- SliceGeoObjRelation"));
}

void SliceGeoCubRelation(void)
{
  DB_MSG(("--> SliceGeoCubRelation"));

  SliceGeoCubRange();
  //called during initialisation of geometry editor: fast update
  if (UT_GetRequest("PVM_GeoObjHandler") == Yes &&
      PVM_GeoObjHandler == GeoInit)
  {
    DB_MSG(("<-- SliceGeoCubRelation"));
    return;
  }

  GTB_GeoObjRelationCalls("SliceGeoObj", "SliceHandler");

  DB_MSG(("<-- SliceGeoCubRelation"));
}


void SliceGeoCubRange()
{
   if(!ParxRelsParHasValue("SliceGeoCub"))
    {
      double ext[3];
      CFG_DefaultExtent(ext);
      ext[2]=CFG_DefaultSliceThickness();
      GObject slice("SliceGeoCub");
      slice.initCuboidArr();
      slice.setExtFromAcqExt(ext,0);
      slice.setCuboidPar();
    }
}


void SliceHandlerRelation(void)
{
  DB_MSG(("--> SliceHandlerRelation"));

  if(No==GTB_GeoEditorPlanningMode())
    backbone();

  DB_MSG(("<-- SliceHandlerRelation"));
  return;
}



/*** toolbox functions ***/

void InitSliceSelection()
{
  DB_MSG(("--> InitSliceSelection"));

  double riseT = CFG_GradientRiseTime();

  if (!ParxRelsParHasValue("SliceSelOnOff"))
    SliceSelOnOff = Off;

  if (!ParxRelsParHasValue("SliceRephDur"))
    SliceRephDur = riseT;

  if (!ParxRelsParHasValue("EchoTime"))
    EchoTime = DeadTime + 0.5*ExcPulse1.Length;

  SliceGeoCubRange();
  transferFromSliceGobj();  

  DB_MSG(("<-- InitSliceSelection"));
}


void UpdateSliceSelection(const char *nucStr)
{
  DB_MSG(("--> UpdateSliceSelection"));

 try
  {
    std::vector<std::string> cardparameter = \
      {
        "SliceSelOnOff",
        "SliceNum",
        "SliceThick",
        "SliceOri",
        "SliceOffset"
      };
 
    SliceGeoCubRange();

    if(SliceSelOnOff==On)
      STB_UpdateAtsReferencePosition("SliceGeoObj");
    else
      STB_UpdateAtsReferencePosition();

    std::string parEdited=UT_RelContext_ParName();

    std::vector<std::string>::iterator 
      foundParam=std::find(cardparameter.begin(),
                           cardparameter.end(),
                           parEdited);


    if(foundParam!=cardparameter.end())
    {
      if(*foundParam=="SliceSelOnOff")
      {
        GObject slice("SliceGeoCub");
        slice.setModalityOffsets(PVM_AtsReferencePosition,
                                 SysInfo_AtsPrecision());
        slice.setCuboidPar();
      }
      else
        deriveSliceGobj();
    }

    /* geometry */

    if (parEdited=="SliceGeoCub")
    {
      // changed directly
      GTB_HandleGeoCuboid("SliceGeoCub", SliceGeoObj.handle);
      GTB_GeoObjDependencyApply("SliceGeoObj");
    }
    else if (GTB_GeoObjDependencySlave("SliceGeoObj") ||
             GTB_GeoObjDependencyIsSlave("SliceGeoObj"))
    {
      // changed as slave or PVM parameters changed
      GTB_GeoObjDependencyApply("SliceGeoObj");
    }

    transferFromSliceGobj();

    double effPulseDur = ExcPulse1.Length * (ExcPulse1.Rpfac/100);

    /* nucleus argument check */
    if(nucStr == NULL || CFG_IsNucleusKnown(nucStr) == No)
      throw PvStdException("Argument 1 : illegal nucleus information!\n");

    /* visibility of parameters and geo object */
    if (SliceSelOnOff == Off)
    {

      /* reset */
      SliceRephDur     = 0.0;
      SliceSpoilDur    = 0.0;
      SliceNum         = 1;

      double *rephgrad = SliceRephGrad;
      double *grad     = SliceGrad;

      memset(rephgrad, 0.0, SliceNum *3*sizeof(double));
      memset(grad, 0.0, SliceNum *3*sizeof(double));

      ParxRelsHideInEditor("Slice_Sel_Parameters");

      EchoTime = effPulseDur + DeadTime;

      PARX_change_dims("SliceFL", 1 );
      SliceFL[0]=0.0;

      PTB_RemoveGeoObject("SliceGeoObj");
    }
    else
    {
      int nsl=SliceNum;
 
      ParxRelsShowInEditor("Slice_Sel_Parameters");

      //to do: RefScan not yet adapted t multi-channel-multi-spec
      //       Rx Phase Adjustment currently only single-chann
      if (nsl>1)
      {
        PVM_RefScanYN=No;
        PVM_ArrayPhaseAdjMode=ArrayPhase_Adj_Skip;
      }

      PARX_change_dims("SliceGrad",      nsl, 3);   // used in ppg
      PARX_change_dims("SliceRephGrad",  nsl, 3);   // used in ppg
      PARX_change_dims("SliceVec",       nsl, 3);   // for visu
      PARX_change_dims("SliceFL",        nsl   );   // frequency list
      PARX_change_dims("SliceOrderList", nsl   );

      //to do: requires a RecoResortFilter if changed for multi-slice
      GTB_SliceOrder( SliceNum, Sequential, SliceOrderList );

      const char *const label[3] = {"1;first", "2;second", "3;third"};

      PTB_InsertGeoObject( "Slice",
                           GeoPlanarId,
                           Standard_GobjShape,
                           "SliceGeoObj",
                           "SliceGeoCub",
                           "SliceOrderList",
                           label,
                           Yes);

      GTB_GeoObjExtentThickness("SliceGeoObj", No);

      GObject slice("SliceGeoCub");

      double *svec     = 0;
      double ori[9]    = {0,0,0,0,0,0,0,0,0};
      double *grad     = SliceGrad;
      double *fl       = SliceFL;
      double *slicevec = SliceVec;
      double gcc       = CFG_GradCalConst(nucStr);
      double pbw       = ExcPulse1.Bandwidth;
      double minthick  = MRT_SliceThickness(pbw,95.0 /*max grad [%]*/, gcc);
      double thick     = 0;
      double gamp[nsl];
      double maxgamp   = 0;
      double modoffs   = 0;

      for(int i=0;i<nsl;i++,grad+=3,slicevec+=3)
      {
        // handle thickness
        slice.getAcqSubcThick(&thick,i);
        thick=MAX_OF(minthick,thick);
        slice.setThickness(thick,i);
        gamp[i]=MRT_SliceGrad(pbw,thick,gcc);

        maxgamp=MAX_OF(maxgamp,gamp[i]);

        // SliceGrad in modality system
        slice.getOriToAcq(ori,i);
        svec=ori+6;

        memcpy(grad,svec,3*sizeof(double));

        grad[0]*=gamp[i],grad[1]*=gamp[i],grad[2]*=gamp[i];

        // SliceVec in object system pat PV
        slice.getOriToAcqObj(ori,i,false);
        svec=ori+6;
        memcpy(slicevec,svec,3*sizeof(double));

        // calculate distance to modality
        slice.getPosToDist(modoffs,i,false);
        fl[i]=MRT_FrequencyOffset(modoffs,gamp[i],gcc);
      }

      // set final values of geometry object to cuboid
      slice.setCuboidPar();

      // make gobj and interface parameter consistent
      transferFromSliceGobj();

      /* rephase*/
      double *rephslicevec=SliceVec;
      double *rephgrad=SliceRephGrad;
      double rephgamp;
      double riseT = CFG_GradientRiseTime();
      double maxrephgamp = 95.0, minrephgamp = 1.0; //limits n [%]
      double minrephdur, maxrephdur;
      double minEchoTime=0.0;

      //min dur to rephase maxgamp using maxrephgamp
      double minrephT = MRT_DephaseTime(effPulseDur, riseT, maxgamp, maxrephgamp);
      //MRT_DephaseTime does not consider rise time restrictions!
      minrephdur = MAX_OF(riseT, minrephT);

      //maxrephdur by rule of 3:
      maxrephdur = minrephT * maxrephgamp / minrephgamp;

      minEchoTime = effPulseDur
                   + riseT            /* ramp down slice grad       */
                   + minrephdur       /* ramp up & plateau rephgrad */
                   + riseT            /* ramp down reph grad        */
                   + DeadTime;

      SliceRephDur = MIN_OF( maxrephdur, MAX_OF(minrephdur, SliceRephDur)) ;

      EchoTime = minEchoTime - minrephdur + SliceRephDur;


      for(int i=0;i<nsl;i++,rephgrad+=3,rephslicevec+=3)
      {
        slice.getOriToAcq(ori,i);
        svec=ori+6;

        rephgamp = (MRT_DephaseGrad( SliceRephDur, effPulseDur, riseT, gamp[i] ));

        memcpy(rephgrad,svec,3*sizeof(double));

        //I use this ternary stuff to prevent '0' from getting a '-0'
        rephgrad[0] = rephgrad[0]==0 ? 0 : (-1)*rephgamp*rephgrad[0];
        rephgrad[1] = rephgrad[1]==0 ? 0 : (-1)*rephgamp*rephgrad[1];
        rephgrad[2] = rephgrad[2]==0 ? 0 : (-1)*rephgamp*rephgrad[2];
      }

      /* spoiler */
      double spoilcycles = 2; 
      //use slicegrad ampl also for spoiling:
      //spoildur = 100 * spoilcycles/(gcc * 1e-3 * minslthick * maxgamp) with
      //denominator = 1000 and ...
      SliceSpoilDur = spoilcycles/10;
  }
  }
  catch(const PvException &ex)
  {
    PvStdException("UpdateSliceSelection failed. Reason:\n%s", ex.what());
  }

  DB_MSG(("<-- UpdateSliceSelection"));
}
