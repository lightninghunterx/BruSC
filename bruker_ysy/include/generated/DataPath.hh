/*==============================================================================
 * genX/h/generated/DataPath.hh 
 *    Header file generated from IDL
 *
 * THIS FILE IS GENERATED AUTOMATICALLY -- DO NOT EDIT
 *
 * Header: gen/src/idl/Generic_header.hh
 * Body  : via pseudo IDL compiler from gen/idl/DataPath.idl
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

#ifndef DataPath_hh
#define DataPath_hh


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
                namespace datapath
                {
                    const int MaxPrefixLen = 335;
                    const int MaxPrefixCStrLen = 336;
                    const int MaxNameLen = 95;
                    const int MaxNameCStrLen = 96;
                    const int MaxProcnoLen = 458;
                    const int MaxProcnoCStrLen = 459;
                    const int MaxDuLen = 255;
                    const int MaxDuCStrLen = 256;
                    const int MaxUserLen = 63;
                    const int MaxUserCStrLen = 64;
                    const int MaxTypeLen = 15;
                    const int MaxTypeCStrLen = 16;
                    const int MinExpno = 1;
                    const int MaxExpno = 999999;
                    const int MinProcno = 1;
                    const int MaxProcno = 999999;
                }
                namespace paramfiles
                {
                    const char* const SubjectFile = "subject";
                    const char* const StudyModalityFilePrefix = "study";
                    const char* const MethodFile = "method";
                    const char* const MethodOutFile = "method.out";
                    const char* const ImndFile = "imnd";
                    const char* const ConfigScanFile = "configscan";
                    const char* const AcqpFile = "acqp";
                    const char* const AcqpOutFile = "acqp.out";
                    const char* const MethRecoFile = "methreco";
                    const char* const MethRecoOutFile = "methreco.out";
                    const char* const RecoFile = "reco";
                    const char* const RecoOutFile = "reco.out";
                    const char* const D3File = "d3proc";
                    const char* const ProcFile = "procs";
                    const char* const VisuFile = "visu_pars";
                    const char* const ProcParsFile = "proc_pars";
                    const char* const DsetIdFile = "id";
                    const char* const PvMetaFile = "pvmeta";
                    const char* const TwoDSeqFile = "2dseq";
                    const char* const RawDataFilePrefix = "rawdata";
                    const char* const RawDataFileJobSuffix = "job";
                    const char* const FidFile = "fid";
                    const char* const SerFile = "ser";
                }
                namespace paramgroups
                {
                    const char* const SubjectGroup = "SUBJECT";
                    const char* const StudyModalityGroup = "STUDY_MODALITY";
                    const char* const MethodGroup = "MethodClass";
                    const char* const MethodFileGroup = "MethodFileGroup";
                    const char* const MethodOutGroup = "MethodOutGroup";
                    const char* const ProtocolGroup = "ProtocolGroup";
                    const char* const StandardProtocolGroup = "StandardProtocolGroup";
                    const char* const ConfigScanGroup = "CONFIG_SCAN";
                    const char* const AcqpGroup = "ACQP";
                    const char* const AcqpOutGroup = "ACQP_OUT";
                    const char* const MethRecoGroup = "MethodRecoGroup";
                    const char* const MethRecoOutGroup = "MethodRecoOutGroup";
                    const char* const RecoGroup = "RECO";
                    const char* const RecoOutGroup = "RECO_OUT";
                    const char* const D3Group = "D3";
                    const char* const ProcGroup = "PROC";
                    const char* const VisuGroup = "Visu";
                    const char* const DsetIdGroup = "DATASET_ID";
                    const char* const PvMetaGroup = "PV_META";
                }
                namespace paramnames
                {
                    const char* const MethodParName = "Method";
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

