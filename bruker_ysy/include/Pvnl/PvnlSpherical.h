/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2007
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/

#ifndef PVNL_SPHERICAL_H
#define PVNL_SPHERICAL_H

#include "Pvnl/PvnlGeneral.h"
#include "Pvnl/DLLMacro.h"


/** @file
 *  @defgroup PvnlSpherical PvnlSpherical
 *  3D spherical functions.
 *
 *  A field expressed in spherical harmonics up to the order \f$n\f$ has
 *  the form:\n
 *
 *  \f[ B_i(r,\phi,\theta) = \sum_{n=0}^{\infty} \sum_{m=-n}^{n} c_{nm}K_{n}^{m} (r,\theta,\phi) \f] \n
 *  Whith order \f$ n \f$ and index  \f$ m=-n,-n+1,\cdots,0,1,\cdots,n \f$
 *  of the spherical harmonic function or order   \f[ n \f] . The relation
 * between  \f$ K_n^m \f$  and the name of the related spherical function is\n
 *  - <CODE> PvnlSphericalAmn </CODE>  \f$ K_{n}^{m} (r,\theta,\phi) \quad m \geq 0 \f$ .\n
 *  - <CODE> PvnlSphericalBmn </CODE>  \f$ K_{n}^{m} (r,\theta,\phi) \quad m < 0 \f$ .\n
 *
 *  Definition of normalization factors and functions done according
 *  to specifications in document ZTPE0002
 *  "Spherical Harmonics and Gradient strength"\n
 *
 *  @note There are different convention to define normalization factors
 *        of spherical harmonic function coefficients:
 *   - Normalization like associated Legendre Polynomials.\n
 *     The normalization is defined by the orthonormality condition
 *     of spherical functions.( \f$ K_n^m \f$ \n
 *   - Maximum on unit sphere equal one ( \f$ \overline{K_n^m} \f$ ).\n
 *   - Coefficient of  \f$ z^{n-m}x^m \f$  equal one ( \f$ \widetilde{K_n^m} \f$ ).\n
 *
 *  The implemented spherical functions use ( \f$ \widetilde{K_n^m} \f$ )
 *  as reference. To translate field coefficients from one definition to
 *  an other the equality may be used:\n
 *   \f[  c_{nm}k_{nm} = \overline{c_{nm}}\overline{k_{nm}} = \widetilde{c_{nm}}\widetilde{k_{nm}} \f] \n
 *  and\n
 *  \f[ \widetilde{k_{nm}} = 1.0 \f] \n
 *  in this implementation of spherical harmonic functions.
 */
//@{

/* spherical function 1 */

/** Returns 0 order spher. harm. func. value 1 (func. index 0)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> 1 </CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalA00(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z ,
			      PVNL_DOUBLE R );

/* spherical function z */

/** Returns 1st 1st order spher. harm. func. value z (func. index 3)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> z </CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalA10(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z ,
			      PVNL_DOUBLE R );

/* spherical function x */
/** Returns 2nd 1st order spher. harm. func. value x (func. index 1)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> x </CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalA11(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z ,
			      PVNL_DOUBLE R );

/* spherical function y */
/** Returns 3rd 1st order spher. harm. func. value y (func. index 2)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> y </CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalB11(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z ,
			      PVNL_DOUBLE R );

/* sperical function z2 */
/** Returns 1st 2nd order spher. harm. func. value z2  (func. index 4)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> z*z - 0.5*R*R </CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalA20(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z ,
			      PVNL_DOUBLE R );

/* spherical function zx*/
/** Returns 2nd 2nd order spher. harm. func. value zx (func. index 6)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> z*x </CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalA21(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z ,
			      PVNL_DOUBLE R  );

/* spherical function zy */
/** Returns 3rd 2nd order spher. harm. func. value zy (func. index 7)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> z*y </CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalB21(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z ,
			      PVNL_DOUBLE R  );

/* spherical function c2 */
/** Returns 4th 2nd order spher. harm. func. value x2-y2 (func. index 5)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> x*x-y*y  </CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalA22(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z ,
			      PVNL_DOUBLE R  );

/* spherical function s2 */
/** Returns 5th 2nd order spher. harm. func. value xy (func. index 8)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> 2*x*y  </CODE>.
 */

DLL_INTERFACE PVNL_DOUBLE PvnlSphericalB22(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z ,
			      PVNL_DOUBLE R  );

