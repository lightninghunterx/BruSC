# -------------------------------------------------------------
# 
# Copyright (c) 2001-2020
# Bruker BioSpin MRI GmbH
# D-76275 Ettlingen, Germany
#
# All Rights Reserved
#
# -------------------------------------------------------------


$(VERBOSE).SILENT:

.PHONY: showppg

showppg::

print-% : ; $(info $* = $($*)) @true

DEFAULT_SYMBOL_VISIBILITY = -fvisibility=hidden

PROGLIBLDFLAGS = $(CC_EXEC_PREFIX_OPTION) $(PT_GNU_RELRO) $(PV_COMPILE_FLAGS) $(COMMON_FLAG) -fno-gnu-unique -fexceptions $(LINK_OPTIONS)

RABBITMQ_ROOT	= $(BUILD_TOOL_ROOT)/rabbitmq-c-0.10.0

NO_LINKTIME_OPTIMIZE = -fno-lto

PT_GNU_RELRO	= -z relro -z now

CC_ROOT		= $(EXECDISKUNIT)/gnu

PIC_FLAG	= -fPIC

COMMON_FLAG	= -fno-common

EXCEPTION_FLAG	= -fexceptions

CC_EXEC_PREFIX_OPTION	= -B $(CC_ROOT)/lib/gcc

STDCXX_FLAGS	= -std=c++14

.SUFFIXES:            # Delete the default suffixes

tests::

WARN_AS_ERROR = -Werror

WARN_LINK_AS_ERROR = -Wl,--fatal-warnings

XWINSHLIBDIR = $(PROGDIR)/shlib
XWINSHLIBDIROPT = -L$(XWINSHLIBDIR)

THIRDPARTY_SHLIBDIR = $(PROGDIR)/pvshlib
THIRDPARTY_SHLIBDIROPT = -L$(THIRDPARTY_SHLIBDIR)

XWINSHARELIBEXT = $(SHARELIBEXT)

PVSHLIBDIR = $(PROGDIR)/pvshlib

RMD		= rm -rf

SHARELIBBASEOPTS = -shared

PV_EXTRA_DEFINES =

PV_DEFINES = -D_REENTRANT -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE

PV_ARCH_FLAG = -m64

PV_COMPILE_FLAGS = $(DEFAULT_SYMBOL_VISIBILITY) $(PV_ARCH_FLAG) $(PIC_FLAG) -rdynamic

CREATE_LINK_LIST = $(PERL)  $(PROGDIR)/bin/createLinkList.pl

MAKEFILE_DEP = $(SRCDIR)/Makefile

DEPDIR = depdir

AWK		= awk

BASENAME	= basename

BASH		= bash

CAT		= cat

CC		= LD_LIBRARY_PATH=$(CC_ROOT)/lib64 $(CC_ROOT)/bin/gcc -B$(CC_ROOT)/lib/gcc/

CHMOD		= chmod

CHOWN		= chown

CKSUM		= cksum

CMP		= cmp

CP		= cp --remove-destination --update -p

CP_R		= cp -ax

CPASC		= cp --remove-destination --update -p

CUT		= cut

DATE		= date

DIFF		= diff

DIRNAME		= dirname

DOS2UNIX	= dos2unix

ECHO		= /bin/echo -e

FIND		= find

GREP		= grep

HEAD		= head

IMAKE_CPP	= $(CPP)

LN		= ln -f

LS		= ls

MAKE		= make

MD5SUM		= md5sum

MKDIR		= mkdir -p

MV		= mv -f

NM		= nm

NOOP		= true

PERL		= $(EXECDISKUNIT)/perl/bin/perl -I$(PROGDIR)/perl

RCP		= rcp

READLINK	= readlink

RM		= rm -f --

RMDIR		= rm -rf

RSYNC	= rsync -a --no-owner

SCP		= scp

SED		= sed

SED_OPT_INPLACE	= -i

SHARELIBCMD	=

SHELL		= sh

TOUCH		= touch

UNAME		= uname

WHOAMI		= whoami

NOOP		= true

STRIP		= $(CC_ROOT)/bin/strip

SORT		= sort -u

DOS2UNIX	= dos2unix

SSH		= ssh

COMM		= comm

NM		= nm

ZIPCMD		= gzip -f

UNZIPCMD	= gunzip -f

SUDO	= /usr/bin/sudo -n

READLINK	= readlink

