/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2004
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
/* relations for ATS handling */
relations PVM_AtsRefPosHandler backbone;

/* nuclei */
relations PVM_NucleiHandler           backbone;

/* encoding */
relations PVM_EncodingHandler         backbone;

/* preparation */
relations PVM_TriggerHandler          backbone;
relations PVM_FatSupHandler           backbone;
relations PVM_FovSatHandler           backbone;
relations PVM_SegmentationHandler     backbone;
relations PVM_DummyScansHandler       backbone;
relations PVM_EvolutionHandler        backbone;

/* redirect relation for visu creation */
relations VisuDerivePars              deriveVisu;

/* ************************************************************
 *
 * relation redirection of single global parameters used in
 * this method:
 *
 * ***********************************************************/

relations PVM_RepetitionTime          backbone;
relations PVM_NAverages               backbone;
relations PVM_NRepetitions            backbone;
relations PVM_DeriveGains             backbone;
relations PVM_EffSWh                  BandWidthRel;
relations DE                          backbone;

/* react on parameter adjustments */
relations PVM_AdjResultHandler        backbone;
relations PVM_AutoRgInitHandler       MyRgInitRel;

/* changes before acquisition */
relations PVM_AcqScanHandler          SetBeforeAcquisition;

/* 
 * parameters that are used but not editable;
 * only the method may change these parameters, they
 * are redirected to the backbone routine.
 */

relations PVM_AcquisitionTime         backbone;
relations PVM_MinRepetitionTime       backbone;

/* relation redirection of method specific reconstruction */
relations RecoUserUpdate              RecoDerive;

/* relations for mapshim parameter group*/
relations PVM_MapShimHandler backbone;