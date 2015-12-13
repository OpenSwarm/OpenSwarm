# Makefile
# ==============================================================================
# 07.02.2002 cmc :  Created based on sample from Kevin Nickels.
# 07.15.2004 cmc :  Updated and applied to sn_sim project.
# 08.05.2004 cmc :  Added tags to removed files in distclean, added "doc" target
# 12.16.2004 cmc :  Remove output files with distclean.
# ------------------------------------------------------------------------------
#  TODO: Figure out why LD won't link C++ object files with standard args.
# ==============================================================================

PROG   = ecal_line_epuck_000
EPUCKLIBROOT = ../../../../library

# ==============================================================================
# Potential Epuck Libraries
# ------------------------------------------------------------------------------

USE_EPUCK_A_D = 1
USE_EPUCK_CODEC = 1
USE_EPUCK_MOTOR_LED = 1
USE_EPUCK_UART = 1
USE_EPUCK_I2C = 1

EXTERNAL_OBJS = #blank

ifeq ($(USE_EPUCK_A_D),1)
	EXTERNAL_OBJS += $(EPUCKLIBROOT)/a_d/e_accelerometer.o $(EPUCKLIBROOT)/a_d/e_ad_conv.o $(EPUCKLIBROOT)/a_d/e_prox_timer2.o
endif

ifeq ($(USE_EPUCK_CODEC),1)
	EXTERNAL_OBJS += $(EPUCKLIBROOT)/codec/e_sub_dci_kickoff.o $(EPUCKLIBROOT)/codec/e_isr_dci.o $(EPUCKLIBROOT)/codec/e_init_dci_master.o $(EPUCKLIBROOT)/codec/e_init_codec_slave.o $(EPUCKLIBROOT)/codec/e_const_sound.o $(EPUCKLIBROOT)/codec/e_sound.o
endif

ifeq ($(USE_EPUCK_MOTOR_LED),1)
	EXTERNAL_OBJS += $(EPUCKLIBROOT)/motor_led/e_init_port.o $(EPUCKLIBROOT)/motor_led/e_led.o $(EPUCKLIBROOT)/motor_led/e_motors.o 
endif

ifeq ($(USE_EPUCK_UART),1)
	EXTERNAL_OBJS += $(EPUCKLIBROOT)/uart/e_init_uart1.o $(EPUCKLIBROOT)/uart/e_uart1_rx_char.o $(EPUCKLIBROOT)/uart/e_uart1_tx_char.o 
endif

ifeq ($(USE_EPUCK_I2C),1)
	EXTERNAL_OBJS += $(EPUCKLIBROOT)/I2C/e_i2c_protocol.o $(EPUCKLIBROOT)/I2C/e_i2c_master_module.o  
endif

# ==============================================================================
# Local variables
# ------------------------------------------------------------------------------
#  first, find all the pertinent filenames
ASMSRCS = $(wildcard *.s)
CSRCS   = $(wildcard *.c)
CXXSRCS = $(wildcard *.cc) $(wildcard *.cpp) $(wildcard *.C)
SRCS = $(CSRCS) $(CXXSRCS) $(ASMSRCS)
HDRS = $(wildcard *.h) $(wildcard *.hh)
OBJS = $(addsuffix .o,$(basename $(SRCS)))
DEPS = $(addsuffix .d,$(basename $(SRCS)))

#  program aliases
CTAGS    = ctags
ASM      = pic30-elf-as
CC       = pic30-elf-gcc
LD	 = pic30-elf-ld

#  common options
DEPCFLAGS = -Wall -I. -I$(EPUCKLIBROOT)/std_microchip/support/h -I$(EPUCKLIBROOT)/std_microchip/include -I$(EPUCKLIBROOT) -I../std_microchip/support/h 
CFLAGS   = $(DEPCFLAGS) -mcpu=30F6014A

LDFLAGS  = -L$(EPUCKLIBROOT)/std_microchip/lib --defsym=__ICD2RAM=1 --script=$(EPUCKLIBROOT)/std_microchip/support/gld/p30f6014a.gld -mpic30_elf32
LDLIBS   = -lpic30-elf -lm-elf -lc-elf 
#LDLIBS   = -lpic30-coff -lm-coff -lc-coff 
#LDLIBS   = --start-group -lpic30-coff -lm-coff -lc-coff --end-group
#LDLIBS   = --start-group -lpic30-elf -lm-elf -lc-elf --end-group

ifeq ($(MAKECMDGOALS),debug)
CFLAGS += -g -DDEBUG -DVERBOSITY=0
endif


# ==============================================================================
# Dependencies & rules
# ------------------------------------------------------------------------------
all: $(PROG).hex tags $(DEPS) 

.PHONY: debug
	debug: all

%.hex: %.cof
	pic30-elf-bin2hex $(PROG).cof

%.cof: $(OBJS)
	$(LD) $(LDFLAGS) --start-group $(LDLIBS) $(OBJS) $(EXTERNAL_OBJS) --end-group -o $@

%.o: %.s
	$(ASM) -I../../../library/std_microchip/support/inc -p=30F6014A $< -o $@
# ==============================================================================
# Clean up directory
# ------------------------------------------------------------------------------
.PHONY:	clean
clean:	
	- $(RM) $(OBJS) *~ core.* *.rpo $(PROG).cof $(PROG).hex

.PHONY: distclean
distclean: clean
	- $(RM) $(DEPS) tags *.a *.so $(OUTPUT)
# ==============================================================================


# ==============================================================================
# make tags files for vim
# ------------------------------------------------------------------------------
tags:	$(SRCS) $(HDRS)
	$(CTAGS) $(SRCS) $(HDRS)
# ==============================================================================


# ==============================================================================
# a default rule for building dependency files
# ------------------------------------------------------------------------------
%.d: %.c
	@ $(SHELL) -ec '$(CXX) -MM $(DEPCFLAGS) $< > $@'
%.d: %.cc
	@ $(SHELL) -ec '$(CXX) -MM $(DEPCFLAGS) $< > $@'
%.d: %.C
	@ $(SHELL) -ec '$(CXX) -MM $(DEPCFLAGS) $< > $@'
%.d: %.cpp
	@ $(SHELL) -ec '$(CXX) -MM $(DEPCFLAGS) $< > $@'
%.d: %.s
	cat /dev/null > ./$@
# ==============================================================================


# ==============================================================================
# include the source code dependencies
# ------------------------------------------------------------------------------
ifneq ($(MAKECMDGOALS),clean)
include $(DEPS)
endif
# ==============================================================================


# ==============================================================================
# end of Makefile
