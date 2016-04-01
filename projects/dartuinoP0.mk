OUTPUT := dartuinoP0
DEFINES := BOARD_CUSTOM __HEAP_SIZE=0 DEBUG

MODULES := \
	src/platform/dartuino \

include make/nrf51-s130.mk
