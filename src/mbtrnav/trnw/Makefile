########################################
# libtrnav/trnw Makefile
# Build Environment
# version and build configuration

# uncomment and optionally define on command line
# to override value defined in source, e.g.
#   trn_ver=1.0.1 make
trn_ver ?=0.0.1
build_date=`date +%Y/%m/%dT%H:%M:%S%z`
OPT_TRN_VER=-DLIBTRN_VERSION=$(trn_ver)
OPT_TRN_BUILD=-DLIBTRN_BUILD=$(build_date)
mb1rs_ver ?=0.0.0b1
OPT_MB1RS_VER=-DMB1RS_VER=$(mb1rs_ver)
OPT_MB1RS_BUILD=-DMB1RS_BUILD=$(build_date)

LIBTRNAV_TOP = ..
ifndef COMMON_DEFS
include ${LIBTRNAV_TOP}/Makefile.common
endif

# build directories
OUTPUT_DIR=${LIBTRNAV_TOP}/bin
BUILD_DIR=${LIBTRNAV_TOP}/build
QNX_DIR=${LIBTRNAV_TOP}/qnx-utils
NEWMAT_DIR=${LIBTRNAV_TOP}/newmat
TNAV_DIR=${LIBTRNAV_TOP}/terrain-nav

# C standard e.g. -std=c99 -std=gnu99
# may be needed for Cygwin (e.g. for loop declare/init)
#STD= -std=c99

# for TRN clients or to force
# netif default MMDEBUG config
# define NETIF_MMDEBUG
# compile using make trnc...
# or make NETIF_MMDEBUG=1
ifeq ($(MAKECMDGOALS),trnc)
OPT_NETIF_MMDEBUG=-DNETIF_MMDEBUG
$(info "INFO +++ using NETIF_MMDEBUG")
else ifdef NETIF_MMDEBUG
OPT_NETIF_MMDEBUG=-DNETIF_MMDEBUG
$(info "INFO +++ using NETIF_MMDEBUG")
endif

# Build options
BUILD_RELEASE_OPTIONS = $(STD) -D_GNU_SOURCE  $(OPT_TRN_VER) $(OPT_TRN_BUILD) $(OPT_NETIF_MMDEBUG) $(OPT_MB1RS_VER) $(OPT_MB1RS_BUILD) $(OPT_USE_PROJ)
BUILD_DEBUG_OPTIONS = $(BUILD_RELEASE_OPTIONS) -DWITH_PDEBUG

BUILD_OPTIONS = $(BUILD_RELEASE_OPTIONS)

# build debug version (else build release)
# [use make BUILD_DEBUG=1...]
ifdef BUILD_DEBUG
BUILD_OPTIONS = $(BUILD_DEBUG_OPTIONS)
else
BUILD_OPTIONS = $(BUILD_RELEASE_OPTIONS)
endif

# mframe is required to build TRN clients
# MFRAME_INC_DIR: location of mframe headers
# MFRAME_LIB_DIR: location of mframe library (libmframe.a)
# configure here, set environment or invoke make using
# make trnc MFRAME_INC_DIR=<path> MFRAME_LIB_DIR=<path>

ifndef MFRAME_INC_DIR
MFRAME_INC_DIR=../../mframe/src
endif

ifndef MFRAME_LIB_DIR
MFRAME_LIB_DIR=../../mframe/bin
endif

MFRAME_LIB=# $(MFRAME_LIB_DIR)/libmframe.a

# Compilation Options
CXX = gcc
CPP = g++ -std=c++11
# compile and link with -pg for profiling support
# then do gprof <application> gmon.out for profile output to console
#GPROF= -pg
# use -Wall unless you know what you're doing
WARN_FLAGS=-Wall

# Compiler flags
CFLAGS = -g -O2 $(WARN_FLAGS) $(GPROF) $(BUILD_OPTIONS) $(OPT_OS_CFLAGS)
INC_PATHS = -I. -I$(MFRAME_INC_DIR) -I$(TNAV_DIR) -I$(NEWMAT_DIR) -I$(QNX_DIR) $(OPT_PROJ_INC) $(OPT_OS_INC)

