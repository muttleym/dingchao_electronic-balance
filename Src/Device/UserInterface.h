#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "stm32f10x.h"
void UI_LED_Init(void);
void LED_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif 
