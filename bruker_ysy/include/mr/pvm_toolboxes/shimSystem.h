/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2001 - 2017
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef _PVSHIM_SYSTEM_
#define _PVSHIM_SYSTEM_
#ifndef CPROTO
#include<vector>
#include<string>
#include<iostream>
#include<sstream>
#include "Pvnl/legendreP.hh"
#include "common/ovl_toolbox/DLLMacro.h"

/**
 * @class PvShimSystem shimSystem.h
 * This concrete class implements routines used to retrieve
 * information about the shim system and checks hardware constraints.
 * @version $Id$
 * @since ParaVision 7.Beta.2
 * @ingroup PVM_MRT_INT
 */


class DLL_INTERFACE PvShimSystem
{
 private:
  /** Implementation of internal class parameter. */
  struct Impl;
  /** Implementation of internal class parameter. */
  Impl *pimpl_;
  /** Deleted constructor. */
  PvShimSystem(const PvShimSystem&) PV_CXX_DELETE;
  /** Deleted operator. */
  PvShimSystem & operator=(const PvShimSystem &) PV_CXX_DELETE;

 public:
  /** Default constructor
   */
  PvShimSystem();
  /** Default destructor
   */
  ~PvShimSystem();

  /** Delivers solid harmonic field coefficients.
   *  The field distribution of a shim coil expressed as vector of
   *  solid harmonic coefficients in units of @f$ Hz/cm^{n}/A @f$ is
   *  delivered by this function. The normalization convention can be
   *  specified by argument with
   *  NcLegendre      : Normalization like associated Legendre Polynomials
   *  NcMaxUnitSphere : Normalization maximum on unit sphere = 1
   *  NcRomeoHoult    : Factor for leading cartesian term =1 (Romeo & Hoult)
   *  According to argument maxdeg (N) the vector is resized to @f$ (N+1)^2 @f$.
   *  In case the specified N is not large enough it is changed to the maximum
   *  degree that is required for the coil specified by id.
   *  @param id  identifier of coil 
   *  @param cmn points to a double vector, resized according to desired maxord
   *  @param maxdeg prescribed max degree of solid harmonic function, 
   *          replaced if degree of coil is higher
   *  @param nconv normalization convention see above
   *  @return true in case shim coefficients were available as BIS group
   */
   bool solidHarmField(int id,
                       std::vector<double> &cmn,
                       unsigned int &maxdeg,
                       const PVNL_LEGENDRE_NCONV nconv);


  /** Provides a key representing the state of the shim chain.
   * The key is constructed from identifies of all elements of the shim chain
   * (shim coil, gradient coil, power supply, gradient power supply ...).
   * The state is intended to be used to describe the hardware scope of shim 
   * calibration information. In case the components of the shim chain are
   * not detected, the key is set to an empty string.
   *  @return string representing the state of the shim chain.
   */
  const std::string & shimState(void);

  /** Provides a code representing the shim status.
   *  @return string representing the shim status.
   */
  const std::string & shimStatus(void);

  /** Delivers, information if dynamic shimming is possible.
   *  @return flag true/false
   */
  bool dynamicShimming(void);
  /** Delivers number of available shims.
   *  @return number of actual shims available
   */
  int  nShims(void);

  /** Delivers risetime of shim ramp.
   *  The risetime is the minimum duration of a 100% step switching
   *  event of dynamic shims.
   *  @return risetime (unit ms, if dynamic shimming is not possible 0.0ms)
   */
  double shimRiseTime(void);

  /** Delivers shim coil name.
   *  @return sn vector of size nshims of shim coil names
   */
  const std::vector<std::string> & shimCoilNames(void);
  /** Delivers shim identifiers.
   * Shim identifiers are strings that represent the specific 
   * soft- and hardware channels involved to control currents 
   * in shim coils.
   * @return vector of size nshims of shim identifiers
   */
  const std::vector<std::string> & shimIdentifiers(void);

