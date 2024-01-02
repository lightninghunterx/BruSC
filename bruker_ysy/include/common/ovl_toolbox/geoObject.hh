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

#ifndef GEOOBJECT_CLASS_H
#define GEOOBJECT_CLASS_H

#ifndef CPROTO

#include "common/PvTypes/geoTypes.h"
#include "libCore/Math/Matrix3x3.hh"
#include "common/ovl_toolbox/DLLMacro.h"

/**
 * @class Gobj geoObject.hh
 * This concrete class implements routines used to transfer information
 * from geometry editor interface parameters of type GEO_CUBOID_PACK
 * to high-level interface parameters and -groups and vice versa.
 * @c Gobj is a class to represent and manipulate parameters of type 
 *            GEO_CUBOID_PACK
 * @version $Id$
 * @since ParaVision 6.Beta.0.101
 * @ingroup PVM_GTB_CLS
 */
class DLL_INTERFACE Gobj
{
 private:
  /** Parameter name of cuboid array of type GEO_CUBOID_PACK. */
  std::string parname_;
  
  /** Size of cuboid array. */
  unsigned int siz_;
  /** Pointer to cuboid array. */
  GEO_CUBOID_PACK *gc_;
  /** Total number of subcuboids. */
  unsigned int totSc_;
  


  /** object position matrix (pv) */
  core::Matrix3x3 oMPV_;
  /** object position matrix (dicom) */
  core::Matrix3x3 oMDic_;
  /** swap matrix */
  core::Matrix3x3 swapMat_;

  /** Check index range */
  void checkIndexRange(unsigned int gcind) const;
  
  void setGc(GEO_CUBOID_PACK *gc, int numCubes);




  /** Initializes a default extent.
   *  @param[out] ext default extent (unit mm).
   */
  virtual void getDefaultExt(double ext[3]); 


  /** Sets object position matrix for transformations.
   *  Can be called only once. Object position matrix transform
   *  from modality system into ParaVision / DICOM patient
   *  coordinate system.
   *  \f[ \overrightarrow{v}_{p}= O \overrightarrow{v}_{m} \f]
   *  In case of an error an exception is thrown.
   *  @param opos object position matrix represented as 9-elem
   *         double vector in row major order.
   *  @param dicom flag indicating transformation into DICOM 
   *         patient coordinate system
   */
  bool setObjectPosMatrix(const double opos[9], bool dicom);

  /**Deleted constructors. */
  Gobj(void) PV_CXX_DELETE;

  /// Deleted copy constructor.
  Gobj(const Gobj &) PV_CXX_DELETE;

  /// Deleted assignment operator.
  Gobj & operator=(const Gobj &) PV_CXX_DELETE;

 protected:
  /** Offset of hardware with respect to modality center. */
  double hwOffs_[3];

  /** Sets hardware origin.
   *  A hardware origin specified as double array (unit mm) 
   *  in modality system is considered. It is assumed to 
   *  describe the offset between hardware origin and
   *  modality origin.
   *  @param[in] hwo hardware offset (unit mm)
   */
   void setHwOffs(const double hwo[3]);

  /** master matrix transforms from modality to master system */
  core::Matrix3x3 masterMat_;


 public:
  
  /** constructor */
  Gobj(const char *const gcnam);

  virtual ~Gobj();

  /** Initializes geometry object.
   *  @param ext extent (unit mm) of geometry object (entries > 0.0)
   */

  void initCuboidArr(double ext[3]);



  /** Checks cuboid packages.
   */
  void checkCuboidArr(void);


  /** Sets number of cubes in cuboid array. 
   *  New entries (<code>ncubes</code> greater than current size) 
   *  generated as copies of the last array entry.
   *  In case of an error an exception is thown.
   *  @param ncubes new number of cuboids
   */
  void setNCubes(const unsigned int ncubes);


  /** Sets number of cubes in cuboid array dependent on selection index.
   *  New entries (<code>ncubes</code> greater than current size) 
   *  generated as copies of the cube that is selected ( specified by
   *  <code> selind </code> ), shifted by the extent along 3rd 
   *  direction. Entries are deleted (<code>ncubes</code> less than current
   *  slice) by deletion of the selected cuboid and (in case) cuboids from
   *  the end until the new size is reached. In case of an error an
   *  exception is thrown. 
   *  In case of an error an exception is thrown.
   *  @param ncubes new number of cuboids
   *  @param selind  index of the selected cuboid
   */
  void setNCubes(const unsigned int ncubes,
                 const int selind);

