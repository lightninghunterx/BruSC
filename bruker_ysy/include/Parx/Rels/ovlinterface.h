/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 1995-2017
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

/*--------------------------------------------------------------*
 * prevent multiple includes
 *--------------------------------------------------------------*/
#ifndef PARX_OVLINTERFACE_H
#define PARX_OVLINTERFACE_H

#ifndef InParxOvl
#error This file can only be included in overlays, methods, or PARX toolboxes.
#endif


#ifndef CPROTO

#ifdef DLL_INTERFACE
#undef DLL_INTERFACE
#endif

/// DLL interface libParxRels identifiers for Windows.
#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#if defined (libParxRels_INTERNAL)
#define DLL_INTERFACE __declspec(dllexport)
#else
#define DLL_INTERFACE __declspec(dllimport)
#endif
#elif defined(__GNUC__) && __GNUC__ >= 4
#define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#define DLL_INTERFACE
#endif 

union ParamValue;


/** 
    @ingroup PvParx
    @defgroup Parcomp parcomp / methcomp parameter access functions .
    These functions are compile into the overlay, toolkit, and method code 
    by parcomp. They must not be used directly in overlay, toolkit, and 
    method code.
 */
/*@{*/

/* gen/src/lib/Parx/libParxRels/exec.c */

/** Get a pointer to a parameter value from a parameter index.
    @param[in] parNdx Parameter index.
    @return a pointer to <code>ParamValue</code> struct containing the 
            parameter value.
    @exception PvException if an error appears.
    @remark This routine is used by parcomp / methcomp to implement 
    parameter access. It must not be used manually in overlay, toolkit, 
    or method code.
*/
DLL_INTERFACE union ParamValue * parxGetPtrToVal (unsigned int parNdx);

/** Get a pointer to a parameter value from a parameter name.
    @see parxGetPtrToVal
    @param[in] name Parameter name.
    @return a pointer to <code>ParamValue</code> struct containing the 
            parameter value.
    @exception PvException if an error appears.
    @remark This routine is used by parcomp / methcomp to implement 
    parameter access. It must not be used manually in overlay, toolkit, 
    or method code.
*/

DLL_INTERFACE union ParamValue * parxGetPtrToNamedVal (const char * name);

/** Get a pointer to a parameter value from a parameter name and overlay index.
    @see parxGetPtrToNamedVal
    @param[in] name Parameter name.
    @param[in] ov_ndx Index to overlay where parameter is defined.
 * @param ovType Type of parameter where overlay is defined.
    @return a pointer to <code>ParamValue</code> struct containing the 
            parameter value.
    @exception PvException if an error appears.
    @remark This routine is used by parcomp / methcomp to implement 
    parameter access. It must not be used manually in overlay, toolkit, 
    or method code.
*/

DLL_INTERFACE union ParamValue * parxGetPtrToOvlNamedVal(const char * name, 
    int ov_ndx, OverlayType ovType);

/** Get a pointer to an array parameter value from a parameter index.
    @see parxGetPtrToVal
    @param[in] parNdx Parameter index.
    @param[in] type_size Size of type in bytes.
    @param[in] ... List of array indices. A negative value indicates the last 
               element in the list.
    @return a pointer to <code>ParamValue</code> struct containing the 
            (dereferenced) array parameter value.
    @exception PvException if an error appears.
    @remark This routine is used by parcomp / methcomp to implement 
    parameter access. It must not be used manually in overlay, toolkit, 
    or method code.
*/
DLL_INTERFACE union ParamValue * parxGetPtrToArrayVal (
  unsigned int parNdx, unsigned int type_size, ...);

/** Get a pointer to an array parameter value from a parameter name.
    @see parxGetPtrToNamedVal
    @see parxGetPtrToArrayVal
    @param[in] name Parameter name.
    @param[in]  type_size Size of type in bytes.
    @param[in] ... List of array indices. A negative value indicates the last 
               element in the list
    @return a pointer to <code>ParamValue</code> struct containing the 
            (dereferenced) array parameter value.
    @exception PvException if an error appears.
    @remark This routine is used by parcomp / methcomp to implement 
    parameter access. It must not be used manually in overlay, toolkit, 
    or method code.
*/
DLL_INTERFACE union ParamValue * parxGetPtrToNamedArrayVal (
  const char * name, unsigned int type_size, ...);


/** Get a pointer to an array parameter from a parameter name and overlay index.
    @see parxGetPtrToOvlNamedVal
    @see parxGetPtrToArrayVal
    @param name parameter name.
    @param[in] ov_ndx Index to overlay where parameter is defined.
 * @param ovType Type of overlay where overlay is defined.
    @param[in] type_size Size of parameter type in bytes.
    @param[in] ... List of array indices. A negative value indicates the last 
               element in the list.
    @return a pointer to <code>ParamValue</code> struct containing the 
            (dereferenced) array parameter value.
    @exception PvException if an error appears.
    @remark This routine is used by parcomp / methcomp to implement 
    parameter access. It must not be used manually in overlay, toolkit, 
    or method code.
*/
DLL_INTERFACE union ParamValue * parxGetPtrToOvlNamedArrayVal(
  const char * name, int ov_ndx, OverlayType ovType, unsigned int type_size, ...);


/*@}*/


#endif

#endif /* ifndef PARX_LIBPARXRELS_H_ */
