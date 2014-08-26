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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Project_P1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Project_P1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=FatFs/ff.c FatFs/mmcPIC32.c Bluetooth_Module.c General_function.c KTY_81_110.c SPI_function.c User_Gestion.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/FatFs/ff.o ${OBJECTDIR}/FatFs/mmcPIC32.o ${OBJECTDIR}/Bluetooth_Module.o ${OBJECTDIR}/General_function.o ${OBJECTDIR}/KTY_81_110.o ${OBJECTDIR}/SPI_function.o ${OBJECTDIR}/User_Gestion.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/FatFs/ff.o.d ${OBJECTDIR}/FatFs/mmcPIC32.o.d ${OBJECTDIR}/Bluetooth_Module.o.d ${OBJECTDIR}/General_function.o.d ${OBJECTDIR}/KTY_81_110.o.d ${OBJECTDIR}/SPI_function.o.d ${OBJECTDIR}/User_Gestion.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/FatFs/ff.o ${OBJECTDIR}/FatFs/mmcPIC32.o ${OBJECTDIR}/Bluetooth_Module.o ${OBJECTDIR}/General_function.o ${OBJECTDIR}/KTY_81_110.o ${OBJECTDIR}/SPI_function.o ${OBJECTDIR}/User_Gestion.o ${OBJECTDIR}/main.o

# Source Files
SOURCEFILES=FatFs/ff.c FatFs/mmcPIC32.c Bluetooth_Module.c General_function.c KTY_81_110.c SPI_function.c User_Gestion.c main.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Project_P1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX795F512L
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
${OBJECTDIR}/FatFs/ff.o: FatFs/ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/FatFs 
	@${RM} ${OBJECTDIR}/FatFs/ff.o.d 
	@${RM} ${OBJECTDIR}/FatFs/ff.o 
	@${FIXDEPS} "${OBJECTDIR}/FatFs/ff.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/FatFs/ff.o.d" -o ${OBJECTDIR}/FatFs/ff.o FatFs/ff.c   
	
${OBJECTDIR}/FatFs/mmcPIC32.o: FatFs/mmcPIC32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/FatFs 
	@${RM} ${OBJECTDIR}/FatFs/mmcPIC32.o.d 
	@${RM} ${OBJECTDIR}/FatFs/mmcPIC32.o 
	@${FIXDEPS} "${OBJECTDIR}/FatFs/mmcPIC32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/FatFs/mmcPIC32.o.d" -o ${OBJECTDIR}/FatFs/mmcPIC32.o FatFs/mmcPIC32.c   
	
${OBJECTDIR}/Bluetooth_Module.o: Bluetooth_Module.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Bluetooth_Module.o.d 
	@${RM} ${OBJECTDIR}/Bluetooth_Module.o 
	@${FIXDEPS} "${OBJECTDIR}/Bluetooth_Module.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Bluetooth_Module.o.d" -o ${OBJECTDIR}/Bluetooth_Module.o Bluetooth_Module.c   
	
${OBJECTDIR}/General_function.o: General_function.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/General_function.o.d 
	@${RM} ${OBJECTDIR}/General_function.o 
	@${FIXDEPS} "${OBJECTDIR}/General_function.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/General_function.o.d" -o ${OBJECTDIR}/General_function.o General_function.c   
	
${OBJECTDIR}/KTY_81_110.o: KTY_81_110.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/KTY_81_110.o.d 
	@${RM} ${OBJECTDIR}/KTY_81_110.o 
	@${FIXDEPS} "${OBJECTDIR}/KTY_81_110.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KTY_81_110.o.d" -o ${OBJECTDIR}/KTY_81_110.o KTY_81_110.c   
	
${OBJECTDIR}/SPI_function.o: SPI_function.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/SPI_function.o.d 
	@${RM} ${OBJECTDIR}/SPI_function.o 
	@${FIXDEPS} "${OBJECTDIR}/SPI_function.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SPI_function.o.d" -o ${OBJECTDIR}/SPI_function.o SPI_function.c   
	
${OBJECTDIR}/User_Gestion.o: User_Gestion.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/User_Gestion.o.d 
	@${RM} ${OBJECTDIR}/User_Gestion.o 
	@${FIXDEPS} "${OBJECTDIR}/User_Gestion.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/User_Gestion.o.d" -o ${OBJECTDIR}/User_Gestion.o User_Gestion.c   
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c   
	
else
${OBJECTDIR}/FatFs/ff.o: FatFs/ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/FatFs 
	@${RM} ${OBJECTDIR}/FatFs/ff.o.d 
	@${RM} ${OBJECTDIR}/FatFs/ff.o 
	@${FIXDEPS} "${OBJECTDIR}/FatFs/ff.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/FatFs/ff.o.d" -o ${OBJECTDIR}/FatFs/ff.o FatFs/ff.c   
	
${OBJECTDIR}/FatFs/mmcPIC32.o: FatFs/mmcPIC32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/FatFs 
	@${RM} ${OBJECTDIR}/FatFs/mmcPIC32.o.d 
	@${RM} ${OBJECTDIR}/FatFs/mmcPIC32.o 
	@${FIXDEPS} "${OBJECTDIR}/FatFs/mmcPIC32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/FatFs/mmcPIC32.o.d" -o ${OBJECTDIR}/FatFs/mmcPIC32.o FatFs/mmcPIC32.c   
	
${OBJECTDIR}/Bluetooth_Module.o: Bluetooth_Module.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Bluetooth_Module.o.d 
	@${RM} ${OBJECTDIR}/Bluetooth_Module.o 
	@${FIXDEPS} "${OBJECTDIR}/Bluetooth_Module.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Bluetooth_Module.o.d" -o ${OBJECTDIR}/Bluetooth_Module.o Bluetooth_Module.c   
	
${OBJECTDIR}/General_function.o: General_function.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/General_function.o.d 
	@${RM} ${OBJECTDIR}/General_function.o 
	@${FIXDEPS} "${OBJECTDIR}/General_function.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/General_function.o.d" -o ${OBJECTDIR}/General_function.o General_function.c   
	
${OBJECTDIR}/KTY_81_110.o: KTY_81_110.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/KTY_81_110.o.d 
	@${RM} ${OBJECTDIR}/KTY_81_110.o 
	@${FIXDEPS} "${OBJECTDIR}/KTY_81_110.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/KTY_81_110.o.d" -o ${OBJECTDIR}/KTY_81_110.o KTY_81_110.c   
	
${OBJECTDIR}/SPI_function.o: SPI_function.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/SPI_function.o.d 
	@${RM} ${OBJECTDIR}/SPI_function.o 
	@${FIXDEPS} "${OBJECTDIR}/SPI_function.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SPI_function.o.d" -o ${OBJECTDIR}/SPI_function.o SPI_function.c   
	
${OBJECTDIR}/User_Gestion.o: User_Gestion.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/User_Gestion.o.d 
	@${RM} ${OBJECTDIR}/User_Gestion.o 
	@${FIXDEPS} "${OBJECTDIR}/User_Gestion.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/User_Gestion.o.d" -o ${OBJECTDIR}/User_Gestion.o User_Gestion.c   
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Project_P1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Project_P1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}           -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Project_P1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Project_P1.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/Project_P1.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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
