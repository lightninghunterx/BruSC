/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001-2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVCFG_MrConfig_H
#define PVCFG_MrConfig_H

#include "bruktyp.h"
#include "acqutyp.h"
#include "mr/acqtyp.h"

#ifndef CPROTO
#include <string>
#endif

#include "PvCfg/MR/DLLMacro.hh"


/** 
 * @file
 * @addtogroup PvCfgMr
 * @{
 */

/*--------------------------------------------------------------*
 * include other header files required here
 *--------------------------------------------------------------*/


/* Definitions for RFPulses */

#define  CPUL_ILLEGAL      0u
#define  CPUL_INIT         1u
#define  CPUL_NAME         2u
#define  CPUL_EXMODE       4u
#define  CPUL_ANGLE        8u
#define  CPUL_BWFAC       16u
#define  CPUL_INTEGFAC    32u
#define  CPUL_REPHFAC     64u
#define  CPUL_TYPE       128u
#define  CPUL_MODE       256u
#define  CPUL_NPOINTS   1024u
#define  CPUL_EXTENSION 2048u



#define CPUL_PPG_USABLE  ( CPUL_NAME | CPUL_NPOINTS| CPUL_INIT | CPUL_INTEGFAC)

#define CPUL_PVM_USABLE  (CPUL_PPG_USABLE | CPUL_MODE | CPUL_ANGLE | CPUL_BWFAC  |CPUL_TYPE | CPUL_REPHFAC |  CPUL_EXMODE  | CPUL_EXTENSION)

#define CPUL_LIB_PULSE   (CPUL_PVM_USABLE)
                     
                     
#define  CPUL_NAME_LEN     128
#define  CPUL_ATTR_IN_FILE   8                   

#define NORMAL_OPMODES ( OPERATION_MODE_FLAG_DEFAULT | OPERATION_MODE_FLAG_AUTOMATIC )
         
/// A pulse file definition struct.
 
typedef struct 
{
    /// Pulse name.
    char         name[CPUL_NAME_LEN];

    /// Pulse extension.
    char         ext[CPUL_NAME_LEN];

    /// Pulse extension mode.
    char         exmode[CPUL_NAME_LEN];

    /// Pulse angle.
    double       angle;

    /// Pulse bandwidth factor.
    double       bwfac;

    /// Pulse ??? factor.
    double       integfac;

    /// Pulse ??? factor.
    double       rephfac;

    /// Pulse type.
    char         type[CPUL_NAME_LEN];

    /// Pulse mode.
    int          mode;

    /// Number of points.
    int          npoints;

    /// Pulse status.
    unsigned int status;
} CPULSE_ATTRIBS;

typedef bool (* FillRfPulseProc)(int psid, const char * dir, 
    const char * filename,  CPULSE_ATTRIBS *attr);

#ifndef CPROTO

#include <vector>
#include <string>

/* gen/src/lib/libPvCfg/ConfigRFPulseList.cc */
DLL_INTERFACE void PvCfgUpdateRFPulses(int psid, FillRfPulseProc fillProc); 

typedef const double * (* PvCfgReadWaveFormProc)(const char *, int&, bool&,
    double&, double&);

DLL_INTERFACE int PvCfgRFPulseGetPowIntFac(const char *filename, double &pif, 
    PvCfgReadWaveFormProc readWaveFormProc);
DLL_INTERFACE CPULSE_ATTRIBS PvCfgRFPulseGetDefinition(const char *filename);
DLL_INTERFACE void PvCfgRFPulseFilesPpgUsable(
  std::vector<std::string> & matchingPulses);
DLL_INTERFACE void PvCfgRFPulseLibNamesWithExmode(
  std::vector<std::string> & matchingPulses, const char *test);
DLL_INTERFACE void PvCfgRFPulsePvmNamesWithExmode(
  std::vector<std::string> & matchingPulses, const char *test);

#endif

/** @} */

#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/