TOP		= .
CURRENT_DIR	= .

RM_CMD		= $(RM) *.CKP *.ln *.BAK *.bak *.o core errs ,* *~ *.a *.lib .emacs_* tags TAGS make.log MakeOut

APP_SUBSYSTEM	=

STD_LIBRARIES	= -ldl -lrt -lpthread $(EXTRA_STDLIBS)
ALL_LIBRARIES	= $(LIBRARIES) $(EXTRA_LIBRARIES) $(STD_LIBRARIES)

STD_LIBPATH	=
ALL_LIBPATH	= $(LIBPATH) $(STD_LIBPATH)

STD_INCPATH	= -I $(GLOBALHEADERDIR)
ALL_INCPATH	= $(INCPATH) $(EXTRA_INCPATH) $(STD_INCPATH)

STD_DEFINES	= -D_REENTRANT -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE
OS_DEFINES	= -DLINUX_INTEL
DLL_DEFINES	=
ALL_DEFINES	= $(OS_DEFINES) $(DEFINES) $(EXTRA_DEFINES) $(DLL_DEFINES) $(STD_DEFINES)

CDEBUGFLAGS	= -O2 -DNDEBUG $(LINKTIME_OPTIMIZE)

CCOPTIONS	= $(CC_EXEC_PREFIX_OPTION) $(PV_COMPILE_FLAGS) $(COMMON_FLAG) -Wall -Wextra $(WARN_AS_ERROR) -Wcast-qual -Wpointer-arith -Wsign-compare -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Winline -Wdisabled-optimization -Waggressive-loop-optimizations -Wlogical-op -Wconversion -Wcast-align -pedantic -std=gnu99 -fexceptions $(COMPILE_OPTIONS)

CCLDOPTIONS	= $(CC_EXEC_PREFIX_OPTION) $(PT_GNU_RELRO) -O2 $(LINKTIME_OPTIMIZE) $(PV_COMPILE_FLAGS) $(COMMON_FLAG) -fno-gnu-unique -fexceptions $(LINK_OPTIONS)

STATICLIBGCC	= -static-libgcc

LDDEBUGFLAGS	=

LDOPTIONS	=

DLLLDOPTIONS	=

CFLAGS		= $(CDEBUGFLAGS) $(CCOPTIONS) $(ALL_INCPATH) $(ALL_DEFINES)
CCLDFLAGS	= $(CDEBUGFLAGS) $(CCOPTIONS) $(CCLDOPTIONS) $(ALL_LIBPATH)

EXECEXT		=

OBJEXT		= .o

STATICLIBEXT	= .a

STATICCRTLIBEXT	= .a

DYNAMICLIBEXT	= .so

ASMEXT		= .asm

ROMEXT		= .rom

ZIPEXT		= .gz

SHARELIBEXT	= .so

BOOST_LIBRARYPATH = $(SHLIBDIR)

NLOPT_LIBRARIES	= $(NLOPT_ROOT)/lib64/libnlopt.a
NLOPT_INCPATH	= -isystem $(NLOPT_ROOT)/include

MATH_LIBRARIES	= -lm

FILESYSTEM_LIBRARIES = -lstdc++fs

FORTRAN_LIBRARIES = -lgfortran

PTHREAD_LIBRARIES = -lpthread

RT_LIBRARIES	= -lrt

PROGDIR		= $(EXECDISKUNIT)/prog

STANDALONEDIR	= $(EXECDISKUNIT)/prog/bin

DLLDIR		= $(PROGDIR)/pvshlib

SHLIBDIR	= $(PROGDIR)/pvshlib

GLOBALHEADERDIR	= $(PROGDIR)/include

TMPDIR		= /tmp

DEPENDFLAGS	= -MT $@ -MD -MP -MF $(DEPDIR)/$(@F:$(OBJEXT)=.Td)

CPLUSPLUS		= LD_LIBRARY_PATH=$(CC_ROOT)/lib64 $(CC_ROOT)/bin/c++ -B$(CC_ROOT)/lib/gcc/

STD_PLUSPLUS_LIBRARIES	= -ldl -lrt -lpthread $(EXTRA_STDLIBS)
ALL_PLUSPLUS_LIBRARIES	= $(LIBRARIES) $(EXTRA_LIBRARIES) $(STD_PLUSPLUS_LIBRARIES)