  /** Sets the cuboid PARX parameter.
   *  In case of an error an exception is thrown.
   */
  void setCuboidPar(void);




  /** Sets cuboid orientation from patient acq matrix.
   *  The patient acquisiton orient matrix <code>ori</code>, represented 
   *  as 9-element double vector in row major order, transforms
   *  from ParaVision patient coordinate system into the 
   *  acquisition coordinate system. It is not necessarily right 
   *  handed (determinant either -1.0 or 1.0).
   *  In case of an error an exception is thrown
   *  To maintain a right-handed cuboid orientation matrix 
   *  (transforming from modality into acquisiton coordinate system) 
   *  the first 2 rows might be swapped (setting cuboid transposition
   *  to 1)
   *  \f[ C = T\widetilde{A}O^{*}M_m^T  \f]
   *  @param ori patient acquisition orient matrix
   *  @param gcind index of cuboid in cuboid array
   *  @param dicom flag indicating dicom or pv-patient coordinate system
   */  
  void setAcqObjToOri(const double ori[9], 
                      const unsigned int gcind,
                      bool dicom);

  /** Sets cuboid orientation from acq matrix.
   *  The acquisiton orient matrix <code>ori</code>, represented 
   *  as 9-element double vector in row major order, transforms
   *  from modality coordinate system into the 
   *  acquisition coordinate system. It is not necessarily right 
   *  handed (determinant either -1.0 or 1.0).
   *  In case of an error an exception is thrown
   *  To maintain a right-handed cuboid orientation matrix 
   *  (transforming from modality into acquisiton coordinate system) 
   *  the first 2 rows might be swapped (setting cuboid transposition
   *  to 1)
   *  \f[ C = TAM_m^T  \f]
   *  @param ori patient acquisition orient matrix
   *  @param gcind index of cuboid in cuboid array
   */  
  void setAcqToOri(const double ori[9], 
                   const unsigned int gcind);

  /** Sets cuboid orientation along modality axes.
   *  In case of an error an exception is thrown.
   *  @param gcind index of cuboid in cuboid array
   */
  void setProjModToOri(const unsigned int gcind);
 
  /** Sets major orientation.
     *  The major orientation is represented as 3-elem vector
     *  <code> cori </code> that is a permutation of triple
     *  0,1,2 dependent on the orientation matrix \f$ A^{*} \f$ 
     *  transforming from object into acquisition system, or
     * \f$ A \f$ transforming from modality into acquisition system
     *  Elements
     *  in <code> cori[i] </code> indicate the index of that element
     *  of row <code> i = 0,1,2 </code> of this matrix (stored 
     *  in row major order) that has the largest absolute value.
     *  In case of an error an exception is thrown.
     *  
     *   
     *  @param cori array to store the major orientation
     *  @param gcind index of cuboid in cuboid array
     */
    void setCoriToOri(const int cori[3], const unsigned int gcind, bool inObject);

  /** Sets major orientation in object system.
   *  The major orientation is represented as 3-elem vector 
   *  <code> cori </code> that is a permutation of triple
   *  0,1,2 dependent on the orientation matrix \f$ A^{*} \f$
   *  transforming from object into acquisition system. Elements
   *  in <code> cori[i] </code> indicate the index of that element
   *  of row <code> i = 0,1,2 </code> of this matrix (stored 
   *  in row major order) that has the largest absolute value.
   *  In case of an error an exception is thrown.
   *
   *  @param cori array to store the major orientation
   *  @param gcind index of cuboid in cuboid array
   */
   inline void setCoriToOri(const int cori[3],const unsigned int gcind) { return setCoriToOri(cori,gcind,true);};
  
  
 
  
  /** Sets major orientation in object system.
   * The major orientation is derived from a slice orientaion
   * state ( @ref SLOR_TYPE ) and the state of param firstdir 
   * (@ref PV_REOR_TYPE)
   *  <code> cori </code> that is a permutation of triple
   *  0,1,2 dependent on the orientation matrix \f$ A^{*} \f$
   *  transforming from object into acquisition system. Elements
   *  in <code> cori[i] </code> indicate the index of that element
   *  of row <code> i = 0,1,2 </code> of this matrix (stored 
   *  in row major order) that has the larges absolute value.
   *  In case of an error an exception is thrown.
   *
   *  @param lastdir enum state defining the slice orientation
   *  @param firstdir enum state defining the first major orientation of the acquisition orientation matrix
   *  @param gcind index of cuboid in cuboid array
   */
  void setSlorToOri(const SLOR_TYPE lastdir, const PV_REOR_TYPE firstdir,const unsigned int gcind);

