/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2012 - 2017
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

/* prevent double includes */
#ifndef GEODESCR_H
#define GEODESCR_H

#ifndef CPROTO
#include <vector>
#include <string>
#include "common/PvTypes/geoTypes.h"
#include "libCore/Math/Matrix3x3.hh"
#include "common/ovl_toolbox/DLLMacro.h"

/*==============================================================*/
/** @file
 * @defgroup PvOvlTools ParaVision overlay toolbox
 *  <p>This utility library contains general functions for:</p>
 *  - handling of completed datasets within relations
 *  - handling boolean vectors used as pixel mask
 */
/*@{*/


namespace gobj
{
  /** Incidence test for cuboid shaped geometry object.
      Units of elements @c p and @c e in mm.
      @param p 3-element vector in cuboid system 
      @param e 3-element vector of extents with respect
      to cuboid center (elements must be > 0.0, not checked for) 
      Conditions for e:
      e[i] = extent[i]/2  for i=0,1,2
      @return flag, true if @c p is in object 
  */
  DLL_INTERFACE bool IsInCube(const double *p,const double *e);

  /** Incidence test for cylinder shaped geometry object. 
      Units of elements @c p and @c e in mm.
      @param p 3-element vector in cuboid system 
      @param e 3-element vector of extents with respect
      to cuboid center (elements must be > 0.0, not checked for)
      Conditions for @c e:
      e[2] = extent[2]/2 
      and for i= 0,1
      e[i] = extent[i]/2 for shape Cylinder_In_GobjShape
      e[i] = extent[i]/sqrt(2) for shape Cylinder_Circ_GobjShape
      @return true/false 
  */
  DLL_INTERFACE bool IsInCylinder(const double *p,const double *e);

  /** Incidence test for ellipsoid shaped geometry object. 
      Units of elements @c p and @c e in mm.
      @param p 3-element vector in cuboid system 
      @param e 3-element vector of extents with respect
      to cuboid center (elements must be > 0.0, not checked for)
      Conditions for @c e:
      for shape Ellipsoid_In_GobjShape
      e[i] = extent[i]/2 
      for shape Ellipsoid_Circ_GobjShape
      e[i] = sqrt(3)*extent[i]/2 
      @return true/false  
  */
  DLL_INTERFACE bool IsInEllipsoid(const double *p,const double *e);


  /** Volume of cube shaped geometry object.
      Units of elements @c e in mm.
 
      @param e 3-element vector of extents with respect
      to cuboid center (elements must be > 0.0, not checked for) 
      Conditions for e:
      e[i] = extent[i]/2  for i=0,1,2
      @return Volume in uL (mm^3)
  */
  DLL_INTERFACE double VolumeCube(const double *e);

  /** Volume of cylinder shaped geometry object.
      Units of elements @c e in mm.
      @param e 3-element vector of extents with respect
      to cuboid center (elements must be > 0.0, not checked for)
      Conditions for @c e:
      e[2] = extent[2]/2 
      and for i= 0,1
      e[i] = extent[i]/2 for shape Cylinder_In_GobjShape
      e[i] = extent[i]/sqrt(2) for shape Cylinder_Circ_GobjShape
      @return Volume in uL (mm^3)
  */
  DLL_INTERFACE double VolumeCylinder(const double *e);

  /** Volume of ellipsoid shaped geometry object.
      Units of elements @c e in mm.
      @param e 3-element vector of extents with respect
      to cuboid center (elements must be > 0.0, not checked for)
      Conditions for @c e:
      for shape Ellipsoid_In_GobjShape
      e[i] = extent[i]/2 
      for shape Ellipsoid_Circ_GobjShape
      e[i] = sqrt(3)*extent[i]/2 
      @return Volume in uL (mm^3)
  */
  DLL_INTERFACE double VolumeEllipsoid(const double *e);