# Linker flags
LD_FLAGS = -g $(GPROF) $(OPT_OS_LDFLAGS)
LIB_PATHS = -L$(OUTPUT_DIR) -L$(MFRAME_LIB_DIR) $(OPT_PROJ_LIB) -L/usr/local/lib  $(OPT_OS_LIB)

ifdef buildDebug
$(info "INFO +++ trnw - OPT_USE_PROJ ${OPT_USE_PROJ}")
$(info "INFO +++ trnw - PROJ_LIB ${PROJ_LIB}")
$(info "INFO +++ trnw - OPT_PROJ_INC ${OPT_PROJ_INC}")
$(info "INFO +++ trnw - OPT_PROJ_LIB ${OPT_PROJ_LIB}")
$(info "INFO +++ trnw - OS_PREFIX          ${OS_PREFIX}")
$(info "INFO +++ trnw - OPT_OS_CFLAGS      ${OPT_OS_CFLAGS}")
$(info "INFO +++ trnw - OPT_OS_LDFLAGS     ${OPT_OS_LDFLAGS}")
$(info "INFO +++ trnw - OPT_OS_INC         ${OPT_OS_INC}")
$(info "INFO +++ trnw - OPT_OS_LIB         ${OPT_OS_LIB}")
$(info "INFO +++ trnw - OPT_OS_LIBS        ${OPT_OS_LIBS}")
$(info "INFO +++       - CFLAGS             ${CFLAGS}")
$(info "INFO +++       - INC_PATHS          ${INC_PATHS}")
$(info "INFO +++       - LD_FLAGS           ${LD_FLAGS}")
$(info "INFO +++       - LIB_PATHS          ${LIB_PATHS}")
endif

########################################
# Target Definitions

# libgeocon: Geoconversion wrapper for NavUtils/GCTP, PROJ
LIBGEOCON=libgeocon.a
LIBGEOCON_SRC = GeoCon.cpp
LIBGEOCON_OBJ=$(LIBGEOCON_SRC:%.cpp=$(BUILD_DIR)/%.o)
LIBGEOCON_LIBS = -lqnx -lgeolib  $(PROJ_LIB)

# libtrnw: TRN C wrappers
LIBTRNW=libtrnw.a
LIBTRNW_SRC=trnw.cpp
LIBTRNW_OBJ=$(LIBTRNW_SRC:%.cpp=$(BUILD_DIR)/%.o)
LIBTRNW_LIBS = -ltrn -lqnx -lgeolib -lnewmat $(PROJ_LIB) -lpthread

LIBMB1=libmb1.a
LIBMB1_SRC = mb1_msg.c
LIBMB1_OBJ=$(LIBMB1_SRC:%.c=$(BUILD_DIR)/%.o)
LIBMB1_LIBS =

TNAV_OBJ=$(BUILD_DIR)/TerrainNav.o \
$(BUILD_DIR)/PositionLog.o \
$(BUILD_DIR)/TerrainNavLog.o \
$(BUILD_DIR)/TNavConfig.o \
$(BUILD_DIR)/TNavFilter.o \
$(BUILD_DIR)/TNavPointMassFilter.o \
$(BUILD_DIR)/TNavParticleFilter.o \
$(BUILD_DIR)/TNavBankFilter.o \
$(BUILD_DIR)/TNavPFLog.o \
$(BUILD_DIR)/TerrainMapOctree.o \
$(BUILD_DIR)/PositionLog.o \
$(BUILD_DIR)/TerrainNavLog.o \
$(BUILD_DIR)/mapio.o \
$(BUILD_DIR)/structDefs.o \
$(BUILD_DIR)/trn_log.o \
$(BUILD_DIR)/myOutput.o \
$(BUILD_DIR)/OctreeSupport.o \
$(BUILD_DIR)/Octree.o \
$(BUILD_DIR)/OctreeNode.o \
$(BUILD_DIR)/TerrainMapDEM.o \
$(BUILD_DIR)/TRNUtils.o \
$(BUILD_DIR)/matrixArrayCalcs.o