  /** Sets number of subcuboids in cuboid.
   *  Throws an exception in case of an error (e.g. illegal arguments).
   *  @param nsubc number of subcuboids in cuboid
   *  @param maxsc max number of subcuboids in cuboid
   *  @param minsc min number of subcuboids in cuboid
   *  @param gcind index of cuboid
   */
  void setNSubc(const int nsubc,
                const int minsc,
                const int maxsc,
                const unsigned int gcind);

  /** Sets number of subcuboids in all cuboids.
   *  Throws an exception in case of an error (e.g. illegal arguments).
   *  @param nscarr array of size arrsiz containing number of subcuboids
   *  @param maxsc max number of subcuboids in all cuboids
   *  @param minsc min number of subcuboids in all cuboid
   *  @param arrsiz
   */
  void setNSubc(const int *nscarr,
                const int maxsc,
                const int minsc,
                const unsigned int arrsiz);

  /** Sets position of cuboids from position in obj acq coordinate system.
   *  The obj acq coodinate system has its zeropoint inside the object.
   *  Position must be set after the cuboid orientations have their
   *  final values.
   *  \f[ \overrightarrow{v}_M = (TC)^T\overrightarrow{v}_{a^{*}}  \f]
   *  Throws an exception in case of an error (e.g. illegal arguments).
   *  @param pos position in acq coordinate system (unit mm)
   *  @param gcind index of cuboid
   */
  void setAcqObjToPos(const double pos[3],
                      const unsigned int gcind);
  /** Sets position of cuboids from position in acq coordinate system.
   *  The acq coodinate system has its zeropoint inside the modality.
   *  Position must be set after the cuboid orientations have their
   *  final values.
   *  
   \f[ \overrightarrow{v}_M = (TC)^T\overrightarrow{v}_{a} + 
   \overrightarrow{o}_{M}
   \f]
   *  Throws an exception in case of an error (e.g. illegal arguments).
   *  @param pos position in acq coordinate system (unit mm)
   *  @param gcind index of cuboid
   */
  void setAcqToPos(const double pos[3],
                   const unsigned int gcind);

  /** Sets modality offset cuboids from position.
   *  Sets modality offset (\f$ \overrightarrow{o}_M \f$) to the projection of 
   *  the cuboid position (\f$ \overrightarrow{v}_M \f$)  along the ATS 
   *  direction (\f$ \overrightarrow{e}_a \f$) in master system.
   *  
   \f[\overrightarrow{o}_{M} =  \overrightarrow{e}_a (\overrightarrow{e}_a \overrightarrow{v}_M)
   \f]
   *  Throws an exception in case of an error (e.g. illegal arguments).
   *  @param gcind index of cuboid
   */
  void setPosToModoffset(const unsigned int gcind);

  /** Sets position of cuboids from position in modality coordinate system.
   *  Position must be set after the cuboid orientations have their
   *  final values.
   *  
   \f[ \overrightarrow{v}_M = M_m \overrightarrow{v}_{m} + 
   \overrightarrow{o}_{M}
   \f]
   *  Throws an exception in case of an error (e.g. illegal arguments).
   *  @param pos position in acq coordinate system (unit mm)
   *  @param gcind index of cuboid
   */
  void setModToPos(const double pos[3],
                   const unsigned int gcind);

  /** Sets position of cuboids from position in master coordinate system.
   *  Position must be set after the cuboid orientations have their
   *  final values.
   *  
   *  Throws an exception in case of an error (e.g. illegal arguments).
   *  @param pos position in acq coordinate system (unit mm)
   *  @param gcind index of cuboid
   */
  void setMasterToPos(const double pos[3],
                      const unsigned int gcind);




  /** Sets  orientation for given slice orientation.
   *  Dependent on slice orientation (axial, sagittal, coronal, SLOR_TYPE)
   *  and direction of 1st vector in orientation matrix (0=L_R, 1=A_P, 
   *  2=HZ_F) an orientation matrix is initialized.
   *  Throws an exception in case of an error (e.g. illegal arguments).
   *  @param slor desired slice orientation
   *  @param dir1 desired direction of 1st orientation vector
   *  @param gcind index of cuboid
   */
  void setSlorToOri(const SLOR_TYPE slor,
                    const unsigned int dir1,
                    const unsigned int gcind);

