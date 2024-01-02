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

/* react on changes of system configuration */
relations PVM_SysConfigHandler backbone;
relations PVM_AtsRefPosHandler backbone;
/* spectroscopy group */
relations PVM_SpecHandler       backbone;

/* mapshim */
relations PVM_MapShimHandler    backbone;

/* other parameters */
relations PVM_NucleiHandler     backbone;
relations PVM_DeriveGains       backbone;
relations PVM_RepetitionTime    backbone;
relations PVM_NAverages         Local_NAveragesHandler;
relations PVM_NRepetitions      backbone;    
relations PVM_EncodingHandler   backbone;
relations PVM_DummyScansHandler backbone;

/* react to adjustment start */
relations PVM_AdjHandler        LocalAdjHandler;
/* react on parameter adjustments */
relations PVM_AdjResultHandler  LocalAdjResultHandler;

/* reconstruction */
relations RecoUserUpdate        DeriveReco;
/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/