# libtrncli: TRN client lib
LIBTRNCLI=libtrncli.a
LIBTRNCLI_SRC=trn_cli.c
LIBTRNCLI_OBJ=$(LIBTRNCLI_SRC:%.c=$(BUILD_DIR)/%.o)
LIBTRNCLI_LIBS = -lgeocon -lqnx -lgeolib -lmframe -lpthread -lm

# trncli-test: trn_cli unit test
TRNCLI_TEST=trncli-test
TRNCLI_TEST_SRC=trncli_test.c mb1_msg.c
TRNCLI_TEST_OBJ=$(TRNCLI_TEST_SRC:%.c=$(BUILD_DIR)/%.o)
TRNCLI_TEST_LIBS = -lmframe -ltrnw  -ltrn -ltrncli -lgeocon -lqnx -lgeolib  -lnewmat -lnetcdf  $(PROJ_LIB) -lpthread -lm  -lstdc++

# libtrnucli: TRNU client lib
LIBTRNUCLI=libtrnucli.a
LIBTRNUCLI_SRC=trnu_cli.c
LIBTRNUCLI_OBJ=$(LIBTRNUCLI_SRC:%.c=$(BUILD_DIR)/%.o)
LIBTRNUCLI_LIBS = -lstdc++ -lmframe -lpthread -lm

# libnetif: TRN network interface lib
LIBNETIF=libnetif.a
LIBNETIF_SRC=netif.c trnif_proto.c trnif_msg.c
LIBNETIF_OBJ=$(LIBNETIF_SRC:%.c=$(BUILD_DIR)/%.o)
LIBNETIF_LIBS =

# trnucli-test: trnu_cli unit test
TRNUCLI_TEST=trnucli-test
TRNUCLI_TEST_SRC=trnucli_test.c 
TRNUCLI_TEST_OBJ=$(TRNUCLI_TEST_SRC:%.c=$(BUILD_DIR)/%.o)
TRNUCLI_TEST_LIBS = -lnetif -ltrnucli -lmframe -lpthread -lm -lqnx -lnewmat -ltrn -ltrncli -lnetcdf -lgeolib $(PROJ_LIB) -lstdc++

# trnusvr-test: trnu_cli test server
TRNUSVR_TEST=trnusvr-test
TRNUSVR_TEST_SRC=trnusvr_test.c netif.c trnif_msg.c trnif_proto.c
TRNUSVR_TEST_OBJ=$(TRNUSVR_TEST_SRC:%.c=$(BUILD_DIR)/%.o)
TRNUSVR_TEST_LIBS = -lmframe -ltrnw -lpthread -lm -lqnx -lnewmat -ltrn -ltrncli -lnetcdf -lgeolib $(PROJ_LIB) -lstdc++

# mcpub: multicast test publisher
MCPUB=mcpub
MCPUB_SRC=mcpub.c
MCPUB_OBJ=$(MCPUB_SRC:%.c=$(BUILD_DIR)/%.o)
MCPUB_LIBS =

# mmcsub: multicast test publisher
MCSUB=mcsub
MCSUB_SRC=mcsub.c
MCSUB_OBJ=$(MCSUB_SRC:%.c=$(BUILD_DIR)/%.o)
MCSUB_LIBS =

# mmcpub: multicast test publisher (uses mframe)
MMCPUB=mmcpub
MMCPUB_SRC=mmcpub.c
MMCPUB_OBJ=$(MMCPUB_SRC:%.c=$(BUILD_DIR)/%.o)
MMCPUB_LIBS = $(MFRAME_LIB) -lmframe -lm -lstdc++

# mmcsub: multicast test publisher  (uses mframe)
MMCSUB=mmcsub
MMCSUB_SRC=mmcsub.c
MMCSUB_OBJ=$(MMCSUB_SRC:%.c=$(BUILD_DIR)/%.o)
MMCSUB_LIBS = $(MFRAME_LIB) -lmframe -lm -lstdc++

