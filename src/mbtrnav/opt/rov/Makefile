########################################
# opt/rov Makefile
# Build Environment
# version and build configuration

# uncomment and optionally define on command line
# to override value defined in source, e.g.
#   trn_ver=1.0.1 make
build_date=`date +%Y/%m/%dT%H:%M:%S%z`
trnxpp_ver?="0.0.1"
OPT_APP_BUILD=-DAPP_BUILD=$(build_date)
OPT_TRNXPP_VER=-DTRNXPP_VER=$(trnxpp_ver)

LIBTRNAV_TOP = ../..
ifndef COMMON_DEFS
include ${LIBTRNAV_TOP}/Makefile.common
endif

# Build directories
OUTPUT_DIR=${LIBTRNAV_TOP}/bin
BUILD_DIR=${LIBTRNAV_TOP}/build
QNX_DIR=${LIBTRNAV_TOP}/qnx-utils
NEWMAT_DIR=${LIBTRNAV_TOP}/newmat
TRN_DIR=${LIBTRNAV_TOP}/terrain-nav
UTILS_DIR=${LIBTRNAV_TOP}/utils
TRNW_DIR=${LIBTRNAV_TOP}/trnw
LCM_JAR=/usr/local/share/java/lcm.jar
LCM_TYPES=lcm_types


# C standard e.g. -std=c99 -std=gnu99
# may be needed for Cygwin (e.g. for loop declare/init)
CXX_STD= -std=c++11
C_STD= -std=c99

# Build options
BUILD_OPTIONS = $(STD) -D_GNU_SOURCE $(OPT_APP_BUILD)  $(OPT_TRNXPP_VER) ${OPT_USE_PROJ}

# Compilation Options
CC = gcc
CXX = g++
CPP = g++ -E
AR = ar

# compile and link with -pg for profiling support
# then do gprof <application> gmon.out for profile output to console
#GPROF= -pg
# use -Wall unless you know what you're doing
WARN_FLAGS=-Wall

SANI_FLAG =
#SANI_FLAG += -fsanitize=address

# Compiler flags
CFLAGS = -g -O2 $(C_STD) $(SANI_FLAG) $(WARN_FLAGS) $(GPROF) $(BUILD_OPTIONS) $(OPT_OS_CFLAGS)
CXXFLAGS = -g -O2 $(CXX_STD) $(SANI_FLAG) $(WARN_FLAGS) $(GPROF) $(BUILD_OPTIONS) $(OPT_OS_CFLAGS)
CPPFLAGS += #-MM -MMD
INC_PATHS =  -I.  -I$(NEWMAT_DIR) -I$(QNX_DIR) \
-I$(TRN_DIR) -I$(UTILS_DIR) -I$(TRNW_DIR) $(OPT_PROJ_INC) $(OPT_OS_INC)

# Linker flags
LD_FLAGS = -g $(GPROF) $(OPT_OS_LDFLAGS)
LIB_PATHS = -L$(OUTPUT_DIR) -L/usr/local/lib $(OPT_PROJ_LIB) $(OPT_OS_LIB)

ifdef buildDebug
$(info "INFO +++ opt/rov - OPT_USE_PROJ ${OPT_USE_PROJ}")
$(info "INFO +++ opt/rov - PROJ_LIB     ${PROJ_LIB}")
$(info "INFO +++ opt/rov - OPT_PROJ_INC ${OPT_PROJ_INC}")
$(info "INFO +++ opt/rov - OPT_PROJ_LIB ${OPT_PROJ_LIB}")
$(info "INFO +++ opt/rov - OS_PREFIX          ${OS_PREFIX}")
$(info "INFO +++ opt/rov - OPT_OS_CFLAGS      ${OPT_OS_CFLAGS}")
$(info "INFO +++ opt/rov - OPT_OS_LDFLAGS     ${OPT_OS_LDFLAGS}")
$(info "INFO +++ opt/rov - OPT_OS_INC         ${OPT_OS_INC}")
$(info "INFO +++ opt/rov - OPT_OS_LIB         ${OPT_OS_LIB}")
$(info "INFO +++ opt/rov - OPT_OS_LIBS        ${OPT_OS_LIBS}")
$(info "INFO +++         - CXXFLAGS           ${CXXFLAGS}")
$(info "INFO +++         - CPPFLAGS           ${CPPFLAGS}")
$(info "INFO +++         - CFLAGS             ${CFLAGS}")
$(info "INFO +++         - INC_PATHS          ${INC_PATHS}")
$(info "INFO +++         - LD_FLAGS           ${LD_FLAGS}")
$(info "INFO +++         - LIB_PATHS          ${LIB_PATHS}")
endif

