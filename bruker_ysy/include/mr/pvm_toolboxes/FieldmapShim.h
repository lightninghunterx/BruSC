/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FieldmapShim.h
 * Author: mawi
 *
 * Created on March 13, 2018, 9:26 AM
 */

#ifndef FIELDMAPSHIM_H
#define FIELDMAPSHIM_H
#ifndef CPROTO

#include "common/ovl_toolbox/dsetPvm.h"
/** 
 * @class FMap FieldmapShim.h
 * This concrete clas implements routines used to retrieve pixels and
 * coordinates of a B0 field map.
 * @version $Id$
 * @since ParaVision 7.Beta.4
 * @ingroup PVM_MRT
 */

class DLL_INTERFACE FMap: public DsetPvm::Map
{
 private:
  /** shim acquired during map acquisition 
   */

  std::vector<double> aq_shimset_;


    /** Deleted constructor. */
    FMap(void);
    /** Deleted constructor. */  
    FMap(const FMap &);
    /** Deleted copy operator */
    void operator=(const FMap &);
    /** checks if B0 map is valid.
     *  @return true if map is valid else false.
     */
    bool checkB0Map(bool readonly);


 public:
  /** Destructor */
    ~FMap();
  /** constructor using AdjProcnoResult */
    FMap(const AdjProcnoResultType & res,
            const std::vector<double> &shimset,
            bool readonly=false);
    /** provides shimset active during acquisition.
     *  @return shim vector(entries in [-100,100] percent of max current
     */
    inline const std::vector<double> &shimSet(void) const { return aq_shimset_;};
    
};





#include "PvAdj/ResultParsType.h"

#define FIELDMAPSHIM_VERSION "V 7.0"
#define FIELDMAPSHIM_FITPTS_FACTOR 1.5

/** 
 * @class FieldMapShim FieldmapShim.h
 * This concrete class implements routines for specification of B0 field maps and shim calculation.
 * @version $Id$
 * @since ParaVision 7.Beta.4
 * @ingroup PVM_MRT
 */

class DLL_INTERFACE FieldMapShim{

    private:
    /** Implementation of internal class parameter. */
    struct Impl;
    /** Implementation of internal class parameter. */
    Impl *pimpl_;
    
    /** deleted constructors. */
    FieldMapShim(const FieldMapShim&);
    /* deleted copy operator */
    FieldMapShim & operator = (const FieldMapShim &);
    
    /** Provides pixel values of the B0 field.
     * @param fieldvalues array of fieldvalues (unit Hz)
     */
    void get_map_field(std::vector<double> &fieldvalues);
    
        
    public:
    /** Constructor.
     * 
     * @param res field map represented as procno result
     * @param shimset double vector of shim set (unit %of max) during field measurement
     * @param readonly indicating usage in readonly parameter space
     */
    FieldMapShim(const AdjProcnoResultType & res,
            const std::vector<double> &shimset,
            bool readonly=false);
    /** Destructor
     */
    ~FieldMapShim();
    
    /** Provides pixel coordinates of map.
     * 
     * @param npixels (out) number of pixels (field values) im map.
     * @return 2d-array (npixels x 3) of the coordinates (unit mm, x,y,z)
     */
    double** getPixelCoordinates(uint64_t &npixels) const;

    /** Provides volume of pixel in ul.
     *  @return volume of pixel in ul. 
     */
    double getPixelVolume(void) const;
    
    /** Provides modality offset of map.
     * The modality offset (in geometry master system) is the distance
     * of the frame center to the origin of the magnet system.
     * @param modalityoffset 3-element double vector.
     */
    void getModalityOffset(double modalityoffset[3]) const;

    /** Check for identical ATS position.
            Checks whether the ATS position (represented as 3-element array)
            is the same (within a given accuracy) as position at measurement
            time of the dataset)

            @param ats_position position to be checked in master coordinate 
                   system (for geometry objects the mod_offset entry) represented
                   as 3-element vector with entries in mm
            @param accuracy (value >= 0.0 defining the accuracy of the 
                             position in mm) For values < 0 false is returned,
                             for values == 0 always true is returned                         
            @return false if one component is significantly different to 
                    the ats position of the dataset.

    */
    bool checkATSPosition( const double ats_position[3], 
                           const double accuracy) const;
                          

    /** Provides field values in voi.
     * The voi is represented as boolean mask of size npixels.
     * @param area (in) boolean mask of area 
     * @param fieldvalues double vector of field values (region
     * calculated as intersection of area and significant field values
     * @ref signalMask
     */
    void get_map_field(std::vector<bool> &area,
            std::vector<double> &fieldvalues);
    /** Provides boolean mask of field values in map.
     *
     * @return signal area represented as boolean mask of size npixels 
     */
    const std::vector<bool> & signalMask(void) const;
    
    
    /** Calculates field map shim in area identified by boolean array.
     * The shim is calculated using pixels of the intersection of voi_area
     * and signal area @ref signalMask . If provided (nonzero pointer) argument
     * @a basic_shim provides a shim condition that should be used instead
     * of the shim present during field map measurement for shim calculations
     * (may be used for dynamic shim calculations on a subset of available shims)
     * @param (in) voi_area area of pixels that are used for shim calculation. 
     * @param (out) mapshim shim in units of % of maximum 
     * @param (out) calc_status (optional) provides information about calculation
     * @param (in) basic_shim (optional) use field map interpolated to basic shim
     * @return 
     */
    bool calc_mapshim(std::vector<bool> &voi_area,
            std::vector<double> &mapshim,
            std::string *calc_status,
            std::vector<double> *basic_shim);
    
