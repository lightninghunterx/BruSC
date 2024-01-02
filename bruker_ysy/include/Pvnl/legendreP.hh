/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2017
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVNL_LEGENDREP_HH
#define PVNL_LEGENDREP_HH

#ifndef CPROTO

#include<vector>
#include<string>
#include<iostream>
#include<sstream>
#include "common/ovl_toolbox/DLLMacro.h"

/** @file
 *  @defgroup PvnlLegendre PvnlLegendre
 *  Provides Legendre polynomials / solid harmonics
 *
 */
//@{

/** Maximum degree and order of Legendre Polynomials*/
#define LEGENDRE_MAXORD 10u

/** Enumeration describing the normalization convention implemented. */
typedef enum {
    /** Normalization like associated Legendre Polynomials */
    NcLegendre,
    /** Normalization maximum on unit sphere = 1 */
    NcMaxUnitSphere,
    /** Factor for leading cartesian term = 1 (Romeo & Hoult)*/
    NcRomeoHoult,
    /** Normalization like spherical harmonics */
    NcSpherical
} PVNL_LEGENDRE_NCONV;

/**
 * LegendreP is a class defining Associated Legendre Polynomials @f$ P_n^m @f$ of
 * degree @f$n@f$ and order @f$m@f$. 
 * Implementation for degree @f$ n \le N @f$ and @f$ m \in [-n,n]@f$ with 
 * maximum degree @f$ N @f$ defined by @ref LegendreP::maxord  
 * Field functions 
 * \f[  K_n^m(x,y,z) = 
 r^{n-\left|m\right|} P_n^m(z/r)  R^{\left|m\right|} \cos(m\phi) \quad (m \geq 0)
\f] 
 * and \f[  K_n^m(x,y,z) = 
 r^{n-\left|m\right|} P_n^m(z/r)  R^{\left|m\right|} \sin(m\phi) \
 \quad (-n \leq m \leq n)
\f] 
 * are provided with @f$ r^2 = x^2 + y^2 + z^2 = R^2 + z^2@f$.
 */
class DLL_INTERFACE LegendreP {
private:

    /** Implementation of internal class parameter. */
    struct Impl;

    /** Implementation of internal class parameter. */
    Impl *pimpl_;

    // deleted constructors
    LegendreP(const LegendreP&);
    LegendreP & operator=(const LegendreP &);

public:
    /** constructor */
    LegendreP();
    /** destructor */
    ~LegendreP();

    /** Returns the maximum degree of the implementation of legendre polynomials.
        @return maximum degree of implemented legendre polynomials
     */
    inline unsigned int maxord() const {
        return LEGENDRE_MAXORD;
    };
    /** Field components of Solid Harmonics for a cartesian coordinate.
     *      On output the field components of solid harmonics functions are stored 
     *  in a vector.
     *
     *  \f[  S_n^m (r,\theta,\phi) = r^n P_n^m(\cos \theta) \cos(m \phi) \quad m \ge 0
     *  \f]
     *  \f[     S_n^m (r,\theta,\phi) = r^n P_n^{|m|}(\cos \theta) \sin(|m| \phi) \quad m < 0
     *  \f]
     * with
     *  @f$ 0 \le n \le N  m = -n,\cdots,-1,0,1,\cdots,n @f$ and @f$ N =@f$ deg 
     *  @f$r = \sqrt{x^2 + y^2 + z^2}@f$, 
     *  @f$ R = \sqrt{x^2 + y^2}  @f$, 
     *  @f$ \theta= \cos^{-1} (\frac{z}{r}) @f$, 
     *  @f$ \phi= \cos^{-1} (x/R) @f$,
     *  @f$ \phi= \sin^{-1} (y/R) @f$,
     * Values of @f$ S_n^m @f$ are stored in a vector (size assumed to be @f$ (N+1)^2 @f$
     * the order of solid harmonic coefficients is defined as follows:
     * \f[ S_n^m \equiv S_i,  i(n,m) = 
     \left\{
     \begin{array}{ll}
     0             & n = 0         \\
     n^2 + m       & n > 0, m >= 0 \\
     n^2 + n + |m| & n > 0, m < 0 
     \end{array} \right. \f]
     * In case of errors an exception is thrown.
     * @param deg maximum degree of legendre polynomial
     * @param x cartesian x-coordinate
     * @param y cartesian y-coordinate
     * @param z cartesian z-coordinate
     * @param normC norming convention of solid harmonic shim
     * @param svec result shim vector
     */
    void Field(const unsigned int deg,
            const double x,
            const double y,
            const double z,
            const PVNL_LEGENDRE_NCONV normC,
            std::vector<double> &svec);
    /** Returns value of Associated Legendre polynomial.
     * Degree @f$ n  \in [0,N] \quad N=@f$ @ref LegendreP::maxord, the order  
     * @f$ m \in [-n,n] @f$ the cosine term @a ct 
     * must be in  @f$ \in [-1.0,1.0] @f$
     * 
     * @param deg degree @f$ n @f$ 
     * @param ord order @f$ m @f$ 
     * @param ct  cosine term @f$\cos (\theta) = y/r @f$
     * @param normC normalization convention
     * @return value of @f$P_n^m(ct)@f$
     */
    double LPoly(const int deg,
            const int ord,
            const double ct,
            const PVNL_LEGENDRE_NCONV normC);

