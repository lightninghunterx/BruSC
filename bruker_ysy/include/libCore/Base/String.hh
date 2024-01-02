//==============================================================================
// Copyright (c) 1998 - 2018
//
// Bruker BioSpin AG
// MRI Division
// CH-8117 Fällanden, Switzerland
//
// All Rights Reserved
//==============================================================================
#ifndef LIB_CORE_BASE_STRING_H
#define LIB_CORE_BASE_STRING_H

#include <cstring>
#include <climits>
#include <cstdarg>
#include <cstdint>
#include <iostream>
#include <sstream>

#include "libCore/Base/internal.hh"

namespace core
{
    //--------------------------------------------------------------------------
    // Interface of class core::String
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Base/String.hh"
     * A static class with some basic string manipulation functions.
     *
     * <b>Multiple Implementations!</b>
     *
     * Implementations of this class exist in C++ (Core/Base library) as
     * well as in Java.
     *
     * All functions in this class use the C++ <code>new[]</code> and
     * <code>delete[]</code> operators instead of <code>malloc()</code> and
     * <code>free()</code> which might be used by the functions from
     * <code>cstring</code>. As mixing the two schemes is not advisable, use
     * <code>String::Dup()</code> as replacement for <code>strdup()</code>.
     *
     * <code>String::Init()</code> and <code>String::Clone()</code> should
     * generally be used in initialization lists only, where expressions are
     * called for. In most other cases, it is safer to use the
     * <code>String::Empty()</code> and <code>String::Dup()</code>
     * functions, which take care of freeing data already allocated to the
     * affected pointer.
     *
     * This class cannot be instantiated.
     *
     * <b>Usage advice</b>
     *
     * - Always initialize <code>char*</code> variables to <code>nullptr</code>
     *   or by calling either <code>Init()</code> or <code>Empty()</code>. The
     *   memory management of the other functions rely on non-nullptr pointers
     *   being safe to <code>delete[]</code>.
     * - Always free <code>char*</code> variables by calling
     *   <code>delete[]</code> or <code>Delete()</code> on them.
     * - Don't mix these functions with C style allocators <code>malloc()</code>
     *   and <code>free()</code>.
     *
     * @since Viper libCore/Base R2
     */
    class DLL_INTERFACE String
    {
    public:

        struct Strcmp
        {
            bool operator()(char const* lhs, char const* rhs) const
            {
                return strcmp(lhs, rhs) < 0;
            }
        };

        /**
         * Get the length (number of code points) of a UTF-8 encoded multibyte
         * string. Returns the same as <tt>strlen</tt> if <tt>mbString</tt>
         * doesn't contain any multibyte characters.
         *
         * Thread safe.
         *
         * Note: this call does not work correctly with modified UTF-8 strings
         * as returned by JNI function, <tt>GetStringUTFChars</tt>.
         *
         * @param mbString A zero-terminated character string, possible
         *                 containing UTF-8 encoded multibyte characters.
         * @return         The number of code points, returns zero if
         *                 <tt>mbString</tt> is <tt>nullptr</tt>. Returns
         *                 <tt>static_cast<std::size_t>(-1)</tt> on error (if
         *                 an invalid multibyte character was encountered).
         */
        static size_t MBSLength(const char *mbString);

        /**
         * Test if a string matches "true".
         * @param str A string.
         * @return    <code>true</code> if <code>s</code> matches "true" in
         *            arbitrary capitalization, <code>false</code> otherwise
         *            (<code>IsFalse()</code> may or may not return
         *            <code>true</code> in this case).
         * @see IsFalse
         */
        static bool IsTrue(const char *str);

        /**
         * Test if a string matches "false".
         * @param str A string.
         * @return    <code>true</code> if <code>s</code> matches "true" in
         *            arbitrary capitalization, <code>false</code> otherwise
         *            (<code>IsTrue()</code> may or may not return
         *            <code>true</code> in this case).
         * @see IsTrue
         */
        static bool IsFalse(const char *str);

