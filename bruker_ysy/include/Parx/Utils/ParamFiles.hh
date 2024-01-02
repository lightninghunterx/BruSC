//===============================================================
//
// Definition of dataset files.
//
// Copyright (c) 2019
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//
//===============================================================

#ifndef PARX_PARAMFILES_HH
#define PARX_PARAMFILES_HH

//---------------------------------------------------------------
// includes
//---------------------------------------------------------------
#include <unordered_set>
#include "PvUtil/Hasher.hh"
#include "generated/DataPath.hh"
#include "Parx/Utils/DLLMacro.h"

//---------------------------------------------------------------
// forwards
//---------------------------------------------------------------

namespace Parx
{
    namespace Utils
    {
        /**
         * @addtogroup ParxUtils
         * @{
         */

        /** 
         * Group identifiers. 
         * The identifiers must correspond to the \c DsetGroupEnum type in 
         * \c gen/idl/ParameterDsetType.idl .
         */
        enum class DatasetGroupId
        {
            /// SUBJECT group (stored in \c subject file).
            GRP_SUBJECT,
            /// TUDY_MODALITY group (stored in \c study.MODALITY file).
            GRP_STUDYMODALITY,
            /// CONFIG_SCAN group (stored in \c configscan file).
            GRP_CONFIG_SCAN,
            /// MethodFileGroup group (stored in \c method file).
            GRP_METHOD,
            /// MethodOutGroup group (stored in \c method,out file).
            GRP_METHOD_OUT,
            /// ACQP group (stored in \c acqp file).
            GRP_ACQP,
            /// ACQP_OUT group (stored in \c acqp.out file).
            GRP_ACQP_OUT,
            /// MethReco group (stored in \c methreco file).
            GRP_METHRECO,
            /// MetRecoOutGroup group (stored in \c methreco.out file).
            GRP_METHRECO_OUT,
            /// RECO group (stored in \c reco file).
            GRP_RECO,
            /// RECO_OUT group (stored in \c reco.out file).
            GRP_RECO_OUT,
            /// Visu group (stored in \c visu_pars file).
            GRP_VISU
        };
        
        /// Set of dataset file identifiers.
        typedef std::unordered_set<DatasetGroupId, PvUtil::EnumClassHash>
        DsetGroupIdSetType;

        /** Datatype for writing mode of parameters. */
        enum class WriteMode
        {
            /** 
             * Write in PARX dataset default mode.
             * The default mode can be set in PARX and is one of the other modes.
             */
            DatasetDefaultMode,

            /** Write in normal PARX mode. */
            NormalMode,

            /** Write in normal PARX mode with simple RLE coding for large arrays. */
            NormalRleMode,

            /** Write in PARX mode with visibility information of parameters. */
            VisibilityMode,

            /** Write in PARX mode with visibility information of parameters 
             * and simple RLE coding for large arrays. */
            VisibilityRleMode,

            /** Write in TopSpin parameter file mode. */
            ForXWinNmrMode
        };

        /** Parameter description of a dataset path. */
        struct DLL_INTERFACE DatasetPathType
        {
            /** Prefix of dataset path. */
            char datpath[::de::bruker::mri::generated::datapath::MaxPrefixCStrLen];

            /** Study directory name of dataset path. */
            char study[::de::bruker::mri::generated::datapath::MaxNameCStrLen];

            /** Experiment number directory of dataset path. */
            int expno;

            /** Processing number directory of dataset path. */
            int procno;
        };

        /** @} */
    }
}

#endif

