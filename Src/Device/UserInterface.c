#include "stm32f10x.h"                  // Device header

void UI_LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    //C13 Board LED
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
}

void LED_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIO_WriteBit(GPIOx, GPIO_Pin, !GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin));
}
