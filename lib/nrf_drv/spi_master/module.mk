MODULES += \
	lib/nrf_drv/hal \

MODULE_SOURCES += \
	third_party/nrf51_sdk/components/drivers_nrf/spi_master/nrf_drv_spi.c \

MODULE_INCLUDES += \
	third_party/nrf51_sdk/components/drivers_nrf/spi_master/ \
