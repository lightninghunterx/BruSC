/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2008-2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

static const char resid[] = "$Id$ (C) 2009 Bruker BioSpin MRI GmbH";

#define DEBUG		0
#define DB_MODULE	0
#define DB_LINE_NR	0

/*:=INFO=:*******************************************************
 *
 * Description :
 *   it contains functions necessary for responding to a 'load'
 *	of this method. This function will be automatically
 *	called in response to a method file for this method
 *	having been read.
 *
 *::=info=:******************************************************/

/****************************************************************/
/****************************************************************/
/*		I N T E R F A C E   S E C T I O N		*/
/****************************************************************/
/****************************************************************/

/****************************************************************/
/*		I N C L U D E   F I L E S			*/
/****************************************************************/

#include "method.h"

/****************************************************************/
/*	I M P L E M E N T A T I O N   S E C T I O N		*/
/****************************************************************/

/* -----------------------------------------------------------
   Relations of ExcPulse
 
   -----------------------------------------------------------*/
   
void ExcPulseEnumRelation(void)
{
  DB_MSG(("-->ExcPulseEnumRelation"));

  UT_SetRequest("ExcPulseEnum");
  backbone();
  
  DB_MSG(("<--ExcPulseEnumRelation"));                                       
}

void ExcPulRelation(void)
{
  DB_MSG(("-->ExcPulRelation"));
  
  UT_SetRequest("ExcPul");
  ExcPulRange();
  backbone();
  
  DB_MSG(("<--ExcPulRelation"));
}

void ExcPulRange(void)
{
  DB_MSG(("-->ExcPulse1Range"));
    
 ExcPul.Flipangle = MIN_OF(90.0,MAX_OF(ExcPul.Flipangle,1.0));

 DB_MSG(("<--ExcPulseRange")); 
}

void ExcPulseAmplRel(void)
{
  DB_MSG(("-->ExcPulseAmplRel"));
  
  UT_SetRequest("ExcPulseAmpl");
  HandleRFPulseAmplitudes();
  
  DB_MSG(("-->ExcPulseAmplRel"));
}

/* --------------------------------------------------------------
   Relations of RefPul

   --------------------------------------------------------------*/

void RefPulseEnumRelation(void)
{
  DB_MSG(("-->RefPulseEnumRelation"));

  UT_SetRequest("RefPulseEnum");
  backbone();
  
  DB_MSG(("<--RefPulseEnumRelation"));                                       
}

void RefPulRelation(void)
{
  DB_MSG(("-->RefPulRelation"));
  
  UT_SetRequest("RefPul");
  RefPulRange();
  backbone();
  
  DB_MSG(("<--RefPulRelation"));
}

void RefPulRange(void)
{
  DB_MSG(("-->RefPulse1Range"));
    
 RefPul.Flipangle = MIN_OF(180.0,MAX_OF(RefPul.Flipangle,1.0));

 DB_MSG(("<--RefPulseRange")); 
}

void RefPulseAmplRel(void)
{
  DB_MSG(("-->RefPulseAmplRel"));
  
  UT_SetRequest("RefPulseAmpl");
  HandleRFPulseAmplitudes();
  
  DB_MSG(("-->RefPulseAmplRel"));
}

/* --------------------------------------------------------------
   Relations of InvPul

   --------------------------------------------------------------*/

void InvPulseEnumRelation(void)
{
  DB_MSG(("-->InvPulseEnumRelation"));

  UT_SetRequest("InvPulseEnum");
  backbone();
  
  DB_MSG(("<--InvPulseEnumRelation"));                                       
}

void InvPulRelation(void)
{
  DB_MSG(("-->InvPulRelation"));
  
  UT_SetRequest("InvPul");
  InvPulRange();
  backbone();
  
  DB_MSG(("<--InvPulRelation"));
}

void InvPulRange(void)
{
  DB_MSG(("-->InvPulse1Range"));
    
 InvPul.Flipangle = MIN_OF(180.0,MAX_OF(InvPul.Flipangle,1.0));

 DB_MSG(("<--InvPulseRange")); 
}

