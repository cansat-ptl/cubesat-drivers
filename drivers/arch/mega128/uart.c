/*
 * uart.c
 * 
 * Created: 06.04.2021 06:02:26
 * Author: ThePetrovich
 */


#ifdef KERNEL_ARCH_avr
#ifdef KERNEL_MCU_atmega128

#include <drivers/arch/mega128/types.h>
#include <drivers/arch/mega128/uart.h>
#include <drivers/arch/mega128/arch.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

drvUart_t busUart0;
drvUart_t busUart1;

void arch_uart_init(drvUart_t *bus, uint8_t num)
{
	/* UART selection & bus struct init */
	switch (num) {
	case 0:
		bus->ubrrh = (drvRegister_t *)(&UBRR0H);
		bus->ubrrl = (drvRegister_t *)(&UBRR0L);
		bus->ucsra = (drvRegister_t *)(&UCSR0A);
		bus->ucsrb = (drvRegister_t *)(&UCSR0B);
		bus->ucsrc = (drvRegister_t *)(&UCSR0C);
		bus->udr = (drvRegister_t *)(&UDR0);
		break;
	case 1:
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

	arch_enterCriticalSection();
	bus->buf_rx_head = 0;
	bus->buf_rx_tail = 0;
	arch_exitCriticalSection();
}

void arch_uart_set_mode(drvUart_t *bus, uint16_t flags)
{
	drvRegister_t ucsra = 0;
	drvRegister_t ucsrb = 0;
	drvRegister_t ucsrc = 0;

	bus->flags = flags;

	/* Default UART setup: RXEN & TXEN = 1, RX interrupt enabled, 8 bits, 1 stop bit, no parity */
	ucsra = (drvRegister_t)((UART_UXX_VAL(flags) << U2X));
	ucsrc = (drvRegister_t)((UART_SIZE_VAL(flags) << UCSZ0) | (UART_PARITY_VAL(flags) << UPM0) \
				| (UART_SYNC_VAL(flags) << UMSEL) | (UART_CPOL_VAL(flags) << UCPOL)) \
				| (UART_STOPBITS_VAL(flags) << USBS);

	/* Writing registers */
	arch_enterCriticalSection();
	*(bus->ucsra) = ucsra;
	*(bus->ucsrc) = ucsrc;
	*(bus->ucsrb) = ucsrb;
	arch_exitCriticalSection();
}

void arch_uart_set_speed(drvUart_t *bus, uint32_t baudrate)
{
	/* Baud rate register variables */
	drvRegister_t ubrrh = 0;
	drvRegister_t ubrrl = 0;
	uint32_t ubrr = UART_BAUD_PRESCALE(baudrate);

	if (UART_UXX_VAL(bus->flags) == UART_U1X) {
		ubrr /= 2;
	}

	/* Splitting UBRR into UBRRH & UBRRL */
	ubrrl = (drvRegister_t)(ubrr & (uint32_t) 0xFF); /* Lowest 8 bits */
	ubrr >>= 8;
	ubrrh = (drvRegister_t)(ubrr & (uint32_t) 0xFF); /* Upper 8 bits (out of 16) */

	/* Writing registers */
	arch_enterCriticalSection();
	*(bus->ubrrh) = ubrrh;
	*(bus->ubrrl) = ubrrl;
	arch_exitCriticalSection();
}

void arch_uart_start(drvUart_t *bus)
{
	drvRegister_t ucsrb = 0;

	ucsrb = (drvRegister_t)((1 << RXEN) | (1 << TXEN) | (1 << RXCIE) | (0 << TXCIE) | (0 << UDRIE));

	arch_enterCriticalSection();
	*(bus->ucsrb) = ucsrb;
	arch_exitCriticalSection();
}

void arch_uart_stop(drvUart_t *bus)
{
	arch_enterCriticalSection();
	*(bus->ucsrb) = 0;
	arch_exitCriticalSection();
}

void arch_uart_write(drvUart_t *bus, uint8_t in)
{
	uart_BUSY_WAIT();
	*(bus->udr) = in;
	bus->status = *(bus->ucsra);
}

uint8_t arch_uart_read(drvUart_t *bus)
{
	uint8_t rx_tail = 0;
	uint8_t data = 0;

	if (bus->buf_rx_head != bus->buf_rx_tail) {
		/* Calculate buffer index, wrap on overflow */
		uart_RXINT_DISABLE();

		rx_tail = (bus->buf_rx_tail + 1) & UART_RX_BUFFER_MASK;
		bus->buf_rx_tail = rx_tail;

		/* Get char from RX buffer */
		data = bus->buf_rx_b[rx_tail];

		uart_RXINT_ENABLE();
	}

	return data;
}

uint8_t arch_uart_read_write(drvUart_t *bus, uint8_t in)
{
	uint8_t data = 0;

	uart_RXINT_DISABLE();

	arch_uart_write(bus, in);
	uart_BUSY_WAIT();
	data = *(bus->udr);

	uart_RXINT_ENABLE();

	return data;
}

uint8_t arch_uart_peek(drvUart_t *bus)
{
	uint8_t rx_tail = 0;
	uint8_t data = '\0';

	if (bus->buf_rx_head != bus->buf_rx_tail) {
		uart_RXINT_DISABLE();

		/* Calculate buffer index, wrap on overflow */
		rx_tail = (bus->buf_rx_tail + 1) & UART_RX_BUFFER_MASK;
		/* Proceed without updating bus->buf_rx_tail */

		/* Get char from RX buffer */
		data = bus->buf_rx_b[rx_tail];

		uart_RXINT_ENABLE();
	}

	return data;
}

uint8_t arch_uart_available(drvUart_t *bus)
{
	uint8_t available = 0;

	arch_enterCriticalSection();
	available = (UART_RX_BUFFER_SIZE + bus->buf_rx_head - bus->buf_rx_tail) & UART_RX_BUFFER_MASK;
	arch_exitCriticalSection();

	return available;
}

void arch_uart_flushRX(drvUart_t *bus)
{
	bus->buf_rx_head = bus->buf_rx_tail;
}

ISR(USART0_RX_vect)
{
	uint8_t data = 0;
	uint8_t rx_head = 0;

	data = *(busUart0.udr);

	/* Calculate buffer index, wrap on overflow */
	rx_head = (busUart0.buf_rx_head + 1) & UART_RX_BUFFER_MASK;

	/* Store data if buffer is not full */
	if (rx_head != busUart0.buf_rx_tail) {
		busUart0.buf_rx_head = rx_head;
		busUart0.buf_rx_b[rx_head] = data;
	}
}

ISR(USART1_RX_vect)
{
	uint8_t data = 0;
	uint8_t rx_head = 0;

	data = *(busUart1.udr);

	/* Calculate buffer index, wrap on overflow */
	rx_head = (busUart1.buf_rx_head + 1) & UART_RX_BUFFER_MASK;

	/* Store data if buffer is not full */
	if (rx_head != busUart1.buf_rx_tail) {
		busUart1.buf_rx_head = rx_head;
		busUart1.buf_rx_b[rx_head] = data;
	}
}

#endif
#endif