OUTPUT := dartuino-blink
DEFINES := BOARD_DARTUINOP0


MODULES := \
	lib/nrf_drv/delay \
	src/blink \

include make/nrf51-s130.mk
