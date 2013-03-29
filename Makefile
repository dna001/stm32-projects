
.PHONY : template audio_usb

template:
	make -C stm32-template -f Makefile

audio_usb:
	make -C stm32-template -f Makefile
