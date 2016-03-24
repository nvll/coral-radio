#include <stdio.h>
#include "nrf_delay.h"
#include "spi.h"
#include "uart.h"
#include "sdk_errors.h"

#if defined(BOARD_DARTUINOP0)
#include "dartuino-p0.h"
#endif

uint8_t tx_buf[255];
uint8_t rx_buf[8];

int main(void)
{
	uint32_t ret;

	// TODO: Add LED beep codes if the UART isn't working
	uart_init();

	if ((ret = spi_init()) != NRF_SUCCESS) {
		printf("spi_init() failure: %lu\n", ret);
	}
	
 	printf("\nwelcome to %s: %s %s.\n", DEVICE_NAME, __TIME__, __DATE__);
 	for (int i = 0; i < sizeof(tx_buf); i++) {
 		tx_buf[i] = i;
 	}

 	while (1) {
 		printf("tick. ");
 		fflush(stdout);
 		nrf_delay_ms(1000);
 		printf("tock. \n");
 		nrf_delay_ms(1000);

 		uint32_t ret = spi_transfer(tx_buf, sizeof(tx_buf), NULL, 0);
 		if (ret != NRF_SUCCESS) {
 			printf("SPI Error: %lu\n", ret);
 		}
 	}
 	return 0;
 }
