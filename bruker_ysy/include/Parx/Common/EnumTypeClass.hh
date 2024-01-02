
//===============================================================
//
// $Source$ 
//    Class for a enum type (value set, etc).
//
// Copyright (c) 2011 - 2016
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//
//===============================================================

#ifndef PARX_COMMON_ENUMTYPE_HH
#define PARX_COMMON_ENUMTYPE_HH

//---------------------------------------------------------------
// includes
//---------------------------------------------------------------
#include <vector>
#include <string>


//---------------------------------------------------------------
// interface of class ParamInterface
//---------------------------------------------------------------

/**
   @ingroup PvParx
*/
namespace Parx
{
    namespace Common
    {

        /**
           @ingroup CommonInterface 
           @{
        */
        /** @class EnumType EnumTypeClass.hh "Parx/Common/EnumTypeClass.hh"
            An interface for enumeration dynamic type value sets and definitions.
         */
        class EnumType
        {
        public:

            /// Destructor.
            virtual ~EnumType() = default;

            /** Get the value set of this enum type.
                @param[out] valSet The value set array.
            */
            virtual void getValueSet(
                std::vector<std::string> & valSet) const = 0;

            /** Get number of enumeration values.
                @return Number of enumeration values.
            */
            virtual int getValueCnt() const = 0;

            /** Get the enumeration string  at a given index.
                @param[in] ndx The enumeration value as index.
                @return The enumeration value as string.
             * @throws PvException if no value for the given index is found.
            */
            virtual const char * getStringValue(int ndx) const = 0;

            /** Get enumeration display string at a given index.
                @param[in] ndx The enumeration value as index.
                @return The enumeration value as display string.
             * @throws PvException if no value for the given index is found.
            */
            virtual const char * getDisplayValue(int ndx) const = 0;

        };
    }
}

/*@}*/

#endif

