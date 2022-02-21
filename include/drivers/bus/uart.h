/*
 * uart.h
 * 
 * Created: 20.02.2022 06:32:09
 * Author: ThePetrovich
 */


#ifndef YKTS_DRIVERS_BUS_UART_H_
#define YKTS_DRIVERS_BUS_UART_H_

#include <stdint.h>
#include <drivers/bus/bus.h>
#include <drivers/arch/arch.h>

class BusUart : public BusBase
{
	public:
		BusUart(uint8_t num);
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
		drvUart_t *uartDriver;
};

extern BusUart Uart0;
extern BusUart Uart1;

#endif /* YKTS_DRIVERS_BUS_UART_H_ */