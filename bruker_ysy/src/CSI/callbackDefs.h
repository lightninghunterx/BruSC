/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999-2002
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

/* geometry */
relations PVM_ImageGeometryHandler backbone;
relations PVM_AtsRefPosHandler     backbone;
relations PVM_VoxCallBack          backbone;

/* Encoding */
relations PVM_EncodingHandler backbone;

/* spectroscopy group */
relations PVM_SpecHandler       backbone;

/* modules */
relations PVM_FovSatHandler      backbone;
relations PVM_NavHandler         backbone;
relations PVM_TriggerHandler     backbone;
relations PVM_OvsHandler         backbone;
relations PVM_WsHandler          backbone;
relations PVM_DecHandler         backbone;
relations PVM_FatSupHandler      backbone;
relations PVM_NoeHandler         backbone;
relations PVM_DriftCompHandler   backbone;
relations PVM_DummyScansHandler  backbone;


/* other parameters */
relations PVM_NucleiHandler     backbone;
relations PVM_DeriveGains       backbone;
relations PVM_RepetitionTime    backbone;
relations PVM_EchoTime          backbone;
relations PVM_NAverages         Local_NAveragesHandler;
relations PVM_MotionSupOnOff    backbone;
relations PVM_NRepetitions      backbone;

/* react to adjustment start */
relations PVM_AdjHandler        LocalAdjHandler;

/* react to adjustment end */
relations PVM_AdjResultHandler  backbone;

/* reconstruction */
relations RecoUserUpdate DeriveReco;

/* relations for mapshim parameter group*/
relations PVM_MapShimHandler backbone;

/* export to topspin */
relations TsDeriveProcPars localTsDeriveProcParsRel;
relations TsDeriveAcquPars localTsDeriveAcquParsRel;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/