    /** Returns value of Associated Legendre polynomial.
     * Degree @f$ n  \in [0,N] \quad N=@f$ @ref LegendreP::maxord, the order  
     * @f$ m \in [-n,n] @f$ the cosine term @a ct 
     * must be in  @f$ \in [-1.0,1.0] @f$
     * 
     * @param deg degree @f$ n @f$ 
     * @param ord order @f$ m @f$ 
     * @param ct  cosine term @f$\cos (\theta) = y/r@f$
     * @return value of @f$P_n^m(ct)@f$
     */
    double LPoly(const int deg,
            const int ord,
            const double ct);

    /** Returns the number of associated legendre polynomials for a given degree.
        @param deg @f$ n \ge 0@f$
        @return number of legendre polynomials
     */
    inline unsigned int NPoly(unsigned int deg) const {
        deg += 1;
        return deg*deg;
    };

    /** Returns coefficient and function index for a given degree and order.
     *  For deg @f$ n @f$ and ord @f$ m @f$ number of 
     *  @f$ F_n = (n+1)^2 @f$ solid harmonic functions and 
     *  @f$ L_n = n+1 + \frac{1}{2}n(n+1) @f$ associated legendre polynomials (and
     *  normalization constants) exist.
     *  
     *  This functions returns the coefficient index <code> kind </code> 
     *  @f$ \in [0,L_n -1] @f$ and function index <code> find </code>
     *  @f$ \in [0,F_n -1] @f$ in arrays delivered by
     *  @ref ShimNames @ref NormConst and @ref Field
     *  @param deg degree of associated Legendre polynomial
     *  @param ord order of associated Legendre polynomial
     *  @param kind coefficient index
     *  @param find function index
     *  @return false in case of illegal degree or order else true
     */
    bool Kindex(unsigned int deg, int ord, int &kind, int &find) const;

    /** Returns function index for given degree and order of solid harmonic function.
     *  The function index may be used to access fields for degree and order in
     *  vectors retrieved by @ref ShimNames and @ref Field . It is the 
     *  counterpart of @ref Ind2Ord .
     *  In case of illegal values of 
     *  @f$n \notin [0,N]\quad m \notin[-n,n] @f$
     *  with maximum degree @f$ N @f$ defined by @ref LegendreP::maxord  
     *  an exception is thrown. 
     *  @param n degree of associated Legendre Polynomial
     *  @param m order of associated Legendre Polynomial
     *  @return function index @f$ \in [0, (n+1)^2] @f$
     */
    int Findex(unsigned int n, int m) const;
    /** Delivers degree and order from function index.
     *  It is the counterpart of @ref Findex .
     *  @param index function index
     *  @param deg degree of Legendre Polynomial
     *  @param ord order  of Legendre Polynomial
     */
    void Ind2Ord(unsigned int index, int &deg, int &ord);

    /** Returns the names of shims up to a given degree.
     *  For deg @f$ N @f$ 
     *  @f$ F_N = (N+1)^2 @f$ field functions @f$ K_m^m @f$ exist
     *  that define shim fields to express fields like: 
     *  \f[ B_i (r,\phi,\theta)=\sum_{n=0}^{N}\sum_{m=-n}^{n}c_{nm}K_n^m (r,\phi,\theta) \f]
     *  @param deg max degree of Legendre Polynomial
     *  @return vector of shim names of length @f$ F_N @f$
     */
    std::vector<std::string> ShimNames(const int deg);

    /** Returns the names of shims up to a given degree.
     *  For deg @f$ N @f$ 
     *  @f$ F_N = (N+1)^2 @f$ field functions @f$ K_m^m @f$ exist
     *  that define shim fields to express fields like: 
     *  \f[ B_i (r,\phi,\theta)=\sum_{n=0}^{N}\sum_{m=-n}^{n}c_{nm}K_n^m (r,\phi,\theta) \f]
     *  @param deg degree of Legendre Polynomial
     *  @param ord order of Legendre Polynomial
     *  @return shim name for degree and order
     */
    std::string ShimName(const int deg, const int ord);
    /** Test procedure of legendre functions.
        @param r radius @f$ > 0 @f$ 
        @param norm normalization convention
        @param outfn if specified ascii output of deg,ord 
               x,y,z coordinates and Field(deg,ord,x,y,z)
               in given normalization convention
        @return test output represented as string
     */
    std::string TestLegKoeff(const double r,
            PVNL_LEGENDRE_NCONV norm = NcRomeoHoult,
            const char *const outfn = 0);

