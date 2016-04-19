OUTPUT := dk-blink
DEFINES := BOARD_PCA10028


MODULES := \
	src/blink

include make/nrf51822-s130.mk
