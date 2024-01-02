/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1995-2002
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/
#ifndef GOSITYP_H
#define GOSITYP_H


/*--------------------------------------------------------------*
 * The following typedef's are used to define several parameters
 * in the GOSI parameter class.
 *--------------------------------------------------------------*/

typedef enum
{
    GOSI_SAWTOOTH_FORM,
    GOSI_FID_FORM,
    GOSI_ECHO_FORM,
    GOSI_METHOD_FORM,
    GOSI_FILE_FORM,
    GOSI_WOBBLECURVE_FORM
} GOSI_WAVEFORM_TYPE;

typedef struct
{
    int jobId;
    int receiverId;
    int maxValue;
    int nrPoints;
    int iteration;
} GOSI_SCAN_DESCRIPTION_TYPE;
#endif /* GOSITYP_H */
/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
