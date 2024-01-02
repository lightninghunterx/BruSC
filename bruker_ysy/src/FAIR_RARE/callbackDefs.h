/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999-2010
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

/* inplane geometry */
relations PVM_ImageGeometryHandler  backbone;
relations PVM_AtsRefPosHandler backbone;

relations PVM_EffSWh     EffSWhRel;

/* modules */
relations PVM_FatSupHandler     backbone;
relations PVM_SatTransHandler   backbone;
relations PVM_FovSatHandler     backbone;
relations PVM_TriggerHandler    backbone; 
relations PVM_EvolutionHandler  backbone; 
relations PVM_DummyScansHandler backbone;
relations PVM_FairHandler       backbone;

/* other parameters */
relations PVM_NucleiHandler    backbone;
relations PVM_DeriveGains      backbone;
relations PVM_EchoTime         backbone;
relations PVM_NAverages        Local_NAveragesHandler;
relations PVM_MotionSupOnOff   backbone;
relations PVM_RepetitionTime   backbone;
relations PVM_RareFactor       backbone;
relations PVM_NRepetitions     backbone;
relations PVM_EchoPosition     backbone;
relations PVM_NEchoImages      backbone;

/* Specific handling of RG adjustment */
relations PVM_AutoRgInitHandler MyRgInitRel;
/* react on parameter adjustments */
relations PVM_AdjResultHandler backbone;

/*
 * Redirect relation for visu creation
 */
relations VisuDerivePars        deriveVisu;

/* relations for mapshim parameter group*/
relations PVM_MapShimHandler backbone;

/* redirection of method specific reconstruction */
relations RecoUserUpdate        RecoDerive;
/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/






