//==============================================================================
// Copyright (c) 2000 - 2019
//
// Bruker BioSpin AG
// MRI Division
// CH-8117 Fällanden, Switzerland
//
// All Rights Reserved
//==============================================================================
#ifndef LIB_CORE_MATH_MATH_HH
#define LIB_CORE_MATH_MATH_HH

#ifndef CPROTO

#include <cstdint>

#ifdef HAS_PURE_WINDOWS_FEATURES
// To get constants like M_PI
#define  _USE_MATH_DEFINES
#endif

#include <cmath>
#include <algorithm>
#include <type_traits>

namespace core
{
    //--------------------------------------------------------------------------
    // Interface of class Math
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Math/Math.hh"
     * A static class with some math functions.
     *
     * @author Christian Werner
     * @since  ParaVision 6.Alpha.0.72
     */
    class Math
    {
    public:

        /** Get the maximum of a range of values. */
        template<typename FIRST, typename... REST>
        static inline typename std::common_type<FIRST, REST...>::type Maximum(
                FIRST a, REST... b)
        {
            return std::max(a, std::max(b...));
        }

        /** Get the minimum of a range of values. */
        template<typename FIRST, typename... REST>
        static inline typename std::common_type<FIRST, REST...>::type Minimum(
                FIRST a, REST... b)
        {
            return std::min(a, std::min(b...));
        }

        /**
         * Get next larger power of two.
         * Returns the smallest power of 2 which is <code>&gt;= value</code>.
         * Implementation source:
         * http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
         */
        static inline uint64_t NextLargerPowerOf2(uint64_t value)
        {
            value--;
            value |= value >> 1;
            value |= value >> 2;
            value |= value >> 4;
            value |= value >> 8;
            value |= value >> 16;
            value |= value >> 32;
            value++;
            value += (value == 0); // edge case: convert 0 to 1
            return value;
        }

        /**
         * Clamp a numeric value.
         * @param value Value to clamp.
         * @param min   Lower bound.
         * @param max   Upper bound.
         * @return      Clamped value.
         */
        template<typename T>
        static inline T Clamp(T value, T min, T max)
        {
            return std::max(min, std::min(max, value));
        }

        /**
         * Get next larger even number.
         * Returns the smallest even number which is <code>&gt;= x</code>.
         * For negative values the next smaller number is returned.
         */
        static inline int64_t NextLargerEvenVal(int64_t x)
        {
            const int64_t sign = x >= 0 ? 1 : -1;
            return 0 == x % 2 ? x : x + sign;
        }

        /**
         * Get next smaller even number.
         * Returns the smallest even number which is <code>&lt;= x</code>.
         * For negative values the next larger number is returned.
         */
        static inline int64_t NextSmallerEvenVal(int64_t x)
        {
            const int64_t sign = x >= 0 ? 1 : -1;
            return 0 == x % 2 ? x : x - sign;
        }

        /**
         * Get next larger multiple.
         * Returns the smallest number which is a multiple of <code>mult</code>
         * and <code>&gt;= x</code>.
         * For negative values the next smaller number is returned.
         */
        static inline int64_t NextLargerMultiple(int64_t x, int64_t mult)
        {
            if (0 == mult)
            {
                return 0;
            }
            const int sign = x >= 0 ? 1 : -1;
            return (x / mult + (0 == x % mult ? 0 : 1)) * mult * sign;
        }

    private:
        /**
         * Inaccessible and unimplemented constructor:
         * This is a final class with static members only.
         */
        Math(void) = delete;
    };
}

#endif
#endif

//==========================================================================EOF=
