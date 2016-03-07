#pragma once

// LEDs definitions for PCA10031
#define LEDS_NUMBER    0

#define LED_START      21
#define LED_STOP       23

#define LEDS_LIST { }
// defining RGB led as 3 single LEDs
#define BSP_LED_0 LED_RGB_RED
#define BSP_LED_1 LED_RGB_GREEN
#define BSP_LED_2 LED_RGB_BLUE

#define LEDS_MASK 0x00000000
//defines which LEDs are lit when signal is low
#define LEDS_INV_MASK LEDS_MASK

// there are no user buttons
#define BUTTONS_NUMBER 0
#define BUTTONS_LIST {}
#define BUTTONS_MASK 0x00000000

// UART connection with J-Link
#define RX_PIN_NUMBER  15
#define TX_PIN_NUMBER  14
#define CTS_PIN_NUMBER 10
#define RTS_PIN_NUMBER 8
#define HWFC           true

// Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      NRF_CLOCK_LFCLKSRC_XTAL_20_PPM
