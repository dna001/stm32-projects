#
# GNU Makefile for STM32 Template application
# Copyright 2013 Daniel Nygren
#

NAME = stm32-usb-audio

# Directory layout
OUTDIR  = out
BASE    = ..
PRJ     = $(BASE)/stm32-usb-audio
STM_LIB = $(BASE)/lib/stm32f4xx_stdperiph_driver
CMSIS   = $(BASE)/lib/cmsis
USB_DEV_LIB  = $(BASE)/lib/stm32_usb_device_library
USB_OTG_LIB  = $(BASE)/lib/stm32_usb_otg_driver

INCLUDES = -I $(PRJ)/inc \
           -I $(STM_LIB)/inc \
           -I $(CMSIS) \
           -I $(USB_DEV_LIB)/class/audio/inc \
           -I $(USB_DEV_LIB)/core/inc \
           -I $(USB_OTG_LIB)/inc

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
       $(PRJ)/src/main.c \
       $(PRJ)/src/stm32f4_discovery_audio_codec.c \
       $(PRJ)/src/stm32f4_discovery.c \
       $(PRJ)/src/usb_bsp.c \
       $(PRJ)/src/usbd_desc.c \
       $(PRJ)/src/usbd_usr.c
CSRC += \
       $(STM_LIB)/src/misc.c \
       $(STM_LIB)/src/stm32f4xx_dac.c \
       $(STM_LIB)/src/stm32f4xx_dma.c \
       $(STM_LIB)/src/stm32f4xx_exti.c \
       $(STM_LIB)/src/stm32f4xx_flash.c \
       $(STM_LIB)/src/stm32f4xx_gpio.c \
       $(STM_LIB)/src/stm32f4xx_i2c.c \
       $(STM_LIB)/src/stm32f4xx_pwr.c \
       $(STM_LIB)/src/stm32f4xx_rcc.c \
       $(STM_LIB)/src/stm32f4xx_rtc.c \
       $(STM_LIB)/src/stm32f4xx_spi.c \
       $(STM_LIB)/src/stm32f4xx_syscfg.c \
       $(STM_LIB)/src/stm32f4xx_tim.c \
       $(STM_LIB)/src/stm32f4xx_usart.c
CSRC += \
       $(CMSIS)/system_stm32f4xx.c
## USB device core
CSRC += \
       $(USB_DEV_LIB)/core/src/usbd_core.c \
       $(USB_DEV_LIB)/core/src/usbd_ioreq.c \
       $(USB_DEV_LIB)/core/src/usbd_req.c \
## USB device audio class
CSRC += \
       $(USB_DEV_LIB)/class/audio/src/usbd_audio_core.c \
       $(USB_DEV_LIB)/class/audio/src/usbd_audio_out_if.c
## USB OTG 
 CSRC += \
        $(USB_OTG_LIB)/src/usb_core.c \
        $(USB_OTG_LIB)/src/usb_dcd_int.c \
        $(USB_OTG_LIB)/src/usb_dcd.c

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