STD_PLUSPLUS_LIBPATH 	=
ALL_PLUSPLUS_LIBPATH	= $(LIBPATH) $(STD_PLUSPLUS_LIBPATH)

STD_PLUSPLUS_INCPATH 	= $(STD_INCPATH)
ALL_PLUSPLUS_INCPATH	= $(INCPATH) $(EXTRA_INCPATH) $(STD_PLUSPLUS_INCPATH)

STATICSTDCPLUSPLUS	= -static-libgcc -static-libstdc++

SHAREDSTDCPLUSPLUS	= libgcc_s.so.1 libstdc++.so.6

STD_PLUSPLUS_DEFINES 	= -D_REENTRANT -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE
ALL_PLUSPLUS_DEFINES	= $(OS_DEFINES) $(DEFINES) $(EXTRA_DEFINES) $(DLL_DEFINES) $(STD_PLUSPLUS_DEFINES)

CPLUSPLUSDEBUGFLAGS	= -O2 -DNDEBUG $(LINKTIME_OPTIMIZE)

CPLUSPLUSOPTIONS	= -x c++ $(CC_EXEC_PREFIX_OPTION) $(PV_COMPILE_FLAGS) $(STDCXX_FLAGS) -Wall -Wextra $(WARN_AS_ERROR) -Weffc++ -Wcast-qual -Wpointer-arith -Wsign-compare -Wundef -Wdisabled-optimization -Waggressive-loop-optimizations -Wshadow -Wlogical-op -Woverloaded-virtual -Wwrite-strings -Wmissing-declarations -Wcast-qual -Wcast-align -Wno-variadic-macros -Wno-long-long -Wstrict-null-sentinel $(COMMON_FLAG) -fimplicit-templates -fnon-call-exceptions  $(COMPILE_OPTIONS) -fvisibility-inlines-hidden
CPLUSPLUSLDOPTIONS	= $(CC_EXEC_PREFIX_OPTION) $(PT_GNU_RELRO) -O2 $(LINKTIME_OPTIMIZE) $(PV_COMPILE_FLAGS) $(COMMON_FLAG) -fno-gnu-unique -fexceptions $(LINK_OPTIONS)

CPLUSPLUSFLAGS		= $(CPLUSPLUSDEBUGFLAGS) $(CPLUSPLUSOPTIONS) $(ALL_PLUSPLUS_INCPATH) $(ALL_PLUSPLUS_DEFINES)

CPLUSPLUSLDFLAGS	= $(CPLUSPLUSLDOPTIONS) $(ALL_PLUSPLUS_LIBPATH)

PLUSPLUSDEPENDFLAGS 	= -MT $@ -MD -MP -MF $(DEPDIR)/$(@F:$(OBJEXT)=.Td)

PLUSPLUSDEPEND		= Depend

PPSUFFIX		= .cc

$(DEPDIR):
	$(MKDIR) $(DEPDIR)

define CREATE_SIMPLE_DEPS

$1 : $3/$(notdir $(1:$(OBJEXT)=$2)) $(DEPDIR)/$(notdir $(1:$(OBJEXT)=.d)) | $(DEPDIR)
$(DEPDIR)/$(notdir $(1:$(OBJEXT)=.d)): ;

endef

clean:: depdirclean

depdirclean: | $(DEPDIR)
	$(RM) $(addsuffix *.d,$(DEPDIR)/)

define INSTALL_FILES_FCN
install:: $$(strip $(2))
$$(strip $(2)) : $(1) | $$(abspath $$(dir $(2)))
	$(RM) $(2)
	$(CP) $(1) $(2)

clean::
	$(RM) $2
endef

define INSTALL_ASCII_FILES_FCN
install:: $$(strip $(2))
$$(strip $(2)) : $(1) | $$(abspath $$(dir $(2)))
	$(RM) $(2)
	$(CPASC) $(1) $(2)

clean::
	$(RM) $2
endef

define COPY_TARGET
$(eval $(call MKDIR_FCN,$$(abspath $$(dir $2))))
$2 : $1 | $$(abspath $$(dir $2))
	$$(info Copy $1 to $2)
	$$(RM) $2
	$$(CP) $1 $2

clean::
	$$(RM) $2

endef

define MKDIR_FCN

ifeq ($$(filter $(1),$$(PV_CREATE_DIRS)),)
PV_CREATE_DIRS += $(1)

$$(strip $(1)):
	$$(MKDIR) $(1)

endif
endef

