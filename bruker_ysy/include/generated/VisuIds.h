/*==============================================================================
 * genX/h/generated/VisuIds.h 
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

#ifndef VisuIds_h
#define VisuIds_h


#ifndef CPROTO

/*----------------------------------------------------------------------------*/

#define de_bruker_mri_generated_visu_CurrentVersion 5
#define de_bruker_mri_generated_visu_derive_Nothing 0
#define de_bruker_mri_generated_visu_derive_Instance 1
#define de_bruker_mri_generated_visu_derive_Subject 2
#define de_bruker_mri_generated_visu_derive_Study 4
#define de_bruker_mri_generated_visu_derive_ImageSeries 8
#define de_bruker_mri_generated_visu_derive_Equipment 16
#define de_bruker_mri_generated_visu_derive_ImageDescription 32
#define de_bruker_mri_generated_visu_derive_Acquisition 64
#define de_bruker_mri_generated_visu_derive_All 255
#define de_bruker_mri_generated_visu_derive_ForExperiment 1073741824
#define de_bruker_mri_generated_visu_overlay_AreaA_X 0.000000e+00
#define de_bruker_mri_generated_visu_overlay_AreaA_Y 0.000000e+00
#define de_bruker_mri_generated_visu_overlay_AreaA_W 3.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaA_H 3.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaB_X 3.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaB_Y 0.000000e+00
#define de_bruker_mri_generated_visu_overlay_AreaB_W 4.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaB_H 2.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaC_X 7.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaC_Y 0.000000e+00
#define de_bruker_mri_generated_visu_overlay_AreaC_W 3.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaC_H 1.500000e-01
#define de_bruker_mri_generated_visu_overlay_AreaD_X 0.000000e+00
#define de_bruker_mri_generated_visu_overlay_AreaD_Y 3.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaD_W 2.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaD_H 3.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaE_X 2.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaE_Y 2.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaE_W 5.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaE_H 5.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaF_X 8.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaF_Y 1.500000e-01
#define de_bruker_mri_generated_visu_overlay_AreaF_W 2.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaF_H 5.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaG_X 0.000000e+00
#define de_bruker_mri_generated_visu_overlay_AreaG_Y 6.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaG_W 3.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaG_H 4.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaH_X 3.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaH_Y 7.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaH_W 4.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaH_H 3.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaI_X 7.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaI_Y 6.500000e-01
#define de_bruker_mri_generated_visu_overlay_AreaI_W 3.000000e-01
#define de_bruker_mri_generated_visu_overlay_AreaI_H 3.500000e-01
#define de_bruker_mri_generated_visu_overlay_ScalePosition 9.300000e-01
#define de_bruker_mri_generated_visu_overlay_ScalebarMinimumHeight 150
#define de_bruker_mri_generated_visu_overlay_SmallScalebarTic 3
#define de_bruker_mri_generated_visu_overlay_BigScalebarTic 5
#define de_bruker_mri_generated_visu_overlay_ItemSpacing 5
#define de_bruker_mri_generated_visuid_series_ACQ_PREFIX "ACQ_"
#define de_bruker_mri_generated_visuid_series_BRUKER_PVM "ACQ_BRUKER_PVM"
#define de_bruker_mri_generated_visuid_series_USER_PVM "ACQ_USER_PVM"
#define de_bruker_mri_generated_visuid_series_IMND "ACQ_IMND"
#define de_bruker_mri_generated_visuid_series_ACQ "ACQ_DIRECT"
#define de_bruker_mri_generated_visuid_series_DERIVED_PREFIX "DERIVED_"
#define de_bruker_mri_generated_visuid_series_MPR "DERIVED_MPR"
#define de_bruker_mri_generated_visuid_series_ISA "DERIVED_ISA"
#define de_bruker_mri_generated_visuid_series_DTI "DERIVED_DTI"
#define de_bruker_mri_generated_visuid_series_MASK "DERIVED_MASK"
#define de_bruker_mri_generated_visuid_series_MAP "DERIVED_MAP"
#define de_bruker_mri_generated_visuid_series_MAP_ATTENUATION "DERIVED_MAP_ATTENUATION"
#define de_bruker_mri_generated_visuid_series_DERIVED_STITCHED "DERIVED_STITCHED"
#define de_bruker_mri_generated_visuid_series_DERIVED_REGISTERED "DERIVED_REGISTERED"
#define de_bruker_mri_generated_visuid_series_DERIVED_OTHER "DERIVED_OTHER"
#define de_bruker_mri_generated_visuid_series_IMPORT_PREFIX "IMPORT_"
#define de_bruker_mri_generated_visuid_series_IMPORT_ACQUIRED "IMPORT_ACQUIRED"
#define de_bruker_mri_generated_visuid_series_IMPORT_DERIVED "IMPORT_DERIVED"
#define de_bruker_mri_generated_visuid_deppars_EchoTime "VisuAcqEchoTime"
#define de_bruker_mri_generated_visuid_deppars_RepetitionTime "VisuAcqRepetitionTime"
#define de_bruker_mri_generated_visuid_deppars_InversionTime "VisuAcqInversionTime"
#define de_bruker_mri_generated_visuid_deppars_DataUnits "VisuCoreDataUnits"
#define de_bruker_mri_generated_visuid_deppars_ElementIds "VisuFGElemId"
#define de_bruker_mri_generated_visuid_deppars_ElementComments "VisuFGElemComment"
#define de_bruker_mri_generated_visuid_deppars_GenericDoubleValues "VisuDepValsDouble"
#define de_bruker_mri_generated_visuid_deppars_Position "VisuCorePosition"
#define de_bruker_mri_generated_visuid_deppars_Orientation "VisuCoreOrientation"
#define de_bruker_mri_generated_visuid_deppars_DiskSliceOrder "VisuCoreDiskSliceOrder"
#define de_bruker_mri_generated_visuid_deppars_ModalityOffset "VisuCoreModalityOffset"
#define de_bruker_mri_generated_visuid_deppars_FrameThickness "VisuCoreFrameThickness"
#define de_bruker_mri_generated_visuid_deppars_FrameType "VisuCoreFrameType"
#define de_bruker_mri_generated_visuid_deppars_DiffusionBMatrix "VisuAcqDiffusionBMatrix"
#define de_bruker_mri_generated_visuid_deppars_DiffusionGradOrient "VisuAcqDiffusionGradOrient"
#define de_bruker_mri_generated_visuid_deppars_ContrastAgentUsage "VisuContrastAgentUsage"
#define de_bruker_mri_generated_visuid_deppars_ContrastAgentPhase "VisuContrastAgentPhase"
#define de_bruker_mri_generated_visuid_deppars_MpiReconstruction "VisuMpiReconstruction"
#define de_bruker_mri_generated_visuid_deppars_MpiReconstructionUsedFrequencyComponents "VisuMpiUsedFreqComponents"
#define de_bruker_mri_generated_visuid_deppars_MpiReconstructionConstraints "VisuMpiReconstructionConstraints"
#define de_bruker_mri_generated_visuid_deppars_CardiacMovieFrameTime "VisuCardiacMovieFrameTime"
#define de_bruker_mri_generated_visuid_deppars_RespMovieFrameTime "VisuRespMovieFrameTime"
#define de_bruker_mri_generated_visuid_deppars_PtAcqStartConditions "VisuPtAcqStartConditions"
#define de_bruker_mri_generated_visuid_deppars_PtAcqStartValues "VisuPtAcqStartValues"
#define de_bruker_mri_generated_visuid_deppars_PtAcqStopConditions "VisuPtAcqStopConditions"
#define de_bruker_mri_generated_visuid_deppars_PtAcqStopValues "VisuPtAcqStopValues"
#define de_bruker_mri_generated_visuid_deppars_PtAcqInstrumentInfos "VisuPtAcqInstrumentInfos"
#define de_bruker_mri_generated_visuid_deppars_PtAcqCompoundUsage "VisuPtAcqCompoundUsage"
#define de_bruker_mri_generated_visuid_framegroup_COIL "FG_COIL"
#define de_bruker_mri_generated_visuid_framegroup_ECHO "FG_ECHO"
#define de_bruker_mri_generated_visuid_framegroup_IRMODE "FG_IRMODE"
#define de_bruker_mri_generated_visuid_framegroup_SLICE "FG_SLICE"
#define de_bruker_mri_generated_visuid_framegroup_MOVIE "FG_MOVIE"
#define de_bruker_mri_generated_visuid_framegroup_SATTRANS "FG_SATTRANS"
#define de_bruker_mri_generated_visuid_framegroup_DIFFUSION "FG_DIFFUSION"
#define de_bruker_mri_generated_visuid_framegroup_RESP_MOVIE "FG_RESP_MOVIE"
#define de_bruker_mri_generated_visuid_framegroup_CARDIAC_MOVIE "FG_CARDIAC_MOVIE"
#define de_bruker_mri_generated_visuid_framegroup_TIME_COURSE "FG_TIME_COURSE"
#define de_bruker_mri_generated_visuid_framegroup_TRIGGER "FG_TRIGGER"
#define de_bruker_mri_generated_visuid_framegroup_ACQ_FRAME_TIME "FG_ACQ_FRAME_TIME"
#define de_bruker_mri_generated_visuid_framegroup_RECO_FRAME_TIME "FG_RECO_FRAME_TIME"
#define de_bruker_mri_generated_visuid_framegroup_CYCLE "FG_CYCLE"
#define de_bruker_mri_generated_visuid_framegroup_COMPLEX "FG_COMPLEX"
#define de_bruker_mri_generated_visuid_framegroup_FLOW "FG_FLOW"
#define de_bruker_mri_generated_visuid_framegroup_ORIENT "FG_ORIENT"
#define de_bruker_mri_generated_visuid_framegroup_EVOLUTION "FG_EVOLUTION"
#define de_bruker_mri_generated_visuid_framegroup_REPETITION "FG_REPETITION"
#define de_bruker_mri_generated_visuid_framegroup_DTI "FG_DTI"
#define de_bruker_mri_generated_visuid_framegroup_ISA "FG_ISA"
#define de_bruker_mri_generated_visuid_framegroup_CONTRAST_MAP "FG_CONTRAST_MAP"
#define de_bruker_mri_generated_visuid_framegroup_CONTRAST_AGENT "FG_CONTRAST_AGENT"
#define de_bruker_mri_generated_visuid_framegroup_RECONSTRUCTION "FG_RECONSTRUCTION"
#define de_bruker_mri_generated_visuid_framegroup_PREFIX_SPECTROSCOPY "FG_SPECTROSCOPY"
#define de_bruker_mri_generated_visuid_framegroup_PREFIX_TEMPORAL "FG_TEMPORAL"
#define de_bruker_mri_generated_visuid_dti_TRACE "DTI_TRACE"
#define de_bruker_mri_generated_visuid_dti_FA "DTI_FA"
#define de_bruker_mri_generated_visuid_dti_A0 "DTI_A0"
#define de_bruker_mri_generated_visuid_dti_TWI "DTI_TWI"
#define de_bruker_mri_generated_visuid_dti_DXX "DTI_DXX"
#define de_bruker_mri_generated_visuid_dti_DYY "DTI_DYY"
#define de_bruker_mri_generated_visuid_dti_DZZ "DTI_DZZ"
#define de_bruker_mri_generated_visuid_dti_DXY "DTI_DXY"
#define de_bruker_mri_generated_visuid_dti_DXZ "DTI_DXZ"
#define de_bruker_mri_generated_visuid_dti_DYZ "DTI_DYZ"
#define de_bruker_mri_generated_visuid_dti_L1 "DTI_L1"
#define de_bruker_mri_generated_visuid_dti_L2 "DTI_L2"
#define de_bruker_mri_generated_visuid_dti_L3 "DTI_L3"
#define de_bruker_mri_generated_visuid_dti_L1X "DTI_L1X"
#define de_bruker_mri_generated_visuid_dti_L1Y "DTI_L1Y"
#define de_bruker_mri_generated_visuid_dti_L1Z "DTI_L1Z"
#define de_bruker_mri_generated_visuid_dti_L2X "DTI_L2X"
#define de_bruker_mri_generated_visuid_dti_L2Y "DTI_L2Y"
#define de_bruker_mri_generated_visuid_dti_L2Z "DTI_L2Z"
#define de_bruker_mri_generated_visuid_dti_L3X "DTI_L3X"
#define de_bruker_mri_generated_visuid_dti_L3Y "DTI_L3Y"
#define de_bruker_mri_generated_visuid_dti_L3Z "DTI_L3Z"
#define de_bruker_mri_generated_visuid_dti_ERR "DTI_ERR"
#define de_bruker_mri_generated_visuid_dti_TRACE_ERR "DTI_ERR_TRACE"
#define de_bruker_mri_generated_visuid_dti_FA_ERR "DTI_ERR_FA"
#define de_bruker_mri_generated_visuid_dti_A0_ERR "DTI_ERR_A0"
#define de_bruker_mri_generated_visuid_dti_TWI_ERR "DTI_ERR_TWI"
#define de_bruker_mri_generated_visuid_dti_DXX_ERR "DTI_ERR_DXX"
#define de_bruker_mri_generated_visuid_dti_DYY_ERR "DTI_ERR_DYY"
#define de_bruker_mri_generated_visuid_dti_DZZ_ERR "DTI_ERR_DZZ"
#define de_bruker_mri_generated_visuid_dti_DXY_ERR "DTI_ERR_DXY"
#define de_bruker_mri_generated_visuid_dti_DXZ_ERR "DTI_ERR_DXZ"
#define de_bruker_mri_generated_visuid_dti_DYZ_ERR "DTI_ERR_DYZ"
#define de_bruker_mri_generated_visuid_dti_L1_ERR "DTI_ERR_L1"
#define de_bruker_mri_generated_visuid_dti_L2_ERR "DTI_ERR_L2"
#define de_bruker_mri_generated_visuid_dti_L3_ERR "DTI_ERR_L3"
#define de_bruker_mri_generated_visuid_complex_IMAG "COMPLEX_IMAG"
#define de_bruker_mri_generated_visuid_complex_REAL "COMPLEX_REAL"
#define de_bruker_mri_generated_visuid_contrast_T1MAP "CONTRAST_T1MAP"
#define de_bruker_mri_generated_visuid_contrast_T2MAP "CONTRAST_T2MAP"
#define de_bruker_mri_generated_visuid_contrast_T1STARMAP "CONTRAST_T1STARMAP"
#define de_bruker_mri_generated_visuid_contrast_PDMAP "CONTRAST_PDMAP"
#define de_bruker_mri_generated_visuid_contrast_TRUFI_STEADYSTATE "TRUFI_STEADYSTATE"
#define de_bruker_mri_generated_visuid_contrast_TRUFI_INVDEGREE "TRUFI_INVDEGREE"
/*------------------------------------------------------------------------------
 * $Source$
 *  - Footer for header files generated from IDL
 *
 * $Id$
 *----------------------------------------------------------------------------*/

#endif

#endif

/*========================================================================EOF=*/

