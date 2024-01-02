# -------------------------------------------------------------
# 
# Copyright (c) 2001-2020
# Bruker BioSpin MRI GmbH
# D-76275 Ettlingen, Germany
#
# All Rights Reserved
#
# -------------------------------------------------------------


print-% : ; $(info $* = $($*)) @true

DEFAULT_SYMBOL_VISIBILITY = -fvisibility=hidden

PROGLIBLDFLAGS = $(CC_EXEC_PREFIX_OPTION) $(PT_GNU_RELRO) $(PV_COMPILE_FLAGS) $(COMMON_FLAG) -fno-gnu-unique -fexceptions $(LINK_OPTIONS)

PLUGIN_MAIN_FILE = $(PROGDIR)/include/Isa/_PluginMain

RABBITMQ_ROOT	= $(BUILD_TOOL_ROOT)/rabbitmq-c-0.10.0

LINKTIME_OPTIMIZE = -flto -ffat-lto-objects

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

PLUGINDIR = $(PROGDIR)/plugins

RMD		= rm -rf

SHARELIBBASEOPTS = -shared

PV_EXTRA_DEFINES =

PV_DEFINES = -D_REENTRANT -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE

PV_ARCH_FLAG = -m64

PV_COMPILE_FLAGS = $(DEFAULT_SYMBOL_VISIBILITY) $(PV_ARCH_FLAG) $(PIC_FLAG) -rdynamic

CREATE_LINK_LIST = $(PERL)  $(PROGDIR)/bin/createLinkList.pl

MAKEFILE_DEP = $(SRCDIR)/Makefile

DEPDIR = depdir

THREAD_DIR	 = $(OMNI_ROOT)
THREAD_DEFINES   =
THREAD_LIBPATH   =
THREAD_LIBRARIES = $(OMNITHREAD_LIBRARIES)
THREAD_INCPATH   = $(SIFLAG) $(OMNI_ROOT)/include
THREAD_COPYLIBS  = $(OMNI_ROOT)/lib/libomnithread.so.4

CORBA_DIR	= $(OMNI_ROOT)
CORBA_DEFINES   =
CORBA_LIBPATH   = -L$(SHLIBDIR) -L$(XWINSHLIBDIR)
CORBA_LIBRARIES = $(CORBA_LIBPATH) -lomniCodeSets4 -lomnisslTP4  -lomniORB4 -lomnithread $(SSL_LIBRARIES)
OMNI_INCDIR     = $(OMNI_ROOT)/include
CORBA_INCPATH   = $(SIFLAG) $(LOCALHEADERDIR)/pvidl $(SIFLAG) $(OMNI_INCDIR) $(SSL_INCPATH)
CORBA_COPYLIBS  = $(OMNI_ROOT)/lib/libomniORB4.so.2 $(OMNI_ROOT)/lib/libomnithread.so.4 $(OMNI_ROOT)/lib/libomniCodeSets4.so.2 $(OMNI_ROOT)/lib/libomnisslTP4.so.2

IDL			= $(OMNI_PYTHON) $(OMNI_ROOT)/bin/omniidl

IDLOPTIONS		= -bcxx -k -K -Wbs=$(PPSUFFIX) -Wbh=.idl.hh -I $(GLOBALROOT)/idl
IDLFLAGS		= $(IDLOPTIONS) $(EXTRA_IDL_INCPATH)
TS_CORBA_INCPATH = $(SIFLAG) $(LOCALHEADERDIR)/CorbaLib

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

PERL		= $(EXECDISKUNIT)/perl/bin/perl -I$(PROGDIR)/perl/lib/perl5 -I$(PROGDIR)/perl

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

BOOST_LIBRARYPATH = $(BOOST_ROOT)/lib
BOOST_INCPATH	= -isystem $(BOOST_ROOT)/include

NLOPT_LIBRARIES	= $(NLOPT_ROOT)/lib64/libnlopt.a
NLOPT_INCPATH	= -isystem $(NLOPT_ROOT)/include