.PHONY : all clean depend dll docu dvd emptyrule idl install release test

DEPEND_START = \# DO NOT DELETE THIS LINE -- make depend depends on it

TEST_LABEL	= test

test:: test_exec

test test_exec:: test_build

test_build::

SRCDIR ?= $(patsubst %/,%,$(dir $(realpath $(firstword $(MAKEFILE_LIST)))))

STD_FORTRAN_LIBRARIES	= -ldl -lrt -lpthread $(EXTRA_STDLIBS)

ALL_FORTRAN_LIBRARIES	= $(LIBRARIES) $(EXTRA_LIBRARIES) $(STD_FORTRAN_LIBRARIES)

SIMPLE_FORTRAN_LIBRARIES = -lgfortran

STD_FORTRAN_LIBPATH 	=
ALL_FORTRAN_LIBPATH	= $(LIBPATH) $(STD_FORTRAN_LIBPATH)

STD_FORTRAN_INCPATH 	= -I $(GLOBALHEADERDIR)
ALL_FORTRAN_INCPATH	= $(INCPATH) $(EXTRA_INCPATH) $(STD_FORTRAN_INCPATH)

STATICSTDFORTRAN	= -static

STD_FORTRAN_DEFINES 	= -D_REENTRANT -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE
ALL_FORTRAN_DEFINES	= $(OS_DEFINES) $(DEFINES) $(EXTRA_DEFINES) $(DLL_DEFINES) $(STD_FORTRAN_DEFINES)

FORTRANDEBUGFLAGS	= -O2 -DNDEBUG $(LINKTIME_OPTIMIZE) -std=legacy -fdefault-integer-8 -frecord-marker=8 -ffast-math -fstrict-aliasing -funroll-all-loops

FORTRANNOOPTDEBUGFLAGS	= -fPIC -DNDEBUG -std=legacy -fdefault-integer-8 -frecord-marker=8

FORTRANOPTIONS		= $(CC_EXEC_PREFIX_OPTION) $(PV_COMPILE_FLAGS) -Wall -W -Winline -Wdisabled-optimization $(COMPILE_OPTIONS) -Wno-compare-reals -Wno-conversion -Wno-unused -Wno-unused-parameter -Wno-unused-dummy-argument -Wno-intrinsic-shadow -Wno-intrinsics-std -Wno-conversion-extra -Wno-surprising -Wno-maybe-uninitialized -Wno-unused-but-set-variable -Wno-unused-variable

FORTRANLDOPTIONS	= $(CC_EXEC_PREFIX_OPTION) $(PT_GNU_RELRO) -O2 $(LINKTIME_OPTIMIZE) $(PV_COMPILE_FLAGS) $(COMMON_FLAG) -fno-gnu-unique -fexceptions $(LINK_OPTIONS)

FORTRANFLAGS		= $(FORTRANDEBUGFLAGS) $(FORTRANOPTIONS) $(ALL_FORTRAN_INCPATH) $(ALL_FORTRAN_DEFINES)

FORTRANNOOPTFLAGS	= $(FORTRANNOOPTDEBUGFLAGS) $(FORTRANOPTIONS) $(ALL_FORTRAN_INCPATH) $(ALL_FORTRAN_DEFINES)

FORTRANLDFLAGS	= $(FORTRANLDOPTIONS) $(ALL_FORTRAN_LIBPATH)

FORTRANDEPENDFLAGS 	= -MD -MP -MF $(DEPDIR)/$(@F:$(OBJEXT)=.Td) -cpp

FORTRANDEPEND		= Depend

FSUFFIX			= .f

LAPACK_COPYLIBS = $(LAPACK_DIR)/lib64/liblapack.so.3 $(LAPACK_DIR)/lib64/libblas.so.3
LAPACK_LIBRARIES = $(LAPACK_DIR)/lib64/liblapack$(SHARELIBEXT) $(LAPACK_DIR)/lib64/libblas$(SHARELIBEXT) $(SIMPLE_FORTRAN_LIBRARIES)

PARCOMP_OPTS     =
PARCOMP_DEFINES  = -DInParxOvl
PARCOMP_DEPENDFLAGS = -d -MT,$@,-MD,-MP,-MF,$(DEPDIR)/$(basename $(@F)).Td
PARCOMP_FLAGS	 = $(PARCOMP_DEPENDFLAGS) $(PARX_EXTRA_FLAGS) -I. $(ALL_INCPATH) $(DEFINES) $(PARCOMP_DEFINES) $(PARCOMP_OPTS) $(PARCOMP_DEBUG)

