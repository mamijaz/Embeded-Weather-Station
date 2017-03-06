#ifndef lib_basics
#include <my_libs/basics.h>
#endif

#include<util/delay.h>


#ifndef D0
#define D0 _PORTA0
#define D1 _PORTA1
#define D2 _PORTA2
#define D3 _PORTA3
#endif




//LCD 8 Bit Interfacing Functions
void Lcd8_Port(char a){
	if(a & 1)
	pinWrite(D0,1);
	else
	pinWrite(D0,0);
	
	if(a & 2)
	pinWrite(D1,1);
	else
	pinWrite(D1,0);
	
	if(a & 4)
	pinWrite(D2,1);
	else
	pinWrite(D2,0);
	
	if(a & 8)
	pinWrite(D3,1);
	else
	pinWrite(D3,0);
	
	if(a & 16)
	pinWrite(D4,1);
	else
	pinWrite(D4,0);

	if(a & 32)
	pinWrite(D5,1);
	else
	pinWrite(D5,0);
	
	if(a & 64)
	pinWrite(D6,1);
	else
	pinWrite(D6,0);
	
	if(a & 128)
	pinWrite(D7,1);
	else
	pinWrite(D7,0);
}

void Lcd8_Cmd(char a){
	pinWrite(RS,0);             // => RS = 0
	Lcd8_Port(a);             //Data transfer
	pinWrite(EN,1);             // => E = 1
	_delay_ms(1);
	pinWrite(EN,0);             // => E = 0
	_delay_ms(1);
}

void Lcd8_Clear(){
	Lcd8_Cmd(1);
}

void Lcd8_Set_Cursor(char a, char b){
	if(a == 1)
	Lcd8_Cmd(0x80 + b);
	else if(a == 2)
	Lcd8_Cmd(0xC0 + b);
}

void Lcd8_Init(){
	pinWrite(RS,0);
	pinWrite(EN,0);
	_delay_ms(20);
	///////////// Reset process from datasheet /////////
	Lcd8_Cmd(0x30);
	_delay_ms(5);
	Lcd8_Cmd(0x30);
	_delay_ms(1);
	Lcd8_Cmd(0x30);
	_delay_ms(10);
	/////////////////////////////////////////////////////
	Lcd8_Cmd(0x38);    //function set
	Lcd8_Cmd(0x0C);    //display on,cursor off,blink off
	Lcd8_Cmd(0x01);    //clear display
	Lcd8_Cmd(0x06);    //entry mode, set increment
}

void Lcd8_Write_Char(char a){
	pinWrite(RS,1);             // => RS = 1
	Lcd8_Port(a);             //Data transfer
	pinWrite(EN,1);             // => E = 1
	_delay_ms(1);
	pinWrite(EN,0);             // => E = 04
	_delay_ms(1);
}

void Lcd8_Write_String(char *a){
	int i;
	for(i=0;a[i]!='\0';i++)
	Lcd8_Write_Char(a[i]);
}

void Lcd8_Shift_Right(){
	Lcd8_Cmd(0x1C);
}

void Lcd8_Shift_Left(){
	Lcd8_Cmd(0x18);
}
//End LCD 8 Bit Interfacing Functions

//LCD 4 Bit Interfacing Functions

void Lcd4_Port(char a){
	if(a & 1)
	pinWrite(D4,1);
	else
	pinWrite(D4,0);
	
	if(a & 2)
	pinWrite(D5,1);
	else
	pinWrite(D5,0);
	
	if(a & 4)
	pinWrite(D6,1);
	else
	pinWrite(D6,0);
	
	if(a & 8)
	pinWrite(D7,1);
	else
	pinWrite(D7,0);
}

void Lcd4_Cmd(char a){
	pinWrite(RS,0);             // => RS = 0
	Lcd4_Port(a);
	pinWrite(EN,1);            // => E = 1
	_delay_ms(1);
	pinWrite(EN,0);             // => E = 0
	_delay_ms(1);
}

void Lcd4_Clear(){
	Lcd4_Cmd(0);
	Lcd4_Cmd(1);
}

void Lcd4_Set_Cursor(char a, char b){
	char temp,z,y;
	if(a == 1)
	{
		temp = 0x80 + b;
		z = temp>>4;
		y = (0x80+b) & 0x0F;
		Lcd4_Cmd(z);
		Lcd4_Cmd(y);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b;
		z = temp>>4;
		y = (0xC0+b) & 0x0F;
		Lcd4_Cmd(z);
		Lcd4_Cmd(y);
	}
}

void Lcd4_Init(){
	Lcd4_Port(0x00);
	_delay_ms(20);
	///////////// Reset process from datasheet /////////
	Lcd4_Cmd(0x03);
	_delay_ms(5);
	Lcd4_Cmd(0x03);
	_delay_ms(11);
	Lcd4_Cmd(0x03);
	/////////////////////////////////////////////////////
	Lcd4_Cmd(0x02);
	Lcd4_Cmd(0x02);
	Lcd4_Cmd(0x08);
	Lcd4_Cmd(0x00);
	Lcd4_Cmd(0x0C);
	Lcd4_Cmd(0x00);
	Lcd4_Cmd(0x06);
}

void Lcd4_Write_Char(char a){
	char temp,y;
	temp = a&0x0F;
	y = a&0xF0;
	pinWrite(RS,1);             // => RS = 1
	Lcd4_Port(y>>4);             //Data transfer
	pinWrite(EN,1);
	_delay_ms(1);
	pinWrite(EN,0);
	_delay_ms(1);
	Lcd4_Port(temp);
	pinWrite(EN,1);
	_delay_ms(1);
	pinWrite(EN,0);
	_delay_ms(1);
}

void Lcd4_Write_String(char *a){
	int i;
	for(i=0;a[i]!='\0';i++)
	Lcd4_Write_Char(a[i]);
}

void Lcd4_Shift_Right(){
	Lcd4_Cmd(0x01);
	Lcd4_Cmd(0x0C);
}

void Lcd4_Shift_Left(){
	Lcd4_Cmd(0x01);
	Lcd4_Cmd(0x08);
}
//End LCD 4 Bit Interfacing Functions
