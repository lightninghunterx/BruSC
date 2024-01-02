/*
 *******************************************************************
 *
 * $Source: /pr/CvsTree/pr/gen/h/frozen/brukdef.h,v $
 *
 * Copyright (c) 1995
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: brukdef.h,v 1.9 2009/02/16 09:14:45 wem Exp $
 *
 *******************************************************************
*/


#ifndef BRUKDEF_H

#define BRUKDEF_H 1			/* avoid multiple includes */


/*---------------------------------------------------------------------*
 * First a set of useful typedef's.
 *---------------------------------------------------------------------*/
typedef unsigned long	BOOLEAN_32;
typedef unsigned short	BOOLEAN_16;
typedef unsigned char	BOOLEAN_8;
typedef volatile int	Vint;
typedef volatile short	Vshort;


/*---------------------------------------------------------------------*
 * Next a set of useful constants
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 * if BYTESTORAGE_ORDER is 0 processed data must be stored little endian
 * if BYTESTORAGE_ORDER is 1 processed data must be stored natural order
 *--------------------------------------------------------------------*/
#define BYTESTORAGE_ORDER	1

#define	MAX_UNSIGNED_CHAR	((unsigned char)(~0))
#define MAX_UNSIGNED_SHORT	((unsigned short)(~0))
#define MAX_UNSIGNED_INT	((unsigned int)(~0))

#define MAX_CHAR		0x7f
#define MIN_CHAR		0x81
#define MAX_SHORT		0x7fff
#define MIN_SHORT		0x8001
#define MAX_INT			0x7fffffff
#define MIN_INT			0x80000001

#ifndef TRUE
#define TRUE			1
#define	FALSE			0
#endif /* TRUE */

#define ON			1
#define OFF			0

#define NULL_PTR		0

/*---------------------------------------------------------------------*
 * Now a collection of useful macro's
 *---------------------------------------------------------------------*/
#define MAX_OF(x,y)		( ( (x) > (y) ) ? (x) : (y) )
#define MIN_OF(x,y)		( ( (x) < (y) ) ? (x) : (y) )
#define KRND(x)			( ( (x) + 0x3ff ) & ( ~0x3ff ) )
#define ROUND(x,b)		( ((x) + (b) - 1) - ((x) + (b) - 1)%(b) )
#define ARRAY_LEN(a)		( sizeof(a)/sizeof((a)[0]) )

/*---------------------------------------------------------------------*
 * Now a collection of macro's for pointer alignment
 *---------------------------------------------------------------------*/
#define PNTR_ALIGN_TO(x, n)	((x) + ((n-1) - (((size_t)(x) + (n-1)) & (n-1))))
#define CONST_ALIGN_TO(x, n)	PNTR_ALIGN_TO((const char*)x, n)
#define ALIGN_TO(x, n)		PNTR_ALIGN_TO((char*)x, n)
#define ALIGN_TO_8(x)		ALIGN_TO((x), 8)
#define ALIGN_TO_4(x)		ALIGN_TO((x), 4)

/*---------------------------------------------------------------------*
 * macro's from bruk.h
 *--------------------------------------------------------------------*/

#ifndef K1MIN
#define K1MIN		0x3ff
					    /* round to page boundary */
#define PGROUND(X)  ((((size_t)(X))+K1MIN)&~(size_t)K1MIN)
#endif

#endif	/* BRUKDEF_H */

/**********************************************************************/
/*	E N D   O F   F I L E					      */
/**********************************************************************/
