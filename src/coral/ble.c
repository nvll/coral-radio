#include <stdio.h>
#include <stdint.h>
#include "ble_priv.h"
#include "nrf_soc.h"
#include "ble_dis.h"
#include "ble_gap.h"
#include "ble_advdata.h"
#include "ble_types.h"
#include "softdevice_handler.h"

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
#define CHECK_NRF_ERROR(r, s) \
    do { \
        if (r != NRF_SUCCESS) { \
            printf("Err: %s %s (%lu)\n", s, (r < COUNT_OF(nrf_err_tbl)) ? nrf_err_tbl[r] : "", r); \
            goto err; \
        } \
    } while (0); \


/* Contains the state of the ble service created for our rx/tx characteristics */
ble_service_t ble_coral_ipc;

uint8_t coral_ipc_rx_buf[256];
uint8_t coral_ipc_tx_buf[256];

uint8_t *coral_ipc_rx_desc = (uint8_t *)"ipc_rx";
uint8_t *coral_ipc_tx_desc = (uint8_t *)"ipc_tx";

char *nrf_err_tbl[] = {
    "NRF_SUCCESS",
    "NRF_ERROR_SVC_HANDLER_MISSING",
    "NRF_ERROR_SOFTDEVICE_NOT_ENABLED",
    "NRF_ERROR_INTERNAL",
    "NRF_ERROR_NO_MEMORY",
    "NRF_ERROR_NOT_FOUND",
    "NRF_ERROR_NOT_SUPPORTED",
    "NRF_ERROR_INVALID_PARAM",
    "NRF_ERROR_INVALID_STATE",
    "NRF_ERROR_INVALID LENGTH",
    "NRF_ERROR_INVALID_FLAGS",
    "NRF_ERROR_INVALID_DATA",
    "NRF_ERROR_DATA_SIZE",
    "NRF_ERROR_TIMEOUT",
    "NRF_ERROR_NULL",
    "NRF_ERROR_FORBIDDEN",
    "NRF_ERROR_INVALID_ADDR",
    "NRF_ERROR_BUSY"
};

void ble_event_handler(ble_evt_t *event)
{
    switch (event->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:
            printf("BLE connected to %02x:%02x:%02x:%02x:%02x:%02x\n",
                    BLE_ADDR_UNPACK(event->evt.gap_evt.params.connected.peer_addr.addr));
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            printf("BLE disconnected\n");
            ble_start_advertising();
            break;
        case BLE_GAP_EVT_CONN_PARAM_UPDATE:
            printf("BLE connection parameters updated\n");
            break;
        default:
            printf("unhandled BLE event id %02X len %d\n", event->header.evt_id, event->header.evt_len);
    }
}

void sys_event_handler(uint32_t event)
{
    printf("sys event %lu\n", event);
}

uint32_t ble_gap_setup(void)
{
    uint32_t err = NRF_SUCCESS;
    ble_gap_addr_t gap_addr = {0};
    ble_gap_conn_sec_mode_t sec_mode = {0};

    /* Set up a randomly generated GAP address */
    gap_addr.addr_type = BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE;
    err = sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_AUTO, &gap_addr);
    CHECK_NRF_ERROR(err, "Failed to setup GAP configuration");

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
    err = sd_ble_gap_device_name_set(&sec_mode, (const uint8_t *)BLE_DEVICE_NAME, strlen(BLE_DEVICE_NAME));
    CHECK_NRF_ERROR(err, "Failed to set BLE security settings and/or device name");

    err = sd_ble_gap_address_get(&gap_addr);
    CHECK_NRF_ERROR(err, "Failed to read GAP address");

    printf("GAP initialized as '%.*s' with address %02x:%02x:%02x:%02x:%02x:%02x\n",
            strlen(BLE_DEVICE_NAME), BLE_DEVICE_NAME, gap_addr.addr[5], gap_addr.addr[4],
            gap_addr.addr[3], gap_addr.addr[2], gap_addr.addr[1], gap_addr.addr[0]);

err:
    return err;
}

uint32_t ble_start_advertising(void)
{
    uint32_t err = NRF_SUCCESS;
    ble_advdata_t advertising_data = {0};
    ble_advdata_manuf_data_t manuf_data = {0};
    ble_gap_adv_params_t advertising_params = {0};

    manuf_data.company_identifier = BLE_COMPANY_ID;

    advertising_data.name_type             = BLE_ADVDATA_FULL_NAME;
    advertising_data.flags                 = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    advertising_data.p_manuf_specific_data = &manuf_data;

    err = ble_advdata_set(&advertising_data, NULL);
    CHECK_NRF_ERROR(err, "Failed to set advertising data");

    advertising_params.type         = BLE_GAP_ADV_TYPE_ADV_IND; // Indirect advertising, connectable
    advertising_params.fp           = BLE_GAP_ADV_FP_ANY; // Allow any connections
    advertising_params.interval     = BLE_GAP_ADV_INTERVAL_MIN; // 20 ms
    advertising_params.timeout      = BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED; // no timeout
    advertising_params.p_peer_addr  = NULL; // Only used for Directed IND advertising

    err = sd_ble_gap_adv_start(&advertising_params);
    CHECK_NRF_ERROR(err, "Failed to start advertising\n");

err:
    return err;
}

