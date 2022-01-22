/*
 * main.c
 * 
 * Created: 30.03.2021 04:15:20
 * Author: ThePetrovich
 */


#define F_CPU 8000000L

#include <util/delay.h>
#include <drivers/arch/mega128/arch.h>
#include <drivers/arch/mega128/uart.h>

int main(void)
{       
        arch_ENABLE_INTERRUPTS();
        uart_init(&busUart0, UART0, 0, 38400);
	while (1)
	{
                uart_puts(&busUart0, "Hello world!\r\n");
		asm volatile("nop"::);
                _delay_ms(1000);

                if (uart_available(&busUart0) >= 8) {
                        uart_puts(&busUart0, "Data: ");
                        while (uart_available(&busUart0)) {
                                uart_putc(&busUart0, uart_getc(&busUart0));
                        }
                        uart_puts(&busUart0, "\r\n");
                }
	}
}