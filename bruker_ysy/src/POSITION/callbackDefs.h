/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1999-2003
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


/* inplane geometry */
relations PVM_ImageGeometryHandler  backbone;
relations PVM_AtsRefPosHandler backbone;
/* bandwidth */
relations PVM_EffSWh           EffSWhRel;

relations PVM_NucleiHandler     backbone;
relations PVM_DeriveGains       backbone;
relations PVM_RepetitionTime    backbone;
relations PVM_NAverages         Local_NAveragesHandler;

/* react on parameter adjustments */
relations PVM_AdjResultHandler backbone;

/* Encoding */
relations PVM_EncodingHandler backbone;
/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/







