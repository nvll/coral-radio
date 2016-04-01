MODULE_SOURCES += \
	third_party/nrf51_sdk/components/drivers_nrf/uart/nrf_drv_uart.c \
	third_party/nrf51_sdk/components/libraries/uart/app_uart_fifo.c \
	third_party/nrf51_sdk/components/libraries/fifo/app_fifo.c \
	src/nrf_common/retarget.c
	#third_party/nrf51_sdk/components/libraries/uart/app_uart.c \

MODULE_INCLUDES += \
	third_party/nrf51_sdk/components/drivers_nrf/uart \
	third_party/nrf51_sdk/components/drivers_nrf/hal \
	third_party/nrf51_sdk/components/libraries/uart \
	third_party/nrf51_sdk/components/libraries/fifo
