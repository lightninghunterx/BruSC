//==============================================================================
// $Source$
//  - Implementation of AtsHandler
//
// Copyright (c) 2016
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//==============================================================================

#ifndef LIB_READPARX_JOBPARAMETERS_HH
#define LIB_READPARX_JOBPARAMETERS_HH

#ifndef CPROTO

#include "libReadParx/DLLMacro.hh"
//---------------------------------------------------------------------------
// includes
//---------------------------------------------------------------------------
#include "mr/acqtyp.h"
#include <vector>
#include <map>
#include <bitset>
#include <string>
#include <iostream>

namespace Parx
{
    namespace Common
    {
        class ParamInterface;
    }
}

/**
 *  Wrapper Library to access Job specific parameters <code></code> 
 */
  
#define LOOP_SETUP   ReadParx::JobDescription::LoopSetup
#define LOOP_AVERAGE ReadParx::JobDescription::LoopAverage
#define LOOP_NONE    ReadParx::JobDescription::LoopNone
#define LOOP_DUMMIES ReadParx::JobDescription::LoopDummies
/** 
 *  The <code> JobDescription </code> class is a container for the various
 *  job specific parameters.
 *  Some methods e.g. hasInnerAverage ... derive properties from a collection
 *  of different parameters.
 */

namespace ReadParx
{
    class DLL_INTERFACE JobDescription
    {
    public:
        
      typedef enum { LoopAverage, LoopSetup, LoopDummies, LoopNone } LoopDescription;

        /**
         * create a job description from the individual structures
         * providing the complete job parameter
         * @param job           ACQ_jobs
         * @param scanSettings  ACQ_ScanPipeJobSettings
         * @param innerWeights  ACQ_ScanInnerAccumWeights
         * @param outerWeights  ACQ_ScanOuterAccumWeights
         * @param filterOptions ACQ_RxFilterFilterOptions
         * @param filterSetting ACQ_RxFilterSettings
         * @param filterInfo    ACQ_RxFilterInfo
         */
        JobDescription( const JOB_DESCRIPTION_TYPE job,
                        const SCAN_PIPE_JOB_SETTINGS_TYPE scanSettings,
                        const int* innerWeights,
                        const int* outerWeights,
                        const SETUP_PIPE_JOB_SETTINGS_TYPE setupSettings,
                        const RX_FILTER_DEFAULT_OPTIONS filterOptions,
                        const RX_FILTER_SETTING filterSetting,
                        const RX_FILTER_INFO    filterInfo,
                        const bool setupMode
                      );


        /**
         * create a default job description for a given scanSize
         * default settings include standard averaging, storing integer
         * accumulated data and activated group delay compensation
         * Derivation of filter settings is not done in this parameter
         * but achieved when written to parameter space
         * @todo: revisit behavior of bandwidth parameter settings
         * 
         * @param title   title to reference commands in pulse program
         * @param channel RF channel, job is applied to - 0 to deactivate
         * @param scanSize number of !complex! points in each scan
         * @param bandWidth desired bandwidth including anti aliasing
         */
        JobDescription( const char* title, 
                        const int channel, 
                        const int scanSize, 
                        const double bandWidth
                      );

        
        /* set basic parameters for job:  clear loop parameters to allow
         * incremental setting of loop structures from scratch.
         * 
         * Default settings for parameters are:
         * Store averaged 32bit data, display each accumulation, 
         * group delay compensation activated, receiver gain 64.0.
         * 
         * Use specific setters to overwrite these defaults.
         * 
         * @param channel RF channel, job is applied to - 0 to deactivate
         * @param scanSize number of !complex! points in each scan
         * @param bandWidth desired bandwidth including anti aliasing
         */
        void initPars( const int channel, 
                       const int scanSize, 
                       const double bandWidth);
        /**
         * Get the JOBDESCRIPTION_TYPE part of the JobDescription class
         * @return the settings for the parameter ACQ_jobs from this job
         */
        JOB_DESCRIPTION_TYPE getJobDescription(void) const;


        /**
         * get the part for setup pipeline settings
         * @return the part to be stored in ACQ_SetupPipeJobSettings
         */
        SETUP_PIPE_JOB_SETTINGS_TYPE getSetupPipeJobSettings(void) const;


        /**
         * get the part for scan pipeline settings
         * @return the part stored in ACQ_ScanPipeJobSettings
         */
        SCAN_PIPE_JOB_SETTINGS_TYPE getScanPipeJobSettings(void) const;


