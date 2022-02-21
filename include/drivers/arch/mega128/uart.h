/*
 * uart.h
 * 
 * Created: 06.04.2021 05:47:48
 * Author: ThePetrovich
 */


#ifndef YKTS_DRIVERS_UART_H_
#define YKTS_DRIVERS_UART_H_

#include <stdint.h>
#include <stddef.h>
#include <drivers/arch/mega128/types.h>
#include <drivers/arch/mega128/arch.h>

#ifndef F_CPU
	#warning "F_CPU is not defined"
#endif

#ifndef UART_BAUD_PRESCALE
	#define UART_BAUD_PRESCALE(x) (((F_CPU / ((uint32_t)x * 16UL))) - 1)
#else
	#warning "UART_BAUD_PRESCALE already defined"
#endif

/* UART sync/async mode defines */
#define UART_ASYNC		(0)
#define UART_SYNC		(1)
#define UART_SYNC_VAL(x)	((uint8_t)(x & (1)))

/* UART paritu defines */
#define UART_PARITY_NONE 	(0 << 1)
#define UART_PARITY_EVEN 	(2 << 1)
#define UART_PARITY_ODD 	(3 << 1)
#define UART_PARITY_VAL(x)	((uint8_t)((x & (3 << 1)) >> 1))

/* UART speed multipliers */
#define UART_U2X		(1 << 3)
#define UART_U1X		(0 << 3)
#define UART_UXX_VAL(x)		((uint8_t)((x & (1 << 3)) >> 3))

/* UART mode defines */
#define UART_SIZE_5BIT 		(0 << 4)
#define UART_SIZE_6BIT 		(1 << 4)
#define UART_SIZE_7BIT		(2 << 4)
#define UART_SIZE_8BIT		(3 << 4)
#define UART_SIZE_VAL(x)	((uint8_t)((x & (3 << 4)) >> 4))

/* UART synchronous clock polarity */
#define UART_CPOL_F		(1 << 3)
#define UART_CPOL_R		(0 << 3)
#define UART_CPOL_VAL(x)	((uint8_t)((x & (1 << 5)) >> 5))

/* UART stop bit defines */
#define UART_STOPBITS_1		(0)
#define UART_STOPBITS_2		(1)
#define UART_STOPBITS_VAL(x)	((uint8_t)(x & (1)))

#define UART_RX_BUFFER_SIZE 32
#define UART_RX_BUFFER_MASK (UART_RX_BUFFER_SIZE - 1)

#define UART_TX_BUFFER_SIZE 32
#define UART_TX_BUFFER_MASK (UART_TX_BUFFER_SIZE - 1)

#define arch_UART_BUSY_WAIT() {while (!((*(bus->ucsra)) & (1 << UDRE))){;}}

#define arch_UART_RXINT_ENABLE() (*(bus->ucsrb) |= (1 << RXCIE))
#define arch_UART_RXINT_DISABLE() (*(bus->ucsrb) &= ~(1 << RXCIE))

typedef struct drvUartStruct_t 
{
	drvRegister_t *udr;
	drvRegister_t *ucsra;
	drvRegister_t *ucsrb;
	drvRegister_t *ucsrc;
	drvRegister_t *ubrrh;
	drvRegister_t *ubrrl;


	uint8_t buf_rx_b[UART_RX_BUFFER_SIZE];
	uint8_t buf_rx_head;
	uint8_t buf_rx_tail;

	uint8_t status;

	uint16_t flags;
	uint32_t speed;
} drvUart_t;

extern drvUart_t busUart0;
extern drvUart_t busUart1;

#ifdef __cplusplus
extern "C" {
#endif

void arch_uart_init(drvUart_t *bus, uint8_t num);

void arch_uart_start(drvUart_t *bus);
void arch_uart_set_mode(drvUart_t *bus, uint16_t flags);
void arch_uart_set_speed(drvUart_t *bus, uint32_t baudrate);
void arch_uart_stop(drvUart_t *bus);

void arch_uart_write(drvUart_t *bus, uint8_t in);
uint8_t arch_uart_read(drvUart_t *bus);
uint8_t arch_uart_read_write(drvUart_t *bus, uint8_t in);

uint8_t arch_uart_peek(drvUart_t *bus);
uint8_t arch_uart_available(drvUart_t *bus);

void arch_uart_flushRX(drvUart_t *bus);

#ifdef __cplusplus
}
#endif

#endif /* YKTS_DRIVERS_UART_H_ */