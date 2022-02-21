/*
 * main.c
 * 
 * Created: 30.03.2021 04:15:20
 * Author: ThePetrovich
 */


#define F_CPU 16000000L

#include <util/delay.h>
#include <drivers/bus/uart.h>
#include <drivers/arch/arch.h>

int main(void)
{       
        arch_ENABLE_INTERRUPTS();
        Uart0.setMode(UART_SIZE_8BIT | UART_PARITY_NONE | UART_STOPBITS_1);
        Uart0.setSpeed(38400);
        Uart0.start();

        Uart0.puts("Hello world!\r\n");

	while (1)
	{
                 if (Uart0.available() >= 8) {
                        Uart0.puts("Data: ");
                        while (Uart0.available()) {
                                Uart0.putc(Uart0.getc());
                        }
                        Uart0.puts("\r\n");
                }
	}
}