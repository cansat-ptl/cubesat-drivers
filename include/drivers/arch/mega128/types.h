/*
 * types.h
 * 
 * Created: 15.06.2021 11:55:18
 * Author: ThePetrovich
 */


#ifndef YKTS_DRIVERS_AVRM128_TYPES_H_
#define YKTS_DRIVERS_AVRM128_TYPES_H_

#include <stdint.h>

typedef uint8_t byte;
typedef volatile uint8_t drvRegister_t;
typedef volatile uint8_t drvStatusRegister_t;
typedef enum {BUS_UNKNOWN, BUS_UART, BUS_SPI, BUS_TWI, BUS_ONEWIRE} drvBusType_t;

#endif /* YKTS_DRIVERS_AVRM128_TYPES_H_ */