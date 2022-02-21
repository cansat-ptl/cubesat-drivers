/*
 * arch.c
 * 
 * Created: 21.02.2022 10:47:06
 * Author: ThePetrovich
 */


#ifdef KERNEL_ARCH_avr
#ifdef KERNEL_MCU_atmega128

#include <drivers/arch/mega128/types.h>
#include <drivers/arch/mega128/arch.h>
#include <avr/wdt.h>

static volatile uint8_t mcusr_mirror __attribute__ ((section (".noinit")));

void arch_init()
{
	arch_ENABLE_INTERRUPTS();
}

void arch_halt()
{	
	arch_DISABLE_INTERRUPTS();
	wdt_disable();
	while(1) {
		; /* wait for reset */
	}
}

void arch_reboot()
{
	arch_DISABLE_INTERRUPTS();
	wdt_enable(WDTO_15MS);
	while(1) {
		; /* wait for reset */
	}
}

static void arch_disable_watchdog_early(void) __attribute__((naked, section(".init3")));

static void arch_disable_watchdog_early(void)
{
	mcusr_mirror = MCUCSR;
	MCUCSR = 0;
	wdt_disable();
}

#endif
#endif