  /** Delivers names of available solid harmonic shim functions.
   *  For @a maxdeg @f$=n@f$ a number of @f$ N=(n+1)^2@f$ solid harmonic 
   *  functions exist.
   *  @param maxdeg (optional) maximum degree of solid harmonic function.
   *  @return vector of names of available solid harmonic functions.
   */
  const  std::vector<std::string> & shimSolidHarmFuncNames(int *const maxdeg=0);
  /** Delivers maximum allowed current for shim coils.
   *  @return double vector resized to nshims containing max. currents (unit A)
   */
  const std::vector<double> & shimMaxCurrentCoil(void);
  /** Delivers maximum current of shim power supply.
      @return double vector resized to nshims containing 
      max. current (unit A) provided by shim power supply
   */
  const std::vector<double> & shimMaxCurrentSPS(void);
  /** Delivers maximum possible current for shim coils.
   *  For each coil the minimum of max. allowed and max current
   *  of shim power supply is returned.
   *  @see PvShimSystem::shimMaxCurrentCoil @see PvShimSystem::shimMaxCurrentSPS
   *  @return double vector resized to nshims containing max. 
             possible shim currents  (unit A)
   */
   const std::vector<double> & shimMaxCurrent(void);
  /** Delivers max shim value (unit % of max current SPS) for each shim coil. 
   *  Shim currents are controlled by vectors in which amplitudes are
   *  specified in units of max. current delivered by the shim power supply.
   *  In case the max. allowed shim current
   *  ( @ref PvShimSystem::shimMaxCurrentCoil ) is lower than the max. current
   *  of the shim power supply (@ref PvShimSystem::shimMaxCurrentSPS ) absolute
   *  values may be lower than 100%.
   *  @return perc_max double vector resized to nshims containing max possible
   *          shim currents (unit % of max current delivered by shim power supply).
   */
  const std::vector<double> & shimMaxPerc(void);

  /** Delivers number of shim power supplies.
   *  @return number of shim power supplies.
   */
  int  shimPowerSupplies(void);
  /** Delivers max. power of shim vector delivered by shim power supplies.
   *  For each shim power supply ( @ref PvShimSystem::shimPowerSupplies )
   *  the maximum electrical output power (unit W) to all attached coils 
   *  is returned by this function. A value of 0W may be treated as unlimited.
   *  @return double vector (resized to number of shim power supplies)
   */
  const std::vector<double> & shimMaxPowerPerSPS(void);

  /** Delivers maximum sum current of shim vector delivered by shim power supplies.
   *  For each shim power supply ( @ref PvShimSystem::shimPowerSupplies )
   *  the maximum sum current (sum of absolute values of output currents unit A)
   *  is returned by this function. A value of 0A may be treated as unlimited.
   *  @return double vector (resized to number of shim power supplies)
   */
  const std::vector<double> & shimMaxSumCurrentPerSPS(void);
  /** Delivers maximum allowed electrical power of shim vectors.
   *  @return max electrical power (unit W) 0.0W can be treated as unlimited.
   */
  double shimMaxTotalPower(void);


  /** Delivers information which coil is affected by total current constraints.
   *  In a integer vector (size number of shims @ref PvShimSystem::nShims )
   *  it is indicated which coil is attached to which power supply.
   *  An index of 0 means the corresponding shim is not attached to any
   *  power supply and thus not limited). Indices > 0 determine the shim
   *  power supply (SPS 1, SPS 2 ...).
   *  @return integer vector (size nshims) containing shim power supply indices
   */
  const std::vector<int> & shimCoilCurrentConstrained(void);
  /** Delivers the resistance of shim coils.
   *  @return double vector (resized to number of shims) 
   *  containing the shim coil resistance (unit Ohm)
   */
  const std::vector<double> & shimCoilResistance(void);

  /** Delivers the max. field deviation within a sphere.
   *  Dependent on sensitivities ( unit @f$ Hz/cm^n/A @f$ or @f$ Hz/mm^n/A @f$ )
   *  defined for real valued solid harmonics the maximum field deviation in 
   *  a sphere is delivered by this function.
   *  Coefficients are provided as double arrays (length unit as defined by 
   *  argument unitCm ) in which entries are related to solid harmonic functions
   *  as defined in @see LegendreP::Field 
   *  @param co [in] coefficients 
   *  @param nconv [in] normalization convention of coefficients
   *  @param radius [in] radius of the sphere in mm
   *  @param fDev [out] resultant field deviation in @f$ Hz/A @f$
   *  @param unitCm [in] (optional) flag indicating length unit of sensitivities 
                    (true unit in cm)
   *  @param skip_zero_order [in] (optional) flag indicating whether constant term should be ignored
   *  @param cosd [in] (optional) standard deviation of coefficients
   *  @param fDevSD [out] (optional) resultant standard deviation of fDev
   */
  void maxFieldDev(std::vector<double> &co,
                   const PVNL_LEGENDRE_NCONV nconv,
                   double radius,
                   double &fDev,
                   bool unitCm=false,
                   bool skip_zero_order=true,
                   std::vector<double> *cosd=0,
                   double *const fDevSD=0);

