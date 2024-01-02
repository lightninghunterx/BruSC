/****************************************************************
 *
 * $Source$
 * Adjustment result definition.
 *
 * Copyright (c) 2009 - 2015
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/


#ifndef PVCFG_RESULTPARSTYPE_H
#define PVCFG_RESULTPARSTYPE_H

#include "bruktyp.h"
#include "modality.h"
#include "PvUtil/TimeDefs.h"
#include "generated/DataPath.h"
#include "generated/KeyUids.h"
#include "adjBaseTypes.h"

/** Result type of parameters which may contain a procno path. */
typedef struct AdjProcnoResult
{
    /** The original prefix of the result study path. */
    char prefix[de_bruker_mri_generated_datapath_MaxPrefixCStrLen];

    /** The study directory name. */
    char study[de_bruker_mri_generated_datapath_MaxNameCStrLen];

    /** The experiment number. */
    int expno;

    /** The processing number. */
    int procno;
} AdjProcnoResultType;


/** Definition of adjustment result parameters. */
typedef struct AdjResultPar
{
    /** The result type. */
	AdjResultType type;
    /** The result parameter name. */
	char parname[ADJ_PAR_NAME_LENGTH];
} ADJ_RESULT_PAR;

/** Adjustment result administration information structure. */
typedef struct ResultParsType
{
    /** Adjustment name that adjusted these parameters. */
    char adjName[ADJ_NAME_LENGTH];
    /** Parameter names stored in result. */
    char parnames[ADJ_ADJPARS_LENGTH];
    /** Method creating the result. */
    char methodName[ADJ_METHOD_LENGTH];
    /** Method context creating the result. */
    char methodContext[ADJ_CONTEXT_LENGTH];
    /** Date/Time of result creation. */
    pvtime_t time;
    /** The result directory id. */
    char resultId[ADJ_RESULTID_LENGTH];
    /** The study which created the result. */
    char studyUid[de_bruker_mri_generated_uid_CStrIsoUidLen];
    /** The experiment number created the result. */
    int expno;
    /** Indicates if result was set manually? */
    YesNo manualResult;
    /** The modality type of the result. */
    PvModalityType modalityType;
} RESULT_PARS_TYPE;


/** Result type of parameters which may contain a procno path. */
typedef struct ProcnoResult
{
    /** The original prefix of the result study path. */
    char prefix[de_bruker_mri_generated_datapath_MaxPrefixCStrLen];

    /** The study directory name. */
    char study[de_bruker_mri_generated_datapath_MaxNameCStrLen];

    /** The experiment number. */
    int expno;

    /** The processing number. */
    int procno;
} ProcnoResultType;

#endif
