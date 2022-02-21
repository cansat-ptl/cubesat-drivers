/*
 * spi.c
 * 
 * Created: 06.04.2021 06:02:21
 * Author: ThePetrovich
 */


#ifdef KERNEL_ARCH_avr
#ifdef KERNEL_MCU_atmega128

#include <drivers/arch/mega128/types.h>
#include <drivers/arch/mega128/spi.h>
#include <drivers/arch/mega128/arch.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

drvSpi_t busSpi0;

void arch_spi_init(drvSpi_t *bus, uint8_t num)
{

	arch_enterCriticalSection();
	/* Setting up SPI pins - data directions to out, MISO pullup enabled */
	SPI_DDR |= (1 << SPI_PIN_MOSI) | (1 << SPI_PIN_SCK) | (1 << SPI_PIN_SS);
	SPI_DDR &= ~(1 << SPI_PIN_MISO);
	SPI_PORT |= (1 << SPI_PIN_MISO) | (1 << SPI_PIN_SS);

	bus->buf_rx_head = 0;
	bus->buf_rx_tail = 0;
	arch_exitCriticalSection();
}

void arch_spi_set_mode(drvSpi_t *bus, uint16_t flags)
{
	drvRegister_t spsr = 0;
	drvRegister_t spcr = 0;

	/* Set 1X / 2X speed */
	spsr = (drvRegister_t)((SPI_XX_VAL(flags) << SPI2X));
	/* Set SPCR - bit order, clock idle state & polarity, prescaler, interrupts, master mode */
	spcr = (drvRegister_t)((SPI_MSB_VAL(flags) << DORD) | (SPI_MODE_VAL(flags) << CPHA) \
				| (SPI_PRESCALER_VAL(flags) << SPR0) | (1 << MSTR) | (1 << SPIE));

	arch_enterCriticalSection();
	SPCR = spcr;
	SPSR = spsr;
	arch_exitCriticalSection();
}

void arch_spi_start(drvSpi_t *bus)
{	
	arch_enterCriticalSection();
	SPCR |= (1 << SPE);
	arch_exitCriticalSection();
}

void arch_spi_stop(drvSpi_t *bus)
{
	arch_enterCriticalSection();
	SPCR = 0;
	SPSR = 0;
	arch_exitCriticalSection();
}

void arch_spi_write(drvSpi_t *bus, uint8_t data)
{
	SPDR = data;
	arch_SPI_BUSY_WAIT();
}

uint8_t arch_spi_read(drvSpi_t *bus)
{
	uint8_t rx_tail = 0;
	uint8_t data = 0;

	if (bus->buf_rx_head != bus->buf_rx_tail) {
		/* Calculate buffer index, wrap on overflow */
		arch_SPI_INT_DISABLE();

		rx_tail = (bus->buf_rx_tail + 1) & SPI_RX_BUFFER_MASK;
		bus->buf_rx_tail = rx_tail;

		/* Get char from RX buffer */
		data = bus->buf_rx_b[rx_tail];

		arch_SPI_INT_ENABLE();
	}

	return data;
}

uint8_t arch_spi_read_write(drvSpi_t *bus, uint8_t in)
{
	uint8_t data = 0;

	arch_SPI_INT_DISABLE();

	arch_spi_write(bus, in);
	arch_SPI_BUSY_WAIT();
	data = SPDR;

	arch_SPI_INT_ENABLE();

	return data;
}

uint8_t arch_spi_peek(drvSpi_t *bus)
{
	uint8_t rx_tail = 0;
	uint8_t data = '\0';

	if (bus->buf_rx_head != bus->buf_rx_tail) {
		arch_SPI_INT_DISABLE();

		/* Calculate buffer index, wrap on overflow */
		rx_tail = (bus->buf_rx_tail + 1) & SPI_RX_BUFFER_MASK;
		/* Proceed without updating bus->buf_rx_tail */

		/* Get char from RX buffer */
		data = bus->buf_rx_b[rx_tail];

		arch_SPI_INT_ENABLE();
	}

	return data;
}

uint8_t arch_spi_available(drvSpi_t *bus)
{
	uint8_t available = 0;

	arch_enterCriticalSection();
	available = (SPI_RX_BUFFER_SIZE + bus->buf_rx_head - bus->buf_rx_tail) & SPI_RX_BUFFER_MASK;
	arch_exitCriticalSection();

	return available;
}

void arch_spi_flushRX(drvSpi_t *bus)
{
	bus->buf_rx_head = bus->buf_rx_tail;
}

ISR(SPI_STC_vect)
{
	uint8_t data = 0;
	uint8_t rx_head = 0;

	data = SPDR;

	/* Calculate buffer index, wrap on overflow */
	rx_head = (busSpi0.buf_rx_head + 1) & SPI_RX_BUFFER_MASK;

	/* Store data if buffer is not full */
	if (rx_head != busSpi0.buf_rx_tail) {
		busSpi0.buf_rx_head = rx_head;
		busSpi0.buf_rx_b[rx_head] = data;
	}
}


#endif
#endif