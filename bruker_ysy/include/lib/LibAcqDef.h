/*
 *******************************************************************
 *
 * $Source: /sc/CvsTree/sc/gen/h/lib/LibAcqDef.h,v $
 *
 * Copyright (c) 1998
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 * $Id: LibAcqDef.h,v 1.2 2015/02/04 14:17:10 nipo Exp $
 *
 *******************************************************************
 */

#ifndef LIB_ACQ_DEF_H
#define LIB_ACQ_DEF_H

/*** types and enums ***/

/*** known types of transmitter switchboxes ***/
typedef enum
{
    AcqSwiBoxError = -1,
    AcqSwiBoxNone = 0,		/* no switchbox */
    AcqSwiBox3in3out,		/* standard DMX transmitter */
    AcqSwiBox2in3out,		/* standard DRX transmitter */
    AcqSwiBox1in2out,		/* only 1 pin diode switch */
    AcqSwiBox2in2out,		/* only 1 pin diode switch */
    AcqSwiboxUnknown		/* invalid swibox entry found */
} AcqSwiBox;


/*** Types of receive configurations ***/
typedef enum
{
    RecConfError = -1, /* Unknown or illegal configuration */
    SingleReceiver,    /* Single receiver ;-)              */
    QuadReceiver,      /* Two receivers acting in parallel */
    MultiReceiver,     /* Separate receive channels        */
    ParallelReceiver   /* DRU system with Rf-Splitter      */
} AcqReceiveConf;

#endif /* LIB_ACQ_DEF_H */
/***********************************************************************/
/*			     E N D   O F   F I L E                     */
/***********************************************************************/
