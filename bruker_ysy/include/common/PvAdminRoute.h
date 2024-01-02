/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2016
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVADMIN_ROUTE_H
#define PVADMIN_ROUTE_H

#
/* 
 * Administration route of components (tracer, compounds, etc). into the subject.
 * DICOM PS3.16 CID 11. 
 */
enum PvAdminRouteEnum
{
    INTRAVENOUS_ROUTE,
    INTRAATERIAL_ROUTE,
    INTRAMUSCULAR_ROUTE,
    SUBCUTANEAOUS_ROUTE,
    INTRACUTANEOUS_ROUTE,
    INTRAPERITONEAL_ROUTE,
    INTRAMEDULLARY_ROUTE,
    INTRATHECAL_ROUTE,
    INTRAARTICULAR_ROUTE,
    INTRAEPITHELIAL_ROUTE,
    TOPICAL_ROUTE,
    ORAL_ROUTE,
    TRANSLUMINAL_ROUTE,
    INTRALIMUNAL_ROUTE,
    EXTRALUMINAL_ROUTE,
    INHALATION_ROUTE,
    RECTUM_ROUTE,
    VAGINAL_ROUTE,
    INTRACORONARY_ROUTE,
    INTRACARDIAC_ROUTE,
    INTRAVENTRICULAR_CARDIAC_ROUTE,
    RETRO_ORBITAL_ROUTE,
    NASAL_ROUTE,
    INTRADERMAL_ROUTE,
    INTRATUMOR_ROUTE,
    UNKNOWN_ROUTE
};



#endif /* PVADMIN_ROUTE_H */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/



