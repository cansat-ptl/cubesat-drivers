/*
 * spi.cpp
 * 
 * Created: 22.02.2022 05:14:30
 * Author: ThePetrovich
 */


#include <drivers/bus/bus.h>
#include <drivers/bus/spi.h>
#include <drivers/arch/arch.h>
#include <stdint.h>
#include <stddef.h>

BusSpi Spi0(0);

BusSpi::BusSpi(uint8_t num)
{
	switch (num) {
	case 0:
		this->spiDriver = &busSpi0;
		break;
	default:
		break;
	}
		
	arch_spi_init(this->spiDriver, num);
}

void BusSpi::start()
{
	arch_spi_start(this->spiDriver);
}

void BusSpi::stop()
{
	arch_spi_stop(this->spiDriver);
}

void BusSpi::setMode(uint16_t flags)
{
	arch_spi_set_mode(this->spiDriver, flags);
}

void BusSpi::setSpeed(uint32_t speed)
{
	return;
}

void BusSpi::write(uint8_t in)
{
	arch_spi_write(this->spiDriver, in);
}

void BusSpi::writeBurst(uint8_t *in, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		this->write(in[i]);
	}
}

uint8_t BusSpi::read()
{
	return arch_spi_read(this->spiDriver);
}

uint8_t BusSpi::readWrite(uint8_t in)
{
	return arch_spi_read_write(this->spiDriver, in);
}

void BusSpi::readWriteBurst(uint8_t *in, uint8_t *out, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		out[i] = this->readWrite(in[i]);
	}
}

uint8_t BusSpi::peek()
{
	return arch_spi_peek(this->spiDriver);
}

uint8_t BusSpi::available()
{
	return arch_spi_available(this->spiDriver);
}