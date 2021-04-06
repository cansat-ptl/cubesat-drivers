/*
 * uart.c
 * 
 * Created: 06.04.2021 06:02:26
 * Author: ThePetrovich
 */


#include <drivers/bus/mega128/uart.h>
#include <string.h>
#include <avr/iom128.h>
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile char rx0_buffer[32] = "";
static volatile char tx0_buffer[32] = "";
static volatile int rx0_pointer = 0;
static volatile int tx0_pointer = 0;
	
static volatile char rx1_buffer[32] = "";
static volatile char tx1_buffer[32] = "";
static volatile int rx1_pointer = 0;
static volatile int tx1_pointer = 0;

int uart0_init(unsigned int ubrr)
{
	UBRR0H = 0;
	UBRR0L = 12;
	UCSR0B = (RX0EN<<RXEN)|(TX0EN<<TXEN)|(RX0IE<<RXCIE)|(TX0IE<<TXCIE)|(0<<UDRIE);
	UCSR0C = (0<<USBS)|(1<<UCSZ00)|(1<<UCSZ01)|(0<<UCSZ02)|(0<<UPM00)|(0<<UPM01)|(0<<UMSEL0);
	return 0;
}

void uart0_transmit()
{
	UDR0 = tx0_buffer[0];
	UCSR0B |= (1<<UDRIE);
}

void uart0_putc(char c)
{
	UDR0 = c;
	while(!(UCSR0A & (1<<UDRE)));
}

void uart0_puts(char * msg)
{
	int i = 0;
	while(msg[i] != '\0'){
		UDR0 = msg[i];
		while(!(UCSR0A & (1<<UDRE)));
		i++;
	}
}

void rx0_buffer_flush()
{
	rx0_buffer[0] = 0;
	rx0_pointer = 0;
}

void tx0_buffer_flush()
{
	tx0_pointer = 0;
	tx0_buffer[0] = '\x0';
}

ISR(USART0_RX_vect)
{
	char data = UDR0;
	if(strlen((char*)&rx0_buffer) < 32){
		rx0_buffer[rx0_pointer] = data;
		rx0_pointer++;
	}
}

ISR(USART0_UDRE_vect)
{
	tx0_pointer+=1;
	if(tx0_buffer[tx0_pointer] != '\x0'){
		UDR0 = tx0_buffer[tx0_pointer];
	}
	else {
		UCSR0B &= ~(1<<UDRIE);
		tx0_buffer_flush();
	}
}

int uart1_init(unsigned int ubrr)
{
	UBRR1H = 0;
	UBRR1L = 51;
	UCSR1B = (RX1EN<<RXEN)|(TX1EN<<TXEN)|(RX1IE<<RXCIE)|(TX1IE<<TXCIE)|(0<<UDRIE);
	UCSR1C = (0<<USBS)|(1<<UCSZ00)|(1<<UCSZ01)|(0<<UCSZ02)|(0<<UPM00)|(0<<UPM01)|(0<<UMSEL0);
	return 0;
}

void uart1_transmit()
{
	UDR1 = tx1_buffer[0];
	UCSR1B |= (1<<UDRIE);
}

void rx1_buffer_flush()
{
	rx1_buffer[0] = 0;
	rx1_pointer = 0;
}

void tx1_buffer_flush()
{
	tx1_pointer = 0;
	tx1_buffer[0] = '\x0';
}


ISR(USART1_RX_vect)
{
	char data = UDR1;
	if(strlen((char*)&rx1_buffer) < 32){
		rx1_buffer[rx1_pointer] = data;
		rx1_pointer++;
	}
}

ISR(USART1_UDRE_vect)
{
	tx1_pointer+=1;
	if(tx1_buffer[tx1_pointer] != '\x0'){
		UDR1 = tx1_buffer[tx1_pointer];
	}
	else {
		UCSR1B &= ~(1<<UDRIE);
		tx1_buffer_flush();
	}
}