        /**
         * get the part for Rx Filter Default Options
         * @return the part stored in ACQ_RxFilterDefaultOptions
         */
        RX_FILTER_DEFAULT_OPTIONS getRxFilterDefaultOptions(void) const;


        /**
         * get the part for Rx Filter Settings
         * @return the part stored in ACQ_RxFilterSettings
         */
        RX_FILTER_SETTING getRxFilterSettings(void) const;


        /**
         * get the part for Rx Filter Info
         * @return the part stored in ACQ_RxFilterInfo
         */
        RX_FILTER_INFO getRxFilterInfo(void) const;


        /**
         * Get the size of a single scan (echo) acquired with one 
         * scan command in a pulse program.
         * @return scanSize field
         */
        int getScanSize(void) const { return scanSize_; };


        /**
         * Get the number of blocks acquired in Setup mode
         * @return transActionBlocks field
         */
        int getTransactionBlocks(void) const;


        /**
         * Set the number of blocks acquired in Setup mode
         * @param int transActionBlocks
         */
        void setTransactionBlocks(const int transactionBlocks) {transactionBlocks_ = transactionBlocks;};


        /**
         * check the current scantype and deliver the corresponding 
         * number of accum scans
         * @return n Accum Scans dependent on mode
         */
        int getNAccumScansNow(void) const;


        /**
         * Set number of scans after averaging
         * @param int nAccumScans
         */
        void setNAccumScans(const int nAccumScans) {nAccumScans_ = nAccumScans;};

        /**
         * Set the number of blocks transfered in Setup mode after averaging
         * @param int nAccumScans
         */
        void setSetupNAccumScans(const int nSetupAccumScans) {setupAccumScans_ = nSetupAccumScans;};



        /**
         * Get the total number of scans to be expected in acquisition
         * Must be divisible by NR
         * @return nTotalScans field
         */
        int getNTotalScans(void) const { return nTotalScans_; };


        /**
         * Set total number of scans to be expected in acquistion
         * Must be divisible by NR
         * @param int nTotalScans 
         */
        void setNTotalScans(const int nTotalScans) {nTotalScans_ = nTotalScans; };


        /**  
         * return the current data mode 
         *   STORE_discard,
         *   STORE_raw,
         *   STORE_processed
         * @return whether data is store at all or before / after averaging
         */
        SCAN_STORE_DATA_MODE_TYPE getStoreDataMode(void) const { return storeDataMode_; };

        void setStoreDataMode(SCAN_STORE_DATA_MODE_TYPE storeDataMode) {storeDataMode_ = storeDataMode; }; 


        /**
         * return the current data format
         *  STORE_32bit_signed,
         *  STORE_64bit_float
         * @return whether data is stored as integer or float
         */
        SCAN_STORAGE_DATA_TYPE getStorageDataType(void) const { return storageDataType_;};

        void setStorageDataType(SCAN_STORAGE_DATA_TYPE storageDataType) {storageDataType_ = storageDataType; };

        /**
         * return the current pipeline mode, i.e. either
         *   PIPELINE_processed (default)
         *   PIPELINE_raw
         * @return the selected pipeline mode
         */
        SCAN_PIPELINE_MODE_TYPE getPipelineMode(void) const
        {
            return pipelineMode_;
        }
        /**
         *  update the current pipeline mode
         */
        void setPipelineMode(const SCAN_PIPELINE_MODE_TYPE pipelineMode)
        {
            pipelineMode_ = pipelineMode;
        }
        
        /**
         * Get the bit mask of which scan tag indeces should be stored
         * @return the bit mask
         */
        const std::bitset<MAX_N_SCAN_TAGS>& getStoreScanTags() const { return storeScanTags_; };
        /**
         * Get the modifiable bit mask of which scan tag indeces should be stored
         * @return the modifiable bit mask
         */
        std::bitset<MAX_N_SCAN_TAGS>& getStoreScanTags() { return storeScanTags_; };
        
        /** 
         * Display mode is either
         *   DISPLAY_off,
         *   DISPLAY_each_scan,
         *   DISPLAY_each_accumulation,
         *   DISPLAY_each_pe_step
         * @return current display mode
         */
        SCAN_DISPLAY_MODE_TYPE getDisplayMode(void) const ;//{return displayMode_; };

        void setDisplayMode(SCAN_DISPLAY_MODE_TYPE displayMode) {displayMode_ = displayMode; };

