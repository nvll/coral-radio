OUTPUT := coral-dongle-dev
DEFINES := BOARD_PCA10031 __HEAP_SIZE=0 DEBUG

MODULES := \
	src/coral \

include make/nrf51822-s130.mk
