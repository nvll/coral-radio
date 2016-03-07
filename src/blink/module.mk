PWD := $(GET_PARENT)

MODULE_SOURCES += \
	$(PWD)/main.c

MODULES += \
	lib/nrf_drv/delay \
	lib/nrf_drv/hal \
	lib/nrf_drv/uart
