//===============================================================
//
// Copyright (c) 1995-2018
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//
//===============================================================

#ifndef PVUTIL_HASHER_HH
#define PVUTIL_HASHER_HH

/**
 * @file
 * @addtogroup PvUtil
 * @{
 */


namespace PvUtil
{

    /**
     * @class Hasher Hasher.hh "PvUtil/Hasher.hh"
     * Class to define a hash function for 
     * std::unordered_map or std::unordered_set.
     * The operator calls the \c hashCode method of the key class.
     * @param key The key class
     */
    template<class Key> class Hasher
    {
    public:

        /**
         * Hash function.
         * @param key Key object.
         * @return The hash key of the key object. 
         */
        size_t operator()(const Key & key) const
        {
            return key.hashCode();
        }
    };

    /**
     * @class IndirectHasher Hasher.hh "PvUtil/Hasher.hh"
     * Class to define a hash function for pointer that uses the pointer value.
     */
    template<class Key> class IndirectHasher
    {
    public:

        /**
         * Hash function.
         * @param key Pointer to key object.
         * @return The hash key of the key object. 
         */
        size_t operator()(const Key * key) const
        {
            if (key == 0)
                return 0;
            else
                return key->hashCode();
        }
    };

    /**
     * @class IndirectEqual Hasher.hh "PvUtil/Hasher.hh"
     * Class to define an equal operator for pointer.
     */
    struct IndirectEqual
    {

        /**
         * Equal operator testing if pointer is equal or content is equal.
         * @param lhs First compare argument.
         * @param rhs Second compare argument.
         * @return \c true if both argument pointer are equal of the contents
         *   are equal.
         */ 
        template< typename X, typename Y >
        bool operator()(X const &lhs, Y const &rhs) const
        {
            return lhs == rhs || *lhs == *rhs;
        }
    };
    
    /**
     * @class EnumClassHash Hasher.hh "PvUtil/Hasher.hh"
     * Class to define a hash function for an enum class.
     */
    struct EnumClassHash
    {
        /**
         * Operator to get a value of the underlying type from an enum class.
         * This value can be used as hash value for enumeration keys.
         * @param T enumeration class type.
         * @param t The enumeration value.
         * @return The enumeration value in the underlying type.
         */
        template <typename T>
        typename std::underlying_type<T>::type operator()(T t) const
        {
            return static_cast<typename std::underlying_type<T>::type>(t);
        }
    };

}

/*@}*/


#endif /* PVUTIL_HASHER_HH */

