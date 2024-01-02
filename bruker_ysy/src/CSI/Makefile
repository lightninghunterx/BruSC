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

OVERLAY = CSI

OBJS = \
	initMeth.o \
	loadMeth.o \
	parsRelations.o \
	backbone.o \
	BaseLevelRelations.o \
	RecoRelations.o \
	LocalizationRelations.o \
	LocalizationBaselevel.o \
	LocalizationInit.o 

PPGS = CSI.ppg

CPROTO_HEADER = relProtos

include $(EXECDISKUNIT)/prog/include/make/MethodMRI.makefile

