#ifndef UART_H
#define UART_H

#include "stm32f10x.h"

//----------------÷∏¡ÓŒª÷√----------------
#define COM_HEAD0					0x00
#define COM_HEAD1					0x01
#define COM_DATA					0x02
#define COM_FRAME_SIZE				6			

typedef enum{	
				Waithead0 = 0,
				Waithead1,
				WaitData
			}receiveStatusEnum;

void USART1_Init(void);
u8  USART_ReceiveByte(USART_TypeDef* USARTx);
void USART_SendByte(USART_TypeDef* USARTx,u8 Data);
void USART_SendString(USART_TypeDef* USARTx,u8* pdata, u16 cnt);

void SendFrame(uint8_t* data);
void USART1_Check_Recive(u8 rxdata);


#endif