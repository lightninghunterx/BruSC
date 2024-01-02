//==============================================================================
//
// Basic functions for ISA fit models
//
// Copyright (c) 2015-2018
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//
//==============================================================================

#ifndef MODELBASE_HH
#define MODELBASE_HH

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// interface of class ModelBase
//------------------------------------------------------------------------------

/**
 * @defgroup ISA_MODEL_BASE ISA Model Base
 */

/** Basic functions for ISA model implementation.
 * 
 * These functions can be used in the model implementation sections. Not all
 * functions can be used in all sections. For each function the signature string
 * <code>[DRPIFA]</code> indicates for which section the function can be used.
 * 
 * For example a signature <code>[DR\-\-FA]</code> indicates that the function
 * can be used in the declaration, requirements, function evaluation and 
 * analysis sections, but not in the parameter setup and initialization
 * sections.
 *
 * The basic functions are divided into the following categories:
 * <ul>
 * <li>General properties</li>
 * <li>Dataset interaction</li>
 * <li>Model Parameters</li>
 * <li>Data points</li>
 * <li>Optimization</li>
 * <li>Miscellaneous</li>
 * </ul>
 */
class ModelBase {
public:

    virtual ~ModelBase() {
    };

    // =========================================================================

    /** @name General properties
     * Functions relating to general properties of the model.
     * @{*/

    /** Set the model description and short description.
     *
     * Sets a textual description of the fit model as part of the model
     * declaration.
     *
     * Sections: <code>[DR\-\-\-\-]</code>
     *
     * @param shortDescription  short model description, e.g. "T2 Relaxation"
     * @param description       a more detailed model description, should
     *                          include a formula, e.g., "Logarithmic T2
     *                          Relaxation Decay: y = ln (C * exp(-t/T2))"
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setDescription(const char* shortDescription,
            const char* description) = 0;

    /** Set the ISA variable.
     *
     * Sets the name of the Parx parameter that will be used as the ISA
     * variable, i.e. the values for the abscissa, e.g. "PVM_EchoTime" for
     * relaxation fits to a multi-echo experiment. If the ISA variable is not
     * set, the abscissa values will take on the values 0, 1, ..., N-1, where N
     * is the number of frames in the ISA frame group, or (if the ISA frame
     * group is also empty) the total number of frames in the dataset.
     *
     * If the ISA variable is set, hasParxValue(ISA variable) is an implicit
     * requirement for a dataset.
     *
     * Sections: [DR\-\-\-\-]
     *
     * @param variable          name of the ISA variable
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setIsaVariable(const char* variable) = 0;

    /** Set unit for the ISA variable.
     *
     * Set physical unit for the independent variable, using the UCUM unit
     * standard, for example "ms".
     *
     * Sections: [DR\-\-\-\-]
     *
     * @param unit              unit for the ISA variable.
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setIsaVariableUnit(const char* unit) = 0;

    /** Sets the ISA frame group.
     *
     * Sets the name of the Visu frame group that defines the category of frames
     * of an image series to be used in image sequence analysis with this model.
     * For example, "FG_ECHO" for relaxation fits to a multi-echo experiment. If
     * the ISA frame group is not set, the frame group is used that lists the
     * ISA variable as dependent variable. If none is found, all frames of the
     * dataset are used.
     *
     * If the ISA frame group is set, lenFrameGroup(ISA frame group) > 0 is an
     * implicit requirement for a dataset.
     *
     * Sections: [DR\-\-\-\-]
     *
     * @param frameGroupName    name of the ISA frame group
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setIsaFrameGroup(const char* frameGroupName) = 0;

    /** Set label for abscissa.
     *
     * Set a label for the abscissa of the fit function plot.
     * Example: "Echo time"
     *
     * Sections: [DR\-\-\-\-]
     *
     * @param abscissaLabel     label for the abscissa
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setAbscissaLabel(const char* abscissaLabel) = 0;

    /** Set the abscissa display range.
     *
     * Increases the range of abscissa values for which the model is evaluated
     * in the plot. By default this range is equal to the range of values of the
     * ISA variable. This function can be used to lower the minium and to
     * increase the maximum of the abscissa range.
     *
     * The minimum (maximum) of the range can be set as a direct value, or
     * relative to the smallest (largest) abscissa value according to the ISA
     * variable in the dataset or as set by setAbscissaValues(). For example,
     * for a multi-echo experiment with echo times {3, 10, 20, 30, 50} ms, the
     * arguments (0.0, 1.2, false, true) will set the range of the abscissa
     * in the plot to [0, 1.2 * 50] ms = [0, 60] ms.
     *
     * Sections: [DRP\-\-\-]
     *
     * @param min               minimum of abscissa range
     * @param max               maximum of abscissa range
     * @param isMinRelative     indicates whether min is a relative value.
     *                          Optional, default is false.
     * @param isMaxRelative     indicates whether max is a relative value
     *                          Optional, default is false.
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setAbscissaRange(double min, double max,
            bool isMinRelative = false, bool isMaxRelative = false) = 0;

    /** Set label for ordinate.
     *
     * Set a label for the ordinate of the fit function plot. For example:
     * "Intensity"
     *
     * Sections: [DR\-\-\-\-]
     *
     * @param ordinateLabel     label for the ordinate
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setOrdinateLabel(const char* ordinateLabel) = 0;

    /** Set unit for ordinate.
     *
     * Set a unit for the ordinate of the fit function plot, using the UCUM unit
     * standard. For example "[arb'U]".
     *
     * Sections: [DR\-\-\-\-]
     *
     * @param ordinateLabel     label for the ordinate
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setOrdinateUnit(const char* ordinateLabel) = 0;

    /** Return model name.
     *
     * Sections: [DRPIFA]
     *
     * @return                  the name of the model
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual const char* getName() = 0;

    /** Return short model description.
     *
     * Sections: [DRPIFA]
     *
     * @return                  the short description of the model
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual const char* getShortDescription() = 0;

    /** Return model description.
     *
     * Sections: [DRPIFA]
     *
     * @return                  the model description
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual const char* getDescription() = 0;

    /** Return the minimum of the abscissa display range.
     *
     * If the minimum was declared as relative to the smallest abscissa value,
     * the return value is equal to the product of smallest abscissa value and
     * the relative minimum.
     *
     * Sections: [-RPIFA]
     *
     * @return                  minimum of the abscissa display range
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual double getAbscissaRangeMin() = 0;

    /** Return the maximum of the abscissa display range.
     *
     * If the maximum was declared as relative to the largest abscissa value,
     * the return value is equal to the product of largest abscissa value and
     * the relative maximum.
     *
     * Sections: [-RPIFA]
     *
     * @return                  maximum of the abscissa display range
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual double getAbscissaRangeMax() = 0;

    /**
     * Specify an auxiliary ROI required for the model.
     *
     * The data in an auxiliary ROI is accessible in sections I, F and A,
     * see getAuxiliaryROIValues() and getAuxiliaryROISTDs(). Repeated calls of
     * this function will add an auxiliary ROI or replace an existing one.
     *
     * Sections: [DRP\-\-\-]
     *
     * @param auxRoiName        name for auxiliary ROI required for this model.
     * @param sameSlice         If true, the auxiliary ROI must be placed in the
     *                          same 2D frame as the ROI for analysis and thus
     *                          there may be multiple instances of the auxiliary
     *                          ROI with the same name in the dataset. If false,
     *                          the auxiliary ROI can be placed in any 2D frame.
     *                          It is then unique in the dataset and valid for
     *                          analysis ROIs in the whole dataset.
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setAuxiliaryROI(const char* auxRoiName, bool sameSlice) = 0;

    /**
     * Declare this model to be a Service model (only available in service mode)
     *
     * Sections: [D\-\-\-\-\-]
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setServiceModel() = 0;

    /** Indicates whether or not the model is a service model
     *
     * Sections: [-RPIFA]
     *
     * @return                  whether the model is a service model
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual bool isServiceModel() = 0;

    /**@} General properties */

