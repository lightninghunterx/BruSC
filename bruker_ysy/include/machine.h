/*
*******************************************************************
*
* $Source: /bscl/CvsTree/bscl/gen/h/machine.h,v $
*
* Copyright (c) 1997
* BRUKER BIOSPIN GMBH
* D-76287 Rheinstetten, Germany
*
* All Rights Reserved
*
* $Id: machine.h,v 1.97 2019/05/15 13:16:21 Marina.Maurer Exp $
*
*******************************************************************
*/


#ifndef MACHINE_H		/* prevent multiple includes */
#define MACHINE_H


/*:=INFO=:**************************************************************
*
* Header : machine.h
*
* Description : contains Machine, OS and Compiler specific definitions
*
* Following Operation Systems are handled:
* - Linux Intel
* - Linux Blade, Cube, Yocto
* - MAC Intel
* - Windows NT
*
* In the current version this file contains 2 parts:
* 1.part: has all machine and OS specific definitions. Depending
*         on compiler built in flags a compiler definition
*	  is defined (e.g. ANSI_COMPILER).
* 2.part: has all compiler specific definitions depending on the
*	  compiler definition.
*
* Notes:
* 1. it must always be the first header file to be included !!!
* 2. if none of the OS flags is defined a syntax error will occur.
*
*::=info=:*************************************************************/

/******************************************************************/
/******************************************************************/
/**********                                               *********/
/**********  Part 1: Machine and OS specific definitions  *********/
/**********                                               *********/
/******************************************************************/
/******************************************************************/


/*
*******************************************************************
*
* Platform: Linux on Intel
*
*******************************************************************
*/

#ifdef LINUX_INTEL
#	undef LINUX_INTEL
#	define found_os

#	define HAS_UNIX_FEATURES
#	define HAS_LINUX_FEATURES
#	define HAS_NO_WAIT2		/* has no "int wait2(int*, int)"  */
#	define HAS_NO_GAMMA		/* no double gamma(3M)	*/
#	define HAS_NO_CFTIME		/* no int cftime(3C)	*/
#	define HAS_NO_STD_FILESYSTEM	/* build for MRI links dynamically, but experimental libstdc++fs is only available as static lib */
#	define HAS_ELF_FILESYSTEM	/* include <elf.h>	*/
#	define HAS_BROKEN_DIRNAME	/* dirname(libc) is broken in RH-7.1 */
#	define STATFS_H <sys/vfs.h>

#	define NULLDEVICE "/dev/null"
#	define LDLIBRARYPATH "LD_LIBRARY_PATH"
#	define EXECUTABLE_EXTENSION ""
#	define SEARCH_PATH_SEPARATOR ":"

#	ifdef  __LP64__
#		define HAS_64BIT_FEATURES	/* 64 bit OS */
#		define DOUBLE_ALIGN_SIZE 8	/* Alignment of double */
#	else
#		define DOUBLE_ALIGN_SIZE 4	/* Alignment of double */
#	endif

#	define BITFIELD_BIT0_FIRST	/* Bit 0 is first entry of bitfield */
					/* for conversion of external bytes */

#	define HAS_PAM		/* pluggable authentication modules	*/
#	define USE_CUPS		/* Common Unix Printing System */
#	define USE_REMOTE_RSDEVICE	/* RS232 rrs-server access	*/
#	define HAS_DHCPD_CONTROL	/* libdhcpctl.a can query DHCP leases */

/*---------------------------
 * acquisition dependencies
 *--------------------------*/
#	define HAS_PTYLIST		/* step list for pty's */
#	define HAS_RRS_CLIENT

#endif /* LINUX_INTEL */


/*
*******************************************************************
*
* Platform: Linux on Blade with Intel 64 bit CPU
*
*******************************************************************
*/

#ifdef LINUX_BLADE
#	undef LINUX_BLADE
#	define found_os

#	define HAS_UNIX_FEATURES
#	define HAS_LINUX_FEATURES
#	define HAS_BLADE_FEATURES
#	define HAS_NO_CPR

