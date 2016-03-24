#include "app_uart.h"
#include "uart.h"
#include <stddef.h>

// TODO: When p1 exists it makes sense to have a toplevel dartuino.h that figures this out for us.
#if defined(BOARD_DARTUINOP0)
#include "dartuino-p0.h"
#endif

/* This file contains the minimum setup to get the UART running on an nRF51. */

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
		.baud_rate = UART_BAUDRATE_BAUDRATE_Baud115200
	};

	APP_UART_INIT(&comm_params, uart_error_handle, APP_IRQ_PRIORITY_LOW, err_code);
	return err_code;
}