TRNIF_TEST=trnif-test
TRNIF_TEST_SRC=trnif_test.c trnif_proto.c trnif_msg.c
TRNIF_TEST_OBJ = $(TRNIF_TEST_SRC:%.c=$(BUILD_DIR)/%.o)
TRNIF_TEST_LIBS = -lnetif -lmframe -ltrnw -lpthread -lm  -lnewmat -ltrn -lnetcdf -lgeocon -lqnx -lgeolib  $(PROJ_LIB) -lstdc++

MB1RS=mb1rs
MB1RS_SRC=mb1_msg.c mb1rs.c mb1rs-app.c
MB1RS_OBJ=$(MB1RS_SRC:%.c=$(BUILD_DIR)/%.o)
MB1RS_LIBS = $(MFRAME_LIB) -lmframe -lm -lpthread -lstdc++

GEOCON_TEST=geocon-test
GEOCON_TEST_SRC=geocon_test.cpp
GEOCON_TEST_OBJ=$(GEOCON_TEST_SRC:%.cpp=$(BUILD_DIR)/%.o)
GEOCON_TEST_LIBS = -lgeocon -lqnx -lgeolib $(PROJ_LIB) -lstdc++

# TRN clients
TRNC_SRC+= $(LIBTRNCLI_SRC) \
$(TRNCLI_TEST_SRC) \
$(LIBTRNUCLI_SRC) \
$(TRNUCLI_TEST_SRC) \
$(TRNUSVR_TEST_SRC) \
$(MMCPUB_SRC) \
$(MMCSUB_SRC) \
$(MCPUB_SRC) \
$(MCSUB_SRC) \
$(TRNCLI_TEST_SRC) \
$(TRNIF_TEST_SRC) \
$(MB1RS_SRC)

TRNC_TARGETS+=  $(OUTPUT_DIR)/$(LIBTRNCLI) \
$(OUTPUT_DIR)/$(LIBTRNUCLI) \
$(OUTPUT_DIR)/$(LIBGEOCON) \
$(OUTPUT_DIR)/$(TRNCLI_TEST) \
$(OUTPUT_DIR)/$(TRNUCLI_TEST)\
$(OUTPUT_DIR)/$(TRNUSVR_TEST) \
$(OUTPUT_DIR)/$(MMCPUB) \
$(OUTPUT_DIR)/$(MMCSUB) \
$(OUTPUT_DIR)/$(MCPUB) \
$(OUTPUT_DIR)/$(MCSUB) \
$(OUTPUT_DIR)/$(TRNIF_TEST) \
$(OUTPUT_DIR)/$(MB1RS)

########################################
# Build Files (mostly for cleanup)
TRNC_SOURCES =  $(TRNC_SRC)
TRNW_SOURCES =  $(LIBTRNW_SRC)
TRNC_OBJECTS = $(TRNC_SOURCES:%.c=$(BUILD_DIR)/%.o)
TRNW_OBJECTS = $(TRNW_SOURCES:%.cpp=$(BUILD_DIR)/%.o)
TRNC_DEPENDS = $(TRNC_SOURCES:%.c=$(BUILD_DIR)/%.d)
TRNW_DEPENDS = $(TRNW_SOURCES:%.cpp=$(BUILD_DIR)/%.d)
CLEAN_OBJECTS = $(TRNC_OBJECTS) $(TRNW_OBJECTS) $(MCSUB_OBJ) $(MMCSUB_OBJ) $(LIBNETIF_OBJ) \
$(LIBMB1_OBJ) $(GEOCON_TEST_OBJ) $(LIBGEOCON_OBJ)
CLEAN_DEPENDS = $(TRNC_DEPENDS) $(TRNW_DEPENDS)
BINARIES =  $(OUTPUT_DIR)/$(LIBTRNW) $(OUTPUT_DIR)/$(TRNC_TARGETS) $(OUTPUT_DIR)/$(LIBNETIF) \
$(OUTPUT_DIR)/$(LIBMB1) $(OUTPUT_DIR)/$(GEOCON_TEST)

