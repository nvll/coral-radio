PWD := $(GET_PARENT)

MODULE_SOURCES += \
	$(PWD)/ble.c \
	$(PWD)/main.c \
	$(PWD)/spi.c \
	$(PWD)/uart.c \
	third_party/nrf51_sdk/components/ble/ble_services/ble_dis/ble_dis.c

MODULE_INCLUDES += \
	third_party/nrf51_sdk/components/ble/ble_services/ble_dis


MODULES += \
	lib/nrf_drv/spi_master \
	lib/nrf_drv/delay \
	lib/nrf_drv/uart \
	lib/nrf_ble
