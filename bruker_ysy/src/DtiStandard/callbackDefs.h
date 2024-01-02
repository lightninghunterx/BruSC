/****************************************************************
 *
 * $Source$
 * Copyright (c) 1999-2010
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

/* react on changes of system configuration */
relations PVM_SysConfigHandler backbone;

/* Encoding */
relations PVM_EncodingHandler backbone;

/* geometry */
relations PVM_ImageGeometryHandler  backbone;
relations PVM_AtsRefPosHandler backbone;

/*bandwidth handling */
relations PVM_EffSWh                  EffSWhRel;

/* modules */
relations PVM_DiffusionHandler        backbone;
relations PVM_FatSupHandler           backbone;
relations PVM_FovSatHandler           backbone;
relations PVM_TriggerHandler          backbone;
relations PVM_MotionSupOnOff          backbone;
relations PVM_DummyScansHandler       backbone;

relations PVM_RepetitionTime          RepetitionTimeRel;
relations PVM_NAverages               AveragesRel;
relations PVM_NRepetitions            repetitionsRel;
relations PVM_EchoTime                EchoTimeRel;
relations PVM_NucleiHandler           backbone;
relations PVM_DeriveGains             backbone;



/*
 * parameters that are used but not shown in editor
 * only the method may change these parameters, they
 * are redirected to the backbone routine.
 */

relations PVM_MinRepetitionTime       backbone;
relations PVM_NEchoImages             backbone;

/*
 * Redirect relation for visu creation
 */
relations VisuDerivePars             deriveVisu;
/* react on parameter adjustments */
relations PVM_AdjResultHandler       backbone;
relations PVM_AdjHandler             handleRgAdjustment;


/* relations for mapshim parameter group*/
relations PVM_MapShimHandler         backbone;

/* redirection of method specific reconstruction */
relations RecoUserUpdate             RecoDerive;
/****************************************************************/
/*		E N D   O F   F I L E				*/
/****************************************************************/
