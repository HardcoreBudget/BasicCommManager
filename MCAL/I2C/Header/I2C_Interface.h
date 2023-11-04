/*
 * I2C_Interface.h
 *
 *  Created on: Oct 31, 2023
 *      Author: Hazim Emad
 */

#ifndef MCAL_I2C_HEADER_I2C_INTERFACE_H_
#define MCAL_I2C_HEADER_I2C_INTERFACE_H_

#include "I2C_Register.h"
#include "I2C_Private.h"
#include "I2C_Config.h"

typedef enum
{
	NoError,
	ErrorStartCondition,
	ErrorRestartCondition,
	ErrorSlaveAddressWriteWithAck,
	ErrorSlaveAddressWriteWithoutAck,
	ErrorSlaveAddressReadWithAck,
	ErrorSlaveAddressReadWithoutAck,
	ErrorWriteDataWithAck,
	ErrorWriteDataWithoutAck,
	ErrorReadDataWithAck,
	ErrorReadDataWithoutAck,
}I2C_Error_State;

void I2C_Master_voidInit(void);                                        //init for master
void I2C_Slave_voidInit(u8 I2C_Address);                               //init fo slave
I2C_Error_State I2C_Master_enuSendStartCond(void);                          //start if bus is idle
I2C_Error_State I2C_Master_enuSendRepeatedStartCond(void);                  //repeated start if bus is busy

I2C_Error_State I2C_Master_enuSendSlaveAddressWithRead(u8 I2C_Address);     //Send slave address With R/W bit Read
I2C_Error_State I2C_Master_enuSendSlaveAddressWithWrite(u8 I2C_Address);    //Send slave address With R/W bit Write
I2C_Error_State I2C_Master_enuSendu8Data(u8 I2C_Data);                      //master Sets Data given in argument
I2C_Error_State I2C_Master_enuReadu8Data(u8* I2C_Data);                     //master Gets Data and saves it in given argument
void I2C_Master_enuSendStopCond(void);                                      //send stop bit
I2C_Error_State I2C_Slave_enuReadu8Data(u8* I2C_Data);                      //Slave Reads Data saves it in given argument
I2C_Error_State I2C_Master_enuFlush();


void I2C_Master_INT_voidSendStartCond(void);
void I2C_Master_INT_voidSendSlaveAddressWithWrite(u8 I2C_Address);
void I2C_Master_INT_voidSendu8Data(u8 I2C_Data);
void I2C_Master_INT_voidFlush();
void I2C_Master_INT_voidSendStopCond(void);
void I2C_voidSetCallBack(void (*Copy_pvCallBackFunc)(void));


#endif /* MCAL_I2C_HEADER_I2C_INTERFACE_H_ */
