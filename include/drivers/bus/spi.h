/*
 * spi.h
 * 
 * Created: 22.02.2022 05:13:18
 * Author: ThePetrovich
 */


#ifndef YKTS_DRIVERS_BUS_SPI_H_
#define YKTS_DRIVERS_BUS_SPI_H_

#include <stdint.h>
#include <drivers/bus/bus.h>
#include <drivers/arch/arch.h>

class BusSpi : public BusBase
{
	public:
		BusSpi(uint8_t num);
		void putc(char c);
		void puts(char *s);
		char getc();

		void start();
		void stop();

		void setSpeed(uint32_t speed);
		void setMode(uint16_t flags);

		void write(uint8_t in);
		void writeBurst(uint8_t *in, size_t size);

		uint8_t read();
		uint8_t readWrite(uint8_t in);
		void readWriteBurst(uint8_t *in, uint8_t *out, size_t size);

		uint8_t peek();
		uint8_t available();
	private:
		drvSpi_t *spiDriver;
};

extern BusSpi Spi0;

#endif /* YKTS_DRIVERS_BUS_SPI_H_ */