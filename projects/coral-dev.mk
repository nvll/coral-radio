OUTPUT := coral-dev
DEFINES := BOARD_CUSTOM __HEAP_SIZE=0 DEBUG

MODULES := \
	src/coral \

include make/nrf51-s130.mk
