/*==============================================================================
 * genX/h/generated/VisuIds.hh 
 *    Header file generated from IDL
 *
 * THIS FILE IS GENERATED AUTOMATICALLY -- DO NOT EDIT
 *
 * Header: gen/src/idl/Generic_header.hh
 * Body  : via pseudo IDL compiler from gen/idl/VisuIds.idl
 * Footer: gen/src/idl/Generic_footer.hh
 *
 * Copyright (c) 2000-2012
 *
 * Bruker BioSpin AG
 * MRI Division
 * CH-8117 Faellanden, Switzerland
 *
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *============================================================================*/

/*------------------------------------------------------------------------------
 * $Source$
 *  - Header for header files generated from IDL
 *
 * $Id$
 *----------------------------------------------------------------------------*/

#ifndef VisuIds_hh
#define VisuIds_hh


#ifndef CPROTO

/*----------------------------------------------------------------------------*/

namespace de
{
    namespace bruker
    {
        namespace mri
        {
            namespace generated
            {
                namespace visu
                {
                    const int CurrentVersion = 5;
                    namespace derive
                    {
                        const int Nothing = 0;
                        const int Instance = 1;
                        const int Subject = 2;
                        const int Study = 4;
                        const int ImageSeries = 8;
                        const int Equipment = 16;
                        const int ImageDescription = 32;
                        const int Acquisition = 64;
                        const int All = 255;
                        const int ForExperiment = 1073741824;
                    }
                    namespace overlay
                    {
                        const double AreaA_X = 0.000000e+00;
                        const double AreaA_Y = 0.000000e+00;
                        const double AreaA_W = 3.000000e-01;
                        const double AreaA_H = 3.000000e-01;
                        const double AreaB_X = 3.000000e-01;
                        const double AreaB_Y = 0.000000e+00;
                        const double AreaB_W = 4.000000e-01;
                        const double AreaB_H = 2.000000e-01;
                        const double AreaC_X = 7.000000e-01;
                        const double AreaC_Y = 0.000000e+00;
                        const double AreaC_W = 3.000000e-01;
                        const double AreaC_H = 1.500000e-01;
                        const double AreaD_X = 0.000000e+00;
                        const double AreaD_Y = 3.000000e-01;
                        const double AreaD_W = 2.000000e-01;
                        const double AreaD_H = 3.000000e-01;
                        const double AreaE_X = 2.000000e-01;
                        const double AreaE_Y = 2.000000e-01;
                        const double AreaE_W = 5.000000e-01;
                        const double AreaE_H = 5.000000e-01;
                        const double AreaF_X = 8.000000e-01;
                        const double AreaF_Y = 1.500000e-01;
                        const double AreaF_W = 2.000000e-01;
                        const double AreaF_H = 5.000000e-01;
                        const double AreaG_X = 0.000000e+00;
                        const double AreaG_Y = 6.000000e-01;
                        const double AreaG_W = 3.000000e-01;
                        const double AreaG_H = 4.000000e-01;
                        const double AreaH_X = 3.000000e-01;
                        const double AreaH_Y = 7.000000e-01;
                        const double AreaH_W = 4.000000e-01;
                        const double AreaH_H = 3.000000e-01;
                        const double AreaI_X = 7.000000e-01;
                        const double AreaI_Y = 6.500000e-01;
                        const double AreaI_W = 3.000000e-01;
                        const double AreaI_H = 3.500000e-01;
                        const double ScalePosition = 9.300000e-01;
                        const int ScalebarMinimumHeight = 150;
                        const int SmallScalebarTic = 3;
                        const int BigScalebarTic = 5;
                        const int ItemSpacing = 5;
                    }
                }
                namespace visuid
                {
                    namespace series
                    {
                        const char* const ACQ_PREFIX = "ACQ_";
                        const char* const BRUKER_PVM = "ACQ_BRUKER_PVM";
                        const char* const USER_PVM = "ACQ_USER_PVM";
                        const char* const IMND = "ACQ_IMND";
                        const char* const ACQ = "ACQ_DIRECT";
                        const char* const DERIVED_PREFIX = "DERIVED_";
                        const char* const MPR = "DERIVED_MPR";
                        const char* const ISA = "DERIVED_ISA";
                        const char* const DTI = "DERIVED_DTI";
                        const char* const MASK = "DERIVED_MASK";
                        const char* const MAP = "DERIVED_MAP";
                        const char* const MAP_ATTENUATION = "DERIVED_MAP_ATTENUATION";
                        const char* const DERIVED_STITCHED = "DERIVED_STITCHED";
                        const char* const DERIVED_REGISTERED = "DERIVED_REGISTERED";
                        const char* const DERIVED_OTHER = "DERIVED_OTHER";
                        const char* const IMPORT_PREFIX = "IMPORT_";
                        const char* const IMPORT_ACQUIRED = "IMPORT_ACQUIRED";
                        const char* const IMPORT_DERIVED = "IMPORT_DERIVED";
                    }
                    namespace deppars
                    {
                        const char* const EchoTime = "VisuAcqEchoTime";
                        const char* const RepetitionTime = "VisuAcqRepetitionTime";
                        const char* const InversionTime = "VisuAcqInversionTime";
                        const char* const DataUnits = "VisuCoreDataUnits";
                        const char* const ElementIds = "VisuFGElemId";
                        const char* const ElementComments = "VisuFGElemComment";
                        const char* const GenericDoubleValues = "VisuDepValsDouble";
                        const char* const Position = "VisuCorePosition";
                        const char* const Orientation = "VisuCoreOrientation";
                        const char* const DiskSliceOrder = "VisuCoreDiskSliceOrder";
                        const char* const ModalityOffset = "VisuCoreModalityOffset";
                        const char* const FrameThickness = "VisuCoreFrameThickness";
                        const char* const FrameType = "VisuCoreFrameType";
                        const char* const DiffusionBMatrix = "VisuAcqDiffusionBMatrix";
                        const char* const DiffusionGradOrient = "VisuAcqDiffusionGradOrient";
                        const char* const ContrastAgentUsage = "VisuContrastAgentUsage";
                        const char* const ContrastAgentPhase = "VisuContrastAgentPhase";
                        const char* const MpiReconstruction = "VisuMpiReconstruction";
                        const char* const MpiReconstructionUsedFrequencyComponents = "VisuMpiUsedFreqComponents";
                        const char* const MpiReconstructionConstraints = "VisuMpiReconstructionConstraints";
                        const char* const CardiacMovieFrameTime = "VisuCardiacMovieFrameTime";
                        const char* const RespMovieFrameTime = "VisuRespMovieFrameTime";
                        const char* const PtAcqStartConditions = "VisuPtAcqStartConditions";
                        const char* const PtAcqStartValues = "VisuPtAcqStartValues";
                        const char* const PtAcqStopConditions = "VisuPtAcqStopConditions";
                        const char* const PtAcqStopValues = "VisuPtAcqStopValues";
                        const char* const PtAcqInstrumentInfos = "VisuPtAcqInstrumentInfos";
                        const char* const PtAcqCompoundUsage = "VisuPtAcqCompoundUsage";
                    }
                    namespace framegroup
                    {
                        const char* const COIL = "FG_COIL";
                        const char* const ECHO = "FG_ECHO";
                        const char* const IRMODE = "FG_IRMODE";
                        const char* const SLICE = "FG_SLICE";
                        const char* const MOVIE = "FG_MOVIE";
                        const char* const SATTRANS = "FG_SATTRANS";
                        const char* const DIFFUSION = "FG_DIFFUSION";
                        const char* const RESP_MOVIE = "FG_RESP_MOVIE";
                        const char* const CARDIAC_MOVIE = "FG_CARDIAC_MOVIE";
                        const char* const TIME_COURSE = "FG_TIME_COURSE";
                        const char* const TRIGGER = "FG_TRIGGER";
                        const char* const ACQ_FRAME_TIME = "FG_ACQ_FRAME_TIME";
                        const char* const RECO_FRAME_TIME = "FG_RECO_FRAME_TIME";
                        const char* const CYCLE = "FG_CYCLE";
                        const char* const COMPLEX = "FG_COMPLEX";
                        const char* const FLOW = "FG_FLOW";
                        const char* const ORIENT = "FG_ORIENT";
                        const char* const EVOLUTION = "FG_EVOLUTION";
                        const char* const REPETITION = "FG_REPETITION";
                        const char* const DTI = "FG_DTI";
                        const char* const ISA = "FG_ISA";
                        const char* const CONTRAST_MAP = "FG_CONTRAST_MAP";
                        const char* const CONTRAST_AGENT = "FG_CONTRAST_AGENT";
                        const char* const RECONSTRUCTION = "FG_RECONSTRUCTION";
                        const char* const PREFIX_SPECTROSCOPY = "FG_SPECTROSCOPY";
                        const char* const PREFIX_TEMPORAL = "FG_TEMPORAL";
                    }
                    namespace dti
                    {
                        const char* const TRACE = "DTI_TRACE";
                        const char* const FA = "DTI_FA";
                        const char* const A0 = "DTI_A0";
                        const char* const TWI = "DTI_TWI";
                        const char* const DXX = "DTI_DXX";
                        const char* const DYY = "DTI_DYY";
                        const char* const DZZ = "DTI_DZZ";
                        const char* const DXY = "DTI_DXY";
                        const char* const DXZ = "DTI_DXZ";
                        const char* const DYZ = "DTI_DYZ";
                        const char* const L1 = "DTI_L1";
                        const char* const L2 = "DTI_L2";
                        const char* const L3 = "DTI_L3";
                        const char* const L1X = "DTI_L1X";
                        const char* const L1Y = "DTI_L1Y";
                        const char* const L1Z = "DTI_L1Z";
                        const char* const L2X = "DTI_L2X";
                        const char* const L2Y = "DTI_L2Y";
                        const char* const L2Z = "DTI_L2Z";
                        const char* const L3X = "DTI_L3X";
                        const char* const L3Y = "DTI_L3Y";
                        const char* const L3Z = "DTI_L3Z";
                        const char* const ERR = "DTI_ERR";
                        const char* const TRACE_ERR = "DTI_ERR_TRACE";
                        const char* const FA_ERR = "DTI_ERR_FA";
                        const char* const A0_ERR = "DTI_ERR_A0";
                        const char* const TWI_ERR = "DTI_ERR_TWI";
                        const char* const DXX_ERR = "DTI_ERR_DXX";
                        const char* const DYY_ERR = "DTI_ERR_DYY";
                        const char* const DZZ_ERR = "DTI_ERR_DZZ";
                        const char* const DXY_ERR = "DTI_ERR_DXY";
                        const char* const DXZ_ERR = "DTI_ERR_DXZ";
                        const char* const DYZ_ERR = "DTI_ERR_DYZ";
                        const char* const L1_ERR = "DTI_ERR_L1";
                        const char* const L2_ERR = "DTI_ERR_L2";
                        const char* const L3_ERR = "DTI_ERR_L3";
                    }
                    namespace complex
                    {
                        const char* const IMAG = "COMPLEX_IMAG";
                        const char* const REAL = "COMPLEX_REAL";
                    }
                    namespace contrast
                    {
                        const char* const T1MAP = "CONTRAST_T1MAP";
                        const char* const T2MAP = "CONTRAST_T2MAP";
                        const char* const T1STARMAP = "CONTRAST_T1STARMAP";
                        const char* const PDMAP = "CONTRAST_PDMAP";
                        const char* const TRUFI_STEADYSTATE = "TRUFI_STEADYSTATE";
                        const char* const TRUFI_INVDEGREE = "TRUFI_INVDEGREE";
                    }
                }
            }
        }
    }
}
/*------------------------------------------------------------------------------
 * $Source$
 *  - Footer for header files generated from IDL
 *
 * $Id$
 *----------------------------------------------------------------------------*/

#endif

#endif

/*========================================================================EOF=*/

