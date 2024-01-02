/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Locker$
 * $Id$
 * $Id$
 *
 *
 ****************************************************************/

/****************************************************************/
/*	TYPEDEF's						*/
/****************************************************************/

typedef enum
  {
    Long_TE,
    Short_TE
  } TE_MODE;

typedef enum
  {
    Cardiac,
    Respiration
  }MOVIE_TYPE;

typedef enum
  {
    CardiacResp,
    OnlyCardiac,
    OnlyResp
  }ANALYSIS_MODE;

typedef enum
  {
    Magnitude,
    Phase,
    Auto
  }SIGNAL_TYPE;

typedef enum  
  {
    sequential,
    interleaved 
  }SEGMENTING_MODE;
/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