    /** Returns normalization constants for fields.
     *  For deg @f$ n @f$ and ord @f$ m @f$
     *  @f$ F_n = (n+1)^2 @f$ solid harmonic functions and 
     *  @f$ L_n = n+1 + 1/2 n(n+1) @f$ associated legendre polynomials (and
     *  normalization constants) exist. A magnetic field approximated by 
     *  solid harmonic coefficients is defined as:
     *  \f[ B_z (x,y,z)=\sum_{n=0}^{N}\sum_{m=-n}^{n}c_{nm}K_n^m (x,y,z) \f]
     *  Dependent on normalization constants 
     * @f$ k_{nm},\overline{k}_{nm}@f$ and @f$\widetilde{k}_{nm}@f$ field  
     * coefficients @f$c_ {nm}, \overline{c}_ {nm}@f$ and @f$ \widetilde{c}_ {nm}@f$ 
     * are defined that are related to each other as 
     * \f[  k_{nm} c_ {nm} = \overline{k}_{nm}\overline{c}_ {nm}= \
        \widetilde{k}_{nm}\widetilde{c}_ {nm} \f]
     * In this implementation normalization constants according to Romeo & Hoult
     * @f$ \widetilde{k}_{nm}=1 \forall n,m @f$. 
     * Romeo & Hoult : @f$ \widetilde{k}_{nm}@f$ normalizes the 
     *                 @f$ z^{n-m} @f$ term of @f$ K_n^m @f$ to 1.
     * Maximum on unit sphere =1 :
     * \f[ \overline{k}_{nm} = \
       \frac{2^{\left|m\right|}\left|m\right|!  \
             (n-\left|m\right|)!}{(n+\left|m\right|)!} \
             \quad\frac{1}{\max_{ct \in [-1,1]} |P_n^m(ct)|} \f]
     * Normalization like associated Legendre Polynomials:
     * \f[ k_{nm} = \frac{(n+\left|m\right|)!}
                     {2^{\left|m\right|}\left|m\right|! (n-\left|m\right|)!} \f]
     *  @param deg degree of the Legendre polynomial
     *  @param normC desired normalization convention
     *  @return vector of normalization constants (length @f$ F_n @f$)
     */
    std::vector<double> NormConst(const unsigned int deg,
            const PVNL_LEGENDRE_NCONV normC) const;

    /** Activates or deactivates Condon Shortley phase factor.
        If activated a factor @f$ (-1)^m @f$ is included in the definition 
        of Legendre Polynomials (default deactivated).
        @param activate flag to activate / deactivate inclusion of Condon 
               shortley phase factor 
     */
    void CondonPhaseFactor(bool activate);

};

/** Name space for interfaces using legendre polynomials for solid harmonic functions.
 */
namespace SolidHarmonics 
{
    /** Delivers the max. field deviation within a sphere.
     *  Dependent on sensitivities ( unit @f$ Hz/cm^n @f$ or @f$ Hz/mm^n @f$ )
     *  defined for real valued solid harmonics the maximum field deviation in 
     *  a sphere is delivered by this function.
     *  Coefficients are provided as double arrays (length unit as defined by 
     *  argument unitCm ) in which entries are related to solid harmonic functions
     *  as defined in @see LegendreP::Field 
     *  @param co [in] coefficients ( unit @f$ Hz/cm^n @f$ or @f$ Hz/mm^n @f$ 
     *  @param nconv [in] normalization convention of coefficients
     *  @param radius [in] radius of the sphere in mm
     *  @param fDev [out] resultant field deviation in @f$ Hz @f$
     *  @param leg [in] legendre polynomial class (used for calculation)
     *  @param unitCm [in] (optional) flag indicating length unit of sensitivities 
                     (true unit in cm)
     *  @param skip_zero_order [in] (optional) flag indicating whether zero order term should be ignored
     *  @param cosd [in] (optional) standard deviation of coefficients ( unit @f$ Hz/cm^n @f$ or @f$ Hz/mm^n @f$ 
     *  @param fDevSD [out] (optional) resultant standard deviation of fDev
     */


    DLL_INTERFACE void MaxFieldDev(std::vector<double> &co,
            const PVNL_LEGENDRE_NCONV nconv,
            double radius,
            double &fDev,
            LegendreP &leg,
            bool unitCm = false,
            bool skip_zero_order = true,
            std::vector<double> *cosd = 0,
            double *const fDevSD = 0);
}
//@}

#endif // CPROTO
#endif // PVNL_LEGENDREP_HH
