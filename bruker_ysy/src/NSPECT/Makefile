# -------------------------------------------------------------
# 
# Copyright (c) 2020
# Bruker BioSpin MRI GmbH
# D-76275 Ettlingen, Germany
#
# All Rights Reserved
#
# You can freely change this Makefile.
#
# -------------------------------------------------------------

EXECDISKUNIT = /opt/PV-360.3.0

OVERLAY = NSPECT

OBJS = \
	initMeth.o \
	loadMeth.o \
	parsRelations.o \
	SliceSel.o \
	backbone.o \
	BaseLevelRelations.o \
	RecoRelations.o 

PPGS = NSPECT.ppg

CPROTO_HEADER = relProtos

include $(EXECDISKUNIT)/prog/include/make/MethodMRI.makefile

