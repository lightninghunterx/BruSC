/*
 *******************************************************************
 *
 * $Source: /pr/CvsTree/pr/gen/h/frozen/erropt.h,v $
 *
 * Copyright (c) 1995
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: erropt.h,v 1.6 2012/11/29 06:28:44 wem Exp $
 *
 *******************************************************************
*/


#define ERROPT_ACKN		07
#define	ERROPT_AK_NO	0
#define	ERROPT_AK_OK	1
#define	ERROPT_AK_ABT	2
#define ERROPT_AK_CAN	3
#define ERROPT_AK_DEF	4
#define ERROPT_AK_PAS	5

#define ERROPT_ICON		070
#define	ERROPT_IC_NO	0
#define ERROPT_IC_QST	010
#define	ERROPT_IC_EXK	020
#define	ERROPT_IC_AST	030

#define	ERROPT_BUTTON		0700
#define	ERROPT_BT_NO	0
#define	ERROPT_BT_1	0100
#define	ERROPT_BT_2	0200
#define	ERROPT_BT_3	0300

#define	ERROPT_BEEP		07000
#define	ERROPT_BEEP_NO	0
#define	ERROPT_BEEP_YES	01000
#define	ERROPT_BEEP_DEF	02000

		 /* Define some usefull combinations of error options */

	 /* Error message with acknowledge (possibly disabled for AU) */
#define ERROR_OPT	(ERROPT_AK_DEF  | \
			 ERROPT_IC_EXK  | \
			 ERROPT_BT_1    | \
			 ERROPT_BEEP_DEF)

#define DEF_ERR_OPT	 ERROR_OPT
				    /* Error message with acknowledge */
#define ERRORH_OPT	(ERROPT_AK_OK   | \
			 ERROPT_IC_EXK  | \
			 ERROPT_BT_1    | \
			 ERROPT_BEEP_YES)
				/* Question with two possible answers */
#define QUESTION_OPT	(ERROPT_AK_CAN  | \
			 ERROPT_IC_QST  | \
			 ERROPT_BT_1    | \
			 ERROPT_BEEP_NO )

#define QUESTION_OPT_PAS (ERROPT_AK_PAS  | \
			  ERROPT_IC_EXK  | \
			  ERROPT_BT_1    | \
			  ERROPT_BEEP_DEF)
			/* Action report with three posssible answers */
#define ACTION_OPT	(ERROPT_AK_ABT  | \
			 ERROPT_IC_AST  | \
			 ERROPT_BT_NO   | \
			 ERROPT_BEEP_NO )
	      /* Information / Progress / Warning without acknowledge */
#define INFO_OPT	(ERROPT_AK_NO   | \
			 ERROPT_IC_AST  | \
			 ERROPT_BT_NO   | \
			 ERROPT_BEEP_NO )


/* the return values of the Proc_err routine follow */
#define		ERR_ERR	       -1 /* error processing error */
#define		ERR_OK		0 /* ok, execution can continue */
#define		ERR_CANCEL	1 /* cancel erroneous operation */
#define		ERR_IGNORE	2 /* ignore erroneous operation */
#define		ERR_RETRY	3 /* retry erroneous operation */
#define		ERR_ABT		4 /* abort erroneous operation */