    // =========================================================================

    /** @name Dataset interaction
     * Functions for accessing dataset properties and specifying requirements.
     * @{*/

    /** Add a requirement.
     *
     * Add a boolean value to the set of requirements. Only if each requirement
     * is met (true), the model is suitable for a given dataset.
     *
     * Example:
     * @verbatim
       require(hasParxValue("A"));
       require(lenFrameGroup("B") > 5, "B must have more than 5 elements");
       @endverbatim
     *
     * is equivalent to
     *
     * @verbatim
       bool reqParam = hasParxValue("A");
       bool reqAbsc  = lenFrameGroup("B") > 5;
       require(reqParam && reqAbsc);
       @endverbatim
     *
     * Sections: [DR\-\-\-\-]
     *
     * @param requirement       evaluated requirement
     * @param errorMsg          optional error message describing the failure of
     *                          the requirement
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void require(bool requirement, const char* errorMsg = "") = 0;

    /** Is the dataset aqcuired with a given method name (MR-specific).
     *
     * checks if the dataset is acquired with method "methodName" or
     * "Bruker:methodName". If the strict flag is false, any method that
     * contains methodName will match. To be used with caution!
     *
     * Example:
     * <table>
     * <tr><th>method in dataset</th><th>methodName</th><th>return value for<br>strict==true</th><th>return value for<br>strict==false</th></tr>
     * <tr><td>FLASH           </td><td>FLASH</td><td>true </td><td>true</td></tr>
     * <tr><td>Bruker:FLASH    </td><td>FLASH</td><td>true </td><td>true</td></tr>
     * <tr><td>User:myFLASHtest</td><td>FLASH</td><td>false</td><td>true</td></tr>
     * <tr><td>Bruker:FAIR_RARE</td><td>RARE </td><td>false</td><td>true</td></tr>
     * <tr><td>Bruker:FAIR_RARE</td><td>FLASH</td><td>false</td><td>false</td></tr>
     * </table>
     *
     * Sections: [-RPIFA]
     *
     * @param methodName        name of the method
     * @param strict            method name must match "methodName" exactly.
     * @return                  whether dataset is acquired with this method
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual bool createdByMRmethod(const char* methodName, bool strict) = 0;

    /** Get length of a frame group.
     *
     * Returns the length of a frame group in the dataset. Can also be used to
     * check for the presence of a frame group, as a negative length is returned
     * if the frame group is not present.
     *
     * Sections: [-RPIFA]
     *
     * @param frameGroupName    name of the frame group
     * @return                  length of frame group if present, -1 otherwise
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual int lenFrameGroup(const char* frameGroupName) = 0;

    /** Check if a Parx parameter exists in the dataset and has a value.
     *
     * Sections: [-RPIFA]
     *
     * @param parxParameterName Parx parameter name
     * @return                  whether a Parx parameter with that name exists
     *                          in the dataset and has a value
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual bool hasParxValue(const char* parxParameterName) = 0;

    /** Get Parx string parameter.
     *
     * Returns the string in a given parameter name. If the given parameter is
     * not a string, an empty string is returned.
     *
     * Sections: [-RPIFA]
     *
     * @param parameterName     name of the parameter
     * @return                  string content
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual std::string PARX_get_string(const char* parameterName) = 0;

    /** Get total number of frames.
     *
     * Returns the total number of 2D frames in the dataset
     *
     * Sections: [-RPIFA]
     *
     * @return                  total number of 2D frames in dataset
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual int numTotal2dFrames() = 0;
    
    /** Get parx parameter space id for given dataset.
     *
     * @return                  parx parameter space id
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual int getPsId() = 0;

    /** Get procno path of dataset.
     *
     * Sections: [-RPIFA]
     *
     * @return                  procno path of dataset
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual const char* getProcnoPath() = 0;

    /** Indicates whether or not the model is suitable for the dataset
     * identified by psId_ and procnoPath_.
     *
     * <code>[-RPIFA]</code>
     *
     * All requirements that are imposed using require() up to the point where
     * this function is called (and only those) are taken into account.
     *
     * @return                  whether the model is suitable for this dataset
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual bool isSuitableForDataset() = 0;

    /**@} Dataset interaction */

