#include <unistd.h>
#include "platform/platform-uart.hh"
#include "py/mpconfig.h"

/*
 * Core UART functions to implement for a port
 */

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif
