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

OVERLAY = ISIS

OBJS = \
	initMeth.o \
	parsRelations.o \
	backbone.o \
	loadMeth.o \
	BaseLevelRelations.o \
	RecoRelations.o 

PPGS = \
	ISIS.ppg \
	ISIS_Head.mod \
	ISISADJ.ppg 

CPROTO_HEADER = relProtos

include $(EXECDISKUNIT)/prog/include/make/MethodMRI.makefile

