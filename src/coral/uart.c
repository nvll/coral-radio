#include "app_uart.h"
#include "uart.h"
#include <stddef.h>
#include "boards.h"

/* This file contains the minimum setup to get the UART running on an nRF51. */

#ifndef UART_TX_BUF
#define UART_TX_BUF 512
#endif

#ifndef UART_RX_BUF
#define UART_RX_BUF 512
#endif

void uart_error_handle(app_uart_evt_t * p_event)
{
	// TODO: See what errors we can try to recover from.
}

uint32_t uart_init(void) {
	uint32_t err_code;
	const app_uart_comm_params_t comm_params = {
		.rx_pin_no = RX_PIN_NUMBER,
		.tx_pin_no = TX_PIN_NUMBER,
		.rts_pin_no = 0,
		.cts_pin_no = 0,
		.flow_control = APP_UART_FLOW_CONTROL_DISABLED,
		.use_parity = false,
		.baud_rate = UART_BAUDRATE_BAUDRATE_Baud38400
	};

    /* With the softdevice interrupts happening we need to use a UART fifo to ensure we don't lose
     * bytes. Additionally, the behavior seems unpredictable at 115200 baud, and after reading
     * responses from nRF employees the safer plan seems to be to use a lower baud rate */
	APP_UART_FIFO_INIT(&comm_params, UART_TX_BUF, UART_RX_BUF, uart_error_handle, APP_IRQ_PRIORITY_LOW, err_code);
	return err_code;
}
