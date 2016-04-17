# This module pulls in everything necessary for the nRF provided
# services for BLE support. Anything unused is stripped by the
# linker.

MODULE_SOURCES += \
	third_party/nrf51_sdk/components/ble/common/ble_advdata.c \
	third_party/nrf51_sdk/components/ble/common/ble_conn_params.c \
    	third_party/nrf51_sdk/components/ble/common/ble_conn_state.c \
	third_party/nrf51_sdk/components/ble/common/ble_srv_common.c \
	third_party/nrf51_sdk/components/softdevice/common/softdevice_handler/softdevice_handler.c \

MODULE_INCLUDES += \
	third_party/nrf51_sdk/components/ble/common \
