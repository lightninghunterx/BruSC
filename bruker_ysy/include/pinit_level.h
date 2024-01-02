/***************************************************************

   Parameter Structure Initialization
   NAME OF FILE:	pinit_level.h
   Class code (hex):	1FF
   No. of parameters:	10
   Created by 'defpar'  from
   * $Id: pdef_level.txt,v 1.4 2009/04/02 15:59:36 wem Exp $
   of 'pdef_level.txt'
   2020-05-25 17:54:36.417 +0200 run by jenkins

***************************************************************/
{

	/* initialization of first table */

  {"A000",             4,   0x10000, 0, PARUNIT_no        , offsetof(struct level_pars, A000)},
  {"LEVELS",                2048,  0x140010, 0, PARUNIT_no        , offsetof(struct level_pars, LEVELS)},
  {"LEVSIGN",                  4,   0x10010, 0, PARUNIT_no        , offsetof(struct level_pars, LEVSIGN)},
  {"MAXLEV",                   4,   0x10010, 0, PARUNIT_no        , offsetof(struct level_pars, MAXLEV)},
  {"METHOD",                   4,   0x10010, 0, PARUNIT_no        , offsetof(struct level_pars, METHOD)},
  {"NEGBASE",                  8,   0x40010, 0, PARUNIT_no        , offsetof(struct level_pars, NEGBASE)},
  {"NEGINCR",                  8,   0x40010, 0, PARUNIT_no        , offsetof(struct level_pars, NEGINCR)},
  {"POSBASE",                  8,   0x40010, 0, PARUNIT_no        , offsetof(struct level_pars, POSBASE)},
  {"POSINCR",                  8,   0x40010, 0, PARUNIT_no        , offsetof(struct level_pars, POSINCR)},
  {"zz00",             4,   0x40000, 0, PARUNIT_no        , offsetof(struct level_pars, zz00)},

	/* initialization of second table */


       10,                          /* A000 */
  {                                                                   /* LEVELS */
  0.000000000000E+00
  },
  3,                                                                  /* LEVSIGN */
  1,                                                                  /* MAXLEV */
  0,                                                                  /* METHOD */
  0.000000000000E+00,                                                 /* NEGBASE */
  0.000000000000E+00,                                                 /* NEGINCR */
  0.000000000000E+00,                                                 /* POSBASE */
  0.000000000000E+00,                                                 /* POSINCR */

  9999   /* ZZ00 */    
  
};
