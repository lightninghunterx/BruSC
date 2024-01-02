/****************************************************************
 *
 * $Source$
 * 
 * Copyright (c) 1988-2008
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 * 
 * All Rights Reserved 
 * 
 * $Id$
 *
 ****************************************************************/

#ifndef CONFIGTYP_H
#define CONFIGTYP_H


#define CONFIG_SCAN_CURRENT_VERSION 1

/**
  * Defines the length of a string buffer that is guaranteed to suffice for
  * holding channel state information.
  *
  * Note for library maintainers:
  * Currently, the longest channel state string is the transmit chain key,
  * whose length (excluding the terminating null byte) is given by 
  *    PROBE_CONFIG_FILENAME_LEN+MANUFACTURER_LEN+PART_NUM_LEN+45
  *
  * The length of the receive chain key is currently
  *    PROBE_CONFIG_FILENAME_LEN+MANUFACTURER_LEN+PART_NUM_LEN+29
  *
  * If those macros (in acqutyp.h) are changed significantly, or if one of
  * the channel state query functions is modified, the definition of the
  * channel state length has to be revised as well!
  */
#define PVSYSINFOCHANSTATE_LEN 1024


#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
