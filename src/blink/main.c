#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include "nrf_delay.h"
#include "app_uart.h"
#include "app_error.h"
#include "nrf_gpio.h"

#if defined(BOARD_PCA10031)
#include "pca10031.h"
#elif defined(BOARD_PCA10028)
#include "pca10028.h"
#elif defined(BOARD_DARTUINOP0)
#include "dartuino-p0.h"
#endif

// Defines came from boards.h
#define LEDS_OFF(leds_mask) \
do { \
    NRF_GPIO->OUTSET = (leds_mask) & (LEDS_MASK & LEDS_INV_MASK); \
    NRF_GPIO->OUTCLR = (leds_mask) & (LEDS_MASK & ~LEDS_INV_MASK); \
} while (0)

#define LEDS_ON(leds_mask) \
do { \
    NRF_GPIO->OUTCLR = (leds_mask) & (LEDS_MASK & LEDS_INV_MASK); \
    NRF_GPIO->OUTSET = (leds_mask) & (LEDS_MASK & ~LEDS_INV_MASK); \
} while (0)

#define LED_IS_ON(leds_mask) ((leds_mask) & (NRF_GPIO->OUT ^ LEDS_INV_MASK) )

#define LEDS_INVERT(leds_mask) \
do { \
    uint32_t gpio_state = NRF_GPIO->OUT;      \
    NRF_GPIO->OUTSET = ((leds_mask) & ~gpio_state); \
    NRF_GPIO->OUTCLR = ((leds_mask) & gpio_state); \
} while (0)

#define LEDS_CONFIGURE(leds_mask) \
do { \
    uint32_t pin; \
    for (pin = 0; pin < 32; pin++) { \
        if ( (leds_mask) & (1 << pin) ) { \
            nrf_gpio_cfg_output(pin); \
        } \
    } \
} while (0)

const uint8_t leds_list[LEDS_NUMBER] = LEDS_LIST;

/**
 * @brief Function for application main entry.
 */

void uart_error_handle(app_uart_evt_t * p_event)
{
}

int main(void)
{
	uint8_t err_code;
	(void) err_code;
	// Configure LED-pins as outputs.
	LEDS_CONFIGURE(LEDS_MASK);
	const app_uart_comm_params_t comm_params = {
		RX_PIN_NUMBER,
		TX_PIN_NUMBER,
		0,
		0,
		APP_UART_FLOW_CONTROL_DISABLED,
		false,
		UART_BAUDRATE_BAUDRATE_Baud115200
	};

	APP_UART_INIT(&comm_params,
	              uart_error_handle,
	              APP_IRQ_PRIORITY_LOW,
	              err_code);

    printf("Image booted\n");

	// Toggle LEDs.
	while (true) {
        if (LEDS_NUMBER > 0) {
            for (int i = 0; i < LEDS_NUMBER; i++) {
                LEDS_INVERT(1 << leds_list[i]);
                printf("blink led %i\n", i);
                nrf_delay_ms(500);
            }
        } else {
			printf("blink.\n");
			nrf_delay_ms(500);
        }
	}
}


/** @} */