    // =========================================================================

    /** @name Model Parameters
     * Functions relating to model parameters.
     * @{*/

    /** Set number of model parameters.
     *
     * Sets the number of model parameters to be used in the fit. The number
     * includes "output only" parameters, that are only calculated from other
     * parameters and not part of the expression for the model function value.
     * It also includes "fixed" parameters, that are part of the model, but not
     * varied in the fit procedure.
     *
     * Sections: [\-\-P\-\-\-]
     *
     * @param numParams         number of model parameters
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setNumParams(unsigned int numParams) = 0;

    /** Set name and optional description of model parameter.
     *
     * Specify a short parameter name, an optional description and an optional
     * unit. For example, name: "T1", description: "longitudinal relaxation time",
     * unit: "ms"
     *
     * Sections: [\-\-P\-\-\-]
     *
     * @param paramIndex        index of the parameter
     * @param name              short name of the parameter
     * @param desc              description of the parameter (optional, default:
     *                          equal to name)
     * @param unit              unit of the parameter in UCUM format, (optional,
     *                          default: no unit).
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setParamName(unsigned int paramIndex, const char* name,
            const char* desc = "", const char* unit = "") = 0;

    /** Set parameter default value.
     *
     * Set the value that is used for a parameter if a fit does not converge.
     * This value may be outside the parameter bounds.
     *
     * Sections: [\-\-P\-\-\-]
     *
     * @param paramIndex        index of the parameter
     * @param value             default value
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setParamDefault(unsigned int paramIndex, double value) = 0;

    /** Set parameter value.
     *
     * Set parameter to an initial value. If the value is outside the bounds
     * given for this parameter, it will be set to the closest value in the
     * allowed range.
     *
     * Sections: [\-\-PI-A] for regular parameters
     *
     * Sections: [\-\-PIFA] for output only parameters
     *
     * @param paramIndex        index of the parameter
     * @param value             parameter value
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setParamValue(unsigned int paramIndex, double value) = 0;

    /** Set function partial derivative with respect to a given parameter.
     *
     * As a part of the model evaluation, sets the first partial derivative of
     * the function with respect to a given parameter.
     *
     * Sections: [\-\-\-\-F-]
     *
     * @param paramIndex        index of the parameter
     * @param value             first partial derivative of fit model with
     *                          respect to the given parameter
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setParamDerivative(unsigned int paramIndex, double value) = 0;

    /** Set parameter to "output only".
     *
     * An "output only" parameter is calculated from other parameters and is not
     * part of the expression for the model function value.
     *
     * In addition, an "output only" parameter is set to "fixed". If the
     * parameter is set back to isOutputOnly=false, it will remain fixed, until
     * it is released using setParamFixed(paramIndex, false);
     *
     * Sections: [\-\-P\-\-\-]
     *
     * @param paramIndex        index of the parameter
     * @param isOutputOnly      whether the parameter is "output only"
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setParamOutputOnly(unsigned int paramIndex, bool isOutputOnly) = 0;

    /** Set parameter to "fixed" or "unfixed".
     *
     * A "fixed" parameter is part of the expression for the model function
     * value, but not varied during the fit procedure.
     *
     * Note: If a parameter is set to fixed/unfixed in setupParameters(), i.e.,
     * section "P", the user may override that value on the GUI. If set in
     * initParameters(), i.e., section "I", the "fixed" state of the parameter
     * is enforced during each parameter initialization and thus cannot be
     * changed in the GUI.
     *
     * Sections: [\-\-PI\-\-]
     *
     * @param paramIndex        index of the parameter
     * @param isFixed           whether the parameter is fixed
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setParamFixed(unsigned int paramIndex, bool isFixed) = 0;

    /** Set lower bound for parameter.
     *
     * The bound is enforced when setting the parameter value using
     * setParameterValue() and during the fit procedure.
     *
     * Sections: [\-\-PI\-\-]
     *
     * @param paramIndex        index of the parameter
     * @param value             lower bound
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setParamLowerBound(unsigned int paramIndex, double value) = 0;

    /** Set upper bound for parameter.
     *
     * The bound is enforced when setting the parameter value using
     * setParameterValue() and during the fit procedure.
     *
     * Sections: [\-\-PI\-\-]
     *
     * @param paramIndex        index of the parameter
     * @param value             upper bound
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setParamUpperBound(unsigned int paramIndex, double value) = 0;

    /** Simultaneously set parameter initial value and boundary values.
     *
     * Sections: [\-\-PI\-\-]
     *
     * @param paramIndex        index of the parameter
     * @param lowerBound        lower bound
     * @param value             parameter value
     * @param upperBound        upper bound
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setParamValueAndBounds(unsigned int paramIndex,
            double lowerBound, double value, double upperBound) = 0;

    /**
     * Set default display parameter.
     *
     * On startup, the ISA tool will display the parameter map for the default
     * display parameter as set in this function.
     *
     * Sections: [-RP\-\-\-]
     *
     * @param paramIndex        index of default display parameter
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setDefaultDisplayParam(unsigned int paramIndex) = 0;


    /** Return number of model parameters.
     *
     * Returns the number of parameters actually declared in section
     * setupParameters().
     *
     * Sections: [\-\-\-IFA]
     *
     * @return                  number of model parameters
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual unsigned int getNumParams() = 0;

    /** Return name of a specific parameter.
     *
     * Returns the name of a parameter as declared in section setupParameters().
     *
     * Sections: [\-\-\-IFA]
     *
     * @param paramIndex        index of parameter
     * @return                  name of parameter specified by index
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual const char* getParamName(unsigned int paramIndex) = 0;

    /** Return description of a specific parameter.
     *
     * Returns the description of a parameter as declared in section
     * setupParameters().
     *
     * Sections: [\-\-\-IFA]
     *
     * @param paramIndex        index of parameter
     * @return                  description of parameter specified by index
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual const char* getParamDesc(unsigned int paramIndex) = 0;

    /** Return unit of a specific parameter.
     *
     * Returns the unit of a parameter as declared in section setupParameters().
     *
     * Sections: [\-\-\-IFA]
     *
     * @param paramIndex        index of parameter
     * @return                  unit of parameter specified by index
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual const char* getParamUnit(unsigned int paramIndex) = 0;

    /** Return default value of a specific parameter.
     *
     * Returns the default value of a parameter as declared in section
     * setupParameters().
     *
     * Sections: [\-\-\-IFA]
     *
     * @param paramIndex        index of parameter
     * @return                  default value of parameter specified by index
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual double getParamDefault(unsigned int paramIndex) = 0;

    /** Return value of a specific parameter.
     *
     * Return the current value of a parameter. In the evaluate() section, the
     * parameter values are set by the fitting routine, and this function is
     * used to evaluate the function value for a given set of parameter values.
     *
     * Sections: [\-\-\-IFA]
     *
     * @param paramIndex        index of parameter
     * @return                  value of parameter specified by index
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual double getParamValue(unsigned int paramIndex) = 0;

    /** Returns whether a parameter was declared as "output only".
     *
     * An "output only" parameter is calculated from other parameters and is not
     * part of the expression for the model function value.
     *
     * Sections: [\-\-\-IFA]
     *
     * @param paramIndex        index of parameter
     * @return                  whether the parameter specified by index was
     *                          declared "output only".
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual bool getParamOutputOnly(unsigned int paramIndex) = 0;

    /** Returns whether a parameter was declared as "fixed".
     *
     * A "fixed" parameter is part of the expression for the model function
     * value, but not varied during the fit procedure.
     *
     * Sections: [\-\-\-IFA]
     *
     * @param paramIndex        index of parameter
     * @return                  whether the parameter specified by index was
     *                          declared "fixed".
     * @ingroup ISA_MODEL_BASE
     */
    virtual bool getParamFixed(unsigned int paramIndex) = 0;