#	define HAS_NO_WAIT2		/* has no "int wait2(int*, int)"  */
#	define HAS_NO_GAMMA		/* no double gamma(3M)	*/
#	define HAS_NO_CFTIME		/* no int cftime(3C)	*/
#	define HAS_ELF_FILESYSTEM	/* include <elf.h>	*/
#	define STATFS_H <sys/vfs.h>

#	define NULLDEVICE "/dev/null"
#	define LDLIBRARYPATH "LD_LIBRARY_PATH"
#	define EXECUTABLE_EXTENSION ""
#	define SEARCH_PATH_SEPARATOR ":"

#	define HAS_64BIT_FEATURES	/* 64 bit OS */
#	define DOUBLE_ALIGN_SIZE 8	/* Alignment of double */

#	define BITFIELD_BIT0_FIRST	/* Bit 0 is first entry of bitfield */
					/* for conversion of external bytes */

#	define HAS_DHCPD_CONTROL	/* libdhcpctl.a can query DHCP leases */

/*---------------------------
 * acquisition dependencies
 *--------------------------*/
#	define HAS_PTYLIST		/* step list for pty's */

#endif /* LINUX_BLADE */


/*
*******************************************************************
*
* Platform: Linux on Cube with ARM 32 bit CPU
*
*******************************************************************
*/

#ifdef LINUX_CUBE
#	undef LINUX_CUBE
#	define found_os

#	define HAS_UNIX_FEATURES
#	define HAS_LINUX_FEATURES
#	define HAS_CUBE_FEATURES
#	define HAS_NO_CPR

#	define HAS_NO_WAIT2		/* has no "int wait2(int*, int)"  */
#	define HAS_NO_GAMMA		/* no double gamma(3M)	*/
#	define HAS_NO_CFTIME		/* no int cftime(3C)	*/
#	define HAS_NO_STD_FILESYSTEM	/* gcc version 4.8.5 does not support std filesystem */
#	define HAS_ELF_FILESYSTEM	/* include <elf.h>	*/
#	define STATFS_H <sys/vfs.h>

#	define NULLDEVICE "/dev/null"
#	define LDLIBRARYPATH "LD_LIBRARY_PATH"
#	define EXECUTABLE_EXTENSION ""
#	define SEARCH_PATH_SEPARATOR ":"

#	define DOUBLE_ALIGN_SIZE 4	/* Alignment of double */

#	define BITFIELD_BIT0_FIRST	/* Bit 0 is first entry of bitfield */
					/* for conversion of external bytes */
#	define HAS_NO_CRYPT

#endif /* LINUX_CUBE */

/*
*******************************************************************
*
* Platform: Linux on Yocto
* Some defines depends on the target machine (e.g. ARM or x86-64)
* and of the TopSpin variant that is being build (for blade, cube
* or CMR).
* Those are present here for reference but commented out.
* They are included in the bscl/gen/h/machine_yocto.h file
* that is generated on the fly by the Yocto recipe.
*
*******************************************************************
*/

#ifdef LINUX_YOCTO
#	undef LINUX_YOCTO
#	define found_os

#	define HAS_UNIX_FEATURES
#	define HAS_LINUX_FEATURES
#	define HAS_NO_CPR


#	define HAS_NO_WAIT2		/* has no "int wait2(int*, int)"  */
#	define HAS_NO_GAMMA		/* no double gamma(3M)	*/
#	define HAS_NO_CFTIME		/* no int cftime(3C)	*/
#	define HAS_NO_STD_FILESYSTEM	/* gcc version 4.8.5 does not support std filesystem */
#	define HAS_ELF_FILESYSTEM	/* include <elf.h>	*/
#	define STATFS_H <sys/vfs.h>

#	define NULLDEVICE "/dev/null"
#	define LDLIBRARYPATH "LD_LIBRARY_PATH"
#	define EXECUTABLE_EXTENSION ""
#	define SEARCH_PATH_SEPARATOR ":"


#	define BITFIELD_BIT0_FIRST	/* Bit 0 is first entry of bitfield */
					/* for conversion of external bytes */

#	define HAS_NO_CRYPT

