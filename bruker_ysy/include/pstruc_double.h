/***************************************************************

   Parameter Structure
   NAME OF FILE:	pstruc_double.h
   NAME OF STRUCTURE:	double_pars
   Class code (hex):	1FF
   No. of parameters:	3
   Created by 'defpar'  from
   * $Id: pdef_double.txt,v 1.3 2009/04/30 13:50:55 wem Exp $
   of 'pdef_double.txt'
   2020-05-25 17:54:36.387 +0200 run by jenkins

***************************************************************/

#pragma once

/*** parameter block structure ***/

struct	double_pars
{
	PNAM	A000_p1;	/* parameter block header */
	PNAM	DOUBLEVAL_p1;
	PNAM	zz00_p1;		/* end of first table par */

/* start of second table */

	int	A000;
	double	DOUBLEVAL;
	int	zz00;		/* end of 2nd table	*/
};

