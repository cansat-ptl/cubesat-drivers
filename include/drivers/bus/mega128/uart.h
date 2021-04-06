/*
 * uart.h
 * 
 * Created: 06.04.2021 05:47:48
 * Author: ThePetrovich
 */


#ifndef KDRIVER_UART_H_
#define KDRIVER_UART_H_

#include <string.h>
#include <stdarg.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#warning "F_CPU is not defined for drivers/bus/mega128/uart.h"
#define F_CPU 8000000UL
#endif

#define BUS_UART_TX0BUSY 0
#define BUS_UART_TX1BUSY 0

#define BUS_UART_BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

/*
 * UART0 setup - uart0_init(unsigned int ubrr)
 * Sets up UART0 registers
 * Arguments: unsigned int ubrr - UBRR value, calculated using
 * 		    formula in drivers/uart.h. Defines UART0 baud rate.
 * Returns: int - 0 (additional checks may be added later)
 */
int uart0_init(unsigned int ubrr);

/*
 * UART0 interrupted TX start - uart0_transmit(unsigned int ubrr)
 * Transmits data stored in tx0_buffer using uart0 interrupts.
 * Data should be put in buffer BEFORE the transmission starts.
 * Arguments: none
 * Returns: nothing
 */
void uart0_transmit();

/*
 * UART0 uninterrupted character TX - uart0_putc(char c)
 * Transmits a single character through uart0. Does not require
 * interrupts.
 * Arguments: char c - character to be sent
 * Returns: nothing
 */
void uart0_putc(char c);

/*
 * UART0 uninterrupted string TX - uart0_puts(char * msg)
 * Transmits a string through uart0. Does not require
 * interrupts.
 * Arguments: char * msg - string pointer
 * Returns: nothing
 */
void uart0_puts(char *msg);

/*
 * UART0 custom printf - uart0_printf(char * format, ...)
 * prints a formatted string to uart0 without stdio.
 * Arguments: char * format - same as printf's format, but does
 * 			not support floats.
 * 		   arg1, arg2, ... - values, as specified in format.
 * Returns: nothing
 */
void uart0_printf(char *format, ...);
/*
 * UART0 receive buffer flush - rx0_buffer_flush()
 * Clears the uart0 receive buffer (rx0_buffer).
 * Arguments: none
 * Returns: nothing
 */
void rx0_buffer_flush();

/*
 * UART0 transmit buffer flush - tx0_buffer_flush()
 * Clears the uart0 transmit buffer (rx0_buffer).
 * Arguments: none
 * Returns: nothing
 */
void tx0_buffer_flush();

/*
 * UART1 setup - uart1_init(unsigned int ubrr)
 * Sets up UART1 registers
 * Arguments: unsigned int ubrr - UBRR value, calculated using
 * 		    formula in drivers/uart.h. Defines UART0 baud rate.
 * Returns: int - 0 (additional checks may be added later)
 */
int uart1_init(unsigned int ubrr);

/*
 * UART1 interrupted TX start - uart1_transmit(unsigned int ubrr)
 * Transmits data stored in tx1_buffer using uart1 interrupts.
 * Data should be put in buffer BEFORE the transmission starts.
 * Arguments: none
 * Returns: nothing
 */
void uart1_transmit();

/*
 * UART1 receive buffer flush - rx1_buffer_flush()
 * Clears the uart1 receive buffer (rx1_buffer).
 * Arguments: none
 * Returns: nothing
 */
void rx1_buffer_flush();

/*
 * UART1 transmit buffer flush - tx1_buffer_flush()
 * Clears the uart1 transmit buffer (rx1_buffer).
 * Arguments: none
 * Returns: nothing
 */
void tx1_buffer_flush();

#endif /* UART_H_ */