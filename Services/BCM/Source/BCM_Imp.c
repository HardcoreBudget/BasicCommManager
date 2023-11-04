/*
 * BCM_Imp.c
 *
 *  Created on: Nov 3, 2023
 *      Author: Hazim Emad
 */
#include "../../../Library/STD_types.h"
#include "../Header/BCM_Interface.h"
#include "../../../MCAL/USART/Header/USART_Interface.h"
#include "../../../MCAL/SPI/Header/SPI_Interface.h"
#include "../../../MCAL/I2C/Header/I2C_Interface.h"
#include "../../../MCAL/GIE/Header/GIE_Interface.h"

typedef struct{
	BCM_COMM_Protocol protocol;
	u8 data;
	void(*Fun_PTR)(void);
	u8 SlaveAddress;
	u8 RegisterAddress;
	u8 RegAvailable;
	COMM_STATE state;
	QUEUE_TYPE type;
}BCM;

static volatile BCM bcm_General_queue[MAX_GENERAL_QUEUE_SIZE];
static volatile BCM bcm_USART_queue[MAX_USART_QUEUE_SIZE];
static volatile BCM bcm_SPI_queue[MAX_SPI_QUEUE_SIZE];
static volatile BCM bcm_I2C_queue[MAX_I2C_QUEUE_SIZE];
static volatile u8 generalQueueIndex = 0;
static volatile u8 generalQueueState = READY;
static volatile u8 USARTQueueIndex = 0;
static volatile u8 USARTQueueState = READY;
static volatile u8 SPIQueueIndex = 0;
static volatile u8 SPIQueueState = READY;
static volatile u8 I2CQueueIndex = 0;
static volatile u8 I2CQueueState = READY;
static volatile u8 currentGeneralIndex = 0;
static volatile u8 currentUSARTIndex = 0;
static volatile u8 currentSPIIndex = 0;
static volatile u8 currentI2CIndex = 0;
static volatile u8 interruptI2Cindex = 1;


static void BCM_voidResetState(volatile u8* index, volatile u8* state, volatile BCM bcm_queue[]){
	bcm_queue[*index].state = DONE;
	if(bcm_queue[*index].Fun_PTR != NULL){
		bcm_queue[*index].Fun_PTR();
	}
	(*index)++;
	*state = READY;
}

static void USARTTransmissionDone(){
	if(bcm_USART_queue[currentUSARTIndex].state != BUSY){
		BCM_voidResetState(&currentGeneralIndex, &generalQueueState, bcm_General_queue);
		currentGeneralIndex = currentGeneralIndex > (MAX_GENERAL_QUEUE_SIZE - 1) ? 0 : currentGeneralIndex;
		USARTQueueState = READY;
	}
	else{
		BCM_voidResetState(&currentUSARTIndex, &USARTQueueState, bcm_USART_queue);
		currentUSARTIndex = currentUSARTIndex > (MAX_USART_QUEUE_SIZE - 1) ? 0 : currentUSARTIndex;
	}
	BCM_voidSendQueuedData();
}

static void SPITransmissionDone(){
	if(bcm_SPI_queue[currentSPIIndex].state != BUSY){
		BCM_voidResetState(&currentGeneralIndex, &generalQueueState, bcm_General_queue);
		currentGeneralIndex = currentGeneralIndex > (MAX_GENERAL_QUEUE_SIZE - 1) ? 0 : currentGeneralIndex;
		SPIQueueState = READY;
	}
	else{
		BCM_voidResetState(&currentSPIIndex, &SPIQueueState, bcm_SPI_queue);
		currentSPIIndex = currentSPIIndex > (MAX_SPI_QUEUE_SIZE - 1) ? 0 : currentSPIIndex;
	}
	BCM_voidSendQueuedData();
}

static void I2CTransmissionDone(){
	if(bcm_I2C_queue[currentI2CIndex].state != BUSY){
		BCM_voidResetState(&currentGeneralIndex, &generalQueueState, bcm_General_queue);
		currentGeneralIndex = currentGeneralIndex > (MAX_GENERAL_QUEUE_SIZE - 1) ? 0 : currentGeneralIndex;
		I2CQueueState = READY;
	}
	else{
		BCM_voidResetState(&currentI2CIndex, &I2CQueueState, bcm_I2C_queue);
		currentI2CIndex = currentI2CIndex > (MAX_I2C_QUEUE_SIZE - 1) ? 0 : currentI2CIndex;
	}
	BCM_voidSendQueuedData();
}

static void I2C_Callback(){
	if(interruptI2Cindex == 1){
		interruptI2Cindex++;
		if(bcm_I2C_queue[currentI2CIndex].state != BUSY){
			I2C_Master_INT_voidSendSlaveAddressWithWrite(bcm_General_queue[currentGeneralIndex].SlaveAddress);
		}
		else{
			I2C_Master_INT_voidSendSlaveAddressWithWrite(bcm_I2C_queue[currentI2CIndex].SlaveAddress);
		}
	}
	else if(interruptI2Cindex == 2){
		if(bcm_I2C_queue[currentI2CIndex].state != BUSY){
			if(bcm_General_queue[currentGeneralIndex].RegAvailable == 1){
				I2C_Master_INT_voidSendu8Data(bcm_General_queue[currentGeneralIndex].RegisterAddress);
				bcm_General_queue[currentGeneralIndex].RegAvailable = 0;
			}
			else{
				interruptI2Cindex++;
				I2C_Master_INT_voidSendu8Data(bcm_General_queue[currentGeneralIndex].data);
			}
		}
		else{
			if(bcm_I2C_queue[currentI2CIndex].RegAvailable == 1){
				I2C_Master_INT_voidSendu8Data(bcm_I2C_queue[currentI2CIndex].RegisterAddress);
				bcm_I2C_queue[currentI2CIndex].RegAvailable = 0;
			}
			else{
				interruptI2Cindex++;
				I2C_Master_INT_voidSendu8Data(bcm_I2C_queue[currentI2CIndex].data);
			}
		}
	}
	else if(interruptI2Cindex == 3){
		interruptI2Cindex = 1;
		I2C_Master_INT_voidSendStopCond();
		I2CTransmissionDone();
	}
}