CLEANUP = gmon.out
# dSYMs : XCode debug symbol file folders
#DSYMS = $(BINARIES:%=%.dSYM)
#RM_DSYMS = rm -rf $(DSYMS)

########################################
# Rules: build targets
all: $(OUTPUT_DIR)/$(LIBTRNW) $(OUTPUT_DIR)/$(LIBMB1) $(OUTPUT_DIR)/$(LIBGEOCON) $(OUTPUT_DIR)/$(GEOCON_TEST)


trnc: $(OUTPUT_DIR)/$(LIBTRNW) $(OUTPUT_DIR)/$(LIBNETIF) $(TRNC_TARGETS)
	@echo building $@...

# build Mb1 library
$(OUTPUT_DIR)/$(LIBMB1): $(LIBMB1_OBJ)
	@echo building $@...
	ar -r $@ $(LIBMB1_OBJ)

# build netif library
$(OUTPUT_DIR)/$(LIBNETIF): $(LIBNETIF_OBJ) 
	@echo building $@...
	ar rcs $@ $(LIBNETIF_OBJ)

# build trn wrapper library
$(OUTPUT_DIR)/$(LIBTRNW): $(LIBTRNW_OBJ)
	@echo building $@...
	ar rcs $@  $(LIBTRNW_OBJ) $(TNAV_OBJ)

