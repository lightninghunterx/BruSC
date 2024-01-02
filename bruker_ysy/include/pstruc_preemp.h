/***************************************************************

   Parameter Structure
   NAME OF FILE:	pstruc_preemp.h
   NAME OF STRUCTURE:	preemp_pars
   Class code (hex):	1FF
   No. of parameters:	17
   Created by 'defpar'  from
   # $Id: pdef_preemp.txt,v 1.21 2017/03/30 06:44:41 mah Exp $
   of 'pdef_preemp.txt'
   2020-05-25 17:54:36.156 +0200 run by jenkins

***************************************************************/

#pragma once

/*** parameter block structure ***/

struct	preemp_pars
{
	PNAM	A000_p1;	/* parameter block header */
	PNAM	ActualCurrent_p1;
	PNAM	B0CC_p1;
	PNAM	COILDEL_p1;
	PNAM	CoarseOffset_p1;
	PNAM	CompDel_p1;
	PNAM	DPPFAC_p1;
	PNAM	GRADCC_p1;
	PNAM	PreempChanIn_p1;
	PNAM	PreempChanOut_p1;
	PNAM	PreempFilters_p1;
	PNAM	PreempMode_p1;
	PNAM	PreempOrder_p1;
	PNAM	RAMPTM_p1;
	PNAM	RISETM_p1;
	PNAM	ScalingFactor_p1;
	PNAM	zz00_p1;		/* end of first table par */

/* start of second table */

	int	A000;
	double	ActualCurrent[10];
	double	B0CC;
	double	COILDEL;
	double	CoarseOffset[10];
	double	CompDel[10];
	double	DPPFAC;
	double	GRADCC;
	int	PreempChanIn[38];
	int	PreempChanOut[38];
	double	PreempFilters[532];
	int	PreempMode;
	int	PreempOrder[38];
	double	RAMPTM;
	double	RISETM;
	double	ScalingFactor[10];
	int	zz00;		/* end of 2nd table	*/
};