static BCM BCM_structLoadData(u8 data,BCM_COMM_Protocol protocol ,void(*Fun_PTR)(void), u8* SlaveAddress, u8* RegisterAddress, QUEUE_TYPE queueType){
	BCM bcm;
	bcm.data = data;
	bcm.protocol = protocol;
	bcm.Fun_PTR = Fun_PTR;
	bcm.state = DONE;
	bcm.type = queueType;
	if(SlaveAddress != NULL){
		bcm.SlaveAddress = *SlaveAddress;
		if(RegisterAddress != NULL){
			bcm.RegAvailable = 1;
			bcm.RegisterAddress = *RegisterAddress;
		}
		else{
			bcm.RegAvailable = 0;
		}
	}
	else{
		bcm.SlaveAddress = 0;
		bcm.RegAvailable = 0;
	}

	return bcm;
}

static void BCM_voidAddProtocol(BCM bcm, volatile u8* index, volatile BCM bcm_queue[]){
	if(bcm_queue[*index].state == DONE){
		bcm_queue[*index] = bcm;
		bcm_queue[*index].state = READY;
		(*index)++;
	}
	BCM_voidSendQueuedData();
}



static void BCM_voidCheckAndStart(volatile u8* state, u8 index, volatile BCM bcm_queue[]){
	if(*state == READY){
		if(bcm_queue[index].state == READY){
			switch(bcm_queue[index].protocol){
			case USART:
				if(USARTQueueState != BUSY){
					USARTQueueState = BUSY;
					bcm_queue[index].state = BUSY;
					*state = BUSY;
					USART_voidSendInterruptByte(bcm_queue[index].data);
				}
				break;
			case SPI:
				if(SPIQueueState != BUSY){
					SPIQueueState = BUSY;
					bcm_queue[index].state = BUSY;
					*state = BUSY;
					SPI_voidSendDataISR(bcm_queue[index].data);
				}
				break;
			case I2C:
				if(I2CQueueState != BUSY){
					I2CQueueState = BUSY;
					bcm_queue[index].state = BUSY;
					*state = BUSY;
					I2C_Master_INT_voidSendStartCond();
				}
				break;
			}
		}
	}
}

void BCM_voidInit(){
	GIE_voidEnable();
#if USART_PROTOCOL == BCM_ENABLE
	USART_voidEnableTXCInterrupt();
	USART_voidTXCSetCallBack(USARTTransmissionDone);
	USART_voidInit();
#endif

#if SPI_PROTOCOL == BCM_ENABLE
	SPI_voidSetCallBack(SPITransmissionDone);
	SPI_voidMasterInit();
#endif

#if I2C_PROTOCOL == BCM_ENABLE
	I2C_voidSetCallBack(I2C_Callback);
	I2C_Master_voidInit();
#endif
}

void BCM_voidAddToQueue(u8 data,BCM_COMM_Protocol protocol ,void(*Fun_PTR)(void), u8* SlaveAddress, u8* RegisterAddress, QUEUE_TYPE queueType){
	BCM bcm = BCM_structLoadData(data, protocol, Fun_PTR, SlaveAddress, RegisterAddress, queueType);
	if(queueType == GENERAL){
		generalQueueIndex = generalQueueIndex > (MAX_GENERAL_QUEUE_SIZE - 1) ? 0 : generalQueueIndex;
		BCM_voidAddProtocol(bcm, &generalQueueIndex, bcm_General_queue);
	}
	else{
		switch(protocol){
		case USART:
			USARTQueueIndex = USARTQueueIndex > (MAX_USART_QUEUE_SIZE - 1) ? 0 : USARTQueueIndex;
			BCM_voidAddProtocol(bcm, &USARTQueueIndex, bcm_USART_queue);
			break;
		case SPI:
			SPIQueueIndex = SPIQueueIndex > (MAX_SPI_QUEUE_SIZE - 1) ? 0 : SPIQueueIndex;
			BCM_voidAddProtocol(bcm, &SPIQueueIndex, bcm_SPI_queue);
			break;
		case I2C:
			I2CQueueIndex = I2CQueueIndex > (MAX_I2C_QUEUE_SIZE - 1) ? 0 : I2CQueueIndex;
			BCM_voidAddProtocol(bcm, &I2CQueueIndex, bcm_I2C_queue);
			break;
		}
	}
	BCM_voidSendQueuedData();
}

void BCM_voidSendQueuedData(){
	BCM_voidCheckAndStart(&generalQueueState, currentGeneralIndex, bcm_General_queue);
	BCM_voidCheckAndStart(&USARTQueueState, currentUSARTIndex, bcm_USART_queue);
	BCM_voidCheckAndStart(&SPIQueueState, currentSPIIndex, bcm_SPI_queue);
	BCM_voidCheckAndStart(&I2CQueueState, currentI2CIndex, bcm_I2C_queue);
}