    /** Provides statistic of field in VOI with zero shim condition.
     *  Precondition: successfully calculated shim in VOI
     * @param mean (optional) mean field
     * @param sd   (optional) standard deviation of field
     * @param adev (optional) absolute deviation of field
     * @param min  (optional) minimum field value
     * @param max  (optional) maximum field value
     * @param nfieldvals(optional) number of field values in voi
     * @return true in case of success (no change of arguments in case of failure)  
     */
    bool get_object_field_statistics(double *const mean=0,
            double *const sd=0,
            double *const adev=0,
            double *const min=0,
            double *const max=0,
            uint64_t *const nfieldvals=0);

    /** Provides statistic of predicted field in VOI with calculated shim condition.
     *  Precondition: successfully calculated shim in VOI
     * @param mean (optional) mean field
     * @param sd   (optional) standard deviation of field
     * @param adev (optional) absolute deviation of field
     * @param min  (optional) minimum field value
     * @param max  (optional) maximum field value
     * @param nfieldvals(optional) number of field values in with signal in voi
     * @return true in case of success (no change of arguments in case of failure)  
     */
    bool get_predicted_field_statistics(double *const mean = 0,
            double *const sd = 0,
            double *const adev = 0,
            double *const min = 0,
            double *const max = 0,
            uint64_t *const nfieldvals=0);
  
    /** Provides statistics of field in VOI of field map.
     *  Precondition: successfully calculated shim in VOI
     * @param mean (optional) mean field
     * @param sd   (optional) standard deviation of field
     * @param adev (optional) absolute deviation of field
     * @param min  (optional) minimum field value
     * @param max  (optional) maximum field value
     * @return true in case of success (no change of arguments in case of failure)  
     */
    bool get_map_field_statistics(double *const mean = 0,
            double *const sd = 0,
            double *const adev = 0,
            double *const min = 0,
            double *const max = 0,
            uint64_t *const nfieldvals=0);
    
    /** Provides statistics of field in VOI of field map.
     * @param voi_mask mask of signal area
     * @param mean (optional) mean field
     * @param sd   (optional) standard deviation of field
     * @param adev (optional) absolute deviation of field
     * @param min  (optional) minimum field value
     * @param max  (optional) maximum field value
     * @return true in case of success (no change of arguments in case of failure)  
     */
    bool get_map_field_statistics(
            std::vector<bool> &voi_mask,
            double *const mean = 0,
            double *const sd = 0,
            double *const adev = 0,
            double *const min = 0,
            double *const max = 0,
            uint64_t *const nfieldvals=0);
    
    /** Calculates histogram of object field in VOI of field map.
     *  The object field is the mapped field without shim fields.
     *  Precondition: successfully calculated shim in VOI
     * @param numBins (in) number of bins of the histogram
     * @param histogram (in/out) pointer to a vector of counts in each bin. 
     * Will be resized to match numBins. Can already contain counts from 
     * previous calls of PvnlHistogram. Optional parameter, 
     * default is NULL. If NULL, no histogram is calculated. 
     * @param bin_centers (out) pointer to vector of histogram bin centers. 
     * Will be resized to match numBins. Optional parameter, default is NULL. 
     * If NULL, bin centers are not returned. 
     * @param cumulative
     * @param normalized
     * @param min(in) minimum of histogram range. In case of nan 
     * the minimum of the range will be set to the minimum of the data. 
     * @param max (in) maximum of histogram range. In case of nan 
     * the minimum of the range will be set to the minimum of the data. 
     * @param outliersLow (out) pinter to a counter of low outliers. The pointee 
     * will be increased by the number of data values less than min. 
     * @param outliersHigh (out) pointer to a counter of high outliers. 
     * The pointee will be increased by the number of data values greater 
     * than max. 
     * @return true in case of success
     */
    bool get_object_field_histogram( int numBins,
            std::vector<double> *histogram,
            std::vector<double> *bin_centers,
            bool cumulative,
            bool normalized,
            double min,
            double max,
            unsigned long *outliersLow,
            unsigned long *outliersHigh);
    