SSL_LIBRARIES	= $(XWINSHLIBDIR)/libssl.so $(XWINSHLIBDIR)/libcrypto.so

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

ISA_STANDARD_LIBS = $(SHLIBDIR)/libParxClientBase$(SHARELIBEXT) \
$(SHLIBDIR)/libParxClientPv$(SHARELIBEXT) \
$(SHLIBDIR)/libParxUtils$(SHARELIBEXT) \
$(SHLIBDIR)/libCoreMisc$(SHARELIBEXT) \
$(SHLIBDIR)/libCoreCommon$(SHARELIBEXT) \
$(SHLIBDIR)/libCoreThread$(SHARELIBEXT) \
$(SHLIBDIR)/libCoreRemote$(SHARELIBEXT) \
$(SHLIBDIR)/libPluma$(SHARELIBEXT) \
$(SHLIBDIR)/libPvIdl$(SHARELIBEXT) \
$(SHLIBDIR)/libPvUtil$(SHARELIBEXT) \
$(SHLIBDIR)/libLogger$(SHARELIBEXT)
ISA_PLUGIN_TYPE = IsaModels
EXPORT_ISA_SCRIPT = $(GLOBALCONFIGDIR)/ExportIsaModel.pl

.PHONY: instclean instlist

$(VERBOSE).SILENT:

LIBS ?= $(ISA_STANDARD_LIBS)

DEFAULT_SYMBOL_VISIBILITY = -fvisibility=hidden

all::	$(PLUGIN)$(SHARELIBEXT)

install_SUBDIRS:
$(SUBDIR_OBJS) : install_SUBDIRS
$(PLUGIN).objlist :  $(OBJS) $(SUBDIR_OBJS)
	$(RM) $(PLUGIN).objlist
	$(TOUCH) $(PLUGIN).objlist
	$(CREATE_LINK_LIST) -o $(PLUGIN).objlist $(OBJS) $(SUBDIR_OBJS)

clean::
	$(RM) $(PLUGIN).objlist

install:: $(PLUGIN)$(SHARELIBEXT)

$(foreach obj,$(notdir $(PLUGIN_MAIN_FILE))$(OBJEXT),$(eval $(call CREATE_SIMPLE_DEPS,$(obj),$(PPSUFFIX), $(dir $(PLUGIN_MAIN_FILE)))))

$(notdir $(PLUGIN_MAIN_FILE))$(OBJEXT): | $(DEPDIR)
	$(info Compile  $(dir $(PLUGIN_MAIN_FILE))/$(@F:$(OBJEXT)=$(PPSUFFIX)))
	$(CPLUSPLUS) $(PLUSPLUSDEPENDFLAGS) $(CPLUSPLUSFLAGS) -c  $(dir $(PLUGIN_MAIN_FILE))/$(@F:$(OBJEXT)=$(PPSUFFIX)) &&\
	$(MV) $(DEPDIR)/$(@F:$(OBJEXT)=.Td) $(DEPDIR)/$(@F:$(OBJEXT)=.d) && touch $@
-include $(addprefix $(DEPDIR)/,$(addsuffix .d,$(basename $(notdir $(notdir $(PLUGIN_MAIN_FILE))$(OBJEXT)))))