  /** Converts shim vector (unit %) into shim vector (unit A).
   *  Counterpart of @ref PvShimSystem::shimCurrentToPerc
   *  @param sperc shim vector represented as double vector (size nshims, unit %)
   *  @param samp resultant shim vector (size nshims, unit A)
   *  @return flag indicating a legal/illegal shim vector (true / false )
   */
  bool shimPercToCurrent(const std::vector<double> &sperc,
                        std::vector<double> &samp);

  /** Converts shim vector (unit A) into shim vector (unit %).
   *  Counterpart of @ref PvShimSystem::shimPercToCurrent
   *  @param samp [in] shim vector represented as double vector (size nshims, 
   *                   unit A)
   *  @param sperc [out] resultant shim vector (size nshims, unit %)
   *  @return flag indicatin a legal/illegal shim vector (true / false )
   */
  bool shimCurrentToPerc(const std::vector<double> &samp,
                        std::vector<double> &sperc);

  /** Tests for legal shim vector.
   *  A shim vector is tested to follow hardware limitations.
   *  Total current (sum of absolute values of output currents) 
   *  and total power (electical output power) of all power supplies
   *  (index > 0 indicates real shim power supplies) is calculated
   *  on demand. 
   *  
   *  @param svec [in] shim vector (size nshims, unit %)
   *  @param t_current [out] optional pointer to double array containing 
   *                         total current (unit A).
   *  @param s_power [out] optional pointer to double array containing 
   *                       output power of shim power supply (unit W)
   *  @param total_power [out] optional pointer to total electic power
   *  @param msg [out] optional pointer to string containing failure reasons
   *  @return flag (valid/invalid true/false)
   */
  bool shimVectorValid(const std::vector<double> &svec,
                       std::vector<double> *t_current=0,
                       std::vector<double> *s_power=0,
                       double *const total_power=0,
                       std::string *msg=0);

  /** Tests for legal and unique shim identifiers.
   *  Shim identifiers (represented as strings) are checked against
   *  identifiers of the actual shim system (as delivered by 
   *  @ref PvShimSystem::shimIdentifiers ). A return value of true
   *  (valid) indicates unique shim identifiers in array @a sid 
   *  that match identifiers of the actual shim system.
   *  On demand the corresponding index in a shim vector is stored
   *  in optional argument @a shidx (remains unchanged in case of 
   *  invalid shim identifiers).
   *
   *  @param sid [in] shim identifiers as string array
   *  @param shidx [out] (optional) array of indices (size : number 
   *                      of elements in @a sid )
   *  @param msg [out] (optional) message about reason that makes 
   *                    @a sid invalid             
   *  @return flag indicating valid/invalid (true/false) and unique 
   *          shim identifiers
   *  
   */
  bool shimIdsValid(const std::vector<std::string> &sid,
                    std::vector<int> *shidx=0,
                    std::string *msg=0);


  /** Tests for legal shim increments.
   *  Shim increments are tested to follow hardware limitations.
   *  Tests the sum of increments to a (optional) base shim vector (default 0 ).
   *  Total current (sum of absolute values of output currents) 
   *  and total power (electical output power) of all power supplies
   *  (index > 0 indicates real shim power supplies) is calculated
   *  on demand. 
   *  
   *  @param sinc shim increment (size nshims, units %)
   *  @param base [in] (optional) base shim vector (size nshims, unit %)
   *  @param t_current [out] optional pointer to double array containing 
   *                         total current (unit A).
   *  @param s_power [out] optional pointer to double array containing 
   *                       output power of shim power supply (unit W)
   *  @param total_power [out] optional pointer to total electic power
   *  @param msg [out] optional pointer to string containing failure reasons
   *  @return flag (valid/invalid true/false)
   */
  bool shimIncrementValid(const std::vector<double> &sinc,
                          const std::vector<double> *base=0,
                          std::vector<double> *t_current=0,
                          std::vector<double> *s_power=0,
                          double *const total_power=0,
                          std::string *msg=0);
                          

  /** Delivers information of shim systems as string.
   *  @return shim information
   */
  std::string printShimInfo(void);

};



#endif //CPROTO
#endif //_PVSHIM_SYSTEM_





