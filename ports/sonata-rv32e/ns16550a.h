/*! 
 * UART control block 
 * memory mapped @ 0x1000_0000
 * -----------------------------------------
 * |   LCR   | FCR/IIR |   IER   | RBR/TBR | 
 * -----------------------------------------
 * |         |         |   LSR   |   MCR   | 
 * -----------------------------------------
 * details from [url](https://opensocdebug.readthedocs.io/en/latest/02_spec/07_modules/dem_uart/uartspec.html)
 */


#ifdef __cplusplus
extern "C" {
#endif

typedef struct NS16550A_UART
{
  union {
    uint8_t tx;     /* TX buffer (W) */
    uint8_t rx;     /* RX buffer (R) */
  };
  uint8_t intr_en;  /* (R/W) */
  union {
    uint8_t isr;    /* Intr Status Register (R) */
    uint8_t fcr;    /* FIFO Control (W) */
  };
  uint8_t lcr;      /* Line Control (R/W) */
  uint8_t mcr;     /* Modem Control (R/W) */
  uint8_t lsr;     /* Line STATUS (R) */
} ns16550a_uart_t; 

#define IER_DataReady       ((uint8_t)0x01)
#define LCR_WordLength_8bit ((uint8_t)0x03)

#define FCR_FifoDisable     ((uint8_t)0x00)
#define FCR_FifoEnable      ((uint8_t)0x01)
#define FCR_RXFifoReset     ((uint8_t)0x02)
#define FCR_TXFifoReset     ((uint8_t)0x04)

#define LSR_DataReady       ((uint8_t)0x01)

int ns16550a_uart_init(void *);
int ns16550a_uart_getc(bool);
int ns16550a_uart_putc(int);

#ifdef __cplusplus
}
#endif
