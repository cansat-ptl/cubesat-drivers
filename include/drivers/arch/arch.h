/*
 * arch.h
 * 
 * Created: 08.02.2021 06:57:40
 * Author: ThePetrovich
 */


#ifndef YKTS_DRIVERS_ARCH_H_
#define YKTS_DRIVERS_ARCH_H_

#ifdef KERNEL_ARCH_avr
	#define KERNEL_ARCH "avr"
	#if defined KERNEL_MCU_atmega128
		#define KERNEL_MCU "atmega128"
		#include "./mega128/arch.h"
		#include "./mega128/uart.h"
		#include "./mega128/spi.h"
	#elif defined KERNEL_MCU_atmega2560
		#define KERNEL_MCU "atmega2560"
		#include "./mega2560/arch.h"
		#include "./mega2560/uart.h"
		#include "./mega2560/spi.h"
	#else
		#error "Unsupported/unknown AVR MCU"
	#endif
#else
	#error "Unsupported/unknown architecture"
#endif

#ifndef KERNEL_ARCH
#define KERNEL_ARCH "unkn"
#endif

#ifndef KERNEL_MCU
#define KERNEL_MCU "unkn"
#endif

#endif /* YKTS_DRIVERS_ARCH_H_ */