void InvPulseAmplRel(void)
{
  DB_MSG(("-->InvPulseAmplRel"));
  
  UT_SetRequest("InvPulseAmpl");
  HandleRFPulseAmplitudes();
  
  DB_MSG(("-->InvPulseAmplRel"));
}

void HandleRFPulseAmplitudes(void)
{
  DB_MSG(("-->HandleRFPulseAmplitude"));

  STB_UpdateRFShapeAmplitude("ExcPulseAmpl,RefPulseAmpl,InvPulseAmpl",No);
  ATB_SetRFPulse("ExcPul","ACQ_RfShapes[0]");
  ATB_SetRFPulse("RefPul","ACQ_RfShapes[1]");
  ATB_SetRFPulse("InvPul","ACQ_RfShapes[2]");

  DB_MSG(("<--HandleRFPulseAmplitude"));
}


/* -----------------------------------------------------------
   Further contraints for InpSlices 
 
   -----------------------------------------------------------*/

void InpSlicesRel(void)
{
  int i,r,d;

  DB_MSG(("-->InpSlicesRel"));

  r=InpSlices % SliceGroupSize;
  d=(int)(InpSlices / SliceGroupSize);

  if(r!=0) InpSlices=(d+1)*SliceGroupSize;
  for(i=0;i<PVM_NSPacks;i++) PVM_SPackArrNSlices[i]=InpSlices;

 /* ToDo: Result could be larger than allowed maximum nr. slices */

  DB_MSG(("<--InpSlicesRel"));

}


/* -----------------------------------------------------------
   Relation of InpSlices 
 
   -----------------------------------------------------------*/
void InpSlicesRelation(void)
{
  int i;

  DB_MSG(("-->InpSlicesRelation"));

  /* Tell the request handling system that the parameter
     InpSlices has been edited */
  UT_SetRequest("InpSlices"); 

  /* Check the values of InpSlices */
  InpSlicesRange();
  for(i=0;i<PVM_NSPacks;i++) PVM_SPackArrNSlices[i]=InpSlices;
   
  /* call the backbone; further handling will take place there */
 
  backbone();

  DB_MSG(("<--InpSlicesRelation"));
}

/* --------------------------------------------------------------
   Range checking of InpSlices

   --------------------------------------------------------------*/
void InpSlicesRange(void)
{
  DB_MSG(("-->InpSlicesRange"));
  
  if(ParxRelsParHasValue("InpSlices") == No)
    InpSlices = 1;  

/* allowed clasification */

  InpSlices=MAX_OF(InpSlices,1);
  InpSlices=MIN_OF(InpSlices,1024);  /* maximum number of slices */
 
  DB_MSG(("<--InpSlicesRange"));
}

/* -----------------------------------------------------------
   Relation of SliceGroupSize
 
   -----------------------------------------------------------*/
void SliceGroupSizeRelation(void)
{
  DB_MSG(("-->SliceGroupSizeRelation"));

  /* Tell the request handling system that the parameter
     SliceGroupSize has been edited */
  UT_SetRequest("SliceGroupSize"); 

  /* Check the value of SliceGroupSize */
  SliceGroupSizeRange();

  /* call the backbone; further handling will take place there */
 
  backbone();

  DB_MSG(("<--SliceGroupSizeRelation"));
}

/* --------------------------------------------------------------
   Range checking of SliceGroupSize

   --------------------------------------------------------------*/
void SliceGroupSizeRange(void)
{
  DB_MSG(("-->SliceGroupSizeRange"));
  
  if(ParxRelsParHasValue("SliceGroupSize") == No)
    SliceGroupSize = 1;  

/* allowed clasification */

  SliceGroupSize=MAX_OF(SliceGroupSize,1);
  SliceGroupSize=MIN_OF(SliceGroupSize,100);

  DB_MSG(("<--SliceGroupSizeRange"));

}

