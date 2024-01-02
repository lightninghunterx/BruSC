/********************************************************************
 *
 * Copyright (c) 2005 - 2019
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ********************************************************************/

#ifndef PVNL_PVNLFT_H			/* prevent multiple includes */
#define PVNL_PVNLFT_H

/** \file
    \defgroup PvnlFT PvnlFT
 */

/*--------------------------------------------------------------*
 * Include files...
 *--------------------------------------------------------------*/

#include <common/recotyp.h>
#include <Pvnl/PvnlGeneral.h>
#include <Pvnl/PvnlMatrix.h>
#ifndef CPROTO
#include <vector>
#endif


#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

/// DLL interface libParx identifiers for Windows.
#if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
#  ifdef libPvnlFT_INTERNAL
#    define DLL_INTERFACE __declspec(dllexport)
#  else
#    define DLL_INTERFACE __declspec(dllimport)
#  endif
#elif defined (__GNUC__) && __GNUC__ >= 4
#    define DLL_INTERFACE __attribute__ ((visibility("default")))
#else
#    define DLL_INTERFACE
#endif 



  /* \{ */

  /*--------------------------------------------------------------*
   * Definitions...
   *--------------------------------------------------------------*/

  /*--------------------------------------------------------------*
   * Typedefs...
   *--------------------------------------------------------------*/

  /** Defines the canned Fourier transformation types.
   */
  typedef enum
  {
    /** Fourier transform of a real sequence. */
    PVNL_FT_REAL,
    /** A simplified real periodic transform. */
    PVNL_FT_EZ_REAL,
    /** Sine transform of a real odd sequence. */
    PVNL_FT_SINE,
    /** Cosine transform of a real even sequence. */
    PVNL_FT_COSINE,
    /** Quarter wave sine transform (odd wave numbers). */
    PVNL_FT_QW_SINE,
    /** Quarter wave cosine transform (odd wave numbers). */
    PVNL_FT_QW_COSINE,
    /** Fourier transform of a complex sequence. */
    PVNL_FT_COMPLEX
  } pvnlFTType;

  /** Definition of the Fourier transformation context.

      Usually the same FT context may be used for the subsequent transforms
      as long as the FT type PvnlFTPlan#type, data precision PvnlFTPlan#prec
      and sequence length PvnlFTPlan#n remain unchanged. In this case the
      subsequent transforms can be obtained faster than the first. The contents
      of the internal elements of PvnlFTPlan must not be destroyed between
      calls of the FT functions.
   */
  typedef struct PvnlFTPlanStruct
  {
    /** Real, complex, sine, cosine, etc. */
    pvnlFTType		type;
    /** Data precision (<CODE>PVNL_SINGLEPREC</CODE> or
	<CODE>PVNL_DOUBLEPREC</CODE>). */
    pvnlDataType	prec;
    /** The length of the sequence to be transformed. */
    long		n;
    /** Temporary buffer which may be needed if the input data
	are not contiguous. */
    void		*c;
    /** Work buffer used for the prime factorization of #n
	together with a tabulation of the trigonometric functions. */
    void		*wsave;
  } PvnlFTPlan;

  /* \} */

  /*--------------------------------------------------------------*
   * Function prototypes...
   *--------------------------------------------------------------*/

  /*--------------------------------------------------------------*
   * ...C wrappers
   *--------------------------------------------------------------*/

/* /home/anba/devel/HEAD/gen/src/lib/libPvnl/FT/fft.c */
DLL_INTERFACE int PvnlFTMixedRadixRealFTFloat(float *, int, int, int, int, PvnlFTPlan *);
DLL_INTERFACE int PvnlFTMixedRadixRealFTDouble(double *, int, int, int, int, PvnlFTPlan *);
DLL_INTERFACE int PvnlFTMixedRadixRealNonDestrFTFloat(float *, float *, float *, float *, int, int, int, int, PvnlFTPlan *);
DLL_INTERFACE int PvnlFTMixedRadixRealNonDestrFTDouble(double *, double *, double *, double *, int, int, int, int, PvnlFTPlan *);
DLL_INTERFACE int PvnlFTMixedRadixSineFTFloat(float *, int, int, PvnlFTPlan *);
DLL_INTERFACE int PvnlFTMixedRadixSineFTDouble(double *, int, int, PvnlFTPlan *);
DLL_INTERFACE int PvnlFTMixedRadixCosineFTFloat(float *, int, int, PvnlFTPlan *);
DLL_INTERFACE int PvnlFTMixedRadixCosineFTDouble(double *, int, int, PvnlFTPlan *);
DLL_INTERFACE int PvnlFTMixedRadixQWSineFTFloat(float *, int, int, int, PvnlFTPlan *);
DLL_INTERFACE int PvnlFTMixedRadixQWSineFTDouble(double *, int, int, int, PvnlFTPlan *);
DLL_INTERFACE int PvnlFTMixedRadixQWCosineFTFloat(float *, int, int, int, PvnlFTPlan *);
DLL_INTERFACE int PvnlFTMixedRadixQWCosineFTDouble(double *, int, int, int, PvnlFTPlan *);
DLL_INTERFACE int PvnlFTMixedRadixComplexFTFloat(float *, int, int, int, int, RECO_QOPTS_TYPE, PvnlFTPlan *);
DLL_INTERFACE int PvnlFTMixedRadixComplexFTDouble(double *, int, int, int, int, RECO_QOPTS_TYPE, PvnlFTPlan *);
DLL_INTERFACE PvnlFTPlan *PvnlFTMixedRadixCreateFTPlan(pvnlFTType, pvnlDataType, int);
DLL_INTERFACE void PvnlFTMixedRadixDeleteFTPlan(PvnlFTPlan *);
DLL_INTERFACE int PvnlFTRealShiftFloat(float *, int, int, int, float *);
DLL_INTERFACE int PvnlFTRealShiftDouble(double *, int, int, int, double *);
DLL_INTERFACE int PvnlFTComplexShiftFloat(float *, int, int, int, float *);
DLL_INTERFACE int PvnlFTComplexShiftDouble(double *, int, int, int, double *);
DLL_INTERFACE void PvnlMatrixMixedRadixDeleteFTPlan(PvnlFTPlan *);
#ifndef CPROTO
DLL_INTERFACE int PvnlMatrixMixedRadixFT(PvnlMatrix&, unsigned, int, int, RECO_QOPTS_TYPE, PvnlFTPlan *);
DLL_INTERFACE PvnlFTPlan *PvnlMatrixMixedRadixCreateFTPlan(const PvnlMatrix&, unsigned);
DLL_INTERFACE int PvnlMatrixShift(PvnlMatrix&, unsigned, int, void *);
DLL_INTERFACE int PvnlFTMixedRadixNdComplexFTDouble(double *, std::vector<int>, int, std::vector<PvnlFTPlan*>);
DLL_INTERFACE int PvnlFTMixedRadixNdCosineFTDouble(double *, std::vector<int>, pvnlFieldType, int, std::vector<PvnlFTPlan*>);
DLL_INTERFACE std::vector<PvnlFTPlan*> PvnlFTMixedRadixNdCreatePlanDouble(pvnlFTType, std::vector<int>);
DLL_INTERFACE void PvnlFTMixedRadixNdDeletePlanDouble(std::vector<PvnlFTPlan*>);
#endif

#endif	/* PVNL_PVNLFT_H */

/****************************************************************/
/*	E N D   O F   F I L E					*/
/****************************************************************/
