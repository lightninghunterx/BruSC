/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2003
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVNL_FIT_H
#define PVNL_FIT_H

#include "Pvnl/PvnlGeneral.h"
#include "PvUtil/PvExcept.hh"
#include "Pvnl/DLLMacro.h"

/** @file
 *  @defgroup PvnlFit PvnlFit
 *  Provides basic functions for data fitting
 */
/*@{*/


    /*--------------------------------------------------------------*
     * Definitions
     *--------------------------------------------------------------*/

#ifndef CPROTO

    /** Callback function interface for Levenberg-Marquardt fit algotithm. */
    class DLL_INTERFACE PvnlFitCallback {
    public:

        virtual ~PvnlFitCallback() {
        };

        /** Function definition.
         *
         * A real-valued function of a real scalar variable with multiple real
         * parameters.
         *
         * @param x             (in)  independent variable
         * @param a             (in)  array of function parameters
         * @param y             (out) evaluated function value
         * @param dyda          (out) array of first partial derivatives of
         *                            function with respect to the parameters
         * @param nPars         (in)  number of parameters
         * @return              zero if evaluation was successful, else nonzero
         */
        virtual int evalFunction(PVNL_DOUBLE x, const PVNL_DOUBLE* a, PVNL_DOUBLE* y, PVNL_DOUBLE* dyda, int nPars) = 0;
    };

    /** Callable wrapper for different types of callback functions */
    class DLL_INTERFACE PvnlFitCallable {
    public:

        /** Empty constructor.
         *
         * Instantiates a non-callable PvnlFitCallable object.
         */
        PvnlFitCallable();

        /** Function constructor.
         *
         * Instantiates a callable PvnlFitCallable object, registering a
         * PvnlFitCallback instance as callback function
         *
         * @param function      PvnlFitCallable instance to register
         */
        PvnlFitCallable(PvnlFitCallback* function);

        /** Destructor. */
        virtual ~PvnlFitCallable();

        /** Register a function pointer as callback function.
         *
         * @param function      function pointer to register
         */
        void registerCallback(int(*function)(PVNL_DOUBLE x, PVNL_DOUBLE* a, PVNL_DOUBLE* y, PVNL_DOUBLE* dyda, int nPars));

        /** Register a PvnlFitCallback instance as callback function
         *
         * @param function      PvnlFitCallback instance to register
         */
        void registerCallback(PvnlFitCallback* function);

        /** Unregister the callback function */
        void unregister();

        /** Callable interface.
         *
         * @param x             (in)  independent variable
         * @param a             (in)  array of function parameters
         * @param y             (out) evaluated function value
         * @param dyda          (out) array of first partial derivatives of
         *                            function with respect to the parameters
         * @param nPars         (in)  number of parameters
         * @return              zero if evaluation was successful, else nonzero
         */
        int operator()(PVNL_DOUBLE x, PVNL_DOUBLE* a, PVNL_DOUBLE* y, PVNL_DOUBLE* dyda, int nPars);

        /** Get callable state.
         *
         * If no callback function has been registered, the PvnlFitCallable
         * instance is not callable.
         *
         * @return              whether the instance is callable
         */
        bool isCallable();

    private:
        PvnlFitCallable(const PvnlFitCallable&) = delete;

        PvnlFitCallable& operator=(const PvnlFitCallable&) = delete;

        PvnlFitCallback* callbackObj_;

        int (*callbackFun_) (PVNL_DOUBLE x, PVNL_DOUBLE* a, PVNL_DOUBLE* y, PVNL_DOUBLE* dyda, int nPars);
    };
