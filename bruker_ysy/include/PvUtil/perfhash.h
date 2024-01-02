/****************************************************************
 *
 * $Source$
 *
 * Copyright (c) 2006-2014
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 * Some definitions in this file are not copyrighted by 
 * Bruker BioSpin MRI GmbH:
 * perfect.h: code to generate code for a hash for perfect hashing.
 * (c) Bob Jenkins, September 1996
 * You may use this code in any way you wish, and it is free.  No warranty.
 * I hereby place this in the public domain.
 * Source is http://burtleburtle.net/bob/c/perfect.h
 *
 * The creation of the perfect hash is derived from
 * perfect.c: code to generate code for a hash for perfect hashing.
 * (c) Bob Jenkins, September 1996, December 1999
 * You may use this code in any way you wish, and it is free.  No warranty.
 * I hereby place this in the public domain.
 * Source is http://burtleburtle.net/bob/c/perfect.c
 *
 * Changes by Andreas Barth, Bruker BioSpin MRI GmbH to adapt it
 * for:
 *   - Only use strings as hash keys
 *   - Only slow  creation of (minimal) perfect hash table
 *   - Only using initial hash 
 *     hash = PHASHSALT;
 *     for (i=0; i<keylength; ++i) {
 *       hash = (hash ^ key[i]) + ((hash<<26)+(hash>>6));
 *     }
 *   - ANSI C
 *   - library usage
 *
 *   First changes on 20060223.
 *
 ****************************************************************/
   
#ifndef PVUTIL_PERFHASH_H
#define PVUTIL_PERFHASH_H
   
#ifndef __STDC_LIMIT_MACROS
/// Use limit macro definition from stdint.h
#define __STDC_LIMIT_MACROS
#endif
 
#include <stdint.h>
#include <stdio.h>

#include "PvUtil/DLLMacro.h"

/** @file
 *  <p>
 *  The utility library contains definitions of functions for a 
 *  creating a perfect hash.
 *  The implementation of this perfect hash is from
 *  http://www.burtleburtle.net/bob/hash/perfect.html . It is derived from</p>
 *  <pre>
 *  perfect.c: code to generate code for a hash for perfect hashing.
 *  (c) Bob Jenkins, September 1996, December 1999
 *  You may use this code in any way you wish, and it is free.  No warranty.
 *  I hereby place this in the public domain.
 *  Source is http://burtleburtle.net/bob/c/perfect.c
 *  </pre>
 *
 *  @addtogroup PvUtil 
 */
/*@{*/


/** Definition of a 4-byte quantities type. */
typedef uint32_t ub4;   

/** Number of bits of type <code>ub4</code> */
#define UB4BITS 32

/** Definition of 2-byte quatities type. */
typedef uint16_t  ub2;
/** Number of bits of type <code>ub2</code> */
#define UB2MAXVAL UINT16_MAX

/** Definiiton of 1-byte quantity type. */
typedef uint8_t ub1;
/** Number of bits of tzpe <code>ub1</code> */
#define UB1MAXVAL UINT8_MAX

/** Definition to find a perfect or minimal perfect hash function. */
typedef enum 
{
  /** Find a perfect hash function (not necessary minimal). */
  NORMAL_HP,                                  
  /** Find a minimal perfect hash function. */
  MINIMAL_HP      
} PvUtilHashType;


