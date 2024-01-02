/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001-2005
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/

/* react on changes of system configuration                  */
relations PVM_SysConfigHandler        backbone;

/* geometry */
relations PVM_ImageGeometryHandler    backbone;
relations PVM_AtsRefPosHandler backbone;

relations PVM_EffSWh                  EffSwRel;

/* modules */
relations PVM_EncodingHandler         backbone;
relations PVM_EpiHandler              backbone;
relations PVM_FatSupHandler           backbone;
relations PVM_TriggerHandler          backbone;
relations PVM_DiffusionHandler        backbone;
relations PVM_FovSatHandler           backbone;
relations PVM_DummyScansHandler       backbone;
relations PVM_DriftCompHandler        backbone;

relations PVM_RepetitionTime          RepetitionTimeRel;
relations PVM_NAverages               AveragesRel;
relations PVM_NRepetitions            RepetitionsRel;
relations PVM_EchoTime                backbone;
relations PVM_MinEchoTime             backbone;

relations PVM_AcquisitionTime         backbone;
relations PVM_NucleiHandler           backbone; 
relations PVM_DeriveGains             DeriveGainsRel;

/* 
 * parameters that are used but not shown in editor
 * only the method may change these parameters, they
 * are redirected to the backbone routine.
 */

relations PVM_MinRepetitionTime       backbone;
relations PVM_NEchoImages             backbone;

/*
 * Redirect reconstruction relations
 */

relations RecoUserUpdate              RecoDerive;

/*
 * Redirect relation for visu creation
 */
relations VisuDerivePars             deriveVisu;

/*
 *  handle method specific adjustments
 */
relations PVM_AdjHandler HandleAdjustmentRequests;
/* react on parameter adjustments */
relations PVM_AdjResultHandler HandleAdjustmentResults;

/* relations for mapshim parameter group*/
relations PVM_MapShimHandler backbone;