  /** Transforms from cuboid to magnet system.
      @param cpos  (in)  3-element vector coordinates 
      in cuboid system
      @param cmpos (in)  3-element vector of cuboid center 
      in magnet system
      @param mpos  (out) 3-element vector coordinates of cpos 
      in magnet system
      @param omat  (in)  transformation matrix from magnet into
      cuboid system
  */

  DLL_INTERFACE void CubeToMagnet(const double *cpos,
                    const double *cmpos,
                    double *mpos,
                    core::Matrix3x3 *omat);

  /** Transforms from magnet to cuboid system.
      @param mpos  (in) 3-element vector coordinates of cpos 
      in magnet system
      @param cmpos (in)  3-element vector of cuboid center 
      in magnet system
      @param cpos  (out) 3-element vector coordinates 
      in cuboid system
      @param omat  (in)  transformation matrix from magnet into
      cuboid system
  */
  DLL_INTERFACE void MagnetToCube(const double *mpos,
                    const double *cmpos,
                    double *cpos,
                    core::Matrix3x3 *omat);

  /** class geometry object

   */

  class DLL_INTERFACE Gcub
  {
  private:
    

    struct ImplType;
    ImplType *pimpl_= nullptr;

    int InitCub(int cubn);
    int InitSubCub(int scubno);
    int CubeEdges(int scind);
    int Bbox(int scind);
    bool ValidScIndex(int scind);

    /** forbidden constructors */
    Gcub(void);
    Gcub(const Gcub &);
    /** forbidden copy constructor */
    void operator=(const Gcub &);
    void MagnetToCube(const double *mpos,
                      const double *ccenter,
                      double *cubepos);
    
    void CubeToMagnet(const double *cubepos,
                      const double *ccenter,
                      double *mpos);
    void CubeToMaster(const double *cubepos,
                      const double *ccenter,
                      double *masterpos);
    
  public:
    Gcub(const GEO_OBJECT *gobj);

    ~Gcub();

    // double *bbox(int cubno,int scubno);
    double volume(int cubno);
    
    double gap(int cubno); 
    double dist(int cubno);
    
    int ncubes();

    int nsubcubes();
    int nsubcubes(int cubn);

    void setMarginExt(int extind,double margin);

    void setMarginThk(double margin);

    
    uint64_t InCube(int cubno,
                    double **coordinates,
                    double modalityoffset[3],
                    std::vector<bool> &mask,
                    uint64_t npixels);

    uint64_t InSubCube(int cubno,
                       int scubno,
                       double **coordinates,
                       double modalityoffset[3],
                       std::vector<bool> &mask,
                       uint64_t npixels);

    void getModalityOffset(double modoffset[3],int cubeno);

     /** indicates an error condition */
    int failed(void);

    /** */
    const std::string error();
  };
}

/** Delivers master cuboid geometry object interface parameter name.
    The master cuboid geometry object interface parameter may be used
    to derive geometry properties to controll the Animal Transport
    System (ATS). A parameter of this name is of type @a GEO_OBJECT.
    Master geometry object is registered in parameter @a PVM_GeoObj and 
    master is either an object with id 
    @a PV_IDL_CONSTANT(geo_ids_SLICE_PACK)
    (slice geometry) or with id @a PV_IDL_CONSTANT(geo_ids_VOXEL_PACK) 
    (localized spectroscopy).

  @PVM_PARS
  NONE
  @PVM_TBCALL
  @ref PvOvlGetGeoParnameFromId

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Illegal state of PVM_GeoObj (illegal parameter names)

  @PVM_EXAMPLE
  @code
  const char *gobjn=PvOvlGetMasterGeoObj();0

  bool gobjdefined=false;

  if(!gobjn)
  {
     // no geometry object defined

  }
  else
  {
    GEO_OBJECT geoobj;
    YesNo hasval=ParxRelsParGetValue(gobjn,&geoobj);
    if(!hasval)
    {
         UT_ReportError("Geometry object %s has no value!\n",gobjn);
    }

    // example code to work with associated cuboid


    GObject(gobjn.par);

    GObject.success(); // will throw an exception if failed

    // .........

  }

  @endcode

  @return @c               An empty string if no master cube is available
                           or the name of the geometry object
                           

  @see @ref PvOvlGetMasterGeoCub @ref PvOvlGetGeoParnameFromId

  @ingroup PVM_UT
 */
