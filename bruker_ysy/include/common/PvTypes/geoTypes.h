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

#ifndef CMN_GEO_TYPES_H
#define CMN_GEO_TYPES_H

#include "pvidl.h"
#include "bruktyp.h"
#include "generated/GeoObjIds.h"
#include "generated/ParxDefs.h"

/** @file
 *  @defgroup GeoInterface ParaVision geometry editor interface.
 *  <p>This file contains the type definitions for the ParaVision
 *     geometry editor interface and the common geometry acquisition.
 */
/*@{*/

/** 
 * The order of slices in an acquisition. 
 * @ingroup PVM_ENUM
 */
typedef enum 
{
    /// Slices are acquired sequential.
    Sequential,
      
    /// Slices are acquired reverse sequential.
    Reverse_sequential,
        
    /// Slices are acquired interlaced.
    Interlaced,
    
    /// Slices are acquired reverse interlaced.
    Reverse_interlaced,
        
    /// Slices are acquired in a special angiography scheme.
    Angiography,
  User_defined_slice_scheme
} PV_SLICE_SCHEME_TYPE;

/** Definition of a slice orientation. 
    A slice orientation is defined in the object coordinate system.
 * @ingroup PVM_ENUM
 */
typedef enum 
{
    /** Slices are acquired in a transversal geometry orientation. */
    axial,
     
    /** Slices are acquired in a sagittal geometry orientation. */
    sagittal,
        
    /** Slices are acquired in a coronal geometry orientation. */
    coronal
} SLOR_TYPE;

/** Definition of the first direction in the orientation matrix.
 * In MRI used to specify the read orientation.
 * With @ref SLOR_TYPE and @ref PV_REOR_TYPE a unique standard orientation 
 * matrix can be defined for the acquisition system.
 * @ingroup PVM_ENUM
 */
typedef enum
{
  /** First orientation vector along left-right direction in subject. */
  L_R,
  /** First orientation vector along anterior-posterior direction in subject. */
  A_P,
  /** First orientation vector along left-right direction in subject. */
  H_F
} PV_REOR_TYPE;

/** Definition of the gap mode of slices. 
 * @ingroup PVM_ENUM
 */
typedef enum
{
  /** No gap (gap=0) between slices. */
  contiguous,
  /** Gap different to 0 between slices. */
  non_contiguous
} PV_SLICE_GAP_MODE ; 




/** States of isotropic image geometry
    @ingroup PVM_ENUM
*/
typedef enum
{
  Isot_None,          /**< No isotropy          */
  Isot_Fov,           /**< Isotropic FOV        */
  Isot_Resolution     /**< Isotropic resolution */
} PV_ISOTROPICFOVRES ;

// old geometry
typedef enum
{
  Isotropic_Matrix,
  Isotropic_Fov,
  Isotropic_Resolution,
  Isotropic_All,
  Isotropic_None
} PV_ISOTROPIC ;


/** Definition of reference type. 
    @ingroup PVM_STRUCT
*/
typedef struct GeoReference
{
  /** Orientation matrix of cuboid in modality coordinate system. 
      The orientation matrix is orthonormal and its determinant is 1.0.
      The last row of the matrix (index 6-8) defines the slice orientation.
  */
  double orient[9];

  /** Position of the cuboid center (in mm) in modality coordinate system. 
      Zero point is defined in the measurement object
  */
  double position[3];
  /** Offset (mm) to  (modality specific) center in modality coordinate
      system.
   */
  double mod_offset[3];

} GEO_REFERENCE;


/** Definition of cuboid type. 
    @ingroup PVM_STRUCT
*/
typedef struct GeoCuboid
{
  /** Reference i.e. orientation and position */
  GEO_REFERENCE ref;

  /** Extent (in mm) of the cuboid. */
  double extent[3];

  /** Labels for the cuboid axes. Labels here need to be permutated 
      according to the transposition of the cuboid orientation  */
  
  char label[3][PV_IDL_CONSTANT(geo_MaxCGeoLableLen)];

  /** Transposition of the cuboid orientation */
  int transp;

} GEO_CUBOID;


/** Definition of a cuboid package type. 
    A cuboid package is a cuboid that contains a number subcuboids having
    the same orientation but different extents in the slice direction.

    @ingroup PVM_STRUCT
 */
typedef struct GeoCuboidPackage
{
  /** The surrounding cuboid for the subcuboids. */
  GEO_CUBOID surrounding;

  /** Number of subcuboids. */
  int nSubCuboids;

  /** Minimum of subcuboids count. */
  int nSubCuboidsMin;

  /** Maximum of subcuboids count. */
  int nSubCuboidsMax;

  /** Thickness of a subcuboid in slice direction. */
  double thickness;

  /** ID of cuboid: valid range is [0,..,NCuboids-1], a value 
      of -1 results in a deletion of the cuboid. Without deletion 
      the values of all ID's inside the cuboid array must be a valid 
      permutation of valid values.
  */
  int id;
  /** Flag whether modifications of master should be transferred to
      other elements of the cuboid array. Master is always implicitly
      selected
  */
  YesNo selected;


} GEO_CUBOID_PACK;


/** Enumeration to identify a cuboid package or a planar object. 
    @ingroup PVM_ENUM
*/
typedef enum GeoObjEnum
{
  /** Identifier for a cuboid package. */
  GeoCuboidPackId,

  /** Identifier for a planar object. */
  GeoPlanarId
} GeoObjType;


/** Enumeration to specify the shape of a cuboid.  
    @ingroup PVM_ENUM
*/
typedef enum GeoObjShapeEnum
{
  /** Standard shape: cuboid or plane */
  Standard_GobjShape,
  
  /** Cylinder inscribed into cuboid */
  Cylinder_In_GobjShape,
  
  /** Cylinder circumscribed to cuboid */
  Cylinder_Circ_GobjShape,
  
  /** Ellipsoid inscribed into cuboid */
  Ellipsoid_In_GobjShape,
  
  /** Ellipsoid circumscribed into cuboid */
  Ellipsoid_Circ_GobjShape
} GeoObjShapeType;


