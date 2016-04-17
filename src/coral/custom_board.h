#pragma once

// These LED definitions mostly matter for the BSP led bits, which we
// will likely not use long term
#define LEDS_NUMBER    0
#define LED_START      0
#define LED_STOP       0

#define LEDS_LIST { }
#define LEDS_MASK 0x00000000
#define LEDS_INV_MASK LEDS_MASK

// there are no user buttons, also used for the BSP bits
#define BUTTONS_NUMBER 0
#define BUTTONS_LIST {}
#define BUTTONS_MASK 0x00000000

// SPI Master pin configuration
#define SPI_MISO_PIN_NUMBER 8
#define SPI_MOSI_PIN_NUMBER 9
#define SPI_SCLK_PIN_NUMBER 10
#define SPI_SS_PIN_NUMBER	11
#define SPI_INT_PIN_NUMBER	12

// UART connection with J-Link
#define RX_PIN_NUMBER  15
#define TX_PIN_NUMBER  14
#define CTS_PIN_NUMBER 0
#define RTS_PIN_NUMBER 0
#define UART_HWFC      false
#define UART_TX_BUF    512
#define UART_RX_BUF    64

// Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      NRF_CLOCK_LFCLKSRC_XTAL_20_PPM

#define DEVICE_NAME "dartuino proto1"