  /** Sets  orientation for given slice orientation.
   *  Dependent on slice orientation (axial, sagittal, coronal, SLOR_TYPE)
   *  an orientation matrix is initialized.
   *  Throws an exception in case of an error (e.g. illegal arguments).
   *  @param slor desired slice orientation
   *  @param gcind index of cuboid
   */
  void setSlorToOri(const SLOR_TYPE slor,
                    const unsigned int gcind);



  /** Sets cuboid orientation.
   *  The cuboid orient matrix <code>ori</code>, represented 
   *  as 9-element double vector in row major order, transforms
   *  from master  coordinate system into the 
   *  acquisition coordinate system. It is checked to be right 
   *  handed (determinant 1.0).
   *  In case of an error an exception is thrown.
   *  If a left handed acquisition matrix is desired optional
   *  argument <code>transp</code> (default false) can be specified.
   *  (in acquisition matrix 
   *  @param ori cuboid orient matrix
   *  @param gcind index of cuboid in cuboid array
   *  @param transp indicate interchanged 1st and 2nd row of matrix
   */  
  void setMasterToOri(const double ori[9], 
                      const unsigned int gcind,
                      const bool transp = false );


  /** Sets cuboid orientation from modality matrix.
   *  The modality orient matrix <code>ori</code>, represented 
   *  as 9-element double vector in row major order, transforms
   *  from modality  coordinate system into the 
   *  acquisition coordinate system. It is checked to be right 
   *  handed (determinant 1.0).
   *  In case of an error an exception is thrown.
   *  If a left handed acquisition matrix is desired optional
   *  argument <code>transp</code> (default false) can be specified.
   *  (in acquisition matrix 
   *  @param ori modality orient matrix
   *  @param gcind index of cuboid in cuboid array
   *  @param transp indicate interchanged 1st and 2nd row of matrix
   */  
  void setModToOri(const double ori[9], 
                   const unsigned int gcind,
                   const bool transp = false );





  /** Sets position of cuboids from distance to modality or object center.
   *  Position must be set after the cuboid orientations have their
   *  final values.
   *  In case of an error an exception is thrown.
   *  @param dist desired distance to modality center (mm)
   *  @param gcind index of cuboid
   *  @param object flag if origin is in object (true)
   *         or in modality (false)
   *
   */
  void setDistToPos(const double dist, 
                    const unsigned int gcind,
                    bool object);


  /** Sets thickness of subcuboids in cuboid.
   *  Throws an except ion in case of an error.
   *  @param thk thickness (mm)
   *  @param gcind index of cuboid
   */
  void setThickness(const double thk,
                    const unsigned int gcind);
  /** Sets thickness of subcuboids in all cuboids.
   *  Throws an except ion in case of an error.
   *  @param thk thickness (mm)
   */
  void setThickness(const double thk);

  /** Sets cuboid extents from extents in acq-coordinate system.
   *  Cuboid orientations must have their final value.
   *  Throws an exception in case of an error.
   *  @param ext 3-element array containing extents along the 
   *         acquisiton axis (mm)
   *  @param gcind index of cuboid
   *
   */
  void setExtFromAcqExt(const double ext[3],
                        const unsigned int gcind);

  /** Set labels for all cuboids .
   *  Cuboid orientations must have their final value.
   *  Throws an exception in case of an error.
   *  @param acl axis labels (without sign)
   *  
   */
  void setAxisLabels(const char *const acl[3]);

  /** Get-functions no modification of cuboid array */


  /** Counts total number of subcubes in cuboid array.
   *  In case of an error an exception is thrown. 
   *  @return total number of subcubes.
   */
  int getNSubCubes(void) const;

  /** Delivers number of subcubes in cube.
   *  In case of an error an exception is thown.
   *  @param gcind index of cuboid in cuboid array.
   *  @return number of subcubes in cuboid array. 
   *          
   */
  int getNSubCubes(const unsigned int gcind) const;

