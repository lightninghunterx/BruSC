/*
 *******************************************************************
 *
 * $Source: /pr/CvsTree/pr/gen/h/frozen/pstruc.h,v $
 *
 * Copyright (c) 1995
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: pstruc.h,v 1.25 2018/07/20 12:01:12 Marina.Maurer Exp $
 *
 *******************************************************************
*/
#ifndef PSTRUC_H		/* prevent multiple includes */
#define PSTRUC_H

#include <stddef.h>

# define XPAR   16	/* max. # of chars. -1 for a param name	*/
# define PSTRUC_MAX    sizeof(double) /* max size of a param. in table 1 */
# define PSTRUC_INT    PSTRUC_MAX-sizeof(int)
# define PSTRUC_FLOAT  PSTRUC_MAX-sizeof(float)
# define PSTRUC_CHAR   PSTRUC_MAX-sizeof(char)
# define PSTRUC_SHORT  PSTRUC_MAX-sizeof(short)
# define PSTRUC_DOUBLE PSTRUC_MAX-sizeof(double)
# define EXPSIZ 128	/* size of explanatory text */
# define MAXREAL32  1.0E38
# define MAXREAL64  1.0E308

/*** bit assignment in "parclass", struc PNAM ***/

				/* parameter classes, bits 0..7 */
# define PSTRUC_SETUP	1
# define PSTRUC_ACQU	2
# define PSTRUC_PROC	4
# define PSTRUC_PLOT	8
# define PSTRUC_DISPL	0X10
# define PSTRUC_CURD	0X20
# define PSTRUC_OUTD	0X40
# define PSTRUC_MISC	0X80
# define PSTRUC_SHAPE	0X100
# define PSTRUC_NOCLASS	~(PSTRUC_SETUP|PSTRUC_ACQU|PSTRUC_PROC|PSTRUC_PLOT|PSTRUC_DISPL|PSTRUC_CURD|PSTRUC_OUTD|PSTRUC_MISC|PSTRUC_SHAPE)
				/* unused bits 8..11		*/
# define NONEDIT    0x1000	/* bit 12 = 1: not editable	*/
# define EDTSELECT  0X2000	/* bit 13 =1 : selectable item	*/
# define RELATED    0X4000	/* bit 14 : 1			*/
# define SUBRANGE   0X8000	/* bit 15 : 1			*/
				/* bit no	16 17 18 19 20	*/
# define INTEGER32  0X10000	/*		 1  0  0  0  0	*/
# define INTEGER64  0X20000	/*		 0  1  0  0  0	*/
# define REAL32     0X30000	/*		 1  1  0  0  0	*/
# define REAL64     0X40000	/*		 0  0  1  0  0	*/
# define PSTRUC_ENUM       0X50000	/*	 1  0  1  0  0	*/
# define PSTRUC_BOOLEAN    0X60000	/*	 0  1  1  0  0	*/
# define COUNTER    0X170000	/*		 1  1  1  0  1	*/
# define CHAR8      0X80000	/*		 0  0  0  1  0	*/
# define TYPEMASK   ~(REAL32|REAL64|CHAR8) /* 16 ..19:0		*/
# define ARRAYT     0X100000	/* bit 20 : 1			*/
# define MODIFIED   0x200000	/* bit 21 : 1			*/
# define EXTFUNCT   0x400000	/* bit 22 : 1			*/
/* this describes the use of the type field in PNAM
   we use bits 23-27 to hold the type id in parclass.		*/
# define PNAM_TYPE_SHIFT       23
# define PNAM_TYPE_CLASS_MASK  0x0f800000
# define MODIFIED_VAL 0x10000000/* bit 28 : value modified      */
# define PAR_ARRAY_TEST(PAR) ((PAR & ARRAYT) || ((PAR & ~TYPEMASK) == CHAR8))

/*** par. range, relation and consistency struct ***/

typedef struct pdeftyp
{
    char    expl[EXPSIZ];	/* explanatory text to parameter*/
    union
    {
	int     i[6];		/* low -, high -, default value */
	float   f[6];		/* low -, high -, default value */
	double  d[3];		/* low -, high -, default value */
	char	c[24];		/* low -, high -, default value */
	int     co[6];		/* default initial, final step	*/
	int     bd[6];		/* default boolean value	*/
	struct
	{			/* enumeration types		*/
	    int   e[4];		/* length, ordinal of high, default*/
	    char* en;		/* pointer to enumeration names */
	} enu;
    } range;
    int     high;		/* highest index, 0 if not an array */
    const char* relptr;		/* pointer to string containing	*/
				/* relations and consistency conditions */
} PDEF, *PDEFPTR;


/*** parameter name & type struct ***/
enum PARUNIT
{
	PARUNIT_no,		/* number without unit = 0 = default */
	PARUNIT_sec,		/* 1 */
	PARUNIT_msec,		/* 2 */
	PARUNIT_usec,		/* 3 */
	PARUNIT_Hz,		/* 4 */
	PARUNIT_kHz,		/* 5 */
	PARUNIT_MHz,		/* 6 */
	PARUNIT_mHz,		/* 7 */
	PARUNIT_ppm,		/* 8 */
	PARUNIT_dB,		/* 9 */
	PARUNIT_percent,	/* 10 */
	PARUNIT_Kelvin,		/* 11 */
	PARUNIT_cminv,		/* 12 cm-1 */
	PARUNIT_degree,		/* 13 */
	PARUNIT_cm,		/* 14 */
	PARUNIT_mm,		/* 15 */
	PARUNIT_um,		/* 16 */
	PARUNIT_W,		/* 17 */
	PARUNIT_lperh,		/* 18 */
	PARUNIT_GpercmA,	/* 19 */
	PARUNIT_ulpermin,	/* 20 */
	PARUNIT_Ampere,		/* 21 */
	PARUNIT_bar,     	/* 22 */
	PARUNIT_degCelsius,     /* 23 */
	PARUNIT_end             /* number of elements and must always be the last element in the enum */
};

typedef struct pnamtyp
{
	char    parnam[XPAR];	/* name of parameter		*/
	int	parsiz;		/* size of parameter in bytes   */
	int	parclass;	/* parameter class		*/
	PDEFPTR def;		/* pointer to PDEF structure	*/
	enum	PARUNIT parunit;/* parameter unit		*/
        ptrdiff_t	paroffs;/* offset to 2nd table		*/
} PNAM, *PNAMPTR;

#endif /* PSTRUC_H */
