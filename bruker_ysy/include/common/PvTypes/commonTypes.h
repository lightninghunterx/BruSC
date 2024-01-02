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

#ifndef CMN_COMMON_TYPES_H
#define CMN_COMMON_TYPES_H

#include "PvAdj/adjConfigTypes.h"
#include "PvAdj/adjManagerTypes.h"
#include "PvAdj/ResultParsType.h"
#include "common/PvTypes/geoTypes.h"
#include "common/PvTypes/reportTypes.h"
#include "generated/ParImport.h"

#define PROT_DESC_LEN           400

/** The dynamic enumeration type for the measurement method. */
typedef dynenum MeasMethodType;

/** The dynamic enumeration type for the dimensions. */
typedef dynenum DIM_ENUM_TYPE;



/** Description type used to negotiate between import and export ps.
    @ingroup PVM_STRUCT
*/
typedef struct ParImportDescType
{
  /** Unique identifier for the import / export feature. */
  char id[de_bruker_mri_generated_parimport_IdCLen];

  /** The method specific context for this import. */
  char context[de_bruker_mri_generated_parimport_ContextCLen];

  /** The human readable description for the Import chooser in GUI.
      For all BRUKER defined features it is not necessary to set
      this description since the human readable description is
      derived from the @c id and the GUI resources.
  */
  char description[de_bruker_mri_generated_parimport_DescCLen];
} PARIMPORT_DESC_TYPE;

/** Structure to define one import element.
    @ingroup PVM_STRUCT
*/
typedef struct ParImportListType
{
  /** The source parameter or group name.
      If empty this import definition describes a relation call
      to the @c handlerPar. If set to a parameter group
      the @c destPar must be empty.
  */
  char sourcePar[de_bruker_mri_generated_parimport_ParCLen];

  /** The destination parameter name.
      If empty source and destination are identical.
  */
  char destPar[de_bruker_mri_generated_parimport_ParCLen];

  /** The handler parameter for a relation call.
      If not empty the relation of the handler parameter is called
      after all elements of a list are copied. The call of the handler
      parameter relations are performed in the given order of the list.
  */
  char handlerPar[de_bruker_mri_generated_parimport_ParCLen];

  /**
   * Indicates if the modality of overlay default relations
   * should be called for the handler parameter (instead of the method
   * defined relation).
   * It is only valid if @c handlerPar is not empty.
  */
  YesNo forceDefault;
} PARIMPORT_LIST_TYPE;


/** The common experiment type. */
enum PvmExperimentType
{
    NO_EXPERIMENT,
    SCAN_EXPERIMENT,
    SETUP_EXPERIMENT
};

/**
PET Gating Signals
@ingroup PVM_ENUM
*/
enum Cmn_Enum_GatingSignalSource
{
    /** The gating signal source is an ECG signal. */
    GATING_SIGNAL_ECG,

    /** The gating signal source is a respiration signal. */
    GATING_SIGNAL_RESPIRATION,

    /** The gating signal source is non of the other options. */
    GATING_SIGNAL_OTHER,

    /** No gating signal source. */
    GATING_SIGNAL_NONE
};

/** Structure to specify a dataset within a study.
    @ingroup PVM_STRUCT
*/
typedef struct
{
  /** experiment number */
  int expno;
  /** process number */
  int procno;
} Cmn_Struct_StudyDset;


/** Enum states to specify bed position handling policy.
    @ingroup PVM_ENUM
 */
typedef enum
{
  /** Use current position of animal bed.*/
  ATS_POS_CURRENT,
  /** Assure ATS position to align modality with image center. */
  ATS_POS_IMAGE_CENTER,
  /** Assure ATS position defined by current reference position. */
  ATS_POS_REFERENCE
}ATS_POS_POLICY;


#endif