uint32_t create_coral_ipc_rx_char(void)
{
    uint32_t err = NRF_SUCCESS;
    ble_uuid_t char_uuid;
    ble_uuid128_t base_uuid = BLE_CORAL_BASE_UUID;
    ble_gatts_char_md_t char_md = {0};
    ble_gatts_attr_md_t attr_md = {0};
    ble_gatts_attr_t attr_char = {0};

    char_uuid.uuid = BLE_CORAL_IPC_RX_CHAR;
    err = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    CHECK_NRF_ERROR(err, "Couldn't add rx char uuid to softdevice table\n");

    // Store the attributes in our userspace buffer
    attr_md.vloc = BLE_GATTS_VLOC_USER;

    // Set up the characteristic values
    attr_char.p_uuid = &char_uuid;
    attr_char.p_attr_md = &attr_md;
    attr_char.max_len = sizeof(coral_ipc_rx_buf);
    attr_char.init_len = 0;
    attr_char.p_value = coral_ipc_rx_buf;

    char_md.char_props.read = 1;
    char_md.p_char_user_desc = coral_ipc_rx_desc;
    char_md.char_user_desc_size = strlen((char *)coral_ipc_rx_desc);
    char_md.char_user_desc_max_size = strlen((char *)coral_ipc_rx_desc);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);

    err = sd_ble_gatts_characteristic_add(ble_coral_ipc.service_handle, &char_md,
            &attr_char, &ble_coral_ipc.char_handles);
    CHECK_NRF_ERROR(err, "Couldn't add coral rx characteristic\n");

err:
    return err;
}

uint32_t create_coral_ipc_tx_char(void)
{
    uint32_t err = NRF_SUCCESS;
    ble_uuid_t char_uuid;
    ble_uuid128_t base_uuid = BLE_CORAL_BASE_UUID;
    ble_gatts_char_md_t char_md = {0};
    ble_gatts_attr_md_t attr_md = {0};
    ble_gatts_attr_t attr_char = {0};

    char_uuid.uuid = BLE_CORAL_IPC_TX_CHAR;
    err = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    CHECK_NRF_ERROR(err, "Couldn't add tx char uuid to softdevice table\n");

    // Store the attributes in our userspace buffer
    attr_md.vloc = BLE_GATTS_VLOC_USER;

    // Set up the characteristic values
    attr_char.p_uuid = &char_uuid;
    attr_char.p_attr_md = &attr_md;
    attr_char.max_len = sizeof(coral_ipc_tx_buf);
    attr_char.init_len = 0;
    attr_char.p_value = coral_ipc_tx_buf;

    char_md.char_props.write = 1;
    char_md.p_char_user_desc = coral_ipc_tx_desc;
    char_md.char_user_desc_size = strlen((char *)coral_ipc_tx_desc);
    char_md.char_user_desc_max_size = strlen((char *)coral_ipc_tx_desc);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    err = sd_ble_gatts_characteristic_add(ble_coral_ipc.service_handle, &char_md,
            &attr_char, &ble_coral_ipc.char_handles);
    CHECK_NRF_ERROR(err, "Couldn't add coral tx characteristic\n");

err:
    return err;
}

uint32_t ble_create_gatt_services(void)
{
    uint32_t err = NRF_SUCCESS;
    ble_uuid_t service_uuid;
    ble_uuid128_t base_uuid = BLE_CORAL_BASE_UUID;

    // Add the base uuid for the coral ipc ble service to the softdevice's table and
    // get an index back to it in coral_ipc_srvc.uuid_type
    err = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
    CHECK_NRF_ERROR(err, "Couldn't add serial service UUID to softdevice table\n");

    // Using the uuid_type we got back, add the uuid for the service itself, referring back
    // to the uuid type we were given before.
    service_uuid.uuid = BLE_CORAL_IPC_UUID;
    err = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
            &service_uuid, &ble_coral_ipc.service_handle);
    CHECK_NRF_ERROR(err, "Couldn't create coral ipc service profile\n");

    err = create_coral_ipc_rx_char();
    CHECK_NRF_ERROR(err, "Couldn't create coral ipc rx characteristic\n");

    err = create_coral_ipc_tx_char();
    CHECK_NRF_ERROR(err, "Couldn't create coral ipc tx characteristic\n");

    /* Nordic Device Information Service */
    ble_dis_init_t dis_init = {0};
    ble_srv_ascii_to_utf8(&dis_init.manufact_name_str, "Google");
    ble_srv_ascii_to_utf8(&dis_init.model_num_str, "coral");
    ble_srv_ascii_to_utf8(&dis_init.serial_num_str, "13579");
    ble_srv_ascii_to_utf8(&dis_init.hw_rev_str, "1.0");
    ble_srv_ascii_to_utf8(&dis_init.fw_rev_str, "1.1");
    ble_srv_ascii_to_utf8(&dis_init.sw_rev_str, "1.2");
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&dis_init.dis_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&dis_init.dis_attr_md.write_perm);

    err = ble_dis_init(&dis_init);
    CHECK_NRF_ERROR(err, "Failed to init Device Information Service\n");


err:
    return err;
}

uint32_t ble_stack_setup(void)
{
    uint32_t err = NRF_SUCCESS;

    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, NULL);
    err = softdevice_sys_evt_handler_set(&sys_event_handler);
    CHECK_NRF_ERROR(err, "Failed to set up SOC event handler");
    err = softdevice_ble_evt_handler_set(&ble_event_handler);
    CHECK_NRF_ERROR(err, "Failed to set up BLE event handler");

    ble_enable_params_t ble_enable_params = {0};
    ble_enable_params.gatts_enable_params.attr_tab_size = BLE_GATTS_ATTR_TAB_SIZE_DEFAULT;
    ble_enable_params.gatts_enable_params.service_changed = 0;

    err = sd_ble_enable(&ble_enable_params);
    CHECK_NRF_ERROR(err, "Failed to enable BLE stack");

    err = ble_gap_setup();
    CHECK_NRF_ERROR(err, "Failed  to initialize GAP\n");

    err = ble_create_gatt_services();
    CHECK_NRF_ERROR(err, "Failed to add initialize GATTS\n");

    ble_start_advertising();

err:

    return err;
}
