/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2016
 * Bruker BioSpin MRI GmbH 
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef CMN_REPORT_TYPES_H
#define CMN_REPORT_TYPES_H


/** Adjustment report page size. */
typedef enum PvmReportPageSize
{
    /** The A4 page size. */
    REPORT_A4,
    /** The US letter page size. */
    REPORT_USLETTER
} PVM_REPORT_PAGE_SIZE;

/** Adjustment report page orientation. */
typedef enum PvmReportPageOrientation
{
    /// The portrait report page orientation.
    REPORT_PORTRAIT,
    /// The landscape report page orientation.
    REPORT_LANDSCAPE
} PVM_REPORT_PAGE_ORIENT;

/** Adjustment report style sheet type. */
typedef enum PvmReportStylesheetType
{
    /// A Bruker defined style sheet should be used for the report.
    REPORT_BRUKER,
    /// A customer defined style sheet should be used for the report.
    REPORT_CUSTOM
} PVM_REPORT_STYLESHEET_TYPE;

typedef dynenum PvmReportTemplateType;

/** Attributes for a report. */
typedef struct PvmReportAttrType
{
    /// The page size.
    PVM_REPORT_PAGE_SIZE pageSize;
    /// The page orientation.
    PVM_REPORT_PAGE_ORIENT pageOrient;
    /// The style sheet.
    PVM_REPORT_STYLESHEET_TYPE styleSheet;
} PVM_REPORT_ATTR_TYPE;



#endif /* PVM_REPORT_TYPES_H */
