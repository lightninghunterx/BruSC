/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2011
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id$
 *
 ****************************************************************/

/* react on changes of system configuration                  */
relations PVM_SysConfigHandler backbone;

/* Encoding */
relations PVM_EncodingHandler backbone;

/* geometry */
relations PVM_ImageGeometryHandler  backbone;
relations PVM_AtsRefPosHandler backbone;

/* bandwidth */
relations PVM_EffSWh           EffSWhRel;

/* modules */
relations PVM_FatSupHandler     backbone;
relations PVM_SatTransHandler   backbone;
relations PVM_FovSatHandler     backbone;
relations PVM_FlowSatHandler    backbone;
relations PVM_TriggerHandler    backbone;
relations PVM_DummyScansHandler backbone;
relations PVM_NavHandler        backbone;
relations PVM_DriftCompHandler  backbone; 

/* other parameters */
relations PVM_NucleiHandler     backbone;
relations PVM_DeriveGains       backbone;
relations PVM_RepetitionTime    backbone;
relations PVM_EchoTime          backbone;
relations PVM_NMovieFrames      backbone;
relations PVM_NRepetitions      backbone;
relations PVM_EchoPosition      backbone;
relations PVM_RareFactor        backbone;
relations PVM_NAverages         backbone;
relations PVM_AutoRgInitHandler MyRgInitRel;

/* changes before acquisition */
relations PVM_AcqScanHandler   SetBeforeAcquisition; 

/* react on parameter adjustments */
relations PVM_AdjResultHandler backbone;

/* Redirect relation for visu creation */
relations VisuDerivePars        deriveVisu;

/* redirection of method specific reconstruction method */
relations RecoUserUpdate        RecoDerive;

/* relations for mapshim parameter group*/
relations PVM_MapShimHandler backbone;

relations RecoCS               SetRecoParam;
/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/