PARCOMP	         = $(PROGDIR)/bin/scripts/methcomp

PARX_OVL_LIBPATH = -L$(SHLIBDIR) -L$(XWINSHLIBDIR)
PARX_OVL_LIBS	 = $(LIBS)
PARX_OVL_LIBRARIES = $(LIBRARIES) $(STD_LIBRARIES)
PARX_OVL_LD      = LD_LIBRARY_PATH=$(CC_ROOT)/lib64 $(CC_ROOT)/bin/c++ -B$(CC_ROOT)/lib/gcc/
PARX_OVL_EXT     = $(SHARELIBEXT)
PARX_OVL_DEF_EXT = .ovl
PARX_OVL_EXT_EXT = .def
PARX_OVL_AR      = $(AR)
PARX_OVL_ARFLAGS =
PARX_TB_EXT      = $(SHARELIBEXT)
PARX_TB_DEF_EXT  = .c

STD_COMMON_METHOD_LIBS = \
$(SHLIBDIR)/libPvCfgTools$(SHARELIBEXT) \
$(SHLIBDIR)/libPvGeo$(SHARELIBEXT) \
$(SHLIBDIR)/libPvPvmTools$(SHARELIBEXT) \
$(SHLIBDIR)/libPvUtilTools$(SHARELIBEXT) \
$(SHLIBDIR)/libPvGeoTools$(SHARELIBEXT) \
$(SHLIBDIR)/libPvSeqTools$(SHARELIBEXT) \
$(SHLIBDIR)/libPvOvlTools$(SHARELIBEXT) \
$(SHLIBDIR)/libAtsProperties$(SHARELIBEXT) \
$(SHLIBDIR)/libModality$(SHARELIBEXT) \
$(SHLIBDIR)/libPvAdj$(SHARELIBEXT) \
$(SHLIBDIR)/libLicenseService$(SHARELIBEXT) \
$(SHLIBDIR)/libLogService$(SHARELIBEXT) \
$(SHLIBDIR)/libCommonConfiguration$(SHARELIBEXT) \
$(SHLIBDIR)/libVisu$(SHARELIBEXT) \
$(SHLIBDIR)/libVisuRels$(SHARELIBEXT) \
$(SHLIBDIR)/libParxRels$(SHARELIBEXT) \
$(SHLIBDIR)/libParxBase$(SHARELIBEXT) \
$(SHLIBDIR)/libParxUtils$(SHARELIBEXT) \
$(SHLIBDIR)/libPvDatabase$(SHARELIBEXT) \
$(SHLIBDIR)/libPvVersion$(SHARELIBEXT) \
$(SHLIBDIR)/libPvUtil$(SHARELIBEXT) \
$(SHLIBDIR)/libPluma$(SHARELIBEXT) \
$(SHLIBDIR)/libPvnl$(SHARELIBEXT) \
$(SHLIBDIR)/libPvnlFT$(SHARELIBEXT) \
$(SHLIBDIR)/libimproc$(SHARELIBEXT) \
$(SHLIBDIR)/libCmd$(SHARELIBEXT) \
$(SHLIBDIR)/libCmdBase$(SHARELIBEXT) \
$(SHLIBDIR)/libCoreMisc$(SHARELIBEXT) \
$(SHLIBDIR)/libCoreThread$(SHARELIBEXT) \
$(SHLIBDIR)/libCoreCommon$(SHARELIBEXT) \
$(SHLIBDIR)/libCoreRemote$(SHARELIBEXT) \
$(SHLIBDIR)/libPvIdl$(SHARELIBEXT) \
$(SHLIBDIR)/libLogger$(SHARELIBEXT)

PARX_OVL_MODALITY = CT

PARX_OVL_PROG_LIBPATH =
PARX_OVL_PROG_LIBRARIES = -lomniCodeSets4 -lomnisslTP4  -lomniORB4 -lomnithread -lssl -lcrypto -lpthread -llapack -lblas -lgfortran -lm -lz

PARX_SCRIPTS_DIR = $(PROGDIR)/parx/scripts
PARX_MAKE_INCLUDE_DIR = $(PROGDIR)/include/make
EXPORT_METHOD_SCRIPT = $(GLOBALCONFIGDIR)/ExportMethod.pl

