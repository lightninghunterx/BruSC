/* ***************************************************************
 *
 * Copyright (c) 2018 - 2020
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * ***************************************************************/

#define DEBUG		1
#define DB_MODULE	0
#define DB_LINE_NR	0

#include "method.h"


#ifndef CPROTO
#include <string>
#include "localUtilities.h"
#include "Parx/Utils/ParxArray.hh"


std::string pathToParentDset(bool ofexpno,const std::string *subdirn)
{
  DB_MSG(("-->pathToParentDset"));
  std::string dsetPath;
  char basepath[4096];
  int state UNUSED_PARAM =0;

  if(subdirn && !subdirn->empty())
    state=PvAdjManGetProcnoResultPath(basepath,
                                      4095,
                                      ofexpno ? IsExpnoPath:IsProcnoPath,
                                      PvAdjGetOriginalDatasetPath(),
                                      adj_type,
                                      subdirn->c_str());
  else
    state=PvAdjManGetProcnoResultPath(basepath,
                                      4095,
                                      ofexpno ? IsExpnoPath:IsProcnoPath,
                                      PvAdjGetOriginalDatasetPath(),
                                      adj_type,
                                      "");

  dsetPath.assign(basepath);
  DB_MSG(("PvAdjManGetProcnoResultPath returns %d size %d Path:\n%s",
          state,static_cast<int>(dsetPath.size()),
          dsetPath.c_str()));

  DB_MSG(("<--pathToParentDset"));
  return dsetPath;
}
#endif