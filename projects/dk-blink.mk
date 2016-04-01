OUTPUT := dk-blink
DEFINES := BOARD_PCA10028


MODULES := \
	lib/nrf_drv/delay \
	src/blink \

include make/nrf51-s130.mk
