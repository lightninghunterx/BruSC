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

OVERLAY = DtiEpi

OBJS = \
	initMeth.o \
	loadMeth.o \
	Phase3D.o \
	SliceSel.o \
	RFPulses.o \
	parsRelations.o \
	BaseLevelRelations.o \
	RecoRelations.o \
	deriveVisu.o \
	backbone.o \
	adjust.o 

PPGS = DtiEpi.ppg

CPROTO_HEADER = relProtos

include $(EXECDISKUNIT)/prog/include/make/MethodMRI.makefile

