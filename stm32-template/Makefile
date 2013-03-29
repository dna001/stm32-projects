#
# GNU Makefile for STM32 Template application
# Copyright 2013 Daniel Nygren
#

NAME = stm32-template

# Directory layout
OUTDIR  = out
BASE    = ..
PRJ     = $(BASE)/stm32-template
STM_LIB = $(BASE)/lib/stm32f4xx_stdperiph_driver
CMSIS   = $(BASE)/lib/cmsis

INCLUDES = -I $(PRJ)/inc \
           -I $(STM_LIB)/inc \
           -I $(CMSIS)

# Toolchain setup
CROSS_COMPILE = arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld -v
AR = $(CROSS_COMPILE)ar
AS = $(CROSS_COMPILE)as
CP = $(CROSS_COMPILE)objcopy
OD = $(CROSS_COMPILE)objdump
  
LNKSCRIPT = stm32f4xx_flash.ld
MFLAGS  = -mcpu=cortex-m4 -mthumb
ASFLAGS = $(MFLAGS)
CFLAGS  = $(MFLAGS) $(INCLUDES) -DUSE_STDPERIPH_DRIVER -c -fno-common -Os -g
LFLAGS  = $(MFLAGS) -T $(LNKSCRIPT) -nostartfiles -Wl,-Map=$(NAME).map

CSRC = \
       $(PRJ)/src/stm32f4xx_it.c \
       $(PRJ)/src/main.c
CSRC += \
       $(STM_LIB)/src/stm32f4xx_gpio.c \
       $(STM_LIB)/src/stm32f4xx_rcc.c
CSRC += \
       $(CMSIS)/system_stm32f4xx.c

COBJ = $(CSRC:.c=.o)

ASRC = $(CMSIS)/cortexm3_macro.s \
       $(CMSIS)/startup_stm32f4xx.s

AOBJ = $(ASRC:.s=.o)

all: binaries

# The assembly has to be converted from IAR style
#ASM_Function-gcc.S: $(PRJ)/source/ASM_Function.s
#	sed -f iar2gas < $(PRJ)/source/ASM_Function.s > ASM_Function-gcc.S

$(NAME).elf: $(AOBJ) $(COBJ) $(LNKSCRIPT)
	$(CC) $(LFLAGS) -o $(NAME).elf $(AOBJ) $(COBJ)

binaries: $(NAME).elf
	$(OD) -S $(NAME).elf > $(NAME).lst
	$(CP) -O binary $(NAME).elf $(NAME).bin
	$(CP) -O ihex $(NAME).elf $(NAME).hex

clean:
	rm -f *.o $(PRJ)/src/*.o $(STM_LIB)/src/*.o \
	    $(NAME).elf $(NAME).bin $(NAME).hex $(NAME).lst $(NAME).map
#	    ASM_Function-gcc.[sS] \
