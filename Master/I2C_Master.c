/*
 * I2C.c
 *
 * Created: 4/10/2024 2:25:57 PM
 *  Author: Soham
 */ 

#include <avr/io.h>

#include "I2C_Master.h"

void TWI_INIT()		//used for only master configuration
{
	TWBR = TWIBR_VAL;
	TWSR = ((1<<TWPS1)*TWI_TWPS1) | ((1<<TWPS0)*TWI_TWPS0);
	TWCR = (1<<TWEN);
}

void TWI_Wait_Completet()
{
	while(~TWCR & (1<<TWINT)){;}
}

uint8_t TWI_Stat()
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	TWI_Wait_Completet();
	return (((TWSR&0XF8) == Start_CMD_Trans) ? 1 : (TWSR&0XF8));
}

void TWI_Stop()
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

uint8_t TWI_Repeated_Start()
{
	TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWSTA);		//Repeated start condition
	TWI_Wait_Completet();
	return (((TWSR&0XF8) == MT_Data_Trans_ACK_Res) ? 1 : (TWSR&0XF8));
}

////////////////////////////////////Master Transmit mode////////////////////////

uint8_t TWI_Mstr_Trans_Mode(uint8_t Slave_add)
{
	MT_SLAW(Slave_add);
	TWCR = (1<<TWINT)|(1<<TWEN);
	TWI_Wait_Completet();
	return (((TWSR&0XF8) == MT_SLAW_Trans_ACK_Res) ? 1 : (TWSR&0XF8));		
}

uint8_t TWI_Mstr_Single_Trans(uint8_t Data)
{
	TWDR = Data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	TWI_Wait_Completet();
	return (((TWSR&0XF8) == MT_Data_Trans_ACK_Res) ? 1 : (TWSR&0XF8));
}

uint8_t TWI_Mstr_Brust_Trans(uint8_t *Data, uint8_t Len)
{
	while(Len){				//if slave receive all data
		TWDR = *Data;			// Master will transmit STOP bit by it self after sending lasy byte
		Data++;
		if (Len == 1){
			TWCR = (1<<TWINT)|(1<<TWEN);	//hear we send stop condition after sending last byte reseting TWINT flag
			TWI_Wait_Completet();			//Because as per data-sheet when you send stop bit at same time while cearing
			TWCR = (1<<TWEN)|(1<<TWSTO);	// TWINT flag there is no any action on TWDR register so TWI hardware will not send data
		}else{
			TWCR = (1<<TWINT)|(1<<TWEN);
			TWI_Wait_Completet();
		}
		
		if ((TWSR&0XF8) == MT_Data_Trans_NACK_Res){		//Slave no longer want to Receive data or number of transmitted is higher then length assigned in argument of this function "uint8_t TWI_Slave_Brust_Read(uint8_t *Data, uint8_t Len)"
			Len = 0;									// check "uint8_t TWI_Slave_Brust_Read(uint8_t *Data, uint8_t Len)" function "Len" argument
			TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
			return (((TWSR&0XF8) == MT_Data_Trans_NACK_Res) ?  1 : (TWSR&0XF8));
		}
		Len--;
	}
	return (((TWSR&0XF8) == MT_Data_Trans_ACK_Res) ? 1 : (TWSR&0XF8));
}

////////////////////////////////////Master Receive mode////////////////////////

uint8_t TWI_Mstr_Res_Mode(uint8_t Slave_add)	//Slave address transmitted
{
	MR_SLAR(Slave_add);
	TWCR = (1<<TWINT)|(1<<TWEN);
	TWI_Wait_Completet();
	return (((TWSR&0XF8) == MR_SLAR_Trans_ACK_Res) ? 1 : (TWSR&0XF8));
}

uint8_t TWI_Mstr_Single_Res(uint8_t *Data)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	TWI_Wait_Completet();
	*Data = TWDR;
	return (((TWSR&0XF8) == MR_Data_Res_NACK_Trans) ? 1 : (TWSR&0XF8));
}

////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t Master_Single_Trans(uint8_t Data)
{
	TWI_Stat();
	TWI_Mstr_Trans_Mode(Slave_Address);
	TWI_Mstr_Single_Trans(Data);
	TWI_Stop();
	return 1;
}

uint8_t Master_Brust_Trans(uint8_t *Data, uint8_t len)
{
	TWI_Stat();
	TWI_Mstr_Trans_Mode(Slave_Address);
	TWI_Mstr_Brust_Trans(Data,len);
	return 1;
}
///////////////////////////////////////////////////////////////////////////////////////

//some external features

uint8_t Device_Scan()
{
	uint8_t Device_Scan_Status=0, Address=0;
	
	while ((Address <= 127) && (Device_Scan_Status != 0X01)){
		TWI_Stat();
		Device_Scan_Status = TWI_Mstr_Trans_Mode(Address);
		TWI_Stop();
		Address ++;
	}
	return (Address-1);
}


/////////////////////////////////////////////////////////