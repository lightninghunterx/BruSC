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

#ifndef COMMON_ACQTYP_H
#define COMMON_ACQTYP_H

#define ACQ_PROTLOC_LEN 64
#define ACQ_METHOD_LEN 40

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type QUALITY_COMPARE_OPERATOR

  @ptypePurpose   structure for quality specifications
  @ptypeAuthor    anba                                  */

/*------------------------------------------------------*/
enum QA_COMPARE_OPERATOR
{
    less_than,
    less_equal,
    greater_than,
    greater_equal,
    equal
};

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type QUALITY_SPEC

  @ptypePurpose   structure for quality specifications
  @ptypeAuthor    anba                                  */

/*------------------------------------------------------*/
struct QUALITY_SPEC
{
    char name[128]; /* Specification name */
    QA_COMPARE_OPERATOR compare_op; /* compare operator */
    double value; /* target value */
    char unit[32]; /* unit, e.g. mm, Hz/cm, %, ... */
    double tolerance; /* tolerance for the 'equal' operator */
    double result; /* evaluation result for methods with own reco */
    int inspec; /* overal result for methods with own reco, only 0,1. */
};

/*------------------------------------------------------*/
/**DOC-TYPE ParaVision Documentation of type CONTRAST_STRUCT

  @ptypePurpose      Struct, containing parameters to
                     document the contrast agent (acc. DICOM).
    volume:          Volume injected  in milliliters
                     of diluted contrast agent
    dose:            Total umount in milliliters of
                     the undiluted contrast agent
    route[16]:       Administration route of contrast agent.
                     Description, how the contrast agent
                     is injected?
    start_time[16]:  Time of start of injection
    stop_time[16]:   Time of end   of injection
  @ptypeAuthor     MAH                                 */

/*------------------------------------------------------*/

struct CONTRAST_STRUCT
{
    double volume;
    double dose;
    char route[16];
    char start_time[16];
    char stop_time[16];
};

/**DOC-TYPE ParaVision Documentation of SOLID_HARMONIC_COEFF
 *
 * @ptypePurpose    Struct containing sensitivity of the
 *                  corresponding solid harmonic coefficient
 *   degree         degree of solid harmonic coefficient
 *   order          order of solid harmonic coefficient
 *   coefficient    sensitivity (for MR: in Hz/cm**order/A)
 * @ptypeAuthor     MAWI                                */

/*------------------------------------------------------*/
typedef struct
{
    int degree; // degree
    int order; // order
    double coefficient; // sensitivity
} SOLID_HARMONIC_COEFF;

#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
