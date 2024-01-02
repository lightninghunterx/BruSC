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

OVERLAY = FAIR_RARE

OBJS = \
	initMeth.o \
	loadMeth.o \
	parsRelations.o \
	backbone.o \
	BaseLevelRelations.o \
	solve.o \
	RecoRelations.o \
	deriveVisu.o 

PPGS = FAIR_RARE.ppg

CPROTO_HEADER = relProtos

include $(EXECDISKUNIT)/prog/include/make/MethodMRI.makefile

