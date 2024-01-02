/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2009
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

/* ************************************************************
 * redirection of global parameter groups
 * ***********************************************************/

/* react on changes of system configuration                  */
relations PVM_SysConfigHandler backbone;

/* geometry */
relations PVM_ImageGeometryHandler  backbone;
/* reference position */
relations PVM_AtsRefPosHandler        backbone;




/* nuclei */

relations PVM_NucleiHandler    backbone;


/* ************************************************************
 *
 * relation redirection of single global parameters used in
 * this method:
 *
 * ***********************************************************/

relations PVM_RepetitionTime          RepetitionTimeRel;
relations PVM_NAverages               AveragesRel;
relations PVM_EchoTime                EchoTimeRel;
relations PVM_DeriveGains             backbone;
relations PVM_EffSWh                  EffSWhRel;
relations PVM_TriggerHandler          backbone;
relations PVM_FovSatHandler           backbone;
relations PVM_FatSupHandler           backbone;
relations PVM_SatTransHandler         backbone;
relations PVM_DummyScansHandler       backbone;
relations PVM_SegmentationHandler     backbone;
relations PVM_DriftCompHandler        backbone;

/*
 * parameters that are used but not shown in editor
 * only the method may change these parameters, they
 * are redirected to the backbone routine.
 */

relations PVM_NRepetitions            backbone;
relations PVM_MinRepetitionTime       backbone;

/* relation redirection of method specific reconstruction */
relations RecoUserUpdate              RecoDerive;

/* Encoding */
relations PVM_EncodingHandler         backbone;

/* Redirect relation for visu creation */
relations VisuDerivePars              deriveVisu;

/* Adjustment related redirections */
relations PVM_TrajHandler             backbone;
relations PVM_TrajRecoHandler         SetRecoParam;
relations PVM_AdjHandler              STB_TrajHandleAdjustmentRequests;
relations PVM_AdjResultHandler        ATB_TrajHandleAdjustmentResults;

/* relations for mapshim parameter group*/
relations PVM_MapShimHandler          backbone;