/* -----------------------------------------------------------
   Relation of NIRPoints
 
   -----------------------------------------------------------*/
void NIRPointsRelation(void)
{
  DB_MSG(("-->NIRPPointsRelation"));

  /* Tell the request handling system that the parameter
     SliceGroupSize has been edited */
  UT_SetRequest("NIRPoints"); 

  /* Check the values of NIRPoints */
  NIRPointsRange();

  /* call the backbone; further handling will take place there */
 
  backbone();

  DB_MSG(("<--NIRPointsRelation"));
}

/* --------------------------------------------------------------
   Range checking of NIRPoints

   --------------------------------------------------------------*/
void NIRPointsRange(void)
{
  DB_MSG(("-->NIRPointsRange"));
  
  if(ParxRelsParHasValue("NIRPoints") == No)
    NIRPoints = 8;  

/* allowed clasification */

  NIRPoints=MAX_OF(NIRPoints,1);
  NIRPoints=MIN_OF(NIRPoints,100);

  DB_MSG(("<--NIRPointsRange"));

}


/* -----------------------------------------------------------
   Further contraints for NIRPoints 
 
   -----------------------------------------------------------*/

void NIRPointsRel(void)
{
  int r,d;

  DB_MSG(("-->NIRPointsRel"));

  if(NIRPoints<SliceGroupSize) NIRPoints=SliceGroupSize;

  r=NIRPoints % SliceGroupSize;
  d=(int)(NIRPoints / SliceGroupSize);

  if(r!=0) NIRPoints=(d+1)*SliceGroupSize;

 /* ToDo: Result could be larger than allowed maximum nr. time points */

  DB_MSG(("<--NIRPointsRel"));

}


/* -----------------------------------------------------------
   Relation of IRSpacing
 
   -----------------------------------------------------------*/
void IRSpacingRelation(void)
{
  DB_MSG(("-->IRSpacingRelation"));

  /* Tell the request handling system that the parameter
     IRSpacing has been edited */
  UT_SetRequest("IRSpacing");

  /* Check the values of IRSpacing */
  IRSpacingRange();

  /* call the backbone; further handling will take place there */
 
  backbone();

  DB_MSG(("<--IRSpacingRelation"));
}

/* --------------------------------------------------------------
   Range checking of IRSpacing

   --------------------------------------------------------------*/
void IRSpacingRange(void)
{
  DB_MSG(("-->IRSpacingRange"));
  
  /* allowed clasification */

  if(ParxRelsParHasValue("IRSpacing") == No)
    IRSpacing = 100.0;
  
  IRSpacing = MAX_OF(IRSpacing,0.0);
  IRSpacing = MIN_OF(IRSpacing,10000.0);

  DB_MSG(("<--IRSpacingRange"));

}


/* --------------------------------------------------------------
   Range checking of IROffset

   --------------------------------------------------------------*/
void IROffsetRange(void)
{
  DB_MSG(("-->IROffsetRange"));
  
  /* allowed clasification */

  if(ParxRelsParHasValue("IROffset") == No)
    IROffset = 20.0;
  
  IROffset = MAX_OF(IROffset,0.0);
  IROffset = MIN_OF(IROffset,1000.0);

  DB_MSG(("<--IROffsetRange"));

}


/* -----------------------------------------------------------
   Relation of IROffset
 
   -----------------------------------------------------------*/
void IROffsetRelation(void)
{
  DB_MSG(("-->IROffsetRelation"));

  /* Tell the request handling system that the parameter
     IROffset has been edited */
  UT_SetRequest("IROffset");

  /* Check the values of IROffset */
  IROffsetRange();

  /* call the backbone; further handling will take place there */
 
  backbone();

  DB_MSG(("<--IROffsetRelation"));
}




/* -----------------------------------------------------------
   Relation RotationRelation (for GroupSizeRotation)
 
   -----------------------------------------------------------*/
