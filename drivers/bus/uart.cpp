/*
 * uart.cpp
 * 
 * Created: 20.02.2022 07:40:24
 * Author: ThePetrovich
 */


#include <drivers/bus/bus.h>
#include <drivers/bus/uart.h>
#include <drivers/arch/arch.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

BusUart Uart0(0);
BusUart Uart1(1);

BusUart::BusUart(uint8_t num)
{
	switch (num) {
	case 0:
		this->uartDriver = &busUart0;
		break;
	case 1:
		this->uartDriver = &busUart1;
		break;
	default:
		break;
	}
		
	arch_uart_init(this->uartDriver, num);
}

void BusUart::start()
{
	arch_uart_start(this->uartDriver);
}

void BusUart::stop()
{
	arch_uart_stop(this->uartDriver);
}

void BusUart::setMode(uint16_t flags)
{
	arch_uart_set_mode(this->uartDriver, flags);
}

void BusUart::setSpeed(uint32_t speed)
{
	arch_uart_set_speed(this->uartDriver, speed);
}

void BusUart::putc(char c)
{
	arch_uart_write(this->uartDriver, (uint8_t)c);
}

void BusUart::puts(char *s)
{
	while (*s) {
		this->putc(*(s++));
	}
}

char BusUart::getc()
{
	return (char)arch_uart_read(this->uartDriver);
}

void BusUart::write(uint8_t in)
{
	arch_uart_write(this->uartDriver, in);
}

void BusUart::writeBurst(uint8_t *in, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		this->write(in[i]);
	}
}

uint8_t BusUart::read()
{
	return arch_uart_read(this->uartDriver);
}

uint8_t BusUart::readWrite(uint8_t in)
{
	return arch_uart_read_write(this->uartDriver, in);
}

void BusUart::readWriteBurst(uint8_t *in, uint8_t *out, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		out[i] = this->readWrite(in[i]);
	}
}

uint8_t BusUart::peek()
{
	return arch_uart_peek(this->uartDriver);
}

uint8_t BusUart::available()
{
	return arch_uart_available(this->uartDriver);
}