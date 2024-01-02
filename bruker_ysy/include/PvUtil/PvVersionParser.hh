//===============================================================
//
// Copyright (c) 2017 - 2018
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//
//===============================================================

#ifndef PVUTIL_PVVERSIONCLASS_HH
#define PVUTIL_PVVERSIONCLASS_HH

#ifndef CPROTO

#include <string>
#include <map>
#include "lib/libPvVersion.h"
#include "PvUtil/DLLMacro.h"

namespace PvUtil
{

    /** 
     * @file
     * @addtogroup PvUtil
     * @{
     */

    /** 
     * Class to parse a ParaVison version string.
     */
    class DLL_INTERFACE PvVersionParser
    {
    public:
        /**
         * Constructor.
         * Parse a version string as created through 
         * <code>PvGetVersionString</code>.
         * @param versionString The string to disassemble.
         * @throws PvException if the given \a versionString cannot be parsed into
         * a version class.
         */
        PvVersionParser(const std::string & versionString);

        /// Destructor
        ~PvVersionParser();
        
        /**
         * Get the product name.
         * @return The product name, empty if not set.
         */
        const std::string & getProduct() const;
        
        /**
         * Get the short product name.
         * @return The short product name, empty is not set.
         */
        const std::string & getShortProduct() const;

        /**
         * Get the version name (e.g. ICON or MPI).
         * @return The version name, may be empty.
         */
        const std::string & getName() const;

        /** 
         * Get the major version number.
         * @return The major version number.
         */
        int getMajor() const;

        /**
         * Get the minor version number.
         * @return The minor version number.
         */
        int getMinor() const;

        /** 
         * Get the ParaVision revision number.
         * @return The ParaVision revision number, if absent set then \c 0.
         */
        int getRevision() const;
        
        /**
         * Map of version types and type numbers.
         *
         * A version can have several types and type numbers (e.g. Beta and RC).
         * These values are administered in this map.
         */
        typedef std::map<PvVersionType, int> TypeNumberMap;

        /**
         * Get the ParaVision version type (e.g. Alpha, Beta, Release, ...).
         * @return The ParaVision version type. 
         */
       const TypeNumberMap & getTypes() const;
       
       /**
        * Check if the version has the given type.
        * @param vt Type to check.
        * @return \c true if version has the given type.
        */
       bool hasType(PvVersionType vt) const;

       /// Indicates if version is a release version. 
       bool isRelease() const;
       
       /**
        * Get the number for a given type.
        * @param vt The type.
        * @return The number for a given number (e.g. the alpha version number),
        *   -1 if the version has not the given type.
        */
       int getTypeNumber(PvVersionType vt) const;
        
       
        /**
         * The compile time if set.
         * @return The compile time, 0 is compile time is not set.
         */
        time_t getCommitTime() const;
        
        /**
         * Get the string representation of the compile time.
         * @return The string representation of the compiler, empty if the 
         * compile time is not set in the version.
         */
        std::string getCommitTimeString() const;
        
        
        /**
         * Get the version display string with the given product.
         * @return The version display string including the product.
         */
        std::string getDisplayString() const;
        
        /**
         * Get a string representation of the version.
         * @param withProduct Indicate if product name should be included.
         *    If there is not product name in the version the default
         *    product name is used.
         * @return A string representation of the version.
         */
        std::string getString(bool withProduct) const;
        
        /**
         * Get a version type string representation.
         * @param vt The version type.
         * @return Return the string representation of the version type.
         */
        static const char * getTypeString(PvVersionType vt);
        

    private:
        PvVersionParser(const PvVersionParser &) = delete;
        PvVersionParser& operator=(const PvVersionParser &) = delete;


        /** Anonymous implementation structure. */
        struct Impl;

        /** The implementation object. */
        Impl * pimpl_;

    };

    /** @} */

}

#endif /* CPROTO */
#endif /* PVUTIL_PVVERSIONCLASS_HH */