    /** Calculates histogram of field in VOI of field map.
     *  Precondition: successfully calculated shim in VOI
     * 
     * @param numBins (in) number of bins of the histogram
     * @param histogram (in/out) pointer to a vector of counts in each bin. 
     * Will be resized to match numBins. Can already contain counts from 
     * previous calls of PvnlHistogram. Optional parameter, 
     * default is NULL. If NULL, no histogram is calculated. 
     * @param bin_centers (out) pointer to vector of histogram bin centers. 
     * Will be resized to match numBins. Optional parameter, default is NULL. 
     * If NULL, bin centers are not returned. 
     * @param cumulative
     * @param normalized
     * @param min(in) minimum of histogram range. In case of nan 
     * the minimum of the range will be set to the minimum of the data. 
     * @param max (in) maximum of histogram range. In case of nan 
     * the minimum of the range will be set to the minimum of the data. 
     * @param outliersLow (out) pinter to a counter of low outliers. The pointee 
     * will be increased by the number of data values less than min. 
     * @param outliersHigh (out) pointer to a counter of high outliers. 
     * The pointee will be increased by the number of data values greater 
     * than max. 
     * @return true in case of success
     */
    bool get_map_field_histogram( int numBins,
            std::vector<double> *histogram,
            std::vector<double> *bin_centers,
            bool cumulative,
            bool normalized,
            double min,
            double max,
            unsigned long *outliersLow,
            unsigned long *outliersHigh);

    /** Calculates histogram of field in VOI of field map.
     * 
     * @param area (in) area of VOI represented as boolean array. 
     * @param numBins (in) number of bins of the histogram
     * @param histogram (in/out) pointer to a vector of counts in each bin. 
     * Will be resized to match numBins. Can already contain counts from 
     * previous calls of PvnlHistogram. Optional parameter, 
     * default is NULL. If NULL, no histogram is calculated. 
     * @param bin_centers (out) pointer to vector of histogram bin centers. 
     * Will be resized to match numBins. Optional parameter, default is NULL. 
     * If NULL, bin centers are not returned. 
     * @param cumulative
     * @param normalized
     * @param min(in) minimum of histogram range. In case of nan 
     * the minimum of the range will be set to the minimum of the data. 
     * @param max (in) maximum of histogram range. In case of nan 
     * the minimum of the range will be set to the minimum of the data. 
     * @param outliersLow (out) pinter to a counter of low outliers. The pointee 
     * will be increased by the number of data values less than min. 
     * @param outliersHigh (out) pointer to a counter of high outliers. 
     * The pointee will be increased by the number of data values greater 
     * than max. 
     * @return true in case of success
     */
    bool get_map_field_histogram(std::vector<bool> area,
            int numBins,
            std::vector<double> *histogram,
            std::vector<double> *bin_centers,
            bool cumulative,
            bool normalized,
            double min,
            double max,
            unsigned long *outliersLow,
            unsigned long *outliersHigh);
    
   /** Calculates histogram of predicted field in VOI of field map.
     * Precondition: successfully calculated shim in VOI
     * The predicted field is the object field superimposed by the shim 
     * field produced by the calculated shim.
     * 
     * @param numBins (in) number of bins of the histogram
     * @param histogram (in/out) pointer to a vector of counts in each bin. 
     * Will be resized to match numBins. Can already contain counts from 
     * previous calls of PvnlHistogram. Optional parameter, 
     * default is NULL. If NULL, no histogram is calculated. 
     * @param bin_centers (out) pointer to vector of histogram bin centers. 
     * Will be resized to match numBins. Optional parameter, default is NULL. 
     * If NULL, bin centers are not returned. 
     * @param cumulative
     * @param normalized
     * @param min(in) minimum of histogram range. In case of nan 
     * the minimum of the range will be set to the minimum of the data. 
     * @param max (in) maximum of histogram range. In case of nan 
     * the minimum of the range will be set to the minimum of the data. 
     * @param outliersLow (out) pinter to a counter of low outliers. The pointee 
     * will be increased by the number of data values less than min. 
     * @param outliersHigh (out) pointer to a counter of high outliers. 
     * The pointee will be increased by the number of data values greater 
     * than max. 
     * @return true in case of success
     */
    bool get_predicted_field_histogram( int numBins,
            std::vector<double> *histogram,
            std::vector<double> *bin_centers,
            bool cumulative,
            bool normalized,
            double min,
            double max,
            unsigned long *outliersLow,
            unsigned long *outliersHigh);
   /** Provides identifiers of calibrated shims.
     * Precondition: successfully calculated shim in VOI
     * The predicted field is the object field superimposed by the shim 
     * field produced by the calculated shim.
     * 
     * @return array of shim identifiers
    */
    const std::vector<std::string> & calibrated_shim_identifiers(void);

    /** Set shims that should be used for shim calculations.
     * Error conditions (keeps state unchanged):
     * Size of boolean vector use_shims is checked to be the number
     * of calibrated shims (same size as string array provided by 
     * @ref calibrated_shim_identifiers . At least one entry has to be true. 
     * @param use_shim shims 
     * @return number of true entries of shim vector or 0 in case of an error
      */
    int set_used_shims(const std::vector<bool> & use_shim);
    /** Get shims that have been used for shim calculations.
     * Size of boolean vector use_shims is the number
     * of calibrated shims (same size as string array provided by 
     * @ref calibrated_shim_identifiers . 
     * @return boolean vector of shims used for shim calculation. 
      */
    const std::vector<bool> & get_used_shims();
    /** Provides number of shims used for shim calculation.
     * 
     * @return number of shims used for shim calculation 
     */
    unsigned int used_shims(void);
    
}; // end class FieldMapShim

#endif /* CPROTO */
#endif /* FIELDMAPSHIM_H */