  /** Delivers number of cubes (entries in cuboid array).
   *  In case of an error an exception is thrown.
   * @return number of cuboids in cuboid array 
   */
  inline int getNCubes(void) const { return (int) siz_; };

  
  /** Delivers modality offsets.
   *  Modality offset (\f$ \overrightarrow{o}_M \f$) defines the shift
   *  between the cuboid center and the modality center (both in master 
   *  coordinate system). This function delivers the modality offset for
   *  all cuboids (@c perCube==false) or all subcuboids  (@c perCube==true) 
   *  @param mos  modality offsets stored rowwise (total entries ncubesx3 
   *              or nsubcubes x 3 dependent on @a perCube)
   *  @param perCube true/false modality offsets for all subcubes/cubes 
   *          are deliverd
   *  @param unique nonzero pointers are set to true/false if modality offsets
   *         of all cubes are unique / not unique.
   */
  void getModalityOffsets(std::vector<double> &mos,
                          bool perCube,
                          bool *unique) const;

  /** Sets modality offset from ATS offset
   *  Modality offset (\f$ \overrightarrow{o}_M \f$) defines the shift
   *  between the cuboid center and the modality center (both in master 
   *  coordinate system). This function sets the modality offset for
   *  one cuboid.
   * 
   *  @param atsoffset offset in ATS system (distance from 0 position)
   *  @param gcind  index of cuboid in cuboid array
   *  @param eps (optional) accuracy for significance check
   *  @param adapt_position (optional) if modality offset differs from
   *         actual value the position is/is not changed to keep same distance
   *         to modality center (true/false)
   *  @return true if modality offset is changed significantly else false.
   */
  bool setModalityOffset(const double atsoffset,
                         const unsigned int gcind,
                         const double eps=1e-3,
                         const bool adapt_position=true );
  /** Sets modality offset
   *  Modality offset (\f$ \overrightarrow{o}_M \f$) defines the shift
   *  between the cuboid center and the modality center (both in master 
   *  coordinate system). This function sets the modality offset for
   *  one cuboid.
   * 
   *  @param moffset \f$ \overrightarrow{o}_M \f$ represented as #
   *         3-element double vector
   *  @param gcind  index of cuboid in cuboid array
   *  @param eps (optional) accuracy for significance check
   *  @param adapt_position (optional) if modality offset differs from
   *         actual value the position is/is not changed to keep same distance
   *         to modality center (true/false)
   *  @return true if modality offset is changed significantly else false.
   */
  bool setModalityOffset(const double moffset[3],
                         const unsigned int gcind,
                         const double eps=1e-3,
                         const bool adapt_position=true );
 /** Sets modality offsets
   *  Modality offset (\f$ \overrightarrow{o}_M \f$) defines the shift
   *  between the cuboid center and the modality center (both in master 
   *  coordinate system). This function sets the modality offset for
   *  all cuboids.
   * 
   *  @param moffset \f$ \overrightarrow{o}_M \f$ represented as #
   *         3-element double vector
   *  @param eps (optional) accuracy for significance check
   *  @param adapt_position (optional) if modality offset differs from
   *         actual value the position is/is not changed to keep same distance
   *         to modality center (true/false)
   *  @return true if modality offset is changed significantly else false
   */
  bool setModalityOffsets(const double moffset[3], const double eps=1e-3,
                          const bool adapt_position=true );
 /** Sets modality offsets from ATS offset
   *  Modality offset (\f$ \overrightarrow{o}_M \f$) defines the shift
   *  between the cuboid center and the modality center (both in master 
   *  coordinate system). This function sets the modality offset for
   *  all cuboids from ATS offset.
   * 
   *  @param atsoffset offset in ATS system (distance from 0 position)
   *  @param eps (optional) accuracy for significance check
   *  @param adapt_position (optional) if modality offset differs from
   *         actual value the position is/is not changed to keep same distance
   *         to modality center (true/false)
   *  @return true if modality offset is changed significantly else false
   */
  bool setModalityOffsets(const double atsoffset, const double eps=1e-3,
                          const bool adapt_position=true );

  /** Delivers cuboid extents in acquisition coordinate system.
   *  In case of an error an exception is thrown
   *  @param fov cuboid extents in acq. coordinate system.
   *  @param gcind index of cuboid in cuboid array
   */
  void getAcqFov(double fov[3], const unsigned int gcind) const;

  /** Delivers cuboid extents in image coordinate system
   *  In case of an error an exception is thrown
   *  @param fov cuboid resized to 3xNCubes entries.
   */
  void getImagFov(std::vector<double> &fov) const;

  /** Delivers slice thickness in acquisition coordinate system.
   *  In case of an error an exception is thrown
   *  @param thk slice thickness
   *  @param gcind index of cuboid in cuboid array
   */
  void getAcqSubcThick(double *const thk, const unsigned int gcind) const;

