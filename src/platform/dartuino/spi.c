#include <app_util_platform.h>
#include "nrf_drv_spi.h"
#include <stdio.h>
#include "custom_board.h"

// For dartuino we're only using the primary spi bus
const nrf_drv_spi_t spi_master_0 = NRF_DRV_SPI_INSTANCE(0);

void spi_event_handler(nrf_drv_spi_event_t event) {
    // NOT IMPLEMENTED
}

/* The EasyDMA controller the nRF51 uses supports only 8bit payload lengths,
 * but does have an ability to chain. As we move forward it may end up being
 * necessary to use scatter / gather transfers based on packets. Fortunately,
 * our use case is generally low throughput due to BLE so it may not be an
 * issue. */
uint32_t spi_transfer(uint8_t *tx_buf, uint8_t tx_len, uint8_t *rx_buf, uint8_t rx_len)
{
    return nrf_drv_spi_transfer(&spi_master_0, tx_buf, tx_len, rx_buf, rx_len);
}

uint32_t spi_init(void) 
{
    nrf_drv_spi_config_t config =
    {
        .ss_pin       = SPI_SS_PIN_NUMBER,
        .sck_pin      = SPI_SCLK_PIN_NUMBER,
        .mosi_pin     = SPI_MOSI_PIN_NUMBER,
        .miso_pin     = SPI_MISO_PIN_NUMBER,
        .irq_priority = APP_IRQ_PRIORITY_LOW,
        .frequency    = NRF_DRV_SPI_FREQ_1M,
        .mode         = NRF_DRV_SPI_MODE_0,
        .bit_order    = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST,
        .orc          = 0xFF,
    };

    return nrf_drv_spi_init(&spi_master_0, &config, spi_event_handler);
}
