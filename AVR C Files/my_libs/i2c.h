#define lib_i2c 1

#ifndef  F_CPU
#define F_CPU 8000000UL
#endif

#ifndef  Prescaler
#define Prescaler 1
#endif

#include <util/twi.h>

#define I2C_READ 0x01
#define I2C_WRITE 0x00


void I2C_Init(unsigned long fscl){
	uint8_t TWBR_val=((((F_CPU / fscl) / Prescaler) - 16 ) / 2);
	TWBR = TWBR_val;
}

uint8_t I2C_Start(uint8_t address){
	TWCR = 0;	
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	
	while( !(TWCR & (1<<TWINT)) );	
	
	if((TWSR & 0xF8) != TW_START){ 
		return 1; 
	}
	
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	while( !(TWCR & (1<<TWINT)) );
	
	uint8_t twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) {
		return 1;
	}
	
	return 0;
}

void I2C_Stop(void){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

uint8_t I2C_Write_bus(uint8_t data){
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	while( !(TWCR & (1<<TWINT)) );
	
	if( (TWSR & 0xF8) != TW_MT_DATA_ACK ){
		return 1; 
	}
	
	return 0;
}

void I2C_Write(uint8_t devaddr, uint8_t regaddr, uint8_t value){
	I2C_Start(devaddr);
	I2C_Write_bus(regaddr);
	I2C_Write_bus(value);
	I2C_Stop();
}

uint8_t I2C_Read_ACK(void){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); 
	while( !(TWCR & (1<<TWINT)) );
	return TWDR;
}

uint8_t I2C_Read_NACK(void){
	TWCR = (1<<TWINT) | (1<<TWEN);
	while( !(TWCR & (1<<TWINT)) );
	return TWDR;
}

uint8_t I2C_Read(uint8_t devaddr, uint8_t regaddr){
	uint8_t data;
	if (I2C_Start(devaddr)){
		 return 1;
	}

	I2C_Write_bus(regaddr);

	if (I2C_Start(devaddr | 0x01)) return 1;

	data = I2C_Read_NACK();

	I2C_Stop();
	
	return data;
}

uint8_t I2C_Transmit(uint8_t address, uint8_t* data, uint16_t length){
	if (I2C_Start(address | I2C_WRITE)){ 
		return 1;
	}

	for (uint16_t i = 0; i < length; i++){
		if (I2C_Write_bus(data[i])) return 1;
	}
	
	I2C_Stop();
	
	return 0;
}

uint8_t I2C_Receive(uint8_t address, uint8_t* data, uint16_t length){
	if (I2C_Start(address | I2C_READ)) return 1;
	
	for (uint16_t i = 0; i < (length-1); i++){
		data[i] = I2C_Read_ACK();
	}
	data[(length-1)] = I2C_Read_NACK();
	
	I2C_Stop();
	
	return 0;
}

uint8_t I2C_WriteReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length){
	if (I2C_Start(devaddr | 0x00)) return 1;

	I2C_Write_bus(regaddr);

	for (uint16_t i = 0; i < length; i++){
		if (I2C_Write_bus(data[i])) {
			return 1;
		}
	}

	I2C_Stop();
	
	return 0;
}