        /** Return the string "true". */
        static inline const char *GetTrue(void)
        {
            return "true";
        }

        /** Return the string "false". */
        static inline const char *GetFalse(void)
        {
            return "false";
        }

        /**
         * Cross-platform <code>sprintf(3)</code> replacement.
         * This function tries to use functions which are safe against writing
         * past the end of the buffer. Currently, this is possible on Linux and
         * Windows.
         *
         * @param buf    Destination buffer, may not be <code>nullptr</code>.
         * @param size   Destination buffer size, may not be zero.
         * @param format Standard <code>printf(3)</code> style format string,
         *               may not be <code>nullptr</code>.
         *
         * The remaining arguments are interpreted exactly like arguments to a
         * <code>printf(3)</code> with <code>format</code> as its first
         * parameter.
         *
         * @return <code>true</code> upon success, <code>false</code> if the size
         *         is zero, buffer or format are <code>nullptr</code>, or the
         *         string had to be truncated. In debug versions, buffer and
         *         format are guarded by an assertion.
         */
        static bool Print(char *buf, size_t size,
                const char *format, ...) VA_PRINTF_FORMAT(3, 4);

        /**
         * Cross-platform <code>vsprintf(3)</code> replacement.
         * This function tries to use functions which are safe against writing
         * past the end of the buffer. Currently, this is possible on Linux and
         * Windows.
         *
         * @param buf    Destination buffer, may not be <code>nullptr</code>.
         * @param size   Destination buffer size, may not be zero.
         * @param format Standard <code>printf(3)</code> style format string, may
         *               not be <code>nullptr</code>.
         * @param ap     List of arguments to print.
         * @return <code>true</code> upon success, <code>false</code> if the size
         *         is zero, buffer or format are <code>nullptr</code>, or the
         *         string had to be truncated. In debug versions, buffer and
         *         format are guarded by an assertion.
         */
        static bool VPrint(char *buf, size_t size, const char *format,
                va_list ap);

        /**
         * Initialize a string to the empty string.
         * This function is designed for use in a constructor's initialization
         * list; so it does <b>not</b> deallocate the current data pointed to.
         * Rather use <code>Empty()</code> for the general case.
         * Note that the canonical form of an empty string is <b>not</b> nullptr,
         * but the zero character, thus it uses one char of memory. Strings
         * initialized this way must be cleaned up using <code>delete[]</code>.
         *
         * @param str The string to be initialized.
         */
        static void Init(char *&str);

        /**
         * Empty a string.
         * This has the same result as <code>Init()</code>, but also takes care
         * to deallocate any existing data. So, <code>str</code> must be
         * <code>nullptr</code> if there's no memory allocated to it; otherwise,
         * results are undefined. Allocation must have been done using the
         * <code>new[]</code> operator.
         *
         * @param str The string to be emptied.
         */
        static void Empty(char *&str);

        /**
         * Duplicate a string.
         * This results in <code>dest</code> being a duplicate of
         * <code>src</code>, thus this function performs a deep copy. Existing
         * data pointed to by <code>dest</code> are freed, <code>dest</code>
         * must be <code>nullptr</code> if there's no associated data.
         * Most other pathological cases (assignment to self, <code>src</code>
         * being <code>nullptr</code>) are dealt with properly.
         * The memory allocated by this function must be freed with
         * <code>delete[]</code>.
         *
         * @param dest The string to hold the duplicate.
         * @param src  The string to be duplicated.
         */
        static void Dup(char *&dest, const char *src);

        /**
         * Clone a string (<code>const</code>).
         * This returns a clone of <code>s</code>, thus this function performs a
         * deep copy. The memory allocated must be freed by client code using the
         * <code>delete[]</code> operator.
         *
         * @param str The string to be cloned.
         * @return    A clone of <code>str</code>.
         */
        static char *Clone(const char *str);

        /** Allocate a string of given length. */
        static char *Alloc(size_t length, bool init=true);

