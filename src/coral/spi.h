// spi.h
#pragma once
#include <stdint.h>

uint32_t spi_init(void);
uint32_t spi_transfer(uint8_t *tx_buf, uint8_t tx_len, uint8_t *rx_buf, uint8_t rx_len);
