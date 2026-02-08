/*
 * I2C.h
 *
 * Created: 4/10/2024 2:26:12 PM
 *  Author: Soham
 */ 


#ifndef I2C_H_
#define I2C_H_

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
#define Start_CMD_Trans				0X08		//Start condition has been transmitted

/// Master Transmit mode
#define MT_SLAW(SLA)				(TWDR=(SLA<<1))

#define Start_CMD_Trans				0X08		//Master Transmit mode start command transmitted
#define MT_SLAW_Trans_ACK_Res		0X18		//Master Transmit mode slave add sent and ACK received
#define MT_SLAW_Trans_NACK_Res		0X20		//Master Transmit mode slave add sent and NACK received
#define MT_Data_Trans_ACK_Res		0X28		//Master Transmit mode Data sent and ACK received
#define MT_Data_Trans_NACK_Res		0X30		//Master Transmit mode Data sent and NACK received

uint8_t TWI_Mstr_Trans_Mode(uint8_t);		//Receiver slave address and Master mode as a arguments
uint8_t TWI_Mstr_Single_Trans(uint8_t);
uint8_t TWI_Mstr_Brust_Trans(uint8_t*, uint8_t);
uint8_t TWI_Repeated_Start(void);

/// Master Receive mode
#define MR_SLAR(SLA)				(TWDR=((SLA<<1)|0X01))

#define MR_SLAR_Trans_ACK_Res		0X40		//Master Receive mode slave add sent and ACK received
#define MR_SLAR_Trans_NACK_Res		0X48		//Master Receive mode slave add sent and NACK received
#define MR_Data_Res_ACK_Trans		0X50		//Master Transmit mode Data sent and ACK Transmitted
#define MR_Data_Res_NACK_Trans		0X58		//Master Transmit mode Data sent and NACK Transmitted

uint8_t TWI_Mstr_Res_Mode(uint8_t);
uint8_t TWI_Mstr_Single_Res(uint8_t*);

/// General Master function
void TWI_INIT(void);
uint8_t TWI_Stat(void);						//Returns current status of TWI Peripheral
void TWI_Wait_Completet(void);
void TWI_Stop(void);

///*************///
uint8_t Master_Single_Trans(uint8_t);
uint8_t Master_Brust_Trans(uint8_t *, uint8_t);

///***********///
//Some Extra features
uint8_t Device_Scan(void);

/// for testing purpose///
void Single_Trans(void);
void Single_Res(void);
void GPIO_INT_Enable(void);

#endif /* I2C_H_ */

