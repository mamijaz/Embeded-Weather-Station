#define lib_basics 1

#define _PORTA0 0
#define _PORTA1 1
#define _PORTA2 2
#define _PORTA3 3
#define _PORTA4 4
#define _PORTA5 5
#define _PORTA6 6
#define _PORTA7 7
#define _PORTB0 10
#define _PORTB1 11
#define _PORTB2 12
#define _PORTB3 13
#define _PORTB4 14
#define _PORTB5 15
#define _PORTB6 16
#define _PORTB7 17
#define _PORTC0 20
#define _PORTC1 21
#define _PORTC2 22
#define _PORTC3 23
#define _PORTC4 24
#define _PORTC5 25
#define _PORTC6 26
#define _PORTC7 27
#define _PORTD0 30
#define _PORTD1 31
#define _PORTD2 32
#define _PORTD3 33
#define _PORTD4 34
#define _PORTD5 35
#define _PORTD6 36
#define _PORTD7 37


void pinWrite(int a, int b){
	if(b == 0){
		if(a == _PORTB0)
		  PORTB &= ~(1<<PB0);  
		else if(a == _PORTB1)
		  PORTB &= ~(1<<PB1);
		else if(a == _PORTB2)
		  PORTB &= ~(1<<PB2);  
		else if(a == _PORTB3)
		  PORTB &= ~(1<<PB3);  
		else if(a == _PORTB4)
		  PORTB &= ~(1<<PB4);  
		else if(a == _PORTB5)
		  PORTB &= ~(1<<PB5);  
		else if(a == _PORTB6)
		  PORTB &= ~(1<<PB6);  
		else if(a == _PORTB7)
		  PORTB &= ~(1<<PB7);
		else if(a == _PORTC0)
		  PORTC &= ~(1<<PC0);   
		else if(a == _PORTC1)
		  PORTC &= ~(1<<PC1); 
		else if(a == _PORTC2)
		  PORTC &= ~(1<<PC2);
		else if(a == _PORTC3)
		  PORTC &= ~(1<<PC3);   
		else if(a == _PORTC4)
		  PORTC &= ~(1<<PC4);  
		else if(a == _PORTC5)
		  PORTC &= ~(1<<PC5);  
        else if(a == _PORTC6)
          PORTC &= ~(1<<PC6);		
		else if(a == _PORTD0)
		  PORTD &= ~(1<<PD0);
		else if(a == _PORTD1)
		  PORTD &= ~(1<<PD1);  
		else if(a == _PORTD2)
		  PORTD &= ~(1<<PD2);
		else if(a == _PORTD3)
		  PORTD &= ~(1<<PD3);
		else if(a == _PORTD4)
		  PORTD &= ~(1<<PD4);
		else if(a == _PORTD5)
		  PORTD &= ~(1<<PD5);
		else if(a == _PORTD6)
		  PORTD &= ~(1<<PD6);   
		else if(a == _PORTD7)
		  PORTD &= ~(1<<PD7);           
	}
	else{
	
		if(a == _PORTB0)
	  	  PORTB |= (1<<PB0);
		else if(a == _PORTB1)
		  PORTB |= (1<<PB1);
		else if(a == _PORTB2)
		  PORTB |= (1<<PB2);
		else if(a == _PORTB3)
		  PORTB |= (1<<PB3);
		else if(a == _PORTB4)
		  PORTB |= (1<<PB4);
		else if(a == _PORTB5)
		  PORTB |= (1<<PB5);
		else if(a == _PORTB6)
		  PORTB |= (1<<PB6);
		else if(a == _PORTB7)
		  PORTB |= (1<<PB7);
		else if(a == _PORTC0)
		  PORTC |= (1<<PC0);
		else if(a == _PORTC1)
		  PORTC |= (1<<PC1);
		else if(a == _PORTC2)
	  	  PORTC |= (1<<PC2);
		else if(a == _PORTC3)
		  PORTC |= (1<<PC3);
		else if(a == _PORTC4)
		  PORTC |= (1<<PC4);
		else if(a == _PORTC5)
		  PORTC |= (1<<PC5);
		else if(a == _PORTC6)
		  PORTC |= (1<<PC6);  
		else if(a == _PORTD0)
		  PORTD |= (1<<PD0);
		else if(a == _PORTD1)
		  PORTD |= (1<<PD1);
		else if(a == _PORTD2)
		  PORTD |= (1<<PD2);
		else if(a == _PORTD3)
		  PORTD |= (1<<PD3);
		else if(a == _PORTD4)
		  PORTD |= (1<<PD4);
		else if(a == _PORTD5)
		  PORTD |= (1<<PD5);
		else if(a == _PORTD6)
		  PORTD |= (1<<PD6);
		else if(a == _PORTD7)
		  PORTD |= (1<<PD7);
	}
}

void Disable_Prescaler(void){
	CLKPR = (1 << CLKPCE); // enable a change to CLKPR
	CLKPR = 0; // set the CLKDIV to 0 - was 0011b = div by 8 taking 8MHz to 1MHz
}

uint16_t ReadADC(uint8_t ch){
	ADMUX=(1<<REFS0);									// For Aref=AVcc;
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);  //Prescalar div factor =128
	ch=ch&0b00000111;									//Select ADC Channel ch must be 0-7
	ADMUX|=ch;
	ADCSRA|=(1<<ADSC);									//Start Single conversion
	while(!(ADCSRA & (1<<ADIF)));						//Wait for conversion to complete
	ADCSRA|=(1<<ADIF);									//Clear ADIF by writing one to it
	return(ADC);
}