/* spherical function z3 */
/** Returns 1st 3rd order spher. harm. func. value z3 (func. index 9)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> z^3 -(3/2)z*R^2 </CODE>.
 */

DLL_INTERFACE PVNL_DOUBLE PvnlSphericalA30(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z,
			      PVNL_DOUBLE R);

/** Returns 2nd 3rd order spher. harm. func. value z2x (func. index 10)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> z^2*x-(1/4)x*R^2 </CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalA31(PVNL_DOUBLE x,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z,
			      PVNL_DOUBLE R);

/* spherical function z2y*/
/** Returns 3rd 3rd order spher. harm. func. value z2y (func. index 11)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> z^2*y-(1/4)y*R^2 </CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalB31(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z,
			      PVNL_DOUBLE R);

/* spherical function zc2 */
/** Returns 4th 3rd order spher. harm. func. value z(x2-y2) (func. index 12)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> z*(x^2-y^2) </CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalA32(PVNL_DOUBLE x,
			      PVNL_DOUBLE y,
			      PVNL_DOUBLE z,
			      PVNL_DOUBLE R );

/* spherical function zs2 */
/** Returns 5th 3rd order spher. harm. func. value zxy (func. index 13)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> z*x*y  </CODE>.
 */

DLL_INTERFACE PVNL_DOUBLE PvnlSphericalB32(PVNL_DOUBLE x,
			      PVNL_DOUBLE y,
			      PVNL_DOUBLE z,
			      PVNL_DOUBLE R );

/* spherical function c3*/
/** Returns 6th 3rd order spher. harm. func. value c3 (func. index 14)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> x^3-3*x*y^2 </CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalA33(PVNL_DOUBLE x,
			      PVNL_DOUBLE y,
			      PVNL_DOUBLE z,
			      PVNL_DOUBLE R );

/* spherical function s3*/
/** Returns 7th 3rd order spher. harm. func. value s3 (func. index 15)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> 3*x^2*y-y^3 </CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalB33(PVNL_DOUBLE x,
			      PVNL_DOUBLE y,
			      PVNL_DOUBLE z,
			      PVNL_DOUBLE R );

/* spherical function z4 */
/** Returns 1st 4th order spher. harm. func. value z4 (func. index 16)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> z^4-3*z^2*R^2+3/8 *R^4 </CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalA40(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z ,
			      PVNL_DOUBLE R );

/* spherical function z3x*/
/** Returns 2nd 4th order spher. harm. func. value z3x (func. index 17)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> z^3*x-(3/4)*z*x*R^2</CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalA41(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z ,
			      PVNL_DOUBLE R );


/* spherical function z3y*/
/** Returns 3rd 4th order spher. harm. func. value z3y (func. index 18)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE> z^3*y-(3/4)*z*y*R^2 </CODE>.
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalB41(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z ,
			      PVNL_DOUBLE R );

/* spherical function z2c2*/
/** Returns 4th 4th order spher. harm. func. value z2c2 (func. index 19)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE>z^2*(x^2-y^2)-(1/6)*(x^2-y^2)*R^2</CODE>
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalA42(PVNL_DOUBLE x,
			      PVNL_DOUBLE y,
			      PVNL_DOUBLE z,
			      PVNL_DOUBLE R);

/* spherical function z2s2*/
/** Returns 5th 4th order spher. harm. func. value z2s2 (func. index 20)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE>z^2*(2*x*y)-(1/6)*(2*x*y)*R^2</CODE>
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalB42(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z ,
			      PVNL_DOUBLE R );

/* spherical function zc3 */
/** Returns 6th 4th order spher. harm. func. value zc3 (func. index 21)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE>z*(x^3-3*x*y^2)</CODE>
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalA43(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z ,
			      PVNL_DOUBLE R );

/* spherical function zs3*/
/** Returns 7th 4th order spher. harm. func. value zs3 (func. index 22)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE>z*(3*x^2*y-y^3)</CODE>
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalB43(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z ,
			      PVNL_DOUBLE R );

  /* spherical function c4*/
