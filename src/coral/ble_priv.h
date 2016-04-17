#pragma once

#define BLE_ADDR_UNPACK(arr) arr[5], arr[4], arr[3], arr[2], arr[1], arr[0]

#define BLE_DEVICE_NAME "coralNRF"
#define BLE_COMPANY_ID   0x00E0

uint32_t ble_stack_setup(void);
uint32_t ble_start_advertising(void);