  /** Delivers cuboid position in acquisition object coordinate system.
   *  The acquisition object and cuboid coordinate system share the same
   *  zeropoint.
   *  In case of an error an exception is thown.
   \f[ \overrightarrow{v}_{a^{*}} = (TC)\overrightarrow{v}_{M}
   \f]   
   @param pos cuboid position in acq. coordinate system.
   *  @param gcind index of cuboid in cuboid array
   */  
  void getPosToAcqObj(double pos[3], const unsigned int gcind) const;

  /** Delivers cuboid position in master coordinate system.
   *  The function delivers the actual cuboid position.
   *  \f[ \overrightarrow{v}_{M}\f] .
   *  In case of an error an exception is thown.
   @param pos cuboid position in master system.
   *  @param gcind index of cuboid in cuboid array
   */  
  void getPosToMaster(double pos[3], const unsigned int gcind) const;

  /** Delivers the projection of orientation vectors to ATS system
   *  In case of an error an exception is thrown
   *  @param proj projection of the 1st, 2nd and 3rd unit vector
   *  @param gcind index of cuboid in cuboid array
   */
  void getProj2Ats(double proj[3], const unsigned int gcind) const;

  /** Delivers ATS offset aligning modality center to cuboid position.
   *  The ATS offset for the given cuboid (in ATS coordinate system)
   *  calculated by this routine aligns the object in the direction
   *  of the ATS with the modality center at the given cuboid postion.
   *  In case of an error an exception is thown
   *  @param offs ATS position 
   *  @param gcind index of cuboid in cuboid array
   */  
  void getPosToAtsOffset(double *const offs,const unsigned int gcind) const;

  /** Delivers cuboid position in acq coordinate system.
   *  The acq coordinate system might have a
   *  different zeropoint.
   *  This position may be used to derive acquisition offsets
   *  with respect to the modaliy zeropoint.
   \f[ \overrightarrow{v}_{a} = TC (\overrightarrow{v}_{M} - 
   \overrightarrow{o}_{M}) \f]   
   *  In case of an error an exception is thrown
   *  @param pos cuboid position in acq. coordinate system.
   *  @param gcind index of cuboid in cuboid array
   */  
  void getPosToAcq(double pos[3], const unsigned int gcind) const;

  /** Delivers cuboid position in modality coordinate system.
   *  The modality coordinate system might have a
   *  different zeropoint.
   \f[ \overrightarrow{v}_m = M^T(\overrightarrow{v}_{M} -\overrightarrow{o}_{M}) \f]
   *  In case of an error an exception is thrown
   *  @param pos cuboid position in mod. coordinate system.
   *  @param gcind index of cuboid in cuboid array
   */  
  void getPosToMod(double pos[3], const unsigned int gcind) const;

  /** Delivers distance of cuboid position to origin.
   *  In case of an error an exception is thrown.
   *  @param dist (o) reference to distance
   *  @param gcind index of cuboid in cuboid array
   *  @param object (optional) flag indicating origin in object (true)
   *         or modality (false)
   */
  void getPosToDist(double &dist, 
                    const unsigned int gcind,
                    bool object) const;


  
  /** Delivers the acquisition object orient matrix.
   *  The acquisition object orient matrix <code>ori</code>, represented 
   *  as 9-element double vector in row major order, transforms
   *  from object coordinate system into the acquisition coordinate 
   *  system. It is not necessarily right handed (determinant either
   *  -1.0 or 1.0).
   *  \f[ A^{*}_{m} = TCM_{m}O^{*T} \f]
   *  In case of an error an exception is thrown
   *  @param ori acquisition orient matrix
   *  @param gcind index of cuboid in cuboid array
   *  @param dicom flag indicating dicom or pat-pv object coordinate system
   */  
  void getOriToAcqObj(double ori[9], 
                      const unsigned int gcind,
                      bool dicom) const;

  /** Delivers the master orient matrix.
   *  The master orient matrix <code>ori</code>, represented 
   *  as 9-element double vector in row major order, transforms
   *  from master coordinate system into the acquisition coordinate 
   *  system. It is always right handed (determinant  1.0).
   *  In case a left handed acquisition matrix is desired
   *  optional argument <code>transp</code> is set to true.
   *  In case of an error an exception is thrown
   *  @param ori master orient matrix
   *  @param gcind index of cuboid in cuboid array
   *  @param transp flag indicating a transposition (swap of 1st and 2nd row)
   */  
  void getOriToMaster(double ori[9],
                      const unsigned int gcind,
                      bool *transp=0) const;

