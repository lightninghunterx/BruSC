/***************************************************************

   Parameter Structure Initialization
   NAME OF FILE:	pinit_all.h
   Class code (hex):	1FF
   No. of parameters:	405
   Created by 'defpar'  from
   * $Id: pdef_all.txt,v 1.158 2020/02/12 08:24:11 chjo Exp $
   of 'pdef_all.txt'
   2020-05-25 17:54:35.953 +0200 run by jenkins

***************************************************************/
{

	/* initialization of first table */

  {"A000",             4,   0x10000, 0, PARUNIT_no        , offsetof(struct all_pars, A000)},
  {"ABSF1",                    4,   0x30004, 0, PARUNIT_ppm       , offsetof(struct all_pars, ABSF1)},
  {"ABSF2",                    4,   0x30004, 0, PARUNIT_ppm       , offsetof(struct all_pars, ABSF2)},
  {"ABSG",                     4,   0x18804, 0, PARUNIT_no        , offsetof(struct all_pars, ABSG)},
  {"ABSL",                     4,   0x38804, 0, PARUNIT_no        , offsetof(struct all_pars, ABSL)},
  {"ACQT0",                    8,   0x40002, 0, PARUNIT_usec      , offsetof(struct all_pars, ACQT0)},
  {"ALPHA",                    4,   0x30004, 0, PARUNIT_no        , offsetof(struct all_pars, ALPHA)},
  {"AMP",                    128,  0x138002, 0, PARUNIT_percent   , offsetof(struct all_pars, AMP)},
  {"AMPCOIL",                132,  0x118002, 0, PARUNIT_no        , offsetof(struct all_pars, AMPCOIL)},
  {"AQORDER",                  4,   0x58004, 0, PARUNIT_no        , offsetof(struct all_pars, AQORDER)},
  {"AQSEQ",                    4,   0x58002, 0, PARUNIT_no        , offsetof(struct all_pars, AQSEQ)},
  {"AQ_mod",                   4,   0x5c802, 0, PARUNIT_no        , offsetof(struct all_pars, AQ_mod)},
  {"ASSFAC",                   4,   0x30004, 0, PARUNIT_no        , offsetof(struct all_pars, ASSFAC)},
  {"ASSFACI",                  4,   0x38004, 0, PARUNIT_no        , offsetof(struct all_pars, ASSFACI)},
  {"ASSFACX",                  4,   0x30004, 0, PARUNIT_no        , offsetof(struct all_pars, ASSFACX)},
  {"ASSWID",                   4,   0x38004, 0, PARUNIT_no        , offsetof(struct all_pars, ASSWID)},
  {"AUNM",                    32,   0x88802, 0, PARUNIT_no        , offsetof(struct all_pars, AUNM)},
  {"AUNMP",                   32,   0x80804, 0, PARUNIT_no        , offsetof(struct all_pars, AUNMP)},
  {"AUTOPOS",                 64,   0x80002, 0, PARUNIT_no        , offsetof(struct all_pars, AUTOPOS)},
  {"AXLEFT",                   8,   0x40004, 0, PARUNIT_no        , offsetof(struct all_pars, AXLEFT)},
  {"AXNAME",                  16,   0x80004, 0, PARUNIT_no        , offsetof(struct all_pars, AXNAME)},
  {"AXNUC",                    8,   0x80004, 0, PARUNIT_no        , offsetof(struct all_pars, AXNUC)},
  {"AXRIGHT",                  8,   0x40004, 0, PARUNIT_no        , offsetof(struct all_pars, AXRIGHT)},
  {"AXTYPE",                   4,   0x50004, 0, PARUNIT_no        , offsetof(struct all_pars, AXTYPE)},
  {"AXUNIT",                  16,   0x88004, 0, PARUNIT_no        , offsetof(struct all_pars, AXUNIT)},
  {"AZFE",                     4,   0x38804, 0, PARUNIT_ppm       , offsetof(struct all_pars, AZFE)},
  {"AZFW",                     4,   0x38804, 0, PARUNIT_ppm       , offsetof(struct all_pars, AZFW)},
  {"BCFW",                     4,   0x38004, 0, PARUNIT_ppm       , offsetof(struct all_pars, BCFW)},
  {"BC_mod",                   4,   0x58804, 0, PARUNIT_no        , offsetof(struct all_pars, BC_mod)},
  {"BF1",                      8,   0x4c002, 0, PARUNIT_MHz       , offsetof(struct all_pars, BF1)},
  {"BF2",                      8,   0x4c002, 0, PARUNIT_MHz       , offsetof(struct all_pars, BF2)},
  {"BF3",                      8,   0x4c002, 0, PARUNIT_MHz       , offsetof(struct all_pars, BF3)},
  {"BF4",                      8,   0x4c002, 0, PARUNIT_MHz       , offsetof(struct all_pars, BF4)},
  {"BF5",                      8,   0x4c002, 0, PARUNIT_MHz       , offsetof(struct all_pars, BF5)},
  {"BF6",                      8,   0x4c002, 0, PARUNIT_MHz       , offsetof(struct all_pars, BF6)},
  {"BF7",                      8,   0x4c002, 0, PARUNIT_MHz       , offsetof(struct all_pars, BF7)},
  {"BF8",                      8,   0x4c002, 0, PARUNIT_MHz       , offsetof(struct all_pars, BF8)},
  {"BLKTR",                  132,  0x138090, 0, PARUNIT_usec      , offsetof(struct all_pars, BLKTR)},
  {"BLKTR_MAN",                8,   0x40080, 0, PARUNIT_usec      , offsetof(struct all_pars, BLKTR_MAN)},
  {"BWFAC",                  256,  0x138002, 0, PARUNIT_no        , offsetof(struct all_pars, BWFAC)},
  {"BYTORDA",                  4,   0x58002, 0, PARUNIT_no        , offsetof(struct all_pars, BYTORDA)},
  {"BYTORDP",                  4,   0x58004, 0, PARUNIT_no        , offsetof(struct all_pars, BYTORDP)},
  {"CHANFCU",                100,  0x118002, 0, PARUNIT_no        , offsetof(struct all_pars, CHANFCU)},
  {"CHANREC",                100,  0x118002, 0, PARUNIT_no        , offsetof(struct all_pars, CHANREC)},
  {"CHEMSTR",                 64,   0x80012, 0, PARUNIT_no        , offsetof(struct all_pars, CHEMSTR)},
  {"CNST",                   256,  0x130002, 0, PARUNIT_no        , offsetof(struct all_pars, CNST)},
  {"COROFFS",                  8,   0x40004, 0, PARUNIT_Hz        , offsetof(struct all_pars, COROFFS)},
  {"CPDPRG",             0x90020,  0x180002, 0, PARUNIT_no        , offsetof(struct all_pars, CPDPRG)},
  {"CUREXP",                  32,   0x80820, 0, PARUNIT_no        , offsetof(struct all_pars, CUREXP)},
  {"CURPLOT",                 80,   0x80840, 0, PARUNIT_no        , offsetof(struct all_pars, CURPLOT)},
  {"CURPRIN",                 80,   0x80840, 0, PARUNIT_no        , offsetof(struct all_pars, CURPRIN)},
  {"CY",                       8,   0x40004, 0, PARUNIT_cm        , offsetof(struct all_pars, CY)},
  {"D",                      256,  0x138802, 0, PARUNIT_sec       , offsetof(struct all_pars, D)},
  {"DATE",                     4,   0x18802, 0, PARUNIT_sec       , offsetof(struct all_pars, DATE)},
  {"DATE_START",               4,   0x18802, 0, PARUNIT_sec       , offsetof(struct all_pars, DATE_START)},
  {"DATMOD",                   4,   0x58804, 0, PARUNIT_no        , offsetof(struct all_pars, DATMOD)},
  {"DATPATH",                336,   0x80020, 0, PARUNIT_no        , offsetof(struct all_pars, DATPATH)},
  {"DATPATH2",               336,   0x80020, 0, PARUNIT_no        , offsetof(struct all_pars, DATPATH2)},
  {"DATPATH3",               336,   0x80020, 0, PARUNIT_no        , offsetof(struct all_pars, DATPATH3)},
  {"DC",                       4,   0x30804, 0, PARUNIT_no        , offsetof(struct all_pars, DC)},
  {"DE",                       4,   0x38802, 0, PARUNIT_usec      , offsetof(struct all_pars, DE)},
  {"DE1",                      8,   0x4c090, 0, PARUNIT_usec      , offsetof(struct all_pars, DE1)},
  {"DE2",                      8,   0x48090, 0, PARUNIT_usec      , offsetof(struct all_pars, DE2)},
  {"DEADC",                    8,   0x4c090, 0, PARUNIT_usec      , offsetof(struct all_pars, DEADC)},
  {"DECBNUC",                  8,   0x80002, 0, PARUNIT_no        , offsetof(struct all_pars, DECBNUC)},
  {"DECIM",                    8,   0x48002, 0, PARUNIT_no        , offsetof(struct all_pars, DECIM)},
  {"DECNUC",                   8,   0x80002, 0, PARUNIT_no        , offsetof(struct all_pars, DECNUC)},
  {"DECSTAT",                  4,   0x58802, 0, PARUNIT_no        , offsetof(struct all_pars, DECSTAT)},
  {"DEPA",                     8,   0x4c090, 0, PARUNIT_usec      , offsetof(struct all_pars, DEPA)},
  {"DERX",                     8,   0x4c090, 0, PARUNIT_usec      , offsetof(struct all_pars, DERX)},
  {"DETAILS",                 64,   0x80820, 0, PARUNIT_no        , offsetof(struct all_pars, DETAILS)},
  {"DFILT",                   16,   0x88804, 0, PARUNIT_no        , offsetof(struct all_pars, DFILT)},
  {"DFORMAT",                 16,   0x80840, 0, PARUNIT_no        , offsetof(struct all_pars, DFORMAT)},
  {"DIGMOD",                   4,   0x5c002, 0, PARUNIT_no        , offsetof(struct all_pars, DIGMOD)},
  {"DIGTYP",                   4,   0x5c802, 0, PARUNIT_no        , offsetof(struct all_pars, DIGTYP)},
  {"DR",                       4,   0x1c802, 0, PARUNIT_no        , offsetof(struct all_pars, DR)},
  {"DS",                       4,   0x18802, 0, PARUNIT_no        , offsetof(struct all_pars, DS)},
  {"DSPFIRM",                  4,   0x5c002, 0, PARUNIT_no        , offsetof(struct all_pars, DSPFIRM)},
  {"DSPFVS",                   4,   0x18002, 0, PARUNIT_no        , offsetof(struct all_pars, DSPFVS)},
  {"DTYPA",                    4,   0x58002, 0, PARUNIT_no        , offsetof(struct all_pars, DTYPA)},
  {"DTYPP",                    4,   0x58004, 0, PARUNIT_no        , offsetof(struct all_pars, DTYPP)},
  {"DU",                     256,   0x88820, 0, PARUNIT_no        , offsetof(struct all_pars, DU)},
  {"DU2",                    256,   0x88820, 0, PARUNIT_no        , offsetof(struct all_pars, DU2)},
  {"DU3",                    256,   0x88820, 0, PARUNIT_no        , offsetof(struct all_pars, DU3)},
  {"ERETIC",                   4,   0x60004, 0, PARUNIT_no        , offsetof(struct all_pars, ERETIC)},
  {"EXP",                     64,   0x80002, 0, PARUNIT_no        , offsetof(struct all_pars, EXP)},
  {"EXPNO",                    4,   0x18820, 0, PARUNIT_no        , offsetof(struct all_pars, EXPNO)},
  {"EXPNO2",                   4,   0x18820, 0, PARUNIT_no        , offsetof(struct all_pars, EXPNO2)},
  {"EXPNO3",                   4,   0x18820, 0, PARUNIT_no        , offsetof(struct all_pars, EXPNO3)},
  {"ExpStatus",                4,   0x50002, 0, PARUNIT_no        , offsetof(struct all_pars, ExpStatus)},
  {"F1P",                      8,   0x40004, 0, PARUNIT_ppm       , offsetof(struct all_pars, F1P)},
  {"F2P",                      8,   0x40004, 0, PARUNIT_ppm       , offsetof(struct all_pars, F2P)},
  {"FCOR",                     4,   0x38004, 0, PARUNIT_no        , offsetof(struct all_pars, FCOR)},
  {"FCUCHAN",                 40,  0x118002, 0, PARUNIT_no        , offsetof(struct all_pars, FCUCHAN)},
  {"FILCOR",                   8,   0x40092, 0, PARUNIT_usec      , offsetof(struct all_pars, FILCOR)},
  {"FL1",                      4,   0x18002, 0, PARUNIT_dB        , offsetof(struct all_pars, FL1)},
  {"FL2",                      4,   0x18002, 0, PARUNIT_dB        , offsetof(struct all_pars, FL2)},
  {"FL3",                      4,   0x18002, 0, PARUNIT_dB        , offsetof(struct all_pars, FL3)},
  {"FL4",                      4,   0x18002, 0, PARUNIT_dB        , offsetof(struct all_pars, FL4)},
  {"FN_INDIRECT",             32,  0x110002, 0, PARUNIT_no        , offsetof(struct all_pars, FN_INDIRECT)},
  {"FOV",                      4,   0x38802, 0, PARUNIT_cm        , offsetof(struct all_pars, FOV)},
  {"FQ1LIST",                 32,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, FQ1LIST)},
  {"FQ2LIST",                 32,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, FQ2LIST)},
  {"FQ3LIST",                 32,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, FQ3LIST)},
  {"FQ4LIST",                 32,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, FQ4LIST)},
  {"FQ5LIST",                 32,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, FQ5LIST)},
  {"FQ6LIST",                 32,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, FQ6LIST)},
  {"FQ7LIST",                 32,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, FQ7LIST)},
  {"FQ8LIST",                 32,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, FQ8LIST)},
  {"FS",                      32,  0x118002, 0, PARUNIT_dB        , offsetof(struct all_pars, FS)},
  {"FTLPGN",                   4,   0x58002, 0, PARUNIT_no        , offsetof(struct all_pars, FTLPGN)},
  {"FTSIZE",                   4,   0x18004, 0, PARUNIT_no        , offsetof(struct all_pars, FTSIZE)},
  {"FT_mod",                   4,   0x58804, 0, PARUNIT_no        , offsetof(struct all_pars, FT_mod)},
  {"FW",                       4,   0x3c802, 0, PARUNIT_Hz        , offsetof(struct all_pars, FW)},
  {"FnILOOP",                  4,   0x10002, 0, PARUNIT_no        , offsetof(struct all_pars, FnILOOP)},
  {"FnMODE",                   4,   0x5c002, 0, PARUNIT_no        , offsetof(struct all_pars, FnMODE)},
  {"FnTYPE",                   4,   0x50002, 0, PARUNIT_no        , offsetof(struct all_pars, FnTYPE)},
  {"GAMMA",                    4,   0x30004, 0, PARUNIT_no        , offsetof(struct all_pars, GAMMA)},
  {"GB",                       4,   0x38804, 0, PARUNIT_no        , offsetof(struct all_pars, GB)},
  {"GPNAM",             0x200040,  0x180002, 0, PARUNIT_no        , offsetof(struct all_pars, GPNAM)},
  {"GPX",                    128,  0x138002, 0, PARUNIT_percent   , offsetof(struct all_pars, GPX)},
  {"GPY",                    128,  0x138002, 0, PARUNIT_percent   , offsetof(struct all_pars, GPY)},
  {"GPZ",                    128,  0x138002, 0, PARUNIT_percent   , offsetof(struct all_pars, GPZ)},
  {"GRADPRE",                 16,  0x140080, 0, PARUNIT_usec      , offsetof(struct all_pars, GRADPRE)},
  {"GRPDLY",                   8,   0x40002, 0, PARUNIT_no        , offsetof(struct all_pars, GRPDLY)},
  {"HDDUTY",                   4,   0x38002, 0, PARUNIT_percent   , offsetof(struct all_pars, HDDUTY)},
  {"HDRATE",                   4,   0x38002, 0, PARUNIT_no        , offsetof(struct all_pars, HDRATE)},
  {"HD_BLKTR",               132,  0x138090, 0, PARUNIT_usec      , offsetof(struct all_pars, HD_BLKTR)},
  {"HD_DE1",                   8,   0x4c090, 0, PARUNIT_usec      , offsetof(struct all_pars, HD_DE1)},
  {"HD_DEADC",                 8,   0x4c090, 0, PARUNIT_usec      , offsetof(struct all_pars, HD_DEADC)},
  {"HD_DEPA",                  8,   0x4c090, 0, PARUNIT_usec      , offsetof(struct all_pars, HD_DEPA)},
  {"HD_DERX",                  8,   0x4c090, 0, PARUNIT_usec      , offsetof(struct all_pars, HD_DERX)},
  {"HGAIN",                   16,  0x118002, 0, PARUNIT_no        , offsetof(struct all_pars, HGAIN)},
  {"HL1",                      4,   0x18802, 0, PARUNIT_dB        , offsetof(struct all_pars, HL1)},
  {"HL2",                      4,   0x18002, 0, PARUNIT_dB        , offsetof(struct all_pars, HL2)},
  {"HL3",                      4,   0x18002, 0, PARUNIT_dB        , offsetof(struct all_pars, HL3)},
  {"HL4",                      4,   0x18002, 0, PARUNIT_dB        , offsetof(struct all_pars, HL4)},
  {"HOLDER",                   4,   0x18002, 0, PARUNIT_no        , offsetof(struct all_pars, HOLDER)},
  {"HPMOD",                  132,  0x158002, 0, PARUNIT_no        , offsetof(struct all_pars, HPMOD)},
  {"HPPRGN",                   4,   0x58002, 0, PARUNIT_no        , offsetof(struct all_pars, HPPRGN)},
  {"IN",                     512,  0x148802, 0, PARUNIT_sec       , offsetof(struct all_pars, IN)},
  {"INF",                     64,  0x140002, 0, PARUNIT_usec      , offsetof(struct all_pars, INF)},
  {"INP",                    512,  0x148002, 0, PARUNIT_usec      , offsetof(struct all_pars, INP)},
  {"INSTRUM",                 64,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, INSTRUM)},
  {"INTBC",                    4,   0x58804, 0, PARUNIT_no        , offsetof(struct all_pars, INTBC)},
  {"INTEGFAC",               256,  0x138002, 0, PARUNIT_no        , offsetof(struct all_pars, INTEGFAC)},
  {"INTSCL",                   8,   0x48804, 0, PARUNIT_no        , offsetof(struct all_pars, INTSCL)},
  {"ISEN",                     4,   0x38804, 0, PARUNIT_no        , offsetof(struct all_pars, ISEN)},
  {"L",                      128,  0x118802, 0, PARUNIT_no        , offsetof(struct all_pars, L)},
  {"LAYOUT",                 256,   0x80040, 0, PARUNIT_no        , offsetof(struct all_pars, LAYOUT)},
  {"LB",                       4,   0x30804, 0, PARUNIT_Hz        , offsetof(struct all_pars, LB)},
  {"LEV0",                     4,   0x38004, 0, PARUNIT_no        , offsetof(struct all_pars, LEV0)},
  {"LFILTER",                  8,   0x48002, 0, PARUNIT_Hz        , offsetof(struct all_pars, LFILTER)},
  {"LFORMAT",                 16,   0x80840, 0, PARUNIT_no        , offsetof(struct all_pars, LFORMAT)},
  {"LGAIN",                    8,   0x48002, 0, PARUNIT_dB        , offsetof(struct all_pars, LGAIN)},
  {"LINPSTP",                  8,   0x40002, 0, PARUNIT_dB        , offsetof(struct all_pars, LINPSTP)},
  {"LOCKED",                   4,   0x60002, 0, PARUNIT_no        , offsetof(struct all_pars, LOCKED)},
  {"LOCKFLD",                  4,   0x10002, 0, PARUNIT_no        , offsetof(struct all_pars, LOCKFLD)},
  {"LOCKGN",                   8,   0x40002, 0, PARUNIT_dB        , offsetof(struct all_pars, LOCKGN)},
  {"LOCKPOW",                  8,   0x48002, 0, PARUNIT_dB        , offsetof(struct all_pars, LOCKPOW)},
  {"LOCKPPM",                  8,   0x40002, 0, PARUNIT_ppm       , offsetof(struct all_pars, LOCKPPM)},
  {"LOCNUC",                   8,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, LOCNUC)},
  {"LOCPHAS",                  4,   0x38002, 0, PARUNIT_degree    , offsetof(struct all_pars, LOCPHAS)},
  {"LOCSHFT",                  4,   0x60002, 0, PARUNIT_no        , offsetof(struct all_pars, LOCSHFT)},
  {"LOCSW",                    4,   0x10003, 0, PARUNIT_no        , offsetof(struct all_pars, LOCSW)},
  {"LPBIN",                    4,   0x18004, 0, PARUNIT_no        , offsetof(struct all_pars, LPBIN)},
  {"LTIME",                    8,   0x48002, 0, PARUNIT_sec       , offsetof(struct all_pars, LTIME)},
  {"MASR",                     4,   0x18002, 0, PARUNIT_Hz        , offsetof(struct all_pars, MASR)},
  {"MAXI",                     4,   0x30804, 0, PARUNIT_cm        , offsetof(struct all_pars, MAXI)},
  {"MC2",                      4,   0x58804, 0, PARUNIT_no        , offsetof(struct all_pars, MC2)},
  {"MEAN",                     8,   0x40804, 0, PARUNIT_no        , offsetof(struct all_pars, MEAN)},
  {"ME_mod",                   4,   0x58004, 0, PARUNIT_no        , offsetof(struct all_pars, ME_mod)},
  {"MI",                       4,   0x30804, 0, PARUNIT_cm        , offsetof(struct all_pars, MI)},
  {"MULEXPNO",                64,  0x110002, 0, PARUNIT_no        , offsetof(struct all_pars, MULEXPNO)},
  {"MddCEXP",                  4,   0x60804, 0, PARUNIT_no        , offsetof(struct all_pars, MddCEXP)},
  {"MddCT_SP",                 4,   0x60804, 0, PARUNIT_no        , offsetof(struct all_pars, MddCT_SP)},
  {"MddF180",                  4,   0x60804, 0, PARUNIT_no        , offsetof(struct all_pars, MddF180)},
  {"MddLAMBDA",                8,   0x48004, 0, PARUNIT_no        , offsetof(struct all_pars, MddLAMBDA)},
  {"MddMEMORY",                4,   0x18804, 0, PARUNIT_no        , offsetof(struct all_pars, MddMEMORY)},
  {"MddMERGE",                 4,   0x18804, 0, PARUNIT_no        , offsetof(struct all_pars, MddMERGE)},
  {"MddNCOMP",                 4,   0x18804, 0, PARUNIT_no        , offsetof(struct all_pars, MddNCOMP)},
  {"MddNITER",                 4,   0x18804, 0, PARUNIT_no        , offsetof(struct all_pars, MddNITER)},
  {"MddNOISE",                 8,   0x48004, 0, PARUNIT_no        , offsetof(struct all_pars, MddNOISE)},
  {"MddPHASE",                 8,   0x48004, 0, PARUNIT_no        , offsetof(struct all_pars, MddPHASE)},
  {"MddSEED",                  4,   0x18804, 0, PARUNIT_no        , offsetof(struct all_pars, MddSEED)},
  {"MddSRSIZE",                8,   0x48004, 0, PARUNIT_ppm       , offsetof(struct all_pars, MddSRSIZE)},
  {"Mdd_CsALG",                4,   0x58804, 0, PARUNIT_no        , offsetof(struct all_pars, Mdd_CsALG)},
  {"Mdd_CsLAMBDA",             8,   0x48004, 0, PARUNIT_no        , offsetof(struct all_pars, Mdd_CsLAMBDA)},
  {"Mdd_CsNITER",              4,   0x18804, 0, PARUNIT_no        , offsetof(struct all_pars, Mdd_CsNITER)},
  {"Mdd_CsNORM",               8,   0x48004, 0, PARUNIT_no        , offsetof(struct all_pars, Mdd_CsNORM)},
  {"Mdd_CsVE",                 4,   0x60004, 0, PARUNIT_no        , offsetof(struct all_pars, Mdd_CsVE)},
  {"Mdd_CsZF",                 4,   0x18804, 0, PARUNIT_no        , offsetof(struct all_pars, Mdd_CsZF)},
  {"Mdd_LrLAMBDA",             8,   0x48004, 0, PARUNIT_no        , offsetof(struct all_pars, Mdd_LrLAMBDA)},
  {"Mdd_LrNITER",              4,   0x18804, 0, PARUNIT_no        , offsetof(struct all_pars, Mdd_LrNITER)},
  {"Mdd_LrRANK",               8,   0x48004, 0, PARUNIT_no        , offsetof(struct all_pars, Mdd_LrRANK)},
  {"Mdd_LrZF",                 8,   0x48004, 0, PARUNIT_no        , offsetof(struct all_pars, Mdd_LrZF)},
  {"Mdd_mod",                  4,   0x58804, 0, PARUNIT_no        , offsetof(struct all_pars, Mdd_mod)},
  {"NAME",                   160,   0x80820, 0, PARUNIT_no        , offsetof(struct all_pars, NAME)},
  {"NAME2",                  160,   0x80820, 0, PARUNIT_no        , offsetof(struct all_pars, NAME2)},
  {"NAME3",                  160,   0x80820, 0, PARUNIT_no        , offsetof(struct all_pars, NAME3)},
  {"NBL",                      4,   0x18802, 0, PARUNIT_no        , offsetof(struct all_pars, NBL)},
  {"NC",                       4,   0x10602, 0, PARUNIT_no        , offsetof(struct all_pars, NC)},
  {"NCOEF",                    4,   0x18004, 0, PARUNIT_no        , offsetof(struct all_pars, NCOEF)},
  {"NC_proc",                  4,   0x10004, 0, PARUNIT_no        , offsetof(struct all_pars, NC_proc)},
  {"NLEV",                     4,   0x18004, 0, PARUNIT_no        , offsetof(struct all_pars, NLEV)},
  {"NLOGCH",                   4,   0x10002, 0, PARUNIT_no        , offsetof(struct all_pars, NLOGCH)},
  {"NOISF1",                   4,   0x30004, 0, PARUNIT_ppm       , offsetof(struct all_pars, NOISF1)},
  {"NOISF2",                   4,   0x30004, 0, PARUNIT_ppm       , offsetof(struct all_pars, NOISF2)},
  {"NOVFLW",                   4,   0x10002, 0, PARUNIT_no        , offsetof(struct all_pars, NOVFLW)},
  {"NS",                       4,   0x18802, 0, PARUNIT_no        , offsetof(struct all_pars, NS)},
  {"NSP",                      4,   0x18804, 0, PARUNIT_no        , offsetof(struct all_pars, NSP)},
  {"NTH_PI",                   4,   0x10404, 0, PARUNIT_no        , offsetof(struct all_pars, NTH_PI)},
  {"NUC1",                     8,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, NUC1)},
  {"NUC2",                     8,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, NUC2)},
  {"NUC3",                     8,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, NUC3)},
  {"NUC4",                     8,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, NUC4)},
  {"NUC5",                     8,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, NUC5)},
  {"NUC6",                     8,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, NUC6)},
  {"NUC7",                     8,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, NUC7)},
  {"NUC8",                     8,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, NUC8)},
  {"NUCLEUS",                  8,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, NUCLEUS)},
  {"NUSLIST",                 32,   0x80002, 0, PARUNIT_no        , offsetof(struct all_pars, NUSLIST)},
  {"NZP",                      4,   0x18804, 0, PARUNIT_no        , offsetof(struct all_pars, NZP)},
  {"NusAMOUNT",                8,   0x48002, 0, PARUNIT_percent   , offsetof(struct all_pars, NusAMOUNT)},
  {"NusFPNZ",                  4,   0x60802, 0, PARUNIT_no        , offsetof(struct all_pars, NusFPNZ)},
  {"NusJSP",                   8,   0x48002, 0, PARUNIT_Hz        , offsetof(struct all_pars, NusJSP)},
  {"NusSEED",                  4,   0x10002, 0, PARUNIT_no        , offsetof(struct all_pars, NusSEED)},
  {"NusSPTYPE",                4,   0x10002, 0, PARUNIT_no        , offsetof(struct all_pars, NusSPTYPE)},
  {"NusT2",                    8,   0x48002, 0, PARUNIT_sec       , offsetof(struct all_pars, NusT2)},
  {"NusTD",                    4,   0x18002, 0, PARUNIT_no        , offsetof(struct all_pars, NusTD)},
  {"O1",                       8,   0x44002, 0, PARUNIT_Hz        , offsetof(struct all_pars, O1)},
  {"O2",                       8,   0x44002, 0, PARUNIT_Hz        , offsetof(struct all_pars, O2)},
  {"O3",                       8,   0x44002, 0, PARUNIT_Hz        , offsetof(struct all_pars, O3)},
  {"O4",                       8,   0x44002, 0, PARUNIT_Hz        , offsetof(struct all_pars, O4)},
  {"O5",                       8,   0x44002, 0, PARUNIT_Hz        , offsetof(struct all_pars, O5)},
  {"O6",                       8,   0x44002, 0, PARUNIT_Hz        , offsetof(struct all_pars, O6)},
  {"O7",                       8,   0x44002, 0, PARUNIT_Hz        , offsetof(struct all_pars, O7)},
  {"O8",                       8,   0x44002, 0, PARUNIT_Hz        , offsetof(struct all_pars, O8)},
  {"OFFSET",                   4,   0x34804, 0, PARUNIT_ppm       , offsetof(struct all_pars, OFFSET)},
  {"OVERFLW",                  4,   0x58002, 0, PARUNIT_no        , offsetof(struct all_pars, OVERFLW)},
  {"P",                      256,  0x138802, 0, PARUNIT_usec      , offsetof(struct all_pars, P)},
  {"PACOIL",                 132,  0x118002, 0, PARUNIT_no        , offsetof(struct all_pars, PACOIL)},
  {"PAPS",                     4,   0x58802, 0, PARUNIT_no        , offsetof(struct all_pars, PAPS)},
  {"PARMODE",                  4,   0x58002, 0, PARUNIT_no        , offsetof(struct all_pars, PARMODE)},
  {"PC",                       4,   0x30804, 0, PARUNIT_no        , offsetof(struct all_pars, PC)},
  {"PCPD",                    40,  0x138002, 0, PARUNIT_usec      , offsetof(struct all_pars, PCPD)},
  {"PEXSEL",                  40,  0x110002, 0, PARUNIT_no        , offsetof(struct all_pars, PEXSEL)},
  {"PFORMAT",                 16,   0x80840, 0, PARUNIT_no        , offsetof(struct all_pars, PFORMAT)},
  {"PHC0",                     4,   0x30804, 0, PARUNIT_degree    , offsetof(struct all_pars, PHC0)},
  {"PHC1",                     4,   0x30804, 0, PARUNIT_degree    , offsetof(struct all_pars, PHC1)},
  {"PHCOR",                  128,  0x138002, 0, PARUNIT_degree    , offsetof(struct all_pars, PHCOR)},
  {"PHLIST",                  32,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, PHLIST)},
  {"PH_mod",                   4,   0x58804, 0, PARUNIT_no        , offsetof(struct all_pars, PH_mod)},
  {"PH_ref",                   4,   0x38002, 0, PARUNIT_degree    , offsetof(struct all_pars, PH_ref)},
  {"PKNL",                     4,   0x60004, 0, PARUNIT_no        , offsetof(struct all_pars, PKNL)},
  {"PL",                     256,  0x138002, 0, PARUNIT_dB        , offsetof(struct all_pars, PL)},
  {"PLSTEP",                   4,   0x30002, 0, PARUNIT_no        , offsetof(struct all_pars, PLSTEP)},
  {"PLSTRT",                   4,   0x30002, 0, PARUNIT_dB        , offsetof(struct all_pars, PLSTRT)},
  {"PLW",                    256,  0x130002, 0, PARUNIT_W         , offsetof(struct all_pars, PLW)},
  {"PLWMAX",                  32,  0x130002, 0, PARUNIT_W         , offsetof(struct all_pars, PLWMAX)},
  {"PPARMOD",                  4,   0x58004, 0, PARUNIT_no        , offsetof(struct all_pars, PPARMOD)},
  {"PPDIAG",                   4,   0x10004, 0, PARUNIT_no        , offsetof(struct all_pars, PPDIAG)},
  {"PPIPTYP",                  4,   0x50004, 0, PARUNIT_no        , offsetof(struct all_pars, PPIPTYP)},
  {"PPMPNUM",                  4,   0x10004, 0, PARUNIT_no        , offsetof(struct all_pars, PPMPNUM)},
  {"PPRESOL",                  4,   0x10004, 0, PARUNIT_no        , offsetof(struct all_pars, PPRESOL)},
  {"PQPHASE",                  8,   0x40002, 0, PARUNIT_degree    , offsetof(struct all_pars, PQPHASE)},
  {"PQSCALE",                  8,   0x40002, 0, PARUNIT_no        , offsetof(struct all_pars, PQSCALE)},
  {"PR",                       4,   0x58802, 0, PARUNIT_no        , offsetof(struct all_pars, PR)},
  {"PRECHAN",                132,  0x118002, 0, PARUNIT_no        , offsetof(struct all_pars, PRECHAN)},
  {"PROBHD",                 128,   0x80002, 0, PARUNIT_no        , offsetof(struct all_pars, PROBHD)},
  {"PROBINPUTS",        0x100040,  0x180002, 0, PARUNIT_no        , offsetof(struct all_pars, PROBINPUTS)},
  {"PROCNO",                   4,   0x18820, 0, PARUNIT_no        , offsetof(struct all_pars, PROCNO)},
  {"PROCNO2",                  4,   0x18820, 0, PARUNIT_no        , offsetof(struct all_pars, PROCNO2)},
  {"PROCNO3",                  4,   0x18820, 0, PARUNIT_no        , offsetof(struct all_pars, PROCNO3)},
  {"PSCAL",                    4,   0x58804, 0, PARUNIT_no        , offsetof(struct all_pars, PSCAL)},
  {"PSIGN",                    4,   0x58804, 0, PARUNIT_no        , offsetof(struct all_pars, PSIGN)},
  {"PULPROG",                 32,   0x80002, 0, PARUNIT_no        , offsetof(struct all_pars, PULPROG)},
  {"PW",                       4,   0x38002, 0, PARUNIT_usec      , offsetof(struct all_pars, PW)},
  {"PYNM",                    32,   0x80002, 0, PARUNIT_no        , offsetof(struct all_pars, PYNM)},
  {"PYNMP",                   32,   0x80004, 0, PARUNIT_no        , offsetof(struct all_pars, PYNMP)},
  {"ProjAngle",                8,   0x40002, 0, PARUNIT_degree    , offsetof(struct all_pars, ProjAngle)},
  {"QNP",                      4,   0x18002, 0, PARUNIT_no        , offsetof(struct all_pars, QNP)},
  {"RDF1",                     8,   0x40004, 0, PARUNIT_ppm       , offsetof(struct all_pars, RDF1)},
  {"RDF2",                     8,   0x40004, 0, PARUNIT_ppm       , offsetof(struct all_pars, RDF2)},
  {"RDFWHM",                   8,   0x48004, 0, PARUNIT_Hz        , offsetof(struct all_pars, RDFWHM)},
  {"RDINT",                    8,   0x40004, 0, PARUNIT_no        , offsetof(struct all_pars, RDINT)},
  {"RDPOS",                    8,   0x40004, 0, PARUNIT_ppm       , offsetof(struct all_pars, RDPOS)},
  {"RECCHAN",                 40,  0x118002, 0, PARUNIT_no        , offsetof(struct all_pars, RECCHAN)},
  {"RECPH",                    4,   0x38002, 0, PARUNIT_no        , offsetof(struct all_pars, RECPH)},
  {"RECPRE",                 100,  0x110002, 0, PARUNIT_no        , offsetof(struct all_pars, RECPRE)},
  {"RECPRFX",                132,  0x110002, 0, PARUNIT_no        , offsetof(struct all_pars, RECPRFX)},
  {"RECSEL",                 100,  0x110002, 0, PARUNIT_no        , offsetof(struct all_pars, RECSEL)},
  {"REVERSE",                  4,   0x60804, 0, PARUNIT_no        , offsetof(struct all_pars, REVERSE)},
  {"RG",                       4,   0x3c802, 0, PARUNIT_no        , offsetof(struct all_pars, RG)},
  {"RO",                       4,   0x18802, 0, PARUNIT_Hz        , offsetof(struct all_pars, RO)},
  {"RSEL",                   100,  0x118002, 0, PARUNIT_no        , offsetof(struct all_pars, RSEL)},
  {"SAMPLEID",               256,   0x80002, 0, PARUNIT_no        , offsetof(struct all_pars, SAMPLEID)},
  {"SCALE2",                   8,   0x48020, 0, PARUNIT_no        , offsetof(struct all_pars, SCALE2)},
  {"SCALE3",                   8,   0x48020, 0, PARUNIT_no        , offsetof(struct all_pars, SCALE3)},
  {"SELREC",                 100,  0x110002, 0, PARUNIT_no        , offsetof(struct all_pars, SELREC)},
  {"SF",                       8,   0x4c804, 0, PARUNIT_MHz       , offsetof(struct all_pars, SF)},
  {"SFO1",                     8,   0x4c802, 0, PARUNIT_MHz       , offsetof(struct all_pars, SFO1)},
  {"SFO2",                     8,   0x4c802, 0, PARUNIT_MHz       , offsetof(struct all_pars, SFO2)},
  {"SFO3",                     8,   0x4c002, 0, PARUNIT_MHz       , offsetof(struct all_pars, SFO3)},
  {"SFO4",                     8,   0x44002, 0, PARUNIT_MHz       , offsetof(struct all_pars, SFO4)},
  {"SFO5",                     8,   0x44002, 0, PARUNIT_MHz       , offsetof(struct all_pars, SFO5)},
  {"SFO6",                     8,   0x44002, 0, PARUNIT_MHz       , offsetof(struct all_pars, SFO6)},
  {"SFO7",                     8,   0x44002, 0, PARUNIT_MHz       , offsetof(struct all_pars, SFO7)},
  {"SFO8",                     8,   0x44002, 0, PARUNIT_MHz       , offsetof(struct all_pars, SFO8)},
  {"SHAPPR",                  64,  0x138090, 0, PARUNIT_usec      , offsetof(struct all_pars, SHAPPR)},
  {"SI",                       4,   0x1c804, 0, PARUNIT_no        , offsetof(struct all_pars, SI)},
  {"SIGF1",                    4,   0x30004, 0, PARUNIT_ppm       , offsetof(struct all_pars, SIGF1)},
  {"SIGF2",                    4,   0x30004, 0, PARUNIT_ppm       , offsetof(struct all_pars, SIGF2)},
  {"SINO",                     4,   0x38004, 0, PARUNIT_no        , offsetof(struct all_pars, SINO)},
  {"SIOLD",                    4,   0x18004, 0, PARUNIT_no        , offsetof(struct all_pars, SIOLD)},
  {"SOLVENT",                 32,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, SOLVENT)},
  {"SOLVOLD",                 32,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, SOLVOLD)},
  {"SP",                     256,  0x138802, 0, PARUNIT_dB        , offsetof(struct all_pars, SP)},
  {"SPECTR",                   4,   0x50002, 0, PARUNIT_no        , offsetof(struct all_pars, SPECTR)},
  {"SPECTYP",                 64,   0x80004, 0, PARUNIT_no        , offsetof(struct all_pars, SPECTYP)},
  {"SPINCNT",                  4,   0x10002, 0, PARUNIT_no        , offsetof(struct all_pars, SPINCNT)},
  {"SPNAM",             0x400040,  0x180002, 0, PARUNIT_no        , offsetof(struct all_pars, SPNAM)},
  {"SPOAL",                  256,  0x138002, 0, PARUNIT_no        , offsetof(struct all_pars, SPOAL)},
  {"SPOFFS",                 256,  0x130002, 0, PARUNIT_Hz        , offsetof(struct all_pars, SPOFFS)},
  {"SPPEX",                  256,  0x150002, 0, PARUNIT_no        , offsetof(struct all_pars, SPPEX)},
  {"SPW",                    256,  0x130002, 0, PARUNIT_W         , offsetof(struct all_pars, SPW)},
  {"SREF_mod",                 4,   0x58004, 0, PARUNIT_no        , offsetof(struct all_pars, SREF_mod)},
  {"SREGLST",                 40,   0x80804, 0, PARUNIT_no        , offsetof(struct all_pars, SREGLST)},
  {"SSB",                      4,   0x38804, 0, PARUNIT_no        , offsetof(struct all_pars, SSB)},
  {"STSI",                     4,   0x1c004, 0, PARUNIT_no        , offsetof(struct all_pars, STSI)},
  {"STSR",                     4,   0x1c004, 0, PARUNIT_no        , offsetof(struct all_pars, STSR)},
  {"SUBNAM",             0xa0010,  0x180002, 0, PARUNIT_no        , offsetof(struct all_pars, SUBNAM)},
  {"SW",                       8,   0x4c802, 0, PARUNIT_ppm       , offsetof(struct all_pars, SW)},
  {"SWIBOX",                 132,  0x118002, 0, PARUNIT_no        , offsetof(struct all_pars, SWIBOX)},
  {"SW_expin",                 4,   0x60080, 0, PARUNIT_no        , offsetof(struct all_pars, SW_expin)},
  {"SW_h",                     8,   0x4c802, 0, PARUNIT_Hz        , offsetof(struct all_pars, SW_h)},
  {"SW_p",                     8,   0x48804, 0, PARUNIT_Hz        , offsetof(struct all_pars, SW_p)},
  {"SWfinal",                  8,   0x44002, 0, PARUNIT_Hz        , offsetof(struct all_pars, SWfinal)},
  {"SYMM",                     4,   0x58804, 0, PARUNIT_no        , offsetof(struct all_pars, SYMM)},
  {"S_DEV",                    4,   0x30804, 0, PARUNIT_no        , offsetof(struct all_pars, S_DEV)},
  {"SigLockShift",             8,   0x40002, 0, PARUNIT_no        , offsetof(struct all_pars, SigLockShift)},
  {"TD",                       4,   0x1c802, 0, PARUNIT_no        , offsetof(struct all_pars, TD)},
  {"TD0",                      4,   0x18002, 0, PARUNIT_no        , offsetof(struct all_pars, TD0)},
  {"TD_INDIRECT",             32,  0x110002, 0, PARUNIT_no        , offsetof(struct all_pars, TD_INDIRECT)},
  {"TDav",                     4,   0x10002, 0, PARUNIT_no        , offsetof(struct all_pars, TDav)},
  {"TDeff",                    4,   0x1c004, 0, PARUNIT_no        , offsetof(struct all_pars, TDeff)},
  {"TDoff",                    4,   0x18004, 0, PARUNIT_no        , offsetof(struct all_pars, TDoff)},
  {"TE",                       4,   0x38802, 0, PARUNIT_Kelvin    , offsetof(struct all_pars, TE)},
  {"TE1",                      4,   0x38802, 0, PARUNIT_Kelvin    , offsetof(struct all_pars, TE1)},
  {"TE2",                      4,   0x38802, 0, PARUNIT_Kelvin    , offsetof(struct all_pars, TE2)},
  {"TE3",                      4,   0x38002, 0, PARUNIT_Kelvin    , offsetof(struct all_pars, TE3)},
  {"TE4",                      4,   0x38002, 0, PARUNIT_Kelvin    , offsetof(struct all_pars, TE4)},
  {"TEG",                      4,   0x38002, 0, PARUNIT_Kelvin    , offsetof(struct all_pars, TEG)},
  {"TE_MAGNET",                4,   0x38802, 0, PARUNIT_Kelvin    , offsetof(struct all_pars, TE_MAGNET)},
  {"TE_PIDX",                  4,   0x10002, 0, PARUNIT_no        , offsetof(struct all_pars, TE_PIDX)},
  {"TE_RAW",                   4,   0x38802, 0, PARUNIT_Kelvin    , offsetof(struct all_pars, TE_RAW)},
  {"TE_STAB",                 40,  0x150002, 0, PARUNIT_no        , offsetof(struct all_pars, TE_STAB)},
  {"TI",                      72,   0x80804, 0, PARUNIT_no        , offsetof(struct all_pars, TI)},
  {"TILT",                     4,   0x60804, 0, PARUNIT_no        , offsetof(struct all_pars, TILT)},
  {"TM1",                      4,   0x38804, 0, PARUNIT_no        , offsetof(struct all_pars, TM1)},
  {"TM2",                      4,   0x38804, 0, PARUNIT_no        , offsetof(struct all_pars, TM2)},
  {"TOPLEV",                   4,   0x38004, 0, PARUNIT_no        , offsetof(struct all_pars, TOPLEV)},
  {"TOTROT",                 256,  0x138002, 0, PARUNIT_no        , offsetof(struct all_pars, TOTROT)},
  {"TUBE_TYPE",               32,   0x80002, 0, PARUNIT_no        , offsetof(struct all_pars, TUBE_TYPE)},
  {"TYPE",                    16,   0x88820, 0, PARUNIT_no        , offsetof(struct all_pars, TYPE)},
  {"USER",                    64,   0x80820, 0, PARUNIT_no        , offsetof(struct all_pars, USER)},
  {"USER2",                   64,   0x80820, 0, PARUNIT_no        , offsetof(struct all_pars, USER2)},
  {"USER3",                   64,   0x80820, 0, PARUNIT_no        , offsetof(struct all_pars, USER3)},
  {"USERA1",                 256,   0x80002, 0, PARUNIT_no        , offsetof(struct all_pars, USERA1)},
  {"USERA2",                 256,   0x80002, 0, PARUNIT_no        , offsetof(struct all_pars, USERA2)},
  {"USERA3",                 256,   0x80002, 0, PARUNIT_no        , offsetof(struct all_pars, USERA3)},
  {"USERA4",                 256,   0x80002, 0, PARUNIT_no        , offsetof(struct all_pars, USERA4)},
  {"USERA5",                 256,   0x80002, 0, PARUNIT_no        , offsetof(struct all_pars, USERA5)},
  {"USERP1",                 256,   0x80004, 0, PARUNIT_no        , offsetof(struct all_pars, USERP1)},
  {"USERP2",                 256,   0x80004, 0, PARUNIT_no        , offsetof(struct all_pars, USERP2)},
  {"USERP3",                 256,   0x80004, 0, PARUNIT_no        , offsetof(struct all_pars, USERP3)},
  {"USERP4",                 256,   0x80004, 0, PARUNIT_no        , offsetof(struct all_pars, USERP4)},
  {"USERP5",                 256,   0x80004, 0, PARUNIT_no        , offsetof(struct all_pars, USERP5)},
  {"UUID",                   256,   0x88802, 0, PARUNIT_no        , offsetof(struct all_pars, UUID)},
  {"V9",                       4,   0x38802, 0, PARUNIT_percent   , offsetof(struct all_pars, V9)},
  {"VALIDCODE",                4,   0x10002, 0, PARUNIT_no        , offsetof(struct all_pars, VALIDCODE)},
  {"VALIST",                  32,   0x80002, 0, PARUNIT_no        , offsetof(struct all_pars, VALIST)},
  {"VCLIST",                  32,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, VCLIST)},
  {"VDLIST",                  32,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, VDLIST)},
  {"VPLIST",                  32,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, VPLIST)},
  {"VTLIST",                  32,   0x80802, 0, PARUNIT_no        , offsetof(struct all_pars, VTLIST)},
  {"WBST",                     4,   0x18002, 0, PARUNIT_no        , offsetof(struct all_pars, WBST)},
  {"WBSW",                    32,  0x130002, 0, PARUNIT_MHz       , offsetof(struct all_pars, WBSW)},
  {"WDW",                      4,   0x58804, 0, PARUNIT_no        , offsetof(struct all_pars, WDW)},
  {"XDIM",                     4,   0x18804, 0, PARUNIT_no        , offsetof(struct all_pars, XDIM)},
  {"XGAIN",                   16,  0x118002, 0, PARUNIT_no        , offsetof(struct all_pars, XGAIN)},
  {"XL",                       4,   0x18002, 0, PARUNIT_no        , offsetof(struct all_pars, XL)},
  {"YL",                       4,   0x18002, 0, PARUNIT_no        , offsetof(struct all_pars, YL)},
  {"YMAX_a",                   8,   0x40802, 0, PARUNIT_no        , offsetof(struct all_pars, YMAX_a)},
  {"YMAX_p",                   4,   0x10804, 0, PARUNIT_no        , offsetof(struct all_pars, YMAX_p)},
  {"YMIN_a",                   8,   0x40802, 0, PARUNIT_no        , offsetof(struct all_pars, YMIN_a)},
  {"YMIN_p",                   4,   0x10804, 0, PARUNIT_no        , offsetof(struct all_pars, YMIN_p)},
  {"ZGOPTNS",                512,   0x80002, 0, PARUNIT_no        , offsetof(struct all_pars, ZGOPTNS)},
  {"ZL1",                      4,   0x38002, 0, PARUNIT_dB        , offsetof(struct all_pars, ZL1)},
  {"ZL2",                      4,   0x38002, 0, PARUNIT_dB        , offsetof(struct all_pars, ZL2)},
  {"ZL3",                      4,   0x38002, 0, PARUNIT_dB        , offsetof(struct all_pars, ZL3)},
  {"ZL4",                      4,   0x38002, 0, PARUNIT_dB        , offsetof(struct all_pars, ZL4)},
  {"scaledByNS",               4,   0x60002, 0, PARUNIT_no        , offsetof(struct all_pars, scaledByNS)},
  {"scaledByRG",               4,   0x60002, 0, PARUNIT_no        , offsetof(struct all_pars, scaledByRG)},
  {"zz00",             4,   0x40000, 0, PARUNIT_no        , offsetof(struct all_pars, zz00)},

	/* initialization of second table */


      405,                          /* A000 */
  1.000000E+01F,                                                      /* ABSF1 */
  0.000000E+00F,                                                      /* ABSF2 */
  5,                                                                  /* ABSG */
  3.000000E+00F,                                                      /* ABSL */
  0.000000000000E+00,                                                 /* ACQT0 */
  0.000000E+00F,                                                      /* ALPHA */
  {                                                                   /* AMP */
	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,
	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,
	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,
	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,
	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,
	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,
	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,
	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,	1.000000E+02F
  },
  {                                                                   /* AMPCOIL */
	0
  },
  0,                                                                  /* AQORDER */
  0,                                                                  /* AQSEQ */
  0,                                                                  /* AQ_mod */
  0.000000E+00F,                                                      /* ASSFAC */
  0.000000E+00F,                                                      /* ASSFACI */
  0.000000E+00F,                                                      /* ASSFACX */
  0.000000E+00F,                                                      /* ASSWID */
  {                                                                   /* AUNM */
   "au_zg"  },
  {                                                                   /* AUNMP */
   "proc_1d"  },
  {                                                                   /* AUTOPOS */
   ""  },
  0.000000000000E+00,                                                 /* AXLEFT */
  {                                                                   /* AXNAME */
   ""  },
  {                                                                   /* AXNUC */
   "off"  },
  0.000000000000E+00,                                                 /* AXRIGHT */
  0,                                                                  /* AXTYPE */
  {                                                                   /* AXUNIT */
   ""  },
  1.000000E-01F,                                                      /* AZFE */
  5.000000E-01F,                                                      /* AZFW */
  1.000000E+00F,                                                      /* BCFW */
  0,                                                                  /* BC_mod */
  1.001300000000E+02,                                                 /* BF1 */
  1.001300000000E+02,                                                 /* BF2 */
  1.001300000000E+02,                                                 /* BF3 */
  5.001300000000E+02,                                                 /* BF4 */
  5.001300000000E+02,                                                 /* BF5 */
  5.001300000000E+02,                                                 /* BF6 */
  5.001300000000E+02,                                                 /* BF7 */
  5.001300000000E+02,                                                 /* BF8 */
  {                                                                   /* BLKTR */
	3.000000E+00F,	3.000000E+00F,	3.000000E+00F,	3.000000E+00F,
	3.000000E+00F,	3.000000E+00F,	3.000000E+00F,	3.000000E+00F,
	3.000000E+00F,	3.000000E+00F,	3.000000E+00F,	3.000000E+00F,
	3.000000E+00F,	3.000000E+00F,	3.000000E+00F,	3.000000E+00F,
	3.000000E+00F,	3.000000E+00F,	3.000000E+00F,	3.000000E+00F,
	3.000000E+00F,	3.000000E+00F,	3.000000E+00F,	3.000000E+00F,
	3.000000E+00F,	3.000000E+00F,	3.000000E+00F,	3.000000E+00F,
	3.000000E+00F,	3.000000E+00F,	3.000000E+00F,	3.000000E+00F,
	3.000000E+00F
  },
  0.000000000000E+00,                                                 /* BLKTR_MAN */
  {                                                                   /* BWFAC */
	0.000000E+00F
  },
  0,                                                                  /* BYTORDA */
  0,                                                                  /* BYTORDP */
  {                                                                   /* CHANFCU */
	0
  },
  {                                                                   /* CHANREC */
	0
  },
  {                                                                   /* CHEMSTR */
   "none"  },
  {                                                                   /* CNST */
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F
  },
  0.000000000000E+00,                                                 /* COROFFS */
  {                                                                   /* CPDPRG */
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},  },
  {                                                                   /* CUREXP */
   ""  },
  {                                                                   /* CURPLOT */
   ""  },
  {                                                                   /* CURPRIN */
   ""  },
  1.500000000000E+01,                                                 /* CY */
  {                                                                   /* D */
	0.000000E+00F
  },
  0,                                                                  /* DATE */
  0,                                                                  /* DATE_START */
  1,                                                                  /* DATMOD */
  {                                                                   /* DATPATH */
   ""  },
  {                                                                   /* DATPATH2 */
   ""  },
  {                                                                   /* DATPATH3 */
   ""  },
  1.000000E+00F,                                                      /* DC */
  6.250000E+02F,                                                      /* DE */
  0.000000000000E+00,                                                 /* DE1 */
  2.000000000000E+00,                                                 /* DE2 */
  0.000000000000E+00,                                                 /* DEADC */
  {                                                                   /* DECBNUC */
   "off"  },
  1.000000000000E+00,                                                 /* DECIM */
  {                                                                   /* DECNUC */
   "off"  },
  7,                                                                  /* DECSTAT */
  0.000000000000E+00,                                                 /* DEPA */
  0.000000000000E+00,                                                 /* DERX */
  {                                                                   /* DETAILS */
   ""  },
  {                                                                   /* DFILT */
   ""  },
  {                                                                   /* DFORMAT */
   ""  },
  1,                                                                  /* DIGMOD */
  6,                                                                  /* DIGTYP */
  16,                                                                 /* DR */
  0,                                                                  /* DS */
  0,                                                                  /* DSPFIRM */
  0,                                                                  /* DSPFVS */
  0,                                                                  /* DTYPA */
  0,                                                                  /* DTYPP */
  {                                                                   /* DU */
   "/u"  },
  {                                                                   /* DU2 */
   "/u"  },
  {                                                                   /* DU3 */
   "/u"  },
  0,                                                                  /* ERETIC */
  {                                                                   /* EXP */
   ""  },
  1,                                                                  /* EXPNO */
  2,                                                                  /* EXPNO2 */
  3,                                                                  /* EXPNO3 */
  0,                                                                  /* ExpStatus */
  0.000000000000E+00,                                                 /* F1P */
  0.000000000000E+00,                                                 /* F2P */
  5.000000E-01F,                                                      /* FCOR */
  {                                                                   /* FCUCHAN */
	0
  },
  0.000000000000E+00,                                                 /* FILCOR */
  90,                                                                 /* FL1 */
  90,                                                                 /* FL2 */
  90,                                                                 /* FL3 */
  90,                                                                 /* FL4 */
  {                                                                   /* FN_INDIRECT */
	0
  },
  2.000000E+01F,                                                      /* FOV */
  {                                                                   /* FQ1LIST */
   "freqlist"  },
  {                                                                   /* FQ2LIST */
   "freqlist"  },
  {                                                                   /* FQ3LIST */
   "freqlist"  },
  {                                                                   /* FQ4LIST */
   "freqlist"  },
  {                                                                   /* FQ5LIST */
   "freqlist"  },
  {                                                                   /* FQ6LIST */
   "freqlist"  },
  {                                                                   /* FQ7LIST */
   "freqlist"  },
  {                                                                   /* FQ8LIST */
   "freqlist"  },
  {                                                                   /* FS */
	83,	83,	83,	83,
	83,	83,	83,	83
  },
  0,                                                                  /* FTLPGN */
  0,                                                                  /* FTSIZE */
  0,                                                                  /* FT_mod */
  2.500000E+03F,                                                      /* FW */
  0,                                                                  /* FnILOOP */
  0,                                                                  /* FnMODE */
  0,                                                                  /* FnTYPE */
  1.000000E+00F,                                                      /* GAMMA */
  1.000000E-01F,                                                      /* GB */
  {                                                                   /* GPNAM */
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
    {""}, {""}, {""}, {""}, {""}, {""},  },
  {                                                                   /* GPX */
	0.000000E+00F
  },
  {                                                                   /* GPY */
	0.000000E+00F
  },
  {                                                                   /* GPZ */
	0.000000E+00F
  },
  {                                                                   /* GRADPRE */
  0.000000000000E+00
  },
  -1.000000000000E+00,                                                /* GRPDLY */
  2.000000E+01F,                                                      /* HDDUTY */
  1.000000E+00F,                                                      /* HDRATE */
  {                                                                   /* HD_BLKTR */
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F
  },
  5.000000000000E+00,                                                 /* HD_DE1 */
  0.000000000000E+00,                                                 /* HD_DEADC */
  2.500000000000E+00,                                                 /* HD_DEPA */
  0.000000000000E+00,                                                 /* HD_DERX */
  {                                                                   /* HGAIN */
	0
  },
  90,                                                                 /* HL1 */
  90,                                                                 /* HL2 */
  90,                                                                 /* HL3 */
  90,                                                                 /* HL4 */
  0,                                                                  /* HOLDER */
  {                                                                   /* HPMOD */
	0
  },
  0,                                                                  /* HPPRGN */
  {                                                                   /* IN */
	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,
	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,
	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,
	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,
	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,
	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,
	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,
	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,
	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,
	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,
	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,
	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,
	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,
	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,
	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,
	1.000000000000E-03,	1.000000000000E-03,	1.000000000000E-03,  1.000000000000E-03
  },
  {                                                                   /* INF */
  0.000000000000E+00
  },
  {                                                                   /* INP */
  0.000000000000E+00
  },
  {                                                                   /* INSTRUM */
   ""  },
  1,                                                                  /* INTBC */
  {                                                                   /* INTEGFAC */
	0.000000E+00F
  },
  1.000000000000E+00,                                                 /* INTSCL */
  1.280000E+02F,                                                      /* ISEN */
  {                                                                   /* L */
	1,	1,	1,	1,
	1,	1,	1,	1,
	1,	1,	1,	1,
	1,	1,	1,	1,
	1,	1,	1,	1,
	1,	1,	1,	1,
	1,	1,	1,	1,
	1,	1,	1,	1
  },
  {                                                                   /* LAYOUT */
   ""  },
  1.000000E+00F,                                                      /* LB */
  0.000000E+00F,                                                      /* LEV0 */
  1.000000000000E+01,                                                 /* LFILTER */
  {                                                                   /* LFORMAT */
   ""  },
  -1.000000000000E+01,                                                /* LGAIN */
  0.000000000000E+00,                                                 /* LINPSTP */
  0,                                                                  /* LOCKED */
  0,                                                                  /* LOCKFLD */
  0.000000000000E+00,                                                 /* LOCKGN */
  -2.000000000000E+01,                                                /* LOCKPOW */
  0.000000000000E+00,                                                 /* LOCKPPM */
  {                                                                   /* LOCNUC */
   "2H"  },
  0.000000E+00F,                                                      /* LOCPHAS */
  0,                                                                  /* LOCSHFT */
  0,                                                                  /* LOCSW */
  0,                                                                  /* LPBIN */
  1.000000000000E-01,                                                 /* LTIME */
  0,                                                                  /* MASR */
  1.000000E+04F,                                                      /* MAXI */
  0,                                                                  /* MC2 */
  0.000000000000E+00,                                                 /* MEAN */
  0,                                                                  /* ME_mod */
  0.000000E+00F,                                                      /* MI */
  {                                                                   /* MULEXPNO */
	0
  },
  0,                                                                  /* MddCEXP */
  0,                                                                  /* MddCT_SP */
  0,                                                                  /* MddF180 */
  0.000000000000E+00,                                                 /* MddLAMBDA */
  0,                                                                  /* MddMEMORY */
  23,                                                                 /* MddMERGE */
  0,                                                                  /* MddNCOMP */
  0,                                                                  /* MddNITER */
  8.000000000000E-01,                                                 /* MddNOISE */
  0.000000000000E+00,                                                 /* MddPHASE */
  0,                                                                  /* MddSEED */
  0.000000000000E+00,                                                 /* MddSRSIZE */
  0,                                                                  /* Mdd_CsALG */
  1.000000000000E+00,                                                 /* Mdd_CsLAMBDA */
  0,                                                                  /* Mdd_CsNITER */
  0.000000000000E+00,                                                 /* Mdd_CsNORM */
  1,                                                                  /* Mdd_CsVE */
  0,                                                                  /* Mdd_CsZF */
  0.000000000000E+00,                                                 /* Mdd_LrLAMBDA */
  0,                                                                  /* Mdd_LrNITER */
  0.000000000000E+00,                                                 /* Mdd_LrRANK */
  0.000000000000E+00,                                                 /* Mdd_LrZF */
  0,                                                                  /* Mdd_mod */
  {                                                                   /* NAME */
   ""  },
  {                                                                   /* NAME2 */
   ""  },
  {                                                                   /* NAME3 */
   ""  },
  1,                                                                  /* NBL */
  0,                                                                  /* NC */
  0,                                                                  /* NCOEF */
  0,                                                                  /* NC_proc */
  6,                                                                  /* NLEV */
  1,                                                                  /* NLOGCH */
  0.000000E+00F,                                                      /* NOISF1 */
  0.000000E+00F,                                                      /* NOISF2 */
  0,                                                                  /* NOVFLW */
  1,                                                                  /* NS */
  1,                                                                  /* NSP */
  0,                                                                  /* NTH_PI */
  {                                                                   /* NUC1 */
   "off"  },
  {                                                                   /* NUC2 */
   "off"  },
  {                                                                   /* NUC3 */
   "off"  },
  {                                                                   /* NUC4 */
   "off"  },
  {                                                                   /* NUC5 */
   "off"  },
  {                                                                   /* NUC6 */
   "off"  },
  {                                                                   /* NUC7 */
   "off"  },
  {                                                                   /* NUC8 */
   "off"  },
  {                                                                   /* NUCLEUS */
   ""  },
  {                                                                   /* NUSLIST */
   "automatic"  },
  0,                                                                  /* NZP */
  2.500000000000E+01,                                                 /* NusAMOUNT */
  0,                                                                  /* NusFPNZ */
  0.000000000000E+00,                                                 /* NusJSP */
  54321,                                                              /* NusSEED */
  0,                                                                  /* NusSPTYPE */
  1.000000000000E+00,                                                 /* NusT2 */
  0,                                                                  /* NusTD */
  0.000000000000E+00,                                                 /* O1 */
  0.000000000000E+00,                                                 /* O2 */
  0.000000000000E+00,                                                 /* O3 */
  0.000000000000E+00,                                                 /* O4 */
  0.000000000000E+00,                                                 /* O5 */
  0.000000000000E+00,                                                 /* O6 */
  0.000000000000E+00,                                                 /* O7 */
  0.000000000000E+00,                                                 /* O8 */
  0.000000E+00F,                                                      /* OFFSET */
  0,                                                                  /* OVERFLW */
  {                                                                   /* P */
	0.000000E+00F
  },
  {                                                                   /* PACOIL */
	0
  },
  0,                                                                  /* PAPS */
  0,                                                                  /* PARMODE */
  4.000000E+00F,                                                      /* PC */
  {                                                                   /* PCPD */
	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,
	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,	1.000000E+02F,
	1.000000E+02F,	1.000000E+02F
  },
  {                                                                   /* PEXSEL */
	1,	1,	1,	1,
	1,	1,	1,	1,
	1,	1
  },
  {                                                                   /* PFORMAT */
   ""  },
  0.000000E+00F,                                                      /* PHC0 */
  0.000000E+00F,                                                      /* PHC1 */
  {                                                                   /* PHCOR */
	0.000000E+00F
  },
  {                                                                   /* PHLIST */
   ""  },
  0,                                                                  /* PH_mod */
  0.000000E+00F,                                                      /* PH_ref */
  1,                                                                  /* PKNL */
  {                                                                   /* PL */
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F
  },
  1.000000E-01F,                                                      /* PLSTEP */
  -6.000000E+00F,                                                     /* PLSTRT */
  {                                                                   /* PLW */
	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,
	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,
	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,
	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,
	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,
	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,
	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,
	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,
	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,
	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,
	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,
	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,
	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,
	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,
	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,
	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F,	-1.000000E+00F
  },
  {                                                                   /* PLWMAX */
	0.000000E+00F
  },
  0,                                                                  /* PPARMOD */
  0,                                                                  /* PPDIAG */
  0,                                                                  /* PPIPTYP */
  2147483647,                                                         /* PPMPNUM */
  1,                                                                  /* PPRESOL */
  0.000000000000E+00,                                                 /* PQPHASE */
  1.000000000000E+00,                                                 /* PQSCALE */
  1,                                                                  /* PR */
  {                                                                   /* PRECHAN */
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1
  },
  {                                                                   /* PROBHD */
   ""  },
  {                                                                   /* PROBINPUTS */
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
    {""}, {""}, {""},  },
  1,                                                                  /* PROCNO */
  2,                                                                  /* PROCNO2 */
  3,                                                                  /* PROCNO3 */
  0,                                                                  /* PSCAL */
  0,                                                                  /* PSIGN */
  {                                                                   /* PULPROG */
   "zg30"  },
  0.000000E+00F,                                                      /* PW */
  {                                                                   /* PYNM */
   "acqu.py"  },
  {                                                                   /* PYNMP */
   "proc.py"  },
  0.000000000000E+00,                                                 /* ProjAngle */
  0,                                                                  /* QNP */
  0.000000000000E+00,                                                 /* RDF1 */
  0.000000000000E+00,                                                 /* RDF2 */
  0.000000000000E+00,                                                 /* RDFWHM */
  0.000000000000E+00,                                                 /* RDINT */
  0.000000000000E+00,                                                 /* RDPOS */
  {                                                                   /* RECCHAN */
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1,	-1
  },
  0.000000E+00F,                                                      /* RECPH */
  {                                                                   /* RECPRE */
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1
  },
  {                                                                   /* RECPRFX */
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,
	-1
  },
  {                                                                   /* RECSEL */
	0
  },
  0,                                                                  /* REVERSE */
  1.000000E+00F,                                                      /* RG */
  20,                                                                 /* RO */
  {                                                                   /* RSEL */
	0
  },
  {                                                                   /* SAMPLEID */
   ""  },
  1.000000000000E+00,                                                 /* SCALE2 */
  1.000000000000E+00,                                                 /* SCALE3 */
  {                                                                   /* SELREC */
	0
  },
  5.001300000000E+02,                                                 /* SF */
  5.001332000000E+02,                                                 /* SFO1 */
  5.001332000000E+02,                                                 /* SFO2 */
  5.001300000000E+02,                                                 /* SFO3 */
  5.001300000000E+02,                                                 /* SFO4 */
  5.001300000000E+02,                                                 /* SFO5 */
  5.001300000000E+02,                                                 /* SFO6 */
  5.001300000000E+02,                                                 /* SFO7 */
  5.001300000000E+02,                                                 /* SFO8 */
  {                                                                   /* SHAPPR */
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,
	1.000000E+00F,	1.000000E+00F,	1.000000E+00F,	1.000000E+00F
  },
  8192,                                                               /* SI */
  0.000000E+00F,                                                      /* SIGF1 */
  0.000000E+00F,                                                      /* SIGF2 */
  1.600000E+01F,                                                      /* SINO */
  1024,                                                               /* SIOLD */
  {                                                                   /* SOLVENT */
   "CDCl3"  },
  {                                                                   /* SOLVOLD */
   "off"  },
  {                                                                   /* SP */
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,
	1.200000E+02F,	1.200000E+02F,	1.200000E+02F,	1.200000E+02F
  },
  0,                                                                  /* SPECTR */
  {                                                                   /* SPECTYP */
   ""  },
  0,                                                                  /* SPINCNT */
  {                                                                   /* SPNAM */
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},  },
  {                                                                   /* SPOAL */
	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,
	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,
	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,
	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,
	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,
	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,
	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,
	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,
	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,
	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,
	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,
	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,
	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,
	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,
	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,
	5.000000E-01F,	5.000000E-01F,	5.000000E-01F,	5.000000E-01F
  },
  {                                                                   /* SPOFFS */
	0.000000E+00F
  },
  {                                                                   /* SPPEX */
	0
  },
  {                                                                   /* SPW */
	0.000000E+00F
  },
  1,                                                                  /* SREF_mod */
  {                                                                   /* SREGLST */
   "1H.CDCl3"  },
  0.000000E+00F,                                                      /* SSB */
  0,                                                                  /* STSI */
  0,                                                                  /* STSR */
  {                                                                   /* SUBNAM */
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},  },
  2.000000000000E+01,                                                 /* SW */
  {                                                                   /* SWIBOX */
	0
  },
  0,                                                                  /* SW_expin */
  2.000000000000E+03,                                                 /* SW_h */
  1.000000000000E+01,                                                 /* SW_p */
  0.000000000000E+00,                                                 /* SWfinal */
  0,                                                                  /* SYMM */
  0.000000E+00F,                                                      /* S_DEV */
  0.000000000000E+00,                                                 /* SigLockShift */
  1024,                                                               /* TD */
  1,                                                                  /* TD0 */
  {                                                                   /* TD_INDIRECT */
	0
  },
  0,                                                                  /* TDav */
  0,                                                                  /* TDeff */
  0,                                                                  /* TDoff */
  3.000000E+02F,                                                      /* TE */
  3.000000E+02F,                                                      /* TE1 */
  3.000000E+02F,                                                      /* TE2 */
  3.000000E+02F,                                                      /* TE3 */
  3.000000E+02F,                                                      /* TE4 */
  3.000000E+02F,                                                      /* TEG */
  3.000000E+02F,                                                      /* TE_MAGNET */
  0,                                                                  /* TE_PIDX */
  3.000000E+02F,                                                      /* TE_RAW */
  {                                                                   /* TE_STAB */
	0
  },
  {                                                                   /* TI */
   ""  },
  0,                                                                  /* TILT */
  1.000000E-01F,                                                      /* TM1 */
  9.000000E-01F,                                                      /* TM2 */
  0.000000E+00F,                                                      /* TOPLEV */
  {                                                                   /* TOTROT */
	0.000000E+00F
  },
  {                                                                   /* TUBE_TYPE */
   ""  },
  {                                                                   /* TYPE */
   ""  },
  {                                                                   /* USER */
   ""  },
  {                                                                   /* USER2 */
   ""  },
  {                                                                   /* USER3 */
   ""  },
  {                                                                   /* USERA1 */
   "user"  },
  {                                                                   /* USERA2 */
   "user"  },
  {                                                                   /* USERA3 */
   "user"  },
  {                                                                   /* USERA4 */
   "user"  },
  {                                                                   /* USERA5 */
   "user"  },
  {                                                                   /* USERP1 */
   "user"  },
  {                                                                   /* USERP2 */
   "user"  },
  {                                                                   /* USERP3 */
   "user"  },
  {                                                                   /* USERP4 */
   "user"  },
  {                                                                   /* USERP5 */
   "user"  },
  {                                                                   /* UUID */
   ""  },
  5.000000E+00F,                                                      /* V9 */
  0,                                                                  /* VALIDCODE */
  {                                                                   /* VALIST */
   "valist"  },
  {                                                                   /* VCLIST */
   ""  },
  {                                                                   /* VDLIST */
   ""  },
  {                                                                   /* VPLIST */
   ""  },
  {                                                                   /* VTLIST */
   ""  },
  1024,                                                               /* WBST */
  {                                                                   /* WBSW */
	1.000000E+01F,	1.000000E+01F,	1.000000E+01F,	1.000000E+01F,
	1.000000E+01F,	1.000000E+01F,	1.000000E+01F,	1.000000E+01F
  },
  0,                                                                  /* WDW */
  64,                                                                 /* XDIM */
  {                                                                   /* XGAIN */
	0
  },
  0,                                                                  /* XL */
  0,                                                                  /* YL */
  0.000000000000E+00,                                                 /* YMAX_a */
  0,                                                                  /* YMAX_p */
  0.000000000000E+00,                                                 /* YMIN_a */
  0,                                                                  /* YMIN_p */
  {                                                                   /* ZGOPTNS */
   ""  },
  1.200000E+02F,                                                      /* ZL1 */
  1.200000E+02F,                                                      /* ZL2 */
  1.200000E+02F,                                                      /* ZL3 */
  1.200000E+02F,                                                      /* ZL4 */
  0,                                                                  /* scaledByNS */
  0,                                                                  /* scaledByRG */

  9999   /* ZZ00 */    
  
};
