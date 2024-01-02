//===========================================================================
// $Source$
//  
//
// Copyright (c) 2005 - 2017
//
// Bruker BioSpin MRI GmbH 
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//===========================================================================

#ifndef PVGENERICSERVER2_MGR_MR_HH
#define PVGENERICSERVER2_MGR_MR_HH


//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------

//
// IMPORTANT: This header should NEVER be included into method source code 
//            or into a public header included in methods.
//

#include <array>
#include "libCore/Misc/Singletons/Adapter.hh"
#include "PvGenericServer2Mgr/Common/GeometryCorrection.hh"
#include "PvGenericServer2Mgr/MR/PreempParameters.hh"
#include "PvGenericServer2Mgr/MR/SconParameters.hh"
#include "PvGenericServer2Mgr/MR/GlobalParameters.hh"
#include "PvGenericServer2Mgr/MR/RFParameters.hh"
#include "PvGenericServer2Mgr/MR/SafetyParameters.hh"
#include "PvGenericServer2Mgr/MR/GradShimParameters.hh"
#include "PvGenericServer2Mgr/MR/ActiveCoilsParameters.hh"
#include "PvGenericServer2Mgr/MR/BisInfoParameters.hh"
#include "PvGenericServer2Mgr/MR/HardwareInfo.hh"
#include "PvGenericServer2Mgr/MR/ShimPins.hh"
#include "PvGenericServer2Mgr/MR/DLLMacro.hh"

#include "libpvrouting/rfconnection.hh"
#include "libpvrouting/operationmode.hh"
#include "libpvrouting/coilconfiguration.hh"
#include "libpvrouting/types.hh"

namespace PvConfigMr {

    class DLL_INTERFACE Manager : public core::SingletonAdapter<Manager> {
    public:

        // Module preemphasis parameters
        void getServerPreempParams(const std::string & status, PreempParameters & pars) const;
        void getServerPreempParams(const std::string & status, const std::string & filename, PreempParameters & pars) const;
        void setServerPreempParams(const std::string & status, const PreempParameters & pars);
        void setServerPreempParams(const std::string & status, const std::string & filename, const PreempParameters & pars);

        // Module SCON parameters
        void getServerSconParams(SconParameters & pars) const;
        void setServerSconParams(const SconParameters & pars);

        // Module Global Parameters
        void getServerGlobalParams(GlobalParameters & pars) const;
        void setServerGlobalParams(const GlobalParameters & pars);

        // Module RF Parameters
        void getServerRFParams(RFParameters & pars) const;
        void setServerRFParams(const RFParameters & pars);

        // Safety Parameters
        void getServerSafetyParams(const std::string & status, SafetyParameters & pars) const;
        void setServerSafetyParams(const std::string & status, const SafetyParameters & pars);

        // Get Shim Limitations
        void getServerShimMaxCurrent(const std::string shimBis, std::vector<double> &shim_CurrentMax) const;

        // GradShim Parameters
        void getServerGradShimParams(GradShimParameters & pars) const;
        
        // Active coils
        void getActiveCoilsParams(ActiveCoilsParameters & pars) const;
        
        void getBisInfoParams(BisInfoParameters & pars) const;
                
        // modality offset
        std::array<double, 3> getModalityOffset();

        // rotation matrix
        std::array<double, 9> getModalityRotation();
        
        /**
         * Persist the global adjustment state and results on the EPU.
         * @throws PvException on error.
         */
        void persistGlobalAdjustmentData() const;
        
        // ShimPins information
        ShimPins getShimPins() const;
        

        /**********************************************************************/
        /* DIGITIZER HANDLING                                                 */
        /**********************************************************************/
        
        /** get the closest realizable bandwidth.  
         *  @param swh        (i) sampling bandwidth/frequency (set by user, in Hz)
         *  @param searchDir  (i) specify the search policy if the input value is not 
         *                        valid: >0: get next higher value
         *                                0: get arithmetically closest value
         *                               <0: get next lower value
         *  @return the value of the closest possible bandwidth width in Hz
         */
        double getNearestRxFilterBandwidthInHz(const double& sweepWidthHz, 
                                               const int searchDir) const;

