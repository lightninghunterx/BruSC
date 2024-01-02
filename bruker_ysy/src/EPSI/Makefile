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

OVERLAY = EPSI

OBJS = \
	initMeth.o \
	loadMeth.o \
	parsRelations.o \
	BaseLevelRelations.o \
	RecoRelations.o \
	adjust.o \
	deriveVisu.o \
	backbone.o 

PPGS = EPSI.ppg

CPROTO_HEADER = relProtos

include $(EXECDISKUNIT)/prog/include/make/MethodMRI.makefile