        /** 
         * Display mode is either
         *   DISPLAY_off,
         *   DISPLAY_each_scan,
         *   DISPLAY_each_accumulation,
         *   DISPLAY_each_pe_step
         * @return current setup display mode
         */
        SCAN_DISPLAY_MODE_TYPE getSetupDisplayMode(void) const ;//{return setupDisplayMode_; };

        void setSetupDisplayMode(SCAN_DISPLAY_MODE_TYPE setupDisplayMode) {setupDisplayMode_ = setupDisplayMode; };


        /**
         * Check if group delay will be compensated.
         * @return bool
         */
        bool getCompensateGrpDelay(void) const { return compensateGrpDelay_; };


        /**
         * Sets group compensation flag.
         * @param bool
         */
        void setCompensateGrpDelay(const bool flag) { compensateGrpDelay_ = flag; };
        
        /**
         * Check if a short filter should be used.
         * @return bool
         */
        bool getForceShortFilter() const { return this->forceShortFilter_; };


        /**
         * Sets group compensation flag.
         * @param bool
         */
        void setForceShortFilter(const bool flag) { this->forceShortFilter_ = flag; };


        /**
         * Check if logging of time stamps is (de-)activated
         * @return timestamp logging mode
         */
        SCAN_LOG_TIMESTAMP_TYPE getLogTimeStamp(void) const { return logTimeStamp_; };


        /**
         * (De-)activates logging of time stamps.
         * @param bool
         */
        void setLogTimeStamp(const SCAN_LOG_TIMESTAMP_TYPE logTimeStamp) { logTimeStamp_ = logTimeStamp; };


        /**
         * Get the accumulation mode (average/accum/none) for the 
         * job
         * @return accumMode field
         */
        SCAN_ACCUM_MODE_TYPE getAccumMode(void) const { return accumMode_; };


        /**
         * Sets accumulation mode.
         * @param ACCUM_none or ACCUM_add or ACCUM_average.
         */
        void setAccumMode(const SCAN_ACCUM_MODE_TYPE accumMode) { accumMode_ = accumMode; };


        /**
         * Get the inner weight list in string form included in curly brackets
         * and separated by commas. May be used in filter parameters.
         * @return std::string with the list to be used as filter parameter
         */
        std::string getInnerAverageListString(void) const;


        /**
         * Get number of inner accumulation weights
         * @return number of inner accumulation weights
         */
        int getInnerAverageListSize(void) const { return innerAccumWeights_.size(); };


        /**
         * Get size of the accumulation loop to be used as number of objects
         * in the first (inner) accumulation cascade.
         * @return size of accumulation loop
         */
        int getNumInnerObjects(void) const { return innerLoops_;};


        /**
         * copy inner accumulation weight vector to given address
         * @todo may add second parameter with size
         * @param averageWeights destination for weight vector 
         *        must have sufficient size!
         */
        void copyInnerAccumWeights(int* averageWeights);


        /**
         * copy Outer accumulation weight vector to given address
         * @todo may add second parameter with size
         * @param averageWeights destination for  weight vector
         *        must have sufficient size!
         */
        void copyOuterAccumWeights(int* averageWeights);


        /**
         * Get the outer weight list in string form included in curly brackets
         * and separated by commas. May be used in filter parameters.
         * @return std::string with the list to be used as filter parameter
         */
        std::string getOuterAverageListString(void) const;


        /**
         * Get number of outer accumulation weights
         * @return number of outer accumulation weights
         */
        int getOuterAverageListSize(void) const { return outerAccumWeights_.size(); };


        /**
         * Get size of the accumulation loop to be used as number of objects
         * in the second (outer) accumulation cascade.
         * @return size of accumulation loop
         */
        int getNumOuterObjects(void) const { return outerLoops_; };


        /**
         * @return dual logarithm of data after filtering
         */
        double getDataRange(void) const {return dataRange_; };


        /**
         * Check whether inner averaging is active, 
         * i.e. at least one weight > 1 defined.
         * @return inner averaging summary
         */
        bool hasInnerAverage(void) const;


        /**
         * Check whether outer averaging is active, 
         * i.e. at least one weight > 1 defined.
         * @return outer averaging summary
         */
        bool hasOuterAverage(void) const;


        /**
         * test for averaging
         * @return hasInnerAverage() || hasOuterAverage()
         */
        bool hasAveraging(void) const {return hasInnerAverage() || hasOuterAverage(); };