$(PLUGIN)$(SHARELIBEXT): $(PLUGIN_CONNECTOR) $(MAKEFILE_DEP) $(notdir $(PLUGIN_MAIN_FILE))$(OBJEXT) $(LIBS) $(PLUGIN).objlist
	$(info Link $(PLUGIN)$(SHARELIBEXT))
	$(RM) $(PLUGIN)$(SHARELIBEXT)
	$(RM) $(PLUGIN).program
	$(MKDIR) $(join link_,$(PLUGIN))
	TMPDIR=$(join link_,$(PLUGIN)) $(CPLUSPLUS) $(PROGLIBLDFLAGS) $(PLUGIN_CONNECTOR) $(notdir $(PLUGIN_MAIN_FILE))$(OBJEXT) @$(PLUGIN).objlist \
	  $(LIBPATH)  $(LIBS) $(PLUGIN_LIBRARIES) $(ALL_PLUSPLUS_LIBRARIES) \
	  -o $(PLUGIN).program
	$(RM) $(PLUGIN).program
	TMPDIR=$(join link_,$(PLUGIN)) $(CPLUSPLUS) -shared $(WARN_LINK_AS_ERROR) -Wl,-soname,$(PLUGIN)$(SHARELIBEXT) $(CPLUSPLUSLDOPTIONS) \
	  $(PLUGIN_CONNECTOR) @$(PLUGIN).objlist $(LIBPATH) $(LIBS) $(PLUGIN_LIBRARIES) \
	  -o $(PLUGIN)$(SHARELIBEXT)
	$(RMD) $(join link_,$(PLUGIN))

clean::
	$(RM) $(PLUGIN)$(SHARELIBEXT) $(PLUGIN).program

$(foreach obj,$(OBJS),$(eval $(call CREATE_SIMPLE_DEPS,$(obj),$(PPSUFFIX), $(SRCDIR))))

$(OBJS): | $(DEPDIR)
	$(info Compile  $(SRCDIR)/$(@F:$(OBJEXT)=$(PPSUFFIX)))
	$(CPLUSPLUS) $(PLUSPLUSDEPENDFLAGS) $(CPLUSPLUSFLAGS) -c  $(SRCDIR)/$(@F:$(OBJEXT)=$(PPSUFFIX)) &&\
	$(MV) $(DEPDIR)/$(@F:$(OBJEXT)=.Td) $(DEPDIR)/$(@F:$(OBJEXT)=.d) && touch $@
-include $(addprefix $(DEPDIR)/,$(addsuffix .d,$(basename $(notdir $(OBJS)))))

ifneq ($(PLUGIN_CONNECTOR),)

$(foreach obj,$(PLUGIN_CONNECTOR),$(eval $(call CREATE_SIMPLE_DEPS,$(obj),$(PPSUFFIX), $(SRCDIR))))

$(PLUGIN_CONNECTOR): | $(DEPDIR)
	$(info Compile  $(SRCDIR)/$(@F:$(OBJEXT)=$(PPSUFFIX)))
	$(CPLUSPLUS) $(PLUSPLUSDEPENDFLAGS) $(CPLUSPLUSFLAGS) -c  $(SRCDIR)/$(@F:$(OBJEXT)=$(PPSUFFIX)) &&\
	$(MV) $(DEPDIR)/$(@F:$(OBJEXT)=.Td) $(DEPDIR)/$(@F:$(OBJEXT)=.d) && touch $@
-include $(addprefix $(DEPDIR)/,$(addsuffix .d,$(basename $(notdir $(PLUGIN_CONNECTOR)))))

endif

$(eval $(call COPY_TARGET,$(PLUGIN)$(SHARELIBEXT),$(PLUGINDIR)/$(ISA_PLUGIN_TYPE)/$(PLUGIN)$(SHARELIBEXT)))
install:: $(PLUGINDIR)/$(ISA_PLUGIN_TYPE)/$(PLUGIN)$(SHARELIBEXT)

instclean: instclean$(SHARELIBEXT)

.PHONY : instclean$(SHARELIBEXT)
instclean$(SHARELIBEXT):
	$(RM) $(PLUGINDIR)/$(ISA_PLUGIN_TYPE)/$(PLUGIN)$(SHARELIBEXT)

instlist: instlist$(SHARELIBEXT)

.PHONY : instlist$(SHARELIBEXT)
instlist$(SHARELIBEXT):
	@echo $(PLUGINDIR)/$(ISA_PLUGIN_TYPE)/$(PLUGIN)$(SHARELIBEXT);

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

