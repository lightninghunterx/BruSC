/***************************************************************

   Parameter Structure Initialization
   NAME OF FILE:	pinit_int.h
   Class code (hex):	1FF
   No. of parameters:	3
   Created by 'defpar'  from
   * $Id: pdef_int.txt,v 1.1 2005/05/19 10:23:45 wem Exp $
   of 'pdef_int.txt'
   2020-05-25 17:54:36.412 +0200 run by jenkins

***************************************************************/
{

	/* initialization of first table */

  {"A000",             4,   0x10000, 0, PARUNIT_no        , offsetof(struct int_pars, A000)},
  {"INTVAL",                   4,   0x18804, 0, PARUNIT_no        , offsetof(struct int_pars, INTVAL)},
  {"zz00",             4,   0x40000, 0, PARUNIT_no        , offsetof(struct int_pars, zz00)},

	/* initialization of second table */


        3,                          /* A000 */
  0,                                                                  /* INTVAL */

  9999   /* ZZ00 */    
  
};