        /** Delete a string (non-const). */
        static void Delete(char *str);

        /** Delete a string (const). */
        static void Delete(const char *str);

        /**
         * Clone a string (not <code>const</code>).
         * Clone a non-const string.
         *
         * @param str The string to be cloned.
         * @return    A clone of <code>str</code>.
         */
        static char *Clone(char *str);

        /**
         * Test for emptiness.
         * Checks whether <code>str</code> is <code>nullptr</code> or the empty
         * string; returns <code>true</code> if that is the case,
         * <code>false</code> otherwise.
         *
         *  @param str The string to be examined.
         *  @return    <code>true</code> or <code>false</code>, as explained.
         */
        inline static bool IsEmpty(const char *str)
        {
            if (!str || !strcmp(str, "")) return true;
            return false;
        }

        /**
         * In-place conversion to lowercase.
         * @param str String to convert. This is a no-op for
         *            <code>nullptr</code> or empty strings.
         */
        static void ToLowerCase(char *str);

        /**
         * Convert a value of arbitrary type to a string.
         * @param data   A value of arbitrary type.
         * @param prefix An optional prefix. It will be copied.
         * @param suffix An optional suffix. It will be copied.
         * @return       A string which must be deleted when it is no longer
         *               used.
         * @see ArrayToString.
         */
        template <class T>
        inline static char *ToString(T data, const char *prefix = 0,
                const char *suffix = 0)
        {
            std::stringstream ostream;
            if (prefix) ostream << prefix;
            ostream << data;
            if (suffix) ostream << suffix;
            const std::string oss(ostream.str());
            return Clone(oss.c_str());
        }

        /**
         * Convert an array of values of arbitrary type to a string.
         *
         * @param n          Number of elements in array.
         * @param data       An array of values of arbitrary type.
         * @param prefix     An optional prefix. It will be copied.
         * @param separator  An optional separator. It will be copied.
         * @param suffix     An optional suffix. It will be copied.
         * @return           A string which must be deleted when it is no longer
         *                   used.
         * @see ToString.
         */
        template <class T>
        inline static char *ArrayToString(unsigned int n, T data,
                const char *prefix = 0, const char *separator = 0,
                const char *suffix = 0)
        {
            std::stringstream ostream;
            if (prefix) ostream << prefix;
            for (size_t idx = 0; idx < n; idx++)
            {
                ostream << data[idx];
                if ((separator) && (idx < n - 1)) ostream << separator;
            }
            if (suffix) ostream << suffix;
            const std::string oss(ostream.str());
            return Clone(oss.c_str());
        }

        /**
         * Convert a string to an <tt>uint64_t</tt>.
         * The function returns <code>false</code> and leaves <code>dest</code>
         * unchanged if an error occurred.
         *
         * @param dest A reference to the data destination.
         * @param src  The string to convert.
         * @return     <code>true</code> upon success, <code>false</code>
         *             otherwise.
         */
        static bool To_uint64_t(uint64_t &dest, const char *src);

        /**
         * Convert a string to an <tt>int64_t</tt>.
         * The function returns <code>false</code> and leaves <code>dest</code>
         * unchanged if an error occurred.
         *
         * @param dest A reference to the data destination.
         * @param src  The string to convert.
         * @return     <code>true</code> upon success, <code>false</code>
         *             otherwise.
         */
        static bool To_int64_t(int64_t &dest, const char *src);

        /**
         * Convert a string to an <tt>uintptr_t</tt>.
         * The function returns <code>false</code> and leaves <code>dest</code>
         * unchanged if an error occurred.
         *
         * @param dest A reference to the data destination.
         * @param src  The string to convert.
         * @return     <code>true</code> upon success, <code>false</code>
         *             otherwise.
         */
        static bool To_uintptr_t(uintptr_t &dest, const char *src);

    private:
        /**
         * Inaccessible and unimplemented constructor:
         * This is a final class with static members only.
         */
        String(void);
    };
}

#endif

//==========================================================================EOF=
