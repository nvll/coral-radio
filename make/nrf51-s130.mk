# This configuration is for NRF51 chips. The nordic libraries
# tend to assume some toolchain embedded .a files are present,
# and rather than having AS/LD find them they resort to having
# gcc due to linking. For that reason gcc is used for both tasks.
# Additionally, this file is being used to automatically handle the
# board support / common driver includes and linker script side of
# building for the platform.

MODULES += lib/s130

include make/nrf51-nosd.mk