# build trn client library
$(OUTPUT_DIR)/$(LIBTRNCLI): $(LIBTRNCLI_OBJ)
	@echo building $@...
	ar rcs $@ $(LIBTRNCLI_OBJ) $(BUILD_DIR)/*.o

# build GeoConverter library
#	--record-libdeps="$(LIB_PATHS) $(LIBGEOCON_LIBS)"
$(OUTPUT_DIR)/$(LIBGEOCON): $(LIBGEOCON_OBJ)
	@echo building $@...
	ar rcs $@ $(LIBGEOCON_OBJ)

# build trncli-test utility
$(OUTPUT_DIR)/$(TRNCLI_TEST): $(TRNCLI_TEST_OBJ) $(LIBTRNW_OBJ) $(LIBTRNCLI_OBJ)
	@echo building $@...
	$(CXX) $(INC_PATHS) $(LIB_PATHS) $^ -o $@ $(LD_FLAGS) $(TRNCLI_TEST_LIBS)
	@echo

# build trnu client library
$(OUTPUT_DIR)/$(LIBTRNUCLI): $(LIBTRNUCLI_OBJ)
	@echo building $@...
	ar rcs $@  $(LIBTRNUCLI_OBJ)  $(BUILD_DIR)/*.o

# build trnucli-test utility
$(OUTPUT_DIR)/$(TRNUCLI_TEST): $(TRNUCLI_TEST_OBJ) $(LIBTRNUCLI_OBJ)
	@echo building $@...
	$(CXX) $(INC_PATHS) $(LIB_PATHS) $^ -o $@ $(LD_FLAGS) $(TRNUCLI_TEST_LIBS)
	@echo

# build trnusvr-test utility
$(OUTPUT_DIR)/$(TRNUSVR_TEST): $(TRNUSVR_TEST_OBJ)
	@echo building $@...
	$(CXX) $(INC_PATHS) $(LIB_PATHS) $^ -o $@ $(LD_FLAGS) $(TRNUSVR_TEST_LIBS)
	@echo

# build mcpub utility
$(OUTPUT_DIR)/$(MCPUB): $(MCPUB_OBJ)
	@echo building $@...
	$(CXX) $(INC_PATHS) $(LIB_PATHS) $^ -o $@ $(LD_FLAGS) $(MCPUB_LIBS)
	@echo

# build mcsub utility
$(OUTPUT_DIR)/$(MCSUB): $(MCSUB_OBJ)
	@echo building $@...
	$(CXX) $(INC_PATHS) $(LIB_PATHS) $^ -o $@ $(LD_FLAGS) $(MCSUB_LIBS)
	@echo

# build mmcpub utility
$(OUTPUT_DIR)/$(MMCPUB): $(MMCPUB_OBJ)
	@echo building $@...
	$(CXX) $(INC_PATHS) $(LIB_PATHS) $^ -o $@ $(LD_FLAGS) $(MMCPUB_LIBS)
	@echo

# build mmcsub utility
$(OUTPUT_DIR)/$(MMCSUB): $(MMCSUB_OBJ)
	@echo building $@...
	$(CXX) $(INC_PATHS) $(LIB_PATHS) $^ -o $@ $(LD_FLAGS) $(MMCSUB_LIBS)
	@echo

# build trnif-test utility
$(OUTPUT_DIR)/$(TRNIF_TEST): $(TRNIF_TEST_OBJ)
	@echo building $@...
	$(CXX) $(INC_PATHS) $(LIB_PATHS) $^ -o $@ $(LD_FLAGS) $(TRNIF_TEST_LIBS)
	@echo

# build mb1rs MB1 server
$(OUTPUT_DIR)/$(MB1RS): $(MB1RS_OBJ)
	@echo building $@...
	$(CXX) $(INC_PATHS) $(LIB_PATHS) $^ -o $@ $(LD_FLAGS) $(MB1RS_LIBS)
	@echo

# build geocon-test utility
$(OUTPUT_DIR)/$(GEOCON_TEST): $(GEOCON_TEST_OBJ)
	@echo building $@...
	$(CPP) $(INC_PATHS) $(LIB_PATHS)  $^ -o $@ $(LD_FLAGS) $(GEOCON_TEST_LIBS)
	@echo

# include the dependencies
ifeq ($(MAKECMDGOALS),trnc)
-include $(TRNC_DEPENDS)
endif
ifeq ($(MAKECMDGOALS),all)
-include $(TRNW_DEPENDS)
endif
ifneq ($(MAKECMDGOALS),trnc)
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),purge)
-include $(TRNW_DEPENDS)
endif
endif
endif

# rule: build object files from source files
$(BUILD_DIR)/%.o :%.c 
	@echo compiling $<...
	$(CXX) $(CFLAGS) $(INC_PATHS) -c $< -o $@
	@echo

$(BUILD_DIR)/%.o :%.cpp
	@echo compiling $<...
	$(CPP) $(CFLAGS) $(INC_PATHS) -c $< -o $@
	@echo

# rule: build dependency files from source files
$(BUILD_DIR)/%.d :%.c
	@[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	@[ -d $(OUTPUT_DIR) ] || mkdir -p $(OUTPUT_DIR)
	@echo generating dependency file for $<
	@set -e; $(CXX) -MM $(CFLAGS) $(INC_PATHS) $< \
	| awk '/o:/ {printf "%s", "$@ $(BUILD_DIR)/"} {print}' > $@; \
	[ -s $@ ] || rm -f $@
	@echo

$(BUILD_DIR)/%.d :%.cpp
	@[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	@[ -d $(OUTPUT_DIR) ] || mkdir -p $(OUTPUT_DIR)
	@echo generating dependency file for $<
	@set -e; $(CXX) -MM $(CFLAGS) $(INC_PATHS) $< \
	| awk '/o:/ {printf "%s", "$@ $(BUILD_DIR)/"} {print}' > $@; \
	[ -s $@ ] || rm -f $@
	@echo


install:
	@echo "Installing...(not implemented)"


########################################
# Rules:

.PHONY: clean
.PHONY: purge
.PHONY: trnc
.PHONY: trnw

# clean : delete object, dependency, binary files
clean:
	rm -f $(CLEAN_OBJECTS) $(CLEAN_DEPENDS) $(BINARIES)
	$(RM_DSYMS)

# purge : delete delete object, dependency, binary files, build directories
purge:
	rm -f $(BINARIES) $(TRNW_OBJECTS) $(TRNW_DEPENDS) (TRNC_OBJECTS) $(TRNC_DEPENDS) $(CLEANUP)
	rm -rf $(OUTPUT_DIR) $(BUILD_DIR) $(DSYMS)
