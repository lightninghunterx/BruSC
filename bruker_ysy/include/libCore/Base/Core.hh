//==============================================================================
// Copyright (c) 2000 - 2018
//
// Bruker BioSpin AG
// MRI Division
// CH-8117 Fällanden, Switzerland
//
// All Rights Reserved
//==============================================================================
#ifndef LIB_CORE_BASE_CORE_H
#define LIB_CORE_BASE_CORE_H

#include <atomic>
#include <cassert>
#include <iostream>

#include "libCore/Base/internal.hh"

//------------------------------------------------------------------------------
// Debug macros
//------------------------------------------------------------------------------

#ifndef NDEBUG

// Comment out the following line to disable status messages.
#define CORE_VERBOSE

// Comment out the following line to disable memory management status messages.
#define CORE_VERBOSE_MEMORY

#endif

#ifdef NDEBUG
#define CoreAssert(m) assert(m)
#else
#define CoreAssert(m) { bool CoreAssert_i = true ;if((!(m)) == CoreAssert_i) ::core::Core::Backtrace(std::cerr); assert(m); }
#endif

namespace core
{
    //--------------------------------------------------------------------------
    // Interface of class core::Core
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Base/Core.hh"
     * Basic defaults and flags.
     * This static class holds defaults and flags. It is used as sort of fake
     * namespace.
     * <br>
     * <b>Multiple Implementations!</b>
     * <br>
     * Implementations of this class exist in C++ (Core/Base library) as
     * well as in Java.
     * <br>
     * The verbose flags and backtrace calls are only available in debug
     * versions.
     *
     * @since Viper libCore/Base R3.2.1
     */
    class DLL_INTERFACE Core
    {
    public:
        /**
         * Set the verbose flag.
         * The <code>CORE_VERBOSE</code> preprocessor symbol must be defined in
         * <code>Core.hh</code> for the flag to be effective.
         */
        static void SetVerbose(bool v);

        /** Get the current setting of the verbose flag. */
        static bool IsVerbose(void);

        /**
         * Set the verbose memory management flag.
         * The <code>CORE_VERBOSE</code> preprocessor symbol must be defined in
         * <code>Core.hh</code> for the flag to be effective.
         */
        static void SetVerboseMemory(bool v);

        /** Get the current setting of the verbose memory management flag. */
        static bool IsVerboseMemory(void);

        /**
         * Set auxiliary backtrace information callback.
         * Does nothing by default.
         * @param cb The CB to call.
         */
        static void SetAuxiliaryInfoCB(void(*cb)(std::ostream &ostream));

        /**
         * Write a stack backtrace to some given stream.
         * This functionality is not available if <code>NDEBUG</code> is set,
         * backtraces aren't supported at all on the given platform, or platform
         * specific support has not yet been written.
         * It is a no-op if unsupported.
         * @param ostream The stream the data are written to.
         * @return        <code>true</code> if a backtrace was generated,
         *                <code>false</code> otherwise.
         */
        static bool Backtrace(std::ostream &ostream);

        /**
         * Print a stack backtrace to stderr.
         * This functionality is not available if <code>NDEBUG</code> is set,
         * backtraces aren't supported at all on the given platform, or platform
         * specific support has not yet been written.
         * It is a no-op if unsupported.
         */
        static void PrintBacktrace(void);

        /**
         * Write the command line of the current process to the given stream.
         * @param ostream The stream to which the command line should be written.
         * @return        <code>true</code> if the command line could be written,
         *                <code>false</code> otherwise.
         */
        static bool CmdLine(std::ostream &ostream);

        /**
         * Print the command line of the current process to
         * <code>stderr</code>.
         */
        static void PrintCmdLine(void);

        /** Disable core dumps for the current process. */
        static void DisableCoreDumps(void);

        /**
         * Do word wrapping on a string.
         * @param ostream Output stream to write to.
         * @param text    Text to format. Nothing is done if
         *                <code>nullptr</code> or empty.
         * @param indent  Left indent, optional, zero by default.
         * @param width   Total line width, optional, uses 80 characters if
         *                omitted, zero, or by default.
         * @return        The output stream.
         */
        static std::ostream &WordWrap(std::ostream &ostream, const char *text,
                unsigned int indent = 0, unsigned int width = 0);

    private:
        /**
         * Helper to insert backtrace header.
         * @param ostream Stream to write to.
         */
        static void BacktraceHeader(std::ostream &ostream);

        /**
         * Verbose  flag.
         * The <code>CORE_VERBOSE</code> preprocessor symbol must be defined in
         * <code>Core.hh</code> for this flag to be effective.
         */
         static std::atomic<bool> Verbose_;

        /**
         * Verbose memory management flag.
         * The <code>CORE_VERBOSE_MEMORY</code> preprocessor symbol must be
         * defined in <code>Core.hh</code> for this flag to be effective.
         */
         static std::atomic<bool> VerboseMemory_;

        /** Auxiliary backtrace information CB. */
         static void(*AuxiliaryInfoCB_)(std::ostream &ostream);

    public:
        //----------------------------------------------------------------------
        // Interface of nested class core::Core::Object
        //----------------------------------------------------------------------
        /**
         * Universal ancestor for RTTI workaround.
         * This is an abstract base class templates derive from in order to be
         * able to get their own class name without including the RTTI header
         * into a header file.
         *
         * @since Viper libCore/Base R3.3
         */
        class DLL_INTERFACE Object
        {
        protected:
            /** Virtual, inaccessible destructor. */
            virtual ~Object() {}
        public:
            /**
             * Naming helper.
             * @return The object's class name. It has to be deleted when it is
             *         no longer used.
             */
            static std::string GetName(const Object *object);
        };

        /** Unimplemented constructor: static class. */
        Core(void) = delete;
    };
}

#endif

//==========================================================================EOF=
