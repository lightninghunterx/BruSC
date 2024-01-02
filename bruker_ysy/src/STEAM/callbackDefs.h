
/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001-2006
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * ***************************************************************/


/* react on changes of system configuration                  */
relations PVM_SysConfigHandler   backbone;

relations PVM_NucleiHandler      backbone;
relations PVM_NAverages          AveragesRel;
relations PVM_EchoTime           backbone;
relations PVM_MinEchoTime        backbone;
relations PVM_RepetitionTime     backbone;
relations PVM_VoxCallBack        backbone;
relations PVM_AtsRefPosHandler   backbone;
relations PVM_SpecHandler        backbone;
relations PVM_DeriveGains        DeriveGainsRel;
relations PVM_NRepetitions       backbone;
relations PVM_TriggerHandler     backbone;
relations PVM_OvsHandler         backbone;
relations PVM_WsHandler          backbone;
relations PVM_DecHandler         backbone;
relations PVM_NoeHandler         backbone;
relations PVM_ScanTimeStr        backbone;
relations PVM_DummyScansHandler  backbone;
relations PVM_NavHandler         backbone;
relations PVM_EncodingHandler    backbone;
relations PVM_DriftCompHandler   backbone; 

/* adjustment related redirections */
relations PVM_AdjResultHandler   backbone;
relations PVM_AdjHandler         HandleAdjustmentRequests;
relations PVM_RefScanHandler     backbone;
relations PVM_TuneShimHandler    backbone;
relations PVM_TuneShimQP         TuneShimQPRel;
relations PVM_TuneShimFinishAdjHandler TuneShimFinishAdj;
/* relations for mapshim parameter group*/
relations PVM_MapShimHandler backbone;

/* reconstruction */
relations RecoUserUpdate         DeriveReco;




/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
