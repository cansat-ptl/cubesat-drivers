/*
 * uart.h
 * 
 * Created: 06.04.2021 05:47:48
 * Author: ThePetrovich
 */


#ifndef YKTS_DRIVERS_UART_H_
#define YKTS_DRIVERS_UART_H_

#include <stdint.h>
#include <drivers/arch/mega128/types.h>

#ifndef F_CPU
    #warning "F_CPU is not defined"
    #define F_CPU 8000000UL
#endif

#ifndef UART_BAUD_PRESCALE
    #define UART_BAUD_PRESCALE(x) (((F_CPU / ((uint32_t)x * 16UL))) - 1)
#else
    #warning "UART_BAUD_PRESCALE already defined"
#endif

#define UART_RX_BUFFER_SIZE 32
#define UART_RX_BUFFER_MASK (UART_RX_BUFFER_SIZE - 1)

#define UART_TX_BUFFER_SIZE 32
#define UART_TX_BUFFER_MASK (UART_TX_BUFFER_SIZE - 1)

typedef enum {UART0, UART1} drvUartEnum_t;

typedef struct drvUartStruct_t 
{
    drvBusType_t type;
    drvUartEnum_t bus;

    drvRegister_t *udr;
    drvRegister_t *ucsra;
    drvRegister_t *ucsrb;
    drvRegister_t *ucsrc;
    drvRegister_t *ubrrh;
    drvRegister_t *ubrrl;

    char buf_rx[UART_RX_BUFFER_SIZE];
    uint8_t buf_rx_head;
    uint8_t buf_rx_tail;

    uint8_t flags;
} drvUart_t;

extern drvUart_t busUart0;
extern drvUart_t busUart1;

void uart_init(drvUart_t *bus, drvUartEnum_t uartSelect, uint16_t flags, uint32_t baudrate);

void uart_putc(drvUart_t *bus, char c);
void uart_puts(drvUart_t *bus, char *msg);

char uart_getc(drvUart_t *bus);
char uart_peek(drvUart_t *bus);
int uart_available(drvUart_t *bus);

void uart_flushRX(drvUart_t *bus);

#endif /* YKTS_DRIVERS_UART_H_ */