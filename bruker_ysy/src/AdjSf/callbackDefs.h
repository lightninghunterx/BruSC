/* ***************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2005
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

/* react on changes of system configuration */
relations PVM_SysConfigHandler backbone;

relations PVM_NucleiHandler     backbone;

relations P                   PulseDurRel;
relations PVM_EffSWh          EffSWhRel;
relations PVM_AcquisitionTime backbone;

relations PVM_NRepetitions    backbone;
relations PVM_NAverages       backbone;


relations PVM_TriggerHandler  backbone;
relations PVM_RepetitionTime  backbone;
relations PVM_AdjHandler      HandleAdjustmentRequests;
relations PVM_EncodingHandler backbone;

/* react on parameter adjustments */
relations PVM_AdjResultHandler backbone;

/* reconstruction */
relations RecoUserUpdate        DeriveReco;

/* reference position */
relations PVM_AtsRefPosHandler backbone;

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
