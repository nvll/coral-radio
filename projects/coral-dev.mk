OUTPUT := coral-dev
DEFINES := BOARD_CUSTOM __HEAP_SIZE=0 DEBUG

MODULES := \
	src/coral \

include make/nrf51822-s130.mk
