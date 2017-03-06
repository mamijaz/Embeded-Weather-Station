#ifndef lib_i2c
#include <my_libs/i2c.h>
#endif

#include <util/delay.h>
#include <math.h>

#define HMC5883_ADDRESS          0x3C
                           
#define Config_Reg_A             0x00
#define Config_Reg_B             0x01
#define Mode_Reg                 0x02

#define X_MSB_Reg                0x03
#define X_LSB_Reg                0x04
#define Z_MSB_Reg                0x05
#define Z_LSB_Reg                0x06
#define Y_MSB_Reg                0x07
#define Y_LSB_Reg                0x08

#define Status_Reg               0x09
#define ID_Reg_A                 0x0A             
#define ID_Reg_B                 0x0B
#define ID_Reg_C                 0x0C

int X_Data;
int Z_Data;
int Y_Data;


void HMC5883_Init(){
   I2C_Init(31250);
   I2C_Write(HMC5883_ADDRESS,Mode_Reg,0x00);
   //I2C_Write(HMC5883_ADDRESS,Config_Reg_A,0x70);
   //I2C_Write(HMC5883_ADDRESS,Config_Reg_B,0xA0);
   //_delay_ms(6);
}

void HMC5883_Calibration(){
   I2C_Start(HMC5883_ADDRESS);
   I2C_Write_bus(X_MSB_Reg);
   I2C_Stop();

   uint8_t data[6];
   I2C_Receive(HMC5883_ADDRESS,data,6);
   X_Data = (data[0] << 8) | data[1];
   Z_Data = (data[2] << 8) | data[3];
   Y_Data = (data[4] << 8) | data[5];
}

float HMC5883_Heading(){
   float heading = atan2(Y_Data, X_Data); 						// pi/180=0.0174532925
   float declinationAngle = -0.03595378; 						//Moratuwa -2.06°  -0.03595378
   heading += declinationAngle;
   
   if(heading < 0){
	   heading += 2*M_PI;
   }
  if(heading > 2*M_PI){
	  heading -= 2*M_PI;
  }
  
  float headingDegrees = heading * 180/M_PI;
  return headingDegrees;
}
