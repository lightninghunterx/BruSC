/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999-2001
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


/* spectroscopy group */
relations PVM_SpecHandler       backbone;


/* other parameters */
relations PVM_NucleiHandler     backbone;
relations PVM_DeriveGains       backbone;
relations PVM_RepetitionTime    backbone;
relations PVM_EchoTime          backbone;
relations PVM_NAverages         Local_NAveragesHandler;
relations PVM_NRepetitions      NrepRel;    
relations PVM_TriggerHandler    backbone;
relations PVM_DummyScansHandler backbone;

/* Encoding */
relations PVM_EncodingHandler   backbone;
/* react to adjustment start */
relations PVM_AdjHandler        LocalAdjHandler;
/* react on parameter adjustments */
relations PVM_AdjResultHandler  backbone;

/* reconstruction */
relations RecoUserUpdate        DeriveReco;
/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/







