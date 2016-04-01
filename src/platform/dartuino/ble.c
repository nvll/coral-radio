#include <stdio.h>
#include <stdint.h>
#include "ble_priv.h"
#include "nrf_soc.h"
#include "ble_dis.h"
#include "ble_gap.h"
#include "ble_advdata.h"
#include "softdevice_handler.h"

#define CHECK_BLE_ERROR(r, s) \
    do { \
        if (r != NRF_SUCCESS) { \
            printf("BLE ERR %lu: %s\n", r, s); \
            goto err; \
        } \
    } while (0); \

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
    CHECK_BLE_ERROR(err, "Failed to setup GAP configuration");

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
    err = sd_ble_gap_device_name_set(&sec_mode, (const uint8_t *)BLE_DEVICE_NAME, strlen(BLE_DEVICE_NAME));
    CHECK_BLE_ERROR(err, "Failed to set BLE security settings and/or device name");

    err = sd_ble_gap_address_get(&gap_addr);
    CHECK_BLE_ERROR(err, "Failed to read GAP address");

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
    CHECK_BLE_ERROR(err, "Failed to set advertising data");

    advertising_params.type         = BLE_GAP_ADV_TYPE_ADV_IND; // Indirect advertising, connectable
    advertising_params.fp           = BLE_GAP_ADV_FP_ANY; // Allow any connections
    advertising_params.interval     = BLE_GAP_ADV_INTERVAL_MIN; // 20 ms
    advertising_params.timeout      = BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED; // no timeout
    advertising_params.p_peer_addr  = NULL; // Only used for Directed IND advertising

    err = sd_ble_gap_adv_start(&advertising_params);
    CHECK_BLE_ERROR(err, "Failed to start advertising\n");

err:
    return err;
}

uint32_t ble_stack_setup(void)
{
    uint32_t err = NRF_SUCCESS;

    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, NULL);
    err = softdevice_sys_evt_handler_set(&sys_event_handler);
    CHECK_BLE_ERROR(err, "Failed to set up SOC event handler");
    err = softdevice_ble_evt_handler_set(&ble_event_handler);
    CHECK_BLE_ERROR(err, "Failed to set up BLE event handler");

    ble_enable_params_t ble_enable_params = {0};
    ble_enable_params.gatts_enable_params.attr_tab_size = BLE_GATTS_ATTR_TAB_SIZE_DEFAULT;
    ble_enable_params.gatts_enable_params.service_changed = 0;

    err = sd_ble_enable(&ble_enable_params);
    CHECK_BLE_ERROR(err, "Failed to enable BLE stack");

    ble_gap_setup();
    ble_dis_init_t dis_init = {0};

#define ASSIGN_NRF_BLE_STR(x, s) x.p_str = (uint8_t *)s; x.length = strlen(s);
    char *dis_name = "GoogleFNLK!";
    char *dis_model = "123";
    char *dis_serial = "987654";
    char *dis_hwrev = "1.0";
    char *dis_fwrev = "1.1";
    char *dis_swrev = "1.2";
    ASSIGN_NRF_BLE_STR(dis_init.manufact_name_str, dis_name);
    ASSIGN_NRF_BLE_STR(dis_init.model_num_str, dis_model);
    ASSIGN_NRF_BLE_STR(dis_init.serial_num_str, dis_serial);
    ASSIGN_NRF_BLE_STR(dis_init.hw_rev_str, dis_hwrev);
    ASSIGN_NRF_BLE_STR(dis_init.fw_rev_str, dis_fwrev);
    ASSIGN_NRF_BLE_STR(dis_init.sw_rev_str, dis_swrev);
    err = ble_dis_init(&dis_init);
    CHECK_BLE_ERROR(err, "Failed to init DIS\n");
    ble_start_advertising();

err:

    return err;
}