DLL_INTERFACE std::string PvOvlGetMasterGeoObj(void);

/** Delivers master cuboid geometry interface parameter name.
    The master cuboid geometry interface parameter may be used
    to derive geometry properties to controll the Animal Transport
    System (ATS). A parameter of this name is of type @a GEO_CUBOID_PACK.
    Master geometry object is registered in parameter @a PVM_GeoObj and 
    master is either an object with id 
    @a PV_IDL_CONSTANT(geo_ids_SLICE_PACK)
    (slice geometry) or with id @a PV_IDL_CONSTANT(geo_ids_VOXEL_PACK) 
    (localized spectroscopy).

  @PVM_PARS
  NONE
  @PVM_TBCALL
  @ref PvOvlGetGeoParnameFromId

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Illegal state of PVM_GeoObj (illegal parameter names)

  @PVM_EXAMPLE
  @code

  const char *cubname=PvOvlGetMasterGeoCub();

  bool cubedefined=false;

  if(!cubname)
  {
     // no geometry object defined

  }
  else
  {
    GObject(cubname);
    GObject.success(); // will throw an exception if failed

    // .........

  }

  @endcode

  @return @c               An empty string if no master cube is available
                           or the name of the geometry cuboid
                           

  @see @ref PvOvlGetMasterGeoObj @ref PvOvlGetGeoParnameFromId


  @ingroup PVM_UT



 */
DLL_INTERFACE std::string PvOvlGetMasterGeoCub(void);



/** Delivers geometry interface parameter name from identifier.
    Parameter that are used as interface to the geometry editor
    are registered in parameter @c PVM_GeoObj. Dependent
    on argument @a cuboidname the parameter name of type GEO_OBJECT or
    the parameter name of type GEO_CUBOID_PACK that is associated
    with @a id is delivered.

  @PVM_PARS
  NONE
  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  - Illegal state of PVM_GeoObj (illegal parameter names)

  @PVM_EXAMPLE
  @code
  const char *gobjn=PvOvlGetGeoParnameFromId(PV_IDL_CONSTANT(geo_ids_SLICE_PACK));

  bool gobjdefined=false;

  if(!gobjn)
  {
     // no geometry object defined

  }
  else
  {
    GEO_OBJECT geoobj;
    YesNo hasval=ParxRelsParGetValue(gobjn,&geoobj);
    if(!hasval)
    {
         UT_ReportError("Geometry object %s has no value!\n",gobjn);
    }

    // example code to get name of cuboid parameter
    const char *cubn=PTB_GeoParNameFromId(PV_IDL_CONSTANT(geo_ids_SLICE_PACK),Yes);

    GObject(cubn);

    GObject.success(); // will throw an exception if failed

    // .........

  }

  @endcode

  @param[in] id            String specifying the displayed geometry object
  @param[in] cuboidname    Flag indicating requirement: Yes name of 
                           geometry cuboid is delivered, No name of geometry
                           description parameter is delivered
  @return @c               An empty string if no geometry object with this id
                           is available or the name of the cuboid
                           (if <code>cuboidname</code> = Yes) or the geometry
                           object (if <code>cuboidname</code> = No)
                           

  @see @ref PvOvlGetMasterGeoObj @ref PvOvlGetMasterGeoCub


  @ingroup PVM_UT



 */
DLL_INTERFACE std::string PvOvlGetGeoParnameFromId(const char *const id,
                                                   YesNo cuboidname);

/*@}*/

#endif /* CPROTO */

#endif
