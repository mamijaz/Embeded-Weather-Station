#ifndef lib_i2c
#include <my_libs/i2c.h>
#endif

#include <util/delay.h>

#define BMP180_ADDRESS          0xEE
#define CONTROL_REG_ADDRESS     0xF4
#define OUT_MSB                 0xF6
#define OUT_LSB                 0xF7
#define OUT_XLSB                0xF8
#define OSS                     0

#define TEMPERATURE             0x2E
#define PRESSURE                0x34

int BMP180_AC1;
int BMP180_AC2;
int BMP180_AC3;
unsigned int BMP180_AC4;
unsigned int BMP180_AC5;
unsigned int BMP180_AC6;
int BMP180_B1;
int BMP180_B2;
int BMP180_MB;
int BMP180_MC;
int BMP180_MD;
long B5;

void BMP180Init(){
   I2C_Init(31250);
}

void BMP180Write(uint8_t address, uint8_t value){
  I2C_Write(BMP180_ADDRESS,address,value);
}

uint8_t BMP180Read(uint8_t address){
   return I2C_Read(BMP180_ADDRESS,address);
}

int BMP180ReadInt(uint8_t address){
   int temp;
   uint8_t reg;
   reg=I2C_Read(BMP180_ADDRESS,address);
   temp=(reg<<8)|I2C_Read(BMP180_ADDRESS,address|0x01);
   return ( temp );
} 

void BMP180Calibration(){
   BMP180_AC1 = BMP180ReadInt(0xAA);
   BMP180_AC2 = BMP180ReadInt(0xAC);
   BMP180_AC3 = BMP180ReadInt(0xAE);
   BMP180_AC4 = BMP180ReadInt(0xB0);
   BMP180_AC5 = BMP180ReadInt(0xB2);
   BMP180_AC6 = BMP180ReadInt(0xB4);
   BMP180_B1  = BMP180ReadInt(0xB6);
   BMP180_B2  = BMP180ReadInt(0xB8);
   BMP180_MB  = BMP180ReadInt(0xBA);
   BMP180_MC  = BMP180ReadInt(0xBC);
   BMP180_MD  = BMP180ReadInt(0xBE);
}

float BMP180ReadTemperature(){
   int BMP180_UT;
   long X1,X2,T;
   BMP180Write(CONTROL_REG_ADDRESS,TEMPERATURE);
   _delay_ms(5);
   BMP180_UT = BMP180ReadInt(OUT_MSB);
   
   X1 = ((long)(BMP180_UT - BMP180_AC6) * BMP180_AC5)/ 32768;
   X2 = ((long)BMP180_MC * 2048) / (X1 + BMP180_MD);
   B5 = X1 + X2;
   T  = (B5 + 8) / 16;
   return ( T * 0.1 );
}

long BMP180ReadPressure(){
   long B6,X1,X2,X3,B3,P;
   unsigned long BMP180_UP,B4,B7;
   long MSB,LSB,XLSB;
   BMP180Write(CONTROL_REG_ADDRESS,PRESSURE + (OSS<<6));
   _delay_ms(5);
   MSB = BMP180Read(OUT_MSB);
   LSB = BMP180Read(OUT_LSB);
   XLSB = BMP180Read(OUT_XLSB); 
   BMP180_UP = ((MSB << 16) | (LSB<<8) | XLSB) >> (8-OSS);
   
   B6 = B5 - 4000;
   X1 = (BMP180_B2 * ((B6 * B6) / 4096)) / 2048;
   X2 = BMP180_AC2 * B6 / 2048;
   X3 = X1 + X2;
   B3 = (((BMP180_AC1 * 4 + X3) << OSS) +2) / 4;
   X1 = (BMP180_AC3 * B6) / 8192;
   X2 = (BMP180_B1 * ((B6 * B6) / 4096)) / 65536;
   X3 = ((X1 + X2) + 2) / 4;
   B4 = (BMP180_AC4 * (unsigned long)(X3 + 32768)) / 32768;
   B7 = ((unsigned long)BMP180_UP - B3) * (50000 >> OSS);
   if (B7 < 0x80000000){
      P = (B7 * 2) / B4;
   }
   else{
      P = (B7 / B4) * 2;
   }
   X1 = (P / 256) * (P / 256);
   X1 = (X1 * 3038) / 65536;
   X2 = (-7357 * P) / 65536;
   P = P + (X1 + X2 + 3791) / 16;
   
   return ( P );
}