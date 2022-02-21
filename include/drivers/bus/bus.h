/*
 * bus.h
 * 
 * Created: 20.02.2022 06:17:42
 * Author: ThePetrovich
 */


#ifndef YKTS_DRIVERS_BUS_BASE_H_
#define YKTS_DRIVERS_BUS_BASE_H_

#include <stdint.h>
#include <stddef.h>

class BusBase
{
	public:
		BusBase() { return; };

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
};

#endif /* YKTS_DRIVERS_BUS_BASE_H_ */