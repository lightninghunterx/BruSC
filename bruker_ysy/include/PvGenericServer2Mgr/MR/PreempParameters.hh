//===========================================================================
// $Source$
//
// Copyright (c) 2017
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//===========================================================================

#ifndef PVGENERICSERVER2_PREEMPPARS_MR_HH
#define PVGENERICSERVER2_PREEMPPARS_MR_HH

#ifndef CPROTO
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <array>
#include "PvGenericServer2Mgr/MR/PreempParameterDefines.h"
#include "PvGenericServer2Mgr/MR/DLLMacro.hh"

namespace PvConfigMr {

    class DLL_INTERFACE PreempParameters {
    public:
        PreempParameters();

        ~PreempParameters();
        
        PreempParameters(const PreempParameters&);

        // clear struct
        void clear();

        // GRADCC
        double getGradCC() const;
        void setGradCC(double gradCC);

        // RAMPTM
        double getRampTime() const;
        void setRampTime(double rampTime);

        // RISETM
        double getRiseTime() const;
        void setRiseTime(double riseTime);

        // B0CC
        double getB0cc() const;
        void setB0cc(double b0cc);

        // COILDEL
        double getCoilDelay() const;
        void setCoilDelay(double coilDelay);
        
        // CoarseOffset list
        std::array<double, PREEMP_COARSEOFFSET_LEN> getCoarseOffset() const;
        void setCoarseOffset(const std::array<double, PREEMP_COARSEOFFSET_LEN> & coarseOffset);

        // DELAY list
        std::array<double, PREEMP_DELAYLIST_LEN> getCompDel() const;
        void setCompDel(const std::array<double, PREEMP_DELAYLIST_LEN> & compDel);
        
        // Scaling Factors
        std::array<double, PREEMP_SCALINGFAC_LEN> getScalingFactors() const;
        void setScalingFactors(const std::array<double, PREEMP_SCALINGFAC_LEN> & scalingFac);
        
        // Actual Currents
        std::array<double, PREEMP_ACTUALCURR_LEN> getActualCurrents() const;
        void setActualCurrents(const std::array<double, PREEMP_ACTUALCURR_LEN> & currActual);

        // DPP factor
        double getDppFactor() const;
        void setDppFactor(double dppFactor);

        // Preemphasis Parameters
        std::array<int, PREEMP_CHAN_IN_LEN> getPreempChanIn() const;
        void setPreempChanIn(const std::array<int, PREEMP_CHAN_IN_LEN> & preempChanIn);

        std::array<int, PREEMP_CHAN_OUT_LEN> getPreempChanOut() const;
        void setPreempChanOut(const std::array<int, PREEMP_CHAN_OUT_LEN> & preempChanOut);

        std::array<double, PREEMP_FILTERS_LEN> getPreempFilters() const;
        void setPreempFilters(const std::array<double, PREEMP_FILTERS_LEN> & preempFilters);

        int getPreempMode() const;
        void setPreempMode(int preempMode);

        std::array<int, PREEMP_ORDER_LEN> getPreempOrder() const;
        void setPreempOrder(const std::array<int, PREEMP_ORDER_LEN> & preempOrder);


    private:
        struct Impl;
        Impl * pimpl_;
        PreempParameters operator=(const PreempParameters&) = delete;

    };
}
#endif /* CPROTO */
#endif /* PVGENERICSERVER2_PREEMPPARS_MR_HH */
