#include "Event.h"
#include "stm32f10x.h"
#include "./Device/UserInterface.h"
#include "hx117.h"
#include "uart.h"

void RCC_Configuration(void);

uint32_t weighInit = 0;



int main(void)
{
    RCC_Configuration();
    Hx117Init();
    USART1_Init();
    UI_LED_Init();

    SysTick_Config(72000);
	weighInit = Read_HX711();

	for(;;)
	{
		Main_Loop();
	}
}

void RCC_Configuration(void)
{
	SystemInit();
}

