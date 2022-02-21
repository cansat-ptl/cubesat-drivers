/*
 * archtypes.h
 * 
 * Created: 15.06.2021 11:56:12
 * Author: ThePetrovich
 */


#ifndef YKTS_DRIVERS_ARCHTYPES_H_
#define YKTS_DRIVERS_ARCHTYPES_H_

#ifdef KERNEL_ARCH_avr
	#if defined KERNEL_MCU_atmega128
		#include "./mega128/types.h"
	#elif defined KERNEL_MCU_atmega2560
		#include "./mega2560/types.h"
	#else
		/* workaround to make IntelliSense happy */
		/* makes you REALLY contemplate directory structure choices */
		#include "./stub/types.h"
		#warning "Unsupported/unknown AVR MCU, using stub types"
	#endif
#else
	/* workaround to make IntelliSense happy */
	/* makes you REALLY contemplate directory structure choices */
	#include "./stub/types.h"
	#warning "Unsupported/unknown architecture, using stub types"
#endif

#endif /* YKTS_DRIVERS_ARCHTYPES_H_ */