EXTRA_DEFINES    = $(PARCOMP_DEFINES)

CPROTO = $(PROGDIR)/bin/cproto
CPROTOOPTIONS   = -E"$(CC) $(CC_EXEC_PREFIX_OPTION) -E" -f2
CPROTO_DEFINES  = -DCPROTO -D__float128=double
CPROTOFLAGS     = $(CPROTOOPTIONS) $(CPROTO_DEFINES) $(ALL_DEFINES) $(subst -isystem, -I, $(ALL_INCPATH))
PARX_PROTOHEAD  = $(GLOBALHEADERDIR)/Parx/Rels/ParxProtoHead.h
PARX_PROTOFOOT  = $(GLOBALHEADERDIR)/Parx/Rels/ParxProtoFoot.h

CREATE_PVUID    = $(STANDALONEDIR)/scripts/pvcmd -a ParxServer -r CreateIsoUid

STD_CT_METHOD_LIBS = \
$(SHLIBDIR)/libPvOvlToolsCt$(SHARELIBEXT) \
$(SHLIBDIR)/libPvCfgCT$(SHARELIBEXT) $(STD_COMMON_METHOD_LIBS)

.PHONY : cproto

.PHONY: instclean instlist

define PARX_LIST_INSTALLED

.PHONY: instlist_$1
instlist_$1: ; $$(info $2) @true

instlist: instlist_$1

endef

installppg :

ifeq (,$(findstring curdir,$(SRCDIR)))
METHODS_DIR ?= $(EXECDISKUNIT)/prog/parx/pub
else
METHODS_DIR ?= $(patsubst %/,%,$(realpath $(SRCDIR)/../..))
endif

SRCS ?= $(addprefix $(SRCDIR)/, $(addsuffix .c, $(basename $(notdir $(OBJS)))))

LIBS ?= $(STD_CT_METHOD_LIBS)

PARCOMP_DEBUG ?= -g

ifdef CPROTO_HEADER

cproto::
	-if [ ! -r $(CPROTO_HEADER)_p.h ]; then			  		\
		$(TOUCH) $(CPROTO_HEADER)_p.h;				\
	fi
	$(CAT) < $(PARX_PROTOHEAD) > $(CPROTO_HEADER).new
	$(CPROTO) $(CPROTOFLAGS) -O $(CPROTO_HEADER).err $(SRCS) >> $(CPROTO_HEADER).new
	$(CAT) < $(PARX_PROTOFOOT) >> $(CPROTO_HEADER).new
	$(CAT) $(CPROTO_HEADER).err
	file=$(CPROTO_HEADER).err; test -z "`cat $$file`"
	$(CMP) -s $(CPROTO_HEADER)_p.h $(CPROTO_HEADER).new || \
	{ \
		$(ECHO) Install prototype file $(CPROTO_HEADER)_p.h; \
		$(RM) $(CPROTO_HEADER)_p.h; \
		$(MV) $(CPROTO_HEADER).new $(CPROTO_HEADER)_p.h; \
	}
	$(RM) $(CPROTO_HEADER).new
	$(RM) $(CPROTO_HEADER).err
endif

$(OVERLAY).id : $(OVERLAY)$(OBJEXT) $(OBJS)
	$(CREATE_PVUID) > $(OVERLAY).id
$(OVERLAY)$(PARX_OVL_EXT): $(OVERLAY).id

all::	$(OVERLAY)$(PARX_OVL_EXT)

install_SUBDIRS:
$(SUBDIR_OBJS) : install_SUBDIRS
$(OVERLAY).objlist :  $(OVERLAY)$(OBJEXT) $(OBJS) $(SUBDIR_OBJS)
	$(RM) $(OVERLAY).objlist
	$(TOUCH) $(OVERLAY).objlist
	$(CREATE_LINK_LIST) -o $(OVERLAY).objlist $(OVERLAY)$(OBJEXT) $(OBJS) $(SUBDIR_OBJS)

clean::
	$(RM) $(OVERLAY).objlist

install:: $(OVERLAY)$(PARX_OVL_EXT)

$(foreach obj,$(notdir $(GLOBALHEADERDIR)/Parx/Rels/_ParxMain)$(OBJEXT),$(eval $(call CREATE_SIMPLE_DEPS,$(obj),$(PPSUFFIX), $(dir $(GLOBALHEADERDIR)/Parx/Rels/_ParxMain))))

