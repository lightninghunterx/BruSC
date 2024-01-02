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

/* image geometry */
relations PVM_ImageGeometryHandler  backbone;
relations PVM_AtsRefPosHandler backbone;
/* bandwidth */
relations PVM_EffSWh           EffSWhRel;

/* modules */
relations PVM_TriggerHandler    backbone;
relations PVM_FatSupHandler     backbone;
relations PVM_FovSatHandler     backbone;

/* other parameters */
relations PVM_NucleiHandler     backbone;
relations PVM_DeriveGains       backbone;
relations PVM_RepetitionTime    backbone;
relations PVM_EchoPosition      EchoPositionRels;
relations PVM_EchoTime          LocalEchoTimeRelation;
relations PVM_NAverages         NAveragesRels;
relations PVM_NRepetitions      backbone;
relations PVM_InversionTime     backbone;
relations PVM_DummyScansHandler backbone;

/* react on parameter adjustments */
relations PVM_AdjResultHandler backbone;

/* Redirect relation for visu creation */
relations VisuDerivePars        deriveVisu;

/* redirection of method specific reconstruction method */
relations RecoUserUpdate        RecoDerive;

/* relations for mapshim parameter group*/
relations PVM_MapShimHandler backbone;

/* relations for ATS handling */
relations PVM_AtsHandler backbone;
/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/