        /**
         * Calculate the maximum value to expected after averaging in the 
         * current scan mode
         * @return maximum value to occur in display
         */
        double getMaxDisplayValue(void) const;

//ChannelNumber
        /**
         * Get the RF channel number (1-8). 0 designates a non active job
         * @return number of RF-channel defined for job
         */
        int getChanNum(void) const {return chanNum_; };

        /**
         * Set RF channel number (1-8). 0 deactivates acquisition
         * @param int (0-8)
         */
        void setChanNum(int chanNum) {chanNum_ = chanNum; };

//Title:
        /**
         * @return job title
         */   
        std::string getTitle(void) const {return title_; }

        /**
         * Assigns job title to job specified
         * @param std::string
         */   
        void setTitle(const std::string title) {title_ = title; }


//ScanSize:
        /**
         * Assign scan size to job specified
         * @param int
         */
        void setScanSize(const int scanSize) {scanSize_ = scanSize; };
        

        /**
         *
         * @return size of single scan
         */
        int getScanSize(void) {return scanSize_; };

//Sweepwidth:
        /**
         * @return sweep width
         */
        double getSwh(void) const {return swh_; };

        /**
         * Assign sweep width to job specified
         * @param double
         */
        void setSwh(const double swh) {swh_ = swh; };

//Dummy Scans:
        /**
         * Get number of dummy scans sett for this job
         * @return int dummyScans
         */
        int getDummyScans(void) const {return dummyScans_; };
        
        /**
         * Set number of dummy scans sett for this job
         * @param int dummyScans
         */
        void setDummyScans(const int dummyScans) {dummyScans_ = dummyScans; };

//ReceiverGain
        /**
         * Get receiver gain for this job
         * @return double receiverGain
         */
        double getReceiverGain(void) const {return receiverGain_; };

        /**
         * Set receivergain for this job
         * @param double receiverGain
         */
        void setReceiverGain(const double receiverGain) {receiverGain_ = receiverGain; };

        void setNormalizeMode(const SCAN_NORMALIZE_TYPE normalizeMode) {normalizeMode_ = normalizeMode; };
        
        /**
         * check, whether normalization is requested
         * @return 
         */
        bool mustNormalize(void) const {return normalizeMode_ == NORMALIZE_divide_by_4rg; };

        /** clear loops, to allow to work on existing jobs */
        void clearLoops(void);
        
        /**
         * Append loop structure for size calculation
         * @param loop
         * @param isAveraging
         * @param isSetup
         */
        void appendLoop( const int loop,
                         LoopDescription firstArg = LoopNone,
                         LoopDescription secondArg = LoopNone
                       );


        /**
         * Append list of averages 
         * @param looplist pointer to list data
         * @param size  # of entries
         * @param firstArg first option
         * @param secondArg second option
         */
        void appendLoopList( const int*         looplist, 
                             const unsigned int size,
                             LoopDescription    firstArg  = LoopNone,
                             LoopDescription    secondArg = LoopNone
                           );


        /**
         * Append list of averages
         * @param looplist pointer to list data
         * @param size # of entries
         * @param dummyScans dummyscan loop
         * @param firstArg first option
         * @param secondArg second option
         */
        void appendLoopList( const int*         looplist, 
                             const unsigned int size, 
                             const int          dummyScans, 
                             LoopDescription    firstArg  = LoopNone, 
                             LoopDescription    secondArg = LoopNone
                           );

        /**
         * get the size of stored data points
         * @return the size in bytes
         */
        size_t getSizeOfStorageDataTypeInBytes() const;
        
        /**
         * get the size of single scan
         * @return the size in bytes
         */
        size_t getSizeOfStoredScanInBytes() const;
        
        /**
         * calculate the necessary disk space
         * @return the data size in bytes
         */
        size_t getSizeOfStoredDataInBytes() const;

        friend std::ostream& operator<<(std::ostream& os, const JobDescription &jobs);


    private:
        void deriveParsFromLoops(void);

        void appendLoopOrList(const int       loop,
                              const int*      looplist,
                              const int       size,
                              const int       dummyScans,
                              LoopDescription firstArg,
                              LoopDescription secondArg);

        struct SingleLoop
        {
            int loop;
            std::vector<int> loopList;
            int ds;
            bool average;
            bool setup;
            SingleLoop():loop(0),loopList(),ds(0),average(false),setup(false){};
        };        
        std::vector<SingleLoop> loops_;