#endif // CPROTO
    /*--------------------------------------------------------------*
     * prototype definition with doxygen documentation
     *--------------------------------------------------------------*/

    /**
     * @defgroup PvnlFitPoly Polynomial fit
     * @ingroup PvnlFit
     */

    /**
     * @ingroup PvnlFitPoly
     * @brief Calculate polynomial fit via SVD.
     *
     * @param x                 sampling points
     * @param y                 sampled function values
     * @param n                 number of sampled points
     * @param deg               degree of polynomial
     * @param a                 resulting coefficient function - (deg+1) memory
     *                          locations must be reserved in advance. The
     *                          coefficients are stored in ascending order, i.e.
     *                          the first entry corresponds to the constant term.
     * @param chi               residual value describing the difference between
     *                          the data and the fit
     * @return                  error code, when algorithm fails
     */
    DLL_INTERFACE int PvnlFitPoly(const PVNL_DOUBLE* x, const PVNL_DOUBLE* y, int n, int deg,
            PVNL_DOUBLE* a, PVNL_DOUBLE* chi);

    /**
     * @ingroup PvnlFitPoly
     * @brief Calculate polynomial fit via SVD with weighted equidistant
     * sampling points.
     *
     * sampling points x[i] = i * scale + offset
     *
     *  @param y                sampled function values
     *  @param n                number of sampled points
     *  @param deg              degree of polynomial
     *  @param w                weight vector of length n - w = NULL will
     *                          apply constant weight 1
     *  @param scale            scaling for sampling
     *  @param offset           offset for sampling
     *  @param a                resulting coefficient function - (deg+1) memory
     *                          locations must be reserved in advance. The
     *                          coefficients are stored in ascending order, i.e.
     *                          the first entry corresponds to the constant term.
     *  @return                 error code, when algorithm fails
     */
    DLL_INTERFACE int PvnlFitPolyWeight(PVNL_DOUBLE* y, int n, int deg,
            PVNL_DOUBLE* w, double scale,
            double offset, PVNL_DOUBLE* a);


    /** @defgroup PvnlFitNonlin Nonlinear fit
     *  @ingroup PvnlFit
     *  @brief Levenberg-Marquardt solution of the least squares curve fitting
     * problem.
     *
     * These functions solve the following least squares curve fitting problem:
     * For @f$ n @f$ data points with individual standard deviations,
     * @f$ (x_i, y_i \pm \sigma_i) @f$, and a model function @f$ f(x,{\bf a}) @f$
     * the @f$ m @f$ fit parameters @f$ a_j @f$ are optimized such that the
     * sum of squares of the deviations
     * @f[
     *   \chi^2({\bf a}) = \sum_{i=0}^{n-1} \left(\frac{y_i - f(x_i, {\bf a})}{\sigma_i} \right)^2
     * @f]
     * is minimal. The reduced chi squared,
     * @f[
     *   \chi^2_{\rm red}({\bf a}) = \frac{\chi^2({\bf a})}{\rm n - m}
     * @f]
     * can be used as a measure of goodness of fit, as it takes into account the
     * number of data points and the model complexity.
     *
     * The model function @f$ f(x,{\bf a}) @f$ must be implemented by the user,
     * evaluating the function value and its partial derivatives with respect to
     * the model parameters. The function interface reads:
     *
     * @code int modelFunc(PVNL_DOUBLE x, PVNL_DOUBLE* a, PVNL_DOUBLE* y, PVNL_DOUBLE* dyda, int nPars) @endcode
     *
     * @param x                 input value for @f$ x @f$
     * @param a                 input parameters of the model function
     * @param y                 resulting function value @f$ f(x,{\bf a}) @f$
     *                          for given <code>x</code> and <code>a</code>.
     * @param dyda              resulting partial derivatives
     *                          @f$ \frac{\partial f}{\partial a_j}(x,{\bf a}) @f$
     *                          of the function along each parameter
     *                          @f$ a_j @f$, for given <code>x</code> and
     *                          <code>a</code>.
     * @param nPars             number of parameters
     * @return                  zero on success, negative on error.
     *
     * This implementation of the Levenberg-Marquardt algorithm is based on the
     * book <a href="http://www.nr.com/" target="_blank">"Numerical Recipes in
     * C"</a> but has been modified and extended to respect limits for the model
     * parameters.
     */