  /** Delivers the modality orient matrix.
   *  The modality orient matrix <code>ori</code>, represented 
   *  as 9-element double vector in row major order, transforms
   *  from modality coordinate system into the acquisition coordinate 
   *  system. It is always right handed (determinant  1.0).
   *  In case a left handed acquisition matrix is desired
   *  optional argument <code>transp</code> is set to true.
   *  In case of an error an exception is thrown
   *  @param ori modality orient matrix
   *  @param gcind index of cuboid in cuboid array
   *  @param transp flag indicating a transposition (swap of 1st and 2nd row)
   */  
  void getOriToModality(double ori[9],
                        const unsigned int gcind,
                        bool *transp=0) const;




  /** Delivers the image orient matrix.
   *  The orient matrix <code>ori</code>, represented 
   *  as 9-element double vector in row major order, transforms
   *  from dicom object coordinate system into the image coordinate 
   *  system. It is always right handed due to multiplication of matrix
   *  N which is unit matrix for right handed matrices whereas for left
   *  handed matrix, the last row will be negated and the reverse flag
   *  is set to true.
   * 
   *  \f[ I_{m} = NTCM_{m}O^{*T} \f]
   *  In case of an error an exception is thrown
   *  @param ori imag orient matrix
   *  @param gcind index of cuboid in cuboid array
   *  @param reverse flag indicating a reversed slice order
   */  
  void getOriToImag(double ori[9],
                    const unsigned int gcind,
                    bool &reverse) const;


  /** Delivers the position of the top left pixel of an image in DICOM patient system.
   *  This coordinate may be used in the VISU description of images.
   *  \f[ \overrightarrow{v}_{ij} = (a_0 - fov_0/2 , a_1 - fov_1/2, a_2 +(d- fov_2)/2)^T \f]
   *  with
   *  \f[ \overrightarrow{a} = \overrightarrow{a_j} \f]
   *  where j is the index of the subcuboid ranging from 0 to the total number of 
   *  subcuboids.
   *  Dependend on dimension dim, argument pos is dimensioned to the total number of
   *  subcuboids*3 (dim=2) or cuboids (dim=3). Any other value of dim will cause an exception.
   *  @param pos vector containing all coordinates of image positions
   *  @param dim spatial dimension of the dataset
   */
  void getPosToImag(std::vector<double> &pos,
                    unsigned int dim) const;

  void getPosToImag(std::vector<double> &pos,
                    unsigned int dim,
                    std::vector<double> *ifov,
                    std::vector<double> *ishift) const;


  /** Delivers the position of the (sub) cuboid center in DICOM patient system.
   *  Dependend on dimension dim, arguments pos is dimensioned to the total number
   *  of subcuboids*3 (<code>dim=2</code>) or cuboids (<code>dim=3</code>). 
   *  Param <code> pos </code> contains coordinates (x,y,z) in the DICOM patient 
   *  system for each (sub)cuboid.
   *  In case of an error an exception is thrown.
   *  @param pos position in DICOM patient system
   *  @param dim spatial dimensionality of frames
   *  @param subcubes if set to true, position of subcuboid center
   *         is delivered for dim==2
   */
  void getPosToPatient(std::vector<double> &pos,
                       const unsigned int dim,
                       bool subcubes) const;


  /** Delivers the slice orientation of cuboid.
   *  Slice orientation (sagittal, axial, coronal) and
   *  orientation of 1st vector in orientation matrix (represented
   *  as unsigned, 0u == L_R, 1u == A_P, 2u==H_F) is
   *  derived from orientation matrix of selected cuboid
   *  In case of an error an exception is thrown
   *  @param slor slice orientation (according to 2rd row in orientation 
   *         matrix)
   *  @param dir1 orientation of 1st row in orientation matrix
   *  @param gcind index of cuboid in cuboid array
   */  
  void getOriToSlor(SLOR_TYPE *slor, 
                    unsigned int *dir1,
                    const unsigned int gcind) const;

  /** Delivers the slice orientation of cuboid.
   *  Slice orientation (sagittal, axial, coronal) and
   *  orientation of 1st vector in orientation matrix (represented
   *  as state of @ref PV_REOR_TYPE) is
   *  derived from orientation matrix of selected cuboid
   *  In case of an error an exception is thrown
   *  @param slor slice orientation (according to 2rd row in orientation 
   *         matrix)
   *  @param firstdir orientation of 1st row in orientation matrix
   *  @param gcind index of cuboid in cuboid array
   */  
  void getOriToSlor(SLOR_TYPE *slor, 
                    PV_REOR_TYPE *firstdir,
                    const unsigned int gcind) const;



