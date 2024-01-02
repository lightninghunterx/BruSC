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

OVERLAY = MDEFT

OBJS = \
	initMeth.o \
	loadMeth.o \
	parsRelations.o \
	BaseLevelRelations.o \
	RecoRelations.o \
	backbone.o \
	MdeftPrepHandler.o \
	deriveVisu.o 

PPGS = \
	MDEFT.ppg \
	MdeftPrep.mod 

CPROTO_HEADER = relProtos

include $(EXECDISKUNIT)/prog/include/make/MethodMRI.makefile

