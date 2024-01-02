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

/* Encoding */
relations PVM_EncodingHandler backbone;

/* geometry */
relations PVM_ImageGeometryHandler  backbone;
/* reference position */
relations PVM_AtsRefPosHandler        backbone;



/* bandwidth  */
relations PVM_EffSWh            EffSWhRel;


/* modules */
relations PVM_FatSupHandler     backbone;
relations PVM_SatTransHandler   backbone;
relations PVM_FovSatHandler     backbone;
relations PVM_FlowSatHandler    backbone; 
relations PVM_TriggerHandler    backbone; 
relations PVM_EvolutionHandler  backbone; 
relations PVM_SelIrHandler      backbone; 
relations PVM_BlBloodHandler    backbone;
relations PVM_DummyScansHandler backbone;
relations PVM_TaggingHandler    backbone;
relations PVM_FwsHandler        backbone;
relations PVM_FwsRecoHandler    SetRecoParam;
relations PVM_DriftCompHandler  backbone;

/* other parameters */
relations PVM_NucleiHandler     backbone;
relations PVM_DeriveGains       backbone;
relations PVM_EchoTime          backbone;
relations PVM_NAverages         NAveragesRels;
relations PVM_MotionSupOnOff    backbone;
relations PVM_RepetitionTime    backbone;
relations PVM_RareFactor        backbone;
relations PVM_FlipBackOnOff     backbone;
relations PVM_NRepetitions      backbone;
relations PVM_EchoPosition      backbone;
relations PVM_NEchoImages       backbone;
relations PVM_InversionTime     localInversionRel;

/* react on parameter adjustments */
relations PVM_AdjResultHandler  backbone;

/* relations for mapshim parameter group*/
relations PVM_MapShimHandler    backbone;


/* Redirect relation for visu creation */
relations VisuDerivePars        deriveVisu;

/* redirection of method specific reconstruction */
relations RecoUserUpdate        RecoDerive;

/* relations for ATS handling */
relations PVM_AtsHandler backbone;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/





