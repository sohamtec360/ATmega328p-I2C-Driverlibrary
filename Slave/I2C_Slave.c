/*
 * I2C_Slave.c
 *
 * Created: 4/18/2024 10:41:24 AM
 *  Author: Soham
 */ 

#include <avr/io.h>
#include "I2C_Slave.h"


void TWI_Wait_Completet()
{
	while((~TWCR) & (1<<TWINT)){;}
}

void TWI_INIT()		
{
	TWBR = TWIBR_VAL;
	TWSR = ((1<<TWPS1)*TWI_TWPS1) | ((1<<TWPS0)*TWI_TWPS0);
	TWAR = (Slave_Address<<1);
	TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	TWI_Wait_Completet();
}

///////////////////*****Slave Recive mode*****///////////////////

uint8_t TWI_Slave_Single_Read(uint8_t *Data)		//slave will read single data byte and send NAC bit
{													// ready to recognized by its own slave address
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	TWI_Wait_Completet();
	*Data=TWDR;
	return (((TWSR&0XFC) == SR_Data_Res_NACK_Trans) ? 1 : (TWSR&0XFC));
}

uint8_t TWI_Slave_Brust_Read(uint8_t *Data, uint8_t Len)
{
	while((TWSR&0XFC) != SR_Stop_Res){		//After receiving last byte slave will sand NACK bit
		if (Len == 0){
			TWCR = (1<<TWINT)|(1<<TWEN);		
		}else{
			TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
		}
		TWI_Wait_Completet();
		*Data=TWDR;
		Len --;
	}
	return	(((TWSR&0XFC) == SR_Data_Res_ACK_Trans) ? 1 : (TWSR&0XFC));
}

///////////////////*****Slave Transmit mode*****///////////////////

uint8_t TWI_Slave_Single_Tans(uint8_t *Data)
{	
	TWDR = *Data;
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	TWI_Wait_Completet();
	return (((TWSR&0XF8) == ST_Data_Trans_NACK_Res) ? 1:(TWSR&0XF8));
}