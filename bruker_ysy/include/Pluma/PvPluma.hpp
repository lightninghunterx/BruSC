//===========================================================================
//
// Copyright (c) 2014 - 2018
//
// Bruker BioSpin MRI GmbH 
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//===========================================================================

#ifndef PLUMA_PVPLUMA_HHP
#define PLUMA_PVPLUMA_HHP

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "Pluma/Pluma.hpp"

//---------------------------------------------------------------------------
// Forward declarations
//---------------------------------------------------------------------------

#ifndef DLL_INTERFACE
#define DLL_INTERFACE
#endif

/**
 * \defgroup PVPLUMA Write a ParaVision plugin.
 * 
 *
 * Example: A Device plugin.
 * <br>
 * Device header Device.hh (in a shared library named libDevice):
 * \code
 * #ifndef DEVICE_HH
 * #define DEVICE_HH
 *
 * #include <Pluma/PvPluma.hpp>
 *
 * // Symbol Visibility setting.
 * #ifdef DLL_INTERFACE
 * #   undef DLL_INTERFACE
 * #endif
 *
 * #if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
 * #  ifdef libDevice_INTERNAL 
 * #    define DLL_INTERFACE __declspec(dllexport)
 * #  else
 * #    define DLL_INTERFACE __declspec(dllimport)
 * #  endif
 * #elif defined (__GNUC__) && __GNUC__ >= 4
 * #    define DLL_INTERFACE __attribute__ ((visibility("default")))
 * #else
 * #    define DLL_INTERFACE
 * #endif 
 *
 * // Device plugin interface
 * class DLL_INTERFACE Device {
 * public:
 *    virtual ~Device() { }
 *     virtual const char * getName() const = 0;
 *     // (...)
 * };
 * 
 * 
 * // DeviceProvider interface.
 * PVPLUMA_PROVIDER_HEADER(Device);
 *
 * #endif // DEVICE_HHH
 * \endcode
 *
 * Device implementation: Device.cc
 * \code
 * #include "Device.hpp"
 * // generate DevicedProvider with version 6, and compatible with at least v.3
 * PLUMA_PROVIDER_SOURCE(Device, 6, 3);
 * 
 * \endcode
 *
 * <br>
 * Printer header on the plugin side (Printer.hh). In this case 
 * the Printer implementation is in the header. This is not necessary.
 * \code
 * #ifndef PRINTER_HH
 * #define PRINTER_HH
 * 
 * #include "Device.hh"
 * 
 * // Interface of the Printer
 * class Printer: public Device{
 * public:
 *     const char * getName() const {
 *         return "Printer Device";
 *     }
 * };
 * 
 * // Definition and implementation of the PrinterProvider
 * PVPLUMA_INHERIT_PROVIDER(Printer, Device);
 * 
 * #endif // PRINTER_HPP
 * \endcode
 *
 * // Implementation of the PrinterProvider in Printer.cc
 *
 * \code
 * Printer::Printer() { ... }
 *
 * PVPLUMA_INHERIT_PROVIDER_SOURCE(Printer)
 *\endcode
 *
 *
 * plugin connector on plugin side:
 * \code
 * #include "Printer.hh"
 * 
 * // Interface visibility settings.
 * #ifdef DLL_INTERFACE
 * #   undef DLL_INTERFACE
 * #endif
 * 
 * /// DLL interface libParx identifiers for Windows.
 * #if defined(HAS_WINDOWS_FEATURES) && defined(DLL_LINKAGE)
 * #  ifdef Device_INTERNAL 
 * #    define DLL_INTERFACE __declspec(dllexport)
 * #  else
 * #    define DLL_INTERFACE __declspec(dllimport)
 * #  endif
 * #elif defined (__GNUC__) && __GNUC__ >= 4
 * #    define DLL_INTERFACE __attribute__ ((visibility("default")))
 * #else
 * #    define DLL_INTERFACE
 * #endif 
 * 
 * // Must be included after the symbol visibility definitions.
 * #include <Pluma/PvConnector.hpp>
 * 
 * // Plugin connector to host:
 * bool pluma_connect(pluma::Host& host){
 *     host.add( new PrinterProvider() );
 *     return true;
 * }
 * \endcode
 *
 *
 * Host application code:
 * \code
 * #include <Pluma/PvPluma.hpp>
 *
 * #include "Device.hh"
 * #include <iostream>
 * #include <vector>
 * #include "PvUtil/PvUtil.h"
 *
 * int main(){
 *
 *     pluma::Pluma plugins;
 *     // Tell plugins manager to accept providers of the type DeviceProvider
 *     plugins.acceptProviderType<DeviceProvider>();
 *     // Load library from plugin directory
 *     char pluginPath[PATH_MAX];
 *     PvPathMkPlugins(pluginPath, PATH_MAX, 
 *         DeviceProvider::PLUMA_PROVIDER_TYPE.c_str(), 0);
 *     plugins.loadFromFolder(pluginPlath);
 *
 *     // Get device providers into a vector
 *     std::vector<DeviceProvider*> providers;
 *     plugins.getProviders(providers);
 *
 *    // Get a device from each provider
 *    std::vector<DeviceProvider*>::iterator it;
 *    for (it = providers.begin() ; it != providers.end() ; ++it){
 *        // Create a device
 *        Device& device = (*it)->getInstance();
 *        // Display it's description
 *        std::cout << device.getName() << std::endl;
 *        // (...)
 *    }
 *     return 0;
 * }
 * \endcode
 *
 */

