/****************************************************************
 *
 * $Source: /pv/CvsTree/pv/gen/src/prg/methods/AdjRefPowX/SliceSelLayouth,v $
 *
 * Copyright (c) 2020
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id: modulesClassDefs.h,v 1.40.2.6 2017/05/16 15:29:32 cmei Exp $
 *
 ****************************************************************/


#define SLICE_SEL_PARS     \
  SliceNum;       \
  SliceThick;     \
  SliceOffset;    \
  SliceSpoilDur;  \
  SliceRephDur;   \
  SliceGrad;      \
  SliceRephGrad;  \
  SliceOri;       \
  SliceVec;       \
  SliceFL;        \
  SliceGeoObj;    \
  SliceOrderList; \
  SliceGeoCub;

pargroup
{
SLICE_SEL_PARS
}
attributes
{
  display_name "Slice Selection";
} Slice_Sel_Parameters;

#undef SLICE_SEL_PARS
