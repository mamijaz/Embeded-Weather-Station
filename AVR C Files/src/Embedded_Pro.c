/*
 * Embedded_Pro.c
 *
 * Created: 1/28/2017 11:30:38 AM
 *  Author: ijaz
 */ 

#define F_CPU 8000000
#define BAUD 9600

#define RS _PORTB0
#define EN _PORTD7
#define D4 _PORTD6
#define D5 _PORTD5
#define D6 _PORTB7
#define D7 _PORTB6

#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <my_libs/lcd.h>
#include <my_libs/uart.h>
#include <my_libs/bmp180.h>
#include <my_libs/HMC5883L.h>

void aquireAndDisplayData();
void ftoa(float num,char *str);
void formatDataAndSendToWUnderground();
void sendDataGSM(char *data);

int int0_count;
float tempC;
long preP;
float windDir;
float windSpeed = 20;
float rainFall;
const char *tempCon;
long timer0_intCount;
char dis[12];

void enable_int0(){
	cli();
	DDRD &= ~(1 << DDD2);					// Clear the PD2 pin
	PORTD |= (1 << PORTD2);					// turn On the Pull-up
	EICRA |= ((1 << ISC00)|(1 << ISC01));   // set INT0 to trigger on rising logic change
	EIMSK |= (1 << INT0);					// Turns on INT0
	sei();
}

ISR(INT0_vect){
	int0_count++;
}


void timer1_init(){
	cli();				
	TIMSK1 |= (1<<TOIE1);
	TCNT1 = 0x00;
	TCCR1B |= (1 << CS10) | (1 << CS12);
	sei();
}

ISR(TIMER1_OVF_vect) {
	TCNT1 = 0x00;
	timer0_intCount ++;
	if(timer0_intCount >=50){
		timer0_intCount = 0;
		windSpeed = ((int0_count * 60)/(7.0 * 10 * 1000))*1000; //*1000 for facter
		int0_count = 0;
		formatDataAndSendToWUnderground();
	}
}


int main(void){
	
	DDRB = 0b11000001;
	DDRD = 0b11100000;
	DDRC = 0b00000000;
	
	enable_int0();
	timer1_init();
	
	Disable_Prescaler();
	
	Lcd4_Init();
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,5);
	Lcd4_Write_String("P W S");
	Lcd4_Set_Cursor(2,4);
	Lcd4_Write_String("Moratuwa");
	_delay_ms(1000);

	aquireAndDisplayData();
	formatDataAndSendToWUnderground();
	
	while(1){
	    aquireAndDisplayData();
	}
	
}

void aquireAndDisplayData(){
	 Lcd4_Clear();
	 BMP180Init();
	 BMP180Calibration();
	 tempC=BMP180ReadTemperature();
	 preP=BMP180ReadPressure();
	 Lcd4_Set_Cursor(1,0);
	 Lcd4_Write_String("Temp:");
	 Lcd4_Set_Cursor(2,0);
	 Lcd4_Write_String("Pre :");
	 ftoa(tempC,dis);
	 tempCon=" C";
	 strcat(dis,tempCon);
	 Lcd4_Set_Cursor(1,6);
	 Lcd4_Write_String(dis);
	 ltoa(preP,dis,10);
	 tempCon=" Pa";
	 strcat(dis,tempCon);
	 Lcd4_Set_Cursor(2,6);
	 Lcd4_Write_String(dis);
	 _delay_ms(2000);
	 
	 Lcd4_Clear();
	 HMC5883_Init();
	 HMC5883_Calibration();
	 windDir = HMC5883_Heading();
	 Lcd4_Set_Cursor(1,0);
	 Lcd4_Write_String("W.Dir:");
	 Lcd4_Set_Cursor(2,0);
	 Lcd4_Write_String("W.Spe:");
	 ftoa(windDir,dis);
	 tempCon=" ";
	 strcat(dis,tempCon);
	 Lcd4_Set_Cursor(1,7);
	 Lcd4_Write_String(dis);
	 ftoa(windSpeed,dis);
	 tempCon=" km/h";
	 strcat(dis,tempCon);
	 Lcd4_Set_Cursor(2,7);
	 Lcd4_Write_String(dis);
	 _delay_ms(2000);
	 
	 Lcd4_Clear();
	 rainFall = ReadADC(0)*0.1;
	 Lcd4_Set_Cursor(1,0);
	 Lcd4_Write_String("Rain:");
	 ftoa(rainFall,dis);
	 tempCon=" mm";
	 strcat(dis,tempCon);
	 Lcd4_Set_Cursor(1,6);
	 Lcd4_Write_String(dis);
	 _delay_ms(2000);
}

void ftoa(float num,char *str){
	int wholeNum = num;
	itoa(wholeNum,str,10);
	const char *tempCons = ".";
	strcat(str,tempCons);
	int decimal;
	if(num >= wholeNum){
		decimal = ((float)num - wholeNum)*100;
	}
	else{
		decimal = (wholeNum - (float)num)*100;
	}
	char *temp = "null";
	itoa(decimal,temp,10);
	tempCons = temp;
	strcat(str,tempCons);
}

void formatDataAndSendToWUnderground(){
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Sending Data ...");
	
	char temp[12];
	char data[300] = "AT+HTTPPARA=\"URL\",\"http://weatherstation.wunderground.com/weatherstation/updateweatherstation.php?ID=IMORATUW5&PASSWORD=lzv6y12w&dateutc=now";
	
	tempCon="&winddir=";
	strcat(data,tempCon);
	ftoa(windDir,temp);
	tempCon = temp;
	strcat(data,tempCon);
	
	tempCon="&windspeedmph=";
	strcat(data,tempCon);
	ftoa(windSpeed*0.621371,temp);
	tempCon = temp;
	strcat(data,tempCon);
	
	tempCon="&tempf=";
	strcat(data,tempCon);
	ftoa((tempC*1.8+32),temp);
	tempCon = temp;
	strcat(data,tempCon);
	
	tempCon="&rainin=";
	strcat(data,tempCon);
	ftoa(rainFall,temp);
	tempCon = temp;
	strcat(data,tempCon);
	
	tempCon="&baromin=";
	strcat(data,tempCon);
	ftoa(preP*0.000295299830714,temp);
	tempCon = temp;
	strcat(data,tempCon);
	
	tempCon="&action=updateraw\"\r\n";
	strcat(data,tempCon);
	sendDataGSM(data);
}
 
void sendDataGSM(char *data){
	cli();
	char *cmd1 = "AT+SAPBR=3,1,\"APN\",\"mobitel\"\r\n";
	char *cmd2 = "AT+SAPBR=1,1\r\n";
	char *cmd3 = "AT+HTTPINIT\r\n";
	char *cmd4 = "AT+HTTPPARA=\"CID\",1\r\n";
	char *cmd5 = "AT+HTTPACTION=0\r\n";
	USART_Init(BAUD);
	USART_Transmit_String(cmd1);
	_delay_ms(1000);
	USART_Transmit_String(cmd2);
	_delay_ms(1000);
	USART_Transmit_String(cmd3);
	_delay_ms(1000);
	USART_Transmit_String(cmd4);
	_delay_ms(1000);
	USART_Transmit_String(data);
	_delay_ms(3000);
	USART_Transmit_String(cmd5);
	_delay_ms(20000);
	sei();
}