/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2006-2010
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef LIBMPI_CALIBRATIONDATABASE_HH
#define LIBMPI_CALIBRATIONDATABASE_HH

#include "mpi/PvTypes/mpiTypes.h"

#include <vector>
#include <string>

#include "libMpiUtil/DLLMacro.h"

namespace MpiUtil
{

/**
 * CalibrationParameters holds all acquisition information
 * that characterizes an MPI calibration measurement
 */
struct DLL_INTERFACE CalibrationParameters
{
    /**
     * Default Constructor
     */
    CalibrationParameters();
    
    /**
     * Copy constructor
     */
    CalibrationParameters(const CalibrationParameters &);
    
    /**
     * Assignment operator.
     */
    CalibrationParameters & operator = (const CalibrationParameters &);
    
    /**
     * Constructor, which parses the calibration parameters from
     * a given string
     * @param str string representation of calibration parameters
     */
    CalibrationParameters(const std::string & str);
    /**
     * init the calibration parameters
     */
    void initCalibrationParameters();
    /**
     * Convert calibration parameters to string
     * @return string representation of calibration parameters
     */    
    std::string toString();
    /**
     * Convert calibration parameters to MpiSystemFunctionType
     * @return struct of system fucntion attributes, used as representation in ParxParameter
     */
    MpiSystemFunctionType toMpiSystemFunctionType();
    /**
     * comparison operator, check if two sets of calibration parameters
     * are equal. Note that not all parameters have to be exactly equal.
     * 
     * @param A calibration parameters to compare to
     * @return true if parameters equal, false otherwise
     */
    bool operator==(const CalibrationParameters& A) const;
    /**
     * Check if the system function request, described by an string, match to the
     * calibtration parameters
     * 
     * @param str string of the system function selection request
     * @return YesNo if the calibration parameters match to the system function selection request
     */
    YesNo checkMatching(const std::string & str) const;
    /**
     * ordering operator, check if the id is smaller than the id of of another 
     * calibration parameters A. 
     * 
     * @param A calibration parameters to compare to
     * @return true if id is smaller, false otherwise
     */
    bool operator<(const CalibrationParameters& A) const;
    /**
     * Check, whether the dataset exists on disk
     * @return true if dataset exists, false otherwise
     */
    bool exists();    
    /**
     * check if two sets of calibration parameters have equal tracer properties
     */
    bool checkTracer(CalibrationParameters A);
    /**
     * check the consistence of the calibration parameters with the calibration Parameters in checkParameter
     */
    MpiRecoInfoSystemFunction checkConsistence(CalibrationParameters checkParameter);
    
    /** imaging dimension */
    unsigned int dim;
    /** field of view */
    double fov[3];
    /** center field of view */
    double fovCenter[3];
    /** sizes of the image matrix */
    unsigned int sizes[3];
    /** drive-field amplitudes */
    double driveField[3];
    /** focus-field value */
    double focusField[3];    
    /** selection field gradient */
    double selectionField;
    /** tracer material */
    std::string tracer; 
    /** batch of the tracer */
    std::string batch;    
    /** path of the dataset */
    std::string datasetPath;
    /** identifier of the calibration measurement */
    unsigned int id;
    /** bandwith */
    double bandwidth;
    /** active receivers */
    unsigned int activeReceivers[3];
    /** orientation */
    unsigned int orientation;
    /** drive field control mode */
    unsigned int driveFieldControlMode;
    
    //FIXME remove
    
    /** number of receivers */
    unsigned int nrReceivers;
    /** wordSize */
    unsigned int wordSize;
    /** background */
    bool background;
    
};


/**
 * CalibrationDatabase administrates all MPI calibration datasets
 * in ParaVision.
 */
class DLL_INTERFACE CalibrationDatabase
{
    
public:
    /**
     * Default Constructor
     */
    CalibrationDatabase();
    /**
     * Default Destructor
     */
    ~CalibrationDatabase();
    /** streaming operator */
    friend std::ostream &operator<<(std::ostream &os, CalibrationDatabase &database);
    /** import the database (currently from a file) */
    void importDatabase();
    /** export the database (currently to a file) */
    void exportDatabase();
    /** clear database */
    void clear();
    /**
     * add a dataset to the calibration database
     * @param params the dataset to be added to the calibration dataset
     */
    void addDataset(CalibrationParameters params);
    /**
     * get all datasets, which equal params
     * @param params dataset, which has to be compared to
     * @return a vector of dataset parameters which equal params
     */
    std::vector<CalibrationParameters> getDatasets(CalibrationParameters params);
    /**
     * get all datasets, with equal tracer properties
     * @param params dataset, which has to be compared to
     * @return a vector of dataset parameters with equal tracer properties
     */
    std::vector<CalibrationParameters> getDatasetsTracer(CalibrationParameters params);
    /**
     * get a singleton object of the calibration database
     * 
     * @return pointer to the calibration database 
     */
    static CalibrationDatabase* getCalibrationDatabase();
    
private:
    /** filename of the calibration database */
    std::string filename_;
    /** map of all calibration parameters */
    std::vector<CalibrationParameters> datasets_;
    /** pointer to the singleton object of the calibration database */
    static CalibrationDatabase *database_;
};

/**
 * output stream for calibration database
 * 
 * @param os output stream
 * @param database calibration database to be output
 * @return the extended output stream
 */
DLL_INTERFACE std::ostream &operator<<(std::ostream &os, MpiUtil::CalibrationDatabase &database);


} // namespace MpiUtil



#endif
