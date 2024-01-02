//==============================================================================
//
// $Source$
// Interface to be implemented by ISA models
//
// Copyright (c) 2015
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//
//==============================================================================

#ifndef MODELINTERFACE_HH
#define MODELINTERFACE_HH

#include <string>

//==============================================================================
// INTERFACE FILE FOR ISA MODEL IMPLEMENTATIONS - DO NOT EDIT
//==============================================================================

/**
 * @defgroup ISA_MODEL_INTERFACE ISA Model Interface
 */

/** For each ISA model the following sections need to be implemented.
 *
 * <ul>
 *   <li><b>(D) Declaration</b>: General properties of the model are defined.</li>
 *   <li><b>(R) Requirements</b>: Model requirements are specified. The
 *              requirements determine whether the model is suitable for a given
 *              dataset or not.</li>
 *   <li><b>(P) Parameter Setup</b>: Model parameters are declared and
 *              configured.</li>
 *   <li><b>(I) Parameter Initialization</b>: Initial values for the model
 *              parameters are determined, based on the data points to which the
 *              model will be fitted.</li>
 *   <li><b>(F) Function</b>: Evaluation of function value and partial
 *              derivatives, based on the current values of the model parameters
 *              during optimization.</li>
 *   <li><b>(A) Analysis</b>: Analysis of the model parameters resulting from
 *              the optimization. Calculation of secondary parameters.</li>
 * </ul>
 *
 */
class ModelInterface {
public:

    virtual ~ModelInterface() {
    };

    /* ---- Model implementation sections --------------------------------------
     * 
     * These functions represent the actual implementation of an ISA fit model 
     * in six sections.
     * 
     * Implementation Functions are "= 0": Must be implemented in derived class.
     */

    /**@{*/

    /** Declaration section of the ISA model (D).
     * 
     * This function is called when the model is instantiated. This function is
     * independent of a dataset and cannot access dataset parameters.
     * 
     * <b>Context</b>: No dataset parameters, no data points.
     *
     * @ingroup ISA_MODEL_INTERFACE
     */
    virtual void modelDeclaration() = 0;

    /**@}*/
    /**@{*/

    /** Requirements section of the ISA model (R).
     * 
     * This function is called to determine whether or not this model can be
     * fitted to a specific dataset. If one or more requirements are not met, 
     * this model is not made available to the user for the given dataset and 
     * the following sections are not called.
     * 
     * This function can also be used to modify the model declaration, adapting
     * it to the specific dataset.
     * 
     * <b>Context</b>: With dataset parameters, no data points.
     *
     * @ingroup ISA_MODEL_INTERFACE
     */
    virtual void modelRequirements() = 0;

    /**@}*/
    /**@{*/

    /** Parameter setup section of the ISA model (P).
     * 
     * This function is used to set up the number of model parameters and their
     * properties. These may depend on the number of data points or other
     * parameters.
     * 
     * <b>Context</b>:  With dataset parameters, no data points.
     *
     * @ingroup ISA_MODEL_INTERFACE
     */
    virtual void setupParameters() = 0;

    /**@}*/
    /**@{*/

    /** Parameter initialization section of the ISA model (I).
     * 
     * Given the sequence of data points to fit, the model parameters can be 
     * initialized to obtain good starting values for the actual fit.
     *     
     * Data point handling:
     * <ul>
     *  <li>Number of data points:         N = getNumDataPoints()</li>
     *  <li>Data point index               i = 0 ... N-1</li>
     *  <li>Abscissa value:                getAbscissaValue(i)</li>
     *  <li>Data point value               getDataValue(i)</li>
     *  <li>Data point standard deviation: getDataSTD(i)</li>
     * </ul>
     *
     * <b>Context</b>: With dataset parameters, data points are available.
     *
     * <b>Note</b>: This section is critical for good performance. For example,
     * Parx access is strongly discouraged. If a dataset parameter is needed,
     * e. g., the flip angle, use model instance variables and assign a value in
     * the setupParameters section.
     *
     * @ingroup ISA_MODEL_INTERFACE
     */
    virtual void initParameters() = 0;

    /**@}*/
    /**@{*/

    /** Section for evaluation of model function value and partial derivatives (F). 
     * 
     * Given a set of parameter values, this function evaluates the function 
     * value and the first partial derivatives of the fit model with respect to 
     * all parameters.
     * 
     * <b>Context</b>: With dataset parameters, data points are available.
     * 
     * <b>Note</b>:This section is critical for good performance. For example,
     * Parx access is strongly discouraged. If a dataset parameter is needed,
     * e. g., the flip angle, use model instance variables and assign a value in
     * the setupParameters section.
     * 
     * @param x                 input abscissa value
     * @param skipDerivatives   can skip calculation of derivatives
     *
     * @ingroup ISA_MODEL_INTERFACE
     */
    virtual void evaluate(double x, bool skipDerivatives) = 0;

    /**@}*/
    /**@{*/

    /** Section for analyzing the fit result (A).
     *
     * This section is called after the fit has converged, or was skipped. It 
     * can be used to analyze the resulting fit parameters and to derive output
     * parameters.
     *
     * If the analysis shows that the fit has failed, this function should
     * returns false, else true. A failed fit results in all parameters being 
     * assigned their respective default value and is indicated in the mask
     * (last parameter map) by a zero value.
     *
     * <b>Context</b>: With dataset parameters, data points are available.
     *
     * @return false, if fit has failed, true else.
     *
     * @ingroup ISA_MODEL_INTERFACE
     */
    virtual bool analyze() = 0;

    /**@}*/

    /* ---- Compilation time stamp ------------------------------------------ */

    virtual std::string getTimeStamp() {
        std::string timeStamp = __DATE__;
        timeStamp.append(" ");
        timeStamp.append(__TIME__);
        return timeStamp;
    } 
};

#endif // MODELINTERFACE_HH