    /**@} Model Parameters */

    // =========================================================================

    /** @name Data points
     * Functions for accessing data points.
     * @{*/

    /** Return number of data points.
     *
     * Returns the number of data points as determined from the length of
     * the ISA variable or ISA frame group. If none of these is set, the
     * number of data points is equal to the number of frames in the
     * dataset.
     *
     * Sections: [-RPIFA]
     *
     * @param uniqueAbscVals    return number of unique abscissa values only.
     *                          Parameter is optional, default = false
     * @return                  number of data points
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual unsigned int getNumDataPoints(bool uniqueAbscVals = false) = 0;

    /** Get all data values.
     *
     * Returns the ordinate values of the data to which the model will be
     * fitted.
     *
     * Sections: [\-\-\-IFA]
     *
     * @param onlyActive        only return the data values of active data
     *                          points. Optional, default is false.
     * @return                  data values of all data points
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual std::vector<double> getDataValues(bool onlyActive = false) = 0;

    /** Get all data values, sorted by abscissa value.
     *
     * Returns the ordinate values of the data to which the model will be
     * fitted, sorted by non-decreasing abscissa value.
     *
     * Sections: [\-\-\-IFA]
     *
     * @param onlyActive        only return the data values of active data
     *                          points. Optional, default is false.
     * @return                  sorted data values of all data points
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual std::vector<double> getSortedDataValues(bool onlyActive = false) = 0;

    /** Get a data value.
     *
     * Returns the ordinate value of one specific point of the data to which the
     * model will be fitted.
     *
     * Sections: [\-\-\-IFA]
     *
     * @param abscissaIndex     abscissa index
     * @return                  all data values
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual double getDataValue(unsigned int abscissaIndex) = 0;

    /** Get standard deviations of all data points.
     *
     * Returns the standard deviations of the data to which the model will be
     * fitted.
     *
     * Sections: [\-\-\-IFA]
     *
     * @param onlyActive        only return the data standard deviations of
     *                          active data points. Optional, default is false.
     *
     * @return                  all data standard deviations
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual std::vector<double> getDataSTDs(bool onlyActive = false) = 0;

    /** Get standard deviations of all data points, sorted by abscissa value.
     *
     * Returns the standard deviations of the data to which the model will be
     * fitted, sorted by non-decreasing abscissa value.
     *
     * Sections: [\-\-\-IFA]
     *
     * @param onlyActive        only return the data standard deviations of
     *                          active data points. Optional, default is false.
     *
     * @return                  all data standard deviations
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual std::vector<double> getSortedDataSTDs(bool onlyActive = false) = 0;

    /** Get standard deviation of one data point.
     *
     * Returns the standard deviation of one specific point of the data to which
     * the model will be fitted.
     *
     * Sections: [\-\-\-IFA]
     *
     * @return                  all data values
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual double getDataSTD(unsigned int abscissaIndex) = 0;

    /** Get values in a given auxiliary ROI.
     *
     * Returns the mean values inside a given auxiliary ROI for all abscissa
     * values (original order, as in the image sequence).
     *
     * Sections: [\-\-\-IFA]
     *
     * @param auxRoiName        name of the auxiliary ROI.
     *
     * @return                  data values in specified auxiliary ROI
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual std::vector<double> getAuxiliaryROIValues(const char* auxRoiName) = 0;

    /** Get standard deviations in a given auxiliary ROI.
     *
     * Returns the standard deviations inside a given auxiliary ROI for all
     * abscissa values (original order, as in the image sequence).
     *
     * Sections: [\-\-\-IFA]
     *
     * @param auxRoiName        name of the auxiliary ROI.
     *
     * @return                  standard deviations in specified auxiliary ROI
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual std::vector<double> getAuxiliaryROISTDs(const char* auxRoiName) = 0;

    /** Return a specific abscissa value.
     *
     * Returns an abscissa value. If an ISA variable is set, the abscissa values
     * are equal to the values of the ISA variable in the current dataset. If no
     * ISA variable is set, the abscissa values are either equal to
     * 0, 1, ..., N-1, or to the values set in setAbscissaValues().
     *
     * Note: The abscissa values do not necessarily increase for increasing
     * abscissa index. If an ISA variable is set, the sequence of values is
     * equal to the sequence of values in the Parx parameter. For access to
     * monotonically increasing abscissa values use getSortedAbscissaValue()
     *
     * Sections: [-RPIFA]
     *
     * @param abscissaIndex     index of (unsorted) abscissa value
     * @return                  abscissa value at specified index
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual double getAbscissaValue(unsigned int abscissaIndex) = 0;

    /** Return all abscissa values.
     *
     * See getAbscissaValue().
     *
     * Sections: [-RPIFA]
     *
     * @param onlyActive        only return the abscissa values of active
     *                          data points. Optional, default is false.
     *
     * @return                  (unsorted) abscissa values
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual std::vector<double> getAbscissaValues(bool onlyActive = false) = 0;

    /** Return a specific value from the sorted abscissa.
     *
     * Returns a value from the sorted abscissa (non-decreasing sort order).
     * If an ISA variable is set, the set of abscissa values is equal to the
     * set of values of the ISA variable in the current dataset. If no ISA
     * variable is set, the abscissa values are either equal to
     * 0, 1, ..., N-1, or to the values set in setAbscissaValues(), after
     * sorting.
     *
     * Sections: [-RPIFA]
     *
     * @param sortedAbscissaIndex index of value on the sorted abscissa
     * @return                    abscissa value at specified index
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual double getSortedAbscissaValue(unsigned int sortedAbscissaIndex) = 0;

    /** Return all sorted abscissa values.
     *
     * See getSortedAbscissaValue().
     *
     * Sections: [-RPIFA]
     * @param onlyActive        only return the abscissa values of active
     *                          data points. Optional, default is false.
     *
     * @return                  sorted abscissa values
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual std::vector<double> getSortedAbscissaValues(bool onlyActive = false) = 0;

    /** Explicitly set abscissa values.
     *
     * Explicitly set the values for the abscissa. This function can only be
     * used if no ISA variable is set, to replace the automatic abscissa values
     * 0, 1, ..., N-1. If the abscissa values are already determined by the
     * ISA variable, the values passed to setAbscissaValues are ignored.
     *
     * Sections: [\-\-PI\-\-]
     *
     * @param abscissaValues    new abscissa values
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setAbscissaValues(const std::vector<double>& abscissaValues) = 0;

    /** Explicitly set data values.
     *
     * Explicitly overwrite y-values of the data points.
     *
     * Sections: [\-\-\-I\-\-]
     *
     * @param dataValues        new ordinate values
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setDataValues(const std::vector<double>& dataValues) = 0;

    /** Explicitly set data standard deviations.
     *
     * Explicitly overwrite y-standard deviations of the data points.
     *
     * Sections: [\-\-\-I\-\-]
     *
     * @param dataSTDs          new standard deviations
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setDataSTDs(const std::vector<double>& dataSTDs) = 0;

    /** Return the number of active data points.
     *
     * Data points have an "active" flag that controls whether or not the
     * point is taken into account during the fit. This function returns the
     * number of active data points.
     *
     * Sections: [\-\-PIFA]
     *
     * @return                  number of active data points
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual unsigned int getNumActiveDataPoints() = 0;

    /** Return the "active" status of one given data point.
     *
     * Sections: [\-\-PIFA]
     *
     * @param abscissaIndex     index of data point on the abscissa
     * @return                  whether data point is active
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual bool getDataPointActive(unsigned int abscissaIndex) = 0;

    /** Return the "active" status of all data points.
     *
     * See getActiveDataPoint()
     *
     * Sections: [\-\-PIFA]
     *
     * @return                  vector of "active" flags for all data points
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual std::vector<bool> getDataPointsActive() = 0;

    /** Set "active" flag for a data point.
     *
     * To exclude a certain data point from the fit procedure, its "active" flag
     * can be set to false. For example, in a multi-echo experiment, to exclude
     * a certain echo.
     *
     * Note: If a data point is set active/inactive in setupParameters(), i.e.,
     * section "P", the user may override that value on the GUI. If set in
     * initParameters(), i.e., section "I", the "active" state of the data point
     * is enforced during each parameter initialization and thus cannot be
     * changed in the GUI.
     *
     * Sections: [\-\-PI\-\-]
     *
     * @param abscissaIndex     index of data point on the abscissa
     * @param active            "active" flag
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setDataPointActive(unsigned int abscissaIndex, bool active) = 0;

    /**@} Data points */