#	define HAS_DHCPD_CONTROL	/* libdhcpctl.a can query DHCP leases */

/* All the values below this line are set by machine_yocto.h */
#   include "machine_yocto.h"
/*
 * #   define HAS_BLADE_FEATURES
 * #   define HAS_CUBE_FEATURES
 * #   define HAS_CMR_FEATURES
 * #	define DOUBLE_ALIGN_SIZE 4
 */

#endif /* LINUX_YOCTO */

/*
*******************************************************************
*
* Platform: MacOS X on Apple with Intel CPU
*
*******************************************************************
*/

#ifdef MAC_INTEL
#	undef MAC_INTEL
#	define found_os

#	define HAS_UNIX_FEATURES
#	define HAS_MAC_FEATURES
#	define HAS_64BIT_FEATURES	/* 64 bit OS */
#	define HAS_NO_SIGTIMEDWAIT
#	define HAS_NO_STD_FILESYSTEM	/* gcc-5.4.0 does not support std filesystem */
#	define STATFS_H <sys/mount.h>

#	define NULLDEVICE "/dev/null"
#	define LDLIBRARYPATH "DYLD_LIBRARY_PATH"
#	define EXECUTABLE_EXTENSION ""
#	define SEARCH_PATH_SEPARATOR ":"
#	define DOUBLE_ALIGN_SIZE 8	/* Alignment of double */

#	define BITFIELD_BIT0_FIRST	/* Bit 0 is first entry of bitfield */
					/* for conversion of external bytes */
#	define USE_CUPS			/* Common Unix Printing System */
#	define HAS_DHCPD_CONTROL	/* libdhcpctl.a can query DHCP leases */


#ifndef SIGCLD
#   define SIGCLD SIGCHLD
#endif

#endif /* MAC_INTEL */



/*
*******************************************************************
*
* Platform: Windows NT
*
*******************************************************************
*/

#ifdef WIN_NT
#	undef WIN_NT
#	define found_os

#	define HAS_WINDOWS_FEATURES
#	define HAS_PURE_WINDOWS_FEATURES
#	define HAS_BROKEN_CHRONO_DURATION_DOUBLE	/* compiler error when using std::chrono::duration<double> */
#	define HAS_NO_CFTIME		/* no int cftime(3C)	*/
#	define HAS_NO_PTHREADS		/* has no pthreads(3THR)*/
#	define HAS_NO_DIRECTORY_STICKY	/* t access bit for dir	*/
#	define HAS_UNLIMITED_SHM
#	define HAS_BACKSLASH	/* uses backslash as path separator*/
#	define HAS_NO_CON_OUTPUT
#	define HAS_STRLWR
#	define HAS_STRUPR
#	define HAS_STRNICMP
#	define HAS_NO_DRAND48		/* no double drand48()	*/
#	define HAS_NO_LOCALTIME_R	/* has no localtime_r(3)*/
#	define HAS_NO_GAMMA		/* no double gamma(3M)	*/
#	define HAS_NO_HSTRERROR
#	define HAS_NO_STRCASECMP
#	define HAS_NO_RINT		/* no double rint(3M)	*/
#	define HAS_NO_DIRNAME		/* no dirname(libc)     */
#	define HAS_NO_BASENAME		/* no basename(libc)    */
#	define HAS_NO_SIGSET_T
#	define HAS_IO_H
#	define HAS_STRING_H
#	define NULLDEVICE "NUL"
#	define EXECUTABLE_EXTENSION ".exe"
#	define SEARCH_PATH_SEPARATOR ";"
#	define DOUBLE_ALIGN_SIZE 8	/* Alignment of double */

#	define BITFIELD_BIT0_FIRST	/* Bit 0 is first entry of bitfield */
					/* for conversion of external bytes */
#	define HAS_DHCPD_CONTROL	/* libdhcpctl.a can query DHCP leases */

#	define MSVC_PLUSPLUS_COMPILER
#	define found_compiler		/* VC++ without /Za	*/
#	ifndef NOMINMAX
#	    define NOMINMAX		/* disable Windef.h min, max definitions */
#	endif
/*---------------------------
 * acquisition dependencies
 *--------------------------*/
