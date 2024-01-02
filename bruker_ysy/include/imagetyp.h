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

#ifndef IMAGETYP_H
#define IMAGETYP_H


typedef enum ImageType
  {
  MAGNITUDE_IMAGE,
  COMPLEX_IMAGE,
  REAL_IMAGE,
  IMAGINARY_IMAGE,
  PHASE_IMAGE,
  IR_IMAGE
  } IMAGE_TYPE;

typedef enum ImageWordtype
  {
  _32BIT_SGN_INT,
  _16BIT_SGN_INT,
  _8BIT_UNSGN_INT,
  _32BIT_FLOAT
  } IMAGE_WORDTYPE;


typedef enum ImageByteOrder
  {
  littleEndian,
  bigEndian
  } IMAGE_BYTE_ORDER;

#endif /* IMAGETYP_H */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
