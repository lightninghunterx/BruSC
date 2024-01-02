/****************************************************************
 *
 * $Source$
 * 
 * Copyright (c) 1988-2016
 * Bruker BioSpin MRI GmbH
 * D7512 Rheinstetten, Germany
 * 
 * All Rights Reserved 
 * 
 * $Id$
 *
 ****************************************************************/

#ifndef SUBJTYP_H
#define SUBJTYP_H

#include "generated/ParxDefs.h"

/*--------------------------------------------------------------*
 * This is the maximum length that the comment parameters may
 * reach.
 *--------------------------------------------------------------*/
#define SUBJECT_MAX_STRING_LEN 5000
#define SUBJECT_OLD_MAX_LEN     2048
#define SUBJECT_PT_COMPOUND_LEN 251
#define SUBJECT_MR_COIL_BIS_LEN 10240 /* must be same value as acqumtype COIL_BIS_LEN */

/*--------------------------------------------------------------*
 * The following typedef's are used to define several parameters
 * in the SUBJECT parameter class.
 *--------------------------------------------------------------*/

/** Subject type information. */
typedef enum
{
    /** A biped animal of a human. */
    Biped,
    /** A quadruped animal. */
    Quadruped,
    /** A phantom. */
    Phantom,
    /** Not an animal or a phantom. */
    Other,
    /** An animal that is not a biped or quadruped. */
    OtherAnimal
} SUBJECT_TYPE_TYPE;


/** Gender of the subject. */
typedef enum
{
    /** Subject is male. */
    MALE,
    /** Subject is female. */
    FEMALE,
    /** Gender is undefined. */
    UNDEFINED,
    /** Gender is unknown. */
    UNKNOWN
} SUBJECT_ANIMAL_SEX_TYPE;


/** 
 * Position of subject in instrument. 
 * The description of the positions is for horizontal devices.
 */
enum SubjectInstrPosType
{
    /** Head to device  back, supine to gravity. */
    Head_Supine,
    /** Head to device back, prone to gravity. */
    Head_Prone,
    /** Head to device back, left to gravity. */
    Head_Left,
    /** Head to device back, right to gravity. */
    Head_Right,
    /** Foot to device back, supine to gravity. */
    Foot_Supine,
    /** Foot to device back, prone to gravity. */
    Foot_Prone,
    /** Foot to device back, left to gravity. */
    Foot_Left,
    /** Foot to device back, right to gravity. */
    Foot_Right,
    /** Left to device back, supine to gravity. */
    Left_Supine,
    /** Left to device back, prone to gravity. */
    Left_Prone,
    /** Right to device back, supine to gravity. */
    Right_Supine,
    /** Right to device back, prone to gravity. */
    Right_Prone,
    /** Anterior to device back, left to gravity. */
    Anterior_Left,
    /** Anterior to device back, right to gravity. */
    Anterior_Right,
    /** Posterior to device back, left to gravity. */
    Posterior_Left,
    /** Posterior to device back, right to gravity. */
    Posterior_Right
};

/****************************************************************/
/*	Deprecated type definitions. */
/****************************************************************/

typedef struct
{
    char Family_name[de_bruker_mri_generated_parx_CSubjectNameLen];
    char Given_names[de_bruker_mri_generated_parx_CSubjectNameLen];
} SUBJECT_NAME_STRUCT;

typedef enum
{
    Male,
    Female
} SUBJECT_HUMAN_SEX_TYPE;

typedef enum
{
    SUBJ_ENTRY_FeetFirst,
    SUBJ_ENTRY_HeadFirst
} SUBJECT_ENTRY;

typedef enum
{
    SUBJ_POS_Supine,
    SUBJ_POS_Prone,
    SUBJ_POS_Left,
    SUBJ_POS_Right
} SUBJECT_POSITION;



#endif /* _SUBJTYP_H_ */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
