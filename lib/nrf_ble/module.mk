# This module pulls in everything necessary for the nRF provided
# services for BLE support. Anything unused is stripped by the
# linker.

MODULES += \
	lib/nrf_drv/pstorage

MODULE_SOURCES += \
	third_party/nrf51_sdk/components/ble/ble_advertising/ble_advertising.c \
	third_party/nrf51_sdk/components/ble/common/ble_advdata.c \
	third_party/nrf51_sdk/components/ble/common/ble_conn_params.c \
    third_party/nrf51_sdk/components/ble/common/ble_conn_state.c \
	third_party/nrf51_sdk/components/ble/common/ble_srv_common.c \
	third_party/nrf51_sdk/components/libraries/trace/app_trace.c \
	third_party/nrf51_sdk/components/libraries/timer/app_timer.c \
	third_party/nrf51_sdk/components/softdevice/common/softdevice_handler/softdevice_handler.c \

MODULE_INCLUDES += \
	third_party/nrf51_sdk/components/ble/ble_advertising \
	third_party/nrf51_sdk/components/ble/common \
	third_party/nrf51_sdk/components/libraries/trace \
	third_party/nrf51_sdk/components/libraries/timer \