        // fields corresponding to ACQ_jobs
        int                       scanSize_;          /// size of single scan/echo : real valued points
        int                       transactionBlocks_; /// number of scans in setup transaction
        int                       dummyScans_;        /// number of dummy scans to be dropped
        int                       nTotalScans_;       /// total number of scans to be expected
        double                    receiverGain_;      /// desired receiver gain for job
        double                    swh_;               /// sweepwidth in Hz for the job
        int                       nStoredScans_;      /// number of eventually acquired scans
        int                       chanNum_;           /// Rf channel on which the job runs: 0 is inactive job
        std::string               title_;             /// text token to represent a job (w/o whitespace)

        // Pipeline Setting Parameters ACQ_ScanPipeJobDescription
        SCAN_STORE_DATA_MODE_TYPE storeDataMode_;     /// select data storage mode
        SCAN_STORAGE_DATA_TYPE    storageDataType_;   /// select data format for storage
        SCAN_NORMALIZE_TYPE       normalizeMode_;      /// define normalization of data
        SCAN_DISPLAY_MODE_TYPE    displayMode_;       /// switch display
        SCAN_LOG_TIMESTAMP_TYPE   logTimeStamp_;      /// flag to select timestamp logging
        SCAN_ACCUM_MODE_TYPE      accumMode_;         /// select accumulation mode
        int                       nAccumScans_;       /// expected number of scans after accumulation
        int                       nStoredAccumScans_; /// actual number of stored accumulated scans
        int                       innerLoops_;        /// size of loop for inner accumulation
        std::vector<int>          innerAccumWeights_; /// weights for inner accumulation loop
        int                       outerLoops_;        /// size of loop for outer accumulation
        std::vector<int>          outerAccumWeights_; /// weights for outer accumulation loop
        SCAN_PIPELINE_MODE_TYPE   pipelineMode_;      /// transfer raw data to pipeline
        std::bitset<MAX_N_SCAN_TAGS> storeScanTags_;  /// mask of scan tag indeces to be stored

        // setup Settings
        SCAN_DISPLAY_MODE_TYPE    setupDisplayMode_;  /// display mode for setup
        int                       setupAccumScans_;   /// number of scans in setup mode
        
        // Filter Info Parameters ACQ_RxFilterDefaultOptions
        bool                      compensateGrpDelay_;/// flag to activate group delay compensation
        bool                      forceShortFilter_;  /// flag to force use of a short filter

        // Filter Parameters ACQ_RxFilterSettings
        HW_FILTER_TYPE            hwFilterType_;      /// hardware (FPGA) filter selector
        SW_FILTER_TYPE            swFilterType_;      /// software (accumctrl) filter selector
        int                       hwDecim_;           /// hardware decimation
        int                       swDecim_;           /// software decimation
        int                       hwScanShift_;       /// hardware filter specific group delay
        int                       swScanShift_;       /// software filter specific group delay
        SW_DATA_EXTENSION         swDataExtension_;   /// mode for group delay handling
        int                       backPrediction_;    /// flag to activate backprediction

        // Filter Info Parameters ACQ_RxFilterInfo
        double                    groupDelay_;        /// effective group delay
        double                    compensatedGrpDelay_;/// group delay after compensation
        double                    waitTime_;          /// requested recycling delay
        double                    dynamicRange_;      /// signal to noise floor ratio for filter
        double                    dataRange_;         /// digital range for output data
        bool                      setupMode_;         /// flag whether setup mode!
    };



/** 
 *  The <code> JobParameters </code> class is an access wrapper, to 
 *  initialize all job parameters from a parameter space. 
 *  It allows to tell, how many jobs are defined, whether a given index
 *  refers to a used job or not etc.
 */


    class DLL_INTERFACE JobParameters
    {

    public:
        /**
         * Create JobParameters object from parameter interface.
         * This allows usage in overlays as well as in native code.
         * 
         * If an object is created with a parameter reference, it
         * will be automatically synchronized and changes will be 
         * written back on destruction.
         * 
         * @param ps reference to 
         */
        JobParameters(const Parx::Common::ParamInterface &ps);
        /**S
         * Create JobParameters object from parameter interface.
         * This allows usage in overlays as well as in native code.
         * @param pointer to Parameter interface
         */
        JobParameters(const Parx::Common::ParamInterface* ps);
        
        /** default constructor: creates empty job parameter interface 
         * 
         * If no parameter interface is given during the constructor,
         * reading from and writing to a parameter space must be
         * done manually using the initFromPS and writeToPS methods,
         * which can take a parameter space id as parameter
         */
        JobParameters();
        
