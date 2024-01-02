#****************************************************************
#
# Copyright (c) 2020
# Bruker BioSpin MRI GmbH
# D-76275 Ettlingen, Germany
#
# All Rights Reserved
#
#****************************************************************

SRCDIR ?= $(patsubst %/,%,$(dir $(realpath $(firstword $(MAKEFILE_LIST)))))

PLUGINDIR ?= $(patsubst %/,%,$(realpath $(SRCDIR)/../..))

SRCS ?= ConvertObjsToSrcs($(OBJS),$(PPSUFFIX))

DEFINES ?= -DIsaModel_INTERNAL

LIBPATH ?= $(CORBA_LIBPATH)

LIBRARIES ?= $(CORBA_LIBRARIES)

ISA_DEBUG ?= 1

ifeq ($(ISA_DEBUG), 0)
include $(EXECDISKUNIT)/prog/include/Isa/make/IsaOpt.makefile
else
include $(EXECDISKUNIT)/prog/include/Isa/make/IsaDebug.makefile
endif