########################################
# Target Definitions

TRNLCM=trnlcm-test
TRNLCM_SRC= trnlcm_test.cpp
TRNLCM_OBJ = $(TRNLCM_SRC:%.cpp=$(BUILD_DIR)/%.o)
TRNLCM_LIBS = -llcm -lstdc++ -lpthread $(OPT_OS_LIBS)

PCF=lcm_pcf
PCF_LCM=$(LCM_TYPES)/signal_t.lcm $(LCM_TYPES)/string_t.lcm
PCF_HPP=$(PCF)/signal_t.hpp $(PCF)/string_t.hpp
PCF_JAVA=$(PCF_HPP:%.hpp=%.java)
PCF_CLASS=$(PCF_HPP:%.hpp=%.class)

GSS=gss
GSS_LCM=$(LCM_TYPES)/nav_solution_t.lcm \
$(LCM_TYPES)/message_t.lcm \
$(LCM_TYPES)/analog_t.lcm \
$(LCM_TYPES)/digital_t.lcm \
$(LCM_TYPES)/dvl_stat_t.lcm \
$(LCM_TYPES)/pcomms_t.lcm

GSS_HPP=$(GSS)/nav_solution_t.hpp \
$(GSS)/message_t.hpp \
$(GSS)/analog_t.hpp \
$(GSS)/digital_t.hpp \
$(GSS)/dvl_stat_t.hpp \
$(GSS)/pcomms_t.hpp \
$(GSS)/parosci_stat_t.hpp
GSS_JAVA=$(GSS_HPP:%.hpp=%.java)
GSS_CLASS=$(GSS_HPP:%.hpp=%.class)

OI=oi
OI_LCM=$(LCM_TYPES)/idt_t.lcm \
$(LCM_TYPES)/lcm_header_t.lcm
OI_HPP=$(OI)/idt_t.hpp \
$(OI)/double_t.hpp \
$(OI)/rdi_pd4_t.hpp \
$(OI)/octans_t.hpp \
$(OI)/kearfott_t.hpp \
$(OI)/ctd_t.hpp \
$(OI)/lcm_header_t.hpp
OI_JAVA=$(OI_HPP:%.hpp=%.java)
OI_CLASS=$(OI_HPP:%.hpp=%.class)

TRN=trn
TRN_LCM=$(LCM_TYPES)/mb1_beam_t.lcm \
$(LCM_TYPES)/trnu_estimate_t.lcm \
$(LCM_TYPES)/trn_stat_t.lcm \
$(LCM_TYPES)/trn_meas_t.lcm \
$(LCM_TYPES)/trn_pose_t.lcm \
$(LCM_TYPES)/trnupub_t.lcm \
$(LCM_TYPES)/trn_mb1_t.lcm \
$(LCM_TYPES)/trn_stat_t.lcm
TRN_HPP=$(TRN)/mb1_beam_t.hpp \
$(TRN)/trnu_estimate_t.hpp \
$(TRN)/trn_stat_t.hpp \
$(TRN)/trn_meas_t.hpp \
$(TRN)/trn_pose_t.hpp \
$(TRN)/trnupub_t.hpp \
$(TRN)/trn_mb1_t.hpp \
$(TRN)/trn_stat_t.hpp
TRN_JAVA=$(TRN_HPP:%.hpp=%.java)
TRN_CLASS=$(TRN_HPP:%.hpp=%.class)

ROVTRN_JAR = lcm_rovtrn.jar

TRNXPP=trnxpp
TRNXPP_SRC= trnxpp_app.cpp
TRNXPP_HPP= trnxpp.hpp
TRNXPP_OBJ = $(TRNXPP_SRC:%.cpp=$(BUILD_DIR)/%.o)
TRNXPP_LIBS = -ludpms   -ltrnxplug   -ltrncli -lmb1  -ltrn -lgeocon -lqnx -lgeolib  -lpthread  -lnewmat $(PROJ_LIB) -lnetcdf -lm -llcm -lstdc++  $(OPT_OS_LIBS)

FVTEST=fv-test
FVTEST_SRC = flag_var_test.cpp
FVTEST_HPP = $(UTILS_DIR)/flag_utils.hpp
FVTEST_OBJ = $(FVTEST_SRC:%.cpp=$(BUILD_DIR)/%.o)
FVTEST_LIBS = -lstdc++ $(OPT_OS_LIBS)

LIBUDPMS=libudpms.a
LIBUDPMS_SRC=udpm_sub.c
LIBUDPMS_OBJ=$(LIBUDPMS_SRC:%.c=$(BUILD_DIR)/%.o)
LIBUDPMS_LIBS=

LIBTRNXPLUG = libtrnxplug.a
LIBTRNXPLUG_SRC = plug-common.cpp plug-idtlass.cpp plug-oisledx.cpp plug-dvl.cpp plug-idt.cpp  plug-oisled.cpp plug-oisled2.cpp plug-lassmb.cpp plug-xmb1.cpp
LIBTRNXPLUG_OBJ = $(LIBTRNXPLUG_SRC:%.cpp=$(BUILD_DIR)/%.o)
LIBTRNXPLUG_LIBS = -ludpms -ltrncli -lmb1  -ltrn -ltrnw  -lgeocon  -lqnx  -lgeolib   -lnewmat -lnetcdf -llcm

########################################
# Build Files (mostly for cleanup)
SOURCES = $(TRNLCM_SRC) $(TRNXPP_SRC) $(FVTEST_SRC) $(LIBTRNXPLUG_SRC)
C_SOURCES = $(LIBUDPMS_SRC)

OBJECTS = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o) \
 $(C_SOURCES:%.c=$(BUILD_DIR)/%.o)

DEPENDS = $(SOURCES:%.cpp=$(BUILD_DIR)/%.d) \
$(C_SOURCES:%.c=$(BUILD_DIR)/%.d)

LIBS = $(OUTPUT_DIR)/$(LIBUDPMS)
PLUGINS = $(OUTPUT_DIR)/$(LIBTRNXPLUG)

BINARIES = $(OUTPUT_DIR)/$(TRNLCM) \
$(OUTPUT_DIR)/$(TRNXPP) \
$(OUTPUT_DIR)/$(FVTEST)

LCM_HPP = $(PCF_HPP) $(OI_HPP) $(GSS_HPP) $(TRN_HPP)
JARS = $(OUTPUT_DIR)/$(ROVTRN_JAR)
LCM_PKGS = $(PCF) $(GSS) $(OI) $(TRNROV) $(TRN)
LCM_SRC = $(PCF_LCM) $(OI_LCM) $(GSS_LCM)  $(TRN_LCM)
JAVA_SRC = $(PCF_JAVA) $(OI_JAVA) $(GSS_JAVA) $(TRN_JAVA)
JAVA_BIN = $(PCF_CLASS) $(OI_CLASS) $(GSS_CLASS) $(TRN_CLASS)
CLEANUP = gmon.out
# dSYMs : XCode debug symbol file folders
#DSYMS = $(BINARIES:%=%.dSYM)
#RM_DSYMS = rm -rf $(DSYMS)

########################################
# Rules: build targets

all: $(LIBS) $(LCM_HPP) $(PLUGINS) $(JARS) $(BINARIES)

# example: build lib
$(OUTPUT_DIR)/$(LIBUDPMS): $(LIBUDPMS_OBJ)
	@echo building $@...
	ar -rcs $@ $^

$(OUTPUT_DIR)/$(LIBTRNXPLUG): $(LIBTRNXPLUG_OBJ)
	@echo building $@...
	ar -rcs $@ $^

# build trnlcm test
$(OUTPUT_DIR)/$(TRNLCM): $(LCM_HPP) $(TRNLCM_OBJ)
	@echo building $@...
	$(CXX) $(CXXFLAGS) $(INC_PATHS) $(LIB_PATHS) -o  $@ $(LD_FLAGS) $(TRNLCM_OBJ) $(TRNLCM_LIBS)
	@echo

$(OUTPUT_DIR)/$(TRNXPP): $(LCM_HPP) $(TRNXPP_HPP) $(TRNXPP_OBJ) $(LIBTRNXPLUG_OBJ)
	@echo building $@...
	$(CXX) $(CXXFLAGS) $(INC_PATHS) $(LIB_PATHS) -o  $@ $(LD_FLAGS) $(TRNXPP_OBJ) $(TRNXPP_LIBS)
	@echo

