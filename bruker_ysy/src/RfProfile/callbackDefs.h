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


/* other parameters */
relations PVM_EffSWh            EffSWhRel;
relations PVM_Matrix            backbone;
relations PVM_Fov               backbone;

relations PVM_NucleiHandler     backbone;
relations PVM_DeriveGains       backbone;
relations PVM_RepetitionTime    backbone;
relations PVM_EchoTime          backbone;
relations PVM_NAverages         Local_NAveragesHandler;
relations PVM_NRepetitions      NrepRel;
/* react on parameter adjustments */
relations PVM_AdjResultHandler backbone;
/* Encoding */
relations PVM_EncodingHandler backbone;
/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/







