/***************************************************************

   Parameter Structure Initialization
   NAME OF FILE:	pinit_preemp.h
   Class code (hex):	1FF
   No. of parameters:	17
   Created by 'defpar'  from
   # $Id: pdef_preemp.txt,v 1.21 2017/03/30 06:44:41 mah Exp $
   of 'pdef_preemp.txt'
   2020-05-25 17:54:36.156 +0200 run by jenkins

***************************************************************/
{

	/* initialization of first table */

  {"A000",             4,   0x10000, 0, PARUNIT_no        , offsetof(struct preemp_pars, A000)},
  {"ActualCurrent",           80,  0x140001, 0, PARUNIT_no        , offsetof(struct preemp_pars, ActualCurrent)},
  {"B0CC",                     8,   0x40080, 0, PARUNIT_Hz        , offsetof(struct preemp_pars, B0CC)},
  {"COILDEL",                  8,   0x40002, 0, PARUNIT_usec      , offsetof(struct preemp_pars, COILDEL)},
  {"CoarseOffset",            80,  0x140001, 0, PARUNIT_no        , offsetof(struct preemp_pars, CoarseOffset)},
  {"CompDel",                 80,  0x140001, 0, PARUNIT_usec      , offsetof(struct preemp_pars, CompDel)},
  {"DPPFAC",                   8,   0x40001, 0, PARUNIT_no        , offsetof(struct preemp_pars, DPPFAC)},
  {"GRADCC",                   8,   0x48080, 0, PARUNIT_no        , offsetof(struct preemp_pars, GRADCC)},
  {"PreempChanIn",           152,  0x110080, 0, PARUNIT_no        , offsetof(struct preemp_pars, PreempChanIn)},
  {"PreempChanOut",          152,  0x110001, 0, PARUNIT_no        , offsetof(struct preemp_pars, PreempChanOut)},
  {"PreempFilters",         4256,  0x140080, 0, PARUNIT_no        , offsetof(struct preemp_pars, PreempFilters)},
  {"PreempMode",               4,   0x60080, 0, PARUNIT_no        , offsetof(struct preemp_pars, PreempMode)},
  {"PreempOrder",            152,  0x110080, 0, PARUNIT_no        , offsetof(struct preemp_pars, PreempOrder)},
  {"RAMPTM",                   8,   0x40080, 0, PARUNIT_usec      , offsetof(struct preemp_pars, RAMPTM)},
  {"RISETM",                   8,   0x40080, 0, PARUNIT_usec      , offsetof(struct preemp_pars, RISETM)},
  {"ScalingFactor",           80,  0x140001, 0, PARUNIT_no        , offsetof(struct preemp_pars, ScalingFactor)},
  {"zz00",             4,   0x40000, 0, PARUNIT_no        , offsetof(struct preemp_pars, zz00)},

	/* initialization of second table */


       17,                          /* A000 */
  {                                                                   /* ActualCurrent */
  0.000000000000E+00
  },
  0.000000000000E+00,                                                 /* B0CC */
  0.000000000000E+00,                                                 /* COILDEL */
  {                                                                   /* CoarseOffset */
  0.000000000000E+00
  },
  {                                                                   /* CompDel */
  0.000000000000E+00
  },
  0.000000000000E+00,                                                 /* DPPFAC */
  0.000000000000E+00,                                                 /* GRADCC */
  {                                                                   /* PreempChanIn */
	0
  },
  {                                                                   /* PreempChanOut */
	0
  },
  {                                                                   /* PreempFilters */
  0.000000000000E+00
  },
  0,                                                                  /* PreempMode */
  {                                                                   /* PreempOrder */
	0
  },
  0.000000000000E+00,                                                 /* RAMPTM */
  0.000000000000E+00,                                                 /* RISETM */
  {                                                                   /* ScalingFactor */
	1.000000000000E+00,	1.000000000000E+00,	1.000000000000E+00,	1.000000000000E+00,
	1.000000000000E+00,	1.000000000000E+00,	1.000000000000E+00,	1.000000000000E+00,
	1.000000000000E+00,  1.000000000000E+00
  },

  9999   /* ZZ00 */    
  
};
