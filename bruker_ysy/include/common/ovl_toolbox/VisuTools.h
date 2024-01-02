/****************************************************************
 *
 * Copyright (c) 1995-2018
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVOVLTOOLS_VISU_H
#define PVOVLTOOLS_VISU_H

#ifndef CPROTO

#include <vector>
#include <string>
#include "common/ovl_toolbox/DLLMacro.h"

struct VisuFrameGroupType;
struct VisuDepValsType;

/* string definitions to be used with toolbox functions */


/*==============================================================*/
/** @file
 *  @addtogroup PvOvlTools ParaVision overlay toolbox
 *  <p>This utility library contains general functions for:</p>
 *  - setting @b VISU parameters from within method code
 */
/*@{*/


/* gen/src/prg/ovl_toolbox/VisuFGTools.c */

/** Insert a new frame group at a given index.
 *  Insert a new frame group at index <code>fgNdx</code> with
 *  <code>len</code> elements into the frame group array.
 *  All frame beginning with <code>fgNdx</code> are moved one index
 *  higher. The dimensionality of the 
 *  @param fgNdx index in frame group array.
 *  @param id identifier of frame group.
 *  @param len number of elements in frame group.
 *  @param comment frame group comment.
 */
DLL_INTERFACE void VisuInsertFG(int fgNdx, const char * id,
    int len, const char * comment);

/** Change number of dependant elements in the frame group order array.
 *  After changing the <code>VisuFGOrderDesc[].valsCnt</code> 
 *  this routine must be called to make the 
 *  <code>VisuFGOrderDesc</code>, <code>VisuGroupDepVals</code> 
 *  consistent with your the setting of the <code>.valsCnt</code>.
 *  If a parameter is inserted / deleted it is inserted /deleted at 
 *  the end of parameter list of this frame group. 
 *  @param from First changed frame group index.
 *  @param to Last changed frame group index.
 */
DLL_INTERFACE void VisuChangeFGOrder(int from, int to);

/** Get frame group index for a given frame group identifier.
 *  @param groupId frame group identifier.
 *  @return frame group index or -1 if a frame group with identifier
 *    <code>groupId</code> does not exist.
 */
DLL_INTERFACE int VisuGetFgNdxFromGroupId(const char * groupId);

/** Search if parameter is a dependant parameter of a frame group.
 * This routine does not make a parameter access in the routine.
 *  @param orderDesc The frame group order description structure.
 *  @param orderDescDim Number of frame group order description elements.
 *  @param depVals Array of dependent values.
 *  @param fgStart start index to search in the frame group array.
 *  @param parName name of dependant parameter to search.
 *  @param depStart return start index in dependant parameter array of
 *           dependant values for the found frame group. Value is only
 *           set if a frame group is found.
 *  @return first frame group index <code> >= fgStart</code> to which
 *     the parameter <code>parName</code> is dependant. -1 is return
 *     if a frame group cannot be found.
 */
DLL_INTERFACE int VisuSearchDepVal(const struct VisuFrameGroupType * orderDesc,
    int orderDescDim, const struct VisuDepValsType * depVals, int fgStart,
    const char * parName, int * depStart);

/** Search if parameter is a dependant parameter of a frame group.
 *  @param fgStart start index to search in the frame group array.
 *  @param parName name of dependant parameter to search.
 *  @param depStart return start index in dependant parameter array of
 *           dependant values for the found frame group. Value is only
 *           set if a frame group is found.
 *  @return first frame group index <code> >= fgStart</code> to which
 *     the parameter <code>parName</code> is dependant. -1 is return
 *     if a frame group cannot be found.
 */
DLL_INTERFACE int VisuSearchDepVal(int fgStart, const char * parName,
    int * depStart);

/** Set a dependant value parameter into a frame group.
 *  @param fgNdx frame group index.
 *  @param valsNdx relative index to dependant parameter in frame group.
 *  @param parName name of dependant parameter.
 *  @param startNdx start index of dependant values in dependant parameter
 *                  array.
 */
DLL_INTERFACE void VisuSetFGDependentVals(int fgNdx, int valsNdx,
    const char * parName, int startNdx);

/**
 * Append a parameter to the list of dependent parameters of a frame group.
 * 
 * @param fgId  The frame group identifier.
 * @param depPar The array parameter to be inserted as dependent parameter of the 
 * frame group.
 * @param startIdx The start index of the dependent values in the \a depPar array.
 * @return \c false if the frame group \a fgId does not exist else \c true
 * @throws PvException in case of an error.
 */
DLL_INTERFACE bool VisuAppendFgDependentParameter(const char * fgId, 
    const char * depPar, int startIdx);

