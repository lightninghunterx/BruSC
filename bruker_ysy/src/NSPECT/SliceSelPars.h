/* ***************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/NSPECT/SliceSelPars.h,v $
 *
 * Copyright (c) 2019
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: SliceSelPars.h,v 1.65.2.13 2017/05/16 15:07:10 cmei Exp $
 *
 * ***************************************************************/



/****************************************************************/
/* INCLUDE FILES                                                */
/****************************************************************/


/****************************************************************/
/* PARAMETER DEFINITIONS                                        */
/****************************************************************/

/* editable */

OnOff parameter
{
  display_name      "Slice Selection";
  relations          SliceRelation;
} SliceSelOnOff;

int parameter
{
  display_name      "Number of Slices";
  relations          SliceRelation;
  minimum            1 outofrange nearestval;
  editable           false; //if true see SliceGeoCub-define
} SliceNum ;

SLOR_TYPE parameter
{
  display_name      "Slice Orientation";
  relations         SliceRelation;
} SliceOri[];

double parameter
{
  display_name      "Slice Thickness";
  format            "%.3f";
  units             "mm";
  minimum           1.0e-3 outofrange nearestval;
  relations         SliceRelation;
} SliceThick[];

double parameter
{
  display_name      "Slice Offset";
  format            "%.3f";
  units             "mm";
  relations         SliceRelation;
} SliceOffset[];

int parameter
{
  editable          false;
} SliceOrderList[];


/* non-editable */

double parameter 
{
  display_name      "Echo Time";
  units             "ms";
  format            "%.3f";
  editable          false;
  minimum           0.0;
}EchoTime;

double parameter 
{
  display_name      "Frequency List";
  short_description "Frequency offset list.";
  units             "Hz";
  format            "%.2f";
  editable          false;
} SliceFL[];

double parameter 
{
  editable          false;
  units             "%";
} SliceGrad[][3];


double parameter 
{
  display_name      "Spoiler Duration";
  units             "ms";
  format            "%.3f";
  editable          false;
} SliceSpoilDur;

double parameter 
{
  editable          false;
  units             "%";
} SliceRephGrad[][3];

double parameter
{
  display_name      "Slice Vector";
  short_description "Orientation vector of slices (in object system).";
  relations         SliceRelation;
  editable          false;
} SliceVec[][3];

double parameter
{
  editable          true;
  minimum           0.0;
  units             "ms";
  format            "%.3f";
  relations         SliceRelation;
} SliceRephDur;

GEO_OBJECT parameter
{
  display_name      "Slice Geo Object";
  short_description "Properties of geometry object.";
  relations         SliceGeoObjRelation;
  editable          false;
} SliceGeoObj;

GEO_CUBOID_PACK parameter
{
  short_description "Description of geometry cuboids.";
  relations         SliceGeoCubRelation;
  maxdim[1];        /* restricts  #cuboids in Geomtry Editor */
  mindim[1];        /* as SliceNum is non-editably 1 */
  editable          true;
} SliceGeoCub[];


YesNo parameter 
{
  editable          false;
  visible           false;
  relations         SliceHandlerRelation;
} SliceHandler;

/********************************************************************** */
/* End of File								*/
/********************************************************************** */
