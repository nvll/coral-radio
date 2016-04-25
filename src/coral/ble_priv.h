#pragma once
#include "ble_gatts.h"
#include "ble_types.h"

// General BLE defines
#define BLE_DEVICE_NAME             "coralBLE"
#define BLE_COMPANY_ID              0x00E0 // Google
#define BLE_ADDR_UNPACK(arr)        arr[5], arr[4], arr[3], arr[2], arr[1], arr[0]

// Defines for BLE services we're creating
// "coralserialsrvc"
#define BLE_CORAL_BASE_UUID    {{0x63, 0x6f, 0x72, 0x61, 0x6c, 0x73, 0x65, 0x72, 0x69, 0x61, \
                               0x6c, 0x73, 0x72, 0x76, 0x63, 0x2e }}
#define BLE_CORAL_IPC_UUID     0xA001
#define BLE_CORAL_IPC_RX_CHAR  0xA002
#define BLE_CORAL_IPC_TX_CHAR  0xA003

typedef struct {
    uint16_t conn_handle;
    uint16_t service_handle;
    ble_gatts_char_handles_t char_handles;
} ble_service_t;

uint32_t ble_stack_setup(void);
uint32_t ble_start_advertising(void);