  /** Delivers the acquisition orient matrix.
   *  The acquisition orient matrix <code>ori</code>, represented 
   *  as 9-element double vector in row major order, transforms
   *  from modality coordinate system into the acquisition coordinate 
   *  system. It is not necessarily right handed (determinant either
   *  -1.0 or 1.0).
   *  \f[ A_{m} = TCM_{m} \f]
   *  In case of an error an exception is thrown.
   *  @param ori acquisition orient matrix
   *  @param gcind index of cuboid in cuboid array
   */  
  void getOriToAcq(double ori[9], const unsigned int gcind) const;



  /** Delivers the subcuboid position in acq coordinates.
   *  Dependent on (optional) argument <code> objorigin </code>
   *  the position is calculated with origin in modality center
   *  (false, default) or in object center (true)
   *  @param v vector, dimensioned to total number of
   *         subcuboids
   *  @param objorigin flag to select the desired origin
   */  
  void getAcqPosOfSc(std::vector<double> &v,bool objorigin=false) const;





  /** Delivers distance between subcuboids in cuboid.
   *  Pointer arguments may be 0, nothing done if both are 0.
   *  If number of subcuboids in cuboid is 1 dist=0 and 
   *  gap= -thickness
   *  In case of an error an exception is thrown.
   *  @param dist pointer to distance (0 pointer allowed)
   *  @param gap pointer to gap (dist - sc thickness)
   *  @param gcind inex of cuboid in cuboid array
   */
  void getScDist(double *const dist, 
                 double *const gap,
                 const unsigned int gcind);


  /** Delivers distance of subcuboid position to origin.
   *  @param md vector, dimensioned to total number of
   *            subucuboids
   *  @param object flag indicating origin in object or modality
   */
  void getDistOfSc(std::vector<double> &md,
                   bool object,
                   bool dicom) const;


  /** Delivers major orientation in object system.
   *  The major orientation is represented as 3-elem vector 
   *  <code> cori </code> that is a permutation of triple
   *  0,1,2 dependent on the orientation matrix \f$ A^{*} \f$
   *  transforming from object into acquisition system. Elements
   *  in <code> cori[i] </code> indicate the index of that element
   *  of row <code> i = 0,1,2 </code> of this matrix (stored 
   *  in row major order) that has the larges absolute value.
   *  In case of an error an exception is thrown.
   *  @param cori array to store the major orientation
   *  @param gcind index of cuboid in cuboid array
   */
  void getOriToCori(int cori[3],const unsigned int gcind) const;

  /** Delivers reco transposition of cuboid.
   *  The orientation matrix is analyzed with respect to major
   *  orientation in object system and a transposition flag
   *  is delivered ( in [0,1] ). This transposition may  be used
   *  to control the transposition stage in a reconstruction 
   *  framework.
   *
   *  In case of an error an exception is thrown.
   *  @param gcind index of cuboid in cuboid array
   *  @return reco transposition
   */
  int getRecoTransposition(const unsigned int gcind) const;


  /** Delivers reco transposition of all (sub)cuboids.
   *  The orientation matrix is analyzed with respect to major
   *  orientation in object system and a transposition flag
   *  is delivered ( in [0,1] ) and stored in an auto array. 
   *  The auto array <code> tap </code> is dimensioned
   *  to the total number of subcuboids (default) or cuboids
   *  (<code> forCubes == true </code>) .
   *  The transposition may  be used
   *  to control the transposition stage in a reconstruction 
   *  framework.
   *
   *  In case of an error an exception is thrown.
   *  @param tap vector, dimensioned to total number of (sub)cuboids
   *  @param forCubes (optional, default = <code> false </code>) flag indicating
   *                  transposition array for cubes or subcubes
   */
  void getRecoTransposition(std::vector<int> &tap, bool forCubes=false) const;

  /** Delivers cuboid index from subcuboid index.
   *  in case of an error an exception is thrown.
   *  @param scind index of the subcuboid
   *  @return index of cuboid
   */
  unsigned int getCubeFromSubcube(const unsigned int scind) const;
};


#endif // CPROTO
#endif // PVGEOCLASSES_H
