#include "Event.h"
#include "./Device/UserInterface.h"
#include "hx117.h"
#include "uart.h"
uint32_t weighInit = 0;

extern uint16_t g_sysTime ;
float weigh1 = 0;
uint32_t weigh2 = 0;
void Program_Setup(void)
{
	
}


void Main_Loop(void)
{
	if(g_sysTime > 50)
	{
		g_sysTime = 0;
		//LED_Toggle(GPIOC, GPIO_Pin_13);
		weigh2  =Read_HX711();
//		weigh1=weigh1/83886.08;
//        weigh2=weigh1*2000;
		SendFrame((uint8_t*)&weigh2);
		//USART_ReceiveByte(USART1);
	}
}


void Delay_US(unsigned long t)
{
	int i = 0,j=0;
	for(i = 0; i<t;i++)
	{
		for(j=0;j<10;j++);
	}
}

