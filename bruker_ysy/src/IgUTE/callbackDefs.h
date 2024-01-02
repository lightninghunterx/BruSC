/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2013
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

/* react on changes of system configuration */
relations PVM_SysConfigHandler backbone;

/* geometry */
relations PVM_ImageGeometryHandler  backbone;
relations PVM_AtsRefPosHandler backbone;
/* nuclei */

relations PVM_NucleiHandler    backbone;


/* ************************************************************
 *
 * relation redirection of single global parameters used in
 * this method:
 *
 * ***********************************************************/

relations PVM_RepetitionTime          RepetitionTimeRel;
relations PVM_EchoTime                backbone;
relations PVM_DeriveGains             backbone;
relations PVM_EffSWh                  EffSWhRel;
relations PVM_FovSatHandler           backbone;
relations PVM_FatSupHandler           backbone;
relations PVM_SatTransHandler         backbone;
relations PVM_NMovieFrames            backbone;
relations PVM_DummyScansHandler       backbone;
relations PVM_NavHandler              backbone;
relations PVM_DriftCompHandler        backbone;
relations PVM_TriggerHandler          backbone;

/*
 * parameters that are used but not shown in editor
 * only the method may change these parameters, they
 * are redirected to the backbone routine.
 */

relations PVM_NRepetitions            backbone;
relations PVM_MinRepetitionTime       backbone;
relations PVM_NEchoImages             backbone;
relations PVM_NAverages               backbone;
relations PVM_AutoRgInitHandler       MyRgInitRel;

/* relation redirection of method specific reconstruction */
relations RecoUserUpdate        RecoDerive;

/* Encoding */
relations PVM_EncodingHandler backbone;

/* Redirect relation for visu creation */
relations VisuDerivePars        deriveVisu;

/* Adjustment related redirections */
relations PVM_TrajHandler             backbone;
relations PVM_TrajRecoHandler         SetRecoParam;
relations PVM_AdjHandler              HandleAdjustmentRequests;
relations PVM_AdjResultHandler        ATB_TrajHandleAdjustmentResults;

/* relations for mapshim parameter group*/
relations PVM_MapShimHandler backbone;

/* changes before acquisition */
relations PVM_AcqScanHandler   SetBeforeAcquisition; 

relations RecoCS               SetRecoParam;