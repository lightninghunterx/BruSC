/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2020
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
relations PVM_SysConfigHandler    SysConfigHandler;
relations PVM_AtsRefPosHandler    backbone;

/* spectroscopy group */
relations PVM_SpecHandler         backbone;

/* modules */
relations PVM_FatSupHandler       backbone;
relations PVM_TriggerHandler      backbone;
relations PVM_DummyScansHandler   backbone;
relations PVM_NavHandler          backbone;
relations PVM_DriftCompHandler    backbone;
relations PVM_MapShimHandler      backbone;

/* other parameters */
relations PVM_NucleiHandler       backbone;
relations PVM_DeriveGains         backbone;
relations PVM_RepetitionTime      backbone;
relations PVM_EchoTime            backbone;
relations PVM_NAverages           backbone;
relations PVM_FovSatHandler       backbone;
relations PVM_WsHandler           backbone;
relations PVM_DecHandler          backbone;
relations PVM_NoeHandler          backbone;
relations PVM_EncodingHandler     backbone;
relations PVM_ImportID            CopySliceParGroup;
relations PVM_AtsRefPosHandler    backbone;

/* on adjustments */
relations PVM_AdjHandler          AdjHandler;
relations PVM_AdjResultHandler    backbone;
relations PVM_GopAdjResultHandler GopAdjResultHandler;
relations PVM_StudyRefPow         StudyRefPowRelation;
relations PVM_AdjRefPowXPwr       ExcDurPowResultRel;
relations PVM_AdjRefPowXDur       ExcDurPowResultRel;

relations PVM_RefScanHandler      backbone;

/* reconstruction */
relations RecoUserUpdate          DeriveReco;

/* visu           */
relations VisuDerivePars          deriveVisu;

/* pdf-report generation */
relations PVM_ReportHandler       HandleReports;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
