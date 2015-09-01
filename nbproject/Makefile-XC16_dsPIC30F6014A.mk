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
ifeq "$(wildcard nbproject/Makefile-local-XC16_dsPIC30F6014A.mk)" "nbproject/Makefile-local-XC16_dsPIC30F6014A.mk"
include nbproject/Makefile-local-XC16_dsPIC30F6014A.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=XC16_dsPIC30F6014A
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/os.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/os.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=user.c traps.c system_Timer_HDI.c system_Scheduler.c system_Process_Management_HDI.c system.c interrupts.c configuration_bits.c HDI_init_port.c main.c system_Events.c system_IO.c system_IO_motors.c system_IO_i2c.c system_IO_uart.c system_IO_camera.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/user.o ${OBJECTDIR}/traps.o ${OBJECTDIR}/system_Timer_HDI.o ${OBJECTDIR}/system_Scheduler.o ${OBJECTDIR}/system_Process_Management_HDI.o ${OBJECTDIR}/system.o ${OBJECTDIR}/interrupts.o ${OBJECTDIR}/configuration_bits.o ${OBJECTDIR}/HDI_init_port.o ${OBJECTDIR}/main.o ${OBJECTDIR}/system_Events.o ${OBJECTDIR}/system_IO.o ${OBJECTDIR}/system_IO_motors.o ${OBJECTDIR}/system_IO_i2c.o ${OBJECTDIR}/system_IO_uart.o ${OBJECTDIR}/system_IO_camera.o
POSSIBLE_DEPFILES=${OBJECTDIR}/user.o.d ${OBJECTDIR}/traps.o.d ${OBJECTDIR}/system_Timer_HDI.o.d ${OBJECTDIR}/system_Scheduler.o.d ${OBJECTDIR}/system_Process_Management_HDI.o.d ${OBJECTDIR}/system.o.d ${OBJECTDIR}/interrupts.o.d ${OBJECTDIR}/configuration_bits.o.d ${OBJECTDIR}/HDI_init_port.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/system_Events.o.d ${OBJECTDIR}/system_IO.o.d ${OBJECTDIR}/system_IO_motors.o.d ${OBJECTDIR}/system_IO_i2c.o.d ${OBJECTDIR}/system_IO_uart.o.d ${OBJECTDIR}/system_IO_camera.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/user.o ${OBJECTDIR}/traps.o ${OBJECTDIR}/system_Timer_HDI.o ${OBJECTDIR}/system_Scheduler.o ${OBJECTDIR}/system_Process_Management_HDI.o ${OBJECTDIR}/system.o ${OBJECTDIR}/interrupts.o ${OBJECTDIR}/configuration_bits.o ${OBJECTDIR}/HDI_init_port.o ${OBJECTDIR}/main.o ${OBJECTDIR}/system_Events.o ${OBJECTDIR}/system_IO.o ${OBJECTDIR}/system_IO_motors.o ${OBJECTDIR}/system_IO_i2c.o ${OBJECTDIR}/system_IO_uart.o ${OBJECTDIR}/system_IO_camera.o

# Source Files
SOURCEFILES=user.c traps.c system_Timer_HDI.c system_Scheduler.c system_Process_Management_HDI.c system.c interrupts.c configuration_bits.c HDI_init_port.c main.c system_Events.c system_IO.c system_IO_motors.c system_IO_i2c.c system_IO_uart.c system_IO_camera.c


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
	${MAKE}  -f nbproject/Makefile-XC16_dsPIC30F6014A.mk dist/${CND_CONF}/${IMAGE_TYPE}/os.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=30F6014A
