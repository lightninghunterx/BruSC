/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2007
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
relations PVM_SysConfigHandler        backbone;

/* geometry */
relations PVM_ImageGeometryHandler    backbone;
relations PVM_AtsRefPosHandler backbone;

/* nuclei */
relations PVM_NucleiHandler           backbone;

/* encoding parameter group */
relations PVM_EncodingHandler         backbone;

/* modules */
relations PVM_FatSupHandler           backbone;
relations PVM_FovSatHandler           backbone;
relations PVM_TriggerHandler          backbone;
relations PVM_TriggerOutHandler       backbone;
relations PVM_DiffusionHandler        backbone;
relations PVM_TaggingHandler          backbone;
relations PVM_DummyScansHandler       backbone;

/* ************************************************************
 * relation redirection of single global parameters used in
 * this method:
 * ***********************************************************/

relations PVM_RepetitionTime          backbone;
relations PVM_NAverages               NAveragesRels;
relations PVM_NRepetitions            backbone;
relations PVM_EchoTime                backbone;
relations PVM_MinEchoTime             backbone;

relations PVM_AcquisitionTime         backbone;
relations PVM_DeriveGains             backbone;
relations PVM_EffSWh                  EffSWhRel;

/* ************************************************************
 * parameters that are used but not shown in editor
 * only the method may change these parameters, they
 * are redirected to the backbone routine.
 * ************************************************************/

relations PVM_NRepetitions            backbone;
relations PVM_MinRepetitionTime       backbone;

/* spiral group */
relations PVM_SpiralHandler           backbone;
relations PVM_TrajHandler             backbone;
relations PVM_TrajRecoHandler         SetRecoParam;

/* changes before acquisition */
relations PVM_AcqScanHandler          HandleScan;

/* reconstruction */
relations RecoUserUpdate              RecoDerive;

/* adjustment related redirections */
relations PVM_AdjHandler              STB_TrajHandleAdjustmentRequests;
relations PVM_AdjResultHandler        ATB_TrajHandleAdjustmentResults;

/* visu creation */
relations VisuDerivePars              deriveVisu;

/* relations for mapshim parameter group*/
relations PVM_MapShimHandler backbone;