#	define HAS_RRS_CLIENT

#endif /* WIN_NT */

/*
*******************************************************************
*
* if none of the above listed OS specific flags is defined, force
* the compiler to stop with a syntax error:
*
*******************************************************************
*/
#ifndef found_os
#	error no OS specific flag (e.g. LINUX_INTEL) defined !!!
#else
#	undef found_os
#endif


/*------------------------------------------------------------------
* if no compiler was defined, use __STDC__, __cplusplus and __GCC__
* to find out what compiler we are using.
* Note: do not use the compiler definitions directly !!!
*-----------------------------------------------------------------*/
#ifndef found_compiler
#	ifdef __cplusplus
#		ifdef __GNUC__
#			define GNU_PLUSPLUS_COMPILER
#		endif
#	else
#		ifdef __STDC__
#			ifdef __GNUC__
#				define GNU_COMPILER
#			else
#				define ANSI_COMPILER
#			endif
#		endif
#	endif
#else
#	undef found_compiler
#endif

/******************************************************************/
/******************************************************************/
/**********                                               *********/
/**********     Part 2: Compiler specific definitions     *********/
/**********                                               *********/
/******************************************************************/
/******************************************************************/

/******************************************************************
 * General Information about C/C++
 * -------------------------------
 *
 * The current standard for Programming Language C is ISO/IEC 9899:1999,
 * published 1999-12-01, in short called "C99".
 *
 * new in C99:
 * - variable length arrays on the stack, e.g.:
 *   char array[non_const];
 *   If the compiler does not support this feature, then
 *   HAS_NO_VAR_LEN_ARRAY is defined.
 *
 * - library function snprintf(3S)
 *   If the libc does not contain this function, then
 *   HAS_NO_SNPRINTF is defined.
 *   If the libc does contain this function, but it does not work
 *   according to the C99 standard, then
 *   SNPRINTF_IS_NOT_C99 is defined.
 *
 * - library function vsnprintf(3S)
 *   If the libc does not contain this function, then
 *   HAS_NO_VSNPRINTF is defined.
 *
 ******************************************************************/

/* signal server argument list definition */

#ifndef SIGSERVARG
#   ifdef __cplusplus
#	define SIGSERVARG int
#   else
#	ifdef __GNUC__
#	    define SIGSERVARG int unused __attribute__((unused))
#	else
#	    define SIGSERVARG void
#	endif
#   endif
#endif


/*
*******************************************************************
*
* Compiler: ANSI C
*
*******************************************************************
*/

#ifdef ANSI_COMPILER
#	undef ANSI_COMPILER

#	define inline			/* mask C++ keyword  */
#	define restrict			/* mask C99 keyword 'restrict' */

#endif

/*
*******************************************************************
*
* Compiler: GNU C
*
*******************************************************************
*/

#ifdef GNU_COMPILER
#	undef GNU_COMPILER

#	define restrict			/* mask C99 keyword 'restrict' */

#endif

/*
*******************************************************************
*
* Compiler: Microsoft Visual C++
*
*******************************************************************
*/

#ifdef MSVC_PLUSPLUS_COMPILER
#	undef MSVC_PLUSPLUS_COMPILER

#	ifndef __cplusplus
#		define inline		/* no inline for C code */
#	endif

#	define restrict			/* mask C99 keyword 'restrict' */

#	ifdef _WIN64
#		define HAS_64BIT_FEATURES       /* 64 bit OS */
#		define BSCL_SSIZE_T __int64
#	else
#		define BSCL_SSIZE_T int
#	endif
#endif


/*
*******************************************************************
*
* Compiler: GNU C++
*
*******************************************************************
*/

#ifdef GNU_PLUSPLUS_COMPILER
#	undef GNU_PLUSPLUS_COMPILER

#	define restrict			/* mask C99 keyword 'restrict' */

#endif


/***********************************************************************/
/*			     E N D   O F   F I L E                     */
/***********************************************************************/

#endif /* MACHINE_H */