        /** derive an information structure using a default policy.
         *  @param swh            (i) sampling bandwidth/frequency (set by user, in Hz)
         *  @param scanSize       (i) number of points to be acquired (set by user, 2*nCmplxPts)
         *  @param policyOpts     (i) default policy options, see "enum DefaultPolicyOpts"
         *  @param filterSetting  (o) the used filter setting 
         *  @return the derived filter information
         */
        RX_FILTER_INFO getDefaultRxFilterInfo(const double& swh, 
                                              const int scanSize = -1, 
                                              const int policyOpts = 0, 
                                              RX_FILTER_SETTING* filterSetting = nullptr) const;

        /** derive a filter setting from given input values using a default policy.
         *  @param swh        (i) sampling bandwidth/frequency (set by user, in Hz)
         *  @param scanSize   (i) number of points to be acquired (set by user, 2*nCmplxPts)
         *  @param policyOpts (i) default policy options, see "enum DefaultPolicyOpts"
         *  @return the derived filter setting
         */
        RX_FILTER_SETTING getDefaultRxFilterSetting(const double& swh, 
                                                    const int scanSize = -1, 
                                                    const int policyOpts = 0) const;

        /** get an information structure.
         *  @param filterSetting (i) the used filter setting 
         *  @return the derived filter information
         */
        RX_FILTER_INFO getRxFilterInfo(const RX_FILTER_SETTING& filterSetting) const;
        
        /**
         *  get the sweep width resulting from a given setup.
         *  @param hwDecim     (i) decimation done on the digitizer (e.g. on DRX)
         *  @param hwFiltType  (i) the used hardware filter.
         *  @param swDecim     (i) decimation done after the digitizer (e.g. on EPU)
         *  @return the sweep width or 0.0 if input values are inconsistent, e.g. filter
         *          and decimation don't match.
         */
        double getRxFilterBandwidthInHz(const int hwDecim, const int hwFilterType, const int swDecim) const;

        /** range check a filter setting
         *  @param filterSetting (i/o) the setting to be checked 
         *  @param oldSwDecim    (i) value to compare to, or -1
         *  @param oldHwDecim    (i) value to compare to, or -1
         *  @param policyOpts    (i) default policy options, see above
         *  @return the derived filter information
         */
        void rangeCheckRxFilterSetting(RX_FILTER_SETTING& filterSetting, const int oldSwDecim, 
                const int oldHwDecim, const int policyOpts) const;
       
        
        /**********************************************************************/
        /* CORTAB HANDLING                                                    */
        /**********************************************************************/
        ///  clear cached correction tables
        void clearCortabCache() const;

        /*  Read the maximum power value for the given transmit path
         *  @param chanNum     (i) logical channel number [1...N]
         *  @param txSguNum    (i) SGU index [1...N]
         *  @param nucleus     (i) nucleus string, e.g. "1H"
         *  @param ampNum      (i) amplifier number [1...N]
         *  @param txPreampNum (i) preamplifier index [1...N]
         *  @param isHighPower (i) true, if high power output of amplifier is used
         *  @return the maximum power read from the CorTab file
         */
        double getRealAmplifierPowerFromCortab(const int chanNum, const int txSguNum, const char* nucleus, 
                const int ampNum, const int txPreampNum, const bool isHighPower) const;
        
        /*  Date of CorTab file creation
         *  @param txSguNum    (i) SGU index [1...N]
         *  @param nucleus     (i) nucleus string, e.g. "1H"
         *  @param ampNum      (i) amplifier number [1...N]
         *  @param txPreampNum (i) preamplifier index [1...N]
         *  @param isHighPower (i) true, if high power output of amplifier is used
         *  @return the date of CorTab file creation
         */
        std::string getCortabDate(const int  txSguNum, const char* nucleus,
                const int  ampNum, const int  txPreampNum, const bool isHighPower) const;

        
        /**********************************************************************/
        /* RF SOCKET INTERFACE                                                */
        /**********************************************************************/
        /**
         * Get the phase correction value of a connection port
         * @param type  The port type
         * @param num   The port number [1...N]
         * @return The value.
         */
        double getRfIfcPortPhaseOffset(const PvRouting::RfConnection::PortType portType, const int portNum) const;
        /**
         * Get the amplitude scaling value of a connection port
         * @param type  The port type
         * @param num   The port number [1...N]
         */
        double getRfIfcPortAmplitudeScaling(const PvRouting::RfConnection::PortType portType, const int portNum) const;
        
