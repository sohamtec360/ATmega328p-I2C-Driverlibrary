/*
 * I2C_Slave.h
 *
 * Created: 4/18/2024 10:41:54 AM
 *  Author: Soham
 */ 


#ifndef I2C_SLAVE_H_
#define I2C_SLAVE_H_

#define F_CPU		16000000UL
#define SCLK_Freq	100000UL


#if (((F_CPU/SCLK_Freq)-16)/2*1)>0 && (((F_CPU/SCLK_Freq)-16)/2*1)<256
#define Prescaler_Val	1
#define TWI_TWPS0		0
#define TWI_TWPS1		0
#elif (((F_CPU/SCLK_Freq)-16)/2*4)>0 && (((F_CPU/SCLK_Freq)-16)/2*4)<256
#define Prescaler_Val	4
#define TWI_TWPS0		1
#define TWI_TWPS1		0
#elif (((F_CPU/SCLK_Freq)-16)/2*16)>0 && (((F_CPU/SCLK_Freq)-16)/2*16)<256
#define Prescaler_Val	16
#define TWI_TWPS0		0
#define TWI_TWPS1		1
#elif (((F_CPU/SCLK_Freq)-16)/2*64)>0 && (((F_CPU/SCLK_Freq)-16)/2*64)<256
#define Prescaler_Val	64
#define TWI_TWPS0		1
#define TWI_TWPS1		1
#else
"error in frequency"
#endif

#define TWIBR_VAL	(((F_CPU/SCLK_Freq)-16)/(2*Prescaler_Val))

#define Slave_Address				0X06		//Fixed Slave Address

void TWI_INIT(void);
void TWI_Wait_Completet(void);

/// Slave Receive mode
#define SR_SLAW_Res_ACK_Trans		0X60		//Slave Receive Own slave address is received and ACK transmitted
#define SR_Data_Res_ACK_Trans		0X80		//Slave Receive Data has been received and ACK transmitted
#define SR_Data_Res_NACK_Trans		0X88		//Slave Receive Data has been received and NACK Transmitted
#define SR_Stop_Res					0XA0		//Stop bit received

uint8_t TWI_Slave_INIT(uint8_t);
uint8_t TWI_Slave_Single_Read(uint8_t*);
uint8_t TWI_Slave_Brust_Read(uint8_t*, uint8_t);

/// Slave Transmitted mode
#define ST_SLAR_Res_ACK_Trans		0XA8		//Slave Transmit Own slave address is received and ACK transmitted
#define	ST_Data_Trans_ACK_Res		0XB8		//Slave Transmit Data Transmitted and ACK received
#define	ST_Data_Trans_NACK_Res		0XC0		//Slave Transmit Data Transmitted and NACK received
#define ST_Last_Data_Trans_ACK_Rea	0XC8		//Slave Transmit Last Data transmitted and ACK received

uint8_t TWI_Slave_Single_Tans(uint8_t*);
uint8_t TWI_Slave_Brust_Tans(uint8_t*);

#endif /* I2C_SLAVE_H_ */