$(OUTPUT_DIR)/$(FVTEST): $(FVTEST_HPP) $(FVTEST_OBJ)
	@echo building $@...
	$(CXX) $(CXXFLAGS) $(INC_PATHS) $(LIB_PATHS) -o  $@ $(LD_FLAGS) $(FVTEST_OBJ) $(FVTEST_LIBS)
	@echo

$(OUTPUT_DIR)/$(ROVTRN_JAR): $(JAVA_SRC) $(JAVA_BIN) #$(PCF_JAVA) $(PCF_CLASS)
	@echo building $@...
	jar cvf $(OUTPUT_DIR)/$(ROVTRN_JAR) $(JAVA_BIN)

# generate dependency files
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),purge)
-include $(DEPENDS)
endif
endif

# rule: build object files from source files
$(BUILD_DIR)/%.o : %.cpp
	@echo compiling $<...
	$(CXX) $(CXXFLAGS) $(INC_PATHS) -MMD -MP -c $< -o $@
	@echo

$(BUILD_DIR)/%.o : %.c
	@echo compiling $<...
	$(CC) $(CFLAGS) $(INC_PATHS) -MMD -MP -c $< -o $@
	@echo

# rule: build dependency files from source files
# -MF  write the generated dependency rule to a file
# -MG  assume missing headers will be generated and don't stop with an error
# -MM  generate dependency rule for prerequisite, skipping system headers
# -MP  add phony target for each header to prevent errors when header is missing
# -MT  add a target to the generated dependency
$(BUILD_DIR)/%.d : %.cpp %.c
	@[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	@[ -d $(OUTPUT_DIR) ] || mkdir -p $(OUTPUT_DIR)
	@echo generating dependency file for $<
	@set -e; $(CPP) -MM -MG $(CPPFLAGS) $(INC_PATHS) $< \
	| awk '/o:/ {printf "%s", "$@ $(BUILD_DIR)/"} {print}' > $@; \
	[ -s $@ ] || rm -f $@
	@echo

$(PCF)/%.hpp : $(LCM_TYPES)/%.lcm
	@ echo compiling $@...
	lcm-gen -x $<
	@echo

$(PCF)/%.java : $(LCM_TYPES)/%.lcm
	@ echo compiling $@...
	lcm-gen -j $<
	@echo

$(PCF)/%.class : $(PCF)/%.java
	@ echo compiling $@...
	javac -cp $(LCM_JAR) $<
	@echo

$(OI)/%.hpp : $(LCM_TYPES)/%.lcm
	@ echo compiling $@...
	lcm-gen -x $<
	@echo

$(OI)/%.java : $(LCM_TYPES)/%.lcm
	@ echo compiling $@...
	lcm-gen -j $<
	@echo

# this rule is different b/c multiple dependent
# files must be compiled together
$(OI)/%.class : $(OI_JAVA)
	@ echo compiling $@...
	javac -cp $(LCM_JAR) $^
	@echo

$(GSS)/%.hpp : $(LCM_TYPES)/%.lcm
	@ echo compiling $@...
	lcm-gen -x $<
	@echo

$(GSS)/%.java : $(LCM_TYPES)/%.lcm
	@ echo compiling $@...
	lcm-gen -j $<
	@echo

# this rule is different b/c multiple dependent
# files must be compiled together
$(GSS)/%.class : $(GSS_JAVA)
	@ echo compiling $@...
	javac -cp $(LCM_JAR) $^
	@echo

$(TRN)/%.hpp : $(LCM_TYPES)/%.lcm
	@ echo compiling $@...
	lcm-gen -x $<
	@echo

$(TRN)/%.java : $(LCM_TYPES)/%.lcm
	@ echo compiling $@...
	lcm-gen -j $<
	@echo

# this rule is different b/c multiple dependent
# files must be compiled together
$(TRN)/%.class : $(TRN_JAVA)
	@ echo compiling $@...
	javac -cp $(LCM_JAR) $^
	@echo

install:
	@echo "make install...(not implemented)"

########################################
# Rules:
.PHONY: clean
.PHONY: purge

# clean : delete object, dependency, binary files
clean:
	rm -f $(OBJECTS) $(DEPENDS) $(LIBS) $(PLUGINS) $(BINARIES) $(JARS)
	rm -rf $(LCM_PKGS)
	$(RM_DSYMS)

# purge : delete delete object, dependency, binary files, build directories
purge:
	rm -f $(BINARIES) $(OBJECTS) $(DEPENDS) $(CLEANUP)
	rm -rf $(OUTPUT_DIR) $(BUILD_DIR) $(DSYMS)

