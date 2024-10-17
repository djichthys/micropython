#include <unistd.h>
#include "py/mpconfig.h"

#ifdef QEMU_DEBUG
# include "ns16550a.h"
#else 
# include "common/defs.h"
# include "platform/platform-uart.hh"
#endif

/*
 * Core UART functions to implement for a port
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef QEMU_DEBUG
// Receive single character
int mp_hal_stdin_rx_chr(void)
{
    volatile OpenTitanUart *uart = reinterpret_cast<volatile OpenTitanUart *>(UART_ADDRESS);
    return uart->blocking_read(); 
}

// Send string of given length
mp_uint_t mp_hal_stdout_tx_strn(const char *str, mp_uint_t len)
{
    volatile OpenTitanUart *uart = reinterpret_cast<volatile OpenTitanUart *>(UART_ADDRESS);
    mp_uint_t ret = len;

    while (len--)
        uart->blocking_write(*str++); 

    return ret;
}

void uart_boot() 
{
    volatile OpenTitanUart *uart = reinterpret_cast<volatile OpenTitanUart *>(UART_ADDRESS);
    uart->init(BAUD_RATE); 
}

#else /* ! QEMU_DEBUG */

// Receive single character
int mp_hal_stdin_rx_chr(void)
{
    return ns16550a_uart_getc(true);
}

// Send string of given length
mp_uint_t mp_hal_stdout_tx_strn(const char *str, mp_uint_t len)
{
    mp_uint_t ret = len;

    while (len--)
        ns16550a_uart_putc(*str++);
    return ret;
}

void uart_boot() 
{
    ns16550a_uart_init((void *)QEMU_UART_ADDRESS); 
}

#endif /* QEMU_DEBUG */
#ifdef __cplusplus
}
#endif
