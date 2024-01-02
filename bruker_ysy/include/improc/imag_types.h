/*
*******************************************************************
*
* $Source$
* 
* Copyright (c) 1988-2011
* Bruker BioSpin MRI GmbH
* D-76275 Ettlingen, Germany
* 
* All Rights Reserved 
* 
* $Id$
*
*******************************************************************
*/
#ifndef IMPROC_IMAG_TYPES_H		/*** prevent multiple includes*/
#define IMPROC_IMAG_TYPES_H

/*
***  define interface to image_proc routines
*/

#include <stdint.h>
#include "generated/DataPath.h"

/* image coordinate system (right handed): 			     */
/* suffix x:  along the rows or horizontal, column number	     */
/* suffix y:  along the columns or vertical, row number              */
/* suffix z:  through a set of 2D slices orthogonal to the x and y   */
/*            directions, slice number				     */

struct imag_def
	{
	int six;		/* image x dimension (image row size) */
	int siy;		/* image y dim. (image column size)   */
	int siz;		/* image z dimension (# of 2D slices) */
	int smx;		/* submatrix x dim. (sm row size)     */
	int smy;		/* submatrix y dim. (sm column size)  */
	int smz;		/* submatrix z dim. (# of sm slices)  */
	char *addr;		/* reference address for in-memory    */
				/*   images:		      	      */
				/* = shared memory start address for  */
				/*     images in shared memories      */
				/* = else arbitrary absolute memory   */
				/*     address			      */
				/* without meaning for on-disk and    */
				/*     on-screen images		      */
	int64_t offs;	/* offset of image data with respect  */
				/*   -- to addr for in-memory images  */
				/*   -- to file beginning for on-disk */
				/*	  images		      */
				/*   -- to frame buffer start for     */
				/*	  on_screen images	      */
	char file[de_bruker_mri_generated_datapath_MaxProcnoCStrLen + 100]; 
        /* image file name if on disk */
	int loc;		/* image storage location             */
	int shmid;		/* image shared memory identifier     */
	int dtype;		/* image data type                    */
	};

struct subimag_def
	{
	int six;		/* subimage x dimension ( si row size)*/
	int siy;		/* subimage y dim. (si column size)   */
	int siz;		/* subimage z dim. (# of si 2D slices)*/
	int x0;			/* x-origin of subimage               */
	int y0;			/* y-origin of subimage               */
	int z0;			/* z-origin of subimage               */
	};

struct map_tab			/* table for 32-to-8 bit mapping      */
	{
	int u_limit;		/* upper limit of interval            */
	int out_val;		/* 8 bit output value to be assigned  */
	};			/* to input values in corr. interval  */

struct imag_scale		/* image scaling parameters           */
	{
	int lcut;		/* lower cutoff value                 */
	int ucut;		/* upper cutoff value                 */
	int floor;		/* set pixel to floor if below lcut   */
	int ceil;		/* set pixel to ceil if above ucut    */
	int offs;		/* linear scaling offset              */
	float fac;		/* linear scaling factor              */
	char *lut;		/* pointer to scaling lut (8-to-8 bit)*/
	int n_interv;		/* # of intervals for 32-to-8 bit     */
				/*	mapping			      */
	struct map_tab *mio;	/* pointer to interval/output value   */
				/* table for 32-to-8 bit mapping      */
	};

struct imag_filter		/* image filtering parameters         */
	{
	int six;		/* filter x dim. (filter row size)    */
	int siy;		/* filter y dim. (filter column size) */
	int siz;		/* filter z dim. (# of filter slices) */
	int *mtx;		/* pointer to filter matrix           */
	};

struct im_proc
	{
	struct imag_def i;	/* input image definition             */
	struct subimag_def is;	/* input subimage definition          */
	struct imag_def o;	/* output image definition            */
	struct subimag_def os;	/* output subimage definition         */
	struct imag_def a;	/* def. of auxiliary (input) image    */
	struct subimag_def as;	/* def. of auxiliary (input) subimage */
	struct imag_def e;	/* definition of extra (output) image */
	struct subimag_def es;	/* def. of extra (output) subimage    */
	struct imag_scale sc;	/* image scaling parameters           */
	struct imag_filter flt;	/* image filtering parameters         */
	};
	
/* values of the "loc" parameter */
#define IP_MEM		0	/* image in main memory               */
#define IP_SCR		1	/* image on screen (in frame buffer)  */
#define IP_DSK		2	/* image on disk                      */
#define IP_NONE		-1	/* no image defined                   */

/* image_proc commands */
#define IP_NOCHECK	0x1	/* skip input parameter check         */
#define IP_NOSELECT	0x2	/* skip processing routine selection  */
#define IP_FASTLD	0x4	/* fast image loading, 16-byte addr.  */
#define IP_DFLT_SCR	0x8	/* set default screen parameters      */
#define IP_LIN_SC	0x10	/* linear 8-to-8 bit scaling is->os   */
#define IP_LUT_SC	0x20	/* look-up table based 8-to-8 bit     */
				/*	scaling is->os		      */
#define IP_MAP_SC	0x40	/* interval table based 32-to-8 bit   */
				/*	scaling is->os		      */
#define IP_FILTER	0x100	/* apply linear filter to os	      */
#define IP_CPMIN	0x1000	/* compress image, mininimum search   */
#define IP_CPMAX	0x2000	/* compress image, maximum search     */
#define IP_CPMINMAX	0x3000	/* compress image, min & max search   */
#define IP_CPSKIP	0x4000  /* compr.image by skipping row & cols */
#define IP_ADD		0x10000	/* add input images: is + as -> os    */
#define IP_SUB		0x20000	/* subtract input images: is-as -> os */
#define IP_MUL		0x40000	/* multiply input images: is*as -> os */
#define IP_DIV		0x80000	/* divide input images: is/as -> os   */
#define IP_NEG		0x10000	/* compl. of input image: !is -> os   */
#define IP_AND		0x200000 /* AND input images: is & as -> os   */
#define IP_OR		0x400000 /* OR input images: is | as -> os    */
#define IP_XOR		0x800000 /* XOR input images: is ^ as -> os   */

/* image_proc data types */
#define IP_BIT		  1	/* 1 bit per pixel		*/
#define IP_BYTE		  8	/* 8 bit per pixel, signed 	*/
#define IP_U_BYTE	 -8	/* 8 bit per pixel, unsigned 	*/
#define IP_SHORT	 16	/* 16 bit per pixel, signed     */
#define IP_U_SHORT	-16	/* 16 bit per pixel, unsigned	*/
#define IP_3BYTE	 24	/* 24 bit per pixel, signed     */
#define IP_U_3BYTE	-24	/* 24 bit per pixel, unsigned   */
#define IP_INT		 32	/* 32 bit per pixel, signed	*/
#define IP_U_INT	-32	/* 32 bit per pixel, unsigned	*/
#define IP_SHORT_XY	 35	/* pair of signed 16 bit ints   */
#define IP_FLOAT	 37
#define IP_DOUBLE	 64


#endif /* IMPROC_IMAG_TYPES_H */
