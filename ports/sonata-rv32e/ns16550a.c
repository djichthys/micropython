#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "ns16550a.h"

static volatile ns16550a_uart_t *uart_dev;

int ns16550a_uart_init(void *uart_addr)
{
  uart_dev = (volatile ns16550a_uart_t *)uart_addr;
  uart_dev->lcr = (uart_dev->lcr | LCR_WordLength_8bit); 

  uart_dev->fcr = FCR_FifoEnable;   /* should be set before Fifos can be reset */
  uart_dev->fcr = (FCR_TXFifoReset | FCR_RXFifoReset | FCR_FifoEnable); 

  uart_dev->intr_en = (uart_dev->intr_en | IER_DataReady);

  return 0;
}

int ns16550a_uart_getc(bool blocking)
{
  bool ready;
  int bytes = 0; 

  do {
    ready = uart_dev->lsr & LSR_DataReady; 
  } while (blocking && !ready);

  if (!ready) { 
    bytes = EOF; 
    goto err_getc_ns16550a;
  }

  bytes = (int)uart_dev->rx; 

err_getc_ns16550a:
  return bytes; 
}

int ns16550a_uart_putc(int byte)
{
  uart_dev->tx = (uint8_t)byte;
  return byte;
}
