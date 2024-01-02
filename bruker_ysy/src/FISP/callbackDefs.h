/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2013
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

/* Encoding */
relations PVM_EncodingHandler   backbone;

/* geometry */
relations PVM_ImageGeometryHandler  backbone;
relations PVM_AtsRefPosHandler  backbone; 

/* other parameters */
relations PVM_NucleiHandler     backbone; 
relations PVM_EffSWh            LocalSWhRels;
relations PVM_DeriveGains       backbone;
relations PVM_EchoTime          backbone;
relations PVM_NAverages         backbone;
relations PVM_RepetitionTime    repTimeRels;
relations PVM_InversionTime     backbone; 
relations PVM_NRepetitions      backbone;
relations PVM_TriggerHandler    backbone;
relations PVM_EchoPosition      MyEchoPosRel;
relations PVM_NMovieFrames      backbone;
relations PVM_DummyScansHandler backbone;
relations PVM_EvolutionHandler  backbone;
relations PVM_RareFactor        backbone;
relations PVM_AutoRgInitHandler MyRgInitRel;

/*
 * Redirect relation for visu creation
 */
relations VisuDerivePars        deriveVisu;

/* react on parameter adjustments */
relations PVM_AdjResultHandler  backbone;

/* relations for mapshim parameter group*/
relations PVM_MapShimHandler    backbone;

/* redirection of method specific reconstruction */
relations RecoUserUpdate        RecoDerive;

/* relations for ATS handling */
relations PVM_AtsHandler backbone;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/