$(notdir $(GLOBALHEADERDIR)/Parx/Rels/_ParxMain)$(OBJEXT): | $(DEPDIR)
	$(info Compile  $(dir $(GLOBALHEADERDIR)/Parx/Rels/_ParxMain)/$(@F:$(OBJEXT)=$(PPSUFFIX)))
	$(CPLUSPLUS) $(PLUSPLUSDEPENDFLAGS) $(CPLUSPLUSFLAGS) -c  $(dir $(GLOBALHEADERDIR)/Parx/Rels/_ParxMain)/$(@F:$(OBJEXT)=$(PPSUFFIX)) &&\
	$(MV) $(DEPDIR)/$(@F:$(OBJEXT)=.Td) $(DEPDIR)/$(@F:$(OBJEXT)=.d) && touch $@
-include $(addprefix $(DEPDIR)/,$(addsuffix .d,$(basename $(notdir $(notdir $(GLOBALHEADERDIR)/Parx/Rels/_ParxMain)$(OBJEXT)))))

$(OVERLAY)$(PARX_OVL_EXT):  $(MAKEFILE_DEP) $(notdir $(GLOBALHEADERDIR)/Parx/Rels/_ParxMain)$(OBJEXT) $(PARX_OVL_LIBS) $(OVERLAY).objlist
	$(info Link $(OVERLAY)$(PARX_OVL_EXT))
	$(RM) $(OVERLAY)$(PARX_OVL_EXT)
	$(RM) $(OVERLAY).program
	$(MKDIR) $(join link_,$(OVERLAY))
	TMPDIR=$(join link_,$(OVERLAY)) $(PARX_OVL_LD) $(PROGLIBLDFLAGS)  $(notdir $(GLOBALHEADERDIR)/Parx/Rels/_ParxMain)$(OBJEXT) @$(OVERLAY).objlist \
	  $(PARX_OVL_LIBPATH) $(PARX_OVL_PROG_LIBPATH) $(PARX_OVL_LIBS) $(PARX_OVL_LIBRARIES) $(PARX_OVL_PROG_LIBRARIES) \
	  -o $(OVERLAY).program
	$(RM) $(OVERLAY).program
	TMPDIR=$(join link_,$(OVERLAY)) $(PARX_OVL_LD) -shared $(WARN_LINK_AS_ERROR) -Wl,-soname,$(shell cat $(OVERLAY).id 2>/dev/null)$(PARX_OVL_EXT) $(CPLUSPLUSLDOPTIONS) \
	   @$(OVERLAY).objlist $(PARX_OVL_LIBPATH) $(PARX_OVL_LIBS) $(PARX_OVL_LIBRARIES) \
	  -o $(OVERLAY)$(PARX_OVL_EXT)
	$(RMD) $(join link_,$(OVERLAY))

clean::
	$(RM) $(OVERLAY)$(PARX_OVL_EXT) $(OVERLAY).program

$(foreach obj,$(OBJS),$(eval $(call CREATE_SIMPLE_DEPS,$(obj),.c,$(SRCDIR))))

$(OBJS): $(SRCDIR)/$(OVERLAY)$(PARX_OVL_DEF_EXT) $(OVERLAY)$(OBJEXT)
	$(info Compile $(SRCDIR)/$(@:$(OBJEXT)=.c))
	$(PARCOMP) $(SRCDIR)/$(OVERLAY)$(PARX_OVL_DEF_EXT) $(PARCOMP_FLAGS) -r $(SRCDIR)/${@:$(OBJEXT)=.c} &&\
	$(MV) $(DEPDIR)/$(@F:$(OBJEXT)=.Td) $(DEPDIR)/$(@F:$(OBJEXT)=.d) && touch $@
-include $(addprefix $(DEPDIR)/,$(addsuffix .d,$(basename $(notdir $(OBJS)))))

$(eval $(call CREATE_SIMPLE_DEPS,$(OVERLAY)$(OBJEXT),$(PARX_OVL_DEF_EXT),$(SRCDIR)))
$(OVERLAY)$(OBJEXT):	 | $(DEPDIR)
	$(info Compile $(SRCDIR)/$(OVERLAY)$(PARX_OVL_DEF_EXT))
	$(PARCOMP) $(SRCDIR)/$(OVERLAY)$(PARX_OVL_DEF_EXT)  $(PARCOMP_FLAGS) &&\
	$(MV) $(DEPDIR)/$(@F:$(OBJEXT)=.Td) $(DEPDIR)/$(@F:$(OBJEXT)=.d) && touch $@
