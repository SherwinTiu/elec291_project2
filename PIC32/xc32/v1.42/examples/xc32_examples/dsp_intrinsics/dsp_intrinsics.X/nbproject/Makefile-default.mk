#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/dsp_intrinsics.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/dsp_intrinsics.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../source/dsp_intrinsics_main.c ../source/filter.c ../source/timer.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/812168374/dsp_intrinsics_main.o ${OBJECTDIR}/_ext/812168374/filter.o ${OBJECTDIR}/_ext/812168374/timer.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/812168374/dsp_intrinsics_main.o.d ${OBJECTDIR}/_ext/812168374/filter.o.d ${OBJECTDIR}/_ext/812168374/timer.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/812168374/dsp_intrinsics_main.o ${OBJECTDIR}/_ext/812168374/filter.o ${OBJECTDIR}/_ext/812168374/timer.o

# Source Files
SOURCEFILES=../source/dsp_intrinsics_main.c ../source/filter.c ../source/timer.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/dsp_intrinsics.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MZ2048EFM144
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/812168374/dsp_intrinsics_main.o: ../source/dsp_intrinsics_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/812168374" 
	@${RM} ${OBJECTDIR}/_ext/812168374/dsp_intrinsics_main.o.d 
	@${RM} ${OBJECTDIR}/_ext/812168374/dsp_intrinsics_main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/dsp_intrinsics_main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MMD -MF "${OBJECTDIR}/_ext/812168374/dsp_intrinsics_main.o.d" -o ${OBJECTDIR}/_ext/812168374/dsp_intrinsics_main.o ../source/dsp_intrinsics_main.c    -no-legacy-libc 
	
${OBJECTDIR}/_ext/812168374/filter.o: ../source/filter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/812168374" 
	@${RM} ${OBJECTDIR}/_ext/812168374/filter.o.d 
	@${RM} ${OBJECTDIR}/_ext/812168374/filter.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/filter.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MMD -MF "${OBJECTDIR}/_ext/812168374/filter.o.d" -o ${OBJECTDIR}/_ext/812168374/filter.o ../source/filter.c    -no-legacy-libc 
	
${OBJECTDIR}/_ext/812168374/timer.o: ../source/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/812168374" 
	@${RM} ${OBJECTDIR}/_ext/812168374/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/812168374/timer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/timer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MMD -MF "${OBJECTDIR}/_ext/812168374/timer.o.d" -o ${OBJECTDIR}/_ext/812168374/timer.o ../source/timer.c    -no-legacy-libc 
	
else
${OBJECTDIR}/_ext/812168374/dsp_intrinsics_main.o: ../source/dsp_intrinsics_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/812168374" 
	@${RM} ${OBJECTDIR}/_ext/812168374/dsp_intrinsics_main.o.d 
	@${RM} ${OBJECTDIR}/_ext/812168374/dsp_intrinsics_main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/dsp_intrinsics_main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MMD -MF "${OBJECTDIR}/_ext/812168374/dsp_intrinsics_main.o.d" -o ${OBJECTDIR}/_ext/812168374/dsp_intrinsics_main.o ../source/dsp_intrinsics_main.c    -no-legacy-libc 
	
${OBJECTDIR}/_ext/812168374/filter.o: ../source/filter.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/812168374" 
	@${RM} ${OBJECTDIR}/_ext/812168374/filter.o.d 
	@${RM} ${OBJECTDIR}/_ext/812168374/filter.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/filter.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MMD -MF "${OBJECTDIR}/_ext/812168374/filter.o.d" -o ${OBJECTDIR}/_ext/812168374/filter.o ../source/filter.c    -no-legacy-libc 
	
${OBJECTDIR}/_ext/812168374/timer.o: ../source/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/812168374" 
	@${RM} ${OBJECTDIR}/_ext/812168374/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/812168374/timer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/timer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MMD -MF "${OBJECTDIR}/_ext/812168374/timer.o.d" -o ${OBJECTDIR}/_ext/812168374/timer.o ../source/timer.c    -no-legacy-libc 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/dsp_intrinsics.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_SIMULATOR=1 -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -o dist/${CND_CONF}/${IMAGE_TYPE}/dsp_intrinsics.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -no-legacy-libc      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/dsp_intrinsics.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -o dist/${CND_CONF}/${IMAGE_TYPE}/dsp_intrinsics.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -no-legacy-libc  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/dsp_intrinsics.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
