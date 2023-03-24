#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile

# Environment
SHELL=cmd.exe
# Adding MPLAB X bin directory to path
PATH:=C:/Program Files/Microchip/MPLABX/mplab_ide/mplab_ide/modules/../../bin/:$(PATH)
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/handling_exceptions.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/handling_exceptions.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/812168374/main.o ${OBJECTDIR}/_ext/812168374/exceptions.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/812168374/main.o.d ${OBJECTDIR}/_ext/812168374/exceptions.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/812168374/main.o ${OBJECTDIR}/_ext/812168374/exceptions.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

# Path to java used to run MPLAB X when this makefile was created
MP_JAVA_PATH="C:\Program Files\Java\jre6/bin/"
OS_CURRENT="$(shell uname -s)"
############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
MP_CC="C:\Program Files\Microchip\xc32\v1.00\bin\xc32-gcc.exe"
# MP_BC is not defined
MP_AS="C:\Program Files\Microchip\xc32\v1.00\bin\xc32-as.exe"
MP_LD="C:\Program Files\Microchip\xc32\v1.00\bin\xc32-ld.exe"
MP_AR="C:\Program Files\Microchip\xc32\v1.00\bin\xc32-ar.exe"
DEP_GEN=${MP_JAVA_PATH}java -jar "C:/Program Files/Microchip/MPLABX/mplab_ide/mplab_ide/modules/../../bin/extractobjectdependencies.jar" 
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps
MP_CC_DIR="C:\Program Files\Microchip\xc32\v1.00\bin"
# MP_BC_DIR is not defined
MP_AS_DIR="C:\Program Files\Microchip\xc32\v1.00\bin"
MP_LD_DIR="C:\Program Files\Microchip\xc32\v1.00\bin"
MP_AR_DIR="C:\Program Files\Microchip\xc32\v1.00\bin"
# MP_BC_DIR is not defined

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/handling_exceptions.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX360F512L
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
${OBJECTDIR}/_ext/812168374/main.o: ../source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/main.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -MMD -MF "${OBJECTDIR}/_ext/812168374/main.o.d" -o ${OBJECTDIR}/_ext/812168374/main.o ../source/main.c  
	
${OBJECTDIR}/_ext/812168374/exceptions.o: ../source/exceptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/exceptions.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/exceptions.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -MMD -MF "${OBJECTDIR}/_ext/812168374/exceptions.o.d" -o ${OBJECTDIR}/_ext/812168374/exceptions.o ../source/exceptions.c  
	
else
${OBJECTDIR}/_ext/812168374/main.o: ../source/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/main.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -MMD -MF "${OBJECTDIR}/_ext/812168374/main.o.d" -o ${OBJECTDIR}/_ext/812168374/main.o ../source/main.c  
	
${OBJECTDIR}/_ext/812168374/exceptions.o: ../source/exceptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/exceptions.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/exceptions.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -MMD -MF "${OBJECTDIR}/_ext/812168374/exceptions.o.d" -o ${OBJECTDIR}/_ext/812168374/exceptions.o ../source/exceptions.c  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/handling_exceptions.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_ICD3=1 -mprocessor=$(MP_PROCESSOR_OPTION) -Os -o dist/${CND_CONF}/${IMAGE_TYPE}/handling_exceptions.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}        -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1 
else
dist/${CND_CONF}/${IMAGE_TYPE}/handling_exceptions.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -Os -o dist/${CND_CONF}/${IMAGE_TYPE}/handling_exceptions.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}        -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/handling_exceptions.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  
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

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