void RotationRelation(void)
{
  DB_MSG(("-->RotationRelation"));

  /* Tell the request handling system that the yes/no parameter
     Permutation has been edited */
  UT_SetRequest("RotationRelation");

  /* call the backbone; further handling will take place there */
 
  backbone();

  DB_MSG(("<--RotationRelation"));
}

void PackDelRange( void )
{
  if(ParxRelsParHasValue("PackDel") == No)
    PackDel = 0.0;
  PackDel = MAX_OF(PackDel,0.0);
}

void PackDelRelation( void )
{
  PackDelRange();
  backbone();
}

void RephaseTimeRels(void)
{
  RephaseTimeRange();
  backbone();
}

void RephaseTimeRange(void)
{
  if(!ParxRelsParHasValue("RephaseTime"))
    RephaseTime = 2.0;

  RephaseTime = MAX_OF(RephaseTime, 2*CFG_GradientRiseTime());
}

void Local_nslicesHandler(void)
{
  Local_nslicesRange();
  backbone();
}

void Local_nslicesRange(void)
{
  int i;

  /*
  if(!ParxRelsParHasValue("PVM_NSPacks"))
    PVM_NSPacks=1;
  */

  if(!ParxRelsParHasValue("PVM_SPackArrNSlices")) {
    PARX_change_dims("PVM_SPackArrNSlices",PVM_NSPacks);
    for(i=0;i<PVM_NSPacks;i++) PVM_SPackArrNSlices[i]=InpSlices;
  }
  InpSlices=PVM_SPackArrNSlices[0];
  InpSlicesRange();
  for(i=0;i<PVM_NSPacks;i++) PVM_SPackArrNSlices[i]=InpSlices;

}



void Local_NAveragesRange(void)
{
  int ival;
  DB_MSG(("Entering Local_NAveragesRange"));
  
  /* 
   *  Range check of PVM_NAverages: prevent it to be negative or 0
   */

  if(ParxRelsParHasValue("PVM_NAverages") == No)
    {
      PVM_NAverages = 1;
    }

  ival = PVM_NAverages;
  PVM_NAverages = MAX_OF(ival,1);
  

  DB_MSG(("Exiting Local_NAveragesRange"));


}

void Local_NAveragesHandler(void)
{

  DB_MSG(("Exiting Local_NAveragesHandler with value %d",PVM_NAverages));

  Local_NAveragesRange();

  /*
   *   Averages range check is finished, handle the request by
   *   the method:
   */

  
  backbone();


  DB_MSG(("Exiting Local_NAveragesHandler with value %d",PVM_NAverages));
  return;
}
void LocalSWhRange(void)
{
  DB_MSG(("-->LocalSWhRange"));
  double minmax[2]={50000.0,1000000.0};

  if(!ParxRelsParHasValue("PVM_EffSWh"))
    PVM_EffSWh=200000.0;

  MRT_GetNearestBandwidthInHz("PVM_EffSWh",minmax);

  DB_MSG(("<--LocalSWhRange"));
}

void LocalSWhRels(void)
{
  DB_MSG(("-->LocalSWhRel"));

  LocalSWhRange();
  backbone();

  DB_MSG(("<--LocalSWhRel"));
  return;
}


/* relations of NSegments */

void NSegmentsRels(void)
{
  NSegmentsRange();
  backbone();
}

void NSegmentsRange(void)
{
  if(!ParxRelsParHasValue("NSegments"))
    NSegments = 1;
  else
    NSegments = MAX_OF(1,NSegments);
}

/* relations of slice spoiler */
void SliceSpoilerRel(void)
{
  DB_MSG(("-->SliceSpoilerRel"));
  UT_SetRequest("SliceSpoiler");
  backbone();
  DB_MSG(("<--SliceSpoilerRel"));
}

void InvSliceSpoilerRel(void)
{
  DB_MSG(("-->InvSliceSpoilerRel"));
  UT_SetRequest("InvSliceSpoiler");
  backbone();
  DB_MSG(("<--InvSliceSpoilerRel"));
}


/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