/** Description of a geometry object for interfacing the geometry editor. 
    @ingroup PVM_STRUCT
 */
typedef struct GeoObjDescr
{
  /** Geometry object identifier */
  char id[PV_IDL_CONSTANT(geo_MaxCGeoIdLen)];

  /** Geometry object type */
  GeoObjType type;

  /** Geometry object shape */
  GeoObjShapeType shape;

  /** Parameter name containing the cuboid package */
  char par[PV_IDL_CONSTANT(geo_MaxCGeoParnameLen)];

  /** Parameter name containing the subcuboid ordering.
      The parameter name provides an int array that specifies the 
      subcuboid ordering. It must be dimensioned with the total number
      of subcuboids and contains a valid permutation of subcuboids.
  */
  char subcord[PV_IDL_CONSTANT(geo_MaxCGeoParnameLen)];

  /** Index of the selected master cuboid.
      May be used in parameter updaters to control which cuboids are 
      removed or how new cuboids are inserted.
  */
  int selInd;

  /** Labels for the cuboid axes.
      Must not contain sign or transposition.
  */
  char label[3][PV_IDL_CONSTANT(geo_MaxCGeoLableLen)];

  /** Visibility flag.
      Controls default visibility of geometry object in geometry editor.
  */
  YesNo visibility;

  /** Bit mask to control/constrain actions inside geometry editor */
  int handle;
} GEO_OBJECT;

/** Enumeration to specify actual state of geometry editor interface.
    @ingroup PVM_ENUM
*/
typedef enum GeoHandlerEnum
{
  /** Derive geometry for method including all acquisition parameters. 
      The final geometry has been set up on a ready  dataset, 
      the method should be invoked to update all acquisition parameters. 
  */
  GeoDerive,

  /** Derive the geometry interface parameters from the method.
      The geometry interface has to be initialized according to
      the current state of the (possibly completed) dataset.
  */
  GeoInit,

  /** Changes from the user should create a fast response from method.
      The user changes geometric properties inside the editor (e.g. 
      using a slider) and a fast response of the method should 
      consider specific constraints.
  */
  GeoInProgress,

  /** The geometry has been imported from another completed scan. */
  GeoImport
} GeoHandlerType;


/** Types of dependencies between geometry objects.
    @ingroup PVM_ENUM
*/
typedef enum
{          
  GeoDep_Unused,              /**< indicates unused array entry */           
  GeoDep_Transp,              /**< same transposition */           
  GeoDep_Ori,                 /**< same orientation */           
  GeoDep_OriRot90Read,        /**< orientation rotated by 90 deg around read */
  GeoDep_OriRot90Phase,       /**< orientation rotated by 90 deg around phase */
  GeoDep_ExtentRead,          /**< same extent in read dimension */
  GeoDep_ExtentPhase,         /**< same extent in phase dimension */
  GeoDep_ExtentSlice,         /**< same extent in slice dimension */
  GeoDep_OffsetRead,          /**< fixed offset in read dimension */
  GeoDep_OffsetPhase,         /**< fixed offset in phase dimension */
  GeoDep_OffsetSlice,         /**< fixed offset in slice dimension */
  GeoDep_OffsetFlexRead,      /**< flexible offset in read dimension */
  GeoDep_OffsetFlexPhase,     /**< flexible offset in phase dimension */
  GeoDep_OffsetFlexSlice,     /**< flexible offset in slice dimension */
  GeoDep_IncludeRead,         /**< included in read dimension */
  GeoDep_IncludePhase,        /**< included in phase dimension */
  GeoDep_IncludeSlice         /**< included in slice dimension */
} GeoObjDependencyType; 

/** Describes dependencies between two geometry objects.
*/
typedef struct GeoObjDependecy
{
  /** name of master geometry object from which property is derived */
  char master[PV_IDL_CONSTANT(geo_MaxCGeoParnameLen)];
  
  /** name of slave geometry object to which property is applied */
  char slave[PV_IDL_CONSTANT(geo_MaxCGeoParnameLen)];
  
  /** unique identifier */
  int id; 
  
  /** number of dependencies */
  int nDep; 
  
  /** number of slave cuboids assigned to one master cuboid */
  int nCub;
  
  /** type of dependency */
  GeoObjDependencyType dependency[PV_IDL_CONSTANT(geo_MaxGeoDep)];
  
  /** cuboid index (-1 = all) */
  int iCub[PV_IDL_CONSTANT(geo_MaxGeoDep)];
  
  /** optional parameter storing a value for this dependency */
  char valuePar[PV_IDL_CONSTANT(geo_MaxGeoDep)][PV_IDL_CONSTANT(parx_CDisplayNameLen)];
  
  /** stores initial minimum number of slave cuboids */
  int slaveCubMin;
  
  /** stores initial maximum number of slave cuboids */
  int slaveCubMax;
  
  /* the following elements serve to keep offsets during orientation changes */
  
  /** previous number of masters */
  int prevMaster;
  
  /** previous number of slaves */
  int prevSlave;
  
  /** previous master transposition */
  int prevTransp[PV_IDL_CONSTANT(geo_MaxGeoDepMaster)];
  
  /** previous master orientation */
  double prevOrient[PV_IDL_CONSTANT(geo_MaxGeoDepMaster)][9];
  
  /** previous slave offset */
  double prevOffset[PV_IDL_CONSTANT(geo_MaxGeoDepSlave)][3];
  
} GEO_OBJ_DEP;

/*@}*/

#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/