    // =========================================================================

    /** @name Optimization
     * Functions controlling the parameter fit.
     * @{*/

    /** Set the function value.
     *
     * Used to set the result of the fit model function evaluation.
     *
     * Sections: [\-\-\-\-F-]
     *
     * @param value
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setFunctionValue(double value) = 0;

    /** Get the target tolerance for chi square.
     *
     * See setTargetTolerance().
     *
     * Sections: [DRPIFA]
     *
     * @return                  the target tolerance for the fit
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual double getTargetTolerance() = 0;

    /** Get maximum number of iterations.
     *
     * See setMaxIterations().
     *
     * Sections: [DRPIFA]
     *
     * @return                  the maximum number of iterations
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual unsigned int getMaxIterations() = 0;

    /** Get all fitted parameter values.
     *
     * Sections: [-----A]
     *
     * @return                  fitted parameter values
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual std::vector<double> getFitParametersValue() = 0;

    /** Get all fitted parameter standard deviations.
     *
     * Sections: [-----A]
     *
     * @return                  fitted parameter STDs
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual std::vector<double> getFitParametersSTD() = 0;

    /** Get chi square of the fit
     *
     * Sections: [-----A]
     *
     * @return                  chi square of the fit
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual double getFitChiSq()= 0;

    /** Get number of iterations needed for the fit.
     *
     * Sections: [-----A]
     *
     * @return                  iterations needed for the fit
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual unsigned int getFitIterations() = 0;

    /** Get degrees of freedom of the fit.
     *
     * For @f$N@f$ active data points and @f$M@f$ variable model parameters
     * (that are determined by fitting the model to the @f$N@f$ data points),
     * the number @f$\textrm{dof} = N - M@f$ is called the number of degrees of
     * freedom of the fit.
     *
     * Fixed and "output only" parameters are not variable parameters.
     *
     * Sections: [\-\-\-IFA]
     *
     * @return                  degrees of freedom
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual int getDegreesOfFreedom() = 0;

    /** Set target tolerance for the fit.
     *
     * A fit of the model parameters is considered to have converged, when
     * the decrease of chi_square is less than tolerance times the previous
     * chi_square.
     *
     * Sections: [DRPI\-\-]
     *
     * @param targetTolerance   new target tolerance for the fit
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setTargetTolerance(double targetTolerance) = 0;

    /** Set maximum number of iterations for the fit.
     *
     * Even if a fit has not converged as defined by the target tolerance, the
     * fit is stopped after a maximum number of iterations.
     *
     * Sections: [DRPI\-\-]
     *
     * @param maxIterations     new maximum number of iterations during the fit
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setMaxIterations(unsigned int maxIterations) = 0;

    /**@} Optimization */