MP_LINKER_FILE_OPTION=,--script=p30F6014A.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/user.o: user.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/user.o.d 
	@${RM} ${OBJECTDIR}/user.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  user.c  -o ${OBJECTDIR}/user.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/user.o.d"      -g -D__DEBUG     -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/user.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/traps.o: traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/traps.o.d 
	@${RM} ${OBJECTDIR}/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  traps.c  -o ${OBJECTDIR}/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/traps.o.d"      -g -D__DEBUG     -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/traps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_Timer_HDI.o: system_Timer_HDI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_Timer_HDI.o.d 
	@${RM} ${OBJECTDIR}/system_Timer_HDI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_Timer_HDI.c  -o ${OBJECTDIR}/system_Timer_HDI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_Timer_HDI.o.d"      -g -D__DEBUG     -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_Timer_HDI.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_Scheduler.o: system_Scheduler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_Scheduler.o.d 
	@${RM} ${OBJECTDIR}/system_Scheduler.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_Scheduler.c  -o ${OBJECTDIR}/system_Scheduler.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_Scheduler.o.d"      -g -D__DEBUG     -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_Scheduler.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_Process_Management_HDI.o: system_Process_Management_HDI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_Process_Management_HDI.o.d 
	@${RM} ${OBJECTDIR}/system_Process_Management_HDI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_Process_Management_HDI.c  -o ${OBJECTDIR}/system_Process_Management_HDI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_Process_Management_HDI.o.d"      -g -D__DEBUG     -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_Process_Management_HDI.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system.o: system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system.o.d 
	@${RM} ${OBJECTDIR}/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system.c  -o ${OBJECTDIR}/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system.o.d"      -g -D__DEBUG     -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/interrupts.o: interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/interrupts.o.d 
	@${RM} ${OBJECTDIR}/interrupts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  interrupts.c  -o ${OBJECTDIR}/interrupts.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/interrupts.o.d"      -g -D__DEBUG     -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/interrupts.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/configuration_bits.o: configuration_bits.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/configuration_bits.o.d 
	@${RM} ${OBJECTDIR}/configuration_bits.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  configuration_bits.c  -o ${OBJECTDIR}/configuration_bits.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/configuration_bits.o.d"      -g -D__DEBUG     -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/configuration_bits.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/HDI_init_port.o: HDI_init_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/HDI_init_port.o.d 
	@${RM} ${OBJECTDIR}/HDI_init_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  HDI_init_port.c  -o ${OBJECTDIR}/HDI_init_port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/HDI_init_port.o.d"      -g -D__DEBUG     -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/HDI_init_port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG     -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_Events.o: system_Events.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_Events.o.d 
	@${RM} ${OBJECTDIR}/system_Events.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_Events.c  -o ${OBJECTDIR}/system_Events.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_Events.o.d"      -g -D__DEBUG     -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_Events.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_IO.o: system_IO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_IO.o.d 
	@${RM} ${OBJECTDIR}/system_IO.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_IO.c  -o ${OBJECTDIR}/system_IO.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_IO.o.d"      -g -D__DEBUG     -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_IO.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_IO_motors.o: system_IO_motors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_IO_motors.o.d 
	@${RM} ${OBJECTDIR}/system_IO_motors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_IO_motors.c  -o ${OBJECTDIR}/system_IO_motors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_IO_motors.o.d"      -g -D__DEBUG     -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_IO_motors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_IO_i2c.o: system_IO_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_IO_i2c.o.d 
	@${RM} ${OBJECTDIR}/system_IO_i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_IO_i2c.c  -o ${OBJECTDIR}/system_IO_i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_IO_i2c.o.d"      -g -D__DEBUG     -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_IO_i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_IO_uart.o: system_IO_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_IO_uart.o.d 
	@${RM} ${OBJECTDIR}/system_IO_uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_IO_uart.c  -o ${OBJECTDIR}/system_IO_uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_IO_uart.o.d"      -g -D__DEBUG     -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_IO_uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_IO_camera.o: system_IO_camera.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_IO_camera.o.d 
	@${RM} ${OBJECTDIR}/system_IO_camera.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_IO_camera.c  -o ${OBJECTDIR}/system_IO_camera.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_IO_camera.o.d"      -g -D__DEBUG     -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_IO_camera.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/user.o: user.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/user.o.d 
	@${RM} ${OBJECTDIR}/user.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  user.c  -o ${OBJECTDIR}/user.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/user.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/user.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/traps.o: traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/traps.o.d 
	@${RM} ${OBJECTDIR}/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  traps.c  -o ${OBJECTDIR}/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/traps.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/traps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_Timer_HDI.o: system_Timer_HDI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_Timer_HDI.o.d 
	@${RM} ${OBJECTDIR}/system_Timer_HDI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_Timer_HDI.c  -o ${OBJECTDIR}/system_Timer_HDI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_Timer_HDI.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_Timer_HDI.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_Scheduler.o: system_Scheduler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_Scheduler.o.d 
	@${RM} ${OBJECTDIR}/system_Scheduler.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_Scheduler.c  -o ${OBJECTDIR}/system_Scheduler.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_Scheduler.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_Scheduler.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_Process_Management_HDI.o: system_Process_Management_HDI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_Process_Management_HDI.o.d 
	@${RM} ${OBJECTDIR}/system_Process_Management_HDI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_Process_Management_HDI.c  -o ${OBJECTDIR}/system_Process_Management_HDI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_Process_Management_HDI.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_Process_Management_HDI.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system.o: system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system.o.d 
	@${RM} ${OBJECTDIR}/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system.c  -o ${OBJECTDIR}/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/interrupts.o: interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/interrupts.o.d 
	@${RM} ${OBJECTDIR}/interrupts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  interrupts.c  -o ${OBJECTDIR}/interrupts.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/interrupts.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/interrupts.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/configuration_bits.o: configuration_bits.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/configuration_bits.o.d 
	@${RM} ${OBJECTDIR}/configuration_bits.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  configuration_bits.c  -o ${OBJECTDIR}/configuration_bits.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/configuration_bits.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/configuration_bits.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/HDI_init_port.o: HDI_init_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/HDI_init_port.o.d 
	@${RM} ${OBJECTDIR}/HDI_init_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  HDI_init_port.c  -o ${OBJECTDIR}/HDI_init_port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/HDI_init_port.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/HDI_init_port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_Events.o: system_Events.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_Events.o.d 
	@${RM} ${OBJECTDIR}/system_Events.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_Events.c  -o ${OBJECTDIR}/system_Events.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_Events.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_Events.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_IO.o: system_IO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_IO.o.d 
	@${RM} ${OBJECTDIR}/system_IO.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_IO.c  -o ${OBJECTDIR}/system_IO.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_IO.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_IO.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_IO_motors.o: system_IO_motors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_IO_motors.o.d 
	@${RM} ${OBJECTDIR}/system_IO_motors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_IO_motors.c  -o ${OBJECTDIR}/system_IO_motors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_IO_motors.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_IO_motors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_IO_i2c.o: system_IO_i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_IO_i2c.o.d 
	@${RM} ${OBJECTDIR}/system_IO_i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_IO_i2c.c  -o ${OBJECTDIR}/system_IO_i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_IO_i2c.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_IO_i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_IO_uart.o: system_IO_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_IO_uart.o.d 
	@${RM} ${OBJECTDIR}/system_IO_uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_IO_uart.c  -o ${OBJECTDIR}/system_IO_uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_IO_uart.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_IO_uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system_IO_camera.o: system_IO_camera.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/system_IO_camera.o.d 
	@${RM} ${OBJECTDIR}/system_IO_camera.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_IO_camera.c  -o ${OBJECTDIR}/system_IO_camera.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system_IO_camera.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off -fms-extensions
	@${FIXDEPS} "${OBJECTDIR}/system_IO_camera.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/os.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/os.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG   -omf=elf     -Wl,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,$(MP_LINKER_FILE_OPTION),--heap=1024,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/os.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/os.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=1024,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/os.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/XC16_dsPIC30F6014A
	${RM} -r dist/XC16_dsPIC30F6014A

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif