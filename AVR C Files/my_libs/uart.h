#ifndef  F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>

void USART_Init( unsigned int baud){
	unsigned int ubrr =((F_CPU/16/baud)-1);
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<USBS0)|(1<<UCSZ00)|(1<<UCSZ01);
}

void USART_Transmit(unsigned char data ){
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
}

void USART_Transmit_String(char *s ){	
	while (*s)
	USART_Transmit(*s++);
}

unsigned char USART_Receive( void ){
	while ( !(UCSR0A & (1<<RXC0)) );
	return UDR0;
}