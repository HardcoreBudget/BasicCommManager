/*
 * main.c
 *
 *  Created on: Oct 17, 2023
 *      Author: Hazim Emad
 */
#include "../Library/STD_types.h"
#include "../Library/Bit_Math.h"
#include "../MCAL/DIO/Header/DIO_Interface.h"
#include "../HAL/LCD/Header/LCD_Interface.h"
#include "../Services/BCM/Header/BCM_Interface.h"
#include "../MCAL/TMR0/Header/TMR0_Interface.h"

void TXC_Complete();
void SPI_Complete();
void I2C_Complete();

u8 name[] = "Hazim";
volatile u8 global_index = 0;
volatile u8 sendFlag = 1;
u8 SLAVE_ADDRESS = 0xA0;
u8 REG_ADDRESS = 0;
u8* slave_ptr = &SLAVE_ADDRESS;
u8* reg_ptr = &REG_ADDRESS;

int main(){
	DIO_voidInit();
	DIO_voidSetPinValue(DIO_PortB,DIO_PIN4,High);
	DIO_voidSetPinValue(DIO_PortC,DIO_PIN0,High);
	DIO_voidSetPinValue(DIO_PortC,DIO_PIN1,High);
	LCD_voidInit();
	TMR0_voidInit();
	BCM_voidInit();
	while(1){
		global_index = 0;
		REG_ADDRESS = 0;
		while(name[global_index] != '\0'){
			BCM_voidAddToQueue(name[global_index],USART, TXC_Complete, NULL, NULL, SPECIFIC);
			BCM_voidAddToQueue(name[global_index],SPI, SPI_Complete, NULL, NULL, SPECIFIC);
			BCM_voidAddToQueue(name[global_index],I2C, I2C_Complete, slave_ptr, reg_ptr, SPECIFIC);
			REG_ADDRESS++;
			global_index++;
			TMR0_voidSetBusyWait(10);
		}
	}
	return 0;
}


void TXC_Complete(){
	//	LCD_voidGoToPosition(0,0);
	//	LCD_voidWriteString((u8*)"USART Data sent!");
}

void SPI_Complete(){
	//	LCD_voidGoToPosition(1,0);
	//	LCD_voidWriteString((u8*)"SPI Data sent!");
}
void I2C_Complete(){
	//	LCD_voidGoToPosition(1,0);
	//	LCD_voidWriteString((u8*)"I2C Data sent!");
}
