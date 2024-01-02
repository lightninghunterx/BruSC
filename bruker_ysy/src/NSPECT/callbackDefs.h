/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2018
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
relations PVM_SysConfigHandler  backbone;
relations PVM_AtsRefPosHandler backbone;
/* spectroscopy group */
relations PVM_SpecHandler       backbone;

/* modules */
relations PVM_FatSupHandler     backbone;
relations PVM_TriggerHandler    backbone;
relations PVM_DummyScansHandler backbone;
relations PVM_NavHandler        backbone;
relations PVM_DriftCompHandler  backbone;
relations PVM_MapShimHandler    backbone;
relations PVM_SatTransHandler   backbone;

/* other parameters */
relations PVM_NucleiHandler     backbone;
relations PVM_DeriveGains       backbone;
relations PVM_RepetitionTime    backbone;
relations PVM_EchoTime          backbone;
relations PVM_NAverages         Local_NAveragesHandler;
relations PVM_NRepetitions      backbone;    
relations PVM_FovSatHandler     backbone;
relations PVM_WsHandler         backbone;
relations PVM_DecHandler        backbone;
relations PVM_NoeHandler        backbone;
relations PVM_EncodingHandler   backbone;

/* react to adjustments */
relations PVM_AdjHandler        LocalAdjHandler;
relations PVM_AdjResultHandler  backbone;
relations PVM_RefScanHandler    backbone;

/* reconstruction */
relations RecoUserUpdate        DeriveReco;

relations PVM_ImportID          CopySliceParGroup;
relations PVM_AtsRefPosHandler  backbone;


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
