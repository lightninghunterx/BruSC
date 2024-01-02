/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2015 - 2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

/*--------------------------------------------------------------*
 * prevent multiple includes
 *--------------------------------------------------------------*/
#ifndef PV_MODALITY_H
#define PV_MODALITY_H


/**
 * @file 
 * 
 * Device modality definitions.
 * 
   @addToGroup PvUtil
   @{
 */


/** Enumeration of supported device modalities. */
enum PvModalityType
{
    /** Modality unknown. */
    OT_Modality,
        
    /** MRI Modality. */
    MR_Modality,
    
    /** MPI Modality. */
    MPI_Modality,
    
    /** PET Modality. */
    PT_Modality,
    
    /** Optical molecular imaging. */
    OMI_Modality,
    
    /** CT Modality. */
    CT_Modality
};

/*@}*/

#endif /* ifndef PV_MODALITY_H */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
