#include <stdio.h>
#include <stddef.h>
#include "nrf_delay.h"
#include "spi.h"
#include "app_uart.h"
#include "sdk_errors.h"
#include "nordic_common.h"
#include "ble_priv.h"
#include "app_trace.h"
#include "boards.h"
#include "uart.h"

uint8_t tx_buf[255];
uint8_t rx_buf[8];

// Default app error handler used by all the nRF libraries / drivers. Overrides their weakref.
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
  printf("APP err %lu at %s:%lu\n", error_code, p_file_name, line_num);
}

void uart_error_handle(app_uart_evt_t * p_event)
{
  // TODO: See what errors we can try to recover from.
}


/* This file contains the minimum setup to get the UART running on an nRF51. */

#ifndef UART_TX_BUF
#define UART_TX_BUF 512
#endif

#ifndef UART_RX_BUF
#define UART_RX_BUF 512
#endif

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

int main(void)
{
  uint32_t ret;

  uart_init();

  puts("welcome to coral.");
  for (int i = 0; i < sizeof(tx_buf); i++) {
    tx_buf[i] = i;
  }

  ble_stack_setup();
  if ((ret = spi_init()) != NRF_SUCCESS) {
    printf("spi_init() failure: %lu\n", ret);
  } 

  while (1) {
    nrf_delay_ms(1000);

    uint32_t ret = spi_transfer(tx_buf, sizeof(tx_buf), NULL, 0);
    if (ret != NRF_SUCCESS) {
      printf("SPI Error: %lu\n", ret);
      break;
    }
  }

  for(;;)
 
  return 0;
}
