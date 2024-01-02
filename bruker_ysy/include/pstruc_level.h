/***************************************************************

   Parameter Structure
   NAME OF FILE:	pstruc_level.h
   NAME OF STRUCTURE:	level_pars
   Class code (hex):	1FF
   No. of parameters:	10
   Created by 'defpar'  from
   * $Id: pdef_level.txt,v 1.4 2009/04/02 15:59:36 wem Exp $
   of 'pdef_level.txt'
   2020-05-25 17:54:36.417 +0200 run by jenkins

***************************************************************/

#pragma once

/*** parameter block structure ***/

struct	level_pars
{
	PNAM	A000_p1;	/* parameter block header */
	PNAM	LEVELS_p1;
	PNAM	LEVSIGN_p1;
	PNAM	MAXLEV_p1;
	PNAM	METHOD_p1;
	PNAM	NEGBASE_p1;
	PNAM	NEGINCR_p1;
	PNAM	POSBASE_p1;
	PNAM	POSINCR_p1;
	PNAM	zz00_p1;		/* end of first table par */

/* start of second table */

	int	A000;
	double	LEVELS[256];
	int	LEVSIGN;
	int	MAXLEV;
	int	METHOD;
	double	NEGBASE;
	double	NEGINCR;
	double	POSBASE;
	double	POSINCR;
	int	zz00;		/* end of 2nd table	*/
};

