/*
 * spi.h
 * 
 * Created: 06.04.2021 05:47:38
 * Author: ThePetrovich
 */


#ifndef YKTS_DRIVERS_SPI_H_
#define YKTS_DRIVERS_SPI_H_

#include <stdint.h>
#include <drivers/arch/mega128/types.h>

/* SPI bit order predefines */
#define SPI_MSBFIRST		(0)
#define SPI_LSBFIRST		(1)
#define SPI_MSB_MASK		(1)

/* SPI speed predefines */
#define SPI_PRESCALER_4 	(0 << 1)
#define SPI_PRESCALER_16 	(1 << 1)
#define SPI_PRESCALER_64 	(2 << 1)
#define SPI_PRESCALER_128 	(3 << 1)
#define SPI_PRESCALER_MASK	(3 << 1)

/* SPI speed multipliers */
#define SPI_2X			(1 << 3)
#define SPI_1X			(0 << 3)
#define SPI_XX_MASK		(1 << 3)

/* SPI mode predefines */
#define SPI_MODE0 		(0 << 4)
#define SPI_MODE1 		(1 << 4)
#define SPI_MODE2		(2 << 4)
#define SPI_MODE3		(3 << 4)
#define SPI_MODE_MASK		(3 << 4)

/* SPI interrupts control*/
#define SPI_INT_ON		(1 << 6)
#define SPI_INT_OFF		(0 << 6)
#define SPI_INT_MASK		(1 << 6)

/*
 *	SPI modes:
 *	0 - 0b00 - CPOL=0, CPHA=0 - mode 0
 *	1 - 0b01 - CPOL=0, CPHA=1 - mode 1
 *	2 - 0b10 - CPOL=1, CPHA=0 - mode 2
 *	3 - 0b11 - CPOL=1, CPHA=1 - mode 3
 */

#define SPI_RX_BUFFER_SIZE 32
#define SPI_RX_BUFFER_MASK (SPI_RX_BUFFER_SIZE - 1)

#define spi_CSLOW() SPI_PORT &= ~(1 << SPI_SS)
#define spi_CSHIGH() SPI_PORT |= (1 << SPI_SS)

typedef enum {SPI0} drvSpiEnum_t;

typedef struct drvSpiStruct_t 
{
	drvBusType_t type;
	drvSpiEnum_t bus;

	drvRegister_t *spdr;
	drvRegister_t *spcr;
	drvRegister_t *spsr;

	char buf_rx[SPI_RX_BUFFER_SIZE];
	uint8_t buf_rx_head;
	uint8_t buf_rx_tail;

	uint8_t flags;
} drvSpi_t;

extern drvSpi_t busSpi0;

void spi_init(drvSpi_t *bus, drvSpiEnum_t SpiSelect, uint16_t flags);

void spi_start(drvSpi_t *bus, uint16_t flags);
void spi_stop(drvSpi_t *bus);

void spi_write(drvSpi_t *bus, byte data);
byte spi_transfer(drvSpi_t *bus, byte data);

byte spi_read(drvSpi_t *bus);
byte spi_peek(drvSpi_t *bus);
int spi_available(drvSpi_t *bus);

void spi_flushRX(drvSpi_t *bus);

#endif /* YKTS_DRIVERS_SPI_H_ */