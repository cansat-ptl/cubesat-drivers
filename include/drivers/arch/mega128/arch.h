/*
 * arch.h
 * 
 * Created: 28.12.2021 01:27:13
 * Author: ThePetrovich
 */


#ifndef YKTS_DRIVERS_MEGA128_H_
#define YKTS_DRIVERS_MEGA128_H_

#include <avr/io.h>
#include <avr/iom128.h>

/* CPU frequency */
#ifndef F_CPU
#define F_CPU 16000000L
#endif

#ifndef KERNEL_ARCH_H_
	#define lo8(x) ((x)&0xff)
	#define hi8(x) ((x)>>8)

	#define arch_DISABLE_INTERRUPTS() 	__asm__ __volatile__ ("cli"::)
	#define arch_ENABLE_INTERRUPTS() 	__asm__ __volatile__ ("sei"::)
	#define arch_STATUS_REG	  		SREG
	#define arch_NOP()			__asm__ __volatile__ ("nop"::)
	#define arch_enterCriticalSection()	__asm__ __volatile__ ("lds __tmp_reg__, __SREG__ \n\t"\
								"cli		       \n\t"\
								"push __tmp_reg__"	    ::)
	#define arch_exitCriticalSection() 	__asm__ __volatile__ ("pop __tmp_reg__	   \n\t"\
								"sei		       \n\t"\
								"sts __SREG__, __tmp_reg__"   ::)
#endif

#ifdef __cplusplus
extern "C" {
#endif

void arch_init();
void arch_halt();
void arch_reboot();

#ifdef __cplusplus
}
#endif

#endif /* YKTS_DRIVERS_MEGA128_H_ */