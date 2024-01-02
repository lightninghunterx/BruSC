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

#ifndef PVGEOCLASSES_H
#define PVGEOCLASSES_H

#ifndef CPROTO

#include "common/PvTypes/geoTypes.h"
#include "common/ovl_toolbox/geoObject.hh"

/**
 * @class GObject PvGeoClasses.hh
 * This concrete class implements routines used to transfer information
 * from geometry editor interface parameters of type GEO_CUBOID_PACK
 * to high-level interface parameters and -groups and vice versa.
 * @c GObject is a class to represent and manipulate parameters of type 
 *            GEO_CUBOID_PACK
 * @version $Id$
 * @since ParaVision 6.Beta.0.101
 * @ingroup PVM_GTB_CLS
 */
class DLL_INTERFACE GObject :  public Gobj
{
  private:


  /** Initializes a default extent.
   *  @param[out] ext default extent (unit mm).
   */
  virtual void getDefaultExt(double ext[3]); 

  /** Sets hardware origin.
   *  A default hardware origin is derived.
   *  It describes the offset between hardware origin and
   *  modality origin.
   */
  void setHwOffs(void);


  /**Deleted constructors. */
  GObject(void) PV_CXX_DELETE;

  /// Deleted copy constructor.
  GObject(const GObject &) PV_CXX_DELETE;

  /// Deleted assignment operator.
  GObject & operator=(const GObject &) PV_CXX_DELETE;

  protected:

  public:
  
  /** constructor */
  GObject(const char *const gcnam);

  virtual ~GObject() = default;

  /** Initializes cuboid packages.
   */
  void initCuboidArr(void);
  

};


/** Derive ATS offset from position in Geometry Master System.
    

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  (...)
  // get vector from cuboid e.g. position of first slice package 
  double vm[3];
  memcpy(vm,PVM_SliceGeo[0].surrounding.ref.position,3*sizeof(double));
  double atsoffs;
  GTB_TransformMasterToAtsOffset(&atsoffs,vm);

  // use this offset to set the table position
  ATB_AtsSetPos(&atsoffs,1);


  @endcode			
  
  @param[out] atsoffs     Offset of ATS in ATS coordinate system
  @param[in]  vm 3-element vectof of cuboid position in master coordinate 
                 system

  @ingroup PVM_GTB
 */
DLL_INTERFACE void GTB_TransformMasterToAtsOffset(double *const atsoffs,
                                                 const double vm[3]);
/** Transform ATS offset into position in Geometry Master System.
    

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  (...)
  // set distance to modality center into geometry object
  // Note: parameter ACQ_AtsCurrentOffset has only 
  // during scan start or in adjustments a valid value
  
  GTB_TransformAtsOffsetToMaster(ACQ_AtsCurentOffset,
                                 PVM_SliceGeo[0].surrounding.ref.mod_offset);


  @endcode			
  
  @param[in]  atsoffs Offset of ATS in ATS coordinate system
  @param[out] vm      3-element vector in master coordinate system

  @ingroup PVM_GTB
 */
DLL_INTERFACE void GTB_TransformAtsOffsetToMaster(const double atsoffs,
                                                  double vm[3]);

#endif // CPROTO
#endif // _GEO_OBJECT_H