////////////////////////////////////////////////////////////
// Macro that helps ParaVision host applications defining
// their provider classes
////////////////////////////////////////////////////////////
#define PVPLUMA_PROVIDER_HEADER(TYPE)\
PVPLUMA_PROVIDER_HEADER_BEGIN(TYPE)\
virtual TYPE & getInstance() const = 0;\
PVPLUMA_PROVIDER_HEADER_END

////////////////////////////////////////////////////////////
// Macro that generate first part of the provider definition
////////////////////////////////////////////////////////////
#define PVPLUMA_PROVIDER_HEADER_BEGIN(TYPE)\
class DLL_INTERFACE TYPE##Provider: public pluma::Provider{\
private:\
    friend class pluma::Pluma;\
    static const unsigned int PLUMA_INTERFACE_VERSION;\
    static const unsigned int PLUMA_INTERFACE_LOWEST_VERSION;\
    std::string plumaGetType() const{ return PLUMA_PROVIDER_TYPE; }\
public:\
    static const std::string PLUMA_PROVIDER_TYPE;\
    virtual unsigned int getVersion() const{ return PLUMA_INTERFACE_VERSION; }


////////////////////////////////////////////////////////////
// Macro that generate last part of the provider definition
////////////////////////////////////////////////////////////
#define PVPLUMA_PROVIDER_HEADER_END };

////////////////////////////////////////////////////////////
// Macro that generate the provider declaration
////////////////////////////////////////////////////////////
#define PVPLUMA_PROVIDER_SOURCE(TYPE, Version, LowestVersion) PLUMA_PROVIDER_SOURCE(TYPE, Version, LowestVersion)


////////////////////////////////////////////////////////////
// Macro that helps plugins generating their provider implementations
// PRE: SPECIALIZED_TYPE must inherit from BASE_TYPE
////////////////////////////////////////////////////////////
#define PVPLUMA_INHERIT_PROVIDER(SPECIALIZED_TYPE, BASE_TYPE)\
class SPECIALIZED_TYPE##Provider: public BASE_TYPE##Provider{\
public:\
    BASE_TYPE & getInstance() const{ return plugin_; }\
private:\
    static SPECIALIZED_TYPE plugin_;\
};

#define PVPLUMA_INHERIT_PROVIDER_SOURCE(SPECIALIZED_TYPE)\
    SPECIALIZED_TYPE SPECIALIZED_TYPE##Provider::plugin_;


#endif /*  PLUMA_PVPLUMA_HHP */
