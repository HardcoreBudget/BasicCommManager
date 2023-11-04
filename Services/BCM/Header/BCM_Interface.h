/*
 * BCM_Interface.h
 *
 *  Created on: Nov 3, 2023
 *      Author: Hazim Emad
 */

#ifndef SERVICES_BCM_HEADER_BCM_INTERFACE_H_
#define SERVICES_BCM_HEADER_BCM_INTERFACE_H_

#include "BCM_Private.h"
#include "BCM_Config.h"

typedef enum{
	USART,
	SPI,
	I2C,
}BCM_COMM_Protocol;
typedef enum{
	DONE,
	BUSY,
	READY,
}COMM_STATE;
typedef enum{
	GENERAL,
	SPECIFIC,
}QUEUE_TYPE;

void BCM_voidInit();
void BCM_voidAddToQueue(u8 data,BCM_COMM_Protocol protocol ,void(*Fun_PTR)(void), u8* SlaveAddress, u8* RegisterAddress, QUEUE_TYPE queueType);
void BCM_voidSendQueuedData();

#endif /* SERVICES_BCM_HEADER_BCM_INTERFACE_H_ */