/**
 * Append a parameter to the list of dependent parameters of a frame group.
 * 
 * @param fgIdx  The frame group index in the list of frame groups..
 * @param depPar The array parameter to be inserted as dependent parameter of the 
 * frame group.
 * @param startIdx The start index of the dependent values in the \a depPar array.
 * @return \c false if the frame group index \a fgIdx is out of range else \c true
 * @throws PvException in case of an error.
 */
DLL_INTERFACE bool VisuAppendFgDependentParameter(int fgIdx, const char * depPar,
    int startIdx);

/** Remove dependant value parameter from frame group.
 *  @param fgNdx frame group index.
 *  @param parName dependant parameter name.
 *  @param adaptDepVals if <code>Yes</code> adapt dependant value 
 *     that is removed, e.g. reset it or decrease its length if 
 *     same parameter is dependant to different frame groups.
 *  @return start index of dependant values in parameter array 
 *     (even if removed) or -1 if the <code>parName</code> does not
 *      depend on the frame group <code>fgNdx</code>.
 */
DLL_INTERFACE int VisuRemoveFGDependantVal(int fgNdx, const char * parName,
    bool adaptDepVals);

/** Change number of frame group elements.
 *  The length (number of elements) of a frame group is changed 
 *  (decreased or increased). The
 *  corresponding array length of the dependant parameters are also 
 *  adapted.
 *  @param[in] fgNdx frame group index.
 *  @param[in] newLen new number of frame group elements, must not be 0 or
 *     lower than 0.
 *  @return 0 in case of success, -1 in case of error.
 */
DLL_INTERFACE int VisuChangeFGLen(int fgNdx, int newLen);

/** Set frame group element identifications as dependant parameter.
 *  @param fgNdx frame group index.
 *  @param ids identifiers for frame group elements in a one
 *     dimensions array. 
 *  @return Yes if setting was of the element identifiers was possible else
 *      No.
 */
DLL_INTERFACE bool VisuSetFgElementId(int fgNdx, const std::vector<std::string> & ids);

/** Set frame group element identifications as dependant parameter.
 *  @param fgNdx frame group index.
 *  @param ids identifiers for frame group elements in a one
 *     dimensions array. Each <code>stringLen</code> characters a new
 *     identifier is located.
 *  @param idLen number of identifiers in <code>ids</code>, must be
 *    identical to the number of elements in the frame group.
 *  @param stringLen maximum number of characters of each string in ids.
 *  @return Yes if setting was of the element identifiers was possible else
 *      No.
 */
DLL_INTERFACE bool VisuSetFgElementId(int fgNdx, const char * ids,
    int idLen, int stringLen);

/** Set frame group element comment  as dependant parameter.
 *  @param fgNdx frame group index.
 *  @param comments comment for frame group elements in a one
 *     dimensions array. 
 *  @return Yes if setting was of the element comments was possible else
 *      No.
 */
DLL_INTERFACE bool VisuSetFgElementComment(
    int fgNdx, const std::vector<std::string> & comments);

/** Set frame group element comment  as dependant parameter.
 *  @param fgNdx frame group index.
 *  @param comments comment for frame group elements in a one
 *     dimensions array. Each <code>stringLen</code> characters a new
 *     comment is located.
 *  @param commentLen number of comments in <code>comments</code>, must be
 *    identical to the number of elements in the frame group.
 *  @param stringLen maximum number of characters of each string in comments.
 *  @return Yes if setting was of the element comments was possible else
 *      No.
 */
DLL_INTERFACE bool VisuSetFgElementComment(
    int fgNdx, const char * comments, int commentLen, int stringLen);


/** Set frame group double values as dependent parameter.
 *  @param fgNdx frame group index.
 *  @param depVals double values for frame group elements in a one
 *     dimensional array.
 *  @param depValsLen number of double values in \a depVals, must be
 *    identical to the number of elements in the frame group.
 *  @return true if setting was of the element comments was possible else
 *      false.
 */
DLL_INTERFACE bool VisuSetFgDepValsDouble(
    int fgNdx, const double * depVals, int depValsLen);


/// Returns if VISU parameters indicate that this dataset was not acquired.
DLL_INTERFACE bool VisuIsDerivedDatasetType();


struct VisuFrameGroupType;
/**
 * Get the frame group description for a frame group identifier.
 * @param groupId The frame group identifier.
 * @return The frame group description, \c 0 if a frame group description
 * for this \a groupId does not exist.
 */
DLL_INTERFACE struct VisuFrameGroupType * VisuGetFgDesc(const char * groupId);

/**
 * Get the frame group description for a frame index.
 * @param fgNdx The frame group index.
 * @return The frame group description, \c 0 if a frame group description
 * for this \a fgNdx does not exist.
 */
DLL_INTERFACE struct VisuFrameGroupType * VisuGetFgDesc(int fgNdx);




#endif
#endif


/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