-include $(DEPDIR)/$(OVERLAY).d

ifeq ($(SRCDIR)/$(OVERLAY).xml,$(wildcard $(SRCDIR)/$(OVERLAY).xml))
install:: $(METHODS_DIR)/$(OVERLAY)/$(OVERLAY).xml

$(eval $(call MKDIR_FCN,$(METHODS_DIR)/$(OVERLAY)))
$(METHODS_DIR)/$(OVERLAY)/$(OVERLAY).xml : $(SRCDIR)/$(OVERLAY).xml | $(METHODS_DIR)/$(OVERLAY)
	$(info Install $(METHODS_DIR)/$(OVERLAY)/$(OVERLAY).xml)
	$(RM) $(METHODS_DIR)/$(OVERLAY)/$(OVERLAY).xml
	$(CPASC) $(SRCDIR)/$(OVERLAY).xml $(METHODS_DIR)/$(OVERLAY)/$(OVERLAY).xml

$(eval $(call PARX_LIST_INSTALLED,.xml,$(METHODS_DIR)/$(OVERLAY)/$(OVERLAY).xml))
endif

ifeq ($(SRCDIR)/$(OVERLAY).html,$(wildcard $(SRCDIR)/$(OVERLAY).html))
install:: $(METHODS_DIR)/$(OVERLAY)/$(OVERLAY).html

$(eval $(call MKDIR_FCN,$(METHODS_DIR)/$(OVERLAY)))
$(METHODS_DIR)/$(OVERLAY)/$(OVERLAY).html : $(SRCDIR)/$(OVERLAY).html | $(METHODS_DIR)/$(OVERLAY)
	$(info Install $(METHODS_DIR)/$(OVERLAY)/$(OVERLAY).html)
	$(RM) $(METHODS_DIR)/$(OVERLAY)/$(OVERLAY).html
	$(CPASC) $(SRCDIR)/$(OVERLAY).html $(METHODS_DIR)/$(OVERLAY)/$(OVERLAY).html

$(eval $(call PARX_LIST_INSTALLED,.html,$(METHODS_DIR)/$(OVERLAY)/$(OVERLAY).html))
endif

clean::
	$(RM) $(OVERLAY).id

$(eval $(call MKDIR_FCN,$(METHODS_DIR)/$(OVERLAY)))
$(METHODS_DIR)/$(OVERLAY)/$(OVERLAY).id : $(OVERLAY).id $(OVERLAY)$(PARX_OVL_EXT) | $(METHODS_DIR)/$(OVERLAY)
	cd $(METHODS_DIR)/$(OVERLAY) && $(RM) *$(PARX_OVL_EXT)
	$(info Install $@ )
	$(CP) $< $@ && $(TOUCH) $@
	$(info Install $(OVERLAY)$(PARX_OVL_EXT) as $(METHODS_DIR)/$(OVERLAY)/$(shell cat $(OVERLAY).id 2>/dev/null)$(PARX_OVL_EXT))
	$(CP) $(OVERLAY)$(PARX_OVL_EXT) $(METHODS_DIR)/$(OVERLAY)/$(shell cat $(OVERLAY).id 2>/dev/null)$(PARX_OVL_EXT) || $(TOUCH) $<

install:: $(METHODS_DIR)/$(OVERLAY)/$(OVERLAY).id

$(eval $(call PARX_LIST_INSTALLED,id,$(METHODS_DIR)/$(OVERLAY)/$(OVERLAY).id))
ifneq ($(OVERLAY).id,)

$(eval $(call PARX_LIST_INSTALLED,$(PARX_OVL_EXT),$(METHODS_DIR)/$(OVERLAY)/$(shell cat $(OVERLAY).id 2>/dev/null)$(PARX_OVL_EXT)))
endif
clean:: clean_$(OVERLAY).dir
instclean: clean_$(OVERLAY).dir

.PHONY : clean_$(OVERLAY).dir
clean_$(OVERLAY).dir :
	$(RMD) $(METHODS_DIR)/$(OVERLAY)

emptyrule::

test::

cproto::

clean::
	$(RM_CMD)

depend::

Makefiles::

all::

sca::

install::

dll::

idl::

docu::

cproto::

