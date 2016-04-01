#include <stdio.h>
#include "nrf_delay.h"
#include "spi.h"
#include "uart.h"
#include "sdk_errors.h"
#include "nordic_common.h"
#include "ble_priv.h"
#include "app_trace.h"
#include "custom_board.h"


uint8_t tx_buf[255];
uint8_t rx_buf[8];

// Default app error handler used by all the nRF libraries / drivers. Overrides their weakref.
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
    printf("APP err %lu at %s:%lu\n", error_code, p_file_name, line_num);
}


int main(void)
{
	uint32_t ret;

	uart_init();
    //app_trace_init();

	if ((ret = spi_init()) != NRF_SUCCESS) {
		printf("spi_init() failure: %lu\n", ret);
	}

 	printf("\nwelcome to %s: %s %s.\n", DEVICE_NAME, __TIME__, __DATE__);
 	for (int i = 0; i < sizeof(tx_buf); i++) {
 		tx_buf[i] = i;
 	}

    ble_stack_setup();
 	while (1) {
 		nrf_delay_ms(1000);

 		uint32_t ret = spi_transfer(tx_buf, sizeof(tx_buf), NULL, 0);
 		if (ret != NRF_SUCCESS) {
 			printf("SPI Error: %lu\n", ret);
 		}
 	}

 	return 0;
 }