        /** write back parameters, if initialized from PS */
        ~JobParameters();
        // do not implement copy constructor / assignment operator!    
        JobParameters(const JobParameters & obj) = delete;
        JobParameters& operator=(const JobParameters & obj) = delete;


        /**
         * This function returns true, when a job index refers to a well 
         * defined job, i.e. it is less than the size of the job array.
         * A channel >= 0 also must be set. Otherwise the job may be ignored.
         * @param ndx
         * @return 
         */
        bool isValidIndex(const int ndx);


        /**
         * @TODO: if we implement arrays with invalid entries, we will need 
         *        a method to return the number of valid jobs as well!
         * @return the size of the arrays 
         */
        int getNumJobs(void);


        /**
         * initialize the parameter structure from the parameter interface 
         * given in the constructor
         * @param psAccess parameter access object
         */
        void initFromPs(const Parx::Common::ParamInterface* psAccess);

        /**
         * Get the job description defined by the index in the parameters
         * @param ndx  position within job array
         * @return job description class
         */
        JobDescription& getJob(const int ndx);


        /**
         * Get the job description defined by the title in the parameters
         * Throws a PvStdExeption, when no job is found
         * @param title 
         * @return job description class
         */
        JobDescription& getJob(const std::string& title);

        /** Get index of a job
         * 
         * @param job title
         * @return current index in the job arrays 
         *         -1 if not existent
         */
        int getIndex(const std::string& title);
        /**
         * set job parameters defined by job description at given entry in the
         * ACQ job parameters. Will cause an exception, if the index exceeds 
         * the parameter table.
         * @param job job description class
         * @param ndx index to be overwritten
         */
        void setJobAtIndex(const JobDescription& job, const unsigned int ndx);
        
        /** remove a job identified by its title from array,
         *  do nothing when the job was not found
         * 
         *  @param title of job
         */
        void removeJobIfExists(const std::string& title);

        /** remove all jobs
         * 
         *  use case: when a protocol with multiple active jobs calls a 
         *  different method for adjustment which itself does not support 
         *  those jobs then they need to be removed from the adjusting 
         *  method. 
         */
        void removeAllJobs(void);

        /* get a reference to a job - either existing,
         * or create anew
         * @param title
         * @return reference to job
         */
        JobDescription* getOrCreateJob(const std::string& title);
        
        /**
         * replace a job defined by the title or add as a new job
         * @param job
         */
        void addReplaceJob(const JobDescription& job);

        /**
         * Write the job description back to parameter space.
         * Parameter relations will be called for ACQ_jobs, not for other parameters.
         * The reason is, that this way filter parameters will be derived from the
         * bandwidth settings.
         * @parameter psAccess parameter interface object to allow PARX access         
         */
        void writeToPs(const Parx::Common::ParamInterface* psAccess);
        void writeToPs(const Parx::Common::ParamInterface& psAccess);

        /**
         *  Write back to parameter space, using the parameter interface
         *  object given in the constructor. 
         */
        void writeToPs(void);

        /**
         *  start iterator (for range based for-loops)
         */
        std::vector<JobDescription>::const_iterator begin() const { return jobs_.begin(); };
        std::vector<JobDescription>::iterator begin() { return jobs_.begin(); };
        
        /**
         *  end iterator (for range based for-loops)
         */
        std::vector<JobDescription>::const_iterator end() const { return jobs_.end(); };
        std::vector<JobDescription>::iterator end() { return jobs_.end(); };
        
        /**
         * friend declaration of output operators
         */
        friend std::ostream& operator<<(std::ostream& os, const JobParameters &job);


    private:
        /// pointer to parameter interface
        const Parx::Common::ParamInterface *psAccess_;
        /// holds the actual job descriptions
        std::vector<JobDescription> jobs_;
    };


DLL_INTERFACE std::ostream& operator<<(std::ostream& os, const JobParameters &job);
DLL_INTERFACE std::ostream& operator<<(std::ostream& os, const JobDescription &jobs);
} // namespace ReadParx


/**
 * Macro to hide access to parameter space in method code
 */
#define JOBPARAMETERS(parameter) \
  Parx::Relation::RelsParamInterface localInterface; \
  ReadParx::JobParameters parameterAccess(localInterface); \
  ReadParx::JobParameters *parameter = &parameterAccess;
  
#define JOBDESCRIPTION ReadParx::JobDescription*

#endif 


#endif