        /**
         *  Get the table of RF coil sockets.
         *  @return  The table of RF coil sockets.
         */
        PvRouting::RfSocketNumLockUp getRfIfcSocketMap() const;
        
        
        /**********************************************************************/
        /* GENERAL HARDWARE INFORMATION                                       */
        /**********************************************************************/
        HardwareInfo getHardwareInfo() const;
        
        
        /**********************************************************************/
        /* COILS                                                              */
        /**********************************************************************/
        /**
         * Try to create wobble operation modes for given coil configuration
         *
         * @param id    The id of the coil configuration
         * @param wobbMode      see "enum class PvRouting::CoiConfiguration::WobbleMode"
         * @return  the <code>operationModes</code> to wobble the given coil configuration.
         */
        PvRouting::OperationModeSet getWobbleOperationModes(const std::string& id, 
                                                            const PvRouting::CoilConfiguration::WobbleMode mode) const;
        
        /**
         * Check if an acquisition can be started for the given RF, gradient, and
         * shim coil configuration.
         * @param coilConfigurationId  The value of the parameter CONFIG_SCAN_coil_configuration.
         * @param operationMode        The enum string representation of the parameter CONFIG_SCAN_operation_mode.
         *                             (Not displayname!)
         * @param gradientSystem       The value of the parameter CONFIG_SCAN_gradient_system.
         * @param shimSystem           The value of the parameter CONFIG_SCAN_shim_system.
         * @return  <code>true</code> if an acquisition can be started,
         *          <code>false</code> if it is not allowed to start an acquisition.
         */
        bool canStartAcquisition(const std::string& coilConfigurationId, 
                                 const std::string& operationMode, 
                                 const std::string& gradientSystem, 
                                 const std::string& shimSystem) const;
        
        /**
         *  Open the routing display for the given operation mode with a certain system wiring.
         *
         *  @param operationMode bis string describing an operation mode.
         *  @param coilTable     bis string describing the coil set.
         *  @param coils         bis data for all coils
         *  @param wiring        bis string describing the system wiring (may be empty, in this
         *                       case the standard system wiring will be used).
         */
        void showRouting(const std::string& operationMode,
                         const std::string& coilTable,
                         const std::vector<std::string>& coils,
                         const std::string& wiring) const;
        
        /**
         *  Get the components of the a coil configuration.
         *  The server will first try to lookup the provided <code>id</code> in its
         *  database and - if the configuration is found - return its components
         *  in the remaining parameters. This is the standard procedure. Empty values
         *  may be passed to <code>coilTable</code> and <code>coils</code> in this case.
         *
         *  If the configuration is not found, it will use <code>coilTable</code> and <code>coils</code>
         *  to look for an identical coil configuration that may be stored with a different
         *  <code>id</code>. If a matching configuration is found in this way, <code>id</code> is adjusted to
         *  the appropriate value, and the coil configuration contents are returned as before.
         *  This scenario applies to the situation where scans are moved between or accessed by
         *  multiple ParaVision installations that use the same MRI scanner but maintain their
         *  coil configuration individually.
         *
         *  @param id             The id key of the coil configuration.
         *  @param coilTable      The bis string describing the coil set.
         *  @param coils          The bis data for all coils
         *  @param wiring         If this coil configuration requires a modified system wiring,
         *                        it is stored in this parameter (empty if the standard wiring
         *                        is to be used).
         *  @param operationModes The bis strings describing the coil configuration's operation modes.
         */
	virtual void getCoilConfiguration(std::string* id,
					  std::string* coilTable,
					  std::vector<std::string>* coils,
					  std::string* wiring,
					  std::vector<std::string>* operationModes) const;
        
        /**********************************************************************/
        
        
    protected:

        static Manager * Create() {
            return new Manager;
        }
        
    private:
        
        std::vector<std::string> splitStatusString(const std::string& s, char delimiter) const;
        
        std::array<double, 3> get_offset();
        std::array<double, 9> get_rotation();

        void set_server_preemp_params(const std::string & status, const std::string & filename, const PreempParameters & pars);
        void get_server_preemp_params(const std::string & status, const std::string & filename, PreempParameters & pars) const;

        Manager(const Manager&) = delete;
        Manager operator=(const Manager&) = delete;

        Manager();
        virtual ~Manager();
        friend class core::PlainSingletonPointer<Manager>;
        static core::PlainSingletonPointer<Manager> TheInstance_;

        struct Impl;
        Impl * pimpl_;

    };
}

#endif /* PVGENERICSERVER2_MGR_MR_HH */
