#include "stm32f10x.h"                  // Device header
#include "uart.h"
#include <string.h>
#include <stdbool.h>
#include "./Device/UserInterface.h"

uint32_t receiveWeigh = 0;

uint8_t  m_Usart1RecvBuff[COM_FRAME_SIZE] = {0};
/*
 * 函数名：USART1_Init
 * 描述  ：USART1 GPIO 配置,工作模式配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void USART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ
	NVIC_Init(&NVIC_InitStructure);	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
}

void USART_SendByte(USART_TypeDef* USARTx,u8 Data)
{
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET); 
	USART_SendData(USARTx, (u16)Data);
	
}


void USART1_Check_Recive(u8 rxdata)
{
	static receiveStatusEnum rs = Waithead0;
	static u16 pos = 0;
	switch(rs)
		{
			case Waithead0:
			{
				pos = 0;
				if( 0x5a == rxdata ) 
				{
					rs = Waithead1; 
					m_Usart1RecvBuff[pos++] = rxdata;
				}
				else rs = Waithead0;
				break;
			}
			case Waithead1:
			{
				if( 0xa5 == rxdata )
				{
					rs = WaitData; 
					m_Usart1RecvBuff[pos++] = rxdata;
				}
				else rs = Waithead0;
				break;
			}
			case WaitData:
			{
				if(pos<(COM_FRAME_SIZE))
				{
					m_Usart1RecvBuff[pos++] = rxdata;
				}
				if( pos == COM_FRAME_SIZE )
				{
					//checksum_u16 = Checksum_u16(&m_Usart1RecvBuff[2],pos-4);
					//if( checksum_u16 == Byte_Contact(m_Usart1RecvBuff[pos-1],m_Usart1RecvBuff[pos-2]) )
					{
						//解析数据包
						//Usart1_ParseFrame(m_Usart1RecvBuff, pos-2);
						memcpy((uint8_t *)&receiveWeigh, m_Usart1RecvBuff+2, COM_FRAME_SIZE-2);
						LED_Toggle(GPIOC, GPIO_Pin_13);
					}
					rs = Waithead0;
				}
				break;
			}
			default:
				break;
		}
		
		
}


u8  USART_ReceiveByte(USART_TypeDef* USARTx)
{
	u8 Data;

	while ( !(USARTx->SR & USART_FLAG_RXNE) );
	Data = 	(u8)(USART_ReceiveData(USARTx) & 0xFF);
	return  Data;
}

void USART_SendString(USART_TypeDef* USARTx,u8* pdata, u16 cnt)
{
	u16 i = 0;
	for(i=0;i<cnt;i++)
		USART_SendByte(USARTx,*(pdata+i));
}

void SendFrame(uint8_t* data)
{
	uint16_t checksum = 0;
	uint8_t i = 0;
	static uint8_t send_frame[COM_FRAME_SIZE] = {0};
	
	send_frame[COM_HEAD0] = 0x5a;
	send_frame[COM_HEAD1] = 0xa5;

	memcpy(&send_frame[COM_DATA], data, COM_FRAME_SIZE-2);
	USART_SendString(USART1,send_frame,COM_FRAME_SIZE);
}



