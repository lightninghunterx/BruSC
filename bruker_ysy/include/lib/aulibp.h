/*
 *******************************************************************
 *
 * $Source: /pr/CvsTree/pr/gen/h/lib/aulibp.h,v $
 *
 * Copyright (c) 1996
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 *
 * $Id: aulibp.h,v 1.22 2018/02/26 14:08:45 alexander.herbig Exp $
 *
 *******************************************************************
*/


#ifndef AULIBP_H		/* prevent multiple includes */
#define AULIBP_H

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

#if defined(DLL_LINKAGE) && !defined(CPROTO)
#   ifdef aulibp_INTERNAL
#      define DLL_INTERFACE __declspec(dllexport)
#   else
#      define DLL_INTERFACE __declspec(dllimport)
#   endif
#else
#   define DLL_INTERFACE
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* stobaypar.c */
extern DLL_INTERFACE int fetchstorbay (const char *, int, int, const char *, const char *, char *);
/* fetchstot1.c */
extern DLL_INTERFACE int fetchstort1 (const char *, int, const char *, ...);
/* fetchstodosy.c */
extern DLL_INTERFACE int fetchstordosy (const char *, int, const char *, ...);
/* fetchstocommon.c */
extern DLL_INTERFACE int fetchstorcommon(const char*, int, int, const char*, ...);
/* peaklistC.c */
extern DLL_INTERFACE int readPeakList(const char*);
extern DLL_INTERFACE int getPeakAddress(unsigned int);
extern DLL_INTERFACE int getPeakAddressForDim(unsigned int, unsigned int);
extern DLL_INTERFACE int getPeakType(unsigned int);
extern DLL_INTERFACE double getPeakFreqPPM(unsigned int);
extern DLL_INTERFACE double getPeakFreqHz(unsigned int);
extern DLL_INTERFACE double getPeakFreqPPMForDim(unsigned int, unsigned int);
extern DLL_INTERFACE double getPeakFreqHzForDim(unsigned int, unsigned int);
extern DLL_INTERFACE double getLeftPeakLimit(void);
extern DLL_INTERFACE double getRightPeakLimit(void);
extern DLL_INTERFACE double getPeakIntensity(unsigned int);
extern DLL_INTERFACE double getPeakVolume(unsigned int);
extern DLL_INTERFACE double getPeakVolumeError(unsigned int);
extern DLL_INTERFACE double getPeakQuality(unsigned int);
extern DLL_INTERFACE const char* getPeakAnnotation(unsigned int);
extern DLL_INTERFACE void setPeakAnnotation(unsigned int, const char*);
extern DLL_INTERFACE int freePeakList(void);
extern DLL_INTERFACE const char* getPeakError(void);
extern DLL_INTERFACE int getPeakDimension(void);
#ifdef __cplusplus
}
#endif
#endif /* AULIBP_H */