    // =========================================================================

    /** @name Miscellaneous
     * Miscellaneous functions.
     * @{*/


    /** Set flag "save fixed parameters".
     *
     * When creating parameter maps, fixed parameters would result in flat
     * images and are therefore omitted by default. If these images should be
     * created, set "save fixed parameters" to true.
     *
     * Sections: [\-\-PIFA]
     *
     * @param saveFixedParams   whether fixed parameter images should be saved.
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setSaveFixedParams(bool saveFixedParams) = 0;

    /** Set flag "reinit parameters".
     *
     * By default, before each fit of the model to a set of data points, the
     * initParameters() section is executed. If initParameters() should not be
     * called each time, the "reinit parameters" flag can be set to false. Then,
     * the initParameters() section is called only once, before the first fit of
     * the model.
     *
     * Sections: [DRP\-\-\-]
     *
     * @param reinitParams      whether or not the initParameters() section is
     *                          executed before each fit (default is true).
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setReinitParams(bool reinitParams) = 0;

    /** Set flag "bypass fit".
     *
     * This flag can be used to bypass the fit of the model parameters. All
     * model parameters keep the values that were set in the section
     * initParameters(). When the model is evaluated for a plot, these
     * parameter values are used.
     *
     * Sections: [DRPI\-\-]
     *
     * @param bypassFit         whether the fit should be bypassed (default is
     *                          false)
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setBypassFit(bool bypassFit) = 0;

    /** Set flag "bypass evaluation".
     *
     * This flag can be used to bypass the evaluation of the model for given
     * abscissa values. This only makes sense for models that are used to
     * display the input data and do not actually model the data , e.g. "View".
     *
     * Sections: [DRPI\-\-]
     *
     * param bypassEval         whether the function evaluation should be
     *                          bypassed (default is false)
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setBypassEval(bool bypassEval) = 0;

    /** Set flag "parameter maps depend on ROI"
     *
     * In some cases, e.g., in TimeCourseCorrelation, the resulting parameter
     * map depends on a ROI specified by the user. In this case, when the ROI is
     * changed, in the ISA GUI the parameter map must be updated. Also, the
     * model will be re-initialized globally, i.e., in section initParameters,
     * the ROI data points are passed and a call to isFirstInit() will return
     * true.
     *
     * Sections: [DRP\-\-\-]
     *
     * @param depends           true if the parameter map depends on ROI state
     *                          (default is false).
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setParMapsDependOnROI(bool depends) = 0;

    /**
     * Specify frame in ISA frame sequence best suited for background detection.
     *
     * If between (and including) 0.0 and 1.0, the frame number is calculated
     * relative to the total ISA frame group length. If larger than 1.0, then
     * bgBestFrame is rounded to the next integer and is interpreted as an
     * absolute frame number (zero-based). If less than 0.0, then
     * -1.0*bgBestFrame is rounded to the next integer and taken to count from
     * the back of the ISA frame group. Default is 0.0 (first frame).
     *
     * Sections: [-RP\-\-\-]
     *
     * @param bgBestFrame       best frame for background detection
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual void setBgBestFrame(double bgBestFrame) = 0;

    /** This is the first parameter initialization.
     *
     * Returns true only in the very first call of the parameter initialization
     * section. This can be used, e.g., to initialize data point dependent model
     * instance variables.
     *
     * Sections: [\-\-\-I\-\-]
     *
     * @return                  whether this is the first call to initParameters
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual bool isFirstInit() = 0;

    /**
     * Return linear interpolation of active data point ordinate value.
     * 
     * Among all active data points, the two points whose abscissa value is 
     * closest to x are identified. Then a linear interpolation is
     * performed if x lies between those points, otherwise an extrapolation is
     * done.
     *
     * Sections: [\-\-\-IFA]
     *
     * @param x                 abscissa value for which the interpolated value
     *                          is to be calculated
     * @return                  interpolated data value
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual double getLinearInterpolation(double x) = 0;

    /** Get flag "save fixed parameters".
     * 
     * See setSaveFixed().
     * 
     * Sections: [\-\-\-IFA]
     * 
     * @return                  "save fixed parameters" flag
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual bool getSaveFixed() = 0;

    /** Get flag "reinit parameters".
     * 
     * See setReinitParams().
     * 
     * Sections: [DRPIFA]
     * 
     * @return                  "reinit parameters" flag
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual bool getReinitParams() = 0;

    /** Get flag "bypass fit".
     * 
     * See setBypassFit().
     * 
     * Sections: [DRPIFA]
     * 
     * @return                  "bypass fit" flag
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual bool getBypassFit() = 0;

    /** Get flag "bypass evaluation".
     * 
     * See setBypassEval().
     * 
     * Sections: [DRPIFA]
     * 
     * @return                  "bypass eval" flag
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual bool getBypassEval() = 0;    

    /** Get flag "parameter maps depend on ROI"
     *
     * See setParMapsDependOnROI().
     *
     * Sections: [DRPIFA]
     *
     * @return "parameter maps depend on ROI" flag
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual bool getParMapsDependOnROI() = 0;

    /** Call the relation of the method parameter, IsaUserUpdate, if present.
     *
     * If the method defines a character array parameter "IsaUserUpdate" and
     * redirects its relation, this function copies the request into
     * IsaUserUpdate and calls the relation. Thus, a model can exchange
     * information with the method.
     *
     * Sections: [-RPIFA]
     *
     * @param request           request to be passed to method
     * @return                  whether the parameter IsaUserUpdate was found
     *                          and set.
     *
     * @ingroup ISA_MODEL_BASE
     */
    virtual bool callMRmethod(const char* request = "") = 0;

    /**@} Miscellaneous */

    // =========================================================================

    /**
     * Store abscissa settings.
     *
     * Store all abscissa settings performed so far using
     * <ul>
     *   <li>setIsaVariable
     *   <li>setIsaVariableUnit
     *   <li>setIsaFrameGroup
     *   <li>setAbscissaLabel
     *   <li>setAbscissaValues
     *   <li>setAbscissaRange
     * </ul>
     * as a possible set of settings for the abscissa. If storeAbscissa is
     * called more than once, the user can chose which setting to use in the
     * IsaTool. It is not necessary to call storeAbscissa for a single abscissa
     * setting.
     *
     * Sections: [DR\-\-\-\-]
     *
     */
    virtual void storeAbscissa() = 0;
};

#endif /* MODELBASE_HH */

//==========================================================================EOF=
