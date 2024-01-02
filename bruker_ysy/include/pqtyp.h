/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1995-2006
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PQTYP_H
#define PQTYP_H

#include "generated/KeyUids.h"
#include "generated/DataPath.h"

#define	PipeNetCmdSize	32
#define	PipeNetPathSize	de_bruker_mri_generated_datapath_MaxProcnoCStrLen

typedef struct
  {
  int	expNo;
  int	procNo;
  } PipeScanRefT;

typedef struct
  {
  char	cmd[PipeNetCmdSize];
  char	path[PipeNetPathSize];
  } PipeNetCmdT;

#define	PipeFilterCmdSize	256

typedef struct
  {
  int	id;
  char	cmd[PipeFilterCmdSize];
  int	minOutSize;
  } PipeNetFilterT;

typedef struct
  {
  char	psKey[de_bruker_mri_generated_uid_CStrMaxPsKeyLen];
  int	srcNdx;
  int	srcId;
  int	sinkNdx;
  int	sinkId;
  int	flags;
  int	shmId;
  int	maxSize;
  } PipeNetQueueT;	

#define PIPE_MIN_REND_CNT	3
#define PIPE_ACQ_REND_ID	0x0001
#define PIPE_RECO_REND_ID	0x0002
#define PIPE_USER_REND_ID       0x0003

typedef struct
  {
  int	rendId;
  int	nrWaiting;
  int	nrExpected;
  } PipeNetSynchT;	

typedef struct
  {
    int inNdx;
    int outNdx;
    int size;    
  } PipeNetUserEdgeT;


typedef enum  { ACQ_CONTROL_ID,
		RECIN_FILTER_ID,
		RECO_FILTER_ID,
		RECOUT_FILTER_ID,
		ACQ_AUTO_FILTER_ID,
		ACQ_TAIL_FILTER_ID,
		ACQ_OUT_FILTER_ID,
		CORBASOURCEFILTER_ID,
		RECO_STAGE_ID} FilterIDs;


#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
