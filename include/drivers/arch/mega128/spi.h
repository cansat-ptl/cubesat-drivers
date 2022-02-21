/*
 * spi.h
 * 
 * Created: 06.04.2021 05:47:38
 * Author: ThePetrovich
 */


#ifndef YKTS_DRIVERS_SPI_H_
#define YKTS_DRIVERS_SPI_H_

#include <stdint.h>
#include <stddef.h>
#include <drivers/arch/mega128/types.h>
#include <drivers/arch/mega128/arch.h>

/* ATmega128 SPI port is on PORTB */
#define SPI_PORT		PORTB
#define SPI_DDR		 	DDRB

/* SPI PORTB pins */
#define SPI_PIN_MISO		PB3
#define SPI_PIN_MOSI		PB2
#define SPI_PIN_SCK		PB1
#define SPI_PIN_SS		PB0

/* SPI bit order defines */
#define SPI_MSBFIRST		(0)
#define SPI_LSBFIRST		(1)
#define SPI_MSB_VAL(x)		((uint8_t)(x & (1)))

/* SPI speed defines */
#define SPI_PRESCALER_4 	(0 << 1)
#define SPI_PRESCALER_16 	(1 << 1)
#define SPI_PRESCALER_64 	(2 << 1)
#define SPI_PRESCALER_128 	(3 << 1)
#define SPI_PRESCALER_VAL(x)	((uint8_t)((x & (3 << 1)) >> 1))

/* SPI speed multipliers */
#define SPI_2X			(1 << 3)
#define SPI_1X			(0 << 3)
#define SPI_XX_VAL(x)		((uint8_t)((x & (1 << 3)) >> 3))

/* SPI mode defines */
#define SPI_MODE0 		(0 << 4)
#define SPI_MODE1 		(1 << 4)
#define SPI_MODE2		(2 << 4)
#define SPI_MODE3		(3 << 4)
#define SPI_MODE_VAL(x)		((uint8_t)((x & (3 << 4)) >> 4))

/*
 *	SPI modes:
 *	0 - 0b00 - CPOL=0, CPHA=0 - mode 0
 *	1 - 0b01 - CPOL=0, CPHA=1 - mode 1
 *	2 - 0b10 - CPOL=1, CPHA=0 - mode 2
 *	3 - 0b11 - CPOL=1, CPHA=1 - mode 3
 */

#define SPI_RX_BUFFER_SIZE 32
#define SPI_RX_BUFFER_MASK (SPI_RX_BUFFER_SIZE - 1)

#define arch_SPI_CS_LOW() SPI_PORT &= ~(1 << SPI_PIN_SS)
#define arch_SPI_CS_HIGH() SPI_PORT |= (1 << SPI_PIN_SS)

#define arch_SPI_BUSY_WAIT() {while (!((*(bus->spsr)) & (1 << SPIF))){;}}

#define arch_SPI_INT_ENABLE() (*(bus->spcr) |= (1 << SPIE))
#define arch_SPI_INT_DISABLE() (*(bus->spcr) &= ~(1 << SPIE))

typedef struct drvSpiStruct_t 
{
	drvRegister_t *spdr;
	drvRegister_t *spcr;
	drvRegister_t *spsr;

	uint8_t buf_rx_b[SPI_RX_BUFFER_SIZE];
	uint8_t buf_rx_head;
	uint8_t buf_rx_tail;

	uint8_t status;

	uint16_t flags;
	uint32_t speed;
} drvSpi_t;

extern drvSpi_t busSpi0;

#ifdef __cplusplus
extern "C" {
#endif

void arch_spi_init(drvSpi_t *bus, uint8_t num);

void arch_spi_start(drvSpi_t *bus);
void arch_spi_set_mode(drvSpi_t *bus, uint16_t flags);
void arch_spi_stop(drvSpi_t *bus);

void arch_spi_write(drvSpi_t *bus, uint8_t data);
uint8_t arch_spi_read(drvSpi_t *bus);
uint8_t arch_spi_read_write(drvSpi_t *bus, uint8_t in);

uint8_t arch_spi_read(drvSpi_t *bus);
uint8_t arch_spi_peek(drvSpi_t *bus);
uint8_t arch_spi_available(drvSpi_t *bus);

void arch_spi_flushRX(drvSpi_t *bus);

#ifdef __cplusplus
}
#endif

#endif /* YKTS_DRIVERS_SPI_H_ */