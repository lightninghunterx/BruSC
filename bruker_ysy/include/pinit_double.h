/***************************************************************

   Parameter Structure Initialization
   NAME OF FILE:	pinit_double.h
   Class code (hex):	1FF
   No. of parameters:	3
   Created by 'defpar'  from
   * $Id: pdef_double.txt,v 1.3 2009/04/30 13:50:55 wem Exp $
   of 'pdef_double.txt'
   2020-05-25 17:54:36.388 +0200 run by jenkins

***************************************************************/
{

	/* initialization of first table */

  {"A000",             4,   0x10000, 0, PARUNIT_no        , offsetof(struct double_pars, A000)},
  {"DOUBLEVAL",                8,   0x40002, 0, PARUNIT_no        , offsetof(struct double_pars, DOUBLEVAL)},
  {"zz00",             4,   0x40000, 0, PARUNIT_no        , offsetof(struct double_pars, zz00)},

	/* initialization of second table */


        3,                          /* A000 */
  0.000000000000E+00,                                                 /* DOUBLEVAL */

  9999   /* ZZ00 */    
  
};