#ifndef CPROTO
    /**
     * @ingroup PvnlFitNonlin
     * @brief Perform Levenberg-Marquardt minimization, step by step.
     *
     * A complete fit consists of the following steps:
     *
     * 1. Initialization: One call of <code>PvnlFitLevenbergMarquardtStep</code>
     * with negative <code>lambda</code> and initial guesses for the model
     * parameters <code>a</code>.
     *
     * 2. Iterations: Multiple calls of <code>PvnlFitLevenbergMarquardtStep</code>
     * until the <code>chisq</code> reaches a given target or the maximum number
     * of iterations is reached. For these calls, parameters must not be
     * modified by the caller.
     *
     * 3. Finalization: One call of <code>PvnlFitLevenbergMarquardtStep</code>
     * with <code>lambda = 0</code> to evaluate the covariance and curvature
     * matrices.
     *
     * The diagonal elements of the final covariance matrix yield estimates for
     * the standard deviation of the parameters,
     * @f$ \sigma_j^2 = \chi^2_{\rm red}{\rm Cov(a_j,a_j)}  @f$.
     *
     * @param x                 x coordinates of data points
     * @param y                 y coordinates of data points
     * @param sigma             standard deviation of data points in y direction.
     *                          Set to <code>NULL</code> if no weighting (equal
     *                          weights) should be used.
     * @param nData             number of data points
     * @param a                 parameters of the model function
     * @param aL                lower limits for parameter values. Set to
     *                          <code>NULL</code> if no lower limit should be
     *                          enforced.
     * @param aU                upper limits for parameter values. Set to
     *                          <code>NULL</code> if no upper limit should be
     *                          enforced.
     * @param ia                flag array indicating which parameters are fixed
     *                          during the optimization (zero value), and which
     *                          parameters are variable (positive value). Do not
     *                          set negative values, these are used internally.
     * @param nPars             number of parameters
     * @param covar             resulting covariance matrix, size:
     *                          <code>nPars</code> x <code>nPars</code>.
     * @param alpha             resulting curvature matrix, size:
     *                          <code>nPars</code> x <code>nPars</code>.
     * @param chisq             chi squared with the current values of
     *                          <code>a</code>
     * @param modelFunc         model function
     * @param lambda            lambda factor
     * @param workmem           working memory, size must be at least
     *                          <code>7*nPars*MAX(sizeof(PVNL_DOUBLE),sizeof(int))</code> bytes.
     *                          If set to <code>NULL</code>,
     *                          <code>PvnlFitLevenbergMarquardtStep</code> will
     *                          allocate and free the memory automatically in
     *                          each step, possibly degrading performance.
     * @return                  zero on success, negative if the model function
     *                          <code>modelFunc</code> returned an error or
     *                          invalid values (NaN / infinity) for the
     *                          parameters or partial derivatives.
     * @throws                  PvStdException
     *
     */
    DLL_INTERFACE int PvnlFitLevenbergMarquardtStep(
            const PVNL_DOUBLE* x,
            const PVNL_DOUBLE* y,
            const PVNL_DOUBLE* sigma,
            int nData,
            PVNL_DOUBLE* a,
            const PVNL_DOUBLE* aL,
            const PVNL_DOUBLE* aU,
            int* ia,
            int nPars,
            PVNL_DOUBLE** covar,
            PVNL_DOUBLE** alpha,
            PVNL_DOUBLE* chisq,
            PvnlFitCallable& modelFunc,
            PVNL_DOUBLE* lambda,
            PVNL_DOUBLE* workmem);

    /**
     * @ingroup PvnlFitNonlin
     * @brief Perform a complete Levenberg-Marquardt minimization.
     *
     * All necessary steps (see <code>@ref PvnlFitLevenbergMarquardtStep</code>)
     * are performed in a single call to this function. The parameters are
     * checked and the working memory is automatically allocated and freed.
     *
     * Call <code>PvnlFitLevenbergMarquardt</code> with initial guesses for the
     * model parameters <code>a</code>, set <code>iter</code> to the desired
     * maximum number of iterations, and set <code>chisq</code> to the desired
     * tolerance.
     *
     * The fit is considered to have converged, if, for decreasing step size,
     * the reduced chi squared does not decrease by more than the fraction given
     * by tolerance:
     * @f[
     *   \chi^2_{\rm new} >= (1-{\rm tolerance})\;\chi^2_{\rm old}
     *   \;\;\rightarrow\;\; {\rm terminate}
     * @f]
     * two times in a row.
     *
     * If <code>PvnlFitLevenbergMarquardt</code> finishes successfully,
     * <code>chisq</code> contains the actual @f$ \chi^2 @f$ reached and
     * <code>iter</code> contains the number of iterations actually performed.
     *
     * The diagonal elements of the final covariance matrix yield estimates for
     * the standard deviation of the parameters,
     * @f$ \sigma_j^2 = \chi^2_{\rm red}\;{\rm Cov(a_j,a_j)} @f$.
     *
     * The disadvantage of <code>PvnlFitLevenbergMarquardt</code> over
     * <code>PvnlFitLevenbergMarquardtStep</code> is only that the fit progress
     * cannot be monitored or interrupted by the caller.
     *
     * @param x                 x coordinates of data points
     * @param y                 y coordinates of data points
     * @param sigma             standard deviation of data points in y direction.
     *                          Set to <code>NULL</code> if no weighting (equal
     *                          weights) should be used.
     * @param nData             number of data points
     * @param a                 parameters of the model function, input: initial
     *                          guesses, output: result
     * @param aL                lower limits for parameter values. Set to
     *                          <code>NULL</code> if no lower limit should be
     *                          enforced.
     * @param aU                upper limits for parameter values. Set to
     *                          <code>NULL</code> if no upper limit should be
     *                          enforced.
     * @param ia                flag array indicating which parameters are fixed
     *                          during the optimization (zero value), and which
     *                          parameters are variable (positive value). Set to
     *                          <code>NULL</code> if all parameters are variable.
     * @param nPars             number of parameters
     * @param covar             resulting covariance matrix, size:
     *                          <code>nPars</code> x <code>nPars</code>.
     * @param alpha             resulting curvature matrix, size:
     *                          <code>nPars</code> x <code>nPars</code>.
     * @param chisq             input: target tolerance, output: resulting chi
     *                          squared (not reduced chi squared).
     * @param iter              number of iterations, input: maximum, output:
     *                          actual number of iterations performed.
     * @param modelFunc         model function
     * @return                  zero on success, negative if the model function
     *                          <code>modelFunc</code> returned an error or
     *                          invalid values (NaN / infinity) for the
     *                          parameters or partial derivatives.
     * @throws                  PvStdException
     */
    DLL_INTERFACE int PvnlFitLevenbergMarquardt(
            const PVNL_DOUBLE* x,
            const PVNL_DOUBLE* y,
            const PVNL_DOUBLE* sigma,
            int nData,
            PVNL_DOUBLE* a,
            const PVNL_DOUBLE* aL,
            const PVNL_DOUBLE* aU,
            const int* ia,
            int nPars,
            PVNL_DOUBLE** covar,
            PVNL_DOUBLE** alpha,
            PVNL_DOUBLE* chisq,
            int* iter,
            PvnlFitCallable& modelFunc);
#endif // CPROTO


/*@}*/


#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
