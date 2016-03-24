PWD := $(GET_PARENT)

MODULE_SOURCES += \
	$(PWD)/main.c \
	$(PWD)/spi.c \
	$(PWD)/uart.c


MODULES += \
	lib/nrf_drv/spi_master \
	lib/nrf_drv/delay \
	lib/nrf_drv/uart