/*
 * uart.c
 * 
 * Created: 06.04.2021 06:02:26
 * Author: ThePetrovich
 */


#include <drivers/arch/mega128/types.h>
#include <drivers/arch/mega128/uart.h>
#include <drivers/arch/mega128/arch.h>
#include <string.h>
#include <avr/iom128.h>
#include <avr/io.h>
#include <avr/interrupt.h>

drvUart_t busUart0;
drvUart_t busUart1;

void uart_init(drvUart_t *bus, drvUartEnum_t uartSelect, uint16_t flags, uint32_t baudrate)
{
	drvRegister_t ucsra = 0;
	drvRegister_t ucsrb = 0;
	drvRegister_t ucsrc = 0;

	/* Baud rate register variables */
	drvRegister_t ubrrh = 0;
	drvRegister_t ubrrl = 0;
	uint32_t ubrr = UART_BAUD_PRESCALE(baudrate);

	/* UART selection & bus struct init */
	switch (uartSelect) {
	case UART0:
		bus->ubrrh = (drvRegister_t *)(&UBRR0H);
		bus->ubrrl = (drvRegister_t *)(&UBRR0L);
		bus->ucsra = (drvRegister_t *)(&UCSR0A);
		bus->ucsrb = (drvRegister_t *)(&UCSR0B);
		bus->ucsrc = (drvRegister_t *)(&UCSR0C);
		bus->udr = (drvRegister_t *)(&UDR0);
		break;
	case UART1:
		bus->ubrrh = (drvRegister_t *)(&UBRR1H);
		bus->ubrrl = (drvRegister_t *)(&UBRR1L);
		bus->ucsra = (drvRegister_t *)(&UCSR1A);
		bus->ucsrb = (drvRegister_t *)(&UCSR1B);
		bus->ucsrc = (drvRegister_t *)(&UCSR1C);
		bus->udr = (drvRegister_t *)(&UDR1);
		break;
	default:
		/* TODO: bugcheck */
		return;
		break;
	}

	bus->type = BUS_UART;
	bus->bus = uartSelect;

	arch_enterCriticalSection();
	bus->buf_rx_head = 0;
	bus->buf_rx_tail = 0;
	arch_exitCriticalSection();

	/* Splitting UBRR into UBRRH & UBRRL */
	ubrrl = (drvRegister_t)(ubrr & (uint32_t) 0xFF); /* Lowest 8 bits */
	ubrr >>= 8;
	ubrrh = (drvRegister_t)(ubrr & (uint32_t) 0xFF); /* Upper 8 bits (out of 16) */

	/* UART setup: RXEN & TXEN = 1, RX interrupt enabled, 8 bits, 1 stop bit, no parity - default */
	ucsra = 0;
	ucsrb = (drvRegister_t)((1 << RXEN) | (1 << TXEN) | (1 << RXCIE) | (0 << TXCIE) | (0 << UDRIE)); /* TODO: flags & modes */
	ucsrc = (drvRegister_t)((0 << USBS) | (1 << UCSZ00) | (1 << UCSZ01) | (0 << UCSZ02) | (0 << UPM00) | (0 << UPM01) | (0 << UMSEL0));

	/* Writing registers */
	*(bus->ubrrh) = ubrrh;
	*(bus->ubrrl) = ubrrl;
	*(bus->ucsra) = ucsra;
	*(bus->ucsrc) = ucsrc;
	*(bus->ucsrb) = ucsrb;
}

void uart_putc(drvUart_t *bus, char c)
{
	/* Send char & wait */
	while (!((*(bus->ucsra)) & (1 << UDRE)));
	*(bus->udr) = c;
}

void uart_puts(drvUart_t *bus, char *msg)
{
	while (*msg) {
		uart_putc(bus, *msg++);
	}
}

char uart_getc(drvUart_t *bus)
{
	uint8_t rx_tail = 0;
	char data = '\0';

	arch_enterCriticalSection();
	if (bus->buf_rx_head == bus->buf_rx_tail) {
		return '\0';   /* RX buffer empty */
	}
	arch_exitCriticalSection();

	/* Calculate buffer index, wrap on overflow */
	rx_tail = (bus->buf_rx_tail + 1) & UART_RX_BUFFER_MASK;
	bus->buf_rx_tail = rx_tail;

	/* Get char from RX buffer */
	data = bus->buf_rx[rx_tail];

	return data;
}

char uart_peek(drvUart_t *bus)
{
	uint8_t rx_tail = 0;
	char data = '\0';

	arch_enterCriticalSection();
	if (bus->buf_rx_head == bus->buf_rx_tail) {
		return '\0';   /* RX buffer empty */
	}
	arch_exitCriticalSection();

	/* Calculate buffer index, wrap on overflow */
	rx_tail = (bus->buf_rx_tail + 1) & UART_RX_BUFFER_MASK;
	/* Proceed without updating bus->buf_rx_tail */

	/* Get char from RX buffer */
	data = bus->buf_rx[rx_tail];

	return data;
}

int uart_available(drvUart_t *bus)
{
	int available = 0;

	arch_enterCriticalSection();
	available = (UART_RX_BUFFER_SIZE + bus->buf_rx_head - bus->buf_rx_tail) & UART_RX_BUFFER_MASK;
	arch_exitCriticalSection();

	return available;
}

void uart_flushRX(drvUart_t *bus)
{
	bus->buf_rx_head = bus->buf_rx_tail;
}

ISR(USART0_RX_vect)
{
	char data = '\0';
	uint8_t rx_head = 0;

	data = *(busUart0.udr);

	/* Calculate buffer index, wrap on overflow */
	rx_head = (busUart0.buf_rx_head + 1) & UART_RX_BUFFER_MASK;

	/* Store data if buffer is not full */
	if (rx_head != busUart0.buf_rx_tail) {
		busUart0.buf_rx_head = rx_head;
		busUart0.buf_rx[rx_head] = data;
	}
}

ISR(USART1_RX_vect)
{
	char data = '\0';
	uint8_t rx_head = 0;

	data = *(busUart1.udr);

	/* Calculate buffer index, wrap on overflow */
	rx_head = (busUart1.buf_rx_head + 1) & UART_RX_BUFFER_MASK;

	/* Store data if buffer is not full */
	if (rx_head != busUart1.buf_rx_tail) {
		busUart1.buf_rx_head = rx_head;
		busUart1.buf_rx[rx_head] = data;
	}
}