/***************************************************************

   Parameter Structure
   NAME OF FILE:	pstruc_int.h
   NAME OF STRUCTURE:	int_pars
   Class code (hex):	1FF
   No. of parameters:	3
   Created by 'defpar'  from
   * $Id: pdef_int.txt,v 1.1 2005/05/19 10:23:45 wem Exp $
   of 'pdef_int.txt'
   2020-05-25 17:54:36.412 +0200 run by jenkins

***************************************************************/

#pragma once

/*** parameter block structure ***/

struct	int_pars
{
	PNAM	A000_p1;	/* parameter block header */
	PNAM	INTVAL_p1;
	PNAM	zz00_p1;		/* end of first table par */

/* start of second table */

	int	A000;
	int	INTVAL;
	int	zz00;		/* end of 2nd table	*/
};

