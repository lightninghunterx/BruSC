/*
 * $Source$
 *
 * Copyright (c) 2000 - 2016
 *
 * Bruker BioSpin AG
 * MRI Division
 * CH-8117 Fällanden, Switzerland
 *
 * All Rights Reserved
 *
 * $Id$
 */

#ifndef LIB_CORE_MISC_SINGLETON_MANAGER_WRAPPER_H
#define LIB_CORE_MISC_SINGLETON_MANAGER_WRAPPER_H

#include "libCore/Misc/internal.hh"

/** @file
 *  @defgroup libcoremiscsingletonscoresingletonmanagercapi C wrappers for CoreSingletonManager
 *  Core/Misc library: C language wrapper interface to the class
 *  <code>CoreSingletonManager</code>.
 */
/*@{*/

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * C wrapper interface to the class CoreSingletonManager
     */

    /**
     * C function to destroy singletons.
     * Cleans up all singletons in correct order. The singleton manager itself
     * destroys itself automatically.
     *
     * @param ignoreMissing Ignore dependencies from singletons that aren't
     *                      present if <code>Yes</code>, complain otherwise.
     * @return              <code>Yes</code> upon success, <code>No</code>
     *                      otherwise. Missing singletons lead an error unless
     *                      they are ignored.
     */
    DLL_INTERFACE bool CoreSingletonManager_DestroySingletons(bool ignoreMissing);

    /**
     * C manager instantiation function.
     * Makes sure the singleton manager is instantiated.
     * Doing so is needed only if libraries used by the singleton framework are
     * present that need to be initialized first.
     *
     * This function may be called an arbitrary number of times.
     */
    DLL_INTERFACE void CoreSingletonManager_Instantiate(void);

    /**
     * Destroy the singleton manager.
     * Helper to destroy the singleton manager at a given time. This is needed
     * only if automatic destruction occurs too late (in connection with
     * libraries that cannot be used after <code>main()</code> exits, for
     * instance).
     * Call <code>CoreSingletonManager_DestroySingletons()</code> first to
     * allow for the standard singleton cleanup.
     */
    DLL_INTERFACE void CoreSingletonManager_Destroy(void);

    /**
     * Register an <code>atexit</code> function. The given function is called
     * during normal program termination (via exit() or return from the
     * program's main() function).
     * @param fun The function pointer to install.
     */
    DLL_INTERFACE void CoreSingletonManager_RegisterAtExitFunction(void (*fun)(void));

    /**
     * Register a function to be called when the program has crashed. The given
     * function is called when SIGSEGV or SIGFPE is caught.
     * @param fun The function pointer to install.
     */
    DLL_INTERFACE void CoreSingletonManager_RegisterAtCrashFunction(void (*fun)(void));

#ifdef __cplusplus
}
#endif

/*@}*/

#endif

/*=======================================================================EOF=*/