/** Returns 8th 4th order spher. harm. func. value c4 (func. index 23)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE>x^4-6*x^2*y^2+y^4</CODE>
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalA44(PVNL_DOUBLE x,
			      PVNL_DOUBLE y,
			      PVNL_DOUBLE z ,
			      PVNL_DOUBLE R );

/* spherical function s4*/
/** Returns 9th 4th order spher. harm. func. value s4 (func. index 24)
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return <CODE>4*(x^3*y-y^3*x)</CODE>
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalB44(PVNL_DOUBLE x ,
			      PVNL_DOUBLE y ,
			      PVNL_DOUBLE z ,
			      PVNL_DOUBLE R );

/** Returns max order of spherical functions defined.
 */
DLL_INTERFACE int PvnlSphericalMaxOrder(void);

/** Returns max number of spherical functions defined.
*/
DLL_INTERFACE int PvnlSphericalMaxNFunc(void);

/** returns value of spherical function i. Index i is checked
    to lie within [0,max[ where max is defined by
    PvnlSphericalMaxNFunc. In case of illegal function index
    0.0 is returned.

 *  @param i index of spherical function
 *  @param x x coordinate
 *  @param y y coordinate
 *  @param z z coordinate
 *  @param R <CODE>sqrt(x*x+y*y)</CODE>
 *  @return value of ith spherical function at place x,y,z
*/
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalFuncVal( int i,
				  PVNL_DOUBLE x,
				  PVNL_DOUBLE y,
				  PVNL_DOUBLE z,
				  PVNL_DOUBLE R);

/** returns name of spherical function i. Index i is checked
    to lie within [0,max[ where max is defined by
    PvnlSphericalMaxNFunc. In case of illegal function index
    an empty string is returned.
 */
DLL_INTERFACE const char * PvnlSphericalFuncName(int i);

/** returns the normalization fractor dependent on convention.
    The norming convention is specified by argument <CODE>normConv</CODE>.
    - <CODE>normConv</CODE> = 0 : Coefficient of  \f$ z^{n-m}x^m \f$  equal one\n
    - <CODE>normConv</CODE> = 1 : Normalization like associated Legendre Polynomials.\n
    - <CODE>normConv</CODE> = 2 : Maximum on unit sphere equal one\n
    Index <CODE>spidx</CODE> is checked
    to lie within [0,max[ where max is defined by
    PvnlSphericalMaxNFunc. In case of illegal function index
    0.0 is returned. Convention <CODE>normConv</CODE> must have one of
    the values 0,1,2 otherwise 0.0 is returned. In other cases the return
    value multiplied with the value of the associated spherical function
    gives the spherical harmonic field in the desired convention.
    * @param spidx index of spherical function
    * @param normConv desired norming convention
    * @return multiplication factor to translate to the desired convention
 */
DLL_INTERFACE PVNL_DOUBLE PvnlSphericalNormFac(int spidx, int normConv);

/** returns array of normalization factors dependent on convention.
    The norming convention is specified by argument <CODE>normConv</CODE>.
    - <CODE>normConv</CODE> = 0 : Coefficient of  \f$ z^{n-m}x^m \f$  equal one\n
    - <CODE>normConv</CODE> = 1 : Normalization like associated Legendre Polynomials.\n
    - <CODE>normConv</CODE> = 2 : Maximum on unit sphere equal one\n
    Convention <CODE>normConv</CODE> must have one of
    the values 0,1,2 otherwise 0.0 is returned into all fields of <CODE>narr</CODE>
    * @param narr Double array of size PvnlSphericalMaxNFunc
    * @param normConv desired norming convention
    * @return nonzero value in case normConv has illegal values or narr is a 0 pointer
*/
DLL_INTERFACE int PvnlSphericalNormFacArr(PVNL_DOUBLE *narr,int normConv);


/** returns the order of spherical function.
    Index <CODE>sfind</CODE> is checked
    to lie within [0,max[ where max is defined by
    PvnlSphericalMaxNFunc. In case of illegal function index
    -1 is returned.
 * @param sfind index of spherical function
 * @return order of spherical function
*/
DLL_INTERFACE int PvnlSphericalOrd(int sfind);
/** returns the order of all spherical functions.
    On output, ith element of param <CODE>ord</CODE> stores the order of
    the ith spherical function.
 * @param  ord int array of size PvnlSphericalMaxNFunc
 * @return 0 if succeded, nonzero value if ord is a 0 pointer
 */
DLL_INTERFACE int PvnlSphericalOrdArr(int *ord);



//@}


#endif

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