/** Opaque definition of the hash. Is used in all perfect hash functions. */
struct pvutil_hash_def;
/** Hash definition type. */
typedef  struct  pvutil_hash_def PvUtilHashDef;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


  /** Find a (minimal) perfect hash for given keys.
   *  Tries to calculate a (minimal) perfect hash and returns the definition
   *  of this hash. This definition must be freed with 
   *  <code>PvUtilFreeHashDef</code>. The minimal perfect hash method is 
   *  described and code used from 
   *  http://www.burtleburtle.net/bob/hash/perfect.html .
   *  \param keys array of string keys for which the hash should be generated.
   *  \param nkeys number of keys int the <code>keys</code> array.
   *  \param hashType find perfect of minimal perfect hash?
   *  \return 0 if it was not possible to find a hash else the
   *      definition of the hash.
   *  \see PvUtilCalcPerfectHashValue
   *  \see PvUtilWriteHashFunctionCode
   *  \see PvUtilGetHashrange
   *  \see PvUtilFreeHashDef
   */
  DLL_INTERFACE PvUtilHashDef * 
  PvUtilFindPerfectHash(const char **keys, ub4 nkeys, 
                        PvUtilHashType hashType );

  /** Calculate perfect hash value for a hash definition.
   *  The perfect hash value is calculated for a given string 
   *  <code>name</code> using a hash definition <code>hashdef</code> found
   *  by the function <code>PvUtilFindPerfectHash</code>. If <code>name</code>
   *  is not in the list of the original key strings used in 
   *  <code>PvUtilFindPerfectHash</code> the returned hash value may be 
   *  arbitrary. This means that is is necessary to test if the returned hash
   *  value really belongs to the key (e.g. one string compare between key 
   *  and <code>name</code>.
   *  \param hashdef hash definition returned by 
   *                 <code>PvUtilFindPerfectHash</code>.
   *  \param name string to calculate hash value for.
   *  \return calculated hash value for <code>name</code>. The hash value is 
   *          the range 0..return value of <code>PvUtilGetHashrange</code>.
   *  \see PvUtilFindPerfectHash
   *  \see PvUtilGetHashrange
   */
  DLL_INTERFACE ub4 PvUtilCalcPerfectHashValue(const PvUtilHashDef * hashdef, 
                                               const char * name);

  /** Write C code for the found hash (minimal) perfect hash to a file.
      This code writes a C function to a file containing a hash function
      calculating the (minimal) perfect hash defined in the 
      <code>hashdef</code> returned by <code>PvUtilFindPerfectHash</code>.
      The code has the form:
      <pre>
      
      \#include "PvUtil/perfhash.h"
    
      static ubX tab[] = ....
    
      {fcnSpecifier} ub4 {fcnPrefix}phash(const char * keystr)
      { 
       ...
      }
      </pre>
      <code>ubX</code> is the type <code>ub1</code> or <code>ub2</code>.
      For example <code>fcnSpecifier</code> may be the word <code>static</code>
      and <code>fcnPrefix</code> may be for C++ a class specifier 
      (<code>ClassName::phash</code>). 
      \param f File handle to write hash calculated function code to.
      \param hashdef definition of (minimal) perfect hash returned by
                     <code>PvUtilFindPerfectHash</code>.
      \param fcnSpecifier hash function specifier. The specifier may
                          be 0 in which case it is left out.
      \param fcnPrefix hash name prefix. The specifier may
                          be 0 in which case it is left out.
      \see PvUtilFindPerfectHash
   */
  DLL_INTERFACE void PvUtilWriteHashFunctionCode(FILE * f, 
                                                 const PvUtilHashDef * hashdef,
                                                 const char * fcnSpecifier, 
                                                 const char * fcnPrefix);

  /** Returns range of hash values for a given hash definition.
   *  \param hashdef definition of (minimal) perfect hash returned by
   *                 <code>PvUtilFindPerfectHash</code>.
   *  \return range of hash values, the range is from 0 to the
   *          returned value.
   *  \see PvUtilFindPerfectHash
   */
  DLL_INTERFACE ub4 PvUtilGetHashrange(const PvUtilHashDef * hashdef);

  /** Frees memory of a hash definition.
    *  \param hashdef definition of (minimal) perfect hash returned by
    *                 <code>PvUtilFindPerfectHash</code>.
    */
  DLL_INTERFACE void PvUtilFreeHashDef(PvUtilHashDef * hashdef);

#ifdef __cplusplus
}
#endif

/*@}*/

#endif /* PVUTIL_PERFHASH_H */
