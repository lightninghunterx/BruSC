/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2002-2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef VISUDEFINES_H
#define VISUDEFINES_H

/** Prefix for study UID creation. */
#define VISU_STUDY_UID_POSTFIX "19.20.21.4.25"

/** Maximum length of most VISU strings. */
#define VISU_MAX_NAME_LEN 65  

/** Length of short VISU strings (from DICOM SH). */
#define VISU_SHORT_NAME_LEN 17

/** Length of the VisuCreator version, may contain 3 versions. */
#define VISU_VERSION_LEN 195

/** Length of date entry in VISU. */
#define VISU_DATE_LEN 9

/** Maximum length of comment strings in VISU. */
#define VISU_COMMENT_LEN 2048

/** Conversion matrix from DICOM to PV subject coordinate system. */
#define VISU_DICOM_PV_MATRIX {-1, 0, 0, 0, -1, 0, 0, 0, 1}
/** Conversion matrix from VISU image to PV image coordinate system. */
#define VISU_IMAGE_TRANS_MATRIX {-1, 0, 0, 0, -1, 0, 0, 0, 1}

#endif


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



