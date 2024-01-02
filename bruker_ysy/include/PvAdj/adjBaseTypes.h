/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999-2015
 * Bruker BioSpin MRI GmbH 
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef ADJ_BASETYPES_H
#define ADJ_BASETYPES_H

/** Execution State of adjustment. */
typedef enum
{
    /// Adjustment was not performed.
    adj_not_done,

    /// Adjustment was successful, result are stored.
    adj_successful,

    /// Adjustment was aborted by user.
    adj_aborted,

    /// Adjustment execution failed.
    adj_failed,

    /// Adjustment was not executed but results were defined manually.
    adj_manual,

    /** 
     * Adjustment was executed as GOP in an instruction (not in adjustment 
     * platform).
     */
    adj_instruction
} AdjExecutionState;

/** 
 * Type of adjustment perform list.
 */
typedef enum
{
    /// The adjustment is performed once per-study.
    per_study,

    /// The adjustment is performed per-scan.
    per_scan,

    /// The study is performed only on-demand.
    on_demand
} AdjPerformListType;

/** Type of result parameter. */
typedef enum
{
    /** The result parameter is system global. 
        This can only be set from the GUI and not from an adjustment
        method.
     */
    global_result,
    /** The result parameter is user specific. */
    user_result,
    /** The result parameter is study specific. */
    study_result,
    /** The result parameter is scan specific. */
    scan_result,
    /** 
     * The result parameter is adjustment type specific and is 
     * AdjContext.defaultResultType.
     */
    adj_type
} AdjResultType;

/** Enumeration describing the context of the adjustment handler call. */
typedef enum AdjHandlerContext
{
    /** Adjustment handler is called before acquisition. */
    HANDLE_ACQUISITION,
    /** Adjustment handler is called in adjustment platform. */
    HANDLE_PLATFORM
} ADJ_HANDLER_CONTEXT;




/****************************************************************/
/* DEFINITIONS						        */
/****************************************************************/

/** Maximum length of adjustment name. */
#define ADJ_NAME_LENGTH       128
/** Maximum length of adjustment file name. */
#define ADJ_FILE_NAME_LENGTH  256
/** Maximum length of adjustment display name. */
#define ADJ_DISPL_NAME_LENGTH 256
/** Maximum length of adjustment short description. */
#define ADJ_DESCR_LENGTH      512
/** Maximum length of adjustment protocol name.*/
#define ADJ_PROTOCOL_LENGTH   128
#define ADJ_CONTEXT_LENGTH    1024
#define ADJ_HWCONTEXT_LENGTH  1024
#define ADJ_HWCONTEXT_KEY_LENGTH 32
#define ADJ_RESULTID_LENGTH   65
#define ADJ_METHOD_LENGTH     128
#define ADJ_PAR_NAME_LENGTH   64
#define ADJ_ADJPARS_LENGTH    1024
#define ADJ_STUDYPATH_LEN     512
#define ADJ_RESULTPATH_LEN    1024
#define ADJ_SUBPROCESS_LENGTH 20


#define ADJ_STATE_STUDY_FILENAME  "AdjStatePerStudy"
#define ADJ_STATE_STUDY_GROUP     "AdjStudyStateGroup"
#define ADJ_STATE_SCAN_FILENAME   "AdjStatePerScan"
#define ADJ_STATE_SCAN_GROUP      "AdjScanStateGroup"
#define ADJ_STATE_GLOBAL_FILENAME "AdjGlobalParState" 
#define ADJ_STATE_GLOBAL_GROUP    "AdjGlobalStateGroup"

/// Directory name for all adjustment manager files outside of data set.
#define ADJMANAGER_DIR            "AdjManager"   
/// Directory name for the adjustment results inside of ADJMANAGER_DIR          
#define RESULT_DIR                "AdjResult"
/// Name of the result state file.
#define RESULT_STATE_FILENAME     "ResultState"

#define NO_ADJ "NO_ADJUSTMENT"


#define ADJ_CONTEXT_EMPTY { NO_ADJ , "", "", "", "", "", scan_result, No, Yes, "", HANDLE_ACQUISITION, Yes}

#endif /* ADJ_BASETYPES_